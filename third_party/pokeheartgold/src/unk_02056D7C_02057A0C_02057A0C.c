#include "global.h"

#include "comm_player_manager.h"
#include "unk_02035900.h"

s32 sub_02057A0C(void);

s32 sub_02057A0C(void) {
    u16 arg0;
    BOOL BVar1;

    if (_021D41C4 == 0) {
        return 0;
    }
    arg0 = sub_0203769C();
    BVar1 = sub_020373B4(arg0);
    if (BVar1 != 0) {
        return 1;
    }
    return 0;
}
