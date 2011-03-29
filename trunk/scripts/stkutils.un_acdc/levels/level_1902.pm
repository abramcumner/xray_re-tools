package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 995, },
{ gvid0 => 989,	name => 'test_effector' },
{ gvid0 => 983,	name => 'test_izlom' },
{ gvid0 => 977,	name => 'test_zombie' },
{ gvid0 => 971,	name => 'test_chimera' },
{ gvid0 => 965,	name => 'test_controller' },
{ gvid0 => 959,	name => 'test_poltergeist' },
{ gvid0 => 953,	name => 'test_pseudogiant' },
{ gvid0 => 679,	name => 'l03_agroprom' },
{ gvid0 => 436,	name => 'l02_garbage' },
{ gvid0 => 148,	name => 'l01_escape' },
{ gvid0 => 142,	name => 'selo' },
{ gvid0 => 136,	name => 'chugai_test' },
{ gvid0 => 130,	name => 'jon_test' },
{ gvid0 => 124,	name => 'test_pseudodog' },
{ gvid0 => 118,	name => 'test_flesh' },
{ gvid0 => 112,	name => 'test_dog' },
{ gvid0 => 106,	name => 'test_burer' },
{ gvid0 => 100,	name => 'test_boar' },
{ gvid0 => 94,	name => 'test_bloodsucker' },
{ gvid0 => 88,	name => 'jim_dark' },
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
