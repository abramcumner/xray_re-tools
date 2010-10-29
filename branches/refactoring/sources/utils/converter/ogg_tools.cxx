#include "ogg_tools.h"
#include "xr_types.h"

using namespace xray_re;

void ogg_tools::process(const cl_parser& cl)
{
	msg("not yet implemented");
}

#if 0
{
	float		base_volume;
	float		min_dist;
	float		max_dist;
	float		max_ai_dist;
	unsigned	game_type;

	unsigned version = r.r_u32();
	if (version == 1) {
		base_volume = 1.f;
		min_dist = r.r_float();
		max_ai_dist = max_dist = r.r_float();
		game_type = r.r_u32();
	} else if (version == 2) {
		min_dist = r.r_float();
		max_ai_dist = max_dist = r.r_float();
		base_volume = r.r_float();
		game_type = r.r_u32();
	} else if (version == 3) {
		min_dist = r.r_float();
		max_dist = r.r_float();
		base_volume = r.r_float();
		game_type = r.r_u32();
		max_ai_dist = r.r_float();
	}
}
#endif
