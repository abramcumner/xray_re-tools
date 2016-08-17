#!perl -w -I \temp\1\bin

#######################################################################
package level_cform;
use strict;
use IO::File;
sub read {
	my ($fn) = @_;
	printf "dumping '%s'\n", $fn;
	my $fh = IO::File->new($fn, 'r') or die;
	binmode $fh;

	my $data;
	$fh->read($data, 0x24);
	my @hdr = unpack 'VVVf6', $data;

	printf "version: %d\n", $hdr[0];
	printf "bbox:\n  min: %f, %f, %f\n  max: %f, %f, %f\n", @hdr[3..8];

	printf "vertcount: %d\n", $hdr[1];
	$fh->read($data, $hdr[1]*12);
	my @vert = unpack "(f3)$hdr[1]", $data;
	for (my $i = 0; $i < $hdr[1]; $i++) {
		printf "  vertex%d: %f, %f, %f\n", $i, @vert[($i*3)..($i*3+2)];
	}

	printf "facecount: %d\n", $hdr[2];
	$fh->read($data, $hdr[2]*16);
	my @tri = unpack "(V3V)$hdr[2]", $data;
	for (my $i = 0; $i < $hdr[2]; $i++) {
		my $o = $i*4;
		printf "  face%d: %d, %d, %d\n", $i, @tri[$o .. $o + 2];
		my $t = $tri[$o + 3];
		printf "    material: %d\n", $t & 0x3fff;
		printf "    suppress_shadows: %s\n", ($t & 0x4000) == 0 ? "no" : "yes";
		printf "    suppress_wm: %s\n", ($t & 0x8000) == 0 ? "no" : "yes";
		printf "    sector: %d\n", ($t >> 16) & 0xffff;
	}
	$fh->close();
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R. level.cform dumper
Usage: dump_cform.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();
level_cform::read($fn);
