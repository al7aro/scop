#include <stdio.h>

#include "scop_engine.h"

/* INPUT CALLBACKS */

void engine_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void)window; (void)key; (void)scancode; (void)action; (void)mods;
	scop_engine_t* engine_ptr = glfwGetWindowUserPointer(window);
	scene_t* scene_ptr = engine_ptr->ative_scene;
	scene_manage_input_callbacks(scene_ptr, key, action);
}

void camera_callback(cam_t* cam, int key, int action)
{
	(void)cam;  (void)key; (void)action;
	if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS)
		{
			cam->empty->input_motion.front = 1;
			cam->empty->input_motion.back = 0;
		}
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.front = 0;
	}
	if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS)
		{
			cam->empty->input_motion.front = 0;
			cam->empty->input_motion.back = 1;
		}
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.back = 0;
	}
	if (key == GLFW_KEY_A)
	{
		if (action == GLFW_PRESS)
		{
			cam->empty->input_motion.left = 1;
			cam->empty->input_motion.right = 0;
		}
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.left = 0;
	}
	if (key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS)
		{
			cam->empty->input_motion.left = 0;
			cam->empty->input_motion.right = 1;
		}
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.right = 0;
	}
	if (key == GLFW_KEY_L)
	{
		if (action == GLFW_PRESS)
		{
			cam->empty->input_motion.rot_right = 1;
			cam->empty->input_motion.rot_left = 0;
		}
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.rot_right = 0;
	}
	if (key == GLFW_KEY_J)
	{
		if (action == GLFW_PRESS)
		{
			cam->empty->input_motion.rot_right = 0;
			cam->empty->input_motion.rot_left = 1;
		}
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.rot_left = 0;
	}
}

/* UPDATE CALLBACKS */

void update_camera(cam_t* cam)
{
	float move_speed = 0.08f;
	float rot_speed = 0.015f;
	if (cam->empty->input_motion.front)
	{
		vec3_t front; (void)front;
		vec3_times_float(cam->lookat, -move_speed, front);
		vec3_plus_vec3(cam->empty->pos, front, cam->empty->pos);
	}
	if (cam->empty->input_motion.back)
	{
		vec3_t front;
		vec3_times_float(cam->lookat, -move_speed, front);
		vec3_minus_vec3(cam->empty->pos, front, cam->empty->pos);
	}
	if (cam->empty->input_motion.left)
	{
		vec3_t right;
		vec3_times_float(cam->right, -move_speed, right);
		vec3_plus_vec3(cam->empty->pos, right, cam->empty->pos);
	}
	if (cam->empty->input_motion.right)
	{
		vec3_t right;
		vec3_times_float(cam->right, -move_speed, right);
		vec3_minus_vec3(cam->empty->pos, right, cam->empty->pos);
	}
	if (cam->empty->input_motion.rot_right)
	{
		mat4_t rotY;
		mat4_get_rotY(rot_speed, rotY);

		vec4_t right4; vec3_to_vec4(cam->right, right4);
		mat4_mult_vec4(rotY, right4, right4);
		cam->right[0] = right4[0];
		cam->right[1] = right4[1];
		cam->right[2] = right4[2];

		vec4_t lookat4; vec3_to_vec4(cam->lookat, lookat4);
		mat4_mult_vec4(rotY, lookat4, lookat4);
		cam->lookat[0] = lookat4[0];
		cam->lookat[1] = lookat4[1];
		cam->lookat[2] = lookat4[2];

	}
	if (cam->empty->input_motion.rot_left)
	{
		mat4_t rotY;
		mat4_get_rotY(-rot_speed, rotY);

		vec4_t right4; vec3_to_vec4(cam->right, right4);
		mat4_mult_vec4(rotY, right4, right4);
		cam->right[0] = right4[0];
		cam->right[1] = right4[1];
		cam->right[2] = right4[2];

		vec4_t lookat4; vec3_to_vec4(cam->lookat, lookat4);
		mat4_mult_vec4(rotY, lookat4, lookat4);
		cam->lookat[0] = lookat4[0];
		cam->lookat[1] = lookat4[1];
		cam->lookat[2] = lookat4[2];

	}
}

void update_rotating_cube(entity_t* e)
{
	e->empty->rot[0] += 0.003f;
	e->empty->rot[1] += 0.001f;
	e->empty->rot[2] += 0.003f;
}

void update_orbiting_light(entity_t* e)
{
	e->empty->rot[1] += 0.002f;
}