#include "global.h"
struct BattleFactoryApp;

#include "unk_020379A0.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_02240048(struct BattleFactoryApp *arg0, s32 arg1, s32 arg2);

s32 ov81_02240048(BattleFactoryApp *arg0, s32 arg1, s32 arg2) {
    u8 temp_r1;

    temp_r1 = arg0->subState;
    switch (temp_r1) {
    case 0:
        *((s8 *)&arg0->printerID) = ov81_0224086C(arg0, 0xF);
        sub_02037BEC();
        sub_02037AC0(0xA5U);
        arg0->subState = (u8)(arg0->subState + 1);
    default:
        break;
    case 1:
        if (sub_02037B38(0xA5U) == 1) {
            sub_02037BEC();
            return 1;
        }
        break;
    }
    return 0;
}
