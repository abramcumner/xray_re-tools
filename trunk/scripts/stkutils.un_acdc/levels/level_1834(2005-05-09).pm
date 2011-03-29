package stkutils::level;

use strict;
use vars qw(@ISA @EXPORT_OK);
require Exporter;

@ISA		= qw(Exporter);
@EXPORT_OK	= qw(name_by_gvid);

# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 4060, },
{ gvid0 => 3754,	name => 'escape_prof_' },
{ gvid0 => 3448,	name => 'escape_free' },
{ gvid0 => 3442,	name => 'selo' },
{ gvid0 => 3398,	name => 'stohe_test' },
{ gvid0 => 3354,	name => 'chugai_test' },
{ gvid0 => 3310,	name => 'jon_test' },
{ gvid0 => 3266,	name => 'test_pdog' },
{ gvid0 => 3222,	name => 'test_flesh' },
{ gvid0 => 321,	name => 'move_zmey' },
{ gvid0 => 3178,	name => 'test_dog' },
{ gvid0 => 3134,	name => 'test_burer' },
{ gvid0 => 3090,	name => 'test_boar' },
{ gvid0 => 3046,	name => 'test_bloodsucker' },
{ gvid0 => 2974,	name => 'andy_script' },
{ gvid0 => 2902,	name => 'vovan_test' },
{ gvid0 => 2797,	name => 'military_game3' },
{ gvid0 => 2692,	name => 'military_game2' },
{ gvid0 => 2620,	name => 'jim_chimera' },
{ gvid0 => 2548,	name => 'jim_controller' },
{ gvid0 => 2476,	name => 'jim_burer' },
{ gvid0 => 2404,	name => 'jim_gigant' },
{ gvid0 => 2360,	name => 'ai_test2' },
{ gvid0 => 2316,	name => 'ai_test' },
{ gvid0 => 2282,	name => 'podval_np_04_r2' },
{ gvid0 => 2238,	name => 'jim_script_inter' },
{ gvid0 => 1932,	name => 'escape1' },
{ gvid0 => 1868,	name => 'jim_new_ai' },
{ gvid0 => 1590,	name => 'agroprom' },
{ gvid0 => 1525,	name => 'dandy_test' },
{ gvid0 => 1401,	name => 'military_ph_demo' },
{ gvid0 => 1329,	name => 'jim_script' },
{ gvid0 => 1264,	name => 'move_zmey2' },
{ gvid0 => 1198,	name => 'ph_pol' },
{ gvid0 => 892,	name => 'escape_prof' },
{ gvid0 => 614,	name => 'agroprom_prof' },
{ gvid0 => 490,	name => 'military_prof' },
{ gvid0 => 425,	name => 'jip_test' },
{ gvid0 => 386,	name => 'fog_dima' },
{ gvid0 => 256,	name => 'jim_blocks' },
{ gvid0 => 251,	name => 'tir3' },
{ gvid0 => 127,	name => 'military_game' },
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
