#include "basic_mesh.h"

void mesh_destroy(mesh_t* mesh)
{
    if (!mesh) return;
    free(mesh->data->buff);
    free(mesh->data);
    for (int i = 0; i < SCOP_TEXTURE_ID_MAX; i++)
        if (mesh->mtl->textures[i].data)
            free(mesh->mtl->textures[i].data);
    free(mesh->mtl);
    free(mesh);
}

void mesh_init(mesh_t* mesh)
{
    mesh->mtl = (mtl_t*)malloc(sizeof(mtl_t));
    if (!mesh->mtl) return;
    mesh->data = (vertex_data_t*)malloc(sizeof(vertex_data_t));
    if (mesh->data)
    {
        mesh->data->buff = NULL;
        mesh->data->buff_size = 0;
        mesh->data->buff_max_size = 0;
        mesh->data->buff_cnt = 0;
        for (int i = 0; i < SCOP_MAX_ATT; i++)
            mesh->data->att_format[i].enabled = 0;
    }
    for (int i = 0; i < SCOP_TEXTURE_ID_MAX; i++)
    {
        memset(mesh->mtl->textures[i].path, 0, sizeof(mesh->mtl->textures[i].path));
        mesh->mtl->textures[i].data = NULL;
        mesh->mtl->textures[i].w = 0;
        mesh->mtl->textures[i].h = 0;
        mesh->mtl->textures[i].chn = 0;
        mesh->mtl->textures[i].id = 0;
    }
    strcpy(mesh->mtl->textures[SCOP_TEXTURE_ID_DEFAULT].path, SCOP_INSTALL_PATH"/assets/defaults/unicorns.pam");
}

void mesh_load_GPU(mesh_t* mesh)
{
    /* Mesh Load to GPU */
    glGenVertexArrays(1, mesh->VAO);
    glBindVertexArray(*(mesh->VAO));

    glGenBuffers(1, mesh->VBO);

    glBindBuffer(GL_ARRAY_BUFFER, *(mesh->VBO));
    glBufferData(GL_ARRAY_BUFFER, mesh->data->buff_bytes, mesh->data->buff, GL_STATIC_DRAW);

    for (int i = 0; i < 8; i++)
    {
        if (mesh->data->att_format[i].enabled)
        {
            glVertexAttribPointer(i,
                mesh->data->att_format[i].count,
                mesh->data->att_format[i].type,
                mesh->data->att_format[i].normaliced,
                mesh->data->att_format[i].stride,
                mesh->data->att_format[i].offset);
            glEnableVertexAttribArray(i);
        }
    }

    /* Mesh Load Textures*/
    for (int i = 0; i < SCOP_TEXTURE_ID_MAX; i++)
    {
        if (!mesh->mtl->textures[i].data) continue;
        glGenTextures(1, &(mesh->mtl->textures[i].id));

        glActiveTexture(GL_TEXTURE0 + i);

        glBindTexture(GL_TEXTURE_2D, mesh->mtl->textures[i].id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width = mesh->mtl->textures[i].w;
        int height = mesh->mtl->textures[i].h;
        int chn = mesh->mtl->textures[i].chn;
        unsigned char* data = mesh->mtl->textures[i].data;
        if (chn == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void mesh_push_att(mesh_t* mesh, float* f, unsigned int n) /* n is MAX 16 */
{
    float* tmp;
    if (mesh->data->buff_size + n >= mesh->data->buff_max_size)
    {
        mesh->data->buff_max_size *= 2;
        if (!mesh->data->buff && !mesh->data->buff_max_size)
        {
            mesh->data->buff_max_size = 16;
            tmp = (float*)malloc(sizeof(float) * mesh->data->buff_max_size);
        }
        else
            tmp = realloc(mesh->data->buff, sizeof(float) * mesh->data->buff_max_size);
        if (tmp)
            mesh->data->buff = tmp;
        else
            return;
    }
    for (size_t i = 0; i < n; i++)
        *(mesh->data->buff + mesh->data->buff_size + i) = f[i];
    mesh->data->buff_size += n;
}

/*
#define SCOP_POS_ATT_ID 0
#define SCOP_TEX_ATT_ID 1
#define SCOP_NOR_ATT_ID 2
#define SCOP_PTO_ATT_ID 3
#define SCOP_COL_ATT_ID 3
#define SCOP_MAX_ATT 4
*/
void  mesh_set_format(mesh_t* mesh, sol_model_t* model)
{
    unsigned int stride = 0;
    for (unsigned int att_id = 0; att_id < SCOP_MAX_ATT; att_id++)
        stride += (unsigned int)model->v_cnt[att_id];
    stride *= sizeof(float);
    
    unsigned int offset = 0;
    for (unsigned int att_id = 0; att_id < SCOP_MAX_ATT; att_id++)
    {
        mesh->data->att_format[att_id].stride = stride;
        mesh->data->att_format[att_id].type = GL_FLOAT;
        mesh->data->att_format[att_id].normaliced = GL_FALSE;
        mesh->data->att_format[att_id].count = (char)model->v_cnt[att_id];
        mesh->data->att_format[att_id].enabled = !!(model->v_cnt[att_id]);
        if (model->v_cnt[att_id])
            mesh->data->att_format[att_id].offset = (const void*)(offset * sizeof(float));
        offset += (unsigned int)model->v_cnt[att_id];
    }
    mesh->data->buff_bytes = sizeof(float) * mesh->data->buff_size;
}

void mesh_render(mesh_t* mesh, shader_t* sh)
{
    glBindVertexArray(*(mesh->VAO));

    if (sh)
    {
        shader_use(sh);
        shader_set_vec3(sh, "mat.Ka", mesh->mtl->Ka);
        shader_set_vec3(sh, "mat.Kd", mesh->mtl->Kd);
        shader_set_vec3(sh, "mat.Ks", mesh->mtl->Ks);
        shader_set_vec3(sh, "mat.Ke", mesh->mtl->Ke);
        shader_set_float(sh, "mat.Ns", mesh->mtl->Ns);
        shader_set_float(sh, "mat.Ni", mesh->mtl->Ni);
        shader_set_float(sh, "mat.d", mesh->mtl->d);
        shader_set_float(sh, "vertex_cnt", (float)mesh->data->buff_cnt);

        /* HANDLE TEXTURES */
        for (int i = 0; i < SCOP_TEXTURE_ID_MAX; i++)
        {
            char tex_name[64]; memset(tex_name, 0, sizeof(tex_name));
            char enabled_tex_name[64]; memset(enabled_tex_name, 0, sizeof(enabled_tex_name));
            if (SCOP_TEXTURE_ID_KD == i)
            {
                strcpy(tex_name, "mat.diffuse_map");
                strcpy(enabled_tex_name, "mat.diffuse_map_enabled");
            }
            if (SCOP_TEXTURE_ID_KS == i)
            {
                strcpy(tex_name, "mat.specular_map");
                strcpy(enabled_tex_name, "mat.specular_map_enabled");
            }
            if (SCOP_TEXTURE_ID_BUMP == i)
            {
                strcpy(tex_name, "mat.bump_map");
                strcpy(enabled_tex_name, "mat.bump_map_enabled");
            }
            if (SCOP_TEXTURE_ID_DEFAULT == i)
            {
                strcpy(tex_name, "mat.default_map");
                strcpy(enabled_tex_name, "mat.default_map_enabled");
            }
            
            shader_set_int(sh, enabled_tex_name, !!(mesh->mtl->textures[i].id));
            shader_set_int(sh, tex_name, i);

            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mesh->mtl->textures[i].id);
        }
    }
    glDrawArrays(GL_TRIANGLES, 0, mesh->data->buff_cnt);
}

void mesh_set_mtl(mesh_t* mesh, sol_mtl_group_t* sol_mtl_group)
{
    memcpy(mesh->mtl->Ka, sol_mtl_group->Ka, sizeof(float) * 3);
    memcpy(mesh->mtl->Kd, sol_mtl_group->Kd, sizeof(float) * 3);
    memcpy(mesh->mtl->Ks, sol_mtl_group->Ks, sizeof(float) * 3);
    memcpy(mesh->mtl->Ke, sol_mtl_group->Ke, sizeof(float) * 3);
    mesh->mtl->Ns = sol_mtl_group->Ns;
    mesh->mtl->Ni = sol_mtl_group->Ni;
    mesh->mtl->d = sol_mtl_group->d;

    /* Load texture data */
    memcpy(mesh->mtl->textures[SCOP_TEXTURE_ID_KS].path, sol_mtl_group->map_Ks, sizeof(sol_mtl_group->map_Ks));
    memcpy(mesh->mtl->textures[SCOP_TEXTURE_ID_KD].path, sol_mtl_group->map_Kd, sizeof(sol_mtl_group->map_Kd));
    memcpy(mesh->mtl->textures[SCOP_TEXTURE_ID_BUMP].path, sol_mtl_group->map_Bump, sizeof(sol_mtl_group->map_Bump));
    for (int i = 0; i < SCOP_TEXTURE_ID_MAX; i++)
    {
        unsigned char* data;

        if (!mesh->mtl->textures[i].path[i]) continue;
        data = scop_image_load(mesh->mtl->textures[i].path,
            &(mesh->mtl->textures[i].w),
            &(mesh->mtl->textures[i].h),
            &(mesh->mtl->textures[i].chn));
        if (data)
            mesh->mtl->textures[i].data = data;
    }
}
