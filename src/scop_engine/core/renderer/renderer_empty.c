#include "renderer.h"

/* EMPTIES */
empty_t* empty_create(void)
{
	empty_t* empty;
	empty = (empty_t*)malloc(sizeof(empty_t));
	if (!empty) return NULL;

	empty->parent = NULL;
	for (int i = 0; i < 3; i++)
		empty->pos[i] = 0.0;
	for (int i = 0; i < 3; i++)
		empty->rot[i] = 0.0;
	for (int i = 0; i < 3; i++)
		empty->scale[i] = 1.0;
	memset(&(empty->input_motion), 0, sizeof(empty->input_motion));
	mat4_get_identity(empty->model);
	return empty;
}

void empty_get_mat_model(empty_t* empty, mat4_t ret)
{
	mat4_t parent_model, model, tras, scale, rotX, rotY, rotZ;
	mat4_get_rotX(empty->rot[0], rotX);
	mat4_get_rotY(empty->rot[1], rotY);
	mat4_get_rotZ(empty->rot[2], rotZ);
	mat4_get_tras(empty->pos[0], empty->pos[1], empty->pos[2], tras);
	mat4_get_scale(empty->scale[0], empty->scale[1], empty->scale[2], scale);

	mat4_mult_mat4(rotZ, scale, model);
	mat4_mult_mat4(rotY, model, model);
	mat4_mult_mat4(rotX, model, model);
	mat4_mult_mat4(tras, model, ret);
	if (empty->parent)
	{
		empty_get_mat_model(empty->parent, parent_model);
		mat4_mult_mat4(parent_model, ret, ret);
	}
}

void empty_set_parent(empty_t* empty, empty_t* parent)
{
	empty->parent = parent;
}

void empty_destroy(empty_t* empty)
{
	if (!empty) return;
	free(empty);
}
