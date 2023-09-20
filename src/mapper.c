/* Mapper source.
 *
 * Douglas McCulloch, September 2023
 */

#include "mapper.h"

static int map_stream(FILE *i_s, FILE *o_s, Symbol *lookup,
    int bits, uint32_t mask, int mod_order);

static long map32(uint32_t buf, Symbol *lookup,
    int bits, uint32_t mask, int mod_order, FILE *o_s);

int mapper(FILE *i_s, FILE *o_s, Symbol *lookup, size_t lookup_len)
{
    /* check if lookup table is populated */
    if(lookup == NULL)
    {
        fputs("mapper: no lookup table in memory! memcheck:\n", stderr);
        MEM_CHECK(lookup);
    }

    /* mod_order = bits/symbol */
    int mod_order = (int)log2((double)lookup_len);

    uint32_t mask = 0;

    /* bits is the number of full symbols that fit into buffer */
    int bits = sizeof(mask) * 8 / mod_order;

    /* generate the mask */
    for(int i = 0; i < mod_order; ++i) 
        mask |= (1 << (sizeof(mask) * 8 - 1 - i));

    /* map stream loop */
    while(!map_stream(i_s, o_s, lookup,
        bits, mask, mod_order));

    return 0;
}

int mapper_s(char *s, FILE *o_s, Symbol *lookup, size_t lookup_len)
{
    FILE *fp = fopen(TEMP_FILENAME, "w");
    if(fp == NULL)
    {
        fputs("mapper: failed to open tmp file.\n", stderr);
        return 1;
    }

    if(!fwrite(s, strlen(s) + 1, 1, fp))
    {
        fputs("mapper: failed to write to tmp file.\n", stderr);
        return 1;
    }

    fclose(fp);

    {
    FILE *fp = open_file(TEMP_FILENAME);

    return mapper(fp, o_s, lookup, lookup_len);
    }
}
    
static int map_stream(FILE *i_s, FILE *o_s, Symbol *lookup,
    int bits, uint32_t mask, int mod_order)
{
    uint32_t buffer = 0;
    int f_stop = 0;

    /* fill the buffer */
    if(!fread(&buffer, sizeof(uint32_t), 1, i_s))
    {
        /* Frankly, I have no idea how to handle a partial fread().
         * Basically, initialising buffer to 0 ensures that a partial
         * fread is reliable up to some extent, especially when 2
         * chars are remaining in the stream. I don't know if this is
         * portable. Luckily, it only messes up the end of the stream,
         * and there may be protocols which put garbage at the end of
         * stream to avoid these issues, but that is a massive cope
         * and it should really work without any issues.
         */
        f_stop = 1;
    }

    buffer = htonl(buffer);

    //fprintf(stderr, "%x\n", buffer);

    if(!buffer) return 1;  /* though continue to map partial buffer */
        
    if(map32(buffer, lookup, bits, mask, 
        mod_order, o_s))
        fputs("mapper: error mapping buffer.\n", stderr);

    if(f_stop) return 1;  /* partial buffer suggests EOF */

    return 0;
}

static long map32(uint32_t buf, Symbol *lookup,
    int bits, uint32_t mask, int mod_order, FILE *o_s)
{
    /* map32 does not know extent to which buffer is partial and so will
     * continue to map '0' until the end of buffer, so there will be 8 * N
     * zeros at the end of the symbol stream where N is the number of 0
     * chars.
     */
    for(int i = 0; i < bits; ++i)
    {
        int val = buf & (mask >> mod_order * i);
        val >>= (sizeof(buf) * 8 - mod_order * (i + 1));
        Symbol sym = lookup[val];

        /* i/o bit. Better to return sym? Deal with it higher up? */
        puts("");
        print_symbol(sym);
    }

    return 0;
}
