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

void mat4_get_proj_ortho(float left, float right, float bottom, float top, float near, float far, mat4_t ret)
{
    ret[0] = 2.0f/(right - left);   ret[4] = 0;                     ret[8] = 0;                     ret[12] = -(right + left)/((right - left));
    ret[1] = 0;                     ret[5] = 2.0f/(top - bottom);   ret[9] = 0;                     ret[13] = -(top + bottom)/((top - bottom));
    ret[2] = 0;                     ret[6] = 0;                     ret[10] = 2.0f/(far - near);    ret[14] = -(far + near)/((far - near));
    ret[3] = 0;                     ret[7] = 0;                     ret[11] = 0;                    ret[15] = 1.0f;
}

void mat4_get_proj_persp(float fov, float aspect, float near, float far, mat4_t ret)
{
    ret[0] = 1.0f/(aspect*(float)tan(fov/2.0)); ret[4] = 0;                         ret[8] = 0;                             ret[12] = 0;
    ret[1] = 0;                                 ret[5] = 1.0f/((float)tan(fov/2));  ret[9] = 0;                             ret[13] = 0;
    ret[2] = 0;                                 ret[6] = 0;                         ret[10] = -(far + near)/(far - near);   ret[14] = -(2.0f * far * near)/(far - near);
    ret[3] = 0;                                 ret[7] = 0;                         ret[11] = -1.0f;                        ret[15] = 0;
}

void mat4_get_rotX(float angle, mat4_t ret)
{
    ret[0] = 1; ret[4] = 0;                 ret[8] = 0;                     ret[12] = 0;
    ret[1] = 0; ret[5] = (float)cos(angle); ret[9] = -(float)sin(angle);    ret[13] = 0;
    ret[2] = 0; ret[6] = (float)sin(angle); ret[10] = (float)cos(angle);    ret[14] = 0;
    ret[3] = 0; ret[7] = 0;                 ret[11] = 0;                    ret[15] = 1;
}

void mat4_get_rotY(float angle, mat4_t ret)
{
    ret[0] = (float)cos(angle); ret[4] = 0; ret[8] = -(float)sin(angle);    ret[12] = 0;
    ret[1] = 0;                 ret[5] = 1; ret[9] = 0;                     ret[13] = 0;
    ret[2] = (float)sin(angle); ret[6] = 0; ret[10] = (float)cos(angle);    ret[14] = 0;
    ret[3] = 0;                 ret[7] = 0; ret[11] = 0;                    ret[15] = 1;
}

void mat4_get_rotZ(float angle, mat4_t ret)
{
    ret[0] = (float)cos(angle); ret[4] = -(float)sin(angle);    ret[8] = 0;     ret[12] = 0;
    ret[1] = (float)sin(angle); ret[5] = (float)cos(angle);     ret[9] = 0;     ret[13] = 0;
    ret[2] = 0;                 ret[6] = 0;                     ret[10] = 1;    ret[14] = 0;
    ret[3] = 0;                 ret[7] = 0;                     ret[11] = 0;    ret[15] = 1;
}

void mat4_get_tras(float x, float y, float z, mat4_t ret)
{
    ret[0] = 1; ret[4] = 0; ret[8] = 0;     ret[12] = x;
    ret[1] = 0; ret[5] = 1; ret[9] = 0;     ret[13] = y;
    ret[2] = 0; ret[6] = 0; ret[10] = 1;    ret[14] = z;
    ret[3] = 0; ret[7] = 0; ret[11] = 0;    ret[15] = 1;
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
    mat2_t m_aux;
    memcpy(m_aux, m, sizeof(mat2_t));
    float det = m_aux[0] * m_aux[3] - m_aux[2] * m_aux[1];

    if (0 == det)
    {
        ret[0] = 0; ret[1] = 0; ret[2] = 0; ret[3] = 0;
        return ;
    }
    ret[0] = m_aux[3] / det;
    ret[1] = -m_aux[1] / det;
    ret[2] = -m_aux[2] / det;
    ret[3] = m_aux[0] / det;
}

void mat3_inv(mat3_t const m, mat3_t ret)
{
    mat3_t m_aux;
    memcpy(m_aux, m, sizeof(mat3_t));
    float det = m_aux[0] * m_aux[4] * m_aux[8] + m_aux[3] * m_aux[7] * m_aux[2] + m_aux[1] * m_aux[5] * m_aux[6]
            - (m_aux[6] * m_aux[4] * m_aux[2] + m_aux[3] * m_aux[1] * m_aux[8] + m_aux[7] * m_aux[5] * m_aux[0]);

    if (0 == det)
    {
        ret[0] = 0; ret[1] = 0; ret[2] = 0; ret[3] = 0; ret[4] = 0; ret[5] = 0; ret[6] = 0; ret[7] = 0; ret[8] = 0;
        return ;
    }
    ret[0] = (m_aux[4] * m_aux[8] - m_aux[7] * m_aux[5]) / det;
    ret[1] = -(m_aux[1] * m_aux[8] - m_aux[7] * m_aux[2]) / det;
    ret[2] = (m_aux[1] * m_aux[5] - m_aux[4] * m_aux[2]) / det;

    ret[3] = -(m_aux[3] * m_aux[8] - m_aux[6] * m_aux[5]) / det;
    ret[4] = (m_aux[0] * m_aux[8] - m_aux[6] * m_aux[2]) / det;
    ret[5] = -(m_aux[0] * m_aux[5] - m_aux[3] * m_aux[2]) / det;

    ret[6] = (m_aux[3] * m_aux[7] - m_aux[6] * m_aux[4]) / det;
    ret[7] = -(m_aux[0] * m_aux[7] - m_aux[6] * m_aux[1]) / det;
    ret[8] = (m_aux[0] * m_aux[4] - m_aux[3] * m_aux[1]) / det;
}

void mat4_inv(mat4_t const m, mat4_t ret)
{
    mat4_t m_aux;
    memcpy(m_aux, m, sizeof(mat4_t));
    float det = m_aux[0] * (m_aux[5] * m_aux[10] * m_aux[15] + m_aux[9] * m_aux[14] * m_aux[7] + m_aux[6] * m_aux[11] * m_aux[13] - (m_aux[13] * m_aux[10] * m_aux[7] + m_aux[9] * m_aux[6] * m_aux[15] + m_aux[14] * m_aux[11] * m_aux[5]))
            - m_aux[4] * (m_aux[1] * m_aux[10] * m_aux[15] + m_aux[9] * m_aux[14] * m_aux[3] + m_aux[2] * m_aux[11] * m_aux[13] - (m_aux[13] * m_aux[10] * m_aux[3] + m_aux[9] * m_aux[2] * m_aux[15] + m_aux[14] * m_aux[11] * m_aux[1]))
            + m_aux[8] * (m_aux[1] * m_aux[6] * m_aux[15] + m_aux[5] * m_aux[14] * m_aux[3] + m_aux[2] * m_aux[7] * m_aux[13] - (m_aux[13] * m_aux[6] * m_aux[3] + m_aux[5] * m_aux[2] * m_aux[15] + m_aux[14] * m_aux[7] * m_aux[1]))
            - m_aux[12] * (m_aux[1] * m_aux[6] * m_aux[11] + m_aux[5] * m_aux[10] * m_aux[3] + m_aux[2] * m_aux[7] * m_aux[9] - (m_aux[9] * m_aux[6] * m_aux[3] + m_aux[5] * m_aux[2] * m_aux[11] + m_aux[10] * m_aux[7] * m_aux[1]));

    if (0 == det)
    {
        ret[0] = 0; ret[1] = 0; ret[2] = 0; ret[3] = 0; ret[4] = 0; ret[5] = 0; ret[6] = 0; ret[7] = 0; ret[8] = 0;
        ret[9] = 0; ret[10] = 0; ret[11] = 0; ret[12] = 0; ret[13] = 0; ret[14] = 0; ret[15] = 0;
        return ;
    }
    ret[0] = (m_aux[5] * m_aux[10] * m_aux[15] + m_aux[9] * m_aux[14] * m_aux[7] + m_aux[6] * m_aux[11] * m_aux[13] - (m_aux[13] * m_aux[10] * m_aux[7] + m_aux[9] * m_aux[6] * m_aux[15] + m_aux[14] * m_aux[11] * m_aux[5])) / det;
    ret[4] = -(m_aux[4] * m_aux[10] * m_aux[15] + m_aux[8] * m_aux[14] * m_aux[7] + m_aux[6] * m_aux[11] * m_aux[12] - (m_aux[12] * m_aux[10] * m_aux[7] + m_aux[8] * m_aux[6] * m_aux[15] + m_aux[14] * m_aux[11] * m_aux[4])) / det;
    ret[8] = (m_aux[4] * m_aux[9] * m_aux[15] + m_aux[8] * m_aux[13] * m_aux[7] + m_aux[5] * m_aux[11] * m_aux[12] - (m_aux[12] * m_aux[9] * m_aux[7] + m_aux[8] * m_aux[5] * m_aux[15] + m_aux[13] * m_aux[11] * m_aux[4])) / det;
    ret[12] = -(m_aux[4] * m_aux[9] * m_aux[14] + m_aux[8] * m_aux[13] * m_aux[6] + m_aux[5] * m_aux[10] * m_aux[12] - (m_aux[12] * m_aux[9] * m_aux[6] + m_aux[8] * m_aux[5] * m_aux[14] + m_aux[13] * m_aux[10] * m_aux[4])) / det;

    ret[1] = -(m_aux[1] * m_aux[10] * m_aux[15] + m_aux[9] * m_aux[14] * m_aux[3] + m_aux[2] * m_aux[11] * m_aux[13] - (m_aux[13] * m_aux[10] * m_aux[3] + m_aux[9] * m_aux[2] * m_aux[15] + m_aux[14] * m_aux[11] * m_aux[1])) / det;
    ret[5] = (m_aux[0] * m_aux[10] * m_aux[15] + m_aux[8] * m_aux[14] * m_aux[3] + m_aux[2] * m_aux[11] * m_aux[12] - (m_aux[12] * m_aux[10] * m_aux[3] + m_aux[8] * m_aux[2] * m_aux[15] + m_aux[14] * m_aux[11] * m_aux[0])) / det;
    ret[9] = -(m_aux[0] * m_aux[9] * m_aux[15] + m_aux[8] * m_aux[13] * m_aux[3] + m_aux[1] * m_aux[11] * m_aux[12] - (m_aux[12] * m_aux[9] * m_aux[3] + m_aux[8] * m_aux[1] * m_aux[15] + m_aux[13] * m_aux[11] * m_aux[0])) / det;
    ret[13] = (m_aux[0] * m_aux[6] * m_aux[11] + m_aux[4] * m_aux[10] * m_aux[3] + m_aux[2] * m_aux[7] * m_aux[8] - (m_aux[8] * m_aux[6] * m_aux[3] + m_aux[4] * m_aux[2] * m_aux[11] + m_aux[10] * m_aux[7] * m_aux[0])) / det;

    
    ret[2] = (m_aux[1] * m_aux[6] * m_aux[15] + m_aux[5] * m_aux[14] * m_aux[3] + m_aux[2] * m_aux[7] * m_aux[13] - (m_aux[13] * m_aux[6] * m_aux[3] + m_aux[5] * m_aux[2] * m_aux[15] + m_aux[14] * m_aux[7] * m_aux[1])) / det;
    ret[6] = -(m_aux[0] * m_aux[6] * m_aux[15] + m_aux[4] * m_aux[14] * m_aux[3] + m_aux[2] * m_aux[7] * m_aux[12] - (m_aux[12] * m_aux[6] * m_aux[3] + m_aux[4] * m_aux[2] * m_aux[15] + m_aux[14] * m_aux[7] * m_aux[0])) / det;
    ret[10] = (m_aux[0] * m_aux[5] * m_aux[15] + m_aux[4] * m_aux[13] * m_aux[3] + m_aux[1] * m_aux[7] * m_aux[12] - (m_aux[12] * m_aux[5] * m_aux[3] + m_aux[4] * m_aux[1] * m_aux[15] + m_aux[13] * m_aux[7] * m_aux[0])) / det;
    ret[14] = -(m_aux[0] * m_aux[5] * m_aux[14] + m_aux[4] * m_aux[13] * m_aux[2] + m_aux[1] * m_aux[6] * m_aux[12] - (m_aux[12] * m_aux[5] * m_aux[2] + m_aux[4] * m_aux[1] * m_aux[14] + m_aux[13] * m_aux[6] * m_aux[0])) / det;


    ret[3] = -(m_aux[1] * m_aux[6] * m_aux[11] + m_aux[5] * m_aux[10] * m_aux[3] + m_aux[2] * m_aux[7] * m_aux[9] - (m_aux[9] * m_aux[6] * m_aux[3] + m_aux[5] * m_aux[2] * m_aux[11] + m_aux[10] * m_aux[7] * m_aux[1])) / det;
    ret[7] = (m_aux[0] * m_aux[9] * m_aux[14] + m_aux[8] * m_aux[13] * m_aux[2] + m_aux[1] * m_aux[10] * m_aux[12] - (m_aux[12] * m_aux[9] * m_aux[2] + m_aux[8] * m_aux[1] * m_aux[14] + m_aux[13] * m_aux[10] * m_aux[0])) / det;
    ret[11] = -(m_aux[0] * m_aux[5] * m_aux[11] + m_aux[4] * m_aux[9] * m_aux[3] + m_aux[1] * m_aux[7] * m_aux[8] - (m_aux[8] * m_aux[5] * m_aux[3] + m_aux[4] * m_aux[1] * m_aux[11] + m_aux[9] * m_aux[7] * m_aux[0])) / det;
    ret[15] = (m_aux[0] * m_aux[5] * m_aux[10] + m_aux[4] * m_aux[9] * m_aux[2] + m_aux[1] * m_aux[6] * m_aux[8] - (m_aux[8] * m_aux[5] * m_aux[2] + m_aux[4] * m_aux[1] * m_aux[10] + m_aux[9] * m_aux[6] * m_aux[0])) / det;
}

/* MATRIX TRANSPOSE */

void mat2_tras(mat2_t const m, mat2_t ret)
{
    mat2_t m_aux;
    memcpy(m_aux, m, sizeof(mat2_t));
    ret[0] = m_aux[0];  ret[2] = m_aux[1];
    ret[1] = m_aux[2];  ret[3] = m_aux[3];
}

void mat3_tras(mat3_t const m, mat3_t ret)
{
    mat3_t m_aux;
    memcpy(m_aux, m, sizeof(mat3_t));
    ret[0] = m_aux[0];  ret[3] = m_aux[1];  ret[6] = m_aux[2];
    ret[1] = m_aux[3];  ret[4] = m_aux[4];  ret[7] = m_aux[5];
    ret[2] = m_aux[6];  ret[5] = m_aux[7];  ret[8] = m_aux[8];
}

void mat4_tras(mat4_t const m, mat4_t ret)
{
    mat4_t m_aux;
    memcpy(m_aux, m, sizeof(mat4_t));
    ret[0] = m_aux[0];  ret[4] = m_aux[1];  ret[8] = m_aux[2];      ret[12] = m_aux[3];
    ret[1] = m_aux[4];  ret[5] = m_aux[5];  ret[9] = m_aux[6];      ret[13] = m_aux[7];
    ret[2] = m_aux[8];  ret[6] = m_aux[9];  ret[10] = m_aux[10];    ret[14] = m_aux[11];
    ret[3] = m_aux[12]; ret[7] = m_aux[13]; ret[11] = m_aux[14];    ret[15] = m_aux[15];
}

/* OPERATIONS */

void mat2_mult_vec2(mat2_t m1, vec2_t v, vec2_t ret)
{
    vec2_t v_aux;
    memcpy(v_aux, v, sizeof(vec2_t));
    ret[0] = m1[0] * v_aux[0] + m1[2] * v_aux[1];
    ret[1] = m1[1] * v_aux[0] + m1[3] * v_aux[1];
}

void mat3_mult_vec3(mat3_t m1, vec3_t v, vec3_t ret)
{
    vec3_t v_aux;
    memcpy(v_aux, v, sizeof(vec3_t));
    ret[0] = m1[0] * v_aux[0] + m1[3] * v_aux[1] + m1[6] * v_aux[2];
    ret[1] = m1[1] * v_aux[0] + m1[4] * v_aux[1] + m1[7] * v_aux[2];
    ret[2] = m1[2] * v_aux[0] + m1[5] * v_aux[1] + m1[8] * v_aux[2];
}

void mat4_mult_vec4(mat4_t m1, vec4_t v, vec4_t ret)
{
    vec4_t v_aux;
    memcpy(v_aux, v, sizeof(vec4_t));
    ret[0] = m1[0] * v_aux[0] + m1[4] * v_aux[1] + m1[8] * v_aux[2] + m1[12] * v_aux[3];
    ret[1] = m1[1] * v_aux[0] + m1[5] * v_aux[1] + m1[9] * v_aux[2] + m1[13] * v_aux[3];
    ret[2] = m1[2] * v_aux[0] + m1[6] * v_aux[1] + m1[10] * v_aux[2] + m1[14] * v_aux[3];
    ret[3] = m1[3] * v_aux[0] + m1[7] * v_aux[1] + m1[11] * v_aux[2] + m1[15] * v_aux[3];
}

/* MULTIPLICATION */

void mat2_mult_mat2(mat2_t m1, mat2_t m2, mat2_t ret)
{
    mat2_t m1_aux, m2_aux;
    memcpy(m1_aux, m1, sizeof(mat2_t));
    memcpy(m2_aux, m2, sizeof(mat2_t));
    ret[0] = m1_aux[0] * m2_aux[0] + m1_aux[2] * m2_aux[1];
    ret[1] = m1_aux[1] * m2_aux[0] + m1_aux[3] * m2_aux[1];
    ret[2] = m1_aux[0] * m2_aux[2] + m1_aux[2] * m2_aux[3];
    ret[3] = m1_aux[1] * m2_aux[2] + m1_aux[3] * m2_aux[3];
}

void mat3_mult_mat3(mat3_t m1, mat3_t m2, mat3_t ret)
{
    mat3_t m1_aux, m2_aux;
    memcpy(m1_aux, m1, sizeof(mat3_t));
    memcpy(m2_aux, m2, sizeof(mat3_t));
    ret[0] = m1_aux[0] * m2_aux[0] + m1_aux[3] * m2_aux[1] + m1_aux[6] * m2_aux[2];
    ret[1] = m1_aux[1] * m2_aux[0] + m1_aux[4] * m2_aux[1] + m1_aux[7] * m2_aux[2];
    ret[2] = m1_aux[2] * m2_aux[0] + m1_aux[5] * m2_aux[1] + m1_aux[8] * m2_aux[2];
    ret[3] = m1_aux[0] * m2_aux[3] + m1_aux[3] * m2_aux[4] + m1_aux[6] * m2_aux[5];
    ret[4] = m1_aux[1] * m2_aux[3] + m1_aux[4] * m2_aux[4] + m1_aux[7] * m2_aux[5];
    ret[5] = m1_aux[2] * m2_aux[3] + m1_aux[5] * m2_aux[4] + m1_aux[8] * m2_aux[5];
    ret[6] = m1_aux[0] * m2_aux[6] + m1_aux[3] * m2_aux[7] + m1_aux[6] * m2_aux[8];
    ret[7] = m1_aux[1] * m2_aux[6] + m1_aux[4] * m2_aux[7] + m1_aux[7] * m2_aux[8];
    ret[8] = m1_aux[2] * m2_aux[6] + m1_aux[5] * m2_aux[7] + m1_aux[8] * m2_aux[8];
}

void mat4_mult_mat4(mat4_t m1, mat4_t m2, mat4_t ret)
{
    mat4_t m1_aux, m2_aux;
    memcpy(m1_aux, m1, sizeof(mat4_t));
    memcpy(m2_aux, m2, sizeof(mat4_t));
    ret[0] = m1_aux[0] * m2_aux[0] + m1_aux[4] * m2_aux[1] + m1_aux[8] * m2_aux[2] + m1_aux[12] * m2_aux[3];
    ret[1] = m1_aux[1] * m2_aux[0] + m1_aux[5] * m2_aux[1] + m1_aux[9] * m2_aux[2] + m1_aux[13] * m2_aux[3];
    ret[2] = m1_aux[2] * m2_aux[0] + m1_aux[6] * m2_aux[1] + m1_aux[10] * m2_aux[2] + m1_aux[14] * m2_aux[3];
    ret[3] = m1_aux[3] * m2_aux[0] + m1_aux[7] * m2_aux[1] + m1_aux[11] * m2_aux[2] + m1_aux[15] * m2_aux[3];
    ret[4] = m1_aux[0] * m2_aux[4] + m1_aux[4] * m2_aux[5] + m1_aux[8] * m2_aux[6] + m1_aux[12] * m2_aux[7];
    ret[5] = m1_aux[1] * m2_aux[4] + m1_aux[5] * m2_aux[5] + m1_aux[9] * m2_aux[6] + m1_aux[13] * m2_aux[7];
    ret[6] = m1_aux[2] * m2_aux[4] + m1_aux[6] * m2_aux[5] + m1_aux[10] * m2_aux[6] + m1_aux[14] * m2_aux[7];
    ret[7] = m1_aux[3] * m2_aux[4] + m1_aux[7] * m2_aux[5] + m1_aux[11] * m2_aux[6] + m1_aux[15] * m2_aux[7];
    ret[8] = m1_aux[0] * m2_aux[8] + m1_aux[4] * m2_aux[9] + m1_aux[8] * m2_aux[10] + m1_aux[12] * m2_aux[11];
    ret[9] = m1_aux[1] * m2_aux[8] + m1_aux[5] * m2_aux[9] + m1_aux[9] * m2_aux[10] + m1_aux[13] * m2_aux[11];
    ret[10] = m1_aux[2] * m2_aux[8] + m1_aux[6] * m2_aux[9] + m1_aux[10] * m2_aux[10] + m1_aux[14] * m2_aux[11];
    ret[11] = m1_aux[3] * m2_aux[8] + m1_aux[7] * m2_aux[9] + m1_aux[11] * m2_aux[10] + m1_aux[15] * m2_aux[11];
    ret[12] = m1_aux[0] * m2_aux[12] + m1_aux[4] * m2_aux[13] + m1_aux[8] * m2_aux[14] + m1_aux[12] * m2_aux[15];
    ret[13] = m1_aux[1] * m2_aux[12] + m1_aux[5] * m2_aux[13] + m1_aux[9] * m2_aux[14] + m1_aux[13] * m2_aux[15];
    ret[14] = m1_aux[2] * m2_aux[12] + m1_aux[6] * m2_aux[13] + m1_aux[10] * m2_aux[14] + m1_aux[14] * m2_aux[15];
    ret[15] = m1_aux[3] * m2_aux[12] + m1_aux[7] * m2_aux[13] + m1_aux[11] * m2_aux[14] + m1_aux[15] * m2_aux[15];
}

/* ADDITION */

void mat2_sum_mat2(mat2_t m1, mat2_t m2, mat2_t ret)
{
    mat2_t m1_aux, m2_aux;
    memcpy(m1_aux, m1, sizeof(mat2_t));
    memcpy(m2_aux, m2, sizeof(mat2_t));
    ret[0] = m1_aux[0] + m2_aux[0];
    ret[1] = m1_aux[1] + m2_aux[1];
    ret[2] = m1_aux[2] + m2_aux[2];
    ret[3] = m1_aux[3] + m2_aux[3];
}

void mat3_sum_mat3(mat3_t m1, mat3_t m2, mat3_t ret)
{
    mat3_t m1_aux, m2_aux;
    memcpy(m1_aux, m1, sizeof(mat3_t));
    memcpy(m2_aux, m2, sizeof(mat3_t));
    ret[0] = m1_aux[0] + m2_aux[0];
    ret[1] = m1_aux[1] + m2_aux[1];
    ret[2] = m1_aux[2] + m2_aux[2];
    ret[3] = m1_aux[3] + m2_aux[3];
    ret[4] = m1_aux[4] + m2_aux[4];
    ret[5] = m1_aux[5] + m2_aux[5];
    ret[6] = m1_aux[6] + m2_aux[6];
    ret[7] = m1_aux[7] + m2_aux[7];
    ret[8] = m1_aux[8] + m2_aux[8];
}

void mat4_sum_mat4(mat4_t m1, mat4_t m2, mat4_t ret)
{
    mat4_t m1_aux, m2_aux;
    memcpy(m1_aux, m1, sizeof(mat4_t));
    memcpy(m2_aux, m2, sizeof(mat4_t));
    ret[0] = m1_aux[0] + m2_aux[0];
    ret[1] = m1_aux[1] + m2_aux[1];
    ret[2] = m1_aux[2] + m2_aux[2];
    ret[3] = m1_aux[3] + m2_aux[3];
    ret[4] = m1_aux[4] + m2_aux[4];
    ret[5] = m1_aux[5] + m2_aux[5];
    ret[6] = m1_aux[6] + m2_aux[6];
    ret[7] = m1_aux[7] + m2_aux[7];
    ret[8] = m1_aux[8] + m2_aux[8];
    ret[9] = m1_aux[9] + m2_aux[9];
    ret[10] = m1_aux[10] + m2_aux[10];
    ret[11] = m1_aux[11] + m2_aux[11];
    ret[12] = m1_aux[12] + m2_aux[12];
    ret[13] = m1_aux[13] + m2_aux[13];
    ret[14] = m1_aux[14] + m2_aux[14];
    ret[15] = m1_aux[15] + m2_aux[15];
}

/* SUBSTRACTION */

void mat2_sub_mat2(mat2_t m1, mat2_t m2, mat2_t ret)
{
    mat2_t m1_aux, m2_aux;
    memcpy(m1_aux, m1, sizeof(mat2_t));
    memcpy(m2_aux, m2, sizeof(mat2_t));
    ret[0] = m1_aux[0] - m2_aux[0];
    ret[1] = m1_aux[1] - m2_aux[1];
    ret[2] = m1_aux[2] - m2_aux[2];
    ret[3] = m1_aux[3] - m2_aux[3];
}

void mat3_sub_mat3(mat3_t m1, mat3_t m2, mat3_t ret)
{
    mat3_t m1_aux, m2_aux;
    memcpy(m1_aux, m1, sizeof(mat3_t));
    memcpy(m2_aux, m2, sizeof(mat3_t));
    ret[0] = m1_aux[0] - m2_aux[0];
    ret[1] = m1_aux[1] - m2_aux[1];
    ret[2] = m1_aux[2] - m2_aux[2];
    ret[3] = m1_aux[3] - m2_aux[3];
    ret[4] = m1_aux[4] - m2_aux[4];
    ret[5] = m1_aux[5] - m2_aux[5];
    ret[6] = m1_aux[6] - m2_aux[6];
    ret[7] = m1_aux[7] - m2_aux[7];
    ret[8] = m1_aux[8] - m2_aux[8];
}

void mat4_sub_mat4(mat4_t m1, mat4_t m2, mat4_t ret)
{
    mat4_t m1_aux, m2_aux;
    memcpy(m1_aux, m1, sizeof(mat4_t));
    memcpy(m2_aux, m2, sizeof(mat4_t));
    ret[0] = m1_aux[0] - m2_aux[0];
    ret[1] = m1_aux[1] - m2_aux[1];
    ret[2] = m1_aux[2] - m2_aux[2];
    ret[3] = m1_aux[3] - m2_aux[3];
    ret[4] = m1_aux[4] - m2_aux[4];
    ret[5] = m1_aux[5] - m2_aux[5];
    ret[6] = m1_aux[6] - m2_aux[6];
    ret[7] = m1_aux[7] - m2_aux[7];
    ret[8] = m1_aux[8] - m2_aux[8];
    ret[9] = m1_aux[9] - m2_aux[9];
    ret[10] = m1_aux[10] - m2_aux[10];
    ret[11] = m1_aux[11] - m2_aux[11];
    ret[12] = m1_aux[12] - m2_aux[12];
    ret[13] = m1_aux[13] - m2_aux[13];
    ret[14] = m1_aux[14] - m2_aux[14];
    ret[15] = m1_aux[15] - m2_aux[15];
}
