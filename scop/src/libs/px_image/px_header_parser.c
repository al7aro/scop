#define PX_INTERNAL_FUNCTIONALITY
#include "px_image.h"

/**
 * @brief Check wether the field read is correct. Only fields specified by PAM file specification are correct.
 * 
 * @param ctx 
 * @param expected_token The kind of field we are expecting
 * @return char Wether the field exists and is valid.
 */
static char token_is_field_valid(img_ctx_t* ctx)
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
static int token_is_value_valid(img_ctx_t* ctx)
{
    int num = 0;
    size_t i = 0;
    if (ctx->current_field == &(ctx->tupletype))
        return (1);
    for (; *(ctx->buff + ctx->buff_ptr + i) && isdigit(*(ctx->buff + ctx->buff_ptr + i)); i++)
        num = (num * 10) + *(ctx->buff + ctx->buff_ptr + i) - '0';
    if (isprint(*(ctx->buff + ctx->buff_ptr + i)) && !isspace(*(ctx->buff + ctx->buff_ptr + i)))
        return (0);
    ctx->current_value = num;
    return (1);
}

/**
 * @brief Finds the next token found in the header. A token consist of:
 * 1) ANYTHING separated by a non-newline space (FIELD / VALUE / NEWLINE)
 * Anything at the right of a '#' character is ignored
 * 
 * @param ctx 
 * @param expected_token Wether we expect to find a value or the field it belongs to
 * @return int – If expected_token is a value it return its value. If expected_token is field it returns a bool check
 */
static int token_next(img_ctx_t* ctx, char expected_token)
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
        return (token_is_field_valid(ctx));
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
void parse_ppm_headers(img_ctx_t* ctx)
{
    ctx->depth = 3;

    if (!token_next(ctx, PX_IMAGE_TOKEN_VALUE))
    {
        px_loader_error(ctx, PX_ERROR_INVALID_SYNTAX);
        return;
    }
    ctx->w = ctx->current_value;
    if (!token_next(ctx, PX_IMAGE_TOKEN_VALUE))
    {
        px_loader_error(ctx, PX_ERROR_INVALID_SYNTAX);
        return;
    }
    ctx->h = ctx->current_value;
    if (!token_next(ctx, PX_IMAGE_TOKEN_NEWLINE))
    {
        px_loader_error(ctx, PX_ERROR_INVALID_SYNTAX);
        return;
    }
    if (!token_next(ctx, PX_IMAGE_TOKEN_VALUE))
    {
        px_loader_error(ctx, PX_ERROR_INVALID_SYNTAX);
        return;
    }
    ctx->maxval = ctx->current_value;
    if (!token_next(ctx, PX_IMAGE_TOKEN_NEWLINE))
    {
        px_loader_error(ctx, PX_ERROR_INVALID_SYNTAX);
        return;
    }
}

/**
 * @brief PPM/PBM/PGM has a specific order in which values appear:
 * WIDTH 'newline' HEIGHT 'newline' MAXVALUE
 * 
 * @param ctx 
 */
void parse_pbm_headers(img_ctx_t* ctx)
{
    ctx->depth = 1;
    ctx->maxval = 1;

    if (!token_next(ctx, PX_IMAGE_TOKEN_VALUE))
    {
        px_loader_error(ctx, PX_ERROR_INVALID_SYNTAX);
        return;
    }
    ctx->w = ctx->current_value;
    if (!token_next(ctx, PX_IMAGE_TOKEN_VALUE))
    {
        px_loader_error(ctx, PX_ERROR_INVALID_SYNTAX);
        return;
    }
    ctx->h = ctx->current_value;
    if (!token_next(ctx, PX_IMAGE_TOKEN_NEWLINE))
    {
        px_loader_error(ctx, PX_ERROR_INVALID_SYNTAX);
        return;
    }
}

/**
 * @brief PPM/PBM/PGM has a specific order in which values appear:
 * WIDTH 'newline' HEIGHT 'newline' MAXVALUE
 * 
 * @param ctx 
 */
void parse_pgm_headers(img_ctx_t* ctx)
{
    parse_ppm_headers(ctx);
    ctx->depth = 1;
}

void parse_pam_headers(img_ctx_t* ctx)
{
    size_t i = 0;
    for (; ctx->current_field && i < 10; i++)
    {
        if (!token_next(ctx, PX_IMAGE_TOKEN_FIELD))
        {
            px_loader_error(ctx, PX_ERROR_INVALID_SYNTAX);
            return;
        }
        if (ctx->current_field)
        {
            if (!token_next(ctx, PX_IMAGE_TOKEN_VALUE))
            {
                px_loader_error(ctx, PX_ERROR_INVALID_SYNTAX);
                return;
            }
            *(ctx->current_field) = ctx->current_value;
            if (!token_next(ctx, PX_IMAGE_TOKEN_NEWLINE))
            {
                px_loader_error(ctx, PX_ERROR_INVALID_SYNTAX);
                return;
            }
        }
    }
    if (!token_next(ctx, PX_IMAGE_TOKEN_NEWLINE) || i < 6 || i >= 10)
    {
        px_loader_error(ctx, PX_ERROR_INVALID_SYNTAX);
        return;
    }
}