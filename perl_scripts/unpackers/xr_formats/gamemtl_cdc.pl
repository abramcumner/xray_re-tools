#!perl -w -I \temp\1\bin
# TODO: 
# -добить флаги у материалов (пока непонятно, что за старший байт такой. Ну и еще с одним флагом непонятно)
# Last edited: March 25, 2011 16:24
#######################################################################
package chunked;
use strict;
use IO::File;
use stkutils::ini_file;
use stkutils::chunked_file;
use Cwd;
sub new {
	my $class = shift;
	my $self = {};
	$self->{material} = {};
	$self->{material_pair} = {};
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
		if ($index == 0x1000) {
			die unless $size == 2;
			$self->{materials_version} = unpack ('v', $cf->r_chunk_data());
		} elsif ($index == 0x1001) {
			die unless $size == 8;
			($self->{material_index},
			$self->{material_pair_index}) = unpack ('VV', $cf->r_chunk_data());
		} elsif ($index == 0x1002) {
			while (1) {
				($index, $size) = $cf->r_chunk_open();
				defined($index) or last;
				my $material = chunked->new();
				$material->read_material($cf);
				push @{$self->{materials}}, $material;
				$cf->r_chunk_close();
			}
		} elsif ($index == 0x1003) {
			while (1) {
				($index, $size) = $cf->r_chunk_open();
				defined($index) or last;
				my $material_pair = chunked->new();
				$material_pair->read_material_pair($cf);
				push @{$self->{material_pairs}}, $material_pair;
				$cf->r_chunk_close();
			}
		} else {
			die "unexpected global chunk - may be this file is not gamemtl.xr?\n";
		}
		$cf->r_chunk_close();
	}
	$cf->close();
}
sub write {
	my $self = shift;
	my ($fn) = @_;
	my $cf = stkutils::chunked_file->new($fn, 'w') or die;
	my $i = 0;
	$cf->w_chunk(4096, pack('v', $self->{materials_version}));
	$cf->w_chunk(4097, pack('VV', $self->{material_index}, $self->{material_pair_index}));
	
	my %id_by_mat = ();
	foreach my $material (@{$self->{materials}}) {
		$id_by_mat{$material->{material}->{name}} = $material->{material}->{id};
	}
	
	$cf->w_chunk_open(4098);
	foreach my $material (@{$self->{materials}}) {
		$cf->w_chunk_open($i);
		$material->write_material($cf);
		$cf->w_chunk_close();
		$i++;
	}
	$cf->w_chunk_close();
	
	my %parent_id_by_name = ();
	foreach my $material_pair (@{$self->{material_pairs}}) {
		my $id = join(' \ ', join('/', split('\W', $material_pair->{material_pair}->{mtl_0})), join('/', split('\W', $material_pair->{material_pair}->{mtl_1})));
		$parent_id_by_name{$id} = $material_pair->{material_pair}->{id};
	}
	
	$cf->w_chunk_open(4099);
	$i = 0;
	foreach my $material_pair (@{$self->{material_pairs}}) {
		$cf->w_chunk_open($i);
		my $mtl_0 = $id_by_mat{$material_pair->{material_pair}->{mtl_0}};
		my $mtl_1 = $id_by_mat{$material_pair->{material_pair}->{mtl_1}};
		my $parent_id = 0;
		if ($material_pair->{material_pair}->{parent} eq 'none') {
			$parent_id = 0xFFFFFFFF;
		} else {
			$parent_id = $parent_id_by_name{$material_pair->{material_pair}->{parent}};
		}
		$material_pair->write_material_pair($cf, $mtl_0, $mtl_1, $parent_id);
		$cf->w_chunk_close();
		$i++
	}
	$cf->w_chunk_close();
	$cf->close();
}
sub write_material {
	my $self = shift;
	my ($cf) = @_;
	
	$cf->w_chunk(0x1000, pack('VZ*', $self->{material}->{id}, $self->{material}->{name}));
	$cf->w_chunk(0x1005, pack('Z*', $self->{material}->{description})) if defined $self->{material}->{description};
	my @flags = split (', ', $self->{material}->{material_flags});
	my $flag_for_write = hex($self->{material}->{upper_byte}) << 16;
	foreach my $flag (@flags) {
		if ($flag eq 'mf_injurious') {
			$flag_for_write += 0x10000000;
		}
		if ($flag eq 'mf_shootable') {
			$flag_for_write += 0x20000000;
		}
		if ($flag eq 'mf_transparent') {
			$flag_for_write += 0x40000000;
		}
		if ($flag eq 'mf_slow_down') {
			$flag_for_write += 0x80000000;
		}
		if ($flag eq 'mf_breakable') {
			$flag_for_write += 0x01;
		}
		if ($flag eq 'mf_unknown_1') {
			$flag_for_write += 0x02;
		}
		if ($flag eq 'mf_bouncable') {
			$flag_for_write += 0x04;
		}
		if ($flag eq 'mf_skidmark') {
			$flag_for_write += 0x08;
		}
		if ($flag eq 'mf_bloodmark') {
			$flag_for_write += 0x10;
		}
		if ($flag eq 'mf_climable') {
			$flag_for_write += 0x20;
		}
		if ($flag eq 'mf_unknown_2') {
			$flag_for_write += 0x40;
		}
		if ($flag eq 'mf_passable') {
			$flag_for_write += 0x80;
		}
		if ($flag eq 'mf_dynamic') {
			$flag_for_write += 0x100;
		}
		if ($flag eq 'mf_liquid') {
			$flag_for_write += 0x200;
		}
		if ($flag eq 'mf_supress_shadow') {
			$flag_for_write += 0x400;
		}
		if ($flag eq 'mf_supress_walmarks') {
			$flag_for_write += 0x800;
		}
		if ($flag eq 'mf_actor_obstacle') {
			$flag_for_write += 0x1000;
		}
		if ($flag eq 'mf_bullet_no_ricoshet') {
			$flag_for_write += 0x2000;
		}
		if ($flag eq 'mf_unknown_3') {
			$flag_for_write += 0x4000;
		}
		if ($flag eq 'mf_unknown_4') {
			$flag_for_write += 0x8000;
		}
	}
	$cf->w_chunk(0x1001, pack('V', $flag_for_write));
	$cf->w_chunk(0x1002, pack('fffff', $self->{material}->{friction},	$self->{material}->{damping}, $self->{material}->{spring}, $self->{material}->{bounce_start_velocity}, $self->{material}->{bouncing}));
	$cf->w_chunk(0x1003, pack('ffff', $self->{material}->{shooting}, $self->{material}->{bouncing_damage}, $self->{material}->{transparency}, $self->{material}->{sound_occlusion}));
	$cf->w_chunk(0x1008, pack('f', $self->{material}->{shooting_mp})) if defined $self->{material}->{shooting_mp};
	$cf->w_chunk(0x1004, pack('f', $self->{material}->{flotation})) if defined $self->{material}->{flotation};
	$cf->w_chunk(0x1006, pack('f', $self->{material}->{injurious})) if defined $self->{material}->{injurious};
	$cf->w_chunk(0x1007, pack('f', $self->{material}->{density_factor})) if defined $self->{material}->{density_factor};;
}
sub write_material_pair {
	my $self = shift;
	my ($cf, $mtl_0, $mtl_1, $parent_id) = @_;
	
	my @props = split (', ', $self->{material_pair}->{own_props});
	my $own_props = 0;
	if ($parent_id != 0xFFFFFFFF) {
		foreach my $flag (@props) {
			if ($flag eq 'breaking_sounds') {
				$own_props += 0x02;
			}
			if ($flag eq 'step_sounds') {
				$own_props += 0x04;
			}
			if ($flag eq 'collide_sounds') {
				$own_props += 0x10;
			}
			if ($flag eq 'collide_particles') {
				$own_props += 0x20;
			}
			if ($flag eq 'collide_marks') {
				$own_props += 0x40;
			}
		} 
	} else {
		$own_props = 0xFFFFFFFF;
	}	
	$cf->w_chunk(4096, pack('VVVVV', $mtl_0, $mtl_1, $self->{material_pair}->{id}, $parent_id, $own_props));
	$cf->w_chunk(4097, pack('V', $self->{material_pair}->{build_1616_unk_1})) if defined $self->{material_pair}->{build_1616_unk_1};
	$cf->w_chunk(4098, pack('Z*', $self->{material_pair}->{breaking_sounds}));
	$cf->w_chunk(4099, pack('Z*', $self->{material_pair}->{step_sounds}));
	$cf->w_chunk(4100, pack('Z*', $self->{material_pair}->{build_1616_unk_2})) if defined $self->{material_pair}->{build_1616_unk_2};
	$cf->w_chunk(4101, pack('Z*Z*Z*', $self->{material_pair}->{collide_sounds}, $self->{material_pair}->{collide_particles}, $self->{material_pair}->{collide_marks}));
}
sub import {
	my $self = shift;

	my $all_file = stkutils::ini_file->new('gamemtl.ltx', 'r');
	$self->{materials_version} = $all_file->value('header', 'gamemtl_version');
	$self->{material_index} = $all_file->value('header', 'material_index');
	$self->{material_pair_index} = $all_file->value('header', 'material_pair_index');
	$all_file->close();
	
	$self->import_main('materials');
	$self->import_main('material_pairs');
}	
sub export {
	my $self = shift;
	
	my $all_file = IO::File->new('gamemtl.ltx', 'w');
	print $all_file "[header]\n";
	print $all_file "gamemtl_version = $self->{materials_version}\n";
	print $all_file "material_index = $self->{material_index}\n";
	print $all_file "material_pair_index = $self->{material_pair_index}\n";
	$all_file->close();
	
	my %mtl_by_id = ();
	my $work_dir = getcwd();
	foreach my $material (@{$self->{materials}}) {
		$mtl_by_id{$material->{material}->{id}} = $material->{material}->{name};
		my $string = $material->{material}->{name};
		my $out;
		my $material_name;
		my $inc_folder;
		if ($string =~ /^(\w+)\\(\w.*)/) {
			$out = $1;
			$material_name = $2;
		} else  {
			$material_name = $string;
		}
		chdir 'materials' or (File::Path::mkpath('materials', 0) and chdir ('materials')) or die "cannot make path to materials\n";
	
		if (defined $out) {
			chdir $out or (File::Path::mkpath($out, 0) and chdir ($out)) or die "cannot make path $out\n";
		}
		my $fh = ''.$material_name.'.ltx';
		my $bin_fh = IO::File->new($fh, 'w') or die "cannot open $fh\n";
		print $bin_fh "[general]\n";
		print $bin_fh "id = $material->{material}->{id}\n";
		if (defined $material->{material}->{description}) {; 					#added in build 1623
			print $bin_fh "description = ";
			my $str = substr($material->{material}->{description}, 0, -1);
			print $bin_fh "$str\n";
		}
		print $bin_fh "\n";
		print $bin_fh "[flags]\n";
		print $bin_fh "material_flags = ";
		my $flStr = '';
		foreach my $flag (keys %{$material->{material}->{flags}}) {
			if ($material->{material}->{flags}{$flag} == 1) {
				if ($flStr eq '') {
					$flStr = $flag;
				} else {
					$flStr = join(', ', $flStr, $flag);
				}
			}
		}
		print $bin_fh "$flStr\n\n";
		print $bin_fh "[physics]\n";
		printf $bin_fh "friction = %.5g\n", $material->{material}->{friction};
		printf $bin_fh "damping = %.5g\n", $material->{material}->{damping};
		printf $bin_fh "spring = %.5g\n", $material->{material}->{spring};
		printf $bin_fh "bounce_start_velocity = %.5g\n", $material->{material}->{bounce_start_velocity};
		printf $bin_fh "bouncing = %.5g\n\n", $material->{material}->{bouncing};
		print $bin_fh "[factors]\n";
		printf $bin_fh "shooting = %.5g\n", $material->{material}->{shooting};
		printf $bin_fh "bouncing_damage = %.5g\n", $material->{material}->{bouncing_damage};
		printf $bin_fh "transparency = %.5g\n", $material->{material}->{transparency};
		printf $bin_fh "sound_occlusion = %.5g\n", $material->{material}->{sound_occlusion};
		printf $bin_fh "shooting_mp = %.5g\n", $material->{material}->{shooting_mp} if defined $material->{material}->{shooting_mp}; #added in Call Of Pripyat
		printf $bin_fh "flotation = %.5g\n", $material->{material}->{flotation} if defined $material->{material}->{flotation}; #added in build 1623
		printf $bin_fh "injurious = %.5g\n", $material->{material}->{injurious} if defined $material->{material}->{injurious}; #added in build 2205
		printf $bin_fh "density_factor = %.5g\n\n", $material->{material}->{density_factor} if defined $material->{material}->{density_factor};  #added in Clear Sky
		chdir $work_dir or die;
	}
	print "exporting material pairs...\n";
	my %mtl_pair_by_id = ();
	foreach my $material_pair (@{$self->{material_pairs}}) {
		$mtl_pair_by_id{$material_pair->{material_pair}->{id}} = join(' \ ', join('/', split('\W', $mtl_by_id{$material_pair->{material_pair}->{mtl_1}})), join('/', split('\W', $mtl_by_id{$material_pair->{material_pair}->{mtl_2}})));
	}
	foreach my $material_pair (@{$self->{material_pairs}}) {
		chdir 'material_pairs' or (File::Path::mkpath('material_pairs', 0) and chdir ('material_pairs')) or die "cannot make path to material_pairs\n";
		my $mtl_1_name = join('-', split('\W', $mtl_by_id{$material_pair->{material_pair}->{mtl_1}}));
		my $mtl_2_name = join('-', split('\W', $mtl_by_id{$material_pair->{material_pair}->{mtl_2}}));
		chdir $mtl_1_name or (File::Path::mkpath($mtl_1_name, 0) and chdir ($mtl_1_name)) or die "cannot make path $mtl_1_name\n";
		my $fh = $mtl_2_name.'.ltx';
		my $bin_fh = IO::File->new($fh, 'w') or die "cannot open $fh\n";
		print $bin_fh "[general]\n";
		print $bin_fh "id = $material_pair->{material_pair}->{id}\n";
		print $bin_fh "build_1616_unk_1 = $material_pair->{material_pair}->{build_1616_unk_1}\n" if defined $material_pair->{material_pair}->{build_1616_unk_1};
		if ($material_pair->{material_pair}->{parent_id} == 0xFFFFFFFF) {
			print $bin_fh "parent = none\n";
		} else {
			print $bin_fh "parent = $mtl_pair_by_id{$material_pair->{material_pair}->{parent_id}}\n";
		}
		print $bin_fh "own_props = ";
		my $flStr = '';
		if ($material_pair->{material_pair}->{own_props} == 0xFFFFFFFF or $material_pair->{material_pair}->{own_props} == 0) {
			print $bin_fh "none\n\n";
		} else {
			foreach my $flag (keys %{$material_pair->{material_pair}->{flags}}) {
				if ($material_pair->{material_pair}->{flags}{$flag} == 1) {
					if ($flStr eq '') {
						$flStr = $flag;
					} else {
						$flStr = join(', ', $flStr, $flag);
					}
				}
			}
			print $bin_fh "$flStr\n\n";
		}
		print $bin_fh "[properties]\n";
		print $bin_fh "build_1616_unk_2 = $material_pair->{material_pair}->{build_1616_unk_2}\n" if defined $material_pair->{material_pair}->{build_1616_unk_2};
		print $bin_fh "breaking_sounds = $material_pair->{material_pair}->{breaking_sounds}\n";
		print $bin_fh "step_sounds = $material_pair->{material_pair}->{step_sounds}\n";
		print $bin_fh "collide_sounds = $material_pair->{material_pair}->{collide_sounds}\n";
		print $bin_fh "collide_particles = $material_pair->{material_pair}->{collide_particles}\n";
		print $bin_fh "collide_marks = $material_pair->{material_pair}->{collide_marks}\n\n";
		
		chdir $work_dir or die;
	}
}
sub read_material {
	my $self = shift;
	my ($cf) = @_;
	
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		if ($index == 0x1000) {
			($self->{material}->{id},
			$self->{material}->{name}) = unpack ('VZ*', $cf->r_chunk_data());
		} elsif ($index == 0x1005) {
			$self->{material}->{description} = $cf->r_chunk_data();
		} elsif ($index == 0x1001) {
			$size == 4 or die;
			my ($flags, $garbage) = unpack ('vv', $cf->r_chunk_data());	
			if (($garbage & 0xFF) != 0) {
				print "==================== WARNING! FOUND UNEXPECTED FLAGS! ====================\n";
				print "Please, send your gamemtl.xr to K.D. on gameru.net or KD87 on amk-team.ru\n";
				print "==========================================================================\n";
			}
			my $upper_flags = $garbage >> 8;
			if (($upper_flags & 0x10) != 0) {
				$self->{material}->{flags}{mf_injurious} = 1;
			}
			if (($upper_flags & 0x20) != 0) {
				$self->{material}->{flags}{mf_shootable} = 1;
			}
			if (($upper_flags & 0x40) != 0) {
				$self->{material}->{flags}{mf_transparent} = 1;
			}
			if (($upper_flags & 0x80) != 0) {
				$self->{material}->{flags}{mf_slow_down} = 1;
			}
			if (($flags & 0x1) != 0) {
				$self->{material}->{flags}{mf_breakable} = 1;
			}
			if (($flags & 0x2) != 0) {
				$self->{material}->{flags}{mf_unknown_1} = 1;
			}
			if (($flags & 0x4) != 0) {
				$self->{material}->{flags}{mf_bouncable} = 1;
			}
			if (($flags & 0x8) != 0) {
				$self->{material}->{flags}{mf_skidmark} = 1;
			}
			if (($flags & 0x10) != 0) {
				$self->{material}->{flags}{mf_bloodmark} = 1;
			}
			if (($flags & 0x20) != 0) {
				$self->{material}->{flags}{mf_climable} = 1;
			}
			if (($flags & 0x40) != 0) {
				$self->{material}->{flags}{mf_unknown_2} = 1;
			}
			if (($flags & 0x80) != 0) {
				$self->{material}->{flags}{mf_passable} = 1;
			}
			my $cutflags = $flags >> 8;
			if (($cutflags & 0x1) != 0) {
				$self->{material}->{flags}{mf_dynamic} = 1;
			}
			if (($cutflags & 0x2) != 0) {
				$self->{material}->{flags}{mf_liquid} = 1;
			}
			if (($cutflags & 0x4) != 0) {
				$self->{material}->{flags}{mf_supress_shadow} = 1;
			}
			if (($cutflags & 0x8) != 0) {
				$self->{material}->{flags}{mf_supress_walmarks} = 1;
			}
			if (($cutflags & 0x10) != 0) {
				$self->{material}->{flags}{mf_actor_obstacle} = 1;
			}
			if (($cutflags & 0x20) != 0) {
				$self->{material}->{flags}{mf_bullet_no_ricoshet} = 1;
			}
			if (($cutflags & 0x40) != 0) {
				$self->{material}->{flags}{mf_unknown_3} = 1;
			}
			if (($cutflags & 0x80) != 0) {
				$self->{material}->{flags}{mf_unknown_4} = 1;
			}
		} elsif ($index == 0x1002) {
			$size == 20 or die;
			($self->{material}->{friction},
			$self->{material}->{damping},
			$self->{material}->{spring},
			$self->{material}->{bounce_start_velocity},
			$self->{material}->{bouncing}) = unpack ('fffff', $cf->r_chunk_data());		
		} elsif ($index == 0x1003) {
			$size == 16 or die;
			($self->{material}->{shooting},
			$self->{material}->{bouncing_damage},
			$self->{material}->{transparency},
			$self->{material}->{sound_occlusion}) = unpack ('ffff', $cf->r_chunk_data());		
		} elsif ($index == 0x1008) {
			$size == 4 or die;
			$self->{material}->{shooting_mp} = unpack ('f', $cf->r_chunk_data());
		} elsif ($index == 0x1004) {
			$size == 4 or die;
			$self->{material}->{flotation} = unpack ('f', $cf->r_chunk_data());		
		} elsif ($index == 0x1006) {
			$size == 4 or die;
			$self->{material}->{injurious} = unpack ('f', $cf->r_chunk_data());		
		} elsif ($index == 0x1007) {
			$size == 4 or die;
			$self->{material}->{density_factor} = unpack ('f', $cf->r_chunk_data());
		} else {
			die "unexpected material chunk\n";
		}
		$cf->r_chunk_close();
	}
}
sub read_material_pair {
	my $self = shift;
	my ($cf) = @_;
	
	while (1) {
		my ($index, $size) = $cf->r_chunk_open();
		defined($index) or last;
		if ($index == 0x1000) {
			$size == 0x14 or die;
			($self->{material_pair}->{mtl_1},
			$self->{material_pair}->{mtl_2},
			$self->{material_pair}->{id}, 
			$self->{material_pair}->{parent_id}, 
			$self->{material_pair}->{own_props}) = unpack ('VVVVV', $cf->r_chunk_data());
			my $prFlags = $self->{material_pair}->{own_props};
			if (($prFlags & 0x2) != 0) {
				$self->{material_pair}->{flags}{breaking_sounds} = 1;
			}
			if (($prFlags & 0x4) != 0) {
				$self->{material_pair}->{flags}{step_sounds} = 1;
			}
			if (($prFlags & 0x10) != 0) {
				$self->{material_pair}->{flags}{collide_sounds} = 1;
			}
			if (($prFlags & 0x20) != 0) {
				$self->{material_pair}->{flags}{collide_particles} = 1;
			}
			if (($prFlags & 0x40) != 0) {
				$self->{material_pair}->{flags}{collide_marks} = 1;
			}
		} elsif ($index == 0x1001) { 														## support for build 1616 and older
			$size == 0x04 or die;															##
			$self->{material_pair}->{build_1616_unk_1} = unpack ('V', $cf->r_chunk_data()); ##
		} elsif ($index == 0x1002) {
			$self->{material_pair}->{breaking_sounds} = unpack('Z*', $cf->r_chunk_data());
		} elsif ($index == 0x1003) {
			$self->{material_pair}->{step_sounds} = unpack ('Z*', $cf->r_chunk_data());
		} elsif ($index == 0x1004) { 														 ## support for build 1616 and older
			$self->{material_pair}->{build_1616_unk_2} = unpack ('Z*', $cf->r_chunk_data()); ##
		} elsif ($index == 0x1005) {
			($self->{material_pair}->{collide_sounds},
			$self->{material_pair}->{collide_particles},
			$self->{material_pair}->{collide_marks}) = unpack ('Z*Z*Z*', $cf->r_chunk_data());	
		} else {
			die "unexpected material_pair chunk\n";
		}
		$cf->r_chunk_close();
	}
}
sub import_main {
	my $self = shift;
	my $type = shift;
	
	my $dir_handle;
	my $dir_handle_1;
	my $m_work_dir = getcwd();
	my $work_dir = $m_work_dir.'/'.$type;
	my $material_root;
	my $material_name;
	opendir $dir_handle, $work_dir or die 'line 554 : cannot open $work_dir';
	chdir $work_dir;
	my $i = 0;
	while ($material_root = readdir $dir_handle) {
		if ((-d $material_root) and ($i > 1)) {
			my $new_dir = $work_dir.'/'.$material_root.'';
			opendir $dir_handle_1, $new_dir or die 'line 559 : cannot open $new_dir';
			my $j = 0;
			while ($material_name = readdir $dir_handle_1) {
				if ((-d $material_root.'/'.$material_name) and ($j > 1)) {
					die "there is some catalogs left";
				} elsif (($j > 1) and (not($material_name eq 'gamemtl.ltx'))) {
					my $fn = $material_root.'/'.$material_name;
					my $material_file = stkutils::ini_file->new($fn, 'r') or die "line 566: cannot open $fn\n";
					my $material = chunked->new();
					if ($type eq 'materials') {
						$material->import_material($material_file);
						$material->{material}->{name} = $material_root.'\\'.substr($material_name, 0, -4);
						push @{$self->{materials}}, $material;
					} else {
						$material->import_material_pair($material_file);
						$material->{material_pair}->{mtl_0} = join('\\', split('-', $material_root));
						$material->{material_pair}->{mtl_1} = join('\\', split('-', substr($material_name, 0, -4)));
						push @{$self->{material_pairs}}, $material
					}
					$material_file->close();
				} else {
					$j++;
				}
			}
			closedir $dir_handle_1;
		} elsif (($i > 1) and (not($material_root eq 'gamemtl.ltx'))) {
			my $material_file = stkutils::ini_file->new($material_root, 'r') or die "line 585: cannot open $material_root\n";
			my $material = chunked->new();
			$material->import_material($material_file);
			$material->{material}->{name} = substr($material_root, 0, -4);
			push @{$self->{materials}}, $material;
			$material_file->close();
		} else {
			$i++;
		}
	}
	closedir $dir_handle;
	chdir ($m_work_dir);
}
sub import_material {
	my $self = shift;
	my ($cf) = shift;
	
	$self->{material}->{id} = $cf->value('general', 'id');
	$self->{material}->{description} = $cf->value('general', 'description');
	$self->{material}->{upper_byte} = $cf->value('flags', 'upper_byte');
	$self->{material}->{material_flags} = $cf->value('flags', 'material_flags');
	$self->{material}->{friction} = $cf->value('physics', 'friction');
	$self->{material}->{damping} = $cf->value('physics', 'damping');
	$self->{material}->{spring} = $cf->value('physics', 'spring');
	$self->{material}->{bounce_start_velocity} = $cf->value('physics', 'bounce_start_velocity');
	$self->{material}->{bouncing} = $cf->value('physics', 'bouncing');
	$self->{material}->{shooting} = $cf->value('factors', 'shooting');
	$self->{material}->{bouncing_damage} = $cf->value('factors', 'bouncing_damage');
	$self->{material}->{transparency} = $cf->value('factors', 'transparency');
	$self->{material}->{sound_occlusion} = $cf->value('factors', 'sound_occlusion');
	$self->{material}->{shooting_mp} = $cf->value('factors', 'shooting_mp');
	$self->{material}->{flotation} = $cf->value('factors', 'flotation');
	$self->{material}->{injurious} = $cf->value('factors', 'injurious');
	$self->{material}->{density_factor} = $cf->value('factors', 'density_factor');
}
sub import_material_pair {
	my $self = shift;
	my ($cf) = shift;
	
	$self->{material_pair}->{id} = $cf->value('general', 'id');
	$self->{material_pair}->{build_1616_unk_1} = $cf->value('general', 'build_1616_unk_1');
	$self->{material_pair}->{parent} = $cf->value('general', 'parent');
	$self->{material_pair}->{own_props} = $cf->value('general', 'own_props');
	$self->{material_pair}->{build_1616_unk_2} = $cf->value('properties', 'build_1616_unk_2');
	$self->{material_pair}->{breaking_sounds} = $cf->value('properties', 'breaking_sounds');
	$self->{material_pair}->{step_sounds} = $cf->value('properties', 'step_sounds');
	$self->{material_pair}->{collide_sounds} = $cf->value('properties', 'collide_sounds');
	$self->{material_pair}->{collide_particles} = $cf->value('properties', 'collide_particles');
	$self->{material_pair}->{collide_marks} = $cf->value('properties', 'collide_marks');
}
#######################################################################
package main;
use strict;
use Getopt::Long;
use File::Path;
use stkutils::data_packet;
use Cwd;

sub usage {
	return <<END
Gamemtl.xr compiiler/decompiler
Usage: gmcdc [-d input_file] [-o outdir]
       gmcdc [-c input_dir] [-o outfile]
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
print "exporting game materials...\n";	
	$file->export();	
	print "done!\n";
} else {
	die "bad params\n" if (defined $input_file);
	if (defined $src_dir) {
		chdir $src_dir or die "cannot change dir to $src_dir\n";
	}
	my $file = chunked->new();
	$out = 'gamemtl.xr.new' unless defined $out;
print "importing game materials...\n";
	$file->import();
print "writing $out...\n";
	chdir $work_dir;
	$file->write($out);
print "done!\n";
}
#######################################################################