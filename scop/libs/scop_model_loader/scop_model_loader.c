#define SCOP_MODEL_LOADER_INTERNAL_FUNCTIONALITY
#include "scop_model_loader.h"

/*TODO: THINK ABOUT USING STRTOD*/
static void parse_line(loader_obj_data_t* obj, char* line)
{
    char* line_type = strtok_r(line, " ", &line);
    char* line_data = strtok_r(NULL, "#\n", &line); (void)line_data;
    if (line_data)
        trim_spaces(line_data);

    /* VERTEX ATTRIBUTES */
    if ('v' == *line_type)
    {
        float f[16];
        float** data = NULL; size_t* cnt = NULL; size_t* ptr = NULL; size_t* max_size = NULL;
        switch (*(line_type + 1))
        {
            case '\0':  /* v */
                data = &(obj->v); cnt = &(obj->v_cnt); ptr = &(obj->v_ptr); max_size = &(obj->v_max_size);
                break;
            case 'n':   /* vn */
                data = &(obj->vn); cnt = &(obj->vn_cnt); ptr = &(obj->vn_ptr); max_size = &(obj->vn_max_size);
                break;
            case 't':   /* vt */
                data = &(obj->vt); cnt = &(obj->vt_cnt); ptr = &(obj->vt_ptr); max_size = &(obj->vt_max_size);
                break;
            case 'p':   /* vp */
                data = &(obj->vp); cnt = &(obj->vp_cnt); ptr = &(obj->vp_ptr); max_size = &(obj->vp_max_size);
                break;
            default:
                printf("unrecogniced\n");
                break;
        }
        int ret = get_line_data(line_data, f);
        if (cnt)
        {
            if (!*cnt) *cnt = ret;
            for (size_t i = 0; i < *cnt; i++)
                buff_push_back(data, ptr, max_size, ((size_t)ret >= (i + 1)) ? f[i] : 1.0f);
        }
    }
    else if (!strncmp("f\0", line_type, 2))
    {
        if (NULL == obj->data)
        {
            obj->data_max_size = obj->v_ptr + obj->vn_ptr + obj->vt_ptr + obj->vp_ptr;
            obj->data = (float*)malloc(sizeof(float) * obj->data_max_size);
            if (obj->data)
                memset(obj->data, 0, sizeof(float)  * obj->data_max_size);
        }
        printf("Creating faces\n");
    }
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
    loader_obj_data_t** scene = NULL;
    int data_ptr = -1;
    char name[64];
    memset(name, 0, sizeof(name));
    while (fgets(line, MAX_LINE_SIZE, fp))
    {
        char aux[64];
        memset(name, 0, sizeof(name));
        if (sscanf_s(line, " o %s ", aux, 64) > 0)
        {
            memcpy(name, aux, sizeof(char) * 64);
            printf("Working on object: %s\n", name);
        }
        if (*name)
        {
            loader_obj_data_t* obj = get_obj_by_id(&scene, &data_ptr, name);
            parse_line(obj, line);
        }
    }
    printf("Scene Size: %d\n", data_ptr + 1);
    for (int i = 0; i < data_ptr + 1; i++)
    {
        free_obj_data(scene[i]);
        free(scene[i]);
    }
    free(scene);
}
