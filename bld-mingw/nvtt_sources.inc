NVCORE_DIR := $(ROOT_DIR)/nvtt/nvcore
NVIMAGE_DIR := $(ROOT_DIR)/nvtt/nvimage
NVMATH_DIR := $(ROOT_DIR)/nvtt/nvmath
NVTT_DIR := $(ROOT_DIR)/nvtt/nvtt
SQUISH_DIR := $(ROOT_DIR)/nvtt/nvtt/squish
CUDA_DIR := $(ROOT_DIR)/nvtt/nvtt/cuda
NVTT_SOURCES := \
	$(NVTT_DIR)/CompressDXT.cpp \
	$(NVTT_DIR)/CompressionOptions.cpp \
	$(NVTT_DIR)/Compressor.cpp \
	$(NVTT_DIR)/CompressRGB.cpp \
	$(NVTT_DIR)/InputOptions.cpp \
	$(NVTT_DIR)/nvtt.cpp \
	$(NVTT_DIR)/OptimalCompressDXT.cpp \
	$(NVTT_DIR)/OutputOptions.cpp \
	$(NVTT_DIR)/QuickCompressDXT.cpp \
	\
	$(SQUISH_DIR)/clusterfit.cpp \
	$(SQUISH_DIR)/colourblock.cpp \
	$(SQUISH_DIR)/colourfit.cpp \
	$(SQUISH_DIR)/colourset.cpp \
	$(SQUISH_DIR)/fastclusterfit.cpp \
	$(SQUISH_DIR)/maths.cpp \
	$(SQUISH_DIR)/weightedclusterfit.cpp \
	\
	$(CUDA_DIR)/CudaCompressDXT.cpp \
	$(CUDA_DIR)/CudaUtils.cpp \
	\
	$(NVCORE_DIR)/Debug.cpp \
	$(NVCORE_DIR)/Memory.cpp \
	$(NVCORE_DIR)/StrLib.cpp \
	$(NVCORE_DIR)/TextReader.cpp \
	$(NVCORE_DIR)/TextWriter.cpp \
	\
	$(NVIMAGE_DIR)/BlockDXT.cpp \
	$(NVIMAGE_DIR)/ColorBlock.cpp \
	$(NVIMAGE_DIR)/DirectDrawSurface.cpp \
	$(NVIMAGE_DIR)/Filter.cpp \
	$(NVIMAGE_DIR)/FloatImage.cpp \
	$(NVIMAGE_DIR)/Image.cpp \
	$(NVIMAGE_DIR)/ImageIO.cpp \
	$(NVIMAGE_DIR)/NormalMap.cpp \
	$(NVIMAGE_DIR)/Quantize.cpp \
	\
	$(NVMATH_DIR)/Montecarlo.cpp \
	$(NVMATH_DIR)/Plane.cpp \
	$(NVMATH_DIR)/Random.cpp \
	$(NVMATH_DIR)/SphericalHarmonic.cpp \
	$(NVMATH_DIR)/Triangle.cpp \
	$(NVMATH_DIR)/TriBox.cpp
