#ifdef _WIN32
	#include <Windows.h>
#endif

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#ifdef __APPLE__
	#include <unistd.h>
#endif

#include <stdio.h>

#include "shader.h"
#include "scop_math.h"
#include "px_image.h"

GLuint VAO;
GLuint VBO;
GLuint EBO;
float angle = 0;
shader_t sh;

float triangle[9] =
{
	-0.5, -0.5, 0.0,
	0.5, -0.5, 0.0,
	0.0, 0.5, 0.0
};

float square[12 * 2] =
{
	-0.5, -0.5, 0.0,	1.0, 0.0, 0.0,
	0.5, -0.5, 0.0,		0.0, 1.0, 0.0,
	0.5, 0.5, 0.0,		0.0, 0.0, 1.0,
	-0.5, 0.5, 0.0,		1.0, 1.0, 1.0
};

unsigned int square_idx[6] =
{
	0, 1, 3,
	1, 2, 3
};

void init(GLFWwindow* window)
{
	(void)window;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_idx), square_idx, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	/* TODO: Study how to make the path more portable */
#ifdef _WIN32
	shader_create(&sh, "..\\scop\\assets\\shaders\\main460.vert", "..\\scop\\assets\\shaders\\main460.frag");
#endif
#ifdef __APPLE__
	shader_create(&sh, "scop/assets/shaders/main410.vert", "scop/assets/shaders/main410.frag");
#endif
	shader_use(&sh);

	/* Some simple test math */
	mat4_t ortho;
	mat4_get_proj_ortho(-1, 1, -1, 1, -1, 1, ortho);
	mat4_print(ortho);
	shader_set_mat4(&sh, "proj", ortho);

	px_load("..\\scop\\assets\\textures\\rgb.pam", NULL, NULL, NULL);
}

void display(GLFWwindow *window, double currentTime)
{
	(void)window;
	(void)currentTime;
	glClearColor(1.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	// glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	mat4_t rot;
	mat4_get_rotY(angle, rot);
	shader_set_mat4(&sh, "rot", rot);
	angle += 0.01f;
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