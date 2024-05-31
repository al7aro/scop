#include "scop_main.h"

scene_t* create_default_scene()
{
	scene_t* scene;
	shader_t* sh = (shader_t*)malloc(sizeof(shader_t));
	shader_t* bulb_sh = (shader_t*)malloc(sizeof(shader_t));

	/* LIGHT */
	vec3_t light_pos = { 0.0, 0.0, 0.0 };
	vec3_t light_diffuse = { 1.0, 1.0, 1.0 };
	vec3_t light_specular = { 1.0, 1.0, 1.0 };
	vec3_t light_ambient= { 0.4f, 0.4f, 0.4f };
	/* CUBE */
	model_t* cube;
	vec3_t cube_pos = { 0.0, 0.0, 0.0 };
	/* BULB */
	model_t* bulb;
	vec3_t bulb_pos = { 0.0, 0.0, 2.0 };
	/* ORBITING OBJECT */
	vec3_t orbit_pos = { 0.0, 0.0, 0.0 };
	/* CAMERA */
	vec3_t cam_pos = {0.0, 0.0, 7.0};

	light_t* light;
	cam_t* cam;
	entity_t* e_cube;
	entity_t* e_bulb;
	entity_t* e_orbit;

	shader_create(sh, "C:/Users/al7ar/DEV/scop/assets/shaders/main460.vert", "C:/Users/al7ar/DEV/scop/assets/shaders/main460.frag");
	shader_create(bulb_sh, "C:/Users/al7ar/DEV/scop/assets/shaders/main460.vert", "C:/Users/al7ar/DEV/scop/assets/shaders/main460_light.frag");

	model_load(&cube, "C:/Users/al7ar/DEV/scop/assets/models/guitar_backpack/guitar.obj");
	model_load(&bulb, "C:/Users/al7ar/DEV/scop/assets/models/sphere/sphere.obj");

	cam = cam_create();
	cam_set_pos(cam, cam_pos);
	cam_set_input_handler(cam, camera_callback);
	cam_set_update_handler(cam, update_camera);

	light = light_create("light");
	light_set_pos(light, light_pos);
	light_set_diffuse(light, light_diffuse);
	light_set_ambient(light, light_ambient);
	light_set_specular(light, light_specular);

	e_cube = entity_create("e_cube");
	entity_set_model(e_cube, cube);
	entity_set_scale(e_cube, (vec3_t) { 0.5f, 0.5f, 0.5f });
	entity_set_pos(e_cube, cube_pos);
	entity_set_update_handler(e_cube, update_rotating_cube);
	e_cube->empty->rot[1] = 3.14f;

	e_bulb = entity_create("e_bulb");
	entity_set_model(e_bulb, bulb);
	entity_set_pos(e_bulb, bulb_pos);
	entity_set_scale(e_bulb, (vec3_t) {0.2f, 0.2f, 0.2f});
	entity_set_shader(e_bulb, bulb_sh);
	light_set_parent_entity(light, e_bulb);

	e_orbit = entity_create("e_orbit");
	entity_set_pos(e_orbit, orbit_pos);
	entity_set_parent(e_bulb, e_orbit);
	entity_set_update_handler(e_bulb, update_orbiting_light);

	scene = scene_create();
	scene_set_shader(scene, sh);
	scene_set_cam(scene, cam);
	scene_add_light(scene, light);
	scene_add_entity(scene, e_cube);
	scene_add_entity(scene, e_orbit);
	scene_add_entity(scene, e_bulb);

	return scene;
}

int main(void)
{
	scop_engine_t* engine = scop_engine_create();
	scene_t* scene = create_default_scene();

	printf("SCENE OG: [%p]\n", scene);

	scop_engine_set_active_scene(engine, scene);
	scop_engine_set_clear_color(engine, (vec3_t){0.3f, 0.3f, 0.3f});
	scop_engine_set_input_callback(engine, engine_input_callback);
	while (!glfwWindowShouldClose(engine->window))
	{
		scop_engine_update(engine);
		scop_engine_render(engine);
	}
	scene_destroy(scene);
#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif
	return (0);
}
