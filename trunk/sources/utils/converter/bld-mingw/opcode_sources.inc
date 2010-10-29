OPCODE_DIR := $(ROOT_DIR)/converter/opcode
ICE_DIR := $(OPCODE_DIR)/Ice
OPCODE_SOURCES := \
	$(ICE_DIR)/IceAABB.cpp \
	$(ICE_DIR)/IceContainer.cpp \
	$(ICE_DIR)/IceHPoint.cpp \
	$(ICE_DIR)/IceIndexedTriangle.cpp \
	$(ICE_DIR)/IceMatrix3x3.cpp \
	$(ICE_DIR)/IceMatrix4x4.cpp \
	$(ICE_DIR)/IceOBB.cpp \
	$(ICE_DIR)/IcePlane.cpp \
	$(ICE_DIR)/IcePoint.cpp \
	$(ICE_DIR)/IceRandom.cpp \
	$(ICE_DIR)/IceRay.cpp \
	$(ICE_DIR)/IceRevisitedRadix.cpp \
	$(ICE_DIR)/IceSegment.cpp \
	$(ICE_DIR)/IceTriangle.cpp \
	$(ICE_DIR)/IceUtils.cpp \
	\
	$(OPCODE_DIR)/OPC_AABBCollider.cpp \
	$(OPCODE_DIR)/OPC_AABBTree.cpp \
	$(OPCODE_DIR)/OPC_BaseModel.cpp \
	$(OPCODE_DIR)/OPC_BoxPruning.cpp \
	$(OPCODE_DIR)/OPC_Collider.cpp \
	$(OPCODE_DIR)/OPC_Common.cpp \
	$(OPCODE_DIR)/OPC_HybridModel.cpp \
	$(OPCODE_DIR)/OPC_LSSCollider.cpp \
	$(OPCODE_DIR)/OPC_MeshInterface.cpp \
	$(OPCODE_DIR)/OPC_Model.cpp \
	$(OPCODE_DIR)/OPC_OBBCollider.cpp \
	$(OPCODE_DIR)/OPC_OptimizedTree.cpp \
	$(OPCODE_DIR)/OPC_Picking.cpp \
	$(OPCODE_DIR)/OPC_PlanesCollider.cpp \
	$(OPCODE_DIR)/OPC_RayCollider.cpp \
	$(OPCODE_DIR)/OPC_SphereCollider.cpp \
	$(OPCODE_DIR)/OPC_SweepAndPrune.cpp \
	$(OPCODE_DIR)/OPC_TreeBuilders.cpp \
	$(OPCODE_DIR)/OPC_TreeCollider.cpp \
	$(OPCODE_DIR)/OPC_VolumeCollider.cpp \
	$(OPCODE_DIR)/Opcode.cpp \
	$(OPCODE_DIR)/StdAfx.cpp
