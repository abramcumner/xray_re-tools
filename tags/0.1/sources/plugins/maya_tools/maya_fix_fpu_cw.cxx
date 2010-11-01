#include <cfloat>
#include "maya_fix_fpu_cw.h"
#include "xr_types.h"

// Official GSC plugins for Maya (xrayMaya_export.mll and xrayMaya_material.mll)
// depend on the xrCore.dll which resets the FPU precision to 24 bits.
// Since both Maya and OGF motion conversion code use doubles, it is _really_
// better to restore the default 53-bit precision.
void fix_fpu_cw()
{
#if defined(_MSC_VER) && defined(_M_IX86)
	unsigned int x87_cw, new_x87_cw;

	if (__control87_2(0, 0, &x87_cw, 0) != 1)
		return;
	if ((x87_cw & _MCW_PC) == _PC_53 && (x87_cw & _MCW_DN) == _DN_SAVE)
		return;
	__control87_2(_PC_53|_DN_SAVE, _MCW_PC|_MCW_DN, &new_x87_cw, 0);
	xray_re::msg("x87 CW fixup: %8.8x -> %8.8x", x87_cw, new_x87_cw);
#endif
}
