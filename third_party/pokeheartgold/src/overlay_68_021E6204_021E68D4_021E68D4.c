#include "global.h"

#include "bg_window.h"
#include "move.h"
#include "msgdata.h"
#include "sprite_system.h"
#include "overlay_68_move_relearner.h"

s32 ov68_021E6234(MoveRelearner *, s32, s32, s32, s32, u8);

s32 ov68_021E62D4(MoveRelearner *, s32, u32, s32, u8);

s32 ov68_021E7124(MoveRelearner *, u16);

void ov68_021E68D4(MoveRelearner *arg0, u32 arg1);

void ov68_021E68D4(MoveRelearner *arg0, u32 arg1) {
    MsgData *temp_r0_3;
    u32 temp_r0;
    FillWindowPixelBuffer(&arg0->windows[5], 0U);
    FillWindowPixelBuffer(&arg0->windows[3], 0U);
    FillWindowPixelBuffer(&arg0->windows[4], 0U);
    ManagedSprite_SetDrawFlag(arg0->sprites[3], 0);
    if (arg1 != (-2U)) {
        temp_r0 = GetMoveAttr(arg1, MOVEATTR_POWER);
        if (temp_r0 <= 1U) {
            ReadMsgDataIntoString(arg0->msgData, 0x21, arg0->string);
        } else {
            ov68_021E62D4(arg0, 0x1D, temp_r0, 3, 0);
        }
        ov68_021E6234(arg0, 3, 0, 0x10200, 1, 0);
        temp_r0 = GetMoveAttr(arg1, MOVEATTR_ACCURACY);
        if (temp_r0 == 0) {
            ReadMsgDataIntoString(arg0->msgData, 0x21, arg0->string);
        } else {
            ov68_021E62D4(arg0, 0x1E, temp_r0, 3, 0);
        }
        ov68_021E6234(arg0, 4, 0, 0x10200, 1, 0);
        temp_r0_3 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x2ED, HEAP_ID_66);
        ReadMsgDataIntoString(temp_r0_3, (s32)arg1, arg0->string);
        ov68_021E6234(arg0, 5, 0, 0x10200, 0, 0);
        DestroyMsgData(temp_r0_3);
        ov68_021E7124(arg0, (u16)arg1);
        ManagedSprite_SetDrawFlag(arg0->sprites[3], 1);
        ScheduleWindowCopyToVram(arg0->windows);
        ScheduleWindowCopyToVram(&arg0->windows[1]);
        ScheduleWindowCopyToVram(&arg0->windows[2]);
    } else {
        ClearWindowTilemapAndScheduleTransfer(arg0->windows);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[1]);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[2]);
    }
    ScheduleWindowCopyToVram(&arg0->windows[5]);
    ScheduleWindowCopyToVram(&arg0->windows[3]);
    ScheduleWindowCopyToVram(&arg0->windows[4]);
}
