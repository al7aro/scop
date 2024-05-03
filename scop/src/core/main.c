#ifdef _WIN32
	#include <Windows.h>
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

float angle = 0;
shader_t sh;
mesh_t* mesh;
unsigned int texture0, texture1;

void init(GLFWwindow* window)
{
	(void)window;
	mesh_load_cube(&mesh);

	/* TODO: Study how to make the path more portable */
#ifdef _WIN32
	shader_create(&sh, "scop\\assets\\shaders\\main460.vert", "scop\\assets\\shaders\\main460.frag");
	px_load("scop\\assets\\textures\\rgb.pam", NULL, NULL, NULL);
#endif
#if defined(__APPLE__) || defined(__linux__)
	shader_create(&sh, "scop/assets/shaders/main410.vert", "scop/assets/shaders/main410.frag");
	// px_load("scop/assets/textures/rgb.pam", NULL, NULL, NULL);
#endif
	shader_use(&sh);

	/* Some simple test math */
	mat4_t ortho, persp;
	mat4_get_proj_ortho(-1, 1, -1, 1, -1, 10, ortho);
	mat4_get_proj_persp(90.0f*3.14f/180.0f, 1, 0.1f, 10, persp);
	mat4_print(persp);
	shader_set_mat4(&sh, "proj", persp);

	/* Textures */
	glGenTextures(1, &texture0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data;
	int w, h, chn;
	glBindTexture(GL_TEXTURE_2D, texture0);
	data = px_load("scop/assets/textures/man.pam", &w, &h, &chn);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	shader_set_int(&sh, "texture0", 0);

	glBindTexture(GL_TEXTURE_2D, texture1);
	data = px_load("scop/assets/textures/woman.pam", &w, &h, &chn);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	shader_set_int(&sh, "texture1", 1);
}

void display(GLFWwindow *window, double currentTime)
{
	(void)window;
	(void)currentTime;
	glClearColor(1.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	mesh_render(mesh);

	mat4_t rot1, rot2, tras, total_rot, model;
	mat4_get_tras(0.0f, 0.0f, -2.0f, tras);
	mat4_get_rotY(angle, rot1);
	mat4_get_rotX(angle, rot2);
	
	mat4_mult_mat4(rot1, rot2, total_rot);
	mat4_mult_mat4(tras, total_rot, model);
	shader_set_mat4(&sh, "rot", model);
	shader_set_float(&sh, "offset", (float)cos(currentTime));
	angle += 0.005f;
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
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(500, 500, "Getting Started with OpenGL", (void *)0, (void *)0);
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
	init(window);
	while (!glfwWindowShouldClose(window))
	{
    	display(window, glfwGetTime());
    	glfwSwapBuffers(window);
    	glfwPollEvents();
	}
	glfwDestroyWindow(window);
    glfwTerminate();
	return (0);
}
