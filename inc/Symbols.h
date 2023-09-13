/* Modulation format simulator.
 * 
 * Header defining symbol structs, reference lookup tables  etc.
 *
 * Todo:
 *  - see "build_lookup"
 *
 * Douglas McCulloch, 09/23
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

/* Macros */
#include "macros.h"

/* Symbol structs */
struct Symbol_Data
{
    Symbol *lookup;
    Symbol *sample;

    int M;
    size_t lookup_len;
    size_t sample_len;
}

typedef struct Symbol_Data Symbol_Data;

struct Symbol
{
    double i;
    double q;
    double mag;
    double arg;

    int val;  // actual value corresponding to symbol (should be == index)
    int count; // number of times symbol used (no memory impact)
};

typedef struct Symbol Symbol;

struct Symbol_Ref
{
    int i;
    int q;
};

typedef struct Symbol_Ref Symbol_Ref;

/* Functions */
// cartesian
Symbol set_symbol(int i, int q, int val);
Symbol set_symbolf(double i, double q, int val);

// polar
Symbol set_symbolp(double mag, double arg, int val);
Symbol noise_symbol(Symbol s, double mag, double arg);

// symbol functions
void print_symbol(const Symbol s);
void print_lookup(const Symbol *l, size_t len);  // print_symbols = confusing
double max_i(const Symbol *s, size_t len);

// construct a lookup table from an integer reference
Symbol *construct_lookup(const Symbol_Ref *r, size_t len);
// generate a lookup table from modulation order
// returns a decayed pointer to lookup table array, but also length
// flag = generation algorithm. Example: gray coding
Symbol *gen_lookup_QAM(int M, char flag, size_t *len);
Symbol *gen_lookup_QAM_dumb(int M, size_t *len);

/* Lookup tables */
extern const Symbol_Ref BPSK[2];
extern const Symbol_Ref QPSK[4];
extern const Symbol_Ref HPSK[8]; // 8PSK

#endif
