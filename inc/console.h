/* A console to interface with stellsim.
 *
 * Douglas McCulloch, September 2023
 */

#include "Symbols.h"
#include "diagram.h"
#include "filehandling.h"
#include "macros.h"

/* Commands */
/* These are for lower-case commands.
 * Caps not supported. This is not DOS.
 */
#define EXIT 0x1ba 

#define LOOKUP 0x29a
#define SAMPLE 0x282

#define GENERATE 0x34b
#define DUMB 0x1a8

#define PRINT 0x22d


/* Functionality */
/* User can generate lookup table from command line arguments. Therefore the
 * lookup table will be present in main. It needs to be passed to the console.
 *
 * When in the console, we can generate additional arrays of symbols, but it
 * would be too cumbersome to provide all the functionality (for example
 * noise) through the command line arguments.
 */
int console(Symbol_Data *symbols);

char **parse_input(char *input, size_t input_len, int *argc);
int handle_input(char **argv, Symbol_Data *symbol);

int generate(char **argv, Symbol_Data *symbol);
int print_table(char **argv, Symbol_Data *symbol);

int free_argv(int argc, char ***argv);

int hash_cmd(const char *cmd);
void print_input(int argc, char **argv);
