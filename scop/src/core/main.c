#ifdef _WIN32
	#include <Windows.h>
	#include <crtdbg.h>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#if defined(__APPLE__) || defined(__linux__)
	#include <unistd.h>
#endif

#include <stdio.h>

#include "shader.h"
#include "scop_math.h"
#include "px_image.h"
#include "basic_mesh.h"
#include "basic_model.h"
#include "scop_obj_loader.h"
#include "renderer.h"

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
model_t* guitar;
vec3_t guitar_pos = { 0.0, 2.0, 0.0 };

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

void init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);

	model_load(&cube, "C:/Users/al7ar/Desktop/woman_cube.obj");
	model_load(&hexagon, "../scop/assets/models/hexagon.obj");
	model_load(&sphere, "../scop/assets/models/multi_sphere.obj");
	model_load(&guitar , "../scop/assets/models/guitar.obj");
	model_load_GPU(cube);
	model_load_GPU(hexagon);
	model_load_GPU(sphere);
	model_load_GPU(guitar);

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
	entity_set_model(e4, guitar);
	entity_set_pos(e4, guitar_pos);
	entity_set_shader(e4, &toy_sh);
	entity_set_scale(e4, (vec3_t){10.0f, 10.0f, 10.0f});
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
	shader_create(&toy_sh, "../scop\\assets\\shaders\\main460.vert", "../scop\\assets\\shaders\\main460.frag");
	// px_load("scop\\assets\\textures\\rgb.pam", NULL, NULL, NULL);
#endif
#if defined(__APPLE__) || defined(__linux__)
	shader_create(&toy_sh, "scop/assets/shaders/main410.vert", "scop/assets/shaders/main410.frag");
	// px_load("scop/assets/textures/rgb.pam", NULL, NULL, NULL);
#endif
}

void display(GLFWwindow *window, double currentTime)
{
	(void)window;
	(void)currentTime;
	glClearColor(0.32f, 0.32f, 0.32f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	e1->empty->rot[0] += 0.003f;
	e1->empty->rot[1] += 0.001f;
	e1->empty->rot[2] += 0.003f;

	e2->empty->rot[0] += 0.004f;

	e3->empty->rot[0] -= 0.005f;
	e3->empty->rot[1] += 0.01f;
	e3->empty->rot[2] -= 0.002f;

	scene_render(scene);
}

int main(void)
{
	if (!glfwInit())
    {
		printf("GLFW initialization failed\n");
		return(1);
    }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifdef _WIN32
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif
#if defined(__APPLE__) || defined(__linux__)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(800, 800, "Getting Started with OpenGL", (void *)0, (void *)0);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize OpenGL context\n");
		return(1);
	}
	printf("OGL Version: %s\n", glGetString(GL_VERSION));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
#ifdef _WIN32
	char buff[128]; GetCurrentDirectory(sizeof(buff), buff); printf("WD: %s\n", buff);
#endif
#if defined(__APPLE__) || defined(__linux__)
	char buff[128]; getcwd(buff, sizeof(buff)); printf("WD: %s\n", buff);
#endif

	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	init(window);
	while (!glfwWindowShouldClose(window))
	{
    	display(window, glfwGetTime());
    	glfwSwapBuffers(window);
    	glfwPollEvents();
	}
	glfwDestroyWindow(window);
    glfwTerminate();
	scene_destroy(scene);
	_CrtDumpMemoryLeaks();
	return (0);
}
