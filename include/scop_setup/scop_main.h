#ifndef SCOP_MAIN_H
#define SCOP_MAIN_H

#ifdef _WIN32
#include <crtdbg.h>
#endif

#include <stdio.h>

#include "scop_engine.h"
#include "scop_macros.h"

/* INPUT HANDLERS */
void common_update_camera(cam_t* cam);
void common_camera_keyboard_callback(cam_t* cam, GLFWwindow* window, int key, int action);
void common_camera_mouse_callback(cam_t* cam, GLFWwindow* window, double xpos, double ypos);
void common_trigger_input_callback(entity_t* e, GLFWwindow* window, int key, int action);
void common_trigger_update(entity_t* e);

scene_t* default_scene_create(const char* scene_name);
scene_t* guitar_scene_create(const char* scene_name);

#endif
