#include "scop_main.h"

void engine_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void)window; (void)key; (void)scancode; (void)action; (void)mods;
	scop_engine_t* engine_ptr = glfwGetWindowUserPointer(window);
	scene_t* scene_ptr = engine_ptr->ative_scene;
	scene_manage_input_callbacks(scene_ptr, key, action);

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		scop_engine_set_active_scene_next(engine_ptr);
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
	scop_engine_set_input_callback(engine, engine_input_callback);
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
