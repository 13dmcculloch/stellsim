/* File loading library.
 *
 * These functions are very similar and will require genericisation.
 *
 * Douglas McCulloch, September 2023
 */

#include "filehandling.h"

Symbol *file_int(const char* filename, size_t *len)
{
    FILE *fp = open_file(filename);
    if(fp == NULL) return NULL;  // yes, I'm doing two checks. print is big

    size_t n_symbols = count_lines(fp);

    Symbol *s = malloc(n_symbols * sizeof(*s));

    /* non-generic part: parse CSV of ints */
    int i, q;
    int idx = 0;
    while(fscanf(fp, "%d,%d\n", &i, &q) == 2)
    {
        s[idx] = set_symbol(i, q, idx);
        ++idx;  // if in above, then get -Wsequence-point
    }

    *len = n_symbols;

    return s;
}

Symbol *file_dbl(const char* filename, size_t *len)
{
    FILE *fp = open_file(filename);
    if(fp == NULL) return NULL;

    size_t n_symbols = count_lines(fp);

    Symbol *s = malloc(n_symbols * sizeof(*s));

    /* non-generic part: parse CSV of doubles */
    double i, q;
    int idx = 0;
    while(fscanf(fp, "%lf,%lf\n", &i, &q) == 2)
    {
        s[idx] = set_symbolf(i, q, idx);
        ++idx;
    }

    *len = n_symbols;

    return s;
}

FILE *open_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
        printf("Failed to open file %s.\n", filename);

    return fp;
}

size_t count_lines(FILE *fp)
{
    /* get length of lookup table */
    int c;
    int n_symbols = 0;

    while((c = fgetc(fp)) != EOF)
        if((char)c == '\n') n_symbols++;

    rewind(fp);
    
    return n_symbols;
}