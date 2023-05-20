set(PROJECT_NAME converter)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "../batch_helper.cxx"
    "../batch_helper.h"
    "../converter.cxx"
    "../converter.h"
    "../opcode/StdAfx.cpp"
    "../opcode/StdAfx.h"
    "../tools_base.h"
)
source_group("" FILES ${no_group_source_files})

set(crc32
    "../crc32/crc32.c"
    "../crc32/crc32.h"
)
source_group("crc32" FILES ${crc32})

set(db
    "../db_tools.cxx"
    "../db_tools.h"
)
source_group("db" FILES ${db})

set(dds
    "../dds_tools.cxx"
    "../dds_tools.h"
)
source_group("dds" FILES ${dds})

set(level
    "../fancy_tools.cxx"
    "../fancy_tools.h"
    "../level_aux_mesh.cxx"
    "../level_aux_mesh.h"
    "../level_mesh.cxx"
    "../level_mesh_commit.cxx"
    "../level_mesh_ladders.cxx"
    "../level_mesh_materials.cxx"
    "../level_mesh_opcode.h"
    "../level_mesh_terrain.cxx"
    "../level_tools.cxx"
    "../level_tools.h"
    "../level_tools_ai_map.cxx"
    "../level_tools_debug_cform.cxx"
    "../level_tools_details.cxx"
    "../level_tools_glows.cxx"
    "../level_tools_hom.cxx"
    "../level_tools_lights.cxx"
    "../level_tools_particles.cxx"
    "../level_tools_portals.cxx"
    "../level_tools_sectors.cxx"
    "../level_tools_som.cxx"
    "../level_tools_sound_envs.cxx"
    "../level_tools_sound_srcs.cxx"
    "../level_tools_spawns.cxx"
    "../level_tools_visuals.cxx"
    "../level_tools_wallmarks.cxx"
    "../level_tools_ways.cxx"
)
source_group("level" FILES ${level})

set(minilzo
    "../lzo/lzoconf.h"
    "../lzo/lzodefs.h"
    "../lzo/minilzo.c"
    "../lzo/minilzo.h"
)
source_group("minilzo" FILES ${minilzo})

set(object
    "../dm_tools.cxx"
    "../dm_tools.h"
    "../object_tools.cxx"
    "../object_tools.h"
    "../ogf_tools.cxx"
    "../ogf_tools.h"
)
source_group("object" FILES ${object})

set(ogg
    "../ogg_tools.cxx"
    "../ogg_tools.h"
)
source_group("ogg" FILES ${ogg})

set(opcode
    "../opcode/OPC_AABBCollider.cpp"
    "../opcode/OPC_AABBCollider.h"
    "../opcode/OPC_AABBTree.cpp"
    "../opcode/OPC_AABBTree.h"
    "../opcode/OPC_BaseModel.cpp"
    "../opcode/OPC_BaseModel.h"
    "../opcode/OPC_BoxBoxOverlap.h"
    "../opcode/OPC_BoxPruning.cpp"
    "../opcode/OPC_BoxPruning.h"
    "../opcode/OPC_Collider.cpp"
    "../opcode/OPC_Collider.h"
    "../opcode/OPC_Common.cpp"
    "../opcode/OPC_Common.h"
    "../opcode/OPC_HybridModel.cpp"
    "../opcode/OPC_HybridModel.h"
    "../opcode/OPC_IceHook.h"
    "../opcode/OPC_LSSAABBOverlap.h"
    "../opcode/OPC_LSSCollider.cpp"
    "../opcode/OPC_LSSCollider.h"
    "../opcode/OPC_LSSTriOverlap.h"
    "../opcode/OPC_MeshInterface.cpp"
    "../opcode/OPC_MeshInterface.h"
    "../opcode/OPC_Model.cpp"
    "../opcode/OPC_Model.h"
    "../opcode/OPC_OBBCollider.cpp"
    "../opcode/OPC_OBBCollider.h"
    "../opcode/OPC_OptimizedTree.cpp"
    "../opcode/OPC_OptimizedTree.h"
    "../opcode/OPC_Picking.cpp"
    "../opcode/OPC_Picking.h"
    "../opcode/OPC_PlanesAABBOverlap.h"
    "../opcode/OPC_PlanesCollider.cpp"
    "../opcode/OPC_PlanesCollider.h"
    "../opcode/OPC_PlanesTriOverlap.h"
    "../opcode/OPC_RayAABBOverlap.h"
    "../opcode/OPC_RayCollider.cpp"
    "../opcode/OPC_RayCollider.h"
    "../opcode/OPC_RayTriOverlap.h"
    "../opcode/OPC_Settings.h"
    "../opcode/OPC_SphereAABBOverlap.h"
    "../opcode/OPC_SphereCollider.cpp"
    "../opcode/OPC_SphereCollider.h"
    "../opcode/OPC_SphereTriOverlap.h"
    "../opcode/OPC_SweepAndPrune.cpp"
    "../opcode/OPC_SweepAndPrune.h"
    "../opcode/OPC_TreeBuilders.cpp"
    "../opcode/OPC_TreeBuilders.h"
    "../opcode/OPC_TreeCollider.cpp"
    "../opcode/OPC_TreeCollider.h"
    "../opcode/OPC_TriBoxOverlap.h"
    "../opcode/OPC_TriTriOverlap.h"
    "../opcode/OPC_VolumeCollider.cpp"
    "../opcode/OPC_VolumeCollider.h"
    "../opcode/Opcode.cpp"
    "../opcode/Opcode.h"
)
source_group("opcode" FILES ${opcode})

set(opcode__Ice
    "../opcode/Ice/IceAABB.cpp"
    "../opcode/Ice/IceAABB.h"
    "../opcode/Ice/IceAxes.h"
    "../opcode/Ice/IceBoundingSphere.h"
    "../opcode/Ice/IceContainer.cpp"
    "../opcode/Ice/IceContainer.h"
    "../opcode/Ice/IceFPU.h"
    "../opcode/Ice/IceHPoint.cpp"
    "../opcode/Ice/IceHPoint.h"
    "../opcode/Ice/IceIndexedTriangle.cpp"
    "../opcode/Ice/IceIndexedTriangle.h"
    "../opcode/Ice/IceLSS.h"
    "../opcode/Ice/IceMatrix3x3.cpp"
    "../opcode/Ice/IceMatrix3x3.h"
    "../opcode/Ice/IceMatrix4x4.cpp"
    "../opcode/Ice/IceMatrix4x4.h"
    "../opcode/Ice/IceMemoryMacros.h"
    "../opcode/Ice/IceOBB.cpp"
    "../opcode/Ice/IceOBB.h"
    "../opcode/Ice/IcePairs.h"
    "../opcode/Ice/IcePlane.cpp"
    "../opcode/Ice/IcePlane.h"
    "../opcode/Ice/IcePoint.cpp"
    "../opcode/Ice/IcePoint.h"
    "../opcode/Ice/IcePreprocessor.h"
    "../opcode/Ice/IceRandom.cpp"
    "../opcode/Ice/IceRandom.h"
    "../opcode/Ice/IceRay.cpp"
    "../opcode/Ice/IceRay.h"
    "../opcode/Ice/IceRevisitedRadix.cpp"
    "../opcode/Ice/IceRevisitedRadix.h"
    "../opcode/Ice/IceSegment.cpp"
    "../opcode/Ice/IceSegment.h"
    "../opcode/Ice/IceTriangle.cpp"
    "../opcode/Ice/IceTriangle.h"
    "../opcode/Ice/IceTrilist.h"
    "../opcode/Ice/IceTypes.h"
    "../opcode/Ice/IceUtils.cpp"
    "../opcode/Ice/IceUtils.h"
)
source_group("opcode\\Ice" FILES ${opcode__Ice})

set(xrdemo
    "../xrdemo_tools.cxx"
    "../xrdemo_tools.h"
)
source_group("xrdemo" FILES ${xrdemo})

set(ALL_FILES
    ${no_group_source_files}
    ${crc32}
    ${db}
    ${dds}
    ${level}
    ${minilzo}
    ${object}
    ${ogg}
    ${opcode}
    ${opcode__Ice}
    ${xrdemo}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "utils")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE converter)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/../binaries/${CMAKE_VS_PLATFORM_NAME}-$<CONFIG>/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/../binaries/${CMAKE_VS_PLATFORM_NAME}-$<CONFIG>/"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/../binaries/${CMAKE_VS_PLATFORM_NAME}-$<CONFIG>/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/../binaries/${CMAKE_VS_PLATFORM_NAME}-$<CONFIG>/"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebugDLL
        >
        $<$<CONFIG:Release>:
            MultiThreadedDLL
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebugDLL
        >
        $<$<CONFIG:Release>:
            MultiThreadedDLL
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../xray_re;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../opcode;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd party/libvorbis/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd party/libogg/include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../xray_re;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../opcode;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd party/libvorbis/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd party/libogg/include"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "ICE_NO_DLL;"
        "BAN_OPCODE_AUTOLINK"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "ICE_NO_DLL;"
        "BAN_OPCODE_AUTOLINK"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Gm;
                /Od;
                /RTC1;
                /ZI
            >
            $<$<CONFIG:Release>:
                /O2;
                /Ob2;
                /Oi;
                /GF;
                /arch:SSE2;
                /Gy-;
                ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
                /Ot;
                /GS-
            >
            /std:c++17;
            /W3;
            /Zc:__cplusplus;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Gm;
                /Od;
                /RTC1;
                /Zi
            >
            $<$<CONFIG:Release>:
                /GL;
                /O2;
                /Ob2;
                /Oi;
                /GF;
                ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
                /Ot;
                /GS-
            >
            /std:c++17;
            /W3;
            /Zc:__cplusplus;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /DEBUG:FULL;
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /NODEFAULTLIB:libcmt.lib;
                /MANIFEST;
                /OPT:REF;
                /OPT:ICF;
                /DYNAMICBASE:NO;
                /LARGEADDRESSAWARE
            >
            /MACHINE:X86;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /DEBUG;
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /NODEFAULTLIB:libcmt.lib;
                /MANIFEST;
                /OPT:REF;
                /OPT:ICF
            >
            /MACHINE:X64;
            /SUBSYSTEM:CONSOLE;
            /DYNAMICBASE:NO
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    libvorbisfile
    xray_re
)

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/../../../../binaries/$<CONFIG>/libraries/x64"
        ">"
    )
endif()

