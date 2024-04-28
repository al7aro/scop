#include "basic_mesh.h"

void mesh_destroy(mesh_t* mesh)
{
    free(mesh->data->raw);
    free(mesh->data->idx);
    free(mesh->data);
    free(mesh);
}

mesh_t* mesh_load_cube(mesh_t** ret)
{
    mesh_t* mesh;
    float square_data[4 * 6] =
    {
        -0.5, -0.5, 0.0,	1.0, 0.0, 0.0,
        0.5, -0.5, 0.0,		0.0, 1.0, 0.0,
        0.5, 0.5, 0.0,		0.0, 0.0, 1.0,
        -0.5, 0.5, 0.0,		1.0, 1.0, 1.0
    };
    unsigned int square_idx[6] =
    {
        0, 1, 3,
        1, 2, 3
    };

    /* Mesh Setup */
    mesh = (mesh_t*)malloc(sizeof(mesh_t));
    mesh->data = (vertex_data_t*)malloc(sizeof(vertex_data_t));
    mesh->data->vertex_cnt = 4; /* Square vertices*/
    mesh->data->raw_size = sizeof(square_data);
    mesh->data->idx_size = sizeof(square_idx);
    mesh->data->idx_cnt = 6;
    mesh->data->att_stride = sizeof(float) * 6;
    mesh->data->raw = (unsigned char*)malloc(sizeof(square_data));
    mesh->data->idx = (unsigned int*)malloc(sizeof(square_idx));

    memcpy(mesh->data->raw, square_data, sizeof(square_data));
    memcpy(mesh->data->idx, square_idx, sizeof(square_idx));

    /* Attribute Format Setup */
    memset(mesh->data->att_format, 0, sizeof(mesh->data->att_format));
    mesh->data->att_format[SCOP_POS_ATTRIBUTE_ID] = VERTEX_SET_ATTRIB_FORMAT((uint64_t)GL_FLOAT, (uint64_t)3, 0, (uint64_t)GL_FALSE);
    mesh->data->att_format[SCOP_COL_ATTRIBUTE_ID] = VERTEX_SET_ATTRIB_FORMAT((uint64_t)GL_FLOAT, (uint64_t)3, (uint64_t)(3 * sizeof(float)), (uint64_t)GL_FALSE);

    /* Mesh Load to GPU */
    glGenVertexArrays(1, mesh->VAO);
    glBindVertexArray(*(mesh->VAO));

    glGenBuffers(1, mesh->VBO);
    glGenBuffers(1, mesh->EBO);

    glBindBuffer(GL_ARRAY_BUFFER, *(mesh->VBO));
    glBufferData(GL_ARRAY_BUFFER, mesh->data->raw_size, mesh->data->raw, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(mesh->EBO));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->data->idx_size, mesh->data->idx, GL_STATIC_DRAW);

    for (size_t i = 0; i < 16; i++)
    {
        if (mesh->data->att_format[i])
        {
            glVertexAttribPointer(i,
                (GLint)VERTEX_GET_ATTRIB_COUNT(mesh->data->att_format[i]),
                (GLenum)VERTEX_GET_ATTRIB_TYPE(mesh->data->att_format[i]),
                (GLboolean)VERTEX_GET_ATTRIB_NORMALICED(mesh->data->att_format[i]),
                (GLsizei)mesh->data->att_stride,
                (const void*)VERTEX_GET_ATTRIB_OFFSET(mesh->data->att_format[i]));
            glEnableVertexAttribArray(i);
        }
    }
    *ret = mesh;
    return mesh;
}

void mesh_render(mesh_t* mesh)
{
    glBindVertexArray(*(mesh->VAO));
    glDrawElements(GL_TRIANGLES, mesh->data->idx_cnt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}