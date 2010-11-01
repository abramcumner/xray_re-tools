#include "xr_shape.h"
#include "level_mesh.h"

using namespace xray_re;

void level_mesh::push(uint16_t sector_idx, const shape_def& shape, const fmatrix& xform,
		uint16_t texture, uint16_t eshader, uint16_t gamemtl)
{
	const float down_tolerance = 0.2f;

	float sx = shape.box._11;
	float sy = shape.box._22;
	float sz = shape.box._33;

//	fvector3 shift;
	if (sx < down_tolerance) {
		sx = down_tolerance;
//		shift.set(down_tolerance, 0, 0);
	}
	if (sy < down_tolerance) {
		sy = down_tolerance;
//		shift.set(0, down_tolerance, 0);
	}
	if (sz < down_tolerance) {
		sz = down_tolerance;
//		shift.set(0, 0, down_tolerance);
	}

#if 0
	fmatrix xform;
	xform.set(xform0);
	shift.rotate(xform);
	xform.c.sub(shift);
#endif

	fvector3 axis, side, normal;
	if (sy <= sx) {
		if (sz <= sy) {
			// z <= y <= x
			axis.mul(xform.i, sx);
			side.mul(xform.j, sy);
			normal.mul(xform.k, sz);
		} else if (sz <= sx) {
			// y < z <= x
			axis.mul(xform.i, sx);
			side.mul(xform.k, sz);
			normal.mul(xform.j, sy);
		} else {
			// y <= x < z
			axis.mul(xform.k, sz);
			side.mul(xform.i, sx);
			normal.mul(xform.j, sy);
		}
	} else {
		if (sz <= sx) {
			// z <= x < y
			axis.mul(xform.j, sy);
			side.mul(xform.i, sx);
			normal.mul(xform.k, sz);
		} if (sz <= sy) {
			// x < z <= y
			axis.mul(xform.j, sy);
			side.mul(xform.k, sz);
			normal.mul(xform.i, sx);
		} else {
			// x < y < z
			axis.mul(xform.k, sz);
			side.mul(xform.j, sy);
			normal.mul(xform.i, sx);
		}
	}
	if (axis.y < 0) {
		axis.invert();
		side.invert();
	}

	normal.normalize();

	fvector3 temp;
	temp.cross_product(axis, side).normalize();
	if (!normal.similar(temp, 1e-4f))
		side.invert();

	fvector3 p[4];
	p[0].sub(xform.c, side); p[0].sub(axis);	// left-bottom
	p[1].sub(xform.c, side); p[1].add(axis);	// left-top
	p[2].add(xform.c, side); p[2].sub(axis);	// right-bottom
	p[3].add(xform.c, side); p[3].add(axis);	// right-top

	fvector2 tc[4];
	tc[0].set(1.f, 1.f);				// left-bottom
	tc[1].set(1.f, 0);				// left-top
	tc[2].set(0, 1.f);				// right-bottom
	tc[3].set(0, 0);				// right-top

	fvector3 n[4];
	for (uint_fast32_t i = 4; i != 0;)
		n[--i].set(normal);

	uint32_t vb_offset = uint32_t(m_vb.size() & UINT32_MAX);
	xr_vbuf vb(4, p, n, tc);
	m_vb.push(vb);

	for (uint_fast32_t i = 0; i != 4; ++i)
		m_refs.push_back(b_proxy(vb_offset + i));

	b_face face_template(sector_idx);
	face_template.surface.set(texture, eshader, gamemtl, RSF_RENDERING|RSF_AUTOMATIC);
	m_faces.push_back(face_template.set(m_vb.signature(), vb_offset + 0, vb_offset + 1, vb_offset + 2));
	m_faces.push_back(face_template.set(m_vb.signature(), vb_offset + 2, vb_offset + 1, vb_offset + 3));
}
