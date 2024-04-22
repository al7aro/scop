#define PX_INTERNAL_FUNCTIONALITY
#include "px_image.h"

void px_loader_error(img_ctx_t* ctx)
{
    if (ctx->buff)
        free(ctx->buff);
    ctx->buff = NULL;
    printf("ERROR: file is not well formated.\n");
}

unsigned char* px_load(const char* path, unsigned int *w, unsigned int *h)
{
    FILE* fp = fopen(path, "rb");
    if (!fp)
        return (NULL);
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
    ctx.current_field = (void *)1;
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