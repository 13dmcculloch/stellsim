/* Entry point for stellsim.
 *
 * Douglas McCulloch, September 2023
 */

#include <string.h>
#include <unistd.h>

#include "console.h"

/* rationale is that one can scale up to have multiple Symbol_Data's */
/* console takes symbols so it is possible to alloc more
 * Symbol_Data if necessary.
 */
Symbol_Data *symbols;

void free_all(Symbol_Data *symbols);
int script(Symbol_Data *symbols, char *filename);

int main(int argc, char **argv)
{
    symbols = malloc(sizeof(*symbols));
    MEM_CHECK(symbols);

    symbols->lookup = NULL;
    symbols->sample = NULL;
    
    /* handle command line args for scripting */
    int opt, f_script = 0;
    char filename[100];

    while((opt = getopt(argc, argv, "s:")) != -1)
    {
        switch(opt)
        {
        case 's':
            strcpy(filename, optarg);
            f_script = 1;
            break;
        default:
            fprintf(stderr, "main: command not found.\n");
            fprintf(stderr, "stellsim -s [filename].\n.");
            return EXIT_FAILURE;
        }
    }

    if(f_script)
    {
        if(script(symbols, filename))
        {
            fprintf(stderr, "main: parsing script failed.\n");
            return EXIT_FAILURE;
        }
    }

    /* then drop to console afterwards */
    int console_status = console(symbols);

    free_all(symbols);

    return console_status;
}

int script(Symbol_Data *symbols, char *filename)
{
    FILE *fp = fopen(filename, "r");

    char line[100];
    int argc = 1;

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        char **argv = parse_input(line, sizeof(line), &argc);

        if(handle_input(argv, symbols))
        {
            fprintf(stderr, "Error handling input from script.\n");
            return EXIT_FAILURE;
        }
    }

    return 0;
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
