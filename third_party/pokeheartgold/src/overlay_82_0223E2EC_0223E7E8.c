#include "global.h"
struct BattleHallApp;

#include "unk_020379A0.h"
#include "overlay_82_battle_hall_app.h"

s32 ov82_0223E7E8(struct BattleHallApp *arg0);

s32 ov82_0223E7E8(BattleHallApp *arg0) {
    u8 temp_r0;

    temp_r0 = arg0->subState;
    switch (temp_r0) {
    case 0:
        sub_02037BEC();
        sub_02037AC0(0x68U);
        arg0->subState = (u8)(arg0->subState + 1);
    default:
        break;
    case 1:
        if (sub_02037B38(0x68U) == 1) {
            sub_02037BEC();
            return 1;
        }
        break;
    }
    return 0;
}
