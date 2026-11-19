#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_02013534.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

MessageFormat *ov40_0222DAB0(s32);

s32 ov40_02230DCC(void *, String *);

String *sub_020315B8(u16 *, s32);

void ov40_0222D3E8(void *arg0, void *arg1, s32 arg2, void *arg3);

void ov40_0222D3E8(void *arg0, void *arg1, s32 strno, void *arg3) {
    Window window;
    String *v1;
    String *string;
    void *v0;
    MessageFormat *messageFormat;
    MessageFormat *v4;
    MsgData *msgData;
    String *v2;
    String *v3;
    String *v5;
    String *v6;
    String *v7;

    v0 = arg1;
    msgData = ((UnkStruct_0208C06C *)arg0)->unk_14.unk_34;
    if (strno == 8) {
        if (*(s32 *)((u8 *)(arg0 + (*(s32 *)((u8 *)arg3 + 0x18) * 4)) + 0x88C) == 0) {
            v7 = NewString_ReadMsgData(msgData, strno);
        } else {
            messageFormat = ov40_0222DAB0(0x6D);
            v5 = NewString_ReadMsgData(((UnkStruct_0208C06C *)arg0)->unk_14.unk_34, 7);
            v2 = sub_020315B8(*(void **)((u8 *)(arg0 + (*(s32 *)((u8 *)arg3 + 0x18) * 4)) + 0x88C), 0x6D);
            string = v2;
            ov40_02230DCC(arg0, string);
            v7 = String_New(0xFFU, HEAP_ID_109);
            BufferString(messageFormat, 0U, string, 0, 1, 2);
            StringExpandPlaceholders(messageFormat, v7, v5);
            String_Delete(v5);
            String_Delete(v2);
            MessageFormat_Delete(messageFormat);
        }
    } else if (strno == 0x29) {
        if (*(s32 *)((u8 *)(arg0 + (*(s32 *)((u8 *)arg3 + 0x18) * 4)) + 0x88C) == 0) {
            v7 = NewString_ReadMsgData(msgData, strno);
        } else {
            v4 = ov40_0222DAB0(0x6D);
            v6 = NewString_ReadMsgData(((UnkStruct_0208C06C *)arg0)->unk_14.unk_34, 7);
            v3 = sub_020315B8(*(void **)((u8 *)(arg0 + (*(s32 *)((u8 *)arg3 + 0x18) * 4)) + 0x88C), 0x6D);
            v1 = v3;
            ov40_02230DCC(arg0, v1);
            v7 = String_New(0xFFU, HEAP_ID_109);
            BufferString(v4, 0U, v1, 0, 1, 2);
            StringExpandPlaceholders(v4, v7, v6);
            String_Delete(v6);
            String_Delete(v3);
            MessageFormat_Delete(v4);
        }
    } else {
        v7 = NewString_ReadMsgData(msgData, strno);
    }
    InitWindow(&window);
    AddTextWindowTopLeftCorner(((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, &window, 0x14U, 2U, 0U, 0U);
    AddTextPrinterParameterizedWithColor(&window, FONT_SYSTEM, v7, 0U, 0U, 0xFFU, 0xE0D00U, NULL);
    TextOBJ_CopyFromBGWindow(*(TextOBJ **)((u8 *)v0 + 0x14), *(UnkStruct_02013910 **)((u8 *)v0 + 0x18), &window, HEAP_ID_109);
    String_Delete(v7);
    RemoveWindow(&window);
}
