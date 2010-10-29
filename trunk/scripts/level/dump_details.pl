#!perl -w -I \temp\1\bin

use constant FSD_HEADER		=> 0x0;
use constant FSD_MESHES		=> 0x1;
use constant FSD_SLOTS		=> 0x2;

use constant chunk_names => {
	0x0		=> 'FSD_HEADER',
	0x1		=> 'FSD_MESHES',
	0x2		=> 'FSD_SLOTS',
};

#######################################################################
package fsd_header;
use strict;
sub read {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version, $object_count, $offs_x, $offs_z, $size_x, $size_z) =
		$packet->unpack('VVllVV');
	die unless $packet->length() == 0;
	printf "  version: %d\n", $version;
	die unless $version == 3;
	printf "  object_count: %d\n", $object_count;
	printf "  offs_x: %d\n", $offs_x;
	printf "  offs_z: %d\n", $offs_z;
	printf "  size_x: %d\n", $size_x;
	printf "  size_z: %d\n", $size_z;
}

#######################################################################
package fsd_meshes;
use strict;
sub read {
	my ($cf) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		printf "  detail%d\n", $id;
		my $packet = stkutils::data_packet->new($cf->r_chunk_data());
		read_detail($packet);
		$cf->r_chunk_close();
	}
}
sub read_detail {
	my ($packet) = @_;
	my ($shader, $textures, $flags, $min_scale, $max_scale, $number_vertices, $number_indices) =
		$packet->unpack('Z*Z*VffVV');
	printf "    shader: '%s'\n", $shader;
	printf "    textures: '%s'\n", $textures;
	printf "    flags: %#x\n", $flags;
	printf "    min_scale: %f\n", $min_scale;
	printf "    max_scale: %f\n", $max_scale;
	printf "    number_vertices: %d\n", $number_vertices;
	my @vert = $packet->unpack("(f3ff)$number_vertices");
	for (my $i = 0; $i < $number_vertices; $i++) {
		my $o = $i*5;
		printf "      vertex%d\n", $i;
		printf "        position: %f, %f, %f\n", @vert[$o + 0 .. $o + 2];
		printf "        u, v: %f, %f\n", @vert[$o + 3 .. $o + 4];
	}
	printf "    number_indices: %d\n", $number_indices;
	my @ind = $packet->unpack("v$number_indices");
	die unless $packet->length() == 0;
}

#######################################################################
package fsd_slots;
use strict;
sub read {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	die unless ($packet->length() % 16) == 0;
	my $slot_count = $packet->length() / 16;
	printf "  slot count: %d\n", $slot_count;
	my @v = $packet->unpack("(VVvvvv)$slot_count");
	die unless $packet->length() == 0;
	for (my $i = 0; $i < $slot_count; $i++) {
		my $o = $i*6;
		printf "    slot%d\n", $i;
		printf "      y_base: %d\n", $v[$o] & 0x3ff;
		printf "      y_height: %d\n", ($v[$o] >> 12) & 0xff;
		printf "      id0: %d\n", ($v[$o] >> 20) & 0x3f;
		printf "      id1: %d\n", ($v[$o] >> 26) & 0x3f;
		printf "      id2: %d\n", $v[$o+1] & 0x3f;
		printf "      id3: %d\n", ($v[$o+1] >> 6) & 0x3f;
		printf "      c_dir: 0x%01x\n", ($v[$o+1] >> 12) & 0xf;
		printf "      c_hemi: 0x%01x\n", ($v[$o+1] >> 16) & 0xf;
		printf "      c_r: 0x%01x\n", ($v[$o+1] >> 20) & 0xf;
		printf "      c_g: 0x%01x\n", ($v[$o+1] >> 24) & 0xf;
		printf "      c_b: 0x%01x\n", ($v[$o+1] >> 28) & 0xf;
		for (my $j = 2; $j < 6; $j++) {
			printf "      palette%d\n", $j-2;
			printf "        a0: %d\n", ($v[$o+$j] >> 0) & 0xf;
			printf "        a1: %d\n", ($v[$o+$j] >> 4) & 0xf;
			printf "        a2: %d\n", ($v[$o+$j] >> 8) & 0xf;
			printf "        a3: %d\n", ($v[$o+$j] >> 12) & 0xf;
		}
	}
}

#######################################################################
package level_details;
use strict;
use stkutils::data_packet;
use stkutils::chunked_file;
sub read {
	my ($fn) = @_;
	printf "dumping '%s'\n", $fn;
	my $cf = stkutils::chunked_file->new($fn, 'r') or die;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		printf "chunk '%s' size %d(%#lx)\n", ::chunk_names->{$id}, $size, $size;
		SWITCH: {
			$id == ::FSD_HEADER && do { fsd_header::read($cf); last SWITCH; };
			$id == ::FSD_MESHES && do { fsd_meshes::read($cf); last SWITCH; };
			$id == ::FSD_SLOTS && do { fsd_slots::read($cf); last SWITCH; };
			die "unexpected chunk id $id\n";
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R. level.details dumper
Usage: dump_details.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();
level_details::read($fn);
