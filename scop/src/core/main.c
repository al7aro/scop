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

void init(GLFWwindow* window)
{
	(void)window;
	mesh_load_cube(&mesh);

	/* TODO: Study how to make the path more portable */
#ifdef _WIN32
	shader_create(&sh, "..\\scop\\assets\\shaders\\main460.vert", "..\\scop\\assets\\shaders\\main460.frag");
	px_load("..\\scop\\assets\\textures\\rgb.pam", NULL, NULL, NULL);
#endif
#if defined(__APPLE__) || defined(__linux__)
	shader_create(&sh, "scop/assets/shaders/main410.vert", "scop/assets/shaders/main410.frag");
	px_load("scop/assets/textures/rgb.pam", NULL, NULL, NULL);
#endif
	shader_use(&sh);

	/* Some simple test math */
	mat4_t ortho;
	mat4_get_proj_ortho(-1, 1, -1, 1, -1, 1, ortho);
	mat4_print(ortho);
	shader_set_mat4(&sh, "proj", ortho);
}

void display(GLFWwindow *window, double currentTime)
{
	(void)window;
	(void)currentTime;
	glClearColor(1.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	mesh_render(mesh);

	mat4_t rot;
	mat4_get_rotY(angle, rot);
	shader_set_mat4(&sh, "rot", rot);
	angle += 0.01f;
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
