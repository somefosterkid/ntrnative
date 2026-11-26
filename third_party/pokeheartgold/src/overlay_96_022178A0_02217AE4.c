#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_96_sprite_res_man.h"

#include "overlay_96_ov96_rd_work.h"

s32 ov96_021E6108(void *arg0);

u32 _s32_div_f(s32, u32);

PokeathlonMonPerf *ov96_021E60C0(PokeathlonCourseData *data, u32 a, u32 b);

void ov96_021E6168(PokeathlonCourseData *data, u32 index, u32 a, void *out);

void ov96_02217AE4(Ov96RdWork *work, PokeathlonCourseData *param_2);

void ov96_02217AE4(Ov96RdWork *work, PokeathlonCourseData *param_2) {
    SpriteList *pSVar1;
    Ov96SpriteSet *uVar2;

    pSVar1 = SpriteManager_GetSpriteList(work->spriteManager);
    uVar2 = ov96_021EA854(work->heapId, 0xc, 7, work->resSrc, pSVar1);
    work->spriteSet = uVar2;
    {
        s32 sVar3;
        s32 iVar4;
        Ov96SpriteResDesc *puVar6;
        s32 quot;
        s32 rem;
        Ov96SpriteResDesc auStack_d8[12];
        Ov96SpriteResLoadParams uStack_11c = { 0 };
        iVar4 = 0;
        puVar6 = auStack_d8;
        do {
            rem = iVar4 % 3;
            quot = iVar4 / 3;
            ov96_021E6168(param_2, quot, rem, puVar6);
            sVar3 = ov96_021E6108((void *)ov96_021E60C0(param_2, quot, rem));
            uStack_11c.unk_14[iVar4] = sVar3;
            puVar6 = puVar6 + 1;
            iVar4 = iVar4 + 1;
        } while (iVar4 < 0xc);
        uStack_11c.unk_4 = 3;
        uStack_11c.vramType = 1;
        uStack_11c.screen = NNS_G2D_VRAM_TYPE_2DMAIN;
        ov96_021EA8A8(work->spriteSet, 0xc, auStack_d8, &uStack_11c, 0, 0);
    }
    return;
}
