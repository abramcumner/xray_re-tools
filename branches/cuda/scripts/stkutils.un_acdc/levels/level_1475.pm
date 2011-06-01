package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 406, },
	{ gvid0 => 385,		name => 'Rostok_offline' },
	{ gvid0 => 351,		name => 'offline_zavod' },
	{ gvid0 => 316,		name => 'jim_ter' },
	{ gvid0 => 295,		name => 'l07_rostok_upg' },
	{ gvid0 => 252,		name => 'offline_test' },
	{ gvid0 => 98,		name => 'labaratory_1' },
	{ gvid0 => 93,		name => 'ai_test' },
	{ gvid0 => 8,		name => 'gra' },
	{ gvid0 => 0,		name => 'occ_part' },
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
