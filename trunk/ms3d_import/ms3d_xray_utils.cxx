#include <msLib.h>
#include "ms3d_xray_utils.h"
#include "xr_file_system.h"

using namespace xray_re;

const size_t COMMENT_MAX_SIZE = 1024;	// arbitrary value

void set_comment(msMaterial* mtl, const char* eshader, const char* cshader,
		const char* gamemtl, bool two_sided)
{
#if 1
	int left = COMMENT_MAX_SIZE;
	char* comment = new char[COMMENT_MAX_SIZE];
	char* p = comment;
	if (eshader && left > 0) {
		int n = sprintf_s(p, left, "eshader = %s\n", eshader);
		comment += n;
		left -= n;
	}
	if (cshader && left > 0) {
		int n = sprintf_s(p, left, "cshader = %s\n", cshader);
		comment += n;
		left -= n;
	}
	if (gamemtl && left > 0) {
		int n = sprintf_s(p, left, "gamemtl = %s\n", gamemtl);
		comment += n;
		left -= n;
	}
	if (two_sided && left > 0)
		sprintf_s(p, left, "two_sided = true\n");
	msMaterial_SetComment(mtl, comment);
	delete[] comment;
#else
	std::string comment;
	if (eshader)
		comment.append("eshader = ").append(eshader).append("\n");
	if (cshader)
		comment.append("cshader = ").append(cshader).append("\n");
	if (gamemtl)
		comment.append("gamemtl = ").append(gamemtl).append("\n");
	if (two_sided)
		comment.append("two_sided = true\n");
	msMaterial_SetComment(mtl, comment.c_str());
#endif
}

void set_texture(msMaterial* mtl, const std::string& texture)
{
	std::string path;
	xr_file_system& fs = xr_file_system::instance();
	if (!fs.resolve_path(PA_GAME_TEXTURES, texture, path))
		path = texture;
	msMaterial_SetDiffuseTexture(mtl, path.append(".dds").c_str());
}
