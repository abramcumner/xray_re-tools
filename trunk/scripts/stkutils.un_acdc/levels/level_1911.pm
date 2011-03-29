package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 1225, },
{ gvid0 => 939,	name => 'l05_bar_rostok' },
{ gvid0 => 933,	name => 'test_effector' },
{ gvid0 => 927,	name => 'test_zombie' },
{ gvid0 => 921,	name => 'test_chimera' },
{ gvid0 => 915,	name => 'test_controller' },
{ gvid0 => 909,	name => 'test_poltergeist' },
{ gvid0 => 903,	name => 'test_pseudogiant' },
{ gvid0 => 629,	name => 'l03_agroprom' },
{ gvid0 => 386,	name => 'l02_garbage' },
{ gvid0 => 98,	name => 'l01_escape' },
{ gvid0 => 92,	name => 'selo' },
{ gvid0 => 86,	name => 'jon_test' },
{ gvid0 => 80,	name => 'test_pseudodog' },
{ gvid0 => 74,	name => 'test_flesh' },
{ gvid0 => 68,	name => 'test_dog' },
{ gvid0 => 62,	name => 'test_burer' },
{ gvid0 => 56,	name => 'test_boar' },
{ gvid0 => 50,	name => 'test_bloodsucker' },
{ gvid0 => 44,	name => 'jim_dark' },
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
