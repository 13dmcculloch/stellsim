/* Console
 *
 * Douglas McCulloch, September 2023
 */

#include <string.h>

#include "console.h"

int f_exit = 0;

/* toy prototypes */
void enter_console_msg();

int console(Symbol_Data *symbols)
{
    enter_console_msg();

    char input_buffer[100];
    memset(input_buffer, '\0', 100);

    int f_free = 1;  // 1 when argv malloc'd 

    /* Event loop */
    while(1)
    {
        printf(">>> ");
        fgets(input_buffer, 100, stdin);

        int argc = 1;
        size_t input_len = sizeof(input_buffer);
        char **argv = parse_input(input_buffer, input_len, &argc);
        MEM_CHECK(argv);
        f_free = 1;

        if(handle_input(argv, symbols)) fprintf(stderr, 
            "[CONSOLE]: Error handing input\n");

        memset(input_buffer, '\0', 100);

        if(f_exit)
        {
            return free_argv(argc, &argv);
        }

        f_free = free_argv(argc, &argv);
        if(f_free) fprintf(stderr, "[WARNING]: Failed to free args.\n");
    }

    return f_free;
}

char **parse_input(char *input, size_t input_len, int *argc)
{
    char **argv;

    for(size_t i = 0; i < input_len; ++i)
        if(input[i] == ' ') ++(*argc);

    argv = malloc((*argc) * sizeof(*argv));
    if(argv == NULL)
    {
        printf("Nullptr at %p\n", (void *)&argv);
    }

    char *tok = strtok(input, " ");
    size_t tok_len = strlen(tok) + 1;
    *argv = malloc(tok_len * sizeof(**argv));
    if(*argv == NULL)
    {
        printf("Nullptr at %p\n", (void *)argv);
    }

    memcpy(*argv, tok, tok_len);

    for(int i = 1; i < *argc; ++i)
    {
        tok = strtok(NULL, " ");
        if(tok == NULL) break;

        tok_len = strlen(tok) + 1;
        argv[i] = malloc(tok_len * sizeof(**argv));
        if(argv[i] == NULL)
        {
            printf("Nullptr at %p\n", (void *)&argv[i]);
        }

        memcpy(argv[i], tok, tok_len);
    }

    return argv;
}

int handle_input(char **argv, Symbol_Data *symbol)
{
    switch(hash_cmd(argv[0]))
    {
        case EXIT:
            f_exit = 1;
            break;

        case GENERATE:
            return generate(argv, symbol);
            break;

        case PRINT:
            return print_table(argv, symbol);
            break;

        case DRAW:
            return draw_argand(argv, symbol);
            break;

        case CONCAT:
            return concat(argv, symbol);
            break;

        case NOISE:
            return noise(argv, symbol);
            break;

        default:
            fprintf(stderr, "handle_input: Command not found.\n");
            return 1;
    }

    return 0;
}

int generate(char **argv, Symbol_Data *symbol)
{
    if(argv[1] == NULL || argv[2] == NULL)
    {
        fprintf(stderr, "generate [type] [M]\n");
        return 1;
    }
    
    /* Check if table null, otherwise free table first */
    if(symbol->lookup != NULL)
    {
        free(symbol->lookup);
    }

    int type = hash_cmd(argv[1]);
    int M = atoi(argv[2]);

    if(type == DUMB) symbol->lookup = gen_lookup_QAM_dumb(M,
        &symbol->lookup_len);
    else 
    {
        fprintf(stderr, "Generation algorithm \"%s\" not found.\n", 
        argv[1]);
        return 1;
    }

    return 0;
}

int print_table(char **argv, Symbol_Data *symbol)
{
    if(argv[1] == NULL)
    {
        fprintf(stderr, "print lookup|sample\n");
        return 1;
    }

    int type = hash_cmd(argv[1]);

    if(type == LOOKUP)
    {
        print_lookup(symbol->lookup, symbol->lookup_len);
    }
    else if(type == SAMPLE)
    {
        print_lookup(symbol->sample, symbol->sample_len);
    }
    else
    {
        fprintf(stderr, "print lookup|sample\n");
        return 1;
    }
    
    return 0;
}

int draw_argand(char **argv, Symbol_Data *symbol)
{
    if(argv[1] == NULL)
    {
        fprintf(stderr, "draw lookup|sample\n");
        return 1;
    }

    int type = hash_cmd(argv[1]);

    if(type == LOOKUP)
    {
        draw_symbols(symbol->lookup, symbol->lookup_len, 'v');
    }
    else if(type == SAMPLE)
    {
        draw_symbols(symbol->sample, symbol->sample_len, 0);
    }
    else
    {
        fprintf(stderr, "draw lookup|sample\n");
        return 1;
    }

    /* stay on table (24 line terminal too short for >>>) */
    while(!getchar());

    return 0;
}

int concat(char **argv, Symbol_Data *symbol)
{
    int n;
    if(argv[1] == NULL) n = 1;
    else n = atoi(argv[1]);

    symbol->sample = cat_symbols(symbol->lookup, symbol->lookup_len, n,
        &symbol->sample_len);

    return 0;
}

int noise(char **argv, Symbol_Data *symbol)
{
    if(argv[1] == NULL)
    {
        fprintf(stderr, "noise amplitude|phase [gain]\n");
        return 1;
    }

    double gain;
    if(argv[2] == NULL) gain = 1;
    else gain = atof(argv[2]);

    int type = hash_cmd(argv[1]); 

    if(type == AMPLITUDE)
    {
        if(amplitude_noise(symbol->sample, symbol->sample_len, gain))
            fprintf(stderr, "Error applying noise to sample.\n");
    }

    else if(type == PHASE)
    {
        if(phase_noise(symbol->sample, symbol->sample_len, gain))
            fprintf(stderr, "Error applying noise to sample.\n");
    }

    else
    {
        fprintf(stderr, "noise amplitude|phase [gain]\n");

        return 1;
    }

    return 0;
}

int free_argv(int argc, char ***argv)
{
    for(int i = 0; i < argc; ++i)
        free((*argv)[i]);  // this is the correct grammar

    free(*argv);

    return 0;
}

int hash_cmd(const char *cmd)
{
    int sum = 0;

    for(int i = 0; i < strlen(cmd); ++i)
    {
        if(cmd[i] == '\n') continue;
        sum += cmd[i];
    }

    return sum;
}

void print_input(int argc, char **argv)
{
    printf("%d args.\n", argc);

    for(int i = 0; i < argc; ++i)
    {
        printf("Cmd %d: %s\n", i, argv[i]);
    }
}

void enter_console_msg()
{
    printf("Stellsim alpha console.\nDouglas McCulloch, September 2023.\n");
}
