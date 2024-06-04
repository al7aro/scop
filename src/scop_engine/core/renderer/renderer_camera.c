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

	cam->keyboard_input_handlers = NULL;
	cam->mouse_input_handlers = NULL;
	cam->update_handlers = NULL;
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

void cam_add_keyboard_input_handler(cam_t* cam, void (*keyboard_input_handler)(struct cam_s*, GLFWwindow*, int, int))
{
	ft_lstadd_back(&(cam->keyboard_input_handlers), ft_lstnew((void*)keyboard_input_handler));
}

void cam_add_mouse_input_handler(cam_t* cam, void (*mouse_input_handler)(struct cam_s*, GLFWwindow*, double, double))
{
	ft_lstadd_back(&(cam->mouse_input_handlers), ft_lstnew((void*)mouse_input_handler));
}

void cam_add_update_handler(cam_t* cam, void (*update_handler)(struct cam_s*))
{
	ft_lstadd_back(&(cam->update_handlers), ft_lstnew((void*)update_handler));
}

void cam_manage_keyboard_input_callbacks(cam_t* cam, GLFWwindow* window, int key, int action)
{
	t_list* keyboard_input_handlers_lst = cam->keyboard_input_handlers;
	while (keyboard_input_handlers_lst)
	{
		void (*keyboard_input_handler)(cam_t*, GLFWwindow*, int, int);
		keyboard_input_handler = (void (*)(cam_t*, GLFWwindow*, int, int))keyboard_input_handlers_lst->content;
		keyboard_input_handler(cam, window, key, action);

		keyboard_input_handlers_lst = keyboard_input_handlers_lst->next;
	}
}

void cam_manage_mouse_input_callbacks(cam_t* cam, GLFWwindow* window, double xpos, double ypos)
{
	t_list* keyboard_mouse_handlers_lst = cam->mouse_input_handlers;
	while (keyboard_mouse_handlers_lst)
	{
		void (*keyboard_mouse_handler)(cam_t*, GLFWwindow*, double, double);
		keyboard_mouse_handler = (void (*)(cam_t*, GLFWwindow*, double, double))keyboard_mouse_handlers_lst->content;
		keyboard_mouse_handler(cam, window, xpos, ypos);

		keyboard_mouse_handlers_lst = keyboard_mouse_handlers_lst->next;
	}
}

void cam_update(cam_t* cam)
{
	t_list* update_handlers_lst = cam->update_handlers;
	while (update_handlers_lst)
	{
		void (*update_handler)(cam_t*);
		update_handler = (void (*)(cam_t*))update_handlers_lst->content;
		update_handler(cam);

		update_handlers_lst = update_handlers_lst->next;
	}
}

void cam_destroy(cam_t* cam)
{
	if (!cam) return;
	empty_destroy(cam->empty);
	ft_lstclear(&(cam->keyboard_input_handlers), NULL);
	ft_lstclear(&(cam->mouse_input_handlers), NULL);
	ft_lstclear(&(cam->update_handlers), NULL);
	free(cam);
}