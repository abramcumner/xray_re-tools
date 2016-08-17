#!perl -w -I \temp\1\bin

#######################################################################
package le_ai_map;
use strict;
use stkutils::data_packet;
use stkutils::chunked_file;
use le_common;
use POSIX qw(floor);
sub read {
	my ($fn) = @_;
	my $cf = stkutils::chunked_file->new($fn, 'r') or die;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7000 && do { le_common::read_guid($cf); last SWITCH; };
			$id == 0x800d && do { read_aimap($cf); last SWITCH; };
			printf "unexpected chunk %04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_aimap {
	my ($cf) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7777 && do { le_common::read_last_modified($cf,0x800e); last SWITCH; };
			$id == 0x0001 && do { read_aimap_0001($cf); last SWITCH; };
			$id == 0x0002 && do { read_aimap_0002($cf); last SWITCH; };
			$id == 0x0003 && do { read_aimap_0003($cf); last SWITCH; };
			$id == 0x0004 && do { read_aimap_0004($cf); last SWITCH; };
			$id == 0x0006 && do { read_aimap_0006($cf); last SWITCH; };
			$id == 0x0007 && do { read_aimap_0007($cf); last SWITCH; };
			$id == 0x0008 && do { read_aimap_0008($cf); last SWITCH; };
			$id == 0x0009 && do { read_aimap_0009($cf); last SWITCH; };
			printf "unexpected chunk 800d,%04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_aimap_0001 {
	my ($cf) = @_;
	printf "chunk 800d,0001\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('v');
	die unless $packet->length() == 0;
	printf "  ai_map version: %#x\n", $version;
}
sub read_aimap_0002 {
	my ($cf) = @_;
	printf "chunk 800d,0002\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($flags) = $packet->unpack('V');
	die unless $packet->length() == 0;
	printf "  flags: %#08x\n", $flags;
	printf "    AI Map::Common::Draw Nodes: %s\n", ($flags & 0x02) ? "no" : "yes";
	printf "    AI Map::Common::Slow Calculate Mode: %s\n", ($flags & 0x04) ? "yes" : "no";
}
my @aabb;
sub read_aimap_0003 {
	my ($cf) = @_;
	printf "chunk 800d,0003\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @box = $packet->unpack('f6');
	die unless $packet->length() == 0;
	printf "  min: %f, %f, %f\n", @box[0 .. 2];
	printf "  max: %f, %f, %f\n", @box[3 .. 5];
	@aabb = @box;
}
sub read_aimap_0004 {
	my ($cf) = @_;
	printf "chunk 800d,0004\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('ffff');
	die unless $packet->length() == 0;
	printf "  cell_size: %f\n", $v[0];
	printf "  factor_y: %f\n", $v[1];
	printf "  AI Map::Params::Can Up: %f\n", $v[2];
	printf "  AI Map::Params::Can Down: %f\n", $v[3];
}
sub read_aimap_0006 {
	my ($cf) = @_;
	printf "chunk 800d,0006\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @nodes = $packet->unpack('V/(a[12]vsvsC)');
	printf "  node count: %d\n", ($#nodes + 1)/6;
if (1) {
my $min_x = 10000;
my $min_z = 10000;
my $max_x = -10000;
my $max_z = -10000;
	for (my $i = 0; $i <= $#nodes; $i += 6) {
		printf "  node%d\n", $i/6;
		my $x = $nodes[$i + 2]*0.7;
		my $y = ($nodes[$i + 3] + 1)*(1.0/65535.0)*($aabb[4] - $aabb[1]) + $aabb[1];
		my $z = $nodes[$i + 4]*0.7;
		printf "    offset: %f, %f, %f\n", $x, $y, $z;
if ($x < $min_x) { $min_x = $x; }
if ($z < $min_z) { $min_z = $z; }
if ($x > $max_x) { $max_x = $x; }
if ($z > $max_z) { $max_z = $z; }
	}
printf "  min: %f, %f, %f\n", $min_x, 0, $min_z;
printf "  max: %f, %f, %f\n", $max_x, 0, $max_z;
}
}
sub read_aimap_0007 {
	my ($cf) = @_;
	printf "chunk 800d,1007\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @snap = $packet->unpack('V/(Z*)');
	die unless $packet->length() == 0;
	printf "  snap object count: %d\n", $#snap + 1;
	for (my $i = 0; $i <= $#snap; $i++) {
		printf "  snap%d: '%s'\n", $i, $snap[$i];
	}
}
sub read_aimap_0008 {
	my ($cf) = @_;
	printf "chunk 800d,0008\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('fV');
	die unless $packet->length() == 0;
	printf "  AI Map::Common::Visible Radius: %f\n", $v[0];
	printf "  AI Map::Params::Brush Size: %d\n", $v[1];
}
sub read_aimap_0009 {
	my ($cf) = @_;
	printf "chunk 800d,0009\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($height) = $packet->unpack('f');
	die unless $packet->length() == 0;
	printf "  AI Map::Common::Smooth Height: %f\n", $height;
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
LevelEditor ai_map.part dumper
Usage: le_dump_ai_map.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage;
le_ai_map::read($fn);
