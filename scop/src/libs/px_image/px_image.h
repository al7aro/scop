#ifndef PX_IMAGE_H
# define PX_IMAGE_H

unsigned char* px_load(const char* path, unsigned int *w, unsigned int *h, unsigned int *chn);

# ifdef PX_INTERNAL_FUNCTIONALITY

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <ctype.h>
# include <string.h>
# include <math.h>
# include <strings.h>

# define PX_IMAGE_WIDTH_ID (uint64_t)((uint64_t)'W' << 32 | (uint64_t)'I' << 24 |  (uint64_t)'D' << 16 | (uint64_t)'T' << 8 | (uint64_t)'H')
# define PX_IMAGE_HEIGHT_ID (uint64_t)((uint64_t)'H' << 40 | (uint64_t)'E' << 32 |  (uint64_t)'I' << 24 | (uint64_t)'G' << 16 | (uint64_t)'H' << 8 | (uint64_t)'T')
# define PX_IMAGE_DEPTH_ID (uint64_t)((uint64_t)'D' << 32 |  (uint64_t)'E' << 24 | (uint64_t)'P' << 16 | (uint64_t)'T' << 8 | (uint64_t)'H')
# define PX_IMAGE_MAXVAL_ID (uint64_t)((uint64_t)'M' << 40 | (uint64_t)'A' << 32 |  (uint64_t)'X' << 24 | (uint64_t)'V' << 16 | (uint64_t)'A' << 8 | (uint64_t)'L')
# define PX_IMAGE_TUPLTYPE_ID (uint64_t)((uint64_t)'T' << 56 | (uint64_t)'U' << 48 | (uint64_t)'P' << 40 | (uint64_t)'L' << 32 |  (uint64_t)'T' << 24 | (uint64_t)'Y' << 16 | (uint64_t)'P' << 8 | (uint64_t)'E')
# define PX_IMAGE_ENDHDR_ID (uint64_t)((uint64_t)'E' << 40 | (uint64_t)'N' << 32 |  (uint64_t)'D' << 24 | (uint64_t)'H' << 16 | (uint64_t)'D' << 8 | (uint64_t)'R')

# define PX_IMAGE_TOKEN_FIELD 0
# define PX_IMAGE_TOKEN_NEWLINE 1 
# define PX_IMAGE_TOKEN_VALUE 2

# define PX_ERROR_INVALID_SYNTAX "Invalid syntax."
# define PX_ERROR_FILE_NOT_FOUND "File not found."
# define PX_ERROR_UNRECOGNICED_FILE "Unrecogniced file type. (Available formats: PAM)."
# define PX_ERROR_FATAL "Fatal error."
# define PX_INVALID_ARGUMENTS "Invalid arguments."

typedef struct img_ctx_s
{
    size_t w;
    size_t h;
    size_t depth;
    size_t maxval;
    size_t tupletype;
    unsigned char bin;

    size_t* current_field;
    size_t file_len;
    unsigned char* buff;
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