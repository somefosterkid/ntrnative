#include "global.h"
struct BattleHallApp;

#include "bg_window.h"
#include "message_format.h"
#include "overlay_82_battle_hall_app.h"

u8 ov82_0223EF1C(u8 *param_1, Window *param_2, s32 param_3, u32 param_4, u32 param_5, u32 param_6, u8 param_7, u8 param_8, u8 param_9, u8 param_10);

u8 ov82_0223EF7C(u8 *param_1, s32 param_2, u8 param_3);

void ov82_0223EFB4(struct BattleHallApp *arg0, u32 arg1, s32 arg2);

u8 ov82_0223EF7C(u8 *param_1, s32 param_2, u8 param_3) {
    u8 uVar1;

    uVar1 = ov82_0223EF1C(param_1, (Window *)(param_1 + 0x4c), param_2, 1, 1, 0, 1, 2, 0xf, param_3);
    CopyWindowToVram((Window *)(param_1 + 0x4c));
    return uVar1;
}

void ov82_0223EFB4(BattleHallApp *arg0, u32 idx, s32 num) {
    BufferIntegerAsString(*((MessageFormat * *)&arg0->strTemplate), idx, num, 2U, PRINTING_MODE_LEFT_ALIGN, 1);
}
