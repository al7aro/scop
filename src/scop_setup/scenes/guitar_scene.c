#include <stdio.h>

#include "scop_engine.h"
#include "scop_main.h"

void guitar_scene_update_rotating_cube(entity_t* e)
{
	e->empty->rot[0] += 0.003f;
	e->empty->rot[1] += 0.001f;
	e->empty->rot[2] += 0.003f;
}

void guitar_scene_update_orbiting_light(entity_t* e)
{
	e->empty->rot[1] += 0.002f;
}

scene_t* guitar_scene_create(const char* scene_name)
{
	scene_t* scene;
	shader_t* sh = (shader_t*)malloc(sizeof(shader_t));
	shader_t* bulb_sh = (shader_t*)malloc(sizeof(shader_t));

	/* LIGHT */
	vec3_t light_pos = { 0.0, 0.0, 0.0 };
	vec3_t light_diffuse = { 1.0, 1.0, 1.0 };
	vec3_t light_specular = { 1.0, 1.0, 1.0 };
	vec3_t light_ambient = { 0.4f, 0.4f, 0.4f };
	/* CUBE */
	model_t* cube;
	vec3_t cube_pos = { 0.0, 0.0, 0.0 };
	/* BULB */
	model_t* bulb;
	vec3_t bulb_pos = { 0.0, 0.0, 2.0 };
	/* ORBITING OBJECT */
	vec3_t orbit_pos = { 0.0, 0.0, 0.0 };
	/* CAMERA */
	vec3_t cam_pos = { 0.0, 0.0, 7.0 };

	light_t* light;
	cam_t* cam;
	entity_t* e_cube;
	entity_t* e_bulb;
	entity_t* e_orbit;

	shader_create(sh, SCOP_INSTALL_PATH"/assets/shaders/material.vert", SCOP_INSTALL_PATH"/assets/shaders/material.frag");
	shader_create(bulb_sh, SCOP_INSTALL_PATH"/assets/shaders/material.vert", SCOP_INSTALL_PATH"/assets/shaders/light_source.frag");

	model_load(&cube, SCOP_INSTALL_PATH"/assets/models/guitar_backpack/guitar.obj");
	model_load(&bulb, SCOP_INSTALL_PATH"/assets/models/sphere/sphere.obj");

	cam = cam_create();
	cam_set_pos(cam, cam_pos);
	cam_add_keyboard_input_handler(cam, common_camera_keyboard_callback);
	cam_add_mouse_input_handler(cam, common_camera_mouse_callback);
	cam_add_update_handler(cam, common_update_camera);

	light = light_create("light");
	light_set_pos(light, light_pos);
	light_set_diffuse(light, light_diffuse);
	light_set_ambient(light, light_ambient);
	light_set_specular(light, light_specular);

	e_cube = entity_create("e_cube");
	entity_set_model(e_cube, cube);
	entity_set_scale(e_cube, (vec3_t) { 0.5f, 0.5f, 0.5f });
	entity_set_pos(e_cube, cube_pos);
	entity_add_keyboard_input_handler(e_cube, common_trigger_input_callback);
	entity_add_update_handler(e_cube, guitar_scene_update_rotating_cube);
	entity_add_update_handler(e_cube, common_trigger_update);
	e_cube->empty->rot[1] = 3.14f;

	e_bulb = entity_create("e_bulb");
	entity_set_model(e_bulb, bulb);
	entity_set_pos(e_bulb, bulb_pos);
	entity_set_scale(e_bulb, (vec3_t) { 0.2f, 0.2f, 0.2f });
	entity_set_shader(e_bulb, bulb_sh);
	light_set_parent_entity(light, e_bulb);

	e_orbit = entity_create("e_orbit");
	entity_set_pos(e_orbit, orbit_pos);
	entity_set_parent(e_bulb, e_orbit);
	entity_add_update_handler(e_orbit, guitar_scene_update_orbiting_light);

	scene = scene_create(scene_name);
	scene_set_shader(scene, sh);
	scene_set_cam(scene, cam);
	scene_add_light(scene, light);
	scene_add_entity(scene, e_cube);
	scene_add_entity(scene, e_orbit);
	scene_add_entity(scene, e_bulb);

	return scene;
}