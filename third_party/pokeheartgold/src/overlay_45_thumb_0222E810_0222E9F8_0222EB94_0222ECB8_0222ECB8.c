#include "global.h"

#include "error_handling.h"
#include "save.h"
#include "unk_0202C034.h"
#include "ppw_lobby.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

struct PPW_LobbyQuestionnaire;

struct PPW_LobbyQuestionnaireRecord;

struct PPW_LobbyRecruitInfo;

struct PPW_LobbySystemProfile;

struct PPW_LobbyTimeInfo;

struct PPW_LobbyVipRecord;

struct SaveData;

struct WiFiHistory;

struct WiFiList;

typedef struct WiFiHistory WiFiHistory;

void ov45_0222ECB8(s64 *param0);

void ov45_0222ECB8(s64 *param0) {
    GF_ASSERT(_022577C0 != NULL);
    *param0 = _022577C0->timeInfo.currentTime;
}

struct PPW_LobbySchedule;

struct PPW_LobbyScheduleRecord;



typedef enum PPW_LOBBY_ROOM_FLAG {
    PPW_LOBBY_ROOM_FLAG_ARCEUS = 0x00000001
} PPW_LOBBY_ROOM_FLAG;

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

typedef enum {
    UnkEnum_ov66_0223287C_00,
    UnkEnum_ov66_0223287C_01,
    UnkEnum_ov66_0223287C_02,
    UnkEnum_ov66_0223287C_03,
    UnkEnum_ov66_0223287C_04
} UnkEnum_ov66_0223287C;

typedef enum {
    UnkEnum_ov66_02232BEC_00,
    UnkEnum_ov66_02232BEC_01,
    UnkEnum_ov66_02232BEC_02,
    UnkEnum_ov66_02232BEC_03,
    UnkEnum_ov66_02232BEC_04,
    UnkEnum_ov66_02232BEC_05,
    UnkEnum_ov66_02232BEC_06,
    UnkEnum_ov66_02232BEC_07,
    UnkEnum_ov66_02232BEC_08,
} UnkEnum_ov66_02232BEC;

u32 ov45_022303BC(const PPW_LobbySchedule *lobbySchedule, PPW_LOBBY_TIME_EVENT lobbyTimeEvent);

u32 ov45_0222ECDC(UnkEnum_ov66_02232BEC param0);

u32 ov45_0222ECDC(UnkEnum_ov66_02232BEC param0) {
    u32 v0;

    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT((_022577C0->unk_12C == UnkEnum_ov66_0223287C_02) || (_022577C0->unk_12C == UnkEnum_ov66_0223287C_04));
    GF_ASSERT(param0 < UnkEnum_ov66_02232BEC_08);

    PPW_LobbySchedule *lobbySchedule = (PPW_LobbySchedule *)_022577C0->unk_1B4;

    switch (param0) {
    case UnkEnum_ov66_02232BEC_00:
        v0 = lobbySchedule->lockTime;
        break;
    case UnkEnum_ov66_02232BEC_01:
        v0 = lobbySchedule->random;
        break;
    case UnkEnum_ov66_02232BEC_02:
        v0 = lobbySchedule->roomType;
        break;
    case UnkEnum_ov66_02232BEC_03:
        v0 = lobbySchedule->season;
        break;
    case UnkEnum_ov66_02232BEC_04:
        if (lobbySchedule->roomFlag & PPW_LOBBY_ROOM_FLAG_ARCEUS) {
            v0 = 1;
        } else {
            v0 = 0;
        }
        break;
    case UnkEnum_ov66_02232BEC_05:
        v0 = ov45_022303BC(lobbySchedule, PPW_LOBBY_TIME_EVENT_CLOSE);
        break;
    case UnkEnum_ov66_02232BEC_06:
        v0 = ov45_022303BC(lobbySchedule, PPW_LOBBY_TIME_EVENT_FIRE_WORKS_START);
        break;
    case UnkEnum_ov66_02232BEC_07:
        v0 = ov45_022303BC(lobbySchedule, PPW_LOBBY_TIME_EVENT_FIRE_WORKS_END);
        break;
    }

    return v0;
}
