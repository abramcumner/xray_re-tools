package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 6456, },
{ gvid0 => 6172,	name => 'l04_darkvalley_ct' },
{ gvid0 => 6170,	name => 'plecha_selo' },
{ gvid0 => 5872,	name => 'k01_darkscape_ai' },
{ gvid0 => 5867,	name => 'l23_soznanie_ai' },
{ gvid0 => 5758,	name => 'agro_under' },
{ gvid0 => 5471,	name => 'l06_darkvalley_ai' },
{ gvid0 => 5453,	name => 'l08u_brainlab_ai' },
{ gvid0 => 5447,	name => 'wik_selo' },
{ gvid0 => 5372,	name => 'l12_deadcity_ai' },
{ gvid0 => 5366,	name => 'selo3' },
{ gvid0 => 5357,	name => 'chugai_test2' },
{ gvid0 => 5348,	name => 'chugai_test1' },
{ gvid0 => 5344,	name => 'l24_sarcofag_mental' },
{ gvid0 => 5181,	name => 'k01_darkscape_ct' },
{ gvid0 => 5122,	name => 'l04u_labx18_test' },
{ gvid0 => 5061,	name => 'l04u_labx18_oles' },
{ gvid0 => 5053,	name => 'vovan_test' },
{ gvid0 => 5047,	name => 'selo2' },
{ gvid0 => 4897,	name => 'l18_pripyat_ai' },
{ gvid0 => 4770,	name => 'l12_stancia_1ch_proba' },
{ gvid0 => 4721,	name => 'l12u_control_monolith_ai' },
{ gvid0 => 4657,	name => 'l12u_sarcofag_ai' },
{ gvid0 => 4388,	name => 'l12_stancia_2ch_proba' },
{ gvid0 => 4259,	name => 'l17_radar_underground_ai' },
{ gvid0 => 4140,	name => 'l07_military_proba' },
{ gvid0 => 4035,	name => 'l05_rostok' },
{ gvid0 => 3930,	name => 'l05_rostok_ai' },
{ gvid0 => 3871,	name => 'l04u_labx18_ai' },
{ gvid0 => 3855,	name => 'l03u_agroprom_ai' },
{ gvid0 => 3844,	name => 'ivan_test' },
{ gvid0 => 3839,	name => 'chugai_ai_test' },
{ gvid0 => 3833,	name => 'slipch_selo' },
{ gvid0 => 3813,	name => 'zmey_test2' },
{ gvid0 => 3802,	name => 'zmey_selo2' },
{ gvid0 => 3793,	name => 'krondor_selo' },
{ gvid0 => 3749,	name => 'slipch_al_test2' },
{ gvid0 => 3710,	name => 'slipch_al_test' },
{ gvid0 => 3704,	name => '$debug$\y_selo' },
{ gvid0 => 3693,	name => 'chugai_test' },
{ gvid0 => 3688,	name => 'jim_dark' },
{ gvid0 => 3645,	name => 'ai_test2' },
{ gvid0 => 3601,	name => 'ai_test' },
{ gvid0 => 3595,	name => 'stohe_selo' },
{ gvid0 => 3304,	name => 'jim_agro' },
{ gvid0 => 3299,	name => 'monster_tests' },
{ gvid0 => 3285,	name => 'l23_final_battle' },
{ gvid0 => 3154,	name => 'l10u_bunker' },
{ gvid0 => 2883,	name => 'l12_stancia_2' },
{ gvid0 => 2581,	name => 'k01_darkscape' },
{ gvid0 => 2531,	name => 'l12u_control_monolith' },
{ gvid0 => 2465,	name => 'l12u_sarcofag' },
{ gvid0 => 2336,	name => 'l12_stancia' },
{ gvid0 => 2182,	name => 'l11_pripyat' },
{ gvid0 => 1928,	name => 'l10_radar' },
{ gvid0 => 1849,	name => 'l09_deadcity' },
{ gvid0 => 1530,	name => 'l07_military' },
{ gvid0 => 1512,	name => 'l08u_brainlab' },
{ gvid0 => 1419,	name => 'l08_yantar' },
{ gvid0 => 1289,	name => 'l06_rostok' },
{ gvid0 => 1168,	name => 'l05_bar' },
{ gvid0 => 1107,	name => 'l04u_labx18' },
{ gvid0 => 817,	name => 'l04_darkvalley' },
{ gvid0 => 707,	name => 'l03u_agr_underground' },
{ gvid0 => 417,	name => 'l03_agroprom' },
{ gvid0 => 253,	name => 'l02_garbage' },
{ gvid0 => 0,	name => 'l01_escape' },
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
