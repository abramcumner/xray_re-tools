package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
#clear sky
use constant levels_info => (
	{ gvid0 => 1620, },
	{ gvid0 => 1573,	name => 'l12_hospital' },
	{ gvid0 => 1565,	name => 'l11_stancia2' },
	{ gvid0 => 1524,	name => 'l10_limansk' },
	{ gvid0 => 1511,	name => 'l09_agroprom_undeground' },
	{ gvid0 => 1313,	name => 'l08_military' },
	{ gvid0 => 1212,	name => 'l07_yantar' },
	{ gvid0 => 1057,	name => 'l06_agroprom' },
	{ gvid0 => 961,		name => 'l05_red_forest' },
	{ gvid0 => 743,		name => 'l04_darkvalley' },
	{ gvid0 => 472,		name => 'l03_escape' },
	{ gvid0 => 245,		name => 'l02_garbage' },
	{ gvid0 => 0,		name => 'l01_marsh' },
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
