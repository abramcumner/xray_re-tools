#include <stdio.h>
#include "ttuner.h"

#define ADJUST_AFFINITY	0
#define ADJUST_PRIORITY	0

static HANDLE g_process_h = NULL;
static DWORD g_process_id = (DWORD)-1;
static DWORD g_thread0_id = (DWORD)-1;

static DWORD g_cpu_avail = 0;

static struct list_node g_queue;

static void fatal(const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fputs("", stderr);

	exit(1);
}

static void reschedule(void)
{
	DWORD cpu_used = 0, priority = THREAD_PRIORITY_HIGHEST, affinity;
	struct thread_info* thread;
	list_for_each(thread, &g_queue, struct thread_info, link) {
		if (thread->worker == FALSE)
			continue;
		for (affinity = 1; affinity != 0; affinity <<= 1) {
			if ((affinity & cpu_used) == 0 && (affinity & g_cpu_avail) != 0) {
				cpu_used |= affinity;
				if (ADJUST_AFFINITY && SetThreadAffinityMask(thread->h, affinity) == 0)
					fatal("can't set affinity %#lx for thread %lu", affinity, thread->id);
				else
					affinity = g_cpu_avail;
				if (ADJUST_PRIORITY && SetThreadPriority(thread->h, priority) == 0)
					fatal("can't set priority %#lx for thread %lu", priority, thread->id);
				if (thread->affinity == 0) {
					if (ResumeThread(thread->h) == (DWORD)-1)
						fatal("can't resume thread %lu", thread->id);
				}
				thread->affinity = affinity;
				break;
			}
		}
		if (affinity == 0 && thread->affinity) {
			if (SuspendThread(thread->h) == (DWORD)-1)
				fatal("can't suspend thread %lu", thread->id);
			thread->affinity = 0;
		}
	}
	printf("running threads:\n");
	list_for_each(thread, &g_queue, struct thread_info, link) {
		if (thread->worker == TRUE && thread->affinity)
			printf("  thread %lu with affinity %#lx\n", thread->id, thread->affinity);
	}
}

static void create_thread(const DEBUG_EVENT* event)
{
//	printf("create_thread: %lu\n", event->dwThreadId);
	struct thread_info* thread = (struct thread_info*)malloc(sizeof(struct thread_info));
	list_init(&thread->link);
	thread->worker = FALSE;
	thread->id = event->dwThreadId;
	thread->h = event->u.CreateThread.hThread;
	thread->affinity = g_cpu_avail;
	list_insert_before(&thread->link, &g_queue);
}

static void exit_thread(const DEBUG_EVENT* event)
{
//	printf("exit_thread: %lu\n", event->dwThreadId);
	struct thread_info* thread;
	list_for_each(thread, &g_queue, struct thread_info, link) {
		if (thread->id == event->dwThreadId) {
			list_erase(&thread->link);
			if (thread->affinity)
				reschedule();
			free(thread);
			return;
		}
	}
	if (g_thread0_id == event->dwThreadId && g_thread0_id != (DWORD)-1) {
		g_thread0_id = (DWORD)-1;
		return;
	}
	fatal("exiting unregistered thread %lu", event->dwThreadId);
}

static BOOL exception(const DEBUG_EVENT* event)
{
//	printf("exception %#lx\n", event->u.Exception.ExceptionRecord.ExceptionCode);
	if (event->u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT)
		return TRUE;
	if (event->u.Exception.ExceptionRecord.ExceptionCode != MSVC_EXCEPTION)
		return FALSE;

	if (event->u.Exception.ExceptionRecord.NumberParameters != 4)
		return FALSE;

	const THREADNAME_INFO* info = (const THREADNAME_INFO*)&event->u.Exception.ExceptionRecord.ExceptionInformation;
	if (info->dwType != 0x1000)
		fatal("unexpected MSVC exception type");
	if (info->dwThreadID != (DWORD)-1)
		fatal("unexpected MSVC exception parameters");

	char name[64], *p = name, *last = name + sizeof(name) - 1;
	const char *src = info->szName;
	for (; p != last && ReadProcessMemory(g_process_h, src, p, 1, NULL) && *p; ++src, ++p) {}
	if (p == last)
		*p = 0;
	if (strcmp(name, "worker-thread") == 0) {
		struct thread_info* thread;
		list_for_each(thread, &g_queue, struct thread_info, link) {
			if (thread->id == event->dwThreadId) {
				thread->worker = TRUE;
				reschedule();
				return TRUE;
			}
		}
		fatal("unregistered worker thread %lu", event->dwThreadId);
	}
	return TRUE;
}

static void create_process(const DEBUG_EVENT* event)
{
	DWORD_PTR unused, cpu_avail;
	printf("create_process: %lu/%lu\n", event->dwProcessId, event->dwThreadId);
	if (g_process_h != NULL)
		fatal("creating unexpected secondary process");
	g_process_h = event->u.CreateProcessInfo.hProcess;
	g_process_id = event->dwProcessId;
	g_thread0_id = event->dwThreadId;
	GetProcessAffinityMask(g_process_h, &unused, &cpu_avail);
	g_cpu_avail = cpu_avail;
	printf("  avail cpu mask: %#lx\n", (DWORD)cpu_avail);
}

static void exit_process(const DEBUG_EVENT* event)
{
	printf("exit_process: %lu/%lu\n", event->dwProcessId, event->dwThreadId);
	if (g_process_h == NULL)
		fatal("exiting unregistered primary process");
	if (g_process_id != event->dwProcessId)
		fatal("exiting unregistered process");
	g_process_h = NULL;
}

static void debug_loop(void)
{
	list_init(&g_queue);

	DEBUG_EVENT event;
	DWORD cont_status = DBG_CONTINUE;
	for (;;) {
		WaitForDebugEvent(&event, INFINITE);
		switch (event.dwDebugEventCode) {
		case EXCEPTION_DEBUG_EVENT:
			if (!exception(&event))
				cont_status = DBG_EXCEPTION_NOT_HANDLED;
			break;

		case CREATE_THREAD_DEBUG_EVENT:
			create_thread(&event);
			break;

		case EXIT_THREAD_DEBUG_EVENT:
			exit_thread(&event);
			break;

		case CREATE_PROCESS_DEBUG_EVENT:
			create_process(&event);
			CloseHandle(event.u.CreateProcessInfo.hFile);
			break;

		case EXIT_PROCESS_DEBUG_EVENT:
			exit_process(&event);
			break;

		case LOAD_DLL_DEBUG_EVENT:
			CloseHandle(event.u.LoadDll.hFile);
			break;
		}
		ContinueDebugEvent(event.dwProcessId, event.dwThreadId, cont_status);
	}
}

static void run_application(const char* cmd, const char* root)
{
	char* temp = _strdup(cmd);

	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	ZeroMemory(&pi, sizeof(pi));

	BOOL success = CreateProcess(NULL, temp, NULL, NULL, FALSE, DEBUG_ONLY_THIS_PROCESS, NULL, root, &si, &pi);
	if (!success)
		fatal("can't launch '%s'", cmd);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	free(temp);
}

#define SDK_ROOT	"c:\\program files\\x-ray sdk\\level_editor\\"
#define SDK_BINARY(x)	(SDK_ROOT "bins\\" x)

int main(int argc, char* argv[])
{
//	run_application(SDK_BINARY("ShaderEditor.exe"), SDK_ROOT);
	run_application(SDK_BINARY("compiler\\xrLC.exe -f lest_re"), SDK_ROOT);
//	run_application(SDK_BINARY("compiler\\xrDO_Light.exe -f lest_re"), SDK_ROOT);
//	run_application(SDK_BINARY("compiler\\xrLC.exe -f ttuner_test1"), SDK_ROOT);
//	run_application(SDK_BINARY("compiler\\xrLC.exe -f ttuner_test2"), SDK_ROOT);
//	run_application(SDK_BINARY("compiler\\xrDO_Light.exe -f ttuner_test2"), SDK_ROOT);
	debug_loop();
	return 0;
}
