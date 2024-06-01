cmake_minimum_required(VERSION 3.15)
project(converter LANGUAGES CXX)

add_library(opcode STATIC)
target_compile_definitions(opcode PUBLIC ICE_NO_DLL BAN_OPCODE_AUTOLINK)
target_include_directories(opcode PRIVATE opcode)
target_sources(opcode PRIVATE
	opcode/OPC_AABBCollider.cpp
	opcode/OPC_AABBCollider.h
	opcode/OPC_AABBTree.cpp
	opcode/OPC_AABBTree.h
	opcode/OPC_BaseModel.cpp
	opcode/OPC_BaseModel.h
	opcode/OPC_BoxBoxOverlap.h
	opcode/OPC_BoxPruning.cpp
	opcode/OPC_BoxPruning.h
	opcode/OPC_Collider.cpp
	opcode/OPC_Collider.h
	opcode/OPC_Common.cpp
	opcode/OPC_Common.h
	opcode/OPC_HybridModel.cpp
	opcode/OPC_HybridModel.h
	opcode/OPC_IceHook.h
	opcode/OPC_LSSAABBOverlap.h
	opcode/OPC_LSSCollider.cpp
	opcode/OPC_LSSCollider.h
	opcode/OPC_LSSTriOverlap.h
	opcode/OPC_MeshInterface.cpp
	opcode/OPC_MeshInterface.h
	opcode/OPC_Model.cpp
	opcode/OPC_Model.h
	opcode/OPC_OBBCollider.cpp
	opcode/OPC_OBBCollider.h
	opcode/OPC_OptimizedTree.cpp
	opcode/OPC_OptimizedTree.h
	opcode/OPC_Picking.cpp
	opcode/OPC_Picking.h
	opcode/OPC_PlanesAABBOverlap.h
	opcode/OPC_PlanesCollider.cpp
	opcode/OPC_PlanesCollider.h
	opcode/OPC_PlanesTriOverlap.h
	opcode/OPC_RayAABBOverlap.h
	opcode/OPC_RayCollider.cpp
	opcode/OPC_RayCollider.h
	opcode/OPC_RayTriOverlap.h
	opcode/OPC_Settings.h
	opcode/OPC_SphereAABBOverlap.h
	opcode/OPC_SphereCollider.cpp
	opcode/OPC_SphereCollider.h
	opcode/OPC_SphereTriOverlap.h
	opcode/OPC_SweepAndPrune.cpp
	opcode/OPC_SweepAndPrune.h
	opcode/OPC_TreeBuilders.cpp
	opcode/OPC_TreeBuilders.h
	opcode/OPC_TreeCollider.cpp
	opcode/OPC_TreeCollider.h
	opcode/OPC_TriBoxOverlap.h
	opcode/OPC_TriTriOverlap.h
	opcode/OPC_VolumeCollider.cpp
	opcode/OPC_VolumeCollider.h
	opcode/Opcode.cpp
	opcode/Opcode.h
	opcode/StdAfx.cpp
	opcode/StdAfx.h
	opcode/Ice/IceAABB.cpp
	opcode/Ice/IceAABB.h
	opcode/Ice/IceAxes.h
	opcode/Ice/IceBoundingSphere.h
	opcode/Ice/IceContainer.cpp
	opcode/Ice/IceContainer.h
	opcode/Ice/IceFPU.h
	opcode/Ice/IceHPoint.cpp
	opcode/Ice/IceHPoint.h
	opcode/Ice/IceIndexedTriangle.cpp
	opcode/Ice/IceIndexedTriangle.h
	opcode/Ice/IceLSS.h
	opcode/Ice/IceMatrix3x3.cpp
	opcode/Ice/IceMatrix3x3.h
	opcode/Ice/IceMatrix4x4.cpp
	opcode/Ice/IceMatrix4x4.h
	opcode/Ice/IceMemoryMacros.h
	opcode/Ice/IceOBB.cpp
	opcode/Ice/IceOBB.h
	opcode/Ice/IcePairs.h
	opcode/Ice/IcePlane.cpp
	opcode/Ice/IcePlane.h
	opcode/Ice/IcePoint.cpp
	opcode/Ice/IcePoint.h
	opcode/Ice/IcePreprocessor.h
	opcode/Ice/IceRandom.cpp
	opcode/Ice/IceRandom.h
	opcode/Ice/IceRay.cpp
	opcode/Ice/IceRay.h
	opcode/Ice/IceRevisitedRadix.cpp
	opcode/Ice/IceRevisitedRadix.h
	opcode/Ice/IceSegment.cpp
	opcode/Ice/IceSegment.h
	opcode/Ice/IceTriangle.cpp
	opcode/Ice/IceTriangle.h
	opcode/Ice/IceTrilist.h
	opcode/Ice/IceTypes.h
	opcode/Ice/IceUtils.cpp
	opcode/Ice/IceUtils.h
)
add_library(Opcode::Opcode ALIAS opcode)

add_executable(converter)
target_include_directories(converter PRIVATE opcode)
target_sources(converter PRIVATE
	batch_helper.cxx
	batch_helper.h
	converter.cxx
	converter.h
	db_tools.cxx
	db_tools.h
	dds_tools.cxx
	dds_tools.h
	dm_tools.cxx
	dm_tools.h
	fancy_tools.cxx
	fancy_tools.h
	level_aux_mesh.cxx
	level_aux_mesh.h
	level_mesh.cxx
	level_mesh.h
	level_mesh_commit.cxx
	level_mesh_ladders.cxx
	level_mesh_materials.cxx
	level_mesh_opcode.h
	level_mesh_terrain.cxx
	level_tools.cxx
	level_tools.h
	level_tools_ai_map.cxx
	level_tools_debug_cform.cxx
	level_tools_details.cxx
	level_tools_glows.cxx
	level_tools_hom.cxx
	level_tools_lights.cxx
	level_tools_particles.cxx
	level_tools_portals.cxx
	level_tools_sectors.cxx
	level_tools_som.cxx
	level_tools_sound_envs.cxx
	level_tools_sound_srcs.cxx
	level_tools_spawns.cxx
	level_tools_visuals.cxx
	level_tools_wallmarks.cxx
	level_tools_ways.cxx
	object_tools.cxx
	object_tools.h
	ogf_tools.cxx
	ogf_tools.h
	ogg_tools.cxx
	ogg_tools.h
	tools_base.h
	xrdemo_tools.cxx
	xrdemo_tools.h
	lzo/lzoconf.h
	lzo/lzodefs.h
	lzo/minilzo.c
	lzo/minilzo.h
	crc32/crc32.c
	crc32/crc32.h
)

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

target_link_libraries(converter PRIVATE
	XrayRe::XrayRe
	Xiph::libogg
	Xiph::libvorbisfile
	Opcode::Opcode
)
  
