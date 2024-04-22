#define PX_INTERNAL_FUNCTIONALITY
#include "px_image.h"

void px_loader_error(img_ctx_t* ctx, const char* str)
{
    if (ctx)
    {
        if (ctx->buff)
            free(ctx->buff);
        ctx->buff = NULL;
    }
    if (str)
    {
        fprintf(stderr, "%s", str);
        fprintf(stderr, "\n");
    }
}

unsigned char* px_load(const char* path, unsigned int *w, unsigned int *h, unsigned int *chn)
{
    FILE* fp = fopen(path, "rb");
    if (!fp)
    {
        px_loader_error(NULL, PX_ERROR_FILE_NOT_FOUND);
        return (NULL);
    }
    img_ctx_t ctx;
    ctx.buff_ptr = 0;
    fseek(fp, 0, SEEK_END);
    ctx.file_len = ftell(fp);
    fseek(fp, 0, SEEK_END);
    fseek(fp, 0, SEEK_SET);
    ctx.buff = (unsigned char*)malloc(sizeof(unsigned char) * (ctx.file_len + 64));
    if (!ctx.buff)
    {
        px_loader_error(NULL, PX_ERROR_FATAL);
        fclose(fp);
        return (NULL);
    }
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
            px_loader_error(&ctx, PX_ERROR_UNRECOGNICED_FILE);
            return NULL;
    }
    if (!ctx.buff)
        return (NULL);
    printf("W: %ld | H: %ld | depth: %ld | maxval: %ld\n", ctx.w, ctx.h, ctx.depth, ctx.maxval);
    ctx.buff_ptr++;
    /* At this point the header is correct and colors MUST start at the very next byte */
    ctx.col_data = (unsigned char*)malloc(sizeof(unsigned char) * (ctx.w * ctx.h * ctx.depth));
    if (!ctx.col_data)
    {
        px_loader_error(&ctx, PX_ERROR_FATAL);
        return (NULL);
    }
    bzero(ctx.col_data, sizeof(unsigned char) * ctx.w * ctx.h * ctx.depth);
    read_color_data(&ctx);
    free(ctx.buff);
    if (w) *w = ctx.w;
    if (h) *h = ctx.h;
    if (chn) *chn = ctx.depth;
    return (ctx.col_data);
}