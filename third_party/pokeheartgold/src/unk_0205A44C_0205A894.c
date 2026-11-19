#include "global.h"
#include "player_data.h"

struct PlayerProfile;
PlayerProfile *sub_02034818(int netId);
u16 sub_0203769C(void);
typedef u16 charcode_t;
typedef struct UnionRoomGameInfo {
    u32 trainerIDs[4];
    u8 unused_10[4];
    u8 unused_14[4];
    u8 appearanceAndGender[4];
    u8 activity;
    u8 padding_1D[3];
} UnionRoomGameInfo;

u8 sub_0205A894(void);
void sub_0205A8B4(UnionRoomGameInfo *gameInfo, int unused);

enum UnionRoomStatus {
    UR_STATUS_NONE = 0,
    UR_STATUS_DRAWING,
    UR_STATUS_MIXING_RECORDS,
    UR_STATUS_UNK_03,
    UR_STATUS_BUSY,
    UR_STATUS_SHOWING_TRAINER_CASE,
    UR_STATUS_BATTLING,
    UR_STATUS_TRADING,
    UR_STATUS_UNK_08,
    UR_STATUS_UNK_09,
    UR_STATUS_UNK_10,
    UR_STATUS_UNK_11,
    UR_STATUS_UNK_12,
    UR_STATUS_SPIN_TRADING,
};
void sub_02035854(void *param0);
void sub_0203588C(void);
void sub_0205A904(enum UnionRoomStatus status);

u8 sub_0205A894(void) {
    PlayerProfile *trainerInfo = sub_02034818(sub_0203769C() ^ 1);
    ((trainerInfo != 0L) ? (void)0 : GF_AssertFail());

    return PlayerProfile_GetVersion(trainerInfo);
}

void sub_0205A8B4(UnionRoomGameInfo *gameInfo, int unused) {
    for (int i = 1; i < 4 + 1; i++) {
        int index = i - 1;
        PlayerProfile *trainerInfo = sub_02034818(i);

        if (trainerInfo != 0L) {
            if (sub_0203769C() == 0) {
                gameInfo->trainerIDs[index] = PlayerProfile_GetTrainerID(trainerInfo);
                gameInfo->appearanceAndGender[index] = PlayerProfile_GetAvatar(trainerInfo) | (PlayerProfile_GetTrainerGender(trainerInfo) << 7);
            }
        } else {
            gameInfo->trainerIDs[index] = 0;
            gameInfo->appearanceAndGender[index] = 0;
        }
    }
}

void sub_0205A904(enum UnionRoomStatus status) {
    UnionRoomGameInfo gameInfo;

    MI_CpuClear8(&gameInfo, sizeof(UnionRoomGameInfo));

    switch (status) {
    case UR_STATUS_NONE:
        break;
    case UR_STATUS_BUSY:
        break;
    case UR_STATUS_UNK_11:
        sub_0205A8B4(&gameInfo, 2);
        break;
    case UR_STATUS_TRADING:
    case UR_STATUS_SHOWING_TRAINER_CASE:
    case UR_STATUS_BATTLING:
        sub_0205A8B4(&gameInfo, 2);
        break;
    case UR_STATUS_UNK_08:
        sub_0205A8B4(&gameInfo, 5);
        break;
    case UR_STATUS_DRAWING:
        sub_0205A8B4(&gameInfo, 4);
        break;
    case UR_STATUS_UNK_09:
        sub_0205A8B4(&gameInfo, 5);
        break;
    case UR_STATUS_MIXING_RECORDS:
        sub_0205A8B4(&gameInfo, 4);
        break;
    case UR_STATUS_UNK_10:
    case UR_STATUS_UNK_12:
        sub_0205A8B4(&gameInfo, 5);
        break;
    case UR_STATUS_UNK_03:
    case UR_STATUS_SPIN_TRADING:
        sub_0205A8B4(&gameInfo, 4);
        break;
    }

    gameInfo.activity = status;

    sub_02035854(&gameInfo);
    sub_0203588C();
}
