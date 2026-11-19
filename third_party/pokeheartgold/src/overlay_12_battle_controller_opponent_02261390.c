#include "global.h"

#include "battle/battle.h"
#include "battle/battle_message_structs.h"
#include "battle/battle_system.h"

#include "bg_window.h"
#include "filesystem.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sys_task.h"
#include "constants/heap.h"
#include "overlay_12_battle_controller_opponent.h"
#include "palette.h"
#include "sprite_system.h"
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

enum BattleMessageTag {
    TAG_NONE = 0,
    TAG_NONE_SIDE_CONSCIOUS = 1,
    TAG_NICKNAME = 2,
    TAG_MOVE = 3,
    TAG_STAT = 4,
    TAG_ITEM = 5,
    TAG_NUMBER = 6,
    TAG_NUMBERS = 7,
    TAG_TRNAME = 8,
    TAG_NICKNAME_NICKNAME = 9,
    TAG_NICKNAME_MOVE = 10,
    TAG_NICKNAME_ABILITY = 11,
    TAG_NICKNAME_STAT = 12,
    TAG_NICKNAME_TYPE = 13,
    TAG_NICKNAME_POKE = 14,
    TAG_NICKNAME_ITEM = 15,
    TAG_NICKNAME_POFFIN = 16,
    TAG_NICKNAME_NUM = 17,
    TAG_NICKNAME_TRNAME = 18,
    TAG_NICKNAME_BOX = 19,
    TAG_MOVE_SIDE = 20,
    TAG_MOVE_NICKNAME = 21,
    TAG_MOVE_MOVE = 22,
    TAG_ABILITY_NICKNAME = 23,
    TAG_ITEM_MOVE = 24,
    TAG_NUMBER_NUMBER = 25,
    TAG_TRNAME_TRNAME = 26,
    TAG_TRNAME_NICKNAME = 27,
    TAG_TRNAME_ITEM = 28,
    TAG_TRNAME_NUM = 29,
    TAG_TRCLASS_TRNAME = 30,
    TAG_NICKNAME_NICKNAME_MOVE = 31,
    TAG_NICKNAME_NICKNAME_ABILITY = 32,
    TAG_NICKNAME_NICKNAME_ITEM = 33,
    TAG_NICKNAME_MOVE_MOVE = 34,
    TAG_NICKNAME_MOVE_NUMBER = 35,
    TAG_NICKNAME_ABILITY_NICKNAME = 36,
    TAG_NICKNAME_ABILITY_MOVE = 37,
    TAG_NICKNAME_ABILITY_ITEM = 38,
    TAG_NICKNAME_ABILITY_STAT = 39,
    TAG_NICKNAME_ABILITY_TYPE = 40,
    TAG_NICKNAME_ABILITY_STATUS = 41,
    TAG_NICKNAME_ABILITY_NUMBER = 42,
    TAG_NICKNAME_ITEM_NICKNAME = 43,
    TAG_NICKNAME_ITEM_MOVE = 44,
    TAG_NICKNAME_ITEM_STAT = 45,
    TAG_NICKNAME_ITEM_STATUS = 46,
    TAG_NICKNAME_BOX_BOX = 47,
    TAG_ITEM_NICKNAME_FLAVOR = 48,
    TAG_TRNAME_NICKNAME_NICKNAME = 49,
    TAG_TRCLASS_TRNAME_NICKNAME = 50,
    TAG_TRCLASS_TRNAME_ITEM = 51,
    TAG_NICKNAME_ABILITY_NICKNAME_MOVE = 52,
    TAG_NICKNAME_ABILITY_NICKNAME_ABILITY = 53,
    TAG_NICKNAME_ABILITY_NICKNAME_STAT = 54,
    TAG_NICKNAME_ITEM_NICKNAME_ITEM = 55,
    TAG_TRNAME_NICKNAME_TRNAME_NICKNAME = 56,
    TAG_TRCLASS_TRNAME_NICKNAME_NICKNAME = 57,
    TAG_TRCLASS_TRNAME_NICKNAME_TRNAME = 58,
    TAG_TRCLASS_TRNAME_TRCLASS_TRNAME = 59,
    TAG_TRCLASS_TRNAME_NICKNAME_TRCLASS_TRNAME_NICKNAME = 60,
};

typedef struct RecallMsgMessage {
    u8 command;
    u8 partySlot;
    u16 hpPercent;
} RecallMsgMessage;

void ov12_02261390(BattleSystem *battleSys, OpponentData *battlerData, RecallMsgMessage *message, BattleMessage *battleMsg);

void ov12_02261AD4(BattleSystem *arg0, OpponentData *arg1, BattleMessage *arg2);

void ov12_02261390(BattleSystem *battleSys, OpponentData *battlerData, RecallMsgMessage *message, BattleMessage *battleMsg) {
    if (battlerData->battlerType & 1) {
        if (BattleSystem_GetBattleType(battleSys) & (1 << 2)) {
            battleMsg->id = 990;
            battleMsg->tag = TAG_TRNAME_NICKNAME;
            battleMsg->param[0] = battlerData->battler;
            battleMsg->param[1] = battlerData->battler | (message->partySlot << 8);
        } else {
            battleMsg->id = 989;
            battleMsg->tag = TAG_TRCLASS_TRNAME_NICKNAME;
            battleMsg->param[0] = battlerData->battler;
            battleMsg->param[1] = battlerData->battler;
            battleMsg->param[2] = battlerData->battler | (message->partySlot << 8);
        }
    } else {
        if ((BattleSystem_GetBattleType(battleSys) & (1 << 1)) == 0 && (BattleSystem_GetBattleType(battleSys) & (1 << 2)) == 0) {
            if (message->hpPercent == 0) {
                battleMsg->id = 984;
            } else if (message->hpPercent < 25) {
                battleMsg->id = 985;
            } else if (message->hpPercent < 50) {
                battleMsg->id = 988;
            } else if (message->hpPercent < 75) {
                battleMsg->id = 986;
            } else {
                battleMsg->id = 987;
            }
        } else {
            battleMsg->id = 985;
        }

        battleMsg->tag = TAG_NICKNAME;
        battleMsg->param[0] = battlerData->battler | (message->partySlot << 8);
    }
}

void ov12_02261464(BattleSystem *battleSys, OpponentData *battlerData, SendOutMsgMessage *message, BattleMessage *battleMsg);

void ov12_02261464(BattleSystem *battleSys, OpponentData *battlerData, SendOutMsgMessage *message, BattleMessage *battleMsg) {
    if (battlerData->battlerType & 1) {
        if ((BattleSystem_GetBattleType(battleSys) & (1 << 2)) == 0) {
            battleMsg->id = 972;
            battleMsg->tag = TAG_TRCLASS_TRNAME_NICKNAME;
            battleMsg->param[0] = battlerData->battler;
            battleMsg->param[1] = battlerData->battler;
            battleMsg->param[2] = battlerData->battler | (message->partySlot << 8);
        } else {
            battleMsg->id = 974;
            battleMsg->tag = TAG_TRNAME_NICKNAME;
            battleMsg->param[0] = battlerData->battler;
            battleMsg->param[1] = battlerData->battler | (message->partySlot << 8);
        }
    } else {
        if ((BattleSystem_GetBattleType(battleSys) & (1 << 1)) == 0 && (BattleSystem_GetBattleType(battleSys) & (1 << 2)) == 0) {
            if (message->hpPercent < 100) {
                battleMsg->id = 982;
            } else if (message->hpPercent < 325) {
                battleMsg->id = 983;
            } else if (message->hpPercent < 550) {
                battleMsg->id = 981;
            } else if (message->hpPercent < 775) {
                battleMsg->id = 980;
            } else {
                battleMsg->id = 979;
            }
        } else {
            battleMsg->id = 979;
        }

        battleMsg->tag = TAG_NICKNAME;
        battleMsg->param[0] = battlerData->battler | (message->partySlot << 8);
    }
}

void ov12_02261544(BattleSystem *battleSys, OpponentData *unused, BattleMessage *battleMsg);

void ov12_02261544(BattleSystem *battleSys, OpponentData *unused, BattleMessage *battleMsg) {
    u32 battleType = BattleSystem_GetBattleType(battleSys);
    int battler1;
    int battler2;

    if (battleType & (1 << 1)) {
        battler1 = BattleSystem_GetBattlerFromBattlerType(battleSys, 3);
        battler2 = BattleSystem_GetBattlerFromBattlerType(battleSys, 5);
    } else {
        battler1 = BattleSystem_GetBattlerFromBattlerType(battleSys, 1);
        battler2 = battler1;
    }

    if (battleType & (1 << 2)) {
        if (battleType & (1 << 7)) {
            battleMsg->id = 992;
            battleMsg->tag = TAG_TRCLASS_TRNAME_TRCLASS_TRNAME;
            battleMsg->param[0] = battler1;
            battleMsg->param[1] = battler1;
            battleMsg->param[2] = battler2;
            battleMsg->param[3] = battler2;
        } else if (battleType & (1 << 3)) {
            battleMsg->id = 971;
            battleMsg->tag = TAG_TRNAME_TRNAME;
            battleMsg->param[0] = battler1;
            battleMsg->param[1] = battler2;
        } else {
            battleMsg->id = 970;
            battleMsg->tag = TAG_TRNAME;
            battleMsg->param[0] = battler1;
        }
    } else {
        if (battleType & (1 << 4) || battleType & (1 << 3)) {
            battleMsg->id = 992;
            battleMsg->tag = TAG_TRCLASS_TRNAME_TRCLASS_TRNAME;
            battleMsg->param[0] = battler1;
            battleMsg->param[1] = battler1;
            battleMsg->param[2] = battler2;
            battleMsg->param[3] = battler2;
        } else {
            battleMsg->id = 969;
            battleMsg->tag = TAG_TRCLASS_TRNAME;
            battleMsg->param[0] = battler1;
            battleMsg->param[1] = battler1;
        }
    }
}

void ov12_022615F0(BattleSystem *battleSys, OpponentData *battlerData, LeadMonMsgMessage *message, BattleMessage *battleMsg);

void ov12_022615F0(BattleSystem *battleSys, OpponentData *battlerData, LeadMonMsgMessage *message, BattleMessage *battleMsg) {
    u32 battleType = BattleSystem_GetBattleType(battleSys);
    int battler1;
    int battler2;

    if (battlerData->battlerType & 1) {
        if (battleType & (1 << 1)) {
            battler1 = battlerData->battler;
            battler2 = BattleSystem_GetBattlerIdPartner(battleSys, battlerData->battler);
        } else {
            battler1 = battlerData->battler;
            battler2 = battler1;
        }

        if (battleType & (1 << 2)) {
            if (battleType & (1 << 7)) {
                battleMsg->id = 991;
                battleMsg->tag = TAG_TRCLASS_TRNAME_NICKNAME_TRCLASS_TRNAME_NICKNAME;
                battleMsg->param[0] = battler1;
                battleMsg->param[1] = battler1;
                battleMsg->param[2] = battler1 | (message->partySlot[battler1] << 8);
                battleMsg->param[3] = battler2;
                battleMsg->param[4] = battler2;
                battleMsg->param[5] = battler2 | (message->partySlot[battler2] << 8);
            } else if (battleType & (1 << 3)) {
                battleMsg->id = 976;
                battleMsg->tag = TAG_TRNAME_NICKNAME_TRNAME_NICKNAME;
                battleMsg->param[0] = battler1;
                battleMsg->param[1] = battler1 | (message->partySlot[battler1] << 8);
                battleMsg->param[2] = battler2;
                battleMsg->param[3] = battler2 | (message->partySlot[battler2] << 8);
            } else if (battleType & (1 << 1)) {
                battleMsg->id = 975;
                battleMsg->tag = TAG_TRNAME_NICKNAME_NICKNAME;
                battleMsg->param[0] = battler1;
                battleMsg->param[1] = battler1 | (message->partySlot[battler1] << 8);
                battleMsg->param[2] = battler2 | (message->partySlot[battler2] << 8);
            } else {
                battleMsg->id = 974;
                battleMsg->tag = TAG_TRNAME_NICKNAME;
                battleMsg->param[0] = battler1;
                battleMsg->param[1] = battler1 | (message->partySlot[battler1] << 8);
            }
        } else {
            if (battleType & (1 << 4) || battleType & (1 << 3)) {
                battleMsg->id = 991;
                battleMsg->tag = TAG_TRCLASS_TRNAME_NICKNAME_TRCLASS_TRNAME_NICKNAME;
                battleMsg->param[0] = battler1;
                battleMsg->param[1] = battler1;
                battleMsg->param[2] = battler1 | (message->partySlot[battler1] << 8);
                battleMsg->param[3] = battler2;
                battleMsg->param[4] = battler2;
                battleMsg->param[5] = battler2 | (message->partySlot[battler2] << 8);
            } else if (battleType & (1 << 1)) {
                battleMsg->id = 973;
                battleMsg->tag = TAG_TRCLASS_TRNAME_NICKNAME_NICKNAME;
                battleMsg->param[0] = battler1;
                battleMsg->param[1] = battler1;
                battleMsg->param[2] = battler1 | (message->partySlot[battler1] << 8);
                battleMsg->param[3] = battler2 | (message->partySlot[battler2] << 8);
            } else {
                battleMsg->id = 972;
                battleMsg->tag = TAG_TRCLASS_TRNAME_NICKNAME;
                battleMsg->param[0] = battler1;
                battleMsg->param[1] = battler1;
                battleMsg->param[2] = battler1 | (message->partySlot[battler1] << 8);
            }
        }
    } else {
        if (battleType & (1 << 2)) {
            u8 networkID = ov12_0223BFC0(battleSys);

            if (battleType & (1 << 3)) {
                switch (ov12_0223BFCC(battleSys, networkID)) {
                case 0:
                case 3:
                    battler1 = BattleSystem_GetBattlerFromBattlerType(battleSys, 4);
                    battler2 = BattleSystem_GetBattlerFromBattlerType(battleSys, 2);
                    break;
                case 1:
                case 2:
                    battler1 = BattleSystem_GetBattlerFromBattlerType(battleSys, 2);
                    battler2 = BattleSystem_GetBattlerFromBattlerType(battleSys, 4);
                    break;
                }
            } else if (battleType & (1 << 1)) {
                battler1 = BattleSystem_GetBattlerFromBattlerType(battleSys, 2);
                battler2 = BattleSystem_GetBattlerFromBattlerType(battleSys, 4);
            } else {
                battler1 = BattleSystem_GetBattlerFromBattlerType(battleSys, 0);
                battler2 = battler1;
            }
        } else if (battleType & (1 << 3)) {
            battler1 = BattleSystem_GetBattlerIdPartner(battleSys, battlerData->battler);
            battler2 = battlerData->battler;
        } else if (battleType & (1 << 1)) {
            battler1 = BattleSystem_GetBattlerFromBattlerType(battleSys, 2);
            battler2 = BattleSystem_GetBattlerFromBattlerType(battleSys, 4);
        } else {
            battler1 = battlerData->battler;
            battler2 = battler1;
        }

        if (battleType & (1 << 2)) {
            if (battleType & (1 << 3)) {
                battleMsg->id = 977;
                battleMsg->tag = TAG_TRNAME_NICKNAME_NICKNAME;
                battleMsg->param[0] = battler1;
                battleMsg->param[1] = battler1 | (message->partySlot[battler1] << 8);
                battleMsg->param[2] = battler2 | (message->partySlot[battler2] << 8);
            } else if (battleType & (1 << 1)) {
                battleMsg->id = 978;
                battleMsg->tag = TAG_NICKNAME_NICKNAME;
                battleMsg->param[0] = battler1 | (message->partySlot[battler1] << 8);
                battleMsg->param[1] = battler2 | (message->partySlot[battler2] << 8);
            } else {
                battleMsg->id = 979;
                battleMsg->tag = TAG_NICKNAME;
                battleMsg->param[0] = battler1 | (message->partySlot[battler1] << 8);
            }
        } else {
            if (battleType & (1 << 3)) {
                battleMsg->id = 993;
                battleMsg->tag = TAG_TRCLASS_TRNAME_NICKNAME_NICKNAME;
                battleMsg->param[0] = battler1;
                battleMsg->param[1] = battler1;
                battleMsg->param[2] = battler1 | (message->partySlot[battler1] << 8);
                battleMsg->param[3] = battler2 | (message->partySlot[battler2] << 8);
            } else if (battleType & (1 << 1)) {
                battleMsg->id = 978;
                battleMsg->tag = TAG_NICKNAME_NICKNAME;
                battleMsg->param[0] = battler1 | (message->partySlot[battler1] << 8);
                battleMsg->param[1] = battler2 | (message->partySlot[battler2] << 8);
            } else {
                battleMsg->id = 979;
                battleMsg->tag = TAG_NICKNAME;
                battleMsg->param[0] = battler1 | (message->partySlot[battler1] << 8);
            }
        }
    }
}

void ov12_02261928(BattleSystem *battleSys, OpponentData *battlerData, BattleMessage *battleMsg);

void ov12_02261928(BattleSystem *battleSys, OpponentData *battlerData, BattleMessage *battleMsg) {
    u32 battleType = BattleSystem_GetBattleType(battleSys);
    int battler1;
    int battler2;
    int battleResultMask = BattleSystem_GetBattleOutcomeFlags(battleSys);

    if (battleType & (1 << 1)) {
        battler1 = BattleSystem_GetBattlerFromBattlerType(battleSys, 3);
        battler2 = BattleSystem_GetBattlerFromBattlerType(battleSys, 5);
    } else {
        battler1 = BattleSystem_GetBattlerFromBattlerType(battleSys, 1);
        battler2 = battler1;
    }

    switch (battleResultMask) {
    case (1 << 0):
        if (battleType & (1 << 3)) {
            battleMsg->id = 786;
            battleMsg->tag = TAG_TRNAME_TRNAME;
            battleMsg->param[0] = battler1;
            battleMsg->param[1] = battler2;
        } else {
            battleMsg->id = 785;
            battleMsg->tag = TAG_TRNAME;
            battleMsg->param[0] = battler1;
        }
        break;
    case (1 << 1):
        if (battleType & (1 << 3)) {
            battleMsg->id = 788;
            battleMsg->tag = TAG_TRNAME_TRNAME;
            battleMsg->param[0] = battler1;
            battleMsg->param[1] = battler2;
        } else {
            battleMsg->id = 787;
            battleMsg->tag = TAG_TRNAME;
            battleMsg->param[0] = battler1;
        }
        break;
    case ((1 << 0) | (1 << 1)):
        if (battleType & (1 << 3)) {
            battleMsg->id = 790;
            battleMsg->tag = TAG_TRNAME_TRNAME;
            battleMsg->param[0] = battler1;
            battleMsg->param[1] = battler2;
        } else {
            battleMsg->id = 789;
            battleMsg->tag = TAG_TRNAME;
            battleMsg->param[0] = battler1;
        }
        break;
    }
}

void ov12_022619E4(BattleSystem *battleSys, OpponentData *battlerData, EscapeMsgMessage *message, BattleMessage *battleMsg);

void ov12_022619E4(BattleSystem *battleSys, OpponentData *battlerData, EscapeMsgMessage *message, BattleMessage *battleMsg) {
    int playerMonCount = 0;
    int enemyMonCount = 0;

    for (int i = 0; i < BattleSystem_GetMaxBattlers(battleSys); i++) {
        if (message->escaperBitmask & MaskOfFlagNo(i)) {
            if (BattleSystem_GetFieldSide(battleSys, i)) {
                enemyMonCount++;
            } else {
                playerMonCount++;
            }
        }
    }

    if (playerMonCount && enemyMonCount) {
        battleMsg->id = 781;
        battleMsg->tag = TAG_NONE;
        BattleSystem_SetBattleOutcomeFlags(battleSys, ((1 << 0) | (1 << 1)) | (1 << 7) | (1 << 6));
    } else if (playerMonCount) {
        battleMsg->id = 781;
        battleMsg->tag = TAG_NONE;
        BattleSystem_SetBattleOutcomeFlags(battleSys, (1 << 1) | (1 << 7) | (1 << 6));
    } else {
        if (BattleSystem_GetBattleType(battleSys) & (1 << 3)) {
            battleMsg->id = 792;
            battleMsg->tag = TAG_TRNAME_TRNAME;
            battleMsg->param[0] = BattleSystem_GetBattlerFromBattlerType(battleSys, 3);
            battleMsg->param[1] = BattleSystem_GetBattlerFromBattlerType(battleSys, 5);
        } else if (BattleSystem_GetBattleType(battleSys) & (1 << 1)) {
            battleMsg->id = 791;
            battleMsg->tag = TAG_TRNAME;
            battleMsg->param[0] = BattleSystem_GetBattlerFromBattlerType(battleSys, 3);
        } else {
            battleMsg->id = 791;
            battleMsg->tag = TAG_TRNAME;
            battleMsg->param[0] = BattleSystem_GetBattlerFromBattlerType(battleSys, 1);
        }

        BattleSystem_SetBattleOutcomeFlags(battleSys, (1 << 0) | (1 << 7) | (1 << 6));
    }
}

void ov12_02261AD4(BattleSystem *battleSystem, OpponentData *arg1, BattleMessage *arg2) {
    arg2->id = 0x3BC;
    arg2->tag = 8;
    if (BattleSystem_GetBattleType(battleSystem) & 4) {
        if (ov12_0223BFCC(battleSystem, ov12_0223BFC0(battleSystem)) != 0) {
            arg2->param[0] = BattleSystem_GetBattlerFromBattlerType(battleSystem, 4);
        } else {
            arg2->param[0] = BattleSystem_GetBattlerFromBattlerType(battleSystem, 2);
        }
    } else {
        arg2->param[0] = (s32)arg1->battler;
    }
    BattleSystem_SetBattleOutcomeFlags(battleSystem, 0xC2U);
}

void ov12_02261B2C(BattleSystem *param_1, u32 param_2, s32 param_3, s32 param_4, BOOL param_5, s16 param_6, s16 param_7) {
    SpriteSystem *renderer;
    SpriteManager *gfxHandler;
    PaletteData *plttData;
    s32 battlerPosition;

    renderer = BattleSystem_GetSpriteSystem(param_1);
    gfxHandler = BattleSystem_GetSpriteManager(param_1);
    plttData = BattleSystem_GetPaletteData(param_1);
    if ((param_2 & 1) != 0) {
        battlerPosition = 2;
    } else {
        battlerPosition = 0;
    }
    sub_02070C24(renderer, gfxHandler, plttData, (s32)param_6, (s32)param_7, param_3, battlerPosition, param_5, param_4, HEAP_ID_BATTLE);
    return;
}
