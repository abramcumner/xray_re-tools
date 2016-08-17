#!perl -w -I \temp\1\bin

use constant CHUNK_RGB	=> 0x0;
use constant CHUNK_HEMI	=> 0x1;
use constant CHUNK_SUN	=> 0x2;

use constant chunk_names => {
	0x0	=> 'CHUNK_RGB',
	0x1	=> 'CHUNK_HEMI',
	0x2	=> 'CHUNK_SUN',
};

#######################################################################
package build_lights;
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
		printf "chunk '%s' size %d(%#lx)\n", ::chunk_names->{$id}, $size, $size;
		SWITCH: {
			$id == ::CHUNK_RGB && do { read_lights($cf); last SWITCH; };
			$id == ::CHUNK_HEMI && do { read_lights($cf); last SWITCH; };
			$id == ::CHUNK_SUN && do { read_lights($cf); last SWITCH; };
			die "unexpected chunk id $id\n";
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
# don't read the 'extra' field for ShoC
sub read_lights {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	die unless $packet->length % 0x68 == 0;
	my $n = $packet->length() / 0x68;
	my @v = $packet->unpack("(vvf3f3f3fff3f(f3)3V)$n");
	printf "lights: %d\n", $n;
	for (my $i = 0; $i < $n; ++$i) {
		my $o = $i*27;
#if ($v[$o + 16] >= 1.0 - 0.001) {
		printf "  light%d:\n", $i;
		printf "    type: %d\n", $v[$o + 0];
		printf "    level: %d\n", $v[$o + 1];
		printf "    diffuse: %f, %f, %f\n", @v[$o + 2 .. $o + 4];
		printf "    position: %f, %f, %f\n", @v[$o + 5 .. $o + 7];
		printf "    direction: %f, %f, %f\n", @v[$o + 8 .. $o + 10];
		printf "    range: %f\n", $v[$o + 11];
		printf "    range2: %f\n", $v[$o + 12];
		printf "    attenuation: %f, %f, %f\n", @v[$o + 13 .. $o + 15];
		printf "    energy: %f\n", $v[$o + 16];
		printf "    tri:\n";
		printf "      p0: %f, %f, %f\n", @v[$o + 17 .. $o + 19];
		printf "      p1: %f, %f, %f\n", @v[$o + 20 .. $o + 22];
		printf "      p2: %f, %f, %f\n", @v[$o + 23 .. $o + 25];
		printf "    extra: %f\n", $v[$o + 26];
#}
	}
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R.: Clear Sky build.lights dumper
Usage: dump_build_lights.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();
build_lights::read($fn);
