#include "global.h"

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
BOOL sub_02037B38(u8 syncState);
void sub_020582F4(FieldCommTask task, u16 time);
void sub_0205882C(void);

void sub_02058854(void);

void sub_02058854(void) {
    if (sub_02037B38(SYNC_INIT_VS_LINK_BATTLE)) {
        sub_020582F4(sub_0205882C, 2);
    }
}
