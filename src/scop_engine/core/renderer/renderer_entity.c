#include "renderer.h"

/* ENTITIES */
entity_t* entity_create(const char* name_id)
{
	entity_t* entity;
	entity = (entity_t*)malloc(sizeof(entity_t));
	if (!entity) exit(-1);
	entity->empty = empty_create();
	if (!entity->empty) return NULL;
	strcpy(entity->name_id, name_id);

	entity->model = NULL;
	entity->shader = NULL;

	entity->keyboard_input_handlers = NULL;
	entity->mouse_input_handlers = NULL;
	entity->update_handlers = NULL;
	return entity;
}

void entity_render(entity_t* entity, shader_t* sh)
{
	if (!entity->model)
		return;
	model_render(entity->model, sh);
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

void entity_add_keyboard_input_handler(entity_t* entity, void (*keyboard_input_handler)(struct entity_s*, GLFWwindow*, int, int))
{
	ft_lstadd_back(&(entity->keyboard_input_handlers), ft_lstnew((void*)keyboard_input_handler));
}

void entity_add_mouse_input_handler(entity_t* entity, void (*mouse_input_handler)(struct entity_s*, GLFWwindow*, double, double))
{
	ft_lstadd_back(&(entity->mouse_input_handlers), ft_lstnew((void*)mouse_input_handler));
}

void entity_add_update_handler(entity_t* entity, void (*update_handler)(struct entity_s*))
{
	ft_lstadd_back(&(entity->update_handlers), ft_lstnew((void*)update_handler));
}

void entity_manage_keyboard_input_callbacks(entity_t* entity, GLFWwindow* window, int key, int action)
{
	t_list* keyboard_input_handlers_lst = entity->keyboard_input_handlers;
	while (keyboard_input_handlers_lst)
	{
		void (*keyboard_input_handler)(entity_t*, GLFWwindow*, int, int);
		keyboard_input_handler = (void (*)(entity_t*, GLFWwindow*, int, int))keyboard_input_handlers_lst->content;
		keyboard_input_handler(entity, window, key, action);

		keyboard_input_handlers_lst = keyboard_input_handlers_lst->next;
	}
}

void entity_manage_mouse_input_callbacks(entity_t* entity, GLFWwindow* window, double xpos, double ypos)
{
	t_list* keyboard_mouse_handlers_lst = entity->mouse_input_handlers;
	while (keyboard_mouse_handlers_lst)
	{
		void (*keyboard_mouse_handler)(entity_t*, GLFWwindow*, double, double);
		keyboard_mouse_handler = (void (*)(entity_t*, GLFWwindow*, double, double))keyboard_mouse_handlers_lst->content;
		keyboard_mouse_handler(entity, window, xpos, ypos);

		keyboard_mouse_handlers_lst = keyboard_mouse_handlers_lst->next;
	}
}

void entity_update(entity_t* entity)
{
	t_list* update_handlers_lst = entity->update_handlers;
	while (update_handlers_lst)
	{
		void (*update_handler)(entity_t*);
		update_handler = (void (*)(entity_t*))update_handlers_lst->content;
		update_handler(entity);

		update_handlers_lst = update_handlers_lst->next;
	}
}

void entity_destroy(entity_t* entity)
{
	if (!entity) return;
	empty_destroy(entity->empty);
	if (entity->model)
		model_destroy(entity->model);
	ft_lstclear(&(entity->keyboard_input_handlers), NULL);
	ft_lstclear(&(entity->mouse_input_handlers), NULL);
	ft_lstclear(&(entity->update_handlers), NULL);
	free(entity);
}
