#!perl -w -I \temp\1\bin
#
#this module handles ogf models with OGF_format_version 4, from build 1893 to 3456+. Last Edit: 26.06.2011
###################################################################################################################
package stkutils::ogf;
use strict;
#use diagnostics;
use stkutils::debug;
use stkutils::data_packet;
use stkutils::chunked_file;
use POSIX qw(ctime);
use constant esmFX		=> 0x1;
use constant esmStopAtEnd	=> 0x2;
use constant esmNoMix		=> 0x4;
use constant esmSyncPart	=> 0x8;
use constant esmHasMotionMarks	=> 0x10;	# guessed name
# vertex constants
use constant OGF_VERTEXFORMAT_FVF_OLD	=> 0x112;
use constant OGF_VERTEXFORMAT_FVF_1L	=> 0x12071980;
use constant OGF_VERTEXFORMAT_FVF_2L	=> 0x240e3300; # skeletonX_pm
use constant OGF_VERTEXFORMAT_FVF_NL	=> 0x36154c80;
use constant OGF_VERTEXFORMAT_FVF_1_CS	=> 1;
use constant OGF_VERTEXFORMAT_FVF_2_CS	=> 2;
use constant OGF_VERTEXFORMAT_FVF_3_CS	=> 3;
use constant OGF_VERTEXFORMAT_FVF_4_CS	=> 4;
# loddef constants
use constant OGF3_HOPPE_HEADER	=> 1;
use constant OGF3_HOPPE_VERT_SPLITS	=> 2;
use constant OGF3_HOPPE_FIX_FACES	=> 3;
# motion constants
use constant KPF_T_PRESENT	=> 0x01;
use constant KPF_R_ABSENT	=> 0x02;
use constant KPF_T_HQ	=> 0x04;		
use constant S_POINTS	=> 0x01;
use constant S_NORMALS	=> 0x02;
use constant S_TEXCOORDS	=> 0x04;
use constant S_LIGHTMAPS	=> 0x08;
use constant S_INFLUENCES	=> 0x10;
use constant S_COLORS	=> 0x20;
use constant UINT32_MAX => 0xffffffff;
my %chunks_loaded = (
3 => {'OGF_HEADER'		=>	0x1,
	'OGF_TEXTURE'		=>	0x2,
	'OGF_TEXTURE_L'		=>	0x4,
	'OGF_CHILD_REFS'	=>	0x8,
	'OGF_BBOX'			=>	0x10,
	'OGF_VERTICES'		=>	0x20,
	'OGF_INDICES'		=>	0x40,
	'OGF_LODDATA'		=>	0x80,
	'OGF_VCONTAINER'	=>	0x100,
	'OGF_BSPHERE'		=>	0x200,
	'OGF_CHILDREN_L'	=>	0x400,
	'OGF_S_BONE_NAMES'	=>	0x800,
	'OGF_S_MOTIONS_0'	=>	0x1000,
	'OGF_DPATCH'		=>	0x2000,
	'OGF_S_LODS'		=>	0x4000,
	'OGF_CHILDREN'		=>	0x8000,
	'OGF_S_SMPARAMS_0'	=>	0x10000,
	'OGF_ICONTAINER'	=>	0x20000,
	'OGF_S_SMPARAMS_1'	=>	0x40000,
	'OGF_LODDEF2'		=>	0x80000,
	'OGF_TREEDEF2'		=>	0x100000,
	'OGF_S_IKDATA_0'	=>	0x200000,
	'OGF_S_USERDATA'	=>	0x400000,
	'OGF_S_IKDATA_1'	=>	0x800000,
	'OGF_S_MOTIONS_1'	=>	0x1000000,
	'OGF_S_DESC'		=>	0x2000000,
	'OGF_S_IKDATA_2'	=>	0x4000000,
	'OGF_S_MOTION_REFS_0'	=>	0x8000000},
4 => {'OGF_HEADER'		=>	0x1,
	'OGF_TEXTURE'		=>	0x2,
	'OGF_VERTICES'		=>	0x4,
	'OGF_INDICES'		=>	0x8,
	'OGF_P_MAP'			=>	0x10,
	'OGF_SWIDATA'		=>	0x20,
	'OGF_VCONTAINER'	=>	0x40,
	'OGF_ICONTAINER'	=>	0x80,
	'OGF_CHILDREN'		=>	0x100,
	'OGF_CHILDREN_L'	=>	0x200,
	'OGF_LODDEF2'		=>	0x400,
	'OGF_TREEDEF2'		=>	0x800,
	'OGF_S_BONE_NAMES'	=>	0x1000,
	'OGF_S_MOTIONS_1'		=>	0x2000,
	'OGF_S_SMPARAMS_1'	=>	0x4000,
	'OGF_S_IKDATA_2'	=>	0x8000,
	'OGF_S_USERDATA'	=>	0x10000,
	'OGF_S_DESC'		=>	0x20000,
	'OGF_S_MOTION_REFS_0'	=>	0x40000,
	'OGF_SWICONTAINER'	=>	0x80000,
	'OGF_GCONTAINER'	=>	0x100000,
	'OGF_FASTPATH'		=>	0x200000,
	'OGF_S_LODS'		=>	0x400000,
	'OGF_S_MOTION_REFS_1'	=>	0x800000,	
	'OGF_TEXTURE_L'		=>	0x1000000,
	'OGF_CHILD_REFS'	=>	0x2000000,
	'OGF_BBOX'			=>	0x4000000,
	'OGF_LODDATA'		=>	0x8000000,
	'OGF_BSPHERE'		=>	0x10000000,
	'OGF_DPATCH'		=>	0x20000000,
	'OGF_S_LODS_CSKY'	=>	0x40000000,},
);
my %mt_names = (		#names of appropriate engine classes
3 => {										
	0x0	=> 'MT_NORMAL',						#FVisual
	0x1	=> 'MT_HIERRARHY',					#FHierrarhyVisual
	0x2	=> 'MT_PROGRESSIVE',				#FProgressiveFixedVisual
	0x3	=> 'MT_SKELETON_GEOMDEF_PM',		#CSkeletonX_PM
	0x4	=> 'MT_SKELETON_ANIM',				#CKinematics						//CKinematicsAnimated since build 1510			
	0x6	=> 'MT_DETAIL_PATCH',				#FDetailPatch
	0x7	=> 'MT_SKELETON_GEOMDEF_ST',		#CSkeletonX_ST
	0x8	=> 'MT_CACHED',						#FCached
	0x9	=> 'MT_PARTICLE',					#CPSVisual	
	0xa	=> 'MT_PROGRESSIVE2',				#FProgressive
	0xb	=> 'MT_LOD',						#FLod
	0xc	=> 'MT_TREE',						#FTreeVisual
	0xd	=> 'MT_PARTICLE_EFFECT',			#PS::CParticleEffect, not used		//introduced in build 1510
	0xe	=> 'MT_PARTICLE_GROUP',				#PS::CParticleGroup, not used		//introduced in build 1510
	0xf	=> 'MT_SKELETON_RIGID',				#CKinematics						//introduced in build 1510
	},
4 => {										
	0x0	=> 'MT_NORMAL',						#FVisual
	0x1	=> 'MT_HIERRARHY',					#FHierrarhy_Visual
	0x2	=> 'MT_PROGRESSIVE',				#FProgressive
	0x3	=> 'MT_SKELETON_ANIM',				#CKinematics_Animated		#CSkeletonAnimated before 2205
	0x4	=> 'MT_SKELETON_GEOMDEF_PM',		#CSkeletonX_PM
	0x5	=> 'MT_SKELETON_GEOMDEF_ST',		#CSkeletonX_ST
	0x6	=> 'MT_LOD',						#FLod
	0x7	=> 'MT_TREE_ST',					#FTreeVisual_ST
	0x8	=> 'MT_PARTICLE_EFFECT',			#CParticleEffect, not used
	0x9	=> 'MT_PARTICLE_GROUP',				#CParticleGroup, not used
	0xa	=> 'MT_SKELETON_RIGID',				#CKinematics
	0xb	=> 'MT_TREE_PM',					#FTreeVisual_PM				#introduced in build 1957
	},
);
my %chunk_names = (
3 => {
	'OGF_HEADER'		=>	0x1,
	'OGF_TEXTURE'		=>	0x2,
	'OGF_TEXTURE_L'		=>	0x3,
	'OGF_CHILD_REFS'	=>	0x5,
	'OGF_BBOX'			=>	0x6,
	'OGF_VERTICES'		=>	0x7,
	'OGF_INDICES'		=>	0x8,
	'OGF_LODDATA'		=>	0x9,
	'OGF_VCONTAINER'	=>	0xa,
	'OGF_BSPHERE'		=>	0xb,
	'OGF_CHILDREN_L'	=>	0xc,
	'OGF_S_BONE_NAMES'	=>	0xd,
	'OGF_S_MOTIONS_0'	=>	0xe,
	'OGF_DPATCH'		=>	0xf,
	'OGF_S_LODS'		=>	0x10,
	'OGF_CHILDREN'		=>	0x11,
	'OGF_S_SMPARAMS_0'	=>	0x12,
	'OGF_ICONTAINER'	=>	0x13,
	'OGF_S_SMPARAMS_1'	=>	0x14,
	'OGF_LODDEF2'		=>	0x15,
	'OGF_TREEDEF2'		=>	0x16,
	'OGF_S_IKDATA_0'	=>	0x17,
	'OGF_S_USERDATA'	=>	0x18,
	'OGF_S_IKDATA_1'	=>	0x19,
	'OGF_S_MOTIONS_1'	=>	0x1A,
	'OGF_S_DESC'		=>	0x1B,
	'OGF_S_IKDATA_2'	=>	0x1C,
	'OGF_S_MOTION_REFS_0'	=>	0x1D,
	},
4 => {
	'OGF_HEADER'		=>	0x1,
	'OGF_TEXTURE'		=>	0x2,
	'OGF_VERTICES'		=>	0x3,
	'OGF_INDICES'		=>	0x4,
	'OGF_P_MAP'			=>	0x5,			#used before build 1925
	'OGF_SWIDATA'		=>	0x6,
	'OGF_VCONTAINER'	=>	0x7,			#used before build 2205
	'OGF_ICONTAINER'	=>	0x8,			#used before build 2205
	'OGF_CHILDREN'		=>	0x9,
	'OGF_CHILDREN_L'	=>	0xa,
	'OGF_LODDEF2'		=>	0xb,
	'OGF_TREEDEF2'		=>	0xc,
	'OGF_S_BONE_NAMES'	=>	0xd,
	'OGF_S_MOTIONS_1'	=>	0xe,			#used before build 2205
	'OGF_S_SMPARAMS_1'	=>	0xf,			#used before build 2205
	'OGF_S_IKDATA_2'	=>	0x10,
	'OGF_S_USERDATA'	=>	0x11,
	'OGF_S_DESC'		=>	0x12,
	'OGF_S_MOTION_REFS_0'	=>	0x13,
	'OGF_SWICONTAINER'	=>	0x14,			#introduced in build 1957
	'OGF_GCONTAINER'	=>	0x15,			#introduced in build 2205
	'OGF_FASTPATH'		=>	0x16,			#introduced in build 2205
	'OGF_S_LODS'		=>	0x17,			#introduced in build ????
	'OGF_S_MOTION_REFS_1'	=>	0x18,		#introduced in Clear Sky
	},
);
sub new {
	my $class = shift;
	my $self = {};
	$self->{bbox} = {};
	$self->{bsphere} = {};
	$self->{c_scale} = {};
	$self->{c_bias} = {};
	$self->{fast_swidata} = {};
	$self->{loaded_chunks} = 0;
	bless $self, $class;
	return $self;
}
sub read {
	my $self = shift;
	my ($cf) = @_;
	if (!($cf->find_chunk(0x1))) {
		stkutils::debug::fail(__PACKAGE__.'::read', __LINE__, '$cf->find_chunk(OGF_HEADER)', 'cannot find OGF_HEADER');
	}
	$self->read_header($cf);
	$cf->close_found_chunk();
	SWITCH: {
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_NORMAL' && do { $self->read_visual($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_HIERRARHY' && do { $self->read_hierrarhy_visual($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_PROGRESSIVE' && do { $self->read_progressive($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_ANIM' && do { $self->read_kinematics_animated($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_GEOMDEF_PM' && do { $self->read_skeletonx_pm($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_GEOMDEF_ST' && do { $self->read_skeletonx_st($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_PROGRESSIVE2' && do { $self->read_progressive2($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_LOD' && do { $self->read_lod($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_TREE' && do { $self->read_tree_visual_st($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_TREE_ST' && do { $self->read_tree_visual_st($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_RIGID' && do { $self->read_kinematics($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_TREE_PM' && do { $self->read_tree_visual_pm($cf); last SWITCH; };
		stkutils::debug::fail(__PACKAGE__.'::read', __LINE__, '', 'unexpected model type '.$self->{type});
	}	
}
sub read_header {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{format_version}, $self->{type}, $self->{shader_id}) = $packet->unpack('CCv');
	if ($self->{format_version} == 4) {
		$self->read_bbox($packet);
		$self->read_bsphere($packet);
	}
	$self->set_loaded('OGF_HEADER');
}
sub read_render_visual {
	my $self = shift;
	my ($cf) = @_;
	if ($self->{format_version} == 3) {
		if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_BBOX'})) {
			my $packet = stkutils::data_packet->new($cf->r_chunk_data());
			$self->read_bbox($packet);
			$cf->close_found_chunk();
		} else {
			stkutils::debug::fail(__PACKAGE__.'::read_render_visual', __LINE__, '$cf->find_chunk(OGF_BBOX)', 'cannot find OGF_BBOX');
		}
	}
	if ($self->{format_version} == 3 && $cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_BSPHERE'})) {
		my $packet = stkutils::data_packet->new($cf->r_chunk_data());
		$self->read_bsphere($packet);
		$cf->close_found_chunk();
	}	
	if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_S_DESC'})) {
		$self->read_s_desc($cf);
		$cf->close_found_chunk();
	}
	if ($self->{format_version} == 3 && $cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_TEXTURE_L'})) {
		$self->read_texture_l($cf);
		$cf->close_found_chunk();
	} elsif ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_TEXTURE'})) {
		$self->read_texture($cf);
		$cf->close_found_chunk();
	}
};
sub read_visual {
	my $self = shift;
	my ($cf) = @_;
	$self->read_render_visual($cf);
	if ($self->{format_version} == 4 && $cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_GCONTAINER'})) {
		$self->read_gcontainer($cf);
		$cf->close_found_chunk();
		if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_FASTPATH'})) {
			$self->read_fastpath($cf) ;
			$cf->close_found_chunk();
		}
	}
	if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_VCONTAINER'})) {
		$self->read_vcontainer($cf);
		$cf->close_found_chunk();
	} elsif ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_VERTICES'})) {
		$self->read_vertices($cf);
		$cf->close_found_chunk();
	}
	if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_ICONTAINER'})) {
		$self->read_icontainer($cf);
		$cf->close_found_chunk();
	} elsif ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_INDICES'})) {
		$self->read_indices($cf);
		$cf->close_found_chunk();
	}
}
sub read_hierrarhy_visual {
	my $self = shift;
	my ($cf) = @_;
	$self->read_render_visual($cf);
	if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_CHILDREN_L'})) {
		$self->read_children_l($cf);
	} elsif ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_CHILDREN'})) {
		$self->read_children($cf);
	} elsif ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_CHILD_REFS'})) {
		$self->read_child_refs($cf);		
	} else {
		stkutils::debug::fail(__PACKAGE__.'::read_hierrarhy_visual', __LINE__, '', 'Invalid visual, no children');
	}
	$cf->close_found_chunk();
};
sub read_progressive {
	my $self = shift;
	my ($cf) = @_;
	$self->read_visual($cf);
	if ($self->{format_version} == 4 && $cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_SWIDATA'})) {
		$self->read_swidata($cf);
		$cf->close_found_chunk();
	} else {
		if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_LODDATA'})) {
			$self->read_loddata($cf);
			$cf->close_found_chunk();
		} else {
			stkutils::debug::fail(__PACKAGE__.'::read_progressive', __LINE__, '', 'Invalid visual, no loddata');
		}
	}
}
sub read_kinematics {
	my $self = shift;
	my ($cf) = @_;
	$self->read_hierrarhy_visual($cf);
	if ($self->{format_version} == 4) {
		my $size = $cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_S_LODS'});
		if ($size) {
			if ($size < 0x100) {
				$self->read_s_lods_csky($cf);
			} else {
				$self->read_s_lods($cf);
			}
			$cf->close_found_chunk();
		}
	}
	if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_S_USERDATA'})) {
		$self->read_s_userdata($cf);
		$cf->close_found_chunk();
	}
	if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_S_BONE_NAMES'})) {
		$self->read_s_bone_names($cf);
		$cf->close_found_chunk();
	} else {
		stkutils::debug::fail(__PACKAGE__.'::read_kinematics', __LINE__, '$cf->find_chunk('.$chunk_names{$self->{format_version}}{'OGF_S_BONE_NAMES'}.')', 'cannot find OGF_S_BONE_NAMES');
	}
	if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_S_IKDATA_2'})) {
		$self->read_s_ikdata($cf, 2);
		$cf->close_found_chunk();
	} elsif ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_S_IKDATA_1'})) {
		$self->read_s_ikdata($cf, 1);
		$cf->close_found_chunk();
	} elsif ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_S_IKDATA_0'})) {
		stkutils::debug::fail(__PACKAGE__.'::read_kinematics', __LINE__, '', 'sorry, ik data of builds 15xx not implemented yet');
		$self->read_s_ikdata($cf, 0);
		$cf->close_found_chunk();
	}	
}
sub read_kinematics_animated {
	my $self = shift;
	my ($cf) = @_;
	$self->read_kinematics($cf);
	if ($self->{format_version} == 4 && $cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_S_MOTION_REFS_1'})) {
		$self->read_smotion_refs_1($cf);
		$cf->close_found_chunk();
		return;
	} elsif ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_S_MOTION_REFS_0'})) {
		$self->read_smotion_refs_0($cf);
		$cf->close_found_chunk();
		return;
	} elsif ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_S_SMPARAMS_1'})) {
		$self->read_s_smparams($cf, 1);
		$cf->close_found_chunk();
	} elsif ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_S_SMPARAMS_0'})) {
		$self->read_s_smparams($cf, 0);
		$cf->close_found_chunk();
	}
	if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_S_MOTIONS_1'})) {
		$self->read_smotions($cf, 1);
		$cf->close_found_chunk();
	} elsif ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_S_MOTIONS_0'})) {
		$self->read_smotions($cf, 0);
		$cf->close_found_chunk();
	} else {
		stkutils::debug::fail(__PACKAGE__.'::read_kinematics_animated', __LINE__, '', 'Invalid visual, no motions');
	}
}
sub read_skeletonx {
	my $self = shift;
	my ($cf) = @_;
	if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_VERTICES'})) {
		$cf->close_found_chunk();
		return;
	} else {
		stkutils::debug::fail(__PACKAGE__.'::read_skeletonx', __LINE__, '$cf->find_chunk('.$chunk_names{$self->{format_version}}{'OGF_VERTICES'}.')', 'cannot find OGF_VERTICES');
	}
}
sub read_skeletonx_pm {
	my $self = shift;
	my ($cf) = @_;
	$self->read_skeletonx($cf);
	$self->read_progressive($cf);
}
sub read_skeletonx_st {
	my $self = shift;
	my ($cf) = @_;
	$self->read_skeletonx($cf);
	$self->read_visual($cf);
}
sub read_progressive2 {
	my $self = shift;
	my ($cf) = @_;
	$self->read_render_visual($cf);
	if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_LODS'})) {
		$self->read_s_lods($cf);
		$cf->close_found_chunk();
	} else {
		stkutils::debug::fail('ogf::read_progressive', 417, 'Invalid visual, no lods');
	}
}
sub read_lod {
	my $self = shift;
	my ($cf) = @_;
	$self->read_hierrarhy_visual($cf);
	if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_LODDEF2'})) {
		$self->read_loddef2($cf);
		$cf->close_found_chunk();
	} else {
		stkutils::debug::fail('ogf::read_lod', 428, 'cannot find chunk OGF_LODDEF2');
	}
}
sub read_tree_visual {
	my $self = shift;
	my ($cf) = @_;
	$self->read_visual($cf);
	if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_TREEDEF2'})) {
		$self->read_treedef2($cf);
		$cf->close_found_chunk();
	} else {
		stkutils::debug::fail(__PACKAGE__.'::read_tree_visual', __LINE__, '!$cf->find_chunk(OGF_TREEDEF2)', 'cannot find OGF_TREEDEF2');
	}
}
sub read_tree_visual_st {
	my $self = shift;
	my ($cf) = @_;
	$self->read_tree_visual($cf);
}
sub read_tree_visual_pm {
	my $self = shift;
	my ($cf) = @_;
	$self->read_tree_visual($cf);
	if ($cf->find_chunk($chunk_names{$self->{format_version}}{'OGF_SWICONTAINER'})) {
		$self->read_swicontainer($cf);
		$cf->close_found_chunk();
	} else {
		stkutils::debug::fail('ogf::read_tree_visual_pm', 457, 'cannot find OGF_SWICONTAINER');
	}
}
sub read_bbox {
	my $self = shift;
	my ($packet) = @_;
	@{$self->{bbox}->{min}} = $packet->unpack('f3');
	@{$self->{bbox}->{max}} = $packet->unpack('f3');
	$self->set_loaded('OGF_BBOX');
}
sub read_bsphere {
	my $self = shift;
	my ($packet) = @_;
	@{$self->{bsphere}->{c}} = $packet->unpack('f3');
	($self->{bsphere}->{r}) = $packet->unpack('f');
	$self->set_loaded('OGF_BSPHERE');
}
sub read_s_desc {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{ogf_object}, $self->{ogf_creator},
	 $self->{unk},
	 $self->{creator}, $self->{create_time},
	 $self->{editor}, $self->{edit_time}) = $packet->unpack('Z*Z*VZ*VZ*V');
	stkutils::debug::fail('ogf::read_s_desc', 486, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
	$self->set_loaded('OGF_S_DESC');
}
sub read_texture_l {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{texture_id}, $self->{shader_id}) = $packet->unpack('VV');
	$self->set_loaded('OGF_TEXTURE_L');
	stkutils::debug::fail('ogf::read_texture_l', 495, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
}
sub read_texture {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{texture_name}, $self->{shader_name}) = $packet->unpack('Z*Z*');
	$self->set_loaded('OGF_TEXTURE');
	stkutils::debug::fail('ogf::read_texture', __LINE__, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
}
sub read_gcontainer {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{ext_vb_index}, $self->{ext_vb_offset}, $self->{ext_vb_size}, 
	$self->{ext_ib_index}, $self->{ext_ib_offset}, $self->{ext_ib_size}) = $packet->unpack('VVVVVV');
	$self->set_loaded('OGF_GCONTAINER');
}
sub read_fastpath {
	my $self = shift;
	my ($cf) = @_;
	$self->{m_fast} = $cf->r_chunk_data();
#	$cf->find_chunk(0x15);
#	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
#	@{$self->{fast_gcontainer}} = $packet->unpack('V6');
#	$cf->close_found_chunk();
#	if ($cf->find_chunk(0x6)) {
#		read_swidata($self->{fast_swidata}, $cf);
#		$cf->close_found_chunk();
#	}
	$self->set_loaded('OGF_FASTPATH');
}
sub read_vcontainer {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{ext_vb_index}, $self->{ext_vb_offset}, $self->{ext_vb_size}) = $packet->unpack('VVV');
	$self->set_loaded('OGF_VCONTAINER');
}
sub read_vertices {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data(8));
	($self->{vertex_format}, $self->{vertex_count}) = $packet->unpack('VV');
	if ($self->{vertex_format} == OGF_VERTEXFORMAT_FVF_OLD) {
		for (my $i = 0; $i < $self->{vertex_count}; $i++) {
			my $packet = stkutils::data_packet->new($cf->r_chunk_data(32));
			my $vertice = {};
			@{$vertice->{point}} = $packet->unpack('f3');
			@{$vertice->{normal}} = $packet->unpack('f3');
			@{$vertice->{textcoords}} = $packet->unpack('f2');
			push @{$self->{vertices}}, $vertice;
		}	
	} elsif ($self->{format_version} == 3 && $self->{vertex_format} == OGF_VERTEXFORMAT_FVF_1L) {
		for (my $i = 0; $i < $self->{vertex_count}; $i++) {
			my $packet = stkutils::data_packet->new($cf->r_chunk_data(36));
			my $vertice = {};
			@{$vertice->{point}} = $packet->unpack('f3');
			@{$vertice->{normal}} = $packet->unpack('f3');
			@{$vertice->{textcoords}} = $packet->unpack('f2');
			($vertice->{matrix}) = $packet->unpack('V');
			push @{$self->{vertices}}, $vertice;
		}
	} elsif ($self->{vertex_format} == OGF_VERTEXFORMAT_FVF_1L or $self->{vertex_format} == OGF_VERTEXFORMAT_FVF_1_CS) {
		for (my $i = 0; $i < $self->{vertex_count}; $i++) {
			my $packet = stkutils::data_packet->new($cf->r_chunk_data(60));
			my $vertice = {};
			@{$vertice->{point}} = $packet->unpack('f3');
			@{$vertice->{normal}} = $packet->unpack('f3');
			@{$vertice->{t}} = $packet->unpack('f3');
			@{$vertice->{b}} = $packet->unpack('f3');
			@{$vertice->{textcoords}} = $packet->unpack('f2');
			($vertice->{matrix}) = $packet->unpack('V');
			push @{$self->{vertices}}, $vertice;
		}
	} elsif ($self->{vertex_format} == OGF_VERTEXFORMAT_FVF_2L or $self->{vertex_format} == OGF_VERTEXFORMAT_FVF_2_CS) {
		for (my $i = 0; $i < $self->{vertex_count}; $i++) {
			my $packet = stkutils::data_packet->new($cf->r_chunk_data(64));
			my $vertice = {};
			($vertice->{matrix0}, $vertice->{matrix1}) = $packet->unpack('vv');
			@{$vertice->{point}} = $packet->unpack('f3');
			@{$vertice->{normal}} = $packet->unpack('f3');
			@{$vertice->{t}} = $packet->unpack('f3');
			@{$vertice->{b}} = $packet->unpack('f3');
			($vertice->{w}) = $packet->unpack('f');
			@{$vertice->{textcoords}} = $packet->unpack('f2');
			push @{$self->{vertices}}, $vertice;
		}
	} elsif ($self->{vertex_format} == OGF_VERTEXFORMAT_FVF_3_CS) {
		for (my $i = 0; $i < $self->{vertex_count}; $i++) {
			my $packet = stkutils::data_packet->new($cf->r_chunk_data(70));
			my $vertice = {};
			($vertice->{matrix0}, $vertice->{matrix1}, $vertice->{matrix2}) = $packet->unpack('vvv');
			@{$vertice->{point}} = $packet->unpack('f3');
			@{$vertice->{normal}} = $packet->unpack('f3');
			@{$vertice->{t}} = $packet->unpack('f3');
			@{$vertice->{b}} = $packet->unpack('f3');
			($vertice->{w0}, $vertice->{w1}) = $packet->unpack('ff');
			@{$vertice->{textcoords}} = $packet->unpack('f2');
			push @{$self->{vertices}}, $vertice;
		}
	} elsif ($self->{vertex_format} == OGF_VERTEXFORMAT_FVF_4_CS) {
		for (my $i = 0; $i < $self->{vertex_count}; $i++) {
			my $packet = stkutils::data_packet->new($cf->r_chunk_data(76));
			my $vertice = {};
			($vertice->{matrix0}, $vertice->{matrix1}, $vertice->{matrix2}, $vertice->{matrix3}) = $packet->unpack('vvvv');
			@{$vertice->{point}} = $packet->unpack('f3');
			@{$vertice->{normal}} = $packet->unpack('f3');
			@{$vertice->{t}} = $packet->unpack('f3');
			@{$vertice->{b}} = $packet->unpack('f3');
			($vertice->{w0}, $vertice->{w1}, $vertice->{w2}) = $packet->unpack('fff');
			@{$vertice->{textcoords}} = $packet->unpack('f2');
			push @{$self->{vertices}}, $vertice;
		}
	} else {
		stkutils::debug::fail('ogf::read_vertices', 605, 'unsupported FVF');
	}
	stkutils::debug::fail('ogf::read_vertices', 607, 'there some data in packet left: '.($cf->{end_offsets}[$#{$cf->{end_offsets}}] - $cf->{offset})) unless $cf->{offset} == $cf->{end_offsets}[$#{$cf->{end_offsets}}];
	$self->set_loaded('OGF_VERTICES');
}
sub read_icontainer {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{ext_ib_index}, $self->{ext_ib_offset}, $self->{ext_ib_size}) = $packet->unpack('VVV');
	$self->set_loaded('OGF_ICONTAINER');
}
sub read_indices {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data(4));
	($self->{indices_count}) = $packet->unpack('V');
	for (my $i = 0; $i < $self->{indices_count}; $i++) {
		my $packet = stkutils::data_packet->new($cf->r_chunk_data(2));
		my ($index) = $packet->unpack('v');
		push @{$self->{indices}}, $index;
	}
	stkutils::debug::fail('ogf::read_indices', 627, 'there some data in packet left: '.($cf->{end_offsets}[$#{$cf->{end_offsets}}] - $cf->{offset})) unless $cf->{offset} == $cf->{end_offsets}[$#{$cf->{end_offsets}}];
	$self->set_loaded('OGF_INDICES');
}
sub read_children_l {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	@{$self->{children_l}} = $packet->unpack('V/V');
	stkutils::debug::fail('ogf::read_children_l', 635, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
	$self->set_loaded('OGF_CHILDREN_L');
}
sub read_children {
	my $self = shift;
	my ($cf) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($nnnn, $size) = $cf->r_chunk_open();
		defined $nnnn or last;
		if ($nnnn != $expected_id) {
			stkutils::debug::fail('ogf::read_children', 645, 'unexpected chunk $nnnn');
		}
		my $child = stkutils::ogf->new();
		$child->read($cf);
		$cf->r_chunk_close();
		push @{$self->{children}}, $child;
	}
	$self->set_loaded('OGF_CHILDREN');
}
sub read_child_refs {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	for (my $i = 0; $i < $count; $i++) {
		my ($ref) = $packet->unpack('Z*');
		push @{$self->{child_refs}}, $ref;
	}
	stkutils::debug::fail('ogf::read_child_refs', 664, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
	$self->set_loaded('OGF_CHILD_REFS');
}
sub read_swidata {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	@{$self->{swi_reserved}} = $packet->unpack('V4');
	($self->{swi_count}) = $packet->unpack('V');
	for (my $i = 0; $i < $self->{swi_count}; $i++) {
		my $swi = {};
		($swi->{offset}, 
		$swi->{num_tris}, 
		$swi->{num_verts}) = $packet->unpack('lvv');
		push @{$self->{swi_data}}, $swi;
	}
	stkutils::debug::fail('ogf::read_swidata', 675, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
	set_loaded($self, 'OGF_SWIDATA');
}
sub read_loddata {
	my $self = shift;
	my ($cf) = @_;
	my $loddata = {};
	while (1) {
		my ($id, $size) = $cf->r_chunk_open();
		defined $id or last;
			SWITCH: {
				$id == OGF3_HOPPE_HEADER && do { read_hoppe_header($loddata, $cf); last SWITCH; };
				$id == OGF3_HOPPE_VERT_SPLITS && do {read_hoppe_vertsplits($loddata, $self, $cf); last SWITCH; };
				$id == OGF3_HOPPE_FIX_FACES && do {read_hoppe_fix_faces($loddata, $cf); last SWITCH; };
				stkutils::debug::fail('ogf::read_loddata', 694, "unexpected chunk $id");
			}			
		$cf->r_chunk_close();
	}
	$cf->r_chunk_close();
	push @{$self->{loddata}}, $loddata;
	$self->set_loaded('OGF_LODDATA');
}
sub read_hoppe_header {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{min_vertices}, $self->{max_vertices}) = $packet->unpack('VV');
}
sub read_hoppe_vertsplits {
	my $self = shift;
	my ($global, $cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	$self->{num_vertsplits} = $global->{vertex_count} - $self->{min_vertices};
	for (my $i = 0; $i < $self->{num_vertsplits}; $i++) {	
		my $split = {};
		($split->{vert}, $split->{num_tris}, $split->{num_verts}) = $packet->unpack('vCC');
		push @{$self->{vertsplits}}, $split;
	}
}
sub read_hoppe_fix_faces {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{num_fix_faces}) = $packet->unpack('V');
	@{$self->{fix_faces}} = $packet->unpack("(v)$self->{num_fix_faces}");
}
sub read_s_lods_csky {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{s_lods_ref}) = $packet->unpack('Z*');
	$self->set_loaded('OGF_S_LODS_CSKY');
}
sub read_s_lods {
	my $self = shift;
	my ($cf) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($nnnn, $size) = $cf->r_chunk_open();
		defined $nnnn or last;
		if ($nnnn != $expected_id) {
			stkutils::debug::fail('ogf::read_s_lods', 741, 'unexpected chunk $nnnn');
		}
		my $lod = stkutils::ogf->new();
		$lod->read($cf);
		$cf->r_chunk_close();
		push @{$self->{s_lods}}, $lod;
	}
	$self->set_loaded('OGF_S_LODS');		
}
sub read_s_userdata {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my $n = $packet->length();
	($self->{userdata}) = $packet->unpack("a$n");
	stkutils::debug::fail('ogf::read_s_userdata', 757, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
	$self->set_loaded('OGF_S_USERDATA');
}
sub read_s_bone_names {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	my ($count) = $packet->unpack('V');
	for (my $i = 0; $i < $count; $i++) {
		my $bone_name_obj = {};
		($bone_name_obj->{name}, $bone_name_obj->{parent_name}) = $packet->unpack('Z*Z*');
		read_obb($bone_name_obj, $packet);
		push @{$self->{bone_names}}, $bone_name_obj;
	}
	$self->set_loaded('OGF_S_BONE_NAMES');
	stkutils::debug::fail('ogf::read_s_bone_names', 772, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
}
sub read_obb {
	my $self = $_[0];
	@{$self->{rotate}} = $_[1]->unpack('f9');
	@{$self->{translate}} = $_[1]->unpack('f3');
	@{$self->{halfsize}} = $_[1]->unpack('f3');
}
sub read_s_ikdata {
#loc_44C45A:
#mov     ecx, [esi+4]
#mov     edx, [esp+1B8h+var_1AC]
#mov     esi, [ecx+edx*4]
#lea     eax, [esi+0E8h]
#push    eax
#mov     ecx, edi
#call    ds:?r_stringZ@IReader@@QAEXPAD@Z ; IReader::r_stringZ(char *)
#push    70h
#lea     ecx, [esi+78h]
#push    ecx
#mov     ecx, edi
#call    ds:?r@IReader@@QAEXPAXH@Z ; IReader::r(void *,int)
#push    3Ch
#lea     edx, [esi+128h]
#push    edx
#mov     ecx, edi
#call    ds:?r@IReader@@QAEXPAXH@Z ; IReader::r(void *,int)
#push    0Ch
#lea     eax, [esi+60h]
#push    eax
#mov     ecx, edi
#call    ds:?r@IReader@@QAEXPAXH@Z ; IReader::r(void *,int)
#push    0Ch
#lea     ecx, [esi+6Ch]
#push    ecx
#mov     ecx, edi
#call    ds:?r@IReader@@QAEXPAXH@Z ; IReader::r(void *,int)
#push    4
#lea     edx, [esp+1BCh+var_190]
#push    edx
#mov     ecx, edi
#call    ds:?r@IReader@@QAEXPAXH@Z ; IReader::r(void *,int)
#mov     eax, [esp+1B8h+var_190]
#mov     [esi+164h], eax
#push    0Ch
#add     esi, 168h
#push    esi
#mov     ecx, edi
#call    ds:?r@IReader@@QAEXPAXH@Z ; IReader::r(void *,int)
#add     [esp+1B8h+var_1AC], 1
#mov     edx, [esp+1B8h+var_1AC]
#jmp     loc_44C410
	my $self = shift;
	my ($cf, $mode) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	while ($packet->length()) {
		my $ik = {};
		$ik->{bone_shape} = {};
		$ik->{joint_data} = {};
		if ($mode == 2) {
			($ik->{version}, $ik->{game_mtl_name}) = $packet->unpack('VZ*');
		} else {
			($ik->{game_mtl_name}) = $packet->unpack('Z*');
		}
		read_s_bone_shape($ik->{bone_shape}, $packet);
		read_s_joint_ik_data($ik->{joint_data}, $packet, $ik->{version});
		@{$ik->{bind_rotation}} = $packet->unpack('f3');
		@{$ik->{bind_position}} = $packet->unpack('f3');
		($ik->{mass}, @{$ik->{center_of_mass}}) = $packet->unpack('ff3');
		push @{$self->{ik_data}}, $ik;
	}
	$self->set_loaded('OGF_S_IKDATA_'.$mode);
	stkutils::debug::fail('ogf::read_s_ikdata', 801, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
}
sub read_s_bone_shape {
	my $self = $_[0];
	($self->{type}, $self->{flags}) = $_[1]->unpack('vv');
	$self->{box} = {};
	$self->{sphere} = {};
	$self->{cylinder} = {};
	read_obb($self->{box}, $_[1]);
	read_sphere($self->{sphere}, $_[1]);
	read_cylinder($self->{cylinder}, $_[1]);
}
sub read_sphere {
	my $self = $_[0];
	@{$self->{p}} = $_[1]->unpack('f3');
	($self->{r}) = $_[1]->unpack('f');
}
sub read_cylinder {
	my $self = $_[0];
	@{$self->{center}} = $_[1]->unpack('f3');
	@{$self->{direction}} = $_[1]->unpack('f3');
	($self->{height}) = $_[1]->unpack('f');
	($self->{radius}) = $_[1]->unpack('f');
}
sub read_s_joint_ik_data {
	my $self = shift;
	my ($packet, $version) = @_;
	($self->{type}) = $packet->unpack('V');
	@{$self->{limits}} = [];
	${$self->{limits}}[0] = {};
	read_s_joint_limit(${$self->{limits}}[0], $packet);
	${$self->{limits}}[1] = {};
	read_s_joint_limit(${$self->{limits}}[1], $packet);
	${$self->{limits}}[2] = {};
	read_s_joint_limit(${$self->{limits}}[2], $packet);
	($self->{spring_factor},
	 $self->{damping_factor},
	 $self->{ik_flags},
	 $self->{break_force},
	 $self->{break_torque}) = $packet->unpack('ffVff');
	($self->{friction}) = $packet->unpack('f') if defined $version;
}
sub read_s_joint_limit {
	my $self = $_[0];
	@{$self->{limit}} = $_[1]->unpack('f2');
	($self->{spring_factor}, $self->{damping_factor}) = $_[1]->unpack('ff');
}
sub read_smotion_refs_1 {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	@{$self->{motion_refs_1}} = $packet->unpack('V/(Z*)');
	stkutils::debug::fail('ogf::read_smotion_refs_1', 853, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
	$self->set_loaded('OGF_S_MOTION_REFS_1');
}
sub read_smotion_refs_0 {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{motion_refs_0}) = $packet->unpack('Z*');
	stkutils::debug::fail('ogf::read_smotion_refs_0', 854, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
	$self->set_loaded('OGF_S_MOTION_REFS_0');
}
sub read_s_smparams {
	my $self = shift;
	my ($cf, $mode) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{sm_params_version}) = $packet->unpack('v') if $mode == 1;  ### build 1865 - exists!!!
	my ($partition_count) = $packet->unpack('v');
	for (my $i = 0; $i < $partition_count; $i++) {
		my $part = {};
		($part->{name}, $part->{bone_count}) = $packet->unpack('Z*v');
		for (my $i = 0; $i < $part->{bone_count}; $i++) {
			my $bone = {};
			if ($mode == 0 || $self->{sm_params_version} == 1) {
				($bone->{bone_id}) = $packet->unpack('V');
			} elsif ($self->{sm_params_version} == 2) {
				($bone->{bone_name}) = $packet->unpack('Z*');
			} elsif ($self->{sm_params_version} == 3 || $self->{sm_params_version} == 4) {	
				($bone->{bone_name}, $bone->{bone_id}) = $packet->unpack('Z*V');
			}
			push @{$part->{bones}}, $bone;
		}
		push @{$self->{s_smparams_partitions}}, $part;
	}
	my ($motion_count) = $packet->unpack('v');
	for (my $i = 0; $i < $motion_count; $i++) {
		my $mot = {};
		if ($mode == 1) {
			($mot->{name}, $mot->{flags}) = $packet->unpack('Z*V');
			read_motion_def($mot, $packet);
			if ($self->{sm_params_version} == 4) {
				my ($num_marks) = $packet->unpack('V');
				for (my $j = 0; $j < $num_marks; ++$j) {
					my $mmark = {};
					read_motion_mark($mmark, $packet);
					push @{$mot->{mmarks}}, $mmark;
				}
			}
		} else {
			($mot->{name}, $mot->{flags}) = $packet->unpack('Z*C');
			read_motion_def($mot, $packet);
			my $flag = $packet->unpack('C');
			$mot->{flags} += 0x2 if $flag != 1;
		}
		push @{$self->{s_smparams_motions}}, $mot;
	}
	stkutils::debug::fail('ogf::read_s_smparams', 908, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
	$self->set_loaded('OGF_S_SMPARAMS_'.$mode);
}
sub read_motion_def {
	my $self = $_[0];
	($self->{bone_or_part},
	 $self->{motion},
	 $self->{speed},
	 $self->{power},
	 $self->{accrue},
	 $self->{falloff}) = $_[1]->unpack('vvffff');
}
sub read_motion_mark {
	my $self = $_[0];
	$self->{name} = '';
	my $c;
	while (1) {
		($c) = $_[1]->unpack('a');
		last if ($c eq "\n" || $c eq "\r");
		$self->{name} .= $c;
	}
	($c) = $_[1]->unpack('a');
	die unless $c eq "\n";
	my ($count) = $_[1]->unpack('V');
	for (my $i = 0; $i < $count; $i++) {
		my $int = {};
		($int->{min}, $int->{max}) = $_[1]->unpack('ff');
		push @{$self->{intervals}}, $int;
	}
}
sub read_smotions {
	my $self = shift;
	my ($cf, $mode) = @_;
	for (my $expected_id = 0;; $expected_id++) {
		my ($nnnn, $size) = $cf->r_chunk_open();
		defined $nnnn or last;
		if ($nnnn != $expected_id) {
			stkutils::debug::fail('ogf::read_smotions', 945, 'unexpected chunk $nnnn');
		}
		if ($nnnn == 0) {
			$self->{motions_count} = unpack('V', $cf->r_chunk_data());
		} else {
			my $motion = {};
			read_motion($motion, $cf, $mode);
			push @{$self->{motions}}, $motion;
		}
		$cf->r_chunk_close();
	}
	$self->set_loaded('OGF_S_MOTIONS_'.$mode);
}
sub read_motion {
	my $self = shift;
	my ($cf, $mode) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{name}, $self->{keys_count}) = $packet->unpack('Z*V');
	if ($self->{keys_count} == 0) {
		stkutils::debug::fail('ogf::read_motion', 965, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
		return;
	}
	if ($mode == 1) {
		for (my $i = 0; $packet->length() > 0; $i++) {
			my @keyst;
			my $bone = {};
			($bone->{flags}) = $packet->unpack('C');
			die unless ($bone->{flags} & ~0x07) == 0;
			if ($bone->{flags} & KPF_R_ABSENT) {
				@{$bone->{keysr}} = $packet->unpack('s4');
			} else {
				($bone->{crc_keysr}, @{$bone->{keysr}}) = $packet->unpack("V(s4)$self->{keys_count}");
			}
	#		dequantize_qr(\@{$bone->{keysr}});
			if ($bone->{flags} & KPF_T_PRESENT) {
				($bone->{crc_keyst}) = $packet->unpack('V');
				if ($bone->{flags} & KPF_T_HQ) {
					@{$bone->{keyst}} = $packet->unpack("(s3)$self->{keys_count}");
				} else {
					@{$bone->{keyst}} = $packet->unpack("(c3)$self->{keys_count}");
				}
				@{$bone->{sizet}} = $packet->unpack('f3');
			} else {
				die unless ($bone->{flags} & KPF_T_HQ) == 0;
			}
			@{$bone->{initt}} = $packet->unpack('f3');
			push @{$self->{bones}}, $bone;
		}
	} else {
		for (my $i = 0; $packet->length() > 0; $i++) {
			my $bone = {};
			@{$bone->{keys}} = $packet->unpack("(s4f3)$self->{keys_count}");
	#		dequantize_qr(\@{$bone->{keysr}});
			push @{$self->{bones}}, $bone;
		}	
	}
	stkutils::debug::fail('ogf::read_motion', 1002, 'there some data in packet left: '.$packet->length()) unless $packet->length() == 0;
}
sub read_loddef {
# if ( !IReader__find_chunk(a3, 21, 0) )
#    xrDebug__fail(Debug, "data->find_chunk(OGF_LODDEF)", ".\\FLOD.cpp", 10);
#  v6 = (int)((char *)v5 + 144);
# v69 = 8;
# do
#  {
 #   v7 = v6 - 56;
#    IReader__r(v4, v6 - 56, 96);
#    v8 = *(float *)(v6 - 32) - *(float *)(v6 - 56);
#    v9 = *(float *)(v6 - 28) - *(float *)(v6 - 52);
#    v10 = *(float *)(v6 - 24) - *(float *)(v6 - 48);
#    v11 = *(float *)(v6 - 8) - *(float *)(v6 - 32);
#    v12 = *(float *)(v6 - 4) - *(float *)(v6 - 28);
#    v13 = *(float *)v6 - *(float *)(v6 - 24);
#    v48 = v13 * v9 - v12 * v10;
#    v52 = v10 * v11 - v13 * v8;
#    v56 = v12 * v8 - v9 * v11;
#    v70 = v56 * v56 + v52 * v52 + v48 * v48;
#    if ( std__numeric_limits_float___min() >= v70 )
#    {
#      v14 = v56;
#    }
#    else
#    {
#      v15 = sqrt(1.0 / v70);
#      v48 = v48 * v15;
#      v52 = v52 * v15;
#      v14 = v15 * v56;
#    }
#    v66 = v14;
#    v16 = *(float *)(v6 - 8) - *(float *)(v6 - 32);
#    v60 = v48;
#    v63 = v52;
#    v17 = *(float *)(v6 - 4) - *(float *)(v6 - 28);
#    v18 = *(float *)v6 - *(float *)(v6 - 24);
#    v19 = *(float *)(v6 + 16) - *(float *)(v6 - 8);
#    v20 = *(float *)(v6 + 20) - *(float *)(v6 - 4);
#    v21 = *(float *)(v6 + 24) - *(float *)v6;
#    v49 = v21 * v17 - v20 * v18;
#    v53 = v18 * v19 - v21 * v16;
#    v57 = v20 * v16 - v17 * v19;
#    v71 = v57 * v57 + v53 * v53 + v49 * v49;
#    if ( std__numeric_limits_float___min() >= v71 )
#    {
#      v22 = v57;
#    }
#    else
#    {
#      v23 = sqrt(1.0 / v71);
#      v49 = v49 * v23;
#      v53 = v53 * v23;
#      v22 = v23 * v57;
#    }
#    v61 = v49 + v60;
#    v64 = v53 + v63;
#    v67 = v22 + v66;
#    v24 = *(float *)(v6 + 16) - *(float *)(v6 - 8);
#    v25 = *(float *)(v6 + 20) - *(float *)(v6 - 4);
#    v26 = *(float *)(v6 + 24) - *(float *)v6;
#    v27 = *(float *)v7 - *(float *)(v6 + 16);
#    v28 = *(float *)(v6 - 52) - *(float *)(v6 + 20);
#    v29 = *(float *)(v6 - 48) - *(float *)(v6 + 24);
#    v50 = v29 * v25 - v28 * v26;
#    v54 = v26 * v27 - v29 * v24;
#    v58 = v28 * v24 - v25 * v27;
#    v72 = v58 * v58 + v54 * v54 + v50 * v50;
#    if ( std__numeric_limits_float___min() >= v72 )
#    {
#      v30 = v58;
#    }
#    else
#    {
#      v31 = sqrt(1.0 / v72);
#      v50 = v50 * v31;
#      v54 = v54 * v31;
#      v30 = v31 * v58;
#    }
#    v62 = v50 + v61;
#    v65 = v54 + v64;
#    v68 = v30 + v67;
#    v32 = *(float *)v7 - *(float *)(v6 + 16);
#    v33 = *(float *)(v6 - 52) - *(float *)(v6 + 20);
#    v34 = *(float *)(v6 - 48) - *(float *)(v6 + 24);
#    v35 = *(float *)(v6 - 32) - *(float *)v7;
#    v36 = *(float *)(v6 - 28) - *(float *)(v6 - 52);
#    v37 = *(float *)(v6 - 24) - *(float *)(v6 - 48);
#    v51 = v37 * v33 - v36 * v34;
#    v55 = v34 * v35 - v37 * v32;
#    v59 = v36 * v32 - v33 * v35;
#    v73 = v59 * v59 + v55 * v55 + v51 * v51;
#   if ( std__numeric_limits_float___min() >= v73 )
#    {
#      v38 = v59;
#    }
#    else
#    {
#      v39 = sqrt(1.0 / v73);
#      v51 = v51 * v39;
#      v55 = v55 * v39;
#      v38 = v39 * v59;
#    }
#    v6 += 108;
#    v40 = v69-- == 1;
#    v41 = v38 + v68;
#    v42 = (v51 + v62) * 0.25;
#    v43 = (v55 + v65) * 0.25;
#    v44 = v41 * 0.25;
#    v45 = sqrt(1.0 / (v44 * v44 + v43 * v43 + v42 * v42));
#    *(float *)(v6 - 68) = v42 * v45;
#    *(float *)(v6 - 64) = v43 * v45;
#    *(float *)(v6 - 60) = v44 * v45;
#    *(float *)(v6 - 68) = -*(float *)(v6 - 68);
#    *(float *)(v6 - 64) = -*(float *)(v6 - 64);
#    *(float *)(v6 - 60) = -*(float *)(v6 - 60);
#  }
#  while ( !v40 );
}
sub read_loddef2 {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	for (my $i = 0; $i < 8; $i++) {
		my $lod_face = {};
		for (my $j = 0; $j < 4; $j++) {
			my $vertex = {};
			@{$vertex->{v}} = $packet->unpack('f3');
			@{$vertex->{t}} = $packet->unpack('f2');
			($vertex->{c_rgb_hemi}, $vertex->{c_sun}) = $packet->unpack('VC');
			@{$vertex->{pad}} = $packet->unpack('C3');
			push @{$lod_face->{vertices}}, $vertex;
		}
		push @{$self->{lod_faces}}, $lod_face;
	}
	$self->set_loaded('OGF_LODDEF2');
}
sub read_treedef {
#  IReader__r(v5, (char *)v4 + 132, 64);
#  IReader__r(v5, (char *)v4 + 196, 16);
#  IReader__r(v5, (char *)v4 + 212, 16);
#  *((_DWORD *)v4 + 16) = CShaderManager__CreateGeom(v25, *((_DWORD *)v4 + 17), *((_DWORD *)v4 + 20));
#  *((_DWORD *)v4 + 24) = sub_599BF0("consts");
#  *((_DWORD *)v4 + 25) = sub_599BF0("wave");
#  *((_DWORD *)v4 + 26) = sub_599BF0("wind");
#  *((_DWORD *)v4 + 27) = sub_599BF0("c_bias");
#  *((_DWORD *)v4 + 28) = sub_599BF0("c_scale");
#  *((_DWORD *)v4 + 29) = sub_599BF0("m_m2w");
#  *((_DWORD *)v4 + 30) = sub_599BF0("m_w2v2p");
#  *((_DWORD *)v4 + 31) = sub_599BF0("v_eye");

}
sub read_treedef2 {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	@{$self->{tree_xform}} = $packet->unpack('f16');
	read_ogf_color($self->{c_scale}, $packet);
	read_ogf_color($self->{c_bias}, $packet);
	$self->set_loaded('OGF_TREEDEF2');
}
sub read_ogf_color {
	my $self = shift;
	my ($packet) = @_;
	@{$self->{rgb}} = $packet->unpack('f3');
	($self->{hemi}, $self->{sun}) = $packet->unpack('ff');
}
sub read_swicontainer {
	my $self = shift;
	my ($cf) = @_;
	my $packet = stkutils::data_packet->new($cf->r_chunk_data());
	($self->{ext_swib_index}) = $packet->unpack('V');
	
}
sub write {
	my $self = shift;
	my ($cf, $subversion) = @_;
	$self->{subversion} = $subversion;
	$self->write_header($cf);
	SWITCH: {
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_NORMAL' && do { $self->write_visual($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_HIERRARHY' && do { $self->write_hierrarhy_visual($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_PROGRESSIVE' && do { $self->write_progressive($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_ANIM' && do { $self->write_kinematics_animated($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_GEOMDEF_PM' && do { $self->write_skeletonx_pm($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_GEOMDEF_ST' && do { $self->write_skeletonx_st($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_PROGRESSIVE2' && do { $self->write_progressive2($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_LOD' && do { $self->write_lod($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_TREE' && do { $self->write_tree_visual_st($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_TREE_ST' && do { $self->write_tree_visual_st($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_RIGID' && do { $self->write_kinematics($cf); last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_TREE_PM' && do { $self->write_tree_visual_pm($cf); last SWITCH; };
		stkutils::debug::fail(__PACKAGE__.'::write', __LINE__, '', "unexpected model type $self->{type}");
	}
}
sub write_header {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open(0x1);
	$cf->w_chunk_data(pack('CCv', $self->{format_version}, $self->{type}, $self->{shader_id}));
	if ($self->{format_version} == 4) {
		$self->write_bbox($cf);
		$self->write_bsphere($cf);
	}
	$cf->w_chunk_close();
}
sub write_render_visual {
	my $self = shift;
	my ($cf) = @_;
	if ($self->{format_version} == 3 && $self->check_loaded('OGF_BBOX')) {
		$cf->w_chunk_open(0x6);
		$self->write_bbox($cf);
		$cf->w_chunk_close();
	}
	if ($self->{format_version} == 3 && $self->check_loaded('OGF_BSPHERE')) {
		$cf->w_chunk_open(0xb);
		$self->write_bsphere($cf);
		$cf->w_chunk_close();
	}
	if ($self->check_loaded('OGF_S_DESC')) {
		$self->write_s_desc($cf);
	}
	if ($self->{format_version} == 3 && $self->check_loaded('OGF_TEXTURE_L')) {
		$self->write_texture_l($cf);
	}	
	if ($self->check_loaded('OGF_TEXTURE')) {
		$self->write_texture($cf);
	}
};
sub write_visual {
	my $self = shift;
	my ($cf) = @_;
	$self->write_render_visual($cf);
	if ($self->{format_version} == 4 && $self->check_loaded('OGF_GCONTAINER')) {
		$self->write_gcontainer($cf);
		if ($self->check_loaded('OGF_FASTPATH')) {
			$self->write_fastpath($cf) ;
		}
	}
	if ($self->check_loaded('OGF_VCONTAINER')) {
		$self->write_vcontainer($cf);
	} elsif ($self->check_loaded('OGF_VERTICES')) {
		$self->write_vertices($cf);
	}
	if ($self->check_loaded('OGF_ICONTAINER')) {
		$self->write_icontainer($cf);
	} elsif ($self->check_loaded('OGF_INDICES')) {
		$self->write_indices($cf);
	}
}
sub write_hierrarhy_visual {
	my $self = shift;
	my ($cf) = @_;
	$self->write_render_visual($cf);
	if ($self->check_loaded('OGF_CHILDREN_L')) {
		$self->write_children_l($cf);
	} elsif ($self->check_loaded('OGF_CHILDREN')) {
		$self->write_children($cf);
	} elsif ($self->check_loaded('OGF_CHILD_REFS')) {
		$self->write_child_refs($cf);
	}
};
sub write_progressive {
	my $self = shift;
	my ($cf) = @_;
	$self->write_visual($cf);
	if ($self->{format_version} == 4) {
		$self->write_swidata($cf);
	} else {
		$self->write_loddata($cf);
	}
}
sub write_kinematics {
	my $self = shift;
	my ($cf) = @_;
	$self->write_hierrarhy_visual($cf);
	if ($self->{format_version} == 4) {
		if ($self->check_loaded('OGF_S_LODS_CSKY')) {
			$self->write_s_lods_csky($cf);
		} elsif ($self->check_loaded('OGF_S_LODS')) {
			$self->write_s_lods($cf);
		}
	}
	if ($self->check_loaded('OGF_S_USERDATA')) {
		$self->write_s_userdata($cf);
	}
	if ($self->check_loaded('OGF_S_BONE_NAMES')) {
		$self->write_s_bone_names($cf);
	}
	if ($self->check_loaded('OGF_S_IKDATA_2')) {
		$self->write_s_ikdata($cf, 2);
	} elsif ($self->check_loaded('OGF_S_IKDATA_1')) {
		$self->write_s_ikdata($cf, 1);
	}
}
sub write_kinematics_animated {
	my $self = shift;
	my ($cf) = @_;
	$self->write_kinematics($cf);
	if ($self->{format_version} == 4 && $self->check_loaded('OGF_S_MOTION_REFS_1')) {
		$self->write_smotion_refs_1($cf);
		return;
	} elsif ($self->check_loaded('OGF_S_MOTION_REFS_0')) {
		$self->write_smotion_refs_0($cf);
		return;
	} elsif ($self->check_loaded('OGF_S_SMPARAMS_1')) {
		$self->write_s_smparams($cf, 1);
	} elsif ($self->{format_version} == 3 && $self->check_loaded('OGF_S_SMPARAMS_0')) {
		$self->write_s_smparams($cf, 2);
	} 
	if ($self->check_loaded('OGF_S_MOTIONS_1')) {
		$self->write_smotions($cf, 1);
	} elsif ($self->{format_version} == 3 && $self->check_loaded('OGF_S_MOTIONS_0')) {
		$self->write_smotions($cf, 0);
	} else {
		stkutils::debug::fail('ogf::write_kinematics_animated', 1184, 'no motions to write');
	}
}
sub write_skeletonx_pm {
	my $self = shift;
	my ($cf) = @_;
	$self->write_progressive($cf);
}
sub write_skeletonx_st {
	my $self = shift;
	my ($cf) = @_;
	$self->write_visual($cf);
}
sub write_progressive2 {
	my $self = shift;
	my ($cf) = @_;
	$self->write_render_visual($cf);
	$self->write_s_lods($cf);
}
sub write_lod {
	my $self = shift;
	my ($cf) = @_;
	$self->write_hierrarhy_visual($cf);
	if ($self->check_loaded('OGF_LODDEF2')) {
		$self->write_loddef2($cf);
	}
}
sub write_tree_visual {
	my $self = shift;
	my ($cf) = @_;
	$self->write_visual($cf);
	$self->write_treedef2($cf);
}
sub write_tree_visual_st {
	my $self = shift;
	my ($cf) = @_;
	$self->write_tree_visual($cf);
}
sub write_tree_visual_pm {
	my $self = shift;
	my ($cf) = @_;
	$self->write_tree_visual($cf);
	$self->write_swicontainer($cf);
}
sub write_bbox {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_data(pack('f3f3', @{$self->{bbox}->{min}}, @{$self->{bbox}->{max}}));
}
sub write_bsphere {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_data(pack('f3f', @{$self->{bsphere}->{c}}, $self->{bsphere}->{r}));
}
sub write_s_desc {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk($chunk_names{$self->{format_version}}{'OGF_S_DESC'}, pack('Z*Z*VZ*VZ*V', $self->{ogf_object}, $self->{ogf_creator}, $self->{unk}, $self->{creator}, $self->{create_time}, $self->{editor}, $self->{edit_time}));
}
sub write_texture_l {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk($chunk_names{$self->{format_version}}{'OGF_TEXTURE_L'}, pack('VV', $self->{texture_id}, $self->{shader_id}));
}
sub write_texture {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk($chunk_names{$self->{format_version}}{'OGF_TEXTURE'}, pack('Z*Z*', $self->{texture_name}, $self->{shader_name}));
}
sub write_vcontainer {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk($chunk_names{$self->{format_version}}{'OGF_VCONTAINER'}, pack('VVV', $self->{ext_vb_index}, $self->{ext_vb_offset}, $self->{ext_vb_size}));
}
sub write_vertices {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_VERTICES'});
	$cf->w_chunk_data(pack('VV', $self->{vertex_format}, $self->{vertex_count}));
	if ($self->{vertex_format} == OGF_VERTEXFORMAT_FVF_OLD) {
		foreach my $vertice (@{$self->{vertices}}) {
			$cf->w_chunk_data(pack('f3f3f2', @{$vertice->{point}}, @{$vertice->{normal}}, @{$vertice->{textcoords}}));
		}
	} elsif ($self->{format_version} == 3 && $self->{vertex_format} == OGF_VERTEXFORMAT_FVF_1L) {
		foreach my $vertice (@{$self->{vertices}}) {
			$cf->w_chunk_data(pack('f3f3f2l', @{$vertice->{point}}, @{$vertice->{normal}}, @{$vertice->{textcoords}}, $vertice->{matrix}));
		}
	} elsif ($self->{vertex_format} == OGF_VERTEXFORMAT_FVF_1L or $self->{vertex_format} == OGF_VERTEXFORMAT_FVF_1_CS) {
		foreach my $vertice (@{$self->{vertices}}) {
			$cf->w_chunk_data(pack('f3f3f3f3f2l', @{$vertice->{point}}, @{$vertice->{normal}}, @{$vertice->{t}}, @{$vertice->{b}}, @{$vertice->{textcoords}}, $vertice->{matrix}));
		}
	} elsif ($self->{vertex_format} == OGF_VERTEXFORMAT_FVF_2L or $self->{vertex_format} == OGF_VERTEXFORMAT_FVF_2_CS) {
		foreach my $vertice (@{$self->{vertices}}) {
			$cf->w_chunk_data(pack('vvf3f3f3f3ff2', $vertice->{matrix0}, $vertice->{matrix1}, @{$vertice->{point}}, @{$vertice->{normal}}, @{$vertice->{t}}, @{$vertice->{b}}, $vertice->{w}, @{$vertice->{textcoords}}));
		}
	} elsif ($self->{vertex_format} == OGF_VERTEXFORMAT_FVF_3_CS) {
		foreach my $vertice (@{$self->{vertices}}) {
			$cf->w_chunk_data(pack('vvvf3f3f3f3fff2', $vertice->{matrix0}, $vertice->{matrix1}, $vertice->{matrix2}, @{$vertice->{point}}, @{$vertice->{normal}}, @{$vertice->{t}}, @{$vertice->{b}}, $vertice->{w0}, $vertice->{w1}, @{$vertice->{textcoords}}));
		}
	} elsif ($self->{vertex_format} == OGF_VERTEXFORMAT_FVF_4_CS) {
		foreach my $vertice (@{$self->{vertices}}) {
			$cf->w_chunk_data(pack('vvvvf3f3f3f3ffff2', $vertice->{matrix0}, $vertice->{matrix1}, $vertice->{matrix2}, $vertice->{matrix3}, @{$vertice->{point}}, @{$vertice->{normal}}, @{$vertice->{t}}, @{$vertice->{b}}, $vertice->{w0}, $vertice->{w1}, $vertice->{w2}, @{$vertice->{textcoords}}));
		}
	}
	$cf->w_chunk_close();
}
sub write_icontainer {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk($chunk_names{$self->{format_version}}{'OGF_ICONTAINER'}, pack('VVV', $self->{ext_ib_index}, $self->{ext_ib_offset}, $self->{ext_ib_size}));
}
sub write_indices {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_INDICES'});
	$cf->w_chunk_data(pack('V', $self->{indices_count}));
	foreach my $index (@{$self->{indices}}) {
		$cf->w_chunk_data(pack('v', $index));
	}
	$cf->w_chunk_close();
}
sub write_children_l {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_CHILDREN_L'});
	$cf->w_chunk_data(pack('V', $#{$self->{children_l}} + 1));
	foreach my $child (@{$self->{children_l}}) {
		$cf->w_chunk_data(pack('V', $child));
	}
	$cf->w_chunk_close();
}
sub write_children {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_CHILDREN'});
	my $id = 0;
	foreach my $child (@{$self->{children}}) {
		$cf->w_chunk_open($id);
		$child->write($cf);
		$cf->w_chunk_close();
		$id++;
	}
	$cf->w_chunk_close();
}
sub write_child_refs {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_CHILD_REFS'});
	$cf->w_chunk_data(pack('V', $#{$self->{children_l}} + 1));
	foreach my $child (@{$self->{child_refs}}) {
		$cf->w_chunk_data(pack('Z*', $child));
	}
	$cf->w_chunk_close();
}
sub write_swidata {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_SWIDATA'});
	$cf->w_chunk_data(pack('V4V', @{$self->{swi_reserved}}, $self->{swi_count}));
	foreach my $swi (@{$self->{swi_data}}) {
		$cf->w_chunk_data(pack('lvv', $swi->{offset}, $swi->{num_tris}, $swi->{num_verts}));
	}
	$cf->w_chunk_close();
}
sub write_loddata {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_LODDATA'});
	foreach my $lod (@{$self->{loddata}}) {
		write_hoppe_header($lod, $cf);
		write_hoppe_vertsplits($lod, $cf);
		write_hoppe_fix_faces($lod, $cf);
	}
	$cf->w_chunk_close();
}
sub write_hoppe_header {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk(OGF3_HOPPE_HEADER, pack('VV', $self->{min_vertices}, $self->{max_vertices}));
}
sub write_hoppe_vertsplits {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open(OGF3_HOPPE_VERT_SPLITS);
	foreach my $vertsplit (@{$self->{vertsplits}}) {
		$cf->w_chunk_data(pack('vCC', $vertsplit->{vert}, $vertsplit->{num_tris}, $vertsplit->{num_verts}));
	}	
	$cf->w_chunk_close();
}
sub write_hoppe_fix_faces {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open(OGF3_HOPPE_FIX_FACES);
	$cf->w_chunk_data(pack('V', $self->{num_fix_faces}));
	$cf->w_chunk_data(pack("(v)$self->{num_fix_faces}", @{$self->{fix_faces}}));
	$cf->w_chunk_close();
}
sub write_s_lods_csky {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk($chunk_names{$self->{format_version}}{'OGF_S_LODS'}, substr(pack('Z*', $self->{s_lods_ref}), 0, -1));
}
sub write_s_lods {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_S_LODS'});
	my $id = 0;
	foreach my $lod (@{$self->{s_lods}}) {
		$cf->w_chunk_open($id);
		$lod->write($cf);
		$cf->w_chunk_close();
		$id++;
	}
	$cf->w_chunk_close();
}
sub write_s_userdata {
	my $self = shift;
	my ($cf) = @_;
	my $len = length($self->{userdata});
	$cf->w_chunk($chunk_names{$self->{format_version}}{'OGF_S_USERDATA'}, pack("a$len", $self->{userdata}));
}
sub write_s_bone_names {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_S_BONE_NAMES'});
	$cf->w_chunk_data(pack('V', $#{$self->{bone_names}} + 1));
	foreach my $bone_name_obj (@{$self->{bone_names}}) {
		$cf->w_chunk_data(pack('Z*Z*', $bone_name_obj->{name}, $bone_name_obj->{parent_name}));
		write_obb($bone_name_obj, $cf);
	}
	$cf->w_chunk_close();
}
sub write_obb {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_data(pack('f9f3f3', @{$self->{rotate}}, @{$self->{translate}}, @{$self->{halfsize}}));
}
sub write_sphere {
	my $self = $_[0];
	$_[1]->w_chunk_data(pack('f3f', @{$self->{p}}, $self->{r}));
}
sub write_cylinder {
	my $self = $_[0];
	$_[1]->w_chunk_data(pack('f3f3ff', @{$self->{center}}, @{$self->{direction}}, $self->{height}, $self->{radius}));
}
sub write_s_ikdata {
	my $self = shift;
	my ($cf, $mode) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_S_IKDATA_'.$mode});
	foreach my $ik (@{$self->{ik_data}}) {
		if ($mode == 2) {
			$cf->w_chunk_data(pack('VZ*', $ik->{version}, $ik->{game_mtl_name}));
		} else {
			$cf->w_chunk_data(pack('Z*', $ik->{game_mtl_name}));
		}
		write_s_bone_shape($ik->{bone_shape}, $cf);
		write_s_joint_ik_data($ik->{joint_data}, $cf);
		if ($mode > 0) {
			$cf->w_chunk_data(pack('f3f3', @{$ik->{bind_rotation}}, @{$ik->{bind_position}}));
		}
		$cf->w_chunk_data(pack('ff3', $ik->{mass}, @{$ik->{center_of_mass}}));
	}
	$cf->w_chunk_close();
}
sub write_s_bone_shape {
	my $self = $_[0];
	$_[1]->w_chunk_data(pack('vv', $self->{type}, $self->{flags}));
	write_obb($self->{box}, $_[1]);
	write_sphere($self->{sphere}, $_[1]);
	write_cylinder($self->{cylinder}, $_[1]);
}
sub write_s_joint_ik_data {
	my $self = $_[0];
	$_[1]->w_chunk_data(pack('V', $self->{type}));
	write_s_joint_limit(${$self->{limits}}[0], $_[1]);
	write_s_joint_limit(${$self->{limits}}[1], $_[1]);
	write_s_joint_limit(${$self->{limits}}[2], $_[1]);
	$_[1]->w_chunk_data(pack('ffVff', $self->{spring_factor}, $self->{damping_factor}, $self->{ik_flags}, $self->{break_force}, $self->{break_torque}));
	$_[1]->w_chunk_data(pack('f', $self->{friction})) if defined $self->{friction};
}
sub write_s_joint_limit {
	my $self = $_[0];
	$_[1]->w_chunk_data(pack('f2ff', @{$self->{limit}}, $self->{spring_factor}, $self->{damping_factor}));
}
sub write_smotion_refs_1 {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_S_MOTION_REFS_1'});
	$cf->w_chunk_data(pack('V', $#{$self->{motion_refs_1}} + 1));
	foreach my $ref (@{$self->{motion_refs_1}}) {
		$cf->w_chunk_data(pack('Z*', $ref));
	}
	$cf->w_chunk_close();
}
sub write_smotion_refs_0 {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk($chunk_names{$self->{format_version}}{'OGF_S_MOTION_REFS_0'}, pack('Z*', $self->{motion_refs_0}));
}
sub write_s_smparams {
	my $self = shift;
	my ($cf, $mode) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_S_SMPARAMS_'.$mode});
	$cf->w_chunk_data(pack('vv', $self->{sm_params_version}, $#{$self->{s_smparams_partitions}} + 1));
	foreach my $part (@{$self->{s_smparams_partitions}}) {
		$cf->w_chunk_data(pack('Z*v', $part->{name}, $part->{bone_count}));
		foreach my $bone (@{$part->{bones}}) {
			if ($mode == 0 || $self->{sm_params_version} == 1) {
				$cf->w_chunk_data(pack('V', $bone->{bone_id}));
			} elsif ($self->{sm_params_version} == 2) {
				$cf->w_chunk_data(pack('Z*', $bone->{bone_name}));
			} elsif ($self->{sm_params_version} == 3 || $self->{sm_params_version} == 4) {	
				$cf->w_chunk_data(pack('Z*V', $bone->{bone_name}, $bone->{bone_id}));
			}
		}
	}
	$cf->w_chunk_data(pack('v', $#{$self->{s_smparams_motions}} + 1));
	foreach my $mot (@{$self->{s_smparams_motions}}) {
		if ($mode == 1) {
			$cf->w_chunk_data(pack('Z*V', $mot->{name}, $mot->{flags}));
			write_motion_def($mot, $cf);
			if ($self->{sm_params_version} == 4) {
				$cf->w_chunk_data(pack('V', $#{$mot->{mmarks}} + 1));
				foreach my $nmark (@{$mot->{mmarks}}) {
					write_motion_mark($nmark, $cf);
				}
			}
		} else {
			my $flag = 0;
			if ($mot->{flags} & 0x2) {
				$mot->{flags} -= 0x2;
				$flag = 1;
			}
			$cf->w_chunk_data(pack('Z*C', $mot->{name}, $mot->{flags}));
			write_motion_def($mot, $cf);
			$cf->w_chunk_data(pack('C', $flag));
		}
	}	
	$cf->w_chunk_close();
}
sub write_motion_def {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_data(pack('vvffff', $self->{bone_or_part}, $self->{motion}, $self->{speed}, $self->{power}, $self->{accrue}, $self->{falloff}));
}
sub write_motion_mark {
	my $self = shift;
	my ($cf) = @_;
	my $len = length($self->{name});
	$cf->w_chunk_data(pack("(a)$len", $self->{name}));
	$cf->w_chunk_data(pack('V', $#{$self->{intervals}} + 1));
	foreach my $int (@{$self->{intervals}}) {
		$cf->w_chunk_data(pack('ff', $int->{min}, $int->{max}));
	}
}
sub write_smotions {
	my $self = shift;
	my ($cf, $mode) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_S_MOTIONS_'.$mode});
	$cf->w_chunk_open(0);
	$cf->w_chunk_data(pack('V', $self->{motions_count}));
	$cf->w_chunk_close();
	my $id = 1;
	foreach my $motion (@{$self->{motions}}) {
		$cf->w_chunk_open($id++);
		write_motion($motion, $cf, $mode);
		$cf->w_chunk_close();
	}
	$cf->w_chunk_close();
}
sub write_motion {
	my $self = shift;
	my ($cf, $mode) = @_;
	$cf->w_chunk_data(pack('Z*V', $self->{name}, $self->{keys_count}));
	if ($self->{keys_count} == 0) {
		return;
	}
	if ($mode == 1) {
		foreach my $bone (@{$self->{bones}}) {
			$cf->w_chunk_data(pack('C', $bone->{flags}));
			if ($bone->{flags} & KPF_R_ABSENT) {
				$cf->w_chunk_data(pack('s4', @{$bone->{keysr}}));
			} else {
				$cf->w_chunk_data(pack("V(s4)$self->{keys_count}", $bone->{crc_keysr}, @{$bone->{keysr}}));
			}			
			if ($bone->{flags} & KPF_T_PRESENT) {
				$cf->w_chunk_data(pack('V', $bone->{crc_keyst}));
				if ($bone->{flags} & KPF_T_HQ) {
					for (my $j = 0; $j < $self->{keys_count}; $j++) {
						$cf->w_chunk_data(pack('s3', @{$bone->{keyst}}[$j*3..$j*3+2]));
					}					
				} else {
					for (my $j = 0; $j < $self->{keys_count}; $j++) {
						$cf->w_chunk_data(pack('c3', @{$bone->{keyst}}[$j*3..$j*3+2]));
					}			
				}
				$cf->w_chunk_data(pack('f3', @{$bone->{sizet}}));
			}
			$cf->w_chunk_data(pack('f3', @{$bone->{initt}}));
		}		
	} else {
		foreach my $bone (@{$self->{bones}}) {
			for (my $n = 0; $n < $self->{keys_count}; $n++) {
				$cf->w_chunk_data(pack('s4f3', @{$bone->{keys}}[$n*7..$n*7+6]));
			}
		}		
	}
}
sub write_loddef2 {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_LODDEF2'});
	foreach my $lod_face (@{$self->{lod_faces}}) {
		foreach my $vertex (@{$lod_face->{vertices}}) {
			$cf->w_chunk_data(pack('f3f2VCC3', @{$vertex->{v}}, @{$vertex->{t}}, $vertex->{c_rgb_hemi}, $vertex->{c_sun}, @{$vertex->{pad}}));
		}
	}
	$cf->w_chunk_close();
}
sub write_treedef2 {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_open($chunk_names{$self->{format_version}}{'OGF_TREEDEF2'});
	$cf->w_chunk_data(pack('f16', @{$self->{tree_xform}}));
	write_ogf_color($self->{c_scale}, $cf);
	write_ogf_color($self->{c_bias}, $cf);
	$cf->w_chunk_close();
}
sub write_ogf_color {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk_data(pack('f3ff', @{$self->{rgb}}, $self->{hemi}, $self->{sun}));
}
sub write_swicontainer {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk($chunk_names{$self->{format_version}}{'OGF_SWICONTAINER'}, pack('V', $self->{ext_swib_index}));
}
sub convert {
	my $self = shift;
	my ($nVer, $subversion) = @_;
	if ($nVer == $self->{format_version}) {
		if ($nVer == 3) {
			$self->subver_3($subversion);
		} else {
			$self->subver_4($subversion);
		}
	} elsif ($nVer == 3) {
		$self->conv_to_3();
	} else {
		$self->conv_to_4();
	}
}
sub subver_4 {
	my $self = shift;
	my ($subversion) = @_;
	### Attention! Now it support only FSL_VISUALS converting! ###
	if ($subversion == 1 || $subversion == 2 || $subversion == 3) {
		if ($self->check_loaded('OGF_GCONTAINER')) {
			print "yes, type = $self->{type}\n";
			$self->set_loaded('OGF_ICONTAINER');
			$self->set_loaded('OGF_VCONTAINER');
			$self->{loaded_chunks} -= $chunks_loaded{$self->{format_version}}{'OGF_GCONTAINER'};
			die if $self->check_loaded('OGF_GCONTAINER');
			die unless $self->check_loaded('OGF_VCONTAINER');
		}
		if ($self->{type} == 11) {
			delete $self->{ext_swib_index};
			$self->{type} = 7;
			$self->{loaded_chunks} -= $chunks_loaded{$self->{format_version}}{'OGF_SWICONTAINER'} if $self->check_loaded('OGF_SWICONTAINER');
			die if $self->check_loaded('OGF_GCONTAINER');
			die unless $self->check_loaded('OGF_VCONTAINER');
		}
		if ($self->{type} == 2) {
			delete $self->{swi_data};
			$self->{type} = 0;
			$self->{loaded_chunks} -= $chunks_loaded{$self->{format_version}}{'OGF_SWIDATA'} if $self->check_loaded('OGF_SWIDATA');
			die if $self->check_loaded('OGF_GCONTAINER');
			die unless $self->check_loaded('OGF_VCONTAINER');
		}
	} elsif ($subversion == 4) {
		if ($self->check_loaded('OGF_GCONTAINER')) {
			$self->set_loaded('OGF_ICONTAINER');
			$self->set_loaded('OGF_VCONTAINER');
			$self->{loaded_chunks} -= $chunks_loaded{$self->{format_version}}{'OGF_GCONTAINER'};
		}		
	} else {
		if ($self->check_loaded('OGF_VCONTAINER')) {
			$self->set_loaded('OGF_GCONTAINER');
			$self->{loaded_chunks} -= $chunks_loaded{$self->{format_version}}{'OGF_VCONTAINER'};
			$self->{loaded_chunks} -= $chunks_loaded{$self->{format_version}}{'OGF_ICONTAINER'};
		}			
	}
}
sub conv_to_3 {
	my $self = shift;
	SWITCH: {
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_NORMAL' && do {last SWITCH; };	
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_HIERRARHY' && do {last SWITCH; }; #FIXME: childeren may vary in different builds		
	#MT_PROGRESSIVE ???
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_PROGRESSIVE' && do { $self->swi_to_loddata(); last SWITCH; }; 
	#MT_SKELETON_ANIM ???
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_ANIM' && do { die; last SWITCH; };
	#MT_SKELETON_GEOMDEF_PM ???
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_GEOMDEF_PM' && do {  $self->swi_to_loddata(); $self->{type} = 0x3; last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_GEOMDEF_ST' && do { $self->{type} = 0x7; last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_LOD' && do { $self->{type} = 0xb; last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_TREE_ST' && do { $self->{type} = 0xc; last SWITCH; };
	#MT_SKELETON_RIGID ???
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_RIGID' && do { die; last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_TREE_PM' && do { $self->{type} = 0xc; last SWITCH; };
		stkutils::debug::fail('ogf::conv_to_3', __LINE__, 'unexpected model type');
	}	
}
sub conv_to_4 {
	my $self = shift;
	SWITCH: {
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_NORMAL' && do {last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_HIERRARHY' && do {last SWITCH; }; #FIXME: children may vary in different builds		
	#MT_PROGRESSIVE ???
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_PROGRESSIVE' && do { $self->loddata_to_swi(); last SWITCH; };
	#MT_SKELETON_ANIM ???
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_ANIM' && do { die; last SWITCH; };
	#MT_SKELETON_GEOMDEF_PM ???
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_GEOMDEF_PM' && do { $self->loddata_to_swi(); $self->{type} = 0x4; last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_GEOMDEF_ST' && do { $self->{type} = 0x5; last SWITCH; };
	#MT_PROGRESSIVE2 ???
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_PROGRESSIVE2' && do {  die; last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_LOD' && do {$self->{type} = 0x6; last SWITCH; };
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_TREE' && do { $self->{type} = 0x7; last SWITCH; };
	#MT_SKELETON_RIGID ???
		$mt_names{$self->{format_version}}{$self->{type}} eq 'MT_SKELETON_RIGID' && do { die; last SWITCH; };
		stkutils::debug::fail('ogf::conv_to_4', __LINE__, 'unexpected model type');
	}
}
sub swi_to_loddata {
	my $self = shift;
	stkutils::debug::fail('ogf::swi_to_loddata', __LINE__, "not implemented yet");
}
sub loddata_to_swi {
	my $self = shift;
	stkutils::debug::fail('ogf::loddata_to_swi', __LINE__, "not implemented yet");
}
sub make_signature {
	my $self = shift;
	$self->{signature} |= S_POINTS;
	$self->{signature} |= S_NORMALS;
	$self->{signature} |= S_TEXCOORDS;
	if ($self->{vertex_format} != OGF_VERTEXFORMAT_FVF_OLD) {
		$self->{signature} |= S_INFLUENCES;
	}
}
sub to_object {
	my $self = shift;
	my $mesh = {};
	if ($self->hierarhical()) {
		my $shader_id = 0;
		foreach my $child (@{$self->{children}}) {
			$child->make_signature();
			$mesh->{signature} |= $child->{signature};
			$child->push_to_mesh($mesh, $shader_id, $shader_id);
			$shader_id++;
		}
	} else {
		$self->push_to_mesh($mesh, 0, 0);
	}
}
sub push_to_mesh {
	my $self = shift;
	my ($mesh, $texture, $eshader) = @_;
	my $vb_offset =  $self->{vertex_count} & UINT32_MAX;
	for (my $i = 0; $i < $self->{indices_count}; $i += 3) {
		my $v0 = @{$self->{indices}}[$i + 0];
		my $v1 = @{$self->{indices}}[$i + 1];
		my $v2 = @{$self->{indices}}[$i + 2];
		my @p0 = @{$self->{vertices}[$v0]->{point}};
		my @p1 = @{$self->{vertices}[$v1]->{point}};
		my @p2 = @{$self->{vertices}[$v2]->{point}};
		if ((join ('', @p0) cmp join ('', @p1)) || 
			(join ('', @p1) cmp join ('', @p2)) ||
			(join ('', @p2) cmp join ('', @p0))) {
			next;
		}
		my $area = calc_area(@p0, @p1, @p2);	
		if ($area <= 0.000001) {
			next;
		}
		my $perim = calc_perimeter(@{$self->{vertices}[$v0]->{textcoords}}, @{$self->{vertices}[$v1]->{textcoords}}, @{$self->{vertices}[$v2]->{textcoords}});
		if ($perim <= 1/8192.) {
			next;
		}
		my $face = set_face($self->{signature}, $vb_offset + $v0, $vb_offset + $v1, $vb_offset + $v2);
		push @{$mesh->{faces}}, $face; 
	}
	if ($#{$mesh->{faces}} != -1) {
		for (my $i = $self->{vertex_count}; $i != 0; $i--) {
#			my $ref = b_proxy($vb_offset);
#			push @{$mesh->{refs}}, $ref;
			$vb_offset++;
		}
#		m_vb.push(vb, &ib, xform);
	}
}
sub set_face {
	my $face = {};
	my $sign = shift;
	if ($sign & S_POINTS) {
		@{$face->{v}} = @_;
	}
	if ($sign & S_NORMALS) {
		@{$face->{n}} = @_;
	}
	if ($sign & S_TEXCOORDS) {
		@{$face->{tc}} = @_;
	}
	return $face;
}
sub calc_area {
	my $tr1 = ($_[4] - $_[3])*($_[8] - $_[6]) - ($_[5] - $_[3])*($_[7] - $_[6]);
	my $tr2 = ($_[1] - $_[0])*($_[8] - $_[6]) - ($_[2] - $_[0])*($_[7] - $_[6]);
	my $tr3 = ($_[1] - $_[0])*($_[5] - $_[3]) - ($_[2] - $_[0])*($_[4] - $_[3]);
	return vector_length($tr1, $tr2, $tr3);
}
sub calc_perimeter {
	return (vector_length(@_[0..2]) + vector_length(@_[3..5]) + vector_length(@_[6..8]));
}
sub vector_length {
	return sqrt($_[0]*$_[0] + $_[1]*$_[1] + $_[2]*$_[2]);
}
sub hierrarhical {
	my $self = shift;
	my $type = $mt_names{$self->{format_version}}{$self->{type}};
	if ($type eq 'MT_NORMAL' ||
		$type eq 'MT_HIERRARHY' ||
		$type eq 'MT_SKELETON_ANIM' ||
		$type eq 'MT_SKELETON_RIGID') {
		return 1;
	} else {return 0;}
}
sub set_loaded {
	my $self = shift;
	my ($chunk) = @_;
	$self->{loaded_chunks} += $chunks_loaded{$self->{format_version}}{$chunk};
}
sub check_loaded {
	my $self = shift;
	my ($chunk) = @_;
	return ($self->{loaded_chunks} & $chunks_loaded{$self->{format_version}}{$chunk});
}
sub check_unhandled_chunks {
	my $self = shift;
	my ($cf, $fn) = @_;
	my %rev_names = reverse %{$chunk_names{$self->{format_version}}};
	$cf->{fh}->seek(0, 0);
	my $fsize = -s $fn;
	for (my $offset = 0; $offset < $fsize;) {
		my $data = '';
		$cf->{fh}->read($data, 8);
		my ($index, $size) = unpack('VV', $data);
		if ($index == 0 && $size ==0) {
			last;
		}
		if (!($self->check_loaded($rev_names{$index}))) {
			my $name = $rev_names{$index};
			if ($name) {
				stkutils::debug::fail(__PACKAGE__.'::check_unhandled_chunks', __LINE__, '$self->check_loaded('.$rev_names{$index}.')', "chunk $name is unhandled");
			} else {
				stkutils::debug::fail(__PACKAGE__.'::check_unhandled_chunks', __LINE__, '$self->check_loaded('.$index.')', "chunk $index is unhandled");
			}
		}
		$cf->{fh}->seek($size, 1);
		$offset = tell $cf->{fh};
	}
}
1;