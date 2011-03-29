package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 4000, },
	{ gvid0 => 3737,		name => 'l03u_agr_underground_hw' },
	{ gvid0 => 3449,		name => 'l03_agroprom_hw' },
	{ gvid0 => 3298,		name => 'l05_bar_hw' },
	{ gvid0 => 2981,		name => 'l07_military_test' },
	{ gvid0 => 2976,		name => 'koan_test' },
	{ gvid0 => 2948,		name => 'ai_test_new' },
	{ gvid0 => 2946,		name => 'plecha_selo' },
	{ gvid0 => 2930,		name => 'l08u_brainlab_ai' },
	{ gvid0 => 2891,		name => 'slipch_al_test' },
	{ gvid0 => 2886,		name => 'jim_dark' },
	{ gvid0 => 2842,		name => 'ai_test2' },
	{ gvid0 => 2798,		name => 'ai_test' },
	{ gvid0 => 2792,		name => 'stohe_selo' },
	{ gvid0 => 2661,	name => 'l10u_bunker' },
	{ gvid0 => 2517,	name => 'l12_stancia_2' },
	{ gvid0 => 2467,	name => 'l12u_control_monolith' },
	{ gvid0 => 2402,	name => 'l12u_sarcofag' },
	{ gvid0 => 2273,	name => 'l12_stancia' },
	{ gvid0 => 2117,	name => 'l11_pripyat' },
	{ gvid0 => 1862,	name => 'l10_radar' },
	{ gvid0 => 1545,	name => 'l07_military' },
	{ gvid0 => 1529,	name => 'l08u_brainlab' },
	{ gvid0 => 1438,	name => 'l08_yantar' },
	{ gvid0 => 1308,	name => 'l06_rostok' },
	{ gvid0 => 1168,	name => 'l05_bar' },
	{ gvid0 => 1109,	name => 'l04u_labx18' },
	{ gvid0 => 811,		name => 'l04_darkvalley' },
	{ gvid0 => 703,		name => 'l03u_agr_underground' },
	{ gvid0 => 416,		name => 'l03_agroprom' },
	{ gvid0 => 252,		name => 'l02_garbage' },
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
