#!perl -w
#
#Last edited: 13 June 2011, ver.0.7
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
use stkutils::ogf;
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
	my $vbs = ::vertex_block_size($self->{build_version});
	my $ebs = ::edge_block_size($self->{build_version});
	
### ×ÒÅÍÈÅ ØÀÏÊÈ ###
	print "	reading header...\n";
	$fh->read($data, ::header_size($self->{build_version})) or die;
	$self->{header} = gg_header->new();
	$self->{header}->read(stkutils::data_packet->new($data), $self->{build_version});
	my $edges_offset = $self->{header}->{vertex_count} * $vbs;
	my $level_points_offset;
	if (!($self->{build_version} eq '1469' || $self->{build_version} eq '1472')) {
		$level_points_offset = $edges_offset + $self->{header}->{edge_count} * $ebs;
	}
	
### ×ÒÅÍÈÅ ÁËÎÊÀ ÓÐÎÂÍÅÉ ###
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
	
### ×ÒÅÍÈÅ ÁËÎÊÀ ÂÅÐÒÅÊÑÎÂ ###	
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
	
### ×ÒÅÍÈÅ ÁËÎÊÀ ÑÂßÇÅÉ ###		
	print "	reading edges...\n";	
	if ($self->{build_version} eq '1469' || $self->{build_version} eq '1472' || ::level_graph()) {
		my $size = -s $fn;
		my $read_offset = tell ($fh);
		print "$read_offset\n";
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
	
### ×ÒÅÍÈÅ ÁËÎÊÀ ËÅÂÅË_ÏÎÈÍÒÎÂ ###			
	if (!((::recompile()) || (::level_graph()) || (::add_edge()) ||  (::ignore()) || ($self->{build_version} eq '1469' || $self->{build_version} eq '1472' || $self->{build_version} eq '1510'))) {
		print "	reading level points...\n";	
		for (my $i = 0; $i < $self->{header}->{level_point_count}; $i++) {
			$fh->read($data, 0x14) or die;
			my $level_point = gg_level_point->new();
			$level_point->read(stkutils::data_packet->new($data));
			push @{$self->{level_points}}, $level_point;
		}
	} elsif (::recompile() && (!(::add_edge()))) {
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
	
### ×ÒÅÍÈÅ ÁËÎÊÀ ÊÐÎÑÑ-ÒÀÁËÈÖ ###		
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
	my $pg = shift;
	my $ag = shift;
	my $parent_file = shift;
	my $append_file = shift;

### ÔÎÐÌÈÐÎÂÀÍÈÅ ÐÅÇÓËÜÒÈÐÓÞÙÅÃÎ ÃÐÀÔÀ ###
	print "forming new graph...\n";
	$self->{parent_levels} = ();
	my $i = 0;	
	foreach my $level (split (/,/, $pg->{levels_to_keep})) {
		$self->{parent_levels}{$level} = $i;
		$i++;
	}
	$self->{append_levels} = ();
	$i = 0;
	foreach my $level (split (/,/, $ag->{levels_to_push})) {
		$self->{append_levels}{$level} = $i;
		$i++;
	}	

	my %parent_level_by_id = ();	
	foreach my $level (@{$pg->{levels}}) {
		$parent_level_by_id{$level->{level_id}} = \$level;
		my $lev = $parent_level_by_id{$level->{level_id}};
		if (exists($self->{parent_levels}{$$lev->{level_name}})) {
			push @{$self->{levels}}, $level;
		}
	}
	my $game_vertex_id = 0;
	my $level_id = -1;
	my %parent_level_by_gvid_count = ();	
	my %parent_level_by_gvid = ();
	my $prev_gvid = 0;
	
	foreach my $vertex (@{$pg->{vertices}}) {
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
		if ($game_vertex_id == ($pg->{header}->{vertex_count} - 1)) {
			my $level = $parent_level_by_id{$vertex->{level_id}};
			my $gvid_count = $game_vertex_id - $prev_gvid;
			$parent_level_by_gvid_count{($$level->{level_name})} = $gvid_count + 1;
			$parent_level_by_gvid{($$level->{level_name})} = $game_vertex_id + 1;
#			print "$$level->{level_name} = $parent_level_by_gvid{($$level->{level_name})}, $parent_level_by_gvid_count{($$level->{level_name})}\n";
		}
		$game_vertex_id++;
	}	

	my %append_level_by_id = ();	
	foreach my $level (@{$ag->{levels}}) {
		$append_level_by_id{$level->{level_id}} = \$level;
		my $lev = $append_level_by_id{$level->{level_id}};
		if (exists($self->{append_levels}{$$lev->{level_name}})) {
			push @{$self->{levels}}, $level;
		}
	}
	
	$game_vertex_id = 0;
	$level_id = -1;
	my %append_level_by_gvid_count = ();	
	my %append_level_by_gvid = ();
	$prev_gvid = 0;
	foreach my $vertex (@{$ag->{vertices}}) {		
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
		if ($game_vertex_id == ($ag->{header}->{vertex_count} - 1)) {
			my $level = $append_level_by_id{$vertex->{level_id}};
			my $gvid_count = $game_vertex_id - $prev_gvid;
			$append_level_by_gvid_count{($$level->{level_name})} = $gvid_count + 1;
			$append_level_by_gvid{($$level->{level_name})} = $game_vertex_id + 1;
		}
		$game_vertex_id++;	
	}
	
	$self->{new_level_gvids_offsets} = ();
	$self->{new_level_gvids} = ();
	my $gvid_to_add = 0;
	my $gvid_to_substract = 0;
	foreach my $level (sort {$parent_level_by_gvid{$a} <=> $parent_level_by_gvid{$b}} keys %parent_level_by_gvid) {
		if (exists($self->{parent_levels}{$level})) {
			$self->{new_level_gvids_offsets}{$level} = $gvid_to_substract;
			$gvid_to_add += $parent_level_by_gvid_count{$level};
#			print "$gvid_to_substract, $gvid_to_add\n";
			$self->{new_level_gvids}{$level} = $parent_level_by_gvid{$level} - $parent_level_by_gvid_count{$level} - $self->{new_level_gvids_offsets}{$level};
		} else {
			$gvid_to_substract += $parent_level_by_gvid_count{$level};
		}
	}
	$gvid_to_substract = 0;
	foreach my $level (sort {$append_level_by_gvid{$a} <=> $append_level_by_gvid{$b}} keys %append_level_by_gvid) {
		if (exists($self->{append_levels}{$level})) {
			$self->{new_level_gvids_offsets}{$level} = $gvid_to_substract - $gvid_to_add;
#			print "$gvid_to_substract, $gvid_to_add\n";
			$self->{new_level_gvids}{$level} = $append_level_by_gvid{$level} - $append_level_by_gvid_count{$level} - $self->{new_level_gvids_offsets}{$level};
		} else {
			$gvid_to_substract += $append_level_by_gvid_count{$level};
		}
	}
##### Ðàç ÷òåíèå êðîññ-òàáëèö è ëåâåë-ïîèíòîâ æðåò ìíîãî ïàìÿòè, áóäåì äîñòàâàòü íóæíûå äàííûå ïðÿìî ïåðåä çàïèñüþ. ####	
	
	my $edge_indexes = 0;
	my $edge_counts = 0;
	my $lp_indexes = 0;
	my $lp_counts = 0;
	foreach my $vertex (@{$pg->{vertices}}) {
		my $next_level = $parent_level_by_id{$vertex->{level_id}};
		if (exists($self->{parent_levels}{$$next_level->{level_name}})) {
			my $edge_counter = $vertex->{edge_count};
			for (my $i = 0; $i < $edge_counter; $i++) {
				my $edge = $pg->{edges}[$vertex->{edge_index} + $i];
				my $vertex2 = $pg->{vertices}[$edge->{game_vertex_id}];
				my $level_control = $parent_level_by_id{$vertex2->{level_id}};
				if (exists($self->{parent_levels}{$$level_control->{level_name}})) {
					$edge->{game_vertex_id} -= $self->{new_level_gvids_offsets}{$$level_control->{level_name}};
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
	foreach my $par_level (sort {$self->{parent_levels}{$a} <=> $self->{parent_levels}{$b}} keys %{$self->{parent_levels}}) {
		my $gvid = $parent_level_by_gvid_count{$par_level};
		$vertex_offset += $gvid;
	}	

	foreach my $vertex (@{$ag->{vertices}}) {
		my $next_level = $append_level_by_id{$vertex->{level_id}};
			if (exists($self->{append_levels}{$$next_level->{level_name}})) {
				my $edge_counter = $vertex->{edge_count};
				for (my $i = 0; $i < $edge_counter; $i++) {
					my $edge = $ag->{edges}[$vertex->{edge_index} + $i];
					my $vertex2 = $ag->{vertices}[$edge->{game_vertex_id}];
					my $level_control = $append_level_by_id{$vertex2->{level_id}};
					if (exists($self->{append_levels}{$$level_control->{level_name}})) {
						$edge->{game_vertex_id} -= $self->{new_level_gvids_offsets}{$$level_control->{level_name}};
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
	foreach my $level (@{$self->{levels}}) {
		if (($pg->{header}->{version} > 7) && ($ag->{header}->{version} == 7) && exists($self->{append_levels}{$level->{level_name}})) {
			$level->{guid} = genGUID($level->{level_name});
		}
	}

### 2. ÃÅÍÅÐÀÖÈß LEVEL.GRAPH ###
	my $work_dir = getcwd();
	print "	writing level graphs...\n";
	foreach my $level (@{$self->{levels}}) {
		my $edge_offsets = 0;
		$edge_indexes = 0;
		$edge_counts = 0;
		my $graph = {};
		if (exists($self->{append_levels}{$level->{level_name}})) {
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
							$edge_clone->{game_vertex_id} -= $self->{new_level_gvids}{$level->{level_name}};
							push @{$graph->{edges}}, $edge_clone;
						} else {
							($vertex_graph->{edge_count})--;
						}
					}
					$vertex_graph->{edge_index} = $edge_indexes + $edge_counts;
					$vertex_graph->{edge_offset} = $edge_offsets + $vertex_graph->{edge_count}*(::edge_block_size($pg->{build_version}));
					$edge_indexes = $vertex_graph->{edge_index};
					$edge_counts = $vertex_graph->{edge_count};
					push @{$graph->{vertices}}, $vertex_graph;
				}
			
			}
			my $lg_header = gg_header->new();
			$lg_header->{version} = $pg->{header}->{version};
			$lg_header->{level_count} = 1;
			$lg_header->{vertex_count} = $#{$graph->{vertices}} + 1;
			$lg_header->{edge_count} = $#{$graph->{edges}} + 1;
			$lg_header->{level_point_count} = 0;
			$lg_header->{guid} = $pg->{header}->{guid};
			my $packet_header =  stkutils::data_packet->new();		
			$lg_header->write($packet_header, $pg->{build_version}, 0);
			$lg_file->write($packet_header->data(), $packet_header->length());
			my $new_level = gg_level->new();;		
			$new_level->{level_name} = '\\\x-ray\stalker$\gamedata\levels\\'.$level->{level_name}.'\\';
			$new_level->{offset} = [0,0,0];
			$new_level->{level_id} = 0 if (defined($level->{level_id}));
			$new_level->{section_name} = '' if (defined($level->{section_name}));
			$new_level->{guid} = $level->{guid};
			my $packet_level =  stkutils::data_packet->new();	
			$new_level->write($packet_level, $pg->{build_version}, $new_level->{level_id});
			$lg_file->write($packet_level->data(), $packet_level->length());		
			my $edge_offset = (::vertex_block_size($pg->{build_version})) * $lg_header->{vertex_count};;
			foreach my $vertex (@{$graph->{vertices}}) {
				my $packet_vertex = stkutils::data_packet->new();
				$vertex->{level_id} = 0;
				$vertex->{level_point_index} = 0;
				$vertex->{level_point_count} = 0;
				$vertex->write($packet_vertex, $pg->{build_version}, $edge_offset, 0, 1);
				$lg_file->write($packet_vertex->data(), $packet_vertex->length());
			}			
			foreach my $edge (@{$graph->{edges}}) {
				my $packet_edge = stkutils::data_packet->new();
				$edge->write($packet_edge, $pg->{build_version});
				$lg_file->write($packet_edge->data(), $packet_edge->length());
			}
			chdir ($work_dir);
		}
	}
	
	print "writing new graph...\n";	
	my $outfile = IO::File->new('game.graph.new', 'w') or die;
	binmode $outfile;
	
### 3. ÇÀÏÈÑÜ ØÀÏÊÈ ###
	$self->{header} = gg_header->new();
	$self->{header} = $pg->{header};
	$self->{header}->{level_count} = $#{$self->{levels}} + 1;
	$self->{header}->{vertex_count} = $#{$self->{vertices}} + 1;
	if (defined $self->{header}->{edge_count}) {
		$self->{header}->{edge_count} = $#{$self->{edges}} + 1;
	}
	if (defined $self->{header}->{level_point_count}) {
		$self->{header}->{level_point_count} = 0;
		foreach my $level (@{$self->{levels}}) {
			if (exists($self->{parent_levels}{$level->{level_name}})) {
				$self->{header}->{level_point_count} += $pg->{lp_counts}{$level->{level_name}};
			}
			if (exists($self->{append_levels}{$level->{level_name}})) {
				$self->{header}->{level_point_count} += $ag->{lp_counts}{$level->{level_name}};
			}
		}
	}
	my $packet_header = stkutils::data_packet->new();
	$self->{header}->write($packet_header, $pg->{build_version}, 1);
	$outfile->write($packet_header->data(), length($packet_header->data()));
	
### 4. ÇÀÏÈÑÜ ÓÐÎÂÍÅÉ ###
	print "	writing levels...\n";
	my $lev_id = 1;
	my %new_id_by_old_id = ();
	foreach my $level (@{$self->{levels}}) {
		my $packet = stkutils::data_packet->new();
		$new_id_by_old_id{$level->{level_id}} = $lev_id;
		$level->write($packet, $pg->{build_version}, $lev_id++);
		$outfile->write($packet->data(), length($packet->data()));
	}
	my $edge_offset = (::vertex_block_size($pg->{build_version})) * $self->{header}->{vertex_count};
	my $lp_offset = $edge_offset + (::edge_block_size($pg->{build_version})) * $self->{header}->{edge_count};

### 5. ÇÀÏÈÑÜ ÂÅÐÒÅÊÑÎÂ ###
	print "	writing vertices...\n";
	foreach my $vertex (@{$self->{vertices}}) {
		my $packet = stkutils::data_packet->new();
		$vertex->{level_id} = $new_id_by_old_id{$vertex->{level_id}};
		$vertex->write($packet, $pg->{build_version}, $edge_offset, $lp_offset);
		$outfile->write($packet->data(), length($packet->data()));
	}

### 6. ÇÀÏÈÑÜ ÑÂßÇÅÉ ###
	print "	writing edges...\n";
	foreach my $edge (@{$self->{edges}}) {
		my $packet = stkutils::data_packet->new();
		$edge->write($packet, $pg->{build_version});
		$outfile->write($packet->data(), length($packet->data()));
	}

	if (defined $self->{header}->{level_point_count}) {
		print "	writing level points...\n";
		my $parent_gg = IO::File->new($parent_file, 'r');
		binmode $parent_gg;
		foreach my $level_name (sort {$parent_level_by_gvid{$a} <=> $parent_level_by_gvid{$b}} keys %parent_level_by_gvid) {
			if (exists($self->{parent_levels}{$level_name})) {
				my $lp_offset = $pg->{lp_offsets}{$level_name} + $pg->{offset_for_ct};
				my $lp_chunk = '';
				$parent_gg->seek($lp_offset, 0);
				$parent_gg->read($lp_chunk, $pg->{lp_counts}{$level_name} * 0x14);
				$outfile->write($lp_chunk, length($lp_chunk));
			}
		}
		$parent_gg->close();
		my $append_gg = IO::File->new($append_file, 'r');
		binmode $append_gg;
		foreach my $level_name (sort {$append_level_by_gvid{$a} <=> $append_level_by_gvid{$b}} keys %append_level_by_gvid) {
			if (exists($self->{append_levels}{$level_name})) {
				my $lp_offset = $ag->{lp_offsets}{$level_name} + $ag->{offset_for_ct};
				my $lp_chunk = '';
				$append_gg->seek($lp_offset, 0);
				$append_gg->read($lp_chunk, $ag->{lp_counts}{$level_name} * 0x14);
				$outfile->write($lp_chunk, length($lp_chunk));
			}
		}
		$append_gg->close();		
	}
if (!(::debug())) {
### 7. ÇÀÏÈÑÜ ÊÐÎÑÑ-ÒÀÁËÈÖ ###
	print "	writing cross tables...\n";
	my $global_version = 0;
	my $global_guid = 0;
	if ($pg->{header}->{version} > 8) {
		foreach my $level_name (sort {$parent_level_by_gvid{$a} <=> $parent_level_by_gvid{$b}} keys %parent_level_by_gvid) {
			if (exists($self->{parent_levels}{$level_name})) {
				print "		writing cross tables for level $level_name...\n";
				my $new_table = gg_cross_table->new();
				my $parent_gg = IO::File->new($parent_file, 'r');
				binmode $parent_gg;
				my $ct_offset = $pg->{ct_offset}{$level_name};
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
				$outfile->write(pack('VVVVa[16]a[16]',$new_table->{size}, $new_table->{version}, $new_table->{cell_count}, $new_table->{vertex_count}, $new_table->{level_guid}, $new_table->{game_guid}), 0x30);
				
				my $ct_raw = '';
				$parent_gg->read($ct_raw, $new_table->{size} - 0x30);
				my $pack = $self->update_cross_table($level_name, $ct_raw);
				$outfile->write($pack->data(), $pack->length());
				$parent_gg->close();
				
			}
		}
		if ($ag->{header}->{version} > 8) {
			foreach my $level_name (sort {$append_level_by_gvid{$a} <=> $append_level_by_gvid{$b}} keys %append_level_by_gvid) {
				if (exists($self->{append_levels}{$level_name})) {
					print "		writing cross tables for level $level_name...\n";
					my $new_table = gg_cross_table->new();
					my $append_gg = IO::File->new($append_file, 'r');
					binmode $append_gg;
					my $ct_offset = $ag->{ct_offset}{$level_name};
					$append_gg->seek($ct_offset,0);
					my $offset = tell $append_gg;
					my $ct_header = '';
					$append_gg->read($ct_header, 0x30);
					$offset = tell $append_gg;
					my $packet = stkutils::data_packet->new($ct_header);
					my @values = $packet->unpack('VVVVa[16]a[16]');
					$new_table->{size} = $values[0];
					$new_table->{version} = $global_version;
					$new_table->{cell_count} = $values[2];
					$new_table->{vertex_count} = $values[3];
					$new_table->{level_guid} = $values[4];
					$new_table->{game_guid} = $global_guid;
					$outfile->write(pack('VVVVa[16]a[16]',$new_table->{size}, $new_table->{version}, $new_table->{cell_count}, $new_table->{vertex_count}, $new_table->{level_guid}, $new_table->{game_guid}), 0x30);
					$offset = tell $append_gg;
				
					my $ct_raw = '';
					$append_gg->read($ct_raw, $new_table->{size} - 0x30);	
					my $pack = $self->update_cross_table($level_name, $ct_raw);
					$outfile->write($pack->data(), $pack->length());
					$append_gg->close();
				}
			}
		} else {
			foreach my $level (sort {$self->{append_levels}{$a} <=> $self->{append_levels}{$b}} keys %{$self->{append_levels}}) {
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
				if ($cross_table->{version} == 7) {
					$cross_table->{level_guid} = $level->{guid};
				}
				$cross_table->{version} = $global_version;
				$cross_table->{game_guid} = $global_guid;
				($index, $size) = $gct_file->r_chunk_open();
				$cross_table->{size} = $size;
				$cross_table->{raw_data} = $gct_file->r_chunk_data();
				$gct_file->r_chunk_close();
				$gct_file->close();
				$outfile->write(pack('VVVVa[16]a[16]',$cross_table->{size} + 48, $cross_table->{version}, $cross_table->{cell_count}, $cross_table->{vertex_count}, $cross_table->{level_guid}, $cross_table->{game_guid}), 0x30);
				my $pack = $self->update_cross_table($level, $cross_table->{raw_data});
				$outfile->write($pack->data(), $pack->length());
				chdir ($work_dir);				
			}
		}
	} else {
		print "		updating old cross tables...\n";
		foreach my $level (keys %{$self->{parent_levels}}) {
			my $dir = 'levels\\'.$level.'';
			chdir ($dir);
			my $data;
			rename 'level.gct', 'level.gct.bak' or (unlink 'level.gct.bak' and rename 'level.gct', 'level.gct.bak') ;
			my $gct_file = stkutils::chunked_file->new('level.gct.bak', 'r');
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
			$gct_file->close();
			
			$gct_file = stkutils::chunked_file->new('level.gct', 'w');
			my $packet = stkutils::data_packet->new();
			if ($cross_table->{version} > 7) {
				$packet->pack('VVVa[16]a[16]',$cross_table->{version}, $cross_table->{cell_count}, $cross_table->{vertex_count},$cross_table->{level_guid}, $cross_table->{game_guid});
			} elsif ($cross_table->{version} == 7) {
				$packet->pack('VVV',$cross_table->{version}, $cross_table->{cell_count}, $cross_table->{vertex_count});
			} else {
				die "unsupported cross table version\n"
			}
			$gct_file->w_chunk(0, $packet->{data});
			my $pack = $self->update_cross_table($level, $cross_table->{raw_data});
			$gct_file->w_chunk(1, $pack->data());
			$gct_file->close();
			chdir ($work_dir);	
		}
		if ($ag->{header}->{version} > 8) {
			print "		writing new cross tables...\n";
			foreach my $level_name (sort {$append_level_by_gvid{$a} <=> $append_level_by_gvid{$b}} keys %append_level_by_gvid) {
				if (exists($self->{append_levels}{$level_name})) {
						my $new_table = gg_cross_table->new();
						my $append_gg = IO::File->new($append_file, 'r');
						binmode $append_gg;
						my $ct_offset = $ag->{ct_offset}{$level_name};
						$append_gg->seek($ct_offset,0);
						my $ct_header = '';
						$append_gg->read($ct_header, 0x30);
						my $packet = stkutils::data_packet->new($ct_header);
						my @values = $packet->unpack('VVVVa[16]a[16]');
						$new_table->{size} = $values[0];
						$new_table->{version} = $pg->{header}->{version};
						$new_table->{cell_count} = $values[2];
						$new_table->{vertex_count} = $values[3];
						$new_table->{level_guid} = $values[4];
						$new_table->{game_guid} = $pg->{header}->{guid};

						my $dir = 'levels\\'.$level_name.'';
						chdir ($dir);
						my $gct_file = stkutils::chunked_file->new('level.gct', 'w');
						if ($pg->{header}->{version} > 7) {
							$packet->pack('VVVa[16]a[16]',$new_table->{version}, $new_table->{cell_count}, $new_table->{vertex_count},$new_table->{level_guid}, $new_table->{game_guid});
						} elsif ($pg->{header}->{version} == 7) {
							$packet->pack('VVV',$new_table->{version}, $new_table->{cell_count}, $new_table->{vertex_count});
						} else {
							die "unsupported cross table version\n"
						}
						$gct_file->w_chunk(0, $packet->{data});
						my $ct_raw = '';
						$append_gg->read($ct_raw, $new_table->{size} - 0x30);
						my $pack = $self->update_cross_table($level_name, $ct_raw);
						$gct_file->w_chunk(1, $pack->data());
						$gct_file->close();
						chdir ($work_dir);	
					}
				}
		} else {
			foreach my $level (keys %{$self->{append_levels}}) {
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
				$cross_table->{version} = $pg->{header}->{version};
				if ($ag->{header}->{version} == 7) {
					$cross_table->{level_guid} = $level->{guid};
				}
				$cross_table->{game_guid} = $pg->{header}->{guid};
				$gct_file->close();
				rename 'level.gct', 'level.gct.bak' or (unlink 'level.gct.bak' and rename 'level.gct', 'level.gct.bak') ;
				$gct_file = stkutils::chunked_file->new('level.gct', 'w');
				my $packet = stkutils::data_packet->new();
				if ($pg->{header}->{version} > 7) {
					$packet->pack('VVVa[16]a[16]',$cross_table->{version}, $cross_table->{cell_count}, $cross_table->{vertex_count},$cross_table->{level_guid}, $cross_table->{game_guid});
				} elsif ($pg->{header}->{version} == 7) {
					$packet->pack('VVV',$cross_table->{version}, $cross_table->{cell_count}, $cross_table->{vertex_count});
				} else {
					die "unsupported cross table version\n"
				}
				$gct_file->w_chunk(0, $packet->{data});
				my $pack = $self->update_cross_table($level, $cross_table->{raw_data});
				$gct_file->w_chunk(1, $pack->data());
				$gct_file->close();
				chdir ($work_dir);	
			}
		}
	}
	$outfile->close();
	
### 8. ÎÁÍÎÂËÅÍÈÅ ÐÅÑÓÐÑÍÛÕ ÔÀÉËÎÂ ###
	if (::sect()) {
		$self->update_level_sectors();
	}
	if ($pg->{header}->{version} != $ag->{header}->{version}) {
		$self->update_ai_map($pg->{header}->{version}, $ag->{header}->{version});
		$self->process_level($pg->{build_version}, $ag->{build_version});
	} else {
		print "no need to update resource files\n";
	}
}
### 9. ÑÊÀÍÈÐÎÂÀÍÈÅ ÍÅÎÁÕÎÄÈÌÛÕ ÒÅÊÑÒÓÐ ###
	if (::rscan()) {
		#ñíà÷àëà ñôîðìèóåì ñïèñîê íåîáõîäèìûõ òåêñòóð, øåéäåðîâ, ìàòåðèàëîâ è çâóêîâ
		my %texts;
		my %shaders;
		my %mats;
		foreach my $level (keys %{$self->{append_levels}}) {
			my $dir = 'levels\\'.$level;
			chdir($dir);
			print "scanning textures and shaders for $level...\n";
			my $lFile = stkutils::chunked_file->new('level.bak', 'r');
			if ($lFile->find_chunk(0x2)) {
				my $packet = stkutils::data_packet->new($lFile->r_chunk_data());
				my ($count) = $packet->unpack('V');
				for (my $i = 0; $i < $count; $i++) {
					my ($st) = $packet->unpack('Z*');
					my ($shader, $textures) = split /\//, $st;
					$shaders{$shader} = $i if defined $shader;
					if (defined $textures) {
						my @mtemp = split /,/, $textures;
						foreach my $text (@mtemp) {
							$texts{$text} = $i if !($text =~ /^(lmap)/);
						}
					}
				}
				$lFile->close_found_chunk();
			}
			print "scanning materials for $level...\n";
			if ($lFile->find_chunk(0x5)) {
				my $packet = stkutils::data_packet->new($lFile->r_chunk_data(36));
				my ($version, 
					$vert_count,
					$face_count) = $packet->unpack('VVV');
				my @bbox = $packet->unpack('f6');
				$lFile->r_chunk_seek($vert_count * 0xc);
				for (my $i = 0; $i < $face_count; $i++) {
					$lFile->r_chunk_seek(12);
					my $data = '';
					$lFile->r_chunk_data(4);
					my ($mat_id) = unpack('V', $data);
					$mat_id &= 0x3fff;
					$mats{$mat_id} = $i;
				}		
				$lFile->close_found_chunk();
				$lFile->close();
			} else {
				$lFile->close();
				my $cFile = IO::File->new('level.cform.bak', 'r');
				my $data = '';
				$cFile->read($data, 12);
				my ($version, 
					$vert_count,
					$face_count) = unpack('VVV', $data);
				$cFile->seek($vert_count * 12 + 0x24, SEEK_SET);
				for (my $i = 0; $i < $face_count; $i++) {
					$cFile->seek(12, SEEK_CUR);
					my $data = '';
					$cFile->read($data, 4);
					my ($mat_id) = unpack('V', $data);
					$mat_id &= 0x3fff;
					$mats{$mat_id} = $i;
				}				
				$cFile->close();
			}
			chdir($work_dir);
		}
		#òåïåðü ïðîñêàíèðóåì ãåéìäàòó è ïðîâåðèì, ÷òî åñòü
		my $dir = ::rscan();
		if ($dir eq '') {
			$dir = $work_dir;
		}
		print "scanning existing textures...\n";
		foreach my $text (keys %texts) {
			$texts{$text} = undef if (-e ($dir.'\textures\\'.$text.'.dds'));
		}
		print "scanning existing shaders...\n";
		my $shFile = stkutils::chunked_file->new($dir.'\shaders.xr', 'r');
		$shFile->find_chunk(0x3);
		my $packet = stkutils::data_packet->new($shFile->r_chunk_data());
		my ($count) = $packet->unpack('V');
		my @temp = $packet->unpack("(Z*)$count");
		foreach my $t (@temp) {
			$shaders{$t} = undef if exists($shaders{$t});
		}			
		$shFile->close_found_chunk();
		$shFile->close();
		print "scanning existing materials...\n";
		my $mFile = stkutils::chunked_file->new($dir.'\gamemtl.xr', 'r');
		$mFile->find_chunk(0x1002);
		my %mtemp;
		while (1) {
			my ($index, $size) = $mFile->r_chunk_open();
			defined $index or last;
			$mFile->find_chunk(0x1000);
			my ($mat_id, $mat_name) = unpack ('VZ*', $mFile->r_chunk_data());		
			$mtemp{$mat_id} = $mat_name;
			$mFile->close_found_chunk();
			$mFile->r_chunk_close();
		}
		$mFile->close_found_chunk();
		$mFile->close();
		foreach my $mat (keys %mtemp) {
			$mats{$mat} = undef if exists($mats{$mat});
		}
		print "printing needed resources...\n";
		my $resLog = IO::File->new('res.log', 'w');
		print $resLog "[textures]\n";
		foreach my $text (sort {$a cmp $b} keys %texts) {
			print $resLog "$text\n" if defined $texts{$text};
		}
		print $resLog "\n[shaders]\n";
		foreach my $shader (sort {$a cmp $b} keys %shaders) {
			print $resLog "$shader\n" if defined $shaders{$shader};
		}
		print $resLog "\n[materials]\n";
		foreach my $mat (sort {$a <=> $b} keys %mats) {
			print $resLog "$mat\n" if defined $mats{$mat};
		}
		$resLog->close();
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
sub update_cross_table {
	my $self = shift;
	my ($level_name, $raw) = @_;
	
	my $temp = IO::File->new('~ggrc_temp', 'w');
	binmode $temp;
	$temp->write($raw, length($raw));
	$temp->close();
	$temp = IO::File->new('~ggrc_temp', 'r');
	binmode $temp;
	my $len = (-s '~ggrc_temp') / 6;
	my $data = '';
	my $packet = stkutils::data_packet->new();
	for (my $i = 0; $i < $len; $i++) {
		$temp->read($data, 0x06);
		my ($vertex, $distance) = unpack('vf', $data);
		$vertex -= $self->{new_level_gvids_offsets}{$level_name};
		$packet->pack('vf', $vertex, $distance);
	}
	$temp->close();
	unlink '~ggrc_temp';
	return $packet;
}
sub update_level_sectors {
	my $self = shift;

	foreach my $level (@{$self->{levels}}) {
		if (exists $self->{append_levels}{$level->{level_name}}) {	
			my $dir = 'levels\\'.$level->{level_name}.'';
			chdir ($dir);
			rename 'level_sectors.ai', 'level_sectors.ai.bak' or (unlink 'level_sectors.ai.bak' and rename 'level_sectors.ai', 'level_sectors.ai.bak');
			my $lsa = IO::File->new('level_sectors.ai.bak','r') or die 'cannot open level_sectors.ai for level'.$level->{level_name}.'';
			binmode $lsa;		
			my $lsa_new = IO::File->new('level_sectors.ai','w');
			binmode $lsa_new;		
			my $data = '';
			$lsa_new->write(pack('VVa[16]', 0, 0x10, $level->{guid}), 0x18);
			$lsa->seek(0x18, SEEK_SET);
			$lsa->read($data, (-s 'level_sectors.ai.bak' - 0x18));	
			$lsa_new->write($data, length($data));
			$lsa->close();
			$lsa_new->close();
		}
	}
}
sub update_ai_map {
	my $self = shift;
	my ($pg, $ag) = @_;
	if (!($pg == 8 && $ag == 9) && !($pg == 9 && $ag == 8)) {
		my $work_dir = getcwd();
		print "updating level.ai...\n";
		foreach my $level (@{$self->{levels}}) {
			if (exists $self->{append_levels}{$level->{level_name}}) {
				print "	updating $level->{level_name}...\n";
				my $dir = 'levels\\'.$level->{level_name}.'';
				chdir ($dir);
				rename 'level.ai', 'level.ai.bak' or (unlink 'level.ai.bak' and rename 'level.ai', 'level.ai.bak');
				my $ai_file = IO::File->new('level.ai.bak','r') or die 'cannot open level.ai for level'.$level->{level_name}.'';
				binmode $ai_file;
				my $ai_file_new = IO::File->new('level.ai','w');
				binmode $ai_file_new;
				my ($data, $version, $vertex_count, $cell_size, $factor_y, @box, $level_guid);
				if ($ag > 7) {
					$ai_file->read($data, 0x38) or die;
					(	$version,
						$vertex_count,
						$cell_size,
						$factor_y,
						@box,
						$level_guid,
					) = unpack('VVfff6a[16]', $data);
				} elsif ($ag == 7) {
					$ai_file->read($data, 0x28) or die;
					(	$version,
						$vertex_count,
						$cell_size,
						$factor_y,
						@box,
					) = unpack('VVfff6', $data);
				} else {
					die "sorry, builds earlier than April 2004 are currently not supported\n";
				}
				if ($pg > 7) {
					$ai_file_new->write(pack('VVfff6a[16]', $pg, $vertex_count, $cell_size, $factor_y, @box, $level->{guid}), 0x38);
				} elsif ($pg == 7) {
					$ai_file_new->write(pack('VVfff6', $pg, $vertex_count, $cell_size, $factor_y, @box), 0x28);
				}
				
				if (($pg < 10) && ($ag < 10)) {
					my $raw_data = '';
					my $size = -s 'level.ai.bak';
					my $offset;
					if ($version > 7) {
						$offset = 0x38;
					} elsif ($version == 7) {
						$offset = 0x28;
					} else {
						die "sorry, builds earlier than April 2004 are currently not supported\n";
					}
					$ai_file->read($raw_data, $size - $offset) or die;
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
							$ai_file_new->write(pack('a[12]vvvCv', $junk, $cover, $plane, $packed_xz_lo, $packed_xz_hi, $packed_y), 0x15);
						}
					} elsif ($version > 6) {
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
							$ai_file_new->write(pack('a[12]vvvvCv', $junk, $cover, $cover, $plane, $packed_xz_lo, $packed_xz_hi, $packed_y), 0x17);
						}		
					} else {
						die "sorry, builds earlier than April 2004 are currently not supported\n";
					}
				}
				$ai_file_new->close();
				chdir ($work_dir);
			}
		}
	} else {
		print "no need to update ai-map\n";
	}
}
sub process_level {
	my $self = shift;
	my ($pg, $ag) = @_;
	my $workdir = getcwd();
	print "updating level...\n";
	my $pLevel = @{$self->{levels}}[0];
	my $dir = 'levels\\'.$pLevel->{level_name}.'';
	chdir ($dir);
	my $pFile = IO::File->new('level', 'r') or die;
	my $data = '';
	$pFile->read($data, 0xa);
	my $packet = stkutils::data_packet->new($data);	
	my ($index,
		$size,
		$pXRLC) = $packet->unpack('VVv');
	$pFile->close();
	chdir ($workdir);

	my $aLevel = @{$self->{levels}}[$#{$self->{levels}}];
	$dir = 'levels\\'.$aLevel->{level_name}.'';
	chdir ($dir);
	my $aFile = IO::File->new('level', 'r') or die;
	$data = '';
	$aFile->read($data, 0xa);
	$packet = stkutils::data_packet->new($data);	
	my $aXRLC;
	($index,
		$size,
		$aXRLC) = $packet->unpack('VVv');
	$aFile->close();
	chdir ($workdir);
	
	if ($pXRLC != $aXRLC) {
		$self->update_xrlc_version($pXRLC, $aXRLC);
		if ($pXRLC > 12 && $aXRLC > 12) {
			$self->update_geom($pXRLC, $aXRLC, 'geom');
			$self->update_geom($pXRLC, $aXRLC, 'geomx');
		} else {
			$self->update_cform($pXRLC, $aXRLC);
		}
	}
}
sub update_geom {
	my $self = shift;
	my ($pg, $ag, $ext) = @_;
	my $workdir = getcwd();
	my $filename = 'level.'.$ext;
	print "updating $filename...\n";
	my $parent_level = @{$self->{levels}}[0];
	my $dir = 'levels\\'.$parent_level->{level_name}.'';
	chdir ($dir);
	my $level_file = IO::File->new($filename, 'r') or die;
	my $data = '';
	$level_file->read($data, 0xa);
	my $packet = stkutils::data_packet->new($data);	
	my ($index,
		$size,
		$parent_xrlc_version) = $packet->unpack('VVv');
	$level_file->close();
	my ($xrlc_version, $xrlc_quality);
	foreach my $level (@{$self->{levels}}) {
		if (exists $self->{append_levels}{$level->{level_name}}) {
			my $dir = 'levels\\'.$level->{level_name}.'';
			print "	updating $filename for level $level->{level_name}...\n";
			chdir ($dir);
			rename $filename, $filename.'bak';
			my $level_r = stkutils::chunked_file->new($filename.'bak', 'r') or die;
			my %chunk_hash = ();
			while (1) {
				my ($index, $size) = $level_r->r_chunk_open();
				defined $index or last;
				if ($index == 0x1) {
					die unless $size == 4;
					($xrlc_version, $xrlc_quality) = unpack('vv', $level_r->r_chunk_data());
				} else {
					$chunk_hash{$index} = $level_r->r_chunk_data();
				}
				$level_r->r_chunk_close();
			}
			$level_r->close();
			my $level_w = stkutils::chunked_file->new($filename, 'w') or die;
			$level_w->w_chunk(0x1, pack('vv', $parent_xrlc_version, $xrlc_quality));
			$level_w->w_chunk(0x9, $chunk_hash{0x9});
			$level_w->w_chunk(0xa, $chunk_hash{0xa});
			$level_w->w_chunk(0xb, $chunk_hash{0xb}) if exists $chunk_hash{0xb};
			$level_w->w_chunk(0x8000000b, $chunk_hash{0x8000000b}) if exists $chunk_hash{0x8000000b};
			$level_w->close();
		}
	}
}
sub update_xrlc_version {
	my $self = shift;
	my ($pXRLC, $aXRLC) = @_;
	my $workdir = getcwd();
	foreach my $level (@{$self->{levels}}) {
		if (exists $self->{append_levels}{$level->{level_name}}) {
			my $dir = 'levels\\'.$level->{level_name}.'';
			print "	updating level for level $level->{level_name}...\n";
			chdir ($dir);
			rename 'level', 'level.bak';
			my $level_r = stkutils::chunked_file->new('level.bak', 'r') or die;
			my %chunk_hash = ();
			my $xrlc_quality;
			while (1) {
				my ($index, $size) = $level_r->r_chunk_open();
				defined $index or last;
				if ($index == 0x1) {
					die unless $size == 4;
					($aXRLC, $xrlc_quality) = unpack('vv', $level_r->r_chunk_data());
				} else {
					$chunk_hash{$index} = $level_r->r_chunk_data();
					$chunk_hash{$index} = 'none' if $size == 0;
				}
				$level_r->r_chunk_close();
			}
			$level_r->close();
			my $level_w = stkutils::chunked_file->new('level', 'w') or die;
			if (($pXRLC > 12) && ($aXRLC == 12)) {
				$level_w->w_chunk(0x1, pack('vv', $pXRLC, $xrlc_quality));
			} elsif (($pXRLC == 12) && ($aXRLC > 12)) {
				$level_w->w_chunk(0x1, pack('V', $pXRLC));
			}
			if (exists $chunk_hash{0x4} && !($chunk_hash{0x4} eq 'none')) {
				$level_w->w_chunk(0x4, $chunk_hash{0x4});
			} elsif (exists $chunk_hash{0x4} && $chunk_hash{0x4} eq 'none') {
				$level_w->w_chunk(0x4, 0);
			}
			if (exists $chunk_hash{0x80000004} && !($chunk_hash{0x80000004} eq 'none')) {
				$level_w->w_chunk(0x80000004, $chunk_hash{0x80000004});
			} elsif (exists $chunk_hash{0x80000004} && $chunk_hash{0x80000004} eq 'none') {
				$level_w->w_chunk(0x80000004, 0);
			}
			$level_w->w_chunk(0x6, $chunk_hash{0x6});
			$level_w->w_chunk(0x7, $chunk_hash{0x7});
			if ($pXRLC == 12 || $aXRLC == 12) {
				$self->convert_visuals($level_w, $chunk_hash{0x3}, $pXRLC, $aXRLC);
			} else {
				$level_w->w_chunk(0x3, $chunk_hash{0x3});
			}
			if (($pXRLC > 12) && ($aXRLC == 12)) {
				my $geom = stkutils::chunked_file->new('level.geom', 'w');
				$geom->w_chunk(0x1, pack('vv', $pXRLC, $xrlc_quality));
				$geom->w_chunk(0xa, $chunk_hash{0x9});
				$geom->w_chunk(0x9, $chunk_hash{0xa});
				$geom->w_chunk(0xb, $chunk_hash{0xb});
				$geom->close();
				copy ('level.geom', 'level.geomx');
			} elsif (($pXRLC == 12) && ($aXRLC > 12)) {
				my $geom = stkutils::chunked_file->new('level.geom', 'r');
				while(1) {
					my ($index, $size) = $geom->r_chunk_open();
					defined $index or last;
					if ($index != 0x1) {
						$chunk_hash{$index} = $geom->r_chunk_data();
					}
					$geom->r_chunk_close();					
				}
				$geom->close();
				$level_w->w_chunk(0xa, $chunk_hash{0x9}) if exists $chunk_hash{0x9};
				$level_w->w_chunk(0xb, $chunk_hash{0xb}) if exists $chunk_hash{0xb};
				$level_w->w_chunk(0x9, $chunk_hash{0xa}) if exists $chunk_hash{0xa};
			}
			$level_w->w_chunk(0x2, $chunk_hash{0x2});
			$level_w->w_chunk(0x8, $chunk_hash{0x8}) if exists $chunk_hash{0x8};
			$level_w->w_chunk(0x80000008, $chunk_hash{0x80000008}) if exists $chunk_hash{0x80000008};
			$level_w->close();
			chdir ($workdir);
		}
	}	
}
sub convert_visuals {
	my $self = shift;
	my ($level, $raw, $pXRLC, $aXRLC) = @_;
	print "	converting visuals...\n";
	my @visuals = ();
	my $temp = IO::File->new('~ggrc_temp_visuals', 'w');
	binmode $temp;
	$temp->write($raw, length($raw));
	$temp->close();
	$temp = stkutils::chunked_file->new('~ggrc_temp_visuals', 'r');
	$level->w_chunk_open(0x3);
	while (1) {
		my ($index, $size) = $temp->r_chunk_open();
		defined $index or last;
		print "$index\n";
		my $vis = stkutils::ogf->new();
		$vis->read($temp);
		$temp->r_chunk_close();
		$level->w_chunk_open($index);
		$vis->convert(4, $self->{subversion});
		$vis->write($level);
		$level->w_chunk_close();
	}
	$level->w_chunk_close();
	$temp->close();
	unlink '~ggrc_temp_visuals';
}
sub update_cform {
	my $self = shift;
	my ($pXRLC, $aXRLC) = @_;
	print "updating level.cform...\n";
	foreach my $level (@{$self->{levels}}) {
		if (exists $self->{append_levels}{$level->{level_name}}) {
			print "	updating level.cform for level $level->{level_name}...\n";
			chdir('levels\\'.$level->{level_name}.'');
			rename 'level.cform', 'level.cform.bak';
			my $rCFORM = IO::File->new('level.cform.bak', 'r');
			binmode $rCFORM;
			my $data = '';
			$rCFORM->read($data, 0x24);
			my $wCFORM = IO::File->new('level.cform', 'w');
			binmode $wCFORM;
			my ($version,
				$vertcount,
				$facecount,
				@bbox) = unpack('VVVf6', $data);
			$wCFORM->write(pack('VVVf6', $version, $vertcount, $facecount, @bbox), 0x24);
			$rCFORM->read($data, 12 * $vertcount) or die;
			$wCFORM->write($data, 12 * $vertcount);
			for (my $i = 0; $i < $facecount; $i++) {
				$rCFORM->read($data, 0x10) or die;
				my ($face,
					$mat_id) = unpack('a[12]V', $data);
				my $sectors = $mat_id >> 16;
				$sectors <<= 16;
				if ($pXRLC == 12) {
					$mat_id &= 0x3FFF;
					$mat_id += $sectors;
#				} else {
#					$mat_id &= 0x3FFF;
				}
				$wCFORM->write(pack('a[12]V', $face, $mat_id), 0x10);
			}
			$rCFORM->close();
			$wCFORM->close();
		}
	}
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
my $ai;
my $sv;
my $sect;
my $rscan;
my $debug;

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
	'debug' => \$debug,
	'sect' => \$sect,
	'ai=s' => \$ai,
	'sv=i' => \$sv,
	'rscan:s' => \$rscan,
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
sub sect {return defined $sect;}
sub rscan {return $rscan;}
 
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
sub prepare_sv {
	my ($sv) = @_;
	if ($sv <= 1917) {
		return 1;
	} elsif ($sv <= 1936) {
		return 2;
	} elsif ($sv <= 1964) {
		return 3;
	} elsif ($sv <= 2218) {
		return 4;
	} else {
		return 5;
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
		if (defined $sv) {
			$result_graph->{subversion} = prepare_sv($sv);
			print "$result_graph->{subversion}\n";
		}
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