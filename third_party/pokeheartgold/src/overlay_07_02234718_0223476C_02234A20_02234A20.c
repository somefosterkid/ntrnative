#include "global.h"

#include "battle/battler_sprite_context.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "overlay_07.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "unk_02013FDC.h"
#include "wild_encounter.h"
#include "overlay_74_main_menu_util.h"

typedef struct SpriteAnimFrame SpriteAnimFrame;

struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
};

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void ov07_02234A20(BattlerSpriteContext *battlerSpriteCtx, enum HeapID heapID);

void ov07_02234A20(BattlerSpriteContext *battlerSpriteCtx, enum HeapID heapID) {
    s32 sp0[5];
    PokepicTemplate *temp_r0;
    s32 temp_r0_2;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r4;
    s32 var_r4;

    Pokepic_Push((Pokepic *)battlerSpriteCtx->sprites[battlerSpriteCtx->targetBattler]);
    *(u16 *)((u8 *)(&sp0[0]) + 4) = 0x72U;
    *(u16 *)((u8 *)(&sp0[0]) + 8) = 0x102U;
    *(s16 *)((u8 *)(&sp0[0]) + 0xA) = 0;
    *(s8 *)((u8 *)(&sp0[0]) + 0xC) = 0;
    sp0[4] = 0;
    temp_r1 = battlerSpriteCtx->targetBattler;
    if (battlerSpriteCtx->types[temp_r1] & 1) {
        *(s16 *)((u8 *)(&sp0[0]) + 6) = 0x101;
        var_r4 = 0x87;
    } else {
        *(s16 *)((u8 *)(&sp0[0]) + 6) = 0x100;
        var_r4 = 0x86;
    }
    temp_r0 = Pokepic_GetTemplate((Pokepic *)battlerSpriteCtx->sprites[temp_r1]);
    *temp_r0 = *(PokepicTemplate *)(&sp0[1]);
    Pokepic_ScheduleReloadFromNarc((Pokepic *)battlerSpriteCtx->sprites[battlerSpriteCtx->targetBattler]);
    sub_02014400((enum NarcId)temp_r0->narcID, (s32)temp_r0->charDataID, heapID, battlerSpriteCtx->pokemonSpriteData[battlerSpriteCtx->targetBattler]->tiles);
    battlerSpriteCtx->pokemonSpriteData[battlerSpriteCtx->targetBattler]->narcID = (s32) * (u16 *)((u8 *)(&sp0[0]) + 4);
    battlerSpriteCtx->pokemonSpriteData[battlerSpriteCtx->targetBattler]->palette = (s32) * (u16 *)((u8 *)(&sp0[0]) + 8);
    ReadWholeNarcMemberByIdPair(&sp0[0], NARC_poketool_pokegra_height_o, var_r4);
    battlerSpriteCtx->pokemonSpriteData[battlerSpriteCtx->targetBattler]->yOffset = (s32) * (u8 *)(&sp0[0]);
    temp_r0_2 = battlerSpriteCtx->targetBattler;
    temp_r4 = battlerSpriteCtx->pokemonSpriteData[temp_r0_2]->yOffset + ov07_02234B5C((s32)battlerSpriteCtx->types[temp_r0_2], 1);
    Pokepic_SetAttr((Pokepic *)battlerSpriteCtx->sprites[temp_r0_2], 1, temp_r4);
    temp_r1_2 = battlerSpriteCtx->targetBattler;
    if (battlerSpriteCtx->types[temp_r1_2] & 1) {
        Pokepic_SetAttr((Pokepic *)battlerSpriteCtx->sprites[temp_r1_2], 0x2E, 1);
        Pokepic_SetAttr((Pokepic *)battlerSpriteCtx->sprites[battlerSpriteCtx->targetBattler], 0x14, temp_r4 + (0x24 - *(u8 *)(&sp0[0])));
        Pokepic_SetAttr((Pokepic *)battlerSpriteCtx->sprites[battlerSpriteCtx->targetBattler], 0x15, 0);
        Pokepic_SetAttr((Pokepic *)battlerSpriteCtx->sprites[battlerSpriteCtx->targetBattler], 0x16, 0x24 - *(u8 *)(&sp0[0]));
        Pokepic_SetAttr((Pokepic *)battlerSpriteCtx->sprites[battlerSpriteCtx->targetBattler], 0x29, 0);
    }
}
