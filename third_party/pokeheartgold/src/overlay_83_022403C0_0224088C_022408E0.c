#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "overlay_83_battle_castle_self_app.h"

u16 ov83_02240F48(struct BattleCastleSelfApp *, u32, s32);

void ov83_02247264(struct BattleCastleAppSpriteManager *, u32, u32);

void ov83_022472A0(struct BattleCastleAppSpriteManager *, u32, u32);

void ov83_0224755C(void *, u32);

void ov83_022479E4(Window *, MsgData *, s32, u32, u32, u32, u32, s32);

void ov83_022408E0(BattleCastleSelfApp *param_1, u32 param_2);

void ov83_022408E0(BattleCastleSelfApp * param_1, u32 param_2) {
    u32 uVar1;
    u16 uVar2;

    FillWindowPixelBuffer(&param_1->windows[61], 0);
    uVar2 = param_2 + param_1->page * 6 & 0xffff;
    ov83_0224755C(param_1->subScreenSprite, 0);
    if ((param_2 < 6) && (uVar2 < param_1->numEntries)) {
        uVar1 = ov83_02240F48(param_1, uVar2, param_1->selectedMenuEntry);
        ov83_022479E4(&param_1->windows[61], param_1->itemMsgLoader, uVar1, 0, 0, 0, 0x10200, 0);
        ov83_02247264(&param_1->spriteMan, 3, uVar1);
        ov83_022472A0(&param_1->spriteMan, 3, uVar1);
        ov83_0224755C(param_1->subScreenSprite, 1);
    }
    ScheduleWindowCopyToVram(&param_1->windows[61]);
    return;
}
