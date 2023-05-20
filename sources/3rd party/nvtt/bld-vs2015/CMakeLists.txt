set(PROJECT_NAME nvtt)

################################################################################
# Source groups
################################################################################
set(Headers
    "../nvconfig.h"
    "../nvcore/Containers.h"
    "../nvcore/Debug.h"
    "../nvcore/DefsVcWin32.h"
    "../nvcore/Memory.h"
    "../nvcore/nvcore.h"
    "../nvcore/Ptr.h"
    "../nvcore/StrLib.h"
    "../nvcore/TextReader.h"
    "../nvcore/TextWriter.h"
    "../nvimage/BlockDXT.h"
    "../nvimage/ColorBlock.h"
    "../nvimage/DirectDrawSurface.h"
    "../nvimage/Filter.h"
    "../nvimage/FloatImage.h"
    "../nvimage/Image.h"
    "../nvimage/ImageIO.h"
    "../nvimage/NormalMap.h"
    "../nvimage/nvimage.h"
    "../nvimage/PixelFormat.h"
    "../nvimage/PsdFile.h"
    "../nvimage/Quantize.h"
    "../nvimage/TgaFile.h"
    "../nvmath/Box.h"
    "../nvmath/Color.h"
    "../nvmath/Matrix.h"
    "../nvmath/Montecarlo.h"
    "../nvmath/nvmath.h"
    "../nvmath/Plane.h"
    "../nvmath/Quaternion.h"
    "../nvmath/Random.h"
    "../nvmath/SphericalHarmonic.h"
    "../nvmath/Triangle.h"
    "../nvmath/Vector.h"
    "../nvtt/CompressDXT.h"
    "../nvtt/CompressionOptions.h"
    "../nvtt/Compressor.h"
    "../nvtt/CompressRGB.h"
    "../nvtt/cuda/Bitmaps.h"
    "../nvtt/cuda/CudaCompressDXT.h"
    "../nvtt/cuda/CudaMath.h"
    "../nvtt/cuda/CudaUtils.h"
    "../nvtt/InputOptions.h"
    "../nvtt/nvtt.h"
    "../nvtt/OptimalCompressDXT.h"
    "../nvtt/OutputOptions.h"
    "../nvtt/QuickCompressDXT.h"
    "../nvtt/SingleColorLookup.h"
    "../nvtt/squish/clusterfit.h"
    "../nvtt/squish/colourblock.h"
    "../nvtt/squish/colourfit.h"
    "../nvtt/squish/colourset.h"
    "../nvtt/squish/fastclusterfit.h"
    "../nvtt/squish/maths.h"
    "../nvtt/squish/simd.h"
    "../nvtt/squish/simd_sse.h"
    "../nvtt/squish/weightedclusterfit.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "../nvcore/Debug.cpp"
    "../nvcore/Memory.cpp"
    "../nvcore/StrLib.cpp"
    "../nvcore/TextReader.cpp"
    "../nvcore/TextWriter.cpp"
    "../nvimage/BlockDXT.cpp"
    "../nvimage/ColorBlock.cpp"
    "../nvimage/DirectDrawSurface.cpp"
    "../nvimage/Filter.cpp"
    "../nvimage/FloatImage.cpp"
    "../nvimage/Image.cpp"
    "../nvimage/ImageIO.cpp"
    "../nvimage/NormalMap.cpp"
    "../nvimage/Quantize.cpp"
    "../nvmath/Montecarlo.cpp"
    "../nvmath/Plane.cpp"
    "../nvmath/Random.cpp"
    "../nvmath/SphericalHarmonic.cpp"
    "../nvmath/Triangle.cpp"
    "../nvmath/TriBox.cpp"
    "../nvtt/CompressDXT.cpp"
    "../nvtt/CompressionOptions.cpp"
    "../nvtt/Compressor.cpp"
    "../nvtt/CompressRGB.cpp"
    "../nvtt/cuda/CudaCompressDXT.cpp"
    "../nvtt/cuda/CudaUtils.cpp"
    "../nvtt/InputOptions.cpp"
    "../nvtt/nvtt.cpp"
    "../nvtt/OptimalCompressDXT.cpp"
    "../nvtt/OutputOptions.cpp"
    "../nvtt/QuickCompressDXT.cpp"
    "../nvtt/squish/clusterfit.cpp"
    "../nvtt/squish/colourblock.cpp"
    "../nvtt/squish/colourfit.cpp"
    "../nvtt/squish/colourset.cpp"
    "../nvtt/squish/fastclusterfit.cpp"
    "../nvtt/squish/maths.cpp"
    "../nvtt/squish/weightedclusterfit.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "3rd party")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE nvtt)

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
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
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
        "_LIB;"
        "__SSE2__;"
        "__SSE__;"
        "__MMX__"
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
        "_LIB;"
        "__SSE2__;"
        "__SSE__;"
        "__MMX__"
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
                /wd4595;
                /wd4244;
                /wd4091
            >
            $<$<CONFIG:Release>:
                /GF;
                /arch:SSE2;
                ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
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
                /GF;
                ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
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

