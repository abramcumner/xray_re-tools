package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 460, },
{ gvid0 => 439,		name => 'Rostok_offline' },
{ gvid0 => 405,		name => 'offline_zavod' },
{ gvid0 => 370,		name => 'jim_ter' },
{ gvid0 => 338,		name => 'present_level2' },
{ gvid0 => 295,		name => 'offline_test' },
{ gvid0 => 141,		name => 'labaratory_1' },
{ gvid0 => 136,		name => 'ai_test' },
{ gvid0 => 51,		name => 'gra' },
{ gvid0 => 8,		name => 'offline_test_dima' },
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
