#!perl -w -I \temp\1\bin

#######################################################################
package dm_file;
use strict;
use IO::File;
use stkutils::data_packet;
sub read {
	my ($fn) = @_;
	printf "dumping '%s'\n", $fn;
	my $fh = IO::File->new($fn, 'r') or die;
	binmode $fh;

	my @stat_info = $fh->stat();
	my $data;
	$fh->read($data, $stat_info[7]) or die;
	my $packet = stkutils::data_packet->new($data);

	my ($shader, $texture) = $packet->unpack('Z*Z*');
	printf "  shader: '%s'\n", $shader;
	printf "  texture: '%s'\n", $texture;

	my ($flags, $min_scale, $max_scale, $vert_count, $index_count) = $packet->unpack('VffVV');
	printf "  flags: %#8.8lx\n", $flags;
	printf "  min_scale: %f\n", $min_scale;
	printf "  max_scale: %f\n", $max_scale;

	printf "  vert_count: %u\n", $vert_count;
	my @vb = $packet->unpack("(f3f2)$vert_count");
	for (my $i = 0; $i < $vert_count; $i++) {
		my $j = $i*5;
		printf "    vertex%d\n", $i;
		printf "      position: %f, %f, %f\n", @vb[$j .. $j + 2];
		printf "      u, v: %f, %f\n", @vb[$j + 3 .. $j + 4];
	}

	printf "  index_count: %u\n", $index_count;
	my @ib = $packet->unpack("v$index_count");

	die unless $packet->length() == 0;
	$fh->close();
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R. .dm dumper
Usage: dump_dm.pl <input_file>
END
}

(my $dm_file = $ARGV[0]) or die usage();
dm_file::read($dm_file);
