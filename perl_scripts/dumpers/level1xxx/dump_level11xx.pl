#!perl -w -I \temp\1\bin

use constant FSL_HEADER		=> 0x1;
use constant FSL_SHADERS	=> 0x2;
use constant FSL_VISUALS	=> 0x3;
use constant FSL_VB		=> 0x4;
use constant FSL_CFORM		=> 0x5;
use constant FSL_PORTALS	=> 0x6;
use constant FSL_LIGHTS		=> 0x7;
use constant FSL_GLOWS		=> 0x9;
use constant FSL_SECTORS	=> 0xa;

use constant chunk_names => {
	0x1		=> 'FSL_HEADER',
	0x2		=> 'FSL_SHADERS',
	0x3		=> 'FSL_VISUALS',
	0x4		=> 'FSL_VB',
	0x5		=> 'FSL_CFORM',
	0x6		=> 'FSL_PORTALS',
	0x7		=> 'FSL_LIGHTS',
	0x9		=> 'FSL_GLOWS',
	0xa		=> 'FSL_SECTORS',
};

#######################################################################
package fsl_header;
use strict;
sub read {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($xrlc_version, $xrlc_quality, $name) = $packet->unpack('vvZ*@128');
	printf "  xrlc_version: %#x\n", $xrlc_version;
	printf "  xrlc_quality: %#x\n", $xrlc_quality;
	printf "  name: '%s'\n", $name;
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
	printf "  shaders + textures count: %d\n", $count;
	for (my $i = 0; $i < $count; $i++) {
		printf "  shader%d: '%s'\n", $i, $v[$i];
	}
	die unless $packet->length() == 0;
}

#######################################################################
package fsl_portals;
use strict;
sub read {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
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
package fsl_lights;
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
	my $count = $packet->length() / 0xb0;
	die unless $packet->length() % 0xb0 == 0;
	my @v = $packet->unpack("(Vf4f4f4f3f3f7VVZ*\@176)$count");
	die unless $packet->length() == 0;
	for (my $i = 0; $i < $count; $i++) {
		my $o = $i*29;
		printf "  light%d\n", $i;
		printf "    name: '%s'\n", $v[$o + 28];
		printf "    type: %s (%d)\n", lt_names->{$v[$o + 0]}, $v[$o + 0];
		printf "    diffuse: %f, %f, %f, %f\n", @v[$o + 1 .. $o + 4];
		printf "    specular: %f, %f, %f, %f\n", @v[$o + 5 .. $o + 8];
		printf "    ambient: %f, %f, %f, %f\n", @v[$o + 9 .. $o + 12];
		printf "    position: %f, %f, %f\n", @v[$o + 13 .. $o + 15];
		printf "    direction: %f, %f, %f\n", @v[$o + 16 .. $o + 18];
		printf "    range: %f\n", $v[$o + 19];
		printf "    falloff: %f\n", $v[$o + 20];
		printf "    attenuation0: %f\n", $v[$o + 21];
		printf "    attenuation1: %f\n", $v[$o + 22];
		printf "    attenuation2: %f\n", $v[$o + 23];
		printf "    theta: %f\n", $v[$o + 24];
		printf "    phi: %f\n", $v[$o + 25];
		printf "    unk0: %#8.8x\n", $v[$o + 26];
		printf "    unk1: %d\n", $v[$o + 27];
	}
}

#######################################################################
package fsl_glows;
use strict;
sub read {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my $count = $packet->length() / 0x18;
	die unless $packet->length() % 0x18 == 0;
	my @v = $packet->unpack("(f3fVV)$count");
	die unless $packet->length() == 0;
	for (my $i = 0; $i < $count; $i++) {
		my $o = $i*6;
		printf "  glow%d\n", $i;
		printf "    position: %f, %f, %f\n", @v[$o + 0 .. $o + 2];
		printf "    radius: %f\n", $v[$o + 3];
		printf "    texture index: %d\n", $v[$o + 4];
		printf "    shader index: %d\n", $v[$o + 5];
	}
}

#######################################################################
package fsl_vb;
use strict;
sub read {
if (0) {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my $num_bufs = $packet->unpack('V');
	for (my $i = 0; $i < $num_bufs; $i++) {
		my ($fvf, $n) = $packet->unpack('VV');
		printf "  buffer %d\n", $i;
	}
	die unless $packet->length() == 0;
}
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
		printf "chunk '%s' size %d\n", ::chunk_names->{$id&0x7fffffff}, $size;
		SWITCH: {
			$id == ::FSL_HEADER && do { fsl_header::read($cf); last SWITCH; };
			$id == ::FSL_SHADERS && do { fsl_shaders::read($cf); last SWITCH; };
#			$id == ::FSL_VISUALS && do { fsl_visuals::read($cf); last SWITCH; };
			$id == ::FSL_PORTALS && do { fsl_portals::read($cf); last SWITCH; };
#			$id == ::FSL_VB && do { fsl_vb::read($cf); last SWITCH; };
			$id == ::FSL_LIGHTS && do { fsl_lights::read($cf); last SWITCH; };
			$id == ::FSL_GLOWS && do { fsl_glows::read($cf); last SWITCH; };
#			$id == ::FSL_SECTORS && do { fsl_sectors::read($cf); last SWITCH; };
			printf "unexpected chunk %04x size %d\n", $id, $size;
#			die;
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
S.T.A.L.K.E.R. builds 1114, 1154 level dumper
Usage: dump_level11xx.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();
level::read($fn);
