package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 1517, },
{ gvid0 => 1451,	name => 'ph_pol' },
{ gvid0 => 1445,	name => 'military_rolik' },
{ gvid0 => 1439,	name => 'military_script11' },
{ gvid0 => 1433,	name => 'military_script2-1' },
{ gvid0 => 1430,	name => 'y_light' },
{ gvid0 => 1158,	name => 'military_temp' },
{ gvid0 => 1126,	name => 'jim_script' },
{ gvid0 => 1120,	name => 'military_script4-3' },
{ gvid0 => 1055,	name => 'move_zmey2' },
{ gvid0 => 783,	name => 'military_experiments' },
{ gvid0 => 777,	name => 'military_sound' },
{ gvid0 => 771,	name => 'military_script1-1' },
{ gvid0 => 765,	name => 'military_script5-2' },
{ gvid0 => 759,	name => 'military_script5-1' },
{ gvid0 => 753,	name => 'military_script4-2' },
{ gvid0 => 747,	name => 'military_script4-1' },
{ gvid0 => 741,	name => 'military_script3' },
{ gvid0 => 735,	name => 'military_script1' },
{ gvid0 => 670,	name => 'jip_test' },
{ gvid0 => 389,	name => 'military_zmey' },
{ gvid0 => 324,	name => 'move_zmey' },
{ gvid0 => 259,	name => 'jim_blocks' },
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
