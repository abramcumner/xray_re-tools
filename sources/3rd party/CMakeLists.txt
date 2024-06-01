project(nvtt)

add_library(nvcore STATIC)
target_include_directories(nvcore PUBLIC nvtt)
target_sources(nvcore PRIVATE
	nvtt/nvcore/nvcore.h
	nvtt/nvcore/Ptr.h
	nvtt/nvcore/BitArray.h
	nvtt/nvcore/Memory.h
	nvtt/nvcore/Memory.cpp
	nvtt/nvcore/Debug.h
	nvtt/nvcore/Debug.cpp
	nvtt/nvcore/Containers.h
	nvtt/nvcore/StrLib.h
	nvtt/nvcore/StrLib.cpp
	nvtt/nvcore/Stream.h
	nvtt/nvcore/StdStream.h
	nvtt/nvcore/TextReader.h
	nvtt/nvcore/TextReader.cpp
	nvtt/nvcore/TextWriter.h
	nvtt/nvcore/TextWriter.cpp
	nvtt/nvcore/Radix.h
	nvtt/nvcore/Radix.cpp
	nvtt/nvcore/Library.h
	nvtt/nvcore/Library.cpp
)
add_library(Nvtt::Core ALIAS nvcore)

add_library(nvimage STATIC)
target_include_directories(nvimage PUBLIC nvtt)
target_sources(nvimage PRIVATE
	nvtt/nvimage/BlockDXT.cpp
	nvtt/nvimage/BlockDXT.h
	nvtt/nvimage/ColorBlock.cpp
	nvtt/nvimage/ColorBlock.h
#	nvtt/nvimage/ConeMap.cpp
#	nvtt/nvimage/ConeMap.h
	nvtt/nvimage/DirectDrawSurface.cpp
	nvtt/nvimage/DirectDrawSurface.h
	nvtt/nvimage/Filter.cpp
	nvtt/nvimage/Filter.h
	nvtt/nvimage/FloatImage.cpp
	nvtt/nvimage/FloatImage.h
	nvtt/nvimage/HoleFilling.cpp
	nvtt/nvimage/HoleFilling.h
	nvtt/nvimage/Image.cpp
	nvtt/nvimage/Image.h
	nvtt/nvimage/ImageIO.cpp
	nvtt/nvimage/ImageIO.h
	nvtt/nvimage/NormalMap.cpp
	nvtt/nvimage/NormalMap.h
	nvtt/nvimage/NormalMipmap.cpp
	nvtt/nvimage/NormalMipmap.h
	nvtt/nvimage/nvimage.h
	nvtt/nvimage/PixelFormat.h
	nvtt/nvimage/PsdFile.h
	nvtt/nvimage/Quantize.cpp
	nvtt/nvimage/Quantize.h
	nvtt/nvimage/TgaFile.h
)
target_link_libraries(nvimage PRIVATE Nvtt::Core)
add_library(Nvtt::Image ALIAS nvimage)

add_library(nvtt STATIC)
target_link_libraries(nvtt PRIVATE nvimage)
target_include_directories(nvtt PUBLIC nvtt)
target_sources(nvtt PRIVATE
	nvtt/nvtt/CompressDXT.cpp
	nvtt/nvtt/CompressDXT.h
	nvtt/nvtt/CompressionOptions.cpp
	nvtt/nvtt/CompressionOptions.h
	nvtt/nvtt/Compressor.cpp
	nvtt/nvtt/Compressor.h
	nvtt/nvtt/CompressRGB.cpp
	nvtt/nvtt/CompressRGB.h
	nvtt/nvtt/InputOptions.cpp
	nvtt/nvtt/InputOptions.h
	nvtt/nvtt/nvtt.cpp
	nvtt/nvtt/nvtt.h
	nvtt/nvtt/nvtt_wrapper.cpp
	nvtt/nvtt/nvtt_wrapper.h
	nvtt/nvtt/OptimalCompressDXT.cpp
	nvtt/nvtt/OptimalCompressDXT.h
	nvtt/nvtt/OutputOptions.cpp
	nvtt/nvtt/OutputOptions.h
	nvtt/nvtt/QuickCompressDXT.cpp
	nvtt/nvtt/QuickCompressDXT.h
	nvtt/nvtt/SingleColorLookup.h
	nvtt/nvtt/cuda/Bitmaps.h
	nvtt/nvtt/cuda/CompressKernel.cu
	nvtt/nvtt/cuda/ConvolveKernel.cu
	nvtt/nvtt/cuda/CudaCompressDXT.cpp
	nvtt/nvtt/cuda/CudaCompressDXT.h
	nvtt/nvtt/cuda/CudaMath.h
	nvtt/nvtt/cuda/CudaUtils.cpp
	nvtt/nvtt/cuda/CudaUtils.h
#	nvtt/nvtt/squish/alpha.cpp
#	nvtt/nvtt/squish/alpha.h
	nvtt/nvtt/squish/clusterfit.cpp
	nvtt/nvtt/squish/clusterfit.h
	nvtt/nvtt/squish/colourblock.cpp
	nvtt/nvtt/squish/colourblock.h
	nvtt/nvtt/squish/colourfit.cpp
	nvtt/nvtt/squish/colourfit.h
	nvtt/nvtt/squish/colourset.cpp
	nvtt/nvtt/squish/colourset.h
	nvtt/nvtt/squish/config.h
	nvtt/nvtt/squish/fastclusterfit.cpp
	nvtt/nvtt/squish/fastclusterfit.h
	nvtt/nvtt/squish/fastclusterlookup.inl
	nvtt/nvtt/squish/maths.cpp
	nvtt/nvtt/squish/maths.h
#	nvtt/nvtt/squish/rangefit.cpp
#	nvtt/nvtt/squish/rangefit.h
	nvtt/nvtt/squish/simd.h
	nvtt/nvtt/squish/simd_3dnow.h
	nvtt/nvtt/squish/simd_sse.h
	nvtt/nvtt/squish/simd_ve.h
#	nvtt/nvtt/squish/singlechannelfit.cpp
#	nvtt/nvtt/squish/singlechannelfit.h
#	nvtt/nvtt/squish/singlecolourfit.cpp
#	nvtt/nvtt/squish/singlecolourfit.h
#	nvtt/nvtt/squish/singlecolourlookup.inl
#	nvtt/nvtt/squish/squish.cpp
#	nvtt/nvtt/squish/squish.h
	nvtt/nvtt/squish/weightedclusterfit.cpp
	nvtt/nvtt/squish/weightedclusterfit.h
)
#target_link_libraries(nvtt PRIVATE Nvtt::Core)
add_library(Nvtt::Nvtt ALIAS nvtt)

add_subdirectory(libogg)

add_library(libvorbis_static STATIC)
target_include_directories(libvorbis_static
	PUBLIC libvorbis/include
	PRIVATE libvorbis/lib)
target_sources(libvorbis_static PRIVATE
#	libvorbis/lib/books
#	libvorbis/lib/modes
	libvorbis/lib/analysis.c
	libvorbis/lib/backends.h
#	libvorbis/lib/barkmel.c
	libvorbis/lib/bitrate.c
	libvorbis/lib/bitrate.h
	libvorbis/lib/block.c
	libvorbis/lib/codebook.c
	libvorbis/lib/codebook.h
	libvorbis/lib/codec_internal.h
	libvorbis/lib/envelope.c
	libvorbis/lib/envelope.h
	libvorbis/lib/floor0.c
	libvorbis/lib/floor1.c
	libvorbis/lib/highlevel.h
	libvorbis/lib/info.c
	libvorbis/lib/lookup.c
	libvorbis/lib/lookup.h
	libvorbis/lib/lookup_data.h
	libvorbis/lib/lookups.pl
	libvorbis/lib/lpc.c
	libvorbis/lib/lpc.h
	libvorbis/lib/lsp.c
	libvorbis/lib/lsp.h
#	libvorbis/lib/Makefile.am
#	libvorbis/lib/Makefile.in
	libvorbis/lib/mapping0.c
	libvorbis/lib/masking.h
	libvorbis/lib/mdct.c
	libvorbis/lib/mdct.h
	libvorbis/lib/misc.h
	libvorbis/lib/os.h
	libvorbis/lib/psy.c
	libvorbis/lib/psy.h
#	libvorbis/lib/psytune.c
	libvorbis/lib/registry.c
	libvorbis/lib/registry.h
	libvorbis/lib/res0.c
	libvorbis/lib/scales.h
	libvorbis/lib/sharedbook.c
	libvorbis/lib/smallft.c
	libvorbis/lib/smallft.h
	libvorbis/lib/synthesis.c
#	libvorbis/lib/tone.c
	libvorbis/lib/vorbisenc.c
	libvorbis/lib/window.c
	libvorbis/lib/window.h)
target_link_libraries(libvorbis_static PRIVATE Ogg::ogg)
add_library(Xiph::libvorbis_static ALIAS libvorbis_static)

add_library(libvorbisfile STATIC)
target_include_directories(libvorbisfile PUBLIC libvorbis/include)
target_sources(libvorbisfile PRIVATE
	libvorbis/lib/vorbisfile.c)
target_link_libraries(libvorbisfile PRIVATE Ogg::ogg Xiph::libvorbis_static)
add_library(Xiph::libvorbisfile ALIAS libvorbisfile)
