package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 762, },
{ gvid0 => 754,		name => 'fog_dima' },
{ gvid0 => 746,	name => 'fog_ph' },
{ gvid0 => 726,	name => 'vehicles' },
{ gvid0 => 528,	name => 'offline_test' },
{ gvid0 => 525,	name => 'fog_dandy' },
{ gvid0 => 481,	name => 'yantar_fin' },
{ gvid0 => 442,	name => 'fog_trader' },
{ gvid0 => 44,	name => 'chapaev_game' },
{ gvid0 => 0,	name => 'yantar_game' },
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
