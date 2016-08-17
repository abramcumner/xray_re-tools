#ifndef LTX_H
#define LTX_H

/*
 * Structures and definitions for working with data in ltx files
 * 
 * Data from an LTX file is implemented as a hash table with sections
 * each section consisting of a name, pointer to section inherited from
 * and the key = value pairs
 */


#define LTX_SUCCESS 0
#define LTX_FAILURE 1

#define MAX_FILE_LINE_LEN    1000
#define MAX_SECT_NAME_LEN     200
#define LINE_HASH_TABLE_SIZE   31
#define SECT_HASH_TABLE_SIZE 1231

/* Forward declarations */
typedef struct LTX_LINE LTX_LINE;
typedef struct LTX_SECT LTX_SECT;


/*
 * Linked list of section names inherited from
 */
typedef struct PARENT_LIST
{
    LTX_SECT *sect_p;
    struct PARENT_LIST *rest_p;
} PARENT_LIST;


/*
 * Data from a line in an LTX file
 */
struct LTX_LINE
{
    LTX_SECT *sect_p;            /* the section containing this line */
    LTX_LINE *super_p;           /* line inherited from (if applicable) */
    unsigned short inherited;    /* inherited from another section */
    unsigned short sequence;     /* sequence in the section */
    unsigned short key_len;      /* length of key string */
    unsigned short val_len;      /* length of value string (if present) */
    char *key;
    char *val;
    struct LTX_LINE* next_p;     /* next line in section's line hash table chain */
};


/*
 * Ltx section structure containing keys/values
 */
struct LTX_SECT
{
    char *name;               /* section name without square brackets */
    PARENT_LIST *parents_p;   /* list of parents if any */
    unsigned short sequence;  /* sequence number of this section in the read data */
    unsigned short num_lines; /* number of lines */
    LTX_LINE *line_table[LINE_HASH_TABLE_SIZE];
    LTX_LINE **line_array;    /* array of lines created when the last line is read */
    LTX_SECT *next_p;         /* next node in hash table chain */
};


/*
 * Main data structure with all loaded sections
 */
typedef struct
{
    int num_sect;
    LTX_SECT *global_sect_p;
    LTX_SECT *sect_table[SECT_HASH_TABLE_SIZE];
    LTX_SECT **sect_array;
} LTX_DATA;


/*
 * Structure for conditional expression breakdown
 */
typedef struct CONDLIST
{
    char *cond;
    char *sect;
    struct CONDLIST *next_p;
} CONDLIST;



#define LTX_VAL(s) ((s) != NULL) ? (s) : "")

#endif
