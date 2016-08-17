package gg_header;
use strict;
use constant properties_info => (
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
	$_[1]->unpack_properties($_[0], properties_info);
}
sub write {
	$_[1]->pack_properties($_[0], properties_info);
}
sub import {
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub export {
	$_[1]->export_properties(undef, $_[0], properties_info);
}
sub export2 {
	my $self = shift;
	my ($fh) = @_;

	print $fh "version = $self->{version}\n";
	print $fh "guid = $self->{guid}\n";
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
	{ name => 'level_id',		type => 'u8' },
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
sub write {
	$_[1]->pack_properties($_[0], properties_info);
}
sub import {
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub export {
	$_[1]->export_properties(undef, $_[0], properties_info);
}
sub export2 {
	my $self = shift;
	my ($fh) = @_;

	print $fh "level_name = $self->{level_name}\n";
	print $fh "offset = ", join(',', @{$self->{offset}}), "\n";
	print $fh "level_id = $self->{level_id}\n";
	print $fh "section_name = $self->{section_name}\n";
	print $fh "guid = $self->{guid}\n\n";
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
	$_[1]->unpack_properties($_[0], properties_info);
	
	$_[0]->{edge_index} = ($_[0]->{edge_offset} - $_[2]) / 0x06;
	if ($_[0]->{level_point_count} > 0) {
		$_[0]->{level_point_index} = ($_[0]->{level_point_offset} - $_[3]) / 0x14;
	} else {
		$_[0]->{level_point_index} = 0;
	}
}
sub write {
	$_[0]->{edge_offset} = $_[2] + ($_[0]->{edge_index}*0x06);
	
	if ($_[0]->{level_point_count} > 0) {
		$_[0]->{level_point_offset} = $_[3] + $_[0]->{level_point_index}*0x14;
	} else {
		$_[0]->{level_point_offset} = 0;
	}
	
	$_[1]->pack_properties($_[0], properties_info);
}
sub import {
	$_[1]->import_properties($_[2], $_[0], properties_info);
	
	$_[0]->{edge_index} = ($_[0]->{edge_offset} - $_[3]) / 0x06;
	if ($_[0]->{level_point_count} > 0) {
		$_[0]->{level_point_index} = ($_[0]->{level_point_offset} - $_[4]) / 0x14;
	} else {
		$_[0]->{level_point_index} = 0;
	}
}
sub export {
	#$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
	$_[0]->{edge_offset} = $_[3] + ($_[0]->{edge_index}*0x06);
	
	if ($_[0]->{level_point_count} > 0) {
		$_[0]->{level_point_offset} = $_[4] + $_[0]->{level_point_index}*0x14;
	} else {
		$_[0]->{level_point_offset} = 0;
	}
	
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}

sub export2 {
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
	$_[1]->unpack_properties($_[0], properties_info);
}
sub write {
	$_[1]->pack_properties($_[0], properties_info);
}
sub import {
	my $self = $_[0];
	$_[1]->import_properties($_[2], $_[0], properties_info);
	$self->{target_level_id} = $_[1]->value($_[2], "target_level_id");
	$self->{target_level_vertex_id} = $_[1]->value($_[2], "target_level_vertex_id");
	$self->{parent_level_vertex_id} = $_[1]->value($_[2], "parent_level_vertex_id");
	#print " gvid_id = " . $self->{game_vertex_id} . "\n";
	#print " lvid_id = " . $self->{target_level_vertex_id} . "\n";
}
sub export {
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}
sub export2 {
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
sub import {
	$_[1]->import_properties($_[2], $_[0], properties_info);
	$_[0]->{parent_level_vertex_id} = $_[1]->value($_[2], "parent_level_vertex_id");
}
sub export {
	$_[1]->export_properties(__PACKAGE__, $_[0], properties_info);
}
sub export2 {
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
	{ name => 'version',		type => 'u32' },
	{ name => 'cell_count',		type => 'u32' },
	{ name => 'vertex_count',	type => 'u32' },
	{ name => 'level_guid',		type => 'guid' },
	{ name => 'game_guid',		type => 'guid' },
);
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	$_[1]->unpack_properties($_[0], properties_info);
	die unless $_[1]->length() == $_[0]->{cell_count}*0x06;
	@{$_[0]->{cells}} = $_[1]->unpack("(vf)$_[0]->{cell_count}");
}
sub export {
	my $self = shift;
	my ($fh) = @_;

	print $fh "version = $self->{version}\n";
	print $fh "cell_count = $self->{cell_count}\n";
	print $fh "vertex_count = $self->{vertex_count}\n";
	for (my $i = 0; 0 && $i < $self->{cell_count}; $i++) {
		my $graph_id = $self->{cells}[$i*2 + 0];
		my $distance = $self->{cells}[$i*2 + 1];
		print $fh "node$i = $graph_id, $distance\n";
	}
	print $fh "\n";
}
sub read_bin {
	my $self = shift;
	my ($data) = @_;
	$self->bin_data = $data;
}
sub write_bin {
	my $self = shift;
	my ($fh) = @_;
	$fh->write($self->bin_data);
}

#######################################################################




package stkutils::game_graph;
use strict;
use IO::File;
use stkutils::data_packet;
use stkutils::ini_file;

sub new {
	my $class = shift;
	my ($fn, $mode) = @_;
	my $self = {};
	bless($self, $class);
	$self->{fn} = $fn;
	$self->{mode} = $mode;
	if (defined $mode)
	{
		my $fh = IO::File->new($fn, $self->{mode}) or die "cannot open $fn\n";
		binmode $fh;
		$self->{fh} = $fh;
	}
	return $self;
}

sub write {
	my $self = shift;
	my ($fn) = @_;
	my $data;
	my $packet;
	
	my $fh = IO::File->new($fn, 'w') or die "cannot open $fn\n";
	binmode $fh;
	
	my $edges_offset = $self->{header}->{vertex_count} * 0x2a;
	my $level_points_offset = $edges_offset + $self->{header}->{edge_count} * 0x06;
	
	print "writing: header       ... ";
	$packet = stkutils::data_packet->new();
	$self->{header}->write($packet);
	$fh->write($packet->data(), $packet->length()) or die "cannot write file $fn\n";
	print "done\n";
	
	print "writing: levels       ... ";
	foreach my $level (@{$self->{levels}}) {
		$packet = stkutils::data_packet->new();
		$level->write($packet);
		$fh->write($packet->data(), $packet->length()) or die "cannot write file $fn\n";
	}
	print "done\n";
	
	print "writing: vertices     ... ";
	foreach my $vertex (@{$self->{vertices}}) {
		$packet = stkutils::data_packet->new();
		$vertex->write($packet, $edges_offset, $level_points_offset);
		#$vertex->write($packet);
		$fh->write($packet->data(), $packet->length()) or die "cannot write file $fn\n";
	}
	print "done\n";
	
	print "writing: edges        ... ";
	if ($main::xedges_write!=1) {
		foreach my $edge (@{$self->{edges}}) {
			$packet = stkutils::data_packet->new();
			$edge->write($packet);
			$fh->write($packet->data(), $packet->length()) or die "cannot write file $fn\n";
		}
	}
	else {
		print "alternative way ... ";
		#alternative experimental way to write edges
		foreach my $vertex (@{$self->{vertices}}) {
			foreach my $edge (@{$self->{edges}}) {
				if ($edge->{level_id} == $vertex->{level_id}) {
					if ($edge->{parent_level_vertex_id} == $vertex->{level_vertex_id}) {
						$packet = stkutils::data_packet->new();
						$edge->write($packet);
						$fh->write($packet->data(), $packet->length()) or die "cannot write file $fn\n";
					}
				}
			}
		}
	}
	print "done\n";
	
	print "writing: level_points ... ";
	if ($main::bin_level_points == 1)
	{
		print "binary ...";
		$fh->write($self->{level_points_bin}, $self->{header}->{level_point_count}*0x14);
	}
	else
	{
		foreach my $level_point (@{$self->{level_points}}) {
			$packet = stkutils::data_packet->new();
			$level_point->write($packet);
			$fh->write($packet->data(), $packet->length()) or die "cannot write file $fn\n";
		}
	}
	print "done\n";
	
	print "writing: cross_tables ... ";
	if ($self->{header}->{level_point_count} != 0) {
		# this is a game.graph, so handle embedded cross tables
		
		if (!(defined $self->{cross_tables}) or ((scalar @{$self->{cross_tables}}) < 1))
		{
			for (my $i = 0; $i < $self->{header}->{level_count}; $i++) {
				$fh->write(pack("V", 0x04));
			}
		}
		else
		{
			foreach my $cross_table (@{$self->{cross_tables}}) {
			  $fh->write(pack("V", ($cross_table->{size} + 0x04)));
			  $fh->write($cross_table->{bin_data}, ($cross_table->{size} + 0x04)) or die "cannot write file $fn\n";
				#$packet = stkutils::data_packet->new();
				#$cross_table->write($packet);
				#$fh->write(pack("V", ($packet->length() + 0x04)));
				#$fh->write($packet->data(), $packet->length()) or die "cannot write file $fn\n";
			}
		}
	}
	print "done\n";
}

sub read_header {
	my $self = shift;
	my $fh = $self->{fh};
	my $data;
	
	print "reading: header        ... ";
	$fh->read($data, 0x1c) or die;
	$self->{header} = gg_header->new();
	$self->{header}->read(stkutils::data_packet->new($data));
	
	$self->{edges_offset} = $self->{header}->{vertex_count} * 0x2a;
	$self->{level_points_offset} = $self->{edges_offset} + $self->{header}->{edge_count} * 0x06;
	
	print "done\n";
}

sub read_levels {
	my $self = shift;
	my $fh = $self->{fh};
	my $data;
	
	print "reading: levels        ... ";
	# 4KB should be enough for the level
	$self->{levelId_by_name} = {};
	$fh->read($data, 0x4000) or die;
	my $packet = stkutils::data_packet->new($data);
	for (my $i = 0; $i < $self->{header}->{level_count}; $i++) {
		my $level = gg_level->new();
		$level->read($packet);
		$self->{levelId_by_name}{$level->{level_name}} = $level->{level_id};
		push @{$self->{levels}}, $level;
	}
	$fh->seek(-$packet->length(), SEEK_CUR);
	print "done\n";
}

sub read_vertices {
	my $self = shift;
	my $fh = $self->{fh};
	my $data;
	
	print "reading: vertices      ... ";
	my $edges_offset = $self->{edges_offset};
	my $level_points_offset = $self->{level_points_offset};
	for (my $i = 0; $i < $self->{header}->{vertex_count}; $i++) {
		$fh->read($data, 0x2a) or die;
		my $vertex = gg_vertex->new();
		$vertex->read(stkutils::data_packet->new($data), $edges_offset, $level_points_offset);
		push @{$self->{vertices}}, $vertex;
	}
	print "done\n";
}

sub read {
	my $self = shift;
	my ($fn) = @_;
	my $fh;
	if (defined $fn)
	{
		$self->{fn} = $fn;
		$self->{mode} = 'r';
		$fh = IO::File->new($fn, $self->{mode}) or die "cannot open $fn\n";
		binmode $fh;
		$self->{fh} = $fh;
	}
	elsif (not defined $fn)
	{
		$fn = $self->{fn};
		$fh = $self->{fh};
	}
	die "shit..." unless defined $fn;
	my $data;
	my $i;
	

	
	$self->read_header();
	
	$self->read_levels();
	
	$self->read_vertices();
	
	print "reading: edges         ... ";
	for (my $i = 0; $i < $self->{header}->{edge_count}; $i++) {
		$fh->read($data, 0x06) or die;
		my $edge = gg_edge->new();
		$edge->read(stkutils::data_packet->new($data));
		push @{$self->{edges}}, $edge;
	}
	print "done\n";
	print "reading: level_points  ... ";
	if ($main::bin_level_points == 1)
	{
		print "binary ...";
		$fh->read($self->{level_points_bin}, $self->{header}->{level_point_count}*0x14);
	}
	elsif ($main::bin_level_points == 2)
	{
    print "binary2 ...";
		for ($i = 0; $i < $self->{header}->{level_point_count}; $i++) {
			$fh->read($data, 0x14) or die;
			my $level_point = gg_level_point->new();
			$level_point->read(stkutils::data_packet->new($data));
			push @{$self->{level_points}}, $level_point;
		}
	}
	else
	{
		for ($i = 0; $i < $self->{header}->{level_point_count}; $i++) {
			$fh->read($data, 0x14) or die;
			my $level_point = gg_level_point->new();
			$level_point->read(stkutils::data_packet->new($data));
			push @{$self->{level_points}}, $level_point;
		}
	}
	print "done\n";
	print "reading: cross_tables (bin)  ... ";
# !!! in this version cross_tables read and write only as binary data
	if ($self->{header}->{level_point_count} != 0) {
#		for (my $i = 0; $i < $self->{header}->{level_count}; $i++) {
		foreach my $level (@{$self->{levels}}) {
			my $level_name = $level->{level_name};
			#my $fh_ct = IO::File->new($level_name. ".level_ai.bin", 'w') or die "cannot create $level_name.level_ai.bin\n";
			#binmode $fh_ct;
			
			$fh->read($data, 0x04) or die;
			my ($size) = unpack('V', $data);
			#printf "size=%d(%x)\n", $size, $size;
			if ($size > 4) {
				my $cross_table = gg_cross_table->new();
				$cross_table->{size} = $size;
				$cross_table->{level_name} = $level_name;
				$fh->read($data, $size - 4);
				$cross_table->{bin_data} = $data;
				#$cross_table->read(stkutils::data_packet->new($data));
				push @{$self->{cross_tables}}, $cross_table;
			}
			#$fh_ct->close();
		}
	}
	print "done\n";
}


sub print_diff {
	my $prev_val = $_[0];
	my $new_val = $_[1];
	my $diff = $new_val - $prev_val;
	if ($diff != 0)
	{
		print " (";
		if ($diff > 0) {
			print "+";
		}
		print $diff;
		print ")";
	}
}

sub update_header_info {
	my $self = shift;
	my ($fn) = @_;
	my $if;
	my $prev_val;
	my $diff;
	
	my $padding = "    ";
	#print $padding;
	print "importing: update header:\n";
	
	print $padding;
	print "cur:level_count      =" .$self->{header}->{level_count}. "\n";
	$prev_val = $self->{header}->{level_count};
	$if = stkutils::ini_file->new($fn. ".levels.ltx", 'r') or die "cannot open " .$fn. ".levels.ltx\n";
	$self->{header}->{level_count} = scalar @{$if->{sections_list}};
	print $padding;
	print "upd:level_count      =" .$self->{header}->{level_count};
	print_diff($prev_val, $self->{header}->{level_count});
	print "\n";
	$if->close();
	
	print $padding;
	print "cur:vertex_count     =" .$self->{header}->{vertex_count}. "\n";
	$prev_val = $self->{header}->{vertex_count};
	$self->{header}->{vertex_count} = 0;
	foreach my $level (@{$self->{levels}}) {
		my $level_name = $level->{level_name};
		$if = stkutils::ini_file->new($fn. ".vertices." .$level_name. ".ltx", 'r') or die "cannot open " .$fn. ".vertices." .$level_name. ".ltx\n";
		$self->{header}->{vertex_count} += scalar @{$if->{sections_list}};
		$if->close();
	}
	print $padding;
	print "upd:vertex_count     =" .$self->{header}->{vertex_count};
	print_diff($prev_val, $self->{header}->{vertex_count});
	print "\n";
	
	print $padding;
	print "cur:edge_count       =" .$self->{header}->{edge_count}. "\n";
	$prev_val = $self->{header}->{edge_count};
	$self->{header}->{edge_count} = 0;
	foreach my $level (@{$self->{levels}}) {
		my $level_name = $level->{level_name};
		$if = stkutils::ini_file->new($fn. ".edges." .$level_name. ".ltx", 'r') or die "cannot open " .$fn. ".vertices." .$level_name. ".ltx\n";
		$self->{header}->{edge_count} += scalar @{$if->{sections_list}};
		$if->close();
	}
	print $padding;
	print "upd:edge_count       =" .$self->{header}->{edge_count};
	print_diff($prev_val, $self->{header}->{edge_count});
	print "\n";
	
	print $padding;
	print "cur:level_point_count=" .$self->{header}->{level_point_count}. "\n";
	
	if ($main::bin_level_points == 1)
	{
		#skip update of level points
		my $new_val = -s $fn. ".level_points.bin";
		$new_val = $new_val / 0x14;
		print $padding;
		print "???:level_point_count=" .$self->{header}->{level_point_count};
		print_diff($self->{header}->{level_point_count}, $new_val);
		print " ...assumed on file size... not used in bin mode";
		print "\n";
	}
	else
	{
		$prev_val = $self->{header}->{level_point_count};
		$self->{header}->{level_point_count} = 0;
		foreach my $level (@{$self->{levels}}) {
			my $level_name = $level->{level_name};
			$if = stkutils::ini_file->new($fn. ".level_points." .$level_name. ".ltx", 'r') or die "cannot open " .$fn. ".vertices." .$level_name. ".ltx\n";
			$self->{header}->{level_point_count} += scalar @{$if->{sections_list}};
			$if->close();
		}
		print $padding;
		print "upd:level_point_count=" .$self->{header}->{level_point_count};
		print_diff($prev_val, $self->{header}->{level_point_count});
		print "\n";
	}
	
}

sub _import {
	my $self = shift;
	my ($fn) = @_;
	my $if;
	my $i=0;
	
	my $edges_offset;
	my $level_points_offset;
	my $prev_vertex;
	
	print "importing: header      ... ";
	$if = stkutils::ini_file->new($fn. ".header.ltx", 'r') or die "cannot open " .$fn. ".header.ltx\n";
	$self->{header} = gg_header->new();
	$self->{header}->import($if, "header");
	$if->close();
	print "done\n";
	
	$edges_offset = $self->{header}->{vertex_count} * 0x2a;
	#print "edges_offset1=$edges_offset\n";
	
	print "importing: levels      ... ";
	$self->{levelId_by_name} = {};
	$if = stkutils::ini_file->new($fn. ".levels.ltx", 'r') or die "cannot open " .$fn. ".levels.ltx\n";
	foreach my $section (@{$if->{sections_list}}) {
		my $level = gg_level->new();
		$level->import($if, $section);
		$self->{levelId_by_name}{$level->{level_name}} = $level->{level_id};
		push @{$self->{levels}}, $level;
	}
	$if->close();
	print "done\n";
	
	#updating header info
	$self->update_header_info($fn);
	
	$edges_offset = $self->{header}->{vertex_count} * 0x2a;
	#print "edges_offset2=$edges_offset\n";
	$level_points_offset = $edges_offset + $self->{header}->{edge_count} * 0x06;
	
	print "importing: vertices    ... ";
	foreach my $level (@{$self->{levels}}) {
		my $level_name = $level->{level_name};
		my $level_id = $level->{level_id};
		$if = stkutils::ini_file->new($fn. ".vertices." .$level_name. ".ltx", 'r') or die "cannot open " .$fn. ".vertices." .$level_name. ".ltx\n";
		foreach my $section (@{$if->{sections_list}}) {
			my $vertex = gg_vertex->new();
			$vertex->import($if, $section, $edges_offset, $level_points_offset, $level_id);
			$vertex->{level_id} = $level_id;
			push @{$self->{vertices}}, $vertex;
		}
		$if->close();
	}
	print "done\n";
	#print "checking : vertices    ... ";
	#$self->check_vertexes();
	#print "done\n";
	
	print "importing: edges       ... ";
	foreach my $vertex (@{$self->{vertices}}) {
		$vertex->{edge_count} = 0;
		$vertex->{edge_offset} = -1;
		$vertex->{edge_index} = -1;
	}
	$i=0;
	$prev_vertex = -1;
	foreach my $level (@{$self->{levels}}) {
		my $level_name = $level->{level_name};
		$if = stkutils::ini_file->new($fn. ".edges." .$level_name. ".ltx", 'r') or die "cannot open " .$fn. ".edges." .$level_name. ".ltx\n";
		foreach my $section (@{$if->{sections_list}}) {
			my $edge = gg_edge->new();
			$edge->import($if, $section);
			
			$edge->{level_id} = $level->{level_id};
			if (!defined $edge->{target_level_id})
			{
				$edge->{target_level_id} = $edge->{level_id};
				#print "x:target_level_id0=" .$edge->{target_level_id}."\n";
			}
			else
			{
				#print "x:target_level_id1=" .$edge->{target_level_id}."\n";
				$edge->{target_level_id} = $self->{levelId_by_name}{$edge->{target_level_id}};
			}
			#print "x:target_level_id2=" .$edge->{target_level_id}."\n";
			
			#update game_vertex_id
			my $gvid = 0;
			$edge->{game_vertex_id} = -1;
			foreach my $vertex (@{$self->{vertices}}) {
				if ($edge->{target_level_id} == $vertex->{level_id})
				{
					if ($vertex->{level_vertex_id} == $edge->{target_level_vertex_id})
					{
						$edge->{game_vertex_id} = $gvid;
						last;
					}
				}
				$gvid++;
			}
			
			#if target not found..... skip???
			if ($edge->{game_vertex_id} == -1)
			{
				print "\n   edge '" . $section. "': target vertex not found... creating loopback\n";
				die;
				$edge->{game_vertex_id} = $edge->{parent_game_vertex_id};
				$edge->{target_level_vertex_id} = $edge->{parent_level_vertex_id};
			}
			
			
			
			#else
			#{
				push @{$self->{edges}}, $edge;
			#}
			$i++;
		}
		$if->close();
	}
	print "done\n";
	
	#print "updating : vertices    ... ";
	print "updating : edge-vertex ... ";
	$i=0;
	foreach my $vertex (@{$self->{vertices}}) {
		foreach my $edge (@{$self->{edges}}) {
			if ($edge->{level_id} == $vertex->{level_id}) {
				if ($edge->{parent_level_vertex_id} == $vertex->{level_vertex_id}) {
					
					if ($vertex->{edge_index} == -1) {
						$vertex->{edge_index} = $i;
						$vertex->{edge_offset} = $edges_offset + $i*0x06;
					}
					$vertex->{edge_count}=$vertex->{edge_count} + 1;
					
					$i++;
				}
			}
		}
	}
	print "done\n";
	
	print "importing: level_points ... ";
	if ($main::bin_level_points == 1)
	{
		print "binary ...";
		my $fh2 = IO::File->new($fn. ".level_points.bin", 'r') or die "cannot open $fn.level_points.bin\n";
		binmode $fh2;
		$fh2->read($self->{level_points_bin}, $self->{header}->{level_point_count}*0x14);
	}
	elsif ($main::bin_level_points == 2)
	{
		$i=0;
		$prev_vertex = -1;
		foreach my $level (@{$self->{levels}}) {
			my $level_name = $level->{level_name};
			$if = IO::File->new($fn. ".level_points." .$level_name. ".bin", 'r') or die "cannot open " .$fn. ".level_points." .$level_name. ".bin\n";
			my $filesize = -s $fn. ".level_points." .$level_name. ".bin";
			my $data;
			for (my $i=0; $i<$filesize; $i+=0x14)
			{
  			$if->read($data, 0x14) or die;
  			my $level_point = gg_level_point->new();
  			$level_point->read(stkutils::data_packet->new($data));
  			
  			if (99 == 1)
  			{
  				if (($prev_vertex != -1) and ($prev_vertex->{level_vertex_id} == $level_point->{parent_level_vertex_id}))
  				{
  					#cont inside same vertex lps
  					$prev_vertex->{level_point_count}++;
  				}
  				else
  				{
  					#find new vertex
  					foreach my $vertex (@{$self->{vertices}}) {
  						if ($level->{level_id} == $vertex->{level_id})
  						{
  							if ($vertex->{level_vertex_id} == $level_point->{parent_level_vertex_id})
  							{
  								$prev_vertex = $vertex;
  								$vertex->{level_point_index} = $i;
  								$vertex->{level_point_count}=1;
  								last;
  							}
  						}
  					}
  				}
				}
				
				push @{$self->{level_points}}, $level_point;
				$i++;
			}
			$if->close();
		}
	}
	else
	{
		$i=0;
		$prev_vertex = -1;
		foreach my $level (@{$self->{levels}}) {
			my $level_name = $level->{level_name};
			$if = stkutils::ini_file->new($fn. ".level_points." .$level_name. ".ltx", 'r') or die "cannot open " .$fn. ".edges." .$level_name. ".ltx\n";
			foreach my $section (@{$if->{sections_list}}) {
				my $level_point = gg_level_point->new();
				$level_point->import($if, $section);
				
				if (($prev_vertex != -1) and ($prev_vertex->{level_vertex_id} == $level_point->{parent_level_vertex_id}))
				{
					#cont inside same vertex lps
					$prev_vertex->{level_point_count}++;
				}
				else
				{
					#find new vertex
					foreach my $vertex (@{$self->{vertices}}) {
						if ($level->{level_id} == $vertex->{level_id})
						{
							if ($vertex->{level_vertex_id} == $level_point->{parent_level_vertex_id})
							{
								$prev_vertex = $vertex;
								$vertex->{level_point_index} = $i;
								$vertex->{level_point_count}=1;
								last;
							}
						}
					}
				}
				
				push @{$self->{level_points}}, $level_point;
				$i++;
			}
			$if->close();
		}
	}
	print "done\n";
	
	#update offsets
	foreach my $vertex (@{$self->{vertices}}) {
		
		$vertex->{edge_offset} = $edges_offset + ($vertex->{edge_index}*0x06);
		
		if ($vertex->{level_point_count} > 0) {
			$vertex->{level_point_offset} = $level_points_offset + ($vertex->{level_point_index}* 0x14);
		} else {
			$vertex->{level_point_index} = 0;
			$vertex->{level_point_offset} = 0;
		}
	}
	
	print "importing: cross_tables      ... ";
	foreach my $level (@{$self->{levels}}) {
		my $level_name = $level->{level_name};
		my $fn_ct = $fn. ".level_ai." .$level_name. ".bin";
		my $fh_ct = IO::File->new($fn_ct, 'r');# or die "cannot open $fn_ct\n";
		my $filesize = -s $fn_ct;
		
		if ($filesize > 0) {
  		binmode $fh_ct;
			my $cross_table = gg_cross_table->new();
			$cross_table->{size} = $filesize;
			$cross_table->{level_name} = $level_name;
			$fh_ct->read($cross_table->{bin_data}, $filesize);
			push @{$self->{cross_tables}}, $cross_table;
		}
		$fh_ct->close();
	}
	print "done\n";
	
	#die"stopped";
}

sub export {
	my $self = shift;
	my ($fn) = @_;
	
	my $if;
	my $fh;
	
	my $vid = 0;
	
	
	my %level_by_id = ();
	foreach my $level (@{$self->{levels}}) {
		$level_by_id{$level->{level_id}} = \$level;
	}
	
	my %level_by_gvid = ();
	my $game_vertex_id = 0;
	my $level_id = -1;
	foreach my $vertex (@{$self->{vertices}}) {
		my $level = $level_by_id{$vertex->{level_id}};
		$level_by_gvid{$game_vertex_id} = \$level;
		$game_vertex_id++;
	}
	
	my $edges_offset = $self->{header}->{vertex_count} * 0x2a;
	my $level_points_offset = $edges_offset + $self->{header}->{edge_count} * 0x06;
	
	print "exporting: header       ... ";
	$if = stkutils::ini_file->new($fn. ".header.ltx", 'w') or die "cannot write ". $fn. ".header.ltx\n";
	$fh = $if->{fh};
	print $fh "[header]\n";
	$self->{header}->export($if, "header");
	$if->close();
	print "done\n";
	
	print "exporting: levels       ... ";
	$if = stkutils::ini_file->new($fn. ".levels.ltx", 'w') or die "cannot write ". $fn. ".levels.ltx\n";
	$fh = $if->{fh};
	my $i = 0;
	my $level_section;
	foreach my $level (@{$self->{levels}}) {
	  $level_section = "level_" .$level->{level_id};
		print $fh "[" .$level_section. "]\n";
  	$level->export($if, $level_section);
		$i++;
	}
	$if->close();
	print "done\n";
	
	print "exporting: vertices     ... ";
	foreach my $level (@{$self->{levels}}) {
		my $level_name = $level->{level_name};
		$if = stkutils::ini_file->new($fn. ".vertices." .$level_name. ".ltx", 'w') or die "cannot write ". $fn. ".vertices." .$level_name. ".ltx\n";
		$fh = $if->{fh};
		print $fh ";note:\n;level_point_offset, level_point_count, edge_offset, edge_count\n;will be ignored on import in order to represent actual updated state \n";
		$if->close();
	}
	
	$i = 0;
	foreach my $vertex (@{$self->{vertices}}) {
		my $levelx = $level_by_id{$vertex->{level_id}};
		my $level_name = $$levelx->{level_name};
		
		$if = stkutils::ini_file->new($fn. ".vertices." .$level_name. ".ltx", 'a') or die "cannot append ". $fn. ".vertices." .$level_name. ".ltx\n";
		$fh = $if->{fh};
		print $fh "[" .$level_name. "_vertex_" .$i. "]\n";
		print $fh "gvid = " .$i. " ; game_vertex_id\n";
		#$vertex->export($if, $level_name. "_vertex_" .$i);
		$vertex->export($if, $level_name. "_vertex_" .$i, $edges_offset, $level_points_offset);
		$if->close();
		$i++;
	}
	print "done\n";
	
	print "exporting: edges        ... ";
	foreach my $level (@{$self->{levels}}) {
		my $level_name = $level->{level_name};
		$if = stkutils::ini_file->new($fn. ".edges." .$level_name. ".ltx", 'w') or die "cannot write ". $fn. ".edges." .$level_name. ".ltx\n";
		$fh = $if->{fh};
		print $fh ";note:\n;target_level_vertex_id will be used instead of game_vertex_id\n";
		$if->close();
	}
	
	my $eid = 0;
	my $last_level_name = '';
	$vid = 0;
	foreach my $vertex (@{$self->{vertices}}) {
		for (my $i = 0; $i < $vertex->{edge_count}; $i++) {
			my $level_vertex_id = $vertex->{level_vertex_id};
			my $edge = $self->{edges}[$vertex->{edge_index} + $i];
			
			#print "vertex->{level_vertex_id} = " .$vertex->{level_vertex_id}."\n";
			#print "vertex->{edge_index} = " .$vertex->{edge_index}."\n";
			#print "vertex->{edge_count}:i = " .$vertex->{edge_count}.":" .$i. "\n";
			if (!defined($edge)) {die"undefined edge... export stopped";};
			#print "edge = $edge\n";
			my $vid2 = $edge->{game_vertex_id};
			#print "target gvid = $vid2\n";
			#if (defined($edge)) { print "target lvid = " .$edge->{target_level_vertex_id}. "\n"; };
			
			my $vertex2 = $self->{vertices}[$vid2];
			#print "tlvid::" .$vertex2->{level_vertex_id}. "\n";
			my $level = $level_by_id{$vertex->{level_id}};
			my $name = $$level->{level_name};
			
			$if = stkutils::ini_file->new($fn. ".edges." .$name. ".ltx", 'a') or die "cannot append ". $fn. ".edges." .$name. ".ltx\n";
			$fh = $if->{fh};
			
			my $level2 = $level_by_id{$vertex2->{level_id}};
			my $name2 = $$level2->{level_name};
			
			if ($vertex->{level_id} != $vertex2->{level_id}) {
				print $fh ";crosslevel edge: " .$name. " - " .$name2. "\n";
				
				my $if2 = stkutils::ini_file->new($fn. ".info.cross_edges.ltx", 'a') or die "cannot append ". $fn. ".info.cross_edges.ltx\n";
				my $fh2 = $if2->{fh};
				if ($last_level_name eq $name) {
				}
				else {
					print $fh2 "; === " .$name. " =============================\n";
					$last_level_name = $name;
				}
				print $fh2 "[" .$name. "_to_" .$name2. "_edge_$eid]\n";
				print $fh2 "edge_offset = " .($edges_offset+$eid*0x06). "\n";
				print $fh2 "parent_level_vertex_id = " .$level_vertex_id. "\n";
				print $fh2 "target_level_id = " .$name2. "\n";
				print $fh2 "target_level_vertex_id = " .$vertex2->{level_vertex_id}. "\n";
				$edge->export($if2, $name. "_edge_" .$eid);
				$if2->close();
			}
				print $fh "[" .$name. "_edge_$eid]\n";
				print $fh "edge_offset = " .($edges_offset+$eid*0x06). "\n";
				print $fh "parent_level_vertex_id = " .$level_vertex_id. "\n";
				#if ($vertex->{level_id} != $vertex2->{level_id}) {
					print $fh "target_level_id = " .$name2. "\n";
				#}
				print $fh "target_level_vertex_id = " .$vertex2->{level_vertex_id}. "\n";
			$edge->export($if, $name. "_edge_" .$eid);
			$if->close();
			
			$eid++;
		}
		$vid++;
	}
	print "done\n";
	
	print "exporting: level_points ... ";
	autoflush STDOUT 0;
	if ($main::bin_level_points == 1) {
		print "binary ...";
		my $fh_lp = IO::File->new($fn. ".level_points.bin", 'w') or die "cannot open $fn.level_points.bin\n";
		binmode $fh_lp;
		$fh_lp->write($self->{level_points_bin}, $self->{header}->{level_point_count}*0x14);
		$fh_lp->close();
	}
	elsif ($main::bin_level_points == 2)
	{
		foreach my $level (@{$self->{levels}}) {
			my $level_name = $level->{level_name};
			$if = stkutils::ini_file->new($fn. ".level_points." .$level_name. ".bin", 'w') or die "cannot write ". $fn. ".level_points." .$level_name. ".bin\n";
			$if->close();
		}
		
		my $lpid = 0;
		$vid = 0;
		foreach my $vertex (@{$self->{vertices}}) {
			for (my $i = 0; $i < $vertex->{level_point_count}; $i++) {
				my $level_vertex_id = $vertex->{level_vertex_id};
				my $level_point = $self->{level_points}[$vertex->{level_point_index} + $i];
				my $level = $level_by_id{$vertex->{level_id}};
				my $name = $$level->{level_name};
				
			$if = IO::File->new($fn. ".level_points." .$name. ".bin", 'a') or die "cannot append ". $fn. ".level_points." .$name. ".bin\n";
			$fh = $if;
			
			$level_point->export($if, $name. "_level_point_" .$lpid);
			$if->close();
			$lpid++;
			}
			$vid++;
		}
	}
	else {
		foreach my $level (@{$self->{levels}}) {
			my $level_name = $level->{level_name};
			$if = stkutils::ini_file->new($fn. ".level_points." .$level_name. ".ltx", 'w') or die "cannot write ". $fn. ".level_points." .$level_name. ".ltx\n";
			$if->close();
		}
		
		my $lpid = 0;
		$vid = 0;
		foreach my $vertex (@{$self->{vertices}}) {
			for (my $i = 0; $i < $vertex->{level_point_count}; $i++) {
				my $level_vertex_id = $vertex->{level_vertex_id};
				my $level_point = $self->{level_points}[$vertex->{level_point_index} + $i];
				my $level = $level_by_id{$vertex->{level_id}};
				my $name = $$level->{level_name};
				
			$if = stkutils::ini_file->new($fn. ".level_points." .$name. ".ltx", 'a') or die "cannot append ". $fn. ".level_points." .$name. ".ltx\n";
			$fh = $if->{fh};
			
				print $fh "[" .$name. "_level_point_$lpid]\n";
				print $fh ";gvid " .$vid. "\n";
				print $fh "parent_level_vertex_id = " .$level_vertex_id. "\n";
				
			$level_point->export($if, $name. "_level_point_" .$lpid);
			$if->close();
			$lpid++;
			}
			$vid++;
		}
	}
	autoflush STDOUT 1;
	print "done\n";
	
	print "exporting: cross_tables ... ";
#	autoflush STDOUT 0;
# bin mode here!
	foreach my $cross_table (@{$self->{cross_tables}}) {
		my $level_name = $cross_table->{level_name};
		my $fh_ct = IO::File->new($fn. ".level_ai." .$level_name. ".bin", 'w') or die "cannot create " .$fn. ".level_ai." .$level_name. ".bin\n";
		binmode $fh_ct;
		$fh_ct->write($cross_table->{bin_data}, $cross_table->{size});
		$fh_ct->close();
	}
#	autoflush STDOUT 1;
	print "done\n";
}

1;