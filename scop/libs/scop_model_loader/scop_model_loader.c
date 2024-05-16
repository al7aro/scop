#include "scop_model_loader.h"

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

void parse_line(loader_data_t* data, char* line)
{
    (void)data;
    char* aux = strdup(line);
    char* token = strtok_r(line, " ", &line);

    float f[16];
    if (!strncmp("v\0", token, 2))
    {
        int ret = sscanf_s(aux, "v %f %f %f %f %f %f %f %f", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5], &f[6], &f[7]);
        buff_push_back(&(data->v), &(data->v_ptr), &(data->v_max_size), (ret >= 1) ? f[0] : 1.0f);
        buff_push_back(&(data->v), &(data->v_ptr), &(data->v_max_size), (ret >= 2) ? f[1] : 1.0f);
        buff_push_back(&(data->v), &(data->v_ptr), &(data->v_max_size), (ret >= 3) ? f[2] : 1.0f);
        buff_push_back(&(data->v), &(data->v_ptr), &(data->v_max_size), (ret >= 4) ? f[3] : 1.0f);
        if (ret >= 5)
        {
            buff_push_back(&(data->c), &(data->c_ptr), &(data->c_max_size), (ret >= 5) ? f[4] : 1.0f);
            buff_push_back(&(data->c), &(data->c_ptr), &(data->c_max_size), (ret >= 6) ? f[5] : 1.0f);
            buff_push_back(&(data->c), &(data->c_ptr), &(data->c_max_size), (ret >= 7) ? f[6] : 1.0f);
            buff_push_back(&(data->c), &(data->c_ptr), &(data->c_max_size), (ret >= 8) ? f[7] : 1.0f);
        }
    }
    else if (!strncmp("vn\0", token, 3))
    {
        int ret = sscanf_s(aux, "vn %f %f %f", &f[0], &f[1], &f[2]);
        buff_push_back(&(data->vn), &(data->vn_ptr), &(data->vn_max_size), (ret >= 1) ? f[0] : 1.0f);
        buff_push_back(&(data->vn), &(data->vn_ptr), &(data->vn_max_size), (ret >= 2) ? f[1] : 1.0f);
        buff_push_back(&(data->vn), &(data->vn_ptr), &(data->vn_max_size), (ret >= 3) ? f[2] : 1.0f);
    }
    else if (!strncmp("vt\0", token, 3))
    {
        int ret = sscanf_s(aux, "vt %f %f %f", &f[0], &f[1], &f[2]);
        buff_push_back(&(data->vt), &(data->vt_ptr), &(data->vt_max_size), (ret >= 1) ? f[0] : 1.0f);
        buff_push_back(&(data->vt), &(data->vt_ptr), &(data->vt_max_size), (ret >= 2) ? f[1] : 1.0f);
        if (ret >= 3)
            buff_push_back(&(data->vt), &(data->vt_ptr), &(data->vt_max_size), f[2]);
    }
    else if (!strncmp("vp\0", token, 3))
    {
        int ret = sscanf_s(aux, "vp %f %f %f", &f[0], &f[1], &f[2]);
        buff_push_back(&(data->vp), &(data->vp_ptr), &(data->vp_max_size), (ret >= 1) ? f[0] : 1.0f);
        buff_push_back(&(data->vp), &(data->vp_ptr), &(data->vp_max_size), (ret >= 2) ? f[1] : 1.0f);
        buff_push_back(&(data->vp), &(data->vp_ptr), &(data->vp_max_size), (ret >= 3) ? f[2] : 1.0f);
    }
    else if (!strncmp("f\0", token, 2))
    {
        printf("Creating faces\n");
    }
    free(aux);
}

void init_data(loader_data_t* data, char* name)
{
    data->v = (float*)malloc(sizeof(float) * 16);
    data->v_ptr = 0;
    data->v_max_size = 16;

    data->c = (float*)malloc(sizeof(float) * 16);
    data->c_ptr = 0;
    data->c_max_size = 16;

    data->vn = (float*)malloc(sizeof(float) * 16);
    data->vn_ptr = 0;
    data->vn_max_size = 16;

    data->vt = (float*)malloc(sizeof(float) * 16);
    data->vt_ptr = 0;
    data->vt_max_size = 16;

    data->vp = (float*)malloc(sizeof(float) * 16);
    data->vp_ptr = 0;
    data->vp_max_size = 16;

    strcpy_s(data->id_name, 64, name);
}

void free_data(loader_data_t* data)
{
    free(data->v);
    free(data->c);
    free(data->vn);
    free(data->vt);
    free(data->vp);
}

loader_data_t* get_obj_by_id(loader_data_t*** data, int *size, char* name)
{
    for (int i = 0; i <= *size; i++)
    {
        if (!strncmp((*data)[i]->id_name, name, strlen(name)))
            return ((*data)[i]);
    }
    *size += 1;
    *data = (loader_data_t**)realloc(*data, sizeof(loader_data_t*) * (*size + 1));
    (*data)[*size] = (loader_data_t*)malloc(sizeof(loader_data_t));
    init_data((*data)[*size], name);
    return (*data)[*size];
}

void sml_load_wavefront_obj(const char* path)
{
    FILE* fp = fopen(path, "rb");
    if (!fp)
    {
        printf("File not found.\n");
        return ;
    }
    char line[512];
    loader_data_t** data = NULL;
    int data_ptr = -1;
    char name[64];
    memset(name, 0, sizeof(name));
    while (fgets(line, MAX_LINE_SIZE, fp))
    {
        char aux[64];
        if (sscanf_s(line, " o %s ", aux, 64) > 0)
        {
            strcpy_s(name, sizeof(name), aux);
            printf("Working on object: %s\n", name);
        }
        if (*name)
        {
            loader_data_t* obj = get_obj_by_id(&data, &data_ptr, name);
            parse_line(obj, line);
        }
    }
    printf("Scene Size: %d\n", data_ptr + 1);
    for (int i = 0; i < data_ptr; i++)
    {
        free_data(data[i]);
        free(data[i]);
    }
    free(data);
}