#ifdef _WIN32
	#include <crtdbg.h>
#endif

#include <stdio.h>

#include "scop_engine.h"

float angle = 0;
unsigned int texture0, texture1;

/* LIGHT */
vec3_t light_pos = { 2.0, 2.0, 0.0 };
vec3_t light_col = { 1.0, 1.0, 1.0 };

shader_t toy_sh;
/* CUBE */
model_t* cube;
vec3_t cube_pos = { 0.0, 0.0, 0.0 };
/* HEXAGON */
model_t* hexagon;
vec3_t hexagon_pos = { 2.0, 0.0, 0.0 };
/* SPHERE */
model_t* sphere;
vec3_t sphere_pos = { 0.0, 2.0, 0.0 };
/* SPHERE */
model_t* teapot2;
vec3_t teapot2_pos = { 0.0, 5.0, 0.0 };

/* CAMERA */
vec3_t cam_pos = {0.0, 0.0, 10.0};

scene_t* scene;
cam_t* cam;
light_t* light;
entity_t* e1; entity_t* e2; entity_t* e3; entity_t* e4;

void test_callback(cam_t* test, int key, int action)
{
	(void)test; (void)key; (void)action;
	if (key == GLFW_KEY_W)
		test->empty->pos[2]--;
	if (key == GLFW_KEY_S)
		test->empty->pos[2]++;
	if (key == GLFW_KEY_A)
		test->empty->pos[0]--;
	if (key == GLFW_KEY_D)
		test->empty->pos[0]++;
	if (key == GLFW_KEY_E)
		test->empty->pos[1]--;
	if (key == GLFW_KEY_Q)
		test->empty->pos[1]++;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void)window; (void)scancode; (void)action; (void)mods;
	scene_manage_input_callbacks(scene, key, action);
}

void init()
{
	model_load(&cube, "C:/Users/al7ar/Desktop/woman_cube.obj");
	model_load(&hexagon, "C:/Users/al7ar/DEV/scop/assets/models/hexagon.obj");
	model_load(&sphere, "C:/Users/al7ar/DEV/scop/assets/models/multi_sphere.obj");
	model_load(&teapot2 , "C:/Users/al7ar/DEV/scop/assets/models/teapot2.obj");

	cam = cam_create();
	cam_set_pos(cam, cam_pos);
	cam_set_input_handler(cam, test_callback);

	light = light_create();
	light_set_pos(light, light_pos);
	light_set_col(light, light_col);

	e1 = entity_create();
	entity_set_model(e1, cube);
	entity_set_pos(e1, cube_pos);
	entity_set_shader(e1, &toy_sh);

	e2 = entity_create();
	entity_set_model(e2, hexagon);
	entity_set_pos(e2, hexagon_pos);
	entity_set_shader(e2, &toy_sh);
	entity_set_parent(e2, e1);

	e3 = entity_create();
	entity_set_model(e3, sphere);
	entity_set_pos(e3, sphere_pos);
	entity_set_shader(e3, &toy_sh);
	entity_set_scale(e3, (vec3_t) { 0.1f, 0.1f, 0.1f });
	entity_set_parent(e3, e2);

	e4 = entity_create();
	entity_set_model(e4, teapot2);
	entity_set_pos(e4, teapot2_pos);
	entity_set_shader(e4, &toy_sh);
	entity_set_parent(e4, e3);

	scene = scene_create();
	scene_set_shader(scene, &toy_sh);
	scene_set_cam(scene, cam);
	scene_add_light(scene, light);
	scene_add_entity(scene, e1);
	scene_add_entity(scene, e2);
	scene_add_entity(scene, e3);
	scene_add_entity(scene, e4);

	/* TODO: Study how to make the path more portable */
#ifdef _WIN32
	shader_create(&toy_sh, "C:/Users/al7ar/DEV/scop/assets/shaders/main460.vert", "C:/Users/al7ar/DEV/scop/assets/shaders/main460.frag");
#endif
#if defined(__APPLE__) || defined(__linux__)
	shader_create(&toy_sh, "scop/assets/shaders/main410.vert", "scop/assets/shaders/main410.frag");
#endif
}

void update()
{
	e1->empty->rot[0] += 0.003f;
	e1->empty->rot[1] += 0.001f;
	e1->empty->rot[2] += 0.003f;

	e2->empty->rot[0] += 0.004f;

	e3->empty->rot[0] -= 0.005f;
	e3->empty->rot[1] += 0.01f;
	e3->empty->rot[2] -= 0.002f;
}

int main(void)
{
	scop_engine_t* engine = scop_engine_create();

	init();
	scop_engine_set_active_scene(engine, scene);
	scop_engine_set_input_callback(engine, key_callback);
	scop_engine_set_clear_color(engine, (vec3_t){0.3f, 0.3f, 0.3f});
	while (!glfwWindowShouldClose(engine->window))
	{
		update();
		scop_engine_render(engine);
	}
	scene_destroy(scene);
#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif
	return (0);
}
