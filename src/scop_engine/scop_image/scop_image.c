#define scop_INTERNAL_FUNCTIONALITY
#include "scop_image.h"

void scop_image_loader_error(img_ctx_t* ctx, const char* str, const char* str2)
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
        if (str2)
            fprintf(stderr, ": [%s]", str2);
        fprintf(stderr, "\n");
    }
}

char scop_image_is_valid(img_ctx_t* ctx)
{
    if (ctx->maxval <= SCOP_IMAGE_MIN_MAXVAL || ctx->maxval >= SCOP_IMAGE_MAX_MAXVAL)
        return 0;
    if (ctx->w <= SCOP_IMAGE_MIN_W || ctx->w >= SCOP_IMAGE_MAX_W)
        return 0;
    if (ctx->h <= SCOP_IMAGE_MIN_H || ctx->h >= SCOP_IMAGE_MAX_H)
        return 0;
    if (ctx->depth <= SCOP_IMAGE_MIN_DEPTH || ctx->depth >= SCOP_IMAGE_MAX_DEPTH)
        return 0;
    return 1;
}

unsigned char* scop_image_load(const char* path, int *w, int *h, int *chn)
{
    FILE* fp = fopen(path, "rb");
    if (!fp)
    {
        scop_image_loader_error(NULL, SCOP_ERROR_FILE_NOT_FOUND, path);
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
        scop_image_loader_error(NULL, SCOP_ERROR_FATAL, path);
        fclose(fp);
        return (NULL);
    }
    memset(ctx.buff, 0, sizeof(unsigned char) * (ctx.file_len + 64));
    fread(ctx.buff, ctx.file_len, 1, fp);
    fclose(fp);
    ctx.w = 0; ctx.h = 0; ctx.depth = 0; ctx.maxval = 0; 

    /* READ MAGIC NUMBER */
    for (; isspace(ctx.buff[ctx.buff_ptr]) && ctx.buff_ptr < ctx.file_len; ctx.buff_ptr++);
    ctx.buff_ptr += 2;
    ctx.current_field = (void *)1;
    switch (ctx.buff[ctx.buff_ptr - 2] << 16 | ctx.buff[ctx.buff_ptr - 1] << 8 | !!isspace(ctx.buff[ctx.buff_ptr]))
    {
        case ('P' << 16 | '7' << 8 | 1):
            ctx.bin = 1;
            parse_pam_headers(&ctx);
            break;
        default:
            scop_image_loader_error(&ctx, SCOP_ERROR_UNRECOGNICED_FILE, path);
            return NULL;
    }
    if (!ctx.buff)
        return (NULL);
    ctx.buff_ptr++;
    if (!scop_image_is_valid(&ctx))
    {
        scop_image_loader_error(&ctx, SCOP_ERROR_FATAL, path);
        return NULL;
    }

    /* At this point the header is correct and colors MUST start at the very next byte */
    ctx.col_data = (unsigned char*)malloc(sizeof(unsigned char) * (ctx.w * ctx.h * ctx.depth));
    if (!ctx.col_data)
    {
        scop_image_loader_error(&ctx, SCOP_ERROR_FATAL, path);
        return (NULL);
    }
    memset(ctx.col_data, 0, sizeof(unsigned char) * ctx.w * ctx.h * ctx.depth);
    read_color_data(&ctx);
    free(ctx.buff);
    if (w) *w = ctx.w;
    if (h) *h = ctx.h;
    if (chn) *chn = ctx.depth;
    return (ctx.col_data);
}
