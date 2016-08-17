#!perl -w -I \temp\1\bin

#######################################################################
package level_snd_env;
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
			$id == 0x0 && do { read_env_ids($packet); last SWITCH; };
			$id == 0x1 && do { read_geom($packet); last SWITCH; };
			die "unexpected chunk id $id\n";
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_env_ids {
	my ($packet) = @_;
	printf "sound environment ids\n";
	for (my $i = 0; $packet->length() > 0; $i++) {
		my ($name) = $packet->unpack('Z*');
		printf "  id%d: %s\n", $i, $name;
	}
	die unless $packet->length() == 0;
}
sub read_geom {
	my ($packet) = @_;
	printf "sound environment geometry\n";

	my @hdr = $packet->unpack('VVVf6');

	printf "  version: %d\n", $hdr[0];
	printf "  bbox:\n    min: %f, %f, %f\n    max: %f, %f, %f\n", @hdr[3..8];

	printf "  vertcount: %d\n", $hdr[1];
	my @vert = $packet->unpack("(f3)$hdr[1]");
	for (my $i = 0; $i < $hdr[1]; $i++) {
		printf "    vertex%d: %f, %f, %f\n", $i, @vert[($i*3)..($i*3+2)];
	}
	printf "  facecount: %d\n", $hdr[2];
	my @tri = $packet->unpack("(V3V)$hdr[2]");
	for (my $i = 0; $i < $hdr[1]; $i++) {
		printf "    face%d: %d, %d, %d\n", $i, @tri[($i*4)..($i*4+2)];
		my $t = $tri[$i*4 + 3];
		printf "      material: %d\n", $t & 0x3fff;
		printf "      suppress_shadows: %s\n", ($t & 0x4000) == 0 ? "no" : "yes";
		printf "      suppress_wm: %s\n", ($t & 0x8000) == 0 ? "no" : "yes";
		printf "      sector: %d\n", ($t >> 16) & 0xffff;
	}
	die unless $packet->length() == 0;
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R. level.snd_env dumper
Usage: dump_snd_env.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();
level_snd_env::read($fn);
