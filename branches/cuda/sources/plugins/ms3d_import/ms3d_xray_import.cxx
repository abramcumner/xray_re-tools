#define NOMINMAX
#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include "ms3d_xray_import.h"
#include "ms3d_object_import.h"
#include "ms3d_ogf_import.h"
#include "ms3d_skl_import.h"
#include "ms3d_dm_import.h"
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
		xr_log::instance().init("xrayMS3DImport");
	}
	return new ms3d_xray_import;
}

void DestroyPlugIn(cMsPlugIn* plugin) { delete plugin; }

ms3d_xray_import::~ms3d_xray_import() {}

int ms3d_xray_import::GetType() { return eTypeImport|eNormalsAndTexCoordsPerTriangleVertex; }

const char* ms3d_xray_import::GetTitle() { return "S.T.A.L.K.E.R. ("__DATE__")"; }

static const TCHAR k_filter[] = _T(
//	"X-Ray objects (*.ogf;*.dm;*.object)\0*.ogf;*.dm;*.object\0"
	"X-Ray objects (*.ogf;*.dm)\0*.ogf;*.dm\0"
	"X-Ray skeletal motions (*.skl)\0*.skl\0"
	"\0\0"
);

int ms3d_xray_import::Execute(msModel* model)
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
//	ofn.nFilterIndex = 0;
	ofn.lpstrFile = path;
	ofn.nMaxFile = xr_dim(path);
//	ofn.lpstrFileTitle = NULL;
//	ofn.nMaxFileTitle = 0;
//	ofn.lpstrInitialDir = NULL;
//	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
//	ofn.nFileOffset = 0;
//	ofn.nFileExtension = 0;
//	ofn.lpstrDefExt = NULL;
//	ofn.lCustData = 0;
//	ofn.lpfnHook = NULL;
//	ofn.lpTemplateName = NULL;
//	ofn.pvReserved = NULL;
//	ofn.dwReserved = 0;
//	ofn.FlagsEx = 0;
	if (GetOpenFileName(&ofn) == FALSE)
		return -1;

	int status = -1;

	TCHAR* ext = ofn.lpstrFile + ofn.nFileExtension;
	if (_tcsicmp(ext, _T("ogf")) == 0) {
		xr_ogf* ogf = xr_ogf::load_ogf(path);
		if (ogf) {
			ms3d_ogf_import ogf_io(model);
			status = ogf_io.import(ogf);
			delete ogf;
		}
	} else if (_tcsicmp(ext, _T("dm")) == 0) {
		xr_dm* dm = new xr_dm;
		if (dm->load_dm(path)) {
			ms3d_dm_import dm_io(model);
			status = dm_io.import(dm);
			delete dm;
		}
	} else if (_tcsicmp(ext, _T("object")) == 0) {
		xr_object* object = new xr_object;
		if (object->load_object(path)) {
			ms3d_object_import object_io(model);
			status = object_io.import(object);
			delete object;
		}
	} else if (_tcsicmp(ext, _T("skl")) == 0) {
		xr_skl_motion* smotion = new xr_skl_motion;
		if (smotion->load_skl(path)) {
			ms3d_skl_import skl_io(model);
			status = skl_io.import(smotion);
			delete smotion;
		}
	}
	if (status < 0)
		msg("can't import %s", path);
	return status;
}
