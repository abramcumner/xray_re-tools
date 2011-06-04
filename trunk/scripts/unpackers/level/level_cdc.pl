#!perl -w
#
#v 0.2 Last Edited: 03 June 2011
###########################################################
package level;
use strict;
use stkutils::chunked_file;
use stkutils::ini_file;
#use stkutils::lzhuf;
use Cwd;
sub new {
	my $class = shift;
	my $self = {};
	$self->{fsl_header} = {};
	$self->{fsl_shaders} = {};
	$self->{fsl_visuals} = {};
	$self->{fsl_portals} = {};
	$self->{fsl_light_dynamic} = {};
	$self->{fsl_glows} = {};
	$self->{fsl_sectors} = {};
	$self->{fsl_vertex_buffer} = {};
	$self->{fsl_index_buffer} = {};
	$self->{fsl_swis} = {};
	$self->{compressed} = {};
	bless($self, $class);
	return $self;
}
sub read {
	my $self = shift;
	my ($fn) = @_;
	
	my $fh = stkutils::chunked_file->new($fn, 'r') or die "$fn: $!\n";
	while (1) {
		my ($index, $size) = $fh->r_chunk_open();
		defined $index or last;
		my $data = $fh->r_chunk_data();
#		if ($index & 0x80000000) {
#			print "chunk compressed!\n"; 
#			my $dec_data = $self->decompress($data, $size); 
#			$index -= 0x80000000;
#			print "$index\n";
#			$data = $dec_data;
#		}
		SWITCH: {
			$index == 0x0 && do {last SWITCH;};
			$index == 0x1 && do {$self->fsl_header::read($data); last SWITCH;};
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_TEXTURES') && do {$self->fsl_textures::read($data); last SWITCH;};
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_SHADERS') && do {$self->fsl_shaders::read($data); last SWITCH;};
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_CFORM') && do {$self->fsl_cform::read($data); last SWITCH; };
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_LIGHT_KEY_FRAMES') && do {$self->fsl_light_key_frames::read($data); last SWITCH;};
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_SHADER_CONSTANT') && do {$self->fsl_shader_constant::read($data); last SWITCH;};
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_VISUALS') && do {$self->fsl_visuals::read($data); last SWITCH;};
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_PORTALS') && do {$self->fsl_portals::read($data); last SWITCH;};
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_LIGHT_DYNAMIC') && do {$self->fsl_light_dynamic::read($data); last SWITCH;};
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_GLOWS') && do {$self->fsl_glows::read($data); last SWITCH; };
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_SECTORS') && do {$self->fsl_sectors::read($data); last SWITCH;};
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_VB') && do {$self->fsl_vertex_buffer::read($data); last SWITCH;};
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_VB_OLD') && do {$self->fsl_vertex_buffer::read($data); last SWITCH;};
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_IB') && do {$self->fsl_index_buffer::read($data); last SWITCH;};
			(chunks::get_name($index, $self->{fsl_header}->{xrlc_version}) eq 'FSL_SWIS') && do {$self->fsl_swis::read($data); last SWITCH;};
			($index &0x80000000) && do {$index -= 0x80000000; $self->compressed::read($index, $data); last SWITCH; };
			printf "unexpected chunk %04x size %d\n", $index, $size;
			die;
		}
		$fh->r_chunk_close();
	}
}
sub decompress {
	my $self = shift;
	my ($data, $size) = @_;
	my $huff = stkutils::lzhuf->new($data);
	$huff->decode($size);
	return $huff->{dec_data};
}
sub write {
	my $self = shift;
	my ($fn) = @_;
	
	my $fh = stkutils::chunked_file->new($fn, 'w') or die "$fn: $!\n";
	$self->fsl_header::write($fh);
	if ($self->{fsl_header}->{xrlc_version} == 10 || $self->{fsl_header}->{xrlc_version} == 11) {
		if (defined $self->{fsl_portals}->{raw_data}) {	
			$self->fsl_portals::write($fh);
		} elsif (defined $self->{compressed}->{chunks::get_index('FSL_PORTALS', $self->{fsl_header}->{xrlc_version})}) {
			$self->compressed::write(chunks::get_index('FSL_PORTALS', $self->{fsl_header}->{xrlc_version}), $fh);
		}	
		if (defined $self->{fsl_cform}->{raw_data}) {	
			$self->fsl_cform::write($fh);
		} elsif (defined $self->{compressed}->{chunks::get_index('FSL_CFORM', $self->{fsl_header}->{xrlc_version})}) {
			$self->compressed::write(chunks::get_index('FSL_CFORM', $self->{fsl_header}->{xrlc_version}), $fh);
		}		
	} else {
		if (defined $self->{fsl_cform}->{raw_data}) {	
			$self->fsl_cform::write($fh);
		} elsif (defined $self->{compressed}->{chunks::get_index('FSL_CFORM', $self->{fsl_header}->{xrlc_version})}) {
			$self->compressed::write(chunks::get_index('FSL_CFORM', $self->{fsl_header}->{xrlc_version}), $fh);
		}		
		if (defined $self->{fsl_portals}->{raw_data}) {	
			$self->fsl_portals::write($fh);
		} elsif (chunks::get_index('FSL_PORTALS', $self->{fsl_header}->{xrlc_version}) && defined $self->{compressed}->{chunks::get_index('FSL_PORTALS', $self->{fsl_header}->{xrlc_version})}) {
			$self->compressed::write(chunks::get_index('FSL_PORTALS', $self->{fsl_header}->{xrlc_version}), $fh);
		}
	}	
	if (defined $self->{fsl_shader_constant}->{raw_data}) {
		$self->fsl_shader_constant::write($fh);	
	} elsif (chunks::get_index('FSL_SHADER_CONSTANT', $self->{fsl_header}->{xrlc_version}) && defined $self->{compressed}->{chunks::get_index('FSL_SHADER_CONSTANT', $self->{fsl_header}->{xrlc_version})}) {
		$self->compressed::write(chunks::get_index('FSL_SHADER_CONSTANT', $self->{fsl_header}->{xrlc_version}), $fh);
	}
	$self->fsl_light_dynamic::write($fh);
	if ($self->{fsl_header}->{xrlc_version} < 8) {
		if (defined $self->{fsl_light_key_frames}->{raw_data}) {
			$self->fsl_light_key_frames::write($fh);	
		} elsif (defined $self->{compressed}->{chunks::get_index('FSL_LIGHT_KEY_FRAMES', $self->{fsl_header}->{xrlc_version})}) {
			$self->compressed::write(chunks::get_index('FSL_LIGHT_KEY_FRAMES', $self->{fsl_header}->{xrlc_version}), $fh);
		}
	}
	$self->fsl_glows::write($fh);
	if (defined $self->{fsl_visuals}->{raw_data}) {
		$self->fsl_visuals::write($fh);
	} elsif (defined $self->{compressed}->{chunks::get_index('FSL_VISUALS', $self->{fsl_header}->{xrlc_version})}) {
		$self->compressed::write(chunks::get_index('FSL_VISUALS', $self->{fsl_header}->{xrlc_version}), $fh);
	}
	if (defined $self->{fsl_vertex_buffer}->{raw_data}) {
		$self->fsl_vertex_buffer::write($fh);	
	} elsif (chunks::get_index('FSL_VB', $self->{fsl_header}->{xrlc_version}) && defined $self->{compressed}->{chunks::get_index('FSL_VB', $self->{fsl_header}->{xrlc_version})}) {
		$self->compressed::write(chunks::get_index('FSL_VB', $self->{fsl_header}->{xrlc_version}), $fh);
	} elsif (defined $self->{compressed}->{chunks::get_index('FSL_VB_OLD', $self->{fsl_header}->{xrlc_version})}) {
		$self->compressed::write(chunks::get_index('FSL_VB_OLD', $self->{fsl_header}->{xrlc_version}), $fh);
	}
	if ($self->{fsl_header}->{xrlc_version} >= 12) {
		if (defined $self->{fsl_swis}->{raw_data}) {
			$self->fsl_swis::write($fh);
		} elsif (defined $self->{compressed}->{11}) {
			$self->compressed::write(11, $fh);
		}
	}
	if (defined $self->{fsl_index_buffer}->{raw_data}) {
		$self->fsl_index_buffer::write($fh);
	} elsif (chunks::get_index('FSL_IB', $self->{fsl_header}->{xrlc_version}) && defined $self->{compressed}->{chunks::get_index('FSL_IB', $self->{fsl_header}->{xrlc_version})}) {
		$self->compressed::write(chunks::get_index('FSL_IB', $self->{fsl_header}->{xrlc_version}), $fh);
	}	
	if (defined $self->{fsl_textures}->{count}) {
		$self->fsl_textures::write($fh);
	}
	$self->fsl_shaders::write($fh);
	if (defined $self->{fsl_sectors}->{raw_data}) {
		$self->fsl_sectors::write($fh);
	} elsif (defined $self->{compressed}->{chunks::get_index('FSL_SECTORS', $self->{fsl_header}->{xrlc_version})}) {
		$self->compressed::write(chunks::get_index('FSL_SECTORS', $self->{fsl_header}->{xrlc_version}), $fh);
	}
}
sub import {
	my $self = shift;
	my $imf = stkutils::ini_file->new('FSL_HEADER.ltx', 'r');
	$self->fsl_header::import($imf);
	$imf->close();
	$imf = stkutils::ini_file->new('FSL_SHADERS.ltx', 'r');
	$self->fsl_shaders::import($imf);
	$imf->close();
	$imf = stkutils::ini_file->new('FSL_LIGHT_DYNAMIC.ltx', 'r');
	$self->fsl_light_dynamic::import($imf);
	$imf->close();
	$imf = stkutils::ini_file->new('FSL_GLOWS.ltx', 'r');
	$self->fsl_glows::import($imf);
	$imf->close();
	if ($imf = IO::File->new('FSL_VISUALS.bin', 'r')) {
		binmode $imf;
		$self->fsl_visuals::import($imf);
		$imf->close();
	}
	if ($imf = IO::File->new('FSL_PORTALS.bin', 'r')) {
		binmode $imf;
		$self->fsl_portals::import($imf);
		$imf->close();
	}
	if ($imf = IO::File->new('FSL_CFORM.bin', 'r')) {
		binmode $imf;
		$self->fsl_cform::import($imf);
		$imf->close();
	}
	if ($imf = stkutils::ini_file->new('FSL_TEXTURES.ltx', 'r')) {
		$self->fsl_textures::import($imf);
		$imf->close();
	}
	if ($imf = IO::File->new('FSL_LIGHT_KEY_FRAMES.bin', 'r')) {
		binmode $imf;
		$self->fsl_light_key_frames::import($imf);
		$imf->close();
	}
	if ($imf = IO::File->new('FSL_SHADER_CONSTANT.bin', 'r')) {
		binmode $imf;
		$self->fsl_shader_constant::import($imf);
		$imf->close();
	}
	if ($imf = IO::File->new('FSL_SECTORS.bin', 'r')) {
		binmode $imf;
		$self->fsl_sectors::import($imf);
		$imf->close();
	}
	if ($imf = IO::File->new('FSL_VERTEX_BUFFER.bin', 'r')) {
		binmode $imf;
		$self->fsl_vertex_buffer::import($imf);
		$imf->close();
	}
	if ($imf = IO::File->new('FSL_INDEX_BUFFER.bin', 'r')) {
		binmode $imf;
		$self->fsl_index_buffer::import($imf);
		$imf->close();
	}
	if ($imf = IO::File->new('FSL_SWIS.bin', 'r')) {
		binmode $imf;
		$self->fsl_swis::import($imf);
		$imf->close();
	}
	my ($dirhandle, $compressed);
	opendir $dirhandle, getcwd() or die;
	while ($compressed = readdir $dirhandle) {
		if ($compressed =~ /^(COMPRESSED_)(\w+)(.bin)$/) {
			my $imf = IO::File->new($compressed, 'r');
			binmode $imf;
			$self->compressed::import($imf, $2);
			$imf->close();
		}
	}
	closedir $dirhandle;
}
sub export {
	my $self = shift;
	my $outf = IO::File->new('FSL_HEADER.ltx', 'w');
	$self->fsl_header::export($outf);
	$outf->close();
	$outf = IO::File->new('FSL_SHADERS.ltx', 'w');
	$self->fsl_shaders::export($outf);
	$outf->close();
	$outf = IO::File->new('FSL_LIGHT_DYNAMIC.ltx', 'w');
	$self->fsl_light_dynamic::export($outf);
	$outf->close();
	$outf = IO::File->new('FSL_GLOWS.ltx', 'w');
	$self->fsl_glows::export($outf);
	$outf->close();
	if (defined $self->{fsl_textures}->{count}) {	
		$outf = IO::File->new('FSL_TEXTURES.ltx', 'w');
		$self->fsl_textures::export($outf);
		$outf->close();
	}
	if (defined $self->{fsl_cform}->{raw_data}) {	
		$outf = IO::File->new('FSL_CFORM.bin', 'w');
		$self->fsl_cform::export($outf);
		$outf->close();
	}
	if (defined $self->{fsl_shader_constant}->{raw_data}) {	
		$outf = IO::File->new('FSL_SHADER_CONSTANT.bin', 'w');
		$self->fsl_shader_constant::export($outf);
		$outf->close();
	}
	if (defined $self->{fsl_light_key_frames}->{raw_data}) {	
		$outf = IO::File->new('FSL_LIGHT_KEY_FRAMES.bin', 'w');
		$self->fsl_light_key_frames::export($outf);
		$outf->close();
	}
	if (defined $self->{fsl_visuals}->{raw_data}) {
		$outf = IO::File->new('FSL_VISUALS.bin', 'w');
		$self->fsl_visuals::export($outf);
		$outf->close();
	}
	if (defined $self->{fsl_portals}->{raw_data}) {
		$outf = IO::File->new('FSL_PORTALS.bin', 'w');
		$self->fsl_portals::export($outf);
		$outf->close();
	}
	if (defined $self->{fsl_sectors}->{raw_data}) {
		$outf = IO::File->new('FSL_SECTORS.bin', 'w');
		$self->fsl_sectors::export($outf);
		$outf->close();
	}
	if (defined $self->{fsl_vertex_buffer}->{raw_data}) {
		$outf = IO::File->new('FSL_VERTEX_BUFFER.bin', 'w');
		$self->fsl_vertex_buffer::export($outf);
		$outf->close();
	}
	if (defined $self->{fsl_index_buffer}->{raw_data}) {
		$outf = IO::File->new('FSL_INDEX_BUFFER.bin', 'w');
		$self->fsl_index_buffer::export($outf);
		$outf->close();
	}
	if (defined $self->{fsl_swis}->{raw_data}) {
		$outf = IO::File->new('FSL_SWIS.bin', 'w');
		$self->fsl_swis::export($outf);
		$outf->close();
	}
	if (defined $self->{compressed}) {
		$self->compressed::export();
	}
}
###########################################################
package fsl_header;
use strict;
use stkutils::data_packet;
use stkutils::chunked_file;
sub read {
	my $self = shift;
	my ($data) = @_;
	my $packet = stkutils::data_packet->new($data);
	($self->{fsl_header}->{xrlc_version}, $self->{fsl_header}->{xrlc_quality}) = $packet->unpack('vv');
	if ($self->{fsl_header}->{xrlc_version} < 11) {
		@{$self->{fsl_header}->{name}} = $packet->unpack('Z*');
	} else {
		die unless $packet->length() == 0;
	}
}
sub write {
	my $self = shift;
	my ($cf) = @_;
	if ($self->{fsl_header}->{xrlc_version} > 10) {
		$cf->w_chunk(1, pack('vv', $self->{fsl_header}->{xrlc_version}, $self->{fsl_header}->{xrlc_quality}));
	} else {
		my $l = length($self->{fsl_header}->{name});
		my $zc = 123 - $l;
		$cf->w_chunk_open(1);
		$cf->w_chunk_data(pack('vvZ*', $self->{fsl_header}->{xrlc_version}, $self->{fsl_header}->{xrlc_quality}, $self->{fsl_header}->{name}));
		for (my $i = 0; $i < $zc; $i++) {
			$cf->w_chunk_data(pack('C', 0));
		}
		$cf->w_chunk_close();
	}
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	
	print $cf "[header]\n";
	print $cf "xrLC version = $self->{fsl_header}->{xrlc_version}\n";
	print $cf "xrLC quality = $self->{fsl_header}->{xrlc_quality}\n";
	print $cf "name = $self->{fsl_header}->{name}[0]\n" if $self->{fsl_header}->{xrlc_version} < 11;
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	$self->{fsl_header}->{xrlc_version} = $cf->value('header', 'xrLC version');
	$self->{fsl_header}->{xrlc_quality} = $cf->value('header', 'xrLC quality');
	$self->{fsl_header}->{name} = $cf->value('header', 'name') if $self->{fsl_header}->{xrlc_version} < 11;
}
#########################################################
package fsl_shaders;
use strict;
use stkutils::data_packet;
sub read {
	my $self = shift;
	my ($data) = @_;
	my $packet = stkutils::data_packet->new($data);
	($self->{fsl_shaders}->{count}) = $packet->unpack('V');
	@{$self->{fsl_shaders}->{shaders}} = $packet->unpack("(Z*)$self->{fsl_shaders}->{count}");
	die unless $packet->length() == 0;
}
sub write {
	my $self = shift;
	my ($cf) = @_;
	my $index = chunks::get_index('FSL_SHADERS', $self->{fsl_header}->{xrlc_version});
	$cf->w_chunk_open($index);
	$cf->w_chunk_data(pack('V', $self->{fsl_shaders}->{count}));
	for (my $i = 0; $i < $self->{fsl_shaders}->{count}; $i++) {
		$cf->w_chunk_data(pack('Z*', $self->{fsl_shaders}->{shaders}[$i]));
	}
	$cf->w_chunk_close();
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	
	for (my $i = 0; $i < $self->{fsl_shaders}->{count}; $i++) {
		print $cf "[$i]\n";
		if ($self->{fsl_header}->{xrlc_version} > 11) {
			if (!(@{$self->{fsl_shaders}->{shaders}}[$i] eq '')) {
				my ($shader, $texture) = split '/', @{$self->{fsl_shaders}->{shaders}}[$i];
				print $cf "shader = $shader\n";
				print $cf "texture = $texture\n\n";
			} else {
				print $cf "shader = @{$self->{fsl_shaders}->{shaders}}[$i]\n";
				print $cf "texture = @{$self->{fsl_shaders}->{shaders}}[$i]\n\n";
			}
		} else {
			print $cf "shader = @{$self->{fsl_shaders}->{shaders}}[$i]\n";
		}
	}
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	for (my $i = 0; $i < $#{$cf->{sections_list}} + 1; $i++) {
		my $shader = $cf->value($i, 'shader');
		if ($self->{fsl_header}->{xrlc_version} > 11) {
			my $texture = $cf->value($i, 'texture');
			if (!($shader eq $texture)) {
				$self->{fsl_shaders}->{shaders}[$i] = join('/', $shader, $texture);
			} else {
				$self->{fsl_shaders}->{shaders}[$i] = $shader;
			}
		} else {
			$self->{fsl_shaders}->{shaders}[$i] = $shader;
		}
	}
	$self->{fsl_shaders}->{count} = $#{$cf->{sections_list}} + 1;
}
###########################################################
package fsl_textures;
use strict;
use IO::File;
sub read {
	my $self = shift;
	my ($data) = @_;
	my $packet = stkutils::data_packet->new($data);
	($self->{fsl_textures}->{count}) = $packet->unpack('V');
	@{$self->{fsl_textures}->{textures}} = $packet->unpack("(Z*)$self->{fsl_textures}->{count}");
	die unless $packet->length() == 0;
}
sub write {
	my $self = shift;
	my ($cf) = @_;

	$cf->w_chunk_open(2);
	$cf->w_chunk_data(pack('V', $self->{fsl_textures}->{count}));
	for (my $i = 0; $i < $self->{fsl_textures}->{count}; $i++) {
		$cf->w_chunk_data(pack('Z*', $self->{fsl_textures}->{textures}[$i]));
	}
	$cf->w_chunk_close();
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	for (my $i = 0; $i < $self->{fsl_textures}->{count}; $i++) {
		print $cf "[$i]\n";
		print $cf "texture = @{$self->{fsl_textures}->{textures}}[$i]\n";
	}
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	for (my $i = 0; $i < $#{$cf->{sections_list}} + 1; $i++) {
		$self->{fsl_textures}->{textures}[$i] = $cf->value($i, 'texture');
	}
	$self->{fsl_textures}->{count} = $#{$cf->{sections_list}} + 1;
}
###########################################################
package fsl_cform;
use strict;
use IO::File;
sub read {
	my $self = shift;
	my ($data) = @_;
	$self->{fsl_cform}->{raw_data} = $data;
}
sub write {
	my $self = shift;
	my ($cf) = @_;
	my $index = chunks::get_index('FSL_SHADERS', $self->{fsl_header}->{xrlc_version});
	$cf->w_chunk($index, $self->{fsl_cform}->{raw_data});	
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	binmode $cf;
	$cf->write($self->{fsl_cform}->{raw_data}, length($self->{fsl_cform}->{raw_data}));
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	$cf->read($self->{fsl_cform}->{raw_data}, ($cf->stat())[7]);
}
###########################################################
package fsl_light_key_frames;
use strict;
use IO::File;
sub read {
	my $self = shift;
	my ($data) = @_;
	$self->{fsl_light_key_frames}->{raw_data} = $data;
}
sub write {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk(9, $self->{fsl_light_key_frames}->{raw_data});	
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	binmode $cf;
	$cf->write($self->{fsl_light_key_frames}->{raw_data}, length($self->{fsl_light_key_frames}->{raw_data}));
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	$cf->read($self->{fsl_light_key_frames}->{raw_data}, ($cf->stat())[7]);
}
###########################################################
package fsl_shader_constant;
use strict;
use IO::File;
sub read {
	my $self = shift;
	my ($data) = @_;
	$self->{fsl_shader_constant}->{raw_data} = $data;
}
sub write {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk(7, $self->{fsl_shader_constant}->{raw_data});	
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	binmode $cf;
	$cf->write($self->{fsl_shader_constant}->{raw_data}, length($self->{fsl_shader_constant}->{raw_data}));
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	$cf->read($self->{fsl_shader_constant}->{raw_data}, ($cf->stat())[7]);
}
###########################################################
package fsl_visuals;
use strict;
use IO::File;
sub read {
	my $self = shift;
	my ($data) = @_;
	$self->{fsl_visuals}->{raw_data} = $data;
}
sub write {
	my $self = shift;
	my ($cf) = @_;
	my $index = chunks::get_index('FSL_VISUALS', $self->{fsl_header}->{xrlc_version});
	$cf->w_chunk($index, $self->{fsl_visuals}->{raw_data});	
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	binmode $cf;
	$cf->write($self->{fsl_visuals}->{raw_data}, length($self->{fsl_visuals}->{raw_data}));
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	$cf->read($self->{fsl_visuals}->{raw_data}, ($cf->stat())[7]);
}
############################################################
package fsl_portals;
use strict;
use IO::File;
sub read {
	my $self = shift;
	my ($data) = @_;
	$self->{fsl_portals}->{raw_data} = $data;
}
sub write {
	my $self = shift;
	my ($cf) = @_;
	my $index = chunks::get_index('FSL_PORTALS', $self->{fsl_header}->{xrlc_version});
	$cf->w_chunk($index, $self->{fsl_portals}->{raw_data});	
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	binmode $cf;
	$cf->write($self->{fsl_portals}->{raw_data}, length($self->{fsl_portals}->{raw_data}));
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	$cf->read($self->{fsl_portals}->{raw_data}, ($cf->stat())[7]);
}
###########################################################
package fsl_light_dynamic;
use strict;
use IO::File;
use constant lt_names => {
	1	=> 'point',
	2	=> 'spot',
	3	=> 'directional',
};
use constant reverse_lt_names => {
	'point'	=> 1,
	'spot'	=> 2,
	'directional'	=> 3,
};
sub read {
	my $self = shift;
	my ($data) = @_;
	my $packet = stkutils::data_packet->new($data);
	if ($self->{fsl_header}->{xrlc_version} > 8) {
		$self->{fsl_light_dynamic}->{count} = $packet->length() / 0x6c;
		die unless $packet->length() % 0x6c == 0;
		for (my $i = 0; $i < $self->{fsl_light_dynamic}->{count}; $i++) {
			($self->{fsl_light_dynamic}->{$i}{controller_id},
			$self->{fsl_light_dynamic}->{$i}{type}) = $packet->unpack('VV');
			@{$self->{fsl_light_dynamic}->{$i}{diffuse}} = $packet->unpack('f4');
			@{$self->{fsl_light_dynamic}->{$i}{specular}} = $packet->unpack('f4');
			@{$self->{fsl_light_dynamic}->{$i}{ambient}} = $packet->unpack('f4');
			@{$self->{fsl_light_dynamic}->{$i}{position}} = $packet->unpack('f3');
			@{$self->{fsl_light_dynamic}->{$i}{direction}} = $packet->unpack('f3');
			@{$self->{fsl_light_dynamic}->{$i}{other}} = $packet->unpack('f7');
		}
		die unless $packet->length() == 0;
	} elsif ($self->{fsl_header}->{xrlc_version} > 5) {
		$self->{fsl_light_dynamic}->{count} = $packet->length() / 0xB0;
		die unless $packet->length() % 0xB0 == 0;
		for (my $i = 0; $i < $self->{fsl_light_dynamic}->{count}; $i++) {
			$self->{fsl_light_dynamic}->{$i}{type} = $packet->unpack('V');
			@{$self->{fsl_light_dynamic}->{$i}{diffuse}} = $packet->unpack('f4');
			@{$self->{fsl_light_dynamic}->{$i}{specular}} = $packet->unpack('f4');
			@{$self->{fsl_light_dynamic}->{$i}{ambient}} = $packet->unpack('f4');
			@{$self->{fsl_light_dynamic}->{$i}{position}} = $packet->unpack('f3');
			@{$self->{fsl_light_dynamic}->{$i}{direction}} = $packet->unpack('f3');
			@{$self->{fsl_light_dynamic}->{$i}{other}} = $packet->unpack('f7');
			($self->{fsl_light_dynamic}->{$i}{unk1},
			$self->{fsl_light_dynamic}->{$i}{unk2},
			$self->{fsl_light_dynamic}->{$i}{name}) = $packet->unpack('VVZ*');
			my $l = 63 - length($self->{fsl_light_dynamic}->{$i}{name});
			$self->{fsl_light_dynamic}->{$i}{garb} = $packet->unpack("C$l");
		}
		die unless $packet->length() == 0;
	} else {
		$self->{fsl_light_dynamic}->{count} = $packet->length() / 0x7c;
		die unless $packet->length() % 0x7c == 0;
		for (my $i = 0; $i < $self->{fsl_light_dynamic}->{count}; $i++) {
			$self->{fsl_light_dynamic}->{$i}{type} = $packet->unpack('V');
			@{$self->{fsl_light_dynamic}->{$i}{diffuse}} = $packet->unpack('f4');
			@{$self->{fsl_light_dynamic}->{$i}{specular}} = $packet->unpack('f4');
			@{$self->{fsl_light_dynamic}->{$i}{ambient}} = $packet->unpack('f4');
			@{$self->{fsl_light_dynamic}->{$i}{position}} = $packet->unpack('f3');
			@{$self->{fsl_light_dynamic}->{$i}{direction}} = $packet->unpack('f3');
			@{$self->{fsl_light_dynamic}->{$i}{other}} = $packet->unpack('f7');
			@{$self->{fsl_light_dynamic}->{$i}{unk}} = $packet->unpack('V5');
		}
		die unless $packet->length() == 0;
	}
}
sub write {
	my $self = shift;
	my ($cf) = @_;
	my $index = chunks::get_index('FSL_LIGHT_DYNAMIC', $self->{fsl_header}->{xrlc_version});
	$cf->w_chunk_open($index);
	if ($self->{fsl_header}->{xrlc_version} > 8) {
		for (my $i = 0; $i < $self->{fsl_light_dynamic}->{count}; $i++) {
			$cf->w_chunk_data(pack('VVf4f4f4f3f3f7', $self->{fsl_light_dynamic}->{$i}{controller_id}, $self->{fsl_light_dynamic}->{$i}{type}, @{$self->{fsl_light_dynamic}->{$i}{diffuse}}, @{$self->{fsl_light_dynamic}->{$i}{specular}}, @{$self->{fsl_light_dynamic}->{$i}{ambient}}, @{$self->{fsl_light_dynamic}->{$i}{position}}, @{$self->{fsl_light_dynamic}->{$i}{direction}}, @{$self->{fsl_light_dynamic}->{$i}{other}}));
		}
	} elsif ($self->{fsl_header}->{xrlc_version} > 5) {
		for (my $i = 0; $i < $self->{fsl_light_dynamic}->{count}; $i++) {
			$cf->w_chunk_data(pack('Vf4f4f4f3f3f7VVZ*', $self->{fsl_light_dynamic}->{$i}{type}, @{$self->{fsl_light_dynamic}->{$i}{diffuse}}, @{$self->{fsl_light_dynamic}->{$i}{specular}}, @{$self->{fsl_light_dynamic}->{$i}{ambient}}, @{$self->{fsl_light_dynamic}->{$i}{position}}, @{$self->{fsl_light_dynamic}->{$i}{direction}}, @{$self->{fsl_light_dynamic}->{$i}{other}}, $self->{fsl_light_dynamic}->{$i}{unk1}, $self->{fsl_light_dynamic}->{$i}{unk2}, $self->{fsl_light_dynamic}->{$i}{name}));
			for (my $i = 0; $i < (64 - length($self->{fsl_light_dynamic}->{$i}{name})); $i++) {
				$cf->w_chunk_data(pack('C', 0xED));
			}
		}
	} else {
		for (my $i = 0; $i < $self->{fsl_light_dynamic}->{count}; $i++) {
			$cf->w_chunk_data(pack('Vf4f4f4f3f3f7V5', $self->{fsl_light_dynamic}->{$i}{type}, @{$self->{fsl_light_dynamic}->{$i}{diffuse}}, @{$self->{fsl_light_dynamic}->{$i}{specular}}, @{$self->{fsl_light_dynamic}->{$i}{ambient}}, @{$self->{fsl_light_dynamic}->{$i}{position}}, @{$self->{fsl_light_dynamic}->{$i}{direction}}, @{$self->{fsl_light_dynamic}->{$i}{other}}, @{$self->{fsl_light_dynamic}->{$i}{unk}}));
		}
	}
	$cf->w_chunk_close();
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	for (my $i = 0; $i < $self->{fsl_light_dynamic}->{count}; $i++) {
		print $cf "[$i]\n";
		print $cf "controller_id = $self->{fsl_light_dynamic}->{$i}{controller_id}\n" if defined $self->{fsl_light_dynamic}->{$i}{controller_id};
		printf $cf "type = %s\n", lt_names->{$self->{fsl_light_dynamic}->{$i}{type}};
		printf $cf "diffuse = %f, %f, %f, %f\n", @{$self->{fsl_light_dynamic}->{$i}{diffuse}};
		printf $cf "specular = %f, %f, %f, %f\n", @{$self->{fsl_light_dynamic}->{$i}{specular}};
		printf $cf "ambient = %f, %f, %f, %f\n", @{$self->{fsl_light_dynamic}->{$i}{ambient}};
		printf $cf "position = %f, %f, %f\n", @{$self->{fsl_light_dynamic}->{$i}{position}};
		printf $cf "direction = %f, %f, %f\n", @{$self->{fsl_light_dynamic}->{$i}{direction}};
		printf $cf "range = %f\n", @{$self->{fsl_light_dynamic}->{$i}{other}}[0];
		printf $cf "falloff = %f\n", @{$self->{fsl_light_dynamic}->{$i}{other}}[1];
		printf $cf "attenuation0 = %f\n", @{$self->{fsl_light_dynamic}->{$i}{other}}[2];
		printf $cf "attenuation1 = %f\n", @{$self->{fsl_light_dynamic}->{$i}{other}}[3];
		printf $cf "attenuation2 = %f\n", @{$self->{fsl_light_dynamic}->{$i}{other}}[4];
		printf $cf "theta = %f\n", @{$self->{fsl_light_dynamic}->{$i}{other}}[5];
		printf $cf "phi = %f\n", @{$self->{fsl_light_dynamic}->{$i}{other}}[6];
		print $cf "unk1 = $self->{fsl_light_dynamic}->{$i}{unk1}\n" if defined $self->{fsl_light_dynamic}->{$i}{unk1};
		print $cf "unk2 = $self->{fsl_light_dynamic}->{$i}{unk2}\n" if defined $self->{fsl_light_dynamic}->{$i}{unk2};
		print $cf "name = $self->{fsl_light_dynamic}->{$i}{name}\n" if defined $self->{fsl_light_dynamic}->{$i}{name};
		if (defined @{$self->{fsl_light_dynamic}->{$i}{unk}}) {
			printf $cf "unk_0 = %s\n", @{$self->{fsl_light_dynamic}->{$i}{unk}}[0];
			printf $cf "unk_1 = %s\n", @{$self->{fsl_light_dynamic}->{$i}{unk}}[1];
			printf $cf "unk_2 = %s\n", @{$self->{fsl_light_dynamic}->{$i}{unk}}[2];
			printf $cf "unk_3 = %s\n", @{$self->{fsl_light_dynamic}->{$i}{unk}}[3];
			printf $cf "unk_4 = %s\n\n", @{$self->{fsl_light_dynamic}->{$i}{unk}}[4];
		}
	}
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	for (my $i = 0; $i < $#{$cf->{sections_list}} + 1; $i++) {
		$self->{fsl_light_dynamic}->{$i}{controller_id} = $cf->value($i, 'controller_id');
		$self->{fsl_light_dynamic}->{$i}{type} = reverse_lt_names->{$cf->value($i, 'type')};
		@{$self->{fsl_light_dynamic}->{$i}{diffuse}} = split(',', $cf->value($i, 'diffuse'));
		@{$self->{fsl_light_dynamic}->{$i}{specular}} = split(',', $cf->value($i, 'specular'));
		@{$self->{fsl_light_dynamic}->{$i}{ambient}} = split(',', $cf->value($i, 'ambient'));
		@{$self->{fsl_light_dynamic}->{$i}{position}} = split(',', $cf->value($i, 'position'));
		@{$self->{fsl_light_dynamic}->{$i}{direction}} = split(',', $cf->value($i, 'direction'));
		$self->{fsl_light_dynamic}->{$i}{other}[0] = $cf->value($i, 'range');
		$self->{fsl_light_dynamic}->{$i}{other}[1] = $cf->value($i, 'falloff');
		$self->{fsl_light_dynamic}->{$i}{other}[2] = $cf->value($i, 'attenuation0');
		$self->{fsl_light_dynamic}->{$i}{other}[3] = $cf->value($i, 'attenuation1');
		$self->{fsl_light_dynamic}->{$i}{other}[4] = $cf->value($i, 'attenuation2');
		$self->{fsl_light_dynamic}->{$i}{other}[5] = $cf->value($i, 'theta');
		$self->{fsl_light_dynamic}->{$i}{other}[6] = $cf->value($i, 'phi');
		$self->{fsl_light_dynamic}->{$i}{unk1} = $cf->value($i, 'unk1');
		$self->{fsl_light_dynamic}->{$i}{unk2} = $cf->value($i, 'unk2');
		$self->{fsl_light_dynamic}->{$i}{name} = $cf->value($i, 'name');
		$self->{fsl_light_dynamic}->{$i}{unk}[0] = $cf->value($i, 'unk_0');
		$self->{fsl_light_dynamic}->{$i}{unk}[1] = $cf->value($i, 'unk_1');
		$self->{fsl_light_dynamic}->{$i}{unk}[2] = $cf->value($i, 'unk_2');
		$self->{fsl_light_dynamic}->{$i}{unk}[3] = $cf->value($i, 'unk_3');
		$self->{fsl_light_dynamic}->{$i}{unk}[4] = $cf->value($i, 'unk_4');
	}
	$self->{fsl_light_dynamic}->{count} = $#{$cf->{sections_list}} + 1;
}
###########################################################
package fsl_glows;
use strict;
use IO::File;
sub read {
	my $self = shift;
	my ($data) = @_;
	my $packet = stkutils::data_packet->new($data);
	if ($self->{fsl_header}->{xrlc_version} > 11) {
		$self->{fsl_glows}->{count} = $packet->length() / 0x12;
		die unless $packet->length() % 0x12 == 0;
	} else {
		$self->{fsl_glows}->{count} = $packet->length() / 0x18;
		die unless $packet->length() % 0x18 == 0;
	}
	for (my $i = 0; $i < $self->{fsl_glows}->{count}; $i++) {
		(${$self->{fsl_glows}->{$i}{position}}[0],
		${$self->{fsl_glows}->{$i}{position}}[1],
		${$self->{fsl_glows}->{$i}{position}}[2],
		$self->{fsl_glows}->{$i}{radius}) = $packet->unpack('ffff');
		if ($self->{fsl_header}->{xrlc_version} <= 11) {
			($self->{fsl_glows}->{$i}{texture_index},
			$self->{fsl_glows}->{$i}{shader_index})	= $packet->unpack('VV');
		} else {
			$self->{fsl_glows}->{$i}{shader_index} = $packet->unpack('v');
		}
	}
	die unless $packet->length() == 0;
}
sub write {
	my $self = shift;
	my ($cf) = @_;
	my $index = chunks::get_index('FSL_GLOWS', $self->{fsl_header}->{xrlc_version});
	$cf->w_chunk_open($index);
	if ($self->{fsl_header}->{xrlc_version} <= 11) {
		for (my $i = 0; $i < $self->{fsl_glows}->{count}; $i++) {
			$cf->w_chunk_data(pack('f3fVV', @{$self->{fsl_glows}->{$i}{position}}, $self->{fsl_glows}->{$i}{radius}, $self->{fsl_glows}->{$i}{texture_index}, $self->{fsl_glows}->{$i}{shader_index}));
		}
	} else {
		for (my $i = 0; $i < $self->{fsl_glows}->{count}; $i++) {
			$cf->w_chunk_data(pack('f3fv', @{$self->{fsl_glows}->{$i}{position}}, $self->{fsl_glows}->{$i}{radius}, $self->{fsl_glows}->{$i}{shader_index}));
		}
	
	}
	$cf->w_chunk_close();
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	
	for (my $i = 0; $i < $self->{fsl_glows}->{count}; $i++) {
		print $cf "[$i]\n";
		printf $cf "position = %f,%f,%f\n", @{$self->{fsl_glows}->{$i}{position}}[0..2];
		printf $cf "radius = %f\n", $self->{fsl_glows}->{$i}{radius};
		print $cf "texture_index = $self->{fsl_glows}->{$i}{texture_index}\n" if $self->{fsl_header}->{xrlc_version} <= 11;
		print $cf "shader_index = $self->{fsl_glows}->{$i}{shader_index}\n\n";
	}
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	for (my $i = 0; $i < $#{$cf->{sections_list}} + 1; $i++) {
		@{$self->{fsl_glows}->{$i}{position}} = split(',', $cf->value($i, 'position'));
		$self->{fsl_glows}->{$i}{radius} = $cf->value($i, 'radius');
		$self->{fsl_glows}->{$i}{texture_index} = $cf->value($i, 'texture_index') if $self->{fsl_header}->{xrlc_version} <= 11;
		$self->{fsl_glows}->{$i}{shader_index} = $cf->value($i, 'shader_index');
	}
	$self->{fsl_glows}->{count} = $#{$cf->{sections_list}} + 1;
}
###########################################################
package fsl_sectors;
use strict;
use IO::File;
sub read {
	my $self = shift;
	my ($data) = @_;
	$self->{fsl_sectors}->{raw_data} = $data;
}
sub write {
	my $self = shift;
	my ($cf) = @_;
	my $index = chunks::get_index('FSL_SECTORS', $self->{fsl_header}->{xrlc_version});
	$cf->w_chunk($index, $self->{fsl_sectors}->{raw_data});	
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	binmode $cf;
	$cf->write($self->{fsl_sectors}->{raw_data}, length($self->{fsl_sectors}->{raw_data}));
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	$cf->read($self->{fsl_sectors}->{raw_data}, ($cf->stat())[7]);
}
###########################################################
package fsl_vertex_buffer;
use strict;
use IO::File;
sub read {
	my $self = shift;
	my ($data) = @_;
	$self->{fsl_vertex_buffer}->{raw_data} = $data;
}
sub write {
	my $self = shift;
	my ($cf) = @_;
	my $index = chunks::get_index('FSL_VB', $self->{fsl_header}->{xrlc_version});
	$cf->w_chunk($index, $self->{fsl_vertex_buffer}->{raw_data});	
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	binmode $cf;
	$cf->write($self->{fsl_vertex_buffer}->{raw_data}, length($self->{fsl_vertex_buffer}->{raw_data}));
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	$cf->read($self->{fsl_vertex_buffer}->{raw_data}, ($cf->stat())[7]);
}
###########################################################
package fsl_index_buffer;
use strict;
use IO::File;
sub read {
	my $self = shift;
	my ($data) = @_;
	$self->{fsl_index_buffer}->{raw_data} = $data;
}
sub write {
	my $self = shift;
	my ($cf) = @_;
	my $index = chunks::get_index('FSL_IB', $self->{fsl_header}->{xrlc_version});
	$cf->w_chunk($index, $self->{fsl_index_buffer}->{raw_data});	
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	binmode $cf;
	$cf->write($self->{fsl_index_buffer}->{raw_data}, length($self->{fsl_index_buffer}->{raw_data}));
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	$cf->read($self->{fsl_index_buffer}->{raw_data}, ($cf->stat())[7]);
}
###########################################################
package fsl_swis;
use strict;
use IO::File;
sub read {
	my $self = shift;
	my ($data) = @_;
	$self->{fsl_swis}->{raw_data} = $data;
}
sub write {
	my $self = shift;
	my ($cf) = @_;
	$cf->w_chunk(11, $self->{fsl_swis}->{raw_data});	
}
sub export {
	my $self = shift;
	my ($cf) = @_;
	binmode $cf;
	$cf->write($self->{fsl_swis}->{raw_data}, length($self->{fsl_swis}->{raw_data}));
}
sub import {
	my $self = shift;
	my ($cf) = @_;
	$cf->read($self->{fsl_swis}->{raw_data}, ($cf->stat())[7]);
}
###########################################################
package compressed;
use strict;
use IO::File;
sub read {
	my $self = shift;
	my ($index, $data) = @_;
	$self->{compressed}->{$index} = $data;
}
sub write {
	my $self = shift;
	my ($index, $cf) = @_;
	my $ind = 0x80000000 + $index;
	$cf->w_chunk($ind, $self->{compressed}->{$index});	
}
sub export {
	my $self = shift;
	for (my $i = 0; $i < 13; $i++) {
		if (defined $self->{compressed}->{$i}) {
			my $outf = IO::File->new('COMPRESSED_'.chunks::get_name($i, $self->{fsl_header}->{xrlc_version}).'.bin', 'w');
			binmode $outf;
			$outf->write($self->{compressed}->{$i}, length($self->{compressed}->{$i}));
			$outf->close();
		}
	}
}
sub import {
	my $self = shift;
	my ($cf, $name) = @_;
	$cf->read($self->{compressed}->{chunks::get_index($name, $self->{fsl_header}->{xrlc_version})}, ($cf->stat())[7]);
}
###########################################################
package chunks;
use constant chunk_table => (
	{name => 'FSL_HEADER', version => 0, chunk_index => 0x1},
	{name => 'FSL_TEXTURES', version => 0, chunk_index => 0x2},
	{name => 'FSL_SHADERS', version => 5, chunk_index => 0x2},
	{name => 'FSL_SHADERS', version => 0, chunk_index => 0x3},
	{name => 'FSL_VISUALS', version => 5, chunk_index => 0x3},
	{name => 'FSL_VISUALS', version => 0, chunk_index => 0x4},
	{name => 'FSL_PORTALS', version => 9, chunk_index => 0x4},
	{name => 'FSL_PORTALS', version => 5, chunk_index => 0x6},
	{name => 'FSL_PORTALS', version => 0, chunk_index => 0x7},
	{name => 'FSL_CFORM', version => 5, chunk_index => 0x5},
	{name => 'FSL_CFORM', version => 0, chunk_index => 0x6},
	{name => 'FSL_SHADER_CONSTANT', version => 8, chunk_index => 0x7},
	{name => 'FSL_LIGHT_KEY_FRAMES', version => 0, chunk_index => 0x9},
	{name => 'FSL_LIGHT_DYNAMIC', version => 9, chunk_index => 0x6},
	{name => 'FSL_LIGHT_DYNAMIC', version => 8, chunk_index => 0x8},
	{name => 'FSL_LIGHT_DYNAMIC', version => 5, chunk_index => 0x7},
	{name => 'FSL_LIGHT_DYNAMIC', version => 0, chunk_index => 0x8},
	{name => 'FSL_GLOWS', version => 9, chunk_index => 0x7},
	{name => 'FSL_GLOWS', version => 5, chunk_index => 0x9},
	{name => 'FSL_GLOWS', version => 0, chunk_index => 0xa},
	{name => 'FSL_SECTORS', version => 9, chunk_index => 0x8},
	{name => 'FSL_SECTORS', version => 5, chunk_index => 0xa},
	{name => 'FSL_SECTORS', version => 0, chunk_index => 0xb},
	{name => 'FSL_VB', version => 12, chunk_index => 0x9},
	{name => 'FSL_VB', version => 9, chunk_index => 0xa},
	{name => 'FSL_VB', version => 8, chunk_index => 0xc},
	{name => 'FSL_VB_OLD', version => 5, chunk_index => 0x4},
	{name => 'FSL_VB_OLD', version => 0, chunk_index => 0x5},
	{name => 'FSL_IB', version => 12, chunk_index => 0xa},
	{name => 'FSL_IB', version => 9, chunk_index => 0x9},
	{name => 'FSL_IB', version => 8, chunk_index => 0xb},
	{name => 'FSL_SWIS', version => 9, chunk_index => 0xb},
);
use constant reverse_chunk_table => (
	{name => 'FSL_HEADER', version => 0, chunk_index => 0x1},
	{name => 'FSL_SHADERS', version => 5, chunk_index => 0x2},
	{name => 'FSL_TEXTURES', version => 0, chunk_index => 0x2},
	{name => 'FSL_VISUALS', version => 5, chunk_index => 0x3},
	{name => 'FSL_SHADERS', version => 0, chunk_index => 0x3},
	{name => 'FSL_PORTALS', version => 9, chunk_index => 0x4},
	{name => 'FSL_VB_OLD', version => 5, chunk_index => 0x4},
	{name => 'FSL_VISUALS', version => 0, chunk_index => 0x4},
	{name => 'FSL_CFORM', version => 5, chunk_index => 0x5},
	{name => 'FSL_VB_OLD', version => 0, chunk_index => 0x5},
	{name => 'FSL_LIGHT_DYNAMIC', version => 9, chunk_index => 0x6},
	{name => 'FSL_PORTALS', version => 5, chunk_index => 0x6},
	{name => 'FSL_CFORM', version => 0, chunk_index => 0x6},
	{name => 'FSL_GLOWS', version => 9, chunk_index => 0x7},
	{name => 'FSL_SHADER_CONSTANT', version => 8, chunk_index => 0x7},
	{name => 'FSL_LIGHT_DYNAMIC', version => 5, chunk_index => 0x7},
	{name => 'FSL_PORTALS', version => 0, chunk_index => 0x7},
	{name => 'FSL_SECTORS', version => 9, chunk_index => 0x8},
	{name => 'FSL_LIGHT_DYNAMIC', version => 8, chunk_index => 0x8},
	{name => 'FSL_LIGHT_DYNAMIC', version => 0, chunk_index => 0x8},
	{name => 'FSL_VB', version => 12, chunk_index => 0x9},
	{name => 'FSL_IB', version => 9, chunk_index => 0x9},
	{name => 'FSL_GLOWS', version => 5, chunk_index => 0x9},
	{name => 'FSL_LIGHT_KEY_FRAMES', version => 0, chunk_index => 0x9},
	{name => 'FSL_IB', version => 12, chunk_index => 0xa},
	{name => 'FSL_VB', version => 9, chunk_index => 0xa},
	{name => 'FSL_SECTORS', version => 5, chunk_index => 0xa},
	{name => 'FSL_GLOWS', version => 0, chunk_index => 0xa},
	{name => 'FSL_SWIS', version => 9, chunk_index => 0xb},
	{name => 'FSL_IB', version => 5, chunk_index => 0xb},
	{name => 'FSL_SECTORS', version => 0, chunk_index => 0xb},
	{name => 'FSL_VB', version => 8, chunk_index => 0xc},
);
sub get_index {
	foreach my $chunk (chunk_table) {
		if (($_[0] eq $chunk->{name}) && ($_[1] > $chunk->{version})) {
			return $chunk->{chunk_index};
		}
	}
	return undef;
}
sub get_name {
	if ($_[0] & 0x80000000) {
		return 'none';
	}
	foreach my $chunk (reverse_chunk_table) {
		if (($_[0] == $chunk->{chunk_index}) && ($_[1] > $chunk->{version})) {
			return $chunk->{name};
		}
	}
	return undef;
}
#########################################################
package main;

use strict;
use Getopt::Long;
use IO::File;
use File::Path;
use Cwd;

sub usage {
	return <<END
level compiiler/decompiler
Usage: lcdc [-d input_file] [-o outdir]
       lcdc [-c input_dir] [-o outfile]
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
	my $file = level->new();
	print "reading level...\n";
	$file->read($input_file);
	defined $out && do {
		File::Path::mkpath($out, 0);
		chdir $out or die "cannot change path to $out\n";
	};
	print "exporting level...\n";
	$file->export();	
	print "done!\n";
} else {
	die "bad params\n" if (defined $input_file);
	if (defined $src_dir) {
		chdir $src_dir or die "cannot change dir to $src_dir\n";
	}
	my $file = level->new();
	$out = 'level.new' unless defined $out;
	print "importing $src_dir...\n";
	$file->import();
	chdir $work_dir;
	print "writing $out...\n";
	$file->write($out);
	print "done!\n";
}
###########################################################