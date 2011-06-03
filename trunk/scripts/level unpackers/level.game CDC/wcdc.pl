#!perl -w -I \temp\1\bin
#
# last edited:	11 Oct 2007
#
# tab size:	8
#
#######################################################################
package way_object;
use strict;
use stkutils::data_packet;
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
		if ($index == 1) {
			$size == 2 or die;
			my $unknown_count = unpack('v', $cf->r_chunk_data());
		} elsif ($index == 2) {
			$self->read_points($cf);
		} elsif ($index == 3) {
			$self->read_links($cf);
		} elsif ($index == 5) {
			$self->{name} = unpack('Z*', $cf->r_chunk_data());
		} elsif ($index == 4) {
			$size == 4 or die;
			my $empty_count = unpack('V', $cf->r_chunk_data());
		}
		$cf->r_chunk_close();
	}
}
sub spawn_read_freaky {
	my $self = shift;
	my ($cf) = @_;
		
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		my %point;
		(
			$point{position}[0],
			$point{position}[1],
			$point{position}[2],
			$point{direction}[0],
			$point{direction}[1],
			$point{direction}[2],
			$point{flags},
		) = unpack('f3f3V', $cf->r_chunk_data());
		push @{$self->{points}}, \%point;
		$cf->r_chunk_close();
	}
}
sub spawn_read_1465 {
	my $self = shift;
	my ($cf) = @_;
		
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		my %point;
		(
			$point{position}[0],
			$point{position}[1],
			$point{position}[2],
		) = unpack('f3', $cf->r_chunk_data());
		push @{$self->{points}}, \%point;
		$cf->r_chunk_close();
	}
}
sub read_points {
	my $self = shift;
	my ($cf) = @_;
	my $data = $cf->r_chunk_data();
	my $point_count = unpack('v', $data);
	my $packet = stkutils::data_packet->new(substr($data, 2));
	my $i = 0;
	if (::use_1154()) {
		while ($i < $point_count) {
			my %point;
			(
				$point{position}[0],
				$point{position}[1],
				$point{position}[2],
				$point{flags},
			) = $packet->unpack('f3V');
			push @{$self->{points}}, \%point;
			$i++;
		}
	} else {
		while ($i < $point_count) {
			my %point;
			(
				$point{position}[0],
				$point{position}[1],
				$point{position}[2],
				$point{flags},
				$point{name},
			) = $packet->unpack('f3VZ*');
			push @{$self->{points}}, \%point;
			$i++;
		}	
	}
}
sub read_links {
	my $self = shift;
	my ($cf) = @_;
	my $data = $cf->r_chunk_data();
	my $links_count = unpack('v', $data);
	my $packet = stkutils::data_packet->new(substr($data, 2));
	my $i = 0;
	if (::use_1154()) {
		while ($i < $links_count) {
			my %link;
			my $from_point;
			($from_point, $link{to}) = $packet->unpack('vv');
			my $point = $self->{points}[$from_point];
			push @{$point->{links}}, \%link;
			$i++;
		}
	} else {
		while ($i < $links_count) {
			my %link;
			my $from_point;
			($from_point, $link{to}, $link{weight}) = $packet->unpack('vvf');
			my $point = $self->{points}[$from_point];
			push @{$point->{links}}, \%link;
			$i++;
		}	
	}
}
sub spawn_write {
	my $self = shift;
	my ($cf, $object_id) = @_;
	
	$cf->w_chunk_open(1);
	$cf->w_chunk_data(pack('v', 0x12));
	$cf->w_chunk_close();
	
	$cf->w_chunk_open(5);
	$cf->w_chunk_data(pack('Z*', $self->{name}));
	$cf->w_chunk_close();

if (::use_1154()) {
	$cf->w_chunk_open(4);
	$cf->w_chunk_data(pack('V', 0));
	$cf->w_chunk_close();
}
	$cf->w_chunk_open(2);
	$cf->w_chunk_data(pack('v', $#{$self->{points}} + 1));
	my $links_data = '';
	my $point_id = 0;
	foreach my $p (@{$self->{points}}) {
		if (defined $p->{name}) {
			my $data = pack('f3VZ*',
			@{$p->{position}},
			$p->{flags},
			$p->{name});
			$cf->w_chunk_data($data);
		} else {
			my $data = pack('f3V',
			@{$p->{position}},
			$p->{flags});
			$cf->w_chunk_data($data);			
		}
	}
	$cf->w_chunk_close();
	$cf->w_chunk_open(3);
	my $id = 0;
	foreach my $p (@{$self->{points}}) {
		if ($#{$p->{links}} != -1) {
			if (::use_1154()) {
				foreach my $l (@{$p->{links}}) {
					$links_data .= pack('vv', $point_id, $l->{to}) if defined $l->{to};
					$id++ if defined $l->{to};
				}
			} else {
				foreach my $l (@{$p->{links}}) {
					$links_data .= pack('vvf', $point_id, $l->{to}, $l->{weight});
					$id++;
				}
			
			}
		}
		$point_id++;
	}
	$cf->w_chunk_data(pack('v', $id));
	$cf->w_chunk_data($links_data);
	$cf->w_chunk_close();
}
sub state_import {
	my $self = shift;
	my ($if, $section) = @_;

	$self->{name} = $section;

	my $points = $if->value($section, 'points');
	die "no points in $section\n" unless defined $points;
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
			$point{links0} = $if->value($section, "$id:links");

			push @{$self->{points}}, \%point;

			$index_by_id{$id} = $i++;
		}
		foreach my $p (@{$self->{points}}) {
			next unless defined $p->{links0};
			foreach my $l (split /,/, $p->{links0}) {
				if (::use_1154()) {
					$l =~ /^\s*(\w+)\s*$/;
#					print "$l\n";
#					print "$1\n";
					die unless defined $1;
					my %link;
					$link{to} = $index_by_id{$1};
					push @{$p->{links}}, \%link;
				} else {
					$l =~ /^\s*(\w+)\s*\((\S+)\)\s*$/;
					die unless defined $1;
					die unless defined $2;
					my %link;
					$link{to} = $index_by_id{$1};
					$link{weight} = $2;
					push @{$p->{links}}, \%link;
				}
			}
			delete $p->{links0};
		}
}
sub state_import_freaky {
	my $self = shift;
	my ($if, $section) = @_;

	$self->{index} = $section;
	my $position = $if->value($section, "position");
	@{$self->{position}} = split /,/, $position;
	my $direction = $if->value($section, "direction");
	@{$self->{direction}} = split /,/, $direction;
	my $flags = $if->value($section, "flags");
	if (!defined $flags) {
		$flags = 0;
	} elsif ($flags =~ /^\s*0[xX]/) {
		$flags = hex $flags;
	}
	$self->{flags} = $flags;
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
	if (::use_1154()) {
		foreach my $p (@{$self->{points}}) {
			my $id = "p$i";
			printf ($fh "$id:flags = %#x\n", $p->{flags}) unless $p->{flags} == 0;
			print $fh "$id:position = ", join(',', @{$p->{position}}), "\n";
			if ($#{$p->{links}} != -1) {
				my @links;
				my $j = 0;
				foreach my $l (@{$p->{links}}) {
					push @links, "p$l->{to}";
					$j++;
				}
				print $fh "$id:links = ", join(',', @links), "\n"; ;
			}
			$i++;
			print $fh "\n";
		}
	} else {
		foreach my $p (@{$self->{points}}) {
			my $id = "p$i";
			print $fh "$id:name = $p->{name}\n";
			printf ($fh "$id:flags = %#x\n", $p->{flags}) unless $p->{flags} == 0;
			print $fh "$id:position = ", join(',', @{$p->{position}}), "\n";
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
	
	}
	print $fh "\n";
}
sub state_export_freaky {
	my $self = shift;
	my ($if, $id) = @_;
	my $fh = $if->{fh};
	my $i = 0;
	foreach my $p (@{$self->{points}}) {
		print $fh "[$i]\n";
		printf ($fh "flags = %#x\n", $p->{flags}) unless $p->{flags} == 0;
		print $fh "position = ", join(',', @{$p->{position}}), "\n";
		print $fh "direction = ", join(',', @{$p->{direction}}), "\n";

		$i++;
		print $fh "\n";
	}
	print $fh "\n";
}
sub state_export_1465 {
	my $self = shift;
	my ($if, $id) = @_;
	my $fh = $if->{fh};
	my $i = 0;
	foreach my $p (@{$self->{points}}) {
		print $fh "[$i]\n";
		print $fh "position = ", join(',', @{$p->{position}}), "\n";
		$i++;
		print $fh "\n";
	}
	print $fh "\n";
}
#######################################################################
package all_spawn;
use strict;
use IO::File;
use stkutils::ini_file;
use stkutils::chunked_file;
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
	my ($index, $size) = $cf->r_chunk_open;
	defined ($index) or last;
	print "reading way objects...\n";
	if (::freaky()) {
		$cf->r_chunk_close();
		my ($index, $size) = $cf->r_chunk_open;
		my $object = way_object->new();
		$object->spawn_read_freaky($cf);
		push @{$self->{way_objects}}, $object;
		$cf->r_chunk_close();
	} elsif (::use_1465()) {
		$cf->r_chunk_close();
		my ($index, $size) = $cf->r_chunk_open;
		$cf->r_chunk_close();
		($index, $size) = $cf->r_chunk_open;
		my $object = way_object->new();
		$object->spawn_read_1465($cf);
		push @{$self->{way_objects}}, $object;
		$cf->r_chunk_close();		
	} else {
		while (1) {
			my ($index, $size) = $cf->r_chunk_open();
			defined($index) or last;
			my $object = way_object->new();
			$object->spawn_read($cf);
			push @{$self->{way_objects}}, $object;
			$cf->r_chunk_close();
		}
	}
	$cf->close();
}
sub write {
	my $self = shift;
	my ($fn) = @_;

	my $cf = stkutils::chunked_file->new($fn, 'w') or die;
	print "writing way objects...\n";
	$cf->w_chunk_open(4096);
	my $id = 0;
	if (::freaky()) {
		$cf->w_chunk_close();
		$cf->w_chunk_open(8192);
		foreach my $object (@{$self->{way_objects}}) {
			$cf->w_chunk_open($id);
			my $data = pack('f3f3V',
			@{$object->{position}},
			@{$object->{direction}},
			$object->{flags});
			$cf->w_chunk_data($data);
			$cf->w_chunk_close();
			$id++;
		}
	} elsif (::use_1465()) {
		$cf->w_chunk_close();
		$cf->w_chunk_open(8192);
		$cf->w_chunk_close();
		$cf->w_chunk_open(8193);
		foreach my $object (@{$self->{way_objects}}) {
			$cf->w_chunk_open($id);
			my $data = pack('f3',@{$object->{position}});
			$cf->w_chunk_data($data);
			$cf->w_chunk_close();
			$id++;
		}
	} else {
		foreach my $object (@{$self->{way_objects}}) {
			$cf->w_chunk_open($id);
			$object->spawn_write($cf, $id++);
			$cf->w_chunk_close();
		}
		if (::use_1154()) {
			$cf->w_chunk_close();
			$cf->w_chunk_open(4097);
			$cf->w_chunk_close();	
			$cf->w_chunk_open(4098);
			$cf->w_chunk_close();	
			$cf->w_chunk_open(4099);
		} else {
			$cf->w_chunk_close();
			$cf->w_chunk_open(8192);	
		}
	}
	$cf->w_chunk_close();
	$cf->close();
}
sub import {
	my $self = shift;
	my ($fn) = @_;
	my ($if) = stkutils::ini_file->new($fn, 'r') or die;
	print "importing way objects from level $fn...\n";
	if (::freaky()) {
		foreach my $section (@{$if->{sections_list}}) {
			my $object = way_object->new();
			$object->state_import_freaky($if, $section);
			push @{$self->{way_objects}}, $object;
		}
	} elsif (::use_1465()) {
		foreach my $section (@{$if->{sections_list}}) {
			my $object = way_object->new();
			$object->{index} = $section;
			my $position = $if->value($section, "position");
			@{$object->{position}} = split /,/, $position;
			push @{$self->{way_objects}}, $object;
		}
	} else {
		foreach my $section (@{$if->{sections_list}}) {
			my $object = way_object->new();
			$object->state_import($if, $section);
			push @{$self->{way_objects}}, $object;
		}
	
	}
	$if->close();
}
sub export {
	my $self = shift;
	my ($fn) = @_;

	my $if = stkutils::ini_file->new($fn, 'w') or die;
	my $id = 0;
	if (::freaky()) {
		foreach my $object (@{$self->{way_objects}}) {
			$object->state_export_freaky($if, $id++);
		}
	} elsif (::use_1465()) {
		foreach my $object (@{$self->{way_objects}}) {
			$object->state_export_1465($if, $id++);
		}		
	} else {
		foreach my $object (@{$self->{way_objects}}) {
			$object->state_export($if, $id++);
		}	
	}
	print "done!\n";
	$if->close();
}
#######################################################################
package main;
use strict;
use Getopt::Long;
use File::Path;

sub usage {
	return <<END
S.T.A.L.K.E.R. level.game compiler/decompiler
Usage: wcdc -d level.game [-o outdir]
       wcdc -c level_game.ltx [-o outfile] [-f] [-1154] [-1465]
END
}

my $way_file;
my $src_file;
my $out;
my $freaky;
my $use_1465;
my $use_1154;

GetOptions(
	'd=s' => \$way_file,
	'c=s' => \$src_file,
	'o=s' => \$out,
	'f' => \$freaky,
	'1465' => \$use_1465,
	'1154' => \$use_1154,
) or die usage();

if (defined $way_file) {
	die "bad params\n" if (defined $src_file);
	my $way = all_spawn->new();
	print "reading $way_file...\n";	
	$way->read($way_file);
	defined $out && do {
		File::Path::mkpath($out, 0);
		chdir $out or die "cannot change path to $out\n";
	};
	print "exporting way objects...\n";	
	$way->export('level_game.ltx');
} elsif (defined $src_file) {
	die "bad params\n" if defined $way_file;
	my $way = all_spawn->new();
	$out = 'level.game.new' unless defined $out;
	print "importing way objects...\n";
	$way->import($src_file);
	print "writing $out...\n";
	$way->write($out);
	print "done!\n";
} else {
	die usage();
}
sub freaky {return defined $freaky};
sub use_1465 {return defined $use_1465};
sub use_1154 {return defined $use_1154};
#######################################################################