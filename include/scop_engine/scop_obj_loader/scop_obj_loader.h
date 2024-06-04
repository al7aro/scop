#ifndef SCOP_OBJ_LOADER_H
#define SCOP_OBJ_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "libft_ll.h"
#include "scop_macros.h"

typedef struct sol_att_s
{
	unsigned int data[MAX_FACE_SIZE];
} sol_att_t;

typedef struct sol_face_s
{
	sol_att_t att[SCOP_MAX_ATT];

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
	float* v[SCOP_MAX_ATT];
	size_t v_ptr[SCOP_MAX_ATT];
	size_t v_cnt[SCOP_MAX_ATT];
	size_t v_max_size[SCOP_MAX_ATT];

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
