#!perl -w -I \temp\1\bin

use constant OGF_HEADER		=> 0x01;
use constant OGF_TEXTURE	=> 0x02;
use constant OGF_VERTICES	=> 0x03;
use constant OGF_INDICES	=> 0x04;
use constant OGF_OGF_P_MAP	=> 0x05;
use constant OGF_SWIDATA	=> 0x06;
use constant OGF_VCONTAINER	=> 0x07;
use constant OGF_ICONTAINER	=> 0x08;
use constant OGF_CHILDREN	=> 0x09;
use constant OGF_CHILDREN_L	=> 0x0a;
use constant OGF_LODDEF2	=> 0x0b;
use constant OGF_TREEDEF	=> 0x0c;
use constant OGF_S_BONE_NAMES	=> 0x0d;
use constant OGF_S_MOTIONS	=> 0x0e;
use constant OGF_S_SMPARAMS	=> 0x0f;
use constant OGF_S_IKDATA	=> 0x10;
use constant OGF_S_USERDATA	=> 0x11;
use constant OGF_S_DESC		=> 0x12;
use constant OGF_S_MOTION_REFS_0=> 0x13;
use constant OGF_SWICONTAINER	=> 0x14;
use constant OGF_GCONTAINER	=> 0x15;
use constant OGF_FASTPATH	=> 0x16;
use constant OGF_S_LODS		=> 0x17;
use constant OGF_S_MOTION_REFS_1=> 0x18;

use constant chunk_names => {
0x1	=> 'OGF_HEADER',
0x2	=> 'OGF_TEXTURE',
0x3	=> 'OGF_VERTICES',
0x4	=> 'OGF_INDICES',
0x5	=> 'OGF_P_MAP',
0x6	=> 'OGF_SWIDATA',
0x7	=> 'OGF_VCONTAINER',
0x8	=> 'OGF_ICONTAINER',
0x9	=> 'OGF_CHILDREN',
0xa	=> 'OGF_CHILDREN_L',
0xb	=> 'OGF_LODDEF2',
0xc	=> 'OGF_TREEDEF',
0xd	=> 'OGF_S_BONE_NAMES',
0xe	=> 'OGF_S_MOTIONS',
0xf	=> 'OGF_S_SMPARAMS',
0x10	=> 'OGF_S_IKDATA',
0x11	=> 'OGF_S_USERDATA',
0x12	=> 'OGF_S_DESC',
0x13	=> 'OGF_S_MOTION_REFS_0',
0x14	=> 'OGF_SWICONTAINER',
0x15	=> 'OGF_GCONTAINER',
0x16	=> 'OGF_FASTPATH',
0x17	=> 'OGF_S_LODS',
0x18	=> 'OGF_S_MOTION_REFS_1',
};

use constant MT_NORMAL			=> 0;
use constant MT_HIERRARHY		=> 1;
use constant MT_PROGRESSIVE		=> 2;
use constant MT_SKELETON_ANIM		=> 3;
use constant MT_SKELETON_GEOMDEF_PM	=> 4;
use constant MT_SKELETON_GEOMDEF_ST	=> 5;
use constant MT_LOD			=> 6;
use constant MT_TREE_ST			=> 7;
use constant MT_PARTICLE_EFFECT		=> 8;
use constant MT_PARTICLE_GROUP		=> 9;
use constant MT_SKELETON_RIGID		=> 10;
use constant MT_TREE_PM			=> 11;

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
package ogf_s_desc;
use strict;
use POSIX qw(ctime);
sub dump {
	my ($cf) = @_;

	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($ogf_object, $ogf_creator,
	    $unk,
	    $creator, $create_time,
	    $editor, $edit_time) = $packet->unpack('Z*Z*VZ*VZ*V');
	printf "s_desc\n  ogf_object: '%s'\n", $ogf_object;
	printf "  ogf_creator: '%s'\n", $ogf_creator;
	printf "  creator: '%s'\n", $creator;
	printf "  create_time: %s", ctime($create_time);
	printf "  editor: '%s'\n", $editor;
	printf "  edit_time: %s", ctime($edit_time);
	die unless $packet->length() == 0;
}

#######################################################################
package ogf_header;
use strict;
sub dump {
	my ($cf) = @_;

	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	printf "header\n";
	my ($format_version, $type, $shader_id) = $packet->unpack('CCv');
	printf "  format_version: %d\n", $format_version;
	printf "  type: %d (%s)\n", $type, ::mt_names->{$type};
	printf "  shader_id: %d\n", $shader_id;
	my $bbox = ogf_bbox::read($packet);
	printf "  bbox\n    min: %f, %f, %f\n", @{$bbox->{min}};
	printf "    max: %f, %f, %f\n", @{$bbox->{max}};
	my $bsphere = ogf_bsphere::read($packet);
	printf "  bsphere\n    center: %f, %f, %f\n", @{$bsphere->{c}};
	printf "    radius: %f\n", $bsphere->{r};
	die unless $packet->length() == 0;
}

#######################################################################
package ogf_texture;
use strict;
sub dump {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($texture_name, $shader_name) = $packet->unpack('Z*Z*');
	printf "texture\n";
	printf "  texture_name: %s\n", $texture_name;
	printf "  shader_name: %s\n", $shader_name;
	die unless $packet->length() == 0;
}

#######################################################################
package ogf_vert_boned_1w;
use strict;
sub read {
	my $self = {};
	@{$self->{p}} = $_[0]->unpack('f3');
	@{$self->{n}} = $_[0]->unpack('f3');
	@{$self->{t}} = $_[0]->unpack('f3');
	@{$self->{b}} = $_[0]->unpack('f3');
	($self->{u}, $self->{v}, $self->{matrix}) = $_[0]->unpack('ffl');
	return $self;
}

#######################################################################
package ogf_vert_boned_2w;
use strict;
sub read {
	my $self = {};
	($self->{matrix0}, $self->{matrix1}) = $_[0]->unpack('vv');
	@{$self->{p}} = $_[0]->unpack('f3');
	@{$self->{n}} = $_[0]->unpack('f3');
	@{$self->{t}} = $_[0]->unpack('f3');
	@{$self->{b}} = $_[0]->unpack('f3');
	($self->{w}, $self->{u}, $self->{v}) = $_[0]->unpack('fff');
	return $self;
}

#######################################################################
package ogf_vertices;
use strict;
use constant OGF_VERTEXFORMAT_FVF_1L	=> 0x12071980;
use constant OGF_VERTEXFORMAT_FVF_2L	=> 0x240e3300;
use constant OGF_VERTEXFORMAT_FVF_NL	=> 0x36154c80;
use constant OGF_VERTEXFORMAT_FVF_OLD	=> 0x00000112;
sub dump {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($vertex_format, $count) = $packet->unpack('VV');
	printf "  vertex_format: %#8.8x\n", $vertex_format;
	printf "  count: %d\n", $count;
	if ($vertex_format == OGF_VERTEXFORMAT_FVF_1L or $vertex_format == 1) {
		my @v = $packet->unpack("(f14l)$count");
		for (my $i = 0; $i < $count; $i++) {
			my $o = $i*15;
			printf "  vertex%d\n", $i;
			printf "    point: %f, %f, %f\n", @v[$o+0..$o+2];
			printf "    normal: %f, %f, %f\n", @v[$o+3..$o+5];
			printf "    t: %f, %f, %f\n", @v[$o+6..$o+8];
			printf "    b: %f, %f, %f\n", @v[$o+9..$o+11];
			printf "    u, v: %f, %f\n", @v[$o+12 .. $o+13];
			printf "    matrix: %d\n", $v[$o+14];
		}
	} elsif ($vertex_format == OGF_VERTEXFORMAT_FVF_2L or $vertex_format == 2) {
		my @v = $packet->unpack("(vvf15)$count");
		for (my $i = 0; $i < $count; $i++) {
			my $o = $i*17;
			printf "  vertex%d\n", $i;
			printf "    matrix0: %d\n", $v[$o+0];
			printf "    matrix1: %d\n", $v[$o+1];
			printf "    point: %f, %f, %f\n", @v[$o+2..$o+4];
			printf "    normal: %f, %f, %f\n", @v[$o+5..$o+7];
			printf "    t: %f, %f, %f\n", @v[$o+8..$o+10];
			printf "    b: %f, %f, %f\n", @v[$o+11..$o+13];
			printf "    w: %f\n", $v[$o+14];
			printf "    u, v: %f, %f\n", @v[$o+15 .. $o+16];
		}
	} elsif ($vertex_format == OGF_VERTEXFORMAT_FVF_OLD) {
		my @v = $packet->unpack("(f8)$count");
		for (my $i = 0; $i < $count; $i++) {
			my $o = $i*8;
			printf "  vertex%d\n", $i;
			printf "    point: %f, %f, %f\n", @v[$o+0..$o+2];
			printf "    normal: %f, %f, %f\n", @v[$o+3..$o+5];
			printf "    u, v: %f, %f\n", @v[$o+6 .. $o+7];
		}
	} elsif ($vertex_format == 3) {
		my @v = $packet->unpack("(v3f16)$count");
		for (my $i = 0; $i < $count; $i++) {
			my $o = $i*19;
			printf "  vertex%d\n", $i;
			printf "    matrix0: %d\n", $v[$o+0];
			printf "    matrix1: %d\n", $v[$o+1];
			printf "    matrix2: %d\n", $v[$o+2];
			printf "    point: %f, %f, %f\n", @v[$o+3..$o+5];
			printf "    normal: %f, %f, %f\n", @v[$o+6..$o+8];
			printf "    t: %f, %f, %f\n", @v[$o+9..$o+11];
			printf "    b: %f, %f, %f\n", @v[$o+12..$o+14];
			printf "    w0: %f\n", $v[$o+15];
			printf "    w1: %f\n", $v[$o+16];
			printf "    u, v: %f, %f\n", @v[$o+17 .. $o+18];
		}
	} elsif ($vertex_format == 4) {
		my @v = $packet->unpack("(v4f17)$count");
		for (my $i = 0; $i < $count; $i++) {
			my $o = $i*21;
			printf "  vertex%d\n", $i;
			printf "    matrix0: %d\n", $v[$o+0];
			printf "    matrix1: %d\n", $v[$o+1];
			printf "    matrix2: %d\n", $v[$o+2];
			printf "    matrix3: %d\n", $v[$o+3];
			printf "    point: %f, %f, %f\n", @v[$o+4..$o+6];
			printf "    normal: %f, %f, %f\n", @v[$o+7..$o+9];
			printf "    t: %f, %f, %f\n", @v[$o+10..$o+12];
			printf "    b: %f, %f, %f\n", @v[$o+13..$o+15];
			printf "    w0: %f\n", $v[$o+16];
			printf "    w1: %f\n", $v[$o+17];
			printf "    w2: %f\n", $v[$o+18];
			printf "    u, v: %f, %f\n", @v[$o+19 .. $o+20];
		}
	} else {
		die "unsupported fvf";
	}
	die unless $packet->length() == 0;
}

#######################################################################
package ogf_indices;
use strict;
sub dump {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	printf "  count: %d\n", $count;
	my @v = $packet->unpack("v$count");
	for (my $i = 0; $i < $count; $i++) {
		printf "    index%d: %d\n", $i, $v[$i];
	}
	die unless $packet->length() == 0;
}

#######################################################################
package ogf_sw;
use strict;
sub read {
	my $self = {};
	($self->{offset}, $self->{num_tris}, $self->{num_verts}) = $_[0]->unpack('lvv');
	return $self;
}

#######################################################################
package ogf_swidata;
use strict;
sub dump {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @reserved = $packet->unpack('V4');
	my ($count) = $packet->unpack('V');
	printf "  count: %d\n", $count;
	my @v = $packet->unpack("(lvv)$count");
	for (my $i = 0; $i < $count; $i++) {
		my $o = $i*3;
		printf "  slide_window%d\n", $i;
		printf "    offset: %#8.8x\n", $v[$o + 0];
		printf "    num_tris: %d\n", $v[$o + 1];
		printf "    num_verts: %d\n", $v[$o + 2];
	}
	die unless $packet->length() == 0;
}

#######################################################################
package ogf_children;
use strict;
sub dump {
	my ($cf) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($nnnn, $size) = $cf->r_chunk_open();
		defined $nnnn or last;
		if ($nnnn != $expected_id) {
			printf "unexpected chunk 0009,%04x\n", $nnnn;
			die;
		}
		printf "child %d size %d(%#lx)\n", $nnnn, $size, $size;
		while (1) {
			my ($id, $size) = $cf->r_chunk_open();
			defined $id or last;
			printf "chunk '%s' size %d(%#lx)\n", ::chunk_names->{$id}, $size, $size;
			SWITCH: {
				$id == ::OGF_HEADER && do { ogf_header::dump($cf); last SWITCH; };
				$id == ::OGF_TEXTURE && do { ogf_texture::dump($cf); last SWITCH; };
				$id == ::OGF_VERTICES && do { ogf_vertices::dump($cf); last SWITCH; };
				$id == ::OGF_INDICES && do { ogf_indices::dump($cf); last SWITCH; };
				$id == ::OGF_SWIDATA && do { ogf_swidata::dump($cf); last SWITCH; };
				printf "unexpected chunk 0009,%04x,%04x size %d\n", $nnnn, $id, $size;
				die;
			}
			$cf->r_chunk_close();
		}
		$cf->r_chunk_close();
	}
}

#######################################################################
package ogf_obb;
use strict;
sub read {
	my $self = {};
	@{$self->{rotate}} = $_[0]->unpack('f9');
	@{$self->{translate}} = $_[0]->unpack('f3');
	@{$self->{halfsize}} = $_[0]->unpack('f3');
	return $self;
}

#######################################################################
package ogf_sphere;
use strict;
sub read {
	my $self = {};
	@{$self->{p}} = $_[0]->unpack('f3');
	($self->{r}) = $_[0]->unpack('f');
	return $self;
}

#######################################################################
package ogf_cylinder;
use strict;
sub read {
	my $self = {};
	@{$self->{center}} = $_[0]->unpack('f3');
	@{$self->{direction}} = $_[0]->unpack('f3');
	($self->{height}) = $_[0]->unpack('f');
	($self->{radius}) = $_[0]->unpack('f');
	return $self;
}

#######################################################################
package ogf_s_bone_shape;
use strict;
sub read {
	my $self = {};
	($self->{type}, $self->{flags}) = $_[0]->unpack('vv');
	$self->{box} = ogf_obb::read($_[0]);
	$self->{sphere} = ogf_sphere::read($_[0]);
	$self->{cylinder} = ogf_cylinder::read($_[0]);
	return $self;
}

#######################################################################
package ogf_s_joint_limit;
use strict;
sub read {
	my $self = {};
	@{$self->{limit}} = $_[0]->unpack('f2');
	($self->{spring_factor}, $self->{damping_factor}) = $_[0]->unpack('ff');
	return $self;
}

#######################################################################
package ogf_s_joint_ik_data;
use strict;
sub read {
	my $self = {};
	($self->{type}) = $_[0]->unpack('V');
	${$self->{limits}}[0] = ogf_s_joint_limit::read($_[0]);
	${$self->{limits}}[1] = ogf_s_joint_limit::read($_[0]);
	${$self->{limits}}[2] = ogf_s_joint_limit::read($_[0]);
	($self->{spring_factor},
	 $self->{damping_factor},
	 $self->{ik_flags},
	 $self->{break_force},
	 $self->{break_torque},
	 $self->{friction},
	) = $_[0]->unpack('ffVfff');
	return $self;
}

#######################################################################
package ogf_motion_def;
use strict;
sub read {
	my $self = {};
	($self->{bone_or_part},
	 $self->{motion},
	 $self->{speed},
	 $self->{power},
	 $self->{accrue},
	 $self->{falloff},
	) = $_[0]->unpack('vvffff');
	return $self;
}

#######################################################################
package ogf_motion_mark;
use strict;
sub read {
	my $self = {};
	# FIXME: express this in perl!
	$self->{name} = '';
	my $c;
	for (;;) {
		($c) = $_[0]->unpack('a');
		last if ($c eq "\n" || $c eq "\r");
		$self->{name} .= $c;
	}
	($c) = $_[0]->unpack('a');
	die unless $c eq "\n";
	@{$self->{intervals}} = $_[0]->unpack('V/(ff)');
	return $self;
}

#######################################################################
package ogf_s_smparams;
use strict;
use constant esmFX		=> 0x1;
use constant esmStopAtEnd	=> 0x2;
use constant esmNoMix		=> 0x4;
use constant esmSyncPart	=> 0x8;
use constant esmHasMotionMarks	=> 0x10;	# guessed name
sub dump {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('v');
	printf "smparams\n  version: %d\n", $version;
	die unless ($version == 3 || $version == 4);
	my ($partition_count) = $packet->unpack('v');
	for (my $i = 0; $i < $partition_count; $i++) {
		my ($partition_name) = $packet->unpack('Z*');
		printf "  partition%d\n", $i;
		printf "    name: '%s'\n", $partition_name;
		my ($bone_count) = $packet->unpack('v');
		for (my $i = 0; $i < $bone_count; $i++) {
			my ($bone_name, $bone_id) = $packet->unpack('Z*V');
			printf "    bone%d\n", $i;
			printf "      name: '%s'\n", $bone_name;
			printf "      id: %d\n", $bone_id;
		}
	}
	my ($motion_count) = $packet->unpack('v');
	for (my $i = 0; $i < $motion_count; $i++) {
		my ($motion_name, $motion_flags) = $packet->unpack('Z*V');
		printf "  motion%d\n", $i;
		printf "    name: '%s'\n", $motion_name;
		if ($motion_flags != 0) {
			my @n;
			($motion_flags & esmFX) && do { push @n, "esmFX"; };
			($motion_flags & esmStopAtEnd) && do { push @n, "esmStopAtEnd"; };
			($motion_flags & esmNoMix) && do { push @n, "esmNoMix"; };
			($motion_flags & esmSyncPart) && do { push @n, "esmSyncPart"; };
			($motion_flags & esmHasMotionMarks) && do { push @n, "esmHasMotionMarks"; };
			printf "    flags: %#x (%s)\n", $motion_flags, join('|', @n);
		} else {
			printf "    flags: %#x\n", $motion_flags;
		}
		my $mdef = ogf_motion_def::read($packet);
		printf "    bone_or_part: %d\n", $mdef->{bone_or_part};
		printf "    motion: %d\n", $mdef->{motion};
		printf "    speed: %f\n", $mdef->{speed};
		printf "    power: %f\n", $mdef->{power};
		printf "    accrue: %f\n", $mdef->{accrue};
		printf "    falloff: %f\n", $mdef->{falloff};
		if ($version == 4) {
			my ($num_marks) = $packet->unpack('V');
			printf "    mark count: %u\n", $num_marks;
			for (my $j = 0; $j < $num_marks; ++$j) {
				my $mmark = ogf_motion_mark::read($packet);
				printf "      mark%d\n", $j;
				printf "        name: '%s'\n", $mmark->{name};
				printf "        interval count: %d\n", ($#{$mmark->{intervals}} + 1)/2;
				for (my $k = 0; $k <= $#{$mmark->{intervals}}; $k += 2) {
					printf "          interval%d: %f, %f\n", $k/2, @{$mmark->{intervals}}[$k .. $k + 1];
				}
			}
		}
	}
	die unless $packet->length() == 0;
}

#######################################################################
package ogf_s_motion_refs;
use strict;
sub dump_0 {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($motion_refs) = $packet->unpack('Z*');
	printf "  motion_refs: '%s'\n", $motion_refs;
	die unless $packet->length() == 0;
}
sub dump_1 {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @motion_refs = $packet->unpack('V/(Z*)');
	printf "  motion_refs: %d\n", $#motion_refs + 1;
	for (my $i = 0; $i <= $#motion_refs; ++$i) {
		printf "    ref%d: '%s'\n", $i, $motion_refs[$i];
	}
	die unless $packet->length() == 0;
}

#######################################################################
package ogf_s_motions;
use strict;
sub dump {
	my ($cf) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($nnnn, $size) = $cf->r_chunk_open();
		defined $nnnn or last;
		if ($nnnn != $expected_id) {
			printf "unexpected chunk 000e,%04x\n", $nnnn;
			die;
		}
		SWITCH: {
			$nnnn == 0 && do { dump_count($cf); last SWITCH; };
			dump_motion($cf, $nnnn);
		}
		$cf->r_chunk_close();
	}
}
sub dump_count {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	printf "  motion count: %d\n", $count;
	die unless $packet->length() == 0;
}
sub dump_motion {
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
	for (my $i = 0; $packet->length() > 0; $i++) {
		my @keysr;
		my @keyst;
		my $crc = 0;
		printf "    bone id: %d\n", $i;
		my ($flags) = $packet->unpack('C');
		printf "      flags: %#2.2x\n", $flags;
		die unless ($flags & ~0x07) == 0;
		printf "      keysR\n";
		if ($flags & 0x02) {
			@keysr = $packet->unpack('s4');
			printf "        crc: not set\n";
		} else {
			($crc, @keysr) = $packet->unpack("V(s4)$count");
			printf "        crc: %#8.8x\n", $crc;
		}
		dequantize_qr(\@keysr);
		for (my $j = 0; $j <= $#keysr; $j += 4) {
			printf "        key%d: %f, %f, %f, %f\n", $j/4, @keysr[$j .. $j + 3];
		}
		printf "      keysT\n";
		my @sizet;
		if ($flags & 0x01) {
			($crc) = $packet->unpack('V');
			printf "        crc: %#8.8x\n", $crc;
			if ($flags & 0x04) {
				(@keyst) = $packet->unpack("(s3)$count");
			} else {
				(@keyst) = $packet->unpack("(c3)$count");
			}
			@sizet = $packet->unpack('f3');
			printf "        sizeT: %f, %f, %f\n", @sizet;
		} else {
			die unless ($flags & 0x04) == 0;
		}
		my @initt = $packet->unpack('f3');
		printf "        initT: %f, %f, %f\n", @initt;
		if ($flags & 0x01) {
			for (my $j = 0; $j <= $#keyst; $j += 3) {
				my $x = $keyst[$j+0]*$sizet[0] + $initt[0];
				my $y = $keyst[$j+1]*$sizet[1] + $initt[1];
				my $z = $keyst[$j+2]*$sizet[2] + $initt[2];
				printf "        key%d: %f, %f, %f\n", $j/3, $x, $y, $z;
			}
		}
	}
	die unless $packet->length() == 0;
}
sub dequantize_qr {
	my ($v) = @_;
	foreach my $e (@$v) {
		$e /= 32767.0;
	}
}
sub dequantize {
	my ($base, $min, $max, $v) = @_;
	foreach my $e (@$v) {
		$e = $e*($max - $min)/((1 << $base) - 1) + $min;
	}
}

#######################################################################
package ogf_s_lods;
use strict;
sub dump_csky {
	my ($cf) = @_;
	printf "s_lods (csky)\n";
}
sub dump {
	my ($cf) = @_;
	printf "s_lods\n";
	for (my $expected_id = 0;; $expected_id++) {
		my ($nnnn, $size) = $cf->r_chunk_open();
		defined $nnnn or last;
		if ($nnnn != $expected_id) {
			printf "unexpected chunk 0009,%04x\n", $nnnn;
			die;
		}
		printf "lod %d size %d(%#lx)\n", $nnnn, $size, $size;
		while (1) {
			my ($id, $size) = $cf->r_chunk_open();
			defined $id or last;
			printf "chunk '%s' size %d(%#lx)\n", ::chunk_names->{$id}, $size, $size;
			SWITCH: {
				$id == ::OGF_HEADER && do { ogf_header::dump($cf); last SWITCH; };
				$id == ::OGF_TEXTURE && do { ogf_texture::dump($cf); last SWITCH; };
				$id == ::OGF_VERTICES && do { ogf_vertices::dump($cf); last SWITCH; };
				$id == ::OGF_INDICES && do { ogf_indices::dump($cf); last SWITCH; };
				$id == ::OGF_SWIDATA && do { ogf_swidata::dump($cf); last SWITCH; };
				printf "unexpected chunk 0009,%04x,%04x size %d\n", $nnnn, $id, $size;
				die;
			}
			$cf->r_chunk_close();
		}
		$cf->r_chunk_close();
	}
}

#######################################################################
package ogf_s_bone_names;
use strict;
sub dump {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	printf "s_bone_names: count=%d\n", $count;
	for (my $i = 0; $i < $count; $i++) {
		my ($name, $parent_name) = $packet->unpack('Z*Z*');
		printf "  bone%d\n", $i;
		printf "    name: '%s'\n", $name;
		printf "    parent_name: '%s'\n", $parent_name;
		my $obb = ogf_obb::read($packet);
		printf "    obb\n      rotate\n        %f, %f, %f\n", @{$obb->{rotate}}[0..2];
		printf "        %f, %f, %f\n", @{$obb->{rotate}}[3..5];
		printf "        %f, %f, %f\n", @{$obb->{rotate}}[6..8];
		printf "      translate: %f, %f, %f\n", @{$obb->{translate}};
		printf "      halfsize: %f, %f, %f\n", @{$obb->{halfsize}};
	}
	die unless $packet->length() == 0;
}

#######################################################################
package ogf_s_ikdata;
use strict;
sub dump {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	printf "s_ikdata\n";
	my $i = 0;
	while ($packet->length()) {
		printf "  bone%d\n", $i;
		my ($unk, $game_mtl_name) = $packet->unpack('VZ*');
		printf "    unk: %d\n", $unk;
		printf "    game_mtl_name: '%s'\n", $game_mtl_name;
		my $bone_shape = ogf_s_bone_shape::read($packet);
		printf "    bone_shape\n";
		printf "      type: %d\n", $bone_shape->{type};
		printf "      flags: %#x\n", $bone_shape->{flags};
		if ($bone_shape->{type} == 1) {
			my $obb = $bone_shape->{box};
			printf "      box\n        rotate\n          %f, %f, %f\n", @{$obb->{rotate}}[0..2];
			printf "          %f, %f, %f\n", @{$obb->{rotate}}[3..5];
			printf "          %f, %f, %f\n", @{$obb->{rotate}}[6..8];
			printf "        translate: %f, %f, %f\n", @{$obb->{translate}};
			printf "        halfsize: %f, %f, %f\n", @{$obb->{halfsize}};
		} elsif ($bone_shape->{type} == 2) {
			my $sphere = $bone_shape->{sphere};
			printf "      sphere\n        center: %f, %f, %f\n", @{$sphere->{p}};
			printf "        radius: %f\n", $sphere->{r};
		} elsif ($bone_shape->{type} == 3) {
			my $cyl = $bone_shape->{cylinder};
			printf "      cylinder\n        center: %f, %f, %f\n", @{$cyl->{center}};
			printf "        direction: %f, %f, %f\n", @{$cyl->{direction}};
			printf "        height: %f\n", $cyl->{height};
			printf "        radius: %f\n", $cyl->{radius};
		}
		my $joint_ik_data = ogf_s_joint_ik_data::read($packet);
		printf "    s_joint_ik_data\n      type: %d\n", $joint_ik_data->{type};
		printf "      limits\n";
		for (my $j = 0; $j < 3; $j++) {
			printf "        limit%d\n", $j;
			printf "          range: %f, %f\n", @{$joint_ik_data->{limits}[$j]->{limit}};
			printf "          spring_factor: %f\n", $joint_ik_data->{limits}[$j]->{spring_factor};
			printf "          damping_factor: %f\n", $joint_ik_data->{limits}[$j]->{damping_factor};
		}
		printf "      spring_factor: %f\n", $joint_ik_data->{spring_factor};
		printf "      damping_factor: %f\n", $joint_ik_data->{damping_factor};
		printf "      ik_flags: %#x\n", $joint_ik_data->{ik_flags};
		printf "      break_force: %f\n", $joint_ik_data->{break_force};
		printf "      break_torque: %f\n", $joint_ik_data->{break_torque};
		printf "      friction: %f\n", $joint_ik_data->{friction};
		my @bind_rotation = $packet->unpack('f3');
		printf "    bind_rotation: %f, %f, %f\n", @bind_rotation;
		my @bind_position = $packet->unpack('f3');
		printf "    bind_position: %f, %f, %f\n", @bind_position;
		my ($mass, @center_of_mass) = $packet->unpack('ff3');
		printf "    mass: %f\n", $mass;
		printf "    center_of_mass: %f,%f,%f\n", @center_of_mass;
		$i++;
	}
	die unless $packet->length() == 0;
}
#######################################################################
package ogf_s_userdata;
use strict;
sub dump {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	printf "s_userdata\n";
	my $n = $packet->length();
	my ($userdata) = $packet->unpack("a$n");
	die unless $packet->length() == 0;
}

#######################################################################
package ogf_file;
use strict;
use stkutils::data_packet;
use stkutils::chunked_file;
sub read {
	my ($fn) = @_;
	my $cf = stkutils::chunked_file->new($fn, 'r') or die;
	printf "dumping '%s'\n", $fn;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		printf "top chunk '%s' size %d(%#lx)\n", ::chunk_names->{$id}, $size, $size;
		SWITCH: {
			$id == ::OGF_HEADER && do { ogf_header::dump($cf); last SWITCH; };
			$id == ::OGF_S_DESC && do { ogf_s_desc::dump($cf); last SWITCH; };
			$id == ::OGF_CHILDREN && do { ogf_children::dump($cf); last SWITCH; };
			$id == ::OGF_CHILDREN_L && do { ogf_children_l::dump($cf); last SWITCH; };
			$id == ::OGF_S_BONE_NAMES && do { ogf_s_bone_names::dump($cf); last SWITCH; };
			$id == ::OGF_S_SMPARAMS && do { ogf_s_smparams::dump($cf); last SWITCH; };
			$id == ::OGF_S_MOTIONS && do { ogf_s_motions::dump($cf); last SWITCH; };
			$id == ::OGF_S_MOTION_REFS_0 &&  do { ogf_s_motion_refs::dump_0($cf); last SWITCH; };
			$id == ::OGF_S_MOTION_REFS_1 &&  do { ogf_s_motion_refs::dump_1($cf); last SWITCH; };
			$id == ::OGF_S_IKDATA && do { ogf_s_ikdata::dump($cf); last SWITCH; };
			$id == ::OGF_S_USERDATA && do { ogf_s_userdata::dump($cf); last SWITCH; };
			$id == ::OGF_TEXTURE && do { ogf_texture::dump($cf); last SWITCH; };
			$id == ::OGF_VERTICES && do { ogf_vertices::dump($cf); last SWITCH; };
			$id == ::OGF_INDICES && do { ogf_indices::dump($cf); last SWITCH; };
			$id == ::OGF_SWIDATA && do { ogf_swidata::dump($cf); last SWITCH; };
			$id == ::OGF_S_LODS && do {
				if ($size < 0x100) {
					ogf_s_lods::dump_csky($cf);
				} else {
					ogf_s_lods::dump($cf);
				}
				last SWITCH;
			};
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
S.T.A.L.K.E.R. .ogf dumper
Usage: dump_ogf.pl <input_file>
END
}

(my $ogf_file = $ARGV[0]) or die usage();
ogf_file::read($ogf_file);
