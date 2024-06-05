#define scop_INTERNAL_FUNCTIONALITY
#include "scop_image.h"

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
        if (!strncmp(strchr(token, 0) + 1, "ENDHDR", strlen("ENDHDR")))
            break;
        token = strtok_r(NULL, " \r\t\v\n", &aux);
    }
    if (token)
    {
        token = strchr(token, 0) + 1;
        token = strchr(token, '\n');
    }
    ctx->buff_ptr = ((char *)token) - ((char *)ctx->buff);
}
