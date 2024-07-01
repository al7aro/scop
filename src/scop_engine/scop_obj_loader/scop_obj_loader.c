#define SCOP_MODEL_LOADER_INTERNAL_FUNCTIONALITY
#include "scop_obj_loader.h"

static void parse_line_face(sol_obj_t* obj, char** line)
{
    char* token = strtok_r(NULL, " ", line);
    sol_face_t *face = (sol_face_t*)malloc(sizeof(sol_face_t));
    init_face(face);
    for (; token && face->size < MAX_FACE_SIZE; token = strtok_r(NULL, " ", line), face->size += 1)
    {
        if (isdigit(*token))
        {
            face->att[SCOP_POS_ATT_ID].data[face->size] = atoi(token);
            face->att[SCOP_COL_ATT_ID].data[face->size] = atoi(token);
        }
        token = strchr(token, '/');
        if (!token++) continue;
        if (isdigit(*token))
            face->att[SCOP_TEX_ATT_ID].data[face->size] = atoi(token);
        token = strchr(token, '/');
        if (!token++) continue;
        if (isdigit(*token))
            face->att[SCOP_NOR_ATT_ID].data[face->size] = atoi(token);
        token = strchr(token, '/');
        if (!token++) continue;
        if (isdigit(*token))
            face->att[SCOP_PTO_ATT_ID].data[face->size] = atoi(token);
    }
    if (!(obj->mtl_group))
    {
        sol_mtl_group_t* mtl = (sol_mtl_group_t*)malloc(sizeof(sol_mtl_group_t));
        init_mtl_group(mtl, "nomaterial");
        ft_lstadd_back(&(obj->mtl_group), ft_lstnew(mtl));
    }
    if (obj->mtl_group)
    {
        sol_mtl_group_t* current_mtl_group = (sol_mtl_group_t*)(ft_lstlast(obj->mtl_group)->content);
        ft_lstadd_back(&(current_mtl_group->faces), ft_lstnew(face));
    }
}

/*TODO: THINK ABOUT USING STRTOD*/
static void parse_obj_line(sol_model_t* model, sol_obj_t* obj, char* line)
{
    char* line_data = NULL;
    char* line_type = strtok_r(line, " #\n\t\v\f\r", &line_data);
    if (!line_type) return;
    unsigned int att_id = 0;

    /* VERTEX ATTRIBUTES */
    if ('v' == *line_type && line_data)
    {
        trim_spaces(line_data);
        float f[16];
        switch (*(line_type + 1))
        {
            case '\0':  /* v */
                att_id = SCOP_POS_ATT_ID;
                break;
            case 't':   /* vt */
                att_id = SCOP_TEX_ATT_ID;
                break;
            case 'n':   /* vn */
                att_id = SCOP_NOR_ATT_ID;
                break;
            case 'p':   /* vp */
                att_id = SCOP_PTO_ATT_ID;
                break;
            default:
                printf("unrecogniced\n");
                break;
        }
        int ret = get_line_data(line_data, f, 16);
        if (ret)
        {
            if (!model->v_cnt[att_id]) model->v_cnt[att_id] = ret;
            if (att_id == SCOP_POS_ATT_ID && ret == 6)
            {
                model->v_cnt[SCOP_POS_ATT_ID] = 3;
                model->v_cnt[SCOP_COL_ATT_ID] = 3;
                for (size_t i = 0; i < 3; i++)
                    buff_push_back_float(model, SCOP_POS_ATT_ID, f[i]);
                for (size_t i = 0; i < 3; i++)
                    buff_push_back_float(model, SCOP_COL_ATT_ID, f[i + 3]);
            }
            else
                for (size_t i = 0; i < model->v_cnt[att_id]; i++)
                    buff_push_back_float(model, att_id, ((size_t)ret >= (i + 1)) ? f[i] : 1.0f);
        }
    }
    else if (!strncmp("f\0", line_type, 2) && line_data)
    {
        if (!obj)
        {
            obj = (sol_obj_t*)malloc(sizeof(sol_obj_t));
            init_obj(obj, "noname");
            ft_lstadd_back(&(model->obj), ft_lstnew(obj));
        }
        trim_spaces(line_data);
        parse_line_face(obj, &line_data);
    }
}

sol_model_t* sol_load_wavefront_obj(const char* path)
{
    FILE* fp = fopen(path, "rb");
    if (!fp)
    {
        printf("File not found [%s]\n", path);
        return NULL;
    }
    char line[512]; memset(line, 0, sizeof(line));
    sol_model_t* model = (sol_model_t*)malloc(sizeof(sol_model_t));
    if (!model) return NULL;
    init_model(model);
    while (fgets(line, MAX_LINE_SIZE, fp))
    {
        char name[64]; memset(name, 0, sizeof(name));
        char mtl_name[64]; memset(mtl_name, 0, sizeof(mtl_name));
        char mtllib[64]; memset(mtllib, 0, sizeof(mtllib));
        if (sscanf(line, " mtllib %s ", mtllib) > 0)
            strcpy(model->mtllib, mtllib);
        if (sscanf(line, " usemtl %s ", mtl_name) > 0)
        {
            if (!model->obj)
            {
                sol_obj_t* obj = (sol_obj_t*)malloc(sizeof(sol_obj_t));
                init_obj(obj, "noname");
                ft_lstadd_back(&(model->obj), ft_lstnew(obj));
            }
            sol_mtl_group_t* mtl = (sol_mtl_group_t*)malloc(sizeof(sol_mtl_group_t));
            init_mtl_group(mtl, mtl_name);
            ft_lstadd_back(&(((sol_obj_t*)ft_lstlast(model->obj)->content)->mtl_group), ft_lstnew(mtl));
        }
        if (sscanf(line, " o %s ", name) > 0)
        {
            sol_obj_t* obj = (sol_obj_t*)malloc(sizeof(sol_obj_t));
            init_obj(obj, name);
            ft_lstadd_back(&(model->obj), ft_lstnew(obj));
        }
        t_list* tmp = ft_lstlast(model->obj);
        if (tmp)
            parse_obj_line(model, tmp->content, line);
        else
            parse_obj_line(model, NULL, line);
    }
    if (strlen(model->mtllib) > 0)
        sol_load_wavefront_mtl(model, path);
    return model;
}
