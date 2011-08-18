#ifndef SHARED_H
#define SHARED_H


#define SUCCESS 0
#define FAILURE 1


typedef struct
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
} RGB;


#define CUBE_TOP_INDEX    2 /* Top side of the skycube */
#define CUBE_BOTTOM_INDEX 3 /* The fog color is in the bottom of the cube */

#define CUBE_ALL_SIDES    1 /* All sides should be included in color calculation */
#define CUBE_EXCL_TOP     2 /* Exclude cube top from color calculations */
#define CUBE_EXCL_BOTTOM  4 /* Exclude bottom from color calculations */


typedef struct
{
    const char *name;
    RGB cube_rgb[6];
    RGB light_rgb[6];
} SKY_INFO;


typedef struct
{
    const char *name;
    RGB avg_rgb;
} TEXTURE_INFO;


#define MIN2(a, b) (((a) < (b)) ? (a) : (b))

#define MAX2(a, b) (((a) > (b)) ? (a) : (b))


#define SHOW_RGB(x) printf("%s = R:%03d G:%03d B:%03d\n", #x, x.r, x.g, x.b)


#define RGB_COMPONENT(rgb, x) ((x==0)?(rgb).r:((x==1)?(rgb).g:(rgb.b)))


#if 0
#define RGB_AVG(rgb) \
    ((unsigned char) ((299 * (rgb).r + 587 * (rgb).g + 114 * (rgb).b) / 1000))
#else
#define RGB_AVG(rgb) \
    ((unsigned char) ((3 * (rgb).r + 4 * (rgb).g + 2 * (rgb).b) / 9))
#endif


#define scale_ok(v) ((v)[0] <= 1.0 && (v)[1] <= 1.0 && (v)[2] <= 1.0)


#define scale_vec(vdst, vsrc, scale) \
    do {\
        (vdst)[0] = (vsrc)[0] * (scale);\
        (vdst)[1] = (vsrc)[1] * (scale);\
        (vdst)[2] = (vsrc)[2] * (scale);\
    } while(0)


#define avg_vec(vdst, v1, v2) \
    do {\
        (vdst)[0] = (v1)[0] + (v2)[0] / 2.0;\
        (vdst)[1] = (v1)[1] + (v2)[1] / 2.0;\
        (vdst)[2] = (v1)[2] + (v2)[2] / 2.0;\
    } while(0)


#define vec_avg_size(v) (((v)[0] + (v)[1] + (v)[2]) / 3.0)

#define is_null_vec(v) ((v)[0] == 0.0 && (v)[1] == 0.0 && (v)[2] == 0.0)

#define STR(s) (((s) != NULL) ? (s) : "")


#define strcmpbeg(str, beg) ((memcmp((str), (beg), sizeof(beg) - 1))


#endif
