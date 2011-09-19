# Last modified: 19 Sep 2011
##########################################################
package gg_header;
use strict;
use constant header_1472 => (
	{ name => 'version',		type => 'u32' },
	{ name => 'vertex_count',	type => 'u32' },
	{ name => 'level_count',	type => 'u32' },
);
use constant header_1935 => (
	{ name => 'version',		type => 'u32' },
	{ name => 'level_count',	type => 'u32' },
	{ name => 'vertex_count',	type => 'u32' },
	{ name => 'edge_count',		type => 'u32' },
	{ name => 'level_point_count',	type => 'u32' },
);
use constant header_2215 => (
	{ name => 'version',		type => 'u32' },
	{ name => 'level_count',	type => 'u32' },
	{ name => 'vertex_count',	type => 'u32' },
	{ name => 'edge_count',		type => 'u32' },
	{ name => 'level_point_count',	type => 'u32' },
	{ name => 'guid',	type => 'guid' },
);
use constant header_SOC => (
	{ name => 'version',		type => 'u8' },
	{ name => 'vertex_count',	type => 'u16' },
	{ name => 'edge_count',		type => 'u32' },
	{ name => 'level_point_count',	type => 'u32' },
	{ name => 'guid',		type => 'guid' },
	{ name => 'level_count',	type => 'u8' },
);
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	if ($_[2] eq '1469' or $_[2] eq '1472') {
		$_[1]->unpack_properties($_[0], header_1472);
	} elsif ($_[2] eq '1510' or $_[2] eq '1935') {
		$_[1]->unpack_properties($_[0], header_1935);	
	} elsif ($_[2] eq '2215') {
		$_[1]->unpack_properties($_[0], (header_2215)[0..5]);	
	} else {
		$_[1]->unpack_properties($_[0], header_SOC);		
	}
}
#######################################################################
package gg_level;
use strict;
use constant level_1469 => (
	{ name => 'level_name',		type => 'sz' },
	{ name => 'offset',		type => 'f32v3' },
);
use constant level_1472 => (
	{ name => 'level_name',		type => 'sz' },
	{ name => 'offset',		type => 'f32v3' },
	{ name => 'level_id',		type => 'u32' },
);
use constant level_1935 => (
	{ name => 'level_name',		type => 'sz' },
	{ name => 'offset',		type => 'f32v3' },
	{ name => 'level_id',		type => 'u32' },
	{ name => 'section_name',	type => 'sz' },
);
use constant level_2215 => (
	{ name => 'level_name',		type => 'sz' },
	{ name => 'offset',		type => 'f32v3' },
	{ name => 'level_id',		type => 'u32' },
	{ name => 'section_name',	type => 'sz' },
	{ name => 'guid',	type => 'guid' },
);
use constant level_SOC => (
	{ name => 'level_name',		type => 'sz' },
	{ name => 'offset',		type => 'f32v3' },
	{ name => 'level_id',		type => 'u8' },
	{ name => 'section_name',	type => 'sz' },
	{ name => 'guid',	type => 'guid' },
);
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	if ($_[2] eq '1469') {
		$_[1]->unpack_properties($_[0], level_1469);
		if ($_[0]->{level_name} eq 'level2_test') {
			$_[0]->{level_id} = 0;
		} elsif ($_[0]->{level_name} eq 'occ_part') {
			$_[0]->{level_id} = 1;
		} else {
			$_[0]->{level_id} = 2;
		}
	} elsif ($_[2] eq '1472' or $_[2] eq '1510') {
		$_[1]->unpack_properties($_[0], level_1472);
	} elsif ($_[2] eq '1935') {
		$_[1]->unpack_properties($_[0], level_1935);	
	} elsif ($_[2] eq '2215') {
		$_[1]->unpack_properties($_[0], level_2215);	
	} else {
		$_[1]->unpack_properties($_[0], level_SOC);		
	}
}
#######################################################################
package gg_vertex;
use strict;
use constant vertex_1472 => (
	{ name => 'level_point',	type => 'f32v3' },
	{ name => 'game_point',		type => 'f32v3' },
	{ name => 'level_id',	type => 'u8' },
	{ name => 'level_vertex_id',	type => 'u24' },
	{ name => 'vertex_type',	type => 'u8v4' },
	{ name => 'edge_count',	type => 'u8' },
	{ name => 'edge_offset',	type => 'u24' },
);
use constant vertex_1935 => (
	{ name => 'level_point',	type => 'f32v3' },
	{ name => 'game_point',		type => 'f32v3' },
	{ name => 'level_id',	type => 'u8' },
	{ name => 'level_vertex_id',	type => 'u24' },
	{ name => 'vertex_type',	type => 'u8v4' },
	{ name => 'edge_count',	type => 'u8' },		#1
	{ name => 'edge_offset',	type => 'u24' },		#4
	{ name => 'level_point_count',		type => 'u8' },			#1
	{ name => 'level_point_offset',	type => 'u24' },	#4
);
use constant vertex_SOC => (
	{ name => 'level_point',	type => 'f32v3' },
	{ name => 'game_point',		type => 'f32v3' },
	{ name => 'level_id',	type => 'u8' },
	{ name => 'level_vertex_id',	type => 'u24' },
	{ name => 'vertex_type',	type => 'u8v4' },
	{ name => 'edge_offset',	type => 'u32' },
	{ name => 'level_point_offset',	type => 'u32' },
	{ name => 'edge_count',		type => 'u8' },
	{ name => 'level_point_count',	type => 'u8' },
);
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	if (!defined $_[4]) {
		$_[1]->unpack_properties($_[0], vertex_1472);
	} elsif ($_[4] eq '1510' or $_[4] eq '1935' or $_[4] eq '2215') {
		$_[1]->unpack_properties($_[0], vertex_1935);
	} else {
		$_[1]->unpack_properties($_[0], vertex_SOC);
	}
	if (defined $_[4]) {
		$_[0]->{edge_index} = ($_[0]->{edge_offset} - $_[2]) / (::edge_block_size($_[4]));
		$_[0]->{level_point_index} = ($_[0]->{level_point_offset} - $_[3]) / 0x14;
	} else {
		$_[0]->{edge_index} = ($_[0]->{edge_offset} - $_[2]) / (::edge_block_size($_[3]));
	}
}
#######################################################################
package gg_edge;
use strict;
use constant edge_builds => (
	{ name => 'game_vertex_id',	type => 'u32' },
	{ name => 'distance',		type => 'f32' },
);
use constant edge_SOC => (
	{ name => 'game_vertex_id',	type => 'u16' },
	{ name => 'distance',		type => 'f32' },
);
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	if ($_[2] eq 'soc' or $_[2] eq 'cop') {
		$_[1]->unpack_properties($_[0], edge_SOC);
	} else {
		$_[1]->unpack_properties($_[0], edge_builds);
	}
}
#######################################################################
package stkutils::graph;
use strict;
use IO::File;
use stkutils::data_packet;
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
	my ($version, $graph_data) = @_;
	my $data;
	my $packet;

	print "reading graph...\n";
	$data = substr($graph_data, 0, 300000);
	$packet = stkutils::data_packet->new($data);
	my $hs = ::header_size($self->{build_version});
	my $vbs = ::vertex_block_size($self->{build_version});
	my $ebs = ::edge_block_size($self->{build_version});
	print "	reading header...\n";
	my $data_h = substr($data, 0, $hs);
	$self->{header} = gg_header->new();
	$self->{header}->read(stkutils::data_packet->new($data_h), $self->{build_version});
	my $edges_offset = $self->{header}->{vertex_count} * $vbs;
	my $level_points_offset;
	if (not ($self->{build_version} eq '1469' or $self->{build_version} eq '1472')) {
		$level_points_offset = $edges_offset + $self->{header}->{edge_count} * $ebs;
	}
	print "	reading levels...\n";	
	# 4KB should be enough for the level
	my $data_l = substr($data, 0, 0x1000);
	my $packet_l = stkutils::data_packet->new(substr($data_l, $hs));
	for (my $i = 0; $i < $self->{header}->{level_count}; $i++) {
		my $level = gg_level->new();			
		$level->read($packet_l, $self->{build_version});
		push @{$self->{levels}}, $level;
	}	
	my %level_by_id = ();
	foreach my $level (@{$self->{levels}}) {
		$level_by_id{$level->{level_id}} = \$level;
	}
	print "	reading vertices...\n";	
	my $packet_v = stkutils::data_packet->new(substr($data, 0x1000 - $packet_l->length()));
	for (my $i = 0; $i < $self->{header}->{vertex_count}; $i++) {
		my $vertex = gg_vertex->new();
		if ($self->{build_version} eq '1469' or $self->{build_version} eq '1472') {
			$vertex->read($packet_v, $edges_offset, $self->{build_version});
		} else {
			$vertex->read($packet_v, $edges_offset, $level_points_offset, $self->{build_version});
		}
		my $level_name = $level_by_id{$vertex->{level_id}};
		if (not defined $level_name) {
			$self->error_handler($version, $graph_data);
			return;
		}
		push @{$self->{vertices}}, $vertex;
	}
	my $game_vertex_id = 0;
	my $level_id = -1;
	foreach my $vertex (@{$self->{vertices}}) {
		if ($vertex->{level_id} != $level_id) {
			my $level = $level_by_id{$vertex->{level_id}};
			$level_id = $vertex->{level_id};
			$self->{level_by_guid}{$game_vertex_id} = $$level->{level_name};
		}
		$game_vertex_id++;
	}
}
sub show_guids {
	my $self = shift;
	my ($fn) = @_;
	my $fh;
	if (defined $fn) {
		$fh = IO::File->new($fn, 'w') or die "cannot open $fn\n";
	}
	my %level_by_id = ();
	foreach my $level (@{$self->{levels}}) {
		$level_by_id{$level->{level_id}} = \$level;
	}
	my $game_vertex_id = 0;
	my $level_id = -1;
	foreach my $vertex (@{$self->{vertices}}) {
		if ($vertex->{level_id} != $level_id) {
			my $level = $level_by_id{$vertex->{level_id}};
			if (defined $fn) {
				print $fh "\n[$$level->{level_name}]\ngvid0 = $game_vertex_id\nid = $vertex->{level_id}\n";
			} else {
				print "{ gvid0 => $game_vertex_id,		name => '$$level->{level_name}' },\n";			
			}
			$level_id = $vertex->{level_id};
		}
		$game_vertex_id++;
	}
	if (defined $fn) {
		$fh->close();
	}
}
sub gvid_by_name {
	my $self = shift;
	my %rev = reverse %{$self->{level_by_guid}};
	foreach my $level (sort {$b cmp $a} keys %rev) {
		if ($_[0] eq $level) {
			return $rev{$level};
		}
	}
	return undef;
}
sub level_name {
	my $self = shift;
	foreach my $guid (sort {$b <=> $a} keys %{$self->{level_by_guid}}) {
		if ($_[0] >= $guid) {
			return $self->{level_by_guid}{$guid};
		}
	}
	return undef;
}
sub error_handler {
	my $self = shift;
	my ($version, $graph_data) = @_;
	print "error occured! trying to fix...\n";
	$self->{build_version} = '1935';
	$self->read($version, $graph_data);
}
1;
#####################################################################