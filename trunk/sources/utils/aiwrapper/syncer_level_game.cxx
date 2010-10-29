#include "xr_level_game.h"
#include "xr_file_system.h"
#include "syncer.h"

using namespace xray_re;

bool syncer::load_mp_rpoints(xr_level_game& game, const char* path, const char* name) const
{
	xr_file_system& fs = xr_file_system::instance();
	xr_reader* r = fs.r_open(path, name);
	if (r == 0)
		return false;
	xr_reader* s = r->open_chunk(GAME_CHUNK_RPOINTS);
	if (s) {
		s->r_chunks(game.rpoints(), mp_rpoint_io());
		r->close_chunk(s);
	}
	fs.r_close(r);
	return true;
}
