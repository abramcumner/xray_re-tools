#!perl -w -I \temp\1\bin
# 
# ver.0.1. Last edited: 23 March 2011, 21:46
#######################################################################
package env;
use strict;
use IO::File;
use stkutils::ini_file;
use stkutils::data_packet;
use stkutils::chunked_file;
use Cwd;
sub new {
	my $class = shift;
	my $self = {};
	$self->{env_object} = {};
	bless($self, $class);
	return $self;
}
sub read {
	my $self = shift;
	my ($fn) = @_;

	my $cf = stkutils::chunked_file->new($fn, 'r') or die;
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		my $object = env->new();
		$object->read_env($cf);
		$cf->r_chunk_close();
		push @{$self->{envs}}, $object;
	}
	$cf->close();
}
sub write {
	my $self = shift;
	my ($fn) = @_;
	my $cf = stkutils::chunked_file->new($fn, 'w') or die;
	my $i = 0;
	foreach my $object (@{$self->{envs}}) {
		$object->write_env($cf, $i++);
	}
	$cf->close();
}
sub import {
	my $self = shift;
	
	my $dir_handle;
	my $env_name;
	my $work_dir = getcwd();
	opendir $dir_handle, $work_dir;
	my $i = 0;
	while ($env_name = readdir $dir_handle) {
		if ($i > 1) {
			my $fn = $env_name;
			my $env = stkutils::ini_file->new($fn, 'r') or die "cannot open $fn\n";
			my $object = env->new();
			$object->import_env($env);
			$object->{env_object}->{Name} = substr($fn, 0, -4);
			push @{$self->{envs}}, $object;
			$env->close();
		} else {
			$i++;
		}
	}
	closedir $dir_handle;
}
sub export {
	my $self = shift;
	
	foreach my $object (@{$self->{envs}}) {
		my $fh = ''.$object->{env_object}->{Name}.'.ltx';
		my $ltx_fh = IO::File->new($fh, 'w') or die "cannot open $fh\n";
		print $ltx_fh "[header]\n";
		print $ltx_fh "version = $object->{env_object}->{Version}\n\n";
		print $ltx_fh "[environment]\n";
		printf $ltx_fh "size = %.5g\n", $object->{env_object}->{Size};
		printf $ltx_fh "diffusion = %.5g\n", $object->{env_object}->{Diffusion};
		if ($object->{env_object}->{Version} > 3) {
			print $ltx_fh "preset_id = $object->{env_object}->{PresetID}\n\n";
		} else {
			print $ltx_fh "\n";
		}
		print $ltx_fh "[room]\n";
		printf $ltx_fh "room = %.5g\n", $object->{env_object}->{Room};
		printf $ltx_fh "room_hf = %.5g\n\n",$object->{env_object}->{RoomHF};
		print $ltx_fh "[distance_effects]\n";
		printf $ltx_fh "room_rolloff_factor = %.5g\n", $object->{env_object}->{RoomRolloffFactor};
		printf $ltx_fh "air_absorption_hf = %.5g\n\n", $object->{env_object}->{AirAbsorptionHF};
		print $ltx_fh "[decay]\n";
		printf $ltx_fh "decay_time = %.5g\n", $object->{env_object}->{DecayTime};
		printf $ltx_fh "decay_hf_ratio = %.5g\n\n", $object->{env_object}->{DecayHFRatio};
		print $ltx_fh "[reflections]\n";
		printf $ltx_fh "reflections = %.5g\n", $object->{env_object}->{Reflections};
		printf $ltx_fh "reflections_delay = %.5g\n\n", $object->{env_object}->{ReflectionsDelay};
		print $ltx_fh "[reverb]\n";
		printf $ltx_fh "reverb = %.5g\n", $object->{env_object}->{Reverb};
		printf $ltx_fh "reverb_delay = %.5g\n", $object->{env_object}->{ReverbDelay};
		$ltx_fh->close();
	}
}
sub read_env {
	my $self = shift;
	my ($cf) = @_;
	
	($self->{env_object}->{Version},
	$self->{env_object}->{Name},
	$self->{env_object}->{Room},
	$self->{env_object}->{RoomHF},
	$self->{env_object}->{RoomRolloffFactor},
	$self->{env_object}->{DecayTime},
	$self->{env_object}->{DecayHFRatio},
	$self->{env_object}->{Reflections},
	$self->{env_object}->{ReflectionsDelay},
	$self->{env_object}->{Reverb},
	$self->{env_object}->{ReverbDelay},
	$self->{env_object}->{Size},
	$self->{env_object}->{Diffusion},
	$self->{env_object}->{AirAbsorptionHF},
	$self->{env_object}->{PresetID}) = unpack ('VZ*f12V', $cf->r_chunk_data());
}
sub write_env {
	my $self = shift;
	my ($cf, $id) = @_;
	
	$cf->w_chunk_open($id);	
	my $packet = stkutils::data_packet->new();
	if ($self->{env_object}->{Version} > 3) {
		$packet->pack('VZ*f12V', $self->{env_object}->{Version}, $self->{env_object}->{Name}, $self->{env_object}->{Room},	$self->{env_object}->{RoomHF}, $self->{env_object}->{RoomRolloffFactor}, $self->{env_object}->{DecayTime}, $self->{env_object}->{DecayHFRatio}, $self->{env_object}->{Reflections}, $self->{env_object}->{ReflectionsDelay}, $self->{env_object}->{Reverb}, $self->{env_object}->{ReverbDelay}, $self->{env_object}->{Size}, $self->{env_object}->{Diffusion}, $self->{env_object}->{AirAbsorptionHF}, $self->{env_object}->{PresetID});
	} else {
		$packet->pack('VZ*f12', $self->{env_object}->{Version}, $self->{env_object}->{Name}, $self->{env_object}->{Room},	$self->{env_object}->{RoomHF}, $self->{env_object}->{RoomRolloffFactor}, $self->{env_object}->{DecayTime}, $self->{env_object}->{DecayHFRatio}, $self->{env_object}->{Reflections}, $self->{env_object}->{ReflectionsDelay}, $self->{env_object}->{Reverb}, $self->{env_object}->{ReverbDelay}, $self->{env_object}->{Size}, $self->{env_object}->{Diffusion}, $self->{env_object}->{AirAbsorptionHF});
	}
	$cf->w_chunk_data($packet->data());
	$cf->w_chunk_close();
}
sub import_env {
	my $self = shift;
	my ($cf) = @_;

	$self->{env_object}->{Version} = $cf->value('header', 'version');
	$self->{env_object}->{Size} = $cf->value('environment', 'size');
	$self->{env_object}->{Diffusion} = $cf->value('environment', 'diffusion');
	$self->{env_object}->{AirAbsorptionHF} = $cf->value('distance_effects', 'air_absorption_hf');
	$self->{env_object}->{PresetID} = $cf->value('environment', 'preset_id') if ($self->{env_object}->{Version} > 3);
	$self->{env_object}->{Room} = $cf->value('room', 'room');
	$self->{env_object}->{RoomHF} = $cf->value('room', 'room_hf');
	$self->{env_object}->{RoomRolloffFactor} = $cf->value('distance_effects', 'room_rolloff_factor');
	$self->{env_object}->{DecayTime} = $cf->value('decay', 'decay_time');
	$self->{env_object}->{DecayHFRatio} = $cf->value('decay', 'decay_hf_ratio');
	$self->{env_object}->{Reflections} = $cf->value('reflections', 'reflections');
	$self->{env_object}->{ReflectionsDelay} = $cf->value('reflections', 'reflections_delay');
	$self->{env_object}->{Reverb} = $cf->value('reverb', 'reverb');
	$self->{env_object}->{ReverbDelay} = $cf->value('reverb', 'reverb_delay');
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
senvironment.xr compiler/decompiler
Usage: secdc.pl [-d input_file] [-o outdir]
       secdc.pl [-c input_dir] [-o outfile]
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
	my $file = env->new();
print "reading $input_file...\n";	
	$file->read($input_file);
	defined $out && do {
		File::Path::mkpath($out, 0);
		chdir $out or die "cannot change path to $out\n";
	};
print "exporting sound environments...\n";	
	$file->export();	
	print "done!\n";
} else {
	die "bad params\n" if (defined $input_file);
	if (defined $src_dir) {
		chdir $src_dir or die "cannot change dir to $src_dir\n";
	}
	my $file = env->new();
	$out = 'senvironment.xr.new' unless defined $out;
print "importing sound environments...\n";
	$file->import();
print "writing $out...\n";
	chdir $work_dir;
	$file->write($out);
print "done!\n";
}
#######################################################################