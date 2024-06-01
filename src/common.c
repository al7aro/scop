#include <stdio.h>

#include "scop_engine.h"

double xprev = -100000, yprev = -100000;

void common_camera_mouse_callback(cam_t* cam, GLFWwindow* window, double xpos, double ypos)
{
	(void)window;
	double xdir = xpos - xprev;
	double ydir = ypos - yprev; (void)ydir;
	vec2_t dir; dir[0] = (float)xdir; dir[1] = (float)ydir;
	vec2_normal(dir, dir);

	cam->empty->input_motion.rot_x_world = (float)-dir[1];
	cam->empty->input_motion.rot_y_world = (float)-dir[0];

	xprev = xpos;
	yprev = ypos;
}

void common_camera_keyboard_callback(cam_t* cam, GLFWwindow* window, int key, int action)
{
	(void)window; (void)cam;  (void)key; (void)action;
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
}

/* UPDATE CALLBACKS */

void common_update_camera(cam_t* cam)
{
	float move_speed = 0.06f;
	float rot_speed = 0.02f;

	vec3_t x;
	vec3_times_float(cam->lookat, move_speed * cam->empty->input_motion.move_x_local, x);
	vec3_plus_vec3(cam->empty->pos, x, cam->empty->pos);

	vec3_t y;
	vec3_times_float(cam->right, move_speed * cam->empty->input_motion.move_y_local, y);
	vec3_plus_vec3(cam->empty->pos, y, cam->empty->pos);

	vec3_t z;
	vec3_times_float(cam->up, move_speed * cam->empty->input_motion.move_z_local, z);
	vec3_plus_vec3(cam->empty->pos, z, cam->empty->pos);

	//TODO: I SHOULD PREVENT CAMERA FROM GO COMPLETELY UP OR DOWN
	/* COMPUTE CAMERA ROTATION */
	mat4_t rotY, rotX;
	vec4_t y_dir, x_dir; y_dir[0] = 0.0; y_dir[1] = 1.0; y_dir[2] = 0.0; y_dir[3] = 1.0;
	/* Y ROTATION */
	mat4_get_rotU(rot_speed * cam->empty->input_motion.rot_y_world, y_dir, rotY);
	vec4_t lookat4; vec3_to_vec4(cam->lookat, lookat4);
	mat4_mult_vec4(rotY, lookat4, lookat4);
	cam->lookat[0] = lookat4[0];
	cam->lookat[1] = lookat4[1];
	cam->lookat[2] = lookat4[2];
	vec3_normal(cam->lookat, cam->lookat);
	vec4_t up4; vec3_to_vec4(cam->up, up4);
	mat4_mult_vec4(rotY, up4, up4);
	cam->up[0] = up4[0];
	cam->up[1] = up4[1];
	cam->up[2] = up4[2];
	vec3_normal(cam->up, cam->up);
	vec4_t right4; vec3_to_vec4(cam->right, right4);
	mat4_mult_vec4(rotY, right4, right4);
	cam->right[0] = right4[0];
	cam->right[1] = right4[1];
	cam->right[2] = right4[2];
	vec3_normal(cam->right, cam->right);
	/* X ROTATION */
	vec3_to_vec4(cam->right, x_dir);
	mat4_get_rotU(rot_speed * cam->empty->input_motion.rot_x_world, x_dir, rotX);
	vec3_to_vec4(cam->right, right4);
	mat4_mult_vec4(rotX, right4, right4);
	cam->right[0] = right4[0];
	cam->right[1] = right4[1];
	cam->right[2] = right4[2];
	vec3_normal(cam->right, cam->right);
	vec3_to_vec4(cam->lookat, lookat4);
	mat4_mult_vec4(rotX, lookat4, lookat4);
	cam->lookat[0] = lookat4[0];
	cam->lookat[1] = lookat4[1];
	cam->lookat[2] = lookat4[2];
	vec3_normal(cam->lookat, cam->lookat);
	vec3_to_vec4(cam->up, up4);
	mat4_mult_vec4(rotX, up4, up4);
	cam->up[0] = up4[0];
	cam->up[1] = up4[1];
	cam->up[2] = up4[2];
	vec3_normal(cam->up, cam->up);

	cam->empty->input_motion.rot_y_world = 0;
	cam->empty->input_motion.rot_x_world = 0;
}