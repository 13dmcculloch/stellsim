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
    uint32_t buffer;

    if(!fread(&buffer, sizeof(uint32_t), 1, i_s))
    {
        fputs("mapper: stream ended.\n", stderr);
        return 1;
    }

    register uint32_t buf = htonl(buffer);

    /* remainder will change each time.
     * rewind to map remaining bits.
     *
     * I have no idea if this works. Disabled for now.
     */
    long remainder = map32(buf, lookup, bits, mask, mod_order, o_s);
    //fseek(i_s, -remainder, SEEK_CUR);

    return 0;
}

static long map32(uint32_t buf, Symbol *lookup,
    int bits, uint32_t mask, int mod_order, FILE *o_s)
{
    for(int i = 0; i < bits; ++i)
    {
        int val = buf & (mask >> mod_order * i);
        val >>= (sizeof(buf) * 8 - mod_order * (i + 1));
        /* debug stuff
        printf("\nmask\t= %x\n", (mask >> mod_order * i));
        printf("buf\t= %x\n", buf);
        printf("val = %d\t= %x\n", val, val);
        */
        Symbol sym = lookup[val];

        puts("");
        print_symbol(sym);
    }

    return sizeof(buf) % bits;
}
