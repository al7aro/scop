#include "renderer.h"

/* SCENE */
scene_t* scene_create(void)
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
	scene->input_handler = NULL;

	return scene;
}

void scene_render(scene_t* scene)
{
	t_list* ent_lst = scene->entity_lst;

	while (ent_lst)
	{
		entity_t* entity = ent_lst->content;
		shader_t* active_shader = 0;
		if (entity->shader)
			active_shader = entity->shader;
		else
			active_shader = scene->default_shader;

		/* UPLOAD UNIFORMS TO GPU AND RENDER */
		shader_use(active_shader);

		scene_empty_uniform(active_shader, entity->empty);
		scene_cam_uniform(active_shader, scene);
		scene_light_uniform(active_shader, scene);

		/* Mesh uploads its material uniforms itself */
		entity_render(entity, active_shader->id);
		ent_lst = ent_lst->next;
	}
}

void scene_set_shader(scene_t* scene, shader_t* shader)
{
	scene->default_shader = shader;
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

void scene_set_input_handler(scene_t* scene, void (*input_handler)(struct scene_s*, int, int))
{
	scene->input_handler = input_handler;
}

void scene_add_entity(scene_t* scene, entity_t* entity)
{
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

		vec3_t col;
		light_get_col(light->content, col);
		sprintf(loc_str, "lights[%d].col", cnt);
		shader_set_vec3(sh, loc_str, col);

		float intensity;
		light_get_intensity(light->content, &intensity);
		sprintf(loc_str, "lights[%d].intensity", cnt);
		shader_set_float(sh, loc_str, intensity);

		light = light->next;
	}
}

void scene_cam_uniform(shader_t* sh, scene_t* scene)
{
	mat4_t proj, view;
	cam_get_mat_view(scene->cam, view);
	shader_set_mat4(sh, "view", view);
	cam_get_mat_proj(scene->cam, proj);
	shader_set_mat4(sh, "proj", proj);
}

void scene_manage_input_callbacks(scene_t* scene, int key, int action)
{
	cam_manage_input_callbacks(scene->cam, key, action);
	t_list* entity_lst = scene->entity_lst;
	while (entity_lst)
	{
		entity_manage_input_callbacks(entity_lst->content, key, action);
		entity_lst = entity_lst->next;
	}
	t_list* light_lst = scene->light_lst;
	while (light_lst)
	{
		light_manage_input_callbacks(light_lst->content, key, action);
		light_lst = light_lst->next;
	}
	if (scene->input_handler)
		scene->input_handler(scene, key, action);
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
