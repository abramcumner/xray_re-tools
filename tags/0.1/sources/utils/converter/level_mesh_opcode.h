#ifndef __GNUC__
#pragma once
#endif
#ifndef __LEVEL_MESH_OPCODE__
#define __LEVEL_MESH_OPCODE__

#include "level_mesh.h"
#include "Opcode.h"

struct b_submesh: public b_model_instance {
					b_submesh(uint32_t _tag, uint16_t _sector);
	uint32_t			group;
	Opcode::MeshInterface		opc_mesh;
	Opcode::Model			opc_model;
	IceMaths::IndexedTriangle	opc_hack[2];
};

inline b_submesh::b_submesh(uint32_t _tag, uint16_t _sector):
	b_model_instance(_tag, _sector), group(BAD_IDX) {}

#endif
