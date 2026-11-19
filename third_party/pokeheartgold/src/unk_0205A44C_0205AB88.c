#include "global.h"
#include "mail_message_types.h"

struct MailMessage;
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
typedef struct UnionRoomGameInfo {
    u32 trainerIDs[4];
    u8 unused_10[4];
    u8 unused_14[4];
    u8 appearanceAndGender[4];
    u8 activity;
    u8 padding_1D[3];
} UnionRoomGameInfo;
void sub_02035838(MailMessage *param0);
void sub_02035854(void *param0);
void sub_0203588C(void);
void sub_0205AA4C(UnionRoomGameInfo *gameInfo);

void sub_0205AB88(MailMessage *sentence);

void sub_0205AB88(MailMessage *sentence) {
    UnionRoomGameInfo gameInfo;

    sub_0205AA4C(&gameInfo);
    gameInfo.activity = UR_STATUS_NONE;

    sub_02035838(sentence);
    sub_02035854(&gameInfo);
    sub_0203588C();
}
