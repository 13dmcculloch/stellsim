/* Given a reference lookup table, print a constellation diagram.
 *
 * Douglas McCulloch, 09/23
 */

#include "diagram.h"

/* write a portable version of this later */
const int ROWS = 30;
const int COLS = 80;

/* make up for coordinates being skewed */
const float HOR_GAIN = 2.0;

/* set up the origin etc */
const int origin_x = COLS / 2;
const int origin_y = ROWS / 2;

void print_diagram(const Symbol *s, size_t len)
{

    /* to "amplify" symbols to retain information after cast to int */
    const int M = sqrt(len);
    const float MUL_CONST = 20.0 / M;

    /* draw the x axis */
    clear();
    gotoxy(0, origin_y);
    for(int i = 0; i < COLS; i++) printf("-");

    /* Print in-phase label */
    //gotoxy(ROWS - 4, origin_y - 2);
    printf("I");

    /* Print quadrature label slightly offset to avoid being covered up */
    int ypos = 0;
    gotoxy(origin_x + 5, ypos + 2);
    printf("Q");

    /* draw the y axis*/
    gotoxy(origin_x, ypos);
    for(int i = 0; i < ROWS; i++)
    {
        printf("|");
        gotoxy(origin_x, ++ypos);
    }

    /* plot symbols on axes */
    for(int i = 0; i < len; i++)
    {
        int sym_x = origin_x + (int)(s[i].i * MUL_CONST * HOR_GAIN); 
        int sym_y = origin_y - (int)(s[i].q * MUL_CONST);  //opposite dir

        gotoxy(sym_x, sym_y);

        printf("%02x", s[i].val);
    }

    /* put cursor back in a sociable place */
    gotoxy(0, ROWS);
}
