set(PROJECT_NAME libvorbis_static)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "../../vorbis.def"
)
source_group("" FILES ${no_group_source_files})

set(Headers
    "../../../include/vorbis/codec.h"
    "../../../include/vorbis/vorbisenc.h"
    "../../../include/vorbis/vorbisfile.h"
    "../../../lib/backends.h"
    "../../../lib/bitrate.h"
    "../../../lib/books/coupled/res_books_stereo.h"
    "../../../lib/books/floor/floor_books.h"
    "../../../lib/books/uncoupled/res_books_uncoupled.h"
    "../../../lib/codebook.h"
    "../../../lib/codec_internal.h"
    "../../../lib/envelope.h"
    "../../../lib/highlevel.h"
    "../../../lib/lookup.h"
    "../../../lib/lookup_data.h"
    "../../../lib/lpc.h"
    "../../../lib/lsp.h"
    "../../../lib/masking.h"
    "../../../lib/mdct.h"
    "../../../lib/misc.h"
    "../../../lib/modes/floor_all.h"
    "../../../lib/modes/psych_11.h"
    "../../../lib/modes/psych_16.h"
    "../../../lib/modes/psych_44.h"
    "../../../lib/modes/psych_8.h"
    "../../../lib/modes/residue_16.h"
    "../../../lib/modes/residue_44.h"
    "../../../lib/modes/residue_44u.h"
    "../../../lib/modes/residue_8.h"
    "../../../lib/modes/setup_11.h"
    "../../../lib/modes/setup_16.h"
    "../../../lib/modes/setup_22.h"
    "../../../lib/modes/setup_32.h"
    "../../../lib/modes/setup_44.h"
    "../../../lib/modes/setup_44u.h"
    "../../../lib/modes/setup_8.h"
    "../../../lib/modes/setup_X.h"
    "../../../lib/os.h"
    "../../../lib/psy.h"
    "../../../lib/registry.h"
    "../../../lib/scales.h"
    "../../../lib/smallft.h"
    "../../../lib/window.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "../../../lib/analysis.c"
    "../../../lib/bitrate.c"
    "../../../lib/block.c"
    "../../../lib/codebook.c"
    "../../../lib/envelope.c"
    "../../../lib/floor0.c"
    "../../../lib/floor1.c"
    "../../../lib/info.c"
    "../../../lib/lookup.c"
    "../../../lib/lpc.c"
    "../../../lib/lsp.c"
    "../../../lib/mapping0.c"
    "../../../lib/mdct.c"
    "../../../lib/psy.c"
    "../../../lib/registry.c"
    "../../../lib/res0.c"
    "../../../lib/sharedbook.c"
    "../../../lib/smallft.c"
    "../../../lib/synthesis.c"
    "../../../lib/vorbisenc.c"
    "../../../lib/window.c"
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
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "3rd party")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
################################################################################
# Includes for CMake from *.props
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    use_props(${PROJECT_NAME} Debug   "../libogg.cmake")
    use_props(${PROJECT_NAME} Release "../libogg.cmake")
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    use_props(${PROJECT_NAME} Debug   "../libogg.cmake")
    use_props(${PROJECT_NAME} Release "../libogg.cmake")
endif()

set(ROOT_NAMESPACE libvorbis)

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
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../libogg/include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../libogg/include"
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
        "_WINDOWS;"
        "_USRDLL;"
        "LIBVORBIS_EXPORTS;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_WINDOWS;"
        "_USRDLL;"
        "LIBVORBIS_EXPORTS;"
        "UNICODE;"
        "_UNICODE"
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
                /Ox;
                /Ob2;
                /Oi;
                /GF;
                /Zi;
                /Ot;
                /wd4244;
                /wd4100;
                /wd4267;
                /wd4189;
                /wd4305;
                /wd4127;
                /wd4706;
                /GS-
            >
            /std:c++17;
            /Gd;
            /W4;
            /TC;
            /Zc:__cplusplus;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Gm;
                /Od;
                /RTC1
            >
            $<$<CONFIG:Release>:
                /Ox;
                /Ob2;
                /Oi;
                /GF;
                /Ot;
                /wd4244;
                /wd4100;
                /wd4267;
                /wd4189;
                /wd4305;
                /wd4127;
                /wd4706;
                /GS-
            >
            /std:c++17;
            /Gd;
            /W4;
            /Zi;
            /TC;
            /Zc:__cplusplus;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
endif()

