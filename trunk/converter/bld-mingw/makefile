OBJ_DIR := _obj
ROOT_DIR := ../..
SRC_DIR := ..

CXXFLAGS := -DICE_NO_DLL=1 -DBAN_OPCODE_AUTOLINK=1 \
	-I$(SRC_DIR)/Opcode -I$(SRC_DIR)/Opcode/Ice

TARGET := converter
SOURCES := \
	$(SRC_DIR)/batch_helper.cxx \
	$(SRC_DIR)/converter.cxx \
	$(SRC_DIR)/db_tools.cxx \
	$(SRC_DIR)/dds_tools.cxx \
	$(SRC_DIR)/dm_tools.cxx \
	$(SRC_DIR)/fancy_tools.cxx \
	$(SRC_DIR)/level_aux_mesh.cxx \
	$(SRC_DIR)/level_mesh.cxx \
	$(SRC_DIR)/level_mesh_commit.cxx \
	$(SRC_DIR)/level_mesh_ladders.cxx \
	$(SRC_DIR)/level_mesh_materials.cxx \
	$(SRC_DIR)/level_mesh_terrain.cxx \
	$(SRC_DIR)/level_tools.cxx \
	$(SRC_DIR)/level_tools_ai_map.cxx \
	$(SRC_DIR)/level_tools_debug_cform.cxx \
	$(SRC_DIR)/level_tools_details.cxx \
	$(SRC_DIR)/level_tools_glows.cxx \
	$(SRC_DIR)/level_tools_hom.cxx \
	$(SRC_DIR)/level_tools_lights.cxx \
	$(SRC_DIR)/level_tools_particles.cxx \
	$(SRC_DIR)/level_tools_portals.cxx \
	$(SRC_DIR)/level_tools_sectors.cxx \
	$(SRC_DIR)/level_tools_som.cxx \
	$(SRC_DIR)/level_tools_sound_envs.cxx \
	$(SRC_DIR)/level_tools_sound_srcs.cxx \
	$(SRC_DIR)/level_tools_spawns.cxx \
	$(SRC_DIR)/level_tools_visuals.cxx \
	$(SRC_DIR)/level_tools_wallmarks.cxx \
	$(SRC_DIR)/level_tools_ways.cxx \
	$(SRC_DIR)/object_tools.cxx \
	$(SRC_DIR)/ogf_tools.cxx \
	$(SRC_DIR)/ogg_tools.cxx \
	$(SRC_DIR)/xrdemo_tools.cxx \
	$(SRC_DIR)/crc32/crc32.c \
	$(SRC_DIR)/lzo/minilzo.c

include opcode_sources.inc
SOURCES += $(OPCODE_SOURCES)

include $(ROOT_DIR)/bld-mingw/engine.mk
