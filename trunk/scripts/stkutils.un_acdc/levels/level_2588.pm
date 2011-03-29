package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 3734, },
	{ gvid0 => 3604,		name => 'l06_rostok_demo' },
	{ gvid0 => 3485,		name => 'l07_military_test' },
	{ gvid0 => 3480,		name => 'koan_test' },
	{ gvid0 => 3468,		name => 'stason_test' },
	{ gvid0 => 3440,		name => 'ai_test_new' },
	{ gvid0 => 3438,		name => 'plecha_selo' },
	{ gvid0 => 3410,		name => 'doc_test' },
	{ gvid0 => 3116,		name => 'l06_darkvalley_ai' },
	{ gvid0 => 3107,		name => 'chugai_test2' },
	{ gvid0 => 3098,		name => 'chugai_test1' },
	{ gvid0 => 3038,		name => 'l04u_labx18_test' },
	{ gvid0 => 3030,		name => 'vovan_test' },
	{ gvid0 => 3024,		name => 'selo2' },
	{ gvid0 => 3019,		name => 'chugai_ai_test' },
	{ gvid0 => 3010,		name => 'krondor_selo' },
	{ gvid0 => 2966,		name => 'slipch_al_test2' },
	{ gvid0 => 2927,		name => 'slipch_al_test' },
	{ gvid0 => 2921,		name => '$debug$\y_selo' },
	{ gvid0 => 2910,		name => 'chugai_test' },
	{ gvid0 => 2905,		name => 'jim_dark' },
	{ gvid0 => 2862,		name => 'ai_test2' },
	{ gvid0 => 2818,		name => 'ai_test' },
	{ gvid0 => 2812,		name => 'stohe_selo' },
	{ gvid0 => 2681,		name => 'l10u_bunker' },
	{ gvid0 => 2533,		name => 'l12_stancia_2' },
	{ gvid0 => 2484,		name => 'l12u_control_monolith' },
	{ gvid0 => 2419,		name => 'l12u_sarcofag' },
	{ gvid0 => 2290,		name => 'l12_stancia' },
	{ gvid0 => 2134,		name => 'l11_pripyat' },
	{ gvid0 => 1880,		name => 'l10_radar' },
	{ gvid0 => 1563,		name => 'l07_military' },
	{ gvid0 => 1545,		name => 'l08u_brainlab' },
	{ gvid0 => 1452,		name => 'l08_yantar' },
	{ gvid0 => 1322,		name => 'l06_rostok' },
	{ gvid0 => 1171,		name => 'l05_bar' },
	{ gvid0 => 1111,		name => 'l04u_labx18' },
	{ gvid0 => 814,		name => 'l04_darkvalley' },
	{ gvid0 => 417,		name => 'l03_agroprom' },
	{ gvid0 => 253,		name => 'l02_garbage' },
	{ gvid0 => 0,		name => 'l01_escape' },
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
