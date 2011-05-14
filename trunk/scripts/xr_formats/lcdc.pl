#!perl -w -I \temp\1\bin
# 
# ver.0.4. Last edited: 23 March 2011, 23:38
#######################################################################
package lanim;
use strict;
use IO::File;
use stkutils::ini_file;
use stkutils::data_packet;
use stkutils::chunked_file;
use Cwd;
sub new {
	my $class = shift;
	my $self = {};
	$self->{lanim_object} = {};
	bless($self, $class);
	return $self;
}
sub read {
	my $self = shift;
	my ($fn) = @_;

	my $i = 0;
	my $cf = stkutils::chunked_file->new($fn, 'r') or die;
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		if ($index == 0) {
			$self->{lanims_version} = unpack ('v', $cf->r_chunk_data());
		} elsif ($index == 1) {
			while (1) {
				my ($index, $size) = $cf->r_chunk_open();
				defined $index or last;
				my $object = lanim->new();
				$object->read_lanims($cf);
				$cf->r_chunk_close();
				push @{$self->{lanims}}, $object;
			}
		} else {
			die;
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub write {
	my $self = shift;
	my ($fn) = @_;
	my $cf = stkutils::chunked_file->new($fn, 'w') or die;
	my $i = 0;
	$cf->w_chunk(0, pack('v', $self->{lanims_version})) if defined $self->{lanims_version};

	$cf->w_chunk_open(1);
	foreach my $object (@{$self->{lanims}}) {
		$object->write_lanims($cf, $i++);
	}
	$cf->w_chunk_close();
	$cf->close();
}
sub import {
	my $self = shift;

	my $all_file = stkutils::ini_file->new('lanims.ltx', 'r');
	$self->{lanims_version} = $all_file->value('header', 'lanims_version');
	$all_file->close();
	
	my $dir_handle;
	my $dir_handle_1;
	my $dir_handle_2;
	my $work_dir = getcwd();
	my $lanim_root;
	my $lanim_name;
	my $in_lanim_name;
	opendir $dir_handle, $work_dir;
	my $i = 0;
	my $lanims_count = 0;
	while ($lanim_root = readdir $dir_handle) {
		if ((-d $lanim_root) and ($i > 1)) {
			my $new_dir = ''.$work_dir.'/'.$lanim_root.'';
			opendir $dir_handle_1, $new_dir ;
			my $j = 0;
			while ($lanim_name = readdir $dir_handle_1) {
				if ((-d $lanim_root.'/'.$lanim_name) and ($j > 1)) {
					my $new_dir = ''.$work_dir.'/'.$lanim_root.'/'.$lanim_name.'';
					opendir $dir_handle_2, $new_dir ;
					my $k = 0;
					while ($in_lanim_name = readdir $dir_handle_2) {
						if ((-d $in_lanim_name) and ($k > 1)) {
							print "there is some catalogs left o_O";
						} elsif (($k > 1) and (not ($in_lanim_name eq 'lanims.ltx'))) {
							my $fn = $lanim_root.'/'.$lanim_name.'/'.$in_lanim_name;
							my $lanim = stkutils::ini_file->new($fn, 'r') or die "cannot open $fn\n";
							my $object = lanim->new();
							$object->import_lanims($lanim);
							$object->{lanim_object}->{name} = substr($lanim_root.'\\'.$lanim_name.'\\'.$in_lanim_name, 0, -4);
							push @{$self->{lanims}}, $object;
							$lanim->close();
						} else {
							$k++;
						}
					}
					closedir $dir_handle_2;
				} elsif (($j > 1) and (not ($lanim_name eq 'lanims.ltx'))) {
					my $fn = $lanim_root.'/'.$lanim_name;
					my $lanim = stkutils::ini_file->new($fn, 'r') or die "cannot open $fn\n";
					my $object = lanim->new();
					$object->import_lanims($lanim);
					$object->{lanim_object}->{name} = substr($lanim_root.'\\'.$lanim_name, 0, -4);
					push @{$self->{lanims}}, $object;
					$lanim->close();
				} else {
					$j++;
				}
			}
			closedir $dir_handle_1;
		} elsif (($i > 1) and (not ($lanim_root eq 'lanims.ltx'))) {
			my $fn = $lanim_root;
			my $lanim = stkutils::ini_file->new($fn, 'r') or die "cannot open $fn\n";
			my $object = lanim->new();
			$object->import_lanims($lanim);
			$object->{lanim_object}->{name} = substr($fn, 0, -4);
			push @{$self->{lanims}}, $object;
			$lanim->close();
		} else {
			$i++;
		}
	}
	closedir $dir_handle;
	$self->{lanims_count} = $lanims_count;
}
sub export {
	my $self = shift;
	
	my $all_file = IO::File->new('lanims.ltx', 'w');
	print $all_file "[header]\n";
	print $all_file "lanims_version = $self->{lanims_version}\n" if defined $self->{lanims_version};
	$all_file->close();
	
	my $work_dir = getcwd();
	my $i = 0;
	foreach my $object (@{$self->{lanims}}) {
		my $string = $object->{lanim_object}->{name};
		my $out;
		my $lanim_name;
		my $inc_folder;
		if ($string =~ /^(\w+)\\(\w.*)/) {
			$out = $1;
			$lanim_name = $2;
			if ($lanim_name =~ /^(\w+)\\(\w.*)/) {
				$inc_folder = $1;
				$lanim_name = $2;
			}
		} else  {
			$lanim_name = $string;
		}
		if (defined $out) {
			chdir $out or File::Path::mkpath($out, 0) or die "cannot make path $out\n";
			chdir ($out);
		}
		my $fh = ''.$lanim_name.'.ltx';
		if (defined $inc_folder) {
			File::Path::mkpath($inc_folder, 0);
			chdir $inc_folder;
		}

		my $ltx_fh = IO::File->new($fh, 'w') or die "cannot open $fh\n";
		print $ltx_fh "[main]\n";
		print $ltx_fh "FPS = $object->{lanim_object}->{fps}\n";
		print $ltx_fh "frame_count = $object->{lanim_object}->{frame_count}\n";
		print $ltx_fh "colors_count = $object->{lanim_object}->{colors_count}\n\n";
		print $ltx_fh "[colors]\n";
		for (my $i = 0; $i < $object->{lanim_object}->{colors_count}; $i++) {
			print $ltx_fh "$i:frame = $object->{lanim_object}->{frame}[$i]\n";
			if (::rgb()) {
				my $RGB = $object->{lanim_object}->{color}[$i];
				my $RG = $RGB >> 8;
				my $R = $RG >> 8;
				my $G = $RG - ($R << 8);
				my $B = $RGB - ($RG << 8);
				printf $ltx_fh "$i:R:G:B = %u:%u:%u\n", $R, $G, $B;
			} else {
				printf $ltx_fh "$i:color = %#x\n", $object->{lanim_object}->{color}[$i];
			}
		}
		$ltx_fh->close();
		chdir $work_dir or die;
	}
}
sub read_lanims {
	my $self = shift;
	my ($cf) = @_;
	
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined $index or last;
		my $data = $cf->r_chunk_data();
		if ($index == 1) {
			($self->{lanim_object}->{name},
			$self->{lanim_object}->{fps},
			$self->{lanim_object}->{frame_count}) = unpack ('Z*fV', $data);
		} elsif ($index == 2) {
			my $packet = stkutils::data_packet->new($data);
			my @values = $packet->unpack('V');
			$self->{lanim_object}->{colors_count} = $values[0];
			for (my $i = 0; $i < $self->{lanim_object}->{colors_count}; $i++) {
				($self->{lanim_object}->{frame}[$i],
				$self->{lanim_object}->{color}[$i]) = $packet->unpack('VV');
			}
			die unless $packet->length() == 0;
		} else {
			die;
		}
		$cf->r_chunk_close();
	}
}
sub write_lanims {
	my $self = shift;
	my ($cf, $id) = @_;
	
	$cf->w_chunk_open($id);	
	my $packet1 = stkutils::data_packet->new();
	$packet1->pack('Z*fV', $self->{lanim_object}->{name}, $self->{lanim_object}->{fps}, $self->{lanim_object}->{frame_count});
	$cf->w_chunk(1, $packet1->data());
	$cf->w_chunk_open(2);	

	$cf->w_chunk_data(pack('V', $self->{lanim_object}->{colors_count}));
	for (my $i = 0; $i < $self->{lanim_object}->{colors_count}; $i++) {
		my $packet2 = stkutils::data_packet->new();
		$packet2->pack('VV', $self->{lanim_object}->{frame}[$i],$self->{lanim_object}->{color}[$i]);
		$cf->w_chunk_data($packet2->data());
	}
	$cf->w_chunk_close();
	$cf->w_chunk_close();
}
sub import_lanims {
	my $self = shift;
	my ($cf) = @_;
	
	$self->{lanim_object}->{fps} = $cf->value('main', 'FPS');
	$self->{lanim_object}->{frame_count} = $cf->value('main', 'frame_count');
	$self->{lanim_object}->{colors_count} = $cf->value('main', 'colors_count');

	for (my $i = 0; $i < $self->{lanim_object}->{colors_count}; $i++) {
		my $frame = ''.$i.':frame';
		$self->{lanim_object}->{frame}[$i] = $cf->value('colors', $frame);
		if (::rgb()) {
			my $color = ''.$i.':R:G:B';
			my ($R, $G, $B) = split /:/, $cf->value('colors', $color);
			$self->{lanim_object}->{color}[$i] = $B + ($G << 8) + ($R << 16);
		} else {
			my $color = ''.$i.':color';
			$self->{lanim_object}->{color}[$i] = hex($cf->value('colors', $color));
		}
	}
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
lanims.xr compiler/decompiler
Usage: lcdc.pl [-d input_file] [-o outdir] [-rgb]
       lcdc.pl [-c input_dir] [-o outfile] [-rgb]
END
}

my $input_file;
my $src_dir;
my $out;
my $rgb;

GetOptions(
	'd=s' => \$input_file,
	'c=s' => \$src_dir,
	'o=s' => \$out,
	'rgb' => \$rgb,
) or die usage();

my $work_dir = getcwd();

if (defined $input_file) {
	die "bad params\n" if (defined $src_dir);
	my $file = lanim->new();
print "reading $input_file...\n";	
	$file->read($input_file);
	defined $out && do {
		File::Path::mkpath($out, 0);
		chdir $out or die "cannot change path to $out\n";
	};
print "exporting light animations...\n";	
	$file->export();	
	print "done!\n";
} else {
	die "bad params\n" if (defined $input_file);
	if (defined $src_dir) {
		chdir $src_dir or die "cannot change dir to $src_dir\n";
	}
	my $file = lanim->new();
	$out = 'lanim_file.new' unless defined $out;
print "importing light animations...\n";
	$file->import();
print "writing $out...\n";
	chdir $work_dir;
	$file->write($out);
print "done!\n";
sub rgb {return defined $rgb;}
}
#######################################################################