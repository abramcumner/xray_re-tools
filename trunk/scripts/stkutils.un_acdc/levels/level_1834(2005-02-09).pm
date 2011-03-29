package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 1303, },
{ gvid0 => 1237,	name => 'ph_pol' },
{ gvid0 => 1231,	name => 'military_rolik' },
{ gvid0 => 959,	name => 'military_experiments' },
{ gvid0 => 953,	name => 'military_sound' },
{ gvid0 => 947,	name => 'military_script1-1' },
{ gvid0 => 941,	name => 'military_script5-2' },
{ gvid0 => 935,	name => 'military_script5-1' },
{ gvid0 => 929,	name => 'military_script4-2' },
{ gvid0 => 923,	name => 'military_script4-1' },
{ gvid0 => 917,	name => 'military_script3' },
{ gvid0 => 911,	name => 'military_script1' },
{ gvid0 => 846,	name => 'jip_test' },
{ gvid0 => 565,	name => 'military_zmey' },
{ gvid0 => 500,	name => 'move_zmey' },
{ gvid0 => 435,	name => 'jim_blocks' },
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
