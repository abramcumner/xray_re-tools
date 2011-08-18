#ifndef SHARED_INC
#define SHARED_INC

#include "shared.h"

#ifndef SWO3_SETTINGS
#include "sky_table.h"
#else
#include "sky_table_swo.h"
#endif

#include "texture_table.h"

static int get_sky_index(const char *skycube)
{
    int i;
    int num_skies = sizeof(sky_table) / sizeof(sky_table[0]);

    for (i = 0; i < num_skies; i++)
    {
        if (strcmpi(skycube, sky_table[i].name) == 0)
            return i;
    }
    return -1;
}

static int get_texture_index(const char *texture)
{
    int i;
    int num_textures = sizeof(texture_table) / sizeof(texture_table[0]);

    for (i = 0; i < num_textures; i++)
    {
        if (strcmpi(texture, texture_table[i].name) == 0)
            return i;
    }
    return -1;
}


/* Stolen from http://www.compuphase.com/cmetric.htm */
long color_distance(RGB e1, RGB e2)
{
  long r, g, b;
  long rmean;

  rmean = ( (int)e1.r + (int)e2.r ) / 2;
  r = (int)e1.r - (int)e2.r;
  g = (int)e1.g - (int)e2.g;
  b = (int)e1.b - (int)e2.b;
  return (((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8);
}


static RGB scale_rgb(RGB rgb1, float scale[3])
{
    RGB rgb2;
    rgb2.r = (unsigned char)((float)rgb1.r * scale[0]);
    rgb2.g = (unsigned char)((float)rgb1.g * scale[1]);
    rgb2.b = (unsigned char)((float)rgb1.b * scale[2]);

    return rgb2;
}

static RGB cube_avg_rgb(RGB cube[6], int mode)
{
    int i, r = 0, g = 0, b = 0;
    RGB rgb;

    for (i = 0; i < 6; i++)
    {
        if ((mode & CUBE_EXCL_TOP) && i == CUBE_TOP_INDEX)
            continue;

        if ((mode & CUBE_EXCL_BOTTOM) && i == CUBE_BOTTOM_INDEX)
            continue;

        r += cube[i].r;
        g += cube[i].g;
        b += cube[i].b;
    }

    if (mode & CUBE_EXCL_TOP)
        i--;

    if (mode & CUBE_EXCL_BOTTOM)
        i--;

    rgb.r = (unsigned char)(r / i);
    rgb.g = (unsigned char)(g / i);
    rgb.b = (unsigned char)(b / i);

    return rgb;
}


static const char* get_fname(const char* filepath)
{
    static char fname[_MAX_FNAME];
    _splitpath(filepath, NULL, NULL, fname, NULL);
    return fname;
}


#endif
