#include "console.h"

Symbol_Data *symbols;  // <- these are meant to have random addresses

void free_all(Symbol_Data *symbols);

int main(int argc, char **argv)
{
    /* WHY DO i HAVE TO DO THIS????????? */
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
