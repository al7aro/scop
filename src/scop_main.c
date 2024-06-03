#include "scop_main.h"

double prev_x = 0;
double prev_y = 0;

void engine_keyboard_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void)window; (void)key; (void)scancode; (void)action; (void)mods;
	scop_engine_t* engine_ptr = glfwGetWindowUserPointer(window);
	scene_t* scene_ptr = engine_ptr->ative_scene;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		printf("LOL?\n");
		if (engine_ptr->ative_scene)
			scene_reset_inputs(engine_ptr->ative_scene);
		engine_ptr->pause = !(engine_ptr->pause);
		if (!engine_ptr->pause)
			glfwSetInputMode(engine_ptr->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(engine_ptr->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (engine_ptr->pause)
		return;
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		scop_engine_set_active_scene_next(engine_ptr);

	glfwSetInputMode(engine_ptr->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	scene_manage_keyboard_input_callbacks(scene_ptr, window, key, action);
}

void engine_mouse_input_callback(GLFWwindow* window, double xpos, double ypos)
{
	(void)window; (double)xpos; (double)ypos;
	scop_engine_t* engine_ptr = glfwGetWindowUserPointer(window);
	scene_t* scene_ptr = engine_ptr->ative_scene;

	if (engine_ptr->pause)
		return;
	scene_manage_mouse_input_callbacks(scene_ptr, window, xpos, ypos);
}

int main(void)
{
	scop_engine_t* engine = scop_engine_create();
	scene_t* guitar_scene = guitar_scene_create("guitar_scene");
	scene_t* default_scene = default_scene_create("default_scene");

	scop_engine_add_scene(engine, guitar_scene);
	scop_engine_add_scene(engine, default_scene);
	scop_engine_set_active_scene(engine, "guitar_scene");
	scop_engine_set_clear_color(engine, (vec3_t){0.3f, 0.3f, 0.3f});
	scop_engine_set_keyboard_input_callback(engine, engine_keyboard_input_callback);
	scop_engine_set_mouse_input_callback(engine, engine_mouse_input_callback);
	printf("Everything loaded correctly. Enjoy!");
	while (!glfwWindowShouldClose(engine->window))
	{
		scop_engine_update(engine);
		scop_engine_render(engine);
	}
#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif
	return (0);
}
