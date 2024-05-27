#ifndef SHADER_READER_H
# define SHADER_READER_H

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

#include "scop_math.h"

typedef struct shader_s
{
    GLuint id;
} shader_t;

void shader_create(shader_t* toy_sh, const char* vert_path, const char* frag_path);
void shader_use(const shader_t* toy_sh);
void shader_set_int(const shader_t* toy_sh, const char* name, int value);
void shader_set_float(const shader_t* toy_sh, const char* name, float value);
void shader_set_mat4(const shader_t* toy_sh, const char* name, mat4_t value);

#endif
