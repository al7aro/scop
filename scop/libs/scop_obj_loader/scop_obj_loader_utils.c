#include "scop_obj_loader.h"

void init_mtl_group(sol_mtl_group_t* mtl, const char* mtl_name)
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

void init_face(sol_face_t* obj)
{
    for (unsigned int att_id = 0; att_id < MAX_ATT_ID; att_id++)
        memset(obj->att[att_id].data, 0, MAX_FACE_SIZE);
    obj->size = 0;
}

void init_scene(sol_scene_t* scene)
{
    memset(scene->mtllib, 0, sizeof(scene->mtllib));
    scene->mtllib;
    scene->obj = NULL;
}

void init_obj(sol_obj_t* obj, char* name)
{
    for (unsigned int att_id = 0; att_id < MAX_ATT_ID; att_id++)
    {
        obj->v[att_id] = NULL;
        obj->v_ptr[att_id] = 0;
        obj->v_cnt[att_id] = 0;
        obj->v_max_size[att_id] = 0;
    }
    obj->mtl_group = NULL;
    strcpy_s(obj->id_name, sizeof(obj->id_name), name);
}

void buff_push_back_float(sol_obj_t* obj, unsigned int att_id, float f)
{
    float* tmp;
    float** buff = &(obj->v[att_id]);
    size_t* ptr = &(obj->v_ptr[att_id]);
    size_t* max_size = &(obj->v_max_size[att_id]);

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
    char* comment_ptr = strrchr(str, '#');
    if (comment_ptr > str) comment_ptr -= 1;
    size_t start = 0, end = (comment_ptr) ? comment_ptr - str : strlen(str) - 1;
    while (isspace(str[start])) start++;
    while (end > start && isspace(str[end])) end--;

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

void free_mlt_group(sol_mtl_group_t* mtl_group)
{
    ft_lstclear(&(mtl_group->faces), free);
    free(mtl_group);
}

void free_obj(sol_obj_t* obj)
{
    for (unsigned int att_id = 0; att_id < MAX_ATT_ID; att_id++)
    {
        if (obj->v[att_id])
            free(obj->v[att_id]);
    }
    ft_lstclear(&(obj->mtl_group), free_mlt_group);
    free(obj);
}

void sol_destroy(sol_scene_t* scene)
{
    ft_lstclear(&(scene->obj), free_obj);
    free(scene);
}