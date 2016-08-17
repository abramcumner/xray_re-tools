#!perl -w

###################################
# version 2 (builds 1xxx)
#
# NodeCompressed
#
#
# 00:
# 02:u16	x_min
# 04:u16	y_min?
# 06:u16	z_min
# 08:u16	x_max
# 0a:u16	y_max?
# 0c:u16	z_max
# 0e:
#
# 24
#
#
# 8 8 8|8  8 8|8 8  8|8 8 8
# 0 1 2 3  4 5 6 7  8 9 a b
#
#######################################################################
package main;
use strict;
use IO::File;
use Carp;
use POSIX;

my $fn = "level.ai";
my $fh = IO::File->new($fn, 'r') or croak "$fn: $!\n!";
binmode $fh;

my $data;
$fh->read($data, 0x28) or die;
my (
	$version,
	$vertex_count,
	$cell_size,
	$factor_y,
	@box,
) = unpack("VVfff6", $data);

printf "version=%d\n", $version;
printf "vertex_count=%d\n", $vertex_count;
printf "cell_size=%g\n", $cell_size;
printf "factor_y=%g\n", $factor_y;
printf "box=(%g,%g,%g) - (%g,%g,%g)\n", @box;

my $row_length = floor(($box[5] - $box[2])/$cell_size + 1.501);
my $column_length = floor(($box[3] - $box[0])/$cell_size + 1.501);

printf "row x column = %d x %d\n", $column_length, $row_length;

die unless $version == 2;

sub get_neighbours {
	my $data = shift;

	my ($r0) = unpack('V', $data);
	my ($r1) = unpack('V', substr($data, 3));
	my ($r2) = unpack('V', substr($data, 6));
	my ($r3) = unpack('V', substr($data, 8));

	my $n0 = $r0 & 0xffffff;
	my $n1 = $r1 & 0xffffff;
	my $n2 = $r2 & 0xffffff;
	my $n3 = ($r3 >> 8) & 0xffffff;

	return ($n0, $n1, $n2, $n3);
}

my @uv_adjustment;
sub get_normal {
	my $plane = shift;
	my ($i, $u, $v);
	my ($x, $y, $z);

	$u = ($plane >> 7) & 0x3f;
	$v = $plane & 0x7f;
	if ($u + $v >= 0x7f) {
		$u -= 0x7f;
		$v -= 0x7f;
	}
	$i = $plane & 0x1fff;
	$x = $uv_adjustment[$i] * $u;
	$y = $uv_adjustment[$i] * $v;
	$z = $uv_adjustment[$i] * (126-$u-$v);
	$x = -$x if $plane & 0x8000;
	$y = -$y if $plane & 0x4000;
	$z = -$z if $plane & 0x2000;
	return ($x, $y, $z);
}

sub prepare_uv_adjustment {
	my ($i, $u, $v);
	for (my $i = 0; $i < 0x2000; $i++) {
		$u = $i >> 7;
		$v = $i & 0x7f;
		if ($u + $v >= 0x7f) {
			$u -= 0x7f;
			$v -= 0x7f;
		}
		$uv_adjustment[$i] = 1.0/sqrt($u*$u + $v*$v + (126-$u-$v)*(126-$u-$v));
	}
}

prepare_uv_adjustment();

my $max_color = 0;
my @vertices = {};
my @matrix;
for (my $i = 0; $i < $vertex_count; $i++) {
	$fh->read($data, 21) or die;
	my (
		$plane,
		$min_qx,
		$min_qy,
		$min_qz,
		$max_qx,
		$max_qy,
		$max_qz,
		$unk,
		$packed_xz_lo, $packed_xz_hi,
		$packed_y,
		$nl,
	) = unpack('vs3s3CvCvC', $data);
	my $packed_xz = unpack('V', pack('vCC', $packed_xz_lo, $packed_xz_hi, 0));
	if ($nl > 0) {
		$fh->read($data, $nl*3) or die;
	}

	my $min_x = $min_qx*$cell_size;
	my $min_y = $min_qy*$factor_y/32767.0;
	my $min_z = $min_qz*$cell_size;
	my $max_x = $max_qx*$cell_size;
	my $max_y = $max_qy*$factor_y/32767.0;
	my $max_z = $max_qz*$cell_size;

# game X -> picture Y
# game Z -> picture X
for (my $gx = floor($min_x - $box[0]); $gx <= floor($max_x - $box[0]); $gx++) {
	for (my $gy = floor($min_z - $box[2]); $gy <= floor($max_z - $box[2]); $gy++) {
		if (defined $matrix[$gy][$gx]) {
			$matrix[$gy][$gx]++;
		} else {
			$matrix[$gy][$gx] = 1;
		}
		if ($matrix[$gy][$gx] > $max_color) {
			$max_color = $matrix[$gy][$gx];
		}
	}
}

#die unless length($link) == 12;

	my $x = $box[0] + $cell_size * floor($packed_xz / $row_length);
	my $y = $box[1] + $factor_y * $packed_y / 65535.0;
	my $z = $box[2] + $cell_size * floor($packed_xz % $row_length);
#printf "   : (%f,%f,%f)\n", $x, $y, $z;

#printf "pos: (%g,%g,%g)-(%g,%g,%g)\n", $min_x, $min_y, $min_z, $max_x, $max_y, $max_z;
#printf "   : (%f,%f,%f)\n", get_normal($plane);
#printf " %d\n", $unk;
#if ($nl) {
#print $nl, "\n";
#	my @v = unpack("(a3)$nl", $data);
#	print @v, "\n";
#}
#	my @n = get_neighbours($link);
#	my @d = get_normal($plane);
#	printf "%6.6x (%3d x %3d): %6.6x (%2d), %6.6x (%2d), %6.6x (%2d), %6.6x (%2d); normal: (%.3f, %.3f, %.3f)\n",
#			$i, 0, 0, $n[0], $c[0], $n[1], $c[1], $n[2], $c[2], $n[3], $c[3], get_normal($plane);
#	printf "%6.6x (%3d x %3d): %6.6x (%2d), %6.6x (%2d), %6.6x (%2d), %6.6x (%2d); light: %d, normal: (%.3f, %.3f, %.3f)\n",
#			$i, $gx, $gy, $n[0], $c[0], $n[1], $c[1], $n[2], $c[2], $n[3], $c[3],
#			unpack('C', substr($data, 11)) >> 4, get_normal($plane);
#exit unless $i != 0;
}
#exit;

$fn = "out.bmp";
my $bmp_fh = IO::File->new($fn, 'w+') or croak "$fn: $!\n!";
binmode $fh;

my $w = $column_length;
my $h = $row_length;
my $rl = ($w*3 + 3) & ~3;
my $pad = $rl - $w*3;
$data = pack("vVvvV VVVvvV6",
	0x4d42,		# bfType
	$h*$rl,		# bfSize
	0,		# bfReserved1
	0,		# bfReserved2
	0x36,		# bfOffBits

	0x28,		# biSize
	$w,		# biWidth
	$h,		# biHeight
	1,		# biPlanes
	24,		# biBitCount
	0,		# biCompression
	0,		# biSizeImage,
	0xb12,		# biXPelsPerMeter,
	0xb12,		# biYPelsPerMeter,
	0,		# biClrUsed,
	0,		# biClrImportant,
);
$bmp_fh->write($data, 0x36);

use constant colors => {
	0 => [255,255,255],
	1 => [255,0,0],
	2 => [128,0,0],
	3 => [0,255,0],
	4 => [0,128,0],
	5 => [0,0,255],
	6 => [0,0,128],
};

#for (my $i = $h - 1; $i >= 0; $i--) {
for (my $i = 0; $i < $h; $i++) {
	for (my $j = 0; $j < $w; $j++) {
		my $floors = 0;
		if (defined $matrix[$i][$j]) {
			$floors = $matrix[$i][$j];
			if ($floors > 255) {
				$floors = floor(255.0 * $floors / $max_color);
			}
		}
#		$data = pack("CCC", @{colors->{$floors}});
		if ($floors == 0) {
			$data = pack("CCC", 255, 255, 255);
		} else {
			$data = pack("CCC", 0, 0, 255 - $floors);
		}
		$bmp_fh->write($data, 3);
	}
	$pad != 0 and $bmp_fh->write(pack("C$pad", 0), $pad);
}
