// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/pml/move_reminder.h: the lines in use so
// far.
#ifndef BWPORT_PML_MOVE_REMINDER_H
#define BWPORT_PML_MOVE_REMINDER_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "gfl/overlay.h"

// Black 2's; twinned code confirms the offsets of moves and status, and its size in Black
struct MoveReminderProcessData {
    PartyPkm *pkm;
    PlayerInfo *playerInfo;
    void *unk08;
    GameSystem *gsys;
    u16 *moves;
    u16 unk14;
    u16 unk16;
    u8 unk18;
    u8 unk19;
    u8 status;
    u8 unk1B;
};

MoveReminderProcessData *func_ov010_0216f918(HeapID heapId);

// Black 2's, not checked against Black
#define OVERLAY_MOVE_REMINDER OVERLAY_ID(258)

// How the screen was left
enum {
    MOVE_REMINDER_STATUS_LEARNED,
    MOVE_REMINDER_STATUS_CANCELLED,
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov010_0216f940(MoveReminderProcessData *data);

#endif // BWPORT_PML_MOVE_REMINDER_H
