#include "global.h"

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_shadow_punch_context.h"

#include "obj_pltt_transfer.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "bg_window.h"
#include "heap.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"

struct BattleAnimSystem;

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

s32 ov07_0221FAE8(BattleAnimSystem *system);

s32 ov07_02231E08(BattleAnimSystem *system, s32 spriteAlpha, s32 bgAlpha);

void ov07_0222AFAC(SysTask *task, void *param);

void ov07_0222B130(BattleAnimSystem *system);

void ov07_0222A8D8(ManagedSprite *sprite);
typedef struct FrenzyPlantSpriteContext {
    BattleAnimScriptFuncCommon common;
    s16 unused;
    s16 lifetime[8];
    ManagedSprite *sprites[8];
    XYTransformContext pos;
} FrenzyPlantSpriteContext;
void ov07_0222B2A8(SysTask *task, void *param);

void ov07_0222B2A8(SysTask *task, void *param);

void ov07_0222B130(BattleAnimSystem *system) {
    PaletteData *v0;
    PaletteData *paletteData;
    enum NarcId narcId;
    ManagedSprite *managedSprite;
    ManagedSprite *v3;
    PaletteData *data;
    PaletteData *v2;
    Pokepic *pokepic;
    s32 memberNo;
    u32 v4;
    u32 v5;
    ShadowPunchContext *v1;

    v1 = ov07_022324D8(system, sizeof(ShadowPunchContext));
    ov07_02231FE4(system, &v1->common);
    v1->unused = 0;
    v1->spriteAlpha = 8U;
    v1->bgAlpha = 8U;
    ov07_02231E08(v1->common.battleAnimSys, -1, -1);
    *(s16 *)REG_BLDALPHA_ADDR = v1->spriteAlpha | (v1->bgAlpha << 8);
    pokepic = ov07_0221FA48(v1->common.battleAnimSys, ov07_0221C468(v1->common.battleAnimSys));
    v1->spriteInfo.monSprite = pokepic;
    v1->spriteInfo.pos.x = (s16)Pokepic_GetAttr(pokepic, 0);
    v1->spriteInfo.pos.y = (s16)Pokepic_GetAttr(v1->spriteInfo.monSprite, 1);
    v1->spriteInfo.hwSprite = ov07_0221C4E8(v1->common.battleAnimSys, 0);
    v1->sprite = ov07_0221C4E8(v1->common.battleAnimSys, 1);
    v1->attackerHeight = (s16)(0 - Pokepic_GetAttr(v1->spriteInfo.monSprite, 0x29));
    memberNo = ov07_0221FA80(v1->common.battleAnimSys, ov07_0221C468(v1->common.battleAnimSys));
    narcId = ov07_0221FA90(v1->common.battleAnimSys, ov07_0221C468(v1->common.battleAnimSys));
    managedSprite = v1->spriteInfo.hwSprite;
    ManagedSprite_SetPriority(managedSprite, ov07_0221FAE8(v1->common.battleAnimSys) + 1);
    v4 = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(managedSprite->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    data = ov07_0221FA78(v1->common.battleAnimSys);
    paletteData = data;
    PaletteData_LoadNarc_CustomTint(data, narcId, memberNo, ov07_0221BFD0(v1->common.battleAnimSys), PLTTBUF_MAIN_OBJ, 0x20U, (u16)(u16)(v4 * 0x10), 0xC4, 0xC4, 0xC4);
    ManagedSprite_SetOamMode(managedSprite, GX_OAM_MODE_XLU);
    v3 = v1->sprite;
    ManagedSprite_SetPriority(v3, ov07_0221FAE8(v1->common.battleAnimSys) + 1);
    v5 = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(v3->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    v2 = ov07_0221FA78(v1->common.battleAnimSys);
    v0 = v2;
    PaletteData_LoadNarc_CustomTint(v2, narcId, memberNo, ov07_0221BFD0(v1->common.battleAnimSys), PLTTBUF_MAIN_OBJ, 0x20U, (u16)(u16)(v5 * 0x10), 0xC4, 0xC4, 0xC4);
    ManagedSprite_SetOamMode(v3, GX_OAM_MODE_XLU);
    ov07_0221C410(v1->common.battleAnimSys, ov07_0222AFAC, v1);
}

void ov07_0222B2A8(SysTask *task, void *param) {
    FrenzyPlantSpriteContext *ctx = param;

    int i;
    int invisibleRoots = 0;

    for (i = 0; i < 8; i++) {
        ctx->lifetime[i]++;

        if (ctx->lifetime[i] < 32) {
            continue;
        }

        ov07_0222A8D8(ctx->sprites[i]);

        if (ctx->lifetime[i] >= 160) {
            ManagedSprite_SetDrawFlag(ctx->sprites[i], 0);
            invisibleRoots++;
        }
    }

    if (invisibleRoots == 8) {
        for (i = 0; i < 8; i++) {
            Sprite_DeleteAndFreeResources(ctx->sprites[i]);
        }

        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->common.primarySpriteManager);
}
