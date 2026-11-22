/* port/gpu piece G12 (texture cache, docs/gpu.md): the mip chain of a cached layer, without bleeding and (for hard-alpha
 * formats) without thinning. OWNED BY G12. Pure CPU. Works on the layer's RGBA8 (g12_tc_rgba8 of a g12_tc_layout image
 * after g12_tc_fill), S x S, S a power of two. */
#ifndef GPU_G12_TCMIPS_H
#define GPU_G12_TCMIPS_H

#include "vc_types.h"

/* bytes of levels 1..log2(S) (S/2 x S/2 down to 1 x 1) laid out one after the other */
size_t g12_tc_mips_size(int S);

/* levels 1.. of the chain from level 0 (S * S * 4 bytes RGBA8) into out (g12_tc_mips_size bytes).
 * Each level from the one above: alpha = mean of the 2 x 2 block; colour = the alpha-weighted mean of the block's colours
 * (straight alpha, so a transparent texel's colour never leaks in); a block of alpha 0 takes the fill rule
 * (g12_tc_fill: its non-transparent neighbours, outward, then the mean). hard != 0 (formats 2, 3, 4 with bit 29, 5, 7:
 * alpha is 0 or full, the shader tests >= 1/2): each level's alpha is then scaled by a k (also < 1 where averaging
 * fills holes) that makes the share of texels with alpha >= 1/2 equal level 0's (Castano 2010, "Computing Alpha Mipmaps"),
 * so a one-texel branch does not fade under the threshold. Colours are always taken from the unscaled alpha chain.
 * Not thread-safe (g12_tc_fill's scratch). */
void g12_tc_mips(const u8 *level0, int S, int hard, u8 *out);

#endif
