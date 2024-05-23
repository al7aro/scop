#include "scop_model_loader.h"

void init_obj_face(loader_obj_face_t* obj)
{
    memset(obj->v_idx, 0, MAX_FACE_SIZE);
    memset(obj->vn_idx, 0, MAX_FACE_SIZE);
    memset(obj->vt_idx, 0, MAX_FACE_SIZE);
    memset(obj->vp_idx, 0, MAX_FACE_SIZE);

    obj->size = 0;
}

void init_obj_data(loader_obj_data_t* obj, char* name)
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

    obj->faces = NULL;
    obj->faces_ptr = 0;
    obj->faces_max_size = 0;

    strcpy_s(obj->id_name, sizeof(obj->id_name), name);
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
    if (obj->faces)
        free(obj->faces);
}

loader_obj_data_t* get_obj_by_id(loader_obj_data_t*** scene, int *size, char* name)
{
    for (int i = 0; i <= *size; i++)
    {
        if (!strncmp((*scene)[i]->id_name, name, strlen(name)))
            return ((*scene)[i]);
    }
    *size += 1;
    loader_obj_data_t** tmp = (loader_obj_data_t**)realloc(*scene, sizeof(loader_obj_data_t*) * (*size + 2));
    if (tmp)
        *scene = tmp;
    else
        return NULL;
    (*scene)[*size] = (loader_obj_data_t*)malloc(sizeof(loader_obj_data_t));
    (*scene)[*size + 1] = NULL;
    printf("SIZE: %d\n", *size);
    init_obj_data((*scene)[*size], name);
    return (*scene)[*size];
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

void buff_push_back_faces(loader_obj_face_t** buff, size_t* ptr, size_t* max_size, loader_obj_face_t face)
{
    loader_obj_face_t* tmp;
    if (*ptr >= *max_size)
    {
        *max_size *= 2;
        if (!*buff && !*max_size)
        {
            *max_size = 16;
            tmp = (loader_obj_face_t*)malloc(sizeof(loader_obj_face_t) * *max_size);
        }
        else
            tmp = realloc(*buff, sizeof(loader_obj_face_t) * *max_size);
        if (tmp)
            *buff = tmp;
        else
            return;
    }
    *(*buff + *ptr) = face;
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
