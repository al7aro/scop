#include "scop_model_loader.h"

void init_mtl_group(sml_mtl_group_t* mtl, const char* mtl_name)
{
    strcpy_s(mtl->usemtl, sizeof(mtl->usemtl), mtl_name);
    mtl->faces = NULL;

    mtl->d = 1.0;
    mtl->illum = 0;
    mtl->Ns = 0.5;
    mtl->Ni = 1.0;
    mtl->Ka[0] = 1.0; mtl->Ka[1] = 1.0; mtl->Ka[2] = 1.0;
    mtl->Kd[0] = 1.0; mtl->Kd[1] = 1.0; mtl->Kd[2] = 1.0;
    mtl->Ks[0] = 1.0; mtl->Ks[1] = 1.0; mtl->Ks[2] = 1.0;
    mtl->Ke[0] = 0.0; mtl->Ke[1] = 0.0; mtl->Ke[2] = 0.0;
}

void init_face(sml_face_t* obj)
{
    memset(obj->v_idx, 0, MAX_FACE_SIZE);
    memset(obj->vn_idx, 0, MAX_FACE_SIZE);
    memset(obj->vt_idx, 0, MAX_FACE_SIZE);
    memset(obj->vp_idx, 0, MAX_FACE_SIZE);

    obj->size = 0;
}

void init_scene(sml_scene_t* scene)
{
    memset(scene->mtllib, 0, sizeof(scene->mtllib));
    scene->mtllib;
    scene->obj = NULL;
}

void init_obj(sml_obj_t* obj, char* name)
{
    obj->v = NULL;
    obj->v_id = -1;
    obj->v_ptr = 0;
    obj->v_cnt = 0;
    obj->v_max_size = 0;

    obj->vn = NULL;
    obj->vn_id = -1;
    obj->vn_ptr = 0;
    obj->vn_cnt = 0;
    obj->vn_max_size = 0;

    obj->vt = NULL;
    obj->vt_id = -1;
    obj->vt_ptr = 0;
    obj->vt_cnt = 0;
    obj->vt_max_size = 0;

    obj->vp = NULL;
    obj->vp_id = -1;
    obj->vp_ptr = 0;
    obj->vp_cnt = 0;
    obj->vp_max_size = 0;

    obj->att_id_cnt = -1;

    obj->mtl_group = NULL;

    strcpy_s(obj->id_name, sizeof(obj->id_name), name);
}

void buff_push_back_float(float **buff, size_t *ptr, size_t *max_size, float f)
{
    float* tmp;
    if (*ptr >= *max_size)
    {
        *max_size *= 2;
        if (!*buff && !*max_size)
        {
            *max_size = 16;
            tmp = (float*)malloc(sizeof(float) * *max_size);
        }
        else
            tmp = realloc(*buff, sizeof(float) * *max_size);
        if (tmp)
            *buff = tmp;
        else
            return;
    }
    *(*buff + *ptr) = f;
    *ptr += 1;
}

void trim_spaces(char* str)
{
    size_t start = 0, end = strlen(str) - 1;
    while (str[start] == ' ') start++;
    while (end > start && str[end] == ' ') end--;

    memmove(str, str + start, end - start + 1);
    str[end - start + 1] = '\0';
}

int get_line_data(char* line, float f[16])
{
    int i = 0;
    while (*line != '\0' && i < 16)
    {
        f[i] = (float)strtod(line, &line);
        i++;
    }
    return (i);
}

void free_mlt_group(sml_mtl_group_t* mtl_group)
{
    ft_lstclear(&(mtl_group->faces), free);
    free(mtl_group);
}

void free_obj(sml_obj_t* obj)
{
    if (obj->v)
        free(obj->v);
    if (obj->vn)
        free(obj->vn);
    if (obj->vt)
        free(obj->vt);
    if (obj->vp)
        free(obj->vp);
    ft_lstclear(&(obj->mtl_group), free_mlt_group);
    free(obj);
}

void sml_destroy(sml_scene_t* scene)
{
    ft_lstclear(&(scene->obj), free_obj);
    free(scene);
}