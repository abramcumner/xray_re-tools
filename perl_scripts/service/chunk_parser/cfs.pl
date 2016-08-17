#!perl -w -I \temp\1\bin
#
#######################################################################
package chunked;
use strict;
use IO::File;
use stkutils::ini_file;
use stkutils::chunked_file;
sub new {
	my $class = shift;
	my $self = {};
	$self->{counter} = {};
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
		$self->{$i} = $cf->r_chunk_data();
		$self->{counter}->{$i} = $index;
		$cf->r_chunk_close();
		$i++;
	}
	if ($i == 0) {
		die "there is no chunks at all!\n";
	}
	$cf->close();
	$self->{chunk_count} = $i;
	print "number of chunks is $self->{chunk_count}\n";
#	die;
}
sub write {
	my $self = shift;
	my ($fn) = @_;
	my $cf = stkutils::chunked_file->new($fn, 'w') or die;
	my $i = 0;
	while ($i < $self->{chunk_count}) {
		$cf->w_chunk($self->{counter}->{$i}, $self->{$i});
		$i++;
	}
	$cf->close();
}
sub import {
	my $self = shift;
	my ($fn) = @_;
	
	my $i = 0;
	my $chltx = stkutils::ini_file->new('chunks.ltx', 'r') or die "cannot open chunks.ltx\n";
	while (1) {
		my $fh = $chltx->value('chunks', 'file_'.$i.'');
		defined $fh or last;
		print "	importing chunk $i\n";
		my $bin_fh = IO::File->new($fh, 'r') or last;
		if ($fh =~ /(\d+)\w+(\d+)/) {
			$self->{counter}->{$i} = $2;
		} else {
			die "unexpected format of chunks.ltx";
		}
		binmode $bin_fh;
		$bin_fh->read($self->{$i}, ($bin_fh->stat())[7]);
		$bin_fh->close();
		$i++;
	}
	$self->{chunk_count} = $i;
	print "number of chunks is $self->{chunk_count}\n";
	$chltx->close();
}
sub export {
	my $self = shift;
	my ($fn) = @_;

	my $i = 0;
	my $chltx = IO::File->new('chunks.ltx', 'w') or die "chunks.ltx\n";
	print $chltx "[chunks]\n";
	while ($i < $self->{chunk_count}) {
		print "	exporting chunk $i with index $self->{counter}->{$i}\n";
		my $fh = 'chunk_'.$i.'_index_'.$self->{counter}->{$i}.'.bin';
		my $bin_fh = IO::File->new($fh, 'w') or die "cannot open $fn\n";
		binmode $bin_fh;
		$bin_fh->write($self->{$i}, length($self->{$i}));
		$bin_fh->close();
		print $chltx "file_$i = $fh\n";
		$i++;
	}
	$chltx->close();
}
#######################################################################
package main;
use strict;
use Getopt::Long;
use File::Path;
use stkutils::data_packet;
use Cwd;
#local $SIG{__WARN__} = sub {die};

sub usage {
	return <<END
Chunk file splitter
Usage: cfs [-d input_file] [-o outdir]
       cfs [-c input_dir] [-o outfile]
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
	my $file = chunked->new();
print "reading $input_file...\n";	
	$file->read($input_file);
	defined $out && do {
		File::Path::mkpath($out, 0);
		chdir $out or die "cannot change path to $out\n";
	};
print "exporting chunks...\n";	
	$file->export('all.ltx');	
	print "done!\n";
} elsif (defined $src_dir) {
	die "bad params\n" if (defined $input_file);
	chdir $src_dir or die "cannot change dir to $src_dir\n";
	my $file = chunked->new();
	$out = 'chunked_file.new' unless defined $out;
print "importing chunks...\n";
	$file->import($src_dir);
print "writing $out...\n";
	chdir $work_dir;
	$file->write($out);
print "done!\n";
} else {
	die usage();
}
#######################################################################