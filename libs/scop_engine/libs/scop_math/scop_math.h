#ifndef SCOP_MATH_H
#define SCOP_MATH_H

#include <stdio.h>
#include <math.h>
#include <string.h>

/*
 * TODO:    MAX_ITER    (to prevent infinite loops)
 *          EPSILON     (to avoid inverse of uninvertible matrices)
*/

typedef float vec2_t[2];
typedef float vec3_t[3];
typedef float vec4_t[4];

/* OPENGL USES COLUMN MAJOR ORDER */
/* Consecutive elements are colums */
typedef float mat2_t[4];
typedef float mat3_t[9];
typedef float mat4_t[16];

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

/* PRINT VECTORS */
void mat2_print(mat2_t const m);
void mat3_print(mat3_t const m);
void mat4_print(mat4_t const m);

/* SCALAR TIMES VECTOR */
void vec2_times_float(vec2_t v, float scalar, vec2_t ret);
void vec3_times_float(vec3_t v, float scalar, vec3_t ret);
void vec4_times_float(vec4_t v, float scalar, vec4_t ret);

/* DOT VECTORS */
float vec2_dot_vec2(vec2_t v1, vec2_t v2);
float vec3_dot_vec3(vec3_t v1, vec3_t v2);
float vec4_dot_vec4(vec4_t v1, vec4_t v2);

/* SUB VECTORS */
void vec2_minus_vec2(vec2_t v1, vec2_t v2, vec2_t ret);
void vec3_minus_vec3(vec3_t v1, vec3_t v2, vec3_t ret);
void vec4_minus_vec4(vec4_t v1, vec4_t v2, vec4_t ret);

/* ADD VECTORS */
void vec2_plus_vec2(vec2_t v1, vec2_t v2, vec2_t ret);
void vec3_plus_vec3(vec3_t v1, vec3_t v2, vec3_t ret);
void vec4_plus_vec4(vec4_t v1, vec4_t v2, vec4_t ret);

/* CROSS VECTORS */
void vec3_cross_vec3(vec3_t v1, vec3_t v2, vec3_t ret);

/* LEGNTH VECTORS */
float vec2_length(vec2_t v);
float vec3_length(vec3_t v);
float vec4_length(vec4_t v);

/* NORMALIZE VECTOR */
void vec2_normal(vec2_t v, vec2_t ret);
void vec3_normal(vec3_t v, vec3_t ret);
void vec4_normal(vec4_t v, vec4_t ret);

/* CAST */
void vec2_to_vec3(vec2_t v1, vec3_t ret);
void vec3_to_vec4(vec3_t v1, vec4_t ret);

/* MATRIX DETERMINANT */
float mat2_det(mat2_t const m);
float mat3_det(mat3_t const m);
float mat4_det(mat4_t const m);

/* MATRIX INVERSE */
void mat2_inv(mat2_t const m, mat2_t ret);
void mat3_inv(mat3_t const m, mat3_t ret);
void mat4_inv(mat4_t const m, mat4_t ret);

/* MATRIX TRANSPOSE */
void mat2_tras(mat2_t const m, mat2_t ret);
void mat3_tras(mat3_t const m, mat3_t ret);
void mat4_tras(mat4_t const m, mat4_t ret);

/* NULL MATRIX */
void mat2_get_zero(mat2_t ret);
void mat3_get_zero(mat3_t ret);
void mat4_get_zero(mat4_t ret);

/* IDENTITY MATRIX*/
void mat2_get_identity(mat2_t ret);
void mat3_get_identity(mat3_t ret);
void mat4_get_identity(mat4_t ret);

/* ROTATION MATRICES */
void mat4_get_rotX(float angle, mat4_t ret);
void mat4_get_rotY(float angle, mat4_t ret);
void mat4_get_rotZ(float angle, mat4_t ret);
void mat4_get_rotU(float angle, vec4_t dir, mat4_t ret);

/* TRASLATION MATRICES */
void mat4_get_tras(float x, float y, float z, mat4_t ret);

/* SCALE MATRICES */
void mat4_get_scale(float x, float y, float z, mat4_t ret);

/* LOOK AT */
void mat4_get_lookat(vec3_t pos, vec3_t up, vec3_t right, vec3_t lookat, mat4_t ret);

/* PROJECTION MATRICES */
void mat4_get_proj_ortho(float left, float right, float bottom, float top, float near, float far, mat4_t ret);
void mat4_get_proj_persp(float fov, float aspect, float near, float far, mat4_t ret);

/* MATRIX MULTIPLICATION */
void mat2_mult_vec2(mat2_t m1, vec2_t m2, vec2_t ret);
void mat3_mult_vec3(mat3_t m1, vec3_t m2, vec3_t ret);
void mat4_mult_vec4(mat4_t m1, vec4_t m2, vec4_t ret);

void mat2_mult_mat2(mat2_t m1, mat2_t m2, mat2_t ret);
void mat3_mult_mat3(mat3_t m1, mat3_t m2, mat3_t ret);
void mat4_mult_mat4(mat4_t m1, mat4_t m2, mat4_t ret);

/* MATRIX ADDITION */
void mat2_sum_mat2(mat2_t m1, mat2_t m2, mat2_t ret);
void mat3_sum_mat3(mat3_t m1, mat3_t m2, mat3_t ret);
void mat4_sum_mat4(mat4_t m1, mat4_t m2, mat4_t ret);

/* MATRIX SUBSTRACTION */
void mat2_sub_mat2(mat2_t m1, mat2_t m2, mat2_t ret);
void mat3_sub_mat3(mat3_t m1, mat3_t m2, mat3_t ret);
void mat4_sub_mat4(mat4_t m1, mat4_t m2, mat4_t ret);

#endif
