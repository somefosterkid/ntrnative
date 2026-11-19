#include "global.h"
struct UnkStruct_ov61_0222C664;

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_00_thumb.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "overlay_39_unk_struct_ov61_0222_c664.h"

void ov39_02228B04(void *arg0, MsgData *arg1, s32 arg2, u32 arg3);

void ov39_02228B04(void *arg0, MsgData *msgData, s32 strno, u32 textSpeed) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    StringExpandPlaceholders(((MessageFormat *)((UnkStruct_ov61_0222C664 *)arg0)->unk_20), ((UnkStruct_ov61_0222C664 *)arg0)->unk_34, string);
    String_Delete(string);
    FillWindowPixelBuffer(arg0 + 0x44, 0xFU);
    DrawFrameAndWindow2(arg0 + 0x44, 0, 1U, 0xAU);
    *((s32 *)&((UnkStruct_ov61_0222C664 *)arg0)->unk_40) = (s32)AddTextPrinterParameterized(arg0 + 0x44, FONT_MESSAGE, ((UnkStruct_ov61_0222C664 *)arg0)->unk_34, 0U, 0U, textSpeed, NULL);
    *((s32 *)&((UnkStruct_ov61_0222C664 *)arg0)->unk_8C) = 0;
    if ((textSpeed == 0xFF) || (textSpeed == 0)) {
        *((s32 *)&((UnkStruct_ov61_0222C664 *)arg0)->unk_40) = 0xFF;
    }
}

void ov39_02228B6C(struct UnkStruct_ov61_0222C664 *arg0, s32 arg1, s32 arg2);

void ov39_02228B6C(UnkStruct_ov61_0222C664 *arg0, s32 arg1, s32 arg2) {
    *((s32 *)&arg0->unk_08) = arg1;
    *((s32 *)&arg0->unk_0C) = arg2;
}

s32 ov39_02228B74(u32 arg0, u32 arg1);

s32 ov39_02228B74(u32 arg0, u32 arg1) {
    u32 v0;

    v0 = (u32)ov00_021E6A70(arg0, arg1);
    switch (v0) {
    case 1:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        return 1;
    default:
        return 0;
    }
}
