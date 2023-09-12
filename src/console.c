/* Console
 *
 * Douglas McCulloch, September 2023
 */

#include <string.h>

#include "console.h"

/* toy prototypes */
void enter_console_msg();

int console(Symbol *lookup, size_t lookup_len)
{
    enter_console_msg();

    char input_buffer[100];
    memset(input_buffer, '\0', 100);

    int f_free = 1;

    /* Event loop */
    while(1)
    {
        printf(">>> ");
        fgets(input_buffer, 100, stdin);

        int argc = 1;
        size_t input_len = sizeof(input_buffer);
        char **argv = parse_input(input_buffer, input_len, &argc);
        MEM_CHECK(argv);

        print_input(argc, argv);

        if(handle_input(input_buffer)) return 1;

        memset(input_buffer, '\0', 100);

        if(!strcmp(argv[0], "exit\n"))
        {
            return free_argv(argc, &argv);
        }

        f_free = free_argv(argc, &argv);
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

int handle_input(char **argv)
{
    /* no idea how to design this as of yet */
    return 0;
}

int free_argv(int argc, char ***argv)
{
    for(int i = 0; i < argc; ++i)
        free((*argv)[i]);  // this is the correct grammar

    free(*argv);

    return 0;
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
