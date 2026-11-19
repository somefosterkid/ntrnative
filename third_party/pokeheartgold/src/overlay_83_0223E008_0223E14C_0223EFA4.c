#include "global.h"

#include "unk_020379A0.h"
#include "overlay_83_battle_castle_self_app.h"

s32 ov83_02241354(void *);

s32 ov83_02241368(BattleCastleSelfApp *, u16, u16);

s32 ov83_0223EFA4(BattleCastleSelfApp *arg0);

s32 ov83_0223EFA4(BattleCastleSelfApp *arg0) {
    u8 v0;
    u8 v1;

    v1 = arg0->subState;
    switch (v1) {
    case 0:
        if (ov83_02241368(arg0, 0xD, 0) == 1) {
            arg0->syncDelay = 0x1EU;
            arg0->subState = (u8)(arg0->subState + 1);
        }
    default:
        break;
    case 1:
        v0 = arg0->syncDelay;
        if (v0 != 0) {
            arg0->syncDelay = (u8)(v0 - 1);
        }
        if (arg0->syncDelay == 0) {
            sub_02037BEC();
            sub_02037AC0(0x83U);
            arg0->subState = (u8)(arg0->subState + 1);
        }
        break;
    case 2:
        if (sub_02037B38(0x83U) == 1) {
            sub_02037BEC();
            ov83_02241354(&arg0->windows[6]);
            return 1;
        }
        break;
    }
    return 0;
}
