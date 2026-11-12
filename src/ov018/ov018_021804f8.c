// Black's code at ov018 0x021804f8-0x02180980. No name is known: the module and address. Black's object of its own
// after event_comm_common.c: its .data holds an "" assert string of its own (func_ov018_02180538's), its .rodata the
// 12-byte table after event_comm_common.c's two event tables.

#include "types.h"
#include "field/field.h"
#include "field/intrude_comm.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "struct_decls.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "field/field_actor.h"
#include "field/comm_player.h"
#include "field/ov018_0217da54.h"
#include "field/intrude_work.h"
#include "system/game_data.h"
#include "system/wordset.h"
#include "field/ov018_0217c458.h"
#include "gfl/net.h"
#include "system/game_comm.h"
#include "field/event_sound.h"
#include "gfl/sound.h"
#include "save/bag.h"
#include "pml/item.h"
#include "constants/items.h"
#include "save/high_link.h"
#include "save/save_control.h"
#include "save/high_link.h"
#include "field/event_comm_common.h"

typedef struct IntrudeCommReward {
    HeapID heapId;
    u8 unk_02[2];
    Ov018MsgWork msg;
    u32 unk_34;
    u8 unk_38[0xb4];
    IntrudeMissionEntrust unk_EC;
    u16 kind;
    u16 msgId;
    BOOL unk_12C;
    s32 itemCount;
    u8 unk_134;
    u8 unk_135;
    u16 timer;
} IntrudeCommReward;

u32 func_ov018_02180538(GameEvent *event, s32 *state, IntrudeCommReward *param);
GameEventReturnCode func_ov018_021808e8(GameEvent *param_1, s32 *param_2, EventCommCommonWork *param_3);

GameEvent *func_ov018_021804f8(GameSystem *param_1, u32 param_2) {
    Field *field;
    GameEvent *event;
    IntrudeCommReward *work;

    field = GSYS_GetField(param_1);
    event = GameEvent_Create(param_1, NULL, (void *)func_ov018_02180538, sizeof(IntrudeCommReward));
    work = GameEvent_GetData(event);
    MI_CpuFill8(work, 0, sizeof(IntrudeCommReward));
    work->heapId = Field_GetHeapID(field);
    work->unk_34 = param_2;
    return event;
}

u32 func_ov018_02180538(GameEvent *event, s32 *state, IntrudeCommReward *param) {
    GameSystem *gsys;
    GameCommSys *commSys;
    GameData *gameData;

    gsys = GameEvent_GetGameSystem(event);
    commSys = GSYS_GetGameCommSystem(gsys);
    gameData = GSYS_GetGameData(gsys);
    GSYS_GetField(gsys);
    switch (*state) {
    case 0: {
        IntrudeComm *work = NULL;
        IntrudeMissionEntrust *entrust;
        PassPowerLevel *levels;

        if (GameCommSys_BootCheck(commSys) == 2) {
            work = func_02026474(commSys);
        }
        if (param->unk_34 == 0) {
            if (work != NULL && func_0200fb24() == FALSE && func_ov011_02175e90(work, &work->unk_1AC) == TRUE) {
                work->unk_904_0 = 1;
            }
            return 1;
        }
        if (work != NULL && func_ov011_02175da8(&work->unk_1AC) == TRUE && func_ov011_021756a4(work) == TRUE) {
            if (func_ov011_02175e90(work, &work->unk_1AC) == FALSE) {
                return 1;
            }
            param->unk_134 = work->unk_358[0];
            param->unk_135 = work->unk_358[1];
            func_ov018_0217f2b4(&param->msg, gsys);
            entrust = func_ov011_02175bcc(&work->unk_1AC);
            levels = func_020127b8(gameData);
            GFL_ASSERT(entrust != NULL);
            param->unk_EC = *entrust;
            param->kind = entrust->entry.mission.kind;
            param->msgId = entrust->entry.mission.msgId;
            param->unk_12C = func_ov011_02175e90(work, &work->unk_1AC);
            param->itemCount = func_ov011_02175de4(work, &work->unk_1AC);
            func_0200be24(levels, param->unk_135);
            func_0200be00(levels, param->unk_134);
            *state = *state + 1;
        } else {
            if (work == NULL || func_0200fb24() != FALSE) {
                return 1;
            }
        }
        break;
    }
    case 1:
        func_ov011_021762d8(&param->unk_EC.entry.mission, &param->unk_EC.entry.player, param->msg.wordSet, param->heapId);
        func_ov018_0217f528(&param->msg, gsys, 1, 4, 0x1e, 0x10);
        func_ov018_0217f574(&param->msg, param->kind, 8, 0);
        func_ov018_0217f574(&param->msg, param->msgId, 8, 0x10);
        *state = *state + 1;
        break;
    case 2:
        param->timer++;
        if (param->timer > 0x3c) {
            func_ov018_0217f58c(&param->msg);
            *state = 3;
        }
        break;
    case 3:
        if (param->unk_12C == 1) {
            func_ov018_0217f4a0(&param->msg, 0x19, 0, 0);
            GameEvent_ChainNext(event, EventMEPlay_Create(gsys, 0x52c));
            *state = 4;
        } else {
            func_ov018_0217f4a0(&param->msg, 0x17, 0, 0);
            GameEvent_ChainNext(event, EventMEPlay_Create(gsys, 0x52d));
            *state = 0xb;
        }
        break;
    case 4:
        if (GFL_SndBGMIsPlaying() == FALSE) {
            BagSave *bag;
            u32 count;
            u32 add;

            GameEvent_ChainNext(event, EventPushBGMFinish_Create(gsys, 0, 6));
            bag = GameData_GetBag(gameData);
            count = BagSave_GetItemCountByID(bag, ITEM_PASS_ORB, param->heapId);
            add = param->itemCount;
            if (PML_ItemGetMaxStorageCount(ITEM_PASS_ORB) < count + add) {
                add = PML_ItemGetMaxStorageCount(ITEM_PASS_ORB) - count;
            }
            BagSave_AddItem(bag, ITEM_PASS_ORB, (u16)add, param->heapId);
            func_0200bf78(getHighLinkBlockAddress(GameData_GetSaveControl(gameData)));
            WordSetNumber(param->msg.wordSet, 0, param->itemCount, 3, 0, 1);
            func_ov018_0217f3c4(&param->msg, 0x18);
            *state = *state + 1;
        }
        break;
    case 5:
        if (func_ov018_0217f40c(&param->msg) == TRUE) {
            *state = *state + 1;
        }
        break;
    case 6:
        if (param->unk_135 != 0) {
            WordSetNumber(param->msg.wordSet, 0, param->unk_135, 3, 0, 1);
            func_ov018_0217f3c4(&param->msg, 0x30);
            *state = *state + 1;
        } else {
            *state = 8;
        }
        break;
    case 7:
        if (func_ov018_0217f40c(&param->msg) == TRUE) {
            *state = *state + 1;
        }
        break;
    case 8:
        if (param->unk_134 != 0) {
            WordSetNumber(param->msg.wordSet, 0, param->unk_134, 3, 0, 1);
            func_ov018_0217f3c4(&param->msg, 0x31);
            *state = *state + 1;
        } else {
            *state = 10;
        }
        break;
    case 9:
        if (func_ov018_0217f40c(&param->msg) == TRUE) {
            *state = 10;
        }
        break;
    case 10:
        *state = 0xc;
        break;
    case 11:
        if (GFL_SndBGMIsPlaying() == FALSE) {
            GameEvent_ChainNext(event, EventPushBGMFinish_Create(gsys, 0, 6));
            *state = 0xc;
        }
        break;
    case 12:
        func_ov018_0217f318(&param->msg);
        GameEvent_Replace(event, func_ov018_0217ca84(gsys, 0));
        return 0;
    }
    return 0;
}


void func_ov018_021808c4(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, (void *)func_ov018_021808e8, 0xa8);
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, 0x98);
}

const u16 data_ov018_0218383c[6] = { 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x00 };

GameEventReturnCode func_ov018_021808e8(GameEvent *param_1, s32 *param_2, EventCommCommonWork *param_3) {
    GameSystem *gsys;
    GameData *gameData;
    IntrudeComm *work;
    PlayerInfo *playerInfo;

    gsys = GameEvent_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    work = func_ov010_02158344(GSYS_GetGameCommSystem(gsys));
    switch (*param_2) {
    case 0:
        playerInfo = func_02012950(gameData, param_3->netId);
        copyVarForText(param_3->msg.wordSet, 0, playerInfo);
        func_ov018_0217f3c4(&param_3->msg, data_ov018_0218383c[param_3->unk_42]);
        (*param_2)++;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->msg) == TRUE) {
            (*param_2)++;
        }
        break;
    case 2:
        if (func_ov018_0217f420() == TRUE) {
            (*param_2)++;
        }
        break;
    case 3:
        func_ov018_0217fe38(work, param_3);
        return 1;
    }
    return 0;
}


void func_ov018_02180978(GameEvent *param_1, void *param_2) {
    func_ov018_021808c4(param_1, param_2);
}
