/* Sandbox for testing functions.
 */

#define FMT HPSK

//#include "Symbols.h"
#include "diagram.h"

void symbol_size();

int main()
{
    /*
    Symbol *lookup = construct_lookup(FMT, LOOKUP_LEN(FMT));

    print_diagram(lookup, LOOKUP_LEN(FMT));
    */ 

    size_t len;
    int M = 4;

    Symbol *lookup = gen_lookup_QAM(M, 'd', &len);

    print_lookup(lookup, len);
    print_diagram(lookup, len);

    free(lookup);

    return 0;
}

void symbol_size()
{
    printf("sizeof(Symbol) = %ld B\nsizeof(Symbol_Ref) = %ld B\nsizeof(size_t) \
        = %ld B\n", sizeof(Symbol), sizeof(Symbol_Ref), sizeof(size_t));


}
