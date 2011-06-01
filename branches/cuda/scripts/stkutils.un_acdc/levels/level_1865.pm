package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 5732, },
{ gvid0 => 5682,	name => 'lvl05_bar_script' },
{ gvid0 => 5679,	name => 'jim_polter' },
{ gvid0 => 5668,	name => 't_bar' },
{ gvid0 => 5662,	name => 'gkhola_test' },
{ gvid0 => 5444,	name => 'lvl08_deadcity' },
{ gvid0 => 5322,	name => 'lvl07_military' },
{ gvid0 => 4910,	name => 'lvl06_yantar' },
{ gvid0 => 4881,	name => 'lvl05u_rostok' },
{ gvid0 => 4831,	name => 'lvl05_bar' },
{ gvid0 => 4714,	name => 'lvl04u_darklab' },
{ gvid0 => 4659,	name => 'lvl04_darkdolina' },
{ gvid0 => 4385,	name => 'lvl03_agroprom' },
{ gvid0 => 4142,	name => 'lvl02_garbage' },
{ gvid0 => 3850,	name => 'lvl01_escape' },
{ gvid0 => 3844,	name => 'selo3' },
{ gvid0 => 3538,	name => 'escape_dk' },
{ gvid0 => 3303,	name => 'garbage_dk' },
{ gvid0 => 3027,	name => 'agroprom_dk' },
{ gvid0 => 3021,	name => 'selo2' },
{ gvid0 => 3005,	name => 'jim_lab' },
{ gvid0 => 2995,	name => 'jim_indoor' },
{ gvid0 => 2755,	name => 'garbage' },
{ gvid0 => 2520,	name => 'garbage_ai' },
{ gvid0 => 2518,	name => 'tir' },
{ gvid0 => 2512,	name => 'jim_pop' },
{ gvid0 => 2506,	name => 'y_light4' },
{ gvid0 => 2500,	name => 'selo' },
{ gvid0 => 2456,	name => 'stohe_test' },
{ gvid0 => 2412,	name => 'chugai_test' },
{ gvid0 => 2368,	name => 'jon_test' },
{ gvid0 => 2324,	name => 'test_pdog' },
{ gvid0 => 2280,	name => 'test_flesh' },
{ gvid0 => 2236,	name => 'test_dog' },
{ gvid0 => 2192,	name => 'test_burer' },
{ gvid0 => 2148,	name => 'test_boar' },
{ gvid0 => 2104,	name => 'test_bloodsucker' },
{ gvid0 => 2032,	name => 'vovan_test' },
{ gvid0 => 1960,	name => 'jim_chimera' },
{ gvid0 => 1888,	name => 'jim_controller' },
{ gvid0 => 1816,	name => 'jim_burer' },
{ gvid0 => 1744,	name => 'jim_gigant' },
{ gvid0 => 1700,	name => 'ai_test2' },
{ gvid0 => 1656,	name => 'ai_test' },
{ gvid0 => 1622,	name => 'podval_np_04_r2' },
{ gvid0 => 1578,	name => 'jim_script_inter' },
{ gvid0 => 1286,	name => 'escape1' },
{ gvid0 => 1222,	name => 'jim_new_ai' },
{ gvid0 => 946,	name => 'agroprom' },
{ gvid0 => 881,	name => 'dandy_test' },
{ gvid0 => 757,	name => 'military_ph_demo' },
{ gvid0 => 685,	name => 'jim_script' },
{ gvid0 => 620,	name => 'move_zmey2' },
{ gvid0 => 554,	name => 'ph_pol' },
{ gvid0 => 278,	name => 'agroprom_prof' },
{ gvid0 => 213,	name => 'jip_test' },
{ gvid0 => 174,	name => 'fog_dima' },
{ gvid0 => 130,	name => 'move_zmey' },
{ gvid0 => 127,	name => 'mike_sicret_lab_tests' },
{ gvid0 => 83,	name => 'script_test' },
{ gvid0 => 39,	name => 'move1' },
{ gvid0 => 0,	name => 'fog_dima1' },

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
