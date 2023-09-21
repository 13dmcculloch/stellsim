/* The goal of this library is to take in a variety of file formats describing
 * symbols that have been pre-generated. A square-root of integer notation
 * and float notation are two functions that should be considered at first.
 *
 * These functions allow the user to implement their own constellation
 * generation algorithms and load them into stellsim.
 *
 * Douglas McCulloch, September 2023
 */

#include "Symbols.h"

/* Return lookup tables */
Symbol *file_int(const char *filename, size_t *len);  // sqrt int notation
Symbol *file_dbl(const char *filename, size_t *len);  // float notation
Symbol *file_plr(const char *filename, size_t *len);  // polar notation

/* io functions */
int stream_symbolf(const Symbol *s, FILE *o_s);
int benchmark(FILE *o_s);

/* Generic file functions */
FILE *open_file(const char *filename);
int save_sample(Symbol *s, size_t len, const char *filename);
size_t count_lines(FILE *fp);
