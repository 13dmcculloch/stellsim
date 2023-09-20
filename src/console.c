/* Console
 *
 * Douglas McCulloch, September 2023
 */

#include <string.h>

#include "console.h"

/* toy prototypes */
static void enter_console_msg();
static void show_help();
static int free_argv(int, char***);
int hash_cmd(const char *cmd);
static int predefined_lookup(char **argv, Symbol_Data *sybmol);
static int load_lookup_file(char **argv, Symbol_Data *symbol);
static int generate(char **argv, Symbol_Data *symbol);
static int print_table(char **argv, Symbol_Data *symbol);
static int print_table_c(char **argv, Symbol_Data *symbol);
static int print_table_p(char **argv, Symbol_Data *symbol);
static int draw_argand(char **argv, Symbol_Data *symbol);
static int concat(char **argv, Symbol_Data *symbol);
static int noise(char **argv, Symbol_Data *symbol);
static int map(char **argv, Symbol_Data *symbol);
static int maps(char **argv, Symbol_Data *symbol);
static int save(char **argv, Symbol_Data *symbol);

int f_exit = 0;

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

        case PRINTA:
            return print_table(argv, symbol);
            break;

        case PRINTC:
            return print_table_c(argv, symbol);
            break;

        case PRINTP:
            return print_table_p(argv, symbol);
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

        case LOAD:
            return load_lookup_file(argv, symbol);
            break;

        case MAP:
            return map(argv, symbol);
            break;

        case MAPS:
            return maps(argv, symbol);
            break;

        default:
            fprintf(stderr, "handle_input: Command not found.\n");
            show_help();
            return 1;
    }

    return 0;
}

static void show_help()
{
    fputs("Global help:\n", stderr);
    fputs("exit - exit Stellsim\n"
        "generate ref BPSK|QPSK|8PSK - generate constellations from ROM\n"
        "load [type] [filename] - load constellation from file\n"
        "map [filename] - map stream to symbols\n"
        "maps [string] - map string to symbols\n"
        "print(a|c|p) lookup|sample - print all, cartesian, polar" 
        " representations\n"
        "draw lookup|sample - draw constellation, samples\n"
        "noise amplitude|phase [level] - add noise to sample\n"
        "concat [n] - duplicate lookup table n times into sample\n",
        stderr);
}

static int free_argv(int argc, char ***argv)
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

        if(input_buffer[0] == ' ' || input_buffer[0] == '\n') goto Free;
        else if(handle_input(argv, symbols)) fprintf(stderr, 
            "[CONSOLE]: Error handling input\n");

        memset(input_buffer, '\0', 100);

        if(f_exit)
        {
            return free_argv(argc, &argv);
        }    

    Free:
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

    /* if last, then remove \n from end */
    argv[*argc - 1][strlen(argv[*argc - 1]) - 1] = '\0';

    return argv;
}

static int predefined_lookup(char **argv, Symbol_Data *symbol)
{
    int type = hash_cmd(argv[2]);

    switch(type)
    {
    case BPSK_R:
        symbol->lookup_len = sizeof(BPSK) / sizeof(BPSK[0]);
        symbol->lookup = construct_lookup(BPSK, symbol->lookup_len);
        break;

    case QPSK_R:
        symbol->lookup_len = sizeof(QPSK) / sizeof(QPSK[0]);
        symbol->lookup = construct_lookup(QPSK, symbol->lookup_len);
        break;

    case HPSK_R:
        symbol->lookup_len = sizeof(HPSK) / sizeof(HPSK[0]);
        symbol->lookup = construct_lookup(HPSK, symbol->lookup_len);
        break;

    default:
        fprintf(stderr, "generate ref BPSK|QPSK|8PSK\n");
        return 1;
    }

    return 0;
}

static int load_lookup_file(char **argv, Symbol_Data *symbol)
{
    if(argv[1] == NULL || argv[2] == NULL)
    {
        fputs("load integer|double|polar [filename]\n", stderr);
        return 1;
    }

    int type = 0;
    type = hash_cmd(argv[1]);

    switch(type)
    {
    case INTEGER:
        symbol->lookup = file_int(argv[2], &symbol->lookup_len);
        break;
    
    case DOUBLE:
        symbol->lookup = file_dbl(argv[2], &symbol->lookup_len);
        break;

    case POLAR:
        symbol->lookup = file_plr(argv[2], &symbol->lookup_len);
        break;

    default:
        fputs("load integer|double|polar [filename]\n", stderr);
        return 1;
    }

    return 0;
}
    
static int generate(char **argv, Symbol_Data *symbol)
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
    
    else if(type == REF) return predefined_lookup(argv, symbol);

    else 
    {
        fprintf(stderr, "Generation algorithm \"%s\" not found.\n", 
        argv[1]);
        return 1;
    }

    return 0;
}

static int print_table(char **argv, Symbol_Data *symbol)
{
    if(argv[1] == NULL)
    {
        fprintf(stderr, "printa lookup|sample\n");
        return 1;
    }

    int type = 0;
    type = hash_cmd(argv[1]);

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
        fprintf(stderr, "printa lookup|sample\n");
        return 1;
    }
    
    return 0;
}

static int print_table_c(char **argv, Symbol_Data *symbol)
{
    if(argv[1] == NULL)
    {
        fputs("printc lookup|sample\n", stderr);
        return 1;
    }

    int type = hash_cmd(argv[1]);

    switch(type)
    {
    case LOOKUP:
        print_cartesian(symbol->lookup, symbol->lookup_len);
        break;

    case SAMPLE:
        print_cartesian(symbol->sample, symbol->sample_len);
        break;

    default:
        fputs("printc lookup|sample\n", stderr);
        return 1;

    }

    return 0;
}

static int print_table_p(char **argv, Symbol_Data *symbol)
{
    if(argv[1] == NULL)
    {
        fputs("printp lookup|sample\n", stderr);
        return 1;
    }

    int type = hash_cmd(argv[1]);

    switch(type)
    {
    case LOOKUP:
        print_polar(symbol->lookup, symbol->lookup_len);
        break;

    case SAMPLE:
        print_polar(symbol->sample, symbol->sample_len);
        break;

    default:
        fputs("printp lookup|sample\n", stderr);
        return 1;

    }

    return 0;
}

static int draw_argand(char **argv, Symbol_Data *symbol)
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

static int concat(char **argv, Symbol_Data *symbol)
{
    int n;
    if(argv[1] == NULL) n = 1;
    else n = atoi(argv[1]);

    symbol->sample = cat_symbols(symbol->lookup, symbol->lookup_len, n,
        &symbol->sample_len);

    return 0;
}

static int noise(char **argv, Symbol_Data *symbol)
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

static int map(char **argv, Symbol_Data *symbol)
{
    if(argv[1] == NULL)
    {
        fputs("map [filename]\n(for now)\n", stderr);
        return 1;
    }

    FILE *i_s = open_file(argv[1]);

    return mapper(i_s, stdout, symbol->lookup, symbol->lookup_len);
}

static int maps(char **argv, Symbol_Data *symbol)
{
    if(argv[1] == NULL)
    {
        fputs("maps [str]\n", stderr);
        return 1;
    }

    return mapper_s(argv[1], stdout, symbol->lookup, symbol->lookup_len);
}

static void enter_console_msg()
{
    printf("Stellsim alpha console.\nDouglas McCulloch, September 2023.\n");
}
