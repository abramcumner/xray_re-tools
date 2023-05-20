cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(EngineToolset_vs2015 C CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_C_FLAGS "")
    set(CMAKE_C_FLAGS_DEBUG "")
    set(CMAKE_C_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

include(CMake/FindLZO.cmake)

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################
add_subdirectory("../sources/3rd party/libogg/win32/VS2015" ${CMAKE_BINARY_DIR}/libogg_static)
#add_subdirectory("../sources/3rd party/libvorbis/win32/VS2015/libvorbis" ${CMAKE_BINARY_DIR}/libvorbis_static)
#add_subdirectory("../sources/3rd party/libvorbis/win32/VS2015/libvorbisfile" ${CMAKE_BINARY_DIR}/libvorbisfile)
#add_subdirectory("../sources/3rd party/nvtt/bld-vs2015" ${CMAKE_BINARY_DIR}/nvtt)
#add_subdirectory("../sources/plugins/max_object/bld-vs2015" ${CMAKE_BINARY_DIR}/max_object90)
#add_subdirectory("../sources/plugins/maya_tools/bld-vs2015" ${CMAKE_BINARY_DIR}/maya_tools)
#add_subdirectory("../sources/plugins/ms3d_export/bld-vs2015" ${CMAKE_BINARY_DIR}/ms3d_export)
#add_subdirectory("../sources/plugins/ms3d_import/bld-vs2015" ${CMAKE_BINARY_DIR}/ms3d_import)
#add_subdirectory("../sources/utils/aiwrapper/bld-vs2015" ${CMAKE_BINARY_DIR}/aiwrapper)
add_subdirectory("../sources/utils/converter/bld-vs2015" ${CMAKE_BINARY_DIR}/converter)
#add_subdirectory("../sources/utils/make_ogf" ${CMAKE_BINARY_DIR}/makeogf)
#add_subdirectory("../sources/utils/ogf2obj" ${CMAKE_BINARY_DIR}/ogf2obj)
#add_subdirectory("../sources/utils/ogfsmda" ${CMAKE_BINARY_DIR}/ogfsmda)
#add_subdirectory("../sources/utils/parsers/parse_lev" ${CMAKE_BINARY_DIR}/parse_lev)
#add_subdirectory("../sources/utils/parsers/parse_obj" ${CMAKE_BINARY_DIR}/parse_obj)
#add_subdirectory("../sources/utils/parsers/parse_ogf" ${CMAKE_BINARY_DIR}/parse_ogf)
add_subdirectory("../sources/xray_re/bld-vs2015" ${CMAKE_BINARY_DIR}/xray_re)

