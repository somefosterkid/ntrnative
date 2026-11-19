#include "global.h"

#include "unk_020379A0.h"
#include "unk_02037C94.h"

typedef void (*FieldCommTask)(void);
enum CommSync {
    SYNC_INIT_VS_LINK_BATTLE = 3,
    SYNC_START_VS_LINK_BATTLE,
    SYNC_5,

    SYNC_BATTLE_ROOM_RETURN = 30,

    SYNC_BATTLE_END_CONNECTION = 91,
    SYNC_PAUSE_BATTLE,

    SYNC_94 = 94,
    SYNC_SEND_TRAINER_CASE,

    SYNC_SWITCH_TRANSITION = 97,
    SYNC_CHANGE_TO_BATTLE_ROOM,
};
void sub_02056EA0(BOOL deletePlayerData);
void sub_020582F4(FieldCommTask task, u16 time);
void sub_02058A78(void);

void sub_02058A38(void);

void sub_02058A38(void) {
    if (sub_02037B38(SYNC_BATTLE_END_CONNECTION)) {
        sub_020398D4(0, 0);
        sub_02056EA0(1);
        sub_020582F4(sub_02058A78, 5);
    }
}
