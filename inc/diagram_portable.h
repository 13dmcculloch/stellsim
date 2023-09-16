/* A portable implementation of diagram using puts()
 *
 * Douglas McCulloch, September 2023
 */

#include "Symbols.h"

#define COLS 80
#define ROWS 24

void draw_symbols(const Symbol *s, size_t len, char type);
