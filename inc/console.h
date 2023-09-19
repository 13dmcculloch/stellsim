/* A console to interface with stellsim.
 *
 * Douglas McCulloch, September 2023
 */

#include "Symbols.h"
#include "diagram_portable.h"
#include "distribution.h"
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
#define REF 317
#define BPSK_R 304
#define QPSK_R 319
#define HPSK_R 294

#define PRINTA 654
#define PRINTC 656
#define PRINTP 669

#define DRAW 0x1ae

#define CONCAT 632 

#define NOISE 542
#define AMPLITUDE 965
#define PHASE 529

#define LOAD 416
#define INTEGER 750
#define DOUBLE 635
#define POLAR 542

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

/*
int generate(char **argv, Symbol_Data *symbol);
int print_table(char **argv, Symbol_Data *symbol);
int draw_argand(char **argv, Symbol_Data *symbol);
int concat(char **argv, Symbol_Data *symbol);
int noise(char **argv, Symbol_Data *symbol);

int free_argv(int argc, char ***argv);

int hash_cmd(const char *cmd);
void print_input(int argc, char **argv);
*/
