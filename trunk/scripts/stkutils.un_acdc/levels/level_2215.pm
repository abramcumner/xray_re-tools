package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 3359, },
{ gvid0 => 3348,	name => '!weapons_edit' },
{ gvid0 => 3258,	name => 'l05_bar_ai' },
{ gvid0 => 3002,	name => 'zmey_escape' },
{ gvid0 => 2985,	name => 'testers_mp_factory_ai' },
{ gvid0 => 2984,	name => 'l04_darkvalley_ai2' },
{ gvid0 => 2634,	name => 'l07_military_proba' },
{ gvid0 => 2526,	name => 'l05_rostok_ai' },
{ gvid0 => 2505,	name => 'zmey_test2' },
{ gvid0 => 2485,	name => 'zmey_test1' },
{ gvid0 => 2064,	name => 'l04_darkvalley_ai' },
{ gvid0 => 2053,	name => 'stohe_selo' },
{ gvid0 => 2042,	name => 'krondor_selo' },
{ gvid0 => 2034,	name => 'vovan_test' },
{ gvid0 => 2027,	name => 'dx9_workshop' },
{ gvid0 => 1983,	name => 'slipch_al_test2' },
{ gvid0 => 1940,	name => 'slipch_al_test' },
{ gvid0 => 1879,	name => 'l10_radar' },
{ gvid0 => 1850,	name => 'l08_deadcity' },
{ gvid0 => 1514,	name => 'l07_military' },
{ gvid0 => 1406,	name => 'l06_rostok' },
{ gvid0 => 1316,	name => 'l05_bar' },
{ gvid0 => 1256,	name => 'l04u_labx18' },
{ gvid0 => 838,	name => 'l04_darkvalley' },
{ gvid0 => 795,	name => 'l03u_agr_underground' },
{ gvid0 => 502,	name => 'l03_agroprom' },
{ gvid0 => 468,	name => 'l02_garbage' },
{ gvid0 => 212,	name => 'l01_escape' },
{ gvid0 => 206,	name => 'selo3' },
{ gvid0 => 200,	name => 'selo2' },
{ gvid0 => 189,	name => 'chugai_test' },
{ gvid0 => 168,	name => 'lestat_test' },
{ gvid0 => 163,	name => 'jim_dark' },
{ gvid0 => 119,	name => 'ai_test' },
{ gvid0 => 59,	name => 'l10_radar_ai' },
{ gvid0 => 0,	name => 'l04u_labx18_ai' },
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
