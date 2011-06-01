package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 248, },
{ gvid0 => 214,	name => 'move1' },
{ gvid0 => 172,	name => 'l02_garbage' },
{ gvid0 => 107,	name => 'script_test' },
{ gvid0 => 73,	name => 'military_rolik' },
{ gvid0 => 39,	name => 'military_script' },
{ gvid0 => 0,	name => 'fog_dima' },
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
