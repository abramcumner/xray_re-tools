#ifndef LTX_INC
#define LTX_INC

#include "ltx.h"



/* Hash function using multipy by 31 */
static unsigned int ltx_hash(const char *key, int key_len)
{
    const char *end_p = key + key_len;
    register unsigned int h = *key++;

    while (key != end_p && *key != 0)
    {
        unsigned int g;
        h = (h << 5) + *key++;
        if ((g = h & 0xf0000000) != 0)
            h = (h ^ (g >> 24) ^ g);
    }
    return h;
}


static int ltx_cmp_line(const void* item1, const void* item2)
{
    return (*(LTX_LINE **)item1)->sequence - (*(LTX_LINE **)item2)->sequence;
}


static int ltx_cmp_sect(const void* item1, const void* item2)
{
    return (*(LTX_SECT **)item1)->sequence - (*(LTX_SECT **)item2)->sequence;
}


/* Create a line entry from scratch */
static LTX_LINE* ltx_create_line(const char *key, 
                                 unsigned short key_len,
                                 const char *val,
                                 unsigned short val_len)
{
    LTX_LINE *line_p = calloc(sizeof(LTX_LINE), 1);

    line_p->key_len = key_len;
    line_p->val_len = val_len;

    line_p->key = calloc(key_len + 1, 1);
    memcpy(line_p->key, key, key_len);

    if (val != NULL && val_len > 0)
    {
        line_p->val = calloc(val_len + 1, 1);
        memcpy(line_p->val, val, val_len);
    }

    return line_p;
}


/* Create a line that inherits from an existing line */
static LTX_LINE *ltx_create_inherited_line(LTX_LINE *super_p)
{
    LTX_LINE *line_p = calloc(sizeof(LTX_LINE), 1);

    line_p->super_p = super_p;
    line_p->inherited = 1;

    line_p->key_len = super_p->key_len;
    line_p->val_len = super_p->val_len;
    line_p->key = super_p->key;
    line_p->val = super_p->val;

    return line_p;
}


/* Override an existing inherited line with a new value */
static void ltx_override_line(LTX_LINE *line_p, const char *val, unsigned short val_len)
{
    if (line_p->inherited)
    {
        line_p->inherited = 0;
        line_p->key = strdup(line_p->key);
    }
    else if (line_p->val != NULL)
        free(line_p->val);

    line_p->val_len = val_len;
    
    if (val != NULL && val_len > 0)
    {    
        line_p->val = calloc(val_len + 1, 1);
        memcpy(line_p->val, val, val_len);
    }
    else
        line_p->val = NULL;
}


static void ltx_destroy_line(LTX_LINE *line_p)
{
    /* Inherited line points to other line's data */
    if (!line_p->inherited)
    {  
        free(line_p->key);

        if (line_p->val != NULL)
            free(line_p->val);
    }
    free(line_p);
}


/* Create and initialize a new section node */
static LTX_SECT* ltx_create_sect(char *name)
{
    LTX_SECT *sect_p = calloc(sizeof(LTX_SECT), 1);

    sect_p->name = strdup(name);
    return sect_p;
}



/* Create array of lines in sorted after the read order */
static void ltx_complete_sect(LTX_SECT *sect_p)
{
    int i;
    int inserted = 0;
    LTX_LINE *line_p;

    sect_p->line_array = calloc(sect_p->num_lines, sizeof(LTX_LINE*));

    for (i = 0; i < LINE_HASH_TABLE_SIZE; i++)
    {
        for (line_p = sect_p->line_table[i]; line_p != NULL; line_p = line_p->next_p)
        {
           sect_p->line_array[inserted] = line_p;
           inserted++;
        }
    }

    qsort(sect_p->line_array, inserted, sizeof(LTX_LINE*), ltx_cmp_line);
}


static void ltx_destroy_sect(LTX_SECT *sect_p)
{
    int i;

    free(sect_p->name);

    while (sect_p->parents_p != NULL)
    {
        PARENT_LIST *parent_p = sect_p->parents_p;
        sect_p->parents_p = parent_p->rest_p;
        free(parent_p);
    }

    for (i = 0; i < sect_p->num_lines; i++)
        ltx_destroy_line(sect_p->line_array[i]);

    if (sect_p->num_lines > 0)
        free(sect_p->line_array);

    free(sect_p); 
}


/* Find the position where a line exists in a section or where it should be inserted */
static LTX_LINE **ltx_find_line_ex(LTX_SECT *sect_p, const char* key, int key_len)
{
    unsigned int hash = ltx_hash(key, key_len);
    LTX_LINE **line_pp = &sect_p->line_table[hash % LINE_HASH_TABLE_SIZE];

    while (*line_pp != NULL && memcmp(key, (*line_pp)->key, key_len) != 0)
        line_pp = &(*line_pp)->next_p;

    return line_pp;
}


static LTX_LINE *ltx_find_line(LTX_SECT *sect_p, const char* key)
{
    LTX_LINE **line_pp = ltx_find_line_ex(sect_p, key, strlen(key));

    return *line_pp;
}


static int ltx_is_comment(const char *str_p)
{
    return (str_p[0] == ';'
            || (str_p[0] == '/' && str_p[1] == '/')
            || (str_p[0] == '-' && str_p[1] == '-'));
    
}


static char *ltx_find_eol(const char* str_p)
{
    while (*str_p != '\r' && *str_p != '\n' && *str_p != 0)
    {
        if (ltx_is_comment(str_p))
            break;

        str_p++;
    }

    str_p--;

    /* Backtrack to the last non-whitespace character */
    for (; isspace(*str_p); str_p--);

    return (char*)str_p + 1;
}


static int ltx_add_line_to_sect(LTX_SECT *sect_p, char *str_p)
{
    int key_len = 0;
    int val_len = 0;
    const char *val_p = str_p;
    const char *eql_p = strchr(str_p, '=');
    const char *end_p = ltx_find_eol(str_p);
    LTX_LINE **line_pp;

    if (eql_p != NULL && eql_p < end_p && eql_p != str_p)
    {
        const char *kend_p = eql_p - 1;

        val_p = eql_p + 1;

        for (; isspace(*kend_p); kend_p--);
        kend_p++;

        key_len = kend_p - str_p;

        for (; val_p < end_p && isspace(*val_p); val_p++);
        
        val_len = end_p - val_p;
    }
    else
        key_len = end_p - str_p;

    line_pp = ltx_find_line_ex(sect_p, str_p, key_len);

    if (*line_pp == NULL)
    {
        *line_pp = ltx_create_line(str_p, key_len, val_p, val_len);

        sect_p->num_lines++;
        (*line_pp)->sequence = sect_p->num_lines;
        (*line_pp)->sect_p = sect_p;
    }
    else
        ltx_override_line(*line_pp, val_p, val_len);

    return LTX_SUCCESS;
}


/* Insert section node into hash table, return failure if duplicate */
static int ltx_insert_sect(LTX_DATA *ltx_p, LTX_SECT *sect_p)
{
    unsigned int hash = ltx_hash(sect_p->name, strlen(sect_p->name));
    LTX_SECT **sect_pp = &ltx_p->sect_table[hash % SECT_HASH_TABLE_SIZE];

    ltx_complete_sect(sect_p);

    while (*sect_pp != NULL)
    {
        if (strcmp(sect_p->name, (*sect_pp)->name) == 0)
            return LTX_FAILURE;

        sect_pp = &(*sect_pp)->next_p;
    }

    *sect_pp = sect_p;

    ltx_p->num_sect++;

    sect_p->sequence = ltx_p->num_sect;

    return LTX_SUCCESS;
}


/* Find section node in hash table, return NULL if not found */
static LTX_SECT *ltx_find_sect(LTX_DATA *ltx_p, const char* name)
{
    unsigned int hash = ltx_hash(name, strlen(name));
    LTX_SECT *sect_p = ltx_p->sect_table[hash % SECT_HASH_TABLE_SIZE];

    while (sect_p != NULL)
    {
        if (strcmp(sect_p->name, name) == 0)
            return sect_p;

        sect_p = sect_p->next_p;
    }

    return NULL;
}


static void ltx_inherit_lines(LTX_SECT *sect_p, LTX_SECT *super_p)
{
    int i;

    PARENT_LIST **parent_pp = &sect_p->parents_p;

    while (*parent_pp != NULL)
        parent_pp = &(*parent_pp)->rest_p;

    *parent_pp = calloc(sizeof(PARENT_LIST), 1);
    (*parent_pp)->sect_p = super_p;

    for (i = 0; i < super_p->num_lines; i++)
    {
        LTX_LINE *line_p = super_p->line_array[i];
        LTX_LINE **line_pp = ltx_find_line_ex(sect_p, line_p->key, line_p->key_len);

        if (*line_pp == NULL)
        {
            *line_pp = ltx_create_inherited_line(line_p);

            sect_p->num_lines++;
            (*line_pp)->sequence = sect_p->num_lines;
            (*line_pp)->sect_p = sect_p;
        }
        else /* Note: inheritance is processed before lines are read from current section */
        {
            (*line_pp)->super_p = line_p->super_p;
            (*line_pp)->val_len = line_p->val_len;
            (*line_pp)->val = line_p->val;
        }
    }
}


static void ltx_init_data(LTX_DATA *ltx_p)
{
    memset(ltx_p, 0, sizeof(LTX_DATA));

    ltx_p->global_sect_p = ltx_create_sect("GLOBAL");
}


static void ltx_complete_data(LTX_DATA *ltx_p)
{
    int i;
    int inserted = 0;
    LTX_SECT *sect_p;

    ltx_complete_sect(ltx_p->global_sect_p);

    ltx_p->sect_array = calloc(ltx_p->num_sect, sizeof(LTX_SECT*));

    for (i = 0; i < SECT_HASH_TABLE_SIZE; i++)
    {
        for (sect_p = ltx_p->sect_table[i]; sect_p != NULL; sect_p = sect_p->next_p)
        {
           ltx_p->sect_array[inserted] = sect_p;
           inserted++;
        }
    }

    qsort(ltx_p->sect_array, inserted, sizeof(LTX_SECT*), ltx_cmp_sect);
}


static void ltx_clear_data(LTX_DATA *ltx_p)
{
    int i;

    ltx_destroy_sect(ltx_p->global_sect_p);

    for (i = 0; i < SECT_HASH_TABLE_SIZE; i++)
    {
        LTX_SECT *sect_p = ltx_p->sect_table[i];

        while (sect_p != NULL)
        {
            LTX_SECT *cur_p = sect_p;
            sect_p = cur_p->next_p;

            ltx_destroy_sect(cur_p);
        }
    }

    if (ltx_p->sect_array != NULL)
        free(ltx_p->sect_array);

    memset(ltx_p, 0, sizeof(LTX_DATA));
}


static int ltx_load_file(LTX_DATA *ltx_p, char *ltx_name)
{
    FILE *fpin;
    LTX_SECT *sect_p = ltx_p->global_sect_p;
    int in_section = 0;
    int line = 0;

    if ((fpin = fopen(ltx_name, "r")) == NULL)
    {
        printf("Error opening file '%s'", ltx_name);
        perror("");
        return LTX_FAILURE;
    }

    while (!feof(fpin))
    {
        char buf[MAX_FILE_LINE_LEN];
        char name[MAX_SECT_NAME_LEN];
        char *str_p = buf;
        char *end_p;

        if (fgets(buf, sizeof(buf), fpin) == NULL)
            break;

        line++;

        end_p = (char*)ltx_find_eol(str_p);
        *end_p = 0;

        for (; isspace(*str_p); str_p++);

        if (*str_p == 0)
            continue;

        if (sscanf(str_p, "#include \"%[^\"]s\" ", name) == 1)
        {
            char path[_MAX_FNAME];
            char fname[_MAX_FNAME];
            char ext[_MAX_EXT];
            int offset;

            _splitpath(ltx_name, NULL, NULL, fname, ext);
            offset = strlen(ltx_name) - strlen(fname) - strlen(ext);
            memcpy(path, ltx_name, offset);
            strcpy(&path[offset], name);
           
            if (in_section)
            {
                if (ltx_insert_sect(ltx_p, sect_p) != LTX_SUCCESS)
                {
                    printf("ERROR: duplicate section '%s' found while loading %s\n", name, ltx_name);
                    fclose(fpin);
                    return LTX_FAILURE;
                }
            }
            in_section = 0;

            if (ltx_load_file(ltx_p, path) != LTX_SUCCESS)
            {
#ifndef LTX_IGNORE_MISSING_INCLUDES
                printf("Error loading file '%s' included from '%s' at line %d\n",
                       path, ltx_name, line);
                fclose(fpin);
                return LTX_FAILURE;
#endif
                
            }
        }
        else if (*str_p == '[' && sscanf(str_p, "[%[^]]s]", name) == 1)
        {
           
            if (in_section)
            {
                if (ltx_insert_sect(ltx_p, sect_p) != LTX_SUCCESS)
                {
                    printf("ERROR: duplicate section '%s' found while loading %s\n", name, ltx_name);
                    fclose(fpin);
                    return LTX_FAILURE;
                }
            }

            in_section = 1;

            sect_p = ltx_create_sect(name);

            str_p = strchr(str_p, ']') + 1;

            /* Inheritance from 1 or more sections? */
            if (*str_p == ':' && str_p + 1 < end_p)
            {
                str_p = strtok(str_p + 1, ",");

                while (str_p != NULL)
                {
                    char super_name[MAX_SECT_NAME_LEN];
                    LTX_SECT *super_p;

                    memset(super_name, 0, sizeof(super_name));

                    end_p = str_p + strlen(str_p);

                    for (; end_p > str_p && isspace(*end_p); end_p--);
                    memcpy(super_name, str_p, end_p - str_p + 1);

                    super_p = ltx_find_sect(ltx_p, super_name);

                    if (super_p == NULL)
                        printf("ERROR: '%s' inherits from unknown section '%s'\n", name, super_name);
                    else
                        ltx_inherit_lines(sect_p, super_p);

                    str_p = strtok(NULL, ",");
                }
            }
        }
        else
        {
            ltx_add_line_to_sect(sect_p, str_p);
        }
    }

    fclose(fpin);

    if (in_section)
    {
        if (ltx_insert_sect(ltx_p, sect_p) != LTX_SUCCESS)
        {
            printf("ERROR: duplicate section '%s' found while loading %s\n", sect_p->name, ltx_name);
            return LTX_FAILURE;
        }
    }

    return LTX_SUCCESS;
}



/* Utilities */


static CONDLIST *ltx_create_condlist(const char *str_p)
{
    CONDLIST *head_p = NULL;
    CONDLIST **tail_pp = &head_p;

    while (str_p != NULL)
    {
        int cond = 0;
        char *l_p = strchr(str_p, '{');
        char *r_p = NULL, *c_p = NULL;
        int cond_len, sect_len;

        if (l_p != NULL)
        {
            l_p++;
            r_p = strchr(l_p, '}');

            if (r_p != NULL)
            {
                c_p = strchr(l_p + 1, ',');
                if (c_p != NULL)
                    cond = 1;
            }
        }

        *tail_pp = calloc(sizeof(CONDLIST), 1);

        if (cond)
        {
            cond_len = r_p - l_p;

            (*tail_pp)->cond = calloc(cond_len + 1, 1);
            memcpy((*tail_pp)->cond, l_p, cond_len);

            for (l_p = r_p + 1; isspace(*l_p); l_p++);
            for (r_p = c_p - 1; isspace(*r_p) && r_p > l_p; r_p--);
            
            sect_len = r_p - l_p  + 1;
            (*tail_pp)->sect = calloc(sect_len + 1, 1);
            memcpy((*tail_pp)->sect, l_p, sect_len);

            str_p = c_p + 1;
        }
        else
        {
            for (; isspace(*str_p); str_p++);
            (*tail_pp)->cond = NULL;
            (*tail_pp)->sect = strdup(str_p);

            str_p = NULL;
        }

        tail_pp = &(*tail_pp)->next_p;
    }

    return head_p;    
}


static const char *ltx_find_sect_in_condlist(CONDLIST *cl_p, const char *cond)
{
    const CONDLIST *c_p = cl_p;

    while (c_p != NULL)
    {
        if (c_p->cond == NULL
            || (cond != NULL && strcmp(cond, c_p->cond) == 0))
        {
            return c_p->sect;
        }
        c_p = c_p->next_p;
    }
    return NULL;
}


static void ltx_destroy_condlist(CONDLIST *head_p)
{
    while (head_p != NULL)
    {
        CONDLIST *cond_p = head_p;
        head_p = head_p->next_p;

        if (cond_p->cond != NULL)
            free(cond_p->cond);

        free(cond_p->sect);
        free(cond_p);
    }
}

#endif
