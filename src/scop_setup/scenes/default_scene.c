#include <stdio.h>

#include "scop_engine.h"
#include "scop_main.h"

void update_42_rot(entity_t* e)
{
	e->empty->rot[0] += 0.002f;
	e->empty->rot[1] += 0.002f;
	e->empty->rot[2] += 0.002f;
}

/* INPUT CALLBACKS */
scene_t* default_scene_create(const char* scene_name)
{
	scene_t* scene = scene_create(scene_name);

	shader_t* sh = (shader_t*)malloc(sizeof(shader_t));
	shader_create(sh, SCOP_INSTALL_PATH"/assets/shaders/coltex_switch.vert", SCOP_INSTALL_PATH"/assets/shaders/coltex_switch.frag");

	cam_t* cam = cam_create();
	cam_set_pos(cam, (vec3_t) { 0.0, 0.0, 7.0 });
	cam_add_keyboard_input_handler(cam, common_camera_keyboard_callback);
	cam_add_mouse_input_handler(cam, common_camera_mouse_callback);
	cam_add_update_handler(cam, common_update_camera);

	entity_t* teapot = entity_create("teapot");
	model_t* teapot_model; model_load(&teapot_model, SCOP_INSTALL_PATH"/assets/models/42_resources/teapot.obj");
	entity_set_model(teapot, teapot_model);
	entity_set_pos(teapot, (vec3_t) { 5.0, 5.0, 5.0 });
	entity_set_scale(teapot, (vec3_t) { 0.25, 0.25, 0.25 });

	entity_t* teapot2 = entity_create("teapot2");
	model_t* teapot2_model; model_load(&teapot2_model, SCOP_INSTALL_PATH"/assets/models/42_resources/teapot2.obj");
	entity_set_model(teapot2, teapot2_model);
	entity_set_pos(teapot2, (vec3_t) { -5.0, -5.0, -5.0 });

	entity_t* e_42_rot = entity_create("42_rot");
	entity_add_update_handler(e_42_rot, update_42_rot);

	entity_t* logo42 = entity_create("logo42");
	model_t* logo42_model; model_load(&logo42_model, SCOP_INSTALL_PATH"/assets/models/42_resources/42.obj");
	entity_set_model(logo42, logo42_model);
	entity_set_pos(logo42, (vec3_t) {0.0, 0.0, -1.0});
	entity_add_keyboard_input_handler(logo42, common_trigger_input_callback);
	entity_add_update_handler(logo42, common_trigger_update);
	entity_set_parent(logo42, e_42_rot);

	light_t* light = light_create("light");
	light_set_pos(light, (vec3_t) {3.0f, 0.0f, 0.0f});
	light_set_ambient(light, (vec3_t) { 1.0f, 1.0f, 1.0f });
	light_set_diffuse(light, (vec3_t) { 1.0f, 1.0f, 1.0f });
	light_set_specular(light, (vec3_t) {1.0f, 1.0f, 1.0f});

	scene_add_entity(scene, teapot);
	scene_add_entity(scene, teapot2);
	scene_add_entity(scene, logo42);
	scene_add_entity(scene, e_42_rot);
	scene_set_cam(scene, cam);
	scene_set_shader(scene, sh);
	scene_add_light(scene, light);

	return scene;
}