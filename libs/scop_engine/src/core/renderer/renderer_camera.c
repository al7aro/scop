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
	cam->lookat[0] = 0.0; cam->lookat[1] = 0.0; cam->lookat[2] = -1.0;
	cam->right[0] = 1.0; cam->right[1] = 0.0; cam->right[2] = 0.0;
	cam->fov = 60.0f * 3.14f / 180.0f;
	cam->input_handler = NULL;
	return cam;
}

void cam_get_mat_view(cam_t* cam, mat4_t ret)
{
	/* TODO: take the camera rotation into account */
	mat4_get_tras(-cam->empty->pos[0], -cam->empty->pos[1], -cam->empty->pos[2], ret);
}

void cam_get_mat_proj(cam_t* cam, mat4_t ret)
{
	mat4_get_proj_persp(cam->fov, 1.0f, 0.1f, 100.0f, ret);
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

void cam_set_input_handler(cam_t* cam, void (*input_handler)(struct cam_s*, int, int))
{
	cam->input_handler = input_handler;
}

void cam_manage_input_callbacks(cam_t* cam, int key, int action)
{
	if (cam->input_handler)
		cam->input_handler(cam, key, action);
}

void cam_destroy(cam_t* cam)
{
	if (!cam) return;
	empty_destroy(cam->empty);
	free(cam);
}