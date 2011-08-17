#!/bin/perl -w -I \temp\1\bin
#
# tab size:	8
#
#######################################################################
package particle_static;
use strict;
use constant properties_info => (
	{ name => 'particle_name',			type => 'sz' },
	{ name => 'M11_M12_M13_M14',		type => 'f32v4' },
	{ name => 'M21_M22_M23_M24',		type => 'f32v4'},
	{ name => 'M31_M32_M33_M34',		type => 'f32v4' },
	{ name => 'M41_M42_M43_M44',		type => 'f32v4' },
);
sub new {
	my $class = shift;
	my $self = {};
	bless $self, $class;
	return $self;
}
sub read {
	$_[1]->unpack_properties($_[0], properties_info);
}
sub write {
	$_[1]->pack_properties($_[0], properties_info);
}
sub import {
	$_[1]->import_properties($_[2], $_[0], properties_info);
}
sub export {
	$_[1]->export_properties(undef, $_[0], properties_info);
}

#######################################################################
package level_particle_static;
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
		my $packet = stkutils::data_packet->new($cf->r_chunk_data());
		my $particle_static = particle_static->new($packet);
		$particle_static->read($packet);
		push @{$self->{particle_statics}}, $particle_static;
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
	foreach my $particle_static (@{$self->{particle_statics}}) {
		my $packet = stkutils::data_packet->new();
		$particle_static->write($packet);
		$cf->w_chunk($index, $packet->data());
		$index++;
	}
	$cf->close();
}
sub import {
	my $self = shift;
	my ($fn) = @_;
	my $if = stkutils::ini_file->new($fn, 'r') or die;
	foreach my $section (@{$if->{sections_list}}) {
		my $particle_static = particle_static->new();
		$particle_static->import($if, $section);
		push @{$self->{particle_statics}}, $particle_static;
	}
	$if->close()
}
sub export {
	my $self = shift;
	my ($fn) = @_;
	my $if = stkutils::ini_file->new($fn, 'w') or die;
	my $fh = $if->{fh};
	my $index = 0;
	foreach my $particle_static (@{$self->{particle_statics}}) {
		my $section = "particle $index";
		print $fh "[$section]\n";
		$particle_static->export($if, $section);
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
S.T.A.L.K.E.R. level.ps_static compiler/decompiler
Usage:	pscdc -d level.ps_static [-o ltxfile]
		pse -c level.ps_static.ltx [-o binfile]
END
}

my $ps_static_file;
my $ps_static_ltx;
my $out_file;

GetOptions(
	'd=s' => \$ps_static_file,
	'c=s' => \$ps_static_ltx,
	'o=s' => \$out_file,
) or die usage();

if (defined $ps_static_file) {
	die "bad params\n" if defined $ps_static_ltx;
	my $ps_static = level_particle_static->new();
	$ps_static->read($ps_static_file);
	$ps_static->export($out_file or 'ps_static.ltx');
} elsif (defined $ps_static_ltx) {
	die "bad params\n" if defined $ps_static_file;
	my $ps_static = level_particle_static->new();
	$ps_static->import($ps_static_ltx);
	$ps_static->write($out_file or 'level.ps_static.new');
} else {
	die usage();
}