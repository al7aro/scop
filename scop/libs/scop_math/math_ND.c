#include "scop_math.h"

/* USEFUL MATRICES */

void matN_get_zero(unsigned int n, matN_t ret)
{
    for (unsigned int i = 0; i < (n * n); i++)
        ret[i] = 0;
}

void matN_get_identity(unsigned int n, matN_t ret)
{
    for (unsigned int i = 0; i < (n * n); i++)
        if (!(i % (n + 1)))
            ret[i] = 1;
        else
            ret[i] = 0;
}

/* LOGGING */

void vecN_print(vecN_t const v, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++)
        printf("%15.3f", v[i]);
    printf("\n");
}

void matN_print(matN_t const m, unsigned int n)
{
    size_t total_size = n * n;
    for (unsigned int i = 0; i < n; i++)
    {
        for (unsigned int k = i; k < total_size; k += n)
            printf("%15.3f", m[k]);
        printf("\n");
    }
}

/* MATRIX OPERATIONS */
void matN_tras(matN_t const m, unsigned int n, matN_t ret)
{
    size_t total_size = n * n;
    for (unsigned int i = 0, j = 0; i < n; i++)
        for (unsigned int k = i; k < total_size; k += n, j++)
            ret[k] = m[j];
}

void matN_sum_matN(matN_t m1, matN_t m2, unsigned int n, matN_t ret)
{
    for (unsigned int i = 0; i < (n * n); i++)
        ret[i] = m1[i] + m2[i];
}

void matN_sub_matN(matN_t m1, matN_t m2, unsigned int n, matN_t ret)
{
    for (unsigned int i = 0; i < (n * n); i++)
        ret[i] = m1[i] - m2[i];
}

void matN_mult_matN(matN_t m1, matN_t m2, unsigned int n, matN_t ret)
{
    for (unsigned int i = 0; i < (n * n); i++)
    {
        int current_row = i % n;
        int current_col = i / n;
        ret[i] = 0;
        for (unsigned int row = current_row, col = (current_col * n); row < (n * n) && col < ((current_col + 1) * n); row += n, col++)
            ret[i] += m1[row] * m2[col];
    }
}

void matN_mult_vecN(matN_t m1, vecN_t v, unsigned int n, vecN_t ret)
{
    for (unsigned int i = 0; i < n; i++)
    {
        int current_row = i % n;
        ret[i] = 0;
        for (unsigned int row = current_row, col = 0; row < (n * n) && col < n; row += n, col++)
            ret[i] += m1[row] * v[col];
    }
}
