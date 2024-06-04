#ifndef SHADER_READER_H
# define SHADER_READER_H

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

#include "scop_math.h"
#include "scop_macros.h"

typedef struct shader_s
{
    GLuint id;
} shader_t;

shader_t* shader_create(const char* vert_path, const char* frag_path);
void shader_destroy(shader_t* sh);
void shader_destroy(shader_t* sh);
void shader_use(const shader_t* sh);
void shader_set_int(const shader_t* sh, const char* name, int value);
void shader_set_float(const shader_t* sh, const char* name, float value);
void shader_set_mat4(const shader_t* sh, const char* name, mat4_t value);
void shader_set_vec3(const shader_t* sh, const char* name, vec3_t value);

#endif
