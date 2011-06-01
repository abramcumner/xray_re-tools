package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 6597, },
	{ gvid0 => 6488,		name => 'l03u_agr_underground_e3' },
	{ gvid0 => 6198,		name => 'l03_agroprom_e3' },
	{ gvid0 => 6186,		name => 'stason_test' },
	{ gvid0 => 6158,		name => 'ai_test_new' },
	{ gvid0 => 5874,		name => 'l04_darkvalley_ct' },
	{ gvid0 => 5872,		name => 'plecha_selo' },
	{ gvid0 => 5867,		name => 'l23_soznanie_ai' },
	{ gvid0 => 5758,		name => 'agro_under' },
	{ gvid0 => 5464,		name => 'l06_darkvalley_ai' },
	{ gvid0 => 5458,		name => 'wik_selo' },
	{ gvid0 => 5383,		name => 'l12_deadcity_ai' },
	{ gvid0 => 5377,		name => 'selo3' },
	{ gvid0 => 5368,		name => 'chugai_test2' },
	{ gvid0 => 5359,		name => 'chugai_test1' },
	{ gvid0 => 5355,		name => 'l24_sarcofag_mental' },
	{ gvid0 => 5192,		name => 'k01_darkscape_ct' },
	{ gvid0 => 5133,		name => 'l04u_labx18_test' },
	{ gvid0 => 5072,		name => 'l04u_labx18_oles' },
	{ gvid0 => 5064,		name => 'vovan_test' },
	{ gvid0 => 5058,		name => 'selo2' },
	{ gvid0 => 4906,		name => 'l18_pripyat_ai' },
	{ gvid0 => 4779,		name => 'l12_stancia_1ch_proba' },
	{ gvid0 => 4730,		name => 'l12u_control_monolith_ai' },
	{ gvid0 => 4666,		name => 'l12u_sarcofag_ai' },
	{ gvid0 => 4397,		name => 'l12_stancia_2ch_proba' },
	{ gvid0 => 4268,		name => 'l17_radar_underground_ai' },
	{ gvid0 => 4149,		name => 'l07_military_proba' },
	{ gvid0 => 4044,		name => 'l05_rostok' },
	{ gvid0 => 3939,		name => 'l05_rostok_ai' },
	{ gvid0 => 3880,		name => 'l04u_labx18_ai' },
	{ gvid0 => 3864,		name => 'l03u_agroprom_ai' },
	{ gvid0 => 3853,		name => 'ivan_test' },
	{ gvid0 => 3848,		name => 'chugai_ai_test' },
	{ gvid0 => 3842,		name => 'slipch_selo' },
	{ gvid0 => 3822,		name => 'zmey_test2' },
	{ gvid0 => 3811,		name => 'zmey_selo2' },
	{ gvid0 => 3802,		name => 'krondor_selo' },
	{ gvid0 => 3758,		name => 'slipch_al_test2' },
	{ gvid0 => 3719,		name => 'slipch_al_test' },
	{ gvid0 => 3713,		name => '$debug$\y_selo' },
	{ gvid0 => 3702,		name => 'chugai_test' },
	{ gvid0 => 3697,		name => 'jim_dark' },
	{ gvid0 => 3654,		name => 'ai_test2' },
	{ gvid0 => 3610,		name => 'ai_test' },
	{ gvid0 => 3604,		name => 'stohe_selo' },
	{ gvid0 => 3313,		name => 'jim_agro' },
	{ gvid0 => 3308,		name => 'monster_tests' },
	{ gvid0 => 3294,		name => 'l23_final_battle' },
	{ gvid0 => 3163,		name => 'l10u_bunker' },
	{ gvid0 => 2892,		name => 'l12_stancia_2' },
	{ gvid0 => 2590,		name => 'k01_darkscape' },
	{ gvid0 => 2540,		name => 'l12u_control_monolith' },
	{ gvid0 => 2474,		name => 'l12u_sarcofag' },
	{ gvid0 => 2345,		name => 'l12_stancia' },
	{ gvid0 => 2189,		name => 'l11_pripyat' },
	{ gvid0 => 1935,		name => 'l10_radar' },
	{ gvid0 => 1856,		name => 'l09_deadcity' },
	{ gvid0 => 1537,		name => 'l07_military' },
	{ gvid0 => 1519,		name => 'l08u_brainlab' },
	{ gvid0 => 1426,		name => 'l08_yantar' },
	{ gvid0 => 1296,		name => 'l06_rostok' },
	{ gvid0 => 1174,		name => 'l05_bar' },
	{ gvid0 => 1113,		name => 'l04u_labx18' },
	{ gvid0 => 816,		name => 'l04_darkvalley' },
	{ gvid0 => 707,		name => 'l03u_agr_underground' },
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
