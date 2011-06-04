#!perl -w -I \temp\1\bin

#######################################################################
package le_light;
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
			$id == 0x8003 && do { read_light($cf); last SWITCH; };
			printf "unexpected chunk %04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_light {
	my ($cf) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7777 && do { le_common::read_last_modified($cf,0x8003); last SWITCH; };
#			$id == 0x0001 && do { read_light_0001($cf); last SWITCH; };
			$id == 0x0002 && do { read_light_0002($cf); last SWITCH; };
			$id == 0x0003 && do { read_light_0003($cf); last SWITCH; };
			$id == 0x1001 && do { read_light_1001($cf); last SWITCH; };
			$id == 0x1002 && do { read_light_1002($cf); last SWITCH; };
			$id == 0x1003 && do { read_light_1003($cf); last SWITCH; };
			$id == 0x1004 && do { read_light_1004($cf); last SWITCH; };
			$id == 0x1006 && do { read_light_1006($cf); last SWITCH; };
			printf "unexpected chunk 8003,%04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_light_0001 {
	my ($cf) = @_;
	printf "chunk 8003,0001\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($unk) = $packet->unpack('v');
	die unless $unk == 3;
	die unless $packet->length() == 0;
	printf "  unk: %d\n", $unk;
}
sub read_light_0002 {
	my ($cf) = @_;
	printf "chunk 8003,0002\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  object count?: %d\n", $count;
}
sub read_light_0003 {
	my ($cf) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		if ($id != $expected_id) {
			printf "unexpected chunk 8003,0003,%04x\n", $id;
			die;
		}
		read_light_0003_nnnn($cf, $id);
		$cf->r_chunk_close();
	}
}
sub read_light_0003_nnnn {
	my ($cf, $nnnn) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7703 && do { read_light_0003_nnnn_7703($cf,$nnnn); last SWITCH; };
			$id == 0x7777 && do { read_light_0003_nnnn_7777($cf,$nnnn); last SWITCH; };
			printf "unexpected chunk 8003,0003,%04x,%04x\n", $nnnn, $id;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_light_0003_nnnn_7703 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8003,0003,%04x,7703\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($unk) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  unk: %d\n", $unk;
}
sub read_light_0003_nnnn_7777 {
	my ($cf, $nnnn) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0xb411 && do { read_light_0003_nnnn_7777_b411($cf,$nnnn); last SWITCH; };
			$id == 0xb413 && do { read_light_0003_nnnn_7777_b413($cf,$nnnn); last SWITCH; };
			$id == 0xb436 && do { read_light_0003_nnnn_7777_b436($cf,$nnnn); last SWITCH; };
			$id == 0xb440 && do { read_light_0003_nnnn_7777_b440($cf,$nnnn); last SWITCH; };
			$id == 0xb441 && do { read_light_0003_nnnn_7777_b441($cf,$nnnn); last SWITCH; };
			$id == 0xb442 && do { read_light_0003_nnnn_7777_b442($cf,$nnnn); last SWITCH; };
			$id == 0xf903 && do { read_light_0003_nnnn_7777_f903($cf,$nnnn); last SWITCH; };
			$id == 0xf906 && do { read_light_0003_nnnn_7777_f906($cf,$nnnn); last SWITCH; };
			$id == 0xf907 && do { read_light_0003_nnnn_7777_f907($cf,$nnnn); last SWITCH; };
			printf "unexpected chunk 8003,0003,%04x,7777,%04x\n", $nnnn, $id;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_light_0003_nnnn_7777_b411 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8003,0003,%04x,7777,b411\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('v');
	die unless $packet->length() == 0;
	printf "  version: %#x\n", $version;
}
sub read_light_0003_nnnn_7777_b413 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8003,0003,%04x,7777,b413\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($flags) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  flags: %#08x\n", $flags;
	printf "    Light::Items::Usage::Light Map: %s\n", ($flags & 0x1) ? "yes" : "no";
	printf "    Light::Items::Usage::Dynamic: %s\n", ($flags & 0x2) ? "yes" : "no";
	printf "    Light::Items::Usage::Animated: %s\n", ($flags & 0x4) ? "yes" : "no";
	printf "    Light::Items::Fuzzy: %s\n", ($flags & 0x10) ? "yes" : "no";
}
sub read_light_0003_nnnn_7777_b436 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8003,0003,%04x,7777,b436\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($flags) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  flags: %#08x\n", $flags;
	printf "    Light::Items::Use in D3D: %s\n", ($flags & 0x1) ? "yes" : "no";
}
sub read_light_0003_nnnn_7777_b440 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8003,0003,%04x,7777,b440\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @h = $packet->unpack('Cf4v');
	my @v = $packet->unpack("(f3)$h[5]");
	die unless $packet->length() == 0;
	if ($h[0] == 0) {
		printf "  Light::Items::Fuzzy::Shape: Sphere\n";
		printf "  Light::Items::Fuzzy::Radius: %f, %f, %f, %f\n", @h[1..4];
	} elsif ($h[0] == 1) {
		printf "  Light::Items::Fuzzy::Shape: Box\n";
		printf "  Light::Items::Fuzzy::Half Dimension: %f, %f, %f, %f\n", @h[1..4];
	} else {
		die;
	}
	printf "  vertex count: %d\n", $h[5];
	for (my $i = 0; $i < $h[5]; $i++) {
		my $o = $i*3;
		printf "    vertex%d: %f, %f, %f\n", $i, @v[$o .. $o + 2];
	}
}
sub read_light_0003_nnnn_7777_b441 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8003,0003,%04x,7777,b441\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($index) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  Light::Items::Light Control: %d\n", $index;
}
sub read_light_0003_nnnn_7777_b442 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8003,0003,%04x,7777,b442\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('Vf4f5fV');
	die unless $packet->length() == 0;
	printf "  Light::Items::Type?: %d\n", $v[0];
	printf "  Light::Items::Color: %f, %f, %f, %f\n", @v[1 .. 4];
	printf "  Light::Items::Brightness: %f\n", $v[5];
	printf "  Light::Items::Range: %f\n", $v[6];
	printf "  Light::Items::Attenuation::Constant: %f\n", $v[7];
	printf "  Light::Items::Attenuation::Linear: %f\n", $v[8];
	printf "  Light::Items::Attenuation::Quadratic: %f\n", $v[9];
	printf "  unknown: %f\n", $v[10];
	printf "  unknown: %x\n", $v[11];
}
sub read_light_0003_nnnn_7777_f903 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8003,0003,%04x,7777,f903\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('f3f3f3');
	die unless $packet->length() == 0;
	printf "  Light::Items::Transform::Position: %f, %f, %f\n", @v[0 .. 2];
	printf "  Light::Items::Transform::Rotation: %f, %f, %f\n", @v[3 .. 5];
	printf "  Light::Items::Transform::Scale: %f, %f, %f\n", @v[6 .. 8];
}
sub read_light_0003_nnnn_7777_f906 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8003,0003,%04x,7777,f906\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($unk) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  custom object flags: %d\n", $unk;
}
sub read_light_0003_nnnn_7777_f907 {
	my ($cf, $nnnn) = @_;
	printf "chunk 8003,0003,%04x,7777,f907\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($name) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  Light::Items::Name: '%s'\n", $name;
}
sub read_light_1001 {
	my ($cf) = @_;
	printf "chunk 8003,1001\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('v');
	die unless $packet->length() == 0;
	printf "  version?: %d\n", $version;
}
sub read_light_1002 {
	my ($cf) = @_;
	printf "chunk 8003,1002\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($static, $static_id, $hemi, $hemi_id, $sun, $sun_id) = $packet->unpack('Z*VZ*VZ*V');
	die unless $packet->length() == 0;
	printf "  Light::Common::Controls::System::'%s': %d\n", $static, $static_id;
	printf "  Light::Common::Controls::System::'%s': %d\n", $hemi, $hemi_id;
	printf "  Light::Common::Controls::System::'%s': %d\n", $sun, $sun_id;
}
sub read_light_1003 {
	my ($cf) = @_;
	printf "chunk 8003,1003\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  Light::Common::Controls::Count?: %d\n", $count;
}
sub read_light_1004 {
	my ($cf) = @_;
	printf "chunk 8003,1004\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($flags) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  flags: %#x\n", $flags;
	printf "    Light::Common::Controls::Draw Name: %s\n", ($flags & 0x40) ? "yes" : "no";
}
sub read_light_1006 {
	my ($cf) = @_;
	printf "chunk 8003,1006\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($visible, $altitude, $longitude) = $packet->unpack('Cff');
	die unless $packet->length() == 0;
	printf "  Light::Common::Sun Shadow::Visible: %s\n", $visible ? "yes" : "no";
	printf "  Light::Common::Sun Shadow::Altitude: %f\n", $altitude;
	printf "  Light::Common::Sun Shadow::Longitude: %f\n", $longitude;
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
LevelEditor light.part dumper
Usage: le_dump_light.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage;
le_light::read($fn);
