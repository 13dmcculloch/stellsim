#include <math.h>
#include <string.h>

#include "diagram_portable.h"
#include "Symbols.h"

void draw_symbols(const Symbol *s, size_t len)
{
    const int origin_cols = COLS / 2;
    const int origin_rows = ROWS / 2;
    
    char buf[COLS];

    for(int i = 0; i < ROWS; ++i)
    {
        memset(buf, '\0', COLS);

        for(int j = 0; j < COLS; ++j)
        {
            char c = ' ';

            /* axes */
            if(i == origin_rows) c = '-';
            if(j == origin_cols) c = '|';

            if(print_dot(s, len, i, j)) c = '.';
            /*
            if(print_dot(symbol, len, i, j) && f_val)
                val_routine(val);
                
            else if(print_dot(symbol, len, i, j)) c = '.';
            */

            /* write to buffer */
            buf[j] = c;

            /* labels */
            if(!i) buf[origin_cols + 2] = 'Q';
            if(i == (origin_rows - 1)) buf[COLS - 1] = 'I';
        }

        if(puts(buf) == EOF) fprintf(stderr, 
            "[ERROR]: could not print buffer\n");
    }
}


int print_dot(const Symbol *s, size_t len, int row, int col)
{
    /* normalise gain */
    double gain = max_arg(s, len) * (ROWS / 2);
    gain = 1;

    for(int i = 0; i < len; i++)
    {
        int in = ceil(s[i].i * gain) + (COLS / 2);
        int qd = ceil(s[i].q * gain) + (ROWS / 2);

        if((row == qd) && (col == in)) return 1;
    }
    return 0;
}
