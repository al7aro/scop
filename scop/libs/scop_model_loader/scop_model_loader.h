#ifndef SCOP_MODEL_LOADER_H
#define SCOP_MODEL_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
	#define strtok_r strtok_s
#endif

#define MAX_LINE_SIZE 512

typedef struct loader_obj_data_s
{
	float* v; float* vn; float* vt; float* vp;
	size_t v_ptr, v_cnt, vn_ptr, vn_cnt, vt_ptr, vt_cnt, vp_ptr, vp_cnt;
	size_t v_max_size, vn_max_size, vt_max_size, vp_max_size;
	char id_name[64];
	float* data;
	size_t data_ptr, data_max_size;
} loader_obj_data_t;

void sml_load_wavefront_obj(const char* path);

# ifdef SCOP_MODEL_LOADER_INTERNAL_FUNCTIONALITY

void init_obj_data(loader_obj_data_t* obj, char* name);
void free_obj_data(loader_obj_data_t* obj);
loader_obj_data_t* get_obj_by_id(loader_obj_data_t*** scene, int *size, char* name);
void buff_push_back(float **buff, size_t *ptr, size_t *max_size, float f);
void trim_spaces(char* str);
int get_line_data(char* line, float f[16]);

# endif

#endif
