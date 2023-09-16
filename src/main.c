/* Entry point for stellsim.
 *
 * Douglas McCulloch, September 2023
 */

#include "console.h"

/* rationale is that one can scale up to have multiple Symbol_Data's */
/* console takes symbols so it is possible to alloc more
 * Symbol_Data if necessary.
 */
Symbol_Data *symbols;

void free_all(Symbol_Data *symbols);

int main(int argc, char **argv)
{
    symbols = malloc(sizeof(*symbols));
    MEM_CHECK(symbols);

    symbols->lookup = NULL;
    symbols->sample = NULL;

    int console_status = console(symbols);

    free_all(symbols);

    return console_status;
}

void free_all(Symbol_Data *symbols)
{
    if(symbols->lookup == NULL)
    {
        fprintf(stderr, "[main]: no need to free lookup.\n");
    }
    else
    {
        free(symbols->lookup);
    }
    
    if(symbols->sample == NULL)
    {
        fprintf(stderr, "[main]: no need to free sample.\n");
    }
    else
    {
        free(symbols->sample);
    }

    free(symbols);
}
