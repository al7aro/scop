#ifndef SCOP_ENGINE_H
#define SCOP_ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.h"
#include "scop_math.h"

typedef struct scop_engine_s
{
    GLFWwindow* window;
    char window_title[128];
    int window_width; 
    int window_height; 
    vec3_t clear_color;
    double current_time;

    scene_t* ative_scene;
} scop_engine_t;

scop_engine_t* scop_engine_create();
void scop_engine_destroy(scop_engine_t* scop_engine);

void scop_engine_set_input_callback(scop_engine_t* scop_engine, void (*input_callback)(GLFWwindow*, int, int, int, int));
void scop_engine_set_active_scene(scop_engine_t* scop_engine, scene_t* scene);
void scop_engine_set_clear_color(scop_engine_t* scop_engine, vec3_t col);

void scop_engine_render(scop_engine_t* scop_engine);


#endif