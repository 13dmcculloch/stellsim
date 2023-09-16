#include <math.h>
#include <string.h>

#include "diagram_portable.h"
#include "Symbols.h"

int print_dot(const Symbol *s, size_t len, int *sym_val, int row, int col);
int ins_value(char *buf, int value, int row);

void draw_symbols(const Symbol *s, size_t len, char type)
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

            /* symbols */
            int sym_val;
            
            if((type == 'v') && print_dot(s, len, &sym_val, i, j)) 
            {
                ins_value(buf, sym_val, j);
                goto L1;
            }

            else if(print_dot(s, len, &sym_val, i, j)) c = '.';

            /* write to buffer */
            buf[j] = c;

            /* labels */
            L1:
            if(!i) buf[origin_cols + 2] = 'Q';
            if(i == (origin_rows - 1)) buf[COLS - 1] = 'I';
        }

        if(fputs(buf, stdout) == EOF) fprintf(stderr, 
            "[ERROR]: could not print buffer\n");

        if(i != ROWS - 1) fputs("\n", stdout);
    }
}


int print_dot(const Symbol *s, size_t len, int *sym_val, int row, int col)
{
    /* normalise gain */
    double vert_gain = ROWS / max_i(s, len) / 4;
    double hor_gain = COLS / max_i(s,len) / 4;

    for(int i = 0; i < len; i++)
    {
        int in = ceil(s[i].i * hor_gain) + (COLS / 2);
        int qd = ceil(s[i].q * vert_gain) + (ROWS / 2);

        if((row == qd) && (col == in)) 
        {
            *sym_val = s[i].val;  /* it could be 0 */
            return 1;
        }
    }

    return 0;
}

int ins_value(char *buf, int value, int col)
{
    /* inserts an n-char hex value into buffer */
    char val_buf[32];
    sprintf(val_buf, "%x", value);

    int idx = 0;
    for(int i = col; i < (col + strlen(val_buf)); ++i)
    {
        buf[i] = val_buf[idx];
        ++idx;
    }

    return idx;
}
