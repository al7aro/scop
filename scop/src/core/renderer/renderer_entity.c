#include "renderer.h"

/* ENTITIES */
entity_t* entity_create(void)
{
	entity_t* entity;
	entity = (entity_t*)malloc(sizeof(entity_t));
	if (!entity) return NULL;
	entity->empty = empty_create();
	if (!entity->empty) return NULL;

	entity->model = NULL;
	entity->shader = NULL;
	entity->input_handler = NULL;
	return entity;
}

void entity_render(entity_t* entity, unsigned int sh_id)
{
	if (!entity->model)
		return;
	model_render(entity->model, sh_id);
}

void entity_set_shader(entity_t* entity, shader_t* shader)
{
	entity->shader = shader;
}

void entity_set_model(entity_t* entity, model_t* model)
{
	entity->model = model;
}

void entity_set_parent(entity_t* entity, entity_t* parent)
{
	empty_set_parent(entity->empty, parent->empty);
}

void entity_set_pos(entity_t* entity, vec3_t pos)
{
	for (int i = 0; i < 3; i++)
		entity->empty->pos[i] = pos[i];
}

void entity_set_scale(entity_t* entity, vec3_t scale)
{
	for (int i = 0; i < 3; i++)
		entity->empty->scale[i] = scale[i];
}

void entity_set_input_handler(entity_t* entity, void (*input_handler)(struct entity_s*, int, int))
{
	entity->input_handler = input_handler;
}

void entity_manage_input_callbacks(entity_t* entity, int key, int action)
{
	if (entity->input_handler)
		entity->input_handler(entity, key, action);
}

void entity_destroy(entity_t* entity)
{
	(void)entity;
}