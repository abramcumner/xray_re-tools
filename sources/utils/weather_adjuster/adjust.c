#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "weather.inc"


#define WARNING_NONE       0
#define WARNING_COLOR      1
#define WARNING_OVERBRIGHT 2
#define WARNING_DARK       4


/*
 * Raw chunk of .ltx data as table of text lines
 */
typedef struct
{
    char *name;
    int num_lines;
    int max_lines;
    char **lines;
} LTX_CHUNK;


static LTX_CHUNK* create_chunk(char *name)
{
    LTX_CHUNK *chunk_p = malloc(sizeof(LTX_CHUNK));

    memset(chunk_p, 0, sizeof(LTX_CHUNK));

    chunk_p->name = strdup(name);
    chunk_p->max_lines = 40;
    chunk_p->lines = malloc(chunk_p->max_lines * sizeof(char*));

    return chunk_p;
}


static void add_line_to_chunk(LTX_CHUNK *chunk_p, char *str_p)
{
    if (chunk_p->num_lines == chunk_p->max_lines)
    {
        chunk_p->max_lines += 40;
        chunk_p->lines = realloc(chunk_p->lines, chunk_p->max_lines * sizeof(char*));
    }

    chunk_p->lines[chunk_p->num_lines] = malloc(strlen(str_p) + 60);
    strcpy(chunk_p->lines[chunk_p->num_lines], str_p);
    chunk_p->num_lines++;
}


static void destroy_chunk(LTX_CHUNK *chunk_p)
{
    int i;

    free(chunk_p->name);

    for (i = 0; i < chunk_p->num_lines; i++)
        free(chunk_p->lines[i]);

    free(chunk_p->lines);
    free(chunk_p);
}


static void process_chunk(LTX_CHUNK *chunk_p, LTX_DATA *ltx_p, FILE *fpout)
{
    LTX_SECT *sect_p = ltx_find_sect(ltx_p, chunk_p->name);
    PHASE *ph_p = NULL;
    int i, sts;

    char *clouds_color_p = NULL;
    char *fog_color_p = NULL;
    char *rain_color_p = NULL;
    char *ambient_p = NULL;
    char *hemi_color_p = NULL;

    /* NOTE: lines[0] contains section name with brackets etc. */
    for (i = 1; i < chunk_p->num_lines; i++)
    {
        char *key_p = chunk_p->lines[i];
        char *eol_p = (char*)ltx_find_eol(key_p);
        char saved = *eol_p;
        char *str_p = NULL;

        for (; isspace(*key_p); key_p++);

        if (eol_p == key_p)
            continue;

        *eol_p = 0;

        if ((str_p = strstr(key_p, "clouds_color")) == key_p)
            clouds_color_p = strchr(str_p, '=') + 1;
        else if ((str_p = strstr(key_p, "fog_color")) == key_p)
            fog_color_p = strchr(str_p, '=') + 1;
        else if ((str_p = strstr(key_p, "rain_color")) == key_p)
            rain_color_p = strchr(str_p, '=') + 1;
        else if ((str_p = strstr(key_p, "ambient")) == key_p)
            ambient_p = strchr(str_p, '=') + 1;
        else if ((str_p = strstr(key_p, "hemi_color")) == key_p)
            hemi_color_p = strchr(str_p, '=') + 1;

        *eol_p = saved;
    }

    if (sect_p != NULL)
    {
        if (wthr_is_phase(sect_p))
        {
            ph_p = wthr_create_phase(sect_p->name);
            sts = wthr_phase_from_sect(ph_p, sect_p);
        }
        else
            sts = NOT_A_PHASE;
    }
    else
    {
        printf("ERROR: cannot find section '%s'\n", chunk_p->name);
        sts = NOT_A_PHASE;
    }

    if (sts == PHASE_OK)
    {
        /* int indoor = (strstr(ph_p->env_ambient, "indoor") != NULL); */
        int warning = WARNING_NONE;
        RGB light_rgb = cube_avg_rgb(sky_table[ph_p->sky_index].light_rgb, CUBE_EXCL_BOTTOM);
        RGB clouds_rgb = texture_table[ph_p->clouds_index].avg_rgb;
        RGB light_adj_rgb = scale_rgb(light_rgb, ph_p->sky_color);
        RGB fog_rgb = sky_table[ph_p->sky_index].cube_rgb[CUBE_BOTTOM_INDEX];

        float sun_weight, sky_weight;
        float max_cloud_factor = 1.0;
        float lightavg = 0.0;
        float sunavg = 0.0;
        float clouds_color[3];
        float fog_color[3];
        float rain_color[3];
        float ambient[3];
        float hemi_color[3];
        float light_total[3];

        for (i = 0; i < 3; i++)
        {
            sunavg   += ph_p->sun_color[i];
            lightavg += RGB_COMPONENT(light_adj_rgb, i);
        }
        sunavg   /= 3.0;
        lightavg /= (3.0 * 255.0);

        sun_weight = (sunavg > 0.0001) ? sunavg / (sunavg + lightavg) : 0.0;

        sky_weight = (lightavg > 0.0001) ? lightavg / (sunavg + lightavg) : 0.0;


        for (i = 0; i < 3; i++)
        {
            float light_component = RGB_COMPONENT(light_adj_rgb, i) / 255.0;
            float cloud_component = RGB_COMPONENT(clouds_rgb, i) / 255.0;
            float light_level     = sunavg * sun_weight + lightavg * sky_weight;

            if (cloud_component > 0.001)
            {
                clouds_color[i] = MAX2(light_level / cloud_component, 0.035 / cloud_component);

                if (clouds_color[i] > max_cloud_factor)
                    max_cloud_factor = clouds_color[i];
            }
            else
                clouds_color[i] = 0.0;

            fog_color[i]    = ph_p->sky_color[i] * RGB_COMPONENT(fog_rgb, i) / 255.0;
            hemi_color[i]   = light_component;


#ifdef SWO3_SETTINGS
            fog_color[i]    *= 0.83;
            rain_color[i]   = light_component * sky_weight + ph_p->sun_color[i] * sun_weight + 0.035;
            ambient[i]      = 0.05 * light_component;
#else
            rain_color[i]   = light_level + 0.03;
            ambient[i]      = 0.12 * (light_component + sun_weight * ph_p->sun_color[i]);
#endif

            light_total[i]  = light_component + ph_p->sun_color[i] + ambient[i];

            if (light_total[i] > 1.0)
                warning |= WARNING_OVERBRIGHT;
            else if (light_total[i] < 0.8 && ph_p->sun_color[i] > 0.0
                     && ph_p->time > 7 * 60
                     && ph_p->time < 20 * 60)
            {
                warning |= WARNING_DARK;
            }
        }

        for (i = 0; i < 3; i++)
            clouds_color[i] /= max_cloud_factor;

        /* Nights are allowed to be blue */
        if (!is_null_vec(ph_p->sun_color) 
            && (ph_p->time > 4 * 60 && ph_p->time < 23 * 60)
            && (light_total[0] < light_total[1] || light_total[1] < light_total[2]))
        {
            warning |= WARNING_COLOR;
        }

        if (warning != WARNING_NONE)
        {
            char warn_txt[100];
            char *str_p = warn_txt;

            if (warning & WARNING_COLOR)
                str_p += sprintf(str_p, "%sCOLOR", (str_p != warn_txt) ? ", " : "");

            if (warning & WARNING_OVERBRIGHT)
                str_p += sprintf(str_p, "%sOVERBRIGHT", (str_p != warn_txt) ? ", " : "");

            if (warning & WARNING_DARK)
                str_p += sprintf(str_p, "%sDARK", (str_p != warn_txt) ? ", " : "");
            
            printf("WARNING (%s): %s->light_total = %.4f, %.4f, %.4f\n", 
                       warn_txt, chunk_p->name, light_total[0], light_total[1], light_total[2]);
        }

#if 1
        printf("%20s = %03d, %03d, %03d\n",
               ph_p->name,
               (int)(light_total[0] * 255.0f),
               (int)(light_total[1] * 255.0f),
               (int)(light_total[2] * 255.0f));
#endif

        if (clouds_color_p)
        {
            sprintf(clouds_color_p,
                    " %.4f, %.4f, %.4f, %.3f, %.3f\n",
                    clouds_color[0],
                    clouds_color[1],
                    clouds_color[2],
                    ph_p->clouds_opacity,
                    ph_p->clouds_saturation);
        }

        if (fog_color_p)
            sprintf(fog_color_p, " %.4f, %.4f, %.4f\n", fog_color[0], fog_color[1], fog_color[2]);

        if (rain_color_p)
            sprintf(rain_color_p, " %.4f, %.4f, %.4f\n", rain_color[0], rain_color[1], rain_color[2]);

        if (hemi_color_p)
            sprintf(hemi_color_p, " %.4f, %.4f, %.4f\n", hemi_color[0], hemi_color[1], hemi_color[2]);

        if (ambient_p)
            sprintf(ambient_p, " %.4f, %.4f, %.4f\n", ambient[0], ambient[1], ambient[2]);
    }
    else if (sts != NOT_A_PHASE)
    {
        printf("Error %d compiling '%s'\n", sts, chunk_p->name);

        if (sect_p != NULL)
        {
            for (i = 0; i < sect_p->num_lines; i++)
            {
                if (sect_p->line_array[i]->val != NULL)
                    printf(" '%s' = '%s'\n", sect_p->line_array[i]->key, sect_p->line_array[i]->val);
                else
                    printf(" '%s'\n", sect_p->line_array[i]->key);
            }
        }
        else
        {
            for (i = 0; i < chunk_p->num_lines; i++)
                printf(" '%s'\n", chunk_p->lines[i]);
        }
    }

    if (ph_p != NULL)
        wthr_destroy_phase(ph_p);


    for (i = 0; i < chunk_p->num_lines; i++)
        fputs(chunk_p->lines[i], fpout);
}


static int adjust_weather(char *ltx_name, LTX_DATA *ltx_p)
{
    FILE *fpin;
    FILE *fpout;
    char bak_name[_MAX_PATH];
    LTX_CHUNK *chunk_p = NULL;

    sprintf(bak_name, "%s.bak", ltx_name);

    if (rename(ltx_name, bak_name) != 0)
    {
        printf("Error renaming %s to %s", ltx_name, bak_name);
        perror("");
        return FAILURE;
    }

    if ((fpin = fopen(bak_name, "r")) == NULL)
    {
        printf("Error opening file %s", bak_name);
        perror("");
        return FAILURE;
    }

    if ((fpout = fopen(ltx_name, "w")) == NULL)
    {
        printf("Error opening file %s", ltx_name);
        perror("");
        return FAILURE;
    }


    while (!feof(fpin))
    {
        char buf[MAX_FILE_LINE_LEN];
        char chunk_name[MAX_SECT_NAME_LEN];
        char *str_p = buf;

        if (fgets(buf, sizeof(buf), fpin) == NULL)
            break;

        for (; isspace(*str_p); str_p++);

        if (*str_p != 0 && sscanf(str_p, "[%[^]]s]", chunk_name) == 1)
        {
            if (chunk_p != NULL)
            {
                process_chunk(chunk_p, ltx_p, fpout);
                destroy_chunk(chunk_p);
            }
            chunk_p = create_chunk(chunk_name);
        }

        if (chunk_p != NULL)
        {
            add_line_to_chunk(chunk_p, buf);

            if (feof(fpin))
            {
                process_chunk(chunk_p, ltx_p, fpout);
                destroy_chunk(chunk_p);
                chunk_p = NULL;
            }
        }
        else
            fputs(buf, fpout);
    }

    if (chunk_p != NULL)
    {
        process_chunk(chunk_p, ltx_p, fpout);
        destroy_chunk(chunk_p);
    }

    fclose(fpin);
    fclose(fpout);

    return SUCCESS;
}


int main(int argc, char** argv)
{

    int i;

    LTX_DATA ltx;

    if (argc < 2)
    {
        printf("USAGE: %s file1 [file2 ...]\n", get_fname(argv[0]));
        return EXIT_FAILURE;
    }

#ifdef SWO3_SETTINGS
    puts(">>> Using SWO3 settings <<<");
#endif

    ltx_init_data(&ltx);

    for (i = 1; i < argc; i++)
    {
        if (ltx_load_file(&ltx, argv[i]) != LTX_SUCCESS)
        {
            printf("Error loading %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }
    ltx_complete_data(&ltx);

    for (i = 1; i < argc; i++)
    {
        printf("Adjusting %s\n", get_fname(argv[i]));
        if (adjust_weather(argv[i], &ltx) != SUCCESS)
        {
            printf("Error adjusting %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    ltx_clear_data(&ltx);

    return EXIT_SUCCESS;
}
