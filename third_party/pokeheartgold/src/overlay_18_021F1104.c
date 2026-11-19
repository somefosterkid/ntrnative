#include "global.h"

#include "sprite.h"
#include "sprite_system.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F10E8(PokedexAppData *, u32);

void ov18_021F1104(PokedexAppData *arg0);

void ov18_021F1104(PokedexAppData *arg0) {
    u32 v0;

    v0 = 0;
    do {
        ov18_021F10E8(arg0, v0);
        v0 += 1;
    } while (v0 < 0x78U);
}

void ov18_021F111C(PokedexAppData *arg0, s32 arg1, void *arg2, u32 arg3, enum NNS_G2D_VRAM_TYPE arg4);

void ov18_021F111C(PokedexAppData *arg0, s32 arg1, void *pSrc, u32 szByte, enum NNS_G2D_VRAM_TYPE type) {
    u32 offset;

    offset = NNS_G2dGetImageLocation(Sprite_GetImageProxy(arg0->unk_0670[arg1]->sprite), type);
    DC_FlushRange(pSrc, szByte);
    if (type == NNS_G2D_VRAM_TYPE_2DMAIN) {
        GX_LoadOBJ(pSrc, offset, szByte);
        return;
    }
    GXS_LoadOBJ(pSrc, offset, szByte);
}

void ov18_021F1160(PokedexAppData *arg0, s32 arg1, s32 arg2);

void ov18_021F1160(PokedexAppData *arg0, s32 arg1, s32 arg2) {
    if (arg2 == 1) {
        ManagedSprite_SetOamMode(arg0->unk_0670[arg1], GX_OAM_MODE_XLU);
        return;
    }
    ManagedSprite_SetOamMode(arg0->unk_0670[arg1], GX_OAM_MODE_NORMAL);
}
