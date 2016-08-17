#!perl -w -I \temp\1\bin

#######################################################################
package level_wallmarks;
use strict;
use stkutils::data_packet;
use stkutils::chunked_file;
sub read {
	my ($fn) = @_;
	printf "dumping '%s'\n", $fn;
	my $cf = stkutils::chunked_file->new($fn, 'r') or die;
	my ($id, $size) = $cf->r_chunk_open();
	die unless defined $id;
	printf "chunk %d size %d(%#lx)\n", $id, $size, $size;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	read_wallmarks($packet);
	$cf->r_chunk_close();
	$cf->close();
}
sub read_wallmarks {
	my ($packet) = @_;
	my ($wallmark_count) = $packet->unpack('V');
	printf "  wallmark count: %d\n", $wallmark_count;
	for (my $i = 0; $i < $wallmark_count; $i++) {
		my ($set_count, $shader, $textures) = $packet->unpack('VZ*Z*');
		printf "  wallmark%d\n", $i;
		printf "    set_count: %d\n", $set_count;
		printf "    shader: '%s'\n", $shader;
		printf "    textures: '%s'\n", $textures;
		for (my $j = 0; $j < $set_count; $j++) {
			printf "    set%d\n", $j;
			my @bounds = $packet->unpack('f4');
			printf "      bounds\n        p: %f, %f, %f\n        r: %f\n", @bounds;
			my ($vertex_count) = $packet->unpack('V');
			printf "      vertex_count: %d\n", $vertex_count;
			my @v = $packet->unpack("(f3Vf2)$vertex_count");
			for (my $k = 0; $k < $vertex_count; $k++) {
				my $o = $k*6;
				printf "        vertex%d\n", $k;
				printf "          p: %f, %f, %f\n", @v[$o + 0 .. $o + 2];
				printf "          color: %#08lx\n", $v[$o + 3];
				printf "          u, v: %f, %f\n", @v[$o + 4 .. $o + 5];
			}
		}
	}
	die unless $packet->length() == 0;
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R. level.wallmarks dumper
Usage: dump_wallmarks.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();
level_wallmarks::read($fn);
