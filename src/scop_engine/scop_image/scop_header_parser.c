#define scop_INTERNAL_FUNCTIONALITY
#include "scop_image.h"

/**
 * @brief Check wether the value is correct. Numbers must be greater than 0 and tupletype is ingored
 * 
 * @param ctx 
 * @return int64_t The actual value read. Zero if it was invalid
 */
static int token_is_value_valid(const char* str)
{
    int num = 0;
    size_t i = 0;
    for (; *(str + i) && isdigit(*(str + i)); i++)
        num = (num * 10) + *(str + i) - '0';
    if (isprint(*(str + i)) && !isspace(*(str + i)))
        return (-1);
    return (num);
}

/**
 * @brief PPM/PBM/PGM has a specific order in which values appear:
 * WIDTH 'newline' HEIGHT 'newline' MAXVALUE
 * 
 * @param ctx 
 */
void parse_ppm_headers(img_ctx_t* ctx)
{
    (void)ctx;
}

/**
 * @brief PPM/PBM/PGM has a specific order in which values appear:
 * WIDTH 'newline' HEIGHT 'newline' MAXVALUE
 * 
 * @param ctx 
 */
void parse_pbm_headers(img_ctx_t* ctx)
{
    (void)ctx;
}

/**
 * @brief PPM/PBM/PGM has a specific order in which values appear:
 * WIDTH 'newline' HEIGHT 'newline' MAXVALUE
 * 
 * @param ctx 
 */
void parse_pgm_headers(img_ctx_t* ctx)
{
    (void)ctx;
}

void parse_pam_headers(img_ctx_t* ctx)
{
    char* aux = (char *)(ctx->buff + ctx->buff_ptr);
    char* token = strtok_r((char *)(ctx->buff + ctx->buff_ptr), " \r\t\v\n", &aux);
    while (token != NULL && strcmp(token, "ENDHDR"))
    {
        if (*token == '#')
        {
            token = strtok_r(NULL, "\n", &aux);
            token = strtok_r(NULL, " \r\t\v\n", &aux);
            continue;
        }
        if (!strcmp(token, "WIDTH") && !(ctx->w))
            ctx->current_field = &(ctx->w);
        else if (!strcmp(token, "HEIGHT") && !(ctx->h))
            ctx->current_field = &(ctx->h);
        else if (!strcmp(token, "DEPTH") && !(ctx->depth))
            ctx->current_field = &(ctx->depth);
        else if (!strcmp(token, "MAXVAL") && !(ctx->maxval))
            ctx->current_field = &(ctx->maxval);
        else if (!strcmp(token, "TUPLTYPE"))
            ctx->current_field = &(ctx->tupletype);
        else if (ctx->current_field != NULL)
        {
            *(ctx->current_field) = token_is_value_valid(token);
            ctx->current_field = NULL;
        }
        else
            printf("Error\n");
        token = strtok_r(NULL, " \r\t\v\n", &aux);
    }
    // printf("Headers: [w: %d] [h: %d] [depth: %d] [maxval: %d]\n", ctx->w, ctx->h, ctx->depth, ctx->maxval);
    token = strchr(token, '\0');
    ctx->buff_ptr = ((char *)token) - ((char *)ctx->buff);
    // printf("End: [%d] -> Header Size: %ld -> File Size: %ld\n", *(ctx->buff + ctx->buff_ptr), ctx->buff_ptr, ctx->file_len);
}
