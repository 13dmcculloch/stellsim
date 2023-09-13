/* A console to interface with stellsim.
 *
 * Douglas McCulloch, September 2023
 */

#include "Symbols.h"
#include "diagram.h"
#include "filehandling.h"
#include "macros.h"

/* Commands */
#define EXIT 0x1ba 

/* Functionality */
/* User can generate lookup table from command line arguments. Therefore the
 * lookup table will be present in main. It needs to be passed to the console.
 *
 * When in the console, we can generate additional arrays of symbols, but it
 * would be too cumbersome to provide all the functionality (for example
 * noise) through the command line arguments.
 */
int console(Symbol *lookup, size_t lookup_len);

char **parse_input(char *input, size_t input_len, int *argc);
int handle_input(char **argv);

int free_argv(int argc, char ***argv);

int hash_cmd(const char *cmd);
void print_input(int argc, char **argv);
