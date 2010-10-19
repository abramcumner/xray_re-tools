#!/bin/perl -w

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
package ppe;
use strict;
use constant shape_names => {
	0x0		=> 'TCB',
	0x1		=> 'HERM',
	0x2		=> 'BEZI',
	0x3		=> 'LINE',
	0x4		=> 'STEP',
	0x5		=> 'BEZ2',
};

sub convert_q16 {
	my @v = ();
	foreach my $r (@_) {
		push @v, ($r * 64.0 / 65535.0 - 32.0);
	}
	return @v;
}

sub envelope_load {
	my $packet = $_[0];

	my ($e_behavior0, $e_behavior1, $e_keys) = $packet->unpack('CCv');
	printf "envelope:\n";
	printf "  e:behavior = %#2.2x,%#2.2x\n", $e_behavior0, $e_behavior1;
	printf "  e:keys = %d\n", $e_keys;
	for (my $j = 0; $j < $e_keys; $j++) {
		my ($value, $time, $shape) = $packet->unpack('ffC');
		printf "    st_key%d\n", $j;
		printf "      k:value = %g\n", $value;
		printf "      k:time = %g\n", $time;
		printf "      k:shape = %d (%s)\n", $shape, shape_names->{$shape};
		next if ($shape == 4);
		my @raw = $packet->unpack('v7');
		my ($tension, $continuity, $bias, @param) = convert_q16(@raw);
		printf "      k:tension = %g\n", $tension;
		printf "      k:continuity = %g\n", $continuity;
		printf "      k:bias = %g\n", $bias;
		printf "      k:param = %g,%g,%g,%g\n", @param;
	}
}

sub value_load {
	my $packet = $_[0];
	envelope_load($packet);
}

sub color_load {
	my $packet = $_[0];
	my ($base) = $packet->unpack('f');
	printf "  base = %g\n", $base;

	envelope_load($packet);
	envelope_load($packet);
	envelope_load($packet);
}

#######################################################################
package main;
use strict;
use IO::File;
use Carp;

(my $fn = $ARGV[0]) or die;
my $fh = IO::File->new($fn, 'r') or croak "$fn: $!\n!";
binmode $fh;

my $data;
$fh->read($data, 4) or die;
my ($signature) = unpack('V', $data);
die "wrong signature in .ppe" unless $signature == 1;

my @stat_info = $fh->stat();
my $size = $stat_info[7] - 4;
$fh->read($data, $size) or die;
my $packet = data_packet->new($data);

# c0: pp_base_color
# c1: pp_add_color
# c2: pp_gray_color
# v0: pp_gray_value
# v1: pp_blur
# v2: pp_dual_h
# v3: pp_dual_v
# v4: pp_noise_i
# v5: pp_noise_g
# v6: pp_noise_f

for (my $i = 0; $i < 3; $i++) {
	printf "############ color%d:\n", $i;
	ppe::color_load($packet);
}
for (my $i = 0; $i < 7; $i++) {
	printf "############ value%d:\n", $i;
	ppe::value_load($packet);
}

die if $packet->length() != 0;
