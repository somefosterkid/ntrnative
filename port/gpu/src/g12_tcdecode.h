/* port/gpu piece G12 (texture cache, docs/gpu.md): the CPU decode of the DS's 7 texture formats and the layer layout.
 * OWNED BY G12. Pure CPU, no GL. Texels are 4 bytes (r6, g6, b6, a5, one value per byte: the numbers g04_texture.glsl's
 * g4_sample returns); g12_tc_rgba8 turns them into the RGBA8 the GPU stores (lossless for level 0). The source is the
 * flattened frame data (gpu_texdata_bytes): tex = 512 KB of slots 0-3, pal = 96 KB (palette slots 0-5; slots 6-7 read 0). */
#ifndef GPU_G12_TCDECODE_H
#define GPU_G12_TCDECODE_H

#include "vc_types.h"

enum { G12_TEX_BYTES = 0x80000, G12_PAL_BYTES = 0x18000, G12_PAL_SPACE = 0x20000, G12_MAX_BUCKET = 512 };

/* what a decode read: byte ranges [lo, hi) of the texture data (texels), of its slot 1 (a 4x4 texture's block words) and of
 * the palette address space; hi <= lo = nothing. A range that would wrap is the whole space. */
typedef struct G12Ranges {
  u32 tex_lo, tex_hi, idx_lo, idx_hi, pal_lo, pal_hi;
} G12Ranges;

/* texparam's width and height (8 << field); 0 for format 0 (no texture) */
int g12_tc_width(u32 texparam);
int g12_tc_height(u32 texparam);

/* w x h texels of texparam (TEXIMAGE_PARAM) with palette base pltt (PLTT_BASE) into out (w * h * 4 bytes); rg may be NULL.
 * Returns 0, or -1 for format 0. A transparent 4x4 texel is all zero (as g4_sample returns it). */
int g12_tc_decode(const u8 *tex, const u8 *pal, u32 texparam, u32 pltt, u8 *out, G12Ranges *rg);

/* the layer's side for texparam's size and wrap (32, 64, 128, 256 or 512), 0 when it needs more than 512 */
int g12_tc_layer_size(u32 texparam);
/* the w x h image img into layer (S * S * 4 bytes, S = g12_tc_layer_size): repeat axes tiled, flip axes tiled with mirrored
 * copies, clamp axes (clamp, and flip without repeat) padded: texel 0..n-1, then (S-n)/2 copies of texel n-1, then copies of
 * texel 0 (so a footprint that wraps past S lands on the right edge). Returns S, or 0. */
int g12_tc_layout(const u8 *img, u32 texparam, u8 *layer);

/* the colour of every texel with alpha 0 := the mean of its non-transparent 8-neighbours (S x S, wrapping), outward for 8
 * passes, then the mean colour of the texels with alpha > 0 (nothing changes when none has). Texels with alpha > 0 are
 * untouched. */
void g12_tc_fill(u8 *layer, int S);

/* n texels (r6 g6 b6 a5) -> RGBA8: v8 = (v6 * 255 + 31) / 63, a8 = (a5 * 255 + 15) / 31 */
void g12_tc_rgba8(const u8 *raw, int n, u8 *out);

#endif
