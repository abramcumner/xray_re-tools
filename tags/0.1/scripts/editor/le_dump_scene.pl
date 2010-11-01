#!perl -w -I \temp\1\bin

#######################################################################
package le_scene;
use strict;
use stkutils::data_packet;
use stkutils::chunked_file;
use le_common;
sub read {
	my ($fn) = @_;
	my $cf = stkutils::chunked_file->new($fn, 'r') or die;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7000 && do { le_common::read_guid($cf); last SWITCH; };
			$id == 0x7709 && do { read_7709($cf); last SWITCH; };
			$id == 0x7710 && do { read_7710($cf); last SWITCH; };
			$id == 0x7711 && do { read_7711($cf); last SWITCH; };
			$id == 0x7777 && do { le_common::read_last_modified($cf); last SWITCH; };
			$id == 0x9df3 && do { read_9df3($cf); last SWITCH; };
			printf "unexpected chunk %04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub read_7709 {
	my ($cf) = @_;
	printf "chunk 7709\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('f3f3');
	die unless $packet->length() == 0;
	printf "  camera position: %f, %f, %f\n", @v[0 .. 2];
	printf "  camera orientation: %f, %f, %f\n", @v[3 .. 5];
}
sub read_7710 {
	my ($cf) = @_;
	printf "chunk 7710\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @snap = $packet->unpack('V/(Z*)');
	die unless $packet->length() == 0;
	printf "  snap object count: %d\n", $#snap + 1;
	for (my $i = 0; $i <= $#snap; $i++) {
		printf "  snap%d: '%s'\n", $i, $snap[$i];
	}
}
sub read_7711 {
	my ($cf) = @_;
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
		SWITCH: {
			$id == 0x7801 && do { read_7711_7801($cf); last SWITCH; };
			$id == 0x7802 && do { read_7711_7802($cf); last SWITCH; };
			$id == 0x7803 && do { read_7711_7803($cf); last SWITCH; };
			$id == 0x7804 && do { read_7711_7804($cf); last SWITCH; };
			$id == 0x7849 && do { read_7711_7849($cf); last SWITCH; };
			$id == 0x7850 && do { read_7711_7850($cf); last SWITCH; };
			$id == 0x7851 && do { read_7711_7851($cf); last SWITCH; };
			printf "unexpected chunk 7711,%04x size %d\n", $id, $size;
			die;
		}
		$cf->r_chunk_close();
	}
}
sub read_7711_7801 {
	my ($cf) = @_;
	printf "chunk 7711,7801\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('V');
	die unless $version == 8;
	die unless $packet->length() == 0;
	printf "  level options version: %#x\n", $version;
}
sub read_7711_7802 {
	my ($cf) = @_;
	printf "chunk 7711,7802\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($name) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  Scene::Name: '%s'\n", $name;
}
sub read_7711_7803 {
	my ($cf) = @_;
	printf "chunk 7711,7803\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($custom_data) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  Scene::Custom data: %s\n", ($custom_data eq '') ? "not present" : "present";
#	printf "  Scene::Custom data: '%s'\n", $custom_data;
}
sub read_7711_7804 {
	my ($cf) = @_;
	printf "chunk 7711,7804\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($prefix) = $packet->unpack('Z*');
	die unless $packet->length() == 0;
	printf "  Scene::Name prefix: '%s'\n", $prefix;
}
sub read_7711_7849 {
	my ($cf) = @_;
	printf "chunk 7711,7849\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('V');
	die unless $version == 9;
	die unless $packet->length() == 0;
	printf "  build params version: %#x\n", $version;
}
sub read_7711_7850 {
	use constant quality_names => { 0 => 'draft', 1 => 'high', 2 => 'custom' };
	my ($cf) = @_;
	printf "chunk 7711,7850\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('f3V4f6');
	die unless $packet->length() == 0;
	printf "  Scene::Build options::Optimizing::Normal smooth angle: %f\n", $v[0];
	printf "  Scene::Build options::Optimizing::Weld distance: %f\n", $v[1];
	printf "  Scene::Build options::Lighting::Pixel per meter: %f\n", $v[2];
	printf "  Scene::Build options::Lighting::Jitter samples: %d\n", $v[3];
	printf "  Scene::Build options::Lighting::Error (LM zero): %d\n", $v[4];
	printf "  Scene::Build options::Lighting::Error (LM collapsing): %d\n", $v[5];
	printf "  Scene::Build options::Quality: %s\n", quality_names->{$v[6]};
	printf "  : %f\n", $v[7];
	printf "  : %f\n", $v[8];
	printf "  : %f\n", $v[9];
	printf "  : %f\n", $v[10];
	printf "  : %f\n", $v[11];
	printf "  : %f\n", $v[12];
}
sub read_7711_7851 {
	my ($cf) = @_;
	printf "chunk 7711,7851\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my @v = $packet->unpack('CC');
	die unless $packet->length() == 0;
	printf "  Scene::Build options::Lighting::Hemisphere quality: %d\n", $v[0];
	printf "  Scene::Build options::Lighting::Sun shadows quality: %d\n", $v[1];
}
sub read_9df3 {
	my ($cf) = @_;
	printf "chunk 9df3\n";
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('V');
	die unless $version == 5;
	die unless $packet->length() == 0;
	printf "  scene version: %#x\n", $version;
}

#######################################################################
package main;
use strict;

sub usage {
	return <<END
LevelEditor .level dumper
Usage: le_dump_scene.pl <input_file>
END
}

(my $fn = $ARGV[0]) or die usage;
le_scene::read($fn);
