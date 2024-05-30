#ifndef SCOP_OBJ_LOADER_H
#define SCOP_OBJ_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "libft_ll.h"

#ifdef _WIN32
	#define strtok_r strtok_s
#endif

#define MAX_LINE_SIZE 512

#define V_ATT_ID 0	/* POSITION */
#define VT_ATT_ID 1	/* TEXTURE */
#define VN_ATT_ID 2	/* NORMAL */
#define VP_ATT_ID 3	/* (no se) */
#define VC_ATT_ID 4	/* COLOR */

#define MAX_ATT_ID 5
#define MAX_FACE_SIZE 64

typedef struct sol_att_s
{
	unsigned int data[MAX_FACE_SIZE];
} sol_att_t;

/* MAKE THIS MORE GENERAL (LOL) */
typedef struct sol_face_s
{
	sol_att_t att[MAX_ATT_ID];

	unsigned int size;
} sol_face_t;

typedef struct sol_mtl_group_s
{
	char usemtl[64];
	t_list* faces;		/*Content -> sol_obj_face*/

	float Ns;
	float Ka[3];
	float Kd[3];
	float Ks[3];
	float Ke[3];
	float Ni;
	float d;
	int illum;

	char map_Ks[128];
	char map_Kd[128];
	char map_Bump[128];
} sol_mtl_group_t;

typedef struct sol_obj_s
{ 
	char id_name[64];
	t_list* mtl_group;	/*Content -> sol_mtl_group*/
} sol_obj_t;

typedef struct sol_model_s
{
	float* v[MAX_ATT_ID];
	size_t v_ptr[MAX_ATT_ID];
	size_t v_cnt[MAX_ATT_ID];
	size_t v_max_size[MAX_ATT_ID];

	char mtllib[64];
	t_list* obj;
} sol_model_t;

sol_model_t* sol_load_wavefront_obj(const char* path);
void sol_destroy(sol_model_t* scene);


# ifdef SCOP_MODEL_LOADER_INTERNAL_FUNCTIONALITY

void sol_load_wavefront_mtl(sol_model_t* model, const char* obj_path);

void init_mtl_group(sol_mtl_group_t* mtl, const char* mtl_name);
void init_obj(sol_obj_t* obj, char* name);
void init_face(sol_face_t* obj);
void init_model(sol_model_t* model);

void buff_push_back_float(sol_model_t* obj, unsigned int att_id, float f);
void trim_spaces(char* str);
int get_line_data(char* line, float* f, int max_size);

# endif

#endif
