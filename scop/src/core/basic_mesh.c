#include "basic_mesh.h"

void mesh_destroy(mesh_t* mesh)
{
    free(mesh->data->vertex);
    free(mesh->data->idx);
    free(mesh->data);
    free(mesh);
}

mesh_t* mesh_load_cube(mesh_t** ret)
{
    mesh_t* mesh;
    /* TODO: there are in fact repeated vertices in this array -> drawElements could be used*/
    float cube_data[] =
    {
        -0.5f, -0.5f, -0.5f, 1.0, 0.0, 0.0,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0, 0.0, 0.0,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0, 0.0, 0.0,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0, 0.0, 0.0,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0, 0.0, 0.0,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0, 0.0, 0.0,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0.0, 1.0, 0.0,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0, 1.0, 0.0,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0, 1.0, 0.0,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0, 1.0, 0.0,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0, 1.0, 0.0,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0, 1.0, 0.0,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 0.0, 0.0, 1.0,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0, 0.0, 1.0,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0, 0.0, 1.0,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0, 0.0, 1.0,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0, 0.0, 1.0,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0, 0.0, 1.0,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 1.0, 0.5, 0.0,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0, 0.5, 0.0,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0, 0.5, 0.0,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0, 0.5, 0.0,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0, 0.5, 0.0,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0, 0.5, 0.0,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.5, 1.0, 0.0,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.5, 1.0, 0.0,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.5, 1.0, 0.0,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.5, 1.0, 0.0,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.5, 1.0, 0.0,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.5, 1.0, 0.0,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 0.0, 0.5, 1.0,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0, 0.5, 1.0,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0, 0.5, 1.0,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0, 0.5, 1.0,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0, 0.5, 1.0,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0, 0.5, 1.0,  0.0f, 1.0f
    };
    unsigned int cube_idx[] =
    {
        0, 1, 3,
        1, 2, 3,
        4, 5, 7,
        5, 6, 7,
        3, 2, 7,
        2, 6, 7,
        0, 1, 4,
        1, 5, 4,
        4, 0, 7,
        0, 3, 7,
        1, 5, 2,
        5, 6, 2
    };

    /* Mesh Setup */
    mesh = (mesh_t*)malloc(sizeof(mesh_t));
    mesh->data = (vertex_data_t*)malloc(sizeof(vertex_data_t));
    mesh->data->vertex_cnt = 6 * 6; /* cube vertices*/
    mesh->data->vertex_size = sizeof(cube_data);
    mesh->data->idx_size = sizeof(cube_idx);
    mesh->data->idx_cnt = sizeof(cube_idx) / sizeof(unsigned int);
    mesh->data->vertex = (unsigned char*)malloc(sizeof(cube_data));
    mesh->data->idx = (unsigned int*)malloc(sizeof(cube_idx));

    memcpy(mesh->data->vertex, cube_data, sizeof(cube_data));
    memcpy(mesh->data->idx, cube_idx, sizeof(cube_idx));

    /* Attribute Format Setup */
    memset(mesh->data->att_format, 0, sizeof(mesh->data->att_format));
    mesh->data->att_format[SCOP_POS_ATTRIBUTE_ID].type = GL_FLOAT;
    mesh->data->att_format[SCOP_POS_ATTRIBUTE_ID].count = 3;
    mesh->data->att_format[SCOP_POS_ATTRIBUTE_ID].offset = 0;
    mesh->data->att_format[SCOP_POS_ATTRIBUTE_ID].normaliced = GL_FALSE;
    mesh->data->att_format[SCOP_POS_ATTRIBUTE_ID].stride = sizeof(float) * 8;

    mesh->data->att_format[SCOP_COL_ATTRIBUTE_ID].type = GL_FLOAT;
    mesh->data->att_format[SCOP_COL_ATTRIBUTE_ID].count = 3;
    mesh->data->att_format[SCOP_COL_ATTRIBUTE_ID].offset = (const void*)(3 * sizeof(float));
    mesh->data->att_format[SCOP_COL_ATTRIBUTE_ID].normaliced = GL_FALSE;
    mesh->data->att_format[SCOP_COL_ATTRIBUTE_ID].stride = sizeof(float) * 8;

    mesh->data->att_format[SCOP_TEX_ATTRIBUTE_ID].type = GL_FLOAT;
    mesh->data->att_format[SCOP_TEX_ATTRIBUTE_ID].count = 2;
    mesh->data->att_format[SCOP_TEX_ATTRIBUTE_ID].offset = (const void*)(6 * sizeof(float));
    mesh->data->att_format[SCOP_TEX_ATTRIBUTE_ID].normaliced = GL_FALSE;
    mesh->data->att_format[SCOP_TEX_ATTRIBUTE_ID].stride = sizeof(float) * 8;

    /* Mesh Load to GPU */
    glGenVertexArrays(1, mesh->VAO);
    glBindVertexArray(*(mesh->VAO));

    glGenBuffers(1, mesh->VBO);
    glGenBuffers(1, mesh->EBO);

    glBindBuffer(GL_ARRAY_BUFFER, *(mesh->VBO));
    glBufferData(GL_ARRAY_BUFFER, mesh->data->vertex_size, mesh->data->vertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(mesh->EBO));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->data->idx_size, mesh->data->idx, GL_STATIC_DRAW);

    for (int i = 0; i < 16; i++)
    {
        if (mesh->data->att_format + i)
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
    *ret = mesh;
    return mesh;
}

void mesh_render(mesh_t* mesh)
{
    glBindVertexArray(*(mesh->VAO));
    // glDrawElements(GL_TRIANGLES, mesh->data->idx_cnt, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, mesh->data->vertex_cnt);
    glBindVertexArray(0);
}
