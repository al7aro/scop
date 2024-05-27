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

float angle = 0;
unsigned int texture0, texture1;

/* LIGHT CUBE */
shader_t light_sh;
model_t* light_model;
vec3_t light_pos = { 2.0, 2.0, 0.0 };

/* TOY CUBE */
shader_t toy_sh;
model_t* toy_model;
vec3_t toy_pos = { 0.0, 0.0, 0.0 };

/* CAMERA */
vec3_t cam_pos = {0.0, 0.0, 5.0};

void init(GLFWwindow* window)
{
	(void)window;
	model_load(&toy_model, "../scop/assets/models/backpack.obj");
	model_load(&light_model, "../scop/assets/models/sphere.obj");
	model_load_GPU(toy_model);
	model_load_GPU(light_model);

	/* TODO: Study how to make the path more portable */
#ifdef _WIN32
	shader_create(&toy_sh, "../scop\\assets\\shaders\\main460.vert", "../scop\\assets\\shaders\\main460.frag");
	shader_create(&light_sh, "../scop\\assets\\shaders\\main460.vert", "../scop\\assets\\shaders\\main460_light.frag");
	// px_load("scop\\assets\\textures\\rgb.pam", NULL, NULL, NULL);
#endif
#if defined(__APPLE__) || defined(__linux__)
	shader_create(&toy_sh, "scop/assets/shaders/main410.vert", "scop/assets/shaders/main410.frag");
	// px_load("scop/assets/textures/rgb.pam", NULL, NULL, NULL);
#endif

	/* TOY & LIGHT CUBE */
	mat4_t ortho, persp;
	mat4_get_proj_ortho(-1, 1, -1, 1, -1, 10, ortho);
	mat4_get_proj_persp(90.0f*3.14f/180.0f, 1, 0.1f, 10, persp);
	mat4_print(persp);
	shader_set_mat4(&toy_sh, "proj", persp);
	shader_set_mat4(&light_sh, "proj", persp);

	/* Textures */
	/*glGenTextures(1, &texture0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char* data;
	int w, h, chn;
	glBindTexture(GL_TEXTURE_2D, texture0);
	data = px_load("../scop/assets/textures/woman.pam", &w, &h, &chn);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	shader_set_int(&toy_sh, "texture0", 0);
	free(data);

	glBindTexture(GL_TEXTURE_2D, texture1);
	data = px_load("../scop/assets/textures/large.pam", &w, &h, &chn);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	shader_set_int(&toy_sh, "texture1", 1);
	free(data);*/
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

	/* MODELS */
	mat4_t view;
	mat4_get_tras(-cam_pos[0], -cam_pos[1], -cam_pos[2], view);

	/* LIGHT CUBE */
	mat4_t lmod, ltras, lscale, lrot;
	mat4_get_tras(light_pos[0], light_pos[1], light_pos[2], ltras);
	mat4_get_identity(lscale); lscale[0] = 0.2f; lscale[5] = 0.2f; lscale[10] = 0.2f;
	mat4_get_rotX(angle, lrot);
	mat4_mult_mat4(lrot, lscale, lmod);
	mat4_mult_mat4(ltras, lmod, lmod);
	mat4_mult_mat4(lrot, lmod, lmod);

	mat4_mult_mat4(view, lmod, lmod);

	shader_set_mat4(&light_sh, "rot", lmod);
	shader_use(&light_sh);
	model_render(light_model);

	/* TOY CUBE */
	mat4_t rot1, rot2, tras, model_mat;
	mat4_get_tras(toy_pos[0], toy_pos[1], toy_pos[2], tras);

	mat4_get_rotY(angle/4, rot1);
	mat4_get_rotX(angle/2, rot2);
	mat4_mult_mat4(rot1, rot2, model_mat);
	mat4_mult_mat4(tras, model_mat, model_mat);
	mat4_mult_mat4(view, model_mat, model_mat);
	shader_set_mat4(&toy_sh, "rot", model_mat);
	//shader_set_float(&toy_sh, "offset", (float)cos(currentTime));
	angle += 0.002f;

	vec4_t lpos = {light_pos[0], light_pos[1], light_pos[2], 1.0};
	mat4_mult_vec4(lmod, lpos, lpos);
	shader_set_vec3(&toy_sh, "light_src_pos", lpos);

	shader_use(&toy_sh);
	model_render(toy_model);
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
	model_destroy(toy_model);
	model_destroy(light_model);
	_CrtDumpMemoryLeaks();
	return (0);
}
