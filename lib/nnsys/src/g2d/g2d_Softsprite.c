// NitroSystem's software sprite (g2d_Softsprite.c), which Platinum draws its Pokemon and its software sprites with
// (pret's pokeplatinum: src/pokemon_sprite.c, src/software_sprite.c); Black never calls it, so the DS build doesn't
// compile this file (no delinks entry). Written for the port from the API's behaviour: the quad is the one
// g2d_OamSoftwareSpriteDraw.c draws for an OAM, in the same vertex order.
#include "nitro/gx.h"
#include "nnsys/g2d.h"

// An orthographic camera over the screen: x right and y down from the top-left pixel, in pixels, z from -1024 (far)
// to 1024 (near); the position matrix is left as the identity
void NNS_G2dSetupSoftwareSpriteCamera(void) {
    G3_MtxMode(GX_MTXMODE_PROJECTION);
    G3_Identity();
    G3_OrthoW(0, 192 * FX32_ONE, 0, 256 * FX32_ONE, -1024 * FX32_ONE, 1024 * FX32_ONE, FX32_ONE, NULL);
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_Identity();
}

// A textured quad sx by sy pixels whose top-left corner is at (x, y, z) in the current matrix; (u0, v0) and (u1, v1)
// are the texels at its top-left and bottom-right corners. The current matrix is kept.
void NNS_G2dDrawSpriteFast(s16 x, s16 y, int z, int sx, int sy, int u0, int v0, int u1, int v1) {
    G3_PushMtx();
    G3_Translate(x << FX32_SHIFT, y << FX32_SHIFT, z << FX32_SHIFT);
    G3_Scale(sx << FX32_SHIFT, sy << FX32_SHIFT, FX32_ONE);
    G3_Begin(GX_BEGIN_QUADS);
    G3_TexCoord(u0 << FX32_SHIFT, v1 << FX32_SHIFT);
    G3_Vtx10(0, FX16_ONE, 0);
    G3_TexCoord(u1 << FX32_SHIFT, v1 << FX32_SHIFT);
    G3_Vtx10(FX16_ONE, FX16_ONE, 0);
    G3_TexCoord(u1 << FX32_SHIFT, v0 << FX32_SHIFT);
    G3_Vtx10(FX16_ONE, 0, 0);
    G3_TexCoord(u0 << FX32_SHIFT, v0 << FX32_SHIFT);
    G3_Vtx10(0, 0, 0);
    G3_End();
    G3_PopMtx(1);
}
