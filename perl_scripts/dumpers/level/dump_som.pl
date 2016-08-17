#!perl -w -I \temp\1\bin

#######################################################################
package level_som;
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
		my $packet = stkutils::data_packet->new($cf->r_chunk_data());
		SWITCH: {
			$id == 0x0 && do { read_version($packet); last SWITCH; };
			$id == 0x1 && do { read_polys($packet); last SWITCH; };
			die "unexpected chunk id $id\n";
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_version {
	my ($packet) = @_;
	printf "SOM version chunk\n";
	my ($version) = $packet->unpack('V');
	printf "  version: %#lx\n", $version;
	die unless $packet->length() == 0;
}
sub read_polys {
	my ($packet) = @_;
	my $count = $packet->length() / 0x2c;
	die unless $packet->length() % 0x2c == 0;
	my @v = $packet->unpack("(f3f3f3Vf)$count");
	die unless $packet->length() == 0;
	printf "SOM polygon count: %d\n", $count;
	for (my $i = 0; $i < $count; $i++) {
		my $o = $i*11;
		printf "  poly%d\n", $i;
		printf "    v0: %f, %f, %f\n", @v[$o + 0 .. $o + 2];
		printf "    v1: %f, %f, %f\n", @v[$o + 3 .. $o + 5];
		printf "    v2: %f, %f, %f\n", @v[$o + 6 .. $o + 8];
		printf "    b2sided: %d\n", $v[9];
		printf "    occ: %f\n", $v[10];
	}
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R. level.som dumper
Usage: dump_som.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();
level_som::read($fn);
