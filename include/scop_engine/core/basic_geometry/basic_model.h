#ifndef BASIC_MODEL_H
#define BASIC_MODEL_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "glad/glad.h"
#include "basic_mesh.h"
#include "scop_obj_loader.h"
#include "shader.h"

typedef struct model_s
{
	t_list* mesh;
} model_t;

void model_load(model_t** ret, const char* file);
void model_load_GPU(model_t* model);
void model_render(model_t* model, shader_t* sh);
void model_destroy(model_t* model);

#endif
