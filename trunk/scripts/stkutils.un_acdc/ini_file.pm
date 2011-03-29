package stkutils::ini_file;

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
			$self->{sections_hash}{$section}{$name} = $value;
		}
	}
	return $self;
}
sub close {
	my $self = shift;
	$self->{fh}->close();
	$self->{fh} = undef;
}
use constant format_for_number => {
	h32	=> '%#x',
	h16	=> '%#x',
	h8	=> '%#x',
	u32	=> '%u',
	u16	=> '%u',
	u8	=> '%u',
	q8	=> '%u',
	s32	=> '%d',
	s16	=> '%d',
	s8	=> '%d',
	f32	=> '%.8g',
};
sub export_properties {
	my $self = shift;
	my $comment = shift;
	my $container = shift;

	my $fh = $self->{fh};
	print $fh "\n; $comment properties\n" if defined $comment;
	foreach my $p (@_) {
		my $format = format_for_number->{$p->{type}};
		if ($p->{type} eq 'f32') {
			print $fh "$p->{name} = $container->{$p->{name}}\n";
		} elsif (defined $format) {
die "undefined field $p->{name}\n" unless defined $container->{$p->{name}};
			next if defined($p->{default}) && $container->{$p->{name}} == $p->{default};
			printf $fh "$p->{name} = $format\n", $container->{$p->{name}};
		} elsif ($p->{type} eq 'sz') {
			next if defined($p->{default}) && $container->{$p->{name}} eq $p->{default};
			my $value = $container->{$p->{name}};
			if ($value =~ /\n/) {
				print $fh "$p->{name} = <<END\n$value\nEND\n";
			} else {
				print $fh "$p->{name} = $value\n";
			}
		} elsif ($p->{type} eq 'shape') {
			my @shapes;
			my $i = 0;
			foreach my $shape (@{$container->{$p->{name}}}) {
				push @shapes, "shape$i";
				$i++;
			}
			print $fh 'shapes = ', join(',', @shapes), "\n";
			$i = 0;
			foreach my $shape (@{$container->{$p->{name}}}) {
				my $id = "shape$i";
				if ($$shape{type} == 0) {
					print $fh "$id:type = sphere\n";
					print $fh "$id:offset = ", join(',', @{$$shape{sphere}}[0 .. 2]), "\n";
					print $fh "$id:radius = $$shape{sphere}[3]\n";
				} elsif ($$shape{type} == 1) {
					print $fh "$id:type = box\n";
					print $fh "$id:axis_x = ", join(',', @{$$shape{box}}[0 .. 2]), "\n";
					print $fh "$id:axis_y = ", join(',', @{$$shape{box}}[3 .. 5]), "\n";
					print $fh "$id:axis_z = ", join(',', @{$$shape{box}}[6 .. 8]), "\n";
					print $fh "$id:offset = ", join(',', @{$$shape{box}}[9 .. 11]), "\n";
				}
				$i++;
			}
		} else {
			next if defined(@{$p->{default}}) && @{$container->{$p->{name}}} == @{$p->{default}};
			print $fh "$p->{name} = ", join(',', @{$container->{$p->{name}}}), "\n";
		}
	}
}
sub import_properties {
	my $self = shift;
	my $section = shift;
	my $container = shift;

	die unless defined $self->{sections_hash}{$section};
	foreach my $p (@_) {
		my $value = $self->value($section, $p->{name});
		if ($p->{type} eq 'sz') {
			$container->{$p->{name}} = (defined $value) ? $value : $p->{default};
		} elsif (defined format_for_number->{$p->{type}}) {
			if (defined $value) {
				$value = hex($value) if ($value =~ /^\s*0x/);
				$container->{$p->{name}} = $value;
			} else {
				$container->{$p->{name}} = $p->{default};
			}
		} elsif ($p->{type} eq 'shape') {
			die unless defined $value;
			foreach my $id (split /,/, $value) {
				$id =~ s/^\s*|\s*$//g;
				my $shape = $self->import_shape_properties($section, $id);
				push @{$container->{$p->{name}}}, $shape;
			}
		} else {
			@{$container->{$p->{name}}} = defined $value ?
					split(/,/, $value) : @{$p->{default}};
		}
	}
}
sub import_shape_properties {
	my $self = shift;
	my ($section, $id) = @_;

	my %shape;

	my $type = $self->value($section, "$id:type") or die "no shape type in $section";
	my $offset = $self->value($section, "$id:offset");
	if ($type eq "sphere") {
		my $radius = $self->value($section, "$id:radius") or die "no radius in $section\n";
		$shape{type} = 0;
		@{$shape{sphere}} = (split(/,/, $offset), $radius);
	} elsif ($type eq "box") {
		$shape{type} = 1;
		my $axis_x = $self->value($section, "$id:axis_x") or die "no axis_x in $section\n";
		my $axis_y = $self->value($section, "$id:axis_y") or die "no axis_y in $section\n";
		my $axis_z = $self->value($section, "$id:axis_z") or die "no axis_z in $section\n";
		push @{$shape{box}}, split(/,/, $axis_x), split(/,/, $axis_y);
		push @{$shape{box}}, split(/,/, $axis_z), split(/,/, $offset);
	} else {
		die "unknown shape type in $section\n";
	}
	return \%shape;
}
sub value {
	my $self = shift;
	my ($section, $name) = @_;

	die unless defined $self->{sections_hash}{$section};
	return $self->{sections_hash}{$section}{$name};
}

1;
