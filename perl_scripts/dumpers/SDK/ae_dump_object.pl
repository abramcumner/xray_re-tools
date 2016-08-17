#!perl -w -I \temp\1\bin

#######################################################################
package ae_object;
use strict;
use stkutils::data_packet;
use stkutils::chunked_file;
use POSIX qw(floor ctime);
sub read {
	my ($fn) = @_;
	printf "dumping '%s'\n", $fn;
	my $cf = stkutils::chunked_file->new($fn, 'r') or die;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7777 && do { read_object($cf); last SWITCH; };
			printf "unexpected chunk %04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_object {
	my ($cf) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x0900 && do { read_object_0900($cf); last SWITCH; };
			$id == 0x0903 && do { read_object_0903($cf); last SWITCH; };
			$id == 0x0907 && do { read_object_0907($cf); last SWITCH; };
			$id == 0x0910 && do { read_object_0910($cf); last SWITCH; };
			$id == 0x0911 && do { read_object_0911($cf); last SWITCH; };
			$id == 0x0912 && do { read_object_0912($cf); last SWITCH; };
			$id == 0x0916 && do { read_object_0916($cf); last SWITCH; };
			$id == 0x0920 && do { read_object_0920($cf); last SWITCH; };
			$id == 0x0921 && do { read_object_0921($cf); last SWITCH; };
			$id == 0x0922 && do { read_object_0922($cf); last SWITCH; };
			$id == 0x0923 && do { read_object_0923($cf); last SWITCH; };
			$id == 0x0924 && do { read_object_0924($cf); last SWITCH; };
			$id == 0x0925 && do { read_object_0925($cf); last SWITCH; };
			printf "unexpected chunk 7777,%04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_object_0900 {
	my ($cf) = @_;
	printf "chunk 7777,0900\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('v');
	die unless $packet->length() == 0;
	printf "  eobj version: %#x\n", $version;
}
sub read_object_0903 {
	my ($cf) = @_;
	printf "chunk 7777,0903\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($flags) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  eobj flags: %#8.8x\n", $flags;
}
sub read_object_0907 {
	my ($cf) = @_;
	printf "chunk 7777,0907\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('V/(Z*Z*Z*Z*Z*Z*VVV)');
	die unless $packet->length() == 0;
	printf "  surface count: %d\n", ($#v + 1)/9;
	for (my $i = 0; $i <= $#v; $i += 9) {
		printf "  surface%d\n", $i/9;
		printf "    name: '%s'\n", $v[$i + 0];
		printf "    engine shader: '%s'\n", $v[$i + 1];
		printf "    compiler shader: '%s'\n", $v[$i + 2];
		printf "    game material: '%s'\n", $v[$i + 3];
		printf "    texture: '%s'\n", $v[$i + 4];
		printf "    vmap: '%s'\n", $v[$i + 5];
		printf "    surface flags: %#8.8x\n", $v[$i + 6];
		printf "    fvf: %#8.8x\n", $v[$i + 7];
		printf "    tc: %d\n", $v[$i + 8];
	}
}
sub read_object_0910 {
	my ($cf) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		if ($id != $expected_id) {
			printf "unexpected chunk 7777,0910,%04x\n", $id;
			die;
		}
		read_object_0910_nnnn($cf, $id);
		$cf->r_chunk_close();
	}
}
sub read_object_0910_nnnn {
	my ($cf, $nnnn) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x1000 && do { read_object_0910_nnnn_1000($cf, $nnnn); last SWITCH; };
			$id == 0x1001 && do { read_object_0910_nnnn_1001($cf, $nnnn); last SWITCH; };
			$id == 0x1002 && do { read_object_0910_nnnn_1002($cf, $nnnn); last SWITCH; };
			$id == 0x1004 && do { read_object_0910_nnnn_1004($cf, $nnnn); last SWITCH; };
			$id == 0x1005 && do { read_object_0910_nnnn_1005($cf, $nnnn); last SWITCH; };
			$id == 0x1006 && do { read_object_0910_nnnn_1006($cf, $nnnn); last SWITCH; };
			$id == 0x1007 && do { read_object_0910_nnnn_1007($cf, $nnnn); last SWITCH; };
			$id == 0x1008 && do { read_object_0910_nnnn_1008($cf, $nnnn); last SWITCH; };
			$id == 0x1009 && do { read_object_0910_nnnn_1009($cf, $nnnn); last SWITCH; };
			$id == 0x1010 && do { read_object_0910_nnnn_1010($cf, $nnnn); last SWITCH; };
			$id == 0x1011 && do { read_object_0910_nnnn_1011($cf, $nnnn); last SWITCH; };
			$id == 0x1012 && do { read_object_0910_nnnn_1012($cf, $nnnn); last SWITCH; };
			$id == 0x1013 && do { read_object_0910_nnnn_1013($cf, $nnnn); last SWITCH; };
			printf "unexpected chunk 7777,0910,%04x,%04x size %d\n", $nnnn, $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_object_0910_nnnn_1000 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0910,%04x,1000\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('v');
	die unless $packet->length() == 0;
	printf "  mesh version: %#x\n", $version;
}
sub read_object_0910_nnnn_1001 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0910,%04x,1001\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($name) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  mesh name: '%s'\n", $name;
}
sub read_object_0910_nnnn_1002 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0910,%04x,1002\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($flags) = $packet->unpack('C');
	die unless $packet->length() == 0;
	printf "  mesh flags: %#2.2x\n", $flags;
}
sub read_object_0910_nnnn_1004 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0910,%04x,1004\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('f3f3');
	die unless $packet->length() == 0;
	printf "  bounding box\n";
	printf "    min: %f, %f, %f\n", @v[0 .. 2];
	printf "    max: %f, %f, %f\n", @v[3 .. 5];
}
sub read_object_0910_nnnn_1005 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0910,%04x,1005\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('V/(f3)');
	printf "  vertex count: %d\n", ($#v + 1)/3;
	return unless main::full_dump();
	if ($packet->length() == 0) {
		for (my $i = 0; $i <= $#v; $i += 3) {
			printf "  vertex%d: %f, %f, %f\n", $i/3, @v[$i .. $i + 2];
		}
	} else {
		for (my $i = 0; $i <= $#v; $i += 3) {
			printf "  vertex%d: %f, %f, %f\n", $i/3, @v[$i .. $i + 2];
			my @a = $packet->unpack('C/V');
			printf "    adjacent faces: %s\n", join(', ', @a);
		}
	}
	die unless $packet->length() == 0;
}
sub read_object_0910_nnnn_1006 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0910,%04x,1006\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('V/(V2V2V2)');
	die unless $packet->length() == 0;
	printf "  face count: %d\n", ($#v + 1)/6;
	return unless main::full_dump();
	for (my $i = 0; $i <= $#v; $i += 6) {
		printf "  face%d\n", $i/6;
		printf "    vertices: %d, %d, %d\n", $v[$i+0], $v[$i+2], $v[$i+4];
		printf "    maps: %d, %d, %d\n", $v[$i+1], $v[$i+3], $v[$i+5];
	}
}
sub read_object_0910_nnnn_1007 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0910,%04x,1007\n", $nnnn;
}
sub read_object_0910_nnnn_1008 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0910,%04x,1008\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	printf "  vmref count: %d\n", $count;
	return unless main::full_dump();
	for (my $i = 0; $i < $count; $i++) {
		printf "  vmref%d\n", $i;
		my @v = $packet->unpack('C/(V2)');
		for (my $j = 0; $j <= $#v; $j += 2) {
			printf "    set%d\n", $j/2;
			printf "      vmap: %d\n      vmap entry: %d\n", @v[$j .. $j + 1];
		}
	}
	die unless $packet->length() == 0;
}
sub read_object_0910_nnnn_1009 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0910,%04x,1009\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('v');
	printf "  surface count: %d\n", $count;
	for (my $i = 0; $i < $count; $i++) {
		my @v = $packet->unpack('Z*V/V');
		printf "  surface%d\n", $i;
		printf "    name: '%s'\n", $v[0];
		printf "      face count: %d\n", $#v;
		next unless main::full_dump();
		for (my $j = 1; $j <= $#v; $j++) {
			printf "    face%d: %d\n", $j-1, $v[$j];
		}
	}
	die unless $packet->length() == 0;
}
sub read_object_0910_nnnn_1010 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0910,%04x,1010\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @unk = $packet->unpack('VV');
	printf "  mesh option0: %#8.8x\n", $unk[0];
	printf "  mesh option1: %#8.8x\n", $unk[1];
	die unless $packet->length() == 0;
}
sub read_object_0910_nnnn_1012 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0910,%04x,1012\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($vmap_count) = $packet->unpack('V');
	printf "  vmap count: %d\n", $vmap_count;
	for (my $i = 0; $i < $vmap_count; $i++) {
		my ($name, $value_dim, $has_pidata, $value_type, $data_count) =
				$packet->unpack('Z*C3V');
		die unless ($value_dim == 1 || $value_dim == 2);
		die unless ($value_type == 0 || $value_type == 1);
		die unless ($has_pidata == 0 || $has_pidata == 1);
		printf "  vmap%d\n", $i;
		printf "    name: '%s'\n", $name;
		printf "    vmap entry dimension: %d\n", $value_dim;
		printf "    vmap type: %s\n", $value_type == 0 ? "vmtUV" : "vmtWeight";
		printf "    has discontig uv map: %s\n", $has_pidata != 0 ? "yes" : "no";
		printf "    data count: %d\n", $data_count;
		my @d = $packet->unpack("(f$value_dim)$data_count");
		my @v = $packet->unpack("V$data_count");
		my @p;
		if ($has_pidata == 1) {
			@p = $packet->unpack("V$data_count");
		}
		next unless main::full_dump();
		for (my $j = 0; $j < $data_count; $j++) {
			printf "    map%d\n", $j;
			printf "      vertex: %d\n", $v[$j];
			if ($has_pidata == 1) {
			printf "      face: %d\n", $p[$j];
			}
			if ($value_dim == 1 && $value_type == 1) {
				printf "      weight: %f\n", $d[$j];
			} elsif ($value_dim == 2 && $value_type == 0) {
				my $o = $j*$value_dim;
				printf "      uv: %f, %f\n", @d[$o .. $o + 1];
			} else {
				die;
			}
		}
	}
	die unless $packet->length() == 0;
}
sub read_object_0910_nnnn_1013 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0910,%04x,1013\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	die unless $packet->length() % 4 == 0;
	my $count = $packet->length() / 4;
	my @sg = $packet->unpack("l$count");
	die unless $packet->length() == 0;
	printf "  smooth group count: %d\n", $#sg + 1;
	return unless main::full_dump();
	for (my $i = 0; $i <= $#sg; $i++) {
		printf "  face%d sg: %d\n", $i, $sg[$i];
	}
}
sub read_object_0911 {
	my ($cf) = @_;
	printf "chunk 7777,0911\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	printf "  0911length: %d\n", $packet->length();
}
sub read_object_0912 {
	my ($cf) = @_;
	printf "chunk 7777,0912\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($userdata) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  userdata: %s\n", ($userdata eq '') ? "not present" : "present";
}
sub read_object_0916 {
	my ($cf) = @_;
	printf "chunk 7777,0916\n";
}
sub read_object_0920 {
	my ($cf) = @_;
	printf "chunk 7777,0920\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('f3f3');
	die unless $packet->length() == 0;
	printf "  transform\n";
	printf "    position: %f, %f, %f\n", @v[0 .. 2];
	printf "    rotation: %f, %f, %f\n", @v[3 .. 5];
}
sub read_object_0921 {
	my ($cf) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		if ($id != $expected_id) {
			printf "unexpected chunk 7777,0921,%04x\n", $id;
			die;
		}
		read_object_0921_nnnn($cf, $id);
		$cf->r_chunk_close();
	}
}
sub read_object_0921_nnnn {
	my ($cf, $nnnn) = @_;
	my $def_type = 0;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x0001 && do { read_object_0921_nnnn_0001($cf, $nnnn); last SWITCH; };
			$id == 0x0002 && do {
				if ($def_type == 0) {
					read_object_0921_nnnn_0002a($cf, $nnnn);
					$def_type = 1;
				} else {
					read_object_0921_nnnn_0002b($cf, $nnnn);
				}
				last SWITCH;
			};
			$id == 0x0003 && do { read_object_0921_nnnn_0003($cf, $nnnn); last SWITCH; };
			$id == 0x0004 && do { read_object_0921_nnnn_0004($cf, $nnnn); last SWITCH; };
			$id == 0x0005 && do { read_object_0921_nnnn_0005($cf, $nnnn); last SWITCH; };
			$id == 0x0006 && do { read_object_0921_nnnn_0006($cf, $nnnn); last SWITCH; };
			$id == 0x0007 && do { read_object_0921_nnnn_0007($cf, $nnnn); last SWITCH; };
			$id == 0x0008 && do { read_object_0921_nnnn_0008($cf, $nnnn); last SWITCH; };
			$id == 0x0009 && do { read_object_0921_nnnn_0009($cf, $nnnn); last SWITCH; };
			$id == 0x0010 && do { read_object_0921_nnnn_0010($cf, $nnnn); last SWITCH; };
			printf "unexpected chunk 7777,0921,%04x,%04x size %d\n", $nnnn, $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_object_0921_nnnn_0001 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0921,%04x,0001\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('v');
	die unless $packet->length() == 0;
	printf "  bone version: %#x\n", $version;
}
sub read_object_0921_nnnn_0002a {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0921,%04x,0002(a)\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($name, $parent, $vmap) = $packet->unpack('Z*Z*Z*');
	die unless $packet->length() == 0;
	printf "  bone name: '%s'\n", $name;
	printf "  parent bone: '%s'\n", $parent;
	printf "  vmap name: '%s'\n", $vmap;
}
sub read_object_0921_nnnn_0002b {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0921,%04x,0002(b)\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($name) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  name: '%s'\n", $name;
}
sub read_object_0921_nnnn_0003 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0921,%04x,0003\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('f3f3f');
	die unless $packet->length() == 0;
	printf "  bind position: %f, %f, %f\n", @v[0 .. 2];
	printf "  bind rotation: %f, %f, %f\n", @v[3 .. 5];
	printf "  bind length: %f\n", $v[6];
}
sub read_object_0921_nnnn_0004 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0921,%04x,0004\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($material) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  material: '%s'\n", $material;
}
sub read_object_0921_nnnn_0005 {
	use constant shape_names => { 0 => 'undefined', 1 => 'box', 2 => 'sphere', 3 => 'cylinder' };
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0921,%04x,0005\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($type, $flags, @box) = $packet->unpack('vvf9f3f3');
	printf "  type: %s\n", shape_names->{$type};
	printf "  bone shape flags: %#4.4x\n", $flags;
	printf "  box\n";
	printf "    rotate\n";
	printf "      %f, %f, %f\n", @box[0 .. 2];
	printf "      %f, %f, %f\n", @box[3 .. 5];
	printf "      %f, %f, %f\n", @box[6 .. 8];
	printf "    translate: %f, %f, %f\n", @box[9 .. 11];
	printf "    halfsize: %f, %f, %f\n", @box[12 .. 14];
	my @sphere = $packet->unpack('f3f');
	printf "  sphere\n";
	printf "    center: %f, %f, %f\n", @sphere[0 .. 2];
	printf "    radius: %f\n", $sphere[3];
	my @cylinder = $packet->unpack('f3f3ff');
	printf "  cylinder\n";
	printf "    center: %f, %f, %f\n", @cylinder[0 .. 2];
	printf "    direction: %f, %f, %f\n", @cylinder[3 .. 5];
	printf "    height: %f\n", $cylinder[6];
	printf "    radius: %f\n", $cylinder[7];
	die unless $packet->length() == 0;
}
sub read_object_0921_nnnn_0006 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0921,%04x,0006\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('V(f2f2)3ff');
	printf "  joint type: %d\n", $v[1];
	printf "  limits\n";
	for (my $i = 0; $i < 3; $i++) {
		my $o = $i * 4 + 1;
		printf "    limit%d\n", $i;
		printf "      range: %f, %f\n", @v[$o .. $o + 1];
		printf "      spring factor: %f\n", $v[$o + 2];
		printf "      damping factor: %f\n", $v[$o + 2];
	}
	printf "  spring factor: %f\n", $v[13];
	printf "  damping factor: %f\n", $v[14];
}
sub read_object_0921_nnnn_0007 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0921,%04x,0007\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('ff3');
	die unless $packet->length() == 0;
	printf "  mass: %f\n", $v[0];
	printf "  center of mass: %f, %f, %f\n", @v[1 .. 3];
}
sub read_object_0921_nnnn_0008 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0921,%04x,0008\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($flags) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  ik_flags: %#8.8x\n", $flags;
}
sub read_object_0921_nnnn_0009 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0921,%04x,0009\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('ff');
	die unless $packet->length() == 0;
	printf "  break force: %f\n", $v[0];
	printf "  break torque: %f\n", $v[1];
}
sub read_object_0921_nnnn_0010 {
	my ($cf, $nnnn) = @_;
	printf "chunk 7777,0921,%04x,0010\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($friction) = $packet->unpack('f');
	die unless $packet->length() == 0;
	printf "  friction: %f\n", $friction;
}
sub read_object_0922 {
	my ($cf) = @_;
	printf "chunk 7777,0922\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('Z*VZ*V');
	printf "  creator: '%s'\n", $v[0];
	printf "  create time: %s", ctime($v[1]);
	printf "  editor: '%s'\n", $v[2];
	printf "  edit time: %s", ctime($v[3]);
}
sub read_object_0923 {
	my ($cf) = @_;
	printf "chunk 7777,0923\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($partition_count) = $packet->unpack('V');
	for (my $i = 0; $i < $partition_count; $i++) {
		my @v = $packet->unpack('Z*V/(Z*)');
		printf "  partition%d: '%s'\n", $i, $v[0];
		for (my $j = 1; $j <= $#v; $j++) {
			printf "    bone%d: '%s'\n", $j-1, $v[$j];
		}
	}
	die unless $packet->length() == 0;
}
sub read_object_0924 {
	my ($cf) = @_;
	printf "chunk 7777,0924\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($unk) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  unknown: '%s'\n", $unk;
}
sub read_object_0925 {
	my ($cf) = @_;
	printf "chunk 7777,0925\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($lod_ref) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  LOD::Reference: '%s'\n", $lod_ref;
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
ActorEditor .object dumper
Usage: ae_dump_object.pl <input_file>
END
}

sub full_dump { return 1; }

(my $fn = $ARGV[0]) or die usage;
ae_object::read($fn);
