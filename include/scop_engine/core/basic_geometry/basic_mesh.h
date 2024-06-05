#ifndef BASIC_MESH_H
#define BASIC_MESH_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "glad/glad.h"
#include "scop_obj_loader.h"
#include "scop_image.h"
#include "shader.h"

typedef struct tex_s
{
    char path[128];
    unsigned char* data;
    int w;
    int h;
    int chn;

    unsigned int id;
} tex_t;

typedef struct mtl_s
{
    float Ns;
    float Ka[3];
    float Kd[3];
    float Ks[3];
    float Ke[3];
    float Ni;
    float d;
    int illum;

    tex_t textures[4]; /* 4TH texture is the default */
} mtl_t;

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
    mtl_t* mtl;
    vertex_data_t* data;    /* TODO: should I check the ownership of data? */
} mesh_t;

void mesh_load_GPU(mesh_t* mesh);
void mesh_set_format(mesh_t* mesh, sol_model_t* model);
void mesh_render(mesh_t* mesh, shader_t* sh);
void mesh_destroy(mesh_t* mesh);
void mesh_init(mesh_t* mesh);
void mesh_push_att(mesh_t* mesh, float* f, unsigned int n); /* n is MAX 16 */
void mesh_set_mtl(mesh_t* mesh, sol_mtl_group_t* sol_mtl_group);

#endif
