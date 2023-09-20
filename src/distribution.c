/* Normal distributions.
 *
 * Douglas McCulloch, September 2023
 */

#include "distribution.h"

int normal_distribution(double *z0, double *z1)
{
    /* Box-Muller transform is easy. Needn't be fast. */
    double u1 = rand() / (float) RAND_MAX;
    double u2 = rand() / (float) RAND_MAX;

    double R2 = -2 * log(u1);
    double theta = 2 * M_PI * u2;

    *z0 = sqrt(R2) * cos(theta);
    *z1 = sqrt(R2) * sin(theta);

    return 0;
}

int amplitude_noise(Symbol *s, size_t len, double gain)
{
    double app, last;

    for(int i = 0; i < len; ++i)
    {
        if(normal_distribution(&app, &last))
        {
            fprintf(stderr, "distribution.h - failed to compute normal"
                " distribution.\n");
            return 1;
        }

        s[i] = noise_symbol(s[i], app * gain, 0);
    }

    return 0;
}

    
int phase_noise(Symbol *s, size_t len, double gain)
{
    double app, last;

    for(int i = 0; i < len; ++i)
    {
        if(normal_distribution(&app, &last))
        {
            fprintf(stderr, "distribution.h - failed to compute normal"
                " distribution.\n");
            return 1;
        }

        /* convert to radians */
        app = app * M_PI / 180;

        s[i] = noise_symbol(s[i], 0, app * gain);
    }

    return 0;
}
