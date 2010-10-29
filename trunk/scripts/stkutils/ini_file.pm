#author: bardak
#2010-04-30: XiaNi - added handling of parented sections and includes
#2010-05-06: XiaNi - comments handling "key = value  ; comment" and "[section]:parent ;comment"
#2010-05-06: XiaNi - added handling of multyple parenting "[section]: parent1, parent0"
#2010-05-06: XiaNi - sections to lower ... M_rat == m_rat
#2010-05-10: XiaNi - added base_path and ext_path

package stkutils::ini_file;

use strict;
use IO::File;
use File::Basename;

sub _include_ini {
	autoflush STDOUT 1;
	
	my ($self, $base_fn) = @_;
	my $fn = $self->{base_path}. $base_fn;
	my $path = $self->{base_path};
	if (defined $self->{ext_path} and (-e $self->{ext_path} . $base_fn))
	{
		$fn = $self->{ext_path} . $base_fn;
		$path = $self->{ext_path};
		#print "ini: reading '$fn'\n";
	}
	
	my $fh = IO::File->new($fn, 'r') or die "cannot include $fn\n";
	#print "ini: reading '$fn'\n";
	
	my $dirname = dirname($fn);
	$dirname = substr($dirname, length $path);
	defined $dirname or $dirname = "";
	$dirname ne "" and $dirname = $dirname."/" ;
	#print "dirname=" . $dirname. "\n";
	
	my $section;
	my $section_parent;
	my $skip_section = 0;
	while (<$fh>) {
		$section_parent = '';
		$_ =~ qr/^\s*;/ and next;
		if (/^\s*#include\s*"(\w.*)"/) {
			if (defined $1) {
				#print "include!:", $1, "\n";
				#$self = _include_ini($self, dirname($fn)."/".$1);
				$self = _include_ini($self, $dirname.$1);
				next;
			}
		}
		if (/^\[(.*)\]\s*:*\s*(\w.*)?/) {
			$section = lc($1);
			if (defined $2 && !::is_flag_defined($2)) {
				#$skip_section = 1;
				#next;
				$section_parent = lc($2);
				if ($section_parent =~ /^(.*?)\s*;(.+)$/) {
					my $comment;
					($section_parent, $comment) = ($1, $2);
				}
				$self->{sections_parents}{$section}=$section_parent;
			}
			
			die "dublicate section '$section' " if defined $self->{sections_hash}->{$section};
			push @{$self->{sections_list}}, $section;
			my %tmp = ();
			$self->{sections_hash}{$section} = \%tmp;
			$skip_section = 0;
			next;
		}
		if (/^([^=]*?)\s*=\s*(.*?)\s*$/) {
			my ($name, $value) = (lc($1), $2);
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
			else
			{
				if ($value =~ /^(.*?)\s*;(.+)$/) {
					my $comment;
					($value, $comment) = ($1, $2);
				}
			}
			$skip_section == 1 and next;
			die unless defined $section;
			$self->{sections_hash}{$section}{$name} = $value;
		}
	}
	$fh->close();
	return $self;
}

sub new {
	my $class = shift;
	my ($base_fn, $mode, $base_path, $ext_path) = @_;
	my $fh;
	my $fn;
	
	defined $base_path or $base_path = "";
	$fn = $base_path . $base_fn;
	
	if (defined $ext_path and (-e $ext_path . $base_fn))
	{
		$fn = $ext_path . $base_fn;
	}
	$fh = IO::File->new($fn, $mode) or die "cannot open $fn\n";
	
	my $self = {};
	$self->{fh} = $fh;
	$self->{sections_list} = [];
	$self->{sections_hash} = ();
	$self->{sections_parents} = ();
	$self->{base_path} = $base_path;
	$self->{ext_path} = $ext_path;
	bless($self, $class);

	$mode eq 'w' and return $self;
	
	$self = _include_ini($self, $base_fn);
	
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

	print $fh "; $comment properties\n" if defined $comment;
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
	print $fh "\n";
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
	$section = lc($section);
	$name = lc($name);
	
	die "section $section is not found... trying to get [$section][$name]\n" unless defined $self->{sections_hash}{$section};
	my $val = $self->{sections_hash}{$section}{$name};
	if (!defined $val)
	{
		if (defined $self->{sections_parents}{$section})
		{
			my $parent = $self->{sections_parents}{$section};
			if ($parent !~ /,/)
			{
				for ($parent) {
				s/^\s+//;
				s/\s+$//;
				}
				if (defined $self->{sections_hash}{$parent})
				{
					#$val = $self->{sections_hash}{$parent}{$name};
					$val = $self->value($parent, $name);
				}
				else
				{
					die "parent section '" .$parent. "' not found for '" .$section. "'\n";
				}
			}
			else
			{
				my @parents_list = split(/,/, $parent);
				@parents_list = reverse(@parents_list);
				foreach (@parents_list) {
					my $p2 = $_;
					for ($p2) {
					s/^\s+//;
					s/\s+$//;
					}
					
					if (defined $self->{sections_hash}{$p2})
					{
						$val = $self->value($p2, $name);
						defined $val and last;
					}
					else
					{
						die "parent section '" .$p2. "' not found for '" .$section. "'\n";
					}
				}
			}
		}
	}
	return $val;
}

sub section_exists()
{
	my $self = shift;
	my ($section) = @_;
	return (defined $self->{sections_hash}{lc($section)});
}

1;
