#!perl -w -I \temp\1\bin

use constant FSL_HEADER		=> 0x1;
use constant FSL_SHADERS	=> 0x2;
use constant FSL_VISUALS	=> 0x3;
use constant FSL_PORTALS	=> 0x4;
use constant FSL_LIGHT_DYNAMIC	=> 0x6;
use constant FSL_GLOWS		=> 0x7;
use constant FSL_SECTORS	=> 0x8;
use constant FSL_VB		=> 0x9;
use constant FSL_IB		=> 0xa;
use constant FSL_SWIS		=> 0xb;

use constant FSP_PORTALS	=> 0x1;
use constant FSP_ROOT		=> 0x2;

use constant chunk_names => {
	0x1		=> 'FSL_HEADER',
	0x2		=> 'FSL_SHADERS',
	0x3		=> 'FSL_VISUALS',
	0x4		=> 'FSL_PORTALS',
	0x6		=> 'FSL_LIGHT_DYNAMIC',
	0x7		=> 'FSL_GLOWS',
	0x8		=> 'FSL_SECTORS',
	0x9		=> 'FSL_VB',
	0xa		=> 'FSL_IB',
	0xb		=> 'FSL_SWIS',
};

#######################################################################
package fsl_header;
use strict;
sub read {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($xrlc_version, $xrlc_quality) = $packet->unpack('vv');
	printf "  xrlc_version: %#x\n", $xrlc_version;
	printf "  xrlc_quality: %#x\n", $xrlc_quality;
	die unless $packet->length() == 0;
}

#######################################################################
package fsl_shaders;
use strict;
sub read {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	my @v = $packet->unpack("(Z*)$count");
	for (my $i = 0; $i < $count; $i++) {
		printf "  shader%d: '%s'\n", $i, $v[$i];
	}
	die unless $packet->length() == 0;
}

#######################################################################
package ogf_bbox;
use strict;
sub read {
	my $self = {};
	@{$self->{min}} = $_[0]->unpack('f3');
	@{$self->{max}} = $_[0]->unpack('f3');
	return $self;
}

#######################################################################
package ogf_bsphere;
use strict;
sub read {
	my $self = {};
	@{$self->{c}} = $_[0]->unpack('f3');
	$self->{r} = $_[0]->unpack('f');
	return $self;
}

#######################################################################
package fsl_visuals;
use strict;
use constant mt_names => {
0	=> 'MT_NORMAL',
1	=> 'MT_HIERRARHY',
2	=> 'MT_PROGRESSIVE',
3	=> 'MT_SKELETON_ANIM',
4	=> 'MT_SKELETON_GEOMDEF_PM',
5	=> 'MT_SKELETON_GEOMDEF_ST',
6	=> 'MT_LOD',
7	=> 'MT_TREE_ST',
8	=> 'MT_PARTICLE_EFFECT',
9	=> 'MT_PARTICLE_GROUP',
10	=> 'MT_SKELETON_RIGID',
11	=> 'MT_TREE_PM',
};
sub read {
	my ($cf) = @_;
	while (1) {
		my ($nnnn, $size) = $cf->r_chunk_open();
		defined $nnnn or last;
		printf "visual %04x size %d (%#lx)\n", $nnnn & 0x7fffffff, $size, $size;
		while (1) {
			my ($id, $size) = $cf->r_chunk_open();
			defined $id or last;
			SWITCH: {
				$id == 0x1 && do { dump_header($cf); last SWITCH; };
				$id == 0x6 && do { dump_swidata($cf, $nnnn); last SWITCH; };
				$id == 0x7 && do { dump_vcontainer($cf, $nnnn); last SWITCH; };
				$id == 0x8 && do { dump_icontainer($cf, $nnnn); last SWITCH; };
				$id == 0xa && do { dump_children_l($cf, $nnnn); last SWITCH; };
				$id == 0xb && do { dump_loddef2($cf, $nnnn); last SWITCH; };
				$id == 0xc && do { dump_treedef2($cf, $nnnn); last SWITCH; };
				$id == 0x14 && do { dump_swicontainer($cf, $nnnn); last SWITCH; };
				$id == 0x15 && do { dump_gcontainer($cf, $nnnn); last SWITCH; };
				$id == 0x16 && do { dump_fastpath($cf, $nnnn); last SWITCH; };
				printf "unexpected chunk 0003,%04x,%04x size %d\n", $nnnn, $id, $size;
				die;
			}
			$cf->r_chunk_close();
		}
		$cf->r_chunk_close();
	}
}
sub dump_header {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	printf "  header\n";
	my ($format_version, $type, $shader_id) = $packet->unpack('CCv');
	printf "    format_version: %d\n", $format_version;
	printf "    type: %d (%s)\n", $type, mt_names->{$type};
	printf "    shader_id: %d\n", $shader_id;
	my $bbox = ogf_bbox::read($packet);
	printf "    bbox\n      min: %f, %f, %f\n", @{$bbox->{min}};
	printf "      max: %f, %f, %f\n", @{$bbox->{max}};
	my $bsphere = ogf_bsphere::read($packet);
	printf "    bsphere\n      center: %f, %f, %f\n", @{$bsphere->{c}};
	printf "      radius: %f\n", $bsphere->{r};
	die unless $packet->length() == 0;
}
sub dump_swidata {
	my ($cf, $id) = @_;
	printf "  swidata\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @reserved = $packet->unpack('V4');
	my ($count) = $packet->unpack('V');
	printf "    count: %d\n", $count;
	my @v = $packet->unpack("(lvv)$count");
	for (my $i = 0; $i < $count; $i++) {
		my $o = $i*3;
		printf "    slide_window%d\n", $i;
		printf "      offset: %u\n", $v[$o + 0];
		printf "      num_tris: %d\n", $v[$o + 1];
		printf "      num_verts: %d\n", $v[$o + 2];
	}
	die unless $packet->length() == 0;
}
sub dump_children_l {
	my ($cf, $id) = @_;
	printf "  children_l\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('V/V');
	printf "    children count: %d\n", $#v + 1;
	for (my $i = 0; $i <= $#v; $i++) {
		printf "    child%d index: %04x\n", $i, $v[$i];
	}
	die unless $packet->length() == 0;
}
sub dump_loddef2 {
	my ($cf, $id) = @_;
	printf "  loddef2\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('(f3f2VC@28)32');
	for (my $i = 0; $i <= $#v; $i += 7) {
		if ($i % 28 == 0) {
			printf "    face%d\n", $i/28;
		}
		printf "      vertex%d\n", $i/7;
		printf "        p: %f, %f, %f\n", @v[$i .. $i + 2];
		printf "        u, v: %f, %f\n", @v[$i + 3 .. $i + 4];
		printf "        c_hemi: %#8.8x\n", $v[$i + 5];
		printf "        c_sun: %#2.2x\n", $v[$i + 6];
	}
	die unless $packet->length() == 0;
}
sub dump_treedef2 {
	my ($cf, $id) = @_;
	printf "  treedef2\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @xform = $packet->unpack('f16');
	printf "    xform:\n";
	printf "      %f, %f, %f, %f\n", @xform[0 .. 3];
	printf "      %f, %f, %f, %f\n", @xform[4 .. 7];
	printf "      %f, %f, %f, %f\n", @xform[8 .. 11];
	printf "      %f, %f, %f, %f\n", @xform[12 .. 15];
	my @scale = $packet->unpack('f5');
	printf "    c_scale:\n";
	printf "      rgb: %f, %f, %f\n", @scale[0 .. 2];
	printf "      hemi: %f\n", $scale[3];
	printf "      sun: %f\n", $scale[4];
	my @bias = $packet->unpack('f5');
	printf "    c_bias:\n";
	printf "      rgb: %f, %f, %f\n", @bias[0 .. 2];
	printf "      hemi: %f\n", $bias[3];
	printf "      sun: %f\n", $bias[4];
	die unless $packet->length() == 0;
}
sub dump_swicontainer {
	my ($cf, $id) = @_;
	printf "  swicontainer\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($swi_set) = $packet->unpack('V');
	printf "    swi set index: %d\n", $swi_set;
	die unless $packet->length() == 0;
}
sub dump_vcontainer {
	my ($cf, $id) = @_;
	printf "  vcontainer\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('V3');
	printf "    vertex buffer set: %u\n", $v[0];
	printf "      vertex base: %u\n", $v[1];
	printf "      vertex count: %u\n", $v[2];
	die unless $packet->length() == 0;
}
sub dump_icontainer {
	my ($cf, $id) = @_;
	printf "  icontainer\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('V3');
	printf "    index buffer set: %u\n", $v[0];
	printf "      index base: %u\n", $v[1];
	printf "      index count: %u\n", $v[2];
	die unless $packet->length() == 0;
}
sub dump_gcontainer {
	my ($cf, $id) = @_;
	printf "  gcontainer\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('V6');
	printf "    vertex buffer set: %u\n", $v[0];
	printf "      vertex base: %u\n", $v[1];
	printf "      vertex count: %u\n", $v[2];
	printf "    index buffer set: %u\n", $v[3];
	printf "      index base: %u\n", $v[4];
	printf "      index count: %u\n", $v[5];
	die unless $packet->length() == 0;
}
sub dump_fastpath {
	my ($cf, $id) = @_;
	printf "  fastpath\n";
}

#######################################################################
package fsl_portals;
use strict;
sub read {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	return unless $packet->length() != 0;
	die unless $packet->length() % 0x50 == 0;
	my $count = $packet->length() / 0x50;
	my @v = $packet->unpack("(vvf18V)$count");
	die unless $packet->length() == 0;
	for (my $i = 0; $i < $count; $i++) {
		my $o = $i*21;
		printf "  portal%d\n", $i;
		printf "    sector_front: %d\n", $v[$o + 0];
		printf "    sector_back: %d\n", $v[$o + 1];
		printf "    vertex0: %f, %f, %f\n", @v[$o + 2 .. $o + 4];
		printf "    vertex1: %f, %f, %f\n", @v[$o + 5 .. $o + 7];
		printf "    vertex2: %f, %f, %f\n", @v[$o + 8 .. $o + 10];
		printf "    vertex3: %f, %f, %f\n", @v[$o + 11 .. $o + 13];
		printf "    vertex4: %f, %f, %f\n", @v[$o + 14 .. $o + 16];
		printf "    vertex5: %f, %f, %f\n", @v[$o + 17 .. $o + 19];
		printf "    count: %d\n", $v[$o + 20];
	}
}

#######################################################################
package fsl_light_dynamic;
use strict;
#use constant lt_names => {
#	0	=> 'direct',
#	1	=> 'point',
#	2	=> 'spot',
#	3	=> 'omnipart',
#	4	=> 'reflected',
#};
use constant lt_names => {
	1	=> 'point',
	2	=> 'spot',
	3	=> 'directional',
};
sub read {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my $count = $packet->length() / 0x6c;
	die unless $packet->length() % 0x6c == 0;
	my @v = $packet->unpack("(VVf4f4f4f3f3f7)$count");
	die unless $packet->length() == 0;
	for (my $i = 0; $i < $count; $i++) {
		my $o = $i*27;
		printf "  light%d\n", $i;
		printf "    controller_id: %d\n", $v[$o + 0];
		printf "    type: %s (%d)\n", lt_names->{$v[$o + 1]}, $v[$o + 1];
		printf "    diffuse: %f, %f, %f, %f\n", @v[$o + 2 .. $o + 5];
		printf "    specular: %f, %f, %f, %f\n", @v[$o + 6 .. $o + 9];
		printf "    ambient: %f, %f, %f, %f\n", @v[$o + 10 .. $o + 13];
		printf "    position: %f, %f, %f\n", @v[$o + 14 .. $o + 16];
		printf "    direction: %f, %f, %f\n", @v[$o + 17 .. $o + 19];
		printf "    range: %f\n", $v[$o + 20];
		printf "    falloff: %f\n", $v[$o + 21];
		printf "    attenuation0: %f\n", $v[$o + 22];
		printf "    attenuation1: %f\n", $v[$o + 23];
		printf "    attenuation2: %f\n", $v[$o + 24];
		printf "    theta: %f\n", $v[$o + 25];
		printf "    phi: %f\n", $v[$o + 26];
	}
}

#######################################################################
package fsl_glows;
use strict;
sub read {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my $count = $packet->length() / 0x12;
	die unless $packet->length() % 0x12 == 0;
	my @v = $packet->unpack("(f3fv)$count");
	die unless $packet->length() == 0;
	for (my $i = 0; $i < $count; $i++) {
		my $o = $i*5;
		printf "  glow%d\n", $i;
		printf "    position: %f, %f, %f\n", @v[$o + 0 .. $o + 2];
		printf "    radius: %f\n", $v[$o + 3];
		printf "    shader/texture index: %d\n", $v[$o + 4];
	}
}

#######################################################################
package fsl_sectors;
use strict;
sub read {
	my ($cf) = @_;
	while (1) {
		my ($nnnn, $size) = $cf->r_chunk_open();
		defined $nnnn or last;
		printf "sector%d\n", $nnnn;
		while (1) {
			my ($id, $size) = $cf->r_chunk_open();
			defined $id or last;
			SWITCH: {
				$id == ::FSP_PORTALS && do { dump_portals($cf); last SWITCH; };
				$id == ::FSP_ROOT && do { dump_root($cf); last SWITCH; };
				printf "unexpected chunk 0008,%04x,%04x size %d\n", $nnnn, $id, $size;
				die;
			}
			$cf->r_chunk_close();
		}
		$cf->r_chunk_close();
	}
}
sub dump_portals {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	if ($packet->length() == 0) {
		printf "  no portals\n";
		return;
	}
	my $count = $packet->length() / 2;
	die unless $packet->length() % 2 == 0;
	my @v = $packet->unpack("v$count");
	printf "  portals: %s\n", join(',', @v);
}
sub dump_root {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($root) = $packet->unpack('V');
	printf "  root: %d (%04x)\n", $root, $root;
	die unless $packet->length() == 0;
}

#######################################################################
package fsl_vb;
use strict;
use constant type_names => {
	1	=> 'FLOAT2',
	2	=> 'FLOAT3',
	3	=> 'FLOAT4',
	4	=> 'D3DCOLOR',
	6	=> 'SHORT2',
	7	=> 'SHORT4',
	17	=> 'UNUSED',
};
use constant method_names => {
	0	=> 'DEFAULT',
	1	=> 'PARTIALU',
	2	=> 'PARTIALV',
	3	=> 'CROSSUV',
	4	=> 'UV',
};
use constant usage_names => {
	0	=> 'POSITION',
	1	=> 'BLENDWEIGHT',
	2	=> 'BLENDINDICES',
	3	=> 'NORMAL',
	4	=> 'PSIZE',
	5	=> 'TEXCOORD',
	6	=> 'TANGENT',
	7	=> 'BINORMAL',
	8	=> 'TESSFACTOR',
	9	=> 'POSITIONT',
	10	=> 'COLOR',
	11	=> 'FOG',
	12	=> 'DEPTH',
	12	=> 'SAMPLE',
};
sub read {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($vertex_buffer_count) = $packet->unpack('V');
	printf "vertex buffer count: %d\n", $vertex_buffer_count;
	for (my $i = 0; $packet->length() > 0; $i++) {
		my $ve_count;
		my $last_offset = 0;
		my $last_type;
		printf "  buffer set%d\n", $i;
		for (my $j = 0; ; $j++) {
			my $ve = read_d3dvertexelement9($packet);
			printf "    vertex element%d\n", $j;
			printf "      stream: %#x\n", $ve->{stream};
			printf "      offset: %#x\n", $ve->{offset};
			printf "      type: %#x (%s)\n", $ve->{type}, type_names->{$ve->{type}};
			printf "      method: %#x (%s)\n", $ve->{method}, method_names->{$ve->{method}};
			printf "      usage: %#x (%s)\n", $ve->{usage}, usage_names->{$ve->{usage}};
			printf "      usage_index: %#x\n", $ve->{usage_index};
			if ($ve->{stream} == 0xff) {
				$ve_count = $j + 1;
				last;
			}
			$last_offset = $ve->{offset};
			$last_type = $ve->{type};
		}
		SWITCH: {
			$last_type == 0x7 && do { $last_offset += 8; last SWITCH; };
			$last_type == 0x6 && do { $last_offset += 4; last SWITCH; };
			$last_type == 0x2 && do { $last_offset += 12; last SWITCH; };
			$last_type == 0x1 && do { $last_offset += 8; last SWITCH; };
			die "$last_type";
		}
		die unless $last_offset % 4 == 0;
		my $elem_count = $last_offset / 4;
		my ($vb_count) = $packet->unpack('V');
		my @vb = $packet->unpack("(V$elem_count)$vb_count");
printf "size=%x\n", $last_offset;
		printf "    vertex buffer%d\n      count: %d\n", $i, $vb_count;
	}
	die unless $packet->length() == 0;
}
sub read_d3dvertexelement9 {
	my $self = {};
	($self->{stream},
	 $self->{offset},
	 $self->{type},
	 $self->{method},
	 $self->{usage},
	 $self->{usage_index}) = $_[0]->unpack('v2C4');
	return $self;
}

#######################################################################
package fsl_ib;
use strict;
sub read {
return;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($ib_count) = $packet->unpack('V');
	printf "  index buffer count: %d\n", $ib_count;
return;
	for (my $i = 0; $i < $ib_count; $i++) {
		my @ib = $packet->unpack('V/v');
		printf "  index buffer%d\n    count: %d\n", $i, $#ib + 1;
	}
	die unless $packet->length() == 0;
}

#######################################################################
package fsl_swis;
use strict;
sub read {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($swb_count) = $packet->unpack('V');
	printf "  swi buffer count: %d\n", $swb_count;
return;
	for (my $i = 0; $i < $swb_count; $i++) {
		my @swb = $packet->unpack('V4V/(lvv)');
		die unless ($#swb + 1 - 4)%3 == 0;
		printf "  swi buffer%d\n    count: %d\n", $i, ($#swb + 1 - 4)/3;
	}
	die unless $packet->length() == 0;
}

#######################################################################
package level;
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
		printf "chunk '%s' size %d(%#lx)\n", ::chunk_names->{$id & 0x7fffffff}, $size, $size;
		SWITCH: {
			$id == ::FSL_HEADER && do { fsl_header::read($cf); last SWITCH; };
			$id == ::FSL_SHADERS && do { fsl_shaders::read($cf); last SWITCH; };
			$id == ::FSL_VISUALS && do { fsl_visuals::read($cf); last SWITCH; };
			$id == ::FSL_PORTALS && do { fsl_portals::read($cf); last SWITCH; };
			$id == ::FSL_LIGHT_DYNAMIC && do { fsl_light_dynamic::read($cf); last SWITCH; };
			$id == ::FSL_GLOWS && do { fsl_glows::read($cf); last SWITCH; };
			$id == ::FSL_SECTORS && do { fsl_sectors::read($cf); last SWITCH; };
			$id == ::FSL_VB && do { fsl_vb::read($cf); last SWITCH; };
			$id == ::FSL_IB && do { fsl_ib::read($cf); last SWITCH; };
			$id == ::FSL_SWIS && do { fsl_swis::read($cf); last SWITCH; };
			($id & 0x80000000) && do { printf "  compressed!\n"; last SWITCH; };
			printf "unexpected chunk %04x size %d\n", $id, $size;
			die;
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
S.T.A.L.K.E.R. level, level.geom, level.geomx dumper
Usage: dump_level.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();

level::read($fn);
