#!/bin/perl -w -I \temp\1\bin
#
# tab size:	8
#
#######################################################################
package snd_static;
use strict;
use constant properties_info => (
	{ name => 'sound_name',		type => 'sz' },
	{ name => 'position',		type => 'f32v3' },
	{ name => 'volume',		type => 'f32' },
	{ name => 'frequency',		type => 'f32' },
	{ name => 'active_time',		type => 'f32v2' },
	{ name => 'play_time',		type => 'f32v2' },
	{ name => 'pause_time',	type => 'f32v2' },
	{ name => 'min_dist',		type => 'f32' },
	{ name => 'max_dist',		type => 'f32' },
);
sub new {
	my $class = shift;
	my $self = {};
	bless $self, $class;
	return $self;
}
sub read {
	if (::old()) {
		$_[1]->unpack_properties($_[0], (properties_info)[0..3, 7..8]);
	} else {
		$_[1]->unpack_properties($_[0], (properties_info)[0..6]);
	}	
}
sub write {
	if (::old()) {
		$_[1]->pack_properties($_[0], (properties_info)[0..3, 7..8]);
	} else {
		$_[1]->pack_properties($_[0], (properties_info)[0..6]);
	}	
}
sub import {
	if (::old()) {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0..3, 7..8]);
	} else {
		$_[1]->import_properties($_[2], $_[0], (properties_info)[0..6]);
	}	
}
sub export {
	if (::old()) {
		$_[1]->export_properties(undef, $_[0], (properties_info)[0..3, 7..8]);
	} else {
		$_[1]->export_properties(undef, $_[0], (properties_info)[0..6]);
	}	
}
#######################################################################
package level_snd_static;
use strict;
use stkutils::data_packet;
use stkutils::chunked_file;
use stkutils::ini_file;
sub new {
	my $class = shift;
	my $self = {};
	bless $self, $class;
	return $self;
}
sub read {
	my $self = shift;
	my ($fn) = @_;
	my $cf = stkutils::chunked_file->new($fn, 'r') or die;
	my $expected_index = 0;
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined $index or last;
		die unless $expected_index == $index;
		my $packet;
		if (::old()) {
			$packet = stkutils::data_packet->new($cf->r_chunk_data());
		} else {
			$packet = stkutils::data_packet->new(substr($cf->r_chunk_data(), 8));
		}
		my $snd_static = snd_static->new($packet);
		$snd_static->read($packet);
		push @{$self->{snd_statics}}, $snd_static;
		$expected_index++;
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub write {
	my $self = shift;
	my ($fn) = @_;
	my $cf = stkutils::chunked_file->new($fn, 'w') or die;
	my $index = 0;
	foreach my $snd_static (@{$self->{snd_statics}}) {
		my $packet = stkutils::data_packet->new();
		$snd_static->write($packet);
		$cf->w_chunk_snd($index, $packet->data());
		$index++;
	}
	$cf->close();
}
sub import {
	my $self = shift;
	my ($fn) = @_;
	my $if = stkutils::ini_file->new($fn, 'r') or die;
	foreach my $section (@{$if->{sections_list}}) {
		my $snd_static = snd_static->new();
		$snd_static->import($if, $section);
		push @{$self->{snd_statics}}, $snd_static;
	}
	$if->close()
}
sub export {
	my $self = shift;
	my ($fn) = @_;
	my $if = stkutils::ini_file->new($fn, 'w') or die;
	my $fh = $if->{fh};
	my $index = 0;
	foreach my $snd_static (@{$self->{snd_statics}}) {
		print $fh "[$index]\n";
		$snd_static->export($if, "$index");
		print $fh "\n";
		$index++;
	}
	$if->close()
}
#######################################################################
package main;
use strict;
use Getopt::Long;

sub usage {
	return <<END
S.T.A.L.K.E.R. level.snd_static compiler/decompiler
Usage: sscdc -d level.snd_static [-o ltxfile] -old
       sscdc -c snd_static.ltx [-o binfile]	-old
END
}

my $snd_static_file;
my $snd_static_ltx;
my $out_file;
my $old;

GetOptions(
	'd=s' => \$snd_static_file,
	'c=s' => \$snd_static_ltx,
	'o=s' => \$out_file,
	'old' => \$old,
) or die usage();

sub old {return defined $old;}

if (defined $snd_static_file) {
	die "bad params\n" if defined $snd_static_ltx;
	my $snd_static = level_snd_static->new();
	$snd_static->read($snd_static_file);
	$snd_static->export($out_file or 'snd_static.ltx');
} elsif (defined $snd_static_ltx) {
	die "bad params\n" if defined $snd_static_file;
	my $snd_static = level_snd_static->new();
	$snd_static->import($snd_static_ltx);
	$snd_static->write($out_file or 'level.snd_static.new');
} else {
	die usage();
}