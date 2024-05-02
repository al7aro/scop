#ifndef PX_IMAGE_H
# define PX_IMAGE_H

unsigned char* px_load(const char* path, int *w, int *h, int *chn);

# ifdef PX_INTERNAL_FUNCTIONALITY

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <ctype.h>
# include <string.h>
# include <math.h>

# define PX_ERROR_INVALID_SYNTAX "Invalid syntax."
# define PX_ERROR_FILE_NOT_FOUND "File not found."
# define PX_ERROR_UNRECOGNICED_FILE "Unrecogniced file type. (Available formats: PAM)."
# define PX_ERROR_FATAL "Fatal error."
# define PX_INVALID_ARGUMENTS "Invalid arguments."

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
void parse_ppm_headers(img_ctx_t* ctx);
void parse_pgm_headers(img_ctx_t* ctx);
void parse_pbm_headers(img_ctx_t* ctx);
void px_loader_error(img_ctx_t* ctx, const char* str);
void read_color_data(img_ctx_t* ctx);

# endif

#endif
