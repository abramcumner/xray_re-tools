#!/bin/perl -w
# obsolete and incomplete. see C++ sources.

#######################################################################
package data_packet;
use strict;
sub new {
	my $class = shift;
	my $self = {};
	($self->{data}) = @_;
	bless($self, $class);
	return $self;
}
sub unpack {
	my $self = shift;
	my $template = shift;
	my @values = unpack($template.'a*', $self->{data});
	$self->{data} = splice(@values, -1);
	return @values;
}
sub length {
	my $self = shift;
	return length($self->{data});
}

#######################################################################
package main;
use strict;
use IO::File;
use Carp;
use constant shape_names => {
	0x0		=> 'TCB',
	0x1		=> 'HERM',
	0x2		=> 'BEZI',
	0x3		=> 'LINE',
	0x4		=> 'STEP',
	0x5		=> 'BEZ2',
};

(my $fn = $ARGV[0]) or die;
my $fh = IO::File->new($fn, 'r') or croak "$fn: $!\n!";
binmode $fh;

my $data;
$fh->read($data, 8) or die;
my ($signature, $size) = unpack('VV', $data);

$fh->read($data, $size) or die;
my $packet = data_packet->new($data);

my (	$ccm_name,
	$ccm_frame_start,
	$ccm_frame_end,
	$ccm_fps,
) = $packet->unpack('Z*VVf');

my ($om_version) = $packet->unpack('v');
die "unsupported version=$om_version" unless $om_version == 5 or $om_version == 4;

printf "ccm:name = %s\n", $ccm_name;
printf "ccm:frame_start = %d\n", $ccm_frame_start;
printf "ccm:frame_end = %d\n", $ccm_frame_end;
printf "ccm:fps = %g\n", $ccm_fps;

printf "om:mtype = %#2.2x\n", $om_version;

sub convert_q16 {
	my @v = ();
	foreach my $r (@_) {
		push @v, ($r * 64.0 / 65535.0 - 32.0);
	}
	return @v;
}

for (my $i = 0; $i < 6; $i++) {
	my ($e_behavior0, $e_behavior1, $e_keys) = $packet->unpack('CCv');
	printf "envelope%d:\n", $i;
	printf "  e:behavior = %#2.2x,%#2.2x\n", $e_behavior0, $e_behavior1;
	printf "  e:keys = %d\n", $e_keys;
	for (my $j = 0; $j < $e_keys; $j++) {
		my ($value, $time, $shape) = $packet->unpack('ffC');
		printf "    st_key%d\n", $j;
		printf "      k:value = %g\n", $value;
		printf "      k:time = %g\n", $time;
		printf "      k:shape = %d (%s)\n", $shape, shape_names->{$shape};
		next if ($shape == 4);
		my @raw = $packet->unpack("v7");
		my ($tension, $continuity, $bias, @param) = convert_q16(@raw);
		printf "      k:tension = %g\n", $tension;
		printf "      k:continuity = %g\n", $continuity;
		printf "      k:bias = %g\n", $bias;
		printf "      k:param = %g,%g,%g,%g\n", @param;
	}
}

die if $packet->length() != 0;
