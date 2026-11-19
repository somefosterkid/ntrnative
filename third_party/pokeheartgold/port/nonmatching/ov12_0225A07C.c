/* PORTOK hk39 */
#include "battle/battle_system.h"
#include "bg_window.h"
#include "heap.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "battle/battle.h"
#include "battle/battle_message_structs.h"
#include "battle/battle_system.h"
#include "bg_window.h"
#include "constants/heap.h"
#include "heap.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "wild_encounter.h"
// header include/overlay_12_battle_controller_opponent.h declares it void; the asm uses the returned sprite
ManagedSprite *ov12_02261B2C(BattleSystem *, u32, u16, u8, u32, s32, s32);
u8 ov12_02261EF0(BattleSystem *, s32, u8);
void ov12_0225CE28(SysTask *task, void *data);
extern s16 ov07_022377DC[][2];  // per position: s16, s16
extern s16 ov07_022377F4[][3];  // per position: x, y, z

typedef struct Ov12TrainerEnterData {
    BattleSystem *bsys;        // 0x00
    Pokepic *pokepic;          // 0x04
    UnkBattleSystemSub17C *unk8; // 0x08
    ManagedSprite *sprite;     // 0x0C
    u8 command;                // 0x10
    u8 battlerId;              // 0x11
    u8 unk12;                  // 0x12
    u8 side;                   // 0x13: 0 or 2
    s16 unk14;                 // 0x14
    u8 filler16[2];
    u32 battlerType;           // 0x18
    u8 filler1C[4];
    u32 unk20;                 // 0x20
} Ov12TrainerEnterData;        // 0x24

void ov12_0225A07C(BattleSystem *param_1, OpponentData *param_2, TrainerEncounterMessage *param_3) {
    Ov12TrainerEnterData *data;
    UnkBattleSystemSub17C *unk;
    u32 pos;
    u32 battleType;
    u32 flag;
    s16 x;
    s16 y;
    PokepicManager *pokepicManager;
    UnkStruct_02070D3C sprite;
    PokepicTemplate tmpl;

    data = Heap_Alloc(HEAP_ID_BATTLE, sizeof(Ov12TrainerEnterData));
    data->unk12 = 0;
    if (param_2->battlerType & 1) {
        data->side = 2;
        unk = ov12_0223A8F4(param_1, 1);
        data->unk8 = unk;
        ManagedSprite_SetPositionXY(unk->unk0, ov07_022377F4[param_2->battlerType & 1][0], 0x58);
    } else {
        data->side = 0;
        unk = ov12_0223A8F4(param_1, 0);
        data->unk8 = unk;
        ManagedSprite_SetPositionXY(unk->unk0, ov07_022377F4[param_2->battlerType & 1][0], 0x88);
    }
    if ((BattleSystem_GetBattleType(param_1) & 8) || ((BattleSystem_GetBattleType(param_1) & 0x10) && (param_2->battlerType & 1))) {
        pos = param_2->battlerType;
    } else {
        pos = param_2->battlerType & 1;
    }
    param_3->trainerType = ov12_02261EF0(param_1, param_2->battlerId, param_3->trainerType);
    battleType = BattleSystem_GetBattleType(param_1);
    flag = 0;
    if (ov12_0223C140(param_1, param_2->battlerId) != 0xFF) {
        if ((battleType & 2) && !(battleType & 8)) {
            flag = 0;
        } else {
            flag = 1;
        }
    }
    x = ov07_022377F4[pos][0];
    y = ov07_022377F4[pos][1];
    param_2->managedSprite = ov12_02261B2C(param_1, pos, param_3->trainerType, param_2->battlerType, flag, x, y);
    data->sprite = param_2->managedSprite;
    if (data->side == 0
        && (BattleSystem_GetBattleType(param_1) == 0 || BattleSystem_GetBattleType(param_1) == 0x20
            || BattleSystem_GetBattleType(param_1) == 0x100 || BattleSystem_GetBattleType(param_1) == 0x200
            || BattleSystem_GetBattleType(param_1) == 0x400 || BattleSystem_GetBattleType(param_1) == 0x1000)) {
        pokepicManager = BattleSystem_GetPokepicManager(param_1);
        sub_02070D84(param_3->trainerType, data->side, &sprite);
        tmpl.narcID = sprite.narcId;
        tmpl.charDataID = sprite.ncbr_id;
        tmpl.palDataID = sprite.nclr_id;
        tmpl.species = 0;
        tmpl.isAnimated = 0;
        tmpl.personality = 0;
        data->pokepic = PokepicManager_CreatePokepic(pokepicManager, &tmpl, x, y, ov07_022377F4[pos][2], param_2->battlerId, NULL, NULL);
    } else {
        data->pokepic = NULL;
    }
    data->unk14 = ov07_022377DC[pos][0];
    data->bsys = param_1;
    data->command = param_3->command;
    data->battlerId = param_2->battlerId;
    data->battlerType = param_2->battlerType;
    data->unk20 = 0;
    if (data->battlerType == 0 || data->battlerType == 2) {
        BgSetPosTextAndCommit(BattleSystem_GetBgConfig(param_1), 3, BG_POS_OP_SUB_X, 0x84);
    }
    SysTask_CreateOnMainQueue(ov12_0225CE28, data, 0);
}
