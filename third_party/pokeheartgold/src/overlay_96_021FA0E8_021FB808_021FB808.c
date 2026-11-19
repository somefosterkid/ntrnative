#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

#include "overlay_96_unk_struct_ov96_021_f94a8.h"

void ov96_021FB808(UnkStruct_ov96_021F94A8 *arg0, s32 arg1);

void ov96_021FB808(UnkStruct_ov96_021F94A8 *arg0, s32 arg1) {
    MessageFormat *temp_r0;
    MsgData *temp_r7;
    String *temp_r0_2;

    temp_r7 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, arg0->heapID);
    temp_r0 = MessageFormat_New(arg0->heapID);
    BufferIntegerAsString(temp_r0, 0U, arg1 / 30, 3U, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(temp_r0, 1U, (s32)(0xA * (arg1 % 30)) / 30, 1U, PRINTING_MODE_LEFT_ALIGN, 1);
    temp_r0_2 = ReadMsgData_ExpandPlaceholders(temp_r0, temp_r7, 0x9CU, arg0->heapID);
    AddTextPrinterParameterizedWithColor(&arg0->window, FONT_UNK_4, temp_r0_2, 0U, 0U, 0xFFU, 0xF0E00U, NULL);
    String_Delete(temp_r0_2);
    MessageFormat_Delete(temp_r0);
    DestroyMsgData(temp_r7);
    CopyWindowToVram(&arg0->window);
}
