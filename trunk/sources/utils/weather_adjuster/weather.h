#ifndef WEATHER_H
#define WEATHER_H


#define MAX_SECT_NAME    80 /* Max length of section name string allowed */
#define MAX_TEXTURE_NAME 40 /* Max length of a texture, flare, etc. argument */


/* Assumed naming convention for cycles */
#define SECT_NAME_START "sect_"     
#define SECT_NAME_END   "_weather"


#define PHASE_OK         1
#define NOT_A_PHASE      2
#define PHASE_INCOMPLETE 3
#define PHASE_MALFORMED  4


typedef struct
{
    char *name;
    char *flares;
    int time;
    int sky_index;
    float sky_color[3];
    float sky_rotation;
    int clouds_index;
    float clouds_color[3];
    float clouds_opacity;
    float clouds_saturation;
    float far_plane;
    float fog_distance;
    float fog_color[3];
    float fog_density;
    float rain_density;
    float rain_color[3];
    char *thunderbolt;
    float bolt_period;
    float bolt_duration;
    float wind_velocity;
    float wind_direction;
    float ambient[3];
    float lmap_color[3];
    float hemi_color[3];
    float sun_color[3];
    float sun_dir[2];
    char *env_ambient;
} PHASE;


#define MAX_PHASES_PER_CYCLE (24 * 60) /* Only allow phases that start at XX:YY:00 */

typedef struct
{
    char *name;
    char *sect_name;
    int num_phases;
    PHASE *phases[MAX_PHASES_PER_CYCLE];
} CYCLE;


#endif
