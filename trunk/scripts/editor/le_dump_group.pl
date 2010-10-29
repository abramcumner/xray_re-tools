#!perl -w -I \temp\1\bin

#######################################################################
package le_group;
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
			$id == 0x8000 && do { read_group($cf); last SWITCH; };
			printf "unexpected chunk %04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_group {
	my ($cf) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7777 && do { le_common::read_last_modified($cf, 0x8000); last SWITCH; };
			$id == 0x0002 && do { read_group_0002($cf); last SWITCH; };
			$id == 0x0003 && do { read_group_0003($cf); last SWITCH; };
			$id == 0x1001 && do { read_group_1001($cf); last SWITCH; };
			printf "unexpected chunk 8000,%04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_group_0002 {
	my ($cf) = @_;
	printf "chunk 8000,0002\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  object count: %d\n", $count;
}
sub read_group_0003 {
	my ($cf) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		if ($id != $expected_id) {
			printf "unexpected chunk 8000,0003,%04x\n", $id;
			die;
		}
		read_group_0003_nnnn($cf, $id);
		$cf->r_chunk_close();
	}
}
sub read_group_0003_nnnn {
	my ($cf, $nnnn) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7703 && do { read_group_0003_nnnn_7703($cf,$nnnn); last SWITCH; };
			$id == 0x7777 && do { read_group_0003_nnnn_7777($cf,$nnnn); last SWITCH; };
			printf "unexpected chunk 8000,0003,%04x,%04x\n", $nnnn, $id;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_group_0003_nnnn_7703 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8000,0003,%04x,7703\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($unk) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  unk: %d\n", $unk;
}
sub read_group_0003_nnnn_7777 {
	my ($cf, $nnnn) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x0000 && do { read_group_0003_nnnn_7777_0000($cf,$nnnn); last SWITCH; };
			$id == 0x0003 && do { read_group_0003_nnnn_7777_0003($cf,$nnnn); last SWITCH; };
			$id == 0x0004 && do { read_group_0003_nnnn_7777_0004($cf,$nnnn); last SWITCH; };
			$id == 0x0005 && do { read_group_0003_nnnn_7777_0005($cf,$nnnn); last SWITCH; };
			$id == 0xf903 && do { read_group_0003_nnnn_7777_f903($cf,$nnnn); last SWITCH; };
			$id == 0xf906 && do { read_group_0003_nnnn_7777_f906($cf,$nnnn); last SWITCH; };
			$id == 0xf907 && do { read_group_0003_nnnn_7777_f907($cf,$nnnn); last SWITCH; };
			printf "unexpected chunk 8000,0003,%04x,7777,%04x\n", $nnnn, $id;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_group_0003_nnnn_7777_0000 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8000,0003,%04x,7777,0000\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('v');
	die unless $packet->length() == 0;
	printf "  groupobj version: %#x\n", $version;
}
sub read_group_0003_nnnn_7777_0003 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8000,0003,%04x,7777,0003\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($state) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  Group::Items::State: %d\n", $state;
}
sub read_group_0003_nnnn_7777_0004 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8000,0003,%04x,7777,0004\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($ref) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  Group::Items::Reference: '%s'\n", $ref;
}
sub read_group_0003_nnnn_7777_0005 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8000,0003,%04x,7777,0005\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('V/(Z*)');
	die unless $packet->length() == 0;
	printf "  count: %d\n", $#v + 1;
	for (my $i = 0; $i <= $#v; $i++) {
		printf "  item%d: '%s'\n", $i, $v[$i];
	}
}
sub read_group_0003_nnnn_7777_f903 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8000,0003,%04x,7777,f903\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('f3f3f3');
	die unless $packet->length() == 0;
	printf "  Group::Items::Transform::Position: %f, %f, %f\n", @v[0 .. 2];
	printf "  Group::Items::Transform::Rotation: %f, %f, %f\n", @v[3 .. 5];
	printf "  Group::Items::Transform::Scale: %f, %f, %f\n", @v[6 .. 8];
}
sub read_group_0003_nnnn_7777_f906 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8000,0003,%04x,7777,f906\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($unk) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  custom object flags: %d\n", $unk;
}
sub read_group_0003_nnnn_7777_f907 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8000,0003,%04x,7777,f907\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($name) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  Group::Items::Name: '%s'\n", $name;
}
sub read_group_1001 {
	my ($cf) = @_;
	printf "chunk 8000,1001\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($unk) = $packet->unpack('v');
	die unless $packet->length() == 0;
	printf "  unk: %d\n", $unk;
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
LevelEditor group.part dumper
Usage: le_dump_group.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage;
le_group::read($fn);
