#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_112_sprite_app.h"

extern WindowTemplate ov112_021FF350;

void ov112_021F1288(Ov112SpriteApp *arg0);

void ov112_021F1288(Ov112SpriteApp *arg0) {
    WindowTemplate *var_r6;
    Window *var_r4;
    s32 var_r7;

    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (arg0->bgConfig == NULL) {
        GF_AssertFail();
    }
    arg0->msgData[0] = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x113, arg0->heapID);
    arg0->msgData[1] = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xED, arg0->heapID);
    arg0->msgData[2] = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xDE, arg0->heapID);
    arg0->msgFormat = MessageFormat_New_Custom(0xDU, 0x20U, arg0->heapID);
    var_r6 = &ov112_021FF350;
    var_r7 = 0;
    var_r4 = &arg0->windows[0];
    do {
        AddWindow(arg0->bgConfig, var_r4, var_r6);
        var_r6 = (void *)((u8 *)var_r6 + (8));
        var_r4 = (void *)((u8 *)var_r4 + (0x10));
        var_r7 += 1;
    } while (var_r7 < 4);
    BG_FillCharDataRange(arg0->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 1U, 0U);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, arg0->heapID);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_14_OFFSET, arg0->heapID);
    FontID_Alloc(FONT_UNK_4, arg0->heapID);
}
