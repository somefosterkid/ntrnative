#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_021E6454(void *param_1, u32 param_2);

void ov96_02216C38(Ov96RdSprites *sprites, u8 *param_2, PokeathlonCourseData *param_3);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_0221966C(struct Ov96DigitWork *param_1, u8 param_2, u16 param_3, s8 *param_4);
#else
void ov96_0221966C(struct Ov96DigitWork *param_1, u32 param_2, u32 param_3, s8 *param_4);
#endif

void ov96_022196E4(void *param_1, s32 param_2);

u32 ov96_02216C1C(u8 *param_1, u8 *param_2);

void ov96_0221768C(Ov96RdSprites *sprites, PokeathlonCourseData *param_2);

void ov96_0221768C(Ov96RdSprites *sprites, PokeathlonCourseData *param_2) {
    u8 *pbVar1;
    u8 *puVar2;
    u8 *pbVar3;
    Ov96RdWork *work;
    BOOL BVar5;
    s32 iVar6;
    PokeathlonCourseData *pPVar7;
    u8 *pbVar8;
    u8 *pbVar9;
    s32 bVar10;
    u32 uVar12;
    s32 unaff_r5;
    u8 abStack_1c[8];

    puVar2 = PokeathlonCourse_GetDataCopyArea(param_2);
    pbVar3 = ov96_021E8A20(puVar2 + 0xf0);
    work = PokeathlonCourse_GetHeapAllocPtr4(param_2);
    pPVar7 = param_2;
    ov96_02216C38(sprites, pbVar3, param_2);
    ov96_021E6454((u8 *)param_2, ((u32)(*(u32 *)(pbVar3 + 0x20) << 13)) >> 16);
    if (((u32)(*(u32 *)(pbVar3 + 0x20) << 13)) >> 16 == 0x1c2) {
        PlaySE(0x6d7);
    }
    BVar5 = ov96_021E5F24(param_2);
    bVar10 = ((u32)(*(pbVar3 + BVar5 * 6 + 5) << 29)) >> 29;
    switch (bVar10) {
    case 0:
        unaff_r5 = 2;
        break;
    case 3:
        unaff_r5 = 3;
        break;
    case 2:
        unaff_r5 = 4;
        break;
    case 4:
        unaff_r5 = 5;
        break;
    case 1:
        unaff_r5 = 6;
        break;
    default:
        GF_AssertFail();
    }
    ManagedSprite_SetAnimNoRestart(sprites->sprite, unaff_r5);
    if (bVar10 != sprites->mode) {
        if (bVar10 == 2) {
            PlaySE(0x89b);
        } else if (bVar10 == 4) {
            PlaySE(0x89c);
        }
    }
    sprites->mode = bVar10;
    iVar6 = 0;
    pbVar8 = abStack_1c;
    pbVar9 = pbVar3;
    do {
        pbVar8[1] = (u8)iVar6;
        pbVar1 = pbVar9 + 3;
        iVar6 = iVar6 + 1;
        *pbVar8 = *pbVar1;
        pbVar9 = pbVar9 + 6;
        pbVar8 = pbVar8 + 2;
    } while (iVar6 < 4);
    MATH_QSort(abStack_1c, 4, 2, (MATHCompareFunc)(ov96_02216C1C), (void *)0x0);
    uVar12 = 0;
    do {
        if ((u32)pbVar3[uVar12 * 6 + 3] != (u32)sprites->stat[uVar12]) {
            ov96_0221966C(sprites->unk_0, uVar12 & 0xff, (u32)pbVar3[uVar12 * 6 + 3], (s8 *)abStack_1c);
        }
        sprites->stat[uVar12] = (u16)pbVar3[uVar12 * 6 + 3];
        uVar12 = uVar12 + 1;
    } while ((s32)uVar12 < 4);
    iVar6 = (((u32)(*(u32 *)(pbVar3 + 0x20) << 13)) >> 16) / 30;
    ov96_022196E4(work->unk_180, iVar6);
    return;
}
