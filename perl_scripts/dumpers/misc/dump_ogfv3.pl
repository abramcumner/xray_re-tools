#!perl -w -I \temp\1\bin

use constant OGF_VERTEXFORMAT_FVF_1L	=> 0x12071980;
use constant OGF_VERTEXFORMAT_FVF_2L	=> 0x240e3300;
use constant OGF_VERTEXFORMAT_FVF_NL	=> 0x36154c80;
use constant OGF_VERTEXFORMAT_FVF_OLD	=> 0x00000112;

use constant chunk_names => {
0x1	=> 'OGF3_HEADER',
0x2	=> 'OGF3_TEXTURE',
0x5	=> 'OGF3_CHILDREN_L',
0x6	=> 'OGF3_BBOX',
0x7	=> 'OGF3_VERTICES',
0x8	=> 'OGF3_INDICES',
0x9	=> 'OGF3_LODDATA',
0xb	=> 'OGF3_BSPHERE',
0xd	=> 'OGF3_S_BONE_NAMES',
0xe	=> 'OGF3_S_MOTIONS',
0x11	=> 'OGF3_CHILDREN',
0x12	=> 'OGF3_S_SMPARAMS',
};

use constant mt_names => {
0	=> 'MT_NORMAL',
1	=> 'MT_HIERRARHY',
2	=> 'MT_PROGRESSIVE',
3	=> 'MT_SKELETON_GEOMDEF_PM',
4	=> 'MT_SKELETON_ANIM',
7	=> 'MT_SKELETON_GEOMDEF_ST',
};

#######################################################################
package ogfv3;
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
		printf "top chunk '%s' size %d(%#lx)\n", ::chunk_names->{$id}, $size, $size;
		SWITCH: {
			$id == 0x0001 && do { read_header($cf); last SWITCH; };
			$id == 0x0002 && do { read_texture($cf); last SWITCH; };
			$id == 0x0005 && do { read_children_l($cf); last SWITCH; };
			$id == 0x0006 && do { read_bbox($cf); last SWITCH; };
			$id == 0x0007 && do { read_vertices($cf); last SWITCH; };
			$id == 0x0008 && do { read_indices($cf); last SWITCH; };
			$id == 0x0009 && do { read_0009($cf); last SWITCH; };
			$id == 0x000b && do { read_bsphere($cf); last SWITCH; };
			$id == 0x000d && do { read_s_bone_names($cf); last SWITCH; };
			$id == 0x000e && do { read_s_motions($cf); last SWITCH; };
			$id == 0x0011 && do { read_children($cf); last SWITCH; };
			$id == 0x0012 && do { read_s_smparams($cf); last SWITCH; };
			printf "unexpected chunk %04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_children {
	my ($cf) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($nnnn, $size) = $cf->r_chunk_open();
		defined $nnnn or last;
		if ($nnnn != $expected_id) {
			printf "unexpected chunk 0011,%04x\n", $nnnn;
			die;
		}
		printf "child %d size %d(%#lx)\n", $nnnn, $size, $size;
		while (1) {
			my ($id, $size) = $cf->r_chunk_open();
			defined $id or last;
			printf "child chunk '%s' size %d(%#lx)\n", ::chunk_names->{$id}, $size, $size;
			SWITCH: {
				$id == 0x0001 && do { read_header($cf); last SWITCH; };
				$id == 0x0002 && do { read_texture($cf); last SWITCH; };
				$id == 0x0006 && do { read_bbox($cf); last SWITCH; };
				$id == 0x0007 && do { read_vertices($cf); last SWITCH; };
				$id == 0x0008 && do { read_indices($cf); last SWITCH; };
				$id == 0x0009 && do { read_0009($cf); last SWITCH; };
				printf "unexpected chunk %04x size %d\n", $id, $size;
				die;
			}
			$cf->r_chunk_close();
		}
		$cf->r_chunk_close();
	}
}
sub read_children_l {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('V/(Z*)');
	printf "children count: %d\n", $#v + 1;
	for (my $i = 0; $i <= $#v; $i++) {
		printf "  child%d ref: '%s'\n", $i, $v[$i];
	}
	die unless $packet->length() == 0;
}
sub read_header {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	printf "header\n";
	my ($format_version, $type, $shader_id) = $packet->unpack('CCv');
	die unless $packet->length() == 0;
	printf "  format_version: %d\n", $format_version;
	printf "  type: %d (%s)\n", $type, ::mt_names->{$type};
	printf "  shader_id: %d\n", $shader_id;
	die unless $packet->length() == 0;
}
sub read_texture {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($texture_name, $shader_name) = $packet->unpack('Z*Z*');
	printf "texture\n";
	printf "  texture_name: %s\n", $texture_name;
	printf "  shader_name: %s\n", $shader_name;
	die unless $packet->length() == 0;
}
sub read_bbox {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	printf "bbox\n";
	my @v = $packet->unpack('f3f3');
	die unless $packet->length() == 0;
	printf "  min: %f, %f, %f\n", @v[0 .. 2];
	printf "  max: %f, %f, %f\n", @v[3 .. 5];
}
sub read_vertices {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($vertex_format, $count) = $packet->unpack('VV');
	printf "  vertex_format: %#8.8x\n", $vertex_format;
	printf "  count: %d\n", $count;
	if ($vertex_format == ::OGF_VERTEXFORMAT_FVF_1L) {
		my @v = $packet->unpack("(f8l)$count");
		for (my $i = 0; $i < $count; $i++) {
			my $o = $i*9;
			printf "  vertex%d\n", $i;
			printf "    point: %f, %f, %f\n", @v[$o+0..$o+2];
			printf "    normal: %f, %f, %f\n", @v[$o+3..$o+5];
			printf "    u, v: %f, %f\n", @v[$o+6 .. $o+7];
			printf "    matrix: %d\n", $v[$o+8];
		}
	} elsif ($vertex_format == ::OGF_VERTEXFORMAT_FVF_OLD) {
		my @v = $packet->unpack("(f8)$count");
		for (my $i = 0; $i < $count; $i++) {
			my $o = $i*8;
			printf "  vertex%d\n", $i;
			printf "    point: %f, %f, %f\n", @v[$o+0..$o+2];
			printf "    normal: %f, %f, %f\n", @v[$o+3..$o+5];
			printf "    u, v: %f\n", @v[$o+6 .. $o+7];
		}
	} else {
			die "unsupported fvf";
	}
	die unless $packet->length() == 0;
}
sub read_indices {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	printf "  index count: %d\n", $count;
	my @v = $packet->unpack("v$count");
	die unless ($count % 3) == 0;
	for (my $i = 0; $i < $count; $i++) {
		printf "    index%d: %d\n", $i, $v[$i];
	}
	die unless $packet->length() == 0;
}
sub read_0009 {
	printf "  lod information?\n";
}
sub read_bsphere {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('f4');
	printf "bsphere\n";
	printf "  center: %f, %f, %f\n  radius: %f\n", @v;
	die unless $packet->length() == 0;
}
sub read_s_bone_names {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	printf "s_bone_names: count=%d\n", $count;
	for (my $i = 0; $i < $count; $i++) {
		my ($name, $parent_name) = $packet->unpack('Z*Z*');
		printf "  bone%d\n", $i;
		printf "    name: '%s'\n", $name;
		printf "    parent_name: '%s'\n", $parent_name;
		my @obb = $packet->unpack('f15');
		printf "    obb\n      rotate\n        %f, %f, %f\n", @obb[0..2];
		printf "        %f, %f, %f\n", @obb[3..5];
		printf "        %f, %f, %f\n", @obb[6..8];
		printf "      translate: %f, %f, %f\n", @obb[9 .. 11];
		printf "      halfsize: %f, %f, %f\n", @obb[12 .. 14];
	}
	die unless $packet->length() == 0;
}
sub read_s_motions {
	my ($cf) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($nnnn, $size) = $cf->r_chunk_open();
		defined $nnnn or last;
		if ($nnnn != $expected_id) {
			printf "unexpected chunk 000e,%04x\n", $nnnn;
			die;
		}
		SWITCH: {
			$nnnn == 0 && do { read_s_motions_count($cf); last SWITCH; };
			read_s_motions_values($cf, $nnnn);
		}
		$cf->r_chunk_close();
	}
}
sub read_s_motions_count {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	printf "  motion count: %d\n", $count;
	die unless $packet->length() == 0;
}
sub read_s_motions_values {
	my ($cf, $nnnn) = @_;
	printf "  motion%d\n", $nnnn - 1;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($name, $count) = $packet->unpack('Z*V');
	printf "    name: '%s'\n", $name;
	printf "    keys count: %d\n", $count;
	if ($count == 0) {
		die unless $packet->length() == 0;
		return;
	}
	die unless $packet->length() % 20 == 0;
	for (my $i = 0; $packet->length() > 0; $i++) {
		my @v = $packet->unpack("(s4f3)$count");
		printf "    bone%d\n", $i;
		for (my $j = 0; $j <= $#v; $j += 7) {
			my @r = @v[$j .. $j + 3];
			dequantize_qr(\@r);
			printf "      key%d\n", $j/7;
			printf "        r: %f, %f, %f, %f\n", @r;
			printf "        t: %f, %f, %f\n",  @v[$j + 4 .. $j + 6];
		}
	}
	die unless $packet->length() == 0;
}
sub dequantize_qr {
	my ($v) = @_;
	foreach my $e (@$v) {
		$e *= 1.0/32767.0;
	}
}
use constant motion_type_names => {
	0	=> 'fx',
	0x1	=> 'cycle',
};
sub read_s_smparams {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($partition_count) = $packet->unpack('v');
	printf "  partition count: %d\n", $partition_count;
	for (my $i = 0; $i < $partition_count; $i++) {
		my ($partition_name) = $packet->unpack('Z*');
		printf "  partition%d\n", $i;
		printf "    name: '%s'\n", $partition_name;
		my ($bone_count) = $packet->unpack('v');
		for (my $i = 0; $i < $bone_count; $i++) {
			my ($bone_id) = $packet->unpack('V');
			printf "    bone%d\n", $i;
			printf "      id: %d\n", $bone_id;
		}
	}
	my ($motion_count) = $packet->unpack('v');
	for (my $i = 0; $i < $motion_count; $i++) {
		my ($motion_name, $motion_type) = $packet->unpack('Z*C');
		printf "  motion%d\n", $i;
		printf "    name: '%s'\n", $motion_name;
		printf "    type: %d (%s)\n", $motion_type, motion_type_names->{$motion_type};
		my @v = $packet->unpack('vvffffC');
		printf "    bone_or_part: %d\n", $v[0];
		printf "    motion: %d\n", $v[1];
		printf "    speed: %f\n", $v[2];
		printf "    power: %f\n", $v[3];
		printf "    accrue: %f\n", $v[4];
		printf "    falloff: %f\n", $v[5];
		printf "    stop\@end: %s\n", $v[6] == 0 ? "off" : "on";
	}
	die unless $packet->length() == 0;
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R. .ogf v3 (builds 1098, 1114, 1154) dumper
Usage: dump_ogfv3.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();
ogfv3::read($fn);
