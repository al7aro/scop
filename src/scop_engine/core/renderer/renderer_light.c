#include "renderer.h"

/* LIGHTS */
light_t* light_create(const char* name_id)
{
	light_t* light;
	light = (light_t*)malloc(sizeof(light_t));
	if (!light) return NULL;
	light->empty = empty_create();
	if (!light->empty) return NULL;
	strcpy_s(light->name_id, sizeof(light->name_id), name_id);

	for (int i = 0; i < 3; i++)
		light->diffuse[i] = 0.25;
	for (int i = 0; i < 3; i++)
		light->ambient[i] = 0.25;
	for (int i = 0; i < 3; i++)
		light->specular[i] = 0.25;

	light->keyboard_input_handlers = NULL;
	light->mouse_input_handlers = NULL;
	light->update_handlers = NULL;
	return light;
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

void light_get_ambient(light_t* light, vec3_t ret)
{
	for (int i = 0; i < 3; i++)
		ret[i] = light->ambient[i];
}

void light_get_diffuse(light_t* light, vec3_t ret)
{
	for (int i = 0; i < 3; i++)
		ret[i] = light->diffuse[i];
}

void light_get_specular(light_t* light, vec3_t ret)
{
	for (int i = 0; i < 3; i++)
		ret[i] = light->specular[i];
}

void light_set_ambient(light_t* light, vec3_t ambient)
{
	for (int i = 0; i < 3; i++)
		light->ambient[i] = ambient[i];
}

void light_set_diffuse(light_t* light, vec3_t diffuse)
{
	for (int i = 0; i < 3; i++)
		light->diffuse[i] = diffuse[i];
}

void light_set_specular(light_t* light, vec3_t specular)
{
	for (int i = 0; i < 3; i++)
		light->specular[i] = specular[i];
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

void light_add_keyboard_input_handler(light_t* light, void (*keyboard_input_handler)(struct light_s*, GLFWwindow*, int, int))
{
	ft_lstadd_back(&(light->keyboard_input_handlers), ft_lstnew((void*)keyboard_input_handler));
}

void light_add_mouse_input_handler(light_t* light, void (*mouse_input_handler)(struct light_s*, GLFWwindow*, double, double))
{
	ft_lstadd_back(&(light->mouse_input_handlers), ft_lstnew((void*)mouse_input_handler));
}

void light_add_update_handler(light_t* light, void (*update_handler)(struct light_s*))
{
	ft_lstadd_back(&(light->update_handlers), ft_lstnew((void*)update_handler));
}

void light_manage_keyboard_input_callbacks(light_t* light, GLFWwindow* window, int key, int action)
{
	t_list* keyboard_input_handlers_lst = light->keyboard_input_handlers;
	while (keyboard_input_handlers_lst)
	{
		void (*keyboard_input_handler)(light_t*, GLFWwindow*, int, int);
		keyboard_input_handler = (void (*)(light_t*, GLFWwindow*, int, int))keyboard_input_handlers_lst->content;
		keyboard_input_handler(light, window, key, action);

		keyboard_input_handlers_lst = keyboard_input_handlers_lst->next;
	}
}

void light_manage_mouse_input_callbacks(light_t* light, GLFWwindow* window, double xpos, double ypos)
{
	t_list* keyboard_mouse_handlers_lst = light->mouse_input_handlers;
	while (keyboard_mouse_handlers_lst)
	{
		void (*keyboard_mouse_handler)(light_t*, GLFWwindow*, double, double);
		keyboard_mouse_handler = (void (*)(light_t*, GLFWwindow*, double, double))keyboard_mouse_handlers_lst->content;
		keyboard_mouse_handler(light, window, xpos, ypos);

		keyboard_mouse_handlers_lst = keyboard_mouse_handlers_lst->next;
	}
}

void light_update(light_t* light)
{
	t_list* update_handlers_lst = light->update_handlers;
	while (update_handlers_lst)
	{
		void (*update_handler)(light_t*);
		update_handler = (void (*)(light_t*))update_handlers_lst->content;
		update_handler(light);

		update_handlers_lst = update_handlers_lst->next;
	}
}

void light_destroy(light_t* light)
{
	if (!light) return;
	empty_destroy(light->empty);
	ft_lstclear(&(light->keyboard_input_handlers), NULL);
	ft_lstclear(&(light->mouse_input_handlers), NULL);
	ft_lstclear(&(light->update_handlers), NULL);
	free(light);
}