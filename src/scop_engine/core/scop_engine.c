#include "scop_engine.h"

scop_engine_t* scop_engine_create()
{
    scop_engine_t* scop_engine = (scop_engine_t*)malloc(sizeof(scop_engine_t));
    if (!scop_engine) return NULL;
    memset(scop_engine->clear_color, 0, sizeof(vec3_t));
    strcpy_s(scop_engine->window_title, sizeof(scop_engine->window_title), "Hello World!");
    scop_engine->window_height = 900;
    scop_engine->window_width = 900;
    scop_engine->current_time = 0;
    scop_engine->ative_scene = NULL;
    scop_engine->scenes = NULL;
    scop_engine->pause = 1;

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

void scop_engine_set_keyboard_input_callback(scop_engine_t* scop_engine, void (*input_callback)(GLFWwindow*, int, int, int, int))
{
    glfwSetWindowUserPointer(scop_engine->window, scop_engine);
	glfwSetKeyCallback(scop_engine->window, input_callback);
}

void scop_engine_set_mouse_input_callback(scop_engine_t* scop_engine, void (*input_callback)(GLFWwindow*, double, double))
{
    glfwSetWindowUserPointer(scop_engine->window, scop_engine);
    glfwSetCursorPosCallback(scop_engine->window, input_callback);
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

void scop_engine_update(scop_engine_t* scop_engine)
{
    if (!scop_engine->pause)
        scene_update(scop_engine->ative_scene);
}

void scop_engine_set_active_scene(scop_engine_t* scop_engine, const char* name_id)
{
    t_list* scene_lst = scop_engine->scenes;
    
    while (scene_lst)
    {
        scene_t* scene;
        scene = scene_lst->content;
        if (!strcmp(scene->name_id, name_id))
        {
            scop_engine->ative_scene = scene;
            return;
        }
        scene_lst = scene_lst->next;
    }
    scop_engine->ative_scene = scop_engine->scenes->content;
}

void scop_engine_set_active_scene_next(scop_engine_t* scop_engine)
{
    t_list* scene_lst = scop_engine->scenes;

    while (scene_lst)
    {
        scene_t* scene;
        scene = scene_lst->content;
        if (!strcmp(scene->name_id, scop_engine->ative_scene->name_id))
        {
            if (scene_lst->next == NULL)
                scop_engine->ative_scene = scop_engine->scenes->content;
            else
                scop_engine->ative_scene = scene_lst->next->content;
            scene_reset_inputs(scop_engine->ative_scene);
            return;
        }
        scene_lst = scene_lst->next;
    }
    scop_engine->ative_scene = scop_engine->scenes->content;
}

void scop_engine_add_scene(scop_engine_t* scop_engine, scene_t* scene)
{
    ft_lstadd_back(&(scop_engine->scenes), ft_lstnew(scene));
}

void scop_engine_destroy(scop_engine_t* scop_engine)
{
    if (!scop_engine) return;
    glfwDestroyWindow(scop_engine->window);
    glfwTerminate();

    ft_lstclear(&(scop_engine->scenes), scene_destroy);
    free(scop_engine);
}
