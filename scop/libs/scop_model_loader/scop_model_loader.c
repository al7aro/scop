#define SCOP_MODEL_LOADER_INTERNAL_FUNCTIONALITY
#include "scop_model_loader.h"

void test_print_face(sml_face_t f)
{
    for (unsigned int i = 0; i < f.size; i++)
        printf("%d/%d/%d ",f.v_idx[i], f.vn_idx[i], f.vt_idx[i]);
    printf("\n");
}

static void parse_line_face(sml_obj_t* obj, char** line)
{
    char* token = strtok_r(NULL, " ", line);
    sml_face_t *face = (sml_face_t*)malloc(sizeof(sml_face_t));
    init_face(face);
    for (; token && face->size < MAX_FACE_SIZE; token = strtok_r(NULL, " ", line), face->size += 1)
    {
        char* aux = strdup(token);
        char* aux_save = NULL;
        token = strtok_r(aux, "/", &aux_save);
        for (size_t att_id = 0; token && att_id < MAX_ATT_ID; token = strtok_r(NULL, "/", &aux_save), att_id++)
        {
            if (att_id == obj->v_id)
                face->v_idx[face->size] = atoi(token);
            else if (att_id == obj->vn_id)
                face->vn_idx[face->size] = atoi(token);
            else if (att_id == obj->vt_id)
                face->vt_idx[face->size] = atoi(token);
            else if (att_id == obj->vp_id)
                face->vp_idx[face->size] = atoi(token);
        }
        free(aux);
    }
    if (obj->mtl_group)
    {
        sml_mtl_group_t* current_mtl_group = (sml_mtl_group_t*)(ft_lstlast(obj->mtl_group)->content);
        ft_lstadd_back(&(current_mtl_group->faces), ft_lstnew(face));
    }
}

/*TODO: THINK ABOUT USING STRTOD*/
static void parse_line(sml_obj_t* obj, char* line)
{

    char* line_type = strtok_r(line, " ", &line);
    char* line_data = strtok_r(NULL, "#\n", &line); (void)line_data;
    if (line_data)
        trim_spaces(line_data);

    /* VERTEX ATTRIBUTES */
    if ('v' == *line_type)
    {
        float f[16];
        float** data = NULL;
        size_t* cnt = NULL; size_t* ptr = NULL; size_t* max_size = NULL;
        switch (*(line_type + 1))
        {
            case '\0':  /* v */
                if (obj->v_id == -1) obj->v_id = ++(obj->att_id_cnt);
                data = &(obj->v); cnt = &(obj->v_cnt); ptr = &(obj->v_ptr); max_size = &(obj->v_max_size);
                break;
            case 'n':   /* vn */
                if (obj->vn_id == -1) obj->vn_id = ++(obj->att_id_cnt);
                data = &(obj->vn); cnt = &(obj->vn_cnt); ptr = &(obj->vn_ptr); max_size = &(obj->vn_max_size);
                break;
            case 't':   /* vt */
                if (obj->vt_id == -1) obj->vt_id = ++(obj->att_id_cnt);
                data = &(obj->vt); cnt = &(obj->vt_cnt); ptr = &(obj->vt_ptr); max_size = &(obj->vt_max_size);
                break;
            case 'p':   /* vp */
                if (obj->vp_id == -1) obj->vp_id = ++(obj->att_id_cnt);
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
                buff_push_back_float(data, ptr, max_size, ((size_t)ret >= (i + 1)) ? f[i] : 1.0f);
        }
    }
    else if (!strncmp("f\0", line_type, 2))
    {
        parse_line_face(obj, &line_data);
    }
}

sml_scene_t* sml_load_wavefront_obj(const char* path)
{
    FILE* fp = fopen(path, "rb");
    if (!fp)
    {
        printf("File not found.\n");
        return NULL;
    }
    char line[512];
    sml_scene_t* scene = (sml_scene_t*)malloc(sizeof(sml_scene_t));
    if (!scene) return NULL;
    init_scene(scene);
    while (fgets(line, MAX_LINE_SIZE, fp))
    {
        char name[64];
        char mtl_name[64];
        if (sscanf_s(line, " usemtl %s ", mtl_name, 64) > 0)
        {
            //printf("Working on material: %s\n", mtl_name);
            sml_mtl_group_t* mtl = (sml_mtl_group_t*)malloc(sizeof(sml_mtl_group_t));
            init_mtl_group(mtl, mtl_name);
            ft_lstadd_back(&(((sml_obj_t*)ft_lstlast(scene->obj)->content)->mtl_group), ft_lstnew(mtl));
        }
        if (sscanf_s(line, " o %s ", name, 64) > 0)
        {
            //printf("Working on object: %s\n", name);
            sml_obj_t* obj = (sml_obj_t*)malloc(sizeof(sml_obj_t));
            init_obj(obj, name);
            ft_lstadd_back(&(scene->obj), ft_lstnew(obj));
        }
        if (scene->obj)
        {
            t_list* tmp = ft_lstlast(scene->obj);
            //printf("Parsing line of object: %s\n", ((sml_obj_t*)(tmp->content))->id_name);
            parse_line(tmp->content, line);
        }
    }
    //printf("LST. SIZE: %d\n", ft_lstsize(scene->obj));
    sml_destroy(scene);
    return scene;
}