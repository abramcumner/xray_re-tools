package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 2499, },
{ gvid0 => 2493,	name => 'test_script' },
{ gvid0 => 2487,	name => 'test_izlom' },
{ gvid0 => 2481,	name => 'test_zombie' },
{ gvid0 => 2475,	name => 'test_pseudogiant' },
{ gvid0 => 2405,	name => 'l13u_warlab' },
{ gvid0 => 2039,	name => 'l13_generators' },
{ gvid0 => 1940,	name => 'l12u_sarcofag' },
{ gvid0 => 1908,	name => 'l12_stancia' },
{ gvid0 => 1870,	name => 'l10u_secret_lab' },
{ gvid0 => 1804,	name => 'l10u_radar_bunker' },
{ gvid0 => 1710,	name => 'l10_radar' },
{ gvid0 => 1667,	name => 'l09_swamp' },
{ gvid0 => 1494,	name => 'l08_deadcity' },
{ gvid0 => 1372,	name => 'l07_military' },
{ gvid0 => 1328,	name => 'l06_yantar' },
{ gvid0 => 1187,	name => 'l05_bar_rostok' },
{ gvid0 => 1070,	name => 'l04u_darklab' },
{ gvid0 => 941,	name => 'l04_darkdolina' },
{ gvid0 => 667,	name => 'l03_agroprom' },
{ gvid0 => 424,	name => 'l02_garbage' },
{ gvid0 => 136,	name => 'l01_escape' },
{ gvid0 => 130,	name => 'selo' },
{ gvid0 => 124,	name => 'jon_test' },
{ gvid0 => 118,	name => 'test_pseudodog' },
{ gvid0 => 112,	name => 'test_flesh' },
{ gvid0 => 106,	name => 'test_dog' },
{ gvid0 => 100,	name => 'test_burer' },
{ gvid0 => 94,	name => 'test_boar' },
{ gvid0 => 88,	name => 'test_bloodsucker' },
{ gvid0 => 44,	name => 'ai_test2' },
{ gvid0 => 0,	name => 'ai_test' },

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
