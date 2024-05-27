#ifndef BASIC_MESH_H
#define BASIC_MESH_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "glad/glad.h"
#include "scop_model_loader.h"

#define SCOP_POS_ATTRIBUTE_ID 0
#define SCOP_TEX_ATTRIBUTE_ID 1
#define SCOP_NOR_ATTRIBUTE_ID 2
#define SCOP_PTO_ATTRIBUTE_ID 3
#define SCOP_COL_ATTRIBUTE_ID 4

#define MESH_MAX_ATT 8

typedef struct attrib_format_s
{
    GLenum type;
    char count;
    const void* offset;
    char normaliced;
    GLsizei stride;

    char enabled;
} attrib_format_t;

/**
 * @brief Contains all vertex data and its format
 * 
 */
typedef struct vertex_data_s
{
    /* TODO: how should I store raw vertex data in a TYPE-INDEPENDENT way*/
    float* buff;                    /** Contains the raw data of vertices. */
    unsigned int buff_cnt;          /* Vertex Cnt */
    unsigned int buff_size;         /* Float Cnt */
    unsigned int buff_max_size;
    unsigned int buff_bytes;        /* Bytes cnt*/

    unsigned int vertex_cnt;    /** Contains the number of vertices. */

    attrib_format_t att_format[8];    /** Space for 64 possible attributes. Each uint64 stores (using 2bytes per each) TYPE, SIZE, OFFSET, NORMALICED */
} vertex_data_t;

/**
 * @brief Contains vertex data and reference to the used VAO, VBO and EBO
 * TODO: mesh should have more info about the material
 */
typedef struct mesh_s
{
    unsigned int VAO[1];
    unsigned int VBO[1];
    char usemtl[64];
    vertex_data_t* data;    /* TODO: should I check the ownership of data? */
} mesh_t;

void mesh_load_GPU(mesh_t* mesh);
void  mesh_set_format(mesh_t* mesh, sml_obj_t* obj);
void mesh_render(mesh_t* mesh);
void mesh_destroy(mesh_t* mesh);
void mesh_init(mesh_t* mesh, const char* usemtl);
void mesh_push_att(mesh_t* mesh, float* f, unsigned int n); /* n is MAX 16 */

#endif
