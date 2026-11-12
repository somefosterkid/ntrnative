// Black's code at ov018 0x02182998-0x02182b54. No name is known: the module and address.

#include "types.h"
#include "struct_decls.h"
#include "gfl/std.h"
#include "nitro/mi.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/wordset.h"
#include "field/intrude_comm.h"
#include "field/ov018_0217da54.h"
#include "field/field_script_event.h"
#include "field/field_script.h"
#include "field/event_comm_common.h"
#include "field/ov018_0217c458.h"

void func_ov018_021828b8(GameEvent *param_1, void *param_2, const u16 *msgIds);
GameEventReturnCode func_ov018_02182aa4(GameEvent *param_1, u32 *param_2, void *data);

// The work of func_ov018_02182aa4's event (0xa0 bytes): the comm events' start, then the message it shows
typedef struct Ov018MsgEvent {
    EventCommCommonWork common;
    u32 unk_98;
    u16 msgId;
} Ov018MsgEvent;


const u16 data_ov018_02183a14[6] = { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 };
const u16 data_ov018_02183a38[6] = { 0x0069, 0x006a, 0x006b, 0x006c, 0x006d };
const u16 data_ov018_02183a2c[6] = { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 };
const u16 data_ov018_02183a20[6] = { 0x005f, 0x0060, 0x0061, 0x0062, 0x0063 };

const u16 data_ov018_02183a44[6][5] = {
    { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 },
    { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 },
    { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 },
    { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 },
    { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 },
    { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 },
};

const u16 data_ov018_02183a80[6][5] = {
    { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 },
    { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 },
    { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 },
    { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 },
    { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 },
    { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072 },
};


void func_ov018_02182988(GameEvent *param_1, void *param_2) {
    func_ov018_021828b8(param_1, param_2, data_ov018_02183a20);
}

void func_ov018_02182998(GameEvent *param_1, void *param_2) {
    func_ov018_021828b8(param_1, param_2, data_ov018_02183a38);
}

void func_ov018_021829a8(GameEvent *param_1, void *param_2) {
    EventCommCommonWork *work = param_2;
    s32 index = work->entry.mission.kind;
    if (index >= 6) {
        index = 0;
    }
    func_ov018_021828b8(param_1, param_2, data_ov018_02183a80[index]);
}

void func_ov018_021829c8(GameEvent *param_1, void *param_2) {
    EventCommCommonWork *work = param_2;
    s32 index = work->entry.mission.kind;
    if (index >= 6) {
        index = 0;
    }
    func_ov018_021828b8(param_1, param_2, data_ov018_02183a44[index]);
}



void func_ov018_021829e8(GameEvent *param_1, void *param_2) {
    func_ov018_021828b8(param_1, param_2, data_ov018_02183a2c);
}

void func_ov018_021829f8(GameEvent *param_1, void *param_2) {
    func_ov018_021828b8(param_1, param_2, data_ov018_02183a14);
}

const u16 data_ov018_02183abc[6] = { 0x007d, 0x007e, 0x007f, 0x0080, 0x0081 };

void func_ov018_02182a08(GameEvent *param_1, void *param_2) {
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    EventCommCommonWork *src = param_2;
    Ov018MsgEvent *dest;
    u16 value;
    u16 dir;

    GameEvent_Transplant(param_1, func_ov018_02182aa4, sizeof(Ov018MsgEvent));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(src, dest, sizeof(EventCommCommonWork));
    if (intrude != NULL) {
        dest->msgId = data_ov018_02183abc[src->unk_42];
        copyVarForText(src->msg.wordSet, 0, &src->player);
    } else {
        GFL_ASSERT(FALSE);
        return;
    }
    value = 0;
    dir = intrude->unk_070.status.dir;
    if (dir == 2) {
        value = 3;
    } else if (dir == 3) {
        value = 2;
    } else if (dir != 1) {
        value = 1;
    }
    intrude->unk_08C[dest->common.netId].status.dir = value;
}

GameEventReturnCode func_ov018_02182aa4(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018MsgEvent *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);

    switch (*param_2) {
    case 0:
        func_ov018_0217f3c4(&param_3->common.msg, param_3->msgId);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 2: {
        GameEvent *call;
        ScriptWork *work;
        func_ov018_0217f318(&param_3->common.msg);
        call = EventScriptCall_Create(gsys, 0xafd, NULL, 4);
        work = EventScriptCall_GetWork(call);
        ScriptWork_SetParams(work, 0x23f, 1, 0, 0);
        GameEvent_ChainNext(param_1, call);
        *param_2 = *param_2 + 1;
        break;
    }
    case 3:
        *param_2 = 4;
        break;
    case 4:
        func_ov018_0217fe38(intrude, &param_3->common);
        GameEvent_Replace(param_1, func_ov018_0217ca84(gsys, 0));
        return 0;
    default:
        break;
    }
    return 0;
}


