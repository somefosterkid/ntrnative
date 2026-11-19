#include "global.h"
struct BallRotation;
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_07_ball_rotation.h"

s32 ov07_02221F04(s32, s32, s16 *, s16 *);

void ov07_022341A4(struct BallRotation *arg0, s16 *arg1, s16 *arg2);

s32 ov07_02232658(u32, u32);
void ov07_022342E4(struct BallRotation *param_1);

void ov07_022341A4(BallRotation *arg0, s16 *arg1, s16 *arg2) {
    u32 v0;

    v0 = *((u32 *)&arg0->unk_90.type);
    switch (v0) {
    case 0:
        ov07_02221F04(0, 0, arg1, arg2);
        *arg2 += 0x28;
        return;
    case 1:
        ov07_02221F04(0, 1, arg1, arg2);
        *arg2 += 0x26;
        return;
    case 2:
        ov07_02221F04(1, 2, arg1, arg2);
        *arg2 += 0x28;
        return;
    case 4:
        ov07_02221F04(1, 4, arg1, arg2);
        *arg2 += 0x28;
        return;
    case 3:
        ov07_02221F04(1, 3, arg1, arg2);
        *arg2 += 0x26;
        return;
    case 5:
        ov07_02221F04(1, 5, arg1, arg2);
        *arg2 += 0x26;
        return;
    case 6:
        *arg1 = 0xA;
        *arg2 = 0x64;
        return;
    case 7:
        *arg1 = 0xA;
        *arg2 = 0x64;
        return;
    case 8:
        *arg1 = 0xA;
        *arg2 = 0x64;
        return;
    case 9:
        ov07_02221F04(0, 0, arg1, arg2);
        return;
    case 10:
        ov07_02221F04(0, 0, arg1, arg2);
        return;
    case 11:
        ov07_02221F04(0, 0, arg1, arg2);
        return;
    case 12:
        ov07_02221F04(0, 0, arg1, arg2);
        return;
    case 13:
        ov07_02221F04(0, 0, arg1, arg2);
        return;
    case 14:
        *arg1 = 0xE6;
        *arg2 = 0x20;
        return;
    case 15:
    case 16:
    case 17:
        *arg1 = -0x1E;
        *arg2 = 0xA0;
        return;
    }
}

void ov07_022342E4(BallRotation *param_1) {
    G2dRenderer *renderer;
    s32 fileId;
    s32 fileId_00;
    s32 fileId_01;
    NARC *narc;
    s32 iVar1;
    SpriteResourceCountsListUnion *pSVar2;
    SpriteResourceCountsListUnion SStack_30;

    SpriteSystem_InitSprites(param_1->unk_90.spriteSystem, param_1->unk_2C, 10);
    if (param_1->unk_90.surface == 0) {
        renderer = SpriteSystem_GetRenderer(param_1->unk_90.spriteSystem);
        G2dRenderer_SetSubSurfaceCoords(renderer, 0, 0x110000);
    }
    iVar1 = 0;
    pSVar2 = &SStack_30;
    do {
        iVar1 = iVar1 + 1;
        pSVar2->numChar = 10;
        pSVar2 = (SpriteResourceCountsListUnion *)((u8 *)pSVar2 + 4);
    } while (iVar1 < 6);
    SStack_30.numMcel = 0;
    SStack_30.numManm = 0;
    SpriteSystem_InitManagerWithCapacities(param_1->unk_90.spriteSystem, param_1->unk_2C, &SStack_30);
    iVar1 = ov07_02232658(*((u32 *)&param_1->unk_90.ball), 0);
    fileId = ov07_02232658(*((u32 *)&param_1->unk_90.ball), 1);
    fileId_00 = ov07_02232658(*((u32 *)&param_1->unk_90.ball), 2);
    fileId_01 = ov07_02232658(*((u32 *)&param_1->unk_90.ball), 3);
    narc = NARC_New(NARC_a_0_0_8, *((enum HeapID *)&param_1->unk_90.heapID));
    SpriteSystem_LoadCharResObjFromOpenNarc(param_1->unk_90.spriteSystem, param_1->unk_2C, narc, iVar1, 1, 1, param_1->unk_90.target + 6000);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(param_1->unk_90.paletteData, PLTTBUF_MAIN_OBJ, param_1->unk_90.spriteSystem, param_1->unk_2C, narc, fileId, 0, 1, 1, param_1->unk_90.target + 6000);
    SpriteSystem_LoadCellResObjFromOpenNarc(param_1->unk_90.spriteSystem, param_1->unk_2C, narc, fileId_00, 1, param_1->unk_90.target + 6000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(param_1->unk_90.spriteSystem, param_1->unk_2C, narc, fileId_01, 1, param_1->unk_90.target + 6000);
    NARC_Delete(narc);
    return;
}
