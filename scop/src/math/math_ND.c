#include "scop_math.h"

/* USEFUL MATRICES */

void matN_get_zero(unsigned int n, matN_t ret)
{
    for (unsigned int i = 0; i < n; i++)
        ret[i] = 0;
}

void matN_get_identity(unsigned int n, matN_t ret)
{
    for (unsigned int i = 0; i < n; i++)
        if (!(i % (n + 1)))
            ret[i] = 1;
        else
            ret[i] = 0;
}

/* LOGGING */

void vecN_print(unsigned int n, vecN_t const v)
{
    for (unsigned int i = 0; i < n; i++)
        printf("%f", v[i]);
    printf("\n");
}

void matN_print(unsigned int n, matN_t const m)
{
    size_t total_size = n * n;
    for (unsigned int i = 0; i < n; i++)
        for (unsigned int k = i; k < total_size; k += n)
            printf("%f", m[k]);
}