#include <stdio.h>

#include "scop_engine.h"

void common_camera_callback(cam_t* cam, int key, int action)
{
	(void)cam;  (void)key; (void)action;
	if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS)
			cam->empty->input_motion.move_x_local--;
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.move_x_local++;
	}
	if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS)
			cam->empty->input_motion.move_x_local++;
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.move_x_local--;
	}
	if (key == GLFW_KEY_A)
	{
		if (action == GLFW_PRESS)
			cam->empty->input_motion.move_y_local--;
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.move_y_local++;
	}
	if (key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS)
			cam->empty->input_motion.move_y_local++;
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.move_y_local--;
	}
	if (key == GLFW_KEY_E)
	{
		if (action == GLFW_PRESS)
			cam->empty->input_motion.move_z_local--;
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.move_z_local++;
	}
	if (key == GLFW_KEY_Q)
	{
		if (action == GLFW_PRESS)
			cam->empty->input_motion.move_z_local++;
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.move_z_local--;
	}
	if (key == GLFW_KEY_RIGHT)
	{
		if (action == GLFW_PRESS)
			cam->empty->input_motion.rot_y_world++;
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.rot_y_world--;
	}
	if (key == GLFW_KEY_LEFT)
	{
		if (action == GLFW_PRESS)
			cam->empty->input_motion.rot_y_world--;
		if (action == GLFW_RELEASE)
			cam->empty->input_motion.rot_y_world++;
	}
}

/* UPDATE CALLBACKS */

void common_update_camera(cam_t* cam)
{
	float move_speed = 0.08f;
	float rot_speed = 0.015f;

	vec3_t x;
	vec3_times_float(cam->lookat, move_speed * cam->empty->input_motion.move_x_local, x);
	vec3_plus_vec3(cam->empty->pos, x, cam->empty->pos);

	vec3_t y;
	vec3_times_float(cam->right, move_speed * cam->empty->input_motion.move_y_local, y);
	vec3_plus_vec3(cam->empty->pos, y, cam->empty->pos);

	vec3_t z;
	vec3_times_float(cam->up, move_speed * cam->empty->input_motion.move_z_local, z);
	vec3_plus_vec3(cam->empty->pos, z, cam->empty->pos);

	mat4_t rotY;
	mat4_get_rotY(rot_speed * cam->empty->input_motion.rot_y_world, rotY);
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