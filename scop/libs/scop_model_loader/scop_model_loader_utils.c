#include "scop_model_loader.h"

void init_obj_data(loader_obj_data_t* obj, char* name)
{
    obj->v = (float*)malloc(sizeof(float) * 16);
    obj->v_ptr = 0;
    obj->v_cnt = 0;
    obj->v_max_size = 16;

    obj->vn = (float*)malloc(sizeof(float) * 16);
    obj->vn_ptr = 0;
    obj->vn_cnt = 0;
    obj->vn_max_size = 16;

    obj->vt = (float*)malloc(sizeof(float) * 16);
    obj->vt_ptr = 0;
    obj->vt_cnt = 0;
    obj->vt_max_size = 16;

    obj->vp = (float*)malloc(sizeof(float) * 16);
    obj->vp_ptr = 0;
    obj->vp_cnt = 0;
    obj->vp_max_size = 16;

    obj->data = NULL;
    obj->data_ptr = 0;
    obj->data_max_size = 0;

    strcpy(obj->id_name, name);
}

void free_obj_data(loader_obj_data_t* obj)
{
    if (obj->v)
        free(obj->v);
    if (obj->vn)
        free(obj->vn);
    if (obj->vt)
        free(obj->vt);
    if (obj->vp)
        free(obj->vp);
    if (obj->data)
        free(obj->data);
}

loader_obj_data_t* get_obj_by_id(loader_obj_data_t*** scene, int *size, char* name)
{
    for (int i = 0; i <= *size; i++)
    {
        if (!strncmp((*scene)[i]->id_name, name, strlen(name)))
            return ((*scene)[i]);
    }
    *size += 1;
    *scene = (loader_obj_data_t**)realloc(*scene, sizeof(loader_obj_data_t*) * (*size + 1));
    (*scene)[*size] = (loader_obj_data_t*)malloc(sizeof(loader_obj_data_t));
    init_obj_data((*scene)[*size], name);
    return (*scene)[*size];
}

void buff_push_back(float **buff, size_t *ptr, size_t *max_size, float f)
{
    if (*ptr >= *max_size)
    {
        *max_size *= 2;
        *buff = realloc(*buff, sizeof(float) * *max_size);
    }
    *(*buff + *ptr) = f;
    *ptr += 1;
}

void trim_spaces(char* str)
{
    int start = 0, end = strlen(str) - 1;
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
        f[i] = strtod(line, &line);
        i++;
    }
    return (i);
}
