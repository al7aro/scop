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

typedef struct loader_data_s
{
	float* v; float* c; float* vn; float* vt; float* vp;
	size_t v_ptr, c_ptr, vn_ptr, vt_ptr, vp_ptr;
	size_t v_max_size, c_max_size, vn_max_size, vt_max_size, vp_max_size;
	char id_name[64];
	float* data;
} loader_data_t;

void sml_load_wavefront_obj(const char* path);

#endif