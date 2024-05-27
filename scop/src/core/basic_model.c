#include "basic_model.h"

t_list* face_subdiv(t_list* face_lst)
{
	sml_face_t* face = face_lst->content;
	if (face->size >= MAX_FACE_SIZE)
		return face_lst;
	t_list* new_face_lst = NULL;
	for (size_t i = 0; i < face->size - 3; i++)
	{
		sml_face_t* new_face = (sml_face_t*)malloc(sizeof(sml_face_t));
		if (!new_face) return NULL;
		new_face->size = 3;
		new_face->v_idx[0] = face->v_idx[0];
		new_face->v_idx[1] = face->v_idx[i + 2];
		new_face->v_idx[2] = face->v_idx[i + 3];

		new_face->vt_idx[0] = face->vt_idx[0];
		new_face->vt_idx[1] = face->vt_idx[i + 2];
		new_face->vt_idx[2] = face->vt_idx[i + 3];

		new_face->vn_idx[0] = face->vn_idx[0];
		new_face->vn_idx[1] = face->vn_idx[i + 2];
		new_face->vn_idx[2] = face->vn_idx[i + 3];

		new_face->vp_idx[0] = face->vp_idx[0];
		new_face->vp_idx[1] = face->vp_idx[i + 2];
		new_face->vp_idx[2] = face->vp_idx[i + 3];
		ft_lstadd_back(&new_face_lst, ft_lstnew(new_face));
	}
	if (!new_face_lst)
		return face_lst;
	face->size = 3;
	(ft_lstlast(new_face_lst))->next = face_lst->next;
	face_lst->next = new_face_lst;
	printf("LST: %d\n", ft_lstsize(new_face_lst));
	return face_lst;
}

mesh_t* model_load_mesh(sml_obj_t* obj, sml_mtl_group_t* mtl_group)
{
	t_list* face_lst = mtl_group->faces;
	mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t));

	mesh_init(mesh, mtl_group->usemtl);
	while (face_lst)
	{
		sml_face_t* face = face_lst->content;
		if (face->size >= 4)
			face_lst = face_subdiv(face_lst);
		for (size_t i = 0; i < face->size; i++)
		{
			float f[32];
			/* Pos att */
			size_t face_id = face->v_idx[i] - 1;
			for (size_t v_i = 0; v_i < obj->v_cnt; v_i++)
				f[v_i] = obj->v[(face_id * obj->v_cnt) + v_i];
			mesh_push_att(mesh, f, (unsigned int)obj->v_cnt);

			/* Tex att */
			face_id = face->vt_idx[i] - 1;
			for (size_t v_i = 0; v_i < obj->vt_cnt; v_i++)
				f[v_i] = obj->vt[(face_id * obj->vt_cnt) + v_i];
			mesh_push_att(mesh, f, (unsigned int)obj->vt_cnt);

			/* Normal att */
			face_id = face->vn_idx[i] - 1;
			for (size_t v_i = 0; v_i < obj->vn_cnt; v_i++)
				f[v_i] = obj->vn[(face_id * obj->vn_cnt) + v_i];
			mesh_push_att(mesh, f, (unsigned int)obj->vn_cnt);

			mesh->data->buff_cnt++;
		}
		face_lst = face_lst->next;
	}
	mesh_set_format(mesh, obj);
	return mesh;
}

/* Loads all material groups from obj */
void model_load_obj(model_t* model, sml_obj_t* obj)
{
	mesh_t* mesh; (void)mesh;
	t_list* mtl_group = obj->mtl_group;

	while (mtl_group)
	{
		(void)model;
		mesh = model_load_mesh(obj, mtl_group->content);
		ft_lstadd_back(&(model->mesh), ft_lstnew(mesh));
		mtl_group = mtl_group->next;
	}
}

void model_load(model_t** ret, const char* file)
{
	model_t* model = (model_t*)malloc(sizeof(model_t));
	if (!model) return;
	model->mesh = NULL;
	sml_scene_t* scene = sml_load_wavefront_obj(file);
	t_list* obj = scene->obj;

	while (obj)
	{
		model_load_obj(model, obj->content);
		obj = obj->next;
	}

	*ret = model;
	sml_destroy(scene);
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

void model_render(model_t* model)
{
	t_list* mesh = model->mesh;
	while (mesh)
	{
		mesh_render(mesh->content);
		mesh = mesh->next;
	}
}