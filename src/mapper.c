/* Mapper source.
 *
 * Douglas McCulloch, September 2023
 */

#include "mapper.h"

static int map_stream(FILE *i_s, FILE *o_s, Symbol *lookup,
    int bits, uint32_t mask, int mod_order, long rewind);

static long map32(uint32_t buf, Symbol *lookup,
    int bits, uint32_t mask, int mod_order, FILE *o_s);

static int handle_io_map(const Symbol *res, FILE *o_s);

/*                 */
/* Mapper function */
/*                 */
int mapper(FILE *i_s, FILE *o_s, Symbol *lookup, size_t lookup_len)
{
    /* check if lookup table is populated */
    if(lookup == NULL)
    {
        fputs("mapper: no lookup table in memory! memcheck:\n", stderr);
        MEM_CHECK(lookup);
    }

    int mod_order = (int)log2((double)lookup_len);

    uint32_t mask = 0;
    long buffer_shorten = 0;
    
    if(mod_order > 31)
    {
        fputs("mapper: modulation order larger than buffer.\n", stderr);
        return 1;
    }

    /* calculate size of buffer from modulation order */
    int remainder;
    int buf_size = sizeof(mask) * 8;  // i.e. 32
    while((remainder = buf_size % mod_order))  // i.e. 32 % 3 = 2
    {
        buffer_shorten += 8;  // must align to char
        buf_size -= buffer_shorten;  // i.e. 24
    }

    if(!buf_size || (buf_size % 8))
    {
        fputs("mapper: invalid modulation order (see manual).\n", stderr);
        return 1;
    }

    /* bits is the number of full symbols that fit into buffer */
    int bits = buf_size / mod_order;

    /* generate the mask */
    for(int i = 0; i < mod_order; ++i) 
        mask |= (1 << (sizeof(mask) * 8 - 1 - i));

    /* map stream loop */
    while(!map_stream(i_s, o_s, lookup,
        bits, mask, mod_order, buffer_shorten / 8))
    {
    /*
        if(handle_io_map(&res, o_s))
        {
            fputs("mapper: error handling io.\n", stderr);
            return 1;
        }
        */
    }

    return 0;
}
    

static int map_stream(FILE *i_s, FILE *o_s, Symbol *lookup,
    int bits, uint32_t mask, int mod_order, long rewind)
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

    fseek(i_s, -rewind, SEEK_CUR);

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

        /* i/o bit. Better to return sym? Deal with it higher up? */
        /*
        puts("");
        print_symbol(*res);
        */
        handle_io_map(&lookup[val], stdout);
    }

    return 0;
}

static int handle_io_map(const Symbol *res, FILE *o_s)
{
    return stream_symbolf(res, o_s);
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
