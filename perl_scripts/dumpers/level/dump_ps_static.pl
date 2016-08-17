#!perl -w -I \temp\1\bin

#######################################################################
package level_ps_static;
use strict;
use stkutils::data_packet;
use stkutils::chunked_file;
sub read {
	my ($fn) = @_;
	printf "dumping '%s'\n", $fn;
	my $cf = stkutils::chunked_file->new($fn, 'r') or die;
	my $version = 0;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		my $packet = stkutils::data_packet->new($cf->r_chunk_data());
		if ($id == 0 && $size == 4) {
			$version = $packet->unpack('V');
			goto skip;
		}
		if ($version == 0) {
			printf "particle%d\n", $id;
			read_particle($packet);
		} else {
			printf "particle%d\n", $id - 1;
			read_particle_cs($packet);
		}
skip:
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_particle {
	my ($packet) = @_;
	my ($name, @xform) = $packet->unpack('Z*f16');
	printf "  name: '%s'\n", $name;
	printf "  xform\n";
	printf "    %f, %f, %f, %f\n", @xform[0 .. 3];
	printf "    %f, %f, %f, %f\n", @xform[4 .. 7];
	printf "    %f, %f, %f, %f\n", @xform[8 .. 11];
	printf "    %f, %f, %f, %f\n", @xform[12 .. 15];
	die unless $packet->length() == 0;
}
sub read_particle_cs {
	my ($packet) = @_;
	my ($extra, $name, @xform) = $packet->unpack('vZ*f16');
	printf "  extra: %#4.4x\n", $extra;
	printf "  name: '%s'\n", $name;
	printf "  xform\n";
	printf "    %f, %f, %f, %f\n", @xform[0 .. 3];
	printf "    %f, %f, %f, %f\n", @xform[4 .. 7];
	printf "    %f, %f, %f, %f\n", @xform[8 .. 11];
	printf "    %f, %f, %f, %f\n", @xform[12 .. 15];
	die unless $packet->length() == 0;
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R. level.ps_static dumper
Usage: dump_ps_static.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();
level_ps_static::read($fn);
