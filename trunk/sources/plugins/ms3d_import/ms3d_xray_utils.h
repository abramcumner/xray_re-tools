#pragma once
#ifndef __MS3D_XRAY_UTILS_H__
#define __MS3D_XRAY_UTILS_H__

#include <string>

struct msMaterial;

void set_comment(msMaterial* mtl, const char* eshader = 0, const char* cshader = 0,
		const char* gamemtl = 0, bool two_sided = false);
void set_texture(msMaterial* mtl, const std::string& texture);

#endif
