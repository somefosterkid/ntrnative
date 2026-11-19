#include "global.h"

#include "unk_02019BA4.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_02241C84(s32, s32, void *, void *);

void ov81_02241CA0(BattleFactoryApp *arg0, void *arg1, void *arg2);

void ov81_02241CA0(BattleFactoryApp *arg0, void *arg1, void *arg2) {
    u32 v0;
    u16 v2;
    u8 v1;

    v0 = (u32)GridInputHandler_GetNextInput(arg0->inputHandler);
    if (v0 < 6U) {
        v1 = arg0->numMonsSelected;
        v2 = 0;
        if ((s32)v1 > 0) {
loop_3:
            if (v0 != arg0->selectedIndices[v2]) {
                v2 += 1;
                if ((s32)v2 < (s32)v1) {
                    goto loop_3;
                }
            }
        }
        ov81_02241C84(v2, *((s32 *)&arg0->numMonsToFade), arg1, arg2);
    }
}
