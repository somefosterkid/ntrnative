#ifndef FIELD_EVENT_COMM_COMMON_H
#define FIELD_EVENT_COMM_COMMON_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "field/intrude_comm.h"
#include "save/player_info.h"
#include "field/ov018_0217da54.h"

// What the comm events of the Entralink share: the events func_ov018_021802a8/0218034c hand over to get a copy
// of it at the start of their work
typedef struct EventCommCommonWork {
    HeapID heapId;
    u8 unk_02[2];
    FieldActor *actor;
    FieldActor *target;
    Field *field;
    Ov018MsgWork msg;
    u8 netId;
    u8 unk_41;
    u8 unk_42;
    u8 unk_43;
    IntrudeMissionEntry entry;
    PlayerInfo player;
} EventCommCommonWork;

// The work of the events event_comm_common.c starts when the player talks to another player (func_ov018_0217fc08,
// 0217fd34)
typedef struct IntrudeCommEvent {
    EventCommCommonWork common;
    u8 unk_98;
    u8 unk_99;
} IntrudeCommEvent;

// Ends an event of the Entralink's: work is the start of the event's work
void func_ov018_0217fe38(IntrudeComm *comm, EventCommCommonWork *work);
GameEvent *func_ov018_021804f8(GameSystem *gsys, u32 param_2);

#endif // FIELD_EVENT_COMM_COMMON_H
