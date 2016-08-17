#include "syncer.h"
#include "xr_level_gct.h"

using namespace xray_re;

void syncer::to_raw(xr_level_gct& gct) const
{
	uint_fast16_t min_graph_id = UINT16_MAX, max_graph_id = 0;
	for (gct_cell *it = gct.cells(), *end = it + gct.num_nodes(); it != end; ++it) {
		uint_fast16_t graph_id = it->graph_id;
		if (graph_id < min_graph_id)
			min_graph_id = graph_id;
		else if (graph_id > max_graph_id)
			max_graph_id = graph_id;
	}
	xr_assert(gct.num_graph_points() == (size_t)max_graph_id - min_graph_id + 1);
	for (gct_cell *it = gct.cells(), *end = it + gct.num_nodes(); it != end; ++it) {
		it->graph_id -= uint16_t(min_graph_id);
	}
}
