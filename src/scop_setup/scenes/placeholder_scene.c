#include <stdio.h>

#include "scop_engine.h"
#include "scop_main.h"

/* INPUT CALLBACKS */
scene_t* placeholder_scene_create(const char* scene_name, const char* obj, const char* vShader, const char* fShader)
{
	scene_t* scene = scene_create(scene_name);

	shader_t* sh;
    if (vShader && fShader)
	    sh = shader_create(vShader, fShader);
	else
		sh = shader_create(SCOP_INSTALL_PATH"/assets/shaders/material.vert", SCOP_INSTALL_PATH"/assets/shaders/material.frag");

	cam_t* cam = cam_create();
	cam_set_pos(cam, (vec3_t) { 0.0, 0.0, 20.0 });
	cam_add_keyboard_input_handler(cam, common_camera_keyboard_callback);
	cam_add_mouse_input_handler(cam, common_camera_mouse_callback);
	cam_add_update_handler(cam, common_update_camera);

	entity_t* entity = entity_create("placeholder");
	model_t* entity_model;
	if (obj)
		model_load(&entity_model, obj);
	else
		model_load(&entity_model, SCOP_INSTALL_PATH"/assets/models/placeholder/untitled.obj");
	entity_set_model(entity, entity_model);
	entity_set_pos(entity, (vec3_t) { 0.0, 0.0, 0.0 });
	entity_set_scale(entity, (vec3_t) { 1.0, 1.0, 1.0 });
	entity_add_keyboard_input_handler(entity, common_trigger_input_callback);
	entity_add_update_handler(entity, common_trigger_update);

	light_t* light = light_create("light");
	light_set_pos(light, (vec3_t) {3.0f, 0.0f, 0.0f});
	light_set_ambient(light, (vec3_t) { 1.0f, 1.0f, 1.0f });
	light_set_diffuse(light, (vec3_t) { 1.0f, 1.0f, 1.0f });
	light_set_specular(light, (vec3_t) {1.0f, 1.0f, 1.0f});

	scene_add_entity(scene, entity, sh);
	scene_set_cam(scene, cam);
	scene_add_light(scene, light);

	return scene;
}
