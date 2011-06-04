#!perl -w -I \temp\aiwrapper_07jan2007\utils
#
# tab size:	8
#
#Last edited: 3 June 2011, ver.0.6
#######################################################################
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
		$_[1]->unpack_properties($_[0], (header_2215)[0..4]);	
		if (not(::level_graph())) {
			$_[1]->unpack_properties($_[0], (header_2215)[5]);	
		}
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
		$_[1]->pack_properties($_[0], (header_2215)[0..4]);	
		if (not(::level_graph()) && ($_[3] == 1)) {
			$_[1]->pack_properties($_[0], (header_2215)[5]);	
		}
	} else {
		$_[1]->pack_properties($_[0], header_SOC);		
	}
}
sub export {
	my $self = shift;
	my ($fh) = @_;
	print $fh "version = $self->{version}\n";
	print $fh "level_count = $self->{level_count}\n";
	print $fh "vertex_count = $self->{vertex_count}\n";
	print $fh "level_point_count = $self->{level_point_count}\n" if ((not(::level_graph())) and (defined $self->{level_point_count}));
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
	my ($fh) = @_;
	print $fh "level_name = $self->{level_name}\n";
	print $fh "level_id = $self->{level_id}\n" if (not (::level_graph()) and (defined $self->{level_id}));
	print $fh "section_name = $self->{section_name}\n" if (not (::level_graph()) and (defined $self->{section_name}));
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
sub write {
	if ($_[2] eq '1469' or $_[2] eq '1472') {
		$_[1]->pack_properties($_[0], vertex_1472);
	} elsif ($_[2] eq '1510' or $_[2] eq '1935' or $_[2] eq '2215') {
		$_[0]->{edge_offset} = (::edge_block_size($_[2])) * ($_[0]->{edge_index}) + $_[3];
		$_[0]->{level_point_offset} = 0x14 * $_[0]->{level_point_index} + $_[4];
		$_[1]->pack_properties($_[0], vertex_1935);
	} else {
		$_[0]->{edge_offset} = (::edge_block_size($_[2])) * ($_[0]->{edge_index}) + $_[3];
		$_[0]->{level_point_offset} = 0x14 * $_[0]->{level_point_index} + $_[4];
		$_[1]->pack_properties($_[0], vertex_SOC);
	}
}
sub export {
	my $self = shift;
	my ($fh) = @_;
	print $fh "level_point = ", join(',', @{$self->{level_point}}), "\n";
	print $fh "game_point = ", join(',', @{$self->{game_point}}), "\n" if (not (::level_graph()));
	print $fh "level_id = $self->{level_id}\n" if (not (::level_graph()));
	print $fh "level_vertex_id = $self->{level_vertex_id}\n";
	print $fh "vertex_type = ", join(',', @{$self->{vertex_type}}), "\n";
	print $fh "level_points = $self->{level_point_index}, $self->{level_point_count}\n";# if (not (::level_graph()));
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
package game_graph;
use strict;
use IO::File;
use stkutils::data_packet;
use stkutils::chunked_file;
use Cwd;
use Digest::MD5 qw(md5_hex);
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

	print "reading $fn...\n";
	my $fh = IO::File->new($fn, 'r') or die "cannot open $fn\n";
	binmode $fh;
	my $hs = ::header_size($self->{build_version});
	my $vbs = ::vertex_block_size($self->{build_version});
	my $ebs = ::edge_block_size($self->{build_version});
	print "	reading header...\n";
	$fh->read($data, $hs) or die;
	$self->{header} = gg_header->new();
	$self->{header}->read(stkutils::data_packet->new($data), $self->{build_version});
	my $edges_offset = $self->{header}->{vertex_count} * $vbs;
	my $level_points_offset;
	if (not ($self->{build_version} eq '1469' or $self->{build_version} eq '1472')) {
		$level_points_offset = $edges_offset + $self->{header}->{edge_count} * $ebs;
	}
	print "	reading levels...\n";	
	# 4KB should be enough for the level
	$fh->read($data, 0x1000) or die;
	my $packet = stkutils::data_packet->new($data);
	for (my $i = 0; $i < $self->{header}->{level_count}; $i++) {
		my $level = gg_level->new();			
		$level->read($packet, $self->{build_version});
		push @{$self->{levels}}, $level;
	}	
	my %level_by_id = ();
	foreach my $level (@{$self->{levels}}) {
		$level_by_id{$level->{level_id}} = \$level;
	}
	print "	reading vertices...\n";	
	$fh->seek(-$packet->length(), SEEK_CUR);
	$self->{offset_for_ct} = tell $fh;
	for (my $i = 0; $i < $self->{header}->{vertex_count}; $i++) {
		$fh->read($data, $vbs) or die;
		my $vertex = gg_vertex->new();
		if ($self->{build_version} eq '1469' or $self->{build_version} eq '1472') {
			$vertex->read(stkutils::data_packet->new($data), $edges_offset, $self->{build_version});
		} else {
			$vertex->read(stkutils::data_packet->new($data), $edges_offset, $level_points_offset, $self->{build_version});
		}
		my $level_name = $level_by_id{$vertex->{level_id}};
		die "no such level for vertice!!!" if (not defined $level_name);
		push @{$self->{vertices}}, $vertex;
	}
	print "	reading edges...\n";	
	if ($self->{build_version} eq '1469' or $self->{build_version} eq '1472' or $self->{build_version} eq '1510' or ::level_graph()) {
		my $size = -s $fn;
		my $read_offset = tell ($fh);
		my $edge_count = ($size - $read_offset)/$ebs;
		for (my $i = 0; $i < $edge_count; $i++) {
			$fh->read($data, $ebs) or die;
			my $edge = gg_edge->new();
			$edge->read(stkutils::data_packet->new($data), $self->{build_version});
			push @{$self->{edges}}, $edge;
		}	
	} else {
		for (my $i = 0; $i < $self->{header}->{edge_count}; $i++) {
			$fh->read($data, $ebs) or die;
			my $edge = gg_edge->new();
			$edge->read(stkutils::data_packet->new($data), $self->{build_version});
			push @{$self->{edges}}, $edge;
		}
	}
	if (not((::recompile()) or (::level_graph()) or (::add_edge()) or  (::ignore()) or ($self->{build_version} eq '1469' or $self->{build_version} eq '1472' or $self->{build_version} eq '1510'))) {
		print "	reading level points...\n";	
		for (my $i = 0; $i < $self->{header}->{level_point_count}; $i++) {
			$fh->read($data, 0x14) or die;
			my $level_point = gg_level_point->new();
			$level_point->read(stkutils::data_packet->new($data));
			push @{$self->{level_points}}, $level_point;
		}
	} elsif (::recompile() and (not (::add_edge()))) {
		$self->{lp_offsets} = {};
		$self->{lp_counts} = {};
		foreach my $vertex (@{$self->{vertices}}) {
			my $level = $level_by_id{$vertex->{level_id}};
			if (!defined $self->{lp_offsets}{$$level->{level_name}}) {
				$self->{lp_offsets}{$$level->{level_name}} = $vertex->{level_point_offset};    ####îôôñåòû èäóò îò áëîêà ëåâåëîâ!
				$self->{lp_counts}{$$level->{level_name}} = $vertex->{level_point_count};
				my $var = $self->{offset_for_ct} + $self->{lp_offsets}{$$level->{level_name}};
			} else {
				$self->{lp_counts}{$$level->{level_name}} += $vertex->{level_point_count};
			}
		}
	}
	if ($self->{build_version} eq 'cop') {
		if (::recompile()) {
			print "	reading cross tables...\n";	
			my $initial_offset = $self->{offset_for_ct} + 0x2A * ($self->{header}->{vertex_count}) + 0x06 * ($self->{header}->{edge_count}) + 0x14 * ($self->{header}->{level_point_count});
			foreach my $level (@{$self->{levels}}) {
				$fh->seek($initial_offset, 0);
				my $size = '';
				$fh->read($size, 0x04) or die 'cannot read size of cross table '.$level->{level_name}.' - it is empty!';
				my $packet = stkutils::data_packet->new($size);
				my @values = $packet->unpack('V');
				$self->{ct_size}{$level->{level_name}} = $values[0];
				$self->{ct_offset}{$level->{level_name}} = $initial_offset;   	####îôôñåòû èäóò ñ íà÷àëà ôàéëà!
				print "cross table offset for $level->{level_name} is $self->{ct_offset}{$level->{level_name}}\n";
				print "size = $self->{ct_size}{$level->{level_name}}\n";
				$initial_offset += $self->{ct_size}{$level->{level_name}};
			}		
		} elsif (::read_cross_tables()) {
			print "	reading cross tables...\n";	
			foreach my $level (@{$self->{levels}}) {
				$fh->read($data, 0x30) or die;
				my $cross_table = gg_cross_table->new();
				$cross_table->read(stkutils::data_packet->new($data));
				print "		reading cross table for $level->{level_name}...\n";
				$cross_table->{level_name} = $level->{level_name};
				for (my $j = 0; $j < $cross_table->{cell_count}; $j++) {
					$fh->read($data, 0x06);
					my $packet = stkutils::data_packet->new($data);
					my @values = $packet->unpack('vf');
					$cross_table->{cells}->{$j}{graph_id} = $values[0];
					$cross_table->{cells}->{$j}{distance} = $values[1];
				}
				push @{$self->{cross_tables}}, $cross_table;
			}			
		}
	}
}
sub write {
	my $self = shift;
	my $parent_graph = shift;
	my $append_graph = shift;
	my $parent_file = shift;
	my $append_file = shift;

### 1. ÔÎÐÌÈÐÓÅÌ ÐÅÇÓËÜÒÈÐÓÞÙÈÉ ÃÅÉÌ-ÃÐÀÔ ###
	print "forming new graph...\n";
	my %hash_parent_levels;
	my $i = 0;	
	foreach my $level (split (/,/, $parent_graph->{levels_to_keep})) {
		$hash_parent_levels{$level} = $i;
		$i++;
	}
	my %hash_append_levels;
	$i = 0;
	foreach my $level (split (/,/, $append_graph->{levels_to_push})) {
		$hash_append_levels{$level} = $i;
		$i++;
	}	

	my %parent_level_by_id = ();	
	foreach my $level (@{$parent_graph->{levels}}) {
		$parent_level_by_id{$level->{level_id}} = \$level;
		my $lev = $parent_level_by_id{$level->{level_id}};
		if (exists($hash_parent_levels{$$lev->{level_name}})) {
			push @{$self->{levels}}, $level;
		}
	}
	my $game_vertex_id = 0;
	my $level_id = -1;
	my %parent_level_by_gvid_count = ();	
	my %parent_level_by_gvid = ();
	my $prev_gvid = 0;
	
	foreach my $vertex (@{$parent_graph->{vertices}}) {
		if ($vertex->{level_id} != $level_id) {
			my $level = $parent_level_by_id{$level_id};
			my $gvid_count = $game_vertex_id - $prev_gvid;
			if ($gvid_count != 0) {
				$parent_level_by_gvid_count{($$level->{level_name})} = $gvid_count;
				$parent_level_by_gvid{($$level->{level_name})} = $game_vertex_id - 1;
			}
			$level_id = $vertex->{level_id};
			$prev_gvid = $game_vertex_id;
#			print "$$level->{level_name} = $parent_level_by_gvid{($$level->{level_name})}, $parent_level_by_gvid_count{($$level->{level_name})}\n";
		}
		if ($game_vertex_id == ($parent_graph->{header}->{vertex_count} - 1)) {
			my $level = $parent_level_by_id{$vertex->{level_id}};
			my $gvid_count = $game_vertex_id - $prev_gvid;
			$parent_level_by_gvid_count{($$level->{level_name})} = $gvid_count + 1;
			$parent_level_by_gvid{($$level->{level_name})} = $game_vertex_id + 1;
#			print "$$level->{level_name} = $parent_level_by_gvid{($$level->{level_name})}, $parent_level_by_gvid_count{($$level->{level_name})}\n";
		}
		$game_vertex_id++;
	}	

	my %append_level_by_id = ();	
	foreach my $level (@{$append_graph->{levels}}) {
		$append_level_by_id{$level->{level_id}} = \$level;
		my $lev = $append_level_by_id{$level->{level_id}};
		if (exists($hash_append_levels{$$lev->{level_name}})) {
			push @{$self->{levels}}, $level;
		}
	}
	
	$game_vertex_id = 0;
	$level_id = -1;
	my %append_level_by_gvid_count = ();	
	my %append_level_by_gvid = ();
	$prev_gvid = 0;
	foreach my $vertex (@{$append_graph->{vertices}}) {		
		if ($vertex->{level_id} != $level_id)  {
			my $level = $append_level_by_id{$level_id};
			my $gvid_count = $game_vertex_id - $prev_gvid;
			if ($gvid_count != 0) {
				$append_level_by_gvid_count{($$level->{level_name})} = $gvid_count;
				$append_level_by_gvid{($$level->{level_name})} = $game_vertex_id - 1;
			}
			$level_id = $vertex->{level_id};
			$prev_gvid = $game_vertex_id;
		}
		if ($game_vertex_id == ($append_graph->{header}->{vertex_count} - 1)) {
			my $level = $append_level_by_id{$vertex->{level_id}};
			my $gvid_count = $game_vertex_id - $prev_gvid;
			$append_level_by_gvid_count{($$level->{level_name})} = $gvid_count + 1;
			$append_level_by_gvid{($$level->{level_name})} = $game_vertex_id + 1;
		}
		$game_vertex_id++;	
	}
	
	my %new_level_gvids_offsets = ();
	my %new_level_gvids = ();
	my $gvid_to_add = 0;
	my $gvid_to_substract = 0;
	foreach my $level (sort {$parent_level_by_gvid{$a} <=> $parent_level_by_gvid{$b}} keys %parent_level_by_gvid) {
		if (exists($hash_parent_levels{$level})) {
			$new_level_gvids_offsets{$level} = $gvid_to_substract;
			$gvid_to_add += $parent_level_by_gvid_count{$level};
#			print "$gvid_to_substract, $gvid_to_add\n";
			$new_level_gvids{$level} = $parent_level_by_gvid{$level} - $parent_level_by_gvid_count{$level} - $new_level_gvids_offsets{$level};
		} else {
			$gvid_to_substract += $parent_level_by_gvid_count{$level};
		}
	}
	$gvid_to_substract = 0;
	foreach my $level (sort {$append_level_by_gvid{$a} <=> $append_level_by_gvid{$b}} keys %append_level_by_gvid) {
		if (exists($hash_append_levels{$level})) {
			$new_level_gvids_offsets{$level} = $gvid_to_substract - $gvid_to_add;
#			print "$gvid_to_substract, $gvid_to_add\n";
			$new_level_gvids{$level} = $append_level_by_gvid{$level} - $append_level_by_gvid_count{$level} - $new_level_gvids_offsets{$level};
		} else {
			$gvid_to_substract += $append_level_by_gvid_count{$level};
		}
	}
##### Ðàç ÷òåíèå êðîññ-òàáëèö è ëåâåë-ïîèíòîâ æðåò ìíîãî ïàìÿòè, áóäåì äîñòàâàòü íóæíûå äàííûå ïðÿìî ïåðåä çàïèñüþ. ####	
	
	my $edge_indexes = 0;
	my $edge_counts = 0;
	my $lp_indexes = 0;
	my $lp_counts = 0;
	foreach my $vertex (@{$parent_graph->{vertices}}) {
		my $next_level = $parent_level_by_id{$vertex->{level_id}};
		if (exists($hash_parent_levels{$$next_level->{level_name}})) {
			my $edge_counter = $vertex->{edge_count};
			for (my $i = 0; $i < $edge_counter; $i++) {
				my $edge = $parent_graph->{edges}[$vertex->{edge_index} + $i];
				my $vertex2 = $parent_graph->{vertices}[$edge->{game_vertex_id}];
				my $level_control = $parent_level_by_id{$vertex2->{level_id}};
				if (exists($hash_parent_levels{$$level_control->{level_name}})) {
					$edge->{game_vertex_id} -= $new_level_gvids_offsets{$$level_control->{level_name}};
					push @{$self->{edges}}, $edge;
				} else {
					($vertex->{edge_count})--;
				}
			}
			$vertex->{edge_index} = $edge_indexes + $edge_counts;
			$edge_indexes = $vertex->{edge_index};
			$edge_counts = $vertex->{edge_count};
			$vertex->{level_point_index} = $lp_indexes + $lp_counts;
			$lp_indexes = $vertex->{level_point_index};
			$lp_counts = $vertex->{level_point_count};
			push @{$self->{vertices}}, $vertex;
		}
	}
	
	my $vertex_offset = 0;
	foreach my $par_level (sort {$hash_parent_levels{$a} <=> $hash_parent_levels{$b}} keys %hash_parent_levels) {
		my $gvid = $parent_level_by_gvid_count{$par_level};
		$vertex_offset += $gvid;
	}	

	foreach my $vertex (@{$append_graph->{vertices}}) {
		my $next_level = $append_level_by_id{$vertex->{level_id}};
			if (exists($hash_append_levels{$$next_level->{level_name}})) {
				my $edge_counter = $vertex->{edge_count};
				for (my $i = 0; $i < $edge_counter; $i++) {
					my $edge = $append_graph->{edges}[$vertex->{edge_index} + $i];
					my $vertex2 = $append_graph->{vertices}[$edge->{game_vertex_id}];
					my $level_control = $append_level_by_id{$vertex2->{level_id}};
					if (exists($hash_append_levels{$$level_control->{level_name}})) {
						$edge->{game_vertex_id} -= $new_level_gvids_offsets{$$level_control->{level_name}};
						push @{$self->{edges}}, $edge;
					} else {
						($vertex->{edge_count})--;
					}
				}
				$vertex->{edge_index} = $edge_indexes + $edge_counts;
				$edge_indexes = $vertex->{edge_index};
				$edge_counts = $vertex->{edge_count};
				$vertex->{level_point_index} = $lp_indexes + $lp_counts;
				$lp_indexes = $vertex->{level_point_index};
				$lp_counts = $vertex->{level_point_count};
				push @{$self->{vertices}}, $vertex;
			}
	}	
	my %level_guid_by_name = ();
	foreach my $level (@{$self->{levels}}) {
		if (($parent_graph->{header}->{version} > 7) && ($append_graph->{header}->{version} == 7) && exists($hash_append_levels{$level->{level_name}})) {
			$level->{guid} = genGUID($level->{level_name});
			$level_guid_by_name{$level->{level_name}} = $level->{guid};
		}
	}
### 2. ÃÅÍÅÐÈÐÓÅÌ LEVEL.GRAPH ###
	my $work_dir = getcwd();
	print "	writing level graphs...\n";
	foreach my $level (@{$self->{levels}}) {
		my $edge_offsets = 0;
		$edge_indexes = 0;
		$edge_counts = 0;
		my $graph = {};
		if (exists($hash_append_levels{$level->{level_name}})) {
			my $dir = 'levels\\'.$level->{level_name}.'';
			chdir ($dir) or die;
			my $lg_file = IO::File->new('level.graph', 'w');
			binmode $lg_file;
			foreach my $vertex (@{$self->{vertices}}) {
				if ($vertex->{level_id} == $level->{level_id}) {
					$edge_offsets += 0x14;
				}
			}
			foreach my $vertex (@{$self->{vertices}}) {
				if ($vertex->{level_id} == $level->{level_id}) {
					my $vertex_graph = gg_vertex->new();
					$vertex_graph->{level_point} = $vertex->{level_point};
					$vertex_graph->{game_point} = $vertex->{level_point};
					$vertex_graph->{level_id} = $vertex->{level_id};
					$vertex_graph->{level_vertex_id} = $vertex->{level_vertex_id};
					$vertex_graph->{vertex_type} = $vertex->{vertex_type};
					$vertex_graph->{edge_count} = $vertex->{edge_count};
					$vertex_graph->{edge_offset} = $vertex->{edge_offset};
					$vertex_graph->{edge_index} = $vertex->{edge_index};
					$vertex_graph->{level_point_index} = $vertex->{level_point_index};
					$vertex_graph->{level_point_count} = $vertex->{level_point_count} if (defined($vertex->{level_point_count}));
					$vertex_graph->{level_point_offset} = $vertex->{level_point_offset} if (defined($vertex->{level_point_offset}));
					my $edge_counter = $vertex_graph->{edge_count};
					for (my $i = 0; $i < $edge_counter; $i++) {
						my $edge = $self->{edges}[$vertex_graph->{edge_index} + $i];
						my $edge_clone = gg_edge->new();
						my $vertex2 = $self->{vertices}[$edge->{game_vertex_id}];

						my $level_control = $append_level_by_id{$vertex2->{level_id}};
						if ((defined $level_control) && ($$level_control->{level_name} eq $level->{level_name})) {
							$edge_clone->{game_vertex_id} = $edge->{game_vertex_id};
							$edge_clone->{distance} = $edge->{distance};
							$edge_clone->{game_vertex_id} -= $new_level_gvids{$level->{level_name}};
							push @{$graph->{edges}}, $edge_clone;
						} else {
							($vertex_graph->{edge_count})--;
						}
					}
					$vertex_graph->{edge_index} = $edge_indexes + $edge_counts;
					$vertex_graph->{edge_offset} = $edge_offsets + $vertex_graph->{edge_count}*(::edge_block_size($parent_graph->{build_version}));
					$edge_indexes = $vertex_graph->{edge_index};
					$edge_counts = $vertex_graph->{edge_count};
					push @{$graph->{vertices}}, $vertex_graph;
				}
			
			}
			my $lg_header = gg_header->new();
			$lg_header->{version} = $parent_graph->{header}->{version};
			$lg_header->{level_count} = 1;
			$lg_header->{vertex_count} = $#{$graph->{vertices}} + 1;
			$lg_header->{edge_count} = $#{$graph->{edges}} + 1;
			$lg_header->{level_point_count} = 0;
			$lg_header->{guid} = $parent_graph->{header}->{guid};
			my $packet_header =  stkutils::data_packet->new();		
			$lg_header->write($packet_header, $parent_graph->{build_version}, 0);
			$lg_file->write($packet_header->data(), length($packet_header->data()));
			my $new_level = gg_level->new();;		
			$new_level->{level_name} = '\\\x-ray\stalker$\gamedata\levels\\'.$level->{level_name}.'\\';
			$new_level->{offset} = [0,0,0];
			$new_level->{level_id} = 0 if (defined($level->{level_id}));
			$new_level->{section_name} = '' if (defined($level->{section_name}));
			if (defined($level->{guid})) {
				$new_level->{guid} = $level->{guid};
			} else {
				$new_level->{guid} = $level_guid_by_name{$level->{level_name}};
			}
			my $packet_level =  stkutils::data_packet->new();	
			$new_level->write($packet_level, $parent_graph->{build_version}, $new_level->{level_id});
			$lg_file->write($packet_level->data(), length($packet_level->data()));		
			my $edge_offset = (::vertex_block_size($parent_graph->{build_version})) * $lg_header->{vertex_count};;
			foreach my $vertex (@{$graph->{vertices}}) {
				my $packet_vertex = stkutils::data_packet->new();
				$vertex->{level_id} = 0;
				$vertex->{level_point_index} = 0;
				$vertex->{level_point_count} = 0;
				$vertex->write($packet_vertex, $parent_graph->{build_version}, $edge_offset, 0, 1);
				$lg_file->write($packet_vertex->data(), length($packet_vertex->data()));
			}			
			foreach my $edge (@{$graph->{edges}}) {
				my $packet_edge = stkutils::data_packet->new();
				$edge->write($packet_edge, $parent_graph->{build_version});
				$lg_file->write($packet_edge->data(), length($packet_edge->data()));
			}
			chdir ($work_dir);
		}
	}
########################################
	
	$self->{header} = gg_header->new();
	$self->{header} = $parent_graph->{header};
	$self->{header}->{level_count} = $#{$self->{levels}} + 1;
	$self->{header}->{vertex_count} = $#{$self->{vertices}} + 1;
	if (defined $self->{header}->{edge_count}) {
		$self->{header}->{edge_count} = $#{$self->{edges}} + 1;
	}
	if (defined $self->{header}->{level_point_count}) {
		$self->{header}->{level_point_count} = 0;
		foreach my $level (@{$self->{levels}}) {
			if (exists($hash_parent_levels{$level->{level_name}})) {
				$self->{header}->{level_point_count} += $parent_graph->{lp_counts}{$level->{level_name}};
#				print "$level->{level_name} = $self->{header}->{level_point_count}\n";
			}
			if (exists($hash_append_levels{$level->{level_name}})) {
				$self->{header}->{level_point_count} += $append_graph->{lp_counts}{$level->{level_name}};
#				print "$level->{level_name} = $self->{header}->{level_point_count}\n";
			}
		}
	}

### 3. ÇÀÏÈÑÛÂÀÅÌ ØÀÏÊÓ ###
	print "writing new graph...\n";
	my $outfile = IO::File->new('game.graph.new', 'w') or die;
	binmode $outfile;
	my $packet_header = stkutils::data_packet->new();
	$self->{header}->write($packet_header, $parent_graph->{build_version}, 1);
	$outfile->write($packet_header->data(), length($packet_header->data()));
### 4. ÇÀÏÈÑÜ ÓÐÎÂÍÅÉ ###
	print "	writing levels...\n";
	my $lev_id = 1;
	my %new_id_by_old_id = ();
	foreach my $level (@{$self->{levels}}) {
		my $packet = stkutils::data_packet->new();
		$new_id_by_old_id{$level->{level_id}} = $lev_id;
		$level->write($packet, $parent_graph->{build_version}, $lev_id++);
		$outfile->write($packet->data(), length($packet->data()));
	}
	my $edge_offset = (::vertex_block_size($parent_graph->{build_version})) * $self->{header}->{vertex_count};
	my $lp_offset = $edge_offset + (::edge_block_size($parent_graph->{build_version})) * $self->{header}->{edge_count};
### 5. ÇÀÏÈÑÜ ÂÅÐÒÅÊÑÎÂ ###
	print "	writing vertices...\n";
	foreach my $vertex (@{$self->{vertices}}) {
		my $packet = stkutils::data_packet->new();
		$vertex->{level_id} = $new_id_by_old_id{$vertex->{level_id}};
		$vertex->write($packet, $parent_graph->{build_version}, $edge_offset, $lp_offset);
		$outfile->write($packet->data(), length($packet->data()));
	}
### 6. ÇÀÏÈÑÜ ÑÂßÇÅÉ ###
	print "	writing edges...\n";
	foreach my $edge (@{$self->{edges}}) {
		my $packet = stkutils::data_packet->new();
		$edge->write($packet, $parent_graph->{build_version});
		$outfile->write($packet->data(), length($packet->data()));
	}

	if (defined $self->{header}->{level_point_count}) {
		print "	writing level points...\n";
		my $parent_gg = IO::File->new($parent_file, 'r');
		binmode $parent_gg;
		foreach my $level_name (sort {$parent_level_by_gvid{$a} <=> $parent_level_by_gvid{$b}} keys %parent_level_by_gvid) {
			if (exists($hash_parent_levels{$level_name})) {
				my $lp_offset = $parent_graph->{lp_offsets}{$level_name} + $parent_graph->{offset_for_ct};
				my $lp_chunk = '';
				$parent_gg->seek($lp_offset, 0);
				$parent_gg->read($lp_chunk, $parent_graph->{lp_counts}{$level_name} * 0x14);
				$outfile->write($lp_chunk, length($lp_chunk));
			}
		}
		$parent_gg->close();
		my $append_gg = IO::File->new($append_file, 'r');
		binmode $append_gg;
		foreach my $level_name (sort {$append_level_by_gvid{$a} <=> $append_level_by_gvid{$b}} keys %append_level_by_gvid) {
			if (exists($hash_append_levels{$level_name})) {
				my $lp_offset = $append_graph->{lp_offsets}{$level_name} + $append_graph->{offset_for_ct};
				my $lp_chunk = '';
				$append_gg->seek($lp_offset, 0);
				$append_gg->read($lp_chunk, $append_graph->{lp_counts}{$level_name} * 0x14);
				$outfile->write($lp_chunk, length($lp_chunk));
			}
		}
		$append_gg->close();		
	}
### 7. ÇÀÏÈÑÜ ÊÐÎÑÑ-ÒÀÁËÈÖ ###
	print "	writing cross tables...\n";
	my $global_version = 0;
	my $global_guid = 0;
	if ($parent_graph->{build_version} eq 'cop') {
		foreach my $level_name (sort {$parent_level_by_gvid{$a} <=> $parent_level_by_gvid{$b}} keys %parent_level_by_gvid) {
			if (exists($hash_parent_levels{$level_name})) {
				print "		writing cross tables for level $level_name...\n";
				my $new_table = gg_cross_table->new();
				my $parent_gg = IO::File->new($parent_file, 'r');
				binmode $parent_gg;
				my $ct_offset = $parent_graph->{ct_offset}{$level_name};
				$parent_gg->seek($ct_offset,0);
				my $ct_header = '';
				$parent_gg->read($ct_header, 0x30);
				my $packet = stkutils::data_packet->new($ct_header);
				my @values = $packet->unpack('VVVVa[16]a[16]');
				$new_table->{size} = $values[0];
				$new_table->{version} = $values[1];
				$new_table->{cell_count} = $values[2];
				$new_table->{vertex_count} = $values[3];
				$new_table->{level_guid} = $values[4];
				$new_table->{game_guid} = $values[5];
				$global_version = $new_table->{version};
				$global_guid = $new_table->{game_guid};		
				my $packet2 = stkutils::data_packet->new();
				$packet2->pack('VVVVa[16]a[16]',$new_table->{size}, $new_table->{version}, $new_table->{cell_count}, $new_table->{vertex_count}, $new_table->{level_guid}, $new_table->{game_guid});
				$outfile->write($packet2->data(), length($packet2->data()));
			
				my $ct_raw = '';
				$parent_gg->read($ct_raw, $new_table->{size} - 0x30);	
				my $packet3 = stkutils::data_packet->new($ct_raw);
				$outfile->write($packet3->data(), length($packet3->data()));
				$parent_gg->close();
				
			}
		}
		if ($append_graph->{build_version} eq 'cop') {
			foreach my $level_name (sort {$append_level_by_gvid{$a} <=> $append_level_by_gvid{$b}} keys %append_level_by_gvid) {
				if (exists($hash_append_levels{$level_name})) {
					print "		writing cross tables for level $level_name...\n";
					my $new_table = gg_cross_table->new();
					my $append_gg = IO::File->new($append_file, 'r');
					binmode $append_gg;
					print "$append_file\n";
					my $ct_offset = $append_graph->{ct_offset}{$level_name};
					print "ct_offset = $append_graph->{ct_offset}{$level_name}\n";
					$append_gg->seek($ct_offset,0);
					my $offset = tell $append_gg;
					print "$offset\n";
					my $ct_header = '';
					$append_gg->read($ct_header, 0x30);
					$offset = tell $append_gg;
					print "$offset\n";
					my $packet = stkutils::data_packet->new($ct_header);
					my @values = $packet->unpack('VVVVa[16]a[16]');
					$new_table->{size} = $values[0];
					$new_table->{version} = $global_version;
					$new_table->{cell_count} = $values[2];
					$new_table->{vertex_count} = $values[3];
					$new_table->{level_guid} = $values[4];
					$new_table->{game_guid} = $global_guid;
					my $packet2 = stkutils::data_packet->new();
					$packet2->pack('VVVVa[16]a[16]',$new_table->{size}, $new_table->{version}, $new_table->{cell_count}, $new_table->{vertex_count}, $new_table->{level_guid}, $new_table->{game_guid});
					$outfile->write($packet2->data(), length($packet2->data()));
					$offset = tell $append_gg;
					print "$offset\n";
	#				die;					
				my $ct_raw = '';
				$append_gg->read($ct_raw, $new_table->{size} - 0x30);	
				my $packet3 = stkutils::data_packet->new($ct_raw);
				$outfile->write($packet3->data(), length($packet3->data()));
					$append_gg->close();
	#				die;
				}
			}
		} else {
			foreach my $level (sort {$hash_append_levels{$a} <=> $hash_append_levels{$b}} keys %hash_append_levels) {
				my $dir = 'levels\\'.$level.'';
				chdir ($dir);
				my $data;
				my $gct_file = stkutils::chunked_file->new('level.gct', 'r');
				my $cross_table = gg_cross_table->new();
				my ($index, $size) = $gct_file->r_chunk_open();
				if ($size == 0x2C) {
					($cross_table->{version},
					$cross_table->{cell_count},
					$cross_table->{vertex_count},
					$cross_table->{level_guid},
					$cross_table->{game_guid}) = unpack('VVVa[16]a[16]', $gct_file->r_chunk_data());
				} elsif ($size == 0x0C) {
					($cross_table->{version},
					$cross_table->{cell_count},
					$cross_table->{vertex_count}) = unpack('VVV', $gct_file->r_chunk_data());				
				} else {
					die "unexpected size of header chunk in $level cross table\n"
				}
				$gct_file->r_chunk_close();
				if ($cross_table->{version} < 8) {
					$cross_table->{level_guid} = $level_guid_by_name{$level};
				}
				$cross_table->{version} = $global_version;
				$cross_table->{game_guid} = $global_guid;
				($index, $size) = $gct_file->r_chunk_open();
				$cross_table->{size} = $size;
				$cross_table->{raw_data} = $gct_file->r_chunk_data();
				$gct_file->r_chunk_close();
				$gct_file->close();
				my $packet = stkutils::data_packet->new();
				$packet->pack('VVVVa[16]a[16]',$cross_table->{size} + 48, $cross_table->{version}, $cross_table->{cell_count}, $cross_table->{vertex_count}, $cross_table->{level_guid}, $cross_table->{game_guid});
				$outfile->write($packet->data(), length($packet->data()));
				$outfile->write($cross_table->{raw_data}, length($cross_table->{raw_data}));
				chdir ($work_dir);				
			}
		}
	} else {
		print "		updating old cross tables...\n";
		if ($append_graph->{build_version} eq 'cop') {
			print "		writing new cross tables...\n";
			foreach my $level_name (sort {$append_level_by_gvid{$a} <=> $append_level_by_gvid{$b}} keys %append_level_by_gvid) {
				if (exists($hash_append_levels{$level_name})) {
						my $new_table = gg_cross_table->new();
						my $append_gg = IO::File->new($append_file, 'r');
						binmode $append_gg;
						my $ct_offset = $append_graph->{ct_offset}{$level_name};
						$append_gg->seek($ct_offset,0);
						my $ct_header = '';
						$append_gg->read($ct_header, 0x30);
						my $packet = stkutils::data_packet->new($ct_header);
						my @values = $packet->unpack('VVVVa[16]a[16]');
						$new_table->{size} = $values[0];
						$new_table->{version} = $parent_graph->{header}->{version};
						$new_table->{cell_count} = $values[2];
						$new_table->{vertex_count} = $values[3];
						$new_table->{level_guid} = $values[4];
						$new_table->{game_guid} = $parent_graph->{header}->{guid};

						my $dir = 'levels\\'.$level_name.'';
						chdir ($dir);
						my $gct_file = stkutils::chunked_file->new('level.gct', 'w');
						if ($parent_graph->{header}->{version} > 7) {
							$packet->pack('VVVa[16]a[16]',$new_table->{version}, $new_table->{cell_count}, $new_table->{vertex_count},$new_table->{level_guid}, $new_table->{game_guid});
						} elsif ($parent_graph->{header}->{version} == 7) {
							$packet->pack('VVV',$new_table->{version}, $new_table->{cell_count}, $new_table->{vertex_count});
						} else {
							die "unsupported cross table version\n"
						}
						$gct_file->w_chunk(0, $packet->{data});
						my $ct_raw = '';
						$append_gg->read($ct_raw, $new_table->{size} - 0x30);
						$gct_file->w_chunk(1, $ct_raw);
						$gct_file->close();
						chdir ($work_dir);	
					}
				}
		} else {
			foreach my $level (keys %hash_append_levels) {
				my $dir = 'levels\\'.$level.'';
				chdir ($dir);
				my $data;
				my $gct_file = stkutils::chunked_file->new('level.gct', 'r');
				my $cross_table = gg_cross_table->new();
				my ($index, $size) = $gct_file->r_chunk_open();
				if ($size == 0x2C) {
					($cross_table->{version},
					$cross_table->{cell_count},
					$cross_table->{vertex_count},
					$cross_table->{level_guid},
					$cross_table->{game_guid}) = unpack('VVVa[16]a[16]', $gct_file->r_chunk_data());
				} elsif ($size == 0x0C) {
					($cross_table->{version},
					$cross_table->{cell_count},
					$cross_table->{vertex_count}) = unpack('VVV', $gct_file->r_chunk_data());				
				} else {
					die "unexpected size of header chunk in $level cross table\n"
				}
				$gct_file->r_chunk_close();
				($index, $size) = $gct_file->r_chunk_open();
				$cross_table->{size} = $size;
				$cross_table->{raw_data} = $gct_file->r_chunk_data();
				$gct_file->r_chunk_close();
				$cross_table->{version} = $parent_graph->{header}->{version};
				if ($append_graph->{header}->{version} == 7) {
					$cross_table->{level_guid} = $level_guid_by_name{$level};
				}
				$cross_table->{game_guid} = $parent_graph->{header}->{guid};
				$gct_file->close();
				rename 'level.gct', 'level.gct.bak' or (unlink 'level.gct.bak' and rename 'level.gct', 'level.gct.bak') ;
				$gct_file = stkutils::chunked_file->new('level.gct', 'w');
				my $packet = stkutils::data_packet->new();
				if ($parent_graph->{header}->{version} > 7) {
					$packet->pack('VVVa[16]a[16]',$cross_table->{version}, $cross_table->{cell_count}, $cross_table->{vertex_count},$cross_table->{level_guid}, $cross_table->{game_guid});
				} elsif ($parent_graph->{header}->{version} == 7) {
					$packet->pack('VVV',$cross_table->{version}, $cross_table->{cell_count}, $cross_table->{vertex_count});
				} else {
					die "unsupported cross table version\n"
				}
				$gct_file->w_chunk(0, $packet->{data});
				$gct_file->w_chunk(1, $cross_table->{raw_data});
				$gct_file->close();
				chdir ($work_dir);	
			}
		}
	}
	$outfile->close();
### 8. ÎÁÍÎÂËÅÍÈÅ ÀÈ-ÑÅÒÊÈ ###
	if (($parent_graph->{header}->{version} != $append_graph->{header}->{version}) && (not ::debug())) {
		my $work_dir = getcwd();
		print "updating level.ai...\n";
		my $parent_version = $parent_graph->{header}->{version};
		my $append_version = $append_graph->{header}->{version};
		foreach my $level (@{$self->{levels}}) {
			if (exists $hash_append_levels{$level->{level_name}}) {
				print "	updating $level->{level_name}...\n";
				my $dir = 'levels\\'.$level->{level_name}.'';
				chdir ($dir);
				rename 'level.ai', 'level.ai.bak' or (unlink 'level.ai.bak' and rename 'level.ai', 'level.ai.bak');
				my $ai_file = IO::File->new('level.ai.bak','r') or die 'cannot open level.ai for level'.$level->{level_name}.'';
				binmode $ai_file;
				my $data;
				$ai_file->read($data, 0x38) or die;
				my (
					$version,
					$vertex_count,
					$cell_size,
					$factor_y,
					@box,
					$neighboors,
				) = unpack('VVfff6V4', $data);
				my $ai_file_new = IO::File->new('level.ai','w');
				binmode $ai_file_new;
				my $packet = stkutils::data_packet->new();
				$packet->pack('VVfff6V4', $parent_version, $vertex_count, $cell_size, $factor_y, @box, $neighboors);
				$ai_file_new->write($packet->data(), length($packet->data()));
				if (($parent_version < 10) && ($append_version < 10)) {
					my $raw_data = '';
					my $size = -s 'level.ai.bak';
					$ai_file->read($raw_data, $size - 0x38) or die;
					$ai_file_new->write($raw_data, length($raw_data));
				} else {
					if ($version == 10) {
						for (my $i = 0; $i < $vertex_count; $i++) {
							my $cell;
							$ai_file->read($cell, 0x17) or die;
							my (
								$junk,
								$cover,
								$low_cover,
								$plane,
								$packed_xz_lo, $packed_xz_hi,
								$packed_y,
							) = unpack("a[12]vvvvCv", $cell);
							my $cell_packet = stkutils::data_packet->new();
							$cell_packet->pack('a[12]vvvCv', $junk, $cover, $plane, $packed_xz_lo, $packed_xz_hi, $packed_y);
							$ai_file_new->write($cell_packet->data(), length($cell_packet->data()));
						}
					} elsif ($version < 10) {
						for (my $i = 0; $i < $vertex_count; $i++) {
							my $cell;
							$ai_file->read($cell, 0x15) or die;
							my (
								$junk,
								$cover,
								$plane,
								$packed_xz_lo, $packed_xz_hi,
								$packed_y,
							) = unpack('a[12]vvvCv', $cell);
							my $cell_packet = stkutils::data_packet->new();
							$cell_packet->pack('a[12]vvvvCv', $junk, $cover, $cover, $plane, $packed_xz_lo, $packed_xz_hi, $packed_y);
							$ai_file_new->write($cell_packet->data(), length($cell_packet->data()));
						}		
					}
				}
				$ai_file_new->close();
				chdir ($work_dir);
			}
		}
	} else {
		print "no need to update ai-map\n";
	}
### 9. ÎÁÍÎÂËÅÍÈÅ ÂÅÐÒÅÊÑÎÂ Â ÊÐÎÑÑ-ÒÀÁËÈÖÀÕ ###
#	if (::debug()) {
	print "updating vertices in level.gct...\n";
	foreach my $level (@{$self->{levels}}) {
		my $dir = 'levels\\'.$level->{level_name}.'';
		print "	level $level->{level_name}\n";
		chdir ($dir);
		my $gct_file = IO::File->new('level.gct', 'r') or die;
		my $data = '';
		$gct_file->read($data, 0x3C);
		my $header_packet = stkutils::data_packet->new($data);	
		my ($header_index,
		$header_size,
		$version,
		$cell_count,
		$vertex_count,
		$level_guid,
		$game_guid,
		$body_index,
		$body_size) = $header_packet->unpack('VVVVVa[16]a[16]VV');
				
		my $gct_file_raw = stkutils::chunked_file->new('level.gct.new', 'w') or die;
		my $packet = stkutils::data_packet->new();	
		$packet->pack('VVVa[16]a[16]',$version,$cell_count,$vertex_count,$level_guid,$game_guid);
		$gct_file_raw->w_chunk(0, $packet->{data});
		$gct_file_raw->w_chunk_open(1);
		for (my $i = 0; $i < $cell_count; $i++) {
			$gct_file->read($data, 0x06);
			my $read_packet = stkutils::data_packet->new($data);	
			my ($vertex, $distance) = $read_packet->unpack('vf');
			$vertex -= $new_level_gvids_offsets{$level->{level_name}};
			my $write_packet = stkutils::data_packet->new();
			$write_packet->pack('vf', $vertex, $distance);
			$gct_file_raw->w_chunk_data($write_packet->data());
		}
		$gct_file->close();
		$gct_file_raw->w_chunk_close();
		$gct_file_raw->close();
		rename 'level.gct', 'level.gct.bak';
		rename 'level.gct.new', 'level.gct';
		chdir ($work_dir);	
	}
#	}
### 10. ÏÐÀÂÈÌ ÂÅÐÑÈÞ XRLC Â LEVEL###
	print "updating xrlc version...\n";
	if (!($parent_graph->{build_version} eq 'soc' or $parent_graph->{build_version} eq 'cop') || !($append_graph->{build_version} eq 'soc' or $append_graph->{build_version} eq 'cop')) {
		my $parent_level = @{$self->{levels}}[0];
		my $dir = 'levels\\'.$parent_level->{level_name}.'';
		print "	reading parent xrlc version...\n";
		chdir ($dir);
		my $level_file = IO::File->new('level', 'r') or die;
		my $data = '';
		$level_file->read($data, 0xa);
		my $packet = stkutils::data_packet->new($data);	
		my ($index,
		$size,
		$global_xrlc_version) = $packet->unpack('VVv');
		$level_file->close();
		foreach my $level (@{$self->{levels}}) {
			if (exists $hash_append_levels{$level->{level_name}}) {
				my $dir = 'levels\\'.$level->{level_name}.'';
				print "	updating xrlc version for level $level->{level_name}...\n";
				chdir ($dir);
				my $level_file = IO::File->new('level', 'r') or die;
				my $data = '';
				$level_file->read($data, ($level_file->stat())[7]);
				my $packet = stkutils::data_packet->new($data);	
				my ($index,
				$size,
				$xrlc_version) = $packet->unpack('VVv');
				$level_file->close();
				$version = $parent_xrlc_version;
				my $level_file_raw = IO::File->new('level.new', 'w') or die;
				$level_file_raw->write(pack('VVv',$index,$size,$xrlc_version), 0xa)
				$level_file_raw->write($packet->data(), $packet->length());
				$level_file_raw->close();
				rename 'level', 'level.bak';
				rename 'level.new', 'level';
				chdir ($work_dir);	
			}
		}		
	} else {
		print "no need to update xrlc version\n";
	}
	print "done!\n";
}
sub export {
	my $self = shift;
	my ($fn) = @_;
	print "exporting...\n";
	my $fh = IO::File->new($fn, 'w') or die "cannot open $fn\n";
	print "	exporting header...\n";
	print $fh "[header]\n";
	$self->{header}->export($fh);
	print "	exporting levels...\n";
	my $i = 0;
	foreach my $level (@{$self->{levels}}) {
		print $fh "[level_$i]\n";
		$level->export($fh);
		$i++;
	}
	$i = 0;
	print "	exporting vertices...\n";
	foreach my $vertex (@{$self->{vertices}}) {
		print $fh "[vertex_$i]\n";
		$vertex->export($fh);
		$i++;
	}
	$i = 0;
	print "	exporting edges...\n";
	foreach my $edge (@{$self->{edges}}) {
		print $fh "[edge_$i]\n";
		$edge->export($fh);
		$i++;
	}
	if (not (::level_graph()) and (not (::ignore())) and (not ($self->{build_version} eq '1469' or $self->{build_version} eq '1472'))) {
		$i = 0;
		print "	exporting level points...\n";
		foreach my $level_point (@{$self->{level_points}}) {
			print $fh "[level_point_$i]\n";
			$level_point->export($fh);
			$i++;
		}
	}
	if (($self->{build_version} eq 'cop') && (::read_cross_tables())) {
		$i = 0;
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
	$fh->close();
}
sub add_edge {
	my $self = shift;
	my ($vertex1, $vertex2, $graph_name) = @_;

	print "adding edges...\n";
	my $filename = $graph_name.'.new_edge';
	my $outfile = IO::File->new($filename, 'w') or die;
	binmode $outfile;
	
	my $packet_header = stkutils::data_packet->new();
	$self->{header}->{edge_count} += 2;
	$self->{header}->write($packet_header, $self->{build_version}, 1);
	$outfile->write($packet_header->data(), length($packet_header->data()));
	
	foreach my $level (@{$self->{levels}}) {
		my $packet = stkutils::data_packet->new();
		$level->write($packet, $self->{build_version});
		$outfile->write($packet->data(), length($packet->data()));
	}
	my $edge_offset = (::vertex_block_size($self->{build_version})) * $self->{header}->{vertex_count};
	my $lp_offset = $edge_offset + (::edge_block_size($self->{build_version})) * $self->{header}->{edge_count};	
	my $i = 0;
	my $delta_index = 0;
	my $edge_index_1 = 0;
	my $edge_index_2 = 0;
	foreach my $vertex (@{$self->{vertices}}) {
		my $packet = stkutils::data_packet->new();
		if ($i == $vertex1) {
			$vertex->{edge_count} += 1;
			$edge_index_1 = $vertex->{edge_index};
		} elsif ($i == $vertex2) {
			$vertex->{edge_count} += 1;
			$edge_index_2 = $vertex->{edge_index};		
		}
		$vertex->{edge_index} += $delta_index;
		$vertex->{level_point_offset} += (::edge_block_size($self->{build_version})) * 2;
		$vertex->write($packet, $self->{build_version}, $edge_offset, $lp_offset);
		$outfile->write($packet->data(), length($packet->data()));
		if ($i == $vertex1 || $i == $vertex2) {
			$delta_index += 1;
		}
		$i++;
	}
	$i = 0;
	foreach my $edge (@{$self->{edges}}) {
		my $packet = stkutils::data_packet->new();
		if ($i == $edge_index_1) {
			my $data = $self->write_new_edge($vertex2, $vertex1); 
			$outfile->write($data, length($data));
		} elsif ($i == $edge_index_2) {
			my $data = $self->write_new_edge($vertex1, $vertex2); 
			$outfile->write($data, length($data));		
		}
		$edge->write($packet, $self->{build_version});
		$outfile->write($packet->data(), length($packet->data()));
		$i++;
	}
	
	if (defined $self->{header}->{level_point_count}) {
		my $parent_gg = IO::File->new($graph_name, 'r');
		binmode $parent_gg;
		my $offset = $self->{offset_for_ct} + 0x2A * ($self->{header}->{vertex_count}) + 0x06 * ($self->{header}->{edge_count});
		$parent_gg->seek($offset, 0);
		my $data = '';
		$parent_gg->read($data, $self->{header}->{level_point_count} * 0x14);
		$outfile->write($data, length($data));
		if ($self->{build_version} eq 'cop') {
			$data = '';
			my $fsize = -s $graph_name;
			my $pos = tell $parent_gg;
			$parent_gg->read($data, $fsize - $pos);
			$outfile->write($data, length($data));
		}
		$parent_gg->close();
	}
	$outfile->close();
}
sub write_new_edge {
	my $self = shift;
	my ($vertex_add, $vertex_par) = @_;
	
	my $edge_new = gg_edge->new();
	$edge_new->{game_vertex_id} = $vertex_add;
	my $vert1 = $self->{vertices}[$vertex_par];
	my $vert2 = $self->{vertices}[$vertex_add];
	$edge_new->{distance} = sqrt(($vert1->{game_point}[0]-$vert2->{game_point}[0])*($vert1->{game_point}[0]-$vert2->{game_point}[0]) + ($vert1->{game_point}[1]-$vert2->{game_point}[1])*($vert1->{game_point}[1]-$vert2->{game_point}[1]) + ($vert1->{game_point}[2]-$vert2->{game_point}[2])*($vert1->{game_point}[2]-$vert2->{game_point}[2]));
	my $packet = stkutils::data_packet->new();
	$edge_new->write($packet, $self->{build_version});
	return $packet->data();
}
sub show_links_and_level_guids {
	my $self = shift;
	my ($fn) = @_;
	my $fh;
	if (::guids_file()) {
		$fh = IO::File->new($fn, 'w') or die "cannot open $fn\n";
	}
	my %level_by_id = ();
	foreach my $level (@{$self->{levels}}) {
		$level_by_id{$level->{level_id}} = \$level;
	}
	my $vid = 0;
	foreach my $vertex (@{$self->{vertices}}) {
		for (my $i = 0; $i < $vertex->{edge_count}; $i++) {
			my $edge = $self->{edges}[$vertex->{edge_index} + $i];
			my $vid2 = $edge->{game_vertex_id};
			my $vertex2 = $self->{vertices}[$vid2];
			if ($vertex->{level_id} != $vertex2->{level_id}) {
				my $level = $level_by_id{$vertex->{level_id}};
				my $level2 = $level_by_id{$vertex2->{level_id}};
				my $name = $$level->{level_name};
				my $name2 = $$level2->{level_name};
				if (::guids_file()) {				
					print $fh "$name ($vid) --  $edge->{distance}  --> $name2 ($vid2)\n";
				} else {
					printf "%s (%d) --%5.2f--> %s (%d)\n", $name, $vid, $edge->{distance}, $name2, $vid2;		
				}				
			}
		}
		$vid++;
	}
	my $game_vertex_id = 0;
	my $level_id = -1;
	foreach my $vertex (@{$self->{vertices}}) {
		if ($vertex->{level_id} != $level_id) {
			my $level = $level_by_id{$vertex->{level_id}};
			if (::guids_file()) {
				print $fh "{ gvid0 => $game_vertex_id,		name => '$$level->{level_name}' },\n";
			} else {
				print "{ gvid0 => $game_vertex_id,		name => '$$level->{level_name}' },\n";			
			}
			$level_id = $vertex->{level_id};
		}
		$game_vertex_id++;
	}
	if (::guids_file()) {
		$fh->close();
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
		$count += $vertex->{level_point_count};
	}
	print "level points: $count\n";
}
sub genGUID {
    my $seed = shift;
    my $md5 = uc md5_hex ($seed);
    my @octets = $md5 =~ /(.{2})/g;
    
    substr $octets[6], 0, 1, '4'; # GUID Version 4
    substr $octets[8], 0, 1, '8'; # draft-leach-uuids-guids-01.txt GUID variant 
    my $GUID = "@octets[0..3]@octets[4..5]@octets[6..7]@octets[8..9]@octets[10..15]";
    
    $GUID =~ s/ //g;
    return $GUID;
}
#######################################################################
package main;
use strict;
use Getopt::Long;
#local $SIG{__WARN__} = sub {die};
sub usage {
	return <<END
S.T.A.L.K.E.R. *.graph tool
Usage: ggtool.pl [-g1 <input_file>, <build_version(1469|1472|1510|1935|2215|soc|cop)>] [-g2 <input_file>, <build_version(1469|1472|1510|1935|2215|soc|cop)>] [-o <output_file>] [-g] [-l] [-i]
END
}

my $f1;
my $f2;
my $fn1;
my $fn2;
my $b1;
my $b2;
my $l1;
my $l2;
my $level_graph;
my $output;
my $ignore;
my $guids_file;
my $out_links;
my $rct;
my $debug;
my $ai;

GetOptions(
	'g1=s' => \$f1,
	'g2=s' => \$f2,
	'l1=s' => \$l1,
	'l2=s' => \$l2,
	'o=s' => \$output,
	'l' => \$level_graph,
	'i' => \$ignore,
	'g' => \$guids_file,
	'ct' => \$rct,
	'd' => \$debug,
	'ai=s' => \$ai,
) or die usage();

if (defined $f1) {
	($fn1, $b1) = split /,/, $f1;
}
if (defined $f2) {
	($fn2, $b2) = split /,/, $f2;
}
if (defined $ai) {
	my ($vert1, $vert2) = split /,/, $ai;
	my $graph = game_graph->new();
	$graph->{build_version} = $b1;
	$graph->read($fn1);
	$graph->add_edge($vert1, $vert2, $fn1);
	print "done!\n";
}
sub level_graph {return defined $level_graph;}
sub ignore {return defined $ignore;}
sub guids_file {return defined $guids_file;}
sub recompile {return defined $f2;}
sub read_cross_tables {return defined $rct;}
sub debug {return defined $debug;}
sub add_edge {return defined $ai;}

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
		if (::level_graph()) {
			return 0x14;
		} else {
			return 0x24;
		}
	} else {
		return 0x1C;
	}
}

if (!(::add_edge())) {
	my $gg1 = game_graph->new();
	$gg1->{build_version} = $b1;
	$gg1->{levels_to_keep} = $l1;
	$gg1->read($fn1);
	my $gg2;
	if (defined $fn2) {
		$gg2 = game_graph->new();
		$gg2->{build_version} = $b2;
		$gg2->{levels_to_push} = $l2;
		$gg2->read($fn2);
		my $result_graph = game_graph->new();
		$result_graph->write($gg1, $gg2, $fn1, $fn2);
	} else {
		if (defined $output) {
			$gg1->export("$output");
		}
		if (!(::level_graph())) {
			$gg1->show_links_and_level_guids("guids.txt");
			if (not (::ignore()) and not ($gg1->{build_version} eq '1469' or $gg1->{build_version} eq '1472')) {
				$gg1->count_level_points();
			}
		}
	}
}