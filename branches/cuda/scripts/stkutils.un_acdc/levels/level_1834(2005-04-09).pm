package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 1702, },
{ gvid0 => 1630,	name => 'jim_controller' },
{ gvid0 => 1558,	name => 'jim_burer' },
{ gvid0 => 1486,	name => 'jim_gigant' },
{ gvid0 => 1442,	name => 'ai_test2' },
{ gvid0 => 1398,	name => 'ai_test' },
{ gvid0 => 1360,	name => 'podval_np_04_r2' },
{ gvid0 => 1316,	name => 'jim_script_inter' },
{ gvid0 => 999,	name => 'escape1' },
{ gvid0 => 935,	name => 'jim_new_ai' },
{ gvid0 => 781,	name => 'agroprom' },
{ gvid0 => 716,	name => 'dandy_test' },
{ gvid0 => 644,	name => 'jim_script' },
{ gvid0 => 579,	name => 'move_zmey2' },
{ gvid0 => 513,	name => 'ph_pol' },
{ gvid0 => 448,	name => 'jip_test' },
{ gvid0 => 409,	name => 'fog_dima' },
{ gvid0 => 344,	name => 'move_zmey' },
{ gvid0 => 279,	name => 'jim_blocks' },
{ gvid0 => 274,	name => 'tir3' },
{ gvid0 => 148,	name => 'military_game' },
{ gvid0 => 104,	name => 'script_test' },
{ gvid0 => 39,	name => 'move1' },
{ gvid0 => 0,	name => 'fog_dima1' },

);
sub name_by_gvid {
	foreach my $info (levels_info) {
		if ($_[0] >= $info->{gvid0}) {
			return $info->{name};
		}
	}
	return undef;
}

1;
