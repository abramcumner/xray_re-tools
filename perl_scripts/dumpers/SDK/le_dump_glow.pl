#!perl -w -I \temp\1\bin

#######################################################################
package le_glow;
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
			$id == 0x8001 && do { read_glow($cf); last SWITCH; };
			printf "unexpected chunk %04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_glow {
	my ($cf) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7777 && do { le_common::read_last_modified($cf,0x8001); last SWITCH; };
			$id == 0x0002 && do { read_glow_0002($cf); last SWITCH; };
			$id == 0x0003 && do { read_glow_0003($cf); last SWITCH; };
			$id == 0x1001 && do { read_glow_1001($cf); last SWITCH; };
			$id == 0x1002 && do { read_glow_1002($cf); last SWITCH; };
			printf "unexpected chunk 8001,%04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_glow_0002 {
	my ($cf) = @_;
	printf "chunk 8001,0002\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  object count: %d\n", $count;
}
sub read_glow_0003 {
	my ($cf) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		if ($id != $expected_id) {
			printf "unexpected chunk 8001,0003,%04x\n", $id;
			die;
		}
		read_glow_0003_nnnn($cf, $id);
		$cf->r_chunk_close();
	}
}
sub read_glow_0003_nnnn {
	my ($cf, $nnnn) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7703 && do { read_glow_0003_nnnn_7703($cf,$nnnn); last SWITCH; };
			$id == 0x7777 && do { read_glow_0003_nnnn_7777($cf,$nnnn); last SWITCH; };
			printf "unexpected chunk 8001,0003,%04x,%04x\n", $nnnn, $id;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_glow_0003_nnnn_7703 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8001,0003,%04x,7703\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($unk) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  object class: %d\n", $unk;
}
sub read_glow_0003_nnnn_7777 {
	my ($cf, $nnnn) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0xc411 && do { read_glow_0003_nnnn_7777_c411($cf,$nnnn); last SWITCH; };
			$id == 0xc413 && do { read_glow_0003_nnnn_7777_c413($cf,$nnnn); last SWITCH; };
			$id == 0xc414 && do { read_glow_0003_nnnn_7777_c414($cf,$nnnn); last SWITCH; };
			$id == 0xc415 && do { read_glow_0003_nnnn_7777_c415($cf,$nnnn); last SWITCH; };
			$id == 0xc416 && do { read_glow_0003_nnnn_7777_c416($cf,$nnnn); last SWITCH; };
			$id == 0xf903 && do { read_glow_0003_nnnn_7777_f903($cf,$nnnn); last SWITCH; };
			$id == 0xf906 && do { read_glow_0003_nnnn_7777_f906($cf,$nnnn); last SWITCH; };
			$id == 0xf907 && do { read_glow_0003_nnnn_7777_f907($cf,$nnnn); last SWITCH; };
			printf "unexpected chunk 8001,0003,%04x,7777,%04x\n", $nnnn, $id;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_glow_0003_nnnn_7777_c411 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8001,0003,%04x,7777,c411\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('v');
	die unless $packet->length() == 0;
	printf "  version: %#x\n", $version;
}
sub read_glow_0003_nnnn_7777_c413 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8001,0003,%04x,7777,c413\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($radius) = $packet->unpack('f');
	die unless $packet->length() == 0;
	printf "  Glow::Items::Radius: %f\n", $radius;
}
sub read_glow_0003_nnnn_7777_c414 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8001,0003,%04x,7777,c414\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($shader) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  Glow::Items::Shader: '%s'\n", $shader;
}
sub read_glow_0003_nnnn_7777_c415 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8001,0003,%04x,7777,c415\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($texture) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  Glow::Items::Texture: '%s'\n", $texture;
}
sub read_glow_0003_nnnn_7777_c416 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8001,0003,%04x,7777,c416\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($unk) = $packet->unpack('v');
	die unless $packet->length() == 0;
	printf "  unk: %d\n", $unk;
}
sub read_glow_0003_nnnn_7777_f903 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8001,0003,%04x,7777,f903\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('f3f3f3');
	die unless $packet->length() == 0;
	printf "  Glow::Items::Transform::Position: %f, %f, %f\n", @v[0 .. 2];
	printf "  Glow::Items::Transform::Rotation: %f, %f, %f\n", @v[3 .. 5];
	printf "  Glow::Items::Transform::Scale: %f, %f, %f\n", @v[6 .. 8];
}
sub read_glow_0003_nnnn_7777_f906 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8001,0003,%04x,7777,f906\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($unk) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  custom object flags: %d\n", $unk;
}
sub read_glow_0003_nnnn_7777_f907 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8001,0003,%04x,7777,f907\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($name) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  Glow::Items::Name: '%s'\n", $name;
}
sub read_glow_1001 {
	my ($cf) = @_;
	printf "chunk 8001,1001\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('v');
	die unless $version == 0;
	die unless $packet->length() == 0;
	printf "  version?: %d\n", $version;
}
sub read_glow_1002 {
	my ($cf) = @_;
	printf "chunk 8001,1002\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($flags) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  flags: %08lx\n", $flags;
	printf "    Glow::Common::Test Visibility: %s\n", ($flags & 0x80000000) ? "yes" : "no";
	printf "    Glow::Common::Draw Cross: %s\n", ($flags & 0x40000000) ? "yes" : "no";
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
LevelEditor glow.part dumper
Usage: le_dump_glow.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage;
le_glow::read($fn);
