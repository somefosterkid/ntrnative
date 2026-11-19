#include "global.h"
struct BattleHallApp;

#include "bg_window.h"
#include "overlay_82_battle_hall_app.h"

void ov82_0223F5E0(BgConfig *, u8, u8);

s32 ov82_0223F6E4(BattleHallApp *app);

u8 sub_02030BD0(u8, u8 *);

void ov82_0223F580(struct BattleHallApp *param_1, BgConfig *param_2);

void ov82_0223F580(BattleHallApp *param_1, BgConfig *param_2) {
    s32 iVar1;
    u32 uVar2;
    u32 uVar3;

    iVar1 = ov82_0223F6E4(param_1);
    if (iVar1 == 1) {
        uVar3 = 0;
        do {
            ov82_0223F5E0(param_2, uVar3 & 0xff, 3);
            uVar3 = uVar3 + 1;
        } while ((s32)uVar3 < 0x11);
    } else {
        uVar3 = 0;
        do {
            uVar2 = sub_02030BD0((u8)uVar3, *((void **)&param_1->typeRanks));
            if (uVar2 >= 0xA) {
                ov82_0223F5E0(param_2, uVar3 & 0xff, 3);
            }
            uVar3 = uVar3 + 1;
        } while ((s32)uVar3 < 0x11);
        ov82_0223F5E0(param_2, 0x13, 3);
    }
    ScheduleBgTilemapBufferTransfer(param_2, 3);
    return;
}
