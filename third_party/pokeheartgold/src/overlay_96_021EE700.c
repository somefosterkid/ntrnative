#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "text.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "options.h"
#include "player_data.h"
#include "pm_string.h"
#include "overlay_96_text_box.h"

void ov96_021EE700(Window *arg0, MsgData *arg1, String *arg2, s32 arg3, enum Font arg4);

Ov96TextBox *ov96_021EE740(enum HeapID arg0);
s32 ov96_021EE9D8(BgConfig *, u8, enum HeapID);
void ov96_021EE75C(Ov96TextBox *arg0, BgConfig *arg1, s32 arg2, s32 arg3, SaveData *arg4);
void ov96_021EE808(Ov96TextBox *ptr);

void ov96_021EE700(Window *window, MsgData *msgData, String *arg2, s32 strno, enum Font fontId) {
    ReadMsgDataIntoString(msgData, strno, arg2);
    FillWindowPixelBuffer(window, 0U);
    AddTextPrinterParameterizedWithColor(window, fontId, arg2, 0U, 0U, 0xFFU, 0x10200U, NULL);
    ScheduleWindowCopyToVram(window);
}

Ov96TextBox *ov96_021EE740(enum HeapID heapID) {
    Ov96TextBox *dest;

    dest = Heap_Alloc(heapID, sizeof(Ov96TextBox));
    MI_CpuFill8(dest, 0U, sizeof(Ov96TextBox));
    dest->heapId = heapID;
    return dest;
}

void ov96_021EE75C(Ov96TextBox *arg0, BgConfig *arg1, s32 arg2, s32 arg3, SaveData *saveData) {
    u8 v0;

    v0 = 4;
    if (arg3 == 0) {
        v0 = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(saveData));
    }
    arg0->bgConfig = arg1;
    arg0->printerId = -1;
    arg0->bgLayer = arg2;
    arg0->unk_28 = arg3;
    arg0->speed = v0;
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, arg0->heapId);
    arg0->msgData2 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x137, arg0->heapId);
    arg0->msgFormat = MessageFormat_New(arg0->heapId);
    ResetAllTextPrinters();
    ov96_021EE9D8(arg0->bgConfig, arg0->bgLayer, arg0->heapId);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, arg0->heapId);
    BG_FillCharDataRange(arg0->bgConfig, (enum GFBgLayer)arg0->bgLayer, 0U, 1U, 0U);
}

void ov96_021EE808(Ov96TextBox *ptr) {
    String *string;

    DestroyMsgData(ptr->msgData);
    DestroyMsgData(ptr->msgData2);
    MessageFormat_Delete(ptr->msgFormat);
    string = ptr->string;
    if (string != NULL) {
        String_Delete(string);
    }
    Heap_Free(ptr);
}
