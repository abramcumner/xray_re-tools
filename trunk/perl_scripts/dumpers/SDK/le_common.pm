package le_common;

use strict;
use stkutils::data_packet;
use POSIX qw(ctime);

sub read_guid {
	my ($cf) = @_;
	printf "chunk 7000\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @guid = $packet->unpack('VvvC8');
	die unless $packet->length() == 0;
	printf "  guid: %08x,%04x,%04x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\n", @guid;
}

sub read_last_modified {
	my ($cf,$nnnn) = @_;
	if (defined $nnnn) {
		printf "chunk %04x,7777\n", $nnnn;
	} else {
		printf "chunk 7777\n";
	}
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($editor, $time) = $packet->unpack('Z*V');
	die unless $packet->length() == 0;
	printf "  editor: '%s'\n", $editor;
	printf "  time: %s", ctime($time);
}

1;
