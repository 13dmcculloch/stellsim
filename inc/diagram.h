/* Given a reference lookup table, print a constellation diagram.
 *
 * Todo:
 *      - portable implementation
 *      - use terminal size
 *      - format specification of different modulation orders
          for example, a modulation order that encodes with a word size % 4
          == 0 would be easily represented by hexadecimal. I don't know if this
          is really necessary
 *
 * Douglas McCulloch, 09/23
 */

#include "Symbols.h"

/* Print the diagram of associated lookup table on UNIX */
void print_diagram(const Symbol *s, size_t len);
