package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 243, },
	{ gvid0 => 238,		name => 'copy_dx9_workshop' },
	{ gvid0 => 232,		name => 'vovan_test' },
	{ gvid0 => 188,		name => 'slipch_al_test2' },
	{ gvid0 => 144,		name => 'slipch_al_test' },
	{ gvid0 => 100,		name => 'dandy_ai_test2' },
	{ gvid0 => 94,		name => 'jon_test' },
	{ gvid0 => 88,		name => 'jim_dark' },
	{ gvid0 => 44,		name => 'ai_test2' },
	{ gvid0 => 0,		name => 'ai_test' },
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
