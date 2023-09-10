/* Sandbox for testing functions.
 */

#include <unistd.h>
#include <string.h>

#include "console.h"

/* Program control */
int set_flags(int argc, char **argv);
int build_table();
int display_data();

/* Debug function */
void symbol_size();

/* Flags */
int f_gen = 0, f_ifile = 0, f_dfile = 0;
int f_table = 0, f_diagram = 0;
int f_console = 0;

/* Control vars */
int mod_order;
char gen_type;
char filename[100];

/* Lookup table */
Symbol *lookup = NULL;
size_t lookup_len = 0;

int main(int argc, char **argv)
{
    if(set_flags(argc, argv))
    {
        printf("[ERROR]: flags not set.\n");
        printf("Usage: %s [-g gen_type] [-M modulation_order] [-i int_file] " \
                "[-d double_file] [-ta]\n", argv[0]);
        return 1;
    }

    if(build_table())
    {
        printf("[ERROR]: could not build table.\n");
        return 1;
    }

    if(display_data())
    {
        printf("[ERROR]: could not display data.\n");
        return 1;
    }

    if(f_console)
        if(console(lookup, lookup_len)) return 1;

    free(lookup);

    return 0;
}

int set_flags(int argc, char **argv)
{
    int opt;
    while((opt = getopt(argc, argv, "g:M:i:d:tac")) != -1)
    {
        switch(opt) {
        case 'g':
            f_gen = 1;
            gen_type = optarg[0];
            break;

        case 'M':
            mod_order = atoi(optarg);
            break;

        case 'i':
            f_ifile = 1;
            strcpy(filename, optarg);
            break;

        case 'd':
            f_dfile = 1;
            strcpy(filename, optarg);
            break;

        case 't':
            f_table = 1;
            break;

        case 'a':
            f_diagram = 1;
            break;

        case 'c':
            f_console = 1;
            break;

        default:
            printf("Usage: %s [-g] [-M modulation_order] [-i int_file] " \
                "[-d double_file] [-ta]\n", argv[0]);
            return 1;
        }
    }

    /* I stole this from the manpage */
    /*
    if(optind >= argc)
    {
        printf("Expected argument after options.\n");
        return 1;
    }
    */    
    return 0;
}

int build_table()
{
    if(f_gen && (f_ifile || f_dfile)) 
    {
        printf("[ERROR]: Cannot generate and take \
            constellation file simultaneously.\n");
        return 1;
    }

    if(f_ifile && f_dfile)
    {
        printf("[ERROR]: Only one constellation file permitted.\n");
        return 1;
    }

    /* Case: generate constellation diagram of modulation order M */
    if(f_gen)
    {
        lookup = gen_lookup_QAM(mod_order, gen_type, &lookup_len); 
        MEM_CHECK(lookup);
    }

    /* Case: generate constellation diagram form integer file */
    else if(f_ifile)
    {
        lookup = file_int(filename, &lookup_len);
        MEM_CHECK(lookup);
    }

    /* Case: generate constellation diagram from double file */
    else if(f_dfile)
    {
        lookup = file_dbl(filename, &lookup_len);
        MEM_CHECK(lookup);
    }

    return 0;
}    

int display_data()
{
    if(f_table)
        print_lookup(lookup, lookup_len);

    if(f_diagram)
        print_diagram(lookup, lookup_len, 's');
    
    return 0;
}

void symbol_size()
{
    printf("sizeof(Symbol) = %ld B\nsizeof(Symbol_Ref) = %ld B\nsizeof(size_t) \
        = %ld B\n", sizeof(Symbol), sizeof(Symbol_Ref), sizeof(size_t));


}
