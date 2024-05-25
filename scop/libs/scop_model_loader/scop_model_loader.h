#ifndef SCOP_MODEL_LOADER_H
#define SCOP_MODEL_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "libft_ll.h"

#ifdef _WIN32
	#define strtok_r strtok_s
#endif

#define MAX_LINE_SIZE 512

#define V_ATT_ID 0
#define VN_ATT_ID 1
#define VT_ATT_ID 2
#define VP_ATT_ID 3
#define MAX_ATT_ID 4

#define MAX_FACE_SIZE 64

typedef struct loader_obj_face_s
{
	unsigned int v_idx[MAX_FACE_SIZE];
	unsigned int vn_idx[MAX_FACE_SIZE];
	unsigned int vt_idx[MAX_FACE_SIZE];
	unsigned int vp_idx[MAX_FACE_SIZE];
	unsigned int size;
} loader_obj_face_t;

typedef struct loader_obj_data_s
{ 
	float* v; int v_id; size_t v_ptr, v_cnt, v_max_size;
	float* vn; int vn_id; size_t vn_ptr, vn_cnt, vn_max_size;
	float* vt; int vt_id; size_t vt_ptr, vt_cnt, vt_max_size;
	float* vp; int vp_id; size_t vp_ptr, vp_cnt, vp_max_size;
	int att_id_cnt;

	char id_name[64];
	loader_obj_face_t* faces;
	size_t faces_ptr, faces_max_size;
} loader_obj_data_t;

loader_obj_data_t** sml_load_wavefront_obj(const char* path);
void sml_destroy(loader_obj_data_t** scene);

# ifdef SCOP_MODEL_LOADER_INTERNAL_FUNCTIONALITY

void init_obj_data(loader_obj_data_t* obj, char* name);
void init_obj_face(loader_obj_face_t* obj);
void free_obj_data(loader_obj_data_t* obj);
loader_obj_data_t* get_obj_by_id(loader_obj_data_t*** scene, int *size, char* name);
void buff_push_back_float(float** buff, size_t* ptr, size_t* max_size, float f);
void buff_push_back_faces(loader_obj_face_t**buff, size_t *ptr, size_t *max_size, loader_obj_face_t f);
void trim_spaces(char* str);
int get_line_data(char* line, float f[16]);

# endif

#endif
