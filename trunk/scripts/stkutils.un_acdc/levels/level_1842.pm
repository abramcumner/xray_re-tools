package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 4189, },
{ gvid0 => 3883,	name => 'escape_dk' },
{ gvid0 => 3648,	name => 'garbage_dk' },
{ gvid0 => 3372,	name => 'agroprom_dk' },
{ gvid0 => 3366,	name => 'selo2' },
{ gvid0 => 3350,	name => 'jim_lab' },
{ gvid0 => 3340,	name => 'jim_indoor' },
{ gvid0 => 3100,	name => 'garbage' },
{ gvid0 => 2865,	name => 'garbage_ai' },
{ gvid0 => 2863,	name => 'tir' },
{ gvid0 => 2857,	name => 'jim_pop' },
{ gvid0 => 2851,	name => 'y_light4' },
{ gvid0 => 2845,	name => 'selo' },
{ gvid0 => 2801,	name => 'stohe_test' },
{ gvid0 => 2757,	name => 'chugai_test' },
{ gvid0 => 2713,	name => 'jon_test' },
{ gvid0 => 2669,	name => 'test_pdog' },
{ gvid0 => 2625,	name => 'test_flesh' },
{ gvid0 => 2581,	name => 'test_dog' },
{ gvid0 => 2537,	name => 'test_burer' },
{ gvid0 => 2493,	name => 'test_boar' },
{ gvid0 => 2449,	name => 'test_bloodsucker' },
{ gvid0 => 2377,	name => 'andy_script' },
{ gvid0 => 2305,	name => 'vovan_test' },
{ gvid0 => 2233,	name => 'jim_chimera' },
{ gvid0 => 2161,	name => 'jim_controller' },
{ gvid0 => 2089,	name => 'jim_burer' },
{ gvid0 => 2017,	name => 'jim_gigant' },
{ gvid0 => 1973,	name => 'ai_test2' },
{ gvid0 => 1929,	name => 'ai_test' },
{ gvid0 => 1895,	name => 'podval_np_04_r2' },
{ gvid0 => 1851,	name => 'jim_script_inter' },
{ gvid0 => 1545,	name => 'escape1' },
{ gvid0 => 1481,	name => 'jim_new_ai' },
{ gvid0 => 1205,	name => 'agroprom' },
{ gvid0 => 1140,	name => 'dandy_test' },
{ gvid0 => 1016,	name => 'military_ph_demo' },
{ gvid0 => 944,	name => 'jim_script' },
{ gvid0 => 879,	name => 'move_zmey2' },
{ gvid0 => 813,	name => 'ph_pol' },
{ gvid0 => 507,	name => 'escape_prof' },
{ gvid0 => 231,	name => 'agroprom_prof' },
{ gvid0 => 166,	name => 'jip_test' },
{ gvid0 => 127,	name => 'fog_dima' },
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
