#include "renderer.h"

/* SCENE */
scene_t* scene_create(const char* name_id)
{
	scene_t* scene;
	scene = (scene_t*)malloc(sizeof(scene_t));
	if (!scene) return NULL;
	for (int i = 0; i < 3; i++)
		scene->ambient[i] = 1.0;
	scene->default_shader = NULL;
	scene->cam = NULL;
	scene->entity_lst = NULL;
	scene->light_lst = NULL;

	scene->keyboard_input_handlers = NULL;
	scene->mouse_input_handlers = NULL;
	scene->update_handlers = NULL;
	strcpy_s(scene->name_id, sizeof(scene->name_id), name_id);

	return scene;
}

void scene_render(scene_t* scene)
{
	t_list* ent_lst = scene->entity_lst;

	while (ent_lst)
	{
		entity_t* entity = ent_lst->content;
		if (!entity->model)
		{
			ent_lst = ent_lst->next;
			continue;
		}
		/* UPLOAD UNIFORMS TO GPU AND RENDER */
		shader_use(entity->shader);

		scene_empty_uniform(entity->shader, entity->empty);
		scene_cam_uniform(entity->shader, scene);
		scene_light_uniform(entity->shader, scene);

		/* Mesh uploads its material uniforms itself */
		entity_render(entity, entity->shader->id);
		ent_lst = ent_lst->next;
	}
}

entity_t* scene_get_entity_by_name(scene_t* scene, const char* name_id)
{
	t_list* entity_lst = scene->entity_lst;
	while (entity_lst)
	{
		entity_t* entity = entity_lst->content;
		if (!strcmp(entity->name_id, name_id))
			return entity;
		entity_lst = entity_lst->next;
	}
	return NULL;
}

light_t* scene_get_light_by_name(scene_t* scene, const char* name_id)
{
	t_list* light_lst = scene->light_lst;
	while (light_lst)
	{
		light_t* light = light_lst->content;
		if (!strcmp(light->name_id, name_id))
			return light;
		light_lst = light_lst->next;
	}
	return NULL;
}

void scene_set_shader(scene_t* scene, shader_t* shader)
{
	scene->default_shader = shader;
	t_list* entity_lst = scene->entity_lst;
	while (entity_lst)
	{
		entity_t* entity = entity_lst->content;
		if (!entity->shader)
			entity->shader = shader;
		entity_lst = entity_lst->next;
	}
}

void scene_set_ambient(scene_t* scene, vec3_t col)
{
	for (int i = 0; i < 3; i++)
		scene->ambient[i] = col[i];
}

void scene_set_cam(scene_t* scene, cam_t* cam)
{
	scene->cam = cam;
}

void scene_add_keyboard_input_handler(scene_t* scene, void (*keyboard_input_handler)(struct scene_s*, GLFWwindow*, int, int))
{
	ft_lstadd_back(&(scene->keyboard_input_handlers), ft_lstnew((void*)keyboard_input_handler));
}

void scene_add_mouse_input_handler(scene_t* scene, void (*mouse_input_handler)(struct scene_s*, GLFWwindow*, double, double))
{
	ft_lstadd_back(&(scene->mouse_input_handlers), ft_lstnew((void*)mouse_input_handler));
}

void scene_add_update_handler(scene_t* scene, void (*update_handler)(struct scene_s*))
{
	ft_lstadd_back(&(scene->update_handlers), ft_lstnew((void*)update_handler));
}

void scene_add_entity(scene_t* scene, entity_t* entity)
{
	if (!entity->shader)
		entity->shader = scene->default_shader;
	ft_lstadd_back(&(scene->entity_lst), ft_lstnew(entity));
}

void scene_add_light(scene_t* scene, light_t* light)
{
	ft_lstadd_back(&(scene->light_lst), ft_lstnew(light));
}

void scene_empty_uniform(shader_t* sh, empty_t* empty)
{
	mat4_t model;
	empty_get_mat_model(empty, model);
	shader_set_mat4(sh, "model", model);
}

void scene_light_uniform(shader_t* sh, scene_t* scene)
{
	shader_set_int(sh, "point_light_cnt", ft_lstsize(scene->light_lst));
	t_list* light = scene->light_lst;
	int cnt = 0;

	while (light && cnt < 64)	/* 64 -> MAX_LIGHTS*/
	{
		char loc_str[64]; memset(loc_str, 0, sizeof(loc_str));

		vec3_t pos;
		light_get_pos_world(light->content, pos);
		sprintf(loc_str, "lights[%d].pos", cnt);
		shader_set_vec3(sh, loc_str, pos);

		vec3_t diffuse, specular, ambient;
		light_get_diffuse(light->content, diffuse);
		sprintf(loc_str, "lights[%d].diffuse", cnt);
		shader_set_vec3(sh, loc_str, diffuse);

		light_get_specular(light->content, specular);
		sprintf(loc_str, "lights[%d].specular", cnt);
		shader_set_vec3(sh, loc_str, specular);

		light_get_ambient(light->content, ambient);
		sprintf(loc_str, "lights[%d].ambient", cnt);
		shader_set_vec3(sh, loc_str, ambient);

		cnt++;
		light = light->next;
	}
}

void scene_cam_uniform(shader_t* sh, scene_t* scene)
{
	mat4_t proj, view;
	vec3_t view_pos;
	cam_get_mat_view(scene->cam, view);
	shader_set_mat4(sh, "view", view);
	cam_get_mat_proj(scene->cam, proj);
	shader_set_mat4(sh, "proj", proj);
	cam_get_pos(scene->cam, view_pos);
	shader_set_vec3(sh, "view_pos", view_pos);
}

void scene_manage_keyboard_input_callbacks(scene_t* scene, GLFWwindow* window, int key, int action)
{
	cam_manage_keyboard_input_callbacks(scene->cam, window, key, action);

	t_list* entity_lst = scene->entity_lst;
	while (entity_lst)
	{
		entity_manage_keyboard_input_callbacks(entity_lst->content, window, key, action);
		entity_lst = entity_lst->next;
	}
	t_list* light_lst = scene->light_lst;
	while (light_lst)
	{
		light_manage_keyboard_input_callbacks(light_lst->content, window, key, action);
		light_lst = light_lst->next;
	}

	/* Manage Scene Callbacks */
	t_list* keyboard_input_handlers_lst = scene->keyboard_input_handlers;
	while (keyboard_input_handlers_lst)
	{
		void (*keyboard_input_handler)(scene_t*, GLFWwindow*, int, int);
		keyboard_input_handler = (void (*)(scene_t*, GLFWwindow*, int, int))keyboard_input_handlers_lst->content;
		keyboard_input_handler(scene, window, key, action);

		keyboard_input_handlers_lst = keyboard_input_handlers_lst->next;
	}
}

void scene_manage_mouse_input_callbacks(scene_t* scene, GLFWwindow* window, double xpos, double ypos)
{
	cam_manage_mouse_input_callbacks(scene->cam, window, xpos, ypos);

	t_list* entity_lst = scene->entity_lst;
	while (entity_lst)
	{
		entity_manage_mouse_input_callbacks(entity_lst->content, window, xpos, ypos);
		entity_lst = entity_lst->next;
	}
	t_list* light_lst = scene->light_lst;
	while (light_lst)
	{
		light_manage_mouse_input_callbacks(light_lst->content, window, xpos, ypos);
		light_lst = light_lst->next;
	}

	/* MANAGE SCENE CALLBACKS */
	t_list* keyboard_mouse_handlers_lst = scene->mouse_input_handlers;
	while (keyboard_mouse_handlers_lst)
	{
		void (*keyboard_mouse_handler)(scene_t*, GLFWwindow*, double, double);
		keyboard_mouse_handler = (void (*)(scene_t*, GLFWwindow*, double, double))keyboard_mouse_handlers_lst->content;
		keyboard_mouse_handler(scene, window, xpos, ypos);

		keyboard_mouse_handlers_lst = keyboard_mouse_handlers_lst->next;
	}
}

void scene_update(scene_t* scene)
{
	cam_update(scene->cam);
	t_list* entity_lst = scene->entity_lst;
	while (entity_lst)
	{
		entity_update(entity_lst->content);
		entity_lst = entity_lst->next;
	}
	t_list* light_lst = scene->light_lst;
	while (light_lst)
	{
		light_update(light_lst->content);
		light_lst = light_lst->next;
	}

	/* SCENE CALLBACKS*/
	t_list* update_handlers_lst = scene->update_handlers;
	while (update_handlers_lst)
	{
		void (*update_handler)(scene_t*);
		update_handler = (void (*)(scene_t*))update_handlers_lst->content;
		update_handler(scene);

		update_handlers_lst = update_handlers_lst->next;
	}
}

void scene_destroy(scene_t* scene)
{
	if (!scene) return;
	ft_lstclear(&(scene->light_lst), light_destroy);
	ft_lstclear(&(scene->entity_lst), entity_destroy);
	cam_destroy(scene->cam);
	if (scene->default_shader)
		shader_destroy(scene->default_shader);
	free(scene);
}
