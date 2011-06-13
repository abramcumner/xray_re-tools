package stkutils::chunked_file;

use strict;
use IO::File;
use diagnostics;
our @ISA = "IO::File";

sub new {
	my $class = shift;
	my ($fn, $mode) = @_;

	my $fh = IO::File->new($fn, $mode) or return undef;
	binmode $fh;

	my $self = {};
	$self->{fh} = $fh;
	$self->{offset} = 0;
	$self->{glob_pos} = 0;
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
sub find_chunk {
	my $self = shift;
	my ($chunk) = @_;
	my $pos = tell $self->{fh};
	my $offset = $self->{end_offsets}[$#{$self->{end_offsets}}];
	defined ($offset) or $offset = -s $self->{fh};
	defined ($offset) && $self->{offset} >= $offset && return undef;
	my $data;
	while ($self->{offset} < $offset) {
		my ($index, $size) = $self->r_chunk_open();
		if ($index == $chunk) {
			$self->{glob_pos} = $pos;
			return $size;
		}
		$self->r_chunk_close();
	}
	$self->{fh}->seek($pos, SEEK_SET);
	$self->{offset} = tell $self->{fh};
	return undef;
}
sub close_found_chunk {
	my $self = shift;
	my $offset = pop @{$self->{end_offsets}};
	defined $offset or $offset = tell $self->{fh};
	$self->{offset} <= $offset or die;
	$self->{fh}->seek($self->{glob_pos}, SEEK_SET);
	$self->{offset} = $self->{glob_pos};
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
sub w_chunk_open {
	my $self = shift;
	my ($index) = @_;
	my $data = pack('VV', $index, 0xaaaaaaaa);
	$self->{fh}->write($data, 8) or die;
	push @{$self->{start_offsets}}, $self->{offset};
	$self->{offset} += 8;
}
sub w_chunk_close {
	my $self = shift;
	my $offset = pop @{$self->{start_offsets}};
	$self->{fh}->seek($offset + 4, SEEK_SET) or die;
	my $data = pack('V', $self->{offset} - $offset - 8);
	$self->{fh}->write($data, 4) or die;
	$self->{fh}->seek($self->{offset}, SEEK_SET) or die;
}
sub w_chunk_data {
	my $self = shift;
	my ($data) = @_;
	my $size = length($data);
	$self->{fh}->write($data, $size) or die;
	$self->{offset} += $size;
}
sub w_chunk {
	my $self = shift;
	my ($index, $data) = @_;
	my $size = length($data);
	my $hdr = pack('VV', $index, $size);
	$self->{fh}->write($hdr, 8) or die;
	$size > 0 && ($self->{fh}->write($data, $size) or die);
	$self->{offset} += $size + 8;
}

1;
