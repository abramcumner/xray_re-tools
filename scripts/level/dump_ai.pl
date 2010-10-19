#!perl -w
#
#NodeCompressed
#---------+++++++++++-----------
#8 8 7|1 8 8 6|2 8 8 5|3 8 8 4|4
#0 1 2   3 4 5   6 7 8   9 a b
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
$fh->read($data, 0x38) or die;
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
printf "diff_y=%g\n", $box[4] - $box[1];
printf "box=(%g,%g,%g) - (%g,%g,%g)\n", @box;

my $row_length = floor(($box[5] - $box[2])/$cell_size + 1.501);
my $column_length = floor(($box[3] - $box[0])/$cell_size + 1.501);

printf "row x column = %d x %d\n", $column_length, $row_length;

die unless ($version == 8 || $version == 9 || $version == 10);

sub get_neighbours {
	my $data = shift;

	my ($r0) = unpack('V', $data);
	my ($r1) = unpack('V', substr($data, 2));
	my ($r2) = unpack('V', substr($data, 5));
	my ($r3) = unpack('V', substr($data, 8));

	my $n0 = $r0 & 0x7fffff;
	my $n1 = ($r1 >> 7) & 0x7fffff;
	my $n2 = ($r2 >> 6) & 0x7fffff;
	my $n3 = ($r3 >> 5) & 0x7fffff;

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
		$u = 0x7f - $u;
		$v = 0x7f - $v;
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
			$u = 0x7f - $u;
			$v = 0x7f - $v;
		}
		$uv_adjustment[$i] = 1.0/sqrt($u*$u + $v*$v + (126-$u-$v)*(126-$u-$v));
	}
}

prepare_uv_adjustment();

my @vertices = {};
my @matrix;
for (my $i = 0; $i < $vertex_count; $i++) {
	my ($data, $cover, $low_cover, $plane, $packed_xz_lo, $packed_xz_hi, $packed_y);
	if ($version == 10) {
		$fh->read($data, 0x17) or die;
		(
			$data,
			$cover,
			$low_cover,
			$plane,
			$packed_xz_lo, $packed_xz_hi,
			$packed_y,
		) = unpack("a12vvvvCv", $data);
	} else {
		$fh->read($data, 0x15) or die;
		(
			$data,
			$cover,
			$plane,
			$packed_xz_lo, $packed_xz_hi,
			$packed_y,
		) = unpack("a12vvvCv", $data);
		$low_cover = $cover;
	}
	my $packed_xz = unpack('V', pack('vCC', $packed_xz_lo, $packed_xz_hi, 0));

# game X -> picture Y
# game Z -> picture X
my $gx = floor($packed_xz / $row_length);
my $gy = floor($packed_xz % $row_length);
if (defined $matrix[$gy][$gx]) {
	$matrix[$gy][$gx]++;
} else {
	$matrix[$gy][$gx] = 1;
}

	my $x = $box[0] + $cell_size * floor($packed_xz / $row_length);
	my $y = $box[1] + $factor_y * $packed_y / 65535.0;
	my $z = $box[2] + $cell_size * floor($packed_xz % $row_length);
	my @c = ($cover & 0xf, ($cover >> 4) & 0xf, ($cover >> 8) & 0xf, ($cover >> 12) & 0xf);
	my @lc = ($low_cover & 0xf, ($low_cover >> 4) & 0xf, ($low_cover >> 8) & 0xf, ($low_cover >> 12) & 0xf);
	my @n = get_neighbours($data);
	my @d = get_normal($plane);
	if (0) {
		my $qx = $x/0.7 + 0.5;
		my $qy = ($y - $box[1])/($box[4] - $box[1])*65535.0;
		my $qz = $z/0.7 + 0.5;
		my $x2 = floor($qx)*0.7;
		my $y2 = floor($qy)*($box[4] - $box[1])/65535.0 + $box[1];
		my $z2 = floor($qz)*0.7;
		printf "%f %f %f	-> %d %u %d	-> %f %f %f\n", $x, $y, $z, $qx, $qy, $qz, $x2, $y2, $z2;
		my $dx = abs($x2 - $x);
		my $dy = abs($y2 - $y);
		my $dz = abs($z2 - $z);
		die unless ($dx < 0.0001 && $dy < 0.001 && $dz < 0.0001);
	}
#printf "%6.6x %4.4x\n", $packed_xz, $packed_y;
	printf "%10d (%3d x %3d x %f): %6.6x (%x/%x), %6.6x (%x/%x), %6.6x (%x/%x), %6.6x (%x/%x); light: %d, normal: (%.3f, %.3f, %.3f)\n",
			$i, $gx, $gy, $y,
			$n[0], $c[0], $lc[0],
			$n[1], $c[1], $lc[1],
			$n[2], $c[2], $lc[2],
			$n[3], $c[3], $lc[3],
			unpack('C', substr($data, 11)) >> 4, get_normal($plane);
#	printf "cover=%4.4x x=%g y=%g z=%g\n", $cover, $x, $y, $z;
#	printf "light=%2.2x x=%g y=%g z=%g\n", $light, $x, $y, $z;
}
exit;

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

	7 => [0,0,128],
	8 => [0,0,128],
	9 => [0,0,128],
};

#for (my $i = $h - 1; $i >= 0; $i--) {
for (my $i = 0; $i < $h; $i++) {
	for (my $j = 0; $j < $w; $j++) {
		my $floors = 0;
		if (defined $matrix[$i][$j]) {
			$floors = $matrix[$i][$j];
		}
		$data = pack("CCC", @{colors->{$floors}});
		$bmp_fh->write($data, 3);
	}
	$pad != 0 and $bmp_fh->write(pack("C$pad", 0), $pad);
}
