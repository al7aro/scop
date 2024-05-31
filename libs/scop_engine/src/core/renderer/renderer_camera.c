#include "renderer.h"

/* CAMERA */
cam_t* cam_create(void)
{
	cam_t* cam;
	cam = (cam_t*)malloc(sizeof(cam_t));
	if (!cam) return NULL;
	cam->empty = empty_create();
	if (!cam->empty) return NULL;

	cam->up[0] = 0.0; cam->up[1] = 1.0; cam->up[2] = 0.0;
	cam->lookat[0] = 0.0; cam->lookat[1] = 0.0; cam->lookat[2] = 1.0;
	cam->right[0] = 1.0; cam->right[1] = 0.0; cam->right[2] = 0.0;
	cam->fov = 60.0f * 3.14f / 180.0f;
	cam->keyboard_input_handler = NULL;
	cam->mouse_input_handler = NULL;
	cam->update_handler = NULL;
	return cam;
}

void cam_get_mat_view(cam_t* cam, mat4_t ret)
{
	mat4_get_lookat(cam->empty->pos, cam->up, cam->right, cam->lookat, ret);
}

void cam_get_mat_proj(cam_t* cam, mat4_t ret)
{
	mat4_get_proj_persp(cam->fov, 1.0f, 0.1f, 100.0f, ret);
}

void cam_get_pos(cam_t* cam, vec3_t ret)
{
	for (int i = 0; i < 3; i++)
		ret[i] = cam->empty->pos[i];
}

void cam_set_pos(cam_t* cam, vec3_t pos)
{
	for (int i = 0; i < 3; i++)
		cam->empty->pos[i] = pos[i];
}

void cam_set_fov(cam_t* cam, float fov)
{
	cam->fov = fov;
}

void cam_set_lookat(cam_t* cam, vec3_t lookat)
{
	for (int i = 0; i < 3; i++)
		cam->lookat[i] = lookat[i];
}

void cam_set_up(cam_t* cam, vec3_t up)
{
	for (int i = 0; i < 3; i++)
		cam->up[i] = up[i];
}

void cam_set_right(cam_t* cam, vec3_t right)
{
	for (int i = 0; i < 3; i++)
		cam->right[i] = right[i];
}

void cam_set_keyboard_input_handler(cam_t* cam, void (*keyboard_input_handler)(struct cam_s*, GLFWwindow*, int, int))
{
	cam->keyboard_input_handler = keyboard_input_handler;
}

void cam_set_mouse_input_handler(cam_t* cam, void (*mouse_input_handler)(struct cam_s*, GLFWwindow*, double, double))
{
	cam->mouse_input_handler = mouse_input_handler;
}

void cam_set_update_handler(cam_t* cam, void (*update_handler)(struct cam_s*))
{
	cam->update_handler = update_handler;
}

void cam_manage_keyboard_input_callbacks(cam_t* cam, GLFWwindow* window, int key, int action)
{
	if (cam->keyboard_input_handler)
		cam->keyboard_input_handler(cam, window, key, action);
}

void cam_manage_mouse_input_callbacks(cam_t* cam, GLFWwindow* window, double xpos, double ypos)
{
	if (cam->mouse_input_handler)
		cam->mouse_input_handler(cam, window, xpos, ypos);
}

void cam_update(cam_t* cam)
{
	if (cam->update_handler)
		cam->update_handler(cam);
}

void cam_destroy(cam_t* cam)
{
	if (!cam) return;
	empty_destroy(cam->empty);
	free(cam);
}