/* Given a reference lookup table, print a constellation diagram.
 *
 * Douglas McCulloch, 09/23
 */

#include "diagram.h"

void print_diagram(const Symbol *s, size_t len, char type)
{

    /* to "amplify" symbols to retain information after cast to int */
    const float MUL_CONST = 10.0 / max_i(s, len); 

    draw_axes(COLS, ROWS);

    switch(type)
    {
    case 'm':
        plot_mask(s, len, COLS, ROWS, MUL_CONST);
        break;
    case 's':
        plot_symbols(s, len, COLS, ROWS, MUL_CONST);
        break;
    default:
        printf("Could not determine symbol plot type.\n");
    }

    /* put cursor back in a sociable place */
    gotoxy(0, ROWS);
}

void plot_mask(const Symbol *s, size_t len, size_t cols, size_t rows,\
                    float mc)
{
    int origin_x = cols / 2;
    int origin_y = rows / 2;

    for(int i = 0; i < len; i++)
    {
        int sym_x = origin_x + (int)(s[i].i * mc * HOR_GAIN);
        // negative deviation from origin because direction code for term -ve
        int sym_y = origin_y - (int)(s[i].q * mc);

        gotoxy(sym_x, sym_y);

        printf("%x", s[i].val);
    }
}

void plot_symbols(const Symbol *s, size_t len, size_t cols, size_t rows,\
                    float mc)
{
    int origin_x = cols / 2;
    int origin_y = rows / 2;

    for(int i = 0; i < len; i++)
    {
        int sym_x = origin_x + (int)(s[i].i * mc * HOR_GAIN);
        // negative deviation from origin because direction code for term -ve
        int sym_y = origin_y - (int)(s[i].q * mc);

        gotoxy(sym_x, sym_y);

        printf(".");
    }
}

void draw_axes(size_t cols, size_t rows)
{
    int origin_x = cols / 2;
    int origin_y = rows / 2;

    /* draw the x axis */
    clear();
    gotoxy(0, origin_y);
    for(int i = 0; i < cols; i++) printf("-");

    /* Print in-phase label */
    //gotoxy(ROWS - 4, origin_y - 2);
    printf("I");

    /* Print quadrature label slightly offset to avoid being covered up */
    int ypos = 0;
    gotoxy(origin_x + 5, ypos + 2);
    printf("Q");

    /* draw the y axis*/
    gotoxy(origin_x, ypos);
    for(int i = 0; i < rows; i++)
    {
        printf("|");
        gotoxy(origin_x, ++ypos);
    }
}
