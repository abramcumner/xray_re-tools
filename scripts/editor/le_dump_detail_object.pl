#!perl -w -I \temp\1\bin

#######################################################################
package le_detail_object;
use strict;
use stkutils::data_packet;
use stkutils::chunked_file;
use le_common;
sub read {
	my ($fn) = @_;
	my $cf = stkutils::chunked_file->new($fn, 'r') or die;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7000 && do { le_common::read_guid($cf); last SWITCH; };
			$id == 0x800c && do { read_detmgr($cf); last SWITCH; };
			printf "unexpected chunk %04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_detmgr {
	my ($cf) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7777 && do { le_common::read_last_modified($cf,0x800e); last SWITCH; };
			$id == 0x0000 && do { read_detmgr_0000($cf); last SWITCH; };
			$id == 0x0001 && do { read_detmgr_0001($cf); last SWITCH; };
			$id == 0x0002 && do { read_detmgr_0002($cf); last SWITCH; };
			$id == 0x1000 && do { read_detmgr_1000($cf); last SWITCH; };
			$id == 0x1001 && do { read_detmgr_1001($cf); last SWITCH; };
			$id == 0x1002 && do { read_detmgr_1002($cf); last SWITCH; };
			$id == 0x1003 && do { read_detmgr_1003($cf); last SWITCH; };
			$id == 0x1004 && do { read_detmgr_1004($cf); last SWITCH; };
			$id == 0x1005 && do { read_detmgr_1005($cf); last SWITCH; };
			$id == 0x1006 && do { read_detmgr_1006($cf); last SWITCH; };
			printf "unexpected chunk 800c,%04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_detmgr_0000 {
	my ($cf) = @_;
	printf "chunk 800c,0000\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version, $object_count, $offs_x, $offs_z, $size_x, $size_z) =
		$packet->unpack('VVllVV');
	die unless $packet->length() == 0;
	printf "  version: %#x\n", $version;
	printf "  object_count: %d\n", $object_count;
	printf "  offs_x: %d\n", $offs_x;
	printf "  offs_z: %d\n", $offs_z;
	printf "  size_x: %d\n", $size_x;
	printf "  size_z: %d\n", $size_z;
}
sub read_detmgr_0001 {
	my ($cf) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		if ($id != $expected_id) {
			printf "unexpected chunk 800c,0001,%04x\n", $id;
			die;
		}
		read_detmgr_0001_nnnn($cf, $id);
		$cf->r_chunk_close();
	}
}
sub read_detmgr_0001_nnnn {
	my ($cf, $nnnn) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x1000 && do { read_detmgr_0001_nnnn_1000($cf, $nnnn); last SWITCH; };
			$id == 0x0101 && do { read_detmgr_0001_nnnn_0101($cf, $nnnn); last SWITCH; };
			$id == 0x0102 && do { read_detmgr_0001_nnnn_0102($cf, $nnnn); last SWITCH; };
			$id == 0x0103 && do { read_detmgr_0001_nnnn_0103($cf, $nnnn); last SWITCH; };
			$id == 0x0104 && do { read_detmgr_0001_nnnn_0104($cf, $nnnn); last SWITCH; };
			printf "unexpected chunk 800c,0001,%04x,%04x size %d\n", $nnnn, $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_detmgr_0001_nnnn_1000 {
	my ($cf, $nnnn) = @_;
	printf "chunk 800c,0001,%04x,1000\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  detobj version: %#x\n", $version;
}
sub read_detmgr_0001_nnnn_0101 {
	my ($cf, $nnnn) = @_;
	printf "chunk 800c,0001,%04x,0101\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($ref) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  Detail Objects::Items::Ref Name: '%s'\n", $ref;
}
sub read_detmgr_0001_nnnn_0102 {
	my ($cf, $nnnn) = @_;
	printf "chunk 800c,0001,%04x,0102\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($min_scale, $max_scale) = $packet->unpack('f2');
	die unless $packet->length() == 0;
	printf "  Detail Objects::Items::Min Scale: %f\n", $min_scale;
	printf "  Detail Objects::Items::Max Scale: %f\n", $max_scale;
}
sub read_detmgr_0001_nnnn_0103 {
	my ($cf, $nnnn) = @_;
	printf "chunk 800c,0001,%04x,0103\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($density) = $packet->unpack('f');
	die unless $packet->length() == 0;
	printf "  Detail Objects::Items::Density: %f\n", $density;
}
sub read_detmgr_0001_nnnn_0104 {
	my ($cf, $nnnn) = @_;
	printf "chunk 800c,0001,%04x,0104\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($flags) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  flags: %#08x\n", $flags;
	printf "    Detail Objects::Items::No Waving: %s\n", ($flags & 1) ? "yes" : "no";
}
sub read_detmgr_0002 {
	my ($cf) = @_;
	printf "chunk 800c,0002\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack("V/(VVvvvv)");
	die unless $packet->length() == 0;
	printf "  slot count: %d\n", ($#v + 1)/6;
	for (my $i = 0; $i <= $#v; $i += 6) {
		printf "    slot%d\n", $i/6;
		printf "      y_base: %d\n", $v[$i] & 0x3ff;
		printf "      y_height: %d\n", ($v[$i] >> 12) & 0xff;
		printf "      id0: %d\n", ($v[$i] >> 20) & 0x3f;
		printf "      id1: %d\n", ($v[$i] >> 26) & 0x3f;
		printf "      id2: %d\n", $v[$i+1] & 0x3f;
		printf "      id3: %d\n", ($v[$i+1] >> 6) & 0x3f;
		printf "      c_dir: 0x%01x\n", ($v[$i+1] >> 12) & 0xf;
		printf "      c_hemi: 0x%01x\n", ($v[$i+1] >> 16) & 0xf;
		printf "      c_r: 0x%01x\n", ($v[$i+1] >> 20) & 0xf;
		printf "      c_g: 0x%01x\n", ($v[$i+1] >> 24) & 0xf;
		printf "      c_b: 0x%01x\n", ($v[$i+1] >> 28) & 0xf;
		for (my $j = 2; $j < 6; $j++) {
			printf "      palette%d\n", $j-2;
			printf "        a0: %d\n", ($v[$i+$j] >> 0) & 0xf;
			printf "        a1: %d\n", ($v[$i+$j] >> 4) & 0xf;
			printf "        a2: %d\n", ($v[$i+$j] >> 8) & 0xf;
			printf "        a3: %d\n", ($v[$i+$j] >> 12) & 0xf;
		}
	}
}
sub read_detmgr_1000 {
	my ($cf) = @_;
	printf "chunk 800c,1000\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  detmgr version: %#x\n", $version;
}
sub read_detmgr_1001 {
	my ($cf) = @_;
	printf "chunk 800c,1001\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @box = $packet->unpack('f3f3');
	die unless $packet->length() == 0;
	printf "  min: %f, %f, %f\n", @box[0 .. 2];
	printf "  max: %f, %f, %f\n", @box[3 .. 5];
}
sub read_detmgr_1002 {
	my ($cf) = @_;
	printf "chunk 800c,1002\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($texture) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  Detail Objects::Base Texture: '%s'\n", $texture;
}
sub read_detmgr_1003 {
	my ($cf) = @_;
	printf "chunk 800c,1003\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($set_count) = $packet->unpack('C');
	printf "  set_count: %d\n", $set_count;
	for (my $i = 0; $i < $set_count; $i++) {
		printf "  set%d\n", $i;
		my ($r, $g, $b, $a, @ref) = $packet->unpack('C4C/(Z*)');
		printf "    color: %02x, %02x, %02x, %02x\n", $r, $g, $b, $a;
		printf "    reference count: %d\n", $#ref + 1;
		for (my $j = 0; $j <= $#ref; $j++) {
			printf "      reference%d: '%s'\n", $j, $ref[$j];
		}
	}
	die unless $packet->length() == 0;
}
sub read_detmgr_1004 {
	my ($cf) = @_;
	printf "chunk 800c,1004\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @snap = $packet->unpack('V/(Z*)');
	die unless $packet->length() == 0;
	printf "  snap object count: %d\n", $#snap + 1;
	for (my $i = 0; $i <= $#snap; $i++) {
		printf "  snap%d: '%s'\n", $i, $snap[$i];
	}
}
sub read_detmgr_1005 {
	my ($cf) = @_;
	printf "chunk 800c,1005\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($obj_per_square) = $packet->unpack('f');
	die unless $packet->length() == 0;
	printf "  Detail Objects::Objects per square: %f\n", $obj_per_square;
}
sub read_detmgr_1006 {
	my ($cf) = @_;
	printf "chunk 800c,1006\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($flags) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  flags: %#08x\n", $flags;
	printf "    Detail Objects::Common::Draw objects: %s\n", ($flags & 0x10000000) ? "yes" : "no";
	printf "    Detail Objects::Common::Draw base texture: %s\n", ($flags & 0x80000000) ? "yes" : "no";
	printf "    Detail Objects::Common::Base texture blended: %s\n", ($flags & 0x40000000) ? "yes" : "no";
	printf "    Detail Objects::Common::Draw slot boxes: %s\n", ($flags & 0x20000000) ? "yes" : "no";
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
LevelEditor detail_object.part dumper
Usage: le_dump_detail_object.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage;
le_detail_object::read($fn);
