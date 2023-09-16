/* A library to generate normally distributed values.
 *
 * Douglas McCulloch, September 2023
 */

#include <stdlib.h>
#include <math.h>

#include "Symbols.h"

int normal_distribution(double *z0, double *z1);

/* apply noise to a symbol array. Destructive. */
int amplitude_noise(Symbol *s, size_t len, double gain);
int phase_noise(Symbol *s, size_t len, double gain);
