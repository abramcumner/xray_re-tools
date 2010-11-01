// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Log.h"
#include "Assert.h"
#include "FileSystem.h"
#include "EngineShadersLib.h"
#include "GameMaterialsLib.h"
#include "CompilerShadersLib.h"
#include "Object.h"



int main(int argc, char* argv[])
{
	xrFSL::FileSystem& fs = xrFSL::FileSystem::Instance();

	ASSERT_F(fs.Initialize("e:\\Projects\\xrFSL\\build\\debug\\fs.ltx", 0), ("File system initialization failed."));

	xrFSL::String logs;
	ASSERT_F(fs.ResolvePath(xrFSL::PA_LOGS, "test.log", logs), ("Resolve path failed"));

	xrFSL::CLog& Log = xrFSL::CLog::Instance();
	Log.Init(logs.c_str());

	//==========================
	// Materials test
	//==========================
#if 0
	xrFSL::EngineShadersLib shaders_lib;
	if (shaders_lib.load(xrFSL::PA_GAME_DATA, "shaders.xr")) 
	{
		for (std::vector<xrFSL::String>::const_iterator it = shaders_lib.names().begin(),
			end = shaders_lib.names().end(); it != end; ++it) 
		{
				LOGI(*it);
		}
	} else 
	{
		LOGE("can't open shaders.xr");
	}

	LOG("");
	LOG("------------------------------");
	LOG("");

	xrFSL::GameMaterialsLib gamemtls_lib;
	if (gamemtls_lib.load(xrFSL::PA_GAME_DATA, "gamemtl.xr")) 
	{
		for (std::vector<xrFSL::SGameMaterial*>::const_iterator it = gamemtls_lib.materials().begin(),
			end = gamemtls_lib.materials().end(); it != end; ++it) 
		{
				LOGI((*it)->sName);
		}
	} else {
		LOGE("can't open gamemtl.xr");
	}
	
	LOG("");
	LOG("------------------------------");
	LOG("");

	xrFSL::SCompilerShadersLib xrlcShaders_lib;
	if (xrlcShaders_lib.load(xrFSL::PA_GAME_DATA, "shaders_xrlc.xr")) 
	{
		for (std::vector<xrFSL::SCompilerShader*>::const_iterator it = xrlcShaders_lib.shaders().begin(),
			end = xrlcShaders_lib.shaders().end(); it != end; ++it) 
		{
			LOGI((*it)->sName);
		}
	} else {
		LOGE("can't open shaders_xrlc.xr");
	}
#endif

	ASSERT_F(fs.ResolvePath(xrFSL::PA_GAME_DATA, "corp_byaka.object", logs), ("Resolve path failed"));
	xrFSL::Object obj;
	obj.Load(logs.c_str());

	LOGS << "Flags: " << obj.flags();
	LOGS << "UserData: " << obj.userdata();
	LOGS << "Num surfaces: " << obj.surfaces().size();
	LOGS << "Num meshes: " << obj.meshes().size();	
	for (std::vector<xrFSL::Mesh*>::const_iterator it = obj.meshes().begin(),
		end = obj.meshes().end(); it != end; ++it) 
	{
		LOGI((*it)->name());
	}

	LOG("");
	LOG("------------------------------");
	LOG("");

	ASSERT_F(fs.ResolvePath(xrFSL::PA_GAME_DATA, "t_bar_03.object", logs), ("Resolve path failed"));
	xrFSL::Object obj2;
	obj2.Load(logs.c_str());

	LOGS << "Flags: " << obj2.flags();
	LOGS << "UserData: " << obj2.userdata();
	LOGS << "Num surfaces: " << obj2.surfaces().size();
	LOGS << "Num meshes: " << obj2.meshes().size();
	for (std::vector<xrFSL::Mesh*>::const_iterator it = obj2.meshes().begin(),
		end = obj2.meshes().end(); it != end; ++it) 
	{
		LOGI((*it)->name());
	}

	LOG("");
	LOG("-------- Textures -----------");
	LOG("");

	for (std::vector<xrFSL::Surface*>::const_iterator it = obj2.surfaces().begin(),
		end = obj2.surfaces().end(); it != end; ++it) 
	{
		LOGI((*it)->texture());
	}

// 	ASSERT_W(false, ("Test app assert"));
// 	ASSERT_F(false, ("Test app assert fatal"));

	return 0;
}

