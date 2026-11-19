#include "global.h"

#include "battle/battler_sprite_context.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_controller.h"
#include "battle/battle_message_structs.h"
#include "battle/battle_system.h"

#include "filesystem.h"
#include "heap.h"
#include "overlay_12_battle_controller_opponent.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02016EDC.h"
#include "wild_encounter.h"
#include "overlay_12_mon_show_data.h"
#include "overlay_74_main_menu_util.h"

typedef struct SpriteAnimFrame SpriteAnimFrame;

typedef struct PokemonSprite PokemonSprite;

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef struct PokemonSpriteShadow PokemonSpriteShadow;

typedef struct PokemonSpriteManager PokemonSpriteManager;

struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
};

typedef void(PokemonSpriteCallback)(PokemonSprite *, PokemonSpriteTransforms *);

struct PokemonSpriteTransforms {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};

struct PokemonSpriteShadow {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
};

struct PokemonSprite {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokemonSpriteTemplate template;
    PokemonSpriteTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow shadow;
    PokemonSpriteShadow shadowBackup;
    SpriteAnimFrame animFrames[10];
};

struct PokemonSpriteManager {
    PokemonSprite sprites[4];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy plttProxy;
    enum HeapID heapID;
    u32 charBaseAddr;
    u32 charSize;
    u32 plttBaseAddr;
    u32 plttSize;
    u8 *charRawData;
    u16 *plttRawData;
    u16 *plttRawDataUnfaded;
    NNSG2dCharacterData charData;
    NNSG2dPaletteData plttData;
    u8 dummy330;
    u8 needLoadChar;
    u8 needLoadPltt;
    u8 excludeIdentity;
    u32 hideShadows;
};

s32 ov07_0221C394(void *arg0);

s32 ov07_0221C3B0(void *arg0);

s32 ov07_0221C3C0(void *arg0);

void ov07_02234A20(BattlerSpriteContext *battlerSpriteCtx, enum HeapID heapID);

Pokepic *ov12_022612A4(BattleSystem *battleSys, PokepicManager *monSpriteMan, void *spriteTemplate, int x, int y, int z, int yOffset, int height, int xOffset, int shadowSize, int battler, SpriteAnimFrame *animFrames, PokemonSpriteCallback *callback);

s32 ov12_02261B80(BattleSystem *, void *, u32 *, MoveAnimation *);

s32 ov12_02261CA8(BattleSystem *, void *, BattlerSpriteContext *, int);

extern const s16 ov07_022377F4[][6][3];

void ov12_0225C6C8(SysTask *arg0, void *arg1);

void ov12_0225C6C8(SysTask *arg0, void *arg1) {
    SpriteAnimFrame sp24[10];
    MoveAnimation sp1A4;
    MoveAnimation sp14C;
    MoveAnimation spF4;
    BattlerSpriteContext spA4;
    MoveAnimation sp4C;
    PokepicManager *temp_r5_2;
    u32 *temp_r0;
    u32 *temp_r5;
    u8 temp_r1;
    u8 temp_r1_2;
    u8 temp_r1_3;
    u8 temp_r1_4;
    u8 temp_r1_5;
    OpponentData *temp_r3_2;

    temp_r0 = ov12_0223A8DC(*(BattleSystem **)arg1);
    temp_r1 = ((MonShowData *)arg1)->state;
    temp_r5 = temp_r0;
    switch ((u32)temp_r1) {
    case 0:
        temp_r5_2 = BattleSystem_GetPokepicManager(*(BattleSystem **)arg1);
        NARC_ReadPokepicAnimScript(((MonShowData *)arg1)->battlerData->narc, (PokepicAnimScript *)sp24, ((MonShowData *)arg1)->species, (u16) ((MonShowData *)arg1)->battlerType);

        ((MonShowData *)arg1)->battlerData->monSprite = ov12_022612A4(*(BattleSystem **)arg1, temp_r5_2, &((MonShowData *)arg1)->spriteTemplate, ov07_022377F4[1][((MonShowData *)arg1)->battlerType][0], ov07_022377F4[1][((MonShowData *)arg1)->battlerType][1], ov07_022377F4[0][((MonShowData *)arg1)->battlerType][2], (s32) ((MonShowData *)arg1)->yOffset, (s32) ((MonShowData *)arg1)->height, (s32) ((MonShowData *)arg1)->shadowXOffset, (s32) ((MonShowData *)arg1)->shadowSize, (s32) ((MonShowData *)arg1)->battler, sp24, NULL);
        Pokepic_SetAttr(((MonShowData *)arg1)->battlerData->monSprite, 0xC, 0x100);
        Pokepic_SetAttr(((MonShowData *)arg1)->battlerData->monSprite, 0xD, 0x100);
        Pokepic_SetAttr(((MonShowData *)arg1)->battlerData->monSprite, 0x2C, 0);
        Pokepic_SetAttr(((MonShowData *)arg1)->battlerData->monSprite, 6, 0);
        ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
        return;
    case 1:
        temp_r1_2 = ((MonShowData *)arg1)->battler;
        BattleController_SetMoveAnimation(*(BattleSystem **)arg1, NULL, &sp1A4, 1, 0x29, (s32)temp_r1_2, (s32)temp_r1_2, 0U);
        ov12_02261B80(*(BattleSystem **)arg1, ((MonShowData *)arg1)->battlerData, temp_r5, &sp1A4);
        ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
        return;
    case 3:
        if (((MonShowData *)arg1)->face == 2) {
            Pokepic_SetAttr(((MonShowData *)arg1)->battlerData->monSprite, 0x2D, 0);
        }
        temp_r3_2 = ((MonShowData *)arg1)->battlerData;
        ov12_02261F38(*(BattleSystem **)arg1, (s32) ((MonShowData *)arg1)->battler, (u32) ((MonShowData *)arg1)->battlerType, temp_r3_2->monSprite, temp_r3_2->narc, (s32) ((MonShowData *)arg1)->species, (s32) ((MonShowData *)arg1)->formNum, (s32) ((MonShowData *)arg1)->face, ((MonShowData *)arg1)->cryMod);
        ((MonShowData *)arg1)->state = 4U;
        return;
    case 4:
        if ((sub_02017068(ov12_0223B750(*(BattleSystem **)arg1), (s32) ((MonShowData *)arg1)->battler) != 1) || (Pokepic_IsAnimActive(((MonShowData *)arg1)->battlerData->monSprite) != 0)) {
            return;
        }
        if (((MonShowData *)arg1)->isShiny != 0) {
            temp_r1_3 = ((MonShowData *)arg1)->battler;
            BattleController_SetMoveAnimation(*(BattleSystem **)arg1, NULL, &sp14C, 1, 0xB, (s32)temp_r1_3, (s32)temp_r1_3, 0U);
            ov12_02261B80(*(BattleSystem **)arg1, ((MonShowData *)arg1)->battlerData, temp_r5, &sp14C);
            ((MonShowData *)arg1)->state = 5U;
            return;
        }
        ((MonShowData *)arg1)->state = 6U;
        return;
    case 2:
    case 5:
    case 7:
    case 9:
        ov07_0221C394(temp_r0);
        if (ov07_0221C3B0(temp_r5) == 0) {
            ov07_0221C3C0(temp_r5);
            ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
            return;
        }
        return;
    case 6:
        if (*((s32 *)&((MonShowData *)arg1)->isSubstitute) != 0) {
            temp_r1_4 = ((MonShowData *)arg1)->battler;
            BattleController_SetMoveAnimation(*(BattleSystem **)arg1, NULL, &spF4, 1, 0xF, (s32)temp_r1_4, (s32)temp_r1_4, 0U);
            ov12_02261B80(*(BattleSystem **)arg1, ((MonShowData *)arg1)->battlerData, temp_r5, &spF4);
            ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
            return;
        }
        ((MonShowData *)arg1)->state = 0xFFU;
        return;
    case 8:
        ov12_02261CA8(*(BattleSystem **)arg1, &((MonShowData *)arg1)->moveAnim, &spA4, ((MonShowData *)arg1)->battler);
        ov07_02234A20(&spA4, HEAP_ID_BATTLE);
        temp_r1_5 = ((MonShowData *)arg1)->battler;
        BattleController_SetMoveAnimation(*(BattleSystem **)arg1, NULL, &sp4C, 1, 0x10, (s32)temp_r1_5, (s32)temp_r1_5, 0U);
        ov12_02261B80(*(BattleSystem **)arg1, ((MonShowData *)arg1)->battlerData, temp_r5, &sp4C);
        ((MonShowData *)arg1)->battlerData->isAlternateSprite = 1;
        ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
        return;
    default:
        BattleController_EmitClearCommand(*(BattleSystem **)arg1, (s32) ((MonShowData *)arg1)->battler, (s32) ((MonShowData *)arg1)->command);
        Heap_Free(arg1);
        SysTask_Destroy(arg0);
        break;
    }
}
