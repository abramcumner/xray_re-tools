package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 1620, },
	{ gvid0 => 1573,	name => 'hospital' },
	{ gvid0 => 1565,	name => 'stancia2' },
	{ gvid0 => 1524,	name => 'limansk' },
	{ gvid0 => 1511,	name => 'agroprom_undeground' },
	{ gvid0 => 1313,	name => 'military' },
	{ gvid0 => 1212,	name => 'yantar' },
	{ gvid0 => 1057,	name => 'agroprom' },
	{ gvid0 => 961,		name => 'red_forest' },
	{ gvid0 => 743,		name => 'darkvalley' },
	{ gvid0 => 472,		name => 'escape' },
	{ gvid0 => 245,		name => 'garbage' },
	{ gvid0 => 0,		name => 'marsh' },
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
