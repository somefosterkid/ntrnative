#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"
#include "overlay_96_ov96_rower_work.h"

extern WindowTemplate ov96_0221C5C4;

void ov96_021FFD4C(Ov96RowerWork *arg0);

s32 sub_0200606C(u32, s32);
void ov96_021FFE38(s32 arg0, u32 arg1, s32 arg2);

void ov96_021FFD4C(Ov96RowerWork *arg0) {
    AddWindow(arg0->bgConfig, &arg0->window, &ov96_0221C5C4);
    BG_FillCharDataRange(arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 1U, 0U);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, arg0->heapID);
}

void _ddiv(s32, s32, u32, u32);

s32 _dfix(void);

u64 _dflt(s32);

void ov96_021FFD80(Ov96RowerWork *param_1, s32 param_2, s32 param_3);

void ov96_021FFD80(Ov96RowerWork *param_1, s32 param_2, s32 param_3) {
    s32 num;
    MsgData *msgData;
    MessageFormat *messageFormat;
    String *string;
    u64 uVar1;

    FillWindowPixelBuffer(&param_1->window, 0);
    uVar1 = _dflt(param_3 * 10);
    _ddiv((s32)uVar1, (s32)((u64)uVar1 >> 0x20), 0, 0x40900000);
    num = _dfix();
    if (9 < num) {
        GF_AssertFail();
    }
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, param_1->heapID);
    messageFormat = MessageFormat_New(param_1->heapID);
    BufferIntegerAsString(messageFormat, 0, param_2, 2, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferIntegerAsString(messageFormat, 1, num, 1, PRINTING_MODE_LEADING_ZEROS, 1);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, 0xa2, param_1->heapID);
    AddTextPrinterParameterizedWithColor(&param_1->window, FONT_UNK_4, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
    CopyWindowToVram(&param_1->window);
    return;
}

void ov96_021FFE38(s32 arg0, u32 arg1, s32 arg2) {
    s32 v0;

    if ((arg0 >= 0) && (arg0 < 0x100)) {
        if (arg2 != 0) {
            v0 = 3;
        } else {
            v0 = 4;
        }
        sub_0200606C((u16)arg1, v0);
    }
}
