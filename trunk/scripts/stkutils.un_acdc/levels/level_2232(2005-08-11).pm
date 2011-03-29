package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 4405, },
{ gvid0 => 4389,	name => 'l03u_agroprom_ai' },
{ gvid0 => 4383,	name => 'new_scripter_selo' },
{ gvid0 => 4293,	name => 'l05_bar_ai2' },
{ gvid0 => 4283,	name => 'pryp_test_ai' },
{ gvid0 => 4224,	name => 'l04u_labx18_test' },
{ gvid0 => 3931,	name => 'l03_agroprom_ai1' },
{ gvid0 => 3803,	name => 'l12_stancia_1ch_proba' },
{ gvid0 => 3695,	name => 'jim_rostok' },
{ gvid0 => 3404,	name => 'l03_agroprom_ai2' },
{ gvid0 => 3393,	name => 'ivan_test' },
{ gvid0 => 3388,	name => 'chugai_ai_test' },
{ gvid0 => 3382,	name => 'slipch_selo' },
{ gvid0 => 3370,	name => 'l02_garbage_ai' },
{ gvid0 => 3280,	name => 'l05_bar_ai' },
{ gvid0 => 3263,	name => 'testers_mp_factory_ai' },
{ gvid0 => 2914,	name => 'l07_military_dima' },
{ gvid0 => 2658,	name => 'l01_escape_ai' },
{ gvid0 => 2531,	name => 'l04_darkvalley_ai2' },
{ gvid0 => 2182,	name => 'l07_military_proba' },
{ gvid0 => 2074,	name => 'l05_rostok_ai' },
{ gvid0 => 2055,	name => 'zmey_test2' },
{ gvid0 => 2033,	name => 'zmey_test1' },
{ gvid0 => 1906,	name => 'l04_darkvalley_ai' },
{ gvid0 => 1895,	name => 'stohe_selo' },
{ gvid0 => 1884,	name => 'krondor_selo' },
{ gvid0 => 1876,	name => 'vovan_test' },
{ gvid0 => 1832,	name => 'slipch_al_test2' },
{ gvid0 => 1788,	name => 'slipch_al_test' },
{ gvid0 => 1439,	name => 'l07_military' },
{ gvid0 => 1331,	name => 'l06_rostok' },
{ gvid0 => 1241,	name => 'l05_bar' },
{ gvid0 => 1181,	name => 'l04u_labx18' },
{ gvid0 => 1049,	name => 'l04_darkvalley' },
{ gvid0 => 1027,	name => 'l03u_agr_underground' },
{ gvid0 => 737,	name => 'l03_agroprom' },
{ gvid0 => 713,	name => 'l02_garbage' },
{ gvid0 => 457,	name => 'l01_escape' },
{ gvid0 => 451,	name => 'selo3' },
{ gvid0 => 445,	name => 'selo2' },
{ gvid0 => 439,	name => '$debug$\y_selo' },
{ gvid0 => 428,	name => 'chugai_test' },
{ gvid0 => 422,	name => 'lestat_test' },
{ gvid0 => 418,	name => 'jim_dark' },
{ gvid0 => 374,	name => 'ai_test' },
{ gvid0 => 363,	name => 'zmey_selo2' },
{ gvid0 => 70,	name => 'l03_agroprom_ai' },
{ gvid0 => 11,	name => 'l04u_labx18_ai' },
{ gvid0 => 0,	name => '5_test' },
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
