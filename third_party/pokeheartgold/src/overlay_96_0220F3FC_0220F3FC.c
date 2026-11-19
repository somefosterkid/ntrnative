#include "global.h"

#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_96_sprite_res_man.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

s32 ov96_021E6108(void *arg0);

u32 _s32_div_f(s32, u32);

void *ov96_021E60C0(PokeathlonCourseData *, u32, u32);

void ov96_021E6168(PokeathlonCourseData *, u32, u32, void *);

void ov96_021EB138(Ov96SpriteSet *set);

void ov96_0220F3FC(UnkStruct_ov96_0220E3F0 *param_1);

void ov96_0220F3FC(UnkStruct_ov96_0220E3F0 *param_1) {
    SpriteList *pSVar1;
    PokeathlonCourseData *uVar2;
    Ov96SpriteSet *pvVar8;
    u32 uVar3;
    u32 uVar7;
    void *arg0;
    s32 sVar4;
    s32 iVar5;
    Ov96SpriteResDesc *puVar7;

    pSVar1 = SpriteManager_GetSpriteList(param_1->spriteManager);
    pvVar8 = ov96_021EA854(param_1->heapId, 0xc, 7, param_1->unk_1C, pSVar1);
    param_1->unk_20 = pvVar8;
    ov96_021EB138(pvVar8);
    {
        Ov96SpriteResDesc auStack_d8[12];
        Ov96SpriteResLoadParams uStack_11c = { 0 };
        iVar5 = 0;
        puVar7 = auStack_d8;
        uVar2 = param_1->course;
        do {
            uVar7 = iVar5 % 3;
            uVar3 = iVar5 / 3;
            ov96_021E6168(uVar2, uVar3, uVar7, puVar7);
            arg0 = ov96_021E60C0(uVar2, uVar3, uVar7);
            sVar4 = ov96_021E6108(arg0);
            uStack_11c.unk_14[iVar5] = sVar4;
            puVar7 = puVar7 + 1;
            iVar5 = iVar5 + 1;
        } while (iVar5 < 0xc);
        uStack_11c.unk_4 = 1;
        uStack_11c.vramType = 1;
        uStack_11c.screen = NNS_G2D_VRAM_TYPE_2DMAIN;
        ov96_021EA8A8(param_1->unk_20, 0xc, auStack_d8, &uStack_11c, 0, 0);
    }
    return;
}
