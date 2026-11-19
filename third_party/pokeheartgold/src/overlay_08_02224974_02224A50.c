#include "global.h"
#include "battle/battle_bag_context.h"
#include "overlay_08_battle_bag.h"

u16 ov08_02223CD4(void *, u32);

s32 ov08_022247E4(void *, u8, s32, s32);

void ov08_02224A50(void *arg0, s32 arg1);

void ov08_02224A50(void *arg0, s32 arg1) {
    u32 v0;

    switch (arg1) {
    case 0:
        ov08_022247E4(arg0, 0U, 0, arg1);
        ov08_022247E4(arg0, 1U, 0, arg1);
        ov08_022247E4(arg0, 2U, 0, arg1);
        ov08_022247E4(arg0, 3U, 0, arg1);
        if (((BattleBag *)arg0)->context->lastUsedItem == 0) {
            ov08_022247E4(arg0, 4U, 3, arg1);
        } else {
            ov08_022247E4(arg0, 4U, 0, arg1);
        }
        ov08_022247E4(arg0, 5U, 0, arg1);
        return;
    case 1:
        v0 = 0;
        do {
            if (ov08_02223CD4(arg0, v0) == 0) {
                ov08_022247E4(arg0, (u8)(v0 + 6), 3, arg1);
            } else {
                ov08_022247E4(arg0, (u8)(v0 + 6), 0, arg1);
            }
            v0 += 1;
        } while (v0 < 6U);
        if (((BattleBag *)arg0)->numBattlePocketPages[((BattleBag *)arg0)->currentBattlePocket] == 0) {
            ov08_022247E4(arg0, 0xCU, 3, arg1);
            ov08_022247E4(arg0, 0xDU, 3, arg1);
        } else {
            ov08_022247E4(arg0, 0xCU, 0, arg1);
            ov08_022247E4(arg0, 0xDU, 0, arg1);
        }
        ov08_022247E4(arg0, 0xEU, 0, arg1);
        return;
    case 2:
        ov08_022247E4(arg0, 0xFU, 0, arg1);
        ov08_022247E4(arg0, 0x10U, 0, arg1);
        return;
    }
}
