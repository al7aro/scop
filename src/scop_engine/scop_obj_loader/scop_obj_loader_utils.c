#include "scop_obj_loader.h"

void init_mtl_group(sol_mtl_group_t* mtl, const char* mtl_name)
{
    strcpy(mtl->usemtl, mtl_name);
    mtl->faces = NULL;

    mtl->d = 1.0;
    mtl->illum = 0;
    mtl->Ns = 0.5;
    mtl->Ni = 1.0;
    mtl->Ka[0] = 1.0; mtl->Ka[1] = 1.0; mtl->Ka[2] = 1.0;
    mtl->Kd[0] = 1.0; mtl->Kd[1] = 1.0; mtl->Kd[2] = 1.0;
    mtl->Ks[0] = 1.0; mtl->Ks[1] = 1.0; mtl->Ks[2] = 1.0;
    mtl->Ke[0] = 0.0; mtl->Ke[1] = 0.0; mtl->Ke[2] = 0.0;
    memset(mtl->map_Ks, 0, sizeof(mtl->map_Ks));
    memset(mtl->map_Kd, 0, sizeof(mtl->map_Kd));
    memset(mtl->map_Bump, 0, sizeof(mtl->map_Bump));
}

void init_face(sol_face_t* obj)
{
    for (unsigned int att_id = 0; att_id < SCOP_MAX_ATT; att_id++)
        memset(obj->att[att_id].data, 0, MAX_FACE_SIZE);
    obj->size = 0;
}

void init_model(sol_model_t* model)
{
    model->obj = NULL;
    for (unsigned int att_id = 0; att_id < SCOP_MAX_ATT; att_id++)
    {
        model->v[att_id] = NULL;
        model->v_ptr[att_id] = 0;
        model->v_cnt[att_id] = 0;
        model->v_max_size[att_id] = 0;
    }
    memset(model->mtllib, 0, sizeof(model->mtllib));
}

void init_obj(sol_obj_t* obj, char* name)
{
    obj->mtl_group = NULL;
    strcpy(obj->id_name, name);
}

void buff_push_back_float(sol_model_t* model, unsigned int att_id, float f)
{
    float* tmp;
    float** buff = &(model->v[att_id]);
    size_t* ptr = &(model->v_ptr[att_id]);
    size_t* max_size = &(model->v_max_size[att_id]);

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

int get_line_data(char* line, float *f, int max_size)
{
    int i = 0;
    while (*line != '\0' && i < max_size)
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
    ft_lstclear(&(obj->mtl_group), (void (*)(void *))free_mlt_group);
    free(obj);
}

void sol_destroy(sol_model_t* model)
{
    for (unsigned int att_id = 0; att_id < SCOP_MAX_ATT; att_id++)
    {
        if (model->v[att_id])
            free(model->v[att_id]);
    }
    ft_lstclear(&(model->obj), (void (*)(void *))free_obj);
    free(model);
}
