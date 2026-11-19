#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "overlay_106.h"
#include "overlay_106_arceus_cinematic.h"
#include "overlay_106_legendary_cinematic.h"
#include "party.h"
#include "pokemon.h"
#include "sprite.h"
#include "unk_02013FDC.h"
#include "gf_gfx_planes.h"
#include "screen_fade.h"
#include "system.h"

s32 ov106_021E6678(void *, u32, s32);

s32 ov106_021E6694(s8 *, u32, s32);

s32 ov106_021E66B0(s32, s32, s32);

void ov106_021E63E0(void *);
s32 ov106_021E6408(void);
void ov106_021E6418(BgConfig **pBgConfig);
void ov106_021E6520(LegendaryCinematic *arg0);
extern s32 ov106_021E70E0;
void ov106_021E64FC(BgConfig **pBgConfig);
void ov106_021E664C(ArceusCinematic *arceus);

void ov106_021E66FC(LegendaryCinematic *arg0) {
    ArceusCinematic *v1;
    s32 v0;
    NNSG3dResTex *nNSG3dResTex;
    NNSG3dResFileHeader *ptr;
    NNSG3dResTex *v2;
    NNSG3dResFileHeader *nNSG3dResFileHeader;
    Party *party;
    Pokemon *mon;
    s8 *v5;
    s8 *texSrc;
    u16 v6;
    u16 v7;
    u32 v3;
    u32 v8;
    u32 v9;
    void *v4;

    v1 = arg0->arceus;
    party = SaveArray_Party_Get(arg0->args->saveData);
    if (Party_GetCount(party) == 0) {
        v7 = 0;
        v6 = 2;
        v0 = 0;
    } else {
        mon = Party_GetMonByIndex(party, 0);
        if (GetMonData(mon, 5, NULL) == 0x1ED) {
            v6 = (u16)GetMonData(mon, 0x6F, NULL);
            v7 = (u16)GetMonData(mon, 0x70, NULL);
            v0 = (s32)MonIsShiny(mon);
        } else {
            v7 = 0;
            v6 = 2;
            v0 = 0;
        }
    }
    nNSG3dResFileHeader = AllocAndReadWholeNarcMemberByIdPair(NARC_data_mmodel_mmodel, ov106_021E66B0(0x1ED, v7, v6), HEAP_ID_153);
    ptr = nNSG3dResFileHeader;
    v2 = NNS_G3dGetTex(nNSG3dResFileHeader);
    nNSG3dResTex = v2;
    v8 = NNS_G2dGetImageLocation(Sprite_GetImageProxy(v1->sprites[0]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    texSrc = &v2->header.chr[v2->texInfo.ofsTex];
    v4 = Heap_Alloc(HEAP_ID_153, 0x800U);
    v9 = 0;
    do {
        sub_020145B4(texSrc, 8, 0, 0, 8, 8, v4);
        ov106_021E6678(v4, v8, 0x800);
        texSrc += 0x800;
        v8 += 0x800;
        v9 += 1;
    } while (v9 < 8U);
    Heap_Free(v4);
    v3 = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(v1->sprites[0]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    v5 = &nNSG3dResTex->header.chr[nNSG3dResTex->plttInfo.ofsPlttData];
    if (v0 == 1) {
        v5 += 0x20;
    }
    ov106_021E6694(v5, v3, 0x20);
    Heap_Free(ptr);
}

s32 ov106_021E6814(LegendaryCinematic *arg0) {
    ArceusCinematic *v0;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_EngineASetPlanes(0U);
    GfGfx_EngineBSetPlanes(0U);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    arg0->arceus = Heap_Alloc(HEAP_ID_153, sizeof(ArceusCinematic));
    MI_CpuFill8(arg0->arceus, 0U, sizeof(ArceusCinematic));
    v0 = arg0->arceus;
    v0->scriptPos = &ov106_021E70E0;
    v0->narc = NARC_New(NARC_a_2_4_5, HEAP_ID_153);
    ov106_021E6408();
    ov106_021E6418(&v0->bgConfig);
    ov106_021E6520(arg0);
    Main_SetVBlankIntrCB(ov106_021E63E0, arg0->arceus);
    return 4;
}

s32 ov106_021E68A8(LegendaryCinematic *arg0) {
    ArceusCinematic *v0;

    v0 = arg0->arceus;
    Main_SetVBlankIntrCB(NULL, NULL);
    ov106_021E664C(v0);
    ov106_021E64FC(&v0->bgConfig);
    NARC_Delete(v0->narc);
    Heap_Free(arg0->arceus);
    return 5;
}

s32 ov106_021E68DC(LegendaryCinematic *arg0) {
    if (IsPaletteFadeFinished() == 1) {
        return arg0->nextState;
    }
    return 2;
}

s32 ov106_021E68F4(LegendaryCinematic *arg0) {
    s32 v1;
    s32 v0;
    s32 v2;
    s32 v3;
    s32 ev1;
    s32 v6;
    s32 v7;
    s32 v8;
    u32 v4;
    ArceusCinematic *v5;

    v5 = arg0->arceus;
    v7 = v5->blendDuration;
    v2 = arg0->frame;
    v1 = v2;
    if (v2 == v7) {
        ev1 = v5->blendTargetEv;
        G2_SetBlendAlpha(v5->blendPlane1, v5->blendPlane2, ev1, abs(0xF - ev1));
        arg0->frame = 0;
        return arg0->nextState;
    }
    v3 = v5->blendTargetEv;
    v6 = v5->blendStartEv;
    v0 = v3;
    v4 = (u32)(((s32)(abs(v6 - v3) << 8) / v7) * v1) >> 8;
    if (v6 > v3) {
        v8 = v6 - v4;
    } else {
        v8 = v4 + v6;
    }
    G2_SetBlendAlpha(v5->blendPlane1, v5->blendPlane2, v8, abs(0xF - v8));
    arg0->frame = (s32)(arg0->frame + 1);
    return 3;
}
