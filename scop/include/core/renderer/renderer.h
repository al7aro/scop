#ifndef RENDERER_H
#define RENDERER_H

#include "basic_model.h"
#include "scop_math.h"
#include "shader.h"

/* TODO: HOW SHOULD I INCLUDE THE INPUT HANDLERS? */

typedef struct empty_s
{
	struct empty_s* parent;
	vec3_t pos;
	vec3_t rot;
	vec3_t scale;

	mat4_t model;
} empty_t;

typedef struct entity_s
{
	empty_t* empty;
	shader_t* shader;
	model_t* model;
	/* Maybe the shader_id(?) | use a default one if it doesn't have any*/
	void (*input_handler)(struct entity_s*, int, int);
} entity_t;

typedef struct cam_s
{
	empty_t* empty;
	vec3_t up;
	vec3_t lookat;
	vec3_t right;

	float fov;
	/* Common Camera Settings */
	void (*input_handler)(struct cam_s*, int, int);
} cam_t;

typedef struct light_s
{
	empty_t* empty;
	vec3_t col;
	float intensity;
	/* spot, point, sun, etc. */

	void (*input_handler)(struct light_s*, int, int);
} light_t;

typedef struct scene_s
{
	shader_t* default_shader;
	vec3_t ambient;
	cam_t* cam;
	t_list* entity_lst;
	t_list* light_lst;
	void (*input_handler)(struct scene_s*, int, int);
} scene_t;

/* SCENE */
scene_t* scene_create(void);
void scene_render(scene_t* scene);
/* update_scene(?) */

void scene_light_uniform(shader_t* sh, scene_t* scene);
void scene_cam_uniform(shader_t* sh, scene_t* scene);

void scene_set_shader(scene_t* scene, shader_t* shader);
void scene_set_ambient(scene_t* scene, vec3_t col);
void scene_set_cam(scene_t* scene, cam_t* cam);
void scene_set_input_handler(scene_t* scene, void (*input_handler)(struct scene_s*, int, int));

void scene_add_entity(scene_t* scene, entity_t* entity);
void scene_add_light(scene_t* scene, light_t* light);

void scene_manage_input_callbacks(scene_t* scene, int key, int action);
void scene_destroy(scene_t* scene);

/* CAM */
cam_t* cam_create(void);		/* Creates camera with default settings */

void cam_get_mat_view(cam_t* cam, mat4_t ret);
void cam_get_mat_proj(cam_t* cam, mat4_t ret);

void cam_set_pos(cam_t* cam, vec3_t pos);
void cam_set_fov(cam_t* cam, float fov);
void cam_set_lookat(cam_t* cam, vec3_t lookat);
void cam_set_up(cam_t* cam, vec3_t up);
void cam_set_right(cam_t* cam, vec3_t right);
void cam_set_input_handler(cam_t* cam, void (*input_handler)(struct cam_s*, int, int));

void cam_manage_input_callbacks(cam_t* cam, int key, int action);
void cam_destroy(cam_t* cam);

/* ENTITIES */
entity_t* entity_create(void);
void entity_render(entity_t* entity, unsigned int sh_id);

void entity_set_shader(entity_t* entity, shader_t* shader);
void entity_set_model(entity_t* entity, model_t* model);
void entity_set_parent(entity_t* entity, entity_t* parent);
void entity_set_pos(entity_t* entity, vec3_t pos);
void entity_set_scale(entity_t* entity, vec3_t scale);
void entity_set_input_handler(entity_t* entity, void (*input_handler)(struct entity_s*, int, int));

void entity_manage_input_callbacks(entity_t* entity, int key, int action);
void entity_destroy(entity_t* entity);

/* LIGHTS */
light_t* light_create(void);

void light_get_pos_world(light_t* light, vec3_t ret);
void light_get_col(light_t* light, vec3_t ret);
void light_get_intensity(light_t* light, float* ret);

void light_set_parent_entity(light_t* light, entity_t* parent);
void light_set_pos(light_t* light, vec3_t pos);
void light_set_col(light_t* light, vec3_t col);
void light_set_intensity(light_t* light, float intensity);
void light_set_input_handler(light_t* light, void (*input_handler)(struct light_s*, int, int));

void light_manage_input_callbacks(light_t* light, int key, int action);
void light_destroy(light_t* light);

/* EMPTY */
empty_t* empty_create(void);

void scene_empty_uniform(shader_t* sh, empty_t* empty);

void empty_get_mat_model(empty_t* empty, mat4_t ret);

void empty_set_parent(empty_t* empty, empty_t* parent);

void empty_destroy(empty_t* empty);

#endif