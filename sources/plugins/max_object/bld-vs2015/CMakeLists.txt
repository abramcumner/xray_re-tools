set(PROJECT_NAME max_object90)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "../kernel/max_object.def"
    "../kernel/dmtlbut.bmp"
    "../kernel/dmtlmsk.bmp"
)
source_group("" FILES ${no_group_source_files})

set(Headers
    "../kernel/resources.h"
    "../kernel/Stdafx.h"
    "../material/BlenderListLoader.h"
    "../material/GameMaterial.h"
    "../material/TexMaps.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "../kernel/DllEntry.cpp"
    "../kernel/stdafx.cpp"
    "../material/BlenderListLoader.cpp"
    "../material/GameMaterial.cpp"
    "../material/TexMaps.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${no_group_source_files}
    ${Headers}
    ${Sources}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "plugins/max_object")

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/../kernel/Stdafx.h>"
)

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE max_object90)

################################################################################
# Target name
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG   "${PROJECT_NAME}"
        TARGET_NAME_RELEASE "${PROJECT_NAME}"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG   "${PROJECT_NAME}"
        TARGET_NAME_RELEASE "${PROJECT_NAME}"
    )
endif()
################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/../binaries/${CMAKE_VS_PLATFORM_NAME}-$<CONFIG>"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/../binaries/${CMAKE_VS_PLATFORM_NAME}-$<CONFIG>"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/../binaries/${CMAKE_VS_PLATFORM_NAME}-$<CONFIG>"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/../binaries/${CMAKE_VS_PLATFORM_NAME}-$<CONFIG>"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_NAME_DEBUG   "${PROJECT_NAME}"
        ARCHIVE_OUTPUT_NAME_RELEASE "${PROJECT_NAME}"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_NAME_DEBUG   "${PROJECT_NAME}"
        ARCHIVE_OUTPUT_NAME_RELEASE "${PROJECT_NAME}"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        PDB_OUTPUT_DIRECTORY         "${OUTPUT_DIRECTORY}"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        PDB_OUTPUT_DIRECTORY         "${OUTPUT_DIRECTORY}"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        MultiThreadedDLL
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        MultiThreadedDLL
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd party/3dmax_sdk/6.0/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd party/3dmax_sdk/6.0/cssdk/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../xray_re"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd party/3dmax_sdk/6.0/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd party/3dmax_sdk/6.0/cssdk/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../xray_re"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_WINDOWS;"
        "_USRDLL;"
        "_MAX_MATERIAL;"
        "_MAX_PLUGIN;"
        "XRCORE_STATIC"
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
        "_WINDOWS;"
        "_USRDLL;"
        "_MAX_MATERIAL;"
        "_MAX_PLUGIN;"
        "XRCORE_STATIC"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /ZI
            >
            $<$<CONFIG:Release>:
                /Gm-;
                /Ob1;
                /Gy;
                ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
            >
            /GF;
            /Gd;
            /W3;
            /fp:fast;
            /GR;
            /LD;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Zc:forScope-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /ZI
            >
            $<$<CONFIG:Release>:
                /Gm-;
                /Ob1;
                /Gy;
                ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
            >
            /GF;
            /Gd;
            /W3;
            /fp:fast;
            /GR;
            /LD;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Zc:forScope-
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /DEBUG;
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /DEBUG:FULL;
                /INCREMENTAL:NO
            >
            /NOLOGO;
            /SUBSYSTEM:WINDOWS;
            /DYNAMICBASE:NO;
            /MACHINE:I386
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /DEBUG;
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /DEBUG:FULL;
                /INCREMENTAL:NO
            >
            /NOLOGO;
            /SUBSYSTEM:WINDOWS;
            /DYNAMICBASE:NO;
            /MACHINE:I386
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:"
            "bmm"
        ">"
        "Winmm;"
        "comctl32;"
        "core;"
        "geom;"
        "gfx;"
        "mesh;"
        "maxutil;"
        "maxscrpt;"
        "manipsys;"
        "paramblk2;"
        "xray_re"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:"
            "bmm"
        ">"
        "Winmm;"
        "comctl32;"
        "core;"
        "geom;"
        "gfx;"
        "mesh;"
        "maxutil;"
        "maxscrpt;"
        "manipsys;"
        "paramblk2;"
        "xray_re"
    )
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd party/3dmax_sdk/6.0/lib;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../binaries/$<CONFIG>/libraries"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd party/3dmax_sdk/6.0/lib;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../binaries/$<CONFIG>/libraries"
    )
endif()

