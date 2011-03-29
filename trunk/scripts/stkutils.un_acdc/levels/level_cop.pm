package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 934, },
	{ gvid0 => 883,		name => 'jupiter_underground' },
	{ gvid0 => 857,		name => 'labx8' },
	{ gvid0 => 666,		name => 'pripyat' },
	{ gvid0 => 317,		name => 'jupiter' },
	{ gvid0 => 0,		name => 'zaton' },
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
