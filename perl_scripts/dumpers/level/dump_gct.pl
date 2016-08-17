#!perl -w -I \temp\1\bin

use constant GCT_HEADER		=> 0x0;
use constant GCT_DATA		=> 0x1;

use constant chunk_names => {
	0x0		=> 'GCT_HEADER',
	0x1		=> 'GCT_DATA',
};

#######################################################################
package level_gct;
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
		printf "chunk '%s' size %d(%#lx)\n", ::chunk_names->{$id}, $size, $size;
		SWITCH: {
			$id == ::GCT_HEADER && do { read_header($cf); last SWITCH; };
			$id == ::GCT_DATA && do { read_data($cf); last SWITCH; };
			die "unexpected chunk id $id\n";
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_header {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version, $num_nodes, $num_vertices) = $packet->unpack('VVV');
	my @level_guid = $packet->unpack('V4');
	my @game_guid = $packet->unpack('V4');
	die unless $packet->length() == 0;
	printf "  version: %d\n", $version;
	die unless $version == 8;
	printf "  num_nodes: %d\n", $num_nodes;
	printf "  num_vertices: %d\n", $num_vertices;
}
sub read_data {
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my $n = $packet->length() / 6;
	my @v = $packet->unpack("(vf)$n");
	for (my $i = 0; $i < $n; $i++) {
		my $o = $i*2;
		printf "  vertex%d: %d (%f)\n", $i, @v[$o .. $o + 1];
	}
	die unless $packet->length() == 0;
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
S.T.A.L.K.E.R. level.gct dumper
Usage: dump_gct.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage();
level_gct::read($fn);
