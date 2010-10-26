#!perl -w
#
# tab size:	8
#

package level;
use strict;
# keep it sorted and in sync with game.graph (ggtool.pl -l)
use constant levels_info => (
	{ gvid0 => 2792 },
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

#######################################################################
package data_packet;
use strict;
sub new {
	my $class = shift;
	my $self = {};
	($self->{data}) = @_;
	bless($self, $class);
	return $self;
}
sub unpack {
	my $self = shift;
	my $template = shift;
die if !(defined $self);
die if !(defined $self->{data});
die if !(defined $template);
die if length($self->{data}) == 0;
	my @values = unpack($template.'a*', $self->{data});
die if $#values == -1;
	$self->{data} = splice(@values, -1);
die if !(defined $self->{data});
	return @values;
}
sub pack {
	my $self = shift;
	my $template = shift;
die if !(defined($template));
die if !(defined(@_));
die unless defined $_[0];
	$self->{data} .= pack($template, @_);
}
use constant template_for_scalar => {
	h32	=> 'V',
	h16	=> 'v',
	h8	=> 'C',
	u32	=> 'V',
	u16	=> 'v',
	u8	=> 'C',
	q8	=> 'C',
	s32	=> 'l',
	s16	=> 'v',
	s8	=> 'C',
	sz	=> 'Z*',
	f32	=> 'f',
};
use constant template_for_vector => {
	l32u8v	=> 'V/C',
	l32u16v	=> 'V/v',
	u8v8	=> 'C8',
	f32v3	=> 'f3',
};
sub unpack_properties {
	my $self = shift;
	my $container = shift;

	foreach my $p (@_) {
		if ($p->{type} eq 'shape') {
			my ($count) = $self->unpack('C');
			while ($count--) {
				my %shape;
				($shape{type}) = $self->unpack('C');
				if ($shape{type} == 0) {
					@{$shape{sphere}} = $self->unpack('f4');
				} elsif ($shape{type} == 1) {
					@{$shape{box}} = $self->unpack('f12');
				} else {
					die;
				}
				push @{$container->{$p->{name}}}, \%shape;
			}
		} else {
			my $template = template_for_scalar->{$p->{type}};
			if (defined $template) {
				($container->{$p->{name}}) = $self->unpack($template);
				if ($p->{type} eq 'sz') {
					chomp $container->{$p->{name}};
					$container->{$p->{name}} =~ s/\r//g;
				}
			} else {
				@{$container->{$p->{name}}} = $self->unpack(template_for_vector->{$p->{type}});
			}
		}
	}
}
sub pack_properties {
	my $self = shift;
	my $container = shift;

	foreach my $p (@_) {
		my $template = template_for_scalar->{$p->{type}};
		if (defined $template) {
			$self->pack($template, $container->{$p->{name}});
		} elsif ($p->{type} eq 'shape') {
			$self->pack('C', $#{$container->{$p->{name}}} + 1);
			foreach my $shape (@{$container->{$p->{name}}}) {
				$self->pack('C', $$shape{type});
				if ($$shape{type} == 0) {
					$self->pack('f4', @{$$shape{sphere}});
				} elsif ($$shape{type} == 1) {
					$self->pack('f12', @{$$shape{box}});
				}
			}
		} else {
			my $n = $#{$container->{$p->{name}}} + 1;
			if ($p->{type} eq 'l32u16v') {
				$self->pack("Vv$n", $n, @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'l32u8v') {
				$self->pack("VC$n", $n, @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'u8v8') {
				$self->pack("C$n", @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'f32v3') {
				$self->pack('f3', @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'q8v') {
				$self->pack("C$n", @{$container->{$p->{name}}});
			} else {
				die;
			}
		}
	}
}
sub length {
	my $self = shift;
	return length($self->{data});
}
sub data {
	my $self = shift;
	return $self->{data};
}

#######################################################################
package ini_file;
use strict;
use IO::File;
use Carp;
sub new {
	my $class = shift;
	my ($fn, $mode) = @_;

	my $fh = IO::File->new($fn, $mode) or croak "$fn: $!\n!";

	my $self = {};
	$self->{fh} = $fh;
	$self->{sections_list} = [];
	$self->{sections_hash} = ();
	bless($self, $class);

	$mode eq 'w' and return $self;

	my $section;
	my $skip_section = 0;
	while (<$fh>) {
		$_ =~ qr/^\s*;/ and next;
		if (/^\[(.*)\]\s*:*\s*(\w.*)?/) {
			if (defined $2 && !::is_flag_defined($2)) {
				$skip_section = 1;
				next;
			}
			$section = $1;
			die if defined $self->{sections_hash}->{$section};
			push @{$self->{sections_list}}, $section;
			my %tmp = ();
			$self->{sections_hash}{$section} = \%tmp;
			$skip_section = 0;
			next;
		}
		if (/^([^=]*?)\s*=\s*(.*?)\s*$/) {
			my ($name, $value) = ($1, $2);
			if ($value =~ /^<<(.+)$/) {
				my $terminator = $1;
				$value = '';
				while (<$fh>) {
					chomp;
					last if $_ eq $terminator;
					$value .= "\n".$_;
				}
				die unless defined $_;
				substr ($value, 0, 1) = '';
			}
			$skip_section == 1 and next;
			die unless defined $section;
			$self->{sections_hash}{$section}{$name} = $value;
		}
	}
	return $self;
}
use constant format_for_number => {
	h32	=> '%#x',
	h16	=> '%#x',
	h8	=> '%#x',
	u32	=> '%u',
	u16	=> '%u',
	u8	=> '%u',
	q8	=> '%u',
	s32	=> '%d',
	s16	=> '%d',
	s8	=> '%d',
	f32	=> '%.8g',
};
sub export_properties {
	my $self = shift;
	my $comment = shift;
	my $container = shift;

	my $fh = $self->{fh};

	print $fh "; $comment properties\n" if defined $comment;
	foreach my $p (@_) {
		my $format = format_for_number->{$p->{type}};
		if ($p->{type} eq "f32") {
			print $fh "$p->{name} = $container->{$p->{name}}\n";
		} elsif (defined $format) {
die "$p->{name}\n" unless defined $container->{$p->{name}};
			next if defined($p->{default}) && $container->{$p->{name}} == $p->{default};
			printf $fh "$p->{name} = $format\n", $container->{$p->{name}};
		} elsif ($p->{type} eq "sz") {
			next if defined($p->{default}) && $container->{$p->{name}} eq $p->{default};
			my $value = $container->{$p->{name}};
			if ($value =~ /\n/) {
				print $fh "$p->{name} = <<END\n$value\nEND\n";
			} else {
				print $fh "$p->{name} = $value\n";
			}
		} elsif ($p->{type} eq 'shape') {
			my @shapes;
			my $i = 0;
			foreach my $shape (@{$container->{$p->{name}}}) {
				push @shapes, "shape$i";
				$i++;
			}
			print $fh 'shapes = ', join(',', @shapes), "\n";
			$i = 0;
			foreach my $shape (@{$container->{$p->{name}}}) {
				my $id = "shape$i";
				if ($$shape{type} == 0) {
					print $fh "$id:type = sphere\n";
					print $fh "$id:offset = ", join(',', @{$$shape{sphere}}[0 .. 2]), "\n";
					print $fh "$id:radius = $$shape{sphere}[3]\n";
				} elsif ($$shape{type} == 1) {
					print $fh "$id:type = box\n";
					print $fh "$id:axis_x = ", join(',', @{$$shape{box}}[0 .. 2]), "\n";
					print $fh "$id:axis_y = ", join(',', @{$$shape{box}}[3 .. 5]), "\n";
					print $fh "$id:axis_z = ", join(',', @{$$shape{box}}[6 .. 8]), "\n";
					print $fh "$id:offset = ", join(',', @{$$shape{box}}[9 .. 11]), "\n";
				}
				$i++;
			}
		} else {
			next if defined(@{$p->{default}}) && @{$container->{$p->{name}}} == @{$p->{default}};
			print $fh "$p->{name} = ", join(',', @{$container->{$p->{name}}}), "\n";
		}
	}
	print $fh "\n";
}
sub import_properties {
	my $self = shift;
	my $section = shift;
	my $container = shift;

	die unless defined $self->{sections_hash}{$section};
	foreach my $p (@_) {
		my $value = $self->value($section, $p->{name});
		if ($p->{type} eq 'sz') {
			$container->{$p->{name}} = (defined $value) ? $value : $p->{default};
		} elsif (defined format_for_number->{$p->{type}}) {
			if (defined $value) {
				$value = hex($value) if ($value =~ /^\s*0x/);
				$container->{$p->{name}} = $value;
			} else {
				$container->{$p->{name}} = $p->{default};
			}
		} elsif ($p->{type} eq 'shape') {
			die unless defined $value;
			foreach my $id (split /,/, $value) {
				$id =~ s/^\s*|\s*$//g;
				my $shape = $self->import_shape_properties($section, $id);
				push @{$container->{$p->{name}}}, $shape;
			}
		} else {
			@{$container->{$p->{name}}} = defined $value ?
					split(/,/, $value) : @{$p->{default}};
		}
	}
}
sub import_shape_properties {
	my $self = shift;
	my ($section, $id) = @_;

	my %shape;

	my $type = $self->value($section, "$id:type") or die "no shape type in $section";
	my $offset = $self->value($section, "$id:offset");
	if ($type eq "sphere") {
		my $radius = $self->value($section, "$id:radius") or die "no radius in $section\n";
		$shape{type} = 0;
		@{$shape{sphere}} = (split(/,/, $offset), $radius);
	} elsif ($type eq "box") {
		$shape{type} = 1;
		my $axis_x = $self->value($section, "$id:axis_x") or die "no axis_x in $section\n";
		my $axis_y = $self->value($section, "$id:axis_y") or die "no axis_y in $section\n";
		my $axis_z = $self->value($section, "$id:axis_z") or die "no axis_z in $section\n";
		push @{$shape{box}}, split(/,/, $axis_x), split(/,/, $axis_y);
		push @{$shape{box}}, split(/,/, $axis_z), split(/,/, $offset);
	} else {
		die "unknown shape type in $section\n";
	}
	return \%shape;
}
sub value {
	my $self = shift;
	my ($section, $name) = @_;

	die unless defined $self->{sections_hash}{$section};
	return $self->{sections_hash}{$section}{$name};
}

#######################################################################
package chunked_file;
use strict;
use IO::File;
use Carp;
sub new {
	my $class = shift;
	my ($fn, $mode) = @_;

	my $fh = IO::File->new($fn, $mode) or croak "$fn: $!\n!";
	binmode $fh;

	my $self = {};
	$self->{fh} = $fh;
	$self->{offset} = 0;
	$self->{end_offsets} = [];
	$self->{start_offsets} = [];
	bless($self, $class);

	return $self;
}
sub r_chunk_open {
	my $self = shift;
	my $offset = $self->{end_offsets}[$#{$self->{end_offsets}}];
	defined($offset) && $self->{offset} >= $offset && return undef;
	my $data;
	$self->{fh}->read($data, 8) or return undef;
	my ($index, $size) = unpack('VV', $data);
	$self->{offset} += 8;
	push @{$self->{end_offsets}}, ($self->{offset} + $size);
	return ($index, $size);
}
sub r_chunk_close {
	my $self = shift;
	my $offset = pop @{$self->{end_offsets}};
	$self->{offset} <= $offset or die;
	if ($self->{offset} < $offset) {
		$self->{fh}->seek($offset, SEEK_SET);
		$self->{offset} = $offset;
	}
}
sub r_chunk_data {
	my $self = shift;
	my ($size) = @_;
	my $offset = $self->{end_offsets}[$#{$self->{end_offsets}}];
	defined($size) or $size = $offset - $self->{offset};
	$self->{offset} + $size <= $offset or die;
	my $data = '';
	$size > 0 && ($self->{fh}->read($data, $size) or die);
	$self->{offset} += $size;
	return $data;
}
sub w_chunk_open {
	my $self = shift;
	my ($index) = @_;
	my $data = pack('VV', $index, 0xaaaaaaaa);
	$self->{fh}->write($data, 8) or die;
	push @{$self->{start_offsets}}, $self->{offset};
	$self->{offset} += 8;
}
sub w_chunk_close {
	my $self = shift;
	my $offset = pop @{$self->{start_offsets}};
	$self->{fh}->seek($offset + 4, SEEK_SET) or die;
	my $data = pack('V', $self->{offset} - $offset - 8);
	$self->{fh}->write($data, 4) or die;
	$self->{fh}->seek($self->{offset}, SEEK_SET) or die;
}
sub w_chunk_data {
	my $self = shift;
	my ($data) = @_;
	my $size = length($data);
	$self->{fh}->write($data, $size) or die;
	$self->{offset} += $size;
}
sub w_chunk {
	my $self = shift;
	my ($index, $data) = @_;
	my $size = length($data);
	my $hdr = pack('VV', $index, $size);
	$self->{fh}->write($hdr, 8) or die;
	$size > 0 && ($self->{fh}->write($data, $size) or die);
	$self->{offset} += $size + 8;
}

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
	{ name => 's_flags',		type => 'h16',	default => 0x21 },	# 0x46
	{ name => 'version',		type => 'h16',	default => 0x76 },	# 0x30
	{ name => 'script_version',	type => 'u16',	default => 6 },		# 0x32
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
package cse_ph_skeleton;
use strict;
use constant properties_info => (
	{ name => 'skeleton_name',	type => 'sz',	default => '$editor' },	# smthg|0x08
	{ name => 'skeleton_flags',	type => 'u8',	default => 0 },		# 0x08
	{ name => 'source_id',		type => 'h16',	default => 0xffff },	# 0x48
);
use constant upd_properties_info => (
	{ name => 'upd:skeleton_flags', type => 'u8' },		# 0x08
);
sub state_read {
	$_[1]->unpack_properties($_[0], properties_info);
}
sub state_write {
	$_[1]->pack_properties($_[0], properties_info);
}
sub update_read {
#	$_[1]->unpack_properties($_[0], upd_properties_info);
}
sub update_write {
#	$_[1]->pack_properties($_[0], upd_properties_info);
}
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
package cse_alife_object_physic;
use strict;
use constant properties_info => (
	{ name => 'physic_type',	type => 'h32',	default => 0 },		# 0x138
	{ name => 'mass',		type => 'f32',	default => 0.0 },	# 0x13c
	{ name => 'fixed_bones',	type => 'sz',	default => '' },	# 0x140
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
	cse_ph_skeleton::update_read(@_);
}
sub update_write {
	cse_ph_skeleton::update_write(@_);
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
	cse_ph_skeleton::update_read(@_);
}
sub update_write {
	cse_ph_skeleton::update_write(@_);
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
sub update_read {
	cse_ph_skeleton::update_read(@_);
}
sub update_write {
	cse_ph_skeleton::update_write(@_);
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
sub update_read {
	cse_ph_skeleton::update_read(@_);
}
sub update_write {
	cse_ph_skeleton::update_write(@_);
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
	{ name => 'checked_characters',	type => 'sz',	default => '' },	# 8+0x40=0x48
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
	{ name => 'was_in_smart_terrain',	type => 'u8',	default => 0 },
	{ name => 'death_dropped',		type => 'u8',	default => 0 },
);
sub state_read {
	cse_alife_human_stalker::state_read(@_);
$_[1]->length() == 3 or die;
	$_[1]->unpack_properties($_[0], properties_info);
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
	{ name => 'was_in_smart_terrain',	type => 'u8',	default => 0 },
);
sub state_read {
	cse_alife_monster_base::state_read(@_);
$_[1]->length() == 2 or die;
	$_[1]->unpack_properties($_[0], properties_info);
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
	{ name => 'startup',		type => 'u8',	default => 1 },
	{ name => 'update_time_present',type => 'u8',	default => 0 },
	{ name => 'zone_count',		type => 'u8',	default => 0 },
);
sub state_read {
	cse_alife_smart_zone::state_read(@_);
$_[1]->length() == 3 or die;
	$_[1]->unpack_properties($_[0], (properties_info)[0]);
	$_[0]->{startup} == 0 or die;
	$_[1]->unpack_properties($_[0], (properties_info)[1]);
	$_[0]->{update_time_present} == 0 or die;
	$_[1]->unpack_properties($_[0], (properties_info)[2]);
	$_[0]->{zone_count} == 0 or die;
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
#my @properties_info = (
	{ name => 'duration_end_present',	type => 'u8',	default => 0 },
	{ name => 'idle_end_present',		type => 'u8',	default => 0 },
	{ name => 'gulag_working',		type => 'u8',	default => 0 },
);
sub state_read {
	cse_alife_smart_zone::state_read(@_);
$_[1]->length() == 3 or die;
	$_[1]->unpack_properties($_[0], (properties_info)[0]);
	$_[0]->{duration_end_present} == 0 or die;
	$_[1]->unpack_properties($_[0], (properties_info)[1]);
	$_[0]->{idle_end_present} == 0 or die;
	$_[1]->unpack_properties($_[0], (properties_info)[2]);
	$_[0]->{gulag_working} == 0 or die;
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
sub update_read {
	my $self = shift;
	my ($packet) = @_;
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
package cse_alife_item;
use strict;
use constant properties_info => (
	{ name => 'condition', type => 'f32', default => 0.0 },		# 0xec
);
use constant upd_properties_info => (
	{ name => 'upd:num_items',			type => 'h8' },		# 0x90
	{ name => 'upd:position',			type => 'f32v3' },	# 0x58
	{ name => 'upd:cse_alife_item__unk1_q8v4',	type => 'q8v' },	# 0x70,0x74,0x78,0x7c
	{ name => 'upd:cse_alife_item__unk2_q8v3',	type => 'q8v' },	# 0x34,0x38,0x3c
	{ name => 'upd:cse_alife_item__unk3_q8v3',	type => 'q8v' },	# 0x28,0x2c,0x30
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
	($_[0]->{'upd:num_items'}) = $_[1]->unpack('C');
	return if ($_[0]->{'upd:num_items'} == 0);
	@{$_[0]->{'upd:position'}} = $_[1]->unpack('f3');
	@{$_[0]->{'upd:cse_alife_item__unk1_q8v4'}} = $_[1]->unpack('C4');
	my $info_mask = $_[0]->{'upd:num_items'} >> 5;
	if (($info_mask & 0x02) == 0) {
		die unless $_[1]->length() >= 3;
		@{$_[0]->{'upd:cse_alife_item__unk2_q8v3'}} = $_[1]->unpack('C3');
	}
	if (($info_mask & 0x04) == 0) {
		die unless $_[1]->length() >= 3;
		@{$_[0]->{'upd:cse_alife_item__unk3_q8v3'}} = $_[1]->unpack('C3');
	}
}
sub update_write {
	$_[1]->pack_properties($_[0], (upd_properties_info)[0]);
	return if ($_[0]->{'upd:num_items'} == 0);
	$_[1]->pack_properties($_[0], (upd_properties_info)[1 .. 2]);
	my $info_mask = $_[0]->{'upd:num_items'} >> 5;
	if (($info_mask & 0x02) == 0) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[3]);
	}
	if (($info_mask & 0x04) == 0) {
		$_[1]->pack_properties($_[0], (upd_properties_info)[4]);
	}
}
sub state_import {
	cse_alife_dynamic_object_visual::state_import(@_);
	$_[1]->import_properties($_[2], $_[0], properties_info);
	$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[0]);
	return if ($_[0]->{'upd:num_items'} == 0);
	$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[1 .. 2]);
	my $info_mask = $_[0]->{'upd:num_items'} >> 5;
	if (($info_mask & 0x02) == 0) {
		$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[3]);
	}
	if (($info_mask & 0x04) == 0) {
		$_[1]->import_properties($_[2], $_[0], (upd_properties_info)[4]);
	}
}
sub state_export {
	cse_alife_dynamic_object_visual::state_export(@_);
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	$_[1]->export_properties(undef, $_[0], (upd_properties_info)[0]);
	return if ($_[0]->{'upd:num_items'} == 0);
	$_[1]->export_properties(undef, $_[0], (upd_properties_info)[1 .. 2]);
	my $info_mask = $_[0]->{'upd:num_items'} >> 5;
	if (($info_mask & 0x02) == 0) {
		$_[1]->export_properties(undef, $_[0], (upd_properties_info)[3]);
	}
	if (($info_mask & 0x04) == 0) {
		$_[1]->export_properties(undef, $_[0], (upd_properties_info)[4]);
	}
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
package alife_object;
use strict;
use constant section_to_class => {
	actor					=> 'cse_alife_creature_actor',

	graph_point				=> 'cse_alife_graph_point',

	stalker					=> 'se_stalker',
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
	burer_normal				=> 'se_monster',
	burer_arena				=> 'se_monster',
	burer_indoor				=> 'se_monster',
	burer_outdoor				=> 'se_monster',

	m_cat_e					=> 'se_monster',
	cat_weak				=> 'se_monster',

	m_chimera				=> 'se_monster',

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

	level_changer				=> 'cse_alife_level_changer',

	script_zone				=> 'cse_alife_smart_zone',

	smart_terrain				=> 'se_smart_terrain',

	respawn					=> 'se_respawn',

	anomaly_field				=> 'se_anomaly_field',

	zone_team_base				=> 'cse_alife_team_base_zone',

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
	'ammo_5.45x39_ap'			=> 'cse_alife_item_ammo',
	'ammo_5.45x39_fmj'			=> 'cse_alife_item_ammo',
	'ammo_5.56x45_ap'			=> 'cse_alife_item_ammo',
	'ammo_5.56x45_ss190'			=> 'cse_alife_item_ammo',
	'ammo_7.62x54_7h1'			=> 'cse_alife_item_ammo',
	'ammo_7.62x54_7h14'			=> 'cse_alife_item_ammo',
	'ammo_7.62x54_ap'			=> 'cse_alife_item_ammo',
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
	wpn_vintorez				=> 'cse_alife_item_weapon_magazined',
	wpn_svd					=> 'cse_alife_item_weapon_magazined',
	wpn_svd_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_svu					=> 'cse_alife_item_weapon_magazined',
	wpn_ak74u				=> 'cse_alife_item_weapon_magazined',
	wpn_ak74u_m1				=> 'cse_alife_item_weapon_magazined',
	wpn_gauss				=> 'cse_alife_item_weapon_magazined',
	wpn_groza				=> 'cse_alife_item_weapon_magazined_w_gl',
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
	'wpn_rg-6'				=> 'cse_alife_item_weapon_shotgun',
	wpn_rg6_m1				=> 'cse_alife_item_weapon_shotgun',
	hunters_toz				=> 'cse_alife_item_weapon_shotgun',

	m_car					=> 'cse_alife_car',
	helicopter				=> 'cse_alife_helicopter',

	turret_mgun				=> 'cse_alife_helicopter',
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
				my $packet = data_packet->new(substr($data, 2));
				if ($index == 0) {
					cse_abstract::state_read($self->{cse_object}, $packet);
					my $class_name = section_to_class->{$self->{cse_object}->{section_name}};
					defined $class_name or die "unknown $self->{cse_object}->{section_name}";
					bless $self->{cse_object}, $class_name;
					$self->{cse_object}->state_read($packet);
$packet->length() == 0 or die "data left in $self->{cse_object}->{name}";
				} elsif ($index == 1) {
					cse_abstract::update_read($self->{cse_object}, $packet);
					UNIVERSAL::can($self->{cse_object}, 'update_read') && do {
						$self->{cse_object}->update_read($packet);
					};
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
	my $packet2 = data_packet->new('');
	$self->{cse_object}->state_write($packet2);
	my $packet1 = data_packet->new('');
	cse_abstract::state_write($self->{cse_object}, $packet1, $object_id, $packet2->length() + 2);
	$cf->w_chunk_data(pack('v', $packet1->length() + $packet2->length()));
	$cf->w_chunk_data($packet1->data());
	$cf->w_chunk_data($packet2->data());
	$cf->w_chunk_close();

	$cf->w_chunk_open(1);
	my $upd_packet2 = data_packet->new('');
	UNIVERSAL::can($self->{cse_object}, 'update_write') && do {
		$self->{cse_object}->update_write($upd_packet2);
	};
	my $upd_packet1 = data_packet->new('');
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
	my $packet = data_packet->new($cf->r_chunk_data());
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
use Carp;
use Scalar::Util 'blessed';
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	my $self = shift;
	my ($fn) = @_;

	my $cf = chunked_file->new($fn, 'r') or die;
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
		} else {
			die;
		}
		$cf->r_chunk_close();
	}
}
sub write {
	my $self = shift;
	my ($fn) = @_;

	my $cf = chunked_file->new($fn, 'w') or die;
	$self->write_header($cf);
	$self->write_alife($cf);
	$self->write_section2($cf);
	$self->write_way($cf);
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
my $class_name = ref $object->{cse_object};
if (1 and ($class_name eq 'se_stalker' or $class_name eq 'se_monster') and $object->{cse_object}->{story_id} == -1) {
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

#	my $packet = data_packet->new($self->{section2_raw_data});
#	my ($obj_count) = $packet->unpack('V');
#	print "obj_count = $obj_count\n";
#	while ($obj_count--) {
#		my ($x, $y, $z, $lvid, $unk) = $packet->unpack('fffVf');
#		print "$lvid: ($x, $y, $z), $unk\n";
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
sub import {
	my $self = shift;
	my ($fn) = @_;

	my $if = ini_file->new($fn, 'r') or die;

	$self->import_header($if);
	$self->import_alife($if);
	$self->import_section2($if);
	$self->import_way($if);
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
		my $lif = ini_file->new($fn, 'r') or die;
		foreach my $section (@{$lif->{sections_list}}) {
			my $object = alife_object->new();
			$object->state_import($lif, $section);
			push @{$self->{alife_objects}}, $object;
		}
	}
}
sub import_section2 {
	my $self = shift;
	my ($if) = @_;

	my $fn = $if->value('section2', 'binary_files') or die;
	my $bin_fh = IO::File->new($fn, 'r') or croak "$fn: $!\n!";
	binmode $bin_fh;
	$bin_fh->read($self->{section2_raw_data}, ($bin_fh->stat())[7]);
}
sub import_way {
	my $self = shift;
	my ($if) = @_;

	foreach my $fn (split /,/, ($if->value('way', 'source_files') or die)) {
		$fn =~ s/^\s*|\s*$//g;
		my $lif = ini_file->new($fn, 'r') or die;
		foreach my $section (@{$lif->{sections_list}}) {
			my $object = way_object->new();
			$object->state_import($lif, $section);
			push @{$self->{way_objects}}, $object;
		}
	}
}
sub export {
	my $self = shift;
	my ($fn) = @_;

	my $if = ini_file->new($fn, 'w') or die;

	$self->export_header($if);
	$self->export_alife($if);
	$self->export_section2($if);
	$self->export_way($if);
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
		my $level = level::name_by_gvid($cse_object->{game_vertex_id});
		die "unknown location of the alife object\n" unless defined $level;
		my $lif = $if_by_level{$level};
		if (!defined $lif) {
			$lif = ini_file->new("alife_$level.ltx", 'w') or die;
			$if_by_level{$level} = $lif;
			push @levels, "alife_$level.ltx";
		}
		$object->state_export($lif, $id++);
	}
	my $fh = $if->{fh};
	print $fh "[alife]\nsource_files = <<END\n", join(",\n", sort @levels), "\nEND\n\n";
}
sub export_section2 {
	my $self = shift;
	my ($if) = @_;

	my $fn = 'section2.bin';
	my $bin_fh = IO::File->new($fn, 'w') or croak "$fn: $!\n!";
	binmode $bin_fh;
	$bin_fh->write($self->{section2_raw_data}, length($self->{section2_raw_data}));

	my $fh = $if->{fh};
	print $fh "[section2]\nbinary_files = $fn\n\n";
}
# FIXME: it is better to scan level.game files
use constant way_name_exceptions => {
	kat_teleport_to_dark_city_orientation	=> 'l03u_agr_underground',
	walk_3					=> 'l05_bar',
	rad_heli_move				=> 'l10_radar',
	pri_heli4_go2_path			=> 'l11_pripyat',
	sar_teleport_0000_exit_look		=> 'l12u_sarcofag',
	sar_teleport_0000_exit_walk		=> 'l12u_sarcofag',
};
sub export_way {
	my $self = shift;
	my ($if) = @_;

	my %info_by_level;

	my $default_level = 'l05_bar';
	foreach my $object (@{$self->{way_objects}}) {
		my $level = level::name_by_gvid($object->{points}[0]->{game_vertex_id});
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
			$info->{lif} = ini_file->new("way_$level.ltx", 'w') or die;
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
	}

	my $fh = $if->{fh};
	print $fh "[way]\nsource_files = <<END\n", join(",\n", map {"way_$_.ltx"} sort(keys %info_by_level)), "\nEND\n\n";
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

test3(); exit;
#local $SIG{__WARN__} = sub {die};

sub usage {
	return <<END
S.T.A.L.K.E.R. all.spawn compiler/decompiler
Usage: acdc -d all.spawn [-o outdir]
       acdc -c all.ltx [-f flag1[,...]] [-a level:point] [-o outfile]
       acdc -l level.spawn
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

use IO::File;
sub test3 {
	my $fn = 'level.spawn';
	my $cf = chunked_file->new($fn, 'r') or die;
	my @cse_objects;
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;

		my $packet = data_packet->new($cf->r_chunk_data());
		my $cse_object = {};
		cse_abstract::state_read($cse_object, $packet);
		my $class_name = alife_object::section_to_class->{$cse_object->{section_name}};
		defined $class_name or die "unknown $cse_object->{section_name}";
		bless $cse_object, $class_name;
		$cse_object->state_read($packet);
		$cf->r_chunk_close();
		push @cse_objects, $cse_object;
	}

	my $if = ini_file->new('level_spawn.ltx', 'w') or die;
	my $fh = $if->{fh};
	my $id = 0;
	foreach my $cse_object (@cse_objects) {
		print $fh "[$id]\n";
		cse_abstract::state_export($cse_object, $if);
		$cse_object->state_export($if);
		print $fh "\n";
		$id++;
	}
}
