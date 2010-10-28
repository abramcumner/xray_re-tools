#!/bin/perl -w -I \temp\1\bin
#
# tab size:	8
#
#######################################################################
package env_modifier;
use strict;
use constant properties_info => (
	{ name => 'position',		type => 'f32v3' },
	{ name => 'radius',		type => 'f32' },
	{ name => 'power',		type => 'f32' },
	{ name => 'far_plane',		type => 'f32' },
	{ name => 'fog_color',		type => 'f32v3' },
	{ name => 'fog_density',	type => 'f32' },
	{ name => 'ambient',		type => 'f32v3' },
	{ name => 'sky_color',		type => 'f32v3' },
	{ name => 'hemi_color',		type => 'f32v3' },
	{ name => 'extra',		type => 'h16' },
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
package level_env_mod;
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

	my ($index, $size) = $cf->r_chunk_open();
	die unless $index == 0 && $size == 4;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($version) = $packet->unpack('V');
	die unless $version == 23;
	$cf->r_chunk_close();

	my $expected_index = 1;
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined $index or last;
		die unless $expected_index == $index;
		my $packet = stkutils::data_packet->new($cf->r_chunk_data());
		my $env_modifier = env_modifier->new($packet);
		$env_modifier->read($packet);
		push @{$self->{env_modifiers}}, $env_modifier;
		$expected_index++;
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub write {
	my $self = shift;
	my ($fn) = @_;
	my $cf = stkutils::chunked_file->new($fn, 'w') or die;

	my $packet = stkutils::data_packet->new();
	$packet->pack('V', 23);
	$cf->w_chunk(0, $packet->data());

	my $index = 1;
	foreach my $env_modifier (@{$self->{env_modifiers}}) {
		my $packet = stkutils::data_packet->new();
		$env_modifier->write($packet);
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
		my $env_modifier = env_modifier->new();
		$env_modifier->import($if, $section);
		push @{$self->{env_modifiers}}, $env_modifier;
	}
	$if->close()
}
sub export {
	my $self = shift;
	my ($fn) = @_;
	my $if = stkutils::ini_file->new($fn, 'w') or die;
	my $fh = $if->{fh};
	my $index = 0;
	foreach my $env_modifier (@{$self->{env_modifiers}}) {
		my $section = "modifier$index";
		print $fh "[$section]\n";
		$env_modifier->export($if, $section);
		print $fh "\n";
		$index++;
	}
	$if->close()
}

#######################################################################
package main;
use strict;
use Getopt::Long;

#test1(); exit();

sub usage {
	return <<END
S.T.A.L.K.E.R: Clear Sky level.env_mod compiler/decompiler
Usage: emcdc -d level.env_mod [-o ltxfile]
       emcdc -c env_mod.ltx [-o binfile]
END
}

my $env_mod_file;
my $env_mod_ltx;
my $out_file;

GetOptions(
	'd=s' => \$env_mod_file,
	'c=s' => \$env_mod_ltx,
	'o=s' => \$out_file,
) or die usage();

if (defined $env_mod_file) {
	die "bad params\n" if defined $env_mod_ltx;
	my $env_mod = level_env_mod->new();
	$env_mod->read($env_mod_file);
	$env_mod->export($out_file or 'env_mod.ltx');
} elsif (defined $env_mod_ltx) {
	die "bad params\n" if defined $env_mod_file;
	my $env_mod = level_env_mod->new();
	$env_mod->import($env_mod_ltx);
	$env_mod->write($out_file or 'level.env_mod.new');
} else {
	die usage();
}

#######################################################################
sub test0 {
	my $em = level_env_mod->new();
	$em->read('level.env_mod');
	$em->write('level.env_mod.new');
}

sub test1 {
	my $em = level_env_mod->new();
	$em->read('level.env_mod');
	$em->export('env_mod.ltx');
}
