#include "global.h"
struct UnkStruct_ov22_0225A0E4;

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"

void ov41_02247C7C(struct UnkStruct_ov22_0225A0E4 *arg0, s32 arg1);

void ov41_02247C7C(UnkStruct_ov22_0225A0E4 *arg0, s32 strno) {
    MsgData *msgData;
    String *string;
    String *v0;

    FillWindowPixelBuffer(*((Window * *)&arg0->unk_6BC), 0xFU);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xD7, HEAP_ID_13);
    v0 = NewString_ReadMsgData(msgData, strno);
    string = String_New(0x100U, HEAP_ID_13);
    StringExpandPlaceholders(*((MessageFormat * *)&arg0->unk_6E8), string, v0);
    AddTextPrinterParameterizedWithColor(*((Window * *)&arg0->unk_6BC), FONT_MESSAGE, string, 0U, 0U, 0U, 0x1020FU, NULL);
    String_Delete(string);
    String_Delete(v0);
    DestroyMsgData(msgData);
    CopyWindowToVram(*((Window * *)&arg0->unk_6BC));
}

void ov41_02247D00(void *arg0);

void ov41_02247D00(void *arg0) {
    ClearWindowTilemapAndCopyToVram(*(Window **)((u8 *)arg0 + 0x6BC));
    RemoveWindow(*(Window **)((u8 *)arg0 + 0x6BC));
}

s32 ov41_02247BB8(void *, s32, u8, u8, u8, u8);

void ov41_02247D1C(void *arg0, s32 arg1);

void ov41_02247D1C(void *arg0, s32 arg1) {
    ov41_02247BB8(arg0, arg1, 2, 1, 0x1B, 2);
}
