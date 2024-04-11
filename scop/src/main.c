#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
	#include <Windows.h>
#endif
#ifdef __APPLE__
	#include <unistd.h>
#endif

#include <stdio.h>

#include "shader_reader.h"
#include "scop_math.h"

GLuint VAO;
GLuint VBO;

float triangle[9] =
{
	-0.5, -0.5, 0.0,
	0.5, -0.5, 0.0,
	0.0, 0.5, 0.0
};

void init(GLFWwindow* window)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	GLuint program;
	/* TODO: Study how to make the path more portable */
	program = shader_create("../../scop/shaders/main410.vert", "../../scop/shaders/main410.frag");
	glUseProgram(program);

	/* Some simple test math */
	mat4_t ortho;
	mat4_get_proj_ortho(-1, 1, -1, 1, -1, 1, ortho);
	mat4_print(ortho);
	GLuint proj_loc = glGetUniformLocation(program, "proj");
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, ortho);
}

void display(GLFWwindow *window, double currentTime)
{
	glClearColor(1.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main()
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
#ifdef __APPLE__
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
	#ifdef __APPLE__
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