#define PX_INTERNAL_FUNCTIONALITY
#include "px_image.h"

/* Bytes per channel = ceil(maxvalue / 255) */
/* Number of bytes needed to be read per color = ceil(maxvalue / 255) * channels */
void read_color_data(img_ctx_t* ctx)
{
    size_t col_ptr = 0;
    unsigned int bytes_per_channel = (unsigned int)ceil((double)ctx->maxval / 255.0);
    /* This is for binary */
    for (; ctx->buff_ptr < ctx->file_len && (ctx->buff_ptr + (bytes_per_channel * ctx->depth)) < ctx->file_len; ctx->buff_ptr += (bytes_per_channel * (ctx->depth)))
        for (int i = 0; i < ctx->depth; i++)
        {
            unsigned char col = 0;
            uint64_t ch = 0;
            for (size_t k = 0; k < bytes_per_channel && (ctx->buff_ptr + (i * bytes_per_channel) + k) < ctx->file_len; k++)
                ch = (ch << 8) | (*(ctx->buff + ctx->buff_ptr + (i * bytes_per_channel) + k));
            col = ((int)(ch > (uint64_t)ctx->maxval) ? 255 : (char)(255.0 * ((double)ch / ctx->maxval)));
            if (col_ptr < (size_t)(ctx->w * ctx->h * ctx->depth))
                *(ctx->col_data + col_ptr) = col;
            col_ptr++; 
        }
}
