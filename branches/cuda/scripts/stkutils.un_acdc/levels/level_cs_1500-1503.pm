package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 1620, },
	{ gvid0 => 1612,		name => 'stancia_2' },
	{ gvid0 => 1565,		name => 'hospital' },
	{ gvid0 => 1524,		name => 'limansk' },
	{ gvid0 => 1326,		name => 'military' },
	{ gvid0 => 1230,		name => 'red_forest' },
	{ gvid0 => 1129,		name => 'yantar' },
	{ gvid0 => 1116,		name => 'agroprom_underground' },
	{ gvid0 => 961,		name => 'agroprom' },
	{ gvid0 => 743,		name => 'darkvalley' },
	{ gvid0 => 516,		name => 'garbage' },
	{ gvid0 => 245,		name => 'escape' },
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
