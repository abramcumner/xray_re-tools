#!perl -w -I \temp\1\bin
# 
# ver.0.1. Last edited: 07 April 2011, 15:01
#######################################################################
package shader_xrlc;
use strict;
use IO::File;
use stkutils::ini_file;
use stkutils::data_packet;
use Cwd;
sub new {
	my $class = shift;
	my $self = {};
	bless($self, $class);
	return $self;
}
sub read {
	my $self = shift;
	my ($fn) = @_;

	my $cf = IO::File->new($fn, 'r') or die;
	my $fSize = -s $fn;
	my $shCount = $fSize/0x90;
	for (my $i = 0; $i < $shCount; $i++) {
		my $shader = shader_xrlc->new();
		my $data = '';
		$cf->read($data, 0x80);
		$shader->{name} = unpack('Z*',$data);
		$data = '';
		$cf->read($data, 0x10);		
		($shader->{dummy},
		$shader->{translucency},
		$shader->{ambient},
		$shader->{lm_density}
		) = unpack('Vfff',$data);
		push @{$self->{shaders}}, $shader;
	}
	$cf->close();
}
sub write {
	my $self = shift;
	my ($fn) = @_;
	my $cf = IO::File->new($fn, 'w') or die;
	binmode $cf;
	foreach my $object (@{$self->{shaders}}) {
		my $packet = stkutils::data_packet->new();
		$packet->pack('Z*', $object->{name});
		my $zero_count = 0x80 - $packet->length();
		for (my $i = 0; $i < $zero_count; $i++) {
			$packet->pack('C',0);
		}
		$cf->write($packet->data(), length($packet->data()));
		$cf->write(pack('Vfff', $object->{dummy}, $object->{translucency}, $object->{ambient}, $object->{lm_density}), 0x10);
	}
	$cf->close();
}
sub import {
	my $self = shift;
	
	my $dir_handle_1;
	my $dir_handle_2;
	my $shader_root;
	my $shader_name;
	my $work_dir = getcwd();
	opendir $dir_handle_1, $work_dir;
	my $i = 0;
	while ($shader_root = readdir $dir_handle_1) {
		if ((-d $shader_root) and ($i > 1)) {
			my $dir = $work_dir.'/'.$shader_root;
			opendir $dir_handle_2, $dir;
			my $k = 0;
			while ($shader_name = readdir $dir_handle_2) {
				if ($k > 1) {
					my $fn = $shader_root.'/'.$shader_name;
					my $shader = stkutils::ini_file->new($fn, 'r') or die "cannot open $fn\n";
					my $object = shader_xrlc->new();
					$object->import_shader($shader);
					$object->{name} = $shader_root.'\\'.substr($shader_name, 0, -4);
					push @{$self->{shaders}}, $object;
					$shader->close();
				} else {
					$k++;
				}				
			}
		} elsif ($i > 1) {
			my $fn = $shader_root;
			my $shader = stkutils::ini_file->new($fn, 'r') or die "cannot open $fn\n";
			my $object = shader_xrlc->new();
			$object->import_shader($shader);
			$object->{name} = substr($fn, 0, -4);
			push @{$self->{shaders}}, $object;
			$shader->close();
		} else {
			$i++;
		}
	}
	closedir $dir_handle_1;
}
sub export {
	my $self = shift;
	my $wDir = getcwd();
	foreach my $shader (@{$self->{shaders}}) {
		my $file;
		if ($shader->{name} =~ /(\w+)\W(\w+)/) {
			$file = $2;
			File::Path::mkpath($1, 0);
			chdir ($1) or die;
		} else {
			$file = $shader->{name};
		}
		my $fh = $file.'.ltx';
		my $ltx_fh = IO::File->new($fh, 'w') or die "cannot open $fh\n";
		print $ltx_fh "[shader]\n";
		print $ltx_fh "flags = ";
		my $bFlags = $shader->{dummy};
		if ($bFlags & 0x01) {
			print $ltx_fh "CSF_COLLISION,";
		}
		if ($bFlags & 0x02) {
			print $ltx_fh "CSF_RENDERING,";
		}
		if ($bFlags & 0x04) {
			print $ltx_fh "CSF_OPTIMIZE_UV,";
		}
		if ($bFlags & 0x08) {
			print $ltx_fh "CSF_VERTEX_LIGHT,";
		}
		if ($bFlags & 0x10) {
			print $ltx_fh "CSF_CAST_SHADOW,";
		}
		if ($bFlags & 0x20) {
			print $ltx_fh "CSF_UNKNOWN_1,";
		}
		if ($bFlags & 0x40) {
			print $ltx_fh "CSF_UNKNOWN_2,";
		}
		if (($bFlags & 0x80) != 0) {
			print "$shader->{name}: SOME ADDITIONAL FLAGS EXISTS!\n";
		}
		printf $ltx_fh "\n";
		printf $ltx_fh "translucency = %.5g\n", $shader->{translucency};
		printf $ltx_fh "ambient = %.5g\n", $shader->{ambient};
		printf $ltx_fh "lm_density = %.5g\n", $shader->{lm_density};
		$ltx_fh->close();
		chdir $wDir or die;
	}
}
sub import_shader {
	my $self = shift;
	my ($cf) = @_;

	my @raw_flags = split(',', $cf->value('shader', 'flags'));
	my $conv_flags = 0;
	foreach my $flag (@raw_flags) {
		if ($flag eq 'CSF_COLLISION') {
			$conv_flags +=0x01;
		}
		if ($flag eq 'CSF_RENDERING') {
			$conv_flags +=0x02;
		}
		if ($flag eq 'CSF_OPTIMIZE_UV') {
			$conv_flags +=0x04;
		}
		if ($flag eq 'CSF_VERTEX_LIGHT') {
			$conv_flags +=0x08;
		}
		if ($flag eq 'CSF_CAST_SHADOW') {
			$conv_flags +=0x10;
		}
		if ($flag eq 'CSF_UNKNOWN_1') {
			$conv_flags +=0x20;
		}
		if ($flag eq 'CSF_UNKNOWN_2') {
			$conv_flags +=0x40;
		}
	}
	$self->{dummy} = $conv_flags;
	$self->{translucency} = $cf->value('shader', 'translucency');
	$self->{ambient} = $cf->value('shader', 'ambient');
	$self->{lm_density} = $cf->value('shader', 'lm_density');
}
#######################################################################
package main;
use strict;
use Getopt::Long;
use File::Path;
use stkutils::data_packet;
use Cwd;

sub usage {
	return <<END
shaders_xrlc.xr compiler/decompiler
Usage: sxcdc.pl [-d input_file] [-o outdir]
       sxcdc.pl [-c input_dir] [-o outfile]
END
}

my $input_file;
my $src_dir;
my $out;

GetOptions(
	'd=s' => \$input_file,
	'c=s' => \$src_dir,
	'o=s' => \$out,
) or die usage();

my $work_dir = getcwd();

if (defined $input_file) {
	die "bad params\n" if (defined $src_dir);
	my $file = shader_xrlc->new();
print "reading $input_file...\n";	
	$file->read($input_file);
	defined $out && do {
		File::Path::mkpath($out, 0);
		chdir $out or die "cannot change path to $out\n";
	};
print "exporting compile shaders...\n";	
	$file->export();	
	print "done!\n";
} else {
	die "bad params\n" if (defined $input_file);
	if (defined $src_dir) {
		chdir $src_dir or die "cannot change dir to $src_dir\n";
	}
	my $file = shader_xrlc->new();
	$out = 'shaders_xrlc.xr.new' unless defined $out;
print "importing compile shaders...\n";
	$file->import();
print "writing $out...\n";
	chdir $work_dir;
	$file->write($out);
print "done!\n";
}
#######################################################################