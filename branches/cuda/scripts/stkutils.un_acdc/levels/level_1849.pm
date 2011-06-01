package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 1902, },
{ gvid0 => 1836,	name => 'ph_pol' },
{ gvid0 => 1830,	name => 'military_rolik' },
{ gvid0 => 1725,	name => 'military_game_naparniki' },
{ gvid0 => 1620,	name => 'military_game_prof' },
{ gvid0 => 1515,	name => 'military_game_dead' },
{ gvid0 => 1450,	name => 'dandy_test' },
{ gvid0 => 1444,	name => 'military_script11' },
{ gvid0 => 1438,	name => 'military_script2-1' },
{ gvid0 => 1435,	name => 'y_light' },
{ gvid0 => 1163,	name => 'military_temp' },
{ gvid0 => 1131,	name => 'jim_script' },
{ gvid0 => 1125,	name => 'military_script4-3' },
{ gvid0 => 1060,	name => 'move_zmey2' },
{ gvid0 => 788,	name => 'military_experiments' },
{ gvid0 => 782,	name => 'military_sound' },
{ gvid0 => 776,	name => 'military_script1-1' },
{ gvid0 => 770,	name => 'military_script5-2' },
{ gvid0 => 764,	name => 'military_script5-1' },
{ gvid0 => 758,	name => 'military_script4-2' },
{ gvid0 => 752,	name => 'military_script4-1' },
{ gvid0 => 746,	name => 'military_script3' },
{ gvid0 => 740,	name => 'military_script1' },
{ gvid0 => 675,	name => 'jip_test' },
{ gvid0 => 394,	name => 'military_zmey' },
{ gvid0 => 329,	name => 'move_zmey' },
{ gvid0 => 264,	name => 'jim_blocks' },
{ gvid0 => 259,	name => 'tir3' },
{ gvid0 => 154,	name => 'military_game' },
{ gvid0 => 110,	name => 'script_test' },
{ gvid0 => 45,	name => 'move1' },
{ gvid0 => 39,	name => 'military_script' },
{ gvid0 => 0,	name => 'fog_dima' },
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
