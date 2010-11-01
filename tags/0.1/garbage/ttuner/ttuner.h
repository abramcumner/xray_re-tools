#ifndef __TTUNER_H__
#define __TTUNER_H__

#include <windows.h>
#include <stddef.h>	// offsetof

struct list_node {
	struct list_node*	prev;
	struct list_node*	next;
};

static __inline void list_init(struct list_node* node)
{
	node->next = node;
	node->prev = node;
}

static __inline int list_empty(const struct list_node* node)
{
	return node->next == node;
}

static __inline void list_erase(struct list_node* node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
	list_init(node);
}

static __inline void list_insert_after(struct list_node* node, struct list_node* node2)
{
	node->next = node2->next;
	node->prev = node2;
	node2->next->prev = node;
	node2->next = node;
}

static __inline void list_insert_before(struct list_node* node, struct list_node* node2)
{
	node->next = node2;
	node->prev = node2->prev;
	node2->prev->next = node;
	node2->prev = node;
}

#define list_for_each(item, head, type, member)					\
	for (item = list_item((head)->next, type, member);			\
			&(item->member) != head;				\
			item = list_item(item->member.next, type, member))

#define list_for_each_backward(item, head, type, member)			\
	for (item = list_item((head)->prev, type, member);			\
			&(item->member) != head;				\
			item = list_item(item->member.prev, type, member))

#define list_item(node, type, member)						\
	((type*)((const unsigned char*)(node) - offsetof(type, member)))


/* exception to let debugger know the name of the thread. */
#define MSVC_EXCEPTION		0x406d1388

typedef struct tagTHREADNAME_INFO {
	DWORD	dwType;		// Must be 0x1000.
	LPCSTR	szName;		// Pointer to name (in user addr space).
	DWORD	dwThreadID;	// Thread ID (-1=caller thread).
	DWORD	dwFlags;	// Reserved for future use, must be zero.
} THREADNAME_INFO;


struct thread_info {
	struct list_node	link;
	BOOL			worker;
	DWORD			id;
	HANDLE			h;
	DWORD			affinity;
};

#endif
