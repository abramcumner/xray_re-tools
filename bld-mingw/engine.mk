ROOT_DIR ?= D:/stk1/xraytools

include $(ROOT_DIR)/bld-mingw/xray_re_sources.inc
include $(ROOT_DIR)/bld-mingw/nvtt_sources.inc
SOURCES += $(XRAY_RE_SOURCES) $(NVTT_SOURCES)



###############################################################################
# Defaults

OBJ_DIR ?= $(ROOT_DIR)/bld-mingw/_obj

CC := gcc
CFLAGS += -Wall -Wno-unknown-pragmas -O3 -mfpmath=sse -msse3 -mthreads \
	-I$(ROOT_DIR)/xray_re

CXX := g++
CXXFLAGS += -Wall -Wno-unknown-pragmas -O3 -mfpmath=sse -msse3 -mthreads \
	-I$(ROOT_DIR)/xray_re -I$(ROOT_DIR)/nvtt \
	-D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS \
	-DWIN32 -DWINVER=0x500 -DNDEBUG

LD := g++
LDFLAGS := -Wl,--large-address-aware -Wl,-s

###############################################################################
# Vars

OBJS := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(foreach file, $(SOURCES), $(basename $(notdir $(file))))))

SOURCE_PATHS := $(sort $(foreach file, $(SOURCES), $(subst /:,,$(addsuffix :, $(dir $(file))))))

VPATH := $(addprefix :, $(SOURCE_PATHS))

###############################################################################
# Build

all: $(OBJ_DIR) $(OBJ_DIR)/$(TARGET)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)
#	@[ -d $(OBJ_DIR) ] || mkdir $(OBJ_DIR) > /dev/null 2>&1

$(OBJ_DIR)/$(TARGET): $(OBJ_DIR) $(OBJS)
	$(LD) -o $@ $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.cxx
	$(CXX) -c $< $(CXXFLAGS) -o $@

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

FORCE:

clean:
	@[ -d $(OBJ_DIR) ] && cd $(OBJ_DIR) && rm -f *.exe *.[aod]
