#ifndef POKEHEARTGOLD_OVERLAY_12_MON_RETURN_DATA_H
#define POKEHEARTGOLD_OVERLAY_12_MON_RETURN_DATA_H

#include "battle/battle.h"
#include "battle/battle_message_structs.h"
#include "overlay_07_ball_rotation.h"
#include "pokepic.h"

// The switch-out (recall) task data: built by ov12_02259F30 from the OpponentData and the
// MonReturnMessage, run by ov12_0225C9BC (own battler) and ov12_0225CC58 (the other one).
// moveAnim is only filled in the species / genders / isShiny / formNums / personalities
// slots (ov12_02261CA8 reads them to draw the battle mon's sprite).
typedef struct MonReturnData {
    BattleSystem *battleSys;
    OpponentData *battlerData;
    Pokepic *monSprite;
    BallRotation *ballRotation;
    MoveAnimation moveAnim;
    u8 command;
    u8 battler;
    u8 battlerType;
    u8 state;
    u8 yOffset;
    u8 unused_6D;
    u16 capturedBall;
    int isSubstitute;
    u32 unk74; // MonReturnMessage.unk2C: compared with ov12_0223C140(battler)
} MonReturnData;

// sizeof(MonReturnData) == 0x78 on the DS

#endif // POKEHEARTGOLD_OVERLAY_12_MON_RETURN_DATA_H
