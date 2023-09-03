/* Symbol reference variable declarations.
 *
 * Douglas McCulloch, 09/23
 */

#include "Symbols.h"


Symbol *construct_lookup(const Symbol_Ref *r, size_t len)
{
    Symbol *s = malloc(len * sizeof(Symbol));

    for(size_t i = 0; i < len; i++)
    {
        double r_i = r[i].i;
        double r_q = r[i].q;

        //(x > 0) - (x < 0) == sgn (x)
        double s_i = ((r_i > 0) - (r_i < 0)) * sqrt(abs(r_i));
        double s_q = ((r_q > 0) - (r_q < 0)) * sqrt(abs(r_q));

        s[i].i = s_i;
        s[i].q = s_q;

        s[i].mag = hypot(s_i, s_q);
        s[i].arg = atan(s_q / s_i);

        s[i].val = i;
    }

    return s;
}

Symbol *gen_lookup_QAM(int M, char flag, size_t *len)
{
    printf("Generating lookup table...");

    Symbol *s = NULL;

    switch(flag)
    {
        case 'd':
            // dumb
            s = gen_lookup_QAM_dumb(M, len);
            break;
        default:
            printf("\ngen_lookup_QAM: Flag not recognised.\n");
    }

    if(s != NULL) printf("done.\n");

    return s;
}

Symbol *gen_lookup_QAM_dumb(int M, size_t *len)
{
    if(M % 2)
    {
        printf("Modulation order must be even for 16, 64, 256-QAM!\n");
        return NULL;
    }

    *len = M; 
    M = sqrt(M);
    Symbol *s = malloc(M * sizeof(*s));

    int q_start = -1 * M / 2;
    int q_stop = M / 2;
    int dec_flag = 1;
    int idx = 0;
    int amp = M / 2;

    for(int i = q_start; i <= q_stop; i++)
    {
        if(!i) continue;
        
        if(dec_flag)
        {
            for(int q = amp; q >= -1 * amp; q--)
            {
                if(!q) continue;
                
                s[idx] = set_symbol(i, q, idx);

                idx++;
                dec_flag = 0;
            }
        }
        else
        {
            for(int q = -1 * amp; q <= amp; q++)
            {
                if(!q) continue;

                s[idx] = set_symbol(i, q, idx);
                
                idx++;
                dec_flag = 1;
            }
        }
    }

    return s;
}

void print_symbol(const Symbol s)
{
    double arg_d = s.arg * 180 / M_PI;
    printf("%d: i = \t%f, q = \t%f, mag = \t%f, arg = \t%f deg", 
        s.val, s.i, s.q, s.mag, arg_d);
}

void print_lookup(const Symbol *l, size_t len)
{
    for(size_t i = 0; i < len; i++)
    {
        print_symbol(l[i]);
        printf("\n");
    }
}

double max_i(const Symbol *s, size_t len)
{
    /* an awful algorithm to get maximum in-phase value for print_diagram */
    double max = 0;

    for(size_t i = 0; i < len; i++)
    {
       if(s[i].i > max) max = s[i].i;
    }

    return max;
}

Symbol set_symbol(int i, int q, int val)
{
    Symbol s; 

    s.i = i;
    s.q = q;

    s.mag = hypot(i, q);
    s.arg = atan(q / i);
    
    s.val = val;

    return s;
}


const Symbol_Ref BPSK[2] = {
    {-1,    0},
    {1,     0}
};

const Symbol_Ref QPSK[4] = {
    {-1,    -1},
    {-1,    1},
    {1,     -1},
    {1,     1}
};

const Symbol_Ref HPSK[8] = {
    {-1,    -1},
    {-2,    0},
    {0,     2},
    {-1,    1},
    {0,     -2},
    {1,     -1},
    {1,     1},
    {2,     0}
};

/* For higher modulation orders, it may be desireable to implement algorithms
 * to generate a reference lookup table.
 */
