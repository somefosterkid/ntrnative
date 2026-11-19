#include "global.h"

#include "comm_player_manager.h"
#include "unk_02056D7C.h"

s32 sub_0205724C(s32, s32, s32);

void sub_02056E38(void) {
    u32 v0;

    if (_021D41C4 != 0) {
        v0 = 0;
        do {
            sub_0205724C((u8)v0, 1, 1);
            v0 += 1;
        } while ((s32)v0 < 8);
    }
}
