#ifndef PX_IMAGE_H
# define PX_IMAGE_H

unsigned char* px_load(const char* path, unsigned int *w, unsigned int *h);

#endif

#ifdef PX_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <strings.h>

#define PX_IMAGE_WIDTH_ID (uint64_t)((uint64_t)'W' << 32 | (uint64_t)'I' << 24 |  (uint64_t)'D' << 16 | (uint64_t)'T' << 8 | (uint64_t)'H')
#define PX_IMAGE_HEIGHT_ID (uint64_t)((uint64_t)'H' << 40 | (uint64_t)'E' << 32 |  (uint64_t)'I' << 24 | (uint64_t)'G' << 16 | (uint64_t)'H' << 8 | (uint64_t)'T')
#define PX_IMAGE_DEPTH_ID (uint64_t)((uint64_t)'D' << 32 |  (uint64_t)'E' << 24 | (uint64_t)'P' << 16 | (uint64_t)'T' << 8 | (uint64_t)'H')
#define PX_IMAGE_MAXVAL_ID (uint64_t)((uint64_t)'M' << 40 | (uint64_t)'A' << 32 |  (uint64_t)'X' << 24 | (uint64_t)'V' << 16 | (uint64_t)'A' << 8 | (uint64_t)'L')
#define PX_IMAGE_TUPLTYPE_ID (uint64_t)((uint64_t)'T' << 56 | (uint64_t)'U' << 48 | (uint64_t)'P' << 40 | (uint64_t)'L' << 32 |  (uint64_t)'T' << 24 | (uint64_t)'Y' << 16 | (uint64_t)'P' << 8 | (uint64_t)'E')
#define PX_IMAGE_ENDHDR_ID (uint64_t)((uint64_t)'E' << 40 | (uint64_t)'N' << 32 |  (uint64_t)'D' << 24 | (uint64_t)'H' << 16 | (uint64_t)'D' << 8 | (uint64_t)'R')

#define PX_IMAGE_TOKEN_FIELD 0
#define PX_IMAGE_TOKEN_NEWLINE 1 
#define PX_IMAGE_TOKEN_VALUE 2

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

static void px_loader_error(img_ctx_t* ctx)
{
    if (ctx->buff)
        free(ctx->buff);
    ctx->buff = NULL;
    printf("ERROR: file is not well formated.\n");
}

/**
 * @brief Check wether the field read is correct. Only fields specified by PAM file specification are correct.
 * 
 * @param ctx 
 * @param expected_token The kind of field we are expecting
 * @return char Wether the field exists and is valid.
 */
static char token_is_field_valid(img_ctx_t* ctx, char expected_token)
{
    size_t i = 0;
    uint64_t id = 0;
    for (; *(ctx->buff + ctx->buff_ptr + i) && !isspace(*(ctx->buff + ctx->buff_ptr + i)); i++)
        id = (id << 8) | *(ctx->buff + ctx->buff_ptr + i);   
    if (!isspace(*(ctx->buff + ctx->buff_ptr + i)))
        return (0);
    switch (id)
    {
        case PX_IMAGE_WIDTH_ID:
            ctx->current_field = &(ctx->w);
            break;
        case PX_IMAGE_HEIGHT_ID:
            ctx->current_field = &(ctx->h);
            break;
        case PX_IMAGE_DEPTH_ID:
            ctx->current_field = &(ctx->depth);
            break;
        case PX_IMAGE_MAXVAL_ID:
            ctx->current_field = &(ctx->maxval);
            break;
        case PX_IMAGE_TUPLTYPE_ID:
            ctx->current_field = &(ctx->tupletype);
            break;
        case PX_IMAGE_ENDHDR_ID:
            ctx->current_field = 0;
            break;
        default:
            return (0);
    }
    return (1);
}

/**
 * @brief Check wether the value is correct. Numbers must be greater than 0 and tupletype is ingored
 * 
 * @param ctx 
 * @return int64_t The actual value read. Zero if it was invalid
 */
static int64_t token_is_value_valid(img_ctx_t* ctx)
{
    int64_t num = 0;
    size_t i = 0;
    if (ctx->current_field == &(ctx->tupletype))
        return (1);
    for (; *(ctx->buff + ctx->buff_ptr + i) && isdigit(*(ctx->buff + ctx->buff_ptr + i)); i++)
        num = (num * 10) + *(ctx->buff + ctx->buff_ptr + i) - '0';
    if (isprint(*(ctx->buff + ctx->buff_ptr + i)) && !isspace(*(ctx->buff + ctx->buff_ptr + i)))
        return (0);
    return (num);
}

/**
 * @brief Finds the next token found in the header. A token consist of:
 * 1) ANYTHING separated by a non-newline space (FIELD / VALUE / NEWLINE)
 * Anything at the right of a '#' character is ignored
 * 
 * @param ctx 
 * @param expected_token Wether we expect to find a value or the field it belongs to
 * @return int64_t – If expected_token is a value it return its value. If expected_token is field it returns a bool check
 */
static int64_t token_next(img_ctx_t* ctx, char expected_token)
{
    char in_comment = 0;
    /* SKIP NEW LINES AND '#' */
    if (*(ctx->buff + ctx->buff_ptr) == '\n')
        for (; *(ctx->buff + ctx->buff_ptr) == '\n' || in_comment; ctx->buff_ptr++)
        {
            if (*(ctx->buff + ctx->buff_ptr + 1) == '#' && !in_comment)
                in_comment = 1;
            else if (*(ctx->buff + ctx->buff_ptr) == '\n' && in_comment)
                in_comment = 0;
        }
    /* SKIP SPACE-SEPARATED TOKENS */
    else if (isprint(*(ctx->buff + ctx->buff_ptr)) && !isspace(*(ctx->buff + ctx->buff_ptr)))
        for (; isprint(*(ctx->buff + ctx->buff_ptr)) && !isspace(*(ctx->buff + ctx->buff_ptr)); ctx->buff_ptr++);

    /* SKIP SPACES AND '#' */
    for (; isspace(*(ctx->buff + ctx->buff_ptr)) && *(ctx->buff + ctx->buff_ptr) != '\n'; ctx->buff_ptr++);
    if (*(ctx->buff + ctx->buff_ptr) == '#')
        for (; *(ctx->buff + ctx->buff_ptr) != '\n'; ctx->buff_ptr++);

    /* VALIDATE THE TYPE OF TOKEN FOUND */
    if (PX_IMAGE_TOKEN_FIELD == expected_token)
        return (token_is_field_valid(ctx, expected_token));
    if (PX_IMAGE_TOKEN_VALUE == expected_token)
        return (token_is_value_valid(ctx));
    return (PX_IMAGE_TOKEN_NEWLINE == expected_token && *(ctx->buff + ctx->buff_ptr) == '\n');
}

/**
 * @brief PPM/PBM/PGM has a specific order in which values appear:
 * WIDTH 'newline' HEIGHT 'newline' MAXVALUE
 * 
 * @param ctx 
 */
static void parse_ppm_headers(img_ctx_t* ctx)
{
    uint64_t value = 0;

    ctx->depth = 3;

    if (!(value = token_next(ctx, PX_IMAGE_TOKEN_VALUE))) return (px_loader_error(ctx));
    ctx->w = value;
    if (!(value = token_next(ctx, PX_IMAGE_TOKEN_VALUE))) return (px_loader_error(ctx));
    ctx->h = value;
    if (!(value = token_next(ctx, PX_IMAGE_TOKEN_NEWLINE))) return (px_loader_error(ctx));
    if (!(value = token_next(ctx, PX_IMAGE_TOKEN_VALUE))) return (px_loader_error(ctx));
    ctx->maxval = value;
    if (!(value = token_next(ctx, PX_IMAGE_TOKEN_NEWLINE))) return (px_loader_error(ctx));
}

/**
 * @brief PPM/PBM/PGM has a specific order in which values appear:
 * WIDTH 'newline' HEIGHT 'newline' MAXVALUE
 * 
 * @param ctx 
 */
static void parse_pbm_headers(img_ctx_t* ctx)
{
    uint64_t value = 0;

    ctx->depth = 1;
    ctx->maxval = 1;

    if (!(value = token_next(ctx, PX_IMAGE_TOKEN_VALUE))) return (px_loader_error(ctx));
    ctx->w = value;
    if (!(value = token_next(ctx, PX_IMAGE_TOKEN_VALUE))) return (px_loader_error(ctx));
    ctx->h = value;
    if (!(value = token_next(ctx, PX_IMAGE_TOKEN_NEWLINE))) return (px_loader_error(ctx));
}

/**
 * @brief PPM/PBM/PGM has a specific order in which values appear:
 * WIDTH 'newline' HEIGHT 'newline' MAXVALUE
 * 
 * @param ctx 
 */
static void parse_pgm_headers(img_ctx_t* ctx)
{
    parse_ppm_headers(ctx);
    ctx->depth = 1;
}

static void parse_pam_headers(img_ctx_t* ctx)
{
    uint64_t value = 0;

    size_t i = 0;
    for (; ctx->current_field && i < 10; i++)
    {
        if (!(value = token_next(ctx, PX_IMAGE_TOKEN_FIELD))) return (px_loader_error(ctx));
        if (ctx->current_field)
        {
            if (!(value = token_next(ctx, PX_IMAGE_TOKEN_VALUE))) return (px_loader_error(ctx));
            *(ctx->current_field) = value;
            if (!(value = token_next(ctx, PX_IMAGE_TOKEN_NEWLINE))) return (px_loader_error(ctx));
        }
    }
    if (!(value = token_next(ctx, PX_IMAGE_TOKEN_NEWLINE)) || i < 6 || i >= 10) return (px_loader_error(ctx));
}

/* Bytes per channel = ceil(maxvalue / 255) */
/* Number of bytes needed to be read per color = ceil(maxvalue / 255) * channels */
static void read_color_data(img_ctx_t* ctx)
{
    size_t col_ptr = 0;
    unsigned int bytes_per_channel = ceil((double)ctx->maxval / 255.0);
    /* This is for binary */
    for (; ctx->buff_ptr < ctx->file_len; ctx->buff_ptr += (bytes_per_channel * (ctx->depth)))
        for (size_t i = 0; i < ctx->depth; i++)
        {
            unsigned char col = 0;
            uint64_t ch = 0;
            for (size_t k = 0; k < bytes_per_channel; k++)
                ch = (ch << 8) | (*(ctx->buff + ctx->buff_ptr + (i * bytes_per_channel) + k));
            col = ((int)(ch > ctx->maxval) ? 255 : (int)(255.0 * ((double)ch / ctx->maxval)));
            *(ctx->col_data + col_ptr) = col;
            col_ptr++; 
        }
}

unsigned char* px_load(const char* path, unsigned int *w, unsigned int *h)
{
    FILE* fp = fopen(path, "rb");
    img_ctx_t ctx;
    ctx.buff_ptr = 0;
    fseek(fp, 0, SEEK_END);
    ctx.file_len = ftell(fp);
    fseek(fp, 0, SEEK_END);
    fseek(fp, 0, SEEK_SET);
    ctx.buff = (unsigned char*)malloc(sizeof(unsigned char) * (ctx.file_len + 64));
    bzero(ctx.buff, ctx.file_len + 64);
    fread(ctx.buff, ctx.file_len, 1, fp);
    fclose(fp);

    /* READ MAGIC NUMBER */
    for (; isspace(ctx.buff[ctx.buff_ptr]) && ctx.buff_ptr < ctx.file_len; ctx.buff_ptr++);
    ctx.buff_ptr += 2;
    switch (ctx.buff[ctx.buff_ptr - 2] << 16 | ctx.buff[ctx.buff_ptr - 1] << 8 | !!isspace(ctx.buff[ctx.buff_ptr]))
    {
        case ('P' << 16 | '1' << 8 | 1):
            ctx.bin = 0;
            printf("ASCII PBM\n");
            parse_pbm_headers(&ctx);
            break;
        case ('P' << 16 | '2' << 8 | 1):
            ctx.bin = 0;
            printf("ASCII PGM\n");
            parse_pgm_headers(&ctx);
            break;
        case ('P' << 16 | '3' << 8 | 1):
            ctx.bin = 0;
            printf("ASCII PPM\n");
            parse_ppm_headers(&ctx);
            break;
        case ('P' << 16 | '4' << 8 | 1):
            ctx.bin = 1;
            printf("BINARY PBM\n");
            parse_pbm_headers(&ctx);
            break;
        case ('P' << 16 | '5' << 8 | 1):
            ctx.bin = 1;
            printf("BINARY PGM\n");
            parse_pgm_headers(&ctx);
            break;
        case ('P' << 16 | '6' << 8 | 1):
            ctx.bin = 1;
            printf("BINARY PPM\n");
            parse_ppm_headers(&ctx);
            break;
        case ('P' << 16 | '7' << 8 | 1):
            ctx.bin = 1;
            printf("PAM\n");
            parse_pam_headers(&ctx);
            break;
        default:
            px_loader_error(&ctx);
            return NULL;
    }
    if (!ctx.buff)
        return (NULL);
    printf("W: %ld | H: %ld | depth: %ld | maxval: %ld\n", ctx.w, ctx.h, ctx.depth, ctx.maxval);
    ctx.buff_ptr++;
    /* At this point the header is correct and colors MUST start at the very next byte */
    ctx.col_data = (unsigned char*)malloc(sizeof(unsigned char) * (ctx.w * ctx.h * ctx.depth));
    bzero(ctx.col_data, sizeof(unsigned char) * ctx.w * ctx.h * ctx.depth);
    read_color_data(&ctx);
    free(ctx.buff);
    *w = ctx.w;
    *h = ctx.h;
    return (ctx.col_data);
}

#endif