package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 576, },
{ gvid0 => 491,		name => 'gra_alife' },
{ gvid0 => 406,		name => 'gra' },
{ gvid0 => 243,		name => 'stancia2' },
{ gvid0 => 167,		name => 'level2_temp_dm' },
{ gvid0 => 8,		name => 'level2_all_4_fin' },
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
