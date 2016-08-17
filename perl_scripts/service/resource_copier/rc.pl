#!perl -w -I \temp\1\bin
#
# v.0.4. Last Modified: 25 May 2011
#######################################################################
package ini_file;
use strict;
use IO::File;
sub new {
	my $class = shift;
	my ($fn, $mode) = @_;

	my $fh = IO::File->new($fn, $mode) or die "cannot open $fn\n";

	my $self = {};
	$self->{fh} = $fh;
	$self->{sections_list} = [];
	$self->{sections_hash} = ();
	bless($self, $class);

	$mode eq 'w' and return $self;

	my $section;
	my $skip_section = 0;
	while (<$fh>) {
		$_ =~ qr/^\s*;/ and next;
		if (/^\[(.*)\]\s*:*\s*(\w.*)?/) {
			if (defined $2 && !::is_flag_defined($2)) {
				$skip_section = 1;
				next;
			}
			$section = $1;
			die if defined $self->{sections_hash}->{$section};
			push @{$self->{sections_list}}, $section;
			my %tmp = ();
			$self->{sections_hash}{$section} = \%tmp;
			$skip_section = 0;
			next;
		}
		if (/^([^=]*?)\s*=\s*(.*?)\s*$/) {
			my ($name, $value) = ($1, $2);
			if ($value =~ /^<<(.+)$/) {
				my $terminator = $1;
				$value = '';
				while (<$fh>) {
					chomp;
					last if $_ eq $terminator;
					$value .= "\n".$_;
				}
				die unless defined $_;
				substr ($value, 0, 1) = '';
			}
			$skip_section == 1 and next;
			die unless defined $section;
			if ($name =~ /^\W*(\w+)\W*/) {
				$self->{sections_hash}{$section}{$1} = $value;
			} else {
				$self->{sections_hash}{$section}{$name} = $value;
			}
		} elsif (/^([^=]*?)\s*$/) {
			my $name = $1;
			$skip_section == 1 and next;
			die unless defined $section;
			if ($name =~ /^\s*(.+)\s*/) {
				$self->{sections_hash}{$section}{$1} = 1;
			} else {
				$self->{sections_hash}{$section}{$name} = 1;
			}		
		}
	}
	return $self;
}
sub close {
	my $self = shift;
	$self->{fh}->close();
	$self->{fh} = undef;
}
sub value {
	my $self = shift;
	my ($section, $name) = @_;
	die unless defined $self->{sections_hash}{$section};
	return $self->{sections_hash}{$section}{$name};
}
#######################################################################
package chunked_file;
use strict;
use IO::File;
sub new {
	my $class = shift;
	my ($fn, $mode) = @_;

	my $fh = IO::File->new($fn, $mode) or die "cannot open $fn\n";
	binmode $fh;

	my $self = {};
	$self->{fh} = $fh;
	$self->{offset} = 0;
	$self->{end_offsets} = [];
	$self->{start_offsets} = [];
	bless($self, $class);

	return $self;
}
sub close {
	my $self = shift;
	$self->{fh}->close();
	$self->{fh} = undef;
}
sub r_chunk_open {
	my $self = shift;
	my $offset = $self->{end_offsets}[$#{$self->{end_offsets}}];
	defined($offset) && $self->{offset} >= $offset && return undef;
	my $data;
	$self->{fh}->read($data, 8) or return undef;
	my ($index, $size) = unpack('VV', $data);
	$self->{offset} += 8;
	push @{$self->{end_offsets}}, ($self->{offset} + $size);
	return ($index, $size);
}
sub r_chunk_close {
	my $self = shift;
	my $offset = pop @{$self->{end_offsets}};
	$self->{offset} <= $offset or die;
	if ($self->{offset} < $offset) {
		$self->{fh}->seek($offset, SEEK_SET);
		$self->{offset} = $offset;
	}
}
sub r_chunk_data {
	my $self = shift;
	my ($size) = @_;
	my $offset = $self->{end_offsets}[$#{$self->{end_offsets}}];
	defined($size) or $size = $offset - $self->{offset};
	$self->{offset} + $size <= $offset or die;
	my $data = '';
	$size > 0 && ($self->{fh}->read($data, $size) or die);
	$self->{offset} += $size;
	return $data;
}
###########################################################
package main;
use strict;
use Getopt::Long;
use IO::File;
use File::Path;
use Cwd;
use File::Copy;
#local $SIG{__WARN__} = sub {die};

sub usage {
	return <<END
Resource copier
Usage: rc [-c <level_name>] [-o <out_dir>] [-tga] [-soc]
END
}

my $level_name;
my $out;
my $tga;
my $soc;

GetOptions(
	'c=s' => \$level_name,
	'o=s' => \$out,
	'tga' => \$tga,
	'soc' => \$soc,
) or die usage();
my $ignore_hash = ini_file->new('ignore.ini', 'r');
if (defined ($level_name && $out)) {
	my $log = IO::File->new('X:\logs\copy.log', 'w');
	copy_part();
	if (defined $soc) {
		copy_old($log);
	} else {
		copy_new($log);
	}
	copy_textures($log);
	$log->close();
	print "done!\n";
} else {
	die usage();
}
$ignore_hash->close();
sub is_soc {return defined $soc;}
sub is_tga {return defined $tga;}

sub get_list {
	my ($type, $log) = @_;
	my $scene;
	my @names = ();
	if (::is_soc()) {
		chdir ('X:\maps\\'.$level_name) or die;
		if ($type eq 'objects') {
			$scene = chunked_file->new('scene_object.part','r') or die "cannot open scene_object.part\n";
		} elsif ($type eq 'sounds'){
			$scene = chunked_file->new('sound_src.part','r') or die "cannot open sound_src.part\n";	
		}
		my ($index, $size) = $scene->r_chunk_open();
		$scene->r_chunk_close();
		($index, $size) = $scene->r_chunk_open();
		($index, $size) = $scene->r_chunk_open();
		$scene->r_chunk_close();
		($index, $size) = $scene->r_chunk_open();
		my $count = unpack('V', $scene->r_chunk_data());
		$scene->r_chunk_close();	
		($index, $size) = $scene->r_chunk_open();
		for (my $i = 0; $i < $count; $i++) {
			($index, $size) = $scene->r_chunk_open();
			($index, $size) = $scene->r_chunk_open();
			$scene->r_chunk_close();		
			($index, $size) = $scene->r_chunk_open();
			my ($junk_1, $junk_2, $junk_3, $name, $junk_4);
			if ($type eq 'objects') {	
				($junk_1, $junk_2, $junk_3, $name, $junk_4) = unpack('a[20]Z*a[70]Z*a[12]', $scene->r_chunk_data());
			} elsif ($type eq 'sounds') {
				($junk_1, $junk_2, $junk_3, $name, $junk_4) = unpack('a[20]Z*a[71]Z*a[84]', $scene->r_chunk_data());
			}
			$scene->r_chunk_close();		
			$scene->r_chunk_close();
			if (defined $name and !defined $ignore_hash->value('game_sounds', $name) and !defined $ignore_hash->value('objects', $name)) {
				push @names, $name;
			} elsif (defined $name and defined $ignore_hash->value('game_sounds', $name) or defined $ignore_hash->value('objects', $name)) {
				print $log "Ignored: $name\n"
			}
		}
		$scene->r_chunk_close();	
		$scene->r_chunk_close();	
		$scene->close();
		return @names;
	} else {
		chdir ('X:\rawdata\levels\\'.$level_name) or die;
		if ($type eq 'objects') {
			$scene = ini_file->new('scene_object.part','r') or die "cannot open scene_object.part\n";
			foreach my $section (@{$scene->{sections_list}}) {
				my $name = $scene->value($section, 'reference_name');
				if (defined $name and !defined $ignore_hash->value('objects', $name)) {
					push @names, $name;
				} elsif (defined $name and defined $ignore_hash->value('objects', $name)) {
					print $log "Ignored: $name\n";
				}
			}
		} elsif ($type eq 'sounds'){
			$scene = ini_file->new('sound_src.part','r') or die "cannot open sound_src.part\n";	
			foreach my $section (@{$scene->{sections_list}}) {
				my $name = $scene->value($section, 'snd_name');
				if (defined $name and !defined $ignore_hash->value('game_sounds', $name)) {
					push @names, $name ;
				} elsif (defined $name and defined $ignore_hash->value('game_sounds', $name)) {
					print $log "Ignored: $name\n";
				}
			}
		}
		$scene->close();	
		return @names;
	}
}
sub copy_part {
###let's copy *.part files###
	print "copying level...\n";
	my $level_path;
	if (::is_soc()) {
		$level_path = '\maps\\';
	} else {
		$level_path = '\rawdata\levels\\';
	}
	my $level_dirhandle;
	my $level_file;
	my $level_dir = 'X:'.$level_path.$level_name;
	chdir $level_dir or (File::Path::mkpath($level_dir) and chdir $level_dir);
	opendir $level_dirhandle, $level_dir or die "cannot open $level_dir\n";
	my $i = 0;
	my $target_part = $out.$level_path.$level_name;
	File::Path::mkpath($target_part);
	while ($level_file = readdir $level_dirhandle) {
		if ($i > 1) {
			my $target_file = $target_part.'\\'.$level_file;
			copy ($level_file, $target_file);
		} else {
			$i++;
		}
	}
}
sub copy_old {
	my $log = shift;
###let's copy *.object files for level###
	print "copying level objects...\n";
	print $log "Ignored OBJECTS\n";
	my @names = get_list('objects', $log);
	my %copied_objects;
	my %uncopied_objects;
	foreach my $name (sort {$a cmp $b} @names) {
		my @folds = split(/\\/, $name);
		splice @folds, $#folds;
		File::Path::mkpath($out.'\rawdata\objects\\'.join('\\', @folds));
		(copy('X:\rawdata\objects\\'.$name.'.object', $out.'\rawdata\objects\\'.$name.'.object') and ($copied_objects{$name} = 'Copied')) or ($uncopied_objects{$name} = $!);
		copy('X:\rawdata\objects\\'.$name.'.thm', $out.'\rawdata\objects\\'.$name.'.thm');
	}
	print $log "\nCopied OBJECTS\n";
	foreach my $log_record (sort {$a cmp $b} keys %copied_objects) {
		print $log "$copied_objects{$log_record}: $log_record\n";
	}	
	print $log "\nUncopied OBJECTS\n";
	foreach my $log_record (sort {$a cmp $b} keys %uncopied_objects) {
		print $log "$uncopied_objects{$log_record}: $log_record\n";
	}	
	
###let's copy sounds for level###
	print "copying sounds...\n";
	print $log "\nIgnored SOUNDS\n";
	my @snd_names = get_list('sounds', $log);
	my %copied_sounds;
	my %uncopied_sounds;
	foreach my $name (sort {$a cmp $b} @snd_names) {
		my @folds = split(/\\/, $name);
		splice @folds, $#folds;
		File::Path::mkpath($out.'\gamedata\sounds\\'.join('\\', @folds));
		(copy('X:\gamedata\sounds\\'.$name.'.ogg', $out.'\gamedata\sounds\\'.$name.'.ogg') and ($copied_sounds{$name} = 'Copied')) or ($uncopied_sounds{$name} = $!);
	}
	print $log "\nCopied SOUNDS\n";
	foreach my $log_record (sort {$a cmp $b} keys %copied_sounds) {
		print $log "$copied_sounds{$log_record}: $log_record\n";
	}	
	print $log "\nUncopied SOUNDS\n";
	foreach my $log_record (sort {$a cmp $b} keys %uncopied_sounds) {
		print $log "$uncopied_sounds{$log_record}: $log_record\n";
	}	
}
sub copy_new {
	my $log = shift;
###let's copy *.object files for level###
	print "copying level objects...\n";
	print $log "Ignored OBJECTS\n";
	my @names = get_list('objects', $log);
	my %copied_objects;
	my %uncopied_objects;
	foreach my $name (sort {$a cmp $b} @names) {
		my @folds = split(/\\/, $name);
		splice @folds, $#folds;
		File::Path::mkpath($out.'\rawdata\objects\\'.join('\\', @folds));
		(copy('X:\rawdata\objects\\'.$name.'.object', $out.'\rawdata\objects\\'.$name.'.object') and ($copied_objects{$name} = 'Copied')) or ($uncopied_objects{$name} = $!);
		copy('X:\rawdata\objects\\'.$name.'.thm', $out.'\rawdata\objects\\'.$name.'.thm');
	}
	print $log "\nCopied OBJECTS\n";
	foreach my $log_record (sort {$a cmp $b} keys %copied_objects) {
		print $log "$copied_objects{$log_record}: $log_record\n";
	}	
	print $log "\nUncopied OBJECTS\n";
	foreach my $log_record (sort {$a cmp $b} keys %uncopied_objects) {
		print $log "$uncopied_objects{$log_record}: $log_record\n";
	}	
	
###let's copy sounds for level###
	print "copying sounds...\n";
	print $log "\nIgnored SOUNDS\n";
	my @snd_names = get_list('sounds', $log);
	my %copied_sounds;
	my %uncopied_sounds;
	foreach my $name (sort {$a cmp $b} @snd_names) {
		my @folds = split(/\\/, $name);
		splice @folds, $#folds;
		File::Path::mkpath($out.'\gamedata\sounds\\'.join('\\', @folds));
		(copy('X:\gamedata\sounds\\'.$name.'.ogg', $out.'\gamedata\sounds\\'.$name.'.ogg') and ($copied_sounds{$name} = 'Copied')) or ($uncopied_sounds{$name} = $!);
	}
	print $log "\nCopied SOUNDS\n";
	foreach my $log_record (sort {$a cmp $b} keys %copied_sounds) {
		print $log "$copied_sounds{$log_record}: $log_record\n";
	}	
	print $log "\nUncopied SOUNDS\n";
	foreach my $log_record (sort {$a cmp $b} keys %uncopied_sounds) {
		print $log "$uncopied_sounds{$log_record}: $log_record\n";
	}	
}
sub copy_textures {
	my $log = shift;
###let's form texture list###
	print "forming texture list...\n";
	my $log_dir = 'X:\logs\\';
	my $log_handle;
	my $log_file;
	opendir $log_handle, $log_dir or die;
	chdir $log_dir;
	my $j = 0;
	my @textures;
	print $log "\nIgnored DDS/THM\n";
	while ($log_file = readdir $log_handle) {
		if (($j > 1) and ($log_file =~ /level/i)) {
			my $level_log = IO::File->new($log_file, 'r');
			while (<$level_log>){
				if (/^\W*(\w+)\W*(.*)\[.*\]./) {
					if ($1 eq 'Loaded') {
						if (!defined $ignore_hash->value('game_textures', substr($2, 18, 4))) {
							push @textures, $2;
						} else {
							print $log "Ignored: $2\n";
						}
					}
				}
			}
			$level_log->close();
		} else {
			$j++;
		}
	}
###let's copy dds textures###
	print "copying dds/thm textures...\n";
	my %copied;
	my %uncopied;
	foreach my $texture (@textures) {
		my @folders = split (/\\/, $texture);
		if (not substr($folders[2], 0, 3) eq 'dds') {
			File::Path::mkpath($out.'\gamedata\textures\\'.$folders[2]);
		}
		my $thm = substr($texture, 0, -4).'.thm';
		my $bump = substr($texture, 0, -4).'_bump.dds';
		my $bump_thm = substr($texture, 0, -4).'_bump.thm';
		my $bump_alt = substr($texture, 0, -4).'_bump#.dds';
		(copy ('X:\\'.$texture, $out.$texture) and ($copied{substr($texture, 18)} = 'Copied')) or ($uncopied{substr($texture, 18)} = $!);
		(copy ('X:\\'.$thm, $out.$thm) and ($copied{substr($thm, 18)} = 'Copied')) or ($uncopied{substr($thm, 18)} = $!);
		(copy ('X:\\'.$bump, $out.$bump) and ($copied{substr($bump, 18)} = 'Copied')) or ($uncopied{substr($bump, 18)} = $!);
		(copy ('X:\\'.$bump_thm, $out.$bump_thm) and ($copied{substr($bump_thm, 18)} = 'Copied')) or ($uncopied{substr($bump_thm, 18)} = $!);
		(copy ('X:\\'.$bump_alt, $out.$bump_alt) and ($copied{substr($bump_alt, 18)} = 'Copied')) or ($uncopied{substr($bump_alt, 18)} = $!);
	}
	print $log "\nCopied DDS/THM\n";
	foreach my $log_record (sort {$a cmp $b} keys %copied) {
		print $log "$copied{$log_record}: $log_record\n";
	}
	print $log "\nUncopied DDS/THM\n";
	foreach my $log_record (sort {$a cmp $b} keys %uncopied) {
		print $log "$uncopied{$log_record}: $log_record\n";
	}
###let's copy tga textures###	
	if (::is_tga()) {
		print "copying tga textures...\n";
		my %copied_tga;
		my %uncopied_tga;
		foreach my $texture (@textures) {
			my @folders = split (/\\/, $texture);
			if (not substr($folders[2], 0, 3) eq 'dds') {
				File::Path::mkpath($out.'\rawdata\textures\\'.$folders[2]);
			}
			$texture = 'rawdata'.substr($texture, 8);
			my $text_tga = substr($texture, 0, -3).'tga';
			my $bump = substr($texture, 0, -4).'_bump.tga';
			my $bump_alt = substr($texture, 0, -4).'_bump#.tga';
			(copy ('X:\\'.$text_tga, $out.$text_tga) and ($copied_tga{substr($text_tga, 17)} = 'Copied')) or ($uncopied_tga{substr($text_tga, 17)} = $!);
			(copy ('X:\\'.$bump, $out.$bump) and ($copied_tga{substr($bump, 17)} = 'Copied')) or ($uncopied_tga{substr($bump, 17)} = $!);
			(copy ('X:\\'.$bump_alt, $out.$bump_alt) and ($copied_tga{substr($bump_alt, 17)} = 'Copied')) or ($uncopied_tga{substr($bump_alt, 17)} = $!);
		}
		print $log "\nCopied TGA\n";
		foreach my $log_record (sort {$a cmp $b} keys %copied_tga) {
			print $log "$copied_tga{$log_record}: $log_record\n";
		}
		print $log "\nUncopied TGA\n";
		foreach my $log_record (sort {$a cmp $b} keys %uncopied_tga) {
			print $log "$uncopied_tga{$log_record}: $log_record\n";
		}
	}
	$log->close();
}
#######################################################################