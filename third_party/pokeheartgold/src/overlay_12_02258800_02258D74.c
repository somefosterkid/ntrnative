#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "overlay_12_02258800.h"
#include "battle/battle.h"
#include "battle/battle_system.h"
#include "bg_window.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_ball_rotation.h"


struct UnkBallData;
struct BattleMonOBJData;
struct PokemonSprite;
struct PokemonSpriteTransforms;
typedef struct PokemonSprite PokemonSprite;
typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;
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
struct PokemonSprite {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokepicTemplate template;
    PokepicTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokepicShadow shadow;
    PokepicShadow shadowBackup;
    PokepicAnimScript animFrames[10];
};
typedef struct BattleMonOBJData BattleMonOBJData;
enum BallThrowMode {
    BALL_THROW_MODE_SEND_OUT = 0,
    BALL_THROW_MODE_SEND_OUT_QUICK,
    BALL_THROW_MODE_UNUSED,
    BALL_THROW_MODE_THROW,
    BALL_THROW_MODE_TRAINER_SEND_OUT,
    BALL_THROW_MODE_RECALL,
};
UnkBallData *ov07_02233DB8(BallThrow *param0);
void ov07_0223449C(UnkBallData *param0, int param1);
void ov07_022344C0(UnkBallData *param0, BOOL param1);
void ov07_022344C4(UnkBallData *param0, u16 param1);
void ov07_022344D0(UnkBallData *param0, u16 param1);
extern const u8 ov12_0226D120[];
void ov12_02258DB0(BattleSystem *battleSys, OpponentData *battlerData, int ballID, int unused);

void *ov12_02258D74(u32 param_1, u8 *param_2) {
    OpponentData *destp;
    NARC *pNVar1;

    destp = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentData));
    MIi_CpuClearFast(0, destp, sizeof(OpponentData));
    destp->battler = *param_2;
    destp->battlerType = param_2[1];
    pNVar1 = NARC_New(NARC_a_1_8_0, HEAP_ID_BATTLE);
    destp->narc = pNVar1;
    return destp;
}

void ov12_02258DB0(BattleSystem *battleSys, OpponentData *battlerData, int ballID, int unused) {
    BallThrow ballThrow;

    if (BattleSystem_GetBattleType(battleSys) & ((1 << 5) | (1 << 9))) {
        return;
    }

    if ((battlerData->battlerType & 1) && (BattleSystem_GetBattleType(battleSys) & (1 << 0)) == 0) {
        return;
    }

    ballThrow.type = ov12_0226D120[battlerData->battlerType];
    ballThrow.heapID = HEAP_ID_BATTLE;
    ballThrow.mode = BALL_THROW_MODE_TRAINER_SEND_OUT;
    ballThrow.target = battlerData->battler;
    ballThrow.ball = ballID;
    ballThrow.spriteSystem = BattleSystem_GetSpriteSystem(battleSys);
    ballThrow.paletteData = BattleSystem_GetPaletteData(battleSys);
    ballThrow.surface = 0;
    ballThrow.bgPrio = 1;

    battlerData->ballRotation = ov07_02233DB8(&ballThrow);

    ov07_022344C4(battlerData->ballRotation, 100);
    ov07_022344D0(battlerData->ballRotation, 2);
    ov07_0223449C(battlerData->ballRotation, 0);
    ov07_022344C0(battlerData->ballRotation, 0);
}
