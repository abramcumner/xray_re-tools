#!perl -w -I \temp\1\bin

#######################################################################
package le_wallmark;
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
			$id == 0x800e && do { read_wallmark($cf); last SWITCH; };
			printf "unexpected chunk %04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_wallmark {
	my ($cf) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7777 && do { le_common::read_last_modified($cf,0x800e); last SWITCH; };
			$id == 0x0001 && do { read_wallmark_0001($cf); last SWITCH; };
			$id == 0x0002 && do { read_wallmark_0002($cf); last SWITCH; };
			$id == 0x0003 && do { read_wallmark_0003($cf); last SWITCH; };
			$id == 0x0005 && do { read_wallmark_0005($cf); last SWITCH; };
			printf "unexpected chunk 800e,%04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_wallmark_0001 {
	my ($cf) = @_;
	printf "chunk 800e,0001\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('v');
	die unless $version == 3;
	die unless $packet->length() == 0;
	printf "  WM_CHUNK_VERSION: %d\n", $version;
}
sub read_wallmark_0002 {
	my ($cf) = @_;
	printf "chunk 800e,0002\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($flags) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  flags: %08x\n", $flags;
	printf "    Wallmark::Common::Draw Wallmarks: %s\n", ($flags & 0x1) ? "yes" : "no";
	printf "    Wallmark::Common::Alignment: %s\n", ($flags & 0x2) ? "By World Axis" : "By Camera";
}
sub read_wallmark_0003 {
	my ($cf) = @_;
	printf "chunk 800e,0002\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($width, $height, $rotate, $shader, $texture) = $packet->unpack('fffZ*Z*');
	die unless $packet->length() == 0;
	printf "  Wallmark::Common::Width: %f\n", $width;
	printf "  Wallmark::Common::Height: %f\n", $height;
	printf "  Wallmark::Common::Rotate: %f\n", $rotate;
	printf "  Wallmark::Common::Shader: '%s'\n", $shader;
	printf "  Wallmark::Common::Texture: '%s'\n", $texture;
}
sub read_wallmark_0005 {
	my ($cf) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		if ($id != $expected_id) {
			printf "unexpected chunk 800e,0005,%04x\n", $id;
			die;
		}
		read_wallmark_0005_nnnn($cf, $id);
		$cf->r_chunk_close();
	}
}
sub read_wallmark_0005_nnnn {
	my ($cf, $nnnn) = @_;
	printf "chunk 800e,0005,%04x\n", $nnnn;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count, $shader, $texture) = $packet->unpack('VZ*Z*');
	printf "  count: %d\n", $count;
	printf "  shader: '%s'\n", $shader;
	printf "  texture: '%s'\n", $texture;
	for (my $i = 0; $i < $count; $i++) {
		printf "  wallmark%d\n", $i;
		my ($selected) = $packet->unpack('C');
		printf "    selected: %d\n", $selected;
		my @box = $packet->unpack('f6');
		printf "    bounding box\n";
		printf "      min: %f, %f, %f\n", @box[0 .. 2];
		printf "      max: %f, %f, %f\n", @box[3 .. 5];
		my @sphere = $packet->unpack('f4');
		printf "    bounding sphere\n";
		printf "      center: %f, %f, %f\n", @sphere[0 .. 2];
		printf "      radius: %f\n", $sphere[3];
		my ($width, $height, $rotate, $vertex_count) = $packet->unpack('fffV');
		printf "    width: %f\n", $width;
		printf "    height: %f\n", $height;
		printf "    rotate: %f\n", $rotate;
		printf "    vertex_count: %d\n", $vertex_count;
		my @v = $packet->unpack("(f3Vf2)$vertex_count");
		for (my $k = 0; $k < $vertex_count; $k++) {
			my $o = $k*6;
			printf "      vertex%d\n", $k;
			printf "        p: %f, %f, %f\n", @v[$o + 0 .. $o + 2];
			printf "        color: %#08lx\n", $v[$o + 3];
			printf "        u, v: %f, %f\n", @v[$o + 4 .. $o + 5];
		}
	}
	die unless $packet->length() == 0;
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
LevelEditor wallmark.part dumper
Usage: le_dump_wallmark.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage;
le_wallmark::read($fn);
