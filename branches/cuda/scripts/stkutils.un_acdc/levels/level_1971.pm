package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 317, },
{ gvid0 => 311,	name => 'shyshkin_test' },
{ gvid0 => 303,	name => 'chugai_test2' },
{ gvid0 => 295,	name => 'chugai_test1' },
{ gvid0 => 289,	name => 'stohe_selo' },
{ gvid0 => 283,	name => 'vovan_test' },
{ gvid0 => 239,	name => 'slipch_al_test2' },
{ gvid0 => 195,	name => 'slipch_al_test' },
{ gvid0 => 189,	name => 'test_zombie' },
{ gvid0 => 183,	name => 'test_chimera' },
{ gvid0 => 177,	name => 'test_controller' },
{ gvid0 => 171,	name => 'test_poltergeist' },
{ gvid0 => 165,	name => 'test_pseudogiant' },
{ gvid0 => 159,	name => 'selo2' },
{ gvid0 => 148,	name => 'chugai_test' },
{ gvid0 => 142,	name => 'jon_test' },
{ gvid0 => 136,	name => 'test_pseudodog' },
{ gvid0 => 130,	name => 'test_flesh' },
{ gvid0 => 124,	name => 'test_dog' },
{ gvid0 => 118,	name => 'test_burer' },
{ gvid0 => 112,	name => 'test_boar' },
{ gvid0 => 106,	name => 'test_bloodsucker' },
{ gvid0 => 100,	name => 'jim_dark' },
{ gvid0 => 56,	name => 'ai_test2' },
{ gvid0 => 12,	name => 'ai_test' },
{ gvid0 => 6,	name => 'zmey_selo2' },
{ gvid0 => 0,	name => 'zmey_selo1' },
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
