#include "scop_math.h"

/* USEFUL MATRICES */

void mat2_get_zero(mat2_t ret)
{
    ret[0] = 0; ret[1] = 0; ret[2] = 0; ret[3] = 0;
}

void mat3_get_zero(mat3_t ret)
{
    ret[0] = 0; ret[1] = 0; ret[2] = 0; ret[3] = 0; ret[4] = 0; ret[5] = 0; ret[6] = 0; ret[7] = 0; ret[8] = 0;
}

void mat4_get_zero(mat4_t ret)
{
    ret[0] = 0; ret[1] = 0; ret[2] = 0; ret[3] = 0; ret[4] = 0; ret[5] = 0; ret[6] = 0; ret[7] = 0; ret[8] = 0; ret[9] = 0; ret[10] = 0; ret[11] = 0; ret[12] = 0; ret[13] = 0; ret[14] = 0; ret[15] = 0;
}

void mat2_get_identity(mat2_t ret)
{
    ret[0] = 1; ret[1] = 0; ret[2] = 0; ret[3] = 1;
}

void mat3_get_identity(mat3_t ret)
{
    ret[0] = 1; ret[1] = 0; ret[2] = 0; ret[3] = 0; ret[4] = 1; ret[5] = 0; ret[6] = 0; ret[7] = 0; ret[8] = 1;
}

void mat4_get_identity(mat4_t ret)
{
    ret[0] = 1; ret[1] = 0; ret[2] = 0; ret[3] = 0; ret[4] = 0; ret[5] = 1; ret[6] = 0; ret[7] = 0; ret[8] = 0; ret[9] = 0; ret[10] = 1; ret[11] = 0; ret[12] = 0; ret[13] = 0; ret[14] = 0; ret[15] = 1;
}

void mat4_get_proj_ortho(float L, float R, float B, float T, float N, float F, mat4_t ret)
{
    ret[0] = 2/(R - L); ret[4] = 0;         ret[8] = 0;             ret[12] = -(R + L)/((R - L));
    ret[1] = 0;         ret[5] = 2/(T - B); ret[9] = 0;             ret[13] = -(T + B)/((T - B));
    ret[2] = 0;         ret[6] = 0;         ret[10] = 2/(F - N);    ret[14] = -(F + N)/((F - N));
    ret[3] = 0;         ret[7] = 0;         ret[11] = 0;            ret[15] = 1;
}

void mat4_get_rotX(float angle, mat4_t ret)
{
    ret[0] = 1; ret[4] = 0;             ret[8] = 0;             ret[12] = 0;
    ret[1] = 0; ret[5] = cos(angle);    ret[9] = -sin(angle);   ret[13] = 0;
    ret[2] = 0; ret[6] = sin(angle);    ret[10] = cos(angle);   ret[14] = 0;
    ret[3] = 0; ret[7] = 0;             ret[11] = 0;            ret[15] = 1;
}

void mat4_get_rotY(float angle, mat4_t ret)
{
    ret[0] = cos(angle);    ret[4] = 0;             ret[8] = -sin(angle);   ret[12] = 0;
    ret[1] = 0;             ret[5] = 1;             ret[9] = 0;             ret[13] = 0;
    ret[2] = sin(angle);    ret[6] = 0;             ret[10] = cos(angle);   ret[14] = 0;
    ret[3] = 0;             ret[7] = 0;             ret[11] = 0;            ret[15] = 1;
}

void mat4_get_rotZ(float angle, mat4_t ret)
{
    ret[0] = cos(angle);    ret[4] = -sin(angle);   ret[8] = 0;     ret[12] = 0;
    ret[1] = sin(angle);    ret[5] = cos(angle);    ret[9] = 0;     ret[13] = 0;
    ret[2] = 0;             ret[6] = 0;             ret[10] = 1;    ret[14] = 0;
    ret[3] = 0;             ret[7] = 0;             ret[11] = 0;    ret[15] = 1;
}

/* LOGGING */

void vec2_print(vec2_t const v)
{
    printf("%f %f\n", v[0], v[1]);
}

void vec3_print(vec3_t const v)
{
    printf("%f %f %f\n", v[0], v[1], v[2]);
}

void vec4_print(vec4_t const v)
{
    printf("%f %f %f %f\n", v[0], v[1], v[2], v[3]);
}

void mat2_print(mat2_t const m)
{
    printf("%f %f\n", m[0], m[2]);
	printf("%f %f\n", m[1], m[3]);
}

void mat3_print(mat3_t const m)
{
    printf("%f %f %f\n", m[0], m[3], m[6]);
	printf("%f %f %f\n", m[1], m[4], m[7]);
	printf("%f %f %f\n", m[2], m[5], m[8]);
}

void mat4_print(mat4_t const m)
{
    printf("%f %f %f %f\n", m[0], m[4], m[8], m[12]);
    printf("%f %f %f %f\n", m[1], m[5], m[9], m[13]);
    printf("%f %f %f %f\n", m[2], m[6], m[10], m[14]);
    printf("%f %f %f %f\n", m[3], m[7], m[11], m[15]);
}

/* MATRIX DETERMINANT */

float mat2_det(mat2_t const m)
{
    return (m[0] * m[3] - m[2] * m[1]);
}

float mat3_det(mat3_t const m)
{
    return (m[0] * m[4] * m[8] + m[3] * m[7] * m[2] + m[1] * m[5] * m[6]
            - (m[6] * m[4] * m[2] + m[3] * m[1] * m[8] + m[7] * m[5] * m[0]));
}

float mat4_det(mat4_t const m)
{
    return (m[0] * (m[5] * m[10] * m[15] + m[9] * m[14] * m[7] + m[6] * m[11] * m[13] - (m[13] * m[10] * m[7] + m[9] * m[6] * m[15] + m[14] * m[11] * m[5]))
            - m[4] * (m[1] * m[10] * m[15] + m[9] * m[14] * m[3] + m[2] * m[11] * m[13] - (m[13] * m[10] * m[3] + m[9] * m[2] * m[15] + m[14] * m[11] * m[1]))
            + m[8] * (m[1] * m[6] * m[15] + m[5] * m[14] * m[3] + m[2] * m[7] * m[13] - (m[13] * m[6] * m[3] + m[5] * m[2] * m[15] + m[14] * m[7] * m[1]))
            - m[12] * (m[1] * m[6] * m[11] + m[5] * m[10] * m[3] + m[2] * m[7] * m[9] - (m[9] * m[6] * m[3] + m[5] * m[2] * m[11] + m[10] * m[7] * m[1])));
}

/* MATRIX INVERSE */

void mat2_inv(mat2_t const m, mat2_t ret)
{
    float det = m[0] * m[3] - m[2] * m[1];

    if (0 == det)
    {
        ret[0] = 0; ret[1] = 0; ret[2] = 0; ret[3] = 0;
        return ;
    }
    ret[0] = m[3] / det;
    ret[1] = -m[1] / det;
    ret[2] = -m[2] / det;
    ret[3] = m[0] / det;
}

void mat3_inv(mat3_t const m, mat3_t ret)
{
    float det = m[0] * m[4] * m[8] + m[3] * m[7] * m[2] + m[1] * m[5] * m[6]
            - (m[6] * m[4] * m[2] + m[3] * m[1] * m[8] + m[7] * m[5] * m[0]);

    if (0 == det)
    {
        ret[0] = 0; ret[1] = 0; ret[2] = 0; ret[3] = 0; ret[4] = 0; ret[5] = 0; ret[6] = 0; ret[7] = 0; ret[8] = 0;
        return ;
    }
    ret[0] = (m[4] * m[8] - m[7] * m[5]) / det;
    ret[1] = -(m[1] * m[8] - m[7] * m[2]) / det;
    ret[2] = (m[1] * m[5] - m[4] * m[2]) / det;

    ret[3] = -(m[3] * m[8] - m[6] * m[5]) / det;
    ret[4] = (m[0] * m[8] - m[6] * m[2]) / det;
    ret[5] = -(m[0] * m[5] - m[3] * m[2]) / det;

    ret[6] = (m[3] * m[7] - m[6] * m[4]) / det;
    ret[7] = -(m[0] * m[7] - m[6] * m[1]) / det;
    ret[8] = (m[0] * m[4] - m[3] * m[1]) / det;
}

void mat4_inv(mat4_t const m, mat4_t ret)
{
    float det = m[0] * (m[5] * m[10] * m[15] + m[9] * m[14] * m[7] + m[6] * m[11] * m[13] - (m[13] * m[10] * m[7] + m[9] * m[6] * m[15] + m[14] * m[11] * m[5]))
            - m[4] * (m[1] * m[10] * m[15] + m[9] * m[14] * m[3] + m[2] * m[11] * m[13] - (m[13] * m[10] * m[3] + m[9] * m[2] * m[15] + m[14] * m[11] * m[1]))
            + m[8] * (m[1] * m[6] * m[15] + m[5] * m[14] * m[3] + m[2] * m[7] * m[13] - (m[13] * m[6] * m[3] + m[5] * m[2] * m[15] + m[14] * m[7] * m[1]))
            - m[12] * (m[1] * m[6] * m[11] + m[5] * m[10] * m[3] + m[2] * m[7] * m[9] - (m[9] * m[6] * m[3] + m[5] * m[2] * m[11] + m[10] * m[7] * m[1]));

    if (0 == det)
    {
        ret[0] = 0; ret[1] = 0; ret[2] = 0; ret[3] = 0; ret[4] = 0; ret[5] = 0; ret[6] = 0; ret[7] = 0; ret[8] = 0;
        ret[9] = 0; ret[10] = 0; ret[11] = 0; ret[12] = 0; ret[13] = 0; ret[14] = 0; ret[15] = 0;
        return ;
    }
    ret[0] = (m[5] * m[10] * m[15] + m[9] * m[14] * m[7] + m[6] * m[11] * m[13] - (m[13] * m[10] * m[7] + m[9] * m[6] * m[15] + m[14] * m[11] * m[5])) / det;
    ret[4] = -(m[4] * m[10] * m[15] + m[8] * m[14] * m[7] + m[6] * m[11] * m[12] - (m[12] * m[10] * m[7] + m[8] * m[6] * m[15] + m[14] * m[11] * m[4])) / det;
    ret[8] = (m[4] * m[9] * m[15] + m[8] * m[13] * m[7] + m[5] * m[11] * m[12] - (m[12] * m[9] * m[7] + m[8] * m[5] * m[15] + m[13] * m[11] * m[4])) / det;
    ret[12] = -(m[4] * m[9] * m[14] + m[8] * m[13] * m[6] + m[5] * m[10] * m[12] - (m[12] * m[9] * m[6] + m[8] * m[5] * m[14] + m[13] * m[10] * m[4])) / det;

    ret[1] = -(m[1] * m[10] * m[15] + m[9] * m[14] * m[3] + m[2] * m[11] * m[13] - (m[13] * m[10] * m[3] + m[9] * m[2] * m[15] + m[14] * m[11] * m[1])) / det;
    ret[5] = (m[0] * m[10] * m[15] + m[8] * m[14] * m[3] + m[2] * m[11] * m[12] - (m[12] * m[10] * m[3] + m[8] * m[2] * m[15] + m[14] * m[11] * m[0])) / det;
    ret[9] = -(m[0] * m[9] * m[15] + m[8] * m[13] * m[3] + m[1] * m[11] * m[12] - (m[12] * m[9] * m[3] + m[8] * m[1] * m[15] + m[13] * m[11] * m[0])) / det;
    ret[13] = (m[0] * m[6] * m[11] + m[4] * m[10] * m[3] + m[2] * m[7] * m[8] - (m[8] * m[6] * m[3] + m[4] * m[2] * m[11] + m[10] * m[7] * m[0])) / det;

    
    ret[2] = (m[1] * m[6] * m[15] + m[5] * m[14] * m[3] + m[2] * m[7] * m[13] - (m[13] * m[6] * m[3] + m[5] * m[2] * m[15] + m[14] * m[7] * m[1])) / det;
    ret[6] = -(m[0] * m[6] * m[15] + m[4] * m[14] * m[3] + m[2] * m[7] * m[12] - (m[12] * m[6] * m[3] + m[4] * m[2] * m[15] + m[14] * m[7] * m[0])) / det;
    ret[10] = (m[0] * m[5] * m[15] + m[4] * m[13] * m[3] + m[1] * m[7] * m[12] - (m[12] * m[5] * m[3] + m[4] * m[1] * m[15] + m[13] * m[7] * m[0])) / det;
    ret[14] = -(m[0] * m[5] * m[14] + m[4] * m[13] * m[2] + m[1] * m[6] * m[12] - (m[12] * m[5] * m[2] + m[4] * m[1] * m[14] + m[13] * m[6] * m[0])) / det;


    ret[3] = -(m[1] * m[6] * m[11] + m[5] * m[10] * m[3] + m[2] * m[7] * m[9] - (m[9] * m[6] * m[3] + m[5] * m[2] * m[11] + m[10] * m[7] * m[1])) / det;
    ret[7] = (m[0] * m[9] * m[14] + m[8] * m[13] * m[2] + m[1] * m[10] * m[12] - (m[12] * m[9] * m[2] + m[8] * m[1] * m[14] + m[13] * m[10] * m[0])) / det;
    ret[11] = -(m[0] * m[5] * m[11] + m[4] * m[9] * m[3] + m[1] * m[7] * m[8] - (m[8] * m[5] * m[3] + m[4] * m[1] * m[11] + m[9] * m[7] * m[0])) / det;
    ret[15] = (m[0] * m[5] * m[10] + m[4] * m[9] * m[2] + m[1] * m[6] * m[8] - (m[8] * m[5] * m[2] + m[4] * m[1] * m[10] + m[9] * m[6] * m[0])) / det;
}

/* MATRIX TRANSPOSE */

void mat2_tras(mat2_t const m, mat2_t ret)
{
    ret[0] = m[0]; ret[1] = m[2]; ret[2] = m[1]; ret[3] = m[3];
}

void mat3_tras(mat3_t const m, mat3_t ret)
{
    ret[0] = m[0]; ret[1] = m[3]; ret[2] = m[6]; ret[3] = m[1]; ret[4] = m[4]; ret[5] = m[7]; ret[6] = m[2]; ret[7] = m[5]; ret[8] = m[8];
}

void mat4_tras(mat4_t const m, mat4_t ret)
{
    ret[0] = m[0]; ret[1] = m[4]; ret[2] = m[8]; ret[3] = m[12]; ret[4] = m[1]; ret[5] = m[5]; ret[6] = m[9]; ret[7] = m[13]; ret[8] = m[2]; ret[9] = m[6]; ret[10] = m[10]; ret[11] = m[14]; ret[12] = m[3]; ret[13] = m[7]; ret[14] = m[11]; ret[15] = m[15];
}

/* OPERATIONS */

void mat2_mult_vec2(mat2_t m1, vec2_t v, vec2_t ret)
{
    ret[0] = m1[0] * v[0] + m1[2] * v[1];
    ret[1] = m1[1] * v[0] + m1[3] * v[1];
}

void mat3_mult_vec3(mat3_t m1, vec3_t v, vec3_t ret)
{
    ret[0] = m1[0] * v[0] + m1[3] * v[1] + m1[6] * v[2];
    ret[1] = m1[1] * v[0] + m1[4] * v[1] + m1[7] * v[2];
    ret[2] = m1[2] * v[0] + m1[5] * v[1] + m1[8] * v[2];
}

void mat4_mult_vec4(mat4_t m1, vec4_t v, vec4_t ret)
{
    ret[0] = m1[0] * v[0] + m1[4] * v[1] + m1[8] * v[2] + m1[12] * v[3];
    ret[1] = m1[1] * v[0] + m1[5] * v[1] + m1[9] * v[2] + m1[13] * v[3];
    ret[2] = m1[2] * v[0] + m1[6] * v[1] + m1[10] * v[2] + m1[14] * v[3];
    ret[3] = m1[3] * v[0] + m1[7] * v[1] + m1[11] * v[2] + m1[15] * v[3];
}

/* MULTIPLICATION */

void mat2_mult_mat2(mat2_t m1, mat2_t m2, mat2_t ret)
{
    ret[0] = m1[0] * m2[0] + m1[2] * m2[1];
    ret[1] = m1[1] * m2[0] + m1[3] * m2[1];
    ret[2] = m1[0] * m2[2] + m1[2] * m2[3];
    ret[3] = m1[1] * m2[2] + m1[3] * m2[3];
}

void mat3_mult_mat3(mat3_t m1, mat3_t m2, mat3_t ret)
{
    ret[0] = m1[0] * m2[0] + m1[3] * m2[1] + m1[6] * m2[2];
    ret[1] = m1[1] * m2[0] + m1[4] * m2[1] + m1[7] * m2[2];
    ret[2] = m1[2] * m2[0] + m1[5] * m2[1] + m1[8] * m2[2];
    ret[3] = m1[0] * m2[3] + m1[3] * m2[4] + m1[6] * m2[5];
    ret[4] = m1[1] * m2[3] + m1[4] * m2[4] + m1[7] * m2[5];
    ret[5] = m1[2] * m2[3] + m1[5] * m2[4] + m1[8] * m2[5];
    ret[6] = m1[0] * m2[6] + m1[3] * m2[7] + m1[6] * m2[8];
    ret[7] = m1[1] * m2[6] + m1[4] * m2[7] + m1[7] * m2[8];
    ret[8] = m1[2] * m2[6] + m1[5] * m2[7] + m1[8] * m2[8];
}

void mat4_mult_mat4(mat4_t m1, mat4_t m2, mat4_t ret)
{
    ret[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
    ret[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
    ret[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
    ret[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];
    ret[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
    ret[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
    ret[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
    ret[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];
    ret[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
    ret[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
    ret[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
    ret[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];
    ret[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
    ret[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
    ret[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    ret[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
}

/* ADDITION */

void mat2_sum_mat2(mat2_t m1, mat2_t m2, mat2_t ret)
{
    ret[0] = m1[0] + m2[0];
    ret[1] = m1[1] + m2[1];
    ret[2] = m1[2] + m2[2];
    ret[3] = m1[3] + m2[3];
}

void mat3_sum_mat3(mat3_t m1, mat3_t m2, mat3_t ret)
{
    ret[0] = m1[0] + m2[0];
    ret[1] = m1[1] + m2[1];
    ret[2] = m1[2] + m2[2];
    ret[3] = m1[3] + m2[3];
    ret[4] = m1[4] + m2[4];
    ret[5] = m1[5] + m2[5];
    ret[6] = m1[6] + m2[6];
    ret[7] = m1[7] + m2[7];
    ret[8] = m1[8] + m2[8];
}

void mat4_sum_mat4(mat4_t m1, mat4_t m2, mat4_t ret)
{
    ret[0] = m1[0] + m2[0];
    ret[1] = m1[1] + m2[1];
    ret[2] = m1[2] + m2[2];
    ret[3] = m1[3] + m2[3];
    ret[4] = m1[4] + m2[4];
    ret[5] = m1[5] + m2[5];
    ret[6] = m1[6] + m2[6];
    ret[7] = m1[7] + m2[7];
    ret[8] = m1[8] + m2[8];
    ret[9] = m1[9] + m2[9];
    ret[10] = m1[10] + m2[10];
    ret[11] = m1[11] + m2[11];
    ret[12] = m1[12] + m2[12];
    ret[13] = m1[13] + m2[13];
    ret[14] = m1[14] + m2[14];
    ret[15] = m1[15] + m2[15];
}

/* SUBSTRACTION */

void mat2_sub_mat2(mat2_t m1, mat2_t m2, mat2_t ret)
{
    ret[0] = m1[0] - m2[0];
    ret[1] = m1[1] - m2[1];
    ret[2] = m1[2] - m2[2];
    ret[3] = m1[3] - m2[3];
}

void mat3_sub_mat3(mat3_t m1, mat3_t m2, mat3_t ret)
{
    ret[0] = m1[0] - m2[0];
    ret[1] = m1[1] - m2[1];
    ret[2] = m1[2] - m2[2];
    ret[3] = m1[3] - m2[3];
    ret[4] = m1[4] - m2[4];
    ret[5] = m1[5] - m2[5];
    ret[6] = m1[6] - m2[6];
    ret[7] = m1[7] - m2[7];
    ret[8] = m1[8] - m2[8];
}

void mat4_sub_mat4(mat4_t m1, mat4_t m2, mat4_t ret)
{
    ret[0] = m1[0] - m2[0];
    ret[1] = m1[1] - m2[1];
    ret[2] = m1[2] - m2[2];
    ret[3] = m1[3] - m2[3];
    ret[4] = m1[4] - m2[4];
    ret[5] = m1[5] - m2[5];
    ret[6] = m1[6] - m2[6];
    ret[7] = m1[7] - m2[7];
    ret[8] = m1[8] - m2[8];
    ret[9] = m1[9] - m2[9];
    ret[10] = m1[10] - m2[10];
    ret[11] = m1[11] - m2[11];
    ret[12] = m1[12] - m2[12];
    ret[13] = m1[13] - m2[13];
    ret[14] = m1[14] - m2[14];
    ret[15] = m1[15] - m2[15];
}