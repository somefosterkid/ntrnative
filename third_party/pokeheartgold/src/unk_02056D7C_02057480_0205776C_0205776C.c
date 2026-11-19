#include "global.h"

#include "comm_player_manager.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_02056D7C.h"

void sub_0205724C(u32, u32, u32);

void sub_0205776C(int unused0, int unused1, void *in_r2, void *unused3) {
    u16 uVar1;
    u32 arg0;

    arg0 = (u32) * (u8 *)in_r2;
    if ((_021D41C4 != 0) && (uVar1 = sub_0203769C(), arg0 != uVar1)) {
        sub_0205724C(arg0, 0, 0);
        sub_020346E8(arg0);
    }
    return;
}
