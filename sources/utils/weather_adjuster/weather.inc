#ifndef WEATHER_INC
#define WEATHER_INC

#include "ltx.inc"
#include "shared.inc"


#include "weather.h"


#define M_STRDUP(s) (((s) != NULL) ? strdup(s) : NULL)


#define M_GET_VAL_OR_RETERR(sect_p, val_p, key) \
    do { \
        LTX_LINE *line_p = ltx_find_line((sect_p), (key)); \
        if (line_p == NULL) { \
            printf("Error: %s is missing in %s\n", (key), (sect_p)->name); \
            return PHASE_MALFORMED; \
        } \
        (val_p) = line_p->val; \
    } while (0)


static int wthr_phase_from_sect(PHASE *ph_p, LTX_SECT *sect_p)
{
    const char *key;
    const char *val;

    M_GET_VAL_OR_RETERR(sect_p, val, key = "flares");
    ph_p->flares = M_STRDUP(val);

    M_GET_VAL_OR_RETERR(sect_p, val, key = "sky_texture");
    ph_p->sky_index = get_sky_index(val);
    if (ph_p->sky_index < 0)
    {
#ifndef WTHR_IGNORE_SKY_TEXTURE_LOOKUP
        printf("Error: %s->%s = %s (lookup failed)\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
#endif
    }
    
    M_GET_VAL_OR_RETERR(sect_p, val, key = "sky_rotation");
    if (sscanf(val, "%f", &ph_p->sky_rotation) != 1)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "sky_color");
    if (sscanf(val, "%f, %f, %f",
               &ph_p->sky_color[0],
               &ph_p->sky_color[1],
               &ph_p->sky_color[2]) != 3)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "clouds_texture");
    ph_p->clouds_index = get_texture_index(val);
    if (ph_p->clouds_index < 0)
    {
#ifndef WTHR_IGNORE_CLOUDS_TEXTURE_LOOKUP
        printf("Error: %s->%s = %s (lookup failed)\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
#endif
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "clouds_color");
    if (sscanf(val, "%f, %f, %f, %f, %f",
               &ph_p->clouds_color[0],
               &ph_p->clouds_color[1],
               &ph_p->clouds_color[2],
               &ph_p->clouds_opacity,
               &ph_p->clouds_saturation) != 5)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "far_plane");
    if (sscanf(val, "%f", &ph_p->far_plane) != 1)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "fog_distance");
    if (sscanf(val, "%f", &ph_p->fog_distance) != 1)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "fog_color");
    if (sscanf(val, "%f, %f, %f",
               &ph_p->fog_color[0],
               &ph_p->fog_color[1],
               &ph_p->fog_color[2]) != 3)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "fog_density");
    if (sscanf(val, "%f", &ph_p->fog_density) != 1)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "rain_density");
    if (sscanf(val, "%f", &ph_p->rain_density) != 1)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "rain_color");
    if (sscanf(val, "%f, %f, %f",
               &ph_p->rain_color[0],
               &ph_p->rain_color[1],
               &ph_p->rain_color[2]) != 3)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, "thunderbolt");
    ph_p->thunderbolt = M_STRDUP(val);

    M_GET_VAL_OR_RETERR(sect_p, val, key = "bolt_period");
    if (sscanf(val, "%f", &ph_p->bolt_period) != 1)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "bolt_duration");
    if (sscanf(val, "%f", &ph_p->bolt_duration) != 1)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "wind_velocity");
    if (sscanf(val, "%f", &ph_p->wind_velocity) != 1)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "wind_direction");
    if (sscanf(val, "%f", &ph_p->wind_direction) != 1)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "ambient");
    if (sscanf(val, "%f, %f, %f",
               &ph_p->ambient[0],
               &ph_p->ambient[1],
               &ph_p->ambient[2]) != 3)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "lmap_color");
    if (sscanf(val, "%f, %f, %f",
               &ph_p->lmap_color[0],
               &ph_p->lmap_color[1],
               &ph_p->lmap_color[2]) != 3)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "hemi_color");
    if (sscanf(val, "%f, %f, %f",
               &ph_p->hemi_color[0],
               &ph_p->hemi_color[1],
               &ph_p->hemi_color[2]) != 3)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "sun_color");
    if (sscanf(val, "%f, %f, %f",
               &ph_p->sun_color[0],
               &ph_p->sun_color[1],
               &ph_p->sun_color[2]) != 3)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, key = "sun_dir");
    if (sscanf(val, "%f, %f", &ph_p->sun_dir[0], &ph_p->sun_dir[1]) != 2)
    {
        printf("Error: %s->%s = %s\n", ph_p->name, key, val);
        return PHASE_MALFORMED;
    }

    M_GET_VAL_OR_RETERR(sect_p, val, "env_ambient");
    ph_p->env_ambient = M_STRDUP(val);

    return PHASE_OK;
}


static PHASE *wthr_create_phase(const char *name)
{
    PHASE *ph_p = malloc(sizeof(PHASE));
    memset(ph_p, 0, sizeof(PHASE));
    ph_p->name = strdup(name);
    return ph_p;
}


static void wthr_destroy_phase(PHASE *ph_p)
{
    free(ph_p->name);

    if (ph_p->flares != NULL)
        free(ph_p->flares);

    if (ph_p->thunderbolt != NULL)
        free(ph_p->thunderbolt);

    if (ph_p->env_ambient != NULL)
        free(ph_p->env_ambient);

    free(ph_p);
}


static CYCLE* wthr_create_cycle(const char *sect_name)
{
    const char *start_p = sect_name + sizeof(SECT_NAME_START) - 1;
    const char *end_p = strstr(sect_name, SECT_NAME_END);
    CYCLE *c_p = malloc(sizeof(CYCLE));

    memset(c_p, 0, sizeof(CYCLE));

    c_p->sect_name = strdup(sect_name);

    if (end_p != NULL && end_p > start_p)
    {
        int name_len = end_p - start_p;
        c_p->name = malloc(name_len + 1);
        memcpy(c_p->name, start_p, name_len);
        c_p->name[name_len] = 0;
    }
    else
        c_p->name = strdup(sect_name);

    return c_p;
}


static void wthr_add_phase_to_cycle(CYCLE *c_p, PHASE *p_p, int index)
{
    p_p->time = index;
    c_p->phases[index] = p_p;
    c_p->num_phases++;
}


static void wthr_destroy_cycle(CYCLE *c_p)
{
    int i;

    free(c_p->name);
    free(c_p->sect_name);

    for (i = 0; i < MAX_PHASES_PER_CYCLE; i++)
    {
        if (c_p->phases[i] != NULL)
            free(c_p->phases[i]);
    }

    free(c_p);
}


static int wthr_is_cycle(LTX_SECT *sect_p)
{
    int i, h, m, s;

    if (memcmp(sect_p->name, SECT_NAME_START, sizeof(SECT_NAME_START) - 1) != 0)
        return 0;

    if (strstr(sect_p->name, SECT_NAME_END) == NULL)
        return 0;

    for (i = 0; i < sect_p->num_lines; i++)
    {
        if (sscanf(sect_p->line_array[i]->key, "%02d:%02d:%02d", &h, &m, &s) == 3)
            return 1;
    }

    return 0;
}


static int wthr_is_phase(LTX_SECT *sect_p)
{
    return (ltx_find_line(sect_p, "sky_texture") != NULL);
}


static int wthr_build_cycles_from_ltx(CYCLE *cycles[], int max_cycles, LTX_DATA *ltx_p)
{
    int i, j, num_cycles = 0;

    for (i = 0; i < ltx_p->num_sect; i++)
    {
        LTX_SECT *sect_p = ltx_p->sect_array[i];

        if (wthr_is_cycle(sect_p))
        {
            CYCLE *cycle_p = NULL;

            if (num_cycles >= max_cycles)
            {
                printf("Warning: max number of cycles (%d) reached, cannot load %s\n",
                       max_cycles, sect_p->name);

                break;
            }

            cycle_p = wthr_create_cycle(sect_p->name);

            /* Find and compile all phases of this cycle */

            for (j = 0; j < sect_p->num_lines; j++)
            {
                int h, m, s;
                const char *key = sect_p->line_array[j]->key;
                const char *val = sect_p->line_array[j]->val;

                if (val == NULL || sscanf(key, "%02d:%02d:%02d", &h, &m, &s) != 3)
                {
                    printf("Error: unexpected expression '%s' in section '%s'\n",
                           key, sect_p->name);
                }
                else if (s != 0)
                {
                    printf("Warning: in %s phase %s starts at non-zero second count\n",
                           sect_p->name, key);
                }
                else
                {
                    LTX_SECT *phase_sect_p = ltx_find_sect(ltx_p, val);

                    if (phase_sect_p == NULL || !wthr_is_phase(phase_sect_p))
                    {
                        printf("Warning: cannot find valid section for phase %s->%s = %s\n",
                               sect_p->name, key, val);
                    }
                    else 
                    {
                        PHASE *ph_p = wthr_create_phase(val);

                        if (wthr_phase_from_sect(ph_p, phase_sect_p) == PHASE_OK)
                        {
                            wthr_add_phase_to_cycle(cycle_p, ph_p, h * 60 + m);
                        }
                        else
                        {
                            printf("Error: bad phase '%s' in cycle '%s'\n", val, cycle_p->name);
                            wthr_destroy_phase(ph_p);
                        }
                    }
                }                
            }

            cycles[num_cycles] = cycle_p;

            num_cycles++;
        }
    }
    
    return num_cycles; 
}

#endif
