/* Mapper function for Stellsim.
 *
 * Douglas McCulloch, September 2023
 */

#include <string.h>
#include <stdint.h>

#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

#include "Symbols.h"
#include "filehandling.h"

#define TEMP_FILENAME "tmp_string"

/*                  */
/* Mapper functions */
/*                  */
/*         (default text mode for now) */
int mapper(FILE *i_s, FILE *o_s, Symbol *lookup, size_t lookup_len);

/* string wrapper */
int mapper_s(char *s, FILE *o_s, Symbol *lookup, size_t lookup_len);

/*                      */
/* Demapper functions   */
/*                      */
/*          (default float csv) output is bin or ASCII */
int demapper(FILE *i_s, FILE *o_s, Symbol *lookup, size_t lookup_len);
