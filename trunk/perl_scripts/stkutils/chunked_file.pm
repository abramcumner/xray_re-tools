package stkutils::chunked_file;

use strict;
use stkutils::debug;
use IO::File;

sub new {
	my $class = shift;
	my ($fn, $mode) = @_;

	my $fh = IO::File->new($fn, $mode) or return undef;
	binmode $fh;

	my $self = {};
	$self->{fh} = $fh;
	$self->{offset} = 0;
	$self->{glob_pos} = [];
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
	$self->{offset} <= $offset or stkutils::debug::fail(
		__PACKAGE__.'::r_chunk_open',
		__LINE__, 
		'$self->{offset} <= $offset', 
		'current position is outside current chunk');
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
		if ($index == 0 && $size == 0) {
			$self->r_chunk_close();
			pop @{$self->{end_offsets}};
			my $pos = tell $self->{fh};
			push @{$self->{end_offsets}}, $pos - 8;
			$offset = $self->{end_offsets}[$#{$self->{end_offsets}}];
			last;
		}
		if ($index == $chunk) {
			push @{$self->{glob_pos}}, $pos;
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
	$self->{offset} <= $offset or stkutils::debug::fail(
		__PACKAGE__.'::close_found_chunk', 
		__LINE__, 
		'$self->{offset} <= $offset', 
		'current position is outside current chunk');
	$self->{fh}->seek($self->{glob_pos}[$#{$self->{glob_pos}}], SEEK_SET);
	$self->{offset} = pop @{$self->{glob_pos}};
}
sub r_chunk_safe {
	my $self = shift;
	my ($id, $dsize) = @_;
	my $size = $self->find_chunk($id);
	if ($size && $size == $dsize) {
		return $size;
	} elsif ($size) {
		stkutils::debug::fail(
		__PACKAGE__.'::close_found_chunk', 
		__LINE__, 
		'$size == $dsize', 
		"size of chunk $id ($size) is not match with expected size ($dsize)");
	} else {
		return undef;
	}
}
sub r_chunk_data {
	my $self = shift;
	my ($size) = @_;
	my $offset = $self->{end_offsets}[$#{$self->{end_offsets}}];
	defined($size) or $size = $offset - $self->{offset};
	$self->{offset} + $size <= $offset or stkutils::debug::fail(
		__PACKAGE__.'::r_chunk_data', 
		__LINE__, 
		'$self->{offset} + $size <= $offset', 
		'length of requested data is bigger than one left in chunk');
	my $data = '';
	$size > 0 && ($self->{fh}->read($data, $size) or stkutils::debug::fail(
		__PACKAGE__.'::r_chunk_data', 
		__LINE__, 
		'$size > 0 && $self->{fh}->read($data, $size)', 
		'cannot read requested data'));
	$self->{offset} += $size;
	return $data;
}
sub r_chunk_seek {
	my $self = shift;
	my ($seek_offset) = @_;
	my $offset = $self->{end_offsets}[$#{$self->{end_offsets}}];
	defined($seek_offset) or stkutils::debug::fail(
		__PACKAGE__.'::r_chunk_seek', 
		__LINE__, 
		'defined($seek_offset)', 
		'you must set seek offset to use this method');
	$self->{offset} + $seek_offset <= $offset or stkutils::debug::fail(
		__PACKAGE__.'::r_chunk_seek', 
		__LINE__, 
		'$self->{offset} + $seek_offset <= $offset', 
		'you try to seek outside current chunk');
	$seek_offset > 0 && ($self->{fh}->seek($seek_offset, SEEK_CUR) or stkutils::debug::fail(
		__PACKAGE__.'::r_chunk_seek', 
		__LINE__, 
		'$seek_offset > 0 && $self->{fh}->seek($seek_offset, SEEK_CUR)', 
		'cannot seek requested offset'));
	$self->{offset} += $seek_offset;
}
sub w_chunk_open {
	my $self = shift;
	my ($index) = @_;
	my $data = pack('VV', $index, 0xaaaaaaaa);
	$self->{fh}->write($data, 8) or stkutils::debug::fail(
		__PACKAGE__.'::w_chunk_open', 
		__LINE__, 
		'$self->{fh}->write($data, 8)',
		"cannot open chunk $index");
	push @{$self->{start_offsets}}, $self->{offset};
	$self->{offset} += 8;
}
sub w_chunk_close {
	my $self = shift;
	my $offset = pop @{$self->{start_offsets}};
	$self->{fh}->seek($offset + 4, SEEK_SET) or stkutils::debug::fail(
		__PACKAGE__.'::w_chunk_close', 
		__LINE__,
		'$self->{fh}->seek($offset + 4, SEEK_SET)',
		"cannot close chunk");
	my $data = pack('V', $self->{offset} - $offset - 8);
	$self->{fh}->write($data, 4) or stkutils::debug::fail(
		__PACKAGE__.'::w_chunk_close', 
		__LINE__, 
		'$self->{fh}->write($data, 4)',
		"cannot write size of chunk");
	$self->{fh}->seek($self->{offset}, SEEK_SET) or stkutils::debug::fail(
		__PACKAGE__.'::w_chunk_close', 
		__LINE__, 
		'$self->{fh}->seek($self->{offset}, SEEK_SET)',
		"cannot seek current write position");
}
sub w_chunk_data {
	my $self = shift;
	my ($data) = @_;
	my $size = length($data);
	$self->{fh}->write($data, $size) or stkutils::debug::fail(
		__PACKAGE__.'::w_chunk_data', 
		__LINE__, 
		'$self->{fh}->write($data, $size)',
		"cannot write data");
	$self->{offset} += $size;
}
sub w_chunk {
	my $self = shift;
	my ($index, $data) = @_;
	my $size = length($data);
	my $hdr = pack('VV', $index, $size);
	$self->{fh}->write($hdr, 8) or stkutils::debug::fail(
		__PACKAGE__.'::w_chunk', 
		__LINE__, 
		"cannot write header of chunk $index");
	$size > 0 && ($self->{fh}->write($data, $size) or stkutils::debug::fail(
		__PACKAGE__.'::w_chunk', 
		__LINE__, 
		'$size > 0 && ($self->{fh}->write($data, $size)',
		"cannot write data in chunk $index"));
	$self->{offset} += $size + 8;
}
1;
