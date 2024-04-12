#ifndef SCOP_MATH_H
#define SCOP_MATH_H

#include <stdio.h>
#include <math.h>

/*
 * TODO:    MAX_ITER    (to prevent infinite loops)
 *          EPSILON     (to avoid inverse of uninvertible matrices)
*/

typedef float vec2_t[2];
typedef float vec3_t[3];
typedef float vec4_t[4];
typedef float* vecN_t;

/* OPENGL USES COLUMN MAJOR ORDER */
/* Consecutive elements are colums */
typedef float mat2_t[4];
typedef float mat3_t[9];
typedef float mat4_t[16];
typedef float* matN_t;
typedef float* matNxM_t;

/*******************************************/
/*           [ m0 m4 m8 m12 ]              */
/*           [ m1 m5 m9 m13 ]              */
/*           [ m2 m6 m10 m14 ]             */
/*           [ m3 m7 m11 m15 ]             */
/*******************************************/

/* PRINT MATRICES */
void vec2_print(vec2_t const v);
void vec3_print(vec3_t const v);
void vec4_print(vec4_t const v);
void vecN_print(vecN_t const v, unsigned int n);

/* PRINT VECTORS */
void mat2_print(mat2_t const m);
void mat3_print(mat3_t const m);
void mat4_print(mat4_t const m);
void matN_print(matN_t const m, unsigned int n);

/* MATRIX DETERMINANT */
float mat2_det(mat2_t const m);
float mat3_det(mat3_t const m);
float mat4_det(mat4_t const m);
float matN_det(matN_t const m, unsigned int n);

/* MATRIX INVERSE */
void mat2_inv(mat2_t const m, mat2_t ret);
void mat3_inv(mat3_t const m, mat3_t ret);
void mat4_inv(mat4_t const m, mat4_t ret);
void matN_inv(matN_t const m, unsigned int n, matN_t ret);

/* MATRIX TRANSPOSE */
void mat2_tras(mat2_t const m, mat2_t ret);
void mat3_tras(mat3_t const m, mat3_t ret);
void mat4_tras(mat4_t const m, mat4_t ret);
void matN_tras(matN_t const m, unsigned int n, matN_t ret);

/* NULL MATRIX */
void mat2_get_zero(mat2_t ret);
void mat3_get_zero(mat3_t ret);
void mat4_get_zero(mat4_t ret);
void matN_get_zero(unsigned int n, matN_t ret);

/* IDENTITY MATRIX*/
void mat2_get_identity(mat2_t ret);
void mat3_get_identity(mat3_t ret);
void mat4_get_identity(mat4_t ret);
void matN_get_identity(unsigned int n, matN_t ret);

/* ROTATION MATRICES */
void mat4_get_rotX(float angle, mat4_t ret);
void mat4_get_rotY(float angle, mat4_t ret);
void mat4_get_rotZ(float angle, mat4_t ret);

/* PROJECTION MATRICES */
void mat4_get_proj_ortho(float L, float R, float B, float T, float N, float F, mat4_t ret);

/* MATRIX MULTIPLICATION */
void mat2_mult_vec2(mat2_t m1, vec2_t m2, vec2_t ret);
void mat3_mult_vec3(mat3_t m1, vec3_t m2, vec3_t ret);
void mat4_mult_vec4(mat4_t m1, vec4_t m2, vec4_t ret);
void matN_mult_vecN(matN_t m1, vecN_t v, unsigned int n, vecN_t ret);

void mat2_mult_mat2(mat2_t m1, mat2_t m2, mat2_t ret);
void mat3_mult_mat3(mat3_t m1, mat3_t m2, mat3_t ret);
void mat4_mult_mat4(mat4_t m1, mat4_t m2, mat4_t ret);
void matN_mult_matN(matN_t m1, matN_t m2, unsigned int n, matN_t ret);

/* MATRIX ADDITION */
void mat2_sum_mat2(mat2_t m1, mat2_t m2, mat2_t ret);
void mat3_sum_mat3(mat3_t m1, mat3_t m2, mat3_t ret);
void mat4_sum_mat4(mat4_t m1, mat4_t m2, mat4_t ret);
void matN_sum_matN(matN_t m1, matN_t m2, unsigned int n, matN_t ret);

/* MATRIX SUBSTRACTION */
void mat2_sub_mat2(mat2_t m1, mat2_t m2, mat2_t ret);
void mat3_sub_mat3(mat3_t m1, mat3_t m2, mat3_t ret);
void mat4_sub_mat4(mat4_t m1, mat4_t m2, mat4_t ret);
void matN_sub_matN(matN_t m1, matN_t m2, unsigned int n, matN_t ret);

#endif