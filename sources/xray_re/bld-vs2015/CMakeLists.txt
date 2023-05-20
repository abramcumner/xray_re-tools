set(PROJECT_NAME xray_re)

################################################################################
# Source groups
################################################################################
set(file_system
    "../xr_file_system.cxx"
    "../xr_file_system.h"
    "../xr_file_system_win32.cxx"
    "../xr_file_system_win32.h"
    "../xr_ini_file.cxx"
    "../xr_ini_file.h"
    "../xr_lzhuf.cxx"
    "../xr_lzhuf.h"
    "../xr_reader.cxx"
    "../xr_reader.h"
    "../xr_reader_scrambler.cxx"
    "../xr_scrambler.cxx"
    "../xr_scrambler.h"
    "../xr_writer.cxx"
    "../xr_writer.h"
)
source_group("file system" FILES ${file_system})

set(game
    "../xr_ai_cross_table.cxx"
    "../xr_ai_cross_table.h"
    "../xr_ai_graph.cxx"
    "../xr_ai_graph.h"
    "../xr_ai_map.h"
    "../xr_ai_version.h"
    "../xr_ai_way.cxx"
    "../xr_ai_way.h"
    "../xr_clsid.cxx"
    "../xr_clsid.h"
    "../xr_entity.cxx"
    "../xr_entity.h"
    "../xr_entity_factory.cxx"
    "../xr_entity_factory.h"
    "../xr_entity_la.cxx"
    "../xr_entity_la.h"
    "../xr_entity_script.cxx"
    "../xr_entity_script.h"
    "../xr_entity_zenobian.cxx"
    "../xr_entity_zenobian.h"
    "../xr_game_graph.cxx"
    "../xr_game_graph.h"
    "../xr_game_spawn.cxx"
    "../xr_game_spawn.h"
    "../xr_packet.cxx"
    "../xr_packet.h"
)
source_group("game" FILES ${game})

set(kinematics
    "../xr_bone.cxx"
    "../xr_bone.h"
    "../xr_envelope.cxx"
    "../xr_envelope.h"
    "../xr_envelope_eval.cxx"
    "../xr_influence.cxx"
    "../xr_influence.h"
    "../xr_motion.cxx"
    "../xr_motion.h"
    "../xr_obj_motion.cxx"
    "../xr_obj_motion.h"
    "../xr_skeleton.cxx"
    "../xr_skeleton.h"
    "../xr_skl_motion.cxx"
    "../xr_skl_motion.h"
)
source_group("kinematics" FILES ${kinematics})

set(level
    "../xr_blender.cxx"
    "../xr_blender.h"
    "../xr_build_lights.cxx"
    "../xr_build_lights.h"
    "../xr_cform.cxx"
    "../xr_cform.h"
    "../xr_d3d_light.cxx"
#    "../xr_d3d_lights.h"
    "../xr_details.cxx"
    "../xr_details.h"
    "../xr_gamemtls_lib.cxx"
    "../xr_gamemtls_lib.h"
    "../xr_level.cxx"
    "../xr_level.h"
    "../xr_level_ai.cxx"
    "../xr_level_ai.h"
    "../xr_level_cform.cxx"
    "../xr_level_cform.h"
    "../xr_level_details.cxx"
    "../xr_level_details.h"
    "../xr_level_dm.cxx"
    "../xr_level_dm.h"
    "../xr_level_env_mod.cxx"
    "../xr_level_env_mod.h"
    "../xr_level_fog_vol.cxx"
    "../xr_level_fog_vol.h"
    "../xr_level_game.cxx"
    "../xr_level_game.h"
    "../xr_level_gct.cxx"
    "../xr_level_gct.h"
    "../xr_level_gct_v9.cxx"
    "../xr_level_geom.cxx"
    "../xr_level_geom.h"
    "../xr_level_glows.cxx"
    "../xr_level_glows.h"
    "../xr_level_graph.cxx"
    "../xr_level_graph.h"
    "../xr_level_hom.cxx"
    "../xr_level_hom.h"
    "../xr_level_lights.cxx"
    "../xr_level_lights.h"
    "../xr_level_ltx.cxx"
    "../xr_level_ltx.h"
    "../xr_level_portals.cxx"
    "../xr_level_portals.h"
    "../xr_level_ps_static.cxx"
    "../xr_level_ps_static.h"
    "../xr_level_sectors.cxx"
    "../xr_level_sectors.h"
    "../xr_level_shaders.cxx"
    "../xr_level_shaders.h"
    "../xr_level_snd_env.cxx"
    "../xr_level_snd_env.h"
    "../xr_level_snd_static.cxx"
    "../xr_level_snd_static.h"
    "../xr_level_som.cxx"
    "../xr_level_som.h"
    "../xr_level_spawn.cxx"
    "../xr_level_spawn.h"
    "../xr_level_version.h"
    "../xr_level_visuals.cxx"
    "../xr_level_visuals.h"
    "../xr_level_wallmarks.cxx"
    "../xr_level_wallmarks.h"
    "../xr_shaders_lib.cxx"
    "../xr_shaders_lib.h"
    "../xr_shaders_xrlc_lib.cxx"
    "../xr_shaders_xrlc_lib.h"
    "../xr_wallmarks.h"
)
source_group("level" FILES ${level})

set(math
    "../xr_aabb.h"
    "../xr_cylinder.h"
    "../xr_limits.h"
    "../xr_math.h"
    "../xr_matrix.cxx"
    "../xr_matrix.h"
    "../xr_obb.h"
    "../xr_plane.h"
    "../xr_quaternion.cxx"
    "../xr_quaternion.h"
    "../xr_rect.h"
    "../xr_shape.h"
    "../xr_sphere.h"
    "../xr_vector2.h"
    "../xr_vector3.cxx"
    "../xr_vector3.h"
    "../xr_vector4.h"
)
source_group("math" FILES ${math})

set(mesh
    "../xr_build_err.cxx"
    "../xr_build_err.h"
    "../xr_geom_buf.cxx"
    "../xr_geom_buf.h"
    "../xr_mesh.cxx"
    "../xr_mesh.h"
    "../xr_mesh_builder.cxx"
    "../xr_mesh_builder.h"
    "../xr_mesh_utils.h"
    "../xr_mesh_vbuf.cxx"
    "../xr_mesh_vbuf.h"
    "../xr_remapper.h"
)
source_group("mesh" FILES ${mesh})

set(misc
    "../xr_cl_parser.cxx"
    "../xr_cl_parser.h"
    "../xr_color.h"
    "../xr_fixed_vector.h"
    "../xr_guid.cxx"
    "../xr_guid.h"
    "../xr_image.cxx"
    "../xr_image.h"
    "../xr_image_bmp.cxx"
    "../xr_image_dds.cxx"
    "../xr_image_tga.cxx"
    "../xr_log.cxx"
    "../xr_log.h"
    "../xr_name_gen.cxx"
    "../xr_name_gen.h"
    "../xr_sound_thumbnail.cxx"
    "../xr_sound_thumbnail.h"
    "../xr_string_utils.h"
    "../xr_texture_thumbnail.cxx"
    "../xr_texture_thumbnail.h"
    "../xr_thumbnail.h"
    "../xr_types.h"
    "../xr_utils.h"
)
source_group("misc" FILES ${misc})

set(object
    "../xr_dm.cxx"
    "../xr_dm.h"
    "../xr_object.cxx"
    "../xr_object.h"
    "../xr_object_format.h"
    "../xr_ogf.cxx"
    "../xr_ogf.h"
    "../xr_ogf_format.h"
    "../xr_ogf_v3.cxx"
    "../xr_ogf_v3.h"
    "../xr_ogf_v4.cxx"
    "../xr_ogf_v4.h"
    "../xr_surface.cxx"
    "../xr_surface.h"
)
source_group("object" FILES ${object})

set(scene
    "../xr_scene.cxx"
    "../xr_scene.h"
    "../xr_scene_ai_map.cxx"
    "../xr_scene_ai_map.h"
    "../xr_scene_common.h"
    "../xr_scene_details.cxx"
    "../xr_scene_details.h"
    "../xr_scene_glows.cxx"
    "../xr_scene_glows.h"
    "../xr_scene_groups.cxx"
    "../xr_scene_groups.h"
    "../xr_scene_lights.cxx"
    "../xr_scene_lights.h"
    "../xr_scene_objects.cxx"
    "../xr_scene_objects.h"
    "../xr_scene_part.cxx"
    "../xr_scene_part.h"
    "../xr_scene_particles.cxx"
    "../xr_scene_particles.h"
    "../xr_scene_portals.cxx"
    "../xr_scene_portals.h"
    "../xr_scene_revision.cxx"
    "../xr_scene_revision.h"
    "../xr_scene_sectors.cxx"
    "../xr_scene_sectors.h"
    "../xr_scene_shapes.cxx"
    "../xr_scene_shapes.h"
    "../xr_scene_sound_envs.cxx"
    "../xr_scene_sound_envs.h"
    "../xr_scene_sound_srcs.cxx"
    "../xr_scene_sound_srcs.h"
    "../xr_scene_spawns.cxx"
    "../xr_scene_spawns.h"
    "../xr_scene_visuals.cxx"
    "../xr_scene_visuals.h"
    "../xr_scene_wallmarks.cxx"
    "../xr_scene_wallmarks.h"
    "../xr_scene_ways.cxx"
    "../xr_scene_ways.h"
)
source_group("scene" FILES ${scene})

set(ALL_FILES
    ${file_system}
    ${game}
    ${kinematics}
    ${level}
    ${math}
    ${mesh}
    ${misc}
    ${object}
    ${scene}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
################################################################################
# Includes for CMake from *.props
################################################################################
use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "$ENV{VCTargetsPath}/BuildCustomizations/masm.cmake")

set(ROOT_NAMESPACE xray_re)

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
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rd party/nvtt"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rd party/nvtt"
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
            "NDEBUG;"
            "__SSE2__;"
            "__SSE__;"
            "__MMX__"
        ">"
        "WIN32;"
        "_LIB"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG;"
            "__SSE2__;"
            "__SSE__;"
            "__MMX__"
        ">"
        "WIN32;"
        "_LIB"
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
                /ZI;
                /wd4996
            >
            $<$<CONFIG:Release>:
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
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Gm;
                /Od;
                /RTC1;
                /Zi
            >
            $<$<CONFIG:Release>:
                /O2;
                /Ob2;
                /Oi;
                /GF;
                /Oy-;
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
endif()

################################################################################
# Dependencies
################################################################################
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    nvtt
)

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Release>:"
            "nvtt"
        ">"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "nvtt"
    )
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Release>:"
            "${OUTPUT_DIRECTORY}"
        ">"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "${OUTPUT_DIRECTORY}"
    )
endif()

