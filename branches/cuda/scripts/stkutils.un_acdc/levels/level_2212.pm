package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 1525, },
{ gvid0 => 1481,	name => 'andy_test_2' },
{ gvid0 => 1475,	name => 'shyshkin_test' },
{ gvid0 => 1474,	name => 'chugai_test2' },
{ gvid0 => 1471,	name => 'chugai_test1' },
{ gvid0 => 1465,	name => 'stohe_selo' },
{ gvid0 => 1459,	name => 'vovan_test' },
{ gvid0 => 1415,	name => 'slipch_al_test2' },
{ gvid0 => 1371,	name => 'slipch_al_test' },
{ gvid0 => 1365,	name => 'test_zombie' },
{ gvid0 => 1359,	name => 'test_chimera' },
{ gvid0 => 1353,	name => 'test_controller' },
{ gvid0 => 1347,	name => 'test_poltergeist' },
{ gvid0 => 1341,	name => 'test_pseudogiant' },
{ gvid0 => 1306,	name => 'l08_deadcity' },
{ gvid0 => 1166,	name => 'l05_bar_rostok' },
{ gvid0 => 1049,	name => 'l04u_darklab' },
{ gvid0 => 1016,	name => 'l04_darkvale' },
{ gvid0 => 701,	name => 'l03_agroprom' },
{ gvid0 => 459,	name => 'l02_garbage' },
{ gvid0 => 179,	name => 'l01_escape' },
{ gvid0 => 173,	name => 'selo2' },
{ gvid0 => 167,	name => 'y_selo' },
{ gvid0 => 156,	name => 'chugai_test' },
{ gvid0 => 150,	name => 'jon_test' },
{ gvid0 => 144,	name => 'test_pseudodog' },
{ gvid0 => 138,	name => 'test_flesh' },
{ gvid0 => 132,	name => 'test_dog' },
{ gvid0 => 126,	name => 'test_burer' },
{ gvid0 => 120,	name => 'test_boar' },
{ gvid0 => 114,	name => 'test_bloodsucker' },
{ gvid0 => 108,	name => 'jim_dark' },
{ gvid0 => 64,	name => 'ai_test2' },
{ gvid0 => 20,	name => 'ai_test' },
{ gvid0 => 14,	name => 'dandy_test' },
{ gvid0 => 8,	name => 'zmey_selo2' },
{ gvid0 => 2,	name => 'zmey_selo1' },
{ gvid0 => 0,	name => '5_test' },

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
