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
#define VT_ATT_ID 1
#define VN_ATT_ID 2
#define VP_ATT_ID 3
#define MAX_ATT_ID 4

#define MAX_FACE_SIZE 64

/* MAKE THIS MORE GENERAL (LOL) */
typedef struct sml_face_s
{
	unsigned int v_idx[MAX_FACE_SIZE];
	unsigned int vn_idx[MAX_FACE_SIZE];
	unsigned int vt_idx[MAX_FACE_SIZE];
	unsigned int vp_idx[MAX_FACE_SIZE];
	unsigned int size;
} sml_face_t;

typedef struct sml_mtl_group_s
{
	char usemtl[64];
	t_list* faces;		/*Content -> sml_obj_face*/

	float Ns;
	float Ka[3];
	float Kd[3];
	float Ks[3];
	float Ke[3];
	float Ni;
	float d;
	int illum;
} sml_mtl_group_t;

typedef struct sml_obj_s
{ 
	float* v; size_t v_ptr, v_cnt, v_max_size;
	float* vn; size_t vn_ptr, vn_cnt, vn_max_size;
	float* vt; size_t vt_ptr, vt_cnt, vt_max_size;
	float* vp; size_t vp_ptr, vp_cnt, vp_max_size;

	char id_name[64];
	t_list* mtl_group;	/*Content -> sml_mtl_group*/
} sml_obj_t;

typedef struct sml_scene_s
{
	t_list* obj;
	char mtllib[64];
} sml_scene_t;

sml_scene_t* sml_load_wavefront_obj(const char* path);
void sml_destroy(sml_scene_t* scene);


# ifdef SCOP_MODEL_LOADER_INTERNAL_FUNCTIONALITY

void init_mtl_group(sml_mtl_group_t* mtl, const char* mtl_name);
void init_obj(sml_obj_t* obj, char* name);
void init_face(sml_face_t* obj);
void init_scene(sml_scene_t* scene);

void buff_push_back_float(float** buff, size_t* ptr, size_t* max_size, float f);
void trim_spaces(char* str);
int get_line_data(char* line, float f[16]);

# endif

#endif
