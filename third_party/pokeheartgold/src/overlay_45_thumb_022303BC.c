#include "global.h"
#include "error_handling.h"
#include "heap.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

struct PPW_LobbySchedule;

struct PPW_LobbyScheduleRecord;



typedef struct PPW_LobbyScheduleRecord {
    s32 time;
    PPW_LOBBY_TIME_EVENT event;
} PPW_LobbyScheduleRecord;

typedef struct PPW_LobbySchedule {
    u32 lockTime;
    u32 random;
    u32 roomFlag;
    u8 roomType;
    u8 season;
    u16 scheduleRecordNum;
    PPW_LobbyScheduleRecord scheduleRecords[1];
} PPW_LobbySchedule;

u32 ov45_022303BC(const PPW_LobbySchedule *lobbySchedule, PPW_LOBBY_TIME_EVENT lobbyTimeEvent);

void ov45_022303E4(UnkStruct_ov66_022336C4_sub5 *arg0, s16 arg1, enum HeapID arg2);
void ov45_022303FC(UnkStruct_ov66_022336C4_sub5 *arg0);
void ov45_0223040C(UnkStruct_ov66_022336C4_sub5 *arg0, const PPW_LobbyVipRecord *arg1, u32 arg2);

u32 ov45_022303BC(const PPW_LobbySchedule *lobbySchedule, PPW_LOBBY_TIME_EVENT lobbyTimeEvent) {
    u32 time = 0;

    for (int i = 0; i < lobbySchedule->scheduleRecordNum; i++) {
        if (lobbySchedule->scheduleRecords[i].event == lobbyTimeEvent) {
            time = lobbySchedule->scheduleRecords[i].time;
        }
    }

    return time;
}

void ov45_022303E4(UnkStruct_ov66_022336C4_sub5 *arg0, s16 arg1, enum HeapID heapID) {
    arg0->lobbyVipRecord = Heap_Alloc(heapID, arg1 * sizeof(PPW_LobbyVipRecord));
    arg0->unk_04 = arg1;
    arg0->unk_06 = 0;
}

void ov45_022303FC(UnkStruct_ov66_022336C4_sub5 *arg0) {
    Heap_Free(arg0->lobbyVipRecord);
    arg0->lobbyVipRecord = NULL;
}

void ov45_0223040C(UnkStruct_ov66_022336C4_sub5 *arg0, const PPW_LobbyVipRecord *srcp, u32 arg2) {
    u16 v0;
    u32 v1;

    v1 = arg2;
    if (v1 >= (u32)arg0->unk_04) {
        GF_AssertFail();
    }
    v0 = arg0->unk_04;
    if (v1 >= (u32)v0) {
        v1 = (u32)v0;
    }
    MIi_CpuCopy32(srcp, arg0->lobbyVipRecord, v1 * sizeof(PPW_LobbyVipRecord));
    arg0->unk_06 = (s16)v1;
}
