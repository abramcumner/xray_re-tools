#!perl -w -I \temp\1\bin

#######################################################################
package cform2obj;
use strict;
use IO::File;
sub process {
	my ($fn_in, $fn_out) = @_;
	printf "reading '%s'\n", $fn_in;
	my $fh = IO::File->new($fn_in, 'r') or die;
	binmode $fh;

	my $data;
	$fh->read($data, 0x24);
	my ($version, $vertcount, $facecount, @box) = unpack 'VVVf6', $data;

	printf "version: %d\n", $version;
	printf "vertcount: %d\n", $vertcount;
	printf "facecount: %d\n", $facecount;
	printf "bbox:\n  min: %f, %f, %f\n  max: %f, %f, %f\n", @box[0..5];

	$fh->read($data, $vertcount*12);
	my @vert = unpack "(f3)$vertcount", $data;

	$fh->read($data, $facecount*16);
	my @tri = unpack "(V3V)$facecount", $data;

	$fh->close();

	$fh = IO::File->new($fn_out, 'w+') or die;
	printf $fh "newmtl cform\n";
	printf $fh "mtllib cform\n";
	printf $fh "g 0\n";
	printf $fh "usemtl cform\n";
	my $mult = 1.0;
	for (my $i = 0; $i < $vertcount; $i++) {
		my $o = $i * 3;
		my $p0 = $vert[$o + 0]*$mult;
		my $p1 = $vert[$o + 1]*$mult;
		my $p2 = $vert[$o + 2]*$mult;
		printf $fh "v %f %f %f\n", $p0, $p1, $p2;
	}
	my $size_x = $box[3] - $box[0];
	my $size_z = $box[5] - $box[2];
	for (my $i = 0; $i < $vertcount; $i++) {
		my $o = $i * 3;
		my $u = ($vert[$o + 0] - $box[0])/$size_x*32.0 - 16.0;
		my $v = ($vert[$o + 2] - $box[2])/$size_z*32.0 - 16.0;
		printf $fh "vt %f %f\n", $u, $v;
	}
	for (my $i = 0; $i < $facecount; $i++) {
		my $o = $i * 4;

		my $v1 = $tri[$o];
		my $v2 = $tri[$o+1];
		my $v3 = $tri[$o+2];

		my $v1x = $vert[$v1*3];
		my $v1y = $vert[$v1*3 + 1];
		my $v1z = $vert[$v1*3 + 2];

		my $v2x = $vert[$v2*3];
		my $v2y = $vert[$v2*3 + 1];
		my $v2z = $vert[$v2*3 + 2];

		my $v3x = $vert[$v3*3];
		my $v3y = $vert[$v3*3 + 1];
		my $v3z = $vert[$v3*3 + 2];

		my $e1x = $v2x - $v1x;
		my $e1y = $v2y - $v1y;
		my $e1z = $v2z - $v1z;

		my $e2x = $v1x - $v3x;
		my $e2y = $v1y - $v3y;
		my $e2z = $v1z - $v3z;

		my $nx = $e1y*$e2z - $e1z*$e2y;
		my $ny = $e1z*$e2x - $e1x*$e2z;
		my $nz = $e1x*$e2y - $e1y*$e2x;

		printf $fh "vn %f %f %f\n", $nx, $ny, $nz;
	}
	for (my $i = 0; $i < $facecount; $i++) {
		my $o = $i * 4;
		my $v1 = $tri[$o];
		my $v2 = $tri[$o+1];
		my $v3 = $tri[$o+2];
		die if ($v1 == $v2 || $v2 == $v3 || $v3 == $v1);

		die if ($vert[$v1*3+0] == $vert[$v2*3+0] && $vert[$v1*3+1] == $vert[$v2*3+1] && $vert[$v1*3+2] == $vert[$v2*3+2]) ||
			($vert[$v2*3+0] == $vert[$v3*3+0] && $vert[$v2*3+1] == $vert[$v3*3+1] && $vert[$v2*3+2] == $vert[$v3*3+2]) ||
			($vert[$v3*3+0] == $vert[$v1*3+0] && $vert[$v3*3+1] == $vert[$v1*3+1] && $vert[$v3*3+2] == $vert[$v1*3+2]);

		$v1++;
		$v2++;
		$v3++;
		my $vn = $i + 1;
		printf $fh "f %d/%d/%d %d/%d/%d %d/%d/%d\n", $v1, $v1, $vn, $v2, $v2, $vn, $v3, $v3, $vn;
	}
	$fh->close();
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
Usage: cform2obj.pl <input_file> <output_file>
END
}

(my $fn_in = $ARGV[0]) or die usage();
(my $fn_out = $ARGV[1]) or die usage();
cform2obj::process($fn_in, $fn_out);
