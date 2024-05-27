#include "basic_mesh.h"

void mesh_destroy(mesh_t* mesh)
{
    free(mesh->data->buff);
    free(mesh->data);
    free(mesh);
}

void mesh_init(mesh_t* mesh, const char* usemtl)
{
    mesh->data = (vertex_data_t*)malloc(sizeof(vertex_data_t));
    strcpy_s(mesh->usemtl, sizeof(mesh->usemtl), usemtl);
    if (mesh->data)
    {
        mesh->data->buff = NULL;
        mesh->data->buff_size = 0;
        mesh->data->buff_max_size = 0;
        mesh->data->buff_cnt = 0;
        for (int i = 0; i < MESH_MAX_ATT; i++)
            mesh->data->att_format[i].enabled = 0;
    }
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
#define V_ATT_ID 0
#define VT_ATT_ID 1
#define VN_ATT_ID 2
#define VP_ATT_ID 3
#define VC_ATT_ID 3
#define MAX_ATT_ID 4
*/
/* TODO; MAKE THIS MORE GENERAL (LOL) */
void  mesh_set_format(mesh_t* mesh, sol_model_t* model)
{
    unsigned int stride = 0;
    for (unsigned int att_id = 0; att_id < MAX_ATT_ID; att_id++)
        stride += (unsigned int)model->v_cnt[att_id];
    stride *= sizeof(float);
    
    unsigned int offset = 0;
    for (unsigned int att_id = 0; att_id < MAX_ATT_ID; att_id++)
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

void mesh_render(mesh_t* mesh)
{
    glBindVertexArray(*(mesh->VAO));
    // glDrawElements(GL_TRIANGLES, mesh->data->idx_cnt, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, mesh->data->buff_cnt);
    glBindVertexArray(0);
}
