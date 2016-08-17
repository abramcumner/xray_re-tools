#!perl -w -I \temp\1\bin

#######################################################################
package level_snd_static;
use strict;
use stkutils::data_packet;
use stkutils::chunked_file;
sub read {
	my ($fn) = @_;
	printf "dumping '%s'\n", $fn;
	my $cf = stkutils::chunked_file->new($fn, 'r') or die;
	while (1) {
		my ($snd_id, $size) = $cf->r_chunk_open();
		defined $snd_id or last;
		while (1) {
			my ($id, $size) = $cf->r_chunk_open();
			defined $id or last;
			printf "sound%d\n", $snd_id;
			my $packet = stkutils::data_packet->new($cf->r_chunk_data());
			read_sound($packet);
			$cf->r_chunk_close();
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_sound {
	my ($packet) = @_;
	my @p = $packet->unpack('Z*f3ffl2l2l2');
	printf "  name: '%s'\n", $p[0];
	printf "  position: %f, %f, %f\n", @p[1..3];
	printf "  volume: %f\n", $p[4];
	printf "  freq: %f\n", $p[5];
	printf "  active_time: %d, %d\n", @p[6..7];
	printf "  play_time: %d, %d\n", @p[8..9];
	printf "  pause_time: %d, %d\n", @p[10..11];
	die unless $packet->length() == 0;
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R. level.snd_static dumper
Usage: dump_snd_static.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();
level_snd_static::read($fn);
