#define SCOP_MODEL_LOADER_INTERNAL_FUNCTIONALITY
#include "scop_obj_loader.h"

char* get_full_path(const char* obj_path, char* mtllib)
{
    char* path_aux = strdup(obj_path);
    if (!mtllib) return NULL;
#if defined(__APPLE__) || defined(__linux__)
    if (*mtllib == '/')
        return strdup(mtllib);
#elif _WIN32
    if (*mtllib)
        if (*(mtllib + 1) == ':')
            return _strdup(mtllib);
#endif

    char* dir_end = strrchr(path_aux, '/');
    if (dir_end) *(dir_end + 1) = '\0';

    size_t path_len = strlen(path_aux) + strlen(mtllib) + 1;
    char* path = (char*)malloc(sizeof(char) * path_len);
    if (!path)
        return NULL;
    memset(path, 0, sizeof(char) * (strlen(path_aux) + strlen(mtllib)));
    strcpy(path, path_aux);
    strcat(path, mtllib);
    free(path_aux);
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

static void parse_mtl_line(sol_mtl_group_t* mtl, char* line)
{
    char* line_data = NULL;
    char* line_type = strtok_r(line, " #\n\t\v\f\r", &line_data);
    if (!line_type) return;

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

void sol_load_wavefront_mtl(sol_model_t* model, const char* obj_path)
{
    char* path = get_full_path((char*)obj_path, model->mtllib);
    if (!path) return;
    FILE* fp = fopen(path, "rb");
    free(path);
    if (!fp)
    {
        printf("File not found [%s]\n", path);
        return ;
    }
    char line[512]; memset(line, 0, sizeof(line));
    sol_mtl_group_t* active_mtl = NULL;
    while (fgets(line, MAX_LINE_SIZE, fp))
    {
        char newmtl[64];
        char map_Kd[128];
        char map_Bump[128];
        char map_Ks[128];

        if (sscanf(line, " newmtl %s ", newmtl) > 0)
            active_mtl = get_mtl_by_name(model, newmtl);
        else if (sscanf(line, " map_Kd %s ", map_Kd) > 0 && active_mtl)
        {
            char* Kd_path = get_full_path((char*)obj_path, map_Kd);
            strcpy(active_mtl->map_Kd, Kd_path);
            free(Kd_path);
        }
        else if (sscanf(line, " map_Ks %s ", map_Ks) > 0 && active_mtl)
        {
            char* Ks_path = get_full_path((char*)obj_path, map_Ks);
            strcpy(active_mtl->map_Ks, Ks_path);
            free(Ks_path);
        }
        else if (sscanf(line, " map_Bump %s ", map_Bump) > 0 && active_mtl)
        {
            char* Bump_path = get_full_path((char*)obj_path, map_Bump);
            strcpy(active_mtl->map_Bump, Bump_path);
            free(Bump_path);
        }
        else
        {
            if (!active_mtl)
                continue;
            parse_mtl_line(active_mtl, line);
        }
    }
}
