# FIXME: this is very SLOW!!!

package stkutils::data_packet;
use strict;

sub new {
	my $class = shift;
	my $data = shift;
	my $self = {};
	$self->{data} = ($data or '');
	$self->{init_length} = CORE::length($self->{data});
	bless($self, $class);
	return $self;
}
sub unpack {
	my $self = shift;
	my $template = shift;
die if !(defined $self);
die if !(defined $self->{data});
die if !(defined $template);
die if CORE::length($self->{data}) == 0;
	my @values = CORE::unpack($template.'a*', $self->{data});
die if $#values == -1;
	$self->{data} = splice(@values, -1);
die if !(defined $self->{data});
	return @values;
}
sub pack {
	my $self = shift;
	my $template = shift;
die if !(defined($template));
die if !(defined(@_));
die unless defined $_[0];
	$self->{data} .= CORE::pack($template, @_);
}
use constant template_for_scalar => {
	h32	=> 'V',
	h16	=> 'v',
	h8	=> 'C',
	u32	=> 'V',
	u16	=> 'v',
	u8	=> 'C',
	q8	=> 'C',
	s32	=> 'l',
	s16	=> 'v',
	s8	=> 'C',
	sz	=> 'Z*',
	f32	=> 'f',
	guid	=> 'a[16]',
};
use constant template_for_vector => {
	l8u8v	=> 'C/C',
	l32u8v	=> 'V/C',
	l32u16v	=> 'V/v',
	l32szv	=> 'V/(Z*)',
	l8szbv	=> 'C/(Z*C)',
	u8v8	=> 'C8',
	u8v4	=> 'C4',
	f32v3	=> 'f3',
	f32v4	=> 'f4',
	s32v3	=> 'l3',
	s32v4	=> 'l4',
};
sub unpack_properties {
	my $self = shift;
	my $container = shift;

	foreach my $p (@_) {
		#print "unpacking $p->{name} type $p->{type}\n";
		if ($p->{type} eq 'shape') {
			my ($count) = $self->unpack('C');
			while ($count--) {
				my %shape;
				($shape{type}) = $self->unpack('C');
				if ($shape{type} == 0) {
					@{$shape{sphere}} = $self->unpack('f4');
				} elsif ($shape{type} == 1) {
					@{$shape{box}} = $self->unpack('f12');
				} else {
					die;
				}
				push @{$container->{$p->{name}}}, \%shape;
			}
		} else {
			my $template = template_for_scalar->{$p->{type}};
			if (defined $template) {
				($container->{$p->{name}}) = $self->unpack($template);
				if ($p->{type} eq 'sz') {
					chomp $container->{$p->{name}};
					$container->{$p->{name}} =~ s/\r//g;
				}
			} elsif ($p->{type} eq 'u24') {
				($container->{$p->{name}}) = CORE::unpack('V', CORE::pack('CCCC', $self->unpack('C3'), 0));
			} else {
				@{$container->{$p->{name}}} = $self->unpack(template_for_vector->{$p->{type}});
			}
		}
	}
}
sub pack_properties {
	my $self = shift;
	my $container = shift;

	foreach my $p (@_) {
		my $template = template_for_scalar->{$p->{type}};
		if (defined $template) {
			$self->pack($template, $container->{$p->{name}});
		} elsif ($p->{type} eq 'shape') {
			$self->pack('C', $#{$container->{$p->{name}}} + 1);
			foreach my $shape (@{$container->{$p->{name}}}) {
				$self->pack('C', $$shape{type});
				if ($$shape{type} == 0) {
					$self->pack('f4', @{$$shape{sphere}});
				} elsif ($$shape{type} == 1) {
					$self->pack('f12', @{$$shape{box}});
				}
			}
		} else {
			my $n = $#{$container->{$p->{name}}} + 1;
			if ($p->{type} eq 'l32u16v') {
				$self->pack("Vv$n", $n, @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'l32u8v') {
				$self->pack("VC$n", $n, @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'l32szv') {
				$self->pack("V(Z*)$n", $n, @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'l8u8v') {
				$self->pack("CC$n", $n, @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'u8v8' or $p->{type} eq 'u8v4') {
				$self->pack("C$n", @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'f32v3') {
				$self->pack('f3', @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'f32v4') {
				$self->pack('f4', @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 's32v3') {
				$self->pack('l3', @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 's32v4') {
				$self->pack('l4', @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'q8v') {
				$self->pack("C$n", @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'l8szbv') {
				$self->pack("C(Z*C)$n", $n/2, @{$container->{$p->{name}}});
			} else {
				die;
			}
		}
	}
}
sub length {
	return CORE::length($_[0]->{data});
}
sub r_tell {
	return $_[0]->{init_length} - CORE::length($_[0]->{data});
}
sub w_tell {
	return CORE::length($_[0]->{data});
}
sub data {
	return $_[0]->{data};
}

1;
