#ifndef SHADER_READER_H
# define SHADER_READER_H

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

GLuint shader_create(const char* vert_path, const char* frag_path);

#endif