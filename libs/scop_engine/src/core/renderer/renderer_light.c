#include "renderer.h"

/* LIGHTS */
light_t* light_create(void)
{
	light_t* light;
	light = (light_t*)malloc(sizeof(light_t));
	if (!light) return NULL;
	light->empty = empty_create();
	if (!light->empty) return NULL;

	for (int i = 0; i < 3; i++)
		light->col[i] = 1.0;
	light->intensity = 1.0;
	light->input_handler = NULL;
	return light;
}

void light_get_col(light_t* light, vec3_t ret)
{
	for (int i = 0; i < 3; i++)
		 ret[i] = light->col[i];
}

void light_get_intensity(light_t* light, float* ret)
{
	*ret = light->intensity;
}

void light_get_pos_world(light_t* light, vec3_t ret)
{
	mat4_t emodel;
	vec4_t pos;
	empty_get_mat_model(light->empty, emodel);
	mat4_mult_vec4(emodel, (vec4_t){ 0.0, 0.0, 0.0, 1.0 }, pos);
	ret[0] = pos[0];
	ret[1] = pos[1];
	ret[2] = pos[2];
}

void light_set_parent_entity(light_t* light, entity_t* parent)
{
	empty_set_parent(light->empty, parent->empty);
}

void light_set_pos(light_t* light, vec3_t pos)
{
	for (int i = 0; i < 3; i++)
		light->empty->pos[i] = pos[i];
}

void light_set_col(light_t* light, vec3_t col)
{
	for (int i = 0; i < 3; i++)
		light->col[i] = col[i];
}

void light_set_intensity(light_t* light, float intensity)
{
	light->intensity = intensity;
}

void light_set_input_handler(light_t* light, void (*input_handler)(struct light_s*, int, int))
{
	if (light->input_handler)
		light->input_handler = input_handler;
}

void light_manage_input_callbacks(light_t* light, int key, int action)
{
	if (light->input_handler)
		light->input_handler(light, key, action);
}

void light_destroy(light_t* light)
{
	if (!light) return;
	empty_destroy(light->empty);
	free(light);
}