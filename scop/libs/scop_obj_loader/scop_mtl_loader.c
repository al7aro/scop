#define SCOP_MODEL_LOADER_INTERNAL_FUNCTIONALITY
#include "scop_obj_loader.h"

static void parse_mtl_line(sol_mtl_group_t* mtl, char* line)
{
    char* line_data = NULL;
    char* line_type = strtok_r(line, " #\n\t\v\f\r", &line_data);
    if (!line_type) return;
    unsigned int att_id = 0;

    /* VERTEX ATTRIBUTES */
    if ('K' == *line_type && line_data)
    {
        trim_spaces(line_data);
        float f[3];
        int ret = get_line_data(line_data, f, 3);
        switch (*(line_type + 1))
        {
            case 'a':  /* Ka */
                for (int i = 0; i < ret; i++) mtl->Ka[i] = f[i];
                break;
            case 'd':   /* Kd */
                for (int i = 0; i < ret; i++) mtl->Kd[i] = f[i];
                break;
            case 's':   /* Ks */
                for (int i = 0; i < ret; i++) mtl->Ks[i] = f[i];
                break;
            case 'e':   /* Ke */
                for (int i = 0; i < ret; i++) mtl->Ke[i] = f[i];
                break;
            default:
                printf("unrecogniced\n");
                break;
        }
    }
    else if ('N' == *line_type && line_data)
    {
        trim_spaces(line_data);
        float f;
        int ret = get_line_data(line_data, &f, 1);
        switch (*(line_type + 1))
        {
            case 's':  /* Ns */
                if (ret) mtl->Ns = f;
                break;
            case 'i':   /* Ni */
                if (ret) mtl->Ni = f;
                att_id = VT_ATT_ID;
                break;
            default:
                printf("unrecogniced\n");
                break;
        }
    }
    else if ('d' == *line_type && line_data)
    {
        trim_spaces(line_data);
        float f;
        int ret = get_line_data(line_data, &f, 1);
        if (ret) mtl->d = f;
    }
}

char* get_mtl_path(char* obj_path, char* mtllib)
{
    size_t dir_end = strrchr(obj_path, '/') - obj_path + 1;
    if (dir_end < 0 && dir_end >= strlen(obj_path))
        return NULL;
    obj_path[dir_end] = 0;
    size_t path_len = strlen(obj_path) + strlen(mtllib) + 1;
    char* path = (char*)malloc(sizeof(char) * path_len);
    if (!path)
        return NULL;
    memset(path, 0, sizeof(char) * (strlen(obj_path) + strlen(mtllib)));
    strcpy_s(path, path_len, obj_path);
    strcat_s(path, path_len, mtllib);
    return path;
}

sol_mtl_group_t* get_mtl_by_name(sol_model_t* model, char* name)
{
    t_list* obj_lst = model->obj;
    while (obj_lst)
    {
        sol_obj_t* obj = obj_lst->content;
        t_list* mtl_lst = obj->mtl_group;
        while (mtl_lst)
        {
            sol_mtl_group_t* mtl = mtl_lst->content;
            if (!strcmp(mtl->usemtl, name))
                return mtl;
            mtl_lst = mtl_lst->next;
        }
        obj_lst = obj_lst->next;
    }
    return NULL;
}

void sol_load_wavefront_mtl(sol_model_t* model, const char* obj_path)
{
    char* path = get_mtl_path((char*)obj_path, model->mtllib);
    FILE* fp = fopen(path, "rb");
    if (!fp)
    {
        printf("File not found.\n");
        return ;
    }
    char line[512]; memset(line, 0, sizeof(line));
    sol_mtl_group_t* active_mtl = NULL;
    while (fgets(line, MAX_LINE_SIZE, fp))
    {
        char newmtl[64];
        if (sscanf_s(line, " newmtl %s ", newmtl, 64) > 0)
            active_mtl = get_mtl_by_name(model, newmtl);
        else
        {
            if (!active_mtl)
                continue;
            parse_mtl_line(active_mtl, line);
        }
    }
}