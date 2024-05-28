#include "basic_model.h"

t_list* face_subdiv(t_list* face_lst)
{
	sol_face_t* face = face_lst->content;
	if (face->size >= MAX_FACE_SIZE)
		return face_lst;
	t_list* new_face_lst = NULL;
	for (size_t i = 0; i < face->size - 3; i++)
	{
		sol_face_t* new_face = (sol_face_t*)malloc(sizeof(sol_face_t));
		if (!new_face) return NULL;
		new_face->size = 3;
		for (unsigned int att_id = 0; att_id < MAX_ATT_ID; att_id++)
		{
			new_face->att[att_id].data[0] = face->att[att_id].data[0];
			new_face->att[att_id].data[1] = face->att[att_id].data[i + 2];
			new_face->att[att_id].data[2] = face->att[att_id].data[i + 3];
		}
		ft_lstadd_back(&new_face_lst, ft_lstnew(new_face));
	}
	if (!new_face_lst)
		return face_lst;
	face->size = 3;
	(ft_lstlast(new_face_lst))->next = face_lst->next;
	face_lst->next = new_face_lst;
	return face_lst;
}

mesh_t* model_load_mesh(sol_model_t* model_data, sol_mtl_group_t* mtl_group)
{
	t_list* face_lst = mtl_group->faces;
	mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t));
	if (!mesh) return NULL;

	mesh_init(mesh);
	mesh_set_mtl(mesh, mtl_group);
	while (face_lst)
	{
		sol_face_t* face = face_lst->content;
		if (face->size >= 4)
			face_lst = face_subdiv(face_lst);
		for (size_t i = 0; i < face->size; i++)
		{
			float f[32];
			for (unsigned int att_id = 0; att_id < MAX_ATT_ID; att_id++)
			{
				size_t face_id = face->att[att_id].data[i] - 1;
				for (size_t v_i = 0; v_i < model_data->v_cnt[att_id]; v_i++)
					f[v_i] = model_data->v[att_id][(face_id * model_data->v_cnt[att_id]) + v_i];
				mesh_push_att(mesh, f, (unsigned int)model_data->v_cnt[att_id]);
			}
			mesh->data->buff_cnt++;
		}
		face_lst = face_lst->next;
	}
	mesh_set_format(mesh, model_data);
	return mesh;
}

/* Loads all material groups from obj */
void model_load_obj(model_t* model, sol_model_t* model_data, sol_obj_t* obj)
{
	mesh_t* mesh; (void)mesh;
	t_list* mtl_group = obj->mtl_group;

	while (mtl_group)
	{
		mesh = model_load_mesh(model_data, mtl_group->content);
		ft_lstadd_back(&(model->mesh), ft_lstnew(mesh));
		mtl_group = mtl_group->next;
	}
}

void model_load(model_t** ret, const char* file)
{
	model_t* model = (model_t*)malloc(sizeof(model_t));
	if (!model) return;
	model->mesh = NULL;
	sol_model_t* model_data = sol_load_wavefront_obj(file);
	t_list* obj = model_data->obj;

	while (obj)
	{
		model_load_obj(model, model_data, obj->content);
		obj = obj->next;
	}

	*ret = model;
	sol_destroy(model_data);
}

void model_load_GPU(model_t* model)
{
	t_list* mesh = model->mesh;
	while (mesh)
	{
		mesh_load_GPU(mesh->content);
		mesh = mesh->next;
	}
}

void model_render(model_t* model, unsigned int sh_id)
{
	t_list* mesh = model->mesh;
	while (mesh)
	{
		mesh_render(mesh->content, sh_id);
		mesh = mesh->next;
	}
}

void model_destroy(model_t* model)
{
	/*TODO: DESTROY FROM GPU */
	ft_lstclear(&(model->mesh), (void (*)(void *))mesh_destroy);
	free(model);
}
