#ifndef SCOP_MAIN_H
#define SCOP_MAIN_H

#ifdef _WIN32
#include <crtdbg.h>
#endif

#include <stdio.h>

#include "scop_engine.h"

/* INPUT HANDLERS */
void common_update_camera(cam_t* cam);
void common_camera_callback(cam_t* cam, int key, int action);

scene_t* default_scene_create(const char* scene_name);
scene_t* guitar_scene_create(const char* scene_name);

#endif