#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#define ALLOW_ZERO_SIZE	0

static char dirname[256];

static const char *margin(int level)
{
	static char	spaces[40];
	memset(spaces, ' ', sizeof(spaces) - 1);
	spaces[sizeof(spaces) - 1] = 0;
	return spaces + sizeof(spaces) - 1 - level*2;
}

static void dump_chunks(int level, uint32_t id, const uint8_t *data, size_t size)
{
	const uint8_t	*p;
	size_t		offset;
	uint32_t	chunk_id, chunk_size;
	static uint32_t	ids[64];

	for (offset = 0;;) {
		if (size - offset < 8)
			goto leaf;
		p = data + offset;
		chunk_id = *(uint32_t *)p;
		chunk_size = *(uint32_t *)(p + 4);
		if (ALLOW_ZERO_SIZE && chunk_size == 0) {
printf("id=%8.8x\n", chunk_id);
			switch (chunk_id) {
			case 0x00000000:

			// detail_object.part
			case 0x00000001:

			// way.part, spawn.part, sound_src.part, sound_env.part,
			// shape.part, sector.part, scene_object.part, ps.part,
			// portal.part, group.part, glow.part
			case 0x00000003:

			// FSL_PORTALS in level
			case 0x00000004:
				break;

			// wallmark.part
			case 0x00000005:
				break;

			// something (light key frames) in 1098 level
			case 0x00000009:
				break;

			// 2215 spawn
			case 0x00000002:
				break;

			// level.game
			case 0x00001000:
			case 0x00002000:
				break;

			default:
				goto leaf;
			}
		}
		offset += 8 + chunk_size;
		if (offset == size)
			goto not_leaf;
		else if (offset > size)
			goto leaf;
	}

not_leaf:
	for (offset = 0; offset != size;) {
		p = data + offset;
		chunk_id = *(uint32_t *)p;
		chunk_size = *(uint32_t *)(p + 4);
		printf("%schunk id=%08x, size=%d\n", margin(level), chunk_id, chunk_size);
		ids[level + 1] = chunk_id;
		dump_chunks(level + 1, chunk_id, p + 8, chunk_size);
		offset += 8 + chunk_size;
	}
	return;

leaf:
	if (level == 0) {
		printf("no chunks in file\n");
	} else {
		FILE	*fp;
		char	filename[512], *p;
		int	i;

		p = filename + sprintf(filename, "%s\\", dirname);
		for (i = 1; i <= level; i++) {
			p += sprintf(p, "%04x%s", ids[i], i == level ? ".bin" : ",");
		}
		fp = fopen(filename, "wb+");
		fwrite(data, 1, size, fp);
		fclose(fp);
	}
}

static long flength(FILE *fp)
{
	long pos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	fseek(fp, pos, SEEK_SET);
	return len;
}

int main(int argc, char *argv[])
{
	FILE	*fp;
	long	len;
	uint8_t	*data;
	char	*filename;

	if (argc != 2) {
		fprintf(stderr, "usage: unidump <input_file>\n");
		return 1;
	}
	filename = argv[1];
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "can't open file '%s'\n", filename);
		return 1;
	}
	len = flength(fp);
	data = malloc(len);
	if (data == NULL) {
		fprintf(stderr, "can't alloc %ld bytes for data\n", len);
		return 1;
	}
	fread(data, 1, len, fp);
	fclose(fp);
	sprintf(dirname, "%s.out", filename);
	mkdir(dirname);
	dump_chunks(0, 0xffffffff, data, len);
	free(data);
	return 0;
}
