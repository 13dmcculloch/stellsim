/* A console to interface with stellsim.
 *
 * Douglas McCulloch, September 2023
 */

#include "Symbols.h"
#include "diagram.h"
#include "filehandling.h"
#include "macros.h"

/* Commands */
#define EXIT_COMMAND "exit"

/* Functionality */
/* User can generate lookup table from command line arguments. Therefore the
 * lookup table will be present in main. It needs to be passed to the console.
 *
 * When in the console, we can generate additional arrays of symbols, but it
 * would be too cumbersome to provide all the functionality (for example
 * noise) through the command line arguments.
 */
int console(Symbol *lookup, size_t lookup_len);
int handle_input(const char *input);
