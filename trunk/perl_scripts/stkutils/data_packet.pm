# SLOW!!!
package stkutils::data_packet;
use strict;
use IO::File;
use stkutils::debug;
use stkutils::ini_file;
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
	my ($len) = @_;
stkutils::debug::fail(__PACKAGE__.'::unpack', __LINE__, 'defined $self', "packet is not defined") if !(defined $self);
stkutils::debug::fail(__PACKAGE__.'::unpack', __LINE__, 'defined $self->{data}', "there is no data in packet") if !(defined $self->{data});
stkutils::debug::fail(__PACKAGE__.'::unpack', __LINE__, 'defined $template', "template is not defined") if !(defined $template);
	my @values;
	if (!defined $len) {
		@values = CORE::unpack($template.'a*', $self->{data});
		stkutils::debug::fail(__PACKAGE__.'::unpack', __LINE__, '$#values != -1', "cannot unpack requested data") if $#values == -1;
		$self->{data} = pop(@values);
	} else {
		my $d = substr($self->{data}, 0, $len, '');
		@values = CORE::unpack($template, $d);
	}
stkutils::debug::fail(__PACKAGE__.'::unpack', __LINE__, 'defined $self->{data}', "data container is empty") if !(defined $self->{data});
#print "@values\n";
	return @values;
}
sub pack {
	my $self = shift;
	my $template = shift;
stkutils::debug::fail(__PACKAGE__.'::pack', __LINE__, 'defined $template', "template is not defined") if !(defined $template);
stkutils::debug::fail(__PACKAGE__.'::pack', __LINE__, 'defined @_', "data is not defined") if !(defined @_);
stkutils::debug::fail(__PACKAGE__.'::pack', __LINE__, '!defined $_[0]', "packet is not defined") unless defined $_[0];
#print "@_\n";
	$self->{data} .= CORE::pack($template, @_);
}
use constant template_for_scalar => {
	h32	=> 'V',
	h16	=> 'v',
	h8	=> 'C',
	u32	=> 'V',
	u16	=> 'v',
	u8	=> 'C',
	s32	=> 'l',
	s16	=> 'v',
	s8	=> 'C',
	sz	=> 'Z*',
	f32	=> 'f',
	guid	=> 'a[16]',
};
use constant template_len => {
	'V' => 4,
	'v' => 2,
	'C' => 1,
	'l' => 4,
	'f' => 4,
	'a[16]' => 16,
	'C8' => 8,
	'C4' => 4,
	'f3' => 12,
	'f4' => 16,
	'l3' => 12,
	'l4' => 16,
	'V3' => 12,
	'V4' => 16,	
	'C171' => 171,
	'C12' => 12,
};
use constant template_for_vector => {
	l8u8v	=> 'C/C',
	l32u8v	=> 'V/C',
	l32u16v	=> 'V/v',
	l32u32v	=> 'V/V',
	l32szv	=> 'V/(Z*)',
	l8szbv	=> 'C/(Z*C)',
	u8v8	=> 'C8',
	u8v4	=> 'C4',
	f32v3	=> 'f3',
	f32v4	=> 'f4',
	s32v3	=> 'l3',
	s32v4	=> 'l4',
	h32v3	=> 'V3',
	h32v4	=> 'V4',	
	actorData => 'C171',
	ha1	=> 'C12',
	ha2	=> 'C8',
};
sub unpack_properties {
	my $self = shift;
	my $container = shift;
	foreach my $p (@_) {
#print "$p->{name} = ";
		if ($p->{type} eq 'shape') {
			my ($count) = $self->unpack('C', 1);
			while ($count--) {
				my %shape;
				($shape{type}) = $self->unpack('C', 1);
				if ($shape{type} == 0) {
					@{$shape{sphere}} = $self->unpack('f4', 16);
				} elsif ($shape{type} == 1) {
					@{$shape{box}} = $self->unpack('f12', 48);
				} else {
					stkutils::debug::fail(__PACKAGE__.'::unpack_properties', __LINE__, '$shape{type} == 0 or $shape{type} == 1', "shape has undefined type ($shape{type})");
				}
				push @{$container->{$p->{name}}}, \%shape;
			}
#		} elsif ($p->{type} eq 'ordaf') {
#			my ($count) = $self->unpack('V');
#			if ($count == 0) {
#				($container->{$p->{name}}) = 0;
#			}
#			while ($count--) {
#				my %obj;
#				($obj{name}, $obj{number}) = $self->unpack('Z*V');
#				my ($inner_count) = $self->unpack('V');
#				while ($inner_count--) {
#					my %afs; 
#					($afs{af_section}) = $self->unpack('Z*VV');
#					push @{$obj{af_sects}}, \%afs;
#				}
#				push @{$container->{$p->{name}}}, \%obj;
#			}
		} elsif ($p->{type} eq 'suppl') {
			my ($count) = $self->unpack('V', 4);
			while ($count--) {
				my %obj;
				($obj{section_name}) = $self->unpack('Z*');
				($obj{item_count}, $obj{min_factor}, $obj{max_factor}) = $self->unpack('Vff', 12);
				push @{$container->{$p->{name}}}, \%obj;
			}
		} elsif ($p->{type} eq 'f32v4') {				#let's shut up #QNAN warnings.
			my @buf = $self->unpack('f4', 16);
			my $i = 0;
			while ($i < 4) {
				if (!defined ($buf[$i] <=> 9**9**9)) {
#					print "replacing bad float $buf[$i]...\n";
					$buf[$i] = 0;
				}
				$i++;
			}
			@{$container->{$p->{name}}} = @buf;
		} elsif ($p->{type} eq 'f32v3') {				
			my @buf = $self->unpack('f3', 12);
			my $i = 0;
			while ($i < 3) {
				if (!defined ($buf[$i] <=> 9**9**9)) {
#					print "replacing bad float $buf[$i]...\n";
					$buf[$i] = 0;
				}
				$i++;
			}
			@{$container->{$p->{name}}} = @buf;
		} elsif ($p->{type} eq 'afspawns' or $p->{type} eq 'afspawns_u32') {
			my ($count) = $self->unpack('v', 2);
			while ($count--) {
				my %obj;
				if ($p->{type} eq 'afspawns') {
					($obj{section_name}) = $self->unpack('Z*');
					($obj{weight}) = $self->unpack('f', 4);
				} else {
					($obj{section_name}) = $self->unpack('Z*');
					($obj{weight}) = $self->unpack('V', 4);
				}
				push @{$container->{$p->{name}}}, \%obj;
			}
		} else {
			my $template = template_for_scalar->{$p->{type}};
			if (defined $template) {
				$self->error_handler($container, $template) if CORE::length($self->{data}) == 0;
				($container->{$p->{name}}) = $self->unpack($template, template_len->{$template});
				if ($p->{type} eq 'sz') {
					chomp $container->{$p->{name}};
					$container->{$p->{name}} =~ s/\r//g;
				}
			} elsif ($p->{type} eq 'u24') {
				($container->{$p->{name}}) = CORE::unpack('V', CORE::pack('CCCC', $self->unpack('C3', 3), 0));
			} elsif ($p->{type} eq 'q16') {
				my ($qf) = $self->unpack('v', 2);
				($container->{$p->{name}}) = convert_q16($qf);
			} elsif ($p->{type} eq 'q16_old') {
				my ($qf) = $self->unpack('v', 2);
				($container->{$p->{name}}) = convert_q16_old($qf);
			} elsif ($p->{type} eq 'q8') {
				my ($q8) = $self->unpack('C', 1);
				($container->{$p->{name}}) = convert_q8($q8);
			} elsif ($p->{type} eq 'q8v3') {
				my (@q8) = $self->unpack('C3', 3);
				my $i = 0;
				while ($i < 3) {
					@{$container->{$p->{name}}}[$i] = convert_q8($q8[$i]);
					$i++;
				}
			} elsif ($p->{type} eq 'q8v4') {
				my (@q8) = $self->unpack('C4', 4);
				my $i = 0;
				while ($i < 4) {
					@{$container->{$p->{name}}}[$i] = convert_q8($q8[$i]);
					$i++;
				}
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
#print "$p->{name} = ";
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
		} elsif ($p->{type} eq 'u24') {
			$self->pack('CCC', CORE::unpack('CCCC', CORE::pack('V', $container->{$p->{name}})));
		} elsif ($p->{type} eq 'q16') {
			my $f16 = $container->{$p->{name}};
			$self->pack("v", convert_u16($f16));
		} elsif ($p->{type} eq 'q16_old') {
			my $f16 = $container->{$p->{name}};
			$self->pack("v", convert_u16_old($f16));
		} elsif ($p->{type} eq 'suppl') {
			$self->pack('V', $#{$container->{$p->{name}}} + 1);
			foreach my $sect (@{$container->{$p->{name}}}) {
				$self->pack('Z*Vff', $$sect{section_name}, $$sect{item_count}, $$sect{min_factor}, $$sect{max_factor});
			}
		} elsif ($p->{type} eq 'afspawns') {
			$self->pack('v', $#{$container->{$p->{name}}} + 1);
			foreach my $sect (@{$container->{$p->{name}}}) {
				$self->pack('Z*f', $$sect{section_name}, $$sect{weight});
			}
		} elsif ($p->{type} eq 'afspawns_u32') {
			$self->pack('v', $#{$container->{$p->{name}}} + 1);
			foreach my $sect (@{$container->{$p->{name}}}) {
				$self->pack('Z*V', $$sect{section_name}, $$sect{weight});
			}
		} elsif ($p->{type} eq 'q8') {
			my $f8 = $container->{$p->{name}};
			$self->pack("C", convert_u8($f8));
#		} elsif ($p->{type} eq 'ordaf') {
#			$self->pack('V', $#{$container->{$p->{name}}} + 1);
#			my $i = 0;
#			foreach my $sect (@{$container->{$p->{name}}}) {
#				$self->pack('Z*V', $$sect[0..1]);
#				$self->pack('V', $#{$sect[2]} + 1);
#				my $k = 0;
#				foreach my $af_sect (@{$sect[2]}) {
#					$self->pack('Z*VV', $af_sect[0], $af_sect[1], $af_sect[2]);
#					$k++;
#				}
#				$i++;
#			}
		} else {
			my $n = $#{$container->{$p->{name}}} + 1;
			if ($p->{type} eq 'l32u16v') {
				$self->pack("Vv$n", $n, @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'l32u8v') {
				$self->pack("VC$n", $n, @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'l32u32v') {
				$self->pack("VV$n", $n, @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'l32szv') {
				$self->pack("V(Z*)$n", $n, @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'l8u8v') {
				$self->pack("CC$n", $n, @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'q8v') {
				$self->pack("C$n", @{$container->{$p->{name}}});
			} elsif ($p->{type} eq 'q8v3' or $p->{type} eq 'q8v4') {
				foreach my $u8 (@{$container->{$p->{name}}}) {
					$self->pack('C', convert_u8($u8));
				}
			} elsif ($p->{type} eq 'l8szbv') {
				$n = $n/2;
				$self->pack("C(Z*C)$n", $n, @{$container->{$p->{name}}});
			} elsif (exists (template_for_vector->{$p->{type}})) {
				$self->pack(template_for_vector->{$p->{type}}, @{$container->{$p->{name}}});
			} else {
				stkutils::debug::fail(__PACKAGE__.'::pack_properties', __LINE__, '', "cannot find proper template for type $p->{type}");
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
sub convert_q8 {
	my ($u) = @_;
	my $q = ($u / 255.0);
	return $q;
}
sub convert_u8 {
	my ($q) = @_;
	my $u = int ($q * 255.0);
	return $u;
}
sub convert_q16 {
	my ($u) = @_;
	my $q = (($u / 43.69) - 500.0);
	return $q;
}
sub convert_u16 {
	my ($q) = @_;
	my $u = (($q + 500.0) * 43.69);
	return $u;
}
sub convert_q16_old {
	my ($u) = @_;
	my $q = (($u / 32.77) - 1000.0);
	return $q;
}
sub convert_u16_old {
	my ($q) = @_;
	my $u = (($q + 1000.0) * 32.77);
	return $u;
}
sub error_handler {
	my $self = shift;
	my ($container, $template) = @_;
	if (($template eq 'C') || (ref($container) eq 'se_zone_anom')) {
		bless $container, 'cse_alife_anomalous_zone';
		my $ini = stkutils::ini_file->new('sections.ini', 'r');
		$ini->{sections_hash}{'sections'}{$container->{section_name}} = 'cse_alife_anomalous_zone';
		my $ini_new = IO::File->new('sections.new.ini', 'w');
		print $ini_new "[sections]\n";
		foreach my $section (keys %{$ini->{sections_hash}{'sections'}}) {
			my $val = $ini->{sections_hash}{'sections'}{$section};
			print $ini_new "$section = $val\n";
		}
		$ini->close();
		$ini_new->close();
		unlink 'sections.ini';
		rename 'sections.new.ini', 'sections.ini';
		unlink 'sections.new.ini';
	} else {
		stkutils::debug::fail(__PACKAGE__.'::error_handler', __LINE__, '', "cannot fix NarSol problem\n");
	}
}
1;
