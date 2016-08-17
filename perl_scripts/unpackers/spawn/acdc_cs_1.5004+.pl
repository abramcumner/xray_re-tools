#!perl -w -I \utils
#
# last edited:	04 Sep 2008
#
# tab size:	8
#
#######################################################################
package cse_abstract;
use strict;
use constant properties_info => (
	{ name => 'dummy16',		type => 'h16',	default => 0x0001 },
	{ name => 'section_name',	type => 'sz',	default => '' },	# 0x40
	{ name => 'name',		type => 'sz',	default => '' },	# 0x11c
	{ name => 's_gameid',		type => 'h8',	default => 0 },		# 0x44
	{ name => 's_rp',		type => 'h8',	default => 0xfe },	# 0x45
	{ name => 'position',		type => 'f32v3',default => [] },	# 0x58
	{ name => 'direction',		type => 'f32v3',default => [] },	# 0x64
	{ name => 'respawn_time',	type => 'h16',	default => 0 },		# 0x34
	{ name => 'unknown_id',		type => 'h16',	default => 0xffff },	# 0x36
	{ name => 'parent_id',		type => 'h16',	default => 0xffff },	# 0x38
	{ name => 'phantom_id',		type => 'h16',	default => 0xffff },	# 0x3a
	{ name => 's_flags',		type => 'h16',	default => 0x21 },	# 0x43 clear sky
	{ name => 'version',		type => 'h16',	default => 0x7C },	# 0x2c clear sky
	{ name => 'cse_abstract_unk1',	type => 'u16', default => 0xffff },	# 0x40 clear sky	
	{ name => 'script_version',	type => 'u16',	default => 8 },		# 0x32
);
sub state_read {
	my $self = shift;
	my ($packet) = @_;
	$packet->unpack_properties($self, properties_info);
	my ($unused, $spawn_id, $extended_size) = $packet->unpack('vvv');
}
sub state_write {
	my $self = shift;
	my ($packet, $spawn_id, $extended_size) = @_;
	$packet->pack_properties($self, properties_info);
	$packet->pack('vvv', 0, $spawn_id, $extended_size);
}
sub update_read {
	my ($size) = $_[1]->unpack('v');
	die "unexpected size in cse_abstract::update_read\n" unless $size == 0;
}
sub update_write {
	$_[1]->pack('v', 0);
}
sub state_import {
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	$_[1]->export_properties('cse_abstract', $_[0], properties_info);
}

#######################################################################
#clear sky 1.5.0.4 compatible
package cse_alife_graph_point;
use strict;
use constant properties_info => (
	{ name => 'connection_point_name',	type => 'sz',	default => '' },	# 0xa4
	{ name => 'connection_level_name',	type => 'sz',	default => '' },	# 0x100
	{ name => 'location0',			type => 'u8',	default => 0 },		# 0xa8
	{ name => 'location1',			type => 'u8',	default => 0 },		# 0xa9
	{ name => 'location2',			type => 'u8',	default => 0 },		# 0xaa
	{ name => 'location3',			type => 'u8',	default => 0 },		# 0xab
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
#clear sky 1.5.0.4 compatible
package cse_ph_skeleton;
use strict;
use constant properties_info => (
	{ name => 'skeleton_name',	type => 'sz',	default => '$editor' },	# smthg|0x08
	{ name => 'skeleton_flags',	type => 'u8',	default => 0 },		# 0x08
	{ name => 'source_id',		type => 'h16',	default => 0xffff },	# 0x48
);
#use constant upd_properties_info => (
#	{ name => 'upd:skeleton_flags', type => 'u8' },		# 0x08
#);
sub state_read {
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	$_[1]->pack_properties($_[0], properties_info);
}
#sub update_read {
#	$_[1]->unpack_properties($_[0], upd_properties_info);
#}
#sub update_write {
#	$_[1]->pack_properties($_[0], upd_properties_info);
#}
sub state_import {
	$_[1]->import_properties($_[2], $_[0], properties_info);
#	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
}
sub state_export {
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
#	$_[1]->export_properties(__PACKAGE__ . ' update', $_[0], upd_properties_info);
}

#######################################################################
package cse_temporary;
use strict;
sub state_read {
	my ($unk) = $_[1]->unpack('V');
}
sub state_write {
}

#######################################################################
#clear sky 1.5.0.4 compatible
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
	{ name => 'game_vertex_id',	type => 'u16',	default => 0xffff },	# 0xa4
	{ name => 'distance',		type => 'f32',	default => 0.0 },	# 0xa8
	{ name => 'direct_control',	type => 'u32',	default => 1 },		# 0xad
	{ name => 'level_vertex_id',	type => 'u32',	default => 0xffffffff },# 0xb0
	{ name => 'object_flags',	type => 'h32',	default => 0 },		# 0xb4
	{ name => 'custom_data',	type => 'sz',	default => '' },	# 0x7c
	{ name => 'story_id',		type => 's32',	default => -1 },	# 0xb8
	{ name => 'spawn_story_id',	type => 's32',	default => -1 },	# 0xbc
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
#clear sky 1.5.0.4 compatible
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
#clear sky 1.5.0.4 compatible
package cse_alife_dynamic_object_visual;
use strict;
sub state_read {
	cse_alife_object::state_read(@_);
	cse_visual::state_read(@_);
}
sub state_write {
	cse_alife_object::state_write(@_);
	cse_visual::state_write(@_);
}
sub state_import {
	cse_alife_object::state_import(@_);
	cse_visual::state_import(@_);
}
sub state_export {
	cse_alife_object::state_export(@_);
	cse_visual::state_export(@_);
}

#######################################################################
package cse_alife_object_climable;
use strict;
sub state_read {
	cse_alife_object::state_read(@_);
	cse_shape::state_read(@_);
}
sub state_write {
	cse_alife_object::state_write(@_);
	cse_shape::state_write(@_);
}
sub state_import {
	cse_alife_object::state_import(@_);
	cse_shape::state_import(@_);
}
sub state_export {
	cse_alife_object::state_export(@_);
	cse_shape::state_export(@_);
}

#######################################################################
#clear sky 1.5.0.4 compatible
package cse_alife_object_physic;
use strict;
use constant properties_info => (
	{ name => 'physic_type',	type => 'h32',	default => 0 },		# 0x138
	{ name => 'mass',		type => 'f32',	default => 0.0 },	# 0x13c
	{ name => 'fixed_bones',	type => 'sz',	default => '' },	# 0x140
);
use constant upd_properties_info => (
	{ name => 'upd:num_items',			type => 'h8' },		# 0x30 clear sky
	{ name => 'upd:position',			type => 'h32v3' },	# 0x4c clear sky
	{ name => 'upd:cse_alife_object_physic__unk1',	type => 'h32v3' },	# 0x58 clear sky
	{ name => 'upd:cse_alife_object_physic__unk2',	type => 'h32v3' },	# 0x64 clear sky
	{ name => 'upd:cse_alife_object_physic__unk3',	type => 'h32v4' },	# 0x7c clear sky
	{ name => 'upd:cse_alife_object_physic__unk4',	type => 'h32v3' },	# 0x40 clear sky
	{ name => 'upd:cse_alife_object_physic__unk5',	type => 'h32v3' },	# 0x34 clear sky	
	{ name => 'upd:cse_alife_object_physic__marker_one',	type => 'h8', default => 1 }, #clear sky	
);
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	cse_ph_skeleton::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	cse_ph_skeleton::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	$_[1]->unpack_properties($_[0], (upd_properties_info)[0]);
	return if ($_[0]->{'upd:num_items'} == 0);
	$_[1]->unpack_properties($_[0], (upd_properties_info)[1 .. 4]);	
	my $info_mask = $_[0]->{'upd:num_items'} >> 5;
	if (($info_mask & 0x02) == 0) {
		die unless $_[1]->length() >= 12;
		$_[1]->unpack_properties($_[0], (upd_properties_info)[5]);
	}
	if (($info_mask & 0x04) == 0) {
		die unless $_[1]->length() >= 12;
		$_[1]->unpack_properties($_[0], (upd_properties_info)[6]);		
	}
	$_[1]->unpack_properties($_[0], (upd_properties_info)[7]);
	$_[0]->{'upd:cse_alife_object_physic__marker_one'} == 1 or die;
}
sub update_write {
	$_[1]->pack_properties($_[0], (upd_properties_info)[0]);
	return if ($_[0]->{'upd:num_items'} == 0);
	$_[1]->pack_properties($_[0], (upd_properties_info)[1 .. 4]);
	my $info_mask = $_[0]->{'upd:num_items'} >> 5;
	if (($info_mask & 0x02) == 0) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[5]);
	}
	if (($info_mask & 0x04) == 0) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[6]);
	}
	$_[1]->pack_properties($_[0], (upd_properties_info)[7]);
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	cse_ph_skeleton::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
	$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[0]);
	return if ($_[0]->{'upd:num_items'} == 0);
	$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[1 .. 4]);
	my $info_mask = $_[0]->{'upd:num_items'} >> 5;
	if (($info_mask & 0x02) == 0) {
		$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[5]);
	}
	if (($info_mask & 0x04) == 0) {
		$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[6]);
	}
	$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[7]);	
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	cse_ph_skeleton::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	$_[1]->export_properties(undef, $_[0], (upd_properties_info)[0]);
	return if ($_[0]->{'upd:num_items'} == 0);
	$_[1]->export_properties(undef, $_[0], (upd_properties_info)[1 .. 4]);
	my $info_mask = $_[0]->{'upd:num_items'} >> 5;
	if (($info_mask & 0x02) == 0) {
		$_[1]->export_properties(undef, $_[0], (upd_properties_info)[5]);
	}
	if (($info_mask & 0x04) == 0) {
		$_[1]->export_properties(undef, $_[0], (upd_properties_info)[6]);
	}
	$_[1]->export_properties(undef, $_[0], (upd_properties_info)[7]);	
}

#######################################################################
#clear sky 1.5.0.4 compatible
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
	{ name => 'main_range',		type => 'f32',	default => 0.0 },	# 0x14c
	{ name => 'light_flags',	type => 'h16',	default => 0 },		# 0x138
	{ name => 'cse_alife_object_hanging_lamp__unk1_sz',	type => 'sz',	default => '$editor' },	# 0xe0
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
	{ name => 'unk1',	type => 'f32',	default => '1' },		#0x158 clear sky
	{ name => 'unk2',	type => 'f32',	default => '1' },		#0x15c clear sky
	{ name => 'unk3',	type => 'f32',	default => '1' },		#0x160 clear sky	
);
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	cse_ph_skeleton::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	cse_ph_skeleton::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	cse_ph_skeleton::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	cse_ph_skeleton::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
package cse_alife_object_projector;
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
package cse_alife_inventory_box;
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
	if ($_[0]->{section_name} eq 'stationary_mgun') {
		$_[1]->unpack_properties($_[0], upd_properties_info);
	}
}
sub update_write {
	if ($_[0]->{section_name} eq 'mounted_weapon') {
		;
	} elsif ($_[0]->{section_name} eq 'stationary_mgun') {
		$_[1]->pack_properties($_[0], upd_properties_info);
	}
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	$_[1]->export_properties(undef, $_[0], upd_properties_info);
}

#######################################################################
#clear sky 1.5.0.4 compatible
package cse_alife_ph_skeleton_object;
use strict;
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	cse_ph_skeleton::state_read(@_);
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	cse_ph_skeleton::state_write(@_);
}
sub update_read {
	cse_ph_skeleton::update_read(@_);
}
sub update_write {
	cse_ph_skeleton::update_write(@_);
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	cse_ph_skeleton::state_import(@_);
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	cse_ph_skeleton::state_export(@_);
}

#######################################################################
package cse_alife_car;
use strict;
use constant properties_info => (
	{ name => 'health', type => 'f32', default => 1.0 },	
);
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	cse_ph_skeleton::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	cse_ph_skeleton::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
#sub update_read {
#	cse_ph_skeleton::update_read(@_);
#}
#sub update_write {
#	cse_ph_skeleton::update_write(@_);
#}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	cse_ph_skeleton::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	cse_ph_skeleton::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
#clear sky 1.5.0.4 compatible
package cse_alife_helicopter;
use strict;
use constant properties_info => (
	{ name => 'cse_alife_helicopter__unk1_sz',	type => 'sz', default => '' },	# 0xe0
	{ name => 'engine_sound',			type => 'sz', default => '' },	# 0x140
);
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	cse_motion::state_read(@_);
	cse_ph_skeleton::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	cse_motion::state_write(@_);
	cse_ph_skeleton::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	cse_motion::state_import(@_);
	cse_ph_skeleton::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	cse_motion::state_export(@_);
	cse_ph_skeleton::state_export(@_);
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
	{ name => 'upd:g_team',		type => 'u8' },		# 0xe8
	{ name => 'upd:g_squad',	type => 'u8' },		# 0xe9
	{ name => 'upd:g_group',	type => 'u8' },		# 0xea
);
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	$_[1]->unpack_properties($_[0], upd_properties_info);
}
sub update_write {
	$_[1]->pack_properties($_[0], upd_properties_info);
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	$_[1]->export_properties(undef, $_[0], upd_properties_info);
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
);
sub state_read {
	cse_alife_creature_abstract::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_creature_abstract::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	cse_alife_creature_abstract::update_read(@_);
	$_[1]->unpack_properties($_[0], upd_properties_info);
}
sub update_write {
	cse_alife_creature_abstract::update_write(@_);
	$_[1]->pack_properties($_[0], upd_properties_info);
}
sub state_import {
	cse_alife_creature_abstract::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
}
sub state_export {
	cse_alife_creature_abstract::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	$_[1]->export_properties(undef, $_[0], upd_properties_info);
}

#######################################################################
package cse_alife_trader_abstract;
use strict;
use constant eTraderFlagInfiniteAmmo	=> 0x00000001;
use constant eTraderFlagDummy		=> 0x00000000;	# really???
use constant properties_info => (
	{ name => 'money',		type => 'u32',	default => 0 },		# 8+0x04=0x0c
	{ name => 'specific_character',	type => 'sz',	default => '' },	# 8+0x3c=0x44 
	{ name => 'trader_flags',	type => 'h32',	default => 0x1 },	# 8+0x0c=0x14
	{ name => 'character_profile',	type => 'sz',	default => '' },	# 8+0x38=0x40
	{ name => 'community_index',	type => 'h32',	default => 0xffffffff },# 8+0x10=0x18 
	{ name => 'rank',		type => 'h32',	default => 0x80000001 },# 8+0x18=0x20
	{ name => 'reputation',		type => 'h32',	default => 0x80000001 },# 8+0x14=0x1c
	{ name => 'character_name',	type => 'sz',	default => '' },	# 8+0x40=0x48
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
package cse_alife_human_abstract;
use strict;
use constant properties_info => (
	{ name => 'predicate5',	type => 'l32u8v', default => [] },	# 0x38
	{ name => 'predicate4',	type => 'l32u8v', default => [] },	# 0x44
);
sub state_read {
	cse_alife_trader_abstract::state_read(@_);

	cse_alife_monster_abstract::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_trader_abstract::state_write(@_);
	cse_alife_monster_abstract::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	cse_alife_monster_abstract::update_read(@_);
}
sub update_write {
	cse_alife_monster_abstract::update_write(@_);
}
sub state_import {
	cse_alife_trader_abstract::state_import(@_);
	cse_alife_monster_abstract::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_trader_abstract::state_export(@_);
	cse_alife_monster_abstract::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
package cse_alife_human_stalker;
use strict;
use constant upd_properties_info => (
	{ name => 'upd:start_dialog', type => 'sz' },	# 0x2b8
);
sub state_read {
	cse_alife_human_abstract::state_read(@_);
	cse_ph_skeleton::state_read(@_);
}
sub state_write {
	cse_alife_human_abstract::state_write(@_);
	cse_ph_skeleton::state_write(@_);
}
sub update_read {
	cse_alife_human_abstract::update_read(@_);
	$_[1]->unpack_properties($_[0], upd_properties_info);
}
sub update_write {
	cse_alife_human_abstract::update_write(@_);
	$_[1]->pack_properties($_[0], upd_properties_info);
}
sub state_import {
	cse_alife_human_abstract::state_import(@_);
	cse_ph_skeleton::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
}
sub state_export {
	cse_alife_human_abstract::state_export(@_);
	cse_ph_skeleton::state_export(@_);
	$_[1]->export_properties(undef, $_[0], upd_properties_info);
}

#######################################################################
package se_stalker;
use strict;
use constant properties_info => (
	{ name => 'job_online',			type => 'u8',	default => 2 },
	{ name => 'death_droped',	type => 'u8',	default => 0 },
	{ name => 'squad_id',		type => 'sz',	default => "nil" },
	{ name => 'sim_forced_online',	type => 'u8',	default => 0 },
);
sub state_read {
	cse_alife_human_stalker::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
	$_[0]->{'job_online'} != 3 or die;	
}
sub state_write {
	cse_alife_human_stalker::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	cse_alife_human_stalker::update_read(@_);
}
sub update_write {
	cse_alife_human_stalker::update_write(@_);
}
sub state_import {
	cse_alife_human_stalker::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_human_stalker::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
package cse_alife_trader;
use strict;
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	cse_alife_trader_abstract::state_read(@_);
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	cse_alife_trader_abstract::state_write(@_);
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	cse_alife_trader_abstract::state_import(@_);
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	cse_alife_trader_abstract::state_export(@_);
}

#######################################################################
package cse_alife_creature_actor;
use strict;
use constant properties_info => (
	{ name => 'holder_id', type => 'h16', default => 0xffff },	# 0x224 (0x22c)
);
use constant upd_properties_info => (
	{ name => 'upd:actor_state',		type => 'h16' },	# 0x200 (0x208) mstate
	{ name => 'upd:actor_accel_header',	type => 'h16' },	#
	{ name => 'upd:actor_accel_data',	type => 'h32' },	# 0x202,0x206,0x21a (0x20a+)
	{ name => 'upd:actor_velocity_header',	type => 'h16' },	#
	{ name => 'upd:actor_velocity_data',	type => 'h32' },	# 0x20e,0x212,0x216 (0x216+)
	{ name => 'upd:actor_radiation',	type => 'f32' },	# 0x21c (0x224) fRadiation
	{ name => 'upd:actor_weapon',		type => 'u8' },		# 0x220 (0x228) weapon
	{ name => 'upd:num_items',		type => 'u16' },	# 0x222 (0x22a) num_items
);
sub state_read {
	cse_alife_creature_abstract::state_read(@_);
	cse_alife_trader_abstract::state_read(@_);
	cse_ph_skeleton::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_creature_abstract::state_write(@_);
	cse_alife_trader_abstract::state_write(@_);
	cse_ph_skeleton::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	cse_alife_creature_abstract::update_read(@_);
	$_[1]->unpack_properties($_[0], upd_properties_info);
	die unless $_[0]->{'upd:num_items'} == 0;
}
sub update_write {
	cse_alife_creature_abstract::update_write(@_);
	$_[1]->pack_properties($_[0], upd_properties_info);
}
sub state_import {
	cse_alife_creature_abstract::state_import(@_);
	cse_alife_trader_abstract::state_import(@_);
	cse_ph_skeleton::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
}
sub state_export {
	cse_alife_creature_abstract::state_export(@_);
	cse_alife_trader_abstract::state_export(@_);
	cse_ph_skeleton::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	$_[1]->export_properties(undef, $_[0], upd_properties_info);
}

#######################################################################
package cse_alife_creature_crow;
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
package cse_alife_monster_base;
use strict;
use constant properties_info => (
	{ name => 'spec_object_id', type => 'u16', default => 65535 },	# 0x248
);
sub state_read {
	cse_alife_monster_abstract::state_read(@_);
	cse_ph_skeleton::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_monster_abstract::state_write(@_);
	cse_ph_skeleton::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	cse_alife_monster_abstract::update_read(@_);
}
sub update_write {
	cse_alife_monster_abstract::update_write(@_);
}
sub state_import {
	cse_alife_monster_abstract::state_import(@_);
	cse_ph_skeleton::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_monster_abstract::state_export(@_);
	cse_ph_skeleton::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
package se_monster;
use strict;
use constant properties_info => (
	{ name => 'job_online',			type => 'u8',	default => 2 },
	{ name => 'squad_id',			type => 'sz',	default => 'nil' },	
	{ name => 'sim_forced_online',	type => 'u8',	default => 0 },
);
sub state_read {
	cse_alife_monster_base::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
	$_[0]->{'job_online'} == 2 or die;
}
sub state_write {
	cse_alife_monster_base::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	cse_alife_monster_base::update_read(@_);
}
sub update_write {
	cse_alife_monster_base::update_write(@_);
}
sub state_import {
	cse_alife_monster_base::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_monster_base::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
package cse_alife_monster_zombie;
use strict;
use constant properties_info => (
	{ name => 'field_of_view',	type => 'f32',	default => 0.0 },	# 0x1f8
	{ name => 'eye_range',		type => 'f32',	default => 0.0 },	# 0x1fc 
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
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_monster_abstract::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	cse_alife_monster_abstract::update_read(@_);
}
sub update_write {
	cse_alife_monster_abstract::update_write(@_);
}
sub state_import {
	cse_alife_monster_abstract::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_monster_abstract::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
#clear sky 1.5.0.4 compatible :)
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
	cse_alife_object::state_read(@_);
	cse_shape::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_object::state_write(@_);
	cse_shape::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_object::state_import(@_);
	cse_shape::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_object::state_export(@_);
	cse_shape::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
#clear sky 1.5.0.4 compatible :)
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
package se_anomaly_field;
use strict;
use constant properties_info => (
	{ name => 'initialized', type => 'u8', default => 0 },
);
sub state_read {
	cse_alife_space_restrictor::state_read(@_);
$_[1]->length() == 1 or die;
	$_[1]->unpack_properties($_[0], properties_info);
	$_[0]->{initialized} == 0 or die;
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
package se_respawn;
use strict;
use constant properties_info => (
	{ name => 'spawned_obj_count', type => 'u8', default => 0 },
);
sub state_read {
	cse_alife_smart_zone::state_read(@_);
$_[1]->length() == 1 or die;
	$_[1]->unpack_properties($_[0], properties_info);
	$_[0]->{spawned_obj_count} == 0 or die;
}
sub state_write {
	cse_alife_smart_zone::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_smart_zone::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_smart_zone::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
package se_smart_terrain;
use strict;
use constant properties_info => (
	#self.combat_manager:save(packet)
	{ name => 'actor_defence_come',	type => 'u8' },	
	{ name => 'combat_quest',	type => 'sz' },		
	{ name => 'task',	type => 'u16' },	
	{ name => 'see_actor_enemy',	type => 'sz' },	
	{ name => 'flag',	type => 'u8' },	#next CTime
	{ name => 'squads_count',	type => 'u8' },	#next squads collection
	{ name => 'force_online',	type => 'u8' },
	{ name => 'force_online_squads_count',	type => 'u8' }, #next force_online_squads collection
		# self.cover_manager:save(packet)
	{ name => 'cover_manager_is_valid',	type => 'u8' },
	{ name => 'cover_manager_cover_table_count',	type => 'u8' }, #next cover_table collection
			#set_save_marker(packet, "save", true, "CCover_manager")
	{ name => 'se_smart_terrain_combat_manager_cover_manager__marker',	type => 'u16', default => 2}, 
		#set_save_marker(packet, "save", true, "CCombat_manager")
	{ name => 'se_smart_terrain_combat_manager__marker',	type => 'u16', default => 19}, 
	
	{ name => 'npc_info_count',	type => 'u8',	default => 0 },	#next npc_info collection
	{ name => 'dead_time_count',	type => 'u8',	default => 0 },	#next dead_time collection
	
	#set_save_marker(packet, "save", true, "se_smart_terrain")	
	{ name => 'se_smart_terrain__marker',	type => 'u16', default => 23}, 	
);
sub state_read {
	cse_alife_smart_zone::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
	$_[0]->{flag} == 0 or die;	
	$_[0]->{squads_count} == 0 or die;
	$_[0]->{force_online_squads_count} == 0 or die;
	$_[0]->{cover_manager_cover_table_count} == 0 or die;
	if ($_[0]->{npc_info_count} != 0) {
		printf "npc_info_count="; printf $_[0]->{npc_info_count};
		die;
	}
	$_[0]->{dead_time_count} == 0 or die;
}
sub state_write {
	cse_alife_smart_zone::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_smart_zone::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_smart_zone::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
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
#clear sky 1.5.0.4 compatible
package cse_alife_level_changer;
use strict;
use constant properties_info => (
	{ name => 'dest_game_vertex_id',	type => 'u16',	default => 0 },
	{ name => 'dest_level_vertex_id',	type => 'u32',	default => 0 },
	{ name => 'dest_position',		type => 'f32v3',default => [] },
	{ name => 'dest_direction',		type => 'f32v3',default => [] },
	{ name => 'dest_level_name',		type => 'sz',	default => '' },
	{ name => 'dest_graph_point',		type => 'sz',	default => '' },
	{ name => 'silent_mode',		type => 'u8',	default => 0 },
);
sub state_read {
	cse_alife_space_restrictor::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_space_restrictor::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
# sub update_read {
	# my $self = shift;
	# my ($packet) = @_;
# }
sub state_import {
	cse_alife_space_restrictor::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_space_restrictor::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
package cse_alife_custom_zone;
use strict;
use constant properties_info => (
	{ name => 'max_power',		type => 'f32',	default => 0.0 },	# 0xf0
	{ name => 'owner_id',		type => 'h32',	default => 0xffffffff },# 0xf8
	{ name => 'enabled_time',	type => 'u32',	default => 0 },		# 0xfc
	{ name => 'disabled_time',	type => 'u32',	default => 0 },		# 0x100
	{ name => 'start_time_shift',	type => 'u32',	default => 0 },		# 0x104
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
package cse_alife_anomalous_zone;
use strict;
use constant properties_info => (
	{ name => 'offline_interactive_radius',	type => 'f32',	default => 0.0 },	# 0x10c
	{ name => 'artefact_spawn_count',	type => 'u16',	default => 0 },		# 0x114
	{ name => 'artefact_position_offset',	type => 'h32',	default => 0 },		# 0x110
);
sub state_read {
	cse_alife_custom_zone::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_custom_zone::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_custom_zone::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_custom_zone::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
package se_zone_anom;
use strict;
use constant properties_info => (
	{ name => 'last_spawn_time_present', type => 'u8', default => 0 },
);
sub state_read {
	cse_alife_anomalous_zone::state_read(@_);
$_[1]->length() == 1 or die;
	$_[1]->unpack_properties($_[0], properties_info);
	$_[0]->{last_spawn_time_present} == 0 or die;
}
sub state_write {
	cse_alife_anomalous_zone::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_anomalous_zone::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_anomalous_zone::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
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
	cse_visual::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_anomalous_zone::state_write(@_);
	cse_visual::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_anomalous_zone::state_import(@_);
	cse_visual::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_anomalous_zone::state_export(@_);
	cse_visual::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
package se_zone_visual;
use strict;
use constant properties_info => (
	{ name => 'last_spawn_time_present', type => 'u8', default => 0 },
);
sub state_read {
	cse_alife_zone_visual::state_read(@_);
$_[1]->length() == 1 or die;
	$_[1]->unpack_properties($_[0], properties_info);
	$_[0]->{last_spawn_time_present} == 0 or die;
}
sub state_write {
	cse_alife_zone_visual::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_zone_visual::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_zone_visual::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
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
sub state_import {
	cse_alife_custom_zone::state_import(@_);
	cse_motion::state_import(@_);
}
sub state_export {
	cse_alife_custom_zone::state_export(@_);
	cse_motion::state_export(@_);
}

#######################################################################
package cse_alife_online_offline_group;
use strict;
sub state_read {
	my $self = shift;
	my ($packet) = @_;

	cse_alife_dynamic_object::state_read($self, $packet);
	my ($temp) = $packet->unpack('l');
	if ($temp > 0) {
		($self->{cse_alife_online_offline_group__unk1_u16}) = $packet->unpack('v');
	}
}
sub state_write {
	my $self = shift;
	my ($packet) = @_;

	die;
}

#######################################################################
#clear sky 1.5.0.4 modified
package cse_alife_item;
use strict;
use constant properties_info => (
	{ name => 'condition', type => 'f32', default => 0.0 },		# 0xec
	{ name => 'upgrade_count', type => 'u32', default => 0 },	# clear sky 1.5.0.4 next upgrade collection with element = stringZ
);
use constant upd_properties_info => (
	{ name => 'upd:num_items',			type => 'h8' },		# 0x30 clear sky
	{ name => 'upd:position',			type => 'f32v3' },	# 0x4c clear sky
	{ name => 'upd:cse_alife_item__unk1_f32v3',	type => 'f32v3' },	# 0x58 clear sky
	{ name => 'upd:cse_alife_item__unk2_f32v3',	type => 'f32v3' },	# 0x64 clear sky
	{ name => 'upd:cse_alife_item__unk3_f32v4',	type => 'f32v4' },	# 0x7c clear sky
	{ name => 'upd:cse_alife_item__unk4_f32v3',	type => 'f32v3' },	# 0x40 clear sky
	{ name => 'upd:cse_alife_item__unk5_f32v3',	type => 'f32v3' },	# 0x34 clear sky	
	{ name => 'upd:cse_alife_item__marker_one',	type => 'u8', default => 1 }, #clear sky	
);
sub state_read {
	cse_alife_dynamic_object_visual::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
	$_[0]->{'upgrade_count'} == 0 or die	
}
sub state_write {
	cse_alife_dynamic_object_visual::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	($_[0]->{'upd:num_items'}) = $_[1]->unpack('C');
	return if ($_[0]->{'upd:num_items'} == 0);
	@{$_[0]->{'upd:position'}} = $_[1]->unpack('f3');
	@{$_[0]->{'upd:cse_alife_item__unk1_f32v3'}} = $_[1]->unpack('f3');
	@{$_[0]->{'upd:cse_alife_item__unk2_f32v3'}} = $_[1]->unpack('f3');
	@{$_[0]->{'upd:cse_alife_item__unk3_f32v4'}} = $_[1]->unpack('f4');
	my $info_mask = $_[0]->{'upd:num_items'} >> 5;
	if (($info_mask & 0x02) == 0) {
		die unless $_[1]->length() >= 12;
		@{$_[0]->{'upd:cse_alife_item__unk4_f32v3'}} = $_[1]->unpack('f3');
	}
	if (($info_mask & 0x04) == 0) {
		die unless $_[1]->length() >= 12;
		@{$_[0]->{'upd:cse_alife_item__unk5_f32v3'}} = $_[1]->unpack('f3');
	}
	($_[0]->{'upd:cse_alife_item__marker_one'}) = $_[1]->unpack('C');
	$_[0]->{'upd:cse_alife_item__marker_one'} == 1 or die;
}
sub update_write {
	$_[1]->pack_properties($_[0], (upd_properties_info)[0]);
	return if ($_[0]->{'upd:num_items'} == 0);
	$_[1]->pack_properties($_[0], (upd_properties_info)[1 .. 4]);
	my $info_mask = $_[0]->{'upd:num_items'} >> 5;
	if (($info_mask & 0x02) == 0) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[5]);
	}
	if (($info_mask & 0x04) == 0) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[6]);
	}
	$_[1]->pack_properties($_[0], (upd_properties_info)[7]);	
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
	$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[0]);
	return if ($_[0]->{'upd:num_items'} == 0);
	$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[1 .. 4]);
	my $info_mask = $_[0]->{'upd:num_items'} >> 5;
	if (($info_mask & 0x02) == 0) {
		$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[5]);
	}
	if (($info_mask & 0x04) == 0) {
		$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[6]);
	}
	$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[7]);	
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	$_[1]->export_properties(undef, $_[0], (upd_properties_info)[0]);
	return if ($_[0]->{'upd:num_items'} == 0);
	$_[1]->export_properties(undef, $_[0], (upd_properties_info)[1 .. 4]);
	my $info_mask = $_[0]->{'upd:num_items'} >> 5;
	if (($info_mask & 0x02) == 0) {
		$_[1]->export_properties(undef, $_[0], (upd_properties_info)[5]);
	}
	if (($info_mask & 0x04) == 0) {
		$_[1]->export_properties(undef, $_[0], (upd_properties_info)[6]);
	}
	$_[1]->export_properties(undef, $_[0], (upd_properties_info)[7]);	
}

#######################################################################
package cse_alife_item_torch;
use strict;
use constant flTorchActive		=> 0x01;
use constant flTorchNightVisionActive	=> 0x02;
use constant flTorchUnknown		=> 0x04;
use constant upd_properties_info => (
	{ name => 'upd:torch_flags', type => 'u8' },	# flags -> 0x178,0x179,0x17a (0x1a0, 0x1a1)
);
sub state_read {
	cse_alife_item::state_read(@_);
}
sub state_write {
	cse_alife_item::state_write(@_);
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
	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
}
sub state_export {
	cse_alife_item::state_export(@_);
	$_[1]->export_properties(undef, $_[0], upd_properties_info);
}

#######################################################################
package cse_alife_item_detector;
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
	$_[1]->unpack_properties($_[0], upd_properties_info);
}
sub update_write {
	cse_alife_item::update_write(@_);
	$_[1]->pack_properties($_[0], upd_properties_info);
}
sub state_import {
	cse_alife_item::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
}
sub state_export {
	cse_alife_item::state_export(@_);
	$_[1]->export_properties(undef, $_[0], upd_properties_info);
}

#######################################################################
package cse_alife_item_pda;
use strict;
use constant properties_info => (
	{ name => 'original_owner',	type => 'u16',	default => 0 },		# 0x180
	{ name => 'specific_character',	type => 'sz',	default => '' },	# 0x184
	{ name => 'info_portion',	type => 'sz',	default => '' },	# 0x188
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
}
sub update_write {
	cse_alife_item::update_write(@_);
}
sub state_import {
	cse_alife_item::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_item::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
package cse_alife_item_document;
use strict;
use constant properties_info => (
	{ name => 'info_portion', type => 'sz', default => '' },	# 0x180
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
}
sub update_write {
	cse_alife_item::update_write(@_);
}
sub state_import {
	cse_alife_item::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_item::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
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
	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
}
sub state_export {
	cse_alife_item::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	$_[1]->export_properties(undef, $_[0], upd_properties_info);
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
	{ name => 'cse_alife_item_weapon:unk1',		type => 'u8', },	# 0x198 clear sky
);
use constant upd_properties_info => (
	{ name => 'upd:condition',	type => 'q8' },		# 0xec
	{ name => 'upd:weapon_flags',	type => 'u8' },		# 0x190 (0x1b0)
	{ name => 'upd:ammo_elapsed',	type => 'u16' },	# 0x196 (0x1b6)
	{ name => 'upd:addon_flags',	type => 'u8' },		# 0x1a8 (0x1cc)
	{ name => 'upd:ammo_type',	type => 'u8' },		# 0x192 (0x1b2)
	{ name => 'upd:weapon_state',	type => 'u8' },		# 0x191 (0x1b1)
	{ name => 'upd:weapon_zoom',	type => 'u8' },		# 0x1a9 (0x1cd)
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
	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
}
sub state_export {
	cse_alife_item::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	$_[1]->export_properties(undef, $_[0], upd_properties_info);
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
	$_[1]->unpack_properties($_[0], upd_properties_info);
}
sub update_write {
	cse_alife_item_weapon::update_write(@_);
	$_[1]->pack_properties($_[0], upd_properties_info);
}
sub state_import {
	cse_alife_item_weapon::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
}
sub state_export {
	cse_alife_item_weapon::state_export(@_);
	$_[1]->export_properties(undef, $_[0], upd_properties_info);
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
	$_[1]->unpack_properties($_[0], upd_properties_info);
	cse_alife_item_weapon_magazined::update_read(@_);
}
sub update_write {
	$_[1]->pack_properties($_[0], upd_properties_info);
	cse_alife_item_weapon_magazined::update_write(@_);
}
sub state_import {
	cse_alife_item_weapon_magazined::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
}
sub state_export {
	cse_alife_item_weapon_magazined::state_export(@_);
	$_[1]->export_properties(undef, $_[0], upd_properties_info);
}

#######################################################################
package cse_alife_item_weapon_shotgun;
use strict;
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
	$_[1]->unpack_properties($_[0], upd_properties_info);
die unless $_[1]->length() == 0;
}
sub update_write {
	cse_alife_item_weapon_magazined::update_write(@_);
	$_[1]->pack_properties($_[0], upd_properties_info);
}
sub state_import {
	cse_alife_item_weapon_magazined::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], upd_properties_info);
}
sub state_export {
	cse_alife_item_weapon_magazined::state_export(@_);
	$_[1]->export_properties(undef, $_[0], upd_properties_info);
}
#######################################################################
package se_actor;
use strict;
use constant properties_info => (
	{ name => 'dumb',	type => 'actorData' }, 
	#set_save_marker(packet, "save", false, "se_actor")
	#task_manager.get_task_manager():save(packet)
	# { name => 'inited_tasks_count',			type => 'u16' },#next inited_task collection
	##	self.id_generator:save(packet)
	# { name => 'id_generator_count',	type => 'u16' },
	# { name => 'id_generator_last_id',	type => 'u16' },
	# { name => 'id_generator_free_count',	type => 'u16' }, #next free collection	
	# { name => 'id_generator_given_count',	type => 'u16' }, #next given collection
	
	# { name => 'rewards_count',	type => 'u8' }, #next rewards collection
	# { name => 'inited_find_upgrade_tasks_count',	type => 'u16' }, #next inited_find_upgrade_tasks collection
	
	##sim_board.get_sim_board().id_generator:save(packet)
	# { name => 'sim_obj_coll_count',	type => 'u16' },
	# { name => 'sim_obj_coll_last_id',	type => 'u16' },
	# { name => 'sim_obj_coll_free_count',	type => 'u16' }, #next free collection	
	# { name => 'sim_obj_coll_given_count',	type => 'u16' }, #next given collection	
	
	##minigames_manager.get_minigames_manager():save(packet)
	# { name => 'minigames_count',	type => 'u16' },
	# { name => 'minigame1_id',	type => 'sz' },	
	# { name => 'minigame1_profile_name',	type => 'sz' },	
	# { name => 'minigame1_state',	type => 'sz' },	
	# { name => 'minigame1_highscore',	type => 'u8' },
	# { name => 'minigame1_timer',	type => 'u16' }, 
	# { name => 'minigame1_win',	type => 'u8' },
	# { name => 'minigame1_crows_to_kill_count',	type => 'u8' },	
	# { name => 'minigame1_crows_to_kill1',	type => 'u8' },	
	# { name => 'minigame1_crows_to_kill1',	type => 'u8' },	
	# { name => 'minigame1_crows_to_kill1',	type => 'u8' },	
	# { name => 'minigame1_crows_to_kill1',	type => 'u8' },	
	# { name => 'minigame1_money_multiplier',	type => 'u16' }, 
	# { name => 'minigame1_champion_multiplier',	type => 'u16' }, 
	# { name => 'minigame1_selected',	type => 'u8' },	
	# { name => 'minigame1_game_type',	type => 'sz' },	
	# { name => 'minigame1_high_score',	type => 'u8' },	
	# { name => 'minigame1_timer_',	type => 'u16' }, 
	# { name => 'minigame1_time_out',	type => 'u16' }, 
	# { name => 'minigame1_killed_counter',	type => 'u8' },	
	# { name => 'minigame1_win_',	type => 'u8' },	
	
	# { name => 'minigame2_id',	type => 'sz' },	
	# { name => 'minigame2_profile_name',	type => 'sz' },	
	# { name => 'minigame2_state',	type => 'sz' },		
	# { name => 'minigame2_game_type',	type => 'sz' },
	# { name => 'minigame2_wpn_type',	type => 'sz' },
	# { name => 'minigame2_stand_way',	type => 'sz' },
	# { name => 'minigame2_look_way',	type => 'sz' },
	# { name => 'minigame2_stand_way_back',	type => 'sz' },
	# { name => 'minigame2_look_way_back',	type => 'sz' },
	# { name => 'minigame2_obj_name',	type => 'sz' },
	# { name => 'minigame2_marker_one',	type => 'u8' },
	# { name => 'minigame2_win',	type => 'u16' },

	# { name => 'minigame2_distance',	type => 'u8' },
	# { name => 'minigame2_ammo',	type => 'u8' },
	# { name => 'minigame2_targets_count',	type => 'u8' }, #next targets collection
	# { name => 'minigame2_target_counter',	type => 'u8' },

	# { name => 'minigame2_inventory_items_count',	type => 'u8' },	# next inventory_items collection
	# { name => 'minigame2_prev_time',	type => 'u32' },
	# { name => 'minigame2_cur_game',	type => 'sz' }, #next cur_game params	
);
sub state_read {
	cse_alife_creature_actor::state_read(@_);

	#printf "$_[1]->length() = "; printf $_[1]->length;
	$_[1]->unpack_properties($_[0], properties_info);
	# $_[0]->{inited_tasks_count} == 0 or die;
	# $_[0]->{id_generator_free_count} == 0 or die;	
	# $_[0]->{id_generator_given_count} == 0 or die;		
	# $_[0]->{rewards_count} == 0 or die;		
	# $_[0]->{inited_find_upgrade_tasks_count} == 0 or die;	
	# $_[0]->{sim_obj_coll_free_count} == 0 or die;	
	# $_[0]->{sim_obj_coll_given_count} == 0 or die;	
	# $_[0]->{minigames_count} == 2 or die;	
	# $_[0]->{minigame1_crows_to_kill_count} == 4 or die;		
	# $_[0]->{minigame2_marker_one} == 1 or die;	
	# $_[0]->{minigame2_targets_count} == 0 or die;	
	# $_[0]->{minigame2_inventory_items_count} == 0 or die;
	# $_[0]->{minigame2_cur_game} eq "nil" or die;	
}
sub state_write {
	cse_alife_creature_actor::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
	cse_alife_creature_actor::update_read(@_);
}
sub update_write {
	cse_alife_creature_actor::update_write(@_);
}
sub state_import {
	cse_alife_creature_actor::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_creature_actor::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
package se_sim_faction;
use strict;
use constant properties_info => (
	{ name => 'community_player', type => 'u8' },
	{ name => 'start_position_filled', type => 'u8' },
	{ name => 'current_expansion_level', type => 'u8' },	
	{ name => 'last_spawn_time_marker', type => 'u8', default => 255 },	#{ name => 'last_spawn_time', type => 'u8v8' },		
	{ name => 'squad_target_cache_count', type => 'u8' }, #next squad_target_cache collection
	{ name => 'random_tasks_count', type => 'u8' }, #next random_tasks collection
	{ name => 'current_attack_quantity_count', type => 'u8' }, #next current_attack_quantity collection
	{ name => 'squads_count', type => 'u16' }, #next squads collection
	#set_save_marker(packet, "save", true, "se_sim_faction")
	{ name => 'se_sim_faction__marker', type => 'u16', default => 9},
);
sub state_read {
	cse_alife_smart_zone::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
	$_[0]->{last_spawn_time_marker} == 255 or die;
	$_[0]->{squad_target_cache_count} == 0 or die;
	$_[0]->{random_tasks_count} == 0 or die;
	$_[0]->{current_attack_quantity_count} == 0 or die;
	$_[0]->{squads_count} == 0 or die;
}
sub state_write {
	cse_alife_smart_zone::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_smart_zone::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_smart_zone::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
#clear sky 1.5.0.4 compatible
package cse_smart_cover;
use strict;
use constant properties_info => (
	{ name => 'unk1', type => 'sz' }, #0xe4 clear sky
	{ name => 'unk2', type => 'f32v3' }, #0xe8 clear sky
	{ name => 'unk3', type => 'u8' }, #0xf4 clear sky
);
sub state_read {
	cse_alife_object::state_read(@_);
	cse_shape::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_object::state_write(@_);
	cse_shape::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_object::state_import(@_);
	cse_shape::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_object::state_export(@_);
	cse_shape::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
#clear sky 1.5.0.4 new
package se_level_changer;
use strict;
use constant properties_info => (
	{ name => 'enabled', type => 'u8', default => 1 },
	{ name => 'hint', type => 'sz', default => "level_changer_invitation" },	
	{ name => 'se_level_changer__marker', type => 'u16', default => 26},	
);
sub state_read {
	cse_alife_level_changer::state_read(@_);
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	cse_alife_level_changer::state_write(@_);
	$_[1]->pack_properties($_[0], properties_info);
}
sub state_import {
	cse_alife_level_changer::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub state_export {
	cse_alife_level_changer::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

#######################################################################
package alife_object;
use strict;
use stkutils::data_packet;
use constant section_to_class => {
	actor					=> 'se_actor',				#clear sky modified

	graph_point				=> 'cse_alife_graph_point',

	stalker					=> 'se_stalker',
	mar_csky_commander		=> 'se_stalker',
	
	stalker_monolith			=> 'se_stalker',
	stalker_zombied				=> 'se_stalker',
	stalker_trader				=> 'se_stalker',
	stalker_sakharov			=> 'se_stalker',
	m_barman				=> 'se_stalker',
	m_osoznanie				=> 'se_stalker',

	m_trader				=> 'cse_alife_trader',

	m_bloodsucker_e				=> 'se_monster',
	bloodsucker_weak			=> 'se_monster',
	bloodsucker_normal			=> 'se_monster',
	bloodsucker_strong			=> 'se_monster',
	bloodsucker_arena			=> 'se_monster',
	bloodsucker_mil				=> 'se_monster',

	m_boar_e				=> 'se_monster',
	boar_weak				=> 'se_monster',
	boar_normal				=> 'se_monster',
	boar_strong				=> 'se_monster',

	m_burer_e				=> 'se_monster',
	burer_arena				=> 'se_monster',
	m_burer_normal				=> 'se_monster',
	m_burer_normal_black			=> 'se_monster',
	burer_indoor				=> 'se_monster',
	burer_outdoor				=> 'se_monster',

	m_cat_e					=> 'se_monster',
	cat_weak				=> 'se_monster',
	cat_normal				=> 'se_monster',
	cat_strong				=> 'se_monster',

	m_chimera_e				=> 'se_monster',
	chimera_weak				=> 'se_monster',
	chimera_normal				=> 'se_monster',
	chimera_strong				=> 'se_monster',

	m_controller_e				=> 'se_monster',
	m_controller_normal			=> 'se_monster',
	m_controller_normal_fat			=> 'se_monster',
	m_controller_old			=> 'se_monster',
	m_controller_old_fat			=> 'se_monster',
	controller_tubeman			=> 'se_monster',

	m_crow					=> 'cse_alife_creature_crow',

	m_dog_e					=> 'se_monster',
	dog_weak				=> 'se_monster',
	dog_normal				=> 'se_monster',
	dog_strong				=> 'se_monster',

	m_flesh_e				=> 'se_monster',
	flesh_weak				=> 'se_monster',
	flesh_normal				=> 'se_monster',
	flesh_strong				=> 'se_monster',

	m_fracture_e				=> 'se_monster',
	fracture_normal				=> 'se_monster',
	fracture_strong				=> 'se_monster',

	m_gigant_e				=> 'se_monster',
	gigant_normal				=> 'se_monster',
	gigant_strong				=> 'se_monster',

	m_phantom				=> 'cse_alife_creature_phantom',

	m_poltergeist_e				=> 'se_monster',
	m_poltergeist_normal			=> 'se_monster',
	m_poltergeist_normal_tele		=> 'se_monster',
	m_poltergeist_tele_outdoor		=> 'se_monster',
	m_poltergeist_normal_flame		=> 'se_monster',
	m_poltergeist_strong_flame		=> 'se_monster',

	m_pseudodog_e				=> 'se_monster',
	pseudodog_weak				=> 'se_monster',
	pseudodog_normal			=> 'se_monster',
	pseudodog_strong			=> 'se_monster',
	pseudodog_arena				=> 'se_monster',
	psy_dog					=> 'se_monster',
	psy_dog_phantom				=> 'se_monster',	# cse_alife_psy_dog_phantom really
	psy_dog_radar				=> 'se_monster',

	m_rat_e					=> 'se_monster',

	m_snork_e				=> 'se_monster',
	snork_weak				=> 'se_monster',
	snork_normal				=> 'se_monster',
	snork_strong				=> 'se_monster',
	aes_snork				=> 'se_monster',
	snork_arena				=> 'se_monster',
	snork_indoor				=> 'se_monster',
	snork_outdoor				=> 'se_monster',
	snork_jumper				=> 'se_monster',

	m_tushkano_e				=> 'se_monster',
	m_tushkano_normal			=> 'se_monster',
	tushkano_normal				=> 'se_monster',

	m_zombie_e				=> 'se_monster',
	zombie_weak				=> 'se_monster',
	zombie_normal				=> 'se_monster',
	zombie_strong				=> 'se_monster',
	zombie_immortal				=> 'se_monster',

	breakable_object			=> 'cse_alife_object_breakable',

	search_light				=> 'cse_alife_object_projector',
	physic_object				=> 'cse_alife_object_physic',
	physic_destroyable_object		=> 'cse_alife_object_physic',
	climable_object				=> 'cse_alife_object_climable',
	lights_hanging_lamp			=> 'cse_alife_object_hanging_lamp',

	inventory_box				=> 'cse_alife_inventory_box',

	space_restrictor			=> 'cse_alife_space_restrictor',
	sim_faction					=> 'se_sim_faction', #clear sky modified
	
	script_zone				=> 'cse_alife_space_restrictor',
	touch_zone				=> 'cse_alife_space_restrictor',

	level_changer				=> 'se_level_changer',

	zone_team_base				=> 'cse_alife_team_base_zone',

	smart_terrain				=> 'se_smart_terrain',
	
	respawn					=> 'se_respawn',
	
	anomaly_field				=> 'se_anomaly_field',

	zone_burning_fuzz			=> 'se_zone_visual',
	zone_burning_fuzz1			=> 'se_zone_visual',
	zone_burning_fuzz_weak			=> 'se_zone_visual',
	zone_burning_fuzz_average		=> 'se_zone_visual',
	zone_burning_fuzz_strong		=> 'se_zone_visual',
	zone_burning_fuzz_bottom_weak		=> 'se_zone_visual',
	zone_burning_fuzz_bottom_average	=> 'se_zone_visual',
	zone_burning_fuzz_bottom_strong		=> 'se_zone_visual',

	zone_buzz				=> 'se_zone_anom',
	zone_buzz_weak				=> 'se_zone_anom',
	zone_buzz_average			=> 'se_zone_anom',
	zone_buzz_strong			=> 'se_zone_anom',

	zone_gravi_zone				=> 'se_zone_anom',
	zone_gravi_zone_weak			=> 'se_zone_anom',
	zone_gravi_zone_average			=> 'se_zone_anom',
	zone_gravi_zone_strong			=> 'se_zone_anom',

	zone_campfire_mp_nolight		=> 'cse_alife_anomalous_zone',
	zone_campfire_grill			=> 'cse_alife_anomalous_zone',
	campfire   					=> 'cse_alife_anomalous_zone',
	
	zone_flame				=> 'cse_alife_anomalous_zone',
	zone_zhar				=> 'cse_alife_anomalous_zone',
	zone_emi				=> 'cse_alife_anomalous_zone',
	zone_flame_small			=> 'cse_alife_anomalous_zone',

	zone_mincer				=> 'se_zone_anom',
	zone_mincer_weak			=> 'se_zone_anom',
	zone_mincer_weak_noart			=> 'se_zone_anom',
	zone_mincer_average			=> 'se_zone_anom',
	zone_mincer_strong			=> 'se_zone_anom',

	zone_mine_field				=> 'cse_alife_anomalous_zone',
	zone_death				=> 'cse_alife_anomalous_zone',

	zone_monolith				=> 'cse_alife_anomalous_zone',

	zone_mosquito_bald			=> 'se_zone_anom',
	zone_mosquito_bald_weak			=> 'se_zone_anom',
	zone_mosquito_bald_weak_noart		=> 'se_zone_anom',
	zone_mosquito_bald_average		=> 'se_zone_anom',
	zone_mosquito_bald_strong		=> 'se_zone_anom',
	zone_mosquito_bald_strong_noart		=> 'se_zone_anom',

	zone_no_gravity				=> 'cse_alife_anomalous_zone',

	zone_radioactive			=> 'cse_alife_anomalous_zone',
	zone_radioactive_weak			=> 'cse_alife_anomalous_zone',
	zone_radioactive_average		=> 'cse_alife_anomalous_zone',
	zone_radioactive_strong			=> 'cse_alife_anomalous_zone',
	zone_radioactive_killing		=> 'cse_alife_anomalous_zone',

	zone_teleport				=> 'cse_alife_anomalous_zone',
	zone_teleport_out			=> 'cse_alife_anomalous_zone',
	zone_teleport_monolith			=> 'cse_alife_anomalous_zone',

	torrid_zone				=> 'cse_alife_torrid_zone',
	fireball_zone				=> 'cse_alife_torrid_zone',

	zone_witches_galantine			=> 'se_zone_anom',
	zone_witches_galantine_weak		=> 'se_zone_anom',
	zone_witches_galantine_average		=> 'se_zone_anom',
	zone_witches_galantine_strong		=> 'se_zone_anom',
	zone_witches_galantine_safe		=> 'se_zone_anom',

	zone_zharka_static			=> 'se_zone_anom',
	zone_zharka_static_weak			=> 'se_zone_anom',
	zone_zharka_static_average		=> 'se_zone_anom',
	zone_zharka_static_strong		=> 'se_zone_anom',

	af_ameba_mica				=> 'cse_alife_item_artefact',
	af_ameba_slug				=> 'cse_alife_item_artefact',
	af_blood				=> 'cse_alife_item_artefact',
	af_cristall_flower			=> 'cse_alife_item_artefact',
	af_dummy_battery			=> 'cse_alife_item_artefact',
	af_dummy_dummy				=> 'cse_alife_item_artefact',
	af_dummy_glassbeads			=> 'cse_alife_item_artefact',
	af_dummy_pellicle			=> 'cse_alife_item_artefact',
	af_dummy_spring				=> 'cse_alife_item_artefact',
	af_electra_flash			=> 'cse_alife_item_artefact',
	af_electra_moonlight			=> 'cse_alife_item_artefact',
	af_electra_sparkler			=> 'cse_alife_item_artefact',
	af_fireball				=> 'cse_alife_item_artefact',
	af_fuzz_kolobok				=> 'cse_alife_item_artefact',
	af_gold_fish				=> 'cse_alife_item_artefact',
	af_gravi				=> 'cse_alife_item_artefact',
	af_medusa				=> 'cse_alife_item_artefact',
	af_mincer_meat				=> 'cse_alife_item_artefact',
	af_night_star				=> 'cse_alife_item_artefact',
	'af_rusty_sea-urchin'			=> 'cse_alife_item_artefact',
	af_vyvert				=> 'cse_alife_item_artefact',

	bread					=> 'cse_alife_item',
	bread_a					=> 'cse_alife_item',
	kolbasa					=> 'cse_alife_item',
	vodka					=> 'cse_alife_item',
	vodka_a					=> 'cse_alife_item',
	medkit					=> 'cse_alife_item',
	conserva				=> 'cse_alife_item',
	energy_drink				=> 'cse_alife_item',
	antirad					=> 'cse_alife_item',
	bandage					=> 'cse_alife_item',
	medkit					=> 'cse_alife_item',
	medkit_army				=> 'cse_alife_item',
	medkit_scientic				=> 'cse_alife_item',
	guitar_a				=> 'cse_alife_item',
	hand_radio				=> 'cse_alife_item',

	af_blood_tutorial			=> 'cse_alife_item_artefact',
	quest_case_01				=> 'cse_alife_item',
	quest_case_02				=> 'cse_alife_item',
	pri_decoder_documents			=> 'cse_alife_item',
	dar_document4				=> 'cse_alife_item',
	gunslinger_flash			=> 'cse_alife_item',
	decoder					=> 'cse_alife_item',

	novice_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_novice_m1			=> 'cse_alife_item_custom_outfit',
	bandit_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_bandit_m1			=> 'cse_alife_item_custom_outfit',
	dolg_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_dolg_m1				=> 'cse_alife_item_custom_outfit',
	dolg_scientific_outfit			=> 'cse_alife_item_custom_outfit',
	exo_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_exo_m1				=> 'cse_alife_item_custom_outfit',
	killer_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_killer_m1			=> 'cse_alife_item_custom_outfit',
	monolit_outfit				=> 'cse_alife_item_custom_outfit',
	specops_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_specnaz_m1			=> 'cse_alife_item_custom_outfit',
	military_outfit				=> 'cse_alife_item_custom_outfit',
	stalker_outfit				=> 'cse_alife_item_custom_outfit',
	outfit_stalker_m1			=> 'cse_alife_item_custom_outfit',
	outfit_stalker_m2			=> 'cse_alife_item_custom_outfit',
	svoboda_heavy_outfit			=> 'cse_alife_item_custom_outfit',
	svoboda_light_outfit			=> 'cse_alife_item_custom_outfit',
	outfit_svoboda_m1			=> 'cse_alife_item_custom_outfit',
	protection_outfit			=> 'cse_alife_item_custom_outfit',
	ecolog_outfit				=> 'cse_alife_item_custom_outfit',

	wpn_addon_scope				=> 'cse_alife_item',
	wpn_addon_scope_susat			=> 'cse_alife_item',
	wpn_addon_silencer			=> 'cse_alife_item',
	wpn_addon_grenade_launcher		=> 'cse_alife_item',
	wpn_addon_grenade_launcher_m203		=> 'cse_alife_item',

	grenade_f1				=> 'cse_alife_item_grenade',
	grenade_rgd5				=> 'cse_alife_item_grenade',

	'ammo_11.43x23_fmj'			=> 'cse_alife_item_ammo',
	'ammo_11.43x23_hydro'			=> 'cse_alife_item_ammo',
	ammo_12x70_buck				=> 'cse_alife_item_ammo',
	ammo_12x76_dart				=> 'cse_alife_item_ammo',
	ammo_12x76_zhekan			=> 'cse_alife_item_ammo',
	ammo_16x70_buck				=> 'cse_alife_item_ammo',
	ammo_16x70_kart				=> 'cse_alife_item_ammo',
	ammo_16x70_zhekan			=> 'cse_alife_item_ammo',
	'ammo_5.45x39_ap'			=> 'cse_alife_item_ammo',
	'ammo_5.45x39_fmj'			=> 'cse_alife_item_ammo',
	'ammo_5.56x45_ap'			=> 'cse_alife_item_ammo',
	'ammo_5.56x45_ss190'			=> 'cse_alife_item_ammo',
	'ammo_7.62x54_7h1'			=> 'cse_alife_item_ammo',
	'ammo_7.62x54_7h14'			=> 'cse_alife_item_ammo',
	'ammo_7.62x54_ap'			=> 'cse_alife_item_ammo',
	'ammo_7.62x39_fmj'			=> 'cse_alife_item_ammo',
	'ammo_7.62x39_ap'			=> 'cse_alife_item_ammo',
	ammo_9x18_fmj				=> 'cse_alife_item_ammo',
	ammo_9x18_pmm				=> 'cse_alife_item_ammo',
	ammo_9x19_fmj				=> 'cse_alife_item_ammo',
	ammo_9x19_pbp				=> 'cse_alife_item_ammo',
	ammo_9x39_pab9				=> 'cse_alife_item_ammo',
	ammo_9x39_sp5				=> 'cse_alife_item_ammo',
	ammo_9x39_ap				=> 'cse_alife_item_ammo',
	'ammo_vog-25'				=> 'cse_alife_item_ammo',
	'ammo_vog-25p'				=> 'cse_alife_item_ammo',
	ammo_m209				=> 'cse_alife_item_ammo',
	'ammo_og-7b'				=> 'cse_alife_item_ammo',
	ammo_gauss				=> 'cse_alife_item_ammo',

	explosive_barrel			=> 'cse_alife_item_explosive',
	explosive_barrel_low			=> 'cse_alife_item_explosive',
	explosive_dinamit			=> 'cse_alife_item_explosive',
	explosive_fuelcan			=> 'cse_alife_item_explosive',
	explosive_mobiltank			=> 'cse_alife_item_explosive',
	explosive_tank				=> 'cse_alife_item_explosive',

	mounted_weapon				=> 'cse_alife_mounted_weapon',
	stationary_mgun				=> 'cse_alife_mounted_weapon',

	wpn_binoc				=> 'cse_alife_item_weapon_magazined',
	wpn_rpg7				=> 'cse_alife_item_weapon_magazined',
	wpn_pm					=> 'cse_alife_item_weapon_magazined',
	wpn_pb					=> 'cse_alife_item_weapon_magazined',
	wpn_fort				=> 'cse_alife_item_weapon_magazined',
	wpn_fort_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_walther				=> 'cse_alife_item_weapon_magazined',
	wpn_walther_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_beretta				=> 'cse_alife_item_weapon_magazined',
	wpn_hpsa				=> 'cse_alife_item_weapon_magazined',
	wpn_sig220				=> 'cse_alife_item_weapon_magazined',
	wpn_usp					=> 'cse_alife_item_weapon_magazined',
	wpn_desert_eagle			=> 'cse_alife_item_weapon_magazined',
	wpn_eagle_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_colt1911				=> 'cse_alife_item_weapon_magazined',
	wpn_colt_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_mp5					=> 'cse_alife_item_weapon_magazined',
	wpn_mp5_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_mp5_m2				=> 'cse_alife_item_weapon_magazined',
	wpn_val					=> 'cse_alife_item_weapon_magazined',
	wpn_val_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_vintorez				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_svd					=> 'cse_alife_item_weapon_magazined',
	wpn_svd_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_svu					=> 'cse_alife_item_weapon_magazined',
	wpn_ak74u				=> 'cse_alife_item_weapon_magazined_w_gl', #clear sky modified
	wpn_ak74u_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_gauss				=> 'cse_alife_item_weapon_magazined',
	wpn_groza				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_groza1				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_groza_m1				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_l85					=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_l85_m1				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_l85_m2				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_lr300				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_lr300_m1				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_sig550				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_sig_m1				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_sig_m2				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_ak74				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_ak74_m1				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_abakan				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_abakan_m1				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_abakan_m2				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_fn2000				=> 'cse_alife_item_weapon_magazined_w_gl',
	wpn_toz34				=> 'cse_alife_item_weapon_shotgun',
	wpn_spas12				=> 'cse_alife_item_weapon_shotgun',
	wpn_spas12_m1				=> 'cse_alife_item_weapon_shotgun',
	wpn_wincheaster1300			=> 'cse_alife_item_weapon_shotgun',
	wpn_winchester_m1			=> 'cse_alife_item_weapon_shotgun',
	wpn_bm16				=> 'cse_alife_item_weapon_shotgun',
	wpn_bm16_full				=> 'cse_alife_item_weapon_shotgun',
	'wpn_rg-6'				=> 'cse_alife_item_weapon_shotgun',
	wpn_rg6_m1				=> 'cse_alife_item_weapon_shotgun',
	hunters_toz				=> 'cse_alife_item_weapon_shotgun',

	m_car					=> 'cse_alife_car',
	helicopter				=> 'cse_alife_helicopter',

	turret_mgun				=> 'se_turret_mgun',
	
	#clear sky new
	smart_cover						=> 'cse_smart_cover',
	wpn_vintorec				=> 'cse_alife_item_weapon_magazined_w_gl',
	zone_mine_gravitational_weak	=> 'se_zone_anom',
	zone_mine_gravitational_average	=> 'se_zone_anom',	
	zone_mine_gravitational_strong	=> 'se_zone_anom',	
	zone_mine_thermal_weak	=> 'se_zone_anom', 		
	zone_mine_thermal_average	=> 'se_zone_anom',
	zone_mine_thermal_strong	=> 'se_zone_anom',	
	zone_mine_thermal_firetube	=> 'se_zone_anom',
	zone_mine_electric_weak	=> 'se_zone_anom',
	zone_mine_electric_average	=> 'se_zone_anom',	
	zone_mine_electric_strong	=> 'se_zone_anom',	
	zone_mine_acidic_weak	=> 'se_zone_anom',
	zone_mine_acidic_average	=> 'se_zone_anom',
	zone_mine_acidic_strong	=> 'se_zone_anom',
	
	mar_csky_trader				=> 'se_stalker',	
	mar_csky_tech				=> 'se_stalker',
	mar_csky_scientist				=> 'se_stalker',
	mar_csky_barman				=> 'se_stalker',
	mar_csky_tutorial_man				=> 'se_stalker',	
	mar_csky_tactic				=> 'se_stalker',	
	
	esc_stalker_guard_east_bridge				=> 'se_stalker',
	esc_stalker_guard_west_bridge				=> 'se_stalker',
	esc_leader_stalkerbase				=> 'se_stalker',
	esc_trader_stalkerbase				=> 'se_stalker',
	esc_tech_stalkerbase				=> 'se_stalker',
	esc_wolf				=> 'se_stalker',
	esc_zak_stalkerbase				=> 'se_stalker',
	esc_driver				=> 'se_stalker',
	esc_military_secret_trader				=> 'se_stalker',
	esc_story_military				=> 'se_stalker',
	esc_zak_stalkerbase_2				=> 'se_stalker',
	esc_military_general				=> 'se_stalker',
	esc_wolf_brother				=> 'se_stalker',
	
	agr_duty_base_trader				=> 'se_stalker',
	agr_stalker_base_trader				=> 'se_stalker',
	agr_secret_trader				=> 'se_stalker',
	agr_holeman				=> 'se_stalker',
	agr_snork_hole_1				=> 'se_monster',
	agr_snork_hole_2				=> 'se_monster',
	agr_snork_hole_3				=> 'se_monster',
	agr_snork_hole_4				=> 'se_monster',
	agr_snork_hole_5				=> 'se_monster',
	agr_snork_hole_6				=> 'se_monster',
	agr_snork_hole_7				=> 'se_monster',
	agr_snork_hole_8				=> 'se_monster',
	agr_snork_hole_9				=> 'se_monster',
	agr_snork_jumper_1				=> 'se_monster',
	agr_snork_jumper_2				=> 'se_monster',
	agr_snork_jumper_3				=> 'se_monster',
	agr_stalker_base_mechanic				=> 'se_stalker',
	agr_stalker_base_leader				=> 'se_stalker',
	agr_stalker_zombied_1_default				=> 'se_stalker',
	agr_bloodsucker_home				=> 'se_monster',
	agr_scientists_bloodsucker				=> 'se_monster',
	agr_weaponmaster				=> 'se_stalker',
	agr_barman				=> 'se_stalker',
	agr_bar_stalker_1				=> 'se_stalker',
	agr_bar_stalker_2				=> 'se_stalker',
	agr_bar_stalker_3				=> 'se_stalker',
	agr_bar_stalker_4				=> 'se_stalker',
	agr_bar_stalker_5				=> 'se_stalker',
	agr_stalker_commander_1				=> 'se_stalker',
	agr_stalker_commander_2				=> 'se_stalker',
	agr_dog_01				=> 'se_monster',
	agr_dog_02				=> 'se_monster',
	agr_dog_03				=> 'se_monster',
	agr_dog_04				=> 'se_monster',
	agr_dog_05				=> 'se_monster',
	agr_dog_06				=> 'se_monster',
	agr_dog_07				=> 'se_monster',
	agr_dog_08				=> 'se_monster',
	agr_dog_09				=> 'se_monster',
	agr_dog_10				=> 'se_monster',
	agr_dolg_blockpost_commander				=> 'se_stalker',
	
	gar_digger_messenger_man				=> 'se_stalker',
	gar_bandit_barman				=> 'se_stalker',
	gar_bandit_fixer				=> 'se_stalker',
	gar_bandit_leader				=> 'se_stalker',
	gar_bandit_trader				=> 'se_stalker',
	gar_bandit_minigame				=> 'se_stalker',
	gar_bandit_senya				=> 'se_stalker',
	gar_bandit_ambush_1				=> 'se_stalker',
	gar_bandit_ambush_2				=> 'se_stalker',
	gar_bandit_robber_1				=> 'se_stalker',
	gar_bandit_robber_2				=> 'se_stalker',
	gar_digger_conc_camp_prisoner_1				=> 'se_stalker',
	gar_digger_conc_camp_prisoner_2				=> 'se_stalker',
	gar_digger_conc_camp_searcher_1				=> 'se_stalker',
	gar_digger_conc_camp_searcher_2				=> 'se_stalker',
	gar_digger_conc_camp_searcher_3				=> 'se_stalker',
	gar_digger_fighter_1				=> 'se_stalker',
	gar_digger_fighter_2				=> 'se_stalker',
	gar_digger_fighter_3				=> 'se_stalker',
	gar_digger_fighter_4				=> 'se_stalker',
	gar_digger_fighter_5				=> 'se_stalker',
	gar_digger_quester				=> 'se_stalker',
	gar_digger_fixer				=> 'se_stalker',
	gar_digger_traitor				=> 'se_stalker',
	gar_digger_smuggler				=> 'se_stalker',
	gar_bandit_digger_traitor				=> 'se_stalker',
	gar_dead_camp_snork				=> 'se_monster',
	
	default_duty				=> 'se_stalker',
	default_freedom				=> 'se_stalker',
	default_bandit				=> 'se_stalker',
	default_stalker				=> 'se_stalker',
	stalker_regular				=> 'se_stalker',
	sim_default_duty_0				=> 'se_stalker',
	sim_default_duty_1				=> 'se_stalker',
	sim_default_duty_2				=> 'se_stalker',
	sim_default_duty_3				=> 'se_stalker',
	sim_default_duty_4				=> 'se_stalker',
	sim_default_monolith_0				=> 'se_stalker',
	sim_default_monolith_1				=> 'se_stalker',
	sim_default_monolith_2				=> 'se_stalker',
	sim_default_monolith_3				=> 'se_stalker',
	sim_default_monolith_4				=> 'se_stalker',
	sim_default_freedom_0				=> 'se_stalker',
	sim_default_freedom_1				=> 'se_stalker',
	sim_default_freedom_2				=> 'se_stalker',
	sim_default_freedom_3				=> 'se_stalker',
	sim_default_freedom_4				=> 'se_stalker',
	sim_default_bandit_0				=> 'se_stalker',
	sim_default_bandit_1				=> 'se_stalker',
	sim_default_bandit_2				=> 'se_stalker',
	sim_default_bandit_3				=> 'se_stalker',
	sim_default_bandit_4				=> 'se_stalker',
	sim_default_renegade_0				=> 'se_stalker',
	sim_default_renegade_1				=> 'se_stalker',
	sim_default_renegade_2				=> 'se_stalker',
	sim_default_csky_0				=> 'se_stalker',
	sim_default_csky_1				=> 'se_stalker',
	sim_default_csky_2				=> 'se_stalker',
	sim_default_csky_3				=> 'se_stalker',
	sim_default_csky_4				=> 'se_stalker',
	sim_default_stalker_0				=> 'se_stalker',
	sim_default_stalker_1				=> 'se_stalker',
	sim_default_stalker_2				=> 'se_stalker',
	sim_default_stalker_3				=> 'se_stalker',
	sim_default_stalker_4				=> 'se_stalker',
	sim_default_military_0				=> 'se_stalker',
	sim_default_military_1				=> 'se_stalker',
	sim_default_military_2				=> 'se_stalker',
	sim_default_military_3				=> 'se_stalker',
	sim_default_military_3_sniper				=> 'se_stalker',
	sim_default_military_4				=> 'se_stalker',
	sim_default_killer_0				=> 'se_stalker',
	sim_default_killer_1				=> 'se_stalker',
	sim_default_killer_2				=> 'se_stalker',
	sim_default_killer_3				=> 'se_stalker',
	sim_default_killer_4				=> 'se_stalker',
	sim_default_digger_0				=> 'se_stalker',
	sim_default_digger_1				=> 'se_stalker',
	sim_default_digger_2				=> 'se_stalker',
	sim_default_digger_3				=> 'se_stalker',
	sim_default_digger_4				=> 'se_stalker',
	arena_first_battle_stalker_1				=> 'se_stalker',
	arena_first_battle_ally_stalker_1				=> 'se_stalker',
	arena_second_battle_stalker_1				=> 'se_stalker',
	arena_second_battle_stalker_1_boss				=> 'se_stalker',
	arena_third_battle_stalker_1				=> 'se_stalker',
	arena_third_battle_stalker_1_boss				=> 'se_stalker',
	arena_fourth_battle_stalker_1				=> 'se_stalker',
	arena_fourth_battle_stalker_2				=> 'se_stalker',
	arena_fourth_battle_stalker_3				=> 'se_stalker',
	arena_fourth_battle_stalker_4				=> 'se_stalker',
	arena_fourth_battle_stalker_5				=> 'se_stalker',
	arena_survival_spawn				=> 'se_stalker',
	arena_survival_spawn1				=> 'se_stalker',
	arena_survival_spawn2				=> 'se_stalker',
	pl_test_sect				=> 'se_stalker',
	pl_test_sect1				=> 'se_stalker',
	
	val_freedom_blockpost_guard_north_1				=> 'se_stalker',
	val_freedom_blockpost_guard_north_2				=> 'se_stalker',
	val_freedom_blockpost_guard_leader_north				=> 'se_stalker',
	val_freedom_blockpost_guard_south_1				=> 'se_stalker',
	val_freedom_blockpost_guard_south_2				=> 'se_stalker',
	val_freedom_blockpost_guard_leader_south				=> 'se_stalker',
	val_freedom_deadblockpost_guard_1				=> 'se_stalker',
	val_freedom_deadblockpost_guard_2				=> 'se_stalker',
	val_freedom_deadblockpost_guard_3				=> 'se_stalker',
	val_freedom_deadblockpost_guard_4				=> 'se_stalker',
	val_freedom_deadblockpost_guard_5				=> 'se_stalker',
	sim_default_freedom_sniper				=> 'se_stalker',
	val_killer_sniper_1				=> 'se_stalker',
	val_killer_sniper_2				=> 'se_stalker',
	val_killer_sniper_3				=> 'se_stalker',
	val_killer_sniper_4				=> 'se_stalker',
	val_killer_sniper_5				=> 'se_stalker',
	val_killer_sniper_6				=> 'se_stalker',
	val_killer_sniper_7				=> 'se_stalker',
	val_killer_sniper_8				=> 'se_stalker',
	val_killer_sniper_9				=> 'se_stalker',
	val_killer_sniper_10				=> 'se_stalker',
	val_killer_1				=> 'se_stalker',
	val_killer_2				=> 'se_stalker',
	val_killer_3				=> 'se_stalker',
	val_killer_4				=> 'se_stalker',
	val_killer_5				=> 'se_stalker',
	val_killer_6				=> 'se_stalker',
	val_killer_7				=> 'se_stalker',
	val_killer_8				=> 'se_stalker',
	val_killer_9				=> 'se_stalker',
	val_killer_10				=> 'se_stalker',
	val_freedom_attack_1				=> 'se_stalker',
	val_freedom_attack_2				=> 'se_stalker',
	val_freedom_attack_3				=> 'se_stalker',
	val_freedom_attack_4				=> 'se_stalker',
	val_freedom_attack_commander				=> 'se_stalker',
	val_freedom_attack_6				=> 'se_stalker',
	val_freedom_attack_7				=> 'se_stalker',
	val_freedom_attack_8				=> 'se_stalker',
	val_freedom_attack_9				=> 'se_stalker',
	val_freedom_attack_10				=> 'se_stalker',
	val_freedom_trader				=> 'se_stalker',
	val_freedom_barmen				=> 'se_stalker',
	val_bandit_spy_1				=> 'se_stalker',
	val_bandit_spy_2				=> 'se_stalker',
	val_bandit_spy_3				=> 'se_stalker',
	val_bandit_spy_4				=> 'se_stalker',
	
	m_lesnik	=> 'cse_alife_trader',
	stalker_die_hard				=> 'se_stalker',
	stalker_strelok				=> 'se_stalker',
	
	device_pda_fang	=> 'cse_alife_item_pda',
	esc_trader_habar	=> 'cse_alife_item_pda',
	device_pda_military	=> 'cse_alife_item_pda',
	device_pda_old	=> 'cse_alife_item_pda',
	device_pda_garbage_traitor	=> 'cse_alife_item_pda',
	device_pda_comendant	=> 'cse_alife_item_pda',
	device_pda_freedom	=> 'cse_alife_item_pda',
	device_pda_bloodsucker	=> 'cse_alife_item_pda',
	device_pda_digger	=> 'cse_alife_item_pda',
	red_forest_pda_map	=> 'cse_alife_item_pda',
	# mar_quest_wpn_pm_1]:wpn_pm
	# mar_quest_wpn_ak74u_1]:wpn_ak74u
	# mar_quest_af_cristall_flower_1]:af_cristall_flower
	mar_quest_scout_pda	=> 'cse_alife_item_pda',
	# mar_quest_novice_outfit_1]:novice_outfit
	# mil_quest_af_fuzz_kolobok]:af_fuzz_kolobok
	mil_device_pda_lost_squard	=> 'cse_alife_item_pda',
	esc_quest_magic_vodka	=> 'cse_alife_item_pda',
	# esc_quest_akm47]:wpn_ak74
	# esc_quest_luky_detector]:detector_simple
	esc_quest_spec_medkit	=> 'cse_alife_item_pda',
	esc_device_pda_driver	=> 'cse_alife_item_pda',
	esc_mechanic_flash_card_1	=> 'cse_alife_item_pda',
	esc_mechanic_flash_card_2	=> 'cse_alife_item_pda',
	esc_mechanic_flash_card_3	=> 'cse_alife_item_pda',
	esc_mechanic_flash_card_4	=> 'cse_alife_item_pda',
	# gar_quest_wpn_wincheaster1300]:wpn_wincheaster1300
	# gar_quest_wpn_desert_eagle]:wpn_desert_eagle
	gar_quest_vodka_2	=> 'cse_alife_item_pda',
	# gar_quest_novice_outfit]:novice_outfit
	# gar_quest_wpn_pm]:wpn_pm
	# val_quest_scope_x8]:wpn_addon_scope
	# val_quest_flash_movies]:dev_flash_2
	# val_quest_guitar_serg]:guitar_a
	# agr_quest_wpn_spas12]:wpn_spas12
	agr_quest_duty_secret_pda	=> 'cse_alife_item_pda',
	agr_quest_duty_case	=> 'cse_alife_item_pda',
	# agr_quest_duty_abakan]:wpn_abakan
	agr_pda_for_secret_trader	=> 'cse_alife_item_pda',
	agr_map_animals	=> 'cse_alife_item_pda',
	agr_mechanic_pda	=> 'cse_alife_item_pda',
	# yan_quest_ammo_sleep]:ammo_7.62x54_ap
	# yan_quest_scarlet_flower]:af_electra_flash
	yan_quest_granade	=> 'cse_alife_item_pda',
	red_forest_pda_map_2	=> 'cse_alife_item_pda',
	# red_quest_tank_minigun]:wpn_pkm
	red_quest_prototipe_device	=> 'cse_alife_item_pda',
	
	anomal_zone	=> 'cse_alife_space_restrictor',
	
	zone_field_thermal_weak	=> 'cse_alife_anomalous_zone',
	zone_field_thermal_average	=> 'cse_alife_anomalous_zone',	
	zone_field_thermal_strong	=> 'cse_alife_anomalous_zone',	
	zone_field_radioactive_weak	=> 'cse_alife_anomalous_zone',
	zone_field_radioactive_average	=> 'cse_alife_anomalous_zone',	
	zone_field_radioactive_strong	=> 'cse_alife_anomalous_zone',	
	zone_field_psychic_weak	=> 'cse_alife_anomalous_zone',
	zone_field_psychic_average	=> 'cse_alife_anomalous_zone',	
	zone_field_psychic_strong	=> 'cse_alife_anomalous_zone',	
	zone_field_acidic_weak	=> 'cse_alife_anomalous_zone',	
	zone_field_acidic_average	=> 'cse_alife_anomalous_zone',	
	zone_field_acidic_strong	=> 'cse_alife_anomalous_zone',	
	zone_mine_field_no_damage	=> 'cse_alife_anomalous_zone',
	zone_mine_field_strong	=> 'cse_alife_anomalous_zone',
	
	balon_01				=> 'cse_alife_object_physic',
	museum_ammo_12x70_buck				=> 'cse_alife_object_physic',
	museum_ammo_545x39_fmj				=> 'cse_alife_object_physic',	
	museum_ammo_762x54_7h14				=> 'cse_alife_object_physic',	
	museum_abakan				=> 'cse_alife_object_physic',
	museum_ak74				=> 'cse_alife_object_physic',
	museum_ak74u				=> 'cse_alife_object_physic',
	museum_groza				=> 'cse_alife_object_physic',
	museum_lr300				=> 'cse_alife_object_physic',
	museum_rg6				=> 'cse_alife_object_physic',
	museum_rpg7				=> 'cse_alife_object_physic',
	museum_sig550				=> 'cse_alife_object_physic',
	museum_spas12				=> 'cse_alife_object_physic',
	museum_svd				=> 'cse_alife_object_physic',
	museum_bm16				=> 'cse_alife_object_physic',
	museum_toz34				=> 'cse_alife_object_physic',
	museum_val				=> 'cse_alife_object_physic',
	museum_vintorez				=> 'cse_alife_object_physic',
	museum_winchester1300				=> 'cse_alife_object_physic',
	balon_01				=> 'cse_alife_object_physic',
	balon_02				=> 'cse_alife_object_physic',
	balon_02a				=> 'cse_alife_object_physic',
	bidon				=> 'cse_alife_object_physic',
	bochka_close_1				=> 'cse_alife_object_physic',
	bochka_close_2				=> 'cse_alife_object_physic',
	bochka_close_3				=> 'cse_alife_object_physic',
	bochka_close_4				=> 'cse_alife_object_physic',
	banka_kraski_1				=> 'cse_alife_object_physic',
	kanistra_01				=> 'cse_alife_object_physic',
	tiski				=> 'cse_alife_object_physic',
	kanistra_02				=> 'cse_alife_object_physic',
	vedro_01				=> 'cse_alife_object_physic',
	notebook				=> 'cse_alife_object_physic',
	priemnik_gorizont				=> 'cse_alife_object_physic',
	rupor				=> 'cse_alife_object_physic',
	transiver				=> 'cse_alife_object_physic',
	tv_1				=> 'cse_alife_object_physic',
	table_lamp_01				=> 'cse_alife_object_physic',
	komp_monitor				=> 'cse_alife_object_physic',
	komp_klava				=> 'cse_alife_object_physic',
	komp_block				=> 'cse_alife_object_physic',
	bottle_3l				=> 'cse_alife_object_physic', 
	freezer				=> 'cse_alife_object_physic',
	bludo				=> 'cse_alife_object_physic',
	kastrula				=> 'cse_alife_object_physic',
	kastrula_up				=> 'cse_alife_object_physic',
	krujka				=> 'cse_alife_object_physic',
	lojka				=> 'cse_alife_object_physic',
	miska				=> 'cse_alife_object_physic',
	tarelka1				=> 'cse_alife_object_physic',
	tarelka2				=> 'cse_alife_object_physic',
	teapot_1				=> 'cse_alife_object_physic',
	kolyaska_01				=> 'cse_alife_object_physic',
	kolyaska_01_braked				=> 'cse_alife_object_physic',
	kolyaska_wheel_01_braked				=> 'cse_alife_object_physic',
	wheel_litter_01				=> 'cse_alife_object_physic',
	wheel_litter_01_braked				=> 'cse_alife_object_physic',
	med_stolik_01				=> 'cse_alife_object_physic',
	shooting_target_1				=> 'cse_alife_object_physic',
	
	agru_door				=> 'cse_alife_object_physic',
	red_forest_bridge				=> 'cse_alife_object_physic',
	
	detector_simple			=> 'cse_alife_item_detector',
	detector_elite			=> 'cse_alife_item_detector',
	
	flesh_up_ac_mp5	=> 'cse_alife_item_pda',
	flesh_up_bd_mp5	=> 'cse_alife_item_pda',
	flesh_up_a_novice_outfit	=> 'cse_alife_item_pda',
	flesh_up_ac_wincheaster1300	=> 'cse_alife_item_pda',
	flesh_up_bd_wincheaster1300	=> 'cse_alife_item_pda',
	flesh_up_ac_ak74u	=> 'cse_alife_item_pda',
	flesh_up_fh_scientific_outfit	=> 'cse_alife_item_pda',
	flesh_up_aceg_scientific_outfit	=> 'cse_alife_item_pda',
	flesh_up_bdfh_scientific_outfit	=> 'cse_alife_item_pda',
	flesh_up_ab_svu	=> 'cse_alife_item_pda',
	flesh_up_cd_svu	=> 'cse_alife_item_pda',
	flesh_up_abcd_svu	=> 'cse_alife_item_pda',
	flesh_up_ab_pkm	=> 'cse_alife_item_pda',
	flesh_up_cd_pkm	=> 'cse_alife_item_pda',
	flesh_up_abcd_pkm	=> 'cse_alife_item_pda',
	flesh_up_ac_desert_eagle	=> 'cse_alife_item_pda',
	flesh_up_bd_desert_eagle	=> 'cse_alife_item_pda',
	flesh_up_ac_spas12	=> 'cse_alife_item_pda',
	
	lights_signal_light	=> 'cse_alife_object_hanging_lamp',
	
	explosive_hide 	=> 'cse_alife_item_explosive',
	explosive_grenade 	=> 'cse_alife_item_explosive',

	af_compass				=> 'cse_alife_item_artefact',
};
sub new {
	my $class = shift;
	my $self = {};
	$self->{cse_object} = {};
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
			$size == 2 or die;
			my $id = unpack('v', $cf->r_chunk_data());
		} elsif ($index == 1) {
			while (1) {
				($index, $size) = $cf->r_chunk_open();
				defined($index) or last;
				my $data = $cf->r_chunk_data();
				my $size16 = unpack('v', $data);
				$size16 == ($size - 2) or die "alife object size mismatch\n";
				my $packet = stkutils::data_packet->new(substr($data, 2));
				if ($index == 0) {
					cse_abstract::state_read($self->{cse_object}, $packet);
					#print "read state $self->{cse_object}->{name} $self->{cse_object}->{section_name} ";
					my $class_name = section_to_class->{$self->{cse_object}->{section_name}};
					defined $class_name or die "unknown $self->{cse_object}->{section_name}";
					bless $self->{cse_object}, $class_name;
					$self->{cse_object}->state_read($packet);
					$packet->length() == 0 or die "data [".$packet->length()."] left in $self->{cse_object}->{name}";
					print "section_name==name:$self->{cse_object}->{name}\n" 
					if $self->{cse_object}->{name} eq $self->{cse_object}->{section_name};
					#print "end\r\n";					
				} elsif ($index == 1) {
					cse_abstract::update_read($self->{cse_object}, $packet);
					UNIVERSAL::can($self->{cse_object}, 'update_read') && do {
						$self->{cse_object}->update_read($packet);
					};
					if ($packet->length())
					{
						print "($packet->length())";print $packet->length();
					}					
					$packet->length() == 0 or die "update data left in $self->{cse_object}->{name}";
				}
				$cf->r_chunk_close();
			}
		}
		$cf->r_chunk_close();
	}
}
sub spawn_write {
	my $self = shift;
	my ($cf, $object_id) = @_;

	$cf->w_chunk(0, pack('v', $object_id));
	$cf->w_chunk_open(1);

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
}
sub state_import {
	my $self = shift;
	my ($if, $section) = @_;

	cse_abstract::state_import($self->{cse_object}, $if, $section);
	my $class_name = section_to_class->{$self->{cse_object}->{section_name}};
	defined $class_name or die "unknown section '$self->{cse_object}->{section_name}'";
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
	if (defined $self->{update_raw_data}) {
		print $fh "; don't touch this\n";
		print $fh 'update_raw_data = ', unpack('H*', $self->{update_raw_data}), "\n";
	}
	print $fh "\n";
}

#######################################################################
package way_object;
use strict;
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
					$size == 4 or die;
					$self->{point_count} = unpack('V', $cf->r_chunk_data());
				} elsif ($index == 1) {
					$self->read_points($cf);
				} elsif ($index == 2) {
					$self->read_links($cf);
				}
				$cf->r_chunk_close();
			}
		} else {
			die;
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
				$size == 4 or die;
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
	die "no points in path $section" unless defined $points;
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
package all_spawn;
use strict;
use IO::File;
#use Scalar::Util 'blessed';
use stkutils::ini_file;
use stkutils::chunked_file;
use stkutils::level 'name_by_gvid';
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	my $self = shift;
	my ($fn) = @_;

	my $cf = stkutils::chunked_file->new($fn, 'r') or die;
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		if ($index == 0) {
			$size == 0x2c or die;
			$self->read_header($cf);
		} elsif ($index == 1) {
			$self->read_alife($cf);
		} elsif ($index == 2) {
			$self->read_section2($cf);
		} elsif ($index == 3) {
			$self->read_way($cf);
		} elsif ($index == 4) {
			$self->read_graph($cf);
		} else {
			die;
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub write {
	my $self = shift;
	my ($fn) = @_;

	my $cf = stkutils::chunked_file->new($fn, 'w') or die;
	$self->write_header($cf);
	$self->write_alife($cf);
	$self->write_section2($cf);
	$self->write_way($cf);
	$self->write_graph($cf);
	$cf->close();
}
sub read_header {
	my $self = shift;
	my ($cf) = @_;
	(
		$self->{version},
		$self->{guid},
		$self->{graph_guid},
		$self->{count},
		$self->{level_count},
	) = unpack('Va[16]a[16]VV', $cf->r_chunk_data());
}
sub write_header {
	my $self = shift;
	my ($cf) = @_;

	my $data = pack('Va[16]a[16]VV',
		$self->{version},
		$self->{guid},
		$self->{graph_guid},
		$#{$self->{alife_objects}} + 1,
		$self->{level_count});
	$cf->w_chunk(0, $data);
}
sub read_alife {
	my $self = shift;
	my ($cf) = @_;
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		if ($index == 0) {
			$size == 4 or die;
			my $alife_count = unpack('V', $cf->r_chunk_data());
			$alife_count == $self->{count} or die 'alife object count mismatch';
		} elsif ($index == 1) {
			while (1) {
				($index, $size) = $cf->r_chunk_open();
				defined($index) or last;
				my $object = alife_object->new();
				$object->spawn_read($cf);
				push @{$self->{alife_objects}}, $object;
				$cf->r_chunk_close();
			}
		} elsif ($index == 2) {
			$size == 0 or die;
		}
		$cf->r_chunk_close();
	}
}
sub write_alife {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open(1);
	$cf->w_chunk(0, pack('V', $#{$self->{alife_objects}} + 1));
	$cf->w_chunk_open(1);
	my $id = 0;
	foreach my $object (@{$self->{alife_objects}}) {
if ($object->{cse_object}->{section_name} eq "inventory_box") {
	$object->{cse_object}->{object_flags} = 0xffffff3b;
}
my $class_name = ref $object->{cse_object};
if (0 and ($class_name eq 'se_stalker' or $class_name eq 'se_monster') and $object->{cse_object}->{story_id} == -1) {
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
}
sub read_section2 {
	my $self = shift;
	my ($cf) = @_;
	$self->{section2_raw_data} = $cf->r_chunk_data();

#	my $packet = stkutils::data_packet->new($self->{section2_raw_data});
#	my ($obj_count) = $packet->unpack('V');
#	print "obj_count = $obj_count\n";
#	my @v = $packet->unpack("(fffVf)$obj_count")
#	for (my $i = 0; $i < $obj_countl; $i++) {
#		my $o = $i*5;
#		printf "(%f, %f, %f), %d, %f\n", @v[$o .. $o + 4];
#	}
}
sub write_section2 {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk(2, $self->{section2_raw_data});
}
sub read_way {
	my $self = shift;
	my ($cf) = @_;
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		if ($index == 0) {
			$size == 4 or die;
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
			die;
		}
		$cf->r_chunk_close();
	}
}
sub write_way {
	my $self = shift;
	my ($cf) = @_;

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

sub read_graph {
	my $self = shift;
	my ($cf) = @_;
	$self->{graph_raw_data} = $cf->r_chunk_data();
}
sub write_graph {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk(4, $self->{graph_raw_data});
}


sub import {
	my $self = shift;
	my ($fn) = @_;

	my $if = stkutils::ini_file->new($fn, 'r') or die;
	$self->import_header($if);
	$self->import_alife($if);
	$self->import_section2($if);
	$self->import_way($if);
	$self->import_graph($if);
	$if->close();
}
sub import_header {
	my $self = shift;
	my ($if) = @_;

	$self->{version} = $if->value('header', 'version');
	$self->{guid} = pack 'H*', $if->value('header', 'guid');
	$self->{graph_guid} = pack 'H*', $if->value('header', 'graph_guid');
	$self->{level_count} = $if->value('header', 'level_count');
}
sub import_alife {
	my $self = shift;
	my ($if) = @_;

	foreach my $fn (split /,/, ($if->value('alife', 'source_files') or die)) {
		$fn =~ s/^\s*|\s*$//g;
		my $lif = stkutils::ini_file->new($fn, 'r') or die;
		foreach my $section (@{$lif->{sections_list}}) {
			my $object = alife_object->new();
			$object->state_import($lif, $section);
			push @{$self->{alife_objects}}, $object;
		}
		$lif->close();
	}
}
sub import_section2 {
	my $self = shift;
	my ($if) = @_;

	my $fn = $if->value('section2', 'binary_files') or die;
	my $bin_fh = IO::File->new($fn, 'r') or die "cannot open $fn\n";
	binmode $bin_fh;
	$bin_fh->read($self->{section2_raw_data}, ($bin_fh->stat())[7]);
}
sub import_way {
	my $self = shift;
	my ($if) = @_;

	foreach my $fn (split /,/, ($if->value('way', 'source_files') or die)) {
		$fn =~ s/^\s*|\s*$//g;
		my $lif = stkutils::ini_file->new($fn, 'r') or die;
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

	my $fn = $if->value('graph', 'binary_files') or die;
	my $bin_fh = IO::File->new($fn, 'r') or die "cannot open $fn\n";
	binmode $bin_fh;
	$bin_fh->read($self->{graph_raw_data}, ($bin_fh->stat())[7]);
}


sub export {
	my $self = shift;
	my ($fn) = @_;

	my $if = stkutils::ini_file->new($fn, 'w') or die;
	$self->export_header($if);
	$self->export_alife($if);
	$self->export_section2($if);
	$self->export_way($if);
	$self->export_graph($if);
	$if->close();
}
sub export_header {
	my $self = shift;
	my ($if) = @_;

	my $fh = $if->{fh};
	print $fh "[header]\n; don't touch these\n";
	print $fh "version = $self->{version}\n";
	print $fh 'guid = ', unpack('H*', $self->{guid}), "\n";
	print $fh 'graph_guid = ', unpack('H*', $self->{graph_guid}), "\n";
	print $fh "level_count = $self->{level_count}\n\n";
}
sub export_alife {
	my $self = shift;
	my ($if) = @_;

	my $id = 0;
	my %if_by_level;
	my @levels;
	foreach my $object (@{$self->{alife_objects}}) {
		my $cse_object = $object->{cse_object};
		my $level = (name_by_gvid($cse_object->{game_vertex_id}) or 'lxx_unknown');
		my $lif = $if_by_level{$level};
		if (!defined $lif) {
			$lif = stkutils::ini_file->new("alife_$level.ltx", 'w') or die;
			$if_by_level{$level} = $lif;
			push @levels, "alife_$level.ltx";
		}
		$object->state_export($lif, $id++);
	}
	my $fh = $if->{fh};
	print $fh "[alife]\nsource_files = <<END\n", join(",\n", sort @levels), "\nEND\n\n";
	foreach $if (values %if_by_level) {
		$if->close();
	}
}
sub export_section2 {
	my $self = shift;
	my ($if) = @_;

	my $fn = 'section2.bin';
	my $bin_fh = IO::File->new($fn, 'w') or die "cannot open $fn\n";
	binmode $bin_fh;
	$bin_fh->write($self->{section2_raw_data}, length($self->{section2_raw_data}));

	my $fh = $if->{fh};
	print $fh "[section2]\nbinary_files = $fn\n\n";
}
# FIXME: it is better to scan level.game files
use constant way_name_exceptions => {
	bloodsucker_01_way_1					=>	'l01_marsh',
	bloodsucker_01_way_2					=>	'l01_marsh',
	bloodsucker_02_way_1					=>	'l01_marsh',
	bloodsucker_02_way_2					=>	'l01_marsh',
	bloodsucker_03_way_1					=>	'l01_marsh',
	bloodsucker_03_way_2					=>	'l01_marsh',
	mar_art_2								=>	'l01_marsh',
	mar_art_3								=>	'l01_marsh',
	mar_monster_01_way_2					=>	'l01_marsh',
	mar_monster_02_way_1					=>	'l01_marsh',
	# mar_monster_03_way_2
	# mar_monster_04_way_2
	# mar_monster_05_way_2
	# mar_monster_06_way_1
	# mar_monster_07_way_2
	# mar_monster_12_way_1
	# mar_monster_12_way_2
	# mar_monster_13_way_1
	# mar_monster_14_way_2
	# mar_monster_15_way_1
	# mar_monster_15_way_2
	# mar_monster_16_way_1
	# mar_monster_16_way_2
	# mar_monster_17_way_2
	# mar_monster_18_way_1
	# mar_monster_18_way_2
	# mar_monster_19_way_1
	# mar_monster_19_way_2
	# mar_monster_20_way_1
	# mar_monster_20_way_2
	
	esc_smart_terrain_2_12_walker_3_look	=> 'l03_escape',
	esc_smart_terrain_5_7_teleport_look		=> 'l03_escape',
	
	agr_smart_terrain_1_2_kamp_1			=>	'l06_agroprom',
	agr_smart_terrain_1_3_kamp_1			=>	'l06_agroprom',	
	agr_smart_terrain_1_2_comander_look		=>	'l06_agroprom',
	agr_smart_terrain_1_3_walker_1_look		=>	'l06_agroprom',
	agr_smart_terrain_1_6_kamp_1			=>	'l06_agroprom',
	agr_smart_terrain_1_6_kamp_2			=>	'l06_agroprom',
	agr_smart_terrain_1_6_kamp_4			=>	'l06_agroprom',	
	agr_smart_terrain_1_6_barman_1_look		=>	'l06_agroprom',
	agr_smart_terrain_1_6_near_1_kamp_1		=>	'l06_agroprom',
	agr_smart_terrain_1_6_near_1_walker_1_look	=>	'l06_agroprom',
	agr_smart_terrain_1_6_near_2_kamp_1		=>	'l06_agroprom',
	agr_smart_terrain_1_6_walker_2_look		=>	'l06_agroprom',		
	agr_smart_terrain_2_2_kamp_1			=>	'l06_agroprom',	
	agr_smart_terrain_3_5_look_snork_5		=>	'l06_agroprom',
	agr_smart_terrain_4_2_kamp_1			=>	'l06_agroprom',	
	agr_smart_terrain_4_4_near_2_kamp_1		=>	'l06_agroprom',	
	agr_smart_terrain_4_4_near_1_kamp_1		=>	'l06_agroprom',	
	agr_smart_terrain_4_4_near_3_kamp_1		=>	'l06_agroprom',	
	agr_smart_terrain_5_2_kamp_1			=>	'l06_agroprom',		
	agr_smart_terrain_4_6_guard_2_walk		=>	'l06_agroprom',		
};
sub export_way {
	my $self = shift;
	my ($if) = @_;

	my %info_by_level;

	my $default_level = 'l05_bar';
	foreach my $object (@{$self->{way_objects}}) {
		my $level = name_by_gvid($object->{points}[0]->{game_vertex_id});
		if (defined $level) {
			$default_level = $level;
		} else {
			# either get the name from the exceptions table
			# or use the current default (i.e. previous) value
			$level = (way_name_exceptions->{$object->{name}} or $default_level);
		}
		die "unknown level of the way object $object->{name}\n" unless defined $level;
		my $info = $info_by_level{$level};
		if (!defined $info) {
			$info = {};
			$info->{lif} = stkutils::ini_file->new("way_$level.ltx", 'w') or die;
			$info->{way_objects} = ();
			$info_by_level{$level} = $info;
		}
		push @{$info->{way_objects}}, \$object;
	}

	my $id = 0;
	foreach my $info (values %info_by_level) {
		foreach my $object (sort {$$a->{name} cmp $$b->{name}}  @{$info->{way_objects}}) {
			$$object->state_export($info->{lif}, $id++);
		}
		$info->{lif}->close();
	}
	my $fh = $if->{fh};
	print $fh "[way]\nsource_files = <<END\n", join(",\n", map {"way_$_.ltx"} sort(keys %info_by_level)), "\nEND\n\n";
}


sub export_graph {
	my $self = shift;
	my ($if) = @_;

	my $fn = 'graph.bin';
	my $bin_fh = IO::File->new($fn, 'w') or die "cannot open $fn\n";
	binmode $bin_fh;
	$bin_fh->write($self->{graph_raw_data}, length($self->{graph_raw_data}));

	my $fh = $if->{fh};
	print $fh "[graph]\nbinary_files = $fn\n\n";
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
		die "cannot find actor or level_changer\n";
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

#test0();
#exit;
#local $SIG{__WARN__} = sub {die};

sub usage {
	return <<END
S.T.A.L.K.E.R. all.spawn compiler/decompiler
Usage: acdc -d all.spawn [-o outdir]
       acdc -c all.ltx [-f flag1[,...]] [-a level:point] [-o outfile]
END
}

my $spawn_file;
my $src_file;
my $out;
my $flags;
my %flags_hash;
my $actor_pos;

GetOptions(
	'd=s' => \$spawn_file,
	'c=s' => \$src_file,
	'o=s' => \$out,
	'f=s' => \$flags,
	'a=s' => \$actor_pos,
) or die usage();

if (defined $flags) {
	foreach my $flag (split /,/, $flags) {
		die "bad flag '$flag'\n" unless $flag =~ /\s*(\w+)\s*/;
		$flags_hash{$1} = 1;
	}
}

if (defined $spawn_file) {
	die "bad params\n" if (defined $src_file or defined $actor_pos);
	my $spawn = all_spawn->new();
	$spawn->read($spawn_file);
	defined $out && do {
		File::Path::mkpath($out, 0);
		chdir $out or die "cannot change path to $out\n";
	};
	$spawn->export('all.ltx');
} elsif (defined $src_file) {
	die "bad params\n" if defined $spawn_file;
	my $spawn = all_spawn->new();
	$out = 'all.spawn.new' unless defined $out;
	$spawn->import($src_file);
	if (defined $actor_pos) {
		$spawn->move_actor($actor_pos);
	}
	$spawn->write($out);
} else {
	die usage();
}

sub is_flag_defined {
	return (defined $flags_hash{$_[0]}) ? 1 : 0;
}

#######################################################################
sub test0 {
	my $s = all_spawn->new();
	$s->read('all.spawn.orig');
	$s->write('all.test');
}

sub test1 {
	my $s1 = all_spawn->new();
	$s1->read('all.spawn.orig');
	$s1->export('all.ltx');

#	my $s2 = all_spawn->new();
#	$s2->import('all.ltx');
#	$s2->write('all.spawn');
}

sub test2 {
	my $s1 = all_spawn->new();
	$s1->read('all.spawn.orig');
	$s1->write('all.spawn');
}
