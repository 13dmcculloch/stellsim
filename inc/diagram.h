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

#define ROWS 30
#define COLS 80
#define HOR_GAIN 2.0

/* Print the diagram of associated lookup table on UNIX */
void print_diagram(const Symbol *s, size_t len, char type);

/* Plotters */
void plot_mask(const Symbol *s, size_t len, size_t cols, size_t rows,\
                float mc);
void plot_symbols(const Symbol *s, size_t len, size_t cols, size_t rows,\
                float mc);

/* Set up functions */
void draw_axes(size_t cols, size_t rows);
