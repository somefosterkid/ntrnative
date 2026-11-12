// Black's code at main 0x0202751c-0x020275e8. The name is pokebw2's file between game_beacon.c and app_menu_common.c (7
// functions there, 10 here).

#include "types.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "struct_decls.h"
#include "system/app_menu_common.h"
#include "system/game_beacon.h"
#include "field/beacon_view.h"
#include "gfl/msg.h"

// The game's beacon status, kept in GameData (pokebw2's BeaconStatus): Black's lacks the Pass Power records and the
// given Pass Power that Black 2's has after the ring
typedef struct BeaconStatus {
    /* 0x000 */ GameBeaconInfoTable table;
    // The greeting that the player's beacons send
    /* 0xb08 */ StrBuf *greeting;
    // Set by the phrase input when the player enters a new greeting
    /* 0xb0c */ u16 greetingChanged;
    // The top row of the beacon view's list
    /* 0xb0e */ u8 logTop;
    /* 0xb0f */ u8 unk_b0f;
} BeaconStatus; // size: 0xb10

// pokebw2's BeaconStatus_Create
BeaconStatus *func_0202751c(HeapID heapId, HeapID tempHeapId) {
    BeaconStatus *status;
    MsgData *msgData;
    StrBuf *strBuf;
    u16 buf[9];

    status = GFL_HeapAllocate(heapId, sizeof(BeaconStatus), TRUE, "beacon_status.c", 42);
    status->greeting = GFL_StrBufCreate(9, heapId);
    msgData = GFL_MsgSysLoadData(FALSE, 2, 8, tempHeapId);
    strBuf = GFL_MsgDataLoadStrbufNew(msgData, 0x11);
    GFL_StrBufStoreString(strBuf, buf, 9);
    GFL_StrBufLoadString(status->greeting, buf);
    GFL_StrBufFree(strBuf);
    GFL_MsgDataFree(msgData);
    return status;
}

// pokebw2's BeaconStatus_Free
void func_02027584(BeaconStatus *status) {
    GFL_StrBufFree(status->greeting);
    MI_CpuFill8(status, 0, sizeof(BeaconStatus));
    GFL_HeapFree(status);
}

// pokebw2's BeaconStatus_GetInfoTable
GameBeaconInfoTable *func_020275a8(BeaconStatus *status) {
    return &status->table;
}

// pokebw2's BeaconStatus_GetLogTop
u8 func_020275ac(BeaconStatus *status) {
    return status->logTop;
}

// pokebw2's BeaconStatus_SetLogTop
void func_020275b8(BeaconStatus *status, u8 value) {
    status->logTop = value;
}

// pokebw2's BeaconStatus_GetGreeting
StrBuf *func_020275c4(BeaconStatus *status) {
    return status->greeting;
}

// pokebw2's BeaconStatus_GetGreetingChanged
u16 *func_020275d0(BeaconStatus *status) {
    return &status->greetingChanged;
}

BOOL func_020275dc(void) {
    return 0;
}

void func_020275e0(void) {
}

u32 getUINarcIdx(void) {
    return 0x53;
}
