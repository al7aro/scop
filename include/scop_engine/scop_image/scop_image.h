#ifndef scop_IMAGE_H
# define scop_IMAGE_H

unsigned char* scop_image_load(const char* path, int *w, int *h, int *chn);

# ifdef scop_INTERNAL_FUNCTIONALITY

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <ctype.h>
# include <string.h>
# include <math.h>

#include "scop_macros.h"

#define SCOP_IMAGE_MIN_MAXVAL 0
#define SCOP_IMAGE_MAX_MAXVAL 1000000

#define SCOP_IMAGE_MIN_W 0
#define SCOP_IMAGE_MAX_W 8192

#define SCOP_IMAGE_MIN_H 0
#define SCOP_IMAGE_MAX_H 8192

#define SCOP_IMAGE_MIN_DEPTH 0
#define SCOP_IMAGE_MAX_DEPTH 8192

typedef struct img_ctx_s
{
    int w;
    int h;
    int depth;
    int maxval;
    int tupletype;
    unsigned char bin;

    int* current_field;
    unsigned char* buff;
    size_t file_len;
    size_t buff_ptr;

    unsigned char* col_data;
} img_ctx_t;

void parse_pam_headers(img_ctx_t* ctx);
void scop_image_loader_error(img_ctx_t* ctx, const char* str, const char* str2);
void read_color_data(img_ctx_t* ctx);

# endif

#endif
