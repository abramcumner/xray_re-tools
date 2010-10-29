#define NOMINMAX
#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include <msLib.h>
#include "ms3d_xray_export.h"
#include "ms3d_object_export.h"
#include "ms3d_skl_export.h"
#include "xr_log.h"
#include "xr_file_system.h"

using namespace xray_re;

static bool g_fs_inited = false;

BOOL WINAPI DllMain(HINSTANCE hinstance, ULONG reason, LPVOID reserved)
{
	return TRUE;
}

cMsPlugIn* CreatePlugIn()
{
	if (!g_fs_inited) {
		char path[MAX_PATH + 16];
		DWORD n = GetModuleFileName(NULL, path, xr_dim(path));
		if (n && PathRemoveFileSpec(path)) {
			strcat_s(path, sizeof(path), "\\xray_path.ltx");
			xr_file_system& fs = xr_file_system::instance();
			if (!fs.initialize(path))
				MessageBox(NULL, _T("can't initialize the file system"), _T("X-Ray error"), MB_OK);
		}
		g_fs_inited = true;
		xr_log::instance().init("xrayMS3DExport");
	}
	return new ms3d_xray_export;
}

void DestroyPlugIn(cMsPlugIn* plugin) { delete plugin; }

ms3d_xray_export::~ms3d_xray_export() {}

//int ms3d_xray_export::GetType() { return eTypeExport|eNormalsAndTexCoordsPerTriangleVertex; }
int ms3d_xray_export::GetType() { return eTypeExport; }

const char* ms3d_xray_export::GetTitle() { return "S.T.A.L.K.E.R... ("__DATE__")"; }

static const TCHAR k_filter[] = _T(
	"X-Ray object (*.object)\0*.object\0"
	"X-Ray skeletal motion (*.skl)\0*.skl\0"
	"\0\0"
);

int ms3d_xray_export::Execute(msModel* model)
{
	if (model == 0)
		return -1;

	TCHAR path[MAX_PATH] = "\0";

	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(OPENFILENAME);
//	ofn.hwndOwner = NULL;
//	ofn.hInstance = NULL;
	ofn.lpstrFilter = k_filter;
//	ofn.lpstrCustomFilter = NULL;
//	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = path;
	ofn.nMaxFile = xr_dim(path);
//	ofn.lpstrFileTitle = NULL;
//	ofn.nMaxFileTitle = 0;
	xr_file_system& fs = xr_file_system::instance();
	ofn.lpstrInitialDir = fs.resolve_path(PA_IMPORT);
//	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;
//	ofn.nFileOffset = 0;
//	ofn.nFileExtension = 0;
//	ofn.lpstrDefExt = NULL;
//	ofn.lCustData = 0;
//	ofn.lpfnHook = NULL;
//	ofn.lpTemplateName = NULL;
//	ofn.pvReserved = NULL;
//	ofn.dwReserved = 0;
//	ofn.FlagsEx = 0;
	if (GetSaveFileName(&ofn) == FALSE)
		return -1;

	int status = -1;
	TCHAR* ext = ofn.lpstrFile + ofn.nFileExtension;
	if (_tcsicmp(ext, _T("object")) == 0) {
		ms3d_object_export object_io(model);
		status = object_io.do_export(path);
	} else if (_tcsicmp(ext, _T("skl")) == 0) {
		ms3d_skl_export skl_io(model);
		status = skl_io.do_export(path);
	}
	if (status < 0) {
		msg("can't export %s", path);
		return -1;
	}
	msModel_Destroy(model);
	return 0;
}
