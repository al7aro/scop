#include "scop_engine.h"

scop_engine_t* scop_engine_create()
{
    scop_engine_t* scop_engine = (scop_engine_t*)malloc(sizeof(scop_engine_t));
    if (!scop_engine) return NULL;
    memset(scop_engine->clear_color, 0, sizeof(vec3_t));
    strcpy_s(scop_engine->window_title, sizeof(scop_engine->window_title), "Hello World!");
    scop_engine->window_height = 500;
    scop_engine->window_width = 500;
    scop_engine->current_time = 0;
    scop_engine->ative_scene = NULL;

    if (!glfwInit())
    {
		printf("GLFW initialization failed\n");
		return(NULL);
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
	scop_engine->window = glfwCreateWindow(scop_engine->window_width, scop_engine->window_height, scop_engine->window_title, (void *)0, (void *)0);
	glfwMakeContextCurrent(scop_engine->window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize OpenGL context\n");
		return(NULL);
	}

	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

    return scop_engine;
}

void scop_engine_set_input_callback(scop_engine_t* scop_engine, void (*input_callback)(GLFWwindow*, int, int, int, int))
{
	glfwSetKeyCallback(scop_engine->window, input_callback);
}

void scop_engine_set_clear_color(scop_engine_t* scop_engine, vec3_t col)
{
    for (int i = 0; i < 3; i++)
        scop_engine->clear_color[i] = col[i];
}

void scop_engine_render(scop_engine_t* scop_engine)
{
    scop_engine->current_time = glfwGetTime();
    glClearColor(scop_engine->clear_color[0], scop_engine->clear_color[1], scop_engine->clear_color[2], 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene_render(scop_engine->ative_scene);

    glfwSwapBuffers(scop_engine->window);
    glfwPollEvents();
}

void scop_engine_set_active_scene(scop_engine_t* scop_engine, scene_t* scene)
{
    scop_engine->ative_scene = scene;
}

void scop_engine_destroy(scop_engine_t* scop_engine)
{
    glfwDestroyWindow(scop_engine->window);
    glfwTerminate();
    free(scop_engine);
}