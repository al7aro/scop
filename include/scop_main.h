#ifndef SCOP_MAIN_H
#define SCOP_MAIN_H

#ifdef _WIN32
#include <crtdbg.h>
#endif

#include <stdio.h>

#include "scop_engine.h"

/* INPUT HANDLERS */
void engine_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void camera_callback(cam_t* cam, int key, int action);

/* UPDATE HANDLERS */
void update_camera(cam_t* cam);
void update_rotating_cube(entity_t* e);
void update_orbiting_light(entity_t* e);

#endif