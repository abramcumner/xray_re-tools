# S.T.A.L.K.E.R. game graph handling module.
# Last modified: 21 Sep 2011
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
		$_[1]->unpack_properties($_[0], header_2215);	
	} else {
		$_[1]->unpack_properties($_[0], header_SOC);		
	}
}
sub write {
	if ($_[2] eq '1469' or $_[2] eq '1472') {
		$_[1]->pack_properties($_[0], header_1472);
	} elsif ($_[2] eq '1510' or $_[2] eq '1935') {
		$_[1]->pack_properties($_[0], header_1935);	
	} elsif ($_[2] eq '2215') {
		$_[1]->pack_properties($_[0], header_2215);	
	} else {
		$_[1]->pack_properties($_[0], header_SOC);		
	}
}
sub export {
	my $self = shift;
	my ($fh, $lg) = @_;
	print $fh "version = $self->{version}\n";
	print $fh "level_count = $self->{level_count}\n";
	print $fh "vertex_count = $self->{vertex_count}\n";
	print $fh "level_point_count = $self->{level_point_count}\n" if (!$lg && defined $self->{level_point_count});
	print $fh "edge_count = $self->{edge_count}\n" if (defined $self->{edge_count});
	print $fh "\n";
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
sub write {
	if (defined ($_[0]->{level_id} && $_[3]) ) {
		$_[0]->{level_id} = $_[3];
	}
	if ($_[2] eq '1469') {
		$_[1]->pack_properties($_[0], level_1469);
	} elsif ($_[2] eq '1472' or $_[2] eq '1510') {
		$_[1]->pack_properties($_[0], level_1472);
	} elsif ($_[2] eq '1935') {
		$_[1]->pack_properties($_[0], level_1935);	
	} elsif ($_[2] eq '2215') {
		$_[1]->pack_properties($_[0], level_2215);	
	} else {
		$_[1]->pack_properties($_[0], level_SOC);		
	}
}
sub export {
	my $self = shift;
	my ($fh, $lg) = @_;
	print $fh "level_name = $self->{level_name}\n";
	print $fh "level_id = $self->{level_id}\n" if (!$lg && defined $self->{level_id});
	print $fh "section_name = $self->{section_name}\n" if (!$lg && defined $self->{section_name});
	print $fh "offset = ", join(',', @{$self->{offset}}), "\n\n";
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
	{ name => 'edge_count',	type => 'u8' },		
	{ name => 'edge_offset',	type => 'u24' },	
	{ name => 'level_point_count',		type => 'u8' },	
	{ name => 'level_point_offset',	type => 'u24' },
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
		$_[0]->{edge_index} = ($_[0]->{edge_offset} - $_[2]) / (stkutils::graph::edge_block_size($_[4]));
		$_[0]->{level_point_index} = ($_[0]->{level_point_offset} - $_[2] - $_[3]) / 0x14;
	} else {
		$_[0]->{edge_index} = ($_[0]->{edge_offset} - $_[2]) / (stkutils::graph::edge_block_size($_[3]));
	}
}
sub write {
	if ($_[4] eq '1469' or $_[4] eq '1472') {
		$_[1]->pack_properties($_[0], vertex_1472);
	} elsif ($_[4] eq '1510' or $_[4] eq '1935' or $_[4] eq '2215') {
		$_[0]->{edge_offset} = (stkutils::graph::edge_block_size($_[4])) * ($_[0]->{edge_index}) + $_[2];
		$_[0]->{level_point_offset} = 0x14 * $_[0]->{level_point_index} + $_[2] + $_[3];
		$_[1]->pack_properties($_[0], vertex_1935);
	} else {
		$_[0]->{edge_offset} = (stkutils::graph::edge_block_size($_[4])) * ($_[0]->{edge_index}) + $_[2];
		$_[0]->{level_point_offset} = 0x14 * $_[0]->{level_point_index} + $_[2] + $_[3];
		$_[1]->pack_properties($_[0], vertex_SOC);
	}
}
sub export {
	my $self = shift;
	my ($fh, $lg) = @_;
	print $fh "level_point = ", join(',', @{$self->{level_point}}), "\n";
	print $fh "game_point = ", join(',', @{$self->{game_point}}), "\n" if (!$lg);
	print $fh "level_id = $self->{level_id}\n" if (!$lg);
	print $fh "level_vertex_id = $self->{level_vertex_id}\n";
	print $fh "vertex_type = ", join(',', @{$self->{vertex_type}}), "\n";
	print $fh "level_points = $self->{level_point_index}, $self->{level_point_count}\n";
	print $fh "edges = $self->{edge_index}, $self->{edge_count}\n\n";
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
sub write {
	if ($_[2] eq 'soc' or $_[2] eq 'cop') {
		$_[1]->pack_properties($_[0], edge_SOC);
	} else {
		$_[1]->pack_properties($_[0], edge_builds);
	}
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
sub write {
	$_[1]->pack_properties($_[0], properties_info);
}
sub export {
	my $self = shift;
	my ($fh) = @_;

	print $fh "point = ", join(',', @{$self->{point}}), "\n";
	print $fh "level_vertex_id = $self->{level_vertex_id}\n";
	print $fh "distance = $self->{distance}\n\n";
}
#######################################################################
package gg_cross_table;
use strict;
use constant properties_info => (
	{ name => 'size',			type => 'u32' },
	{ name => 'version',		type => 'u32' },
	{ name => 'cell_count',		type => 'u32' },
	{ name => 'vertex_count',	type => 'u32' },
	{ name => 'level_guid',		type => 'guid' },
	{ name => 'game_guid',		type => 'guid' },
);
sub new {
	my $class = shift;
	my $self = {};
	$self->{cells} = {};
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
	print $fh "cell_count = $self->{cell_count}\n";
	print $fh "vertex_count = $self->{vertex_count}\n";
	print $fh "level_guid = $self->{level_guid}\n";
	print $fh "game_guid = $self->{game_guid}\n";
	print $fh "\n";
}
#######################################################################
package stkutils::graph;
use strict;
use IO::File;
use stkutils::data_packet;
use stkutils::debug;
use diagnostics;

sub new {
	my $class = shift;
	my $self = {};
	$self->{level_by_id} = {};
	$self->{level_by_guid} = {};
	$self->{lp_offsets} = {};
	$self->{lp_counts} = {};
	$self->{ct_size} = {};
	$self->{ct_offset} = {};
	$self->{raw_cross_tables} = {};
	$self->{raw_vertices} = '';
	$self->{raw_edges} = '';
	$self->{raw_level_points} = '';
	$self->{raw_cross_tables_all} = '';
	$self->{is_level_graph} = 0;
	bless($self, $class);
	return $self;
}
sub decompose_graph {
	my $self = shift;
	my ($data) = @_;
	print "reading game graph...\n";
	$self->read_header(substr($data, 0, stkutils::graph::header_size($self->{gg_version})));
	$self->read_levels(substr($data, 0, 0x1000));
	$self->{raw_vertices} = substr($data, $self->{vertices_offset}, $self->{edges_offset});
	if ($self->{gg_version} eq '1469' || $self->{gg_version} eq '1472' || $self->level_graph()) {
		$self->{raw_edges} = substr($data, $self->{vertices_offset} + $self->{edges_offset});
	} else {
		if ($self->{gg_version} eq '1510' && !$self->level_graph() && length($data) != $self->{vertices_offset} + $self->{edges_offset} + $self->{level_points_offset} + $self->{header}->{level_point_count} * 0x14) {
			$self->error_handler($data);
			return;
		}
		$self->{raw_edges} = substr($data, $self->{vertices_offset} + $self->{edges_offset}, $self->{level_points_offset});
		if ($self->{gg_version} eq 'cop') {
			$self->{raw_level_points} = substr($data, $self->{vertices_offset} + $self->{edges_offset} + $self->{level_points_offset}, $self->{cross_tables_offset});
			$self->{raw_cross_tables_all} = substr($data, $self->{vertices_offset} + $self->{edges_offset} + $self->{level_points_offset} + $self->{cross_tables_offset});
			$self->read_ct_offsets($self->{raw_cross_tables_all});
			foreach my $level (@{$self->{levels}}) {
				my $curr_offset = $self->{vertices_offset} + $self->{edges_offset} + $self->{level_points_offset} + $self->{cross_tables_offset} + $self->{ct_offset}{$level->{level_name}};
				if ($curr_offset + $self->{ct_size}{$level->{level_name}} + 0x4 < length($self->{raw_cross_tables_all})) {
					$self->{raw_cross_tables}{$level->{level_name}} = substr($self->{raw_cross_tables_all}, $self->{ct_offset}{$level->{level_name}} - $curr_offset, $self->{ct_size}{$level->{level_name}})
				} else {
					$self->{raw_cross_tables}{$level->{level_name}} = substr($self->{raw_cross_tables_all}, $self->{ct_offset}{$level->{level_name}} - $curr_offset)
				}
			}
		} else {
			$self->{raw_level_points} = substr($data, $self->{vertices_offset} + $self->{edges_offset} + $self->{level_points_offset});
		}
	}
}
sub read_header {
	my $self = shift;
	my ($data) = @_;
	print "	reading header...\n";
	$self->{header} = gg_header->new();
	$self->{header}->read(stkutils::data_packet->new($data), $self->{gg_version});
	$self->{edges_offset} = $self->{header}->{vertex_count} * stkutils::graph::vertex_block_size($self->{gg_version});
	if (not ($self->{gg_version} eq '1469' or $self->{gg_version} eq '1472')) {
		$self->{level_points_offset} = $self->{header}->{edge_count} * stkutils::graph::edge_block_size($self->{gg_version});
	}
	if ($self->{gg_version} eq 'cop') {
		$self->{cross_tables_offset} = $self->{header}->{level_point_count} * 0x14;
	}
}
sub read_levels {
	my $self = shift;
	my ($data) = @_;
	print "	reading levels...\n";	
	my $packet = stkutils::data_packet->new(substr($data, stkutils::graph::header_size($self->{gg_version})));
	for (my $i = 0; $i < $self->{header}->{level_count}; $i++) {
		my $level = gg_level->new();			
		$level->read($packet, $self->{gg_version});
		push @{$self->{levels}}, $level;
	}	
	foreach my $level (@{$self->{levels}}) {
		$self->{level_by_id}{$level->{level_id}} = \$level;
	}
	$self->{vertices_offset} = 0x1000 - $packet->length();
}
sub read_vertices {
	my $self = shift;
	my ($data) = @_;
	print "	reading vertices...\n";	
	my $packet = stkutils::data_packet->new($data);
	for (my $i = 0; $i < $self->{header}->{vertex_count}; $i++) {
		my $vertex = gg_vertex->new();
		if ($self->{gg_version} eq '1469' or $self->{gg_version} eq '1472') {
			$vertex->read($packet, $self->{edges_offset}, $self->{gg_version});
		} else {
			$vertex->read($packet, $self->{edges_offset}, $self->{level_points_offset}, $self->{gg_version});
		}
		my $level_name = $self->{level_by_id}{$vertex->{level_id}};
		push @{$self->{vertices}}, $vertex;
	}
	my $game_vertex_id = 0;
	my $level_id = -1;
	foreach my $vertex (@{$self->{vertices}}) {
		if ($vertex->{level_id} != $level_id) {
			my $level = $self->{level_by_id}{$vertex->{level_id}};
			$level_id = $vertex->{level_id};
			$self->{level_by_guid}{$game_vertex_id} = $$level->{level_name};
		}
		$game_vertex_id++;
	}
}
sub read_edges {
	my $self = shift;
	my ($data) = @_;
	print "	reading edges...\n";	
	my $packet = stkutils::data_packet->new($data);
	my $edge_count = 0;
	if ($self->{gg_version} eq '1469' || $self->{gg_version} eq '1472' || $self->level_graph()) {
		$edge_count = ($packet->length())/stkutils::graph::edge_block_size($self->{gg_version});
	} else {
		$edge_count = $self->{header}->{edge_count};
	}
	for (my $i = 0; $i < $edge_count; $i++) {
		my $edge = gg_edge->new();
		$edge->read($packet, $self->{gg_version});
		push @{$self->{edges}}, $edge;
	}	
}
sub read_level_points {
	my $self = shift;
	my ($data) = @_;
	my $packet = stkutils::data_packet->new($data);
	print "	reading level points...\n";	
	for (my $i = 0; $i < $self->{header}->{level_point_count}; $i++) {
		my $level_point = gg_level_point->new();
		$level_point->read($packet);
		push @{$self->{level_points}}, $level_point;
	}
}
sub read_lp_offsets {
	my $self = shift;
	$self->{lp_offsets} = {};
	$self->{lp_counts} = {};
	foreach my $vertex (@{$self->{vertices}}) {
		my $level = $self->{level_by_id}{$vertex->{level_id}};
		if (!defined $self->{lp_offsets}{$$level->{level_name}}) {
			$self->{lp_offsets}{$$level->{level_name}} = $vertex->{level_point_offset};    ####оффсеты идут с начала файла!
			$self->{lp_counts}{$$level->{level_name}} = $vertex->{level_point_count};
			my $var = $self->{offset_for_ct} + $self->{lp_offsets}{$$level->{level_name}};
		} else {
			$self->{lp_counts}{$$level->{level_name}} += $vertex->{level_point_count};
		}
	}
}
sub read_cross_table {
	my $self = shift;
	my ($data, $level_name) = @_;
	print "	reading cross table of level $level_name...\n";	
	my $packet = stkutils::data_packet->new($data);
	my $cross_table = gg_cross_table->new();
	$cross_table->read($packet);
	$cross_table->{level_name} = $level_name;
	for (my $j = 0; $j < $cross_table->{cell_count}; $j++) {
		($cross_table->{cells}->{$j}{graph_id},
		$cross_table->{cells}->{$j}{distance}) = $packet->unpack('vf', 6);
	}
	push @{$self->{cross_tables}}, $cross_table;
}
sub read_ct_offsets {
	my $self = shift;
	my ($data) = @_;
	my $offset = 0;
	foreach my $level (@{$self->{levels}}) {
		my $packet = stkutils::data_packet->new(substr ($data, 0, 0x04));
		($self->{ct_size}{$level->{level_name}}) = $packet->unpack('V', 4);
		$self->{ct_offset}{$level->{level_name}} = $offset;   		####оффсеты идут с начала файла!
		$offset += $self->{ct_size}{$level->{level_name}};
		if (length($data) > 0x4 + $self->{ct_size}{$level->{level_name}}) {
			$data = substr($data, 0x4 + $self->{ct_size}{$level->{level_name}});
		}
	}		
}
sub compose_graph {
	my $self = shift;
	my $h = $self->write_header();
	my $l = $self->write_levels();
	my $hlve = $h.$l.$self->{raw_vertices}.$self->{raw_edges};
	if (($self->{gg_version} ne '1469') && ($self->{gg_version} ne '1472')) {
		$hlve .= $self->{raw_level_points};
		if ($self->{gg_version} eq 'cop') {
			foreach my $level (@{$self->{levels}}) {
				$self->{raw_cross_tables_all} .= $self->{raw_cross_tables}{$level->{level_name}};
			}			
			$hlve .= $self->{raw_cross_tables_all};
		}
	}
	return \$hlve;
}
sub write_header {
	my $self = shift;
	print "	writing header...\n";
	my $packet = stkutils::data_packet->new();
	$self->{header}->write($packet, $self->{gg_version});
	return $packet->data();
}
sub write_levels {
	my $self = shift;
	print "	writing levels...\n";
	my $packet = stkutils::data_packet->new();
	foreach my $level (@{$self->{levels}}) {
		$level->write($packet, $self->{gg_version});
	}
	return $packet->data();
}
sub write_vertices {
	my $self = shift;
	print "	writing vertices...\n";
	my $packet = stkutils::data_packet->new();
	foreach my $vertex (@{$self->{vertices}}) {
		$vertex->write($packet, $self->{edges_offset}, $self->{level_points_offset}, $self->{gg_version});
	}
	$self->{raw_vertices} = $packet->data();
	my $v = $packet->length();
	print "$v";
}
sub write_edges {
	my $self = shift;
	print "	writing edges...\n";
	my $packet = stkutils::data_packet->new();
	foreach my $edge (@{$self->{edges}}) {
		$edge->write($packet, $self->{gg_version});
	}
	$self->{raw_edges} = $packet->data();
	my $s = $packet->length();
	print "$s\n";
}
sub write_level_points {
	my $self = shift;
	print "	writing level points...\n";
	my $packet = stkutils::data_packet->new();
	foreach my $lpoint (@{$self->{level_points}}) {
		$lpoint->write($packet, $self->{gg_version});
	}
	$self->{raw_level_points} = $packet->data();	
}
sub write_cross_table {
	my $self = shift;
	my ($level_name) = @_;
	print "	writing cross table for level $level_name...\n";
	my $packet = stkutils::data_packet->new();
	foreach my $ct (@{$self->{cross_tables}}) {
		if ($ct->{level_name} eq $level_name) {
			$ct->write($packet, $self->{gg_version});
			for (my $j = 0; $j < $ct->{cell_count}; $j++) {
				$packet->pack('vf', $ct->{cells}->{$j}{graph_id}, $ct->{cells}->{$j}{distance});
			}
		}
	}
	my $data = $packet->data();	
	return \$data;	
}
sub export_header {
	my $self = shift;
	my ($fh) = @_;
	print "	exporting header...\n";
	print $fh "[header]\n";
	my $lg = $self->level_graph();
	$self->{header}->export($fh, $lg);
}
sub export_levels {	
	my $self = shift;
	my ($fh) = @_;
	print "	exporting levels...\n";
	my $i = 0;
	my $lg = $self->level_graph();
	foreach my $level (@{$self->{levels}}) {
		print $fh "[level_$i]\n";
		$level->export($fh, $lg);
		$i++;
	}
}
sub export_vertices {	
	my $self = shift;
	my ($fh) = @_;
	my $i = 0;
	my $lg = $self->level_graph();
	print "	exporting vertices...\n";
	foreach my $vertex (@{$self->{vertices}}) {
		print $fh "[vertex_$i]\n";
		$vertex->export($fh, $lg);
		$i++;
	}
}
sub export_edges {	
	my $self = shift;
	my ($fh) = @_;
	my $i = 0;
	print "	exporting edges...\n";
	foreach my $edge (@{$self->{edges}}) {
		print $fh "[edge_$i]\n";
		$edge->export($fh);
		$i++;
	}
}
sub export_level_points {	
	my $self = shift;
	my ($fh) = @_;
	my $i = 0;
	print "	exporting level points...\n";
	foreach my $level_point (@{$self->{level_points}}) {
		print $fh "[level_point_$i]\n";
		$level_point->export($fh);
		$i++;
	}
}
sub export_cross_tables {	
	my $self = shift;
	my ($fh) = @_;
	my $i = 0;
	print "	exporting cross tables...\n";
	foreach my $cross_table (@{$self->{cross_tables}}) {
		print $fh "[cross_table_$i]\n";
		$cross_table->export($fh);
		print $fh "level_name = $cross_table->{level_name}\n";
		for (my $j = 0; $j < $cross_table->{cell_count}; $j++) {
			my $graph_id = $cross_table->{cells}->{$j}{graph_id};
			my $distance = $cross_table->{cells}->{$j}{distance};
			print $fh "node$j = $graph_id, $distance\n";
		}			
		$i++;
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
sub level_graph {
	return $_[0]->{is_level_graph} == 1;
}
sub error_handler {
	my $self = shift;
	my ($data) = @_;
	print "error occured! trying to fix...\n";
	$self->{gg_version} = '1935';
	$self->decompose_graph($data);
}
1;
#####################################################################