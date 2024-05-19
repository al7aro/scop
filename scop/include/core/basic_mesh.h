#ifndef RENDERER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "glad/glad.h"

#define SCOP_POS_ATTRIBUTE_ID 0
#define SCOP_NOR_ATTRIBUTE_ID 1
#define SCOP_TEX_ATTRIBUTE_ID 2
#define SCOP_COL_ATTRIBUTE_ID 3

typedef struct attrib_format_s
{
    GLenum type;
    char count;
    const void* offset;
    char normaliced;
    GLsizei stride;
} attrib_format_t;

/**
 * @brief Contains all vertex data and its format
 * 
 */
typedef struct vertex_data_s
{
    /* TODO: how should I store raw vertex data in a TYPE-INDEPENDENT way*/
    unsigned char* vertex;         /** Contains the raw data of vertices. */
    unsigned int vertex_size;      /** Contains the total byte size of 'vert' */
    unsigned int* idx;          /** Contains the indices in which vertices are used. */
    unsigned int idx_size;      /** Contains the total byte size of 'idx' */
    unsigned int idx_cnt;       /** Contains the number of indeices */

    unsigned int vertex_cnt;    /** Contains the number of vertices. */

    attrib_format_t att_format[16];    /** Space for 64 possible attributes. Each uint64 stores (using 2bytes per each) TYPE, SIZE, OFFSET, NORMALICED */
} vertex_data_t;

/**
 * @brief Contains vertex data and reference to the used VAO, VBO and EBO
 * 
 */
typedef struct mesh_s
{
    unsigned int VAO[1];
    unsigned int VBO[1];
    unsigned int EBO[1];
    vertex_data_t* data;    /* TODO: should I check the ownership of data? */
} mesh_t;

mesh_t* mesh_load_cube(mesh_t** ret);
void mesh_render(mesh_t* mesh);
void mesh_destroy(mesh_t* mesh);

#endif
