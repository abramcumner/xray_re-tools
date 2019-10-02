#define NOMINMAX
#include <windows.h>
#include "ogg_tools.h"
#include "xr_types.h"
#include "xr_file_system.h"
#include "xr_sound_thumbnail.h"

#include <vorbis/vorbisfile.h>

using namespace xray_re;

bool ogg_tools::check_paths() const
{
	bool status = true;
	check_path(PA_GAME_SOUNDS, status);
	check_path(PA_SOUNDS, status);
	return status;
}

#pragma region ogg utils
static int quiet = 1;
static int bits = 16;
static int endian = 0;
static int raw = 0;
static int sign = 1;
unsigned char headbuf[44]; /* The whole buffer */

#define WRITE_U32(buf, x) *(buf)     = (unsigned char)((x)&0xff);\
	*((buf)+1) = (unsigned char)(((x)>>8)&0xff);\
	*((buf)+2) = (unsigned char)(((x)>>16)&0xff);\
	*((buf)+3) = (unsigned char)(((x)>>24)&0xff);

#define WRITE_U16(buf, x) *(buf)     = (unsigned char)((x)&0xff);\
	*((buf)+1) = (unsigned char)(((x)>>8)&0xff);

/* Some of this based on ao/src/ao_wav.c */
int write_prelim_header(OggVorbis_File *vf, FILE *out, int64_t knownlength) {
	unsigned int size = 0x7fffffff;
	int channels = ov_info(vf,0)->channels;
	int samplerate = ov_info(vf,0)->rate;
	int bytespersec = channels*samplerate*bits/8;
	int align = channels*bits/8;
	int samplesize = bits;

	if(knownlength && knownlength*bits/8*channels < size)
		size = (unsigned int)(knownlength*bits/8*channels+44) ;

	memcpy(headbuf, "RIFF", 4);
	WRITE_U32(headbuf+4, size-8);
	memcpy(headbuf+8, "WAVE", 4);
	memcpy(headbuf+12, "fmt ", 4);
	WRITE_U32(headbuf+16, 16);
	WRITE_U16(headbuf+20, 1); /* format */
	WRITE_U16(headbuf+22, channels);
	WRITE_U32(headbuf+24, samplerate);
	WRITE_U32(headbuf+28, bytespersec);
	WRITE_U16(headbuf+32, align);
	WRITE_U16(headbuf+34, samplesize);
	memcpy(headbuf+36, "data", 4);
	WRITE_U32(headbuf+40, size - 44);

	if(fwrite(headbuf, 1, 44, out) != 44) {
		msg("ERROR: Failed to write Wave header: %s", strerror(errno));
		return 1;
	}

	return 0;
}

static void
permute_channels(char *in, char *out, int len, int channels, int bytespersample)
{
	int permute[6][6] = {{0}, {0,1}, {0,2,1}, {0,1,2,3}, {0,1,2,3,4}, 
	{0,2,1,5,3,4}};
	int i,j,k;
	int samples = len/channels/bytespersample;

	/* Can't handle, don't try */
	if (channels > 6)
		return;

	for (i=0; i < samples; i++) {
		for (j=0; j < channels; j++) {
			for (k=0; k < bytespersample; k++) {
				out[i*bytespersample*channels + 
					bytespersample*permute[channels-1][j] + k] = 
					in[i*bytespersample*channels + bytespersample*j + k];
			}
		}
	}
}

int rewrite_header(FILE *out, unsigned int written) 
{
	unsigned int length = written;

	length += 44;

	WRITE_U32(headbuf+4, length-8);
	WRITE_U32(headbuf+40, length-44);
	if(fseek(out, 0, SEEK_SET) != 0)
		return 1;

	if(fwrite(headbuf, 1, 44, out) != 44) {
		msg("ERROR: Failed to write Wave header: %s", strerror(errno));
		return 1;
	}
	return 0;
}
#pragma endregion

void ogg_tools::process_file(const std::string& path)
{
	std::string::size_type offs = path.size();
	if (offs < 4 || path.compare(offs -= 4, 4, ".ogg") != 0)
		return;

	xr_file_system& fs = xr_file_system::instance();
	std::string src; fs.resolve_path(PA_GAME_SOUNDS, path, src);
	std::string dest_path(path); dest_path.replace(offs, 4, ".wav");
	std::string dest; fs.resolve_path(PA_SOUNDS, dest_path, dest);
	std::string folder; fs.split_path(dest, &folder, NULL, NULL);
	fs.create_path(folder);
	
	//чтобы существующие звуки не перезаписывались
	if (fs.file_exist(dest.c_str())) {
		msg("skipping %s (already exists)", src.c_str());
		return;
	}


	OggVorbis_File vf;
	int bs = 0;
	char buf[8192], outbuf[8192];
	char *p_outbuf;
	int buflen = 8192;
	unsigned int written = 0;
	int ret;
	int64_t length = 0;
	int64_t done = 0;
	int size = 0;
	int seekable = 0;
	int percent = 0;
	int channels;
	int samplerate;

	FILE* in = fopen(src.c_str(), "rb");
	if(!in) {
		msg("ERROR: Failed to open input file: %s", strerror(errno));
		return;
	}
	FILE* out = fopen(dest.c_str(), "wb");
	if(!out) {
		msg("ERROR: Failed to open output file: %s", strerror(errno));
		return;
	}

	if (ov_open_callbacks(in, &vf, NULL, 0, OV_CALLBACKS_DEFAULT) < 0) {
		msg("ERROR: Failed to open input as Vorbis");
		fclose(in);
		return;
	}

	channels = ov_info(&vf,0)->channels;
	samplerate = ov_info(&vf,0)->rate;

	if(ov_seekable(&vf)) {
		int link;
		int chainsallowed = 0;
		for(link = 0; link < ov_streams(&vf); link++) {
			if(ov_info(&vf, link)->channels == channels && 
				ov_info(&vf, link)->rate == samplerate)
			{
				chainsallowed = 1;
			}
		}

		seekable = 1;
		if(chainsallowed)
			length = ov_pcm_total(&vf, -1);
		else
			length = ov_pcm_total(&vf, 0);
		size = bits/8 * channels;

		msg("Decoding \"%s\"", path.c_str());
	}

	if(!raw) {
		if(write_prelim_header(&vf, out, length)) {
			ov_clear(&vf);
			return;
		}
	}

	while((ret = ov_read(&vf, buf, buflen, endian, bits/8, sign, &bs)) != 0) {
		if(bs != 0) {
			vorbis_info *vi = ov_info(&vf, -1);
			if(channels != vi->channels || samplerate != vi->rate) {
				msg("Logical bitstreams with changing parameters are not supported");
				break;
			}
		}

		if(ret < 0 ) {
			if( !quiet ) {
				msg("WARNING: hole in data (%d)", ret);
			}
			continue;
		}

		if(channels > 2 && !raw) {
			/* Then permute! */
			permute_channels(buf, outbuf, ret, channels, bits/8);
			p_outbuf = outbuf;
		}
		else {
			p_outbuf = buf;
		}

		if(fwrite(p_outbuf, 1, ret, out) != ret) {
			msg("Error writing to file: %s", strerror(errno));
			ov_clear(&vf);
			return;
		}

		written += ret;
		if(!quiet && seekable) {
			done += ret/size;
			if((double)done/(double)length * 200. > (double)percent) {
				percent = (int)((double)done/(double)length *200);
				msg("\r\t[%5.1f%%]", (double)percent/2.);
			}
		}
	}

	if(seekable && !quiet)
		msg("\n");

	if(!raw)
		rewrite_header(out, written); /* We don't care if it fails, too late */

	if (*vf.vc->user_comments && *vf.vc->comment_lengths > 4) {
		xr_reader r(*vf.vc->user_comments, *vf.vc->comment_lengths);
		
		uint32_t version = r.r_u32();
		xr_assert(version >= 1 && version <= 3);

		xr_sound_thumbnail thm;
		thm.min_dist = r.r_float();
		thm.max_dist = r.r_float();
		if (version >= 2)
			thm.base_volume = r.r_float();
		else
			thm.base_volume = 1.f;
		thm.game_type = r.r_u32();
		if (version >= 3)
			thm.max_ai_dist = r.r_float();
		else
			thm.max_ai_dist = thm.max_dist;

		dest_path.replace(offs, 4, ".thm");
		xr_writer * w = fs.w_open(PA_SOUNDS, dest_path);
		if (w) {
			thm.save(*w);
			fs.w_close(w);
		}
	}
	else
		msg("Unknown vorbis comments: len=%d", vf.vc->comment_lengths);


	ov_clear(&vf);

	fclose(in);
	fclose(out);
}

void ogg_tools::process_folder(const std::string& path)
{
	WIN32_FIND_DATAA info;
	HANDLE h = FindFirstFileA((m_sounds + path + '*').c_str(), &info);
	if (h == INVALID_HANDLE_VALUE)
		return;
	do {
		if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (std::strcmp(info.cFileName, ".") != 0 && std::strcmp(info.cFileName, "..") != 0)
				process_folder((path + info.cFileName) + '\\');
		} else {
			process_file(path + info.cFileName);
		}
	} while (FindNextFileA(h, &info));
	FindClose(h);
}

#ifdef _CONSOLE
void ogg_tools::process(const cl_parser& cl)
{
	if (!check_paths())
		return;

	xr_file_system& fs = xr_file_system::instance();
	m_sounds = fs.resolve_path(PA_GAME_SOUNDS);

	process_folder();
}
#endif // _CONSOLE