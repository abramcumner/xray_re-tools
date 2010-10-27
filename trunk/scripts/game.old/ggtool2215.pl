#!/bin/perl -w -I \temp\1\bin
#
# tab size:	8
#
#######################################################################
package gg_header;
use strict;
use constant properties_info => (
	{ name => 'version',		type => 'u32' },
	{ name => 'level_count',	type => 'u32' },
	{ name => 'vertex_count',	type => 'u32' },
	{ name => 'edge_count',		type => 'u32' },
	{ name => 'level_point_count',	type => 'u32' },
	{ name => 'guid',		type => 'guid' },
);
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	$_[1]->unpack_properties($_[0], properties_info);
}
sub export {
	my $self = shift;
	my ($fh) = @_;

	print $fh "version = $self->{version}\n";
	print $fh "level_count = $self->{level_count}\n";
	print $fh "vertex_count = $self->{vertex_count}\n";
	print $fh "edge_count = $self->{edge_count}\n";
	print $fh "level_point_count = $self->{level_point_count}\n\n";
}

#######################################################################
package gg_level;
use strict;
use constant properties_info => (
	{ name => 'level_name',		type => 'sz' },
	{ name => 'offset',		type => 'f32v3' },
	{ name => 'level_id',		type => 'u32' },
	{ name => 'section_name',	type => 'sz' },
	{ name => 'guid',		type => 'guid' },
);
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	$_[1]->unpack_properties($_[0], properties_info);
}
sub export {
	my $self = shift;
	my ($fh) = @_;

	print $fh "level_name = $self->{level_name}\n";
	print $fh "offset = ", join(',', @{$self->{offset}}), "\n";
	print $fh "level_id = $self->{level_id}\n";
	print $fh "section_name = $self->{section_name}\n\n";
}

#######################################################################
package gg_vertex;
use strict;
use constant properties_info => (
	{ name => 'level_point',	type => 'f32v3' },
	{ name => 'game_point',		type => 'f32v3' },
	{ name => 'level_id',		type => 'u8' },
	{ name => 'level_vertex_id',	type => 'u24' },
	{ name => 'vertex_type',	type => 'u8v4' },
	{ name => 'edge_count',		type => 'u8' },
	{ name => 'edge_offset',	type => 'u24' },
	{ name => 'level_point_count',	type => 'u8' },
	{ name => 'level_point_offset',	type => 'u24' },
);
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	$_[1]->unpack_properties($_[0], properties_info);
	$_[0]->{edge_index} = ($_[0]->{edge_offset} - $_[2]) / 0x08;
	if ($_[0]->{level_point_count} > 0) {
		$_[0]->{level_point_index} = ($_[0]->{level_point_offset} - $_[3]) / 0x14;
	} else {
		$_[0]->{level_point_index} = 0;
	}
}
sub export {
	my $self = shift;
	my ($fh) = @_;

	print $fh "level_point = ", join(',', @{$self->{level_point}}), "\n";
	print $fh "game_point = ", join(',', @{$self->{game_point}}), "\n";
	print $fh "level_id = $self->{level_id}\n";
	print $fh "level_vertex_id = $self->{level_vertex_id}\n";
	print $fh "vertex_type = ", join(',', @{$self->{vertex_type}}), "\n";
	print $fh "edges = $self->{edge_index}, $self->{edge_count}\n";
	print $fh "level_points = $self->{level_point_index}, $self->{level_point_count}\n\n";
}

#######################################################################
package gg_edge;
use strict;
use constant properties_info => (
	{ name => 'game_vertex_id',	type => 'u32' },
	{ name => 'distance',		type => 'f32' },
);
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	$_[1]->unpack_properties($_[0], properties_info);
}
sub export {
	my $self = shift;
	my ($fh) = @_;

	print $fh "game_vertex_id = $self->{game_vertex_id}\n";
	print $fh "distance = $self->{distance}\n\n";
}

#######################################################################
package gg_level_point;
use strict;
use constant properties_info => (
	{ name => 'point',		type => 'f32v3' },
	{ name => 'level_vertex_id',	type => 'u32' },
	{ name => 'distance',		type => 'f32' },
);
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	$_[1]->unpack_properties($_[0], properties_info);
}
sub export {
	my $self = shift;
	my ($fh) = @_;

	print $fh "point = ", join(',', @{$self->{point}}), "\n";
	print $fh "level_vertex_id = $self->{level_vertex_id}\n";
	print $fh "distance = $self->{distance}\n\n";
}

#######################################################################
package game_graph;
use strict;
use IO::File;
use stkutils::data_packet;
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	my $self = shift;
	my ($fn) = @_;
	my $data;

	my $fh = IO::File->new($fn, 'r') or die "cannot open $fn\n";
	binmode $fh;

	$fh->read($data, 0x24) or die;
	$self->{header} = gg_header->new();
	$self->{header}->read(stkutils::data_packet->new($data));

	my $edges_offset = $self->{header}->{vertex_count} * 0x28;
	my $level_points_offset = $edges_offset + $self->{header}->{edge_count} * 0x08;

	# 4KB should be enough for the level
	$fh->read($data, 0x1000) or die;
	my $packet = stkutils::data_packet->new($data);
	for (my $i = 0; $i < $self->{header}->{level_count}; $i++) {
		my $level = gg_level->new();
		$level->read($packet);
		push @{$self->{levels}}, $level;
	}
	$fh->seek(-$packet->length(), SEEK_CUR);
	for (my $i = 0; $i < $self->{header}->{vertex_count}; $i++) {
		$fh->read($data, 0x28) or die;
		my $vertex = gg_vertex->new();
		$vertex->read(stkutils::data_packet->new($data), $edges_offset, $level_points_offset);
		push @{$self->{vertices}}, $vertex;
	}
	for (my $i = 0; $i < $self->{header}->{edge_count}; $i++) {
		$fh->read($data, 0x08) or die;
		my $edge = gg_edge->new();
		$edge->read(stkutils::data_packet->new($data));
		push @{$self->{edges}}, $edge;
	}
	for (my $i = 0; $i < $self->{header}->{level_point_count}; $i++) {
		$fh->read($data, 0x14) or die;
		my $level_point = gg_level_point->new();
		$level_point->read(stkutils::data_packet->new($data));
		push @{$self->{level_points}}, $level_point;
	}
}
sub export {
	my $self = shift;
	my ($fn) = @_;

	my $fh = IO::File->new($fn, 'w') or die "cannot open $fn\n";

	print $fh "[header]\n";
	$self->{header}->export($fh);

	my $i = 0;
	foreach my $level (@{$self->{levels}}) {
		print $fh "[level_$i]\n";
		$level->export($fh);
		$i++;
	}
	$i = 0;
	foreach my $vertex (@{$self->{vertices}}) {
		print $fh "[vertex_$i]\n";
		$vertex->export($fh);
		$i++;
	}
	$i = 0;
	foreach my $edge (@{$self->{edges}}) {
		print $fh "[edge_$i]\n";
		$edge->export($fh);
		$i++;
	}

	$i = 0;
	foreach my $level_point (@{$self->{level_points}}) {
		print $fh "[level_point_$i]\n";
		$level_point->export($fh);
		$i++;
	}
}
sub show_links {
	my $self = shift;

	my %level_by_id = ();
	foreach my $level (@{$self->{levels}}) {
#		next unless $level->{level_id} <= 20;
		$level_by_id{$level->{level_id}} = \$level;
	}

	my $vid = 0;
	foreach my $vertex (@{$self->{vertices}}) {
		for (my $i = 0; $i < $vertex->{edge_count}; $i++) {
			my $edge = $self->{edges}[$vertex->{edge_index} + $i];
			my $vid2 = $edge->{game_vertex_id};
			my $vertex2 = $self->{vertices}[$vid2];
			if ($vertex->{level_id} != $vertex2->{level_id}) {
#				my $name = ${$level_by_id{$vertex->{level_id}}}->{level_name};
#				my $name2 = ${$level_by_id{$vertex2->{level_id}}}->{level_name};
				my $level = $level_by_id{$vertex->{level_id}};
				my $level2 = $level_by_id{$vertex2->{level_id}};
				my $name = $$level->{level_name};
				my $name2 = $$level2->{level_name};
				print "$name ($vid)", ' -> ', "$name2 ($vid2), $edge->{distance}\n";
#				print "$name ($vid)", ' -> ', "$name2 ($vid2), $edge->{distance}, ",
#					"[", join(',', @{$vertex->{vertex_type}}), "], ", 
#					"[", join(',', @{$vertex2->{vertex_type}}), "]\n";
			}
		}
		$vid++;
	}
}
sub show_level_gvids {
	my $self = shift;

	my %level_by_id = ();
	foreach my $level (@{$self->{levels}}) {
		$level_by_id{$level->{level_id}} = \$level;
	}
	my $game_vertex_id = 0;
	my $level_id = -1;
	foreach my $vertex (@{$self->{vertices}}) {
		if ($vertex->{level_id} != $level_id) {
			my $level = $level_by_id{$vertex->{level_id}};
			print "$$level->{level_name} => $game_vertex_id\n";
			$level_id = $vertex->{level_id};
		}
		$game_vertex_id++;
	}
}
sub count_level_points {
	my $self = shift;

	my %level_by_id = ();
	foreach my $level (@{$self->{levels}}) {
		$level_by_id{$level->{level_id}} = \$level;
	}

	my $count = 0;
	foreach my $vertex (@{$self->{vertices}}) {
		last if $vertex->{level_id} > 20;
		$count += $vertex->{level_point_count};
	}
	print "level points: $count\n";
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R. build 2215 game.graph tool
Usage: ggtool.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage;
my $gg = game_graph->new();
$gg->read($fn);
#$gg->count_level_points();
$gg->export("out.txt");
#$gg->show_links();
#$gg->show_level_gvids();
