#!perl -w -I \temp\1\bin

#######################################################################
package level_fog_vol;
use strict;
use IO::File;
sub read {
	my ($fn) = @_;
	printf "dumping '%s'\n", $fn;
	my $fh = IO::File->new($fn, 'r') or die;
	binmode $fh;

	my $data;
	$fh->read($data, 0x6) or die;
	my ($version, $num_volumes) = unpack 'vV', $data;

	die unless ($version == 2 || $version == 3);

	printf "version: %d\n", $version;
	printf "volume count: %d\n", $num_volumes;
	for (my $i = 0; $i < $num_volumes; ++$i) {
		printf "volume%d:\n", $i;

		my $ltx = '';
		for (;;) {
			$fh->read($data, 1) or die;
			last if ($data eq "\n" || $data eq "\r");
			$ltx .= $data;
		}
		$fh->read($data, 1) or die;
		die unless $data eq "\n";

		printf "  ltx: '%s'\n", $ltx;

		$fh->read($data, 0x40) or die;
		my @xform = unpack 'f16', $data;
		printf "  xform:\n";
		printf "    %f, %f, %f, %f\n", @xform[0 .. 3];
		printf "    %f, %f, %f, %f\n", @xform[4 .. 7];
		printf "    %f, %f, %f, %f\n", @xform[8 .. 11];
		printf "    %f, %f, %f, %f\n", @xform[12 .. 15];

		$fh->read($data, 0x4) or die;
		my ($num_particles) = unpack 'V', $data;
		printf "  particle count: %d\n", $num_particles;

		for (my $j = 0; $j < $num_particles; ++$j) {
			$fh->read($data, 0x40);
			printf "    particle%d xform:\n", $j;
			my @xform = unpack 'f16', $data;
			printf "      %f, %f, %f, %f\n", @xform[0 .. 3];
			printf "      %f, %f, %f, %f\n", @xform[4 .. 7];
			printf "      %f, %f, %f, %f\n", @xform[8 .. 11];
			printf "      %f, %f, %f, %f\n", @xform[12 .. 15];
		}
	}
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R. level.fog_vol dumper
Usage: dump_fog_vol.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();
level_fog_vol::read($fn);
