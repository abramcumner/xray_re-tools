#!perl -w -I \temp\1\bin

#######################################################################
package level_hom;
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
	printf "HOM version chunk\n";
	my ($version) = $packet->unpack('V');
	printf "  version: %#lx\n", $version;
	die unless $packet->length() == 0;
}
sub read_polys {
	my ($packet) = @_;
	my $count = $packet->length() / 0x28;
	die unless $packet->length() % 0x28 == 0;
	my @v = $packet->unpack("(f3f3f3V)$count");
	die unless $packet->length() == 0;
	printf "HOM polygon count: %d\n", $count;
	for (my $i = 0; $i < $count; $i++) {
		my $o = $i*10;
		printf "  poly%d\n", $i;
		printf "    v0: %f, %f, %f\n", @v[$o + 0 .. $o + 2];
		printf "    v1: %f, %f, %f\n", @v[$o + 3 .. $o + 5];
		printf "    v2: %f, %f, %f\n", @v[$o + 6 .. $o + 8];
		printf "    flags: %#lx\n", $v[9];
	}
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R. level.hom dumper
Usage: dump_hom.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();
level_hom::read($fn);
