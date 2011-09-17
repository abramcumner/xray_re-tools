#!perl -w -I \temp\1\bin
#
# last edited: 17 Sep 2011
#######################################################################
package cse_abstract;
use strict;
use stkutils::debug;
####	enum s_gameid
#use constant	GAME_ANY		=> 0;
#use constant	GAME_SINGLE		=> 0x01;
#use constant	GAME_DEATHMATCH	=> 0x02;
#use constant	GAME_CTF		=> 0x03;
#use constant	GAME_ASSAULT	=> 0x04;
#use constant	GAME_CS			=> 0x05;
#use constant	GAME_TEAMDEATHMATCH	=> 0x06;
#use constant	GAME_ARTEFACTHUNT	=> 0x07;
#use constant	GAME_LASTSTANDING	=> 0x64;
#use constant	GAME_DUMMY		=> 0xFF;
####	enum s_flags
#use constant	FL_SPAWN_ENABLED		=> 0x01;
#use constant	FL_SPAWN_ON_SURGE_ONLY		=> 0x02;
#use constant	FL_SPAWN_SINGLE_ITEM_ONLY	=> 0x04;
#use constant	FL_SPAWN_IF_DESTROYED_ONLY	=> 0x08;
#use constant	FL_SPAWN_INFINITE_COUNT		=> 0x10;
#use constant	FL_SPAWN_DESTROY_ON_SPAWN	=> 0x20;
use constant properties_info => (
			{ name => 'dummy16',				type => 'h16',	default => 0x0001 },
			{ name => 'section_name',			type => 'sz',	default => '' },	# 0x40
			{ name => 'name',					type => 'sz',	default => '' },	# 0x11c
			{ name => 's_gameid',				type => 'h8',	default => 0 },		# 0x44
			{ name => 's_rp',					type => 'h8',	default => 0xfe },	# 0x45
			{ name => 'position',				type => 'f32v3',default => [] },	# 0x58
			{ name => 'direction',				type => 'f32v3',default => [] },	# 0x64
			{ name => 'respawn_time',			type => 'h16',	default => 0 },		# 0x34
			{ name => 'unknown_id',				type => 'h16',	default => 0xffff },	# 0x36
			{ name => 'parent_id',				type => 'h16',	default => 0xffff },	# 0x38
			{ name => 'phantom_id',				type => 'h16',	default => 0xffff },	# 0x3a
			{ name => 's_flags',				type => 'h16',	default => 0x21 },	# 0x46
			{ name => 'version',				type => 'u16',	default => 109 },	# 0x30 
			{ name => 'cse_abstract__unk1_u16',	type => 'h16',	default => 0xFFFF },	# 0x30 
			{ name => 'script_version',			type => 'u16',	default => 2 },		# 0x32
			{ name => 'spawn_probability',		type => 'f32',	default => 1.0 },		# 0x34
			{ name => 'spawn_flags',			type => 'u32',	default => 31 },		# 0x34
			{ name => 'spawn_control',			type => 'sz',	default => '' },	# 0x36
			{ name => 'max_spawn_count',		type => 'u32',	default => 1 },	# 0x38
			{ name => 'spawn_count',			type => 'u32',	default => 0 },	# 0x3a
			{ name => 'last_spawn_time',		type => 'u8v8', default => [0,0,0,0,0,0,0,0] },	# 0x46
			{ name => 'min_spawn_interval',		type => 'u8v8', default => [0,0,0,0,0,0,0,0] },	# 0x30
			{ name => 'max_spawn_interval',		type => 'u8v8', default => [0,0,0,0,0,0,0,0] },		# 0x32
);
sub state_read {
	my $self = shift;
	my ($packet) = @_;
	$packet->unpack_properties($self, (properties_info)[0]);
	fail(__PACKAGE__.'::state_read', __LINE__, 'M_SPAWN == dummy16', 'cannot open M_SPAWN!') if $self->{'dummy16'} != 1;
	$packet->unpack_properties($self, (properties_info)[1..11]);
	if ($self->{s_flags} & 0x20) {
		$packet->unpack_properties($self, (properties_info)[12]);
	}
	if ($self->{version} > 120) {
		$packet->unpack_properties($self, (properties_info)[13]);
	}
	if ($self->{version} > 69) {
		$packet->unpack_properties($self, (properties_info)[14]);
	}
	my ($unused, $spawn_id);
	if (($self->{version} > 70) && ($self->{version} <= 79)) {
		$unused = $packet->unpack('C');
	} elsif (($self->{version} > 79) && ($self->{version} <= 93)) {
		($unused, $spawn_id) = $packet->unpack('Cv');
	} elsif ($self->{version} > 93) {
		($unused, $spawn_id) = $packet->unpack('vv');
	}
	if ($self->{version} < 112) {
		if ($self->{version} > 82) {
			$packet->unpack_properties($self, (properties_info)[15]);
		}
		if ($self->{version} > 83) {
			$packet->unpack_properties($self, (properties_info)[16..20]);
		}		
		if ($self->{version} > 84) {
			$packet->unpack_properties($self, (properties_info)[21..22]);
		}	
	}
	my $extended_size = $packet->unpack('v');
}
sub state_write {
	my $self = shift;
	my ($packet, $spawn_id, $extended_size) = @_;
	$packet->pack_properties($self, (properties_info)[0..11]);
	if ($self->{s_flags} >> 5 == 1) {
		$packet->pack_properties($self, (properties_info)[12]);
	}
	if ($self->{version} > 120) {
		$packet->pack_properties($self, (properties_info)[13]);
	}
	if ($self->{version} > 69) {
		$packet->pack_properties($self, (properties_info)[14]);
	}
	if (($self->{version} > 70) && ($self->{version} <= 79)) {
		$packet->pack('C', 0);
	} elsif (($self->{version} > 79) && ($self->{version} <= 93)) {
		$packet->pack('Cv', 0, $spawn_id);
	} elsif (($self->{version} > 93) && ($self->{version} <= 122)) {
		if (::level()) {
			$packet->pack('vv', 0, 0xffff);
		} else {
			$packet->pack('vv', 0, $spawn_id);
		}
	} elsif ($self->{version} > 122) {
		if (::level() && ($self->{section_name} eq 'graph_point')) { 
			$packet->pack('vv', 0, 0xcccc);
		} elsif (::level()) {
			$packet->pack('vv', 0, 0xffff);
		} else {
			$packet->pack('vv', 0, $spawn_id);
		}
	}
	if ($self->{version} < 112) {
		if ($self->{version} > 82) {
			$packet->pack_properties($self, (properties_info)[15]);
		}
		if ($self->{version} > 83) {
			$packet->pack_properties($self, (properties_info)[16]);
			$packet->pack_properties($self, (properties_info)[17]);
			$packet->pack_properties($self, (properties_info)[18]);
			$packet->pack_properties($self, (properties_info)[19]);
			$packet->pack_properties($self, (properties_info)[20]);
		}		
		if ($self->{version} > 84) {
			$packet->pack_properties($self, (properties_info)[21]);
			$packet->pack_properties($self, (properties_info)[22]);
		}	
	}
	if (::level()) {
		$packet->pack('v', $extended_size + 2);
	} else {
		$packet->pack('v', $extended_size);
	}
}
sub update_read {
	my ($size) = $_[1]->unpack('v');
	stkutils::debug::fail(__PACKAGE__.'::update_read', __LINE__, '$size == 0', 'unexpected size of CSE_Abstract M_UPDATE packet') unless $size == 0;
}
sub update_write {
	$_[1]->pack('v', 0);
}
sub state_import {
	$_[1]->import_properties($_[2], $_[0], (properties_info)[0..11]);
	if ($_[0]->{s_flags} >> 5 == 1) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[12]);
	}
	if ($_[0]->{version} > 120) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[13]);
	}
	if ($_[0]->{version} > 69) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[14]);
	}
	if ($_[0]->{version} < 112) {
		if ($_[0]->{version} > 82) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[15]);
		}
		if ($_[0]->{version} > 83) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[16]);
			$_[1]->import_properties($_[2], $_[0], (properties_info)[17]);
			$_[1]->import_properties($_[2], $_[0], (properties_info)[18]);
			$_[1]->import_properties($_[2], $_[0], (properties_info)[19]);
			$_[1]->import_properties($_[2], $_[0], (properties_info)[20]);
		}		
		if ($_[0]->{version} > 84) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[21]);
			$_[1]->import_properties($_[2], $_[0], (properties_info)[22]);
		}	
	}
}
sub state_export {
	$_[1]->export_properties('cse_abstract', $_[0], (properties_info)[0..11]);
	if ($_[0]->{s_flags} >> 5 == 1) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[12]);
	}
	if ($_[0]->{version} > 120) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[13]);
	}
	if ($_[0]->{version} > 69) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[14]);
	}
	if ($_[0]->{version} < 112) {
		if ($_[0]->{version} > 82) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[15]);
		}
		if ($_[0]->{version} > 83) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[16]);
			$_[1]->export_properties(undef, $_[0], (properties_info)[17]);
			$_[1]->export_properties(undef, $_[0], (properties_info)[18]);
			$_[1]->export_properties(undef, $_[0], (properties_info)[19]);
			$_[1]->export_properties(undef, $_[0], (properties_info)[20]);
		}		
		if ($_[0]->{version} > 84) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[21]);
			$_[1]->export_properties(undef, $_[0], (properties_info)[22]);
		}	
	}
}
#######################################################################
package cse_alife_graph_point;
use strict;
use constant properties_info => (
	{ name => 'connection_point_name',	type => 'sz',	default => '' },	# 0xa4
	{ name => 'connection_level_name',	type => 'sz',	default => '' },	# 0x100
	{ name => 'connection_level_name_old_format',	type => 's32',	default => -1 },	# 0x100
	{ name => 'location0',			type => 'u8',	default => 0 },		# 0xa8
	{ name => 'location1',			type => 'u8',	default => 0 },		# 0xa9
	{ name => 'location2',			type => 'u8',	default => 0 },		# 0xaa
	{ name => 'location3',			type => 'u8',	default => 0 },		# 0xab
);
sub state_read {
	$_[1]->unpack_properties($_[0], (properties_info)[0]);
	if ($_[0]->{version} > 33) {
		$_[1]->unpack_properties($_[0], (properties_info)[1]);
	} else {
		$_[1]->unpack_properties($_[0], (properties_info)[2]);
	}
	$_[1]->unpack_properties($_[0], (properties_info)[3..6]);
}
sub state_write {
	$_[1]->pack_properties($_[0], (properties_info)[0]);
	if ($_[0]->{version} > 33) {
		$_[1]->pack_properties($_[0], (properties_info)[1]);
	} else {
		$_[1]->pack_properties($_[0], (properties_info)[2]);
	}
	$_[1]->pack_properties($_[0], (properties_info)[3..6]);
}
sub state_import {
	$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
	if ($_[0]->{version} > 33) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);
	} else {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[2]);
	}
	$_[1]->import_properties($_[2], $_[0], (properties_info)[3..6]);
}
sub state_export {
	$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
	if ($_[0]->{version} > 33) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[1]);
	} else {
		$_[1]->export_properties(undef, $_[0], (properties_info)[2]);
	}
	$_[1]->export_properties(undef, $_[0], (properties_info)[3..6]);
}
#######################################################################
package cse_shape;
use strict;
use constant properties_info => (
	{ name => 'shapes', type => 'shape', default => {} },
);
sub state_read {
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}
#######################################################################
package cse_visual;
use strict;
use constant flObstacle	=> 0x01;
use constant properties_info => (
	{ name => 'visual_name',	type => 'sz',	default => '' },	# 0x04
	{ name => 'visual_flags',	type => 'h8',	default => 0 },		# 0x0c
);
sub state_read {
	$_[1]->unpack_properties($_[0], (properties_info)[0]);
	if ($_[0]->{version} >= 104) {
		$_[1]->unpack_properties($_[0], (properties_info)[1]);
	}
}
sub state_write {
	$_[1]->pack_properties($_[0], (properties_info)[0]);
	if ($_[0]->{version} >= 104) {
		$_[1]->pack_properties($_[0], (properties_info)[1]);
	}
}
sub state_import {
	$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
	if ($_[0]->{version} >= 104) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);
	}
}
sub state_export {
	$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
	if ($_[0]->{version} >= 104) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[1]);
	}
}
#######################################################################
package cse_alife_object_dummy;
use strict;
use constant properties_info => (
	{ name => 'cse_alife_object_dummy__unk1_u8',	type => 'u8',	default => 0 },	# 0x04
);
sub state_read {
	$_[1]->unpack_properties($_[0], properties_info);
	cse_visual::state_read(@_);
}
sub state_write {
	$_[1]->pack_properties($_[0], properties_info);
	cse_visual::state_write(@_);
}
sub state_import {
	$_[1]->import_properties($_[2], $_[0], properties_info);
	cse_visual::state_import(@_);
}
sub state_export {
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	cse_visual::state_export(@_);
}
#######################################################################
package cse_motion;
use strict;
use constant properties_info => (
	{ name => 'motion_name', type => 'sz', default => '' },		# 0x04
);
sub state_read {
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}
#######################################################################
package cse_ph_skeleton;
use strict;
use constant properties_info => (
	{ name => 'skeleton_name',	type => 'sz',	default => '$editor' },	# smthg|0x08
	{ name => 'skeleton_flags',	type => 'u8',	default => 0 },		# 0x08
	{ name => 'source_id',		type => 'h16',	default => 0xffff },	# 0x48
);
sub state_read {
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}
#######################################################################
package cse_alife_object;
use strict;
use constant flUseSwitches		=> 0x00000001;
use constant flSwitchOnline		=> 0x00000002;
use constant flSwitchOffline		=> 0x00000004;
use constant flInteractive		=> 0x00000008; 
use constant flVisibleForAI		=> 0x00000010;
use constant flUsefulForAI		=> 0x00000020;
use constant flOfflineNoMove		=> 0x00000040;
use constant flUsedAI_Locations		=> 0x00000080;
use constant flUseGroupBehaviour	=> 0x00000100;
use constant flCanSave			=> 0x00000200;
use constant flVisibleForMap		=> 0x00000400;
use constant flUseSmartTerrains		=> 0x00000800;
use constant flCheckForSeparator	=> 0x00001000;
use constant flCorpseRemoval		=> 0x00002000;
use constant properties_info => (
			{ name => 'cse_alife_object__unk1_u8',	type => 'u8'},	# 0xa4
			{ name => 'spawn_probability',	type => 'f32'},	# 0xa4
			{ name => 'spawn_id',	type => 's32'},	# 0xa4
			{ name => 'cse_alife_object__unk2_u16',	type => 'u16'},	# 0xa4		
			{ name => 'game_vertex_id',	type => 'u16',	default => 0xffff },	# 0xa4
			{ name => 'distance',		type => 'f32',	default => 0.0 },	# 0xa8
			{ name => 'direct_control',	type => 'u32',	default => 1 },		# 0xad
			{ name => 'level_vertex_id',	type => 'u32',	default => 0xffffffff },# 0xb0
			{ name => 'cse_alife_object__unk3_u16',	type => 'u16',	default => 0 },		# 0x34
			{ name => 'spawn_control',		type => 'sz',	default => '' },	# 0x36
			{ name => 'object_flags',	type => 'h32',	default => 0 },		# 0xb4
			{ name => 'custom_data',	type => 'sz',	default => ''},	
			{ name => 'story_id',		type => 's32',	default => -1 },	# 0xb8
			{ name => 'spawn_story_id',		type => 's32',	default => -1 },	# 0xb8
);
sub state_read {
	if ($_[0]->{version} >= 1) {
		if ($_[0]->{version} <= 24) {
			$_[1]->unpack_properties($_[0], (properties_info)[0]);
		} elsif (($_[0]->{version} > 24) && ($_[0]->{version} < 83)) {
			$_[1]->unpack_properties($_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} < 83) {
			$_[1]->unpack_properties($_[0], (properties_info)[2]);
		}
		if ($_[0]->{version} < 4) {
			$_[1]->unpack_properties($_[0], (properties_info)[3]);
		}
		$_[1]->unpack_properties($_[0], (properties_info)[4..5]);
	}
	if ($_[0]->{version} >= 4) {
		$_[1]->unpack_properties($_[0], (properties_info)[6]);
	}
	if ($_[0]->{version} >= 8) {
		$_[1]->unpack_properties($_[0], (properties_info)[7]);
	}
	if (($_[0]->{version} > 22) && ($_[0]->{version} <= 79)) {
		$_[1]->unpack_properties($_[0], (properties_info)[8]);
	}
	if (($_[0]->{version} > 23) && ($_[0]->{version} <= 84)) {
		$_[1]->unpack_properties($_[0], (properties_info)[9]);
	}
	if ($_[0]->{version} > 49) {
		$_[1]->unpack_properties($_[0], (properties_info)[10]);
	}
	if ($_[0]->{version} > 57) {
		$_[1]->unpack_properties($_[0], (properties_info)[11]);
	}
	if ($_[0]->{version} > 61) {
		$_[1]->unpack_properties($_[0], (properties_info)[12]);
	}
	if ($_[0]->{version} > 111) {
		$_[1]->unpack_properties($_[0], (properties_info)[13]);
	}
}
sub state_write {
	if ($_[0]->{version} >= 1) {
		if ($_[0]->{version} <= 24) {
			$_[1]->pack_properties($_[0], (properties_info)[0]);
		} elsif (($_[0]->{version} > 24) && ($_[0]->{version} < 83)) {
			$_[1]->pack_properties($_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} < 83) {
			$_[1]->pack_properties($_[0], (properties_info)[2]);
		}
		if ($_[0]->{version} < 4) {
			$_[1]->pack_properties($_[0], (properties_info)[3]);
		}
		$_[1]->pack_properties($_[0], (properties_info)[4..5]);
	}
	if ($_[0]->{version} >= 4) {
		$_[1]->pack_properties($_[0], (properties_info)[6]);
	}
	if ($_[0]->{version} >= 8) {
		$_[1]->pack_properties($_[0], (properties_info)[7]);
	}
	if (($_[0]->{version} > 22) && ($_[0]->{version} <= 79)) {
		$_[1]->pack_properties($_[0], (properties_info)[8]);
	}
	if (($_[0]->{version} > 23) && ($_[0]->{version} <= 84)) {
		$_[1]->pack_properties($_[0], (properties_info)[9]);
	}
	if ($_[0]->{version} > 49) {
		$_[1]->pack_properties($_[0], (properties_info)[10]);
	}
	if ($_[0]->{version} > 57) {
		$_[1]->pack_properties($_[0], (properties_info)[11]);
	}
	if ($_[0]->{version} > 61) {
		$_[1]->pack_properties($_[0], (properties_info)[12]);
	}
	if ($_[0]->{version} > 111) {
		$_[1]->pack_properties($_[0], (properties_info)[13]);
	}
}
sub state_import {
	if ($_[0]->{version} >= 1) {
		if ($_[0]->{version} <= 24) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
		} elsif (($_[0]->{version} > 24) && ($_[0]->{version} < 83)) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} < 83) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[2]);
		}
		if ($_[0]->{version} < 4) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[3]);
		}
		$_[1]->import_properties($_[2], $_[0], (properties_info)[4..5]);
	}
	if ($_[0]->{version} >= 4) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[6]);
	}
	if ($_[0]->{version} >= 8) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[7]);
	}
	if (($_[0]->{version} > 22) && ($_[0]->{version} <= 79)) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[8]);
	}
	if (($_[0]->{version} > 23) && ($_[0]->{version} <= 84)) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[9]);
	}
	if ($_[0]->{version} > 49) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[10]);
	}
	if ($_[0]->{version} > 57) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[11]);
	}
	if ($_[0]->{version} > 61) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[12]);
	}
	if ($_[0]->{version} > 111) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[13]);
	}
}
sub state_export {
	if ($_[0]->{version} >= 1) {
		if ($_[0]->{version} <= 24) {
			$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
		} elsif (($_[0]->{version} > 24) && ($_[0]->{version} < 83)) {
			$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} < 83) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[2]);
		}
		if ($_[0]->{version} < 4) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[3]);
		}
		my $pack;
		if ($_[0]->{version} >= 83) {
			$pack = __PACKAGE__;
		} else {
			$pack = undef;
		}
		$_[1]->export_properties($pack, $_[0], (properties_info)[4..5]);
	}
	if ($_[0]->{version} >= 4) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[6]);
	}
	if ($_[0]->{version} >= 8) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[7]);
	}
	if (($_[0]->{version} > 22) && ($_[0]->{version} <= 79)) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[8]);
	}
	if (($_[0]->{version} > 23) && ($_[0]->{version} <= 84)) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[9]);
	}
	if ($_[0]->{version} > 49) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[10]);
	}
	if ($_[0]->{version} > 57) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[11]);
	}
	if ($_[0]->{version} > 61) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[12]);
	}
	if ($_[0]->{version} > 111) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[13]);
	}
}
#######################################################################
package cse_alife_dynamic_object;
use strict;
sub state_read {
	cse_alife_object::state_read(@_);
}
sub state_write {
	cse_alife_object::state_write(@_);
}
sub state_import {
	cse_alife_object::state_import(@_);
}
sub state_export {
	cse_alife_object::state_export(@_);
}
#######################################################################
package cse_alife_dynamic_object_visual;
use strict;
sub state_read {
	cse_alife_object::state_read(@_);
	cse_visual::state_read(@_) if ($_[0]->{version} > 31);
}
sub state_write {
	cse_alife_object::state_write(@_);
	cse_visual::state_write(@_) if ($_[0]->{version} > 31);
}
sub state_import {
	cse_alife_object::state_import(@_);
	cse_visual::state_import(@_) if ($_[0]->{version} > 31);
}
sub state_export {
	cse_alife_object::state_export(@_);
	cse_visual::state_export(@_) if ($_[0]->{version} > 31);
}
#######################################################################
package cse_alife_object_climable;
use strict;
use constant properties_info => (
	{ name => 'game_material',	type => 'sz',	default => 'materials\\fake_ladders' },	# smthg|0x08
);
sub state_read {
	cse_alife_object::state_read(@_) if ($_[0]->{version} > 99);
	cse_shape::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info) if ($_[0]->{version} >= 128);
}
sub state_write {
	cse_alife_object::state_write(@_) if ($_[0]->{version} > 99);
	cse_shape::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info) if ($_[0]->{version} >= 128);
}
sub state_import {
	cse_alife_object::state_import(@_) if ($_[0]->{version} > 99);
	cse_shape::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info) if ($_[0]->{version} >= 128);
}
sub state_export {
	cse_alife_object::state_export(@_) if ($_[0]->{version} > 99);
	cse_shape::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info) if ($_[0]->{version} >= 128);
}
#######################################################################
package cse_alife_object_physic;
use strict;
use constant properties_info => (
	{ name => 'physic_type',	type => 'h32',	default => 0 },		# 0x138
	{ name => 'mass',		type => 'f32',	default => 0.0 },	# 0x13c
	{ name => 'fixed_bones',	type => 'sz',	default => '' },	# 0x140
	{ name => 'startup_animation',	type => 'sz',	default => '' },		# 0x138
	{ name => 'skeleton_flags',		type => 'u8',	default => 0 },	# 0x13c
	{ name => 'source_id',	type => 'u16',	default => 0 },	# 0x140
);
use constant upd_properties_info => (
	{ name => 'upd:num_items',	type => 'h8',	default => 0 },		# 0x138
	{ name => 'upd:ph_force',		type => 'f32v3',	default => [] },	# 0x13c
	{ name => 'upd:ph_torque',		type => 'f32v3',	default => [] },	# 0x13c
	{ name => 'upd:ph_position',		type => 'f32v3',	default => [] },	# 0x13c
	{ name => 'upd:ph_rotation',		type => 'f32v4',	default => [] },	# 0x13c
	{ name => 'upd:ph_angular_velosity',		type => 'f32v3',	default => [] },	# 0x13c
	{ name => 'upd:ph_linear_velosity',		type => 'f32v3',	default => [] },	# 0x13c
	{ name => 'upd:enabled',		type => 'u8'},	# 0x13c
);
sub state_read {
	if ($_[0]->{version} >= 14) {
		if ($_[0]->{version} < 16) {
			cse_alife_dynamic_object::state_read(@_);
		} else {
			cse_alife_dynamic_object_visual::state_read(@_);
		}
		if ($_[0]->{version} < 32) {
			cse_visual::state_read(@_);
		}
	}
	if ($_[0]->{version} >= 64) {
		cse_ph_skeleton::state_read(@_);
	}
	$_[1]->unpack_properties($_[0], (properties_info)[0..1]);
	if ($_[0]->{version} > 9) {
		$_[1]->unpack_properties($_[0], (properties_info)[2]);
	}
	if (($_[0]->{version} > 28) && ($_[0]->{version} < 65)) {
		$_[1]->unpack_properties($_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} < 64) {
		if ($_[0]->{version} > 39) {
			$_[1]->unpack_properties($_[0], (properties_info)[4]);
		}
		if ($_[0]->{version} > 56) {
			$_[1]->unpack_properties($_[0], (properties_info)[5]);
		}
	}
}
sub state_write {
	if ($_[0]->{version} >= 14) {
		if ($_[0]->{version} < 16) {
			cse_alife_dynamic_object::state_write(@_);
		} else {
			cse_alife_dynamic_object_visual::state_write(@_);
		}
		if ($_[0]->{version} < 32) {
			cse_visual::state_write(@_);
		}
	}
	if ($_[0]->{version} >= 64) {
		cse_ph_skeleton::state_write(@_);
	}
	$_[1]->pack_properties($_[0], (properties_info)[0..1]);
	if ($_[0]->{version} > 9) {
		$_[1]->pack_properties($_[0], (properties_info)[2]);
	}
	if (($_[0]->{version} > 28) && ($_[0]->{version} < 65)) {
		$_[1]->pack_properties($_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} < 64) {
		if ($_[0]->{version} > 39) {
			$_[1]->pack_properties($_[0], (properties_info)[4]);
		}
		if ($_[0]->{version} > 56) {
			$_[1]->pack_properties($_[0], (properties_info)[5]);
		}
	}
}
sub update_read {
	if ($_[1]->length() != 0) {
		if (($_[0]->{version} >= 122) && ($_[0]->{version} <= 128)) {
			$_[1]->unpack_properties($_[0], (upd_properties_info)[0]);
			if ($_[0]->{'upd:num_items'} != 0) {
				$_[1]->unpack_properties($_[0], (upd_properties_info)[1..4]);
				my $flags = $_[0]->{'upd:num_items'} >> 5;
				if (($flags & 0x2) == 0) {
					$_[1]->unpack_properties($_[0], (upd_properties_info)[5]);
				}
				if (($flags & 0x4) == 0) {
					$_[1]->unpack_properties($_[0], (upd_properties_info)[6]);
				}
				if ($_[1]->length() != 0) {
					$_[1]->unpack_properties($_[0], (upd_properties_info)[7]);  #actually bool. Dunno how to make better yet.
				}
			}
		}
	}
}
sub update_write {
		if (($_[0]->{version} >= 122) && ($_[0]->{version} <= 128)) {
			$_[1]->pack_properties($_[0], (upd_properties_info)[0]);
			if ($_[0]->{'upd:num_items'} != 0) {
				$_[1]->pack_properties($_[0], (upd_properties_info)[1..4]);
				my $flags = $_[0]->{'upd:num_items'} >> 5;
				if (($flags & 0x2) == 0) {
					$_[1]->pack_properties($_[0], (upd_properties_info)[5]);
				}
				if (($flags & 0x4) == 0) {
					$_[1]->pack_properties($_[0], (upd_properties_info)[6]);
				}
#				if (defined $_[0]->{'upd:enabled'}) {
				if ($_[1]->length() != 0) {
					$_[1]->pack_properties($_[0], (upd_properties_info)[7]);  #actually bool. Dunno how to make better yet.
				}
			}
		}
}
sub state_import {
	if ($_[0]->{version} >= 14) {
		if ($_[0]->{version} < 16) {
			cse_alife_dynamic_object::state_import(@_);
		} else {
			cse_alife_dynamic_object_visual::state_import(@_);
		}
		if ($_[0]->{version} < 32) {
			cse_visual::state_import(@_);
		}
	}
	if ($_[0]->{version} >= 64) {
		cse_ph_skeleton::state_import(@_);
	}
	$_[1]->import_properties($_[2], $_[0], (properties_info)[0..1]);
	if ($_[0]->{version} > 9) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[2]);
	}
	if (($_[0]->{version} > 28) && ($_[0]->{version} < 65)) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} < 64) {
		if ($_[0]->{version} > 39) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[4]);
		}
		if ($_[0]->{version} > 56) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[5]);
		}
	}
	if (not(::level())) {
		if (($_[0]->{version} >= 122) && ($_[0]->{version} <= 128)) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[0]);
			if ($_[0]->{'upd:num_items'} != 0) {
				$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[1..4]);
				my $flags = $_[0]->{'upd:num_items'} >> 5;
				if (($flags & 0x2) == 0) {
					$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[5]);
				}
				if (($flags & 0x4) == 0) {
					$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[6]);
				}
				if ($_[1]->is_value_exists($_[2], 'upd:enabled')) {
					$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[7]);  #actually bool. Dunno how to make better yet.
				}
			}
		}
	}
}
sub state_export {
	if ($_[0]->{version} >= 14) {
		if ($_[0]->{version} < 16) {
			cse_alife_dynamic_object::state_export(@_);
		} else {
			cse_alife_dynamic_object_visual::state_export(@_);
		}
		if ($_[0]->{version} < 32) {
			cse_visual::state_export(@_);
		}
	}
	if ($_[0]->{version} >= 64) {
		cse_ph_skeleton::state_export(@_);
	}
	$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..1]);
	if ($_[0]->{version} > 9) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[2]);
	}
	if (($_[0]->{version} > 28) && ($_[0]->{version} < 65)) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} < 64) {
		if ($_[0]->{version} > 39) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[4]);
		}
		if ($_[0]->{version} > 56) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[5]);
		}
	}
	if (not(::level())) {
		if (($_[0]->{version} >= 122) && ($_[0]->{version} <= 128)) {
			if (defined ($_[0]->{'upd:num_items'})) {
				$_[1]->export_properties(undef, $_[0], (upd_properties_info)[0]);
				if ($_[0]->{'upd:num_items'} != 0) {
					$_[1]->export_properties(undef, $_[0], (upd_properties_info)[1..4]);
					my $flags = $_[0]->{'upd:num_items'} >> 5;
					if (($flags & 0x2) == 0) {
						$_[1]->export_properties(undef, $_[0], (upd_properties_info)[5]);
					}
					if (($flags & 0x4) == 0) {
						$_[1]->export_properties(undef, $_[0], (upd_properties_info)[6]);
					}
					if (defined ($_[0]->{'upd:enabled'})) {
						$_[1]->export_properties(undef, $_[0], (upd_properties_info)[7]);
					}
				}
			}
		}
	}
}
#######################################################################
package cse_alife_object_hanging_lamp;
use strict;
use constant flPhysic		=> 0x0001;
use constant flCastShadow	=> 0x0002;
use constant flR1		=> 0x0004;
use constant flR2		=> 0x0008;
use constant flTypeSpot		=> 0x0010;
use constant flPointAmbient	=> 0x0020;
use constant properties_info => (
	{ name => 'main_color',		type => 'h32',	default => 0x00ffffff },# 0x134
	{ name => 'main_brightness',	type => 'f32',	default => 0.0 },	# 0x140
	{ name => 'main_color_animator',type => 'sz',	default => '' },	# 0x144
	{ name => 'cse_alife_object_hanging_lamp__unk1_sz',type => 'sz',	default => '' },	# 0x144
	{ name => 'cse_alife_object_hanging_lamp__unk2_sz',type => 'sz',	default => '' },	# 0x144
	{ name => 'main_range',		type => 'f32',	default => 0.0 },	# 0x14c
	{ name => 'light_flags',	type => 'h16',	default => 0 },		# 0x138
	{ name => 'cse_alife_object_hanging_lamp__unk3_f32',type => 'f32',	default => 0 },	# 0x144
	{ name => 'animation',	type => 'sz',	default => '$editor' },	# 0xe0
	{ name => 'cse_alife_object_hanging_lamp__unk4_sz',type => 'sz',	default => '' },	# 0x144
	{ name => 'cse_alife_object_hanging_lamp__unk5_f32',type => 'f32',	default => 0 },	# 0x144
	{ name => 'lamp_fixed_bones',	type => 'sz',	default => '' },	# 0x15c
	{ name => 'health',		type => 'f32',	default => 1.0 },	# 0x178
	{ name => 'main_virtual_size',	type => 'f32',	default => 0.0 },	# 0x150
	{ name => 'ambient_radius',	type => 'f32',	default => 0.0 },	# 0x164
	{ name => 'ambient_power',	type => 'f32',	default => 0.0 },	# 0x168
	{ name => 'ambient_texture',	type => 'sz',	default => '' },	# 0x16c
	{ name => 'main_texture',	type => 'sz',	default => '' },	# 0x148
	{ name => 'main_bone',		type => 'sz',	default => '' },	# 0x158
	{ name => 'main_cone_angle',	type => 'f32',	default => 0.0 },	# 0x160
	{ name => 'glow_texture',	type => 'sz',	default => '' },	# 0x170
	{ name => 'glow_radius',	type => 'f32',	default => 0.0 },	# 0x174
	{ name => 'ambient_bone',	type => 'sz',	default => '' },	# 0x154
	{ name => 'cse_alife_object_hanging_lamp__unk6_f32',type => 'f32',	default => 0.0 },	# 0x144
	{ name => 'cse_alife_object_hanging_lamp__unk7_f32',type => 'f32',	default => 0.0 },	# 0x144
	{ name => 'cse_alife_object_hanging_lamp__unk8_f32',type => 'f32',	default => 0.0 },	# 0x144
	{ name => 'main_cone_angle_old_format',	type => 'q8',	default => 0.0 },	# 0x160
);
sub state_read {
	if ($_[0]->{version} > 20) {
		cse_alife_dynamic_object_visual::state_read(@_);
	}
	if ($_[0]->{version} >= 69) {
		cse_ph_skeleton::state_read(@_);
	}
	if ($_[0]->{version} < 32) {
		cse_visual::state_read(@_);
	}
	if ($_[0]->{version} < 49) {
		$_[1]->unpack_properties($_[0], (properties_info)[0]);
		$_[1]->unpack_properties($_[0], (properties_info)[2..5]);
		$_[1]->unpack_properties($_[0], (properties_info)[26]);
		if ($_[0]->{version} > 10) {
			$_[1]->unpack_properties($_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} > 11) {
			$_[1]->unpack_properties($_[0], (properties_info)[6]);
		}
		if ($_[0]->{version} > 12) {
			$_[1]->unpack_properties($_[0], (properties_info)[7]);
		}
		if ($_[0]->{version} > 17) {
			$_[1]->unpack_properties($_[0], (properties_info)[8]);
		}
		if ($_[0]->{version} > 42) {
			$_[1]->unpack_properties($_[0], (properties_info)[9..10]);
		}
		if ($_[0]->{version} > 43) {
			$_[1]->unpack_properties($_[0], (properties_info)[11]);
		}
		if ($_[0]->{version} > 44) {
			$_[1]->unpack_properties($_[0], (properties_info)[12]);
		}
	} else {
		$_[1]->unpack_properties($_[0], (properties_info)[0..2]);
		$_[1]->unpack_properties($_[0], (properties_info)[5..6]);
		$_[1]->unpack_properties($_[0], (properties_info)[8]);
		$_[1]->unpack_properties($_[0], (properties_info)[11..12]);
	}
	if ($_[0]->{version} > 55) {
		$_[1]->unpack_properties($_[0], (properties_info)[13..21]);
	}
	if ($_[0]->{version} > 96) {
		$_[1]->unpack_properties($_[0], (properties_info)[22]);
	}
	if ($_[0]->{version} > 118) {
		$_[1]->unpack_properties($_[0], (properties_info)[23..25]);
	}
}
sub state_write {
	if ($_[0]->{version} > 20) {
		cse_alife_dynamic_object_visual::state_write(@_);
	}
	if ($_[0]->{version} >= 69) {
		cse_ph_skeleton::state_write(@_);
	}
	if ($_[0]->{version} < 32) {
		cse_visual::state_write(@_);
	}
	if ($_[0]->{version} < 49) {
		$_[1]->pack_properties($_[0], (properties_info)[0]);
		$_[1]->pack_properties($_[0], (properties_info)[2..5]);
		$_[1]->pack_properties($_[0], (properties_info)[26]);
		if ($_[0]->{version} > 10) {
			$_[1]->pack_properties($_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} > 11) {
			$_[1]->pack_properties($_[0], (properties_info)[6]);
		}
		if ($_[0]->{version} > 12) {
			$_[1]->pack_properties($_[0], (properties_info)[7]);
		}
		if ($_[0]->{version} > 17) {
			$_[1]->pack_properties($_[0], (properties_info)[8]);
		}
		if ($_[0]->{version} > 42) {
			$_[1]->pack_properties($_[0], (properties_info)[9..10]);
		}
		if ($_[0]->{version} > 43) {
			$_[1]->pack_properties($_[0], (properties_info)[11]);
		}
		if ($_[0]->{version} > 44) {
			$_[1]->pack_properties($_[0], (properties_info)[12]);
		}
	} else {
		$_[1]->pack_properties($_[0], (properties_info)[0..2]);
		$_[1]->pack_properties($_[0], (properties_info)[5..6]);
		$_[1]->pack_properties($_[0], (properties_info)[8]);
		$_[1]->pack_properties($_[0], (properties_info)[11..12]);
	}
	if ($_[0]->{version} > 55) {
		$_[1]->pack_properties($_[0], (properties_info)[13..21]);
	}
	if ($_[0]->{version} > 96) {
		$_[1]->pack_properties($_[0], (properties_info)[22]);
	}
	if ($_[0]->{version} > 118) {
		$_[1]->pack_properties($_[0], (properties_info)[23..25]);
	}
}
sub state_import {
	if ($_[0]->{version} > 20) {
		cse_alife_dynamic_object_visual::state_import(@_);
	}
	if ($_[0]->{version} >= 69) {
		cse_ph_skeleton::state_import(@_);
	}
	if ($_[0]->{version} < 32) {
		cse_visual::state_import(@_);
	}
	if ($_[0]->{version} < 49) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
		$_[1]->import_properties($_[2], $_[0], (properties_info)[2..5]);
		$_[1]->import_properties($_[2], $_[0], (properties_info)[26]);
		if ($_[0]->{version} > 10) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} > 11) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[6]);
		}
		if ($_[0]->{version} > 12) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[7]);
		}
		if ($_[0]->{version} > 17) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[8]);
		}
		if ($_[0]->{version} > 42) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[9..10]);
		}
		if ($_[0]->{version} > 43) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[11]);
		}
		if ($_[0]->{version} > 44) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[12]);
		}
	} else {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0..2]);
		$_[1]->import_properties($_[2], $_[0], (properties_info)[5..6]);
		$_[1]->import_properties($_[2], $_[0], (properties_info)[8]);
		$_[1]->import_properties($_[2], $_[0], (properties_info)[11..12]);
	}
	if ($_[0]->{version} > 55) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[13..21]);
	}
	if ($_[0]->{version} > 96) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[22]);
	}
	if ($_[0]->{version} > 118) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[23..25]);
	}
}
sub state_export {
	if ($_[0]->{version} > 20) {
		cse_alife_dynamic_object_visual::state_export(@_);
	}
	if ($_[0]->{version} >= 69) {
		cse_ph_skeleton::state_export(@_);
	}
	if ($_[0]->{version} < 32) {
		cse_visual::state_export(@_);
	}
	if ($_[0]->{version} < 49) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
		$_[1]->export_properties(undef, $_[0], (properties_info)[2..5]);
		$_[1]->export_properties(undef, $_[0], (properties_info)[26]);
		if ($_[0]->{version} > 10) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} > 11) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[6]);
		}
		if ($_[0]->{version} > 12) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[7]);
		}
		if ($_[0]->{version} > 17) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[8]);
		}
		if ($_[0]->{version} > 42) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[9..10]);
		}
		if ($_[0]->{version} > 43) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[11]);
		}
		if ($_[0]->{version} > 44) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[12]);
		}
	} else {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..2]);
		$_[1]->export_properties(undef, $_[0], (properties_info)[5..6]);
		$_[1]->export_properties(undef, $_[0], (properties_info)[8]);
		$_[1]->export_properties(undef, $_[0], (properties_info)[11..12]);
	}
	if ($_[0]->{version} > 55) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[13..21]);
	}
	if ($_[0]->{version} > 96) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[22]);
	}
	if ($_[0]->{version} > 118) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[23..25]);
	}
}
#######################################################################
package cse_alife_object_projector;
use strict;
use constant properties_info => (
	{ name => 'main_color',		type => 'h32',	default => 0x00ffffff },
	{ name => 'main_color_animator',type => 'sz',	default => '' },
	{ name => 'animation',	type => 'sz',	default => '$editor' },
	{ name => 'ambient_radius',	type => 'f32',	default => 0.0 },
	{ name => 'main_cone_angle',	type => 'q8',	default => 0.0 },
	{ name => 'main_virtual_size',	type => 'f32',	default => 0.0 },
	{ name => 'glow_texture',	type => 'sz',	default => '' },
	{ name => 'glow_radius',	type => 'f32',	default => 0.0 },
	{ name => 'cse_alife_object_hanging_lamp__unk3_u8',	type => 'u16',	default => 0 },
);
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	if ($_[0]->{version} < 48) {
		$_[1]->unpack_properties($_[0], (properties_info)[0..5]);
		if ($_[0]->{version} > 40) {
			$_[1]->unpack_properties($_[0], (properties_info)[6..7]);
		}
		if ($_[0]->{version} > 45) {
			$_[1]->unpack_properties($_[0], (properties_info)[8]);
		}
	}
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	if ($_[0]->{version} < 48) {
		$_[1]->pack_properties($_[0], (properties_info)[0..5]);
		if ($_[0]->{version} > 40) {
			$_[1]->pack_properties($_[0], (properties_info)[6..7]);
		}
		if ($_[0]->{version} > 45) {
			$_[1]->pack_properties($_[0], (properties_info)[8]);
		}
	}
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	if ($_[0]->{version} < 48) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0..5]);
		if ($_[0]->{version} > 40) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[6..7]);
		}
		if ($_[0]->{version} > 45) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[8]);
		}
	}
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	if ($_[0]->{version} < 48) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..5]);
		if ($_[0]->{version} > 40) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[6..7]);
		}
		if ($_[0]->{version} > 45) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[8]);
		}
	}
}
#######################################################################
package cse_alife_inventory_box;
use strict;
use constant properties_info => (
	{ name => 'cse_alive_inventory_box__unk1_u8', type => 'u8', default => 1 },
	{ name => 'cse_alive_inventory_box__unk2_u8', type => 'u8', default => 0 },
	{ name => 'tip', type => 'sz', default => '' },
);
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	if ($_[0]->{version} >= 128) {
		$_[1]->unpack_properties($_[0], properties_info);
	}
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	if ($_[0]->{version} >= 128) {
		$_[1]->pack_properties($_[0], properties_info);
	}
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	if ($_[0]->{version} >= 128) {
		$_[1]->import_properties($_[2], $_[0], properties_info);
	}
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	if ($_[0]->{version} >= 128) {
		$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	}
}
#######################################################################
package cse_alife_object_breakable;
use strict;
use constant properties_info => (
	{ name => 'health', type => 'f32', default => 1.0 },	# 0xe8
);
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}
#######################################################################
package cse_alife_mounted_weapon;
use strict;
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
}
#######################################################################
package cse_alife_stationary_mgun;
use strict;
use constant upd_properties_info => (
	{ name => 'upd:working',		type => 'u8' },		# 0xe8
	{ name => 'upd:dest_enemy_direction',	type => 'f32v3' },	# 0xe9
);
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
}
sub update_read {
	$_[1]->unpack_properties($_[0], upd_properties_info);
}
sub update_write {
	$_[1]->pack_properties($_[0], upd_properties_info);
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	if (not (::level())) {
		$_[1]->import_properties($_[2], $_[0], upd_properties_info);
	}
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	if (not (::level())) {
		$_[1]->export_properties(undef, $_[0], upd_properties_info);
	}
}
#######################################################################
package cse_alife_ph_skeleton_object;
use strict;
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	if ($_[0]->{version} >= 64) {
		cse_ph_skeleton::state_read(@_);
	}
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	if ($_[0]->{version} >= 64) {
		cse_ph_skeleton::state_write(@_);
	}
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	if ($_[0]->{version} >= 64) {
		cse_ph_skeleton::state_import(@_);
	}
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	if ($_[0]->{version} >= 64) {
		cse_ph_skeleton::state_export(@_);
	}
}
#######################################################################
package cse_alife_car;
use strict;
use constant properties_info => (
	{ name => 'cse_alife_car__unk1_f32', type => 'f32', default => 1.0 },	
	{ name => 'health', type => 'f32', default => 1.0 },	
	{ name => 'g_team', type => 'u8', default => 0 },	
	{ name => 'g_squad', type => 'u8', default => 0 },	
	{ name => 'g_group', type => 'u8', default => 0 },		
);
sub state_read {
	if (($_[0]->{version} < 8) || ($_[0]->{version} > 16)) {
	cse_alife_dynamic_object_visual::state_read(@_);
	}
	if ($_[0]->{version} < 8) {
	$_[1]->unpack_properties($_[0], (properties_info)[2..4]);
	}
	if ($_[0]->{version} > 65) {
		cse_ph_skeleton::state_read(@_);
	}
	if (($_[0]->{version} > 52) && (($_[0]->{version} < 55))) {
		$_[1]->unpack_properties($_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 92) {
		$_[1]->unpack_properties($_[0], (properties_info)[1]);
	}
#	if ($_[0]->{health} > 1.0) {
#		$_[0]->{health} *= 0.01;
#	}
}
sub state_write {
	if (($_[0]->{version} < 8) || ($_[0]->{version} > 16)) {
	cse_alife_dynamic_object_visual::state_write(@_);
	}
	if ($_[0]->{version} < 8) {
	$_[1]->pack_properties($_[0], (properties_info)[2..4]);
	}
	if ($_[0]->{version} > 65) {
		cse_ph_skeleton::state_write(@_);
	}
	if (($_[0]->{version} > 52) && (($_[0]->{version} < 55))) {
		$_[1]->pack_properties($_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 92) {
		$_[1]->pack_properties($_[0], (properties_info)[1]);
	}
#	if ($_[0]->{health} > 1.0) {
#		$_[0]->{health} *= 0.01;
#	}
}
sub state_import {
	if (($_[0]->{version} < 8) || ($_[0]->{version} > 16)) {
	cse_alife_dynamic_object_visual::state_import(@_);
	}
	if ($_[0]->{version} < 8) {
	$_[1]->import_properties($_[2], $_[0], (properties_info)[2..4]);
	}
	if ($_[0]->{version} > 65) {
		cse_ph_skeleton::state_import(@_);
	}
	if (($_[0]->{version} > 52) && (($_[0]->{version} < 55))) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 92) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);
	}
}
sub state_export {
	if (($_[0]->{version} < 8) || ($_[0]->{version} > 16)) {
	cse_alife_dynamic_object_visual::state_export(@_);
	}
	if ($_[0]->{version} < 8) {
	$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[2..4]);
	}
	if ($_[0]->{version} > 65) {
		cse_ph_skeleton::state_export(@_);
	}
	if (($_[0]->{version} > 52) && (($_[0]->{version} < 55))) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 92) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[1]);
	}
}
#######################################################################
package cse_alife_object_idol; 		#Define some properties!
use strict;
use constant properties_info => (
	{ name => 'cse_alife_object_idol__unk1_sz',	type => 'sz',	default => '' },
	{ name => 'cse_alife_object_idol__unk2_u32',	type => 'u32',	default => 0 },
);
sub state_read {
	cse_alife_human_abstract::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_human_abstract::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	cse_alife_human_abstract::update_read(@_);
}
sub update_write {
	cse_alife_human_abstract::update_write(@_);
}
sub state_import {
	cse_alife_human_abstract::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_human_abstract::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}
#######################################################################
package cse_alife_helicopter;
use strict;
use constant properties_info => (
	{ name => 'cse_alife_helicopter__unk1_sz',	type => 'sz', default => '' },	# 0xe0
	{ name => 'engine_sound',			type => 'sz', default => '' },	# 0x140
);
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	cse_motion::state_read(@_);
	if ($_[0]->{version} >= 69) {
		cse_ph_skeleton::state_read(@_);
	}
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	cse_motion::state_write(@_);
	if ($_[0]->{version} >= 69) {
		cse_ph_skeleton::state_write(@_);
	}
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	cse_motion::state_import(@_);
	if ($_[0]->{version} >= 69) {
		cse_ph_skeleton::state_import(@_);
	}
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	cse_motion::state_export(@_);
	if ($_[0]->{version} >= 69) {
		cse_ph_skeleton::state_export(@_);
	}
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}
#######################################################################
package se_turret_mgun;
use strict;
use constant properties_info => (
	{ name => 'health', type => 'f32', default => 1.0 },
);
sub state_read {
	cse_alife_helicopter::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_helicopter::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_helicopter::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_helicopter::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}
#######################################################################
package cse_alife_creature_abstract;
use strict;
use constant properties_info => (
	{ name => 'g_team',			type => 'u8',	default => 0xff },	# 0xe8
	{ name => 'g_squad',			type => 'u8',	default => 0xff },	# 0xe9
	{ name => 'g_group',			type => 'u8',	default => 0xff },	# 0xea
	{ name => 'health',			type => 'f32',	default => 1.0 },	# 0xec
	{ name => 'dynamic_out_restrictions',	type => 'l32u16v', default => [] },	# 0x118
	{ name => 'dynamic_in_restrictions',	type => 'l32u16v', default => [] },	# 0x128
	{ name => 'killer_id',			type => 'h16', default => 0xffff },	# 0x144
	{ name => 'game_death_time',		type => 'u8v8', default => [0,0,0,0,0,0,0,0] },	# 0x148
);
use constant upd_properties_info => (
	{ name => 'upd:health',		type => 'f32' },	# 0xec
	{ name => 'upd:timestamp',	type => 'h32' },	# 0xfc
	{ name => 'upd:creature_flags',	type => 'h8' },		# 0x100
	{ name => 'upd:position',	type => 'f32v3' },	# 0x58
	{ name => 'upd:o_model',	type => 'f32' },	# 0x104
	{ name => 'upd:o_torso',	type => 'f32v3' },	# 0x108, 0x10c, 0x110
	{ name => 'upd:o_model_old',	type => 'q8' },	# 0x104
	{ name => 'upd:o_torso_old',	type => 'q8v3' },	# 0x108, 0x10c, 0x110
	{ name => 'upd:g_team',		type => 'u8' },		# 0xe8
	{ name => 'upd:g_squad',	type => 'u8' },		# 0xe9
	{ name => 'upd:g_group',	type => 'u8' },		# 0xea
	{ name => 'upd:health_old',		type => 'q16' },	# 0xec
	{ name => 'upd:health_very_old',		type => 'q16_old' },	# 0xec
	{ name => 'upd:cse_alife_creature_abstract__unk1_f32v3',		type => 'f32v3', default => [0,0,0]},	# 0xec
);
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	$_[1]->unpack_properties($_[0], (properties_info)[0..2]);
	if ($_[0]->{version} > 18) {
		$_[1]->unpack_properties($_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} < 32) {
		cse_visual::state_read(@_);
	}	
	if ($_[0]->{version} > 87) {
		$_[1]->unpack_properties($_[0], (properties_info)[4..5]);
	}
	if ($_[0]->{version} > 94) {
		$_[1]->unpack_properties($_[0], (properties_info)[6]);
	}
	if ($_[0]->{version} > 115) {
		$_[1]->unpack_properties($_[0], (properties_info)[7]);
	}
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	$_[1]->pack_properties($_[0], (properties_info)[0..2]);
	if ($_[0]->{version} > 18) {
		$_[1]->pack_properties($_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} < 32) {
		cse_visual::state_write(@_);
	}	
	if ($_[0]->{version} > 87) {
		$_[1]->pack_properties($_[0], (properties_info)[4..5]);
	}
	if ($_[0]->{version} > 94) {
		$_[1]->pack_properties($_[0], (properties_info)[6]);
	}
	if ($_[0]->{version} > 115) {
		$_[1]->pack_properties($_[0], (properties_info)[7]);
	}
}
sub update_read {
	if ($_[0]->{version} > 109) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[0]);
	} elsif ($_[0]->{version} > 40) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[11]);
	} else {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[12]);
	}
	if (($_[0]->{version} < 17) && (ref($_[0]) eq 'se_actor')) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[13]);
	}
	$_[1]->unpack_properties($_[0], (upd_properties_info)[1..3]);
	if (($_[0]->{version} > 109) && 
		((($_[1]->length() > 7) && ((ref($_[0]) eq 'cse_alife_creature_crow') || (ref($_[0]) eq 'cse_alife_creature_phantom'))) || 
		(($_[1]->length() > 19) && ((ref($_[0]) eq 'cse_alife_monster_zombie') || (ref($_[0]) eq 'cse_alife_psy_dog_phantom') || (ref($_[0]) eq 'se_monster'))) || 
		(($_[1]->length() > 20) && (ref($_[0]) eq 'se_stalker') && ($_[0]->{version} > 109)) || 
		(($_[1]->length() > 28) && (ref($_[0]) eq 'se_actor')))) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[4..5]);
	} else {
		if ($_[0]->{version} > 85) {
			$_[1]->unpack_properties($_[0], (upd_properties_info)[6]);
		}
		if ($_[0]->{version} > 63) {
			$_[1]->unpack_properties($_[0], (upd_properties_info)[7]);
		}
	}
	$_[1]->unpack_properties($_[0], (upd_properties_info)[8..10]);
}
sub update_write {
	if ($_[0]->{version} > 109) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[0]);
	} elsif ($_[0]->{version} > 40) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[11]);
	} else {
		$_[1]->pack_properties($_[0], (upd_properties_info)[12]);
	}
	if (($_[0]->{version} < 17) && (ref($_[0]) eq 'se_actor')) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[13]);
	}
	$_[1]->pack_properties($_[0], (upd_properties_info)[1..3]);
	if (defined $_[0]->{'upd:o_model'}) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[4..5]);
	} else {
		if (defined $_[0]->{'upd:o_model_old'}) {
			$_[1]->pack_properties($_[0], (upd_properties_info)[6]);
		}
		if (defined $_[0]->{'upd:o_torso_old'}) {
			$_[1]->pack_properties($_[0], (upd_properties_info)[7]);
		}
	}
	$_[1]->pack_properties($_[0], (upd_properties_info)[8..10]);
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], (properties_info)[0..2]);
	if ($_[0]->{version} > 18) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} < 32) {
		cse_visual::state_import(@_);
	}	
	if ($_[0]->{version} > 87) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[4..5]);
	}
	if ($_[0]->{version} > 94) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[6]);
	}
	if ($_[0]->{version} > 115) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[7]);
	}
	if (not (::level())) {
		if ($_[0]->{version} > 109) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[0]);
		} elsif ($_[0]->{version} > 40) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[11]);
		} else {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[12]);
		}
		if (($_[0]->{version} < 17) && (ref($_[0]) eq 'se_actor')) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[13]);
		}
		$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[1..3]);
		if (defined $_[1]->value($_[2], 'upd:o_model')) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[4..5]);
		} else {
			if (defined $_[1]->value($_[2], 'upd:o_model_old')) {
				$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[6]);
			}
			if (defined $_[1]->value($_[2], 'upd:o_torso_old')) {
				$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[7]);
			}
		}
		$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[8..10]);
	}
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..2]);
	if ($_[0]->{version} > 18) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} < 32) {
		cse_visual::state_export(@_);
	}	
	if ($_[0]->{version} > 87) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[4..5]);
	}
	if ($_[0]->{version} > 94) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[6]);
	}
	if ($_[0]->{version} > 115) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[7]);
	}
	if (not (::level())) {
		if ($_[0]->{version} > 109) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[0]);
		} elsif ($_[0]->{version} > 40) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[11]);
		} else {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[12]);
		}
		if (($_[0]->{version} < 17) && (ref($_[0]) eq 'se_actor')) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[13]);
		}
		$_[1]->export_properties(undef, $_[0], (upd_properties_info)[1..3]);
		if (defined $_[0]->{'upd:o_model'}) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[4..5]);
		} else {
			if ($_[0]->{version} > 79) {
				$_[1]->export_properties(undef, $_[0], (upd_properties_info)[6]);
			}
			if ($_[0]->{version} > 63) {
				$_[1]->export_properties(undef, $_[0], (upd_properties_info)[7]);
			}
		}
		$_[1]->export_properties(undef, $_[0], (upd_properties_info)[8..10]);
	}
}
#######################################################################
package cse_alife_monster_abstract;
use strict;
use constant properties_info => (
	{ name => 'base_out_restrictors',	type => 'sz',	default => '' },
	{ name => 'base_in_restrictors',	type => 'sz',	default => '' },
	{ name => 'smart_terrain_id',		type => 'u16',	default => 65535 },
	{ name => 'smart_terrain_task_active',	type => 'u8',	default => 0 },
);
use constant upd_properties_info => (
	{ name => 'upd:next_game_vertex_id',	type => 'u16' },	# 0x168
	{ name => 'upd:prev_game_vertex_id',	type => 'u16' },	# 0x16a
	{ name => 'upd:distance_from_point',	type => 'f32' },	# 0x178
	{ name => 'upd:distance_to_point',	type => 'f32' },	# 0x17c
	{ name => 'upd:cse_alife_monster_abstract__unk1_u32',	type => 'u32',	default => 0 },	# 0x16a
	{ name => 'upd:cse_alife_monster_abstract__unk2_u32',	type => 'u32',	default => 0 },	# 0x178
);
sub state_read {
	cse_alife_creature_abstract::state_read(@_);
	if ($_[0]->{version} > 72) {
		$_[1]->unpack_properties($_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 73) {
		$_[1]->unpack_properties($_[0], (properties_info)[1]);
	}
	if ($_[0]->{version} > 111) {
		$_[1]->unpack_properties($_[0], (properties_info)[2]);
	}	
	if ($_[0]->{version} > 113) {
		$_[1]->unpack_properties($_[0], (properties_info)[3]);
	}
}
sub state_write {
	cse_alife_creature_abstract::state_write(@_);
	if ($_[0]->{version} > 72) {
		$_[1]->pack_properties($_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 73) {
		$_[1]->pack_properties($_[0], (properties_info)[1]);
	}
	if ($_[0]->{version} > 111) {
		$_[1]->pack_properties($_[0], (properties_info)[2]);
	}	
	if ($_[0]->{version} > 113) {
		$_[1]->pack_properties($_[0], (properties_info)[3]);
	}
}
sub update_read {
	cse_alife_creature_abstract::update_read(@_);
	$_[1]->unpack_properties($_[0], (upd_properties_info)[0..3]);
	if ($_[0]->{version} <= 79) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[4..5]);
	}
}
sub update_write {
	cse_alife_creature_abstract::update_write(@_);
	$_[1]->pack_properties($_[0], (upd_properties_info)[0..3]);
	if ($_[0]->{version} <= 79) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[4..5]);
	}
}
sub state_import {
	cse_alife_creature_abstract::state_import(@_);
	if ($_[0]->{version} > 72) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 73) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);
	}
	if ($_[0]->{version} > 111) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[2]);
	}	
	if ($_[0]->{version} > 113) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[3]);
	}
	if (not (::level())) {
		$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[0..3]);
		if ($_[0]->{version} <= 79) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[4..5]);
		}
	}
}
sub state_export {
	cse_alife_creature_abstract::state_export(@_);
	if ($_[0]->{version} > 72) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 73) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[1]);
	}
	if ($_[0]->{version} > 111) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[2]);
	}	
	if ($_[0]->{version} > 113) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[3]);
	}
	if (not (::level())) {
		my $pack;
		if ($_[0]->{version} <= 72) {
			$pack = __PACKAGE__;
		} else {
			$pack = undef;
		}
		$_[1]->export_properties($pack, $_[0], (upd_properties_info)[0..3]);
		if ($_[0]->{version} <= 79) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[4..5]);
		}
	}
}
#######################################################################
package cse_alife_trader_abstract;
use strict;
use constant eTraderFlagInfiniteAmmo	=> 0x00000001;
use constant eTraderFlagDummy		=> 0x00000000;	# really???
use constant properties_info => (
	{ name => 'cse_alife_trader_abstract__unk1_u32',	type => 'u32',	default => 0 },
	{ name => 'money',		type => 'u32',	default => 0 },		# 8+0x04=0x0c
	{ name => 'specific_character',	type => 'sz',	default => '' },	# 8+0x3c=0x44 
	{ name => 'trader_flags',	type => 'h32',	default => 0x1 },	# 8+0x0c=0x14
	{ name => 'character_profile',	type => 'sz',	default => '' },	# 8+0x38=0x40
	{ name => 'community_index',	type => 'h32',	default => 0xffffffff },# 8+0x10=0x18 
	{ name => 'rank',		type => 'h32',	default => 0x80000001 },# 8+0x18=0x20
	{ name => 'reputation',		type => 'h32',	default => 0x80000001 },# 8+0x14=0x1c
	{ name => 'checked_characters',	type => 'sz',	default => '' },	# 8+0x40=0x48
	{ name => 'cse_alife_trader_abstract__unk2_u8',	type => 'u8',	default => 0 },
	{ name => 'cse_alife_trader_abstract__unk3_u8',	type => 'u8',	default => 0 },
	{ name => 'cse_alife_trader_abstract__unk4_u32',	type => 'u32',	default => 0 },
	{ name => 'cse_alife_trader_abstract__unk5_u32',	type => 'u32',	default => 0 },
	{ name => 'cse_alife_trader_abstract__unk6_u32',	type => 'u32',	default => 0 },
);
use constant upd_properties_info => (
	{ name => 'upd:cse_alife_trader_abstract__unk1_u32',	type => 'u32',	default => 0 },
	{ name => 'upd:money',		type => 'u32',	default => 0 },		# 8+0x04=0x0c
	{ name => 'upd:cse_trader_abstract__unk2_u32',	type => 'u32',	default => 1 },# 8+0x10=0x18 
);
sub state_read {
	if ($_[0]->{version} > 19) {
		if ($_[0]->{version} < 108) {
			$_[1]->unpack_properties($_[0], (properties_info)[0]);
		}
		if ($_[0]->{version} < 36) {
			$_[1]->unpack_properties($_[0], (properties_info)[13]);
		}
		if ($_[0]->{version} > 62) {
			$_[1]->unpack_properties($_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} > 94) {
			$_[1]->unpack_properties($_[0], (properties_info)[2]);
		}
		if (($_[0]->{version} > 75) && ($_[0]->{version} < 95)) {
			$_[1]->unpack_properties($_[0], (properties_info)[11]);
			if ($_[0]->{version} > 79) {
				$_[1]->unpack_properties($_[0], (properties_info)[12]);
			}
		}
		if ($_[0]->{version} > 77) {
			$_[1]->unpack_properties($_[0], (properties_info)[3]);
		}
		if ($_[0]->{version} > 95) {
			$_[1]->unpack_properties($_[0], (properties_info)[4]);
		}
		if ($_[0]->{version} > 85) {
			$_[1]->unpack_properties($_[0], (properties_info)[5]);
		}
		if ($_[0]->{version} > 86) {
			$_[1]->unpack_properties($_[0], (properties_info)[6..7]);
		}
		if ($_[0]->{version} > 104) {
			$_[1]->unpack_properties($_[0], (properties_info)[8]);
		}
		if ($_[0]->{version} >= 128) {
			$_[1]->unpack_properties($_[0], (properties_info)[9..10]);
		}
	}
}
sub state_write {
	if ($_[0]->{version} > 19) {
		if ($_[0]->{version} < 108) {
			$_[1]->pack_properties($_[0], (properties_info)[0]);
		}
		if ($_[0]->{version} < 36) {
			$_[1]->pack_properties($_[0], (properties_info)[13]);
		}
		if ($_[0]->{version} > 62) {
			$_[1]->pack_properties($_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} > 94) {
			$_[1]->pack_properties($_[0], (properties_info)[2]);
		}
		if (($_[0]->{version} > 75) && ($_[0]->{version} < 95)) {
			$_[1]->pack_properties($_[0], (properties_info)[11]);
			if ($_[0]->{version} > 79) {
				$_[1]->pack_properties($_[0], (properties_info)[12]);
			}
		}
		if ($_[0]->{version} > 77) {
			$_[1]->pack_properties($_[0], (properties_info)[3]);
		}
		if ($_[0]->{version} > 95) {
			$_[1]->pack_properties($_[0], (properties_info)[4]);
		}
		if ($_[0]->{version} > 85) {
			$_[1]->pack_properties($_[0], (properties_info)[5]);
		}
		if ($_[0]->{version} > 86) {
			$_[1]->pack_properties($_[0], (properties_info)[6..7]);
		}
		if ($_[0]->{version} > 104) {
			$_[1]->pack_properties($_[0], (properties_info)[8]);
		}
		if ($_[0]->{version} >= 128) {
			$_[1]->pack_properties($_[0], (properties_info)[9..10]);
		}
	}
}
sub update_read {
	if (($_[0]->{version} > 19) && ($_[0]->{version} < 102)) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[0..1]);
		if ($_[0]->{version} < 86) {
			$_[1]->unpack_properties($_[0], (upd_properties_info)[2]);
		}
	}
}
sub update_write {
	if (($_[0]->{version} > 19) && ($_[0]->{version} < 102)) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[0..1]);
		if ($_[0]->{version} < 86) {
			$_[1]->pack_properties($_[0], (upd_properties_info)[2]);
		}
	}
}
sub state_import {	
	if ($_[0]->{version} > 19) {
		if ($_[0]->{version} < 108) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
		}
		if ($_[0]->{version} < 36) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[13]);
		}
		if ($_[0]->{version} > 62) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} > 94) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[2]);
		}
		if (($_[0]->{version} > 75) && ($_[0]->{version} < 95)) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[11]);
			if ($_[0]->{version} > 79) {
				$_[1]->import_properties($_[2], $_[0], (properties_info)[12]);
			}
		}
		if ($_[0]->{version} > 77) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[3]);
		}
		if ($_[0]->{version} > 95) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[4]);
		}
		if ($_[0]->{version} > 85) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[5]);
		}
		if ($_[0]->{version} > 86) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[6..7]);
		}
		if ($_[0]->{version} > 104) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[8]);
		}
		if ($_[0]->{version} >= 128) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[9..10]);
		}
	}
	if (not(::level())) {
		if (($_[0]->{version} > 19) && ($_[0]->{version} < 102)) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[0..1]);
			if ($_[0]->{version} < 86) {
				$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[2]);
			}
		}
	}
}
sub state_export {
	if ($_[0]->{version} > 19) {
		if ($_[0]->{version} < 108) {
			$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
		}
		if ($_[0]->{version} < 36) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[13]);
		}
		if ($_[0]->{version} > 62) {
			my $pack;
			if ($_[0]->{version} > 108) {
				$pack = __PACKAGE__;
			} else {
				$pack = undef;
			}
			$_[1]->export_properties($pack, $_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} > 94) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[2]);
		}
		if (($_[0]->{version} > 75) && ($_[0]->{version} < 95)) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[11]);
			if ($_[0]->{version} > 79) {
				$_[1]->export_properties(undef, $_[0], (properties_info)[12]);
			}
		}
		if ($_[0]->{version} > 77) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[3]);
		}
		if ($_[0]->{version} > 95) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[4]);
		}
		if ($_[0]->{version} > 85) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[5]);
		}
		if ($_[0]->{version} > 86) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[6..7]);
		}
		if ($_[0]->{version} > 104) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[8]);
		}
		if ($_[0]->{version} >= 128) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[9..10]);
		}
	}
	if (not(::level())) {
		if (($_[0]->{version} > 19) && ($_[0]->{version} < 102)) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[0..1]);
			if ($_[0]->{version} < 86) {
				$_[1]->export_properties(undef, $_[0], (upd_properties_info)[2]);
			}
		}
	}
}
#######################################################################
package cse_alife_human_abstract;
use strict;
use constant properties_info => (
	{ name => 'path',	type => 'l32u32v',	default => [] },	# 0x38
	{ name => 'visited_vertices',	type => 'u32', 		default => 0 },
	{ name => 'known_customers_sz',		type => 'sz', default => '' },	# 0x38
	{ name => 'known_customers',	type => 'l32u32v', default => [] },	# 0x44
	{ name => 'equipment_preferences',	type => 'l32u8v', default => [] },	# 0x38
	{ name => 'main_weapon_preferences',	type => 'l32u8v', default => [] },	# 0x44
	{ name => 'smart_terrain_id',	type => 'u16', default => 0 },	# 0x44
	{ name => 'cse_alife_human_abstract__unk1_u32',	type => 'ha1', 		default => [] },
	{ name => 'cse_alife_human_abstract__unk2_u32',	type => 'ha2', 		default => [] },
	{ name => 'cse_alife_human_abstract__unk3_u32',	type => 'u32', 		default => 0 },
);
use constant upd_properties_info => (
	{ name => 'upd:cse_alife_human_abstract__unk3_u32',	type => 'u32', 		default => 0 },
	{ name => 'upd:cse_alife_human_abstract__unk4_u32',	type => 'u32', 		default => 0xffffffff },
	{ name => 'upd:cse_alife_human_abstract__unk5_u32',	type => 'u32', 		default => 0xffffffff },
);
sub state_read {
	cse_alife_trader_abstract::state_read(@_);
	cse_alife_monster_abstract::state_read(@_);
	if ($_[0]->{version} > 19) {
		if ($_[0]->{version} < 110) {
			$_[1]->unpack_properties($_[0], (properties_info)[0..1]);
		}
		if ($_[0]->{version} > 35) {
			if ($_[0]->{version} < 110) {
				$_[1]->unpack_properties($_[0], (properties_info)[2]);
			}
			if ($_[0]->{version} < 118) {
				$_[1]->unpack_properties($_[0], (properties_info)[3]);
			}
		} else {
			$_[1]->unpack_properties($_[0], (properties_info)[9]);
		}
		if ($_[0]->{version} > 63) {
			$_[1]->unpack_properties($_[0], (properties_info)[4..5]);
		} elsif (($_[0]->{version} > 37) && ($_[0]->{version} <= 63)) {
			$_[1]->unpack_properties($_[0], (properties_info)[7..8]);
		}
	}
	if (($_[0]->{version} >= 110) && ($_[0]->{version} < 112)) {
		$_[1]->unpack_properties($_[0], (properties_info)[6]);
	}
}
sub state_write {
	cse_alife_trader_abstract::state_write(@_);
	cse_alife_monster_abstract::state_write(@_);
	if ($_[0]->{version} > 19) {
		if ($_[0]->{version} < 110) {
			$_[1]->pack_properties($_[0], (properties_info)[0..1]);
		}
		if ($_[0]->{version} > 35) {
			if ($_[0]->{version} < 110) {
				$_[1]->pack_properties($_[0], (properties_info)[2]);
			}
			if ($_[0]->{version} < 118) {
				$_[1]->pack_properties($_[0], (properties_info)[3]);
			}
		} else {
			$_[1]->pack_properties($_[0], (properties_info)[9]);
		}
		if ($_[0]->{version} > 63) {
			$_[1]->pack_properties($_[0], (properties_info)[4..5]);
		} elsif (($_[0]->{version} > 37) && ($_[0]->{version} <= 63)) {
			$_[1]->pack_properties($_[0], (properties_info)[7..8]);
		}
	}
	if (($_[0]->{version} >= 110) && ($_[0]->{version} < 112)) {
		$_[1]->pack_properties($_[0], (properties_info)[6]);
	}
}
sub update_read {
	cse_alife_trader_abstract::update_read(@_);
	cse_alife_monster_abstract::update_read(@_);
	if ($_[0]->{version} <= 109) {
		$_[1]->unpack_properties($_[0], upd_properties_info);
	}
}
sub update_write {
	cse_alife_trader_abstract::update_write(@_);
	cse_alife_monster_abstract::update_write(@_);
	if ($_[0]->{version} <= 109) {
		$_[1]->pack_properties($_[0], upd_properties_info);
	}
}
sub state_import {
	cse_alife_trader_abstract::state_import(@_);
	cse_alife_monster_abstract::state_import(@_);
	if ($_[0]->{version} > 19) {
		if ($_[0]->{version} < 110) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[0..1]);
		}
		if ($_[0]->{version} > 35) {
			if ($_[0]->{version} < 110) {
				$_[1]->import_properties($_[2], $_[0], (properties_info)[2]);
			}
			if ($_[0]->{version} < 118) {
				$_[1]->import_properties($_[2], $_[0], (properties_info)[3]);
			}
		} else {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[9]);
		}
		if ($_[0]->{version} > 63) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[4..5]);
		} elsif (($_[0]->{version} > 37) && ($_[0]->{version} <= 63)) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[7..8]);
		}
	}
	if (($_[0]->{version} >= 110) && ($_[0]->{version} < 112)) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[6]);
	}
	if (not (::level())) {
		if ($_[0]->{version} <= 109) {
			$_[1]->import_properties($_[2], $_[0], upd_properties_info);
		}
	}
}
sub state_export {
	cse_alife_trader_abstract::state_export(@_);
	cse_alife_monster_abstract::state_export(@_);
	if ($_[0]->{version} > 19) {
		if ($_[0]->{version} < 110) {
			$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..1]);
		}
		if ($_[0]->{version} > 35) {
			if ($_[0]->{version} < 110) {
				$_[1]->export_properties(undef, $_[0], (properties_info)[2]);
			}
			if ($_[0]->{version} < 118) {
				$_[1]->export_properties(undef, $_[0], (properties_info)[3]);
			}
		}
		if ($_[0]->{version} > 63) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[4..5]);
		} elsif (($_[0]->{version} > 37) && ($_[0]->{version} <= 63)) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[7..8]);
		} else {
			$_[1]->export_properties(undef, $_[0], (properties_info)[9]);
		}
	}
	if (($_[0]->{version} >= 110) && ($_[0]->{version} < 112)) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[6]);
	}
	if (not (::level())) {
		if ($_[0]->{version} <= 109) {
			$_[1]->export_properties(undef, $_[0], upd_properties_info);
		}
	}
}
#######################################################################
package cse_alife_human_stalker;
use strict;
use constant properties_info => (
	{ name => 'cse_alife_human_stalker__unk1_bool', type => 'u8', default => 0 },	# 0x2b8
);
use constant upd_properties_info => (
	{ name => 'upd:start_dialog', type => 'sz' },	# 0x2b8
);
sub state_read {
	cse_alife_human_abstract::state_read(@_);
	if ($_[0]->{version} > 67) {
		cse_ph_skeleton::state_read(@_);
	}
	if (($_[0]->{version} > 90) && ($_[0]->{version} < 111)) {
		$_[1]->unpack_properties($_[0], properties_info);
	}
}
sub state_write {
	cse_alife_human_abstract::state_write(@_);
	if ($_[0]->{version} > 67) {
		cse_ph_skeleton::state_write(@_);
	}
	if (($_[0]->{version} > 90) && ($_[0]->{version} < 111)) {
		$_[1]->pack_properties($_[0], properties_info);
	}
}
sub update_read {
	cse_alife_human_abstract::update_read(@_);
	if ($_[0]->{version} > 94) {
	$_[1]->unpack_properties($_[0], upd_properties_info);
	}
}
sub update_write {
	cse_alife_human_abstract::update_write(@_);
	if ($_[0]->{version} > 94) {
	$_[1]->pack_properties($_[0], upd_properties_info);
	}
}
sub state_import {
	cse_alife_human_abstract::state_import(@_);
	if ($_[0]->{version} > 67) {
		cse_ph_skeleton::state_import(@_);
	}
	if (($_[0]->{version} > 90) && ($_[0]->{version} < 111)) {
		$_[1]->import_properties($_[2], $_[0], properties_info);
	}
	if (not (::level())) {
		if ($_[0]->{version} > 94) {
		$_[1]->import_properties($_[2], $_[0], upd_properties_info);
		}
	}
}
sub state_export {
	cse_alife_human_abstract::state_export(@_);
	if ($_[0]->{version} > 67) {
		cse_ph_skeleton::state_export(@_);
	}
	if (($_[0]->{version} > 90) && ($_[0]->{version} < 111)) {
		$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	}
	if (not (::level())) {
		if ($_[0]->{version} > 94) {
		$_[1]->export_properties(__PACKAGE__, $_[0], upd_properties_info);
		}
	}
}
#######################################################################
package se_stalker;
use strict;
use stkutils::debug;
use constant properties_info => (
	{ name => 'job_online',			type => 'u8',	default => 2 },
	{ name => 'was_in_smart_terrain',	type => 'u8',	default => 0 },
	{ name => 'death_dropped',		type => 'u8',	default => 0 },
	{ name => 'squad_id',		type => 'sz',	default => "nil" },
	{ name => 'sim_forced_online',	type => 'u8',	default => 0 },
);
sub state_read {
	cse_alife_human_stalker::state_read(@_);
	if ($_[0]->{version} > 94) {
		$_[1]->unpack_properties($_[0], (properties_info)[0]);
		if ($_[0]->{version} >= 128) {
			if (not ::sdk()) {
				if ($_[1]->length() != 2) {
					stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[1]->length() == 2', 'unexpected size ['.$_[1]->length().'] in '.$_[0]->{name});
				}
			}
			$_[1]->unpack_properties($_[0], (properties_info)[1..2]);
			$_[1]->unpack_properties($_[0], (properties_info)[3..4]) if ::sdk();
		} elsif ($_[0]->{version} >= 122) {
			$_[1]->unpack_properties($_[0], (properties_info)[2..4]);	
		} elsif ($_[0]->{version} == 118) {
			$_[1]->unpack_properties($_[0], (properties_info)[1]);	
			if ($_[0]->{script_version} > 5) {
				$_[1]->unpack_properties($_[0], (properties_info)[2]);	
			}
		}
	}
}
sub state_write {
	cse_alife_human_stalker::state_write(@_);
	if ($_[0]->{version} > 94) {
	$_[1]->pack_properties($_[0], (properties_info)[0]);
	if ($_[0]->{version} >= 128) {
		$_[1]->pack_properties($_[0], (properties_info)[1..2]);
	} elsif ($_[0]->{version} >= 122) {
		$_[1]->pack_properties($_[0], (properties_info)[2..4]);	
	} elsif ($_[0]->{version} == 118) {
		$_[1]->pack_properties($_[0], (properties_info)[1]);	
		if ($_[0]->{script_version} > 5) {
			$_[1]->pack_properties($_[0], (properties_info)[2]);	
		}
	}
	}
}
sub update_read {
	cse_alife_human_stalker::update_read(@_);
}
sub update_write {
	cse_alife_human_stalker::update_write(@_);
}
sub state_import {
	cse_alife_human_stalker::state_import(@_);
	if ($_[0]->{version} > 94) {
	$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
	if ($_[0]->{version} >= 128) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[1..2]);
	} elsif ($_[0]->{version} >= 122) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[2..4]);	
	} elsif ($_[0]->{version} == 118) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);	
		if ($_[0]->{script_version} > 5) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[2]);	
		}
	}
	}
}
sub state_export {
	cse_alife_human_stalker::state_export(@_);
	if ($_[0]->{version} > 94) {
	$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
	if ($_[0]->{version} >= 128) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[1..2]);
	} elsif ($_[0]->{version} >= 122) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[2..4]);	
	} elsif ($_[0]->{version} == 118) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[1]);	
		if ($_[0]->{script_version} > 5) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[2]);	
		}
	}
	}
}
#######################################################################
package cse_alife_trader;
use strict;
use constant properties_info => (
	{ name => 'organization_id',			type => 'u32',	default => 1 },
	{ name => 'ordered_artefacts',	type => 'u32',	default => 0 }, ###FIXME. there is complex type.
	{ name => 'supplies',		type => 'suppl',	default => [] },
);
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	cse_alife_trader_abstract::state_read(@_);
	if ($_[0]->{version} < 118) {
		if ($_[0]->{version} > 35) {
			$_[1]->unpack_properties($_[0], (properties_info)[0]);
		}
		if ($_[0]->{version} > 29) {
			$_[1]->unpack_properties($_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} > 30) {
			$_[1]->unpack_properties($_[0], (properties_info)[2]);
		}
	}
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	cse_alife_trader_abstract::state_write(@_);
	if ($_[0]->{version} < 118) {
		if ($_[0]->{version} > 35) {
			$_[1]->pack_properties($_[0], (properties_info)[0]);
		}
		if ($_[0]->{version} > 29) {
			$_[1]->pack_properties($_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} > 30) {
			$_[1]->pack_properties($_[0], (properties_info)[2]);
		}
	}
}
sub update_read {
	cse_alife_trader_abstract::update_read(@_);
}
sub update_write {
	cse_alife_trader_abstract::update_write(@_);
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	cse_alife_trader_abstract::state_import(@_);
	if ($_[0]->{version} < 118) {
		if ($_[0]->{version} > 35) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
		}
		if ($_[0]->{version} > 29) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} > 30) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[2]);
		}
	}
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	cse_alife_trader_abstract::state_export(@_);
	if ($_[0]->{version} < 118) {
		if ($_[0]->{version} > 35) {
			$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
		}
		if ($_[0]->{version} > 29) {
			my $pack;
			if ($_[0]->{version} <= 35) {
				$pack = __PACKAGE__;
			} else {
				$pack = undef;
			}
			$_[1]->export_properties($pack, $_[0], (properties_info)[1]);
		}
		if ($_[0]->{version} > 30) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[2]);
		}
	}
}
#######################################################################
package cse_alife_creature_actor;
use strict;
use stkutils::debug;
use constant properties_info => (
	{ name => 'g_team',			type => 'u8',	default => 0xff },	# 0xe8
	{ name => 'g_squad',			type => 'u8',	default => 0xff },	# 0xe9
	{ name => 'g_group',			type => 'u8',	default => 0xff },	# 0xea
	{ name => 'health',			type => 'f32',	default => 1.0 },	# 0xec
	{ name => 'holder_id', type => 'h16', default => 0xffff },	# 0x224 (0x22c)
);
use constant upd_properties_info => (
	{ name => 'upd:actor_state',		type => 'h16' },	# 0x200 (0x208) mstate
	{ name => 'upd:actor_accel_header',	type => 'h16' },	#
	{ name => 'upd:actor_accel_data',	type => 'h32' },	# 0x202,0x206,0x21a (0x20a+)
	{ name => 'upd:actor_velocity_header',	type => 'h16' },	#
	{ name => 'upd:actor_velocity_data',	type => 'h32' },	# 0x20e,0x212,0x216 (0x216+)
	{ name => 'upd:actor_radiation',	type => 'f32' },	# 0x21c (0x224) fRadiation
	{ name => 'upd:actor_radiation_old',	type => 'q16' },	# 0x21c (0x224) fRadiation
	{ name => 'upd:cse_alife_creature_actor_unk1_q16',	type => 'q16' },	# 0x21c (0x224) fRadiation
	{ name => 'upd:actor_weapon',		type => 'u8' },		# 0x220 (0x228) weapon
	{ name => 'upd:num_items',		type => 'u16' },	# 0x222 (0x22a) num_items
	{ name => 'upd:actor_radiation_very_old',	type => 'q16_old' },	# 0x21c (0x224) fRadiation
);
sub state_read {
	cse_alife_creature_abstract::state_read(@_);
	cse_alife_trader_abstract::state_read(@_);
	if ($_[0]->{version} > 91) {
		cse_ph_skeleton::state_read(@_);
	}
	if ($_[0]->{version} > 88) {
		$_[1]->unpack_properties($_[0], (properties_info)[4]);
	}
}
sub state_write {
	cse_alife_creature_abstract::state_write(@_);
	cse_alife_trader_abstract::state_write(@_);
	if ($_[0]->{version} > 91) {
		cse_ph_skeleton::state_write(@_);
	}
	if ($_[0]->{version} > 88) {
		$_[1]->pack_properties($_[0], (properties_info)[4]);
	}
}
sub update_read {
	cse_alife_creature_abstract::update_read(@_);
	cse_alife_trader_abstract::update_read(@_);
	$_[1]->unpack_properties($_[0], (upd_properties_info)[0..4]);
	if ($_[0]->{version} > 109) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[5]);
	} elsif ($_[0]->{version} > 40) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[6]);
	} else {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[10]);
	}
	if (($_[0]->{version} > 101) && ($_[0]->{version} <= 104)) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[7]);
	}
	$_[1]->unpack_properties($_[0], (upd_properties_info)[8]);
	if ($_[0]->{version} > 39) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[9]);
	}
	if (defined $_[0]->{'upd:num_items'}) {
		stkutils::debug::fail(__PACKAGE__.'::update_read', __LINE__, '$_[0]->{upd:num_items} == 0', 'unexpected upd:num_items') unless $_[0]->{'upd:num_items'} == 0;
	}
}
sub update_write {
	cse_alife_creature_abstract::update_write(@_);
	cse_alife_trader_abstract::update_write(@_);
	$_[1]->pack_properties($_[0], (upd_properties_info)[0..4]);
	if ($_[0]->{version} > 109) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[5]);
	} elsif ($_[0]->{version} > 40) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[6]);
	} else {
		$_[1]->pack_properties($_[0], (upd_properties_info)[10]);
	}
	if (($_[0]->{version} > 101) && ($_[0]->{version} <= 104)) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[7]);
	}
	$_[1]->pack_properties($_[0], (upd_properties_info)[8]);
	if ($_[0]->{version} > 39) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[9]);
	}
}
sub state_import {
	cse_alife_creature_abstract::state_import(@_);
	cse_alife_trader_abstract::state_import(@_);
	if ($_[0]->{version} > 91) {
		cse_ph_skeleton::state_import(@_);
	}
	if ($_[0]->{version} > 88) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[4]);
	}
	if (not (::level())) {
		$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[0..4]);
		if ($_[0]->{version} > 109) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[5]);
		} elsif ($_[0]->{version} > 40) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[6]);
		} else {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[10]);
		}
		if (($_[0]->{version} > 101) && ($_[0]->{version} <= 104)) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[7]);
		}
		$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[8]);
		if ($_[0]->{version} > 39) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[9]);
		}
	}
}
sub state_export {
	cse_alife_creature_abstract::state_export(@_);
	cse_alife_trader_abstract::state_export(@_);
	if ($_[0]->{version} > 91) {
		cse_ph_skeleton::state_export(@_);
	}
	if ($_[0]->{version} > 88) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[4]);
	}
	if (not (::level())) {
		my $pack;
		if (($_[0]->{version} >= 21) && ($_[0]->{version} <= 88)) {
			$pack = __PACKAGE__;
		} else {
			$pack = undef;
		}
		$_[1]->export_properties($pack, $_[0], (upd_properties_info)[0..4]);
		if ($_[0]->{version} > 109) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[5]);
		} elsif ($_[0]->{version} > 40) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[6]);
		} else {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[10]);
		}
		if (($_[0]->{version} > 101) && ($_[0]->{version} <= 104)) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[7]);
		}
		$_[1]->export_properties(undef, $_[0], (upd_properties_info)[8]);
		if ($_[0]->{version} > 39) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[9]);
		}
	}
}
#######################################################################
package se_actor;
use strict;
use constant properties_info => (
	{ name => 'start_position_filled',	type => 'u8',	default => 0 },
	{ name => 'dumb',	type => 'actorData' }, 
);
sub state_read {
	cse_alife_creature_actor::state_read(@_);
	if ($_[0]->{version} >= 128) {
		::set_save_marker($_[1], 'load', 0, 'se_actor');
		$_[1]->unpack_properties($_[0], (properties_info)[0]);
		::set_save_marker($_[1], 'load', 1, 'se_actor');
	} elsif ($_[0]->{version} >= 122) {
		$_[1]->unpack_properties($_[0], (properties_info)[1]);
	}
}
sub state_write {
	cse_alife_creature_actor::state_write(@_);
	if ($_[0]->{version} >= 128) {
		::set_save_marker($_[1], 'save', 0, 'se_actor');
		$_[1]->pack_properties($_[0], (properties_info)[0]);
		::set_save_marker($_[1], 'save', 1, 'se_actor');
	} elsif ($_[0]->{version} >= 122) {
		$_[1]->pack_properties($_[0], (properties_info)[1]);
	}
}
sub update_read {
	cse_alife_creature_actor::update_read(@_);
}
sub update_write {
	cse_alife_creature_actor::update_write(@_);
}
sub state_import {
	cse_alife_creature_actor::state_import(@_);
	if ($_[0]->{version} >= 128) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
	} elsif ($_[0]->{version} >= 122) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);
	}
}
sub state_export {
	cse_alife_creature_actor::state_export(@_);
	if ($_[0]->{version} >= 128) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
	} elsif ($_[0]->{version} >= 122) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[1]);
	}
}
#######################################################################
package cse_alife_creature_crow;
use strict;
sub state_read {
	if ($_[0]->{version} > 20) {
		cse_alife_creature_abstract::state_read(@_);
		if ($_[0]->{version} < 32) {
			cse_visual::state_read(@_);
		}
	}
}
sub state_write {
	if ($_[0]->{version} > 20) {
		cse_alife_creature_abstract::state_write(@_);
		if ($_[0]->{version} < 32) {
			cse_visual::state_write(@_);
		}
	}
}
sub update_read {
	cse_alife_creature_abstract::update_read(@_);
}
sub update_write {
	cse_alife_creature_abstract::update_write(@_);
}
sub state_import {
	if ($_[0]->{version} > 20) {
		cse_alife_creature_abstract::state_import(@_);
		if ($_[0]->{version} < 32) {
			cse_visual::state_import(@_);
		}
	}
}
sub state_export {
	if ($_[0]->{version} > 20) {
		cse_alife_creature_abstract::state_export(@_);
		if ($_[0]->{version} < 32) {
			cse_visual::state_export(@_);
		}
	}
}
#######################################################################
package cse_alife_creature_phantom;
use strict;
sub state_read {
	cse_alife_creature_abstract::state_read(@_);
}
sub state_write {
	cse_alife_creature_abstract::state_write(@_);
}
sub update_read {
	cse_alife_creature_abstract::update_read(@_);
}
sub update_write {
	cse_alife_creature_abstract::update_write(@_);
}
sub state_import {
	cse_alife_creature_abstract::state_import(@_);
}
sub state_export {
	cse_alife_creature_abstract::state_export(@_);
}
#######################################################################
package cse_alife_psy_dog_phantom;
use strict;
sub state_read {
	cse_alife_monster_base::state_read(@_);
}
sub state_write {
	cse_alife_monster_base::state_write(@_);
}
sub update_read {
	cse_alife_monster_base::update_read(@_);
}
sub update_write {
	cse_alife_monster_base::update_write(@_);
}
sub state_import {
	cse_alife_monster_base::state_import(@_);
}
sub state_export {
	cse_alife_monster_base::state_export(@_);
}
#######################################################################
package cse_alife_monster_base;
use strict;
use constant properties_info => (
	{ name => 'spec_object_id', type => 'u16', default => 65535 },	# 0x248
);
sub state_read {
	cse_alife_monster_abstract::state_read(@_);
	if ($_[0]->{version} >= 68) {
		cse_ph_skeleton::state_read(@_);
	}
	if ($_[0]->{version} >= 109) {
		$_[1]->unpack_properties($_[0], properties_info);
	}
}
sub state_write {
	cse_alife_monster_abstract::state_write(@_);
	if ($_[0]->{version} >= 68) {
		cse_ph_skeleton::state_write(@_);
	}
	if ($_[0]->{version} >= 109) {
		$_[1]->pack_properties($_[0], properties_info);
	}
}
sub update_read {
	cse_alife_monster_abstract::update_read(@_);
}
sub update_write {
	cse_alife_monster_abstract::update_write(@_);
}
sub state_import {
	cse_alife_monster_abstract::state_import(@_);
	if ($_[0]->{version} >= 68) {
		cse_ph_skeleton::state_import(@_);
	}
	if ($_[0]->{version} >= 109) {
		$_[1]->import_properties($_[2], $_[0], properties_info);
	}
}
sub state_export {
	cse_alife_monster_abstract::state_export(@_);
	if ($_[0]->{version} >= 68) {
		cse_ph_skeleton::state_export(@_);
	}
	if ($_[0]->{version} >= 109) {
		$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	}
}
#######################################################################
package se_monster;
use strict;
use stkutils::debug;
use constant properties_info => (
	{ name => 'job_online',			type => 'u8',	default => 2 },
	{ name => 'was_in_smart_terrain',	type => 'u8',	default => 0 },
	{ name => 'squad_id',			type => 'sz',	default => 'nil' },	
	{ name => 'sim_forced_online',	type => 'u8',	default => 0 },
);
sub state_read {
	if (($_[0]->{section_name} eq 'm_rat_e') && ($_[0]->{version} <= 104)) {
		cse_alife_monster_rat::state_read(@_);
	} else {
		cse_alife_monster_base::state_read(@_);
		if (defined $_[0]->{script_version}) {
			if ((($_[0]->{version} >= 116) && ($_[0]->{script_version} > 3)) || ($_[0]->{script_version} == 2)) {
				$_[1]->unpack_properties($_[0], (properties_info)[0]);
				if ($_[0]->{version} >= 128) {
					if (not ::sdk()) {
						if ($_[1]->length() != 1) {
							stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[1]->length() == 1', 'unexpected size ['.$_[1]->length().'] in '.$_[0]->{name});
						}
					}
					$_[1]->unpack_properties($_[0], (properties_info)[1]);
					$_[1]->unpack_properties($_[0], (properties_info)[2..3]) if ::sdk();
				} elsif ($_[0]->{version} >= 122) {
					$_[1]->unpack_properties($_[0], (properties_info)[2..3]);	
				} elsif (($_[0]->{version} == 118) && ($_[0]->{script_version} >= 5)) {
					$_[1]->unpack_properties($_[0], (properties_info)[1]);	
				}
			}
		}
	}
}
sub state_write {
	if (($_[0]->{section_name} eq 'm_rat_e') && ($_[0]->{version} <= 104)) {
		cse_alife_monster_rat::state_write(@_);
	} else {
		cse_alife_monster_base::state_write(@_);
		if (defined $_[0]->{script_version}) {
			if ((($_[0]->{version} >= 116) && ($_[0]->{script_version} > 3)) || ($_[0]->{script_version} == 2)) {
				$_[1]->pack_properties($_[0], (properties_info)[0]);
				if ($_[0]->{version} >= 128) {
					$_[1]->pack_properties($_[0], (properties_info)[1]);
				} elsif ($_[0]->{version} >= 122) {
					$_[1]->pack_properties($_[0], (properties_info)[2..3]);	
				} elsif (($_[0]->{version} == 118) && ($_[0]->{script_version} >= 5)) {
					$_[1]->pack_properties($_[0], (properties_info)[1]);	
				}
			}
		}
	}
}
sub update_read {
	if (($_[0]->{section_name} eq 'm_rat_e') && ($_[0]->{version} <= 104)) {
		cse_alife_monster_rat::update_read(@_);
	} else {
		cse_alife_monster_base::update_read(@_);
	}
}
sub update_write {
	if (($_[0]->{section_name} eq 'm_rat_e') && ($_[0]->{version} <= 104)) {
		cse_alife_monster_rat::update_write(@_);
	} else {
		cse_alife_monster_base::update_write(@_);
	}
}
sub state_import {
	if (($_[0]->{section_name} eq 'm_rat_e') && ($_[0]->{version} <= 104)) {
		cse_alife_monster_rat::state_import(@_);
	} else {
		cse_alife_monster_base::state_import(@_);
		if (defined $_[0]->{script_version}) {
			if ((($_[0]->{version} >= 116) && ($_[0]->{script_version} > 3)) || ($_[0]->{script_version} == 2)) {
				$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
				if ($_[0]->{version} >= 128) {
					$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);
				} elsif ($_[0]->{version} >= 122) {
					$_[1]->import_properties($_[2], $_[0], (properties_info)[2..3]);	
				} elsif (($_[0]->{version} == 118) && ($_[0]->{script_version} >= 5)) {
					$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);	
				}
			}
		}
	}
}
sub state_export {
	if (($_[0]->{section_name} eq 'm_rat_e') && ($_[0]->{version} <= 104)) {
		cse_alife_monster_rat::state_export(@_);
	} else {
		cse_alife_monster_base::state_export(@_);
		if (defined $_[0]->{script_version}) {
			if ((($_[0]->{version} >= 116) && ($_[0]->{script_version} > 3)) || ($_[0]->{script_version} == 2)) {
				$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
				if ($_[0]->{version} >= 128) {
					$_[1]->export_properties(undef, $_[0], (properties_info)[1]);
				} elsif ($_[0]->{version} >= 122) {
					$_[1]->export_properties(undef, $_[0], (properties_info)[2..3]);	
				} elsif (($_[0]->{version} == 118) && ($_[0]->{script_version} >= 5)) {
					$_[1]->export_properties(undef, $_[0], (properties_info)[1]);	
				}
			}
		}
	}
}
#######################################################################
package cse_alife_monster_zombie;
use strict;
use constant properties_info => (
	{ name => 'field_of_view',	type => 'f32',	default => 0.0 },	# 0x1f8
	{ name => 'eye_range',		type => 'f32',	default => 0.0 },	# 0x1fc 
	{ name => 'health',	type => 'f32',	default => 1.0 },	# 0x200 
	{ name => 'minimum_speed',	type => 'f32',	default => 0.0 },	# 0x200 
	{ name => 'maximum_speed',	type => 'f32',	default => 0.0 },	# 0x204 
	{ name => 'attack_speed',	type => 'f32',	default => 0.0 },	# 0x208 
	{ name => 'pursuit_distance',	type => 'f32',	default => 0.0 },	# 0x20c 
	{ name => 'home_distance',	type => 'f32',	default => 0.0 },	# 0x210 
	{ name => 'hit_power',		type => 'f32',	default => 0.0 },	# 0x214 
	{ name => 'hit_interval',	type => 'u16',	default => 0 },		# 0x218 
	{ name => 'distance',		type => 'f32',	default => 0.0 },	# 0x21c
	{ name => 'maximum_angle',	type => 'f32',	default => 0.0 },	# 0x220
);
sub state_read {
	cse_alife_monster_abstract::state_read(@_);
	$_[1]->unpack_properties($_[0], (properties_info)[0..1]);
	if ($_[0]->{version} <= 5) {
		$_[1]->unpack_properties($_[0], (properties_info)[2]);
	}
	$_[1]->unpack_properties($_[0], (properties_info)[3..11]);
}
sub state_write {
	cse_alife_monster_abstract::state_write(@_);
	$_[1]->pack_properties($_[0], (properties_info)[0..1]);
	if ($_[0]->{version} <= 5) {
		$_[1]->pack_properties($_[0], (properties_info)[2]);
	}
	$_[1]->pack_properties($_[0], (properties_info)[3..11]);
}
sub update_read {
	cse_alife_monster_abstract::update_read(@_);
}
sub update_write {
	cse_alife_monster_abstract::update_write(@_);
}
sub state_import {
	cse_alife_monster_abstract::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], (properties_info)[0..1]);
	if ($_[0]->{version} <= 5) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[2]);
	}
	$_[1]->import_properties($_[2], $_[0], (properties_info)[3..11]);
}
sub state_export {
	cse_alife_monster_abstract::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..1]);
	if ($_[0]->{version} <= 5) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[2]);
	}
	$_[1]->export_properties(undef, $_[0], (properties_info)[3..11]);
}

#######################################################################
package cse_alife_monster_rat;
use strict;
use constant properties_info => (
	{ name => 'field_of_view',			type => 'f32', default => 120.0 },	# 0xe0
	{ name => 'eye_range',				type => 'f32', default => 10.0 },	# 0x140
	{ name => 'minimum_speed',			type => 'f32', default => 0.5 },	# 0xe0
	{ name => 'maximum_speed',			type => 'f32', default => 1.5 },	# 0x140
	{ name => 'attack_speed',			type => 'f32', default => 4.0 },	# 0xe0
	{ name => 'pursiut_distance',		type => 'f32', default => 100.0 },	# 0x140
	{ name => 'home_distance',			type => 'f32', default => 10.0 },	# 0xe0
	{ name => 'success_attack_quant',	type => 'f32', default => 20.0 },	# 0x140
	{ name => 'death_quant',			type => 'f32', default => -10.0 },	# 0xe0
	{ name => 'fear_quant',				type => 'f32', default => -20.0 },	# 0x140
	{ name => 'restore_quant',			type => 'f32', default => 10.0 },	# 0xe0
	{ name => 'restore_time_interval',	type => 'u16', default => 3000 },	# 0x140
	{ name => 'minimum_value',			type => 'f32', default => 0.0 },	# 0x140
	{ name => 'maximum_value',			type => 'f32', default => 100.0 },	# 0xe0
	{ name => 'normal_value',			type => 'f32', default => 66.0 },	# 0x140
	{ name => 'hit_power',				type => 'f32', default => 10.0 },	# 0xe0
	{ name => 'hit_interval',			type => 'u16', default => 1500 },	# 0x140
	{ name => 'distance',				type => 'f32', default => 0.7 },	# 0x140
	{ name => 'maximum_angle',			type => 'f32', default => 45.0 },	# 0x140
	{ name => 'success_probability',	type => 'f32', default => 0.5 },	# 0xe0	
	{ name => 'cse_alife_monster_rat__unk1_f32',	type => 'f32', default => 5.0 },	# 0xe0	
);
sub state_read {
	cse_alife_monster_abstract::state_read(@_);
	$_[1]->unpack_properties($_[0], (properties_info)[0..1]);
	if ($_[0]->{version} < 7) {
		$_[1]->unpack_properties($_[0], (properties_info)[20]);
	}
	$_[1]->unpack_properties($_[0], (properties_info)[2..19]);
	if ($_[0]->{version} > 39) {
	cse_alife_inventory_item::state_read(@_);
	}
}
sub state_write {
	cse_alife_monster_abstract::state_write(@_);
	$_[1]->pack_properties($_[0], (properties_info)[0..1]);
	if ($_[0]->{version} < 7) {
	$_[1]->pack_properties($_[0], (properties_info)[20]);
	}
	$_[1]->pack_properties($_[0], (properties_info)[2..19]);
	if ($_[0]->{version} > 39) {
	cse_alife_inventory_item::state_write(@_);
	}
}
sub update_read {
	cse_alife_monster_abstract::update_read(@_);
	if ($_[0]->{version} > 39) {
		cse_alife_inventory_item::update_read(@_);
	}
}
sub update_write {
	cse_alife_monster_abstract::update_write(@_);
	if ($_[0]->{version} > 39) {
		cse_alife_inventory_item::update_write(@_);
	}
}
sub state_import {
	cse_alife_monster_abstract::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], (properties_info)[0..1]);
	if ($_[0]->{version} < 7) {
	$_[1]->import_properties($_[2], $_[0], (properties_info)[20]);
	}
	$_[1]->import_properties($_[2], $_[0], (properties_info)[2..19]);
	if ($_[0]->{version} > 39) {
	cse_alife_inventory_item::state_import(@_);
	}
}
sub state_export {
	cse_alife_monster_abstract::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..1]);
	if ($_[0]->{version} < 7) {
	$_[1]->export_properties(undef, $_[0], (properties_info)[20]);
	}
	$_[1]->export_properties(undef, $_[0], (properties_info)[2..19]);
	if ($_[0]->{version} > 39) {
	cse_alife_inventory_item::state_export(@_);
	}
}
#######################################################################
package cse_alife_rat_group; 		#Define some properties!
use strict;
use constant properties_info => (
	{ name => 'cse_alife_rat_group__unk_1_u32',		type => 'u32', default => 1 },	# 0x140
	{ name => 'alife_count',						type => 'u16', default => 5 },							# confirmed	
	{ name => 'cse_alife_rat_group__unk_2_l32u16v',	type => 'l32u16v', default => [] },	# 0xe0	
);
use constant upd_properties_info => (
	{ name => 'upd:alife_count',		type => 'u32', default => 1 },	# 0x140
);
sub state_read {
	cse_alife_monster_rat::state_read(@_);
	$_[1]->unpack_properties($_[0], (properties_info)[0..1]);
	if ($_[0]->{version} > 16) {
		$_[1]->unpack_properties($_[0], (properties_info)[2]);
	}
}
sub state_write {
	cse_alife_monster_rat::state_write(@_);
	$_[1]->pack_properties($_[0], (properties_info)[0..1]);
	if ($_[0]->{version} > 16) {
		$_[1]->pack_properties($_[0], (properties_info)[2]);
	}
}
sub update_read {
	cse_alife_monster_rat::update_read(@_);
	$_[1]->unpack_properties($_[0], upd_properties_info);	
}
sub update_write {
	cse_alife_monster_rat::update_write(@_);
	$_[1]->pack_properties( $_[0], upd_properties_info);	
}
sub state_import {
	cse_alife_monster_rat::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], (properties_info)[0..1]);
	if ($_[0]->{version} > 16) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[2]);
	}
	if (not (::level())) {
		$_[1]->import_properties($_[2], $_[0],upd_properties_info);	
	}
}
sub state_export {
	cse_alife_monster_rat::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..1]);
	if ($_[0]->{version} > 16) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[2]);
	}
	if (not (::level())) {
		$_[1]->export_properties(undef, $_[0], upd_properties_info);	
	}
}
#######################################################################
package cse_alife_flesh_group; 		#Define some properties!
use strict;
use constant properties_info => (
	{ name => 'cse_alife_flash_group__unk_1_u32',		type => 'u32', default => 0 },	# 0x140
	{ name => 'alife_count',						type => 'u16', default => 0 },							# confirmed	
	{ name => 'cse_alife_flash_group__unk_2_l32u16v',	type => 'l32u16v', default => [] },	# 0xe0	
);
use constant upd_properties_info => (
	{ name => 'upd:alife_count',	type => 'u32', default => 1 },	# 0x140
);
sub state_read {
	cse_alife_monster_base::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_monster_base::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	cse_alife_monster_base::update_read(@_);
	$_[1]->unpack_properties($_[0], upd_properties_info);
}
sub update_write {
	cse_alife_monster_base::update_write(@_);
	$_[1]->pack_properties($_[0], upd_properties_info);
}
sub state_import {
	cse_alife_monster_base::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
	if (not (::level())) {
		$_[1]->import_properties($_[2], $_[0], upd_properties_info);
	}
}
sub state_export {
	cse_alife_monster_base::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	if (not (::level())) {
		$_[1]->export_properties(undef, $_[0], upd_properties_info);
	}
}
#######################################################################
package cse_alife_space_restrictor;
use constant eDefaultRestrictorTypeNone	=> 0x00;
use constant eDefaultRestrictorTypeOut	=> 0x01;
use constant eDefaultRestrictorTypeIn	=> 0x02;
use constant eRestrictorTypeNone	=> 0x03;
use constant eRestrictorTypeIn		=> 0x04;
use constant eRestrictorTypeOut		=> 0x05;
use strict;
use constant properties_info => (
	{ name => 'restrictor_type', type => 'u8', default => 0xff },
);
sub state_read {
	if ($_[0]->{version} > 14) {
		cse_alife_object::state_read(@_);
	}
	cse_shape::state_read(@_);
	if ($_[0]->{version} > 74) {
		$_[1]->unpack_properties($_[0], properties_info);
	}
}
sub state_write {
	if ($_[0]->{version} > 14) {
	cse_alife_object::state_write(@_);
	}
	cse_shape::state_write(@_);
	if ($_[0]->{version} > 74) {
		$_[1]->pack_properties($_[0], properties_info);
	}
}
sub state_import {
	if ($_[0]->{version} > 14) {
	cse_alife_object::state_import(@_);
	}
	cse_shape::state_import(@_);
	if ($_[0]->{version} > 74) {
		$_[1]->import_properties($_[2], $_[0], properties_info);
	}
}
sub state_export {
	if ($_[0]->{version} > 14) {
	cse_alife_object::state_export(@_);
	}
	cse_shape::state_export(@_);
	if ($_[0]->{version} > 74) {
		$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	}
}
#######################################################################
package cse_alife_smart_zone;
use strict;
sub state_read {
	cse_alife_space_restrictor::state_read(@_);
}
sub state_write {
	cse_alife_space_restrictor::state_write(@_);
}
sub state_import {
	cse_alife_space_restrictor::state_import(@_);
}
sub state_export {
	cse_alife_space_restrictor::state_export(@_);
}
#######################################################################
package cse_smart_cover;
use strict;
use constant properties_info => (
	{ name => 'description',	type => 'sz',	default => '' },	# 0xec
	{ name => 'hold_position_time',	type => 'f32',	default => 0.0 },	# 0xf0
	{ name => 'enter_min_enemy_distance',	type => 'f32',	default => 0.0 },	# 0xf4
	{ name => 'exit_min_enemy_distance',	type => 'f32',	default => 0.0 },	# 0xf8
	{ name => 'is_combat_cover',		type => 'u8',	default => 0 },		# 0xfc
	{ name => 'MP_respawn',	type => 'u8',	default => 0 },		# 0x100
);
sub state_read {
	cse_alife_dynamic_object::state_read(@_);
	cse_shape::state_read(@_);
	$_[1]->unpack_properties($_[0], (properties_info)[0..1]);
	if ($_[0]->{version} >= 120) {
		$_[1]->unpack_properties($_[0], (properties_info)[2..3]);
	}
	if ($_[0]->{version} >= 122) {
		$_[1]->unpack_properties($_[0], (properties_info)[4]);
	}
	if ($_[0]->{version} >= 128) {
		$_[1]->unpack_properties($_[0], (properties_info)[5]);
	}
}
sub state_write {
	cse_alife_dynamic_object::state_write(@_);
	cse_shape::state_write(@_);
	$_[1]->pack_properties($_[0], (properties_info)[0..1]);
	if ($_[0]->{version} >= 120) {
		$_[1]->pack_properties($_[0], (properties_info)[2..3]);
	}
	if ($_[0]->{version} >= 122) {
		$_[1]->pack_properties($_[0], (properties_info)[4]);
	}
	if ($_[0]->{version} >= 128) {
		$_[1]->pack_properties($_[0], (properties_info)[5]);
	}
}
sub state_import {
	cse_alife_dynamic_object::state_import(@_);
	cse_shape::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], (properties_info)[0..1]);
	if ($_[0]->{version} >= 120) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[2..3]);
	}
	if ($_[0]->{version} >= 122) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[4]);
	}
	if ($_[0]->{version} >= 128) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[5]);
	}
}
sub state_export {
	cse_alife_dynamic_object::state_export(@_);
	cse_shape::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..1]);
	if ($_[0]->{version} >= 120) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[2..3]);
	}
	if ($_[0]->{version} >= 122) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[4]);
	}
	if ($_[0]->{version} >= 128) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[5]);
	}
}
#######################################################################
package se_smart_cover;
use strict;
use constant properties_info => (
	{ name => 'last_description',	type => 'sz',		default => '' },
	{ name => 'loopholes',		type => 'l8szbv',	default => [] }
);
sub state_read {
	cse_smart_cover::state_read(@_);
	if ($_[0]->{version} >=128) {
		$_[1]->unpack_properties($_[0], properties_info);
	}
};
sub state_write {
	cse_smart_cover::state_write(@_);
	if ($_[0]->{version} >=128) {
		$_[1]->pack_properties($_[0], properties_info);
	}
}
sub state_import {
	cse_smart_cover::state_import(@_);
	if ($_[0]->{version} >=128) {
		$_[1]->import_properties($_[2], $_[0], properties_info);
	}
}
sub state_export {
	cse_smart_cover::state_export(@_);
	if ($_[0]->{version} >=128) {
		$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	}
}
#######################################################################
package se_anomaly_field;
use strict;
use stkutils::debug;
use constant properties_info => (
	{ name => 'startup',		type => 'u8',	default => 1 },
	{ name => 'update_time_present',type => 'u8',	default => 0 },
	{ name => 'zone_count',		type => 'u8',	default => 0 },
);
sub state_read {
	cse_alife_space_restrictor::state_read(@_);
$_[1]->length() == 3 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[1]->length() == 3', 'unexpected size');
	$_[1]->unpack_properties($_[0], (properties_info)[0]);
	$_[0]->{startup} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{startup} == 0', 'unexpected value');
	$_[1]->unpack_properties($_[0], (properties_info)[1]);
	$_[0]->{update_time_present} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{update_time_present} == 0', 'unexpected value');
	$_[1]->unpack_properties($_[0], (properties_info)[2]);
	$_[0]->{zone_count} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{zone_count} == 0', 'unexpected value');
}
sub state_write {
	cse_alife_space_restrictor::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_space_restrictor::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_space_restrictor::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}
#######################################################################
package cse_alife_spawn_group; 													
use strict;
use constant properties_info => (
	{ name => 'group_probability', type => 'f32'},
);
sub state_read {
	if ($_[0]->{version} <= 79) {
		$_[1]->unpack_properties($_[0], properties_info);
	}
}
sub state_write {
	if ($_[0]->{version} <= 79) {
		$_[1]->pack_properties($_[0], properties_info);
	}
}
sub state_import {
	if ($_[0]->{version} <= 79) {
		$_[1]->import_properties($_[2], $_[0], properties_info);
	}
}
sub state_export {
	if ($_[0]->{version} <= 79) {
		$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	}
}
#######################################################################
package se_respawn;
use strict;
use stkutils::debug;
use constant properties_info => (
	{ name => 'spawned_obj_count', type => 'u8', default => 0 },
);
sub state_read {
	cse_alife_smart_zone::state_read(@_);
	if ($_[0]->{version} >= 116) {
	$_[1]->length() == 1 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[1]->length() == 1', 'unexpected size');
		$_[1]->unpack_properties($_[0], properties_info);
		$_[0]->{spawned_obj_count} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{spawned_obj_count} == 0', 'unexpected value');
	}
}
sub state_write {
	cse_alife_smart_zone::state_write(@_);
	if ($_[0]->{version} >= 116) {
		$_[1]->pack_properties($_[0], properties_info);
	}
}
sub state_import {
	cse_alife_smart_zone::state_import(@_);
	if ($_[0]->{version} >= 116) {
		$_[1]->import_properties($_[2], $_[0], properties_info);
	}
}
sub state_export {
	cse_alife_smart_zone::state_export(@_);
	if ($_[0]->{version} >= 116) {
		$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	}
}
#######################################################################
package se_sim_faction;
use strict;
use stkutils::debug;
use constant properties_info => (
	{ name => 'community_player', type => 'u8' },
	{ name => 'start_position_filled', type => 'u8' },
	{ name => 'current_expansion_level', type => 'u8' },	
	{ name => 'last_spawn_time_marker', type => 'u8', default => 255 },	#{ name => 'last_spawn_time', type => 'u8v8' },		
	{ name => 'squad_target_cache_count', type => 'u8' }, #next squad_target_cache collection
	{ name => 'random_tasks_count', type => 'u8' }, #next random_tasks collection
	{ name => 'current_attack_quantity_count', type => 'u8' }, #next current_attack_quantity collection
	{ name => 'squads_count', type => 'u16' }, #next squads collection
	{ name => 'se_sim_faction__marker', type => 'u16', default => 9},
);
sub state_read {
	cse_alife_smart_zone::state_read(@_);
	if ($_[0]->{version} >= 122) {
		$_[1]->unpack_properties($_[0], (properties_info)[0..7]);
		$_[0]->{last_spawn_time_marker} == 255 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{last_spawn_time_marker} == 255', 'unexpected value');
		$_[0]->{squad_target_cache_count} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{squad_target_cache_count} == 0', 'unexpected value');
		$_[0]->{random_tasks_count} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{random_tasks_count} == 0', 'unexpected value');
		$_[0]->{current_attack_quantity_count} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{current_attack_quantity_count} == 0', 'unexpected value');
		$_[0]->{squads_count} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{squads_count} == 0', 'unexpected value');
		if (($_[0]->{version} >= 124) || (($_[0]->{version} == 123) && (::level()))) {
			$_[1]->unpack_properties($_[0], (properties_info)[8]);
		}
	}
}
sub state_write {
	cse_alife_smart_zone::state_write(@_);
	if ($_[0]->{version} >= 122) {
		$_[1]->pack_properties($_[0], (properties_info)[0..7]);
		if ($_[0]->{version} >= 124) {
			$_[1]->pack_properties($_[0], (properties_info)[8]);
		}
	}
}
sub state_import {
	cse_alife_smart_zone::state_import(@_);
	if ($_[0]->{version} >= 122) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0..7]);
		if ($_[0]->{version} >= 124) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[8]);
		}
	}
}
sub state_export {
	cse_alife_smart_zone::state_export(@_);
	if ($_[0]->{version} >= 122) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..7]);
		if ($_[0]->{version} >= 124) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[8]);
		}
	}
}
#######################################################################
package se_smart_terrain;
use strict;
use stkutils::debug;
use constant properties_info => (
###SOC
	{ name => 'duration_end_present',	type => 'u8',	default => 0 },
	{ name => 'idle_end_present',		type => 'u8',	default => 0 },
	{ name => 'gulag_working',		type => 'u8',	default => 0 },
###CS	
	{ name => 'actor_defence_come',	type => 'u8' },	
	{ name => 'combat_quest',	type => 'sz' },		
	{ name => 'task',	type => 'u16' },	
	{ name => 'see_actor_enemy',	type => 'sz' },	
	{ name => 'flag',	type => 'u8' },	#next CTime
	{ name => 'squads_count',	type => 'u8' },	#next squads collection
	{ name => 'force_online',	type => 'u8' },
	{ name => 'force_online_squads_count',	type => 'u8' }, #next force_online_squads collection
	{ name => 'cover_manager_is_valid',	type => 'u8' },
	{ name => 'cover_manager_cover_table_count',	type => 'u8' }, #next cover_table collection
	{ name => 'se_smart_terrain_combat_manager_cover_manager__marker',	type => 'u16', default => 2}, 
	{ name => 'se_smart_terrain_combat_manager__marker',	type => 'u16', default => 19}, 
	{ name => 'npc_info_count',	type => 'u8',	default => 0 },	#next npc_info collection
	{ name => 'dead_time_count',	type => 'u8',	default => 0 },	#next dead_time collection
	{ name => 'se_smart_terrain__marker',	type => 'u16', default => 23}, 	
####COP
	{ name => 'arriving_npc_count',			type => 'u8',	default => 0 },
	{ name => 'base_on_actor_control_present',	type => 'u8',	default => 0 },
	{ name => 'is_respawn_point',	type => 'u8',	default => 0 },
	{ name => 'population',		type => 'u8',	default => 0 },

);
sub state_read {
	cse_alife_smart_zone::state_read(@_);
	if ($_[0]->{version} < 122) {
		$_[1]->unpack_properties($_[0], (properties_info)[0]);
		$_[0]->{duration_end_present} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{duration_end_present} == 0', 'unexpected value');
		$_[1]->unpack_properties($_[0], (properties_info)[1]);
		$_[0]->{idle_end_present} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{idle_end_present} == 0', 'unexpected value');
		$_[1]->unpack_properties($_[0], (properties_info)[2]);
		$_[0]->{gulag_working} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{gulag_working} == 0', 'unexpected value');
		if ($_[0]->{script_version} <= 2) {
			$_[1]->unpack_properties($_[0], (properties_info)[21]);
		}
	} elsif (($_[0]->{version} >= 122) && ($_[0]->{version} < 128)) {
		$_[1]->unpack_properties($_[0], (properties_info)[3..12]);
		if ((($_[0]->{version} >= 123) && (::level())) || ($_[0]->{version} >= 124)) {
			$_[1]->unpack_properties($_[0], (properties_info)[13..17]);
		} else {
			$_[1]->unpack_properties($_[0], (properties_info)[15..16]);
		}
	} else {
		::set_save_marker($_[1], 'load', 0, 'se_smart_terrain');
		$_[1]->unpack_properties($_[0], (properties_info)[18]);
		$_[0]->{arriving_npc_count} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{arriving_npc_count} == 0', 'unexpected value');
		$_[1]->unpack_properties($_[0], (properties_info)[15]);
		$_[0]->{npc_info_count} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{npc_info_count} == 0', 'unexpected value');
		$_[1]->unpack_properties($_[0], (properties_info)[16]);
		$_[0]->{dead_time_count} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{dead_time_count} == 0', 'unexpected value');
		$_[1]->unpack_properties($_[0], (properties_info)[19]);
		$_[0]->{base_on_actor_control_present} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{base_on_actor_control_present} == 0', 'unexpected value');
		$_[1]->unpack_properties($_[0], (properties_info)[20]);
		$_[0]->{is_respawn_point} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{is_respawn_point} == 0', 'unexpected value');
		$_[1]->unpack_properties($_[0], (properties_info)[21]);
		::set_save_marker($_[1], 'load', 1, 'se_smart_terrain');
	}
}
sub state_write {
	cse_alife_smart_zone::state_write(@_);
	if ($_[0]->{version} < 122) {
		$_[1]->pack_properties($_[0], (properties_info)[0..2]);
		if ($_[0]->{script_version} <= 2) {
			$_[1]->pack_properties($_[0], (properties_info)[21]);
		}
	} elsif (($_[0]->{version} >= 122) && ($_[0]->{version} < 128)) {
		$_[1]->pack_properties($_[0], (properties_info)[3..12]);
		if ((($_[0]->{version} >= 123) && (::level())) || ($_[0]->{version} >= 124)) {
			$_[1]->pack_properties($_[0], (properties_info)[13..17]);
		} else {
			$_[1]->pack_properties($_[0], (properties_info)[15..16]);
		}
	} else {
		::set_save_marker($_[1], 'save', 0, 'se_smart_terrain');
		$_[1]->pack_properties($_[0], (properties_info)[18]);
		$_[1]->pack_properties($_[0], (properties_info)[15 .. 16]);
		$_[1]->pack_properties($_[0], (properties_info)[19 .. 21]);
		::set_save_marker($_[1], 'save', 1, 'se_smart_terrain');
	}
}
sub state_import {
	cse_alife_smart_zone::state_import(@_);
	if ($_[0]->{version} < 122) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0..2]);
		if ($_[0]->{script_version} <= 2) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[21]);
		}
	} elsif (($_[0]->{version} >= 122) && ($_[0]->{version} < 128)) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[3..12]);
		if ((($_[0]->{version} >= 123) && (::level())) || ($_[0]->{version} >= 124)) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[13..17]);
		} else {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[15..16]);
		}
	} else {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[18]);
		$_[1]->import_properties($_[2], $_[0], (properties_info)[15 .. 16]);
		$_[1]->import_properties($_[2], $_[0], (properties_info)[19 .. 21]);
	}
}
sub state_export {
	cse_alife_smart_zone::state_export(@_);
	if ($_[0]->{version} < 122) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..2]);
		if ($_[0]->{script_version} <= 2) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[21]);
		}
	} elsif (($_[0]->{version} >= 122) && ($_[0]->{version} < 128)) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[3..12]);
		if ((($_[0]->{version} >= 123) && (::level())) || ($_[0]->{version} >= 124)) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[13..17]);
		} else {
			$_[1]->export_properties(undef, $_[0], (properties_info)[15..16]);
		}
	} else {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[18]);
		$_[1]->export_properties(undef, $_[0], (properties_info)[15 .. 16]);
		$_[1]->export_properties(undef, $_[0], (properties_info)[19 .. 21]);
	}
}
#######################################################################
package cse_alife_team_base_zone;
use strict;
use constant properties_info => (
	{ name => 'team', type => 'u8', default => 0 },
);
sub state_read {
	cse_alife_space_restrictor::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_space_restrictor::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_space_restrictor::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_space_restrictor::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}
#######################################################################
package cse_alife_level_changer;
use strict;
use constant properties_info => (
	{ name => 'cse_alife_level_changer__unk1_s32',	type => 's32',	default => -1 },
	{ name => 'cse_alife_level_changer__unk2_s32',	type => 's32',	default => -1 },
	{ name => 'dest_game_vertex_id',	type => 'u16',	default => 0 },
	{ name => 'dest_level_vertex_id',	type => 'u32',	default => 0 },
	{ name => 'dest_position',		type => 'f32v3',default => [] },
	{ name => 'dest_direction',		type => 'f32v3',default => [] },
	{ name => 'angle_y',		type => 'f32',default => 0.0 },
	{ name => 'dest_level_name',		type => 'sz',	default => '' },
	{ name => 'dest_graph_point',		type => 'sz',	default => '' },
	{ name => 'silent_mode',		type => 'u8',	default => 0 },
);
sub state_read {
	cse_alife_space_restrictor::state_read(@_);
	if ($_[0]->{version} < 34) {
		$_[1]->unpack_properties($_[0], (properties_info)[0..1]);
	} else {
		$_[1]->unpack_properties($_[0], (properties_info)[2..4]);
		if ($_[0]->{version} > 53) {
			$_[1]->unpack_properties($_[0], (properties_info)[5]);
		} else {
			$_[1]->unpack_properties($_[0], (properties_info)[6]);
		}
	}
	$_[1]->unpack_properties($_[0], (properties_info)[7..8]);
	if ($_[0]->{version} > 116) {
		$_[1]->unpack_properties($_[0], (properties_info)[9]);
	}
}
sub state_write {
	cse_alife_space_restrictor::state_write(@_);
	if ($_[0]->{version} < 34) {
		$_[1]->pack_properties($_[0], (properties_info)[0..1]);
	} else {
		$_[1]->pack_properties($_[0], (properties_info)[2..4]);
		if ($_[0]->{version} > 53) {
			$_[1]->pack_properties($_[0], (properties_info)[5]);
		} else {
			$_[1]->pack_properties($_[0], (properties_info)[6]);
		}
	}
	$_[1]->pack_properties($_[0], (properties_info)[7..8]);
	if ($_[0]->{version} > 116) {
		$_[1]->pack_properties($_[0], (properties_info)[9]);
	}
}
sub state_import {
	cse_alife_space_restrictor::state_import(@_);
	if ($_[0]->{version} < 34) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0..1]);
	} else {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[2..4]);
		if ($_[0]->{version} > 53) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[5]);
		} else {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[6]);
		}
	}
	$_[1]->import_properties($_[2], $_[0], (properties_info)[7..8]);
	if ($_[0]->{version} > 116) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[9]);
	}
}
sub state_export {
	cse_alife_space_restrictor::state_export(@_);
	if ($_[0]->{version} < 34) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..1]);
	} else {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[2..4]);
		if ($_[0]->{version} > 53) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[5]);
		} else {
			$_[1]->export_properties(undef, $_[0], (properties_info)[6]);
		}
	}
	$_[1]->export_properties(undef, $_[0], (properties_info)[7..8]);
	if ($_[0]->{version} > 116) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[9]);
	}
}
#######################################################################
package se_level_changer;
use strict;
use constant properties_info => (
	{ name => 'enabled',	type => 'u8',	default => 1 },
	{ name => 'hint',	type => 'sz',	default => 'level_changer_invitation' },
);
sub state_read {
	cse_alife_level_changer::state_read(@_);
	if ((($_[0]->{version} >= 123) && (::level())) || ($_[0]->{version} >= 124)) {
		::set_save_marker($_[1], 'load', 0, 'se_level_changer');
		$_[1]->unpack_properties($_[0], properties_info);
		::set_save_marker($_[1], 'load', 1, 'se_level_changer');
	}
}
sub state_write {
	cse_alife_level_changer::state_write(@_);
	if ((($_[0]->{version} >= 123) && (::level())) || ($_[0]->{version} >= 124)) {
		::set_save_marker($_[1], 'save', 0, 'se_level_changer');
		$_[1]->pack_properties($_[0], properties_info);
		::set_save_marker($_[1], 'save', 1, 'se_level_changer');
	}
}
sub state_import {
	cse_alife_level_changer::state_import(@_);
	if ((($_[0]->{version} >= 123) && (::level())) || ($_[0]->{version} >= 124)) {
		$_[1]->import_properties($_[2], $_[0], properties_info);
	}
}
sub state_export {
	cse_alife_level_changer::state_export(@_);
	if ((($_[0]->{version} >= 123) && (::level())) || ($_[0]->{version} >= 124)) {
		$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	}
}
#######################################################################
package cse_alife_custom_zone;
use strict;
use constant properties_info => (
	{ name => 'max_power',		type => 'f32',	default => 0.0 },	# 0xf0
	{ name => 'attenuation',		type => 'f32',	default => 0.0 },# 0xf8
	{ name => 'period',	type => 'u32',	default => 0 },		# 0xfc
	{ name => 'owner_id',		type => 'h32',	default => 0xffffffff },# 0xf8
	{ name => 'enabled_time',	type => 'u32',	default => 0 },		# 0xfc
	{ name => 'disabled_time',	type => 'u32',	default => 0 },		# 0x100
	{ name => 'start_time_shift',	type => 'u32',	default => 0 },		# 0x104
);
use constant upd_properties_info => (
	{ name => 'upd:cse_alife_custom_zone__unk1_h32',		type => 'h32',	default => 0xffffffff },# 0xf8
);
sub state_read {
	cse_alife_space_restrictor::state_read(@_);
	$_[1]->unpack_properties($_[0], (properties_info)[0]);
	if ($_[0]->{version} < 113) {
		$_[1]->unpack_properties($_[0], (properties_info)[1..2]);
	}
	if (($_[0]->{version} > 66) && ($_[0]->{version} < 118)) {
		$_[1]->unpack_properties($_[0], (properties_info)[6]);
	}
	if ($_[0]->{version} > 102) {
		$_[1]->unpack_properties($_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} > 105) {
		$_[1]->unpack_properties($_[0], (properties_info)[4..5]);
	}
	if ($_[0]->{version} > 106) {
		$_[1]->unpack_properties($_[0], (properties_info)[6]);
	}
}
sub state_write {
	cse_alife_space_restrictor::state_write(@_);
	$_[1]->pack_properties($_[0], (properties_info)[0]);
	if ($_[0]->{version} < 113) {
		$_[1]->pack_properties($_[0], (properties_info)[1..2]);
	}
	if (($_[0]->{version} > 66) && ($_[0]->{version} < 118)) {
		$_[1]->pack_properties($_[0], (properties_info)[6]);
	}
	if ($_[0]->{version} > 102) {
		$_[1]->pack_properties($_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} > 105) {
		$_[1]->pack_properties($_[0], (properties_info)[4..5]);
	}
	if ($_[0]->{version} > 106) {
		$_[1]->pack_properties($_[0], (properties_info)[6]);
	}
}
sub update_read {
	if (($_[0]->{version} > 101) && ((($_[0]->{version} <= 118) && ($_[0]->{script_version} <= 5)) || (::use_hack()))) {
		$_[1]->unpack_properties($_[0], upd_properties_info);
	}
}
sub update_write {
	if (($_[0]->{version} > 101) && ((($_[0]->{version} <= 118) && ($_[0]->{script_version} <= 5)) || (::use_hack()))) {
		$_[1]->pack_properties($_[0], upd_properties_info);
	}
}
sub state_import {
	cse_alife_space_restrictor::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
	if ($_[0]->{version} < 113) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[1..2]);
	}
	if (($_[0]->{version} > 66) && ($_[0]->{version} < 118)) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[6]);
	}
	if ($_[0]->{version} > 102) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} > 105) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[4..5]);
	}
	if ($_[0]->{version} > 106) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[6]);
	}
	if ((not ::level()) && ($_[0]->{version} > 101) && ((($_[0]->{version} <= 118) && ($_[0]->{script_version} <= 5)) || (::use_hack()))) {
		$_[1]->import_properties($_[2], $_[0], upd_properties_info);
	}
}
sub state_export {
	cse_alife_space_restrictor::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
	if ($_[0]->{version} < 113) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[1..2]);
	}
	if (($_[0]->{version} > 66) && ($_[0]->{version} < 118)) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[6]);
	}
	if ($_[0]->{version} > 102) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} > 105) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[4..5]);
	}
	if ($_[0]->{version} > 106) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[6]);
	}
	if ((not ::level()) && ($_[0]->{version} > 101) && ((($_[0]->{version} <= 118) && ($_[0]->{script_version} <= 5)) || (::use_hack()))) {
		$_[1]->export_properties(undef, $_[0], upd_properties_info);
	}
}
######################################################################
package cse_alife_anomalous_zone;
use strict;
use constant properties_info => (
	{ name => 'offline_interactive_radius',	type => 'f32',	default => 0.0 },	# 0x10c
	{ name => 'artefact_birth_probability',	type => 'f32',	default => 0.0 },	# 0x10c
	{ name => 'artefact_spawns_old_format',	type => 'afspawns_u32',	default => [] },	# 0x10c
	{ name => 'artefact_spawns',	type => 'afspawns',	default => [] },	# 0x10c
	{ name => 'artefact_spawn_count',	type => 'u16',	default => 0 },		# 0x114
	{ name => 'artefact_position_offset',	type => 'h32',	default => 0 },		# 0x110
	{ name => 'start_time_shift',	type => 'u32',	default => 0 },	# 0x10c
	{ name => 'cse_alife_anomalous_zone__unk2_f32',	type => 'f32',	default => 0.0 },	# 0x10c
	{ name => 'min_start_power',	type => 'f32',	default => 0.0 },		# 0x114
	{ name => 'max_start_power',	type => 'f32',	default => 0.0 },		# 0x114
	{ name => 'power_artefact_factor',	type => 'f32',	default => 0.0 },		# 0x114
	{ name => 'owner_id',	type => 'h32',	default => 0 },		# 0x110
);
sub state_read {
	cse_alife_custom_zone::state_read(@_);
	if ($_[0]->{version} > 21) {
		$_[1]->unpack_properties($_[0], (properties_info)[0]);
		if ($_[0]->{version} < 113) {
			$_[1]->unpack_properties($_[0], (properties_info)[1]);
			if ($_[0]->{version} < 26) {
				$_[1]->unpack_properties($_[0], (properties_info)[2]);
			} else {
				$_[1]->unpack_properties($_[0], (properties_info)[3]);
			}
		}
	}
	if ($_[0]->{version} > 25) {
		$_[1]->unpack_properties($_[0], (properties_info)[4..5]);
	}
	if (($_[0]->{version} > 27) && ($_[0]->{version} < 67)) {
		$_[1]->unpack_properties($_[0], (properties_info)[6]);
	}
	if (($_[0]->{version} > 38) && ($_[0]->{version} < 113)) {
		$_[1]->unpack_properties($_[0], (properties_info)[7]);
	}
	if ($_[0]->{version} > 78 && $_[0]->{version} < 113) {
		$_[1]->unpack_properties($_[0], (properties_info)[8..10]);
	}
	if ($_[0]->{version} == 102) {
		$_[1]->unpack_properties($_[0], (properties_info)[11]);
	}
}
sub state_write {
	cse_alife_custom_zone::state_write(@_);
	if ($_[0]->{version} > 21) {
		$_[1]->pack_properties($_[0], (properties_info)[0]);
		if ($_[0]->{version} < 113) {
			$_[1]->pack_properties($_[0], (properties_info)[1]);
			if ($_[0]->{version} < 26) {
				$_[1]->pack_properties($_[0], (properties_info)[2]);
			} else {
				$_[1]->pack_properties($_[0], (properties_info)[3]);
			}
		}
	}
	if ($_[0]->{version} > 25) {
		$_[1]->pack_properties($_[0], (properties_info)[4..5]);
	}
	if (($_[0]->{version} > 27) && ($_[0]->{version} < 67)) {
		$_[1]->pack_properties($_[0], (properties_info)[6]);
	}
	if (($_[0]->{version} > 38) && ($_[0]->{version} < 113)) {
		$_[1]->pack_properties($_[0], (properties_info)[7]);
	}
	if ($_[0]->{version} > 78 && $_[0]->{version} < 113) {
		$_[1]->pack_properties($_[0], (properties_info)[8..10]);
	}
	if ($_[0]->{version} == 102) {
		$_[1]->pack_properties($_[0], (properties_info)[11]);
	}
}
sub update_read {
	cse_alife_custom_zone::update_read(@_);
}
sub update_write {
	cse_alife_custom_zone::update_write(@_);
}
sub state_import {
	cse_alife_custom_zone::state_import(@_);
	if ($_[0]->{version} > 21) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
		if ($_[0]->{version} < 113) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);
			if ($_[0]->{version} < 26) {
				$_[1]->import_properties($_[2], $_[0], (properties_info)[2]);
			} else {
				$_[1]->import_properties($_[2], $_[0], (properties_info)[3]);
			}
		}
	}
	if ($_[0]->{version} > 25) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[4..5]);
	}
	if (($_[0]->{version} > 27) && ($_[0]->{version} < 67)) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[6]);
	}
	if (($_[0]->{version} > 38) && ($_[0]->{version} < 113)) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[7]);
	}
	if ($_[0]->{version} > 78 && $_[0]->{version} < 113) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[8..10]);
	}
	if ($_[0]->{version} == 102) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[11]);
	}
}
sub state_export {
	cse_alife_custom_zone::state_export(@_);
	if ($_[0]->{version} > 21) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
		if ($_[0]->{version} < 113) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[1]);
			if ($_[0]->{version} < 26) {
				$_[1]->export_properties(undef, $_[0], (properties_info)[2]);
			} else {
				$_[1]->export_properties(undef, $_[0], (properties_info)[3]);
			}
		}
	}
	if ($_[0]->{version} > 25) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[4..5]);
	}
	if (($_[0]->{version} > 27) && ($_[0]->{version} < 67)) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[6]);
	}
	if (($_[0]->{version} > 38) && ($_[0]->{version} < 113)) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[7]);
	}
	if ($_[0]->{version} > 78 && $_[0]->{version} < 113) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[8..10]);
	}
	if ($_[0]->{version} == 102) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[11]);
	}
}
#######################################################################
package se_zone_anom;
use strict;
use stkutils::debug;
use constant properties_info => (
	{ name => 'last_spawn_time_present', type => 'u8', default => 0 },
);
sub state_read {
	cse_alife_anomalous_zone::state_read(@_);
	return if (($_[0]->{version} < 128) && (substr($_[0]->{section_name}, 0, 10) eq 'zone_field'));
	if ($_[0]->{version} >= 118) {
		$_[1]->unpack_properties($_[0], properties_info);
		if (ref($_[0]) eq 'se_zone_anom') {
			$_[0]->{last_spawn_time_present} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{last_spawn_time_present} == 0', 'unexpected value');
		}
	}
}
sub state_write {
	cse_alife_anomalous_zone::state_write(@_);
	return if (($_[0]->{version} < 128) && (substr($_[0]->{section_name}, 0, 10) eq 'zone_field'));
	if ($_[0]->{version} >= 118) {
		$_[1]->pack_properties($_[0], properties_info);
	}
}
sub update_read {
	cse_alife_anomalous_zone::update_read(@_);
}
sub update_write {
	cse_alife_anomalous_zone::update_write(@_);
}
sub state_import {
	cse_alife_anomalous_zone::state_import(@_);
	return if (($_[0]->{version} < 128) && (substr($_[0]->{section_name}, 0, 10) eq 'zone_field'));
	if ($_[0]->{version} >= 118) {
		$_[1]->import_properties($_[2], $_[0], properties_info);
	}
}
sub state_export {
	cse_alife_anomalous_zone::state_export(@_);
	return if (($_[0]->{version} < 128) && (substr($_[0]->{section_name}, 0, 10) eq 'zone_field'));
	if ($_[0]->{version} >= 118) {
		$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	}
}
#######################################################################
package cse_alife_zone_visual;
use strict;
use constant properties_info => (
	{ name => 'idle_animation',	type => 'sz',	default => '' },	# 0x120
	{ name => 'attack_animation',	type => 'sz',	default => '' },	# 0x128
);
sub state_read {
	cse_alife_anomalous_zone::state_read(@_);
	if (($_[0]->{version} > 104) || ($_[0]->{section_name} eq 'zone_burning_fuzz1')) {
	cse_visual::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
	}
}
sub state_write {
	cse_alife_anomalous_zone::state_write(@_);
	if (($_[0]->{version} > 104) || ($_[0]->{section_name} eq 'zone_burning_fuzz1')) {
	cse_visual::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
	}
}
sub update_read {
	cse_alife_custom_zone::update_read(@_);
}
sub update_write {
	cse_alife_custom_zone::update_write(@_);
}
sub state_import {
	cse_alife_anomalous_zone::state_import(@_);
	if (($_[0]->{version} > 104) || ($_[0]->{section_name} eq 'zone_burning_fuzz1')) {
	cse_visual::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
	}
}
sub state_export {
	cse_alife_anomalous_zone::state_export(@_);
	if (($_[0]->{version} > 104) || ($_[0]->{section_name} eq 'zone_burning_fuzz1')) {
	cse_visual::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	}
}
#######################################################################
package se_zone_visual;
use strict;
use stkutils::debug;
use constant properties_info => (
	{ name => 'last_spawn_time_present', type => 'u8', default => 0 },
);
sub state_read {
	cse_alife_zone_visual::state_read(@_);
	if ($_[0]->{version} >= 118) {
	$_[1]->length() == 1 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[1]->length() == 1', 'unexpected size');
		$_[1]->unpack_properties($_[0], properties_info);
		$_[0]->{last_spawn_time_present} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{last_spawn_time_present} == 0', 'unexpected value');
	}
}
sub state_write {
	cse_alife_zone_visual::state_write(@_);
	if ($_[0]->{version} >= 118) {
		$_[1]->pack_properties($_[0], properties_info);
	}
}
sub update_read {
	cse_alife_zone_visual::update_read(@_);
}
sub update_write {
	cse_alife_zone_visual::update_write(@_);
}
sub state_import {
	cse_alife_zone_visual::state_import(@_);
	if ($_[0]->{version} >= 118) {
		$_[1]->import_properties($_[2], $_[0], properties_info);
	}
}
sub state_export {
	cse_alife_zone_visual::state_export(@_);
	if ($_[0]->{version} >= 118) {
		$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	}
}
#######################################################################
package cse_alife_torrid_zone;
use strict;
sub state_read {
	cse_alife_custom_zone::state_read(@_);
	cse_motion::state_read(@_);
}
sub state_write {
	cse_alife_custom_zone::state_write(@_);
	cse_motion::state_write(@_);
}
sub update_read {
	cse_alife_custom_zone::update_read(@_);
}
sub update_write {
	cse_alife_custom_zone::update_write(@_);
}
sub state_import {
	cse_alife_custom_zone::state_import(@_);
	cse_motion::state_import(@_);
}
sub state_export {
	cse_alife_custom_zone::state_export(@_);
	cse_motion::state_export(@_);
}
#######################################################################
package se_zone_torrid;
use strict;
use stkutils::debug;
use constant properties_info => (
	{ name => 'last_spawn_time_present', type => 'u8', default => 0 },
);
sub state_read {
	cse_alife_torrid_zone::state_read(@_);
	if ($_[0]->{version} >= 128) {
		$_[1]->unpack_properties($_[0], properties_info);
		$_[0]->{last_spawn_time_present} == 0 or stkutils::debug::fail(__PACKAGE__.'::state_read', __LINE__, '$_[0]->{last_spawn_time_present} == 0', 'unexpected value');
	}
}
sub state_write {
	cse_alife_torrid_zone::state_write(@_);
	if ($_[0]->{version} >= 128) {
		$_[1]->pack_properties($_[0], properties_info);
	}
}
sub update_read {
	cse_alife_torrid_zone::update_read(@_);
}
sub update_write {
	cse_alife_torrid_zone::update_write(@_);
}
sub state_import {
	cse_alife_torrid_zone::state_import(@_);
	if ($_[0]->{version} >= 128) {
		$_[1]->import_properties($_[2], $_[0], properties_info);
	}
}
sub state_export {
	cse_alife_torrid_zone::state_export(@_);
	if ($_[0]->{version} >= 128) {
		$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	}
}
#######################################################################
package cse_alife_online_offline_group;
use strict;
use constant properties_info => (
	{ name => 'members', type => 'l32u8v', default => [] },
);
sub state_read {
	cse_alife_dynamic_object::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_dynamic_object::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
#######################################################################
package cse_alife_inventory_item;
use strict;
use stkutils::debug;
#use constant FLAG_NO_POSITION => 0x8000,
use constant properties_info => (
	{ name => 'condition', type => 'f32', default => 0.0 },		# 0xec
	{ name => 'upgrades', type => 'l32szv', default => [] },		# 0xec
);
use constant upd_properties_info => (
	{ name => 'upd:num_items',			type => 'h8' },		# 0x90
	{ name => 'upd:force',			type => 'f32v3' },	# 0x58
	{ name => 'upd:torque',			type => 'f32v3' },	# 0x58
	{ name => 'upd:position',			type => 'f32v3' },	# 0x58
	{ name => 'upd:quaternion',			type => 'f32v4' },	# 0x5
	{ name => 'upd:angular_velocity',			type => 'f32v3' },	# 0x58
	{ name => 'upd:linear_velocity',			type => 'f32v3' },	# 0x58
	{ name => 'upd:enabled',			type => 'u8'},	# 0x58
	{ name => 'upd:quaternion_SOC',			type => 'q8v4' },	# 0x5
	{ name => 'upd:angular_velocity_SOC',	type => 'q8v3' },	# 0x28,0x2c,0x30
	{ name => 'upd:linear_velocity_SOC',	type => 'q8v3' },	# 0x28,0x2c,0x30
	{ name => 'upd:condition',			type => 'f32' },		# 0x90
	{ name => 'upd:timestamp',			type => 'u32' },	# 0x58
	{ name => 'upd:num_items_old_format',			type => 'u16' },	# 0x58
	{ name => 'upd:cse_alife_inventory_item__unk1_u8',			type => 'u8' },	# 0x58
);
sub state_read {
	if ($_[0]->{version} > 52) {
		$_[1]->unpack_properties($_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 123) {
		$_[1]->unpack_properties($_[0], (properties_info)[1]);
	}
}
sub state_write {
	if ($_[0]->{version} > 52) {
		$_[1]->pack_properties($_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 123) {
		$_[1]->pack_properties($_[0], (properties_info)[1]);
	}
}
sub update_read {
	if (($_[0]->{version} >= 122) && ($_[0]->{version} <= 128)) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[0]);
		if ($_[0]->{'upd:num_items'} != 0) {
			$_[1]->unpack_properties($_[0], (upd_properties_info)[1..4]);
			my $flags = $_[0]->{'upd:num_items'} >> 5;
			if (($flags & 0x2) == 0) {
				$_[1]->unpack_properties($_[0], (upd_properties_info)[5]);
			}
			if (($flags & 0x4) == 0) {
				$_[1]->unpack_properties($_[0], (upd_properties_info)[6]);
			}
			if ($_[1]->length() != 0) {
				$_[1]->unpack_properties($_[0], (upd_properties_info)[7]);
			}
		}
	} elsif ((($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5)) && (not (::use_hack()))) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[0]);
		if ($_[0]->{'upd:num_items'} != 0) {
			$_[1]->unpack_properties($_[0], (upd_properties_info)[3]);
			$_[1]->unpack_properties($_[0], (upd_properties_info)[8]);
			my $flags = $_[0]->{'upd:num_items'} >> 5;
			if ($_[0]->{is_first_patch} || (($flags & 0x02) == 0)) {
				stkutils::debug::fail(__PACKAGE__.'::update_read', __LINE__, '[1]->length() >= 3', 'unexpected size') unless $_[1]->length() >= 3;
				$_[1]->unpack_properties($_[0], (upd_properties_info)[9]);
			}
			if ($_[0]->{is_first_patch} || (($flags & 0x04) == 0)) {
				stkutils::debug::fail(__PACKAGE__.'::update_read', __LINE__, '[1]->length() >= 3', 'unexpected size') unless $_[1]->length() >= 3;
				$_[1]->unpack_properties($_[0], (upd_properties_info)[10]);
			}
		}
	} else {
		if (($_[0]->{version} > 59) &&($_[0]->{version} <= 63)) {
			$_[1]->unpack_properties($_[0], (upd_properties_info)[14]);
		}
		$_[1]->unpack_properties($_[0], (upd_properties_info)[11..13]);
		my $flags = $_[0]->{'upd:num_items_old_format'};
		if ($flags != 0x8000) {
			$_[1]->unpack_properties($_[0], (upd_properties_info)[3]);
		}
		if ($flags & ~0x8000) {
			$_[1]->unpack_properties($_[0], (upd_properties_info)[7]);
			$_[1]->unpack_properties($_[0], (upd_properties_info)[5..6]);
			$_[1]->unpack_properties($_[0], (upd_properties_info)[1..2]);
			$_[1]->unpack_properties($_[0], (upd_properties_info)[4]);
		}
	}
}
sub update_write {
	if (($_[0]->{version} >= 122) && ($_[0]->{version} <= 128)) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[0]);
		if ($_[0]->{'upd:num_items'} != 0) {
			$_[1]->pack_properties($_[0], (upd_properties_info)[1..4]);
			my $flags = $_[0]->{'upd:num_items'} >> 5;
			if (($flags & 0x2) == 0) {
				$_[1]->pack_properties($_[0], (upd_properties_info)[5]);
			}
			if (($flags & 0x4) == 0) {
				$_[1]->pack_properties($_[0], (upd_properties_info)[6]);
			}
			if ($_[1]->length() != 0) {
				$_[1]->pack_properties($_[0], (upd_properties_info)[7]);
			}
		}
	} elsif ((($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5)) && (not (::use_hack()))) {
	my $flags = ($_[0]->{'upd:num_items'});
	my $mask = $flags >> 5;
		$_[1]->pack_properties($_[0], (upd_properties_info)[0]);
		if ($flags != 0) {
			$_[1]->pack_properties($_[0], (upd_properties_info)[3]);
			$_[1]->pack_properties($_[0], (upd_properties_info)[8]);
			if ($_[0]->{is_first_patch} || (($mask & 0x02) == 0)) {
				$_[1]->pack_properties($_[0], (upd_properties_info)[9]);
			}
			if ($_[0]->{is_first_patch} || (($mask & 0x04) == 0)) {
				$_[1]->pack_properties($_[0], (upd_properties_info)[10]);
			}
		}
	} else {
		if (($_[0]->{version} > 59) &&($_[0]->{version} <= 63)) {
			$_[1]->pack_properties($_[0], (upd_properties_info)[14]);
		}
		$_[1]->pack_properties($_[0], (upd_properties_info)[11..13]);
		my $flags = $_[0]->{'upd:num_items_old_format'};
		if ($flags != 0x8000) {
			$_[1]->pack_properties($_[0], (upd_properties_info)[3]);
		}
		if ($flags & ~0x8000) {
			$_[1]->pack_properties($_[0], (upd_properties_info)[7]);
			$_[1]->pack_properties($_[0], (upd_properties_info)[5..6]);
			$_[1]->pack_properties($_[0], (upd_properties_info)[1..2]);
			$_[1]->pack_properties($_[0], (upd_properties_info)[4]);
		}
	}
}
sub state_import {
	if ($_[0]->{version} > 52) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 123) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);
	}
	if (not(::level())) {
		if (($_[0]->{version} >= 122) && ($_[0]->{version} <= 128)) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[0]);
			if ($_[0]->{'upd:num_items'} != 0) {
				$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[1..4]);
				my $flags = $_[0]->{'upd:num_items'} >> 5;
				if (($flags & 0x2) == 0) {
					$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[5]);
				}
				if (($flags & 0x4) == 0) {
					$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[6]);
				}
				if ($_[1]->is_value_exists($_[2], 'upd:enabled')) {
					$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[7]);
				}
			}
	} elsif ((($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5)) && (not (::use_hack()))) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[0]);
			if ($_[0]->{'upd:num_items'} != 0) {
				$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[3]);
				$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[8]);
				my $flags = $_[0]->{'upd:num_items'} >> 5;
				if ($_[0]->{is_first_patch} || (($flags & 0x02) == 0)) {
					$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[9]);
				}
				if ($_[0]->{is_first_patch} || (($flags & 0x04) == 0)) {
					$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[10]);
				}
			}
		} else {
		if (($_[0]->{version} > 59) &&($_[0]->{version} <= 63)) {
				$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[14]);
			}
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[11..13]);
			my $flags = $_[0]->{'upd:num_items_old_format'};
			if ($flags != 0x8000) {
				$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[3]);
			}
			if ($flags & ~0x8000) {
				$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[7]);
				$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[5..6]);
				$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[1..2]);
				$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[4]);
			}
		}
	}
}
sub state_export {
	if ($_[0]->{version} > 52) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 123) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[1]);
	}
	if (not(::level())) {
		if (($_[0]->{version} >= 122) && ($_[0]->{version} <= 128)) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[0]);
			if ($_[0]->{'upd:num_items'} != 0) {
				$_[1]->export_properties(undef, $_[0], (upd_properties_info)[1..4]);
				my $flags = $_[0]->{'upd:num_items'} >> 5;
				if (($flags & 0x2) == 0) {
					$_[1]->export_properties(undef, $_[0], (upd_properties_info)[5]);
				}
				if (($flags & 0x4) == 0) {
					$_[1]->export_properties(undef, $_[0], (upd_properties_info)[6]);
				}
				if (defined $_[0]->{'upd:enabled'}) {
					$_[1]->export_properties(undef, $_[0], (upd_properties_info)[7]);
				}
			}
		} elsif ((($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5)) && (not (::use_hack()))) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[0]);
			if ($_[0]->{'upd:num_items'} != 0) {
				$_[1]->export_properties(undef, $_[0], (upd_properties_info)[3]);
				$_[1]->export_properties(undef, $_[0], (upd_properties_info)[8]);
				my $flags = $_[0]->{'upd:num_items'} >> 5;
				if ($_[0]->{is_first_patch} || (($flags & 0x02) == 0)) {
					$_[1]->export_properties(undef, $_[0], (upd_properties_info)[9]);
				}
				if ($_[0]->{is_first_patch} || (($flags & 0x04) == 0)) {
					$_[1]->export_properties(undef, $_[0], (upd_properties_info)[10]);
				}
			}
		} else {
			if (($_[0]->{version} > 59) &&($_[0]->{version} <= 63)) {
				$_[1]->export_properties(undef, $_[0], (upd_properties_info)[14]);
			}
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[11..13]);
			my $flags = $_[0]->{'upd:num_items_old_format'};
			if ($flags != 0x8000) {
				$_[1]->export_properties(undef, $_[0], (upd_properties_info)[3]);
			}
			if ($flags & ~0x8000) {
				$_[1]->export_properties(undef, $_[0], (upd_properties_info)[7]);
				$_[1]->export_properties(undef, $_[0], (upd_properties_info)[5..6]);
				$_[1]->export_properties(undef, $_[0], (upd_properties_info)[1..2]);
				$_[1]->export_properties(undef, $_[0], (upd_properties_info)[4]);
			}
		}
	}
}
#######################################################################
package cse_alife_item;
use strict;
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	if ($_[0]->{version} > 39) {
	cse_alife_inventory_item::state_read(@_);
	}
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	if ($_[0]->{version} > 39) {
	cse_alife_inventory_item::state_write(@_);
	}
}
sub update_read {
	if ($_[0]->{version} > 39) {
		cse_alife_inventory_item::update_read(@_);
	}
}
sub update_write {
	if ($_[0]->{version} > 39) {
		cse_alife_inventory_item::update_write(@_);
	}
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	if ($_[0]->{version} > 39) {
	cse_alife_inventory_item::state_import(@_);
	}
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	if ($_[0]->{version} > 39) {
	cse_alife_inventory_item::state_export(@_);
	}
}
#######################################################################
package cse_alife_item_binocular;
use strict;
use constant properties_info => (
	{ name => 'cse_alife_item__unk1_s16', type => 's16', default => 0 },		# 0xec
	{ name => 'cse_alife_item__unk2_s16', type => 's16', default => 0 },		# 0xec
	{ name => 'cse_alife_item__unk3_s8', type => 's8', default => 0 },		# 0xec
);
sub state_read {
	cse_alife_item::state_read(@_);
	if ($_[0]->{version} < 37) {
		$_[1]->unpack_properties($_[0], properties_info);
	}
}
sub state_write {
	cse_alife_item::state_write(@_);
	if ($_[0]->{version} < 37) {
		$_[1]->pack_properties($_[0], properties_info);
	}
}
sub update_read {
	cse_alife_item::update_read(@_);
}
sub update_write {
	cse_alife_item::update_write(@_);
}
sub state_import {
	cse_alife_item::state_import(@_);
	if ($_[0]->{version} < 37) {
		$_[1]->import_properties($_[2], $_[0], properties_info);
	}
}
sub state_export {
	cse_alife_item::state_write(@_);
	if ($_[0]->{version} < 37) {
		$_[1]->export_properties(undef, $_[0], properties_info);
	}
}
#######################################################################
package cse_alife_item_torch;
use strict;
use constant flTorchActive		=> 0x01;
use constant flTorchNightVisionActive	=> 0x02;
use constant flTorchUnknown		=> 0x04;
use constant properties_info => (
	{ name => 'main_color',		type => 'h32',	default => 0x00ffffff },
	{ name => 'main_color_animator',type => 'sz',	default => '' },
	{ name => 'animation',	type => 'sz',	default => '$editor' },
	{ name => 'ambient_radius',	type => 'f32',	default => 0.0 },
	{ name => 'main_cone_angle',	type => 'q8',	default => 0.0 },
	{ name => 'main_virtual_size',	type => 'f32',	default => 0.0 },
	{ name => 'glow_texture',	type => 'sz',	default => '' },
	{ name => 'glow_radius',	type => 'f32',	default => 0.0 },
	{ name => 'cse_alife_object_hanging_lamp__unk3_u8',	type => 'u16',	default => 0 },
);
use constant upd_properties_info => (
	{ name => 'upd:torch_flags', type => 'u8' },	# flags -> 0x178,0x179,0x17a (0x1a0, 0x1a1)
);
sub state_read {
	if ($_[0]->{version} > 20) {
		cse_alife_item::state_read(@_);
	}
	if ($_[0]->{version} < 48) {
		$_[1]->unpack_properties($_[0], (properties_info)[0..5]);
		if ($_[0]->{version} > 40) {
			$_[1]->unpack_properties($_[0], (properties_info)[6..7]);
		}
		if ($_[0]->{version} > 45) {
			$_[1]->unpack_properties($_[0], (properties_info)[8]);
		}
	}
}
sub state_write {
	if ($_[0]->{version} > 20) {
		cse_alife_item::state_write(@_);
	}
	if ($_[0]->{version} < 48) {
		$_[1]->pack_properties($_[0], (properties_info)[0..5]);
		if ($_[0]->{version} > 40) {
			$_[1]->pack_properties($_[0], (properties_info)[6..7]);
		}
		if ($_[0]->{version} > 45) {
			$_[1]->pack_properties($_[0], (properties_info)[8]);
		}
	}
}
sub update_read {
	cse_alife_item::update_read(@_);
	$_[1]->unpack_properties($_[0], upd_properties_info);
}
sub update_write {
	cse_alife_item::update_write(@_);
	$_[1]->pack_properties($_[0], upd_properties_info);
}
sub state_import {
	if ($_[0]->{version} > 20) {
		cse_alife_item::state_import(@_);
	}
	if ($_[0]->{version} < 48) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0..5]);
		if ($_[0]->{version} > 40) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[6..7]);
		}
		if ($_[0]->{version} > 45) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[8]);
		}
	}
	if (not (::level())) {
		$_[1]->import_properties($_[2], $_[0], upd_properties_info);
	}
}
sub state_export {
	if ($_[0]->{version} > 20) {
		cse_alife_item::state_export(@_);
	}
	if ($_[0]->{version} < 48) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..5]);
		if ($_[0]->{version} > 40) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[6..7]);
		}
		if ($_[0]->{version} > 45) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[8]);
		}
	}
	if (not (::level())) {
		$_[1]->export_properties(undef, $_[0], upd_properties_info);
	}
}
#######################################################################
package cse_alife_item_detector;
use strict;
sub state_read {
	if ($_[0]->{version} > 20) {
		cse_alife_item::state_read(@_);
	}
}
sub state_write {
	if ($_[0]->{version} > 20) {
		cse_alife_item::state_write(@_);
	}
}
sub update_read {
	if ($_[0]->{version} > 20) {
	cse_alife_item::update_read(@_);
	}
}
sub update_write {
	if ($_[0]->{version} > 20) {
	cse_alife_item::update_write(@_);
	}
}
sub state_import {
	if ($_[0]->{version} > 20) {
	cse_alife_item::state_import(@_);
	}
}
sub state_export {
	if ($_[0]->{version} > 20) {
	cse_alife_item::state_export(@_);
	}
}

#######################################################################
package cse_alife_item_artefact;
use strict;
sub state_read {
	cse_alife_item::state_read(@_);
}
sub state_write {
	cse_alife_item::state_write(@_);
}
sub update_read {
	cse_alife_item::update_read(@_);
}
sub update_write {
	cse_alife_item::update_write(@_);
}
sub state_import {
	cse_alife_item::state_import(@_);
}
sub state_export {
	cse_alife_item::state_export(@_);
}
#######################################################################
package cse_alife_item_grenade;
use strict;
sub state_read {
	cse_alife_item::state_read(@_);
}
sub state_write {
	cse_alife_item::state_write(@_);
}
sub update_read {
	cse_alife_item::update_read(@_);
}
sub update_write {
	cse_alife_item::update_write(@_);
}
sub state_import {
	cse_alife_item::state_import(@_);
}
sub state_export {
	cse_alife_item::state_export(@_);
}

#######################################################################
package cse_alife_item_explosive;
use strict;
sub state_read {
	cse_alife_item::state_read(@_);
}
sub state_write {
	cse_alife_item::state_write(@_);
}
sub update_read {
	cse_alife_item::update_read(@_);
}
sub update_write {
	cse_alife_item::update_write(@_);
}
sub state_import {
	cse_alife_item::state_import(@_);
}
sub state_export {
	cse_alife_item::state_export(@_);
}
#######################################################################
package cse_alife_item_bolt;
use strict;
sub state_read {
	cse_alife_item::state_read(@_);
}
sub state_write {
	cse_alife_item::state_write(@_);
}
sub update_read {
	cse_alife_item::update_read(@_);
}
sub update_write {
	cse_alife_item::update_write(@_);
}
sub state_import {
	cse_alife_item::state_import(@_);
}
sub state_export {
	cse_alife_item::state_export(@_);
}

#######################################################################
package cse_alife_item_custom_outfit;
use strict;
use constant upd_properties_info => (
	{ name => 'upd:condition', type => 'q8' },	# 0xec
);
sub state_read {
	cse_alife_item::state_read(@_);
}
sub state_write {
	cse_alife_item::state_write(@_);
}
sub update_read {
	cse_alife_item::update_read(@_);
	if (($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5) && (not ::use_hack())) {
		$_[1]->unpack_properties($_[0], upd_properties_info);
	}
}
sub update_write {
	cse_alife_item::update_write(@_);
	if (($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5) && (not ::use_hack())) {
		$_[1]->pack_properties($_[0], upd_properties_info);
	}
}
sub state_import {
	cse_alife_item::state_import(@_);
	if (not (::level()) && ($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5) && (not ::use_hack())) {
		$_[1]->import_properties($_[2], $_[0], upd_properties_info);
	}
}
sub state_export {
	cse_alife_item::state_export(@_);
	if (not (::level()) && ($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5) && (not ::use_hack())) {
		$_[1]->export_properties(undef, $_[0], upd_properties_info);
	}
}
#######################################################################
package cse_alife_item_helmet;
use strict;
use constant upd_properties_info => (
	{ name => 'upd:condition', type => 'q8' },	# 0xec
);
sub state_read {
	cse_alife_item::state_read(@_);
}
sub state_write {
	cse_alife_item::state_write(@_);
}
sub update_read {
	cse_alife_item::update_read(@_);
	if (($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5)) {
		$_[1]->unpack_properties($_[0], upd_properties_info);
	}
}
sub update_write {
	cse_alife_item::update_write(@_);
	if (($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5)) {
		$_[1]->pack_properties($_[0], upd_properties_info);
	}
}
sub state_import {
	cse_alife_item::state_import(@_);
	if (not (::level()) && ($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5)) {
		$_[1]->import_properties($_[2], $_[0], upd_properties_info);
	}
}
sub state_export {
	cse_alife_item::state_export(@_);
	if (not (::level()) && ($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5)) {
		$_[1]->export_properties(undef, $_[0], upd_properties_info);
	}
}
#######################################################################
package cse_alife_item_pda;
use strict;
use constant properties_info => (
	{ name => 'original_owner',	type => 'u16',	default => 0 },		# 0x180
	{ name => 'specific_character',	type => 'sz',	default => '' },	# 0x184
	{ name => 'info_portion',	type => 'sz',	default => '' },	# 0x188
	{ name => 'cse_alife_item_pda__unk1_s32',	type => 's32',	default => -1 },	# 0x184
	{ name => 'cse_alife_item_pda__unk2_s32',	type => 's32',	default => -1 },	# 0x184
);
sub state_read {
	cse_alife_item::state_read(@_);
	if ($_[0]->{version} > 58) {
		$_[1]->unpack_properties($_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 89) {
		if ($_[0]->{version} < 98) {
			$_[1]->unpack_properties($_[0], (properties_info)[3..4]);
		} else {
			$_[1]->unpack_properties($_[0], (properties_info)[1..2]);
		}
	}
}
sub state_write {
	cse_alife_item::state_write(@_);
	if ($_[0]->{version} > 58) {
		$_[1]->pack_properties($_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 89) {
		if ($_[0]->{version} < 98) {
			$_[1]->pack_properties($_[0], (properties_info)[3..4]);
		} else {
			$_[1]->pack_properties($_[0], (properties_info)[1..2]);
		}
	}
}
sub update_read {
	cse_alife_item::update_read(@_);
}
sub update_write {
	cse_alife_item::update_write(@_);
}
sub state_import {
	cse_alife_item::state_import(@_);
	if ($_[0]->{version} > 58) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 89) {
		if ($_[0]->{version} < 98) {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[3..4]);
		} else {
			$_[1]->import_properties($_[2], $_[0], (properties_info)[1..2]);
		}
	}
}
sub state_export {
	cse_alife_item::state_export(@_);
	if ($_[0]->{version} > 58) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
	}
	if ($_[0]->{version} > 89) {
		if ($_[0]->{version} < 98) {
			$_[1]->export_properties(undef, $_[0], (properties_info)[3..4]);
		} else {
			$_[1]->export_properties(undef, $_[0], (properties_info)[1..2]);
		}
	}
}
#######################################################################
package cse_alife_item_document;
use strict;
use constant properties_info => (
	{ name => 'info_portion', type => 'sz', default => '' },	# 0x180
	{ name => 'cse_alife_item_document__unk1_u16', type => 'u16', default => 0 },	# 0x180
);
sub state_read {
	cse_alife_item::state_read(@_);
	if ($_[0]->{version} < 98) {
		$_[1]->unpack_properties($_[0], (properties_info)[1]);
	} else {
		$_[1]->unpack_properties($_[0], (properties_info)[0]);
	}
}
sub state_write {
	cse_alife_item::state_write(@_);
	if ($_[0]->{version} < 98) {
		$_[1]->pack_properties($_[0], (properties_info)[1]);
	} else {
		$_[1]->pack_properties($_[0], (properties_info)[0]);
	}
}
sub update_read {
	cse_alife_item::update_read(@_);
}
sub update_write {
	cse_alife_item::update_write(@_);
}
sub state_import {
	cse_alife_item::state_import(@_);
	if ($_[0]->{version} < 98) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[1]);
	} else {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0]);
	}
}
sub state_export {
	cse_alife_item::state_export(@_);
	if ($_[0]->{version} < 98) {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[1]);
	} else {
		$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0]);
	}
}
#######################################################################
package cse_alife_item_ammo;
use strict;
use constant properties_info => (
	{ name => 'ammo_left', type => 'u16', default => 0 },	#0x180
);
use constant upd_properties_info => (
	{ name => 'upd:ammo_left', type => 'u16' },	#0x180
);
sub state_read {
	cse_alife_item::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_item::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	cse_alife_item::update_read(@_);
	$_[1]->unpack_properties($_[0], upd_properties_info);
}
sub update_write {
	cse_alife_item::update_write(@_);
	$_[1]->pack_properties($_[0], upd_properties_info);
}
sub state_import {
	cse_alife_item::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
	if (not (::level())) {
	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
	}
}
sub state_export {
	cse_alife_item::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	if (not (::level())) {
	$_[1]->export_properties(undef, $_[0], upd_properties_info);
	}
}
#######################################################################
package cse_alife_item_weapon;
use strict;
use constant flAddonSilencer	=> 0x01;
use constant flAddonLauncher	=> 0x02;
use constant flAddonScope	=> 0x04;
use constant properties_info => (
	{ name => 'ammo_current',	type => 'u16',	default => 0 },	# 0x194	(0x1b4)
	{ name => 'ammo_elapsed',	type => 'u16',	default => 0 },	# 0x196 (0x1b6)
	{ name => 'weapon_state',	type => 'u8',	default => 0 },	# 0x191 (0x1b1)
	{ name => 'addon_flags',	type => 'u8',	default => 0 },	# 0x1a8 (0x1cc)
	{ name => 'ammo_type',		type => 'u8',	default => 0 },	# 0x192 (0x1b2)
	{ name => 'cse_alife_item_weapon__unk1_u8',		type => 'u8',	default => 0 },	# 0x192 (0x1b2)
);
use constant upd_properties_info => (
	{ name => 'upd:condition',	type => 'q8' },		# 0xec
	{ name => 'upd:weapon_flags',	type => 'u8' },		# 0x190 (0x1b0)
	{ name => 'upd:ammo_elapsed',	type => 'u16' },	# 0x196 (0x1b6)
	{ name => 'upd:addon_flags',	type => 'u8' },		# 0x1a8 (0x1cc)
	{ name => 'upd:ammo_type',	type => 'u8' },		# 0x192 (0x1b2)
	{ name => 'upd:weapon_state',	type => 'u8' },		# 0x191 (0x1b1)
	{ name => 'upd:weapon_zoom',	type => 'u8' },		# 0x1a9 (0x1cd)
	{ name => 'upd:ammo_current',	type => 'u16' },		# 0x1a9 (0x1cd)
	{ name => 'upd:position',	type => 'f32v3' },		# 0x1a9 (0x1cd)
	{ name => 'upd:timestamp',	type => 'u32' },		# 0x1a9 (0x1cd)
);
sub state_read {
	cse_alife_item::state_read(@_);
	$_[1]->unpack_properties($_[0], (properties_info)[0..2]);
	if ($_[0]->{version} >= 40) {
		$_[1]->unpack_properties($_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} > 46) {
		$_[1]->unpack_properties($_[0], (properties_info)[4]);
	}
	if ($_[0]->{version} > 122) {
		$_[1]->unpack_properties($_[0], (properties_info)[5]);
	}
}
sub state_write {
	cse_alife_item::state_write(@_);
	$_[1]->pack_properties($_[0], (properties_info)[0..2]);
	if ($_[0]->{version} >= 40) {
		$_[1]->pack_properties($_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} > 46) {
		$_[1]->pack_properties($_[0], (properties_info)[4]);
	}
	if ($_[0]->{version} > 122) {
		$_[1]->pack_properties($_[0], (properties_info)[5]);
	}
}
sub update_read {
	cse_alife_item::update_read(@_);
	if (($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5)) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[0]);
	}
	if ($_[0]->{version} > 39) {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[1..6]);
	} else {
		$_[1]->unpack_properties($_[0], (upd_properties_info)[9]);
		$_[1]->unpack_properties($_[0], (upd_properties_info)[1]);
		$_[1]->unpack_properties($_[0], (upd_properties_info)[7]);
		$_[1]->unpack_properties($_[0], (upd_properties_info)[2]);
		$_[1]->unpack_properties($_[0], (upd_properties_info)[8]);
		$_[1]->unpack_properties($_[0], (upd_properties_info)[3..5]);
	}
}
sub update_write {
	cse_alife_item::update_write(@_);
	if (($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5)) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[0]);
	}
	if ($_[0]->{version} > 39) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[1..6]);
	} else {
		$_[1]->pack_properties($_[0], (upd_properties_info)[9]);
		$_[1]->pack_properties($_[0], (upd_properties_info)[1]);
		$_[1]->pack_properties($_[0], (upd_properties_info)[7]);
		$_[1]->pack_properties($_[0], (upd_properties_info)[2]);
		$_[1]->pack_properties($_[0], (upd_properties_info)[8]);
		$_[1]->pack_properties($_[0], (upd_properties_info)[3..5]);
	}
}
sub state_import {
	cse_alife_item::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], (properties_info)[0..2]);
	if ($_[0]->{version} >= 40) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} > 46) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[4]);
	}
	if ($_[0]->{version} > 122) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[5]);
	}
	if (not (::level())) {
		if (($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5)) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[0]);
		}
		if ($_[0]->{version} > 39) {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[1..6]);
		} else {
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[9]);
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[1]);
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[7]);
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[2]);
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[8]);
			$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[3..5]);
		}
	}
}
sub state_export {
	cse_alife_item::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], (properties_info)[0..2]);
	if ($_[0]->{version} >= 40) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[3]);
	}
	if ($_[0]->{version} > 46) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[4]);
	}
	if ($_[0]->{version} > 122) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[5]);
	}
	if (not (::level())) {
		if (($_[0]->{version} >= 118) && ($_[0]->{script_version} > 5)) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[0]);
		}
		if ($_[0]->{version} > 39) {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[1..6]);
		} else {
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[9]);
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[1]);
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[7]);
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[2]);
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[8]);
			$_[1]->export_properties(undef, $_[0], (upd_properties_info)[3..5]);
		}
	}
}
#######################################################################
package cse_alife_item_weapon_magazined;
use strict;
use constant upd_properties_info => (
	{ name => 'upd:current_fire_mode', type => 'u8' },	# 0x1b8 (0x1d8)
);
sub state_read {
	cse_alife_item_weapon::state_read(@_);
}
sub state_write {
	cse_alife_item_weapon::state_write(@_);
}
sub update_read {
	cse_alife_item_weapon::update_read(@_);
	if ($_[1]->length() != 0) {
		$_[1]->unpack_properties($_[0], upd_properties_info);
	}
}
sub update_write {
	cse_alife_item_weapon::update_write(@_);
	if (defined $_[0]->{'upd:current_fire_mode'}) {
		$_[1]->pack_properties($_[0], upd_properties_info);
	}
}
sub state_import {
	cse_alife_item_weapon::state_import(@_);
	if (not (::level())) {
		if (defined $_[1]->value($_[2], 'upd:current_fire_mode')) {
			$_[1]->import_properties($_[2], $_[0], upd_properties_info);
		}
	}
}
sub state_export {
	cse_alife_item_weapon::state_export(@_);
	if (not (::level())) {
		if (defined $_[0]->{'upd:current_fire_mode'}) {
			$_[1]->export_properties(undef, $_[0], upd_properties_info);
		}
	}
}
#######################################################################
package cse_alife_item_weapon_magazined_w_gl;
use strict;
use constant upd_properties_info => (
	{ name => 'upd:grenade_mode', type => 'u8' },	# 0x1c0 (0x1e0)
);
sub state_read {
	cse_alife_item_weapon::state_read(@_);
}
sub state_write {
	cse_alife_item_weapon::state_write(@_);
}
sub update_read {
	if ($_[0]->{version} > 118) {
		$_[1]->unpack_properties($_[0], upd_properties_info);
	}
	cse_alife_item_weapon_magazined::update_read(@_);
	if ($_[1]->length() != 0) {
		$_[1]->unpack_properties($_[0], upd_properties_info);
	}
}
sub update_write {
	if ($_[0]->{version} > 118) {
		$_[1]->pack_properties($_[0], upd_properties_info);
	}
	cse_alife_item_weapon_magazined::update_write(@_);
	if ((defined $_[0]->{'upd:grenade_mode'}) && ($_[0]->{version} <= 118)) {
		$_[1]->pack_properties($_[0], upd_properties_info);
	}
}
sub state_import {
	cse_alife_item_weapon_magazined::state_import(@_);
	if (not (::level())) {
		if (defined $_[1]->value($_[2], 'upd:grenade_mode')) {
			$_[1]->import_properties($_[2], $_[0], upd_properties_info);
		}
	}
}
sub state_export {
	cse_alife_item_weapon_magazined::state_export(@_);
	if (not (::level())) {
		if (defined $_[0]->{'upd:grenade_mode'}) {
			$_[1]->export_properties(undef, $_[0], upd_properties_info);
		}
	}
}
#######################################################################
package cse_alife_item_weapon_shotgun;
use strict;
use stkutils::debug;
use constant upd_properties_info => (
	{ name => 'upd:ammo_ids', type => 'u8' },	# 0x1c0 (0x1e0)
);
sub state_read {
	cse_alife_item_weapon::state_read(@_);
}
sub state_write {
	cse_alife_item_weapon::state_write(@_);
}
sub update_read {
	cse_alife_item_weapon_magazined::update_read(@_);
	if ($_[1]->length() != 0) {
		$_[1]->unpack_properties($_[0], upd_properties_info);
	}
}
sub update_write {
	cse_alife_item_weapon_magazined::update_write(@_);
	if (defined $_[0]->{'upd:ammo_ids'}) {
		$_[1]->pack_properties($_[0], upd_properties_info);
	}
}
sub state_import {
	cse_alife_item_weapon_magazined::state_import(@_);
	if (not (::level())) {
		if (defined $_[1]->value($_[2], 'upd:ammo_ids')) {
			$_[1]->import_properties($_[2], $_[0], upd_properties_info);
		}
	}
}
sub state_export {
	cse_alife_item_weapon_magazined::state_export(@_);
	if (not (::level())) {
		if (defined $_[0]->{'upd:grenade_mode'}) {
			$_[1]->export_properties(undef, $_[0], upd_properties_info);
		}
	}
}
#######################################################################
package cse_target_cs_cask; 																
use strict;
use constant properties_info => (
	{ name => 'cse_target_cs_cask__unk1_u8',	type => 'u8',	default => 0 },
);
sub state_read {
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}
#######################################################################
package cse_target_cs_base; 																
use strict;
use constant properties_info => (
	{ name => 'cse_target_cs_base__unk1_f32',	type => 'f32',	default => 0 },
	{ name => 'team_id',	type => 'u8'},
);
sub state_read {
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}
#######################################################################
package build_version;
use strict;
use constant build_verions => (
	{ version => 128, script_version => 12, build => 'Call Of Pripyat (any patch)', 		short_build => 'cop',  graph_build => 'cop'},
	{ version => 124, script_version => 8, build => 'Clear Sky (patch 1.5.04 or higher)', 	short_build => 'cs4',  graph_build => 'cop'},
	{ version => 123, script_version => 8, build => 'Clear Sky (patch 1.5.03)', 			short_build => 'cs3',  graph_build => 'cop'},
	{ version => 122, script_version => 8, build => 'Clear Sky (patch 1.5.00 - 1.5.02)', 	short_build => 'cs0',  graph_build => 'cop'},
	{ version => 118, script_version => 6, build => 'Shadow Of Chernobyl (patch 1.0001 or higher)', short_build => 'soc1',  graph_build => 'soc'},
	{ version => 118, script_version => 5, build => 'xrCore build 2559-2947', 				short_build => 'soc0', graph_build => 'soc'},
	{ version => 117, script_version => 4, build => 'xrCore build 2571', graph_build => 'soc'},
	{ version => 115, script_version => 3, build => 'xrCore build 2365', graph_build => 'soc'},
	{ version => 109, script_version => 2, build => 'xrCore build 2307', graph_build => '2215'},
	{ version => 104, script_version => 2, build => 'xrCore build 2217, 2232', graph_build => '2215'},
	{ version => 103, script_version => 2, build => 'xrCore build 2198, 2191', graph_build => '2215'},
	{ version => 102, script_version => 2, build => 'xrCore build 2221', graph_build => '2215'},
	{ version => 101, script_version => 2, build => 'xrCore build 2205, 2215', graph_build => '2215'},
	{ version => 95, script_version => 1, build => 'xrCore build 2218-2201', graph_build => '1935'},
	{ version => 94, script_version => 1, build => 'xrCore build 2212-2217', graph_build => '1935'},
	{ version => 93, script_version => 0, build => 'xrCore build 2202', graph_build => '1935'},
	{ version => 92, script_version => 0, build => 'xrCore build 1994', graph_build => '1935'},
	{ version => 90, script_version => 0, build => 'xrCore build 1964-1971', graph_build => '1935'},
	{ version => 89, script_version => 0, build => 'xrCore build 1957', graph_build => '1935'},
	{ version => 85, script_version => 0, build => 'xrCore build 1936', graph_build => '1935'},
	{ version => 79, script_version => 0, build => 'xrCore build 1935', graph_build => '1935'},
	{ version => 77, script_version => 0, build => 'xrCore build 1925', graph_build => '1935'},
	{ version => 76, script_version => 0, build => 'xrCore build 1902-1917', graph_build => '1510'},
	{ version => 75, script_version => 0, build => 'xrCore build 1893', graph_build => '1510'},
	{ version => 73, script_version => 0, build => 'xrCore build 1875', graph_build => '1510'},
	{ version => 72, script_version => 0, build => 'xrCore build 1865', graph_build => '1510'},
	{ version => 65, script_version => 0, build => 'xrCore build 1850', graph_build => '1510'},
	{ version => 63, script_version => 0, build => 'xrCore build 1842', graph_build => '1510'},
	{ version => 60, script_version => 0, build => 'xrCore build 1844 (19 May 2005)', graph_build => '1510'},
	{ version => 59, script_version => 0, build => 'xrCore build 1833-1835', graph_build => '1510'},
	{ version => 56, script_version => 0, build => 'xrCore build 1834 (09 April 2005)', graph_build => '1510'},
	{ version => 51, script_version => 0, build => 'xrCore build 1844-1849', graph_build => '1510'},
	{ version => 49, script_version => 0, build => 'xrCore build 1835', graph_build => '1510'},
	{ version => 47, script_version => 0, build => 'xrCore build 1834 (09 Feb 2005)', graph_build => '1510'},
	{ version => 46, script_version => 0, build => 'xrCore build 1829', graph_build => '1510'},
	{ version => 45, script_version => 0, build => 'xrCore build 1828', graph_build => '1510'},
	{ version => 44, script_version => 0, build => 'xrCore build 1851', graph_build => '1510'},
	{ version => 41, script_version => 0, build => 'xrCore build 1837', graph_build => '1510'},
	{ version => 40, script_version => 0, build => 'xrCore build 1610-1638', graph_build => '1510'},
	{ version => 39, script_version => 0, build => 'xrCore build 1511-1580', graph_build => '1510'},
	{ version => 38, script_version => 0, build => 'xrCore build 1510', graph_build => '1510'},
	{ version => 35, script_version => 0, build => 'xrCore build 1475', graph_build => '1510'},
	{ version => 34, script_version => 0, build => 'xrCore build 1475', graph_build => '1510'},
	{ version => 16, script_version => 0, build => 'xrCore build 1472', graph_build => '1472'},
	{ version => 14, script_version => 0, build => 'xrCore build 1472', graph_build => '1472'},
	{ version => 13, script_version => 0, build => 'xrCore build 1472', graph_build => '1472'},
	{ version => 8, script_version => 0, build => 'xrCore build 1469', graph_build => '1469'},
	{ version => 7, script_version => 0, build => 'xrCore build 1465', graph_build => '1469'},
	{ version => 3, script_version => 0, build => 'xrCore build 1233-1265'},
	{ version => 2, script_version => 0, build => 'xrCore build 1230-1254'},
);
sub build_by_version {
	foreach my $info (build_verions) {
		if (($_[0] == $info->{version}) && ($_[1] == $info->{script_version})) {
			return $info->{build};
		}
	}
	return undef;
}
sub version_by_build {
	foreach my $info (build_verions) {
		if ($_[0] eq $info->{short_build}) {
			return $info->{version}, $info->{script_version}, $info->{build};
		}
	}
	return undef;
}
sub graph_build {
	foreach my $info (build_verions) {
		if (($_[0] == $info->{version}) && ($_[1] == $info->{script_version})) {
			return $info->{graph_build};
		}
	}
	return undef;
}
#######################################################################
package alife_object;
use strict;
use stkutils::data_packet;
use stkutils::scan;
use stkutils::debug;
sub new {
	my $class = shift;
	my $self = {};
	$self->{cse_object} = {};
	bless($self, $class);
	return $self;
}
sub spawn_read {
	my $self = shift;
	my ($cf, $version, $ini) = @_;
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		my $id;
		if ($index == 0) {
			if ($version <= 0x5E) {
				$self->read_common($cf, $ini);
			} else {
				$id = unpack('v', $cf->r_chunk_data());
			}
		} elsif ($index == 1) {
			if ($version <= 0x5E) {
				$id = unpack('v', $cf->r_chunk_data());
			} else {
				$self->read_common($cf, $ini);
			}
		}
		$cf->r_chunk_close();
	}
}
sub read_common {
	my $self = shift;
	my ($cf, $ini) = @_;
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		my $data = $cf->r_chunk_data();
		my $size16 = unpack('v', $data);
		$size16 == ($size - 2) or stkutils::debug::fail(__PACKAGE__.'::read_common', __LINE__, '$size16 == ($size - 2)', 'alife object size mismatch');
		my $packet = stkutils::data_packet->new(substr($data, 2));
		if ($index == 0) {
			cse_abstract::state_read($self->{cse_object}, $packet);
			my $class_name = stkutils::scan->get_class($self->{cse_object}->{section_name});
			if (!defined $class_name && (::with_scan())) {
				$class_name = $ini->value('sections', $self->{cse_object}->{section_name});
			}
			defined $class_name or stkutils::debug::fail(__PACKAGE__.'::read_common', __LINE__, 'defined $class_name', 'unknown class for section '.$self->{cse_object}->{section_name});
			bless $self->{cse_object}, $class_name;
			$self->{cse_object}->state_read($packet);
			$packet->length() == 0 or stkutils::debug::warn(__PACKAGE__.'::read_common', __LINE__, '$packet->length() == 0', 'state data left ['.$packet->length().'] in entity '.$self->{cse_object}->{name});
		} elsif ($index == 1) {
			cse_abstract::update_read($self->{cse_object}, $packet);
			UNIVERSAL::can($self->{cse_object}, 'update_read') && do {
				$self->{cse_object}->update_read($packet);
			};
			if ($packet->length() == 6 && $self->{cse_object}->{version} == 118) {$packet = $self->first_patch_handler($data, $packet)};
			$packet->length() == 0 or stkutils::debug::fail(__PACKAGE__.'::read_common', __LINE__, '$packet->length() == 0', 'update data left ['.$packet->length().'] in entity '.$self->{cse_object}->{name});	
		}
		$cf->r_chunk_close();
	}
}
sub spawn_read_1935 {
	my $self = shift;
	my ($cf, $ini) = @_;
	my ($index, $size) = $cf->r_chunk_open();
	
	my $data = $cf->r_chunk_data();
	my $size16 = unpack('v', $data);
	my $update_packet_offset = $size16 + 4;
	my $state_packet = stkutils::data_packet->new(substr($data, 2, $size16));
	cse_abstract::state_read($self->{cse_object}, $state_packet);
	my $class_name = stkutils::scan->get_class($self->{cse_object}->{section_name});
	if (!defined $class_name && (::with_scan())) {
		$class_name = $ini->value('sections', $self->{cse_object}->{section_name});
	}
	defined $class_name or stkutils::debug::fail(__PACKAGE__.'::spawn_read_1935', __LINE__, 'defined $class_name', 'unknown class for section '.$self->{cse_object}->{section_name});
	bless $self->{cse_object}, $class_name;
	$self->{cse_object}->state_read($state_packet);
	$state_packet->length() == 0 or stkutils::debug::fail(__PACKAGE__.'::spawn_read_1935', __LINE__, '$state_packet->length() == 0', 'state data left ['.$state_packet->length().'] in entity '.$self->{cse_object}->{name});
	my $update_packet = stkutils::data_packet->new(substr($data, $update_packet_offset));
	my $update_packet_length = $size - $update_packet_offset;
	cse_abstract::update_read($self->{cse_object}, $update_packet);
	UNIVERSAL::can($self->{cse_object}, 'update_read') && do {
		$self->{cse_object}->update_read($update_packet);
	};
	$update_packet->length() == 0 or stkutils::debug::fail(__PACKAGE__.'::spawn_read_1935', __LINE__, '$update_packet->length() == 0', 'update data left ['.$update_packet->length().'] in entity '.$self->{cse_object}->{name});
	$cf->r_chunk_close();
}
sub spawn_read_level {
	my $self = shift;
	my ($cf, $size, $ini) = @_;
	my $data = $cf->r_chunk_data();
	my $state_packet = stkutils::data_packet->new($data);
	cse_abstract::state_read($self->{cse_object}, $state_packet);
	my $class_name = stkutils::scan->get_class($self->{cse_object}->{section_name});
	if (!defined $class_name && (::with_scan())) {
		$class_name = $ini->value('sections', $self->{cse_object}->{section_name});
	}
	defined $class_name or stkutils::debug::fail(__PACKAGE__.'::spawn_read_level', __LINE__, 'defined $class_name', 'unknown class for section '.$self->{cse_object}->{section_name});
	bless $self->{cse_object}, $class_name;
	$self->{cse_object}->state_read($state_packet);
	$state_packet->length() == 0 or stkutils::debug::fail(__PACKAGE__.'::spawn_read_level', __LINE__, '$state_packetf->length() == 0', 'state data left ['.$state_packet->length().'] in entity '.$self->{cse_object}->{name});
}
sub spawn_write {
	my $self = shift;
	my ($cf, $object_id, $ini) = @_;
	if ($self->{cse_object}->{version} > 94) {
		$cf->w_chunk(0, pack('v', $object_id));
		$cf->w_chunk_open(1);
	} else {
	$cf->w_chunk_open(0);
	}
	

	$cf->w_chunk_open(0);
	my $packet2 = stkutils::data_packet->new('');
	$self->{cse_object}->state_write($packet2);
	my $packet1 = stkutils::data_packet->new('');
	cse_abstract::state_write($self->{cse_object}, $packet1, $object_id, $packet2->length() + 2);
	$cf->w_chunk_data(pack('v', $packet1->length() + $packet2->length()));
	$cf->w_chunk_data($packet1->data());
	$cf->w_chunk_data($packet2->data());
	$cf->w_chunk_close();

	$cf->w_chunk_open(1);
	my $upd_packet2 = stkutils::data_packet->new('');
	UNIVERSAL::can($self->{cse_object}, 'update_write') && do {
		$self->{cse_object}->update_write($upd_packet2);
	};
	my $upd_packet1 = stkutils::data_packet->new('');
	cse_abstract::update_write($self->{cse_object}, $upd_packet1);
	$cf->w_chunk_data(pack('v', $upd_packet1->length() + $upd_packet2->length()));
	$cf->w_chunk_data($upd_packet1->data());
	$cf->w_chunk_data($upd_packet2->data());
	$cf->w_chunk_close();
	
	$cf->w_chunk_close();
	if ($self->{cse_object}->{version} <= 94) {
		$cf->w_chunk(1, pack('v', $object_id));
	}	
}
sub spawn_write_1935 {
	my $self = shift;
	my ($cf, $object_id) = @_;
	my $packet2 = stkutils::data_packet->new('');
	$self->{cse_object}->state_write($packet2);
	my $packet1 = stkutils::data_packet->new('');
	cse_abstract::state_write($self->{cse_object}, $packet1, $object_id, $packet2->length() + 2);
	$cf->w_chunk_data(pack('v', $packet1->length() + $packet2->length()));
	$cf->w_chunk_data($packet1->data());
	$cf->w_chunk_data($packet2->data());
	my $upd_packet2 = stkutils::data_packet->new('');
	if (ref $self->{cse_object} eq 'cse_alife_trader_shop') {
		cse_alife_trader::update_write($self->{cse_object}, $upd_packet2);
	} else {
		UNIVERSAL::can($self->{cse_object}, 'update_write') && do {
			$self->{cse_object}->update_write($upd_packet2);
		};
	}
	my $upd_packet1 = stkutils::data_packet->new('');
	cse_abstract::update_write($self->{cse_object}, $upd_packet1);
	$cf->w_chunk_data(pack('v', $upd_packet1->length() + $upd_packet2->length()));
	$cf->w_chunk_data($upd_packet1->data());
	$cf->w_chunk_data($upd_packet2->data());
}
sub spawn_write_level {
	my $self = shift;
	my ($cf, $object_id) = @_;
	my $packet2 = stkutils::data_packet->new('');
	$self->{cse_object}->state_write($packet2);
	my $packet1 = stkutils::data_packet->new('');
	cse_abstract::state_write($self->{cse_object}, $packet1, $object_id, $packet2->length());
	$cf->w_chunk_data($packet1->data());
	$cf->w_chunk_data($packet2->data());
}
sub state_import {
	my $self = shift;
	my ($if, $section, $ini) = @_;

	cse_abstract::state_import($self->{cse_object}, $if, $section);
	my $class_name = stkutils::scan->get_class($self->{cse_object}->{section_name});
	if (!defined $class_name && $ini) {
		$class_name = $ini->value('sections', $self->{cse_object}->{section_name});
	}
	defined $class_name or stkutils::debug::fail(__PACKAGE__.'::state_import', __LINE__, 'defined $class_name', 'unknown class for section '.$self->{cse_object}->{section_name});
	bless $self->{cse_object}, $class_name;
	$self->{cse_object}->state_import($if, $section);
	my $update_raw_data = $if->value($section, 'update_raw_data');
	if (defined $update_raw_data) {
		$self->{update_raw_data} = pack 'H*', $update_raw_data;
	} else {
		$self->{update_raw_data} = pack 'H4', '0000';
	}
}
sub state_export {
	my $self = shift;
	my ($if, $id) = @_;

	my $fh = $if->{fh};
	print $fh "[$id]\n";
	cse_abstract::state_export($self->{cse_object}, $if);
	$self->{cse_object}->state_export($if);
	if (defined $self->{update_raw_data} && (not defined ::is_convert())) {
		print $fh "; don't touch this\n";
		print $fh 'update_raw_data = ', unpack('H*', $self->{update_raw_data}), "\n";
	}
	print $fh "\n";
}
sub convert_spawn {
	my $self = shift;
	my ($new_version, $new_script_version, $old_gvid, $new_gvid) = @_;
	if ($new_version >= 122 && $self->{cse_object}->{version} < 122) {
		$self->{cse_object}->{cse_abstract__unk1_u16} = 0xffff;
		$self->{cse_object}->{cse_alife_object_hanging_lamp__unk6_f32} = 0.;
		$self->{cse_object}->{cse_alife_object_hanging_lamp__unk7_f32} = 0.;
		$self->{cse_object}->{cse_alife_object_hanging_lamp__unk8_f32} = 0.;
		$self->{cse_object}->{cse_alife_item_weapon__unk1_u8} = 0;
		$self->{cse_object}->{squad_id} = 'nil' if (ref $self->{cse_object} eq 'se_stalker' || ref $self->{cse_object} eq 'se_monster');
		$self->{cse_object}->{sim_forced_online} = 0 if (ref $self->{cse_object} eq 'se_stalker' || ref $self->{cse_object} eq 'se_monster');
		$self->{cse_object}->{dumb} = [0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,15,0,0,0,0,0,0,0,0,0,8,0,2,0,116,101,115,116,95,99,114,111,119,107,105,108,108,101,114,0,67,77,71,67,114,111,119,75,105,108,108,101,114,0,118,97,108,105,97,98,108,101,0,0,60,0,0,4,0,0,0,0,10,0,100,0,0,0,0,0,0,10,0,0,0,22,0,116,101,115,116,95,115,104,111,111,116,105,110,103,0,67,77,71,83,104,111,111,116,105,110,103,0,118,97,108,105,97,98,108,101,0,0,0,110,105,108,0,110,105,108,0,110,105,108,0,110,105,108,0,110,105,108,0,1,0,0,0,0,0,0,0,0,0,0,0,110,105,108,0,38,0,140,0,169,0] if ref $self->{cse_object} eq 'se_actor';
		if (ref $self->{cse_object} eq 'se_smart_terrain') {
			$self->{cse_object}->{actor_defence_come} = 0;
			$self->{cse_object}->{combat_quest} = 'nil';
			$self->{cse_object}->{task} = 0xFFFF;
			$self->{cse_object}->{see_actor_enemy} = 'nil';
			$self->{cse_object}->{flag} = 0;
			$self->{cse_object}->{squads_count} = 0;
			$self->{cse_object}->{force_online} = 0;
			$self->{cse_object}->{force_online_squads_count} = 0;
			$self->{cse_object}->{cover_manager_is_valid} = 0;
			$self->{cse_object}->{cover_manager_cover_table_count} = 0;		
			$self->{cse_object}->{npc_info_count} = 0;
			$self->{cse_object}->{dead_time_count} = 0;	
		}
	}
	if ($new_version >= 124 && $self->{cse_object}->{version} < 124) {
		$self->{cse_object}->{se_sim_faction__marker} = 0 if ref $self->{cse_object} eq 'se_sim_faction';
		$self->{cse_object}->{upgrades} = [0];
		if (ref $self->{cse_object} eq 'se_level_changer') {
			$self->{cse_object}->{enabled} = 1;
			$self->{cse_object}->{hint} = 'level_changer_invitation';
		} elsif (ref $self->{cse_object} eq 'se_smart_terrain') {
			$self->{cse_object}->{se_smart_terrain_combat_manager_cover_manager__marker} = 2;
			$self->{cse_object}->{se_smart_terrain_combat_manager__marker} = 19;			
		}
	}
	if ($new_version >= 128 && $self->{cse_object}->{version} < 128) {
		$self->{cse_object}->{game_material} = 'materials\\fake_ladders';
		$self->{cse_object}->{cse_alive_inventory_box__unk1_u8} = 1;
		$self->{cse_object}->{cse_alive_inventory_box__unk2_u8} = 0;
		$self->{cse_object}->{tip} = '';
		$self->{cse_object}->{cse_alife_trader_abstract__unk2_u8} = 0;
		$self->{cse_object}->{cse_alife_trader_abstract__unk3_u8} = 0;
		$self->{cse_object}->{start_position_filled} = 0;
		$self->{cse_object}->{last_spawn_time_present} = 0 if ref $self->{cse_object} eq 'se_zone_torrid';
		if (ref $self->{cse_object} eq 'se_smart_cover') {
			$self->{cse_object}->{MP_respawn} = 0 if ref $self->{cse_object} eq 'se_smart_cover';
			$self->{cse_object}->{last_description} = $self->{cse_object}->{description};
			$self->{cse_object}->{loopholes} = [$self->{cse_object}->{description}, 1];
		} elsif (ref $self->{cse_object} eq 'se_smart_terrain') {
			$self->{cse_object}->{arriving_npc_count} = 0;
			$self->{cse_object}->{base_on_actor_control_present} = 0;
			$self->{cse_object}->{is_respawn_point} = 0;
			$self->{cse_object}->{population} = 0;
		}
	}
	if (not ::level()) {
		if ((substr(ref $self->{cse_object}, 0, 14) eq 'cse_alife_item') and ($new_version >= 122) and ($self->{cse_object}->{version} < 122)) {
			$self->{cse_object}->{'upd:num_items'} = 0 if ref $self->{cse_object} eq 'cse_alife_object_physic';
			if ($self->{cse_object}->{'upd:num_items'} != 0) {
				$self->{cse_object}->{'upd:force'} = [0,0,0];
				$self->{cse_object}->{'upd:torque'} = [0,0,0];
				if (defined $self->{cse_object}->{'upd:quaternion_SOC'}) {
					$self->{cse_object}->{'upd:quaternion'} = $self->{cse_object}->{'upd:quaternion_SOC'};
				} else {
					$self->{cse_object}->{'upd:quaternion'} = [0,0,0,0];
				}
				if (defined $self->{cse_object}->{'upd:angular_velocity_SOC'}) {
					$self->{cse_object}->{'upd:angular_velocity'} = $self->{cse_object}->{'upd:angular_velocity_SOC'} ;
				}
				if (defined $self->{cse_object}->{'upd:linear_velocity_SOC'}) {
					$self->{cse_object}->{'upd:linear_velocity'} = $self->{cse_object}->{'upd:linear_velocity_SOC'};
				}
			}
		}
	}
	$self->{cse_object}->{version} = $new_version;
	$self->{cse_object}->{script_version} = $new_script_version;
	$self->{cse_object}->{game_vertex_id} += ($new_gvid - $old_gvid);
}
sub first_patch_handler {
	my $self = shift;
	my ($data, $packet) = @_;
	if (substr(ref($self->{cse_object}), 0, 14) eq 'cse_alife_item') {
		print "$self->{cse_object}->{name}\n";
		$packet = stkutils::data_packet->new(substr($data, 2));
		$self->{cse_object}->{is_first_patch} = 1;
		cse_abstract::update_read($self->{cse_object}, $packet);
		$self->{cse_object}->update_read($packet);
		return $packet;
	} else {
		return $packet;
	}
}
#######################################################################
package way_object;
use strict;
use stkutils::debug;
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub spawn_read {
	my $self = shift;
	my ($cf) = @_;
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		if ($index == 0) {
			$self->{name} = unpack('Z*', $cf->r_chunk_data());
		} elsif ($index == 1) {
			while (1) {
				($index, $size) = $cf->r_chunk_open();
				defined($index) or last;
				if ($index == 0) {
					$size == 4 or stkutils::debug::fail(__PACKAGE__.'::spawn_read', __LINE__, '$size == 4', 'point count size mismatch');
					$self->{point_count} = unpack('V', $cf->r_chunk_data());
				} elsif ($index == 1) {
					$self->read_points($cf);
				} elsif ($index == 2) {
					$self->read_links($cf);
				}
				$cf->r_chunk_close();
			}
		} else {
			stkutils::debug::fail(__PACKAGE__.'::spawn_read', __LINE__, '$index == 0 || $index == 1', 'unexpected index '.$index);
		}
		$cf->r_chunk_close();
	}
}
sub read_points {
	my $self = shift;
	my ($cf) = @_;
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		while (1) {
			($index, $size) = $cf->r_chunk_open();
			defined($index) or last;
			if ($index == 0) {
				$size == 4 or stkutils::debug::fail(__PACKAGE__.'::spawn_read', __LINE__, '$size == 4', 'point index size mismatch');
				my $point_index = unpack('V', $cf->r_chunk_data());
			} elsif ($index == 1) {
				my %point;
				(
					$point{name},
					$point{position}[0],
					$point{position}[1],
					$point{position}[2],
					$point{flags},
					$point{level_vertex_id},
					$point{game_vertex_id},
				) = unpack('Z*f3VVv', $cf->r_chunk_data());
				push @{$self->{points}}, \%point;
			}
			$cf->r_chunk_close();
		}
		$cf->r_chunk_close();
	}
}
sub read_links {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	while (1) {
		$packet->length() > 0 or last;
		my ($from, $to_count) = $packet->unpack('VV');
		my $point = $self->{points}[$from];
		while ($to_count--) {
			my %link;
			($link{to}, $link{weight}) = $packet->unpack('Vf');
			push @{$point->{links}}, \%link;
		}
	}
}
sub spawn_write {
	my $self = shift;
	my ($cf, $object_id) = @_;

	$cf->w_chunk(0, pack('Z*', $self->{name}));
	$cf->w_chunk_open(1);
		$cf->w_chunk(0, pack('V', $#{$self->{points}} + 1));

		my $links_data = '';

		$cf->w_chunk_open(1);
		my $point_id = 0;
		foreach my $p (@{$self->{points}}) {
			$cf->w_chunk_open($point_id);
			$cf->w_chunk(0, pack('V', $point_id));
#if ($self->{name} =~ /look/ && $p->{flags} == 0) {
#	printf "look=%s:%s\n", $self->{name}, $p->{name};
#}
#if (($p->{flags} & ($p->{flags} - 1)) != 0) {
#	printf "way=%s:%s\n", $self->{name}, $p->{name};
#}
			my $data = pack('Z*f3VVv',
				$p->{name},
				@{$p->{position}},
				$p->{flags},
				$p->{level_vertex_id},
				$p->{game_vertex_id});
			$cf->w_chunk(1, $data);
			$cf->w_chunk_close();

			if ($#{$p->{links}} != -1) {
				$links_data .= pack('VV', $point_id, $#{$p->{links}} + 1);
				foreach my $l (@{$p->{links}}) {
					$links_data .= pack('Vf', $l->{to}, $l->{weight});
				}
			}
			$point_id++;
		}
		$cf->w_chunk_close();

		$cf->w_chunk(2, $links_data);
	$cf->w_chunk_close();
}
sub state_import {
	my $self = shift;
	my ($if, $section) = @_;

	$self->{name} = $section;

	my $points = $if->value($section, 'points');
	stkutils::debug::fail(__PACKAGE__.'::spawn_read', __LINE__, 'defined $points', 'no points in path '.$section) unless defined $points;
	my %index_by_id;
	my $i = 0;
	foreach my $id (split /,/, $points) {
		$id =~ s/^\s*|\s*$//g;

		my %point;
		$point{name} = $if->value($section, "$id:name");
		my $flags = $if->value($section, "$id:flags");
		if (!defined $flags) {
			$flags = 0;
		} elsif ($flags =~ /^\s*0[xX]/) {
			$flags = hex $flags;
		}
		$point{flags} = $flags;
		my $position = $if->value($section, "$id:position");
		@{$point{position}} = split /,/, $position;
		$point{game_vertex_id} = $if->value($section, "$id:game_vertex_id");
		$point{level_vertex_id} = $if->value($section, "$id:level_vertex_id");
		$point{links0} = $if->value($section, "$id:links");

		push @{$self->{points}}, \%point;

		$index_by_id{$id} = $i++;
	}
	foreach my $p (@{$self->{points}}) {
		next unless defined $p->{links0};
		foreach my $l (split /,/, $p->{links0}) {
			$l =~ /^\s*(\w+)\s*\((\S+)\)\s*$/;
			die unless defined $1;
			die unless defined $2;
			my %link;
			$link{to} = $index_by_id{$1};
			$link{weight} = $2;
			push @{$p->{links}}, \%link;
		}
		delete $p->{links0};
	}
}
sub state_export {
	my $self = shift;
	my ($if, $id) = @_;
	my $fh = $if->{fh};

	print $fh "[$self->{name}]\n";
	my @points;
	my $i = 0;
	foreach my $p (@{$self->{points}}) {
		push @points, "p$i";
		$i++;
	}
	print $fh "points = ", join(',', @points), "\n";
	$i = 0;
	foreach my $p (@{$self->{points}}) {
		my $id = "p$i";
		print $fh "$id:name = $p->{name}\n";
		printf ($fh "$id:flags = %#x\n", $p->{flags}) unless $p->{flags} == 0;
		print $fh "$id:position = ", join(',', @{$p->{position}}), "\n";
		print $fh "$id:game_vertex_id = $p->{game_vertex_id}\n";
		printf $fh "$id:level_vertex_id = %d\n", $p->{level_vertex_id};
		if ($#{$p->{links}} != -1) {
			my @links;
			my $j = 0;
			foreach my $l (@{$p->{links}}) {
				push @links, "p$l->{to}($l->{weight})";
				$j++;
			}
			print $fh "$id:links = ", join(',', @links), "\n"; ;
		}
		$i++;
		print $fh "\n";
	}
	print $fh "\n";
}
#######################################################################
package converting ;
use strict;
use IO::File;
use stkutils::ini_file;
use stkutils::chunked_file;
use stkutils::debug;

sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
use constant way_name_exceptions => {
	kat_teleport_to_dark_city_orientation	=> 'l03u_agr_underground',
	walk_3					=> 'l05_bar',
	rad_heli_move				=> 'l10_radar',
	pri_heli4_go2_path			=> 'l11_pripyat',
	sar_teleport_0000_exit_look		=> 'l12u_sarcofag',
	sar_teleport_0000_exit_walk		=> 'l12u_sarcofag',
	val_ambush_dest_look		=> 'l04_darkvalley',
};
sub import {
	my $self = shift;
	my ($fn) = @_;
	
	if (not(::level())) {
		$self->import_alife($fn);
		$self->import_way($fn) if ::way();
	} else {
		$self->import_level($fn);
	}
}
sub import_alife {
	my $self = shift;
	my ($if) = @_;
	if ($if eq 'all.ltx') {
		my $nif = stkutils::ini_file->new($if, 'r') or stkutils::debug::fail(__PACKAGE__.'::import_alife', __LINE__, '', 'cannot open '.$if);
		foreach my $fn (split /,/, ($nif->value('alife', 'source_files') or stkutils::debug::fail(__PACKAGE__.'::import_alife', __LINE__, '', 'cannot open '.$if))) {
			$fn =~ s/^\s*|\s*$//g;
			my $lif;
			if ($fn eq 'alife_$debug$\y_selo.ltx') {
				$lif = stkutils::ini_file->new('alife_debug_y_selo.ltx', 'r') or stkutils::debug::fail(__PACKAGE__.'::import_alife', __LINE__, '', 'cannot open alife_debug_y_selo.ltx');
			} else {
				$lif = stkutils::ini_file->new($fn, 'r') or stkutils::debug::fail(__PACKAGE__.'::import_alife', __LINE__, '', 'cannot open '.$fn);
			}
			print "importing alife objects from file $fn...\n";
			foreach my $section (@{$lif->{sections_list}}) {
				my $object = alife_object->new();
				$object->state_import($lif, $section);
				push @{$self->{alife_objects}}, $object;
			}
			$lif->close();
		}
		$nif->close();
	} else {
		foreach my $fn (split /,/, $if) {
			my $location = 'alife_'.$fn.'.ltx';
			my $lif;
			if ($location eq 'alife_$debug$\y_selo.ltx') {
				$lif = stkutils::ini_file->new('alife_debug_y_selo.ltx', 'r') or stkutils::debug::fail(__PACKAGE__.'::import_alife', __LINE__, '', 'cannot open alife_debug_y_selo.ltx');
			} else {
				$lif = stkutils::ini_file->new($location, 'r') or stkutils::debug::fail(__PACKAGE__.'::import_alife', __LINE__, '', 'cannot open '.$location);
			}
			print "importing alife objects from file $location...\n";
			foreach my $section (@{$lif->{sections_list}}) {
				my $object = alife_object->new();
				$object->state_import($lif, $section);
				push @{$self->{alife_objects}}, $object;
			}
			$lif->close();
		}
	}
}
sub import_level {
	my $self = shift;
	my ($if) = @_;
	my ($lif) = stkutils::ini_file->new($if, 'r') or stkutils::debug::fail(__PACKAGE__.'::import_level', __LINE__, '', 'cannot open '.$if);
	print "importing alife objects from level_spawn.ltx\n";
	foreach my $section (@{$lif->{sections_list}}) {
		my $object = alife_object->new();
		$object->state_import($lif, $section);
		push @{$self->{alife_objects}}, $object;
	}
	$lif->close();
}
sub import_way {
	my $self = shift;
	my ($if) = @_;
	if ($if eq 'all') {
		my $fn = $if->is_section_exists('way') or return;
		foreach my $fn (split /,/, ($if->value('way', 'source_files') or return)) {
			$fn =~ s/^\s*|\s*$//g;
			my $lif;
			if ($fn eq 'way_$debug$\y_selo.ltx') {
				$lif = stkutils::ini_file->new('way_debug_y_selo.ltx', 'r') or stkutils::debug::fail(__PACKAGE__.'::import_way', __LINE__, '', 'cannot open way_debug_y_selo.ltx');
			} else {
				$lif = stkutils::ini_file->new($fn, 'r') or stkutils::debug::fail(__PACKAGE__.'::import_way', __LINE__, '', 'cannot open '.$fn);
			}
			print "importing way objects from file $fn...\n";
			foreach my $section (@{$lif->{sections_list}}) {
				my $object = way_object->new();
				$object->state_import($lif, $section);
				push @{$self->{way_objects}}, $object;
			}
			$lif->close();
		}
	} else {
		foreach my $fn (split /,/, $if) {
			my $location = 'way_'.$fn.'.ltx';
			my $lif;
			if ($location eq 'way_$debug$\y_selo.ltx') {
				$lif = stkutils::ini_file->new('way_debug_y_selo.ltx', 'r') or stkutils::debug::fail(__PACKAGE__.'::import_way', __LINE__, '', 'cannot open way_debug_y_selo.ltx');
			} else {
				$lif = stkutils::ini_file->new($location, 'r') or stkutils::debug::fail(__PACKAGE__.'::import_way', __LINE__, '', 'cannot open '.$location);
			}
			print "importing way objects from file $location...\n";
			foreach my $section (@{$lif->{sections_list}}) {
				my $object = way_object->new();
				$object->state_import($lif, $section);
				push @{$self->{way_objects}}, $object;
			}
			$lif->close();
		}	
	}
}
sub export {
	my $self = shift;
	my ($version, $script_version, $old_gvid, $new_gvid, $convert, $ini) = @_;

	if (not(::level())) {
		$self->export_alife($version, $script_version, $old_gvid, $new_gvid, $convert, $ini);
		$self->export_way($old_gvid, $new_gvid, $convert) if ::way();
	} else {
		$self->export_level($version, $script_version, 0, 0);
	}		
}
sub export_alife {
	my $self = shift;
	my ($version, $script_version, $old_gvid, $new_gvid, $convert, $ini) = @_;
	my @temp;
	if ($ini->value('sections', 'to_exclude') =~ /^\w+,\w+/) {
		@temp = split /,/, $ini->{sections_hash}{'sections'}{'to_exclude'};
	} else {
		$temp[0] = $ini->{sections_hash}{'sections'}{'to_exclude'};
	}
	my %exclude;
	foreach my $sect (@temp) {
		$exclude{$sect} = 1;
	}
	my $id = 0;
	my %if_by_level;
	my @levels;
	foreach my $object (sort sort_sub @{$self->{alife_objects}}) {
		next if defined $exclude{$object->{cse_object}->{section_name}};
		my %add;
		my %rep;
		if (defined $ini->{sections_hash}{$object->{cse_object}->{section_name}}) {
			foreach my $param (keys %{$ini->{sections_hash}{$object->{cse_object}->{section_name}}}) {
				my @temp = split /:/, $param;
				$add{$temp[1]} = $ini->{sections_hash}{$object->{cse_object}->{section_name}}{$param} if $temp[0] eq 'add';
				$rep{$temp[1]} = $ini->{sections_hash}{$object->{cse_object}->{section_name}}{$param} if $temp[0] eq 'rep';
			}
			foreach my $param (keys %{$object->{cse_object}}) {
				if (defined $add{$param}) {
					if ($add{$param} =~ /^[0-9]$/) {
						$object->{cse_object}->{$param} += $add{$param};
					} else {
						$object->{cse_object}->{$param} = join ('', $object->{cse_object}->{$param}, $add{$param});
					}
				}
				if (defined $rep{$param}) {
					$object->{cse_object}->{$param} = $rep{$param};
				}
			}
		}
		$object->convert_spawn($version, $script_version, $old_gvid, $new_gvid);
		my $cse_object = $object->{cse_object};
#		my $level = $graph->level_name($cse_object->{game_vertex_id});
		my $level = $convert;
#		print "$cse_object->{game_vertex_id}\n";
		stkutils::debug::fail(__PACKAGE__.'::export_alife', __LINE__, 'defined $level', 'unknown location of the alife object '.$cse_object->{name}) unless defined $level;
		my $lif = $if_by_level{$level};
		if (!defined $lif) {
			if ($level eq '$debug$\y_selo') {
				$lif = stkutils::ini_file->new("alife_debug_y_selo.ltx", 'w') or stkutils::debug::fail(__PACKAGE__.'::export_alife', __LINE__, '', 'cannot open alife_debug_y_selo.ltx');
			} else {
				$lif = stkutils::ini_file->new("alife_$level.ltx", 'w') or stkutils::debug::fail(__PACKAGE__.'::export_alife', __LINE__, '', 'cannot open alife_'.$level.'.ltx');
			}
			print "exporting alife objects on level $level...\n";
			$if_by_level{$level} = $lif;
			push @levels, "alife_$level.ltx";
		}
		$object->state_export($lif, $id++);
	}
	foreach my $if (values %if_by_level) {
		$if->close();
	}
}
sub export_level {
	my $self = shift;
	my ($version, $script_version) = @_;

	my $id = 0;
	my $lif = stkutils::ini_file->new("level_spawn.ltx", 'w') or stkutils::debug::fail(__PACKAGE__.'::export_level', __LINE__, '', 'cannot open level_spawn.ltx');
	foreach my $object (@{$self->{alife_objects}}) {
		$object->convert_spawn($version, $script_version, 0, 0);
		my $cse_object = $object->{cse_object};
		$object->state_export($lif, $id++);
	}
	$lif->close();
	print "done!\n";
}
sub export_way {
	my $self = shift;
	my ($version, $script_version, $old_gvid, $new_gvid, $graph) = @_;
	
	if (defined @{$self->{way_objects}}) {
		my %info_by_level;

		my $default_level = 'l05_bar';
		foreach my $object (@{$self->{way_objects}}) {
			foreach my $point (@{$object->{points}}) {
				$point->{game_vertex_id} += ($new_gvid - $old_gvid);
			}
			my $level = $graph->level_name($object->{points}[0]->{game_vertex_id});
			if (defined $level) {
				$default_level = $level;
			} else {
				# either get the name from the exceptions table
				# or use the current default (i.e. previous) value
				$level = (way_name_exceptions->{$object->{name}} or $default_level);
			}
			stkutils::debug::fail(__PACKAGE__.'::export_way', __LINE__, 'defined $level', 'unknown level of the way object '.$object->{name}) unless defined $level;
			my $info = $info_by_level{$level};
			if (!defined $info) {
				$info = {};
				if ($level eq '$debug$\y_selo') {
					$info->{lif} = stkutils::ini_file->new("way_debug_y_selo.ltx", 'w') or stkutils::debug::fail(__PACKAGE__.'::export_way', __LINE__, '', 'cannot open way_debug_y_selo.ltx');
				} else {
					$info->{lif} = stkutils::ini_file->new("way_$level.ltx", 'w') or stkutils::debug::fail(__PACKAGE__.'::export_way', __LINE__, '', 'cannot open way_'.$level.'.ltx');
				}
				$info->{way_objects} = ();
				$info_by_level{$level} = $info;
			}
			push @{$info->{way_objects}}, \$object;
		}

		my $id = 0;
		while (my ($level, $info) = each %info_by_level) {
			print "exporting way objects on level $level...\n";
			foreach my $object (sort {$$a->{name} cmp $$b->{name}}  @{$info->{way_objects}}) {
				$$object->state_export($info->{lif}, $id++);
			}
			$info->{lif}->close();
		}
	}
}
sub sort_sub {
	$a->{cse_object}->{section_name} cmp $b->{cse_object}->{section_name};
}
############################################################
package parsing ;
use strict;
use IO::File;
use stkutils::ini_file;
use stkutils::chunked_file;
use stkutils::debug;
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
use constant way_name_exceptions => {
	kat_teleport_to_dark_city_orientation	=> 'l03u_agr_underground',
	walk_3					=> 'l05_bar',
	rad_heli_move				=> 'l10_radar',
	pri_heli4_go2_path			=> 'l11_pripyat',
	sar_teleport_0000_exit_look		=> 'l12u_sarcofag',
	sar_teleport_0000_exit_walk		=> 'l12u_sarcofag',
	val_ambush_dest_look		=> 'l04_darkvalley',
};
sub import {
	my $self = shift;
	my ($fn) = @_;

	$self->import_alife($fn);
	if (::way()) {
		$self->import_way($fn);
	}
}
sub import_alife {
	my $self = shift;
	my ($if) = @_;
	my $lif = stkutils::ini_file->new($if, 'r') or stkutils::debug::fail(__PACKAGE__.'::import_alife', __LINE__, '', 'cannot open '.$if);
	print "importing alife objects from file $if...\n";
	foreach my $section (@{$lif->{sections_list}}) {
		my $object = alife_object->new();
		$object->state_import($lif, $section);
		push @{$self->{alife_objects}}, $object;
	}
	$lif->close();
}
sub import_way {
	my $self = shift;
	my ($if) = @_;

	my $lif = stkutils::ini_file->new('way'.substr($if, 5), 'r') or stkutils::debug::fail(__PACKAGE__.'::import_way', __LINE__, '', 'cannot open way'.substr($if, 5));
	print "importing way objects from file $if...\n";
	foreach my $section (@{$lif->{sections_list}}) {
		my $object = way_object->new();
		$object->state_import($lif, $section);
		push @{$self->{way_objects}}, $object;
	}
	$lif->close();
}
sub export {
	my $self = shift;
	my ($parse, $old_gvid, $new_gvid) = @_;

	$self->export_alife($parse, $old_gvid, $new_gvid);
	if (::way()) {
		$self->export_way($parse, $old_gvid, $new_gvid);
	}
}
sub export_alife {
	my $self = shift;
	my ($fn, $old_gvid, $new_gvid, $graph) = @_;

	my $id = 0;
	my %if_by_level;
	my @levels;
	my $if = stkutils::ini_file->new($fn, 'w') or stkutils::debug::fail(__PACKAGE__.'::export_alife', __LINE__, '', 'cannot open '.$fn);
	print "exporting alife objects...\n";
	foreach my $object (@{$self->{alife_objects}}) {
		$object->{cse_object}->{game_vertex_id} += $new_gvid - $old_gvid;
		$object->state_export($if, $id++);
	}
	$if->close();
}
sub export_way {
	my $self = shift;
	my ($fn, $old_gvid, $new_gvid) = @_;
	
	if (defined @{$self->{way_objects}}) {

		foreach my $object (@{$self->{way_objects}}) {
			foreach my $point (@{$object->{points}}) {
				$point->{game_vertex_id} += ($new_gvid - $old_gvid);
			}
			
		}
		my $if = stkutils::ini_file->new('way'.substr($fn, 5), 'w') or stkutils::debug::fail(__PACKAGE__.'::export_way', __LINE__, '', 'cannot open way'.substr($fn, 5));
		my $id = 0;
		print "exporting way objects...\n";
		foreach my $object (@{$self->{way_objects}}) {
			$object->state_export($if, $id++);
		}
		$if->close();
	}
}
############################################################
package all_spawn;
use strict;
use IO::File;
use stkutils::ini_file;
use stkutils::chunked_file;
use stkutils::debug;
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	my $self = shift;
	my ($fn, $version) = @_;
	my $ini = stkutils::ini_file->new('sections.ini', 'r') if ::with_scan();
	my $cf = stkutils::chunked_file->new($fn, 'r') or stkutils::debug::fail(__PACKAGE__.'::read', __LINE__, '', 'cannot open '.$fn);
	if (not(::level()) && ($version > 79)) {
		while (1) {
			my ($index, $size) = $cf->r_chunk_open();
			defined($index) or last;
			if ($index == 0) {
				print "reading header...\n";
				$self->read_header($cf, $version);
			} elsif ($index == 1) {
				print "reading alife objects...\n";
				$self->read_alife($cf, $version, $ini);
			} elsif ($index == 2) {
				print "reading artefact spawn places...\n";
				$self->read_section2($cf);
			} elsif ($index == 3) {
				print "reading way objects...\n";
				$self->read_way($cf);
			} elsif ($index == 4) {
				$self->read_graph($cf, $ini);
			} else {
				stkutils::debug::fail(__PACKAGE__.'::read', __LINE__, '$index <= 4', 'unexpected chunk index '.$index);
			}
			$cf->r_chunk_close();
		}
	} elsif (not(::level()) && ($version <= 79)) {
		my $count;
		my ($index, $size) = $cf->r_chunk_open();
		print "reading header...\n";
		$self->read_header($cf, $version);
		$cf->r_chunk_close();
		print "reading alife objects...\n";
		$self->read_alife($cf, $version, $ini);
		if ($version > 16) {
			($count, $size) = $cf->r_chunk_open();
			print "reading artefact spawn places...\n";
			$self->read_section2($cf);
			$cf->r_chunk_close();	
		}
	} else {
		$self->read_alife_level($cf, $ini);
	}
	$cf->close();
	$ini->close() if ::with_scan();
}
sub write {
	my $self = shift;
	my ($fn) = @_;
	my $version = (@{$self->{alife_objects}}[0])->{cse_object}->{version};
	my $cf = stkutils::chunked_file->new($fn, 'w') or stkutils::debug::fail(__PACKAGE__.'::write', __LINE__, '', 'cannot open '.$fn);
	if (not(::level())) {
		$self->write_header($cf, $version);
		$self->write_alife($cf, $version);
		$self->write_section2($cf, $version);
		$self->write_way($cf);
		$self->write_graph($cf);
	} else {
		$self->write_level($cf);
	}
	$cf->close();
}
sub read_header {
	my $self = shift;
	my ($cf, $version) = @_;
	if ($version > 0x5E) {
		(
			$self->{version},
			$self->{guid},
			$self->{graph_guid},
			$self->{count},
			$self->{level_count},
		) = unpack('Va[16]a[16]VV', $cf->r_chunk_data());
	} else {
		(
			$self->{version},
			$self->{count},
			$self->{unknown},
		) = unpack('VVV', $cf->r_chunk_data());	
	}
}
sub write_header {
	my $self = shift;
	my ($cf, $version) = @_;
	print "writing header...\n";
	if ($version > 94) {
		my $data = pack('Va[16]a[16]VV',
			$self->{version},
			$self->{guid},
			$self->{graph_guid},
			$#{$self->{alife_objects}} + 1,
			$self->{level_count});
		$cf->w_chunk(0, $data);
	} else {
		my $data = pack('VVV',
			$self->{version},
			$#{$self->{alife_objects}} + 1,
			$self->{unknown},);
		if ($version > 79) {
			$cf->w_chunk(0, $data);	
		} else {
			$cf->w_chunk(0xFFFF, $data);	
		}
	}
}
sub read_alife {
	my $self = shift;
	my ($cf, $version, $ini) = @_;
	my $i = 0;
	
	if ($version > 79) {
		while (1) {
			my ($index, $size) = $cf->r_chunk_open();
			defined($index) or last;
			if ($index == 0) {
				$size == 4 or stkutils::debug::fail(__PACKAGE__.'::read_alife', __LINE__, '$size == 4', 'unexpected alife objects count size');
				my $alife_count = unpack('V', $cf->r_chunk_data());
				$alife_count == $self->{count} or stkutils::debug::fail(__PACKAGE__.'::read_alife', __LINE__, '$alife_count == $self->{count}', 'alife object count mismatch');
			} elsif ($index == 1) {
				while (1) {
					($index, $size) = $cf->r_chunk_open();
					defined($index) or last;
					my $object = alife_object->new();
					$object->{cse_object}->{is_first_patch} = 1 if ($self->{is_first_patch} && $self->{is_first_patch} == 1);
					$object->spawn_read($cf, $version, $ini);
					$self->{is_first_patch} = 1 if ($object->{cse_object}->{is_first_patch} && $object->{cse_object}->{is_first_patch} == 1);
					push @{$self->{alife_objects}}, $object;
					$cf->r_chunk_close();
				}
			} elsif ($index == 2) {
				$size == 0 or stkutils::debug::fail(__PACKAGE__.'::read_alife', __LINE__, '$size == 0', 'unexpected unknown section size '.$size);
			}
			$cf->r_chunk_close();
		}
	} else {
		while ($i < $self->{count}) {
			my $object = alife_object->new();
			$object->spawn_read_1935($cf, $ini);
			$i++;
			push @{$self->{alife_objects}}, $object;
		}
	}
}
sub read_alife_level {
	my $self = shift;
	my ($cf, $ini) = @_;
	my $i = 0;
	while (1) {
		my $object = alife_object->new();
		my	($index, $size) = $cf->r_chunk_open();
		defined $index or last;
		die unless $i == $index;
		$object->spawn_read_level($cf, $size, $ini);
		$cf->r_chunk_close();
		$i++;
		push @{$self->{alife_objects}}, $object;
	}
}
sub write_alife {
	my $self = shift;
	my ($cf, $version) = @_;
	print "writing alife objects...\n";
	if ($version > 0x4F) {
		$cf->w_chunk_open(1);
		$cf->w_chunk(0, pack('V', $#{$self->{alife_objects}} + 1));
		$cf->w_chunk_open(1);
		my $id = 0;
		foreach my $object (@{$self->{alife_objects}}) {
			if ($object->{cse_object}->{section_name} eq "inventory_box") {
				$object->{cse_object}->{object_flags} = 0xffffff3b;
			}
			my $class_name = ref $object->{cse_object};
			if (0 && ($class_name eq 'se_stalker' or $class_name eq 'se_monster') && $object->{cse_object}->{story_id} == -1) {
				print "skipping $object->{cse_object}->{name}\n";
			} else {
				$cf->w_chunk_open($id);
				$object->spawn_write($cf, $id++);
				$cf->w_chunk_close();
			}
		}
		$cf->w_chunk_close();
		$cf->w_chunk(2, '');
		$cf->w_chunk_close();
	} else {
		my $id = 0;
		foreach my $object (@{$self->{alife_objects}}) {
			$cf->w_chunk_open($id);
			$object->spawn_write_1935($cf, $id++);
			$cf->w_chunk_close();
		}	
	}
}
sub write_level {
	my $self = shift;
	my ($cf) = @_;
	my $id = 0;
	foreach my $object (@{$self->{alife_objects}}) {
		$cf->w_chunk_open($id);
		$object->spawn_write_level($cf, $id++);
		$cf->w_chunk_close();
	}
}
sub read_section2 {
	my $self = shift;
	my ($cf) = @_;
	$self->{section2_raw_data} = $cf->r_chunk_data();

#	my $packet = stkutils::data_packet->new($self->{section2_raw_data});
#	my ($obj_count) = $packet->unpack('V');
#	print "obj_count = $obj_count\n";
#	while ($obj_count--) {
#		my ($x, $y, $z, $lvid, $unk) = $packet->unpack('fffVf');
#		print "$lvid: ($x, $y, $z), $unk\n";
#	}
}
sub write_section2 {
	my $self = shift;
	my ($cf, $version) = @_;
	if (defined $self->{section2_raw_data}){
		print "writing artefact spawn places...\n";
		if ($version > 0x4F) {
			$cf->w_chunk(2, $self->{section2_raw_data});
		} else {
			$cf->w_chunk($#{$self->{alife_objects}} + 1, $self->{section2_raw_data});
		}
	}
}
sub read_way {
	my $self = shift;
	my ($cf) = @_;
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		if ($index == 0) {
			$size == 4 or stkutils::debug::fail(__PACKAGE__.'::read_way', __LINE__, '$size == 4', 'unexpected way objects count size');
			my $way_count = unpack('V', $cf->r_chunk_data());
		} elsif ($index == 1) {
			while (1) {
				($index, $size) = $cf->r_chunk_open();
				defined($index) or last;
				my $object = way_object->new();
				$object->spawn_read($cf);
				push @{$self->{way_objects}}, $object;
				$cf->r_chunk_close();
			}
		} else {
			stkutils::debug::fail(__PACKAGE__.'::read_way', __LINE__, '$index <= 1', 'unexpected chunk index '.$index);
		}
		$cf->r_chunk_close();
	}
}
sub write_way {
	my $self = shift;
	my ($cf) = @_;
	if (defined @{$self->{way_objects}}) {
		print "writing way objects...\n";
		$cf->w_chunk_open(3);
		$cf->w_chunk(0, pack('V', $#{$self->{way_objects}} + 1));
		$cf->w_chunk_open(1);
		my $id = 0;
		foreach my $object (@{$self->{way_objects}}) {
			$cf->w_chunk_open($id);
			$object->spawn_write($cf, $id++);
			$cf->w_chunk_close();
		}
		$cf->w_chunk_close();
		$cf->w_chunk_close();
	}
}
sub read_graph {
	my $self = shift;
	my ($cf, $ini) = @_;
#	if (not (::split_spawns())) {
		$self->{section4_raw_data} = $cf->r_chunk_data();
	if (2 == 3) {
	#} else {
		print "	reading header...\n";
		my $data = $cf->r_chunk_data();
		($self->{graph}->{version},
		$self->{graph}->{vertex_count},
		$self->{graph}->{edge_count},
		$self->{graph}->{level_point_count},
		$self->{graph}->{guid},
		$self->{graph}->{level_count},)
		= unpack ('CvVVa[16]C', $data);
		my $edges_offset = $self->{graph}->{vertex_count} * 0x2a;
		my $lp_offset = $self->{graph}->{edge_count} * 0x06;
		my $packet = stkutils::data_packet->new(substr($data, 28));
		my $tr_packet = stkutils::data_packet->new($packet->trunk($edges_offset + $lp_offset + 8192));

		print "	reading levels...\n";
		my $i = 0;
		while ($i < $self->{graph}->{level_count}) {
			my $object = graph_level->new();
			$object->read($tr_packet);
			push @{$self->{graph_levels}}, $object;
			$i++;
		}

		my %level_by_id = ();
		foreach my $level (@{$self->{graph_levels}}) {
			$level_by_id{$level->{level_id}} = \$level;
		}
		
		print "	reading vertices...\n";
		$i = 0;
		my $gvid_offset = 0;
		while ($i < $self->{graph}->{vertex_count}) {
			my $object = graph_vertex->new();			
			my $level = levels::level_offset($i);
			if (defined $level) {
				print "			reading vertices on level $level\n";
				$gvid_offset = $i;
			}
			$object->read($tr_packet, $edges_offset, $i, ($i - $gvid_offset));
			push @{$self->{graph_vertices}}, $object;
			$i++;
		}	

		print "	reading edges...\n";
		$i = 0;
		while ($i < $self->{graph}->{edge_count}) {
			my $object = graph_edge->new();
			$object->read($tr_packet);
			push @{$self->{graph_edges}}, $object;
			$i++;
		}	
		
		print "converting vertices into alife objects...\n";
		for my $vertex (@{$self->{graph_vertices}}) {
			my $object = alife_object->new();
	
			$object->{cse_object}->{dummy16} = 0x0001;
			$object->{cse_object}->{section_name} = 'graph_point';
			$object->{cse_object}->{name} = $vertex->{name};
			$object->{cse_object}->{s_gameid} = 0;
			$object->{cse_object}->{s_rp} = 0xfe;
			$object->{cse_object}->{position} = $vertex->{level_point};
			$object->{cse_object}->{direction} = [0.0,0.0,0.0];
			$object->{cse_object}->{respawn_time} = 0;
			$object->{cse_object}->{unknown_id} = 0xffff;
			$object->{cse_object}->{parent_id} = 0xffff;
			$object->{cse_object}->{phantom_id} = 0xffff;
			$object->{cse_object}->{s_flags} = 0x21;
			$object->{cse_object}->{version} = 0x80;
			$object->{cse_object}->{cse_abstract__unk1_h16} = 0xffff;
			$object->{cse_object}->{script_version} = 12;
			$object->{cse_object}->{game_vertex_id} = $vertex->{game_vertex_id};
			
			my $class_name;
			if (::with_scan()) {
				$class_name = $ini->value('sections', $object->{cse_object}->{section_name});
			} else {
				$class_name = stkutils::scan->get_class($object->{cse_object}->{section_name});
			}
			bless $object->{cse_object}, $class_name;
			for (my $i = 0; $i < $vertex->{edge_count}; $i++) {
				my $edge = $self->{graph_edges}[$vertex->{edge_index} + $i];
				my $vid2 = $edge->{game_vertex_id};
				my $vertex2 = $self->{graph_vertices}[$vid2];
				if ($vertex->{level_id} != $vertex2->{level_id}) {
					my $level2 = $level_by_id{$vertex2->{level_id}};
					my $name2 = $$level2->{level_name};
					$object->{cse_object}->{connection_point_name} = $vertex2->{name};
					$object->{cse_object}->{connection_level_name} = $name2;
				} else {
					$object->{cse_object}->{connection_point_name} = '';
					$object->{cse_object}->{connection_level_name} = '';			
				}
			}
			
			$object->{cse_object}->{location0} = $vertex->{vertex_type}[0];
			$object->{cse_object}->{location1} = $vertex->{vertex_type}[1];
			$object->{cse_object}->{location2} = $vertex->{vertex_type}[2];
			$object->{cse_object}->{location3} = $vertex->{vertex_type}[3];
			push @{$self->{alife_objects}}, $object;
		}
	}
}
sub write_graph {
	my $self = shift;
	my ($cf) = @_;
	
	if (defined $self->{section4_raw_data}){
		print "writing graph...\n";
		$cf->w_chunk(4, $self->{section4_raw_data});
	}
}
sub import {
	my $self = shift;
	my ($ini) = @_;
	my $if = stkutils::ini_file->new('all.ltx', 'r') or stkutils::debug::fail(__PACKAGE__.'::import', __LINE__, '', 'cannot open all.ltx');
	if (not(::level())) {
		$self->import_header($if);
		$self->import_alife($if, $ini);
		$self->import_section2($if);
		$self->import_way($if);
		$self->import_graph($if);
	} else {
		$self->import_level('all.ltx', $ini);
	}
	$if->close();
	
}
sub import_header {
	my $self = shift;
	my ($if) = @_;

	$self->{version} = $if->value('header', 'version');
	$self->{guid} = pack 'H*', $if->value('header', 'guid') if ($if->is_value_exists('header', 'guid'));
	$self->{graph_guid} = pack 'H*', $if->value('header', 'graph_guid') if ($if->is_value_exists('header', 'graph_guid'));
	$self->{level_count} = $if->value('header', 'level_count');
	$self->{unknown} = $if->value('header', 'unknown');
	$self->{is_first_patch} = $if->value('header', 'is_first_patch') if $if->is_value_exists('header', 'is_first_patch');
}
sub import_alife {
	my $self = shift;
	my ($if, $ini) = @_;
	
	foreach my $fn (split /,/, ($if->value('alife', 'source_files') or stkutils::debug::fail(__PACKAGE__.'::import_alife', __LINE__, '$if->value(alife, source_files)', 'cannot find any locations in all.ltx'))) {
		$fn =~ s/^\s*|\s*$//g;
		my $lif;
		if ($fn eq 'alife_$debug$\y_selo.ltx') {
			$lif = stkutils::ini_file->new('alife_debug_y_selo.ltx', 'r') or stkutils::debug::fail(__PACKAGE__.'::import_alife', __LINE__, '', 'cannot open alife_debug_y_selo.ltx');
		} else {
			$lif = stkutils::ini_file->new($fn, 'r') or stkutils::debug::fail(__PACKAGE__.'::import_alife', __LINE__, '', 'cannot open '.$fn);
		}
		print "importing alife objects from file $fn...\n";
		foreach my $section (@{$lif->{sections_list}}) {
			my $object = alife_object->new();
			$object->{cse_object}->{is_first_patch} = 1 if ($self->{is_first_patch} && $self->{is_first_patch} == 1);
			$object->state_import($lif, $section, $ini);
			push @{$self->{alife_objects}}, $object;
		}
		$lif->close();
	}
}
sub import_level {
	my $self = shift;
	my ($if, $ini) = @_;
	my ($lif) = stkutils::ini_file->new($if, 'r') or stkutils::debug::fail(__PACKAGE__.'::import_level', __LINE__, '', 'cannot open '.$if);
	print "importing alife objects from level_spawn.ltx\n";
	foreach my $section (@{$lif->{sections_list}}) {
		my $object = alife_object->new();
		$object->state_import($lif, $section, $ini);
		push @{$self->{alife_objects}}, $object;
	}
	$lif->close();
}
sub import_section2 {
	my $self = shift;
	my ($if) = @_;
	my $fn = $if->is_value_exists('section2', 'binary_files') or return;
	$fn = $if->value('section2', 'binary_files') or stkutils::debug::fail(__PACKAGE__.'::import_section2', __LINE__, '$if->value(section2, binary_files)', 'cannot find section2.bin record in all.ltx');
	my $bin_fh = IO::File->new($fn, 'r') or stkutils::debug::fail(__PACKAGE__.'::import_section2', __LINE__, '', 'cannot open '.$fn);
	binmode $bin_fh;
	print "importing artefact spawn places data...\n";
	$bin_fh->read($self->{section2_raw_data}, ($bin_fh->stat())[7]);
}
sub import_way {
	my $self = shift;
	my ($if) = @_;
	my $fn = $if->is_section_exists('way') or return;
	foreach my $fn (split /,/, ($if->value('way', 'source_files') or return)) {
		$fn =~ s/^\s*|\s*$//g;
		my $lif;
		if ($fn eq 'way_$debug$\y_selo.ltx') {
			$lif = stkutils::ini_file->new('way_debug_y_selo.ltx', 'r') or stkutils::debug::fail(__PACKAGE__.'::import_way', __LINE__, '', 'cannot open way_debug_y_selo.ltx');
		} else {
			$lif = stkutils::ini_file->new($fn, 'r') or stkutils::debug::fail(__PACKAGE__.'::import_way', __LINE__, '', 'cannot open '.$fn);
		}
		print "importing way objects from file $fn...\n";
		foreach my $section (@{$lif->{sections_list}}) {
			my $object = way_object->new();
			$object->state_import($lif, $section);
			push @{$self->{way_objects}}, $object;
		}
		$lif->close();
	}
}
sub import_graph {
	my $self = shift;
	my ($if) = @_;
	my $fn = $if->is_value_exists('graph', 'binary_files') or return;
	print "importing graph...\n";
	$fn = $if->value('graph', 'binary_files');
	my $bin_fh = IO::File->new($fn, 'r') or stkutils::debug::fail(__PACKAGE__.'::import_graph', __LINE__, '', 'cannot open '.$fn);
	binmode $bin_fh;
	$bin_fh->read($self->{section4_raw_data}, ($bin_fh->stat())[7]);
}
sub export {
	my $self = shift;
	my ($fn, $graph) = @_;

	if (not(::level())) {
		my $if = stkutils::ini_file->new($fn, 'w') or stkutils::debug::fail(__PACKAGE__.'::export', __LINE__, '', 'cannot open '.$fn);
		$self->export_header($if);
		$self->export_alife($if, $graph);
		$self->export_section2($if);
		$self->export_way($if, $graph);
		$self->export_graph($if);
		$if->close();
	} else {
		$self->export_level($fn);
	}		
}
sub export_header {
	my $self = shift;
	my ($if) = @_;

	my $fh = $if->{fh};
	print $fh "[header]\n; don't touch these\n";
	print $fh "version = $self->{version}\n";
	print $fh 'guid = ', unpack('H*', $self->{guid}), "\n" if (defined $self->{guid});
	print $fh 'graph_guid = ', unpack('H*', $self->{graph_guid}), "\n" if (defined $self->{graph_guid});
	print $fh "level_count = $self->{level_count}\n" if (defined $self->{level_count});
	print $fh "unknown = $self->{unknown}\n" if (defined $self->{unknown});
	print $fh "is_first_patch = 1\n" if ($self->{is_first_patch} && $self->{is_first_patch} == 1);
	print $fh "\n";
}
sub export_alife {
	my $self = shift;
	my ($if, $graph) = @_;

	my $id = 0;
	my %if_by_level;
	my @levels;
	foreach my $object (@{$self->{alife_objects}}) {
		my $cse_object = $object->{cse_object};
		my $level = $graph->level_name($cse_object->{game_vertex_id});
		stkutils::debug::fail(__PACKAGE__.'::export_alife', __LINE__, 'defined $level', 'unknown location of the alife object '.$cse_object->{name}) unless defined $level; 
		my $lif = $if_by_level{$level};
		if (!defined $lif) {
			if ($level eq '$debug$\y_selo') {
				$lif = stkutils::ini_file->new("alife_debug_y_selo.ltx", 'w') or stkutils::debug::fail(__PACKAGE__.'::export_alife', __LINE__, '', 'cannot open alife_alife_debug_y_selo.ltx');
			} else {
				$lif = stkutils::ini_file->new("alife_$level.ltx", 'w') or stkutils::debug::fail(__PACKAGE__.'::export_alife', __LINE__, '', 'cannot open alife_'.$level.'.ltx');
			}
			print "exporting alife objects on level $level...\n";
			$if_by_level{$level} = $lif;
			push @levels, "alife_$level.ltx";
		}
		$object->state_export($lif, $id++);
	}
	my $fh = $if->{fh};
	print $fh "[alife]\nsource_files = <<END\n", join(",\n", @levels), "\nEND\n\n";
	foreach $if (values %if_by_level) {
		$if->close();
	}
}
sub export_level {
	my $self = shift;
	my ($if) = @_;

	my $id = 0;
	my $lif = stkutils::ini_file->new("level_spawn.ltx", 'w') or stkutils::debug::fail(__PACKAGE__.'::export_level', __LINE__, '', 'cannot open level_spawn.ltx');
	foreach my $object (@{$self->{alife_objects}}) {
		my $cse_object = $object->{cse_object};
		$object->state_export($lif, $id++);
	}
	$lif->close();
	print "done!\n";
}
sub export_section2 {
	my $self = shift;
	my ($if) = @_;
	if (defined $self->{section2_raw_data}) {
		my $fn = 'section2.bin';
		my $bin_fh = IO::File->new($fn, 'w') or stkutils::debug::fail(__PACKAGE__.'::export_section2', __LINE__, '', 'cannot open '.$fn);
		binmode $bin_fh;
		print "exporting raw data...\n";
		$bin_fh->write($self->{section2_raw_data}, length($self->{section2_raw_data}));

		my $fh = $if->{fh};
		print $fh "[section2]\nbinary_files = $fn\n\n";
	}
}
sub export_graph {
	my $self = shift;
	my ($if) = @_;
	if (defined $self->{section4_raw_data}) {
		print "exporting graph...\n";
		my $fn = 'section4.bin';
		my $bin_fh = IO::File->new($fn, 'w') or stkutils::debug::fail(__PACKAGE__.'::export_graph', __LINE__, '', 'cannot open '.$fn);
		binmode $bin_fh;
		$bin_fh->write($self->{section4_raw_data}, length($self->{section4_raw_data}));
		my $fh = $if->{fh};
		print $fh "[graph]\nbinary_files = $fn\n\n";
	}
	print "done!\n";
}
# FIXME: it is better to scan level.game files
use constant way_name_exceptions => {
	kat_teleport_to_dark_city_orientation	=> 'l03u_agr_underground',
	walk_3					=> 'l05_bar',
	rad_heli_move				=> 'l10_radar',
	pri_heli4_go2_path			=> 'l11_pripyat',
	sar_teleport_0000_exit_look		=> 'l12u_sarcofag',
	sar_teleport_0000_exit_walk		=> 'l12u_sarcofag',
	val_ambush_dest_look		=> 'l04_darkvalley',
};
sub export_way {
	my $self = shift;
	my ($if, $graph) = @_;
	
	if (defined @{$self->{way_objects}}) {
		my %info_by_level;

		my $default_level = 'l05_bar';
		foreach my $object (@{$self->{way_objects}}) {
			my $level = $graph->level_name($object->{points}[0]->{game_vertex_id});
			if (defined $level) {
				$default_level = $level;
			} else {
				# either get the name from the exceptions table
				# or use the current default (i.e. previous) value
				$level = (way_name_exceptions->{$object->{name}} or $default_level);
			}
			stkutils::debug::fail(__PACKAGE__.'::export_way', __LINE__, 'defined $level', 'unknown level of the way object '.$object->{name}) unless defined $level;
			my $info = $info_by_level{$level};
			if (!defined $info) {
				$info = {};
				if ($level eq '$debug$\y_selo') {
					$info->{lif} = stkutils::ini_file->new("way_debug_y_selo.ltx", 'w') or stkutils::debug::fail(__PACKAGE__.'::export_way', __LINE__, '', 'cannot open way_debug_y_selo.ltx');
				} else {
					$info->{lif} = stkutils::ini_file->new("way_$level.ltx", 'w') or stkutils::debug::fail(__PACKAGE__.'::export_way', __LINE__, '', 'cannot open way_'.$level.'.ltx');
				}
				$info->{way_objects} = ();
				$info_by_level{$level} = $info;
			}
			push @{$info->{way_objects}}, \$object;
		}

		my $id = 0;
#		foreach my $info (values %info_by_level) {
		while (my ($level, $info) = each %info_by_level) {
			print "exporting way objects on level $level...\n";
			foreach my $object (sort {$$a->{name} cmp $$b->{name}}  @{$info->{way_objects}}) {
				$$object->state_export($info->{lif}, $id++);
			}
			$info->{lif}->close();
		}
		my $fh = $if->{fh};
		print $fh "[way]\nsource_files = <<END\n", join(",\n", map {"way_$_.ltx"} (keys %info_by_level)), "\nEND\n\n";
	}
}
sub move_actor {
	my $self = shift;

	my ($level_name, $graph_point) = split ':', $_[0];

	my $actor;
	my $lchanger;
	foreach my $object (@{$self->{alife_objects}}) {
		my $cse_object = $object->{cse_object};
		if ($cse_object->{section_name} eq "level_changer" and
				lc($cse_object->{dest_graph_point}) eq $graph_point and
				lc($cse_object->{dest_level_name}) eq $level_name) {
			$lchanger = $cse_object;
			last if defined $actor;
		} elsif ($cse_object->{section_name} eq "actor") {
			$actor = $cse_object;
			last if defined $lchanger;
		}
	}
	if (defined $actor && defined $lchanger) {
		print "moving actor to $level_name:$graph_point\n";
		$actor->{game_vertex_id} = $lchanger->{dest_game_vertex_id};
		$actor->{level_vertex_id} = $lchanger->{dest_level_vertex_id};
		@{$actor->{position}} = @{$lchanger->{dest_position}};
		@{$actor->{direction}} = @{$lchanger->{dest_direction}};
		@{$actor->{'upd:position'}} = @{$actor->{position}};
		$actor->{o_model} = 0;
		@{$actor->{'upd:o_torso'}}[0] = @{$actor->{direction}}[1];
		@{$actor->{'upd:o_torso'}}[1] = @{$actor->{direction}}[2];
		@{$actor->{'upd:o_torso'}}[2] = 0;
	} else {
		stkutils::debug::fail(__PACKAGE__.'::move_actor', __LINE__, 'defined $actor && defined $lchanger', 'cannot find actor or level_changer');
	}
}
#######################################################################
use constant good_sects => {
	zone_buzz				=> 'se_zone_anom',
	zone_buzz_weak				=> 'se_zone_anom',
	zone_buzz_average			=> 'se_zone_anom',
	zone_buzz_strong			=> 'se_zone_anom',

	zone_gravi_zone				=> 'se_zone_anom',
	zone_gravi_zone_weak			=> 'se_zone_anom',
	zone_gravi_zone_average			=> 'se_zone_anom',
	zone_gravi_zone_strong			=> 'se_zone_anom',

	zone_mincer				=> 'se_zone_anom',
	zone_mincer_weak			=> 'se_zone_anom',
	zone_mincer_weak_noart			=> 'se_zone_anom',
	zone_mincer_average			=> 'se_zone_anom',
	zone_mincer_strong			=> 'se_zone_anom',

	zone_mosquito_bald			=> 'se_zone_anom',
	zone_mosquito_bald_weak			=> 'se_zone_anom',
	zone_mosquito_bald_weak_noart		=> 'se_zone_anom',
	zone_mosquito_bald_average		=> 'se_zone_anom',
	zone_mosquito_bald_strong		=> 'se_zone_anom',
	zone_mosquito_bald_strong_noart		=> 'se_zone_anom',
	zone_mosquito_bald			=> 'se_zone_anom',

	zone_no_gravity				=> 'cse_alife_anomalous_zone',

	zone_witches_galantine			=> 'se_zone_anom',
	zone_witches_galantine_weak		=> 'se_zone_anom',
	zone_witches_galantine_average		=> 'se_zone_anom',
	zone_witches_galantine_strong		=> 'se_zone_anom',

	zone_zharka_static			=> 'se_zone_anom',
	zone_zharka_static_weak			=> 'se_zone_anom',
	zone_zharka_static_average		=> 'se_zone_anom',
	zone_zharka_static_strong		=> 'se_zone_anom',
};
sub good_anom {
	my ($sect) = @_;
	return defined(good_sects->{$sect});
}
#######################################################################
package main;
use strict;
use Getopt::Long;
use File::Path;
use stkutils::scan;
use stkutils::graph;
use stkutils::chunked_file;
use Cwd;
#use diagnostics;

sub usage {
	return <<END
S.T.A.L.K.E.R. all.spawn compiler/decompiler
Usage: acdc -d all.spawn [-o outdir] [-scan scandir]
       acdc -c all.ltx [-f flag1[,...]] [-a level:point] [-o outfile]
END
}
sub convert_usage {
	return <<END
	Bad Game Syntax!
Usage: acdc -convert <location> [-game1 <game1>,<old_gvid0>] [-game2 <game2>,<new_gvid0>]
END
}
sub parse_usage {
	return <<END
	Bad Game Syntax!
Usage: acdc -parse <location> [-game1 <old_gvid0>] [-game2 <new_gvid0>] [-way]
END
}

my $spawn_file;
my $src_dir;
my $out;
my $flags;
my %flags_hash;
my $actor_pos;
my $level_spawn;
my $use_hack;
my $convert;
my ($game1, $game2);
my $parse;
my $way;
my $sc;
my $sdk;

GetOptions(
	'd=s' => \$spawn_file,
	'c:s' => \$src_dir,
	'convert=s' => \$convert,
	'game1=s' => \$game1,
	'game2=s' => \$game2,
	'o=s' => \$out,
	'f=s' => \$flags,
	'a=s' => \$actor_pos,
	'l' => \$level_spawn,	
	'hack' => \$use_hack,
	'parse=s' => \$parse,		
	'way' => \$way,	
	'scan=s' => \$sc,		
	'sdk' => \$sdk	
) or die usage();

if (defined $flags) {
	foreach my $flag (split /,/, $flags) {
		die "bad flag '$flag'\n" unless $flag =~ /\s*(\w+)\s*/;
		$flags_hash{$1} = 1;
	}
}

if ((defined $sc) && !(::is_alredy_scanned())) {
	stkutils::scan->launch($sc);
}

if (defined $convert) {
	die convert_usage() if (defined $src_dir or defined $actor_pos or defined $spawn_file or (not (defined $game1 or defined $game2)));
	my $params = {};
	$params->{old_game} = {};
	$params->{new_game} = {};
	if ($game1 =~ /(\w+)\W(\w+)/) {
		$params->{old_game}->{short_build} = $1;
		$params->{old_game}->{gvid} = $2;
	} else {
		die convert_usage(); 
	}
	if ($game2 =~ /(\w+)\W(\w+)/) {
		$params->{new_game}->{short_build} = $1;
		$params->{new_game}->{gvid} = $2;
	} else {
		die convert_usage(); 
	}
	($params->{old_game}->{version},
	$params->{old_game}->{script_version},
	$params->{old_game}->{build}) = build_version::version_by_build($params->{old_game}->{short_build});
	($params->{new_game}->{version},
	$params->{new_game}->{script_version},
	$params->{new_game}->{build}) = build_version::version_by_build($params->{new_game}->{short_build});
	print "converting from $params->{old_game}->{build} spawn format to $params->{new_game}->{build} spawn format...\n";
#	my $graph = stkutils::graph->new();
#	$graph->{build_version} = build_version::graph_build($params->{old_game}->{version}, $params->{old_game}->{script_version});
#	if (not ::level()) {
#		$graph->read();
#	}
	my $spawn = converting->new();
	$spawn->import($convert);
	my $ini = stkutils::ini_file->new('convert.ini', 'r') or  stkutils::debug::fail(__PACKAGE__, __LINE__, '', 'cannot open convert.ini');
	File::Path::mkpath('converted_spawn', 0);
	chdir 'converted_spawn' or stkutils::debug::fail(__PACKAGE__, __LINE__, '', 'cannot change path to converted_spawn');
	$spawn->export($params->{new_game}->{version}, $params->{new_game}->{script_version}, $params->{old_game}->{gvid}, $params->{new_game}->{gvid}, $convert, $ini);
	$ini->close();
	print "done!\n";
} elsif (defined $spawn_file) {
	stkutils::debug::fail(__PACKAGE__, __LINE__, '!defined $src_file && !defined $actor_pos', 'bad params') if (defined $src_dir or defined $actor_pos);
	print "checking version of $spawn_file...\n";	
	my $fh = IO::File->new($spawn_file, 'r') or stkutils::debug::fail(__PACKAGE__, __LINE__, '', 'cannot open '.$spawn_file);
	binmode $fh;
	my $data;
	$fh->read($data, 0x12C) or stkutils::debug::fail(__PACKAGE__, __LINE__, '$fh->read($data, 0x12C)', 'cannot read '.$spawn_file);
	$fh->close();
	my $table = ();
	if (::level()) {
			(
			$table->{garb_1},
			$table->{section_name},
			$table->{name},
			$table->{garb_2},
			$table->{version},
			$table->{script_version},
			$table->{backup},
		) = unpack('a[10]Z*Z*a[36]vvv', $data);
	} else {
		($table->{switch}, $table->{header_size}) = unpack('VV',$data);
		if (($table->{switch} == 0) && ($table->{header_size} == 0x2c)) {
			(
				$table->{garb_1},
				$table->{section_name},
				$table->{name},
				$table->{garb_2},
				$table->{version},
				$table->{script_version},
				$table->{backup},
			) = unpack('a[118]Z*Z*a[36]vvv', $data);
		} elsif (($table->{switch} == 0) && ($table->{header_size} == 0x0c)) {
			(
				$table->{garb_1},
				$table->{section_name},
				$table->{name},
				$table->{garb_2},
				$table->{version},
				$table->{script_version},
				$table->{backup},
			) = unpack('a[76]Z*Z*a[36]vvv', $data);
		} else {
			(
				$table->{garb_1},
				$table->{section_name},
				$table->{name},
				$table->{garb_2},
				$table->{version},
				$table->{script_version},
			) = unpack('a[32]Z*Z*a[36]vv', $data);	
		}	
	}
	if ($table->{script_version} == 0xffff) {
		$table->{script_version} = $table->{backup};
	}
	if ($table->{version} <= 0x45) {
		$table->{script_version} = 0;
	}
	my $build = (build_version::build_by_version($table->{version}, $table->{script_version}) or 'unknown,  spawn ver. '.$table->{version}.'');
	if ($table->{version} == 118 && $table->{script_version} == 6) {
		$fh = stkutils::chunked_file->new($spawn_file, 'r');
		if ($fh->find_chunk(0x4)) {
			print "you're trying to unpack spawn of S.T.A.L.K.E.R. xrCore build 3120\n";
			$fh->close_found_chunk();
		} else {
			print "you're trying to unpack spawn of S.T.A.L.K.E.R. $build\n";
		}
		$fh->close();
	} else {
		print "you're trying to unpack spawn of S.T.A.L.K.E.R. $build\n";
	}
	my $spawn = all_spawn->new();
	print "reading $spawn_file...\n";
	$spawn->read($spawn_file, $table->{version});
	my $graph = stkutils::graph->new();
	$graph->{build_version} = build_version::graph_build($table->{version}, $table->{script_version});
	if (not ::level()) {
		$graph->read($table->{version}, $spawn);
	}
	defined $out && do {
		File::Path::mkpath($out, 0);
		chdir $out or stkutils::debug::fail(__PACKAGE__, __LINE__, '', 'cannot change path to '.$out);
	};
	print "exporting alife objects...\n";	
	$spawn->export('all.ltx', $graph);
} elsif (defined $src_dir) {
	stkutils::debug::fail(__PACKAGE__, __LINE__, '!defined $spawn_file', 'bad params') if defined $spawn_file;
	my $wd = getcwd();
	my $ini = stkutils::ini_file->new('sections.ini', 'r') if ::with_scan();
	if ($src_dir ne '') {
		chdir $src_dir or stkutils::debug::fail(__PACKAGE__, __LINE__, '', 'cannot change dir to '.$src_dir);
	}
	my $spawn = all_spawn->new();
	$out = 'all.spawn.new' unless defined $out;
	print "importing alife objects...\n";
	$spawn->import($ini);
	if (defined $actor_pos) {
		$spawn->move_actor($actor_pos);
	}
	print "writing $out...\n";
	chdir $wd or die;
	$ini->close() if ::with_scan();
	$spawn->write($out);
	print "done!\n";
} elsif (defined $parse) {
	die parse_usage() if (defined $src_dir or defined $actor_pos or defined $spawn_file or (not (defined $game1 or defined $game2)));
	print "parsing...\n";
	my $spawn = parsing->new();
	$spawn->import($parse);
	File::Path::mkpath('parsed_spawn', 0);
	chdir 'parsed_spawn' or die stkutils::debug::fail(__PACKAGE__, __LINE__, '', 'cannot change path to parsed_spawn');
	$spawn->export($parse, $game1, $game2);
	print "done!\n";	
} else {
	die usage();
}

sub with_scan {return ((defined $sc) || (-e 'sections.ini'));}
sub is_alredy_scanned {return (-e 'sections.ini');}
sub is_convert {return defined $convert;}
sub way {return defined $way;}
sub is_flag_defined {
	return (defined $flags_hash{$_[0]}) ? 1 : 0;
}
sub level {return defined $level_spawn;}
sub use_hack {return defined $use_hack;}
sub edge_block_size {
	if ($_[0] eq 'soc' or $_[0] eq 'cop') {
		return 0x06;
	} else {
		return 0x08;
	}
}
sub vertex_block_size {
	if ($_[0] eq '1469' or $_[0] eq '1472') {
		return 0x24;
	} elsif ($_[0] eq '1510' or $_[0] eq '1935' or $_[0] eq '2215') {
		return 0x28;
	} else {
		return 0x2a;
	}
}
sub header_size {
	if ($_[0] eq '1469' or $_[0] eq '1472') {
		return 0x0C;
	} elsif ($_[0] eq '1510' or $_[0] eq '1935') {
		return 0x14;
	} elsif ($_[0] eq '2215') {
		return 0x24;
	} else {
		return 0x1C;
	}
}
sub sdk {return defined $sdk}

my %markers;
sub set_save_marker {
	my $packet = shift;
	my $mode = shift;
	my $check = shift;
	my $name = shift;
	if ($check) {
		die unless defined($markers{$name});
		if ($mode eq 'save') {
			my $diff = $packet->w_tell() - $markers{$name};
			die unless $diff > 0;
			$packet->pack('v', $diff);
		} else {
			my $diff = $packet->r_tell() - $markers{$name};
			die unless $diff > 0;
			my ($diff1) = $packet->unpack('v');
			die unless $diff == $diff1;
		}
	} else {
		if ($mode eq 'save') {
			$markers{$name} = $packet->w_tell();
		} else {
			$markers{$name} = $packet->r_tell();
		}
	}
}
#######################################################################