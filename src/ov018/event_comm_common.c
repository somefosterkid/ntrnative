// Black's code at ov018 0x0217fb5c-0x021804f8. The name is the file name its asserts pass (tools/name_files.py).

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
#include "save/save_control.h"
#include "field/event_comm_common.h"

typedef struct IntrudeCommStart {
    Field *field;
    FieldActor *actor;
    u32 netId;
    HeapID heapId;
} IntrudeCommStart;

u32 func_ov018_0217fb94(GameEvent *param_1, s32 *param_2, IntrudeCommStart *param_3);
u32 func_ov018_0217fcac(GameEvent *param_1, s32 *param_2, IntrudeCommStart *param_3);
void func_ov011_0217545c(IntrudeComm *work);
GameEvent *func_ov018_0217fc08(GameSystem *gsys, Field *field, IntrudeComm *work, FieldActor *actor, u32 param_5, u16 param_6);
GameEvent *func_ov018_0217fd34(GameSystem *gsys, Field *field, IntrudeComm *work, FieldActor *actor, u32 param_5, u16 param_6);
void func_ov018_021808c4(GameEvent *param_1, void *param_2);
void func_ov018_021829f8(GameEvent *param_1, void *param_2);

void func_ov018_02182a08(GameEvent *param_1, void *param_2);
void func_ov018_02182998(GameEvent *param_1, void *param_2);
void func_ov018_02181be8(GameEvent *param_1, void *param_2);
void func_ov018_02182988(GameEvent *param_1, void *param_2);
void func_ov018_021802a8(GameEvent *param_1, EventCommCommonWork *param_2, IntrudeComm *param_3);
void func_ov018_0218034c(GameEvent *param_1, EventCommCommonWork *param_2, IntrudeComm *param_3);

GameEvent *func_ov018_0217fb5c(GameSystem *gsys, Field *field, IntrudeComm *work, FieldActor *actor, u32 netId, HeapID heapId) {
    GameEvent *event;
    IntrudeCommStart *dest;

    event = GameEvent_Create(gsys, NULL, (void *)func_ov018_0217fb94, sizeof(IntrudeCommStart));
    dest = GameEvent_GetData(event);
    MI_CpuFill8(dest, 0, sizeof(IntrudeCommStart));
    dest->field = field;
    dest->actor = actor;
    dest->netId = netId;
    dest->heapId = heapId;
    return event;
}

u32 func_ov018_0217fb94(GameEvent *param_1, s32 *param_2, IntrudeCommStart *param_3) {
    GameSystem *gsys;
    GameCommSys *commSys;
    IntrudeComm *intrudeComm;
    GameEvent *next;
    MMSys *mmSys;

    gsys = GameEvent_GetGameSystem(param_1);
    commSys = GSYS_GetGameCommSystem(gsys);
    intrudeComm = func_ov010_02158344(commSys);
    switch (*param_2) {
    case 0:
        if (intrudeComm == NULL) {
            return 1;
        }
        mmSys = Field_GetActorSystem(param_3->field);
        DisableAllActorsMovement(mmSys);
        next = func_ov018_0217fc08(gsys, param_3->field, intrudeComm, param_3->actor, param_3->netId, param_3->heapId);
        GameEvent_ChainNext(param_1, next);
        (*param_2)++;
        break;
    case 1:
        mmSys = Field_GetActorSystem(param_3->field);
        EnableAllActorsMovement(mmSys);
        return 1;
    }
    return 0;
}

void func_ov018_0217fda0(GameSystem *gsys, IntrudeComm *work, IntrudeCommEvent *dest, Field *field, FieldActor *actor, u32 param_5, u16 param_6);
u32 func_ov018_021803f0(IntrudeCommEvent *param, IntrudeComm *work);
GameEventReturnCode func_ov018_0217fe6c(GameEvent *event, s32 *state, IntrudeCommEvent *param);

GameEvent *func_ov018_0217fc08(GameSystem *gsys, Field *field, IntrudeComm *work, FieldActor *actor, u32 param_5, u16 param_6) {
    GameEvent *event;
    IntrudeCommEvent *dest;

    event = GameEvent_Create(gsys, NULL, (void *)func_ov018_0217fe6c, sizeof(IntrudeCommEvent));
    dest = GameEvent_GetData(event);
    MI_CpuFill8(dest, 0, sizeof(IntrudeCommEvent));
    func_ov018_0217fda0(gsys, work, dest, field, actor, param_5, param_6);
    func_ov011_0217538c(work, param_5);
    func_ov011_02175368(work);
    dest->common.unk_41 = func_ov018_021803f0(dest, work);
    return event;
}



GameEvent *func_ov018_0217fc6c(GameSystem *gsys, Field *field, IntrudeComm *work, FieldActor *actor, u32 netId, HeapID heapId) {
    GameEvent *event;
    IntrudeCommStart *dest;

    event = GameEvent_Create(gsys, NULL, (void *)func_ov018_0217fcac, sizeof(IntrudeCommStart));
    dest = GameEvent_GetData(event);
    MI_CpuFill8(dest, 0, sizeof(IntrudeCommStart));
    dest->field = field;
    dest->actor = actor;
    dest->netId = netId;
    dest->heapId = heapId;
    func_ov011_0217545c(work);
    return event;
}

u32 func_ov018_0217fcac(GameEvent *param_1, s32 *param_2, IntrudeCommStart *param_3) {
    GameSystem *gsys;
    GameCommSys *commSys;
    IntrudeComm *intrudeComm;
    GameEvent *next;
    MMSys *mmSys;

    gsys = GameEvent_GetGameSystem(param_1);
    commSys = GSYS_GetGameCommSystem(gsys);
    intrudeComm = func_ov010_02158344(commSys);
    switch (*param_2) {
    case 0:
        if (intrudeComm == NULL) {
            return 1;
        }
        if (func_ov010_02167a54(intrudeComm->commPlayer, param_3->netId) == FALSE) {
            return 1;
        }
        mmSys = Field_GetActorSystem(param_3->field);
        DisableAllActorsMovement(mmSys);
        next = func_ov018_0217fd34(gsys, param_3->field, intrudeComm, param_3->actor, param_3->netId, param_3->heapId);
        GameEvent_ChainNext(param_1, next);
        (*param_2)++;
        break;
    case 1:
        mmSys = Field_GetActorSystem(param_3->field);
        EnableAllActorsMovement(mmSys);
        return 1;
    }
    return 0;
}

GameEventReturnCode func_ov018_021800a4(GameEvent *event, s32 *state, IntrudeCommEvent *param);
GameEvent *func_ov018_0217fd34(GameSystem *gsys, Field *field, IntrudeComm *work, FieldActor *actor, u32 param_5, u16 param_6) {
    GameEvent *event;
    IntrudeCommEvent *dest;

    event = GameEvent_Create(gsys, NULL, (void *)func_ov018_021800a4, sizeof(IntrudeCommEvent));
    dest = GameEvent_GetData(event);
    MI_CpuFill8(dest, 0, sizeof(IntrudeCommEvent));
    func_ov018_0217fda0(gsys, work, dest, field, actor, param_5, param_6);
    dest->common.unk_41 = work->unk_11C.unk_00;
    dest->common.entry = work->unk_11C.entry;
    return event;
}

void func_ov018_0217fda0(GameSystem *gsys, IntrudeComm *work, IntrudeCommEvent *dest, Field *field, FieldActor *actor, u32 param_5, u16 param_6) {
    int netId;
    u8 id;

    dest->common.heapId = param_6;
    dest->common.actor = actor;
    netId = param_5;
    dest->common.target = func_ov010_02167a14(work->commPlayer, netId);
    dest->common.field = field;
    id = param_5;
    dest->common.netId = id;
    dest->common.unk_42 = func_ov018_0217f5f0(work, id);
    func_02008494(func_ov010_021583ec(GSYS_GetGameData(gsys), netId), &dest->common.player);
    if (func_ov011_02175b14(&work->unk_1AC) == TRUE) {
        MI_CpuCopy8(func_ov011_02175b34(&work->unk_1AC), &dest->common.entry, sizeof(IntrudeMissionEntry));
    }
    func_ov018_0217f2b4(&dest->common.msg, gsys);
    func_ov011_02174e14(work, 1);
    func_ov011_02175368(work);
    work->unk_93F_4 = 0;
}




void func_ov018_0217fe38(IntrudeComm *param_1, EventCommCommonWork *param_2) {
    func_ov018_0217f4c4(&param_2->msg);
    func_ov018_0217f318(&param_2->msg);
    if (param_1 != (IntrudeComm *)0x0) {
        func_ov011_02174e14(param_1, 0);
        func_ov011_0217538c(param_1, 4);
        func_ov011_02175368(param_1);
    }
}

GameEventReturnCode func_ov018_0217fe6c(GameEvent *event, s32 *state, IntrudeCommEvent *param) {
    GameSystem *gsys;
    GameCommSys *commSys;
    IntrudeComm *work;
    u32 missionState;
    void *dest;
    u32 result;

    gsys = GameEvent_GetGameSystem(event);
    commSys = GSYS_GetGameCommSystem(gsys);
    work = func_ov010_02158344(commSys);
    if (work == NULL) {
        if (func_ov018_0217f40c(&param->common.msg) == FALSE) {
            return 0;
        }
        if (*state <= 6) {
            func_ov018_0217f3c4(&param->common.msg, 4);
            *state = 8;
            return 0;
        }
    }
    switch (*state) {
    case 0:
        missionState = func_ov011_02175dcc(&work->unk_1AC);
        if (work->unk_08C[param->common.netId].unk_16_15 == 1) {
            dest = GFL_HeapAllocate((param->common.heapId & 0x7fff) | 0x8000, sizeof(EventCommCommonWork), TRUE, "event_comm_common.c", 0x1ed);
            MI_CpuCopy8(param, dest, sizeof(EventCommCommonWork));
            if (func_ov011_02175bcc(&work->unk_1AC) == NULL && func_ov011_02175b14(&work->unk_1AC) == TRUE
                && func_ov011_02175b8c(&work->unk_1AC) == TRUE
                && func_ov011_0217620c(&work->unk_1AC, param->common.netId) == TRUE) {
                func_ov018_02182a08(event, dest);
            } else {
                func_ov018_02182998(event, dest);
            }
            GFL_HeapFree(dest);
        } else if (work->unk_08C[param->common.netId].unk_16_9 != 0) {
            dest = GFL_HeapAllocate((param->common.heapId & 0x7fff) | 0x8000, sizeof(EventCommCommonWork), TRUE, "event_comm_common.c", 0x205);
            MI_CpuCopy8(param, dest, sizeof(EventCommCommonWork));
            if (work->unk_08C[param->common.netId].unk_16_9 == 2) {
                if (missionState == 5) {
                    func_ov018_02181be8(event, dest);
                } else {
                    func_ov018_02182988(event, dest);
                }
            } else {
                func_ov018_02182998(event, dest);
            }
            GFL_HeapFree(dest);
        } else {
            *state = *state + 1;
        }
        break;
    case 1:
        if (func_ov011_02174258(work) == TRUE) {
            *state = *state + 1;
        }
        break;
    case 2:
        result = func_ov011_02175c40(&work->unk_1AC);
        switch (result) {
        case 1:
            param->unk_99 = 1;
            *state = *state + 1;
            break;
        case 2:
            *state = *state + 1;
            break;
        }
        break;
    case 3:
        result = func_ov018_0217c914(work, param, &param->unk_98);
        switch (result) {
        case 1:
            func_ov011_0217553c(work);
            *state = 4;
            break;
        case 2:
            *state = 9;
            break;
        case 3:
            *state = 7;
            break;
        }
        break;
    case 4:
        if (func_ov011_02173c04(param->common.netId, param->unk_99) == TRUE) {
            *state = *state + 1;
        }
        break;
    case 5: {
        IntrudeUnk15C *unk = func_ov011_0217537c(work);

        if (unk != NULL) {
            if (unk->unk_01 != param->unk_99 || param->unk_99 == 1) {
                param->common.unk_41 = 0;
            }
            *state = *state + 1;
        }
        break;
    }
    case 6:
        func_ov018_021802a8(event, &param->common, work);
        return 0;
    case 7:
        *state = 9;
        break;
    case 8:
        if (func_ov018_0217f420() == 1) {
            *state = 9;
        }
        break;
    case 9:
        func_ov018_0217fe38(work, &param->common);
        return 1;
    }
    return 0;
}

GameEventReturnCode func_ov018_021800a4(GameEvent *event, s32 *state, IntrudeCommEvent *param) {
    GameSystem *gsys;
    GameCommSys *commSys;
    GameData *gameData;
    IntrudeComm *work;
    PlayerInfo *playerInfo;
    u32 result;
    BOOL done;
    u16 dir;
    u16 acmd;

    gsys = GameEvent_GetGameSystem(event);
    commSys = GSYS_GetGameCommSystem(gsys);
    gameData = GSYS_GetGameData(gsys);
    work = func_ov010_02158344(commSys);
    if (work == NULL) {
        if (func_ov018_0217f40c(&param->common.msg) == FALSE) {
            return 0;
        }
        if (*state < 10) {
            func_ov018_0217f3c4(&param->common.msg, 4);
            *state = 10;
            return 0;
        }
    }
    switch (*state) {
    case 0:
        func_ov011_0217553c(work);
        DisableAllActorsMovement(Field_GetActorSystem(param->common.field));
        playerInfo = func_02012950(gameData, work->unk_154.netId);
        copyVarForText(param->common.msg.wordSet, 0, playerInfo);
        func_ov018_0217f3c4(&param->common.msg, 1);
        *state = *state + 1;
        break;
    case 1:
        if (IsAllActorAcmdFinished(param->common.actor) == TRUE) {
            dir = work->unk_08C[param->common.netId].status.dir;
            if (dir == 2) {
                acmd = 3;
            } else if (dir == 3) {
                acmd = 2;
            } else {
                acmd = 0;
                if (dir != 1) {
                    acmd = 1;
                }
            }
            FldAct_SetAcmd(param->common.actor, acmd);
            *state = *state + 1;
        }
        break;
    case 2:
        if (func_ov010_0216d480(param->common.actor) == TRUE) {
            *state = *state + 1;
        }
        break;
    case 3:
        if (func_ov011_02174258(work) == TRUE) {
            *state = *state + 1;
        }
        break;
    case 4:
        result = func_ov011_02175c40(&work->unk_1AC);
        switch (result) {
        case 1:
            param->unk_99 = 1;
            *state = *state + 1;
            break;
        case 2:
            *state = *state + 1;
            break;
        }
        break;
    case 5:
        if (func_ov011_02173ae0(work, work->unk_154.netId, work->unk_154.unk_02, work->unk_154.unk_04) == TRUE) {
            *state = *state + 1;
        }
        break;
    case 6:
        if (func_ov018_0217f40c(&param->common.msg) == TRUE) {
            result = func_ov011_02175514(work);
            done = FALSE;
            switch (result) {
            case 0:
                break;
            case 1:
                *state = 7;
                break;
            case 2:
            case 3:
            default:
                done = TRUE;
                break;
            }
            if (done == TRUE) {
                func_ov018_0217f3c4(&param->common.msg, 4);
                *state = 10;
            }
        }
        break;
    case 7:
        if (func_ov011_02173c04(work->unk_154.netId, param->unk_99) == TRUE) {
            *state = *state + 1;
        }
        break;
    case 8: {
        IntrudeUnk15C *unk = func_ov011_0217537c(work);

        if (unk != NULL) {
            if (unk->unk_01 != param->unk_99 || param->unk_99 == 1) {
                param->common.unk_41 = 0;
            }
            *state = 9;
        }
        break;
    }
    case 9:
        func_ov018_0218034c(event, &param->common, work);

        return 0;
    case 10:
        if (func_ov018_0217f420() == 1) {
            *state = 11;
        }
        break;
    case 11:
        if (func_ov018_0217f40c(&param->common.msg) == TRUE) {
            EnableAllActorsMovement(Field_GetActorSystem(param->common.field));
            func_ov018_0217fe38(work, &param->common);
            return 1;
        }
        break;
    }
    return 0;
}



void func_ov018_021811e4(GameEvent *param_1, void *param_2);
void func_ov018_021829a8(GameEvent *param_1, void *param_2);
void func_ov018_021829e8(GameEvent *param_1, void *param_2);
void func_ov018_0218152c(GameEvent *param_1, void *param_2);
void func_ov018_02180de8(GameEvent *param_1, void *param_2);
void func_ov018_02180eb0(GameEvent *param_1, void *param_2);
void func_ov018_02180980(GameEvent *param_1, void *param_2);
void func_ov018_02180b14(GameEvent *param_1, void *param_2);
void func_ov018_0218236c(GameEvent *param_1, void *param_2);
void func_ov018_021825f8(GameEvent *param_1, void *param_2);
void func_ov018_02181e6c(GameEvent *param_1, void *param_2);
void func_ov018_02181f10(GameEvent *param_1, void *param_2);
void func_ov018_0218191c(GameEvent *param_1, void *param_2);
void func_ov018_021819d0(GameEvent *param_1, void *param_2);

void (*const data_ov018_0218374c[30])(GameEvent *, void *) = {
    func_ov018_021811e4, func_ov018_021829a8, func_ov018_021829e8, func_ov018_0218152c,
    func_ov018_021808c4, func_ov018_02180de8, func_ov018_021829a8, func_ov018_021829e8,
    func_ov018_02180eb0, func_ov018_021808c4, func_ov018_02180980, func_ov018_021829a8,
    func_ov018_021829e8, func_ov018_02180b14, func_ov018_021808c4, func_ov018_0218236c,
    func_ov018_021829a8, func_ov018_021829e8, func_ov018_021825f8, func_ov018_021808c4,
    func_ov018_02181e6c, func_ov018_021829a8, func_ov018_021829e8, func_ov018_02181f10,
    func_ov018_021808c4, func_ov018_0218191c, func_ov018_021829a8, func_ov018_021829e8,
    func_ov018_021819d0, func_ov018_021808c4,
};
void func_ov018_021802a8(GameEvent *param_1, EventCommCommonWork *param_2, IntrudeComm *param_3) {
    void *dest;

    dest = GFL_HeapAllocate((param_2->heapId & 0x7fff) | 0x8000, sizeof(EventCommCommonWork), TRUE, "event_comm_common.c", 0x329);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
    if (param_2->unk_41 == 0) {
        func_ov018_021808c4(param_1, dest);
    } else if (param_2->unk_41 == 1) {
        func_ov018_021829f8(param_1, dest);
    } else if (param_2->unk_41 == 2) {
        func_ov018_021808c4(param_1, dest);
    } else if (param_2->unk_41 >= 3) {
        GFL_ASSERT(data_ov018_0218374c[param_2->unk_41 - 8] != NULL);
        data_ov018_0218374c[param_2->unk_41 - 8](param_1, dest);
    } else {
        func_ov018_021808c4(param_1, dest);
    }
    GFL_HeapFree(dest);
}

void func_ov018_021818c4(GameEvent *param_1, void *param_2);
void func_ov018_021829c8(GameEvent *param_1, void *param_2);
void func_ov018_0218186c(GameEvent *param_1, void *param_2);
void func_ov018_0218111c(GameEvent *param_1, void *param_2);
void func_ov018_02180fa8(GameEvent *param_1, void *param_2);
void func_ov018_02180ca4(GameEvent *param_1, void *param_2);
void func_ov018_02180bdc(GameEvent *param_1, void *param_2);
void func_ov018_021828b0(GameEvent *param_1, void *param_2);
void func_ov018_021828a8(GameEvent *param_1, void *param_2);
void func_ov018_02182080(GameEvent *param_1, void *param_2);
void func_ov018_02181fb4(GameEvent *param_1, void *param_2);
void func_ov018_02181b44(GameEvent *param_1, void *param_2);
void func_ov018_02181a74(GameEvent *param_1, void *param_2);
void func_ov018_02180978(GameEvent *param_1, void *param_2);

void (*const data_ov018_021837c4[30])(GameEvent *, void *) = {
    func_ov018_021818c4, func_ov018_021829c8, func_ov018_021829e8, func_ov018_0218186c,
    func_ov018_02180978, func_ov018_0218111c, func_ov018_021829c8, func_ov018_021829e8,
    func_ov018_02180fa8, func_ov018_02180978, func_ov018_02180ca4, func_ov018_021829c8,
    func_ov018_021829e8, func_ov018_02180bdc, func_ov018_02180978, func_ov018_021828b0,
    func_ov018_021829c8, func_ov018_021829e8, func_ov018_021828a8, func_ov018_02180978,
    func_ov018_02182080, func_ov018_021829c8, func_ov018_021829e8, func_ov018_02181fb4,
    func_ov018_02180978, func_ov018_02181b44, func_ov018_021829c8, func_ov018_021829e8,
    func_ov018_02181a74, func_ov018_02180978,
};
void func_ov018_0218034c(GameEvent *param_1, EventCommCommonWork *param_2, IntrudeComm *param_3) {
    void *dest;

    dest = GFL_HeapAllocate((param_2->heapId & 0x7fff) | 0x8000, sizeof(EventCommCommonWork), TRUE, "event_comm_common.c", 0x354);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
    if (param_2->unk_41 == 0) {
        func_ov018_02180978(param_1, dest);
    } else if (param_2->unk_41 == 1) {
        func_ov018_021829f8(param_1, dest);
    } else if (param_2->unk_41 == 2) {
        func_ov018_02180978(param_1, dest);
    } else if (param_2->unk_41 >= 3) {
        GFL_ASSERT(data_ov018_021837c4[param_2->unk_41 - 8] != NULL);
        data_ov018_021837c4[param_2->unk_41 - 8](param_1, dest);
    } else {
        func_ov018_02180978(param_1, dest);
    }
    GFL_HeapFree(dest);
}

static inline s32 GetEventKind(IntrudeCommEvent *param, IntrudeComm *work, u8 random) {
    if (func_ov011_02175bcc(&work->unk_1AC) == NULL && func_ov011_02175b14(&work->unk_1AC) == TRUE) {
        if (func_ov011_02175b8c(&work->unk_1AC) == TRUE) {
            if (func_ov011_0217620c(&work->unk_1AC, param->common.netId) == TRUE) {
                return 3;
            }
            if (work->unk_08C[param->common.netId].unk_1A_5 == 1) {
                return 4;
            }
            return 5;
        } else {
            if (func_ov011_0217620c(&work->unk_1AC, random) == TRUE) {
                if (work->unk_08C[param->common.netId].unk_1A_5 == 1) {
                    return 6;
                }
                return 7;
            } else if (work->unk_08C[param->common.netId].unk_1A_5 == 1) {
                return 1;
            } else if (func_ov011_0217620c(&work->unk_1AC, param->common.netId) == TRUE) {
                return 2;
            } else {
                return 0;
            }
        }
    }
    return 0;
}

u32 func_ov018_021803f0(IntrudeCommEvent *param, IntrudeComm *work) {
    GameSystem *gsys;
    GameData *gameData;
    u8 random;
    u32 subType;
    s32 result;

    gsys = Field_GetGameSystem(param->common.field);
    gameData = GSYS_GetGameData(gsys);
    random = func_02012b58(gameData);
    subType = func_ov011_02175dcc(&work->unk_1AC);
    result = GetEventKind(param, work, random);
    if (result >= 3) {
        switch (subType) {
        case 0:
            result += 8;
            break;
        case 1:
            result += 0xd;
            break;
        case 2:
            result += 0x12;
            break;
        case 3:
            result += 0x17;
            break;
        case 4:
            result += 0x1c;
            break;
        case 5:
            result += 0x21;
            break;
        }
        result -= 3;
    }
    return result;
}
