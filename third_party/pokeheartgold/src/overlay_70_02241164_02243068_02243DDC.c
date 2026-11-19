#include "global.h"
struct UnkStruct_ov70_02242E58;

#include "bg_window.h"
#include "heap.h"
#include "list_menu_items.h"
#include "message_printer.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "text.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_022428C0(UnkStruct_ov70_02242E58 *work);

s32 ov70_02243DDC(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_02243DDC(UnkStruct_ov70_02242E58 *arg0) {
    s32 v0;

    v0 = ov70_022428C0(arg0);
    switch (v0) {
    case -1:
        break;
    case -2:
        sub_020198FC(arg0->unk_1C, 0, 4, 0, 4);
        Sprite_SetDrawFlag(arg0->unk_C, 0);
        Sprite_SetDrawFlag(arg0->unk_10, 0);
        Sprite_SetDrawFlag(arg0->unk_14, 0);
        arg0->unk_4C = 3;
        arg0->unk_50 = 0x21;
        arg0->unk_45 = -2;
        break;
    default:
        sub_020198FC(arg0->unk_1C, 0, 4, 0, 4);
        Sprite_SetDrawFlag(arg0->unk_C, 0);
        Sprite_SetDrawFlag(arg0->unk_10, 0);
        Sprite_SetDrawFlag(arg0->unk_14, 0);
        arg0->unk_4C = 3;
        arg0->unk_50 = 0x21;
        arg0->unk_45 = (s8)arg0->unk_34[v0 + (arg0->unk_5A * 4)].value;
        break;
    }
    return -1;
}

s32 ov70_02241DB4(void *, s32);

s8 ov70_02243E74(UnkStruct_ov70_02242E58 *arg0);

s8 ov70_02243E74(UnkStruct_ov70_02242E58 *arg0) {
    ov70_02241DB4(arg0, 2);
    ListMenuItems_Delete(arg0->unk_34);
    return arg0->unk_45;
}

s32 ov70_02242FC4(void *, Window *, String *, s32, s32);

void ov70_02243E8C(UnkStruct_0201956C *arg0, MsgData *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5);

void ov70_02243E8C(UnkStruct_0201956C *arg0, MsgData *msgData, void *arg2, s32 strno, s32 arg4, s32 arg5) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    ov70_02242FC4(arg0, arg2, string, arg4, arg5);
    String_Delete(string);
}

s32 sub_02019A60(UnkStruct_0201956C *, s32, Window *);

void ov70_02243EB8(UnkStruct_0201956C *arg0, MsgData *arg1, Window *arg2, s32 arg3);

void ov70_02243EB8(UnkStruct_0201956C *arg0, MsgData *msgData, Window *window, s32 strno) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    AddTextPrinterParameterizedWithColor(window, FONT_UNK_4, string, 0U, 0U, 0xFFU, 0xF0E02U, NULL);
    CopyWindowPixelsToVram_TextMode(window);
    sub_02019A60(arg0, 0, window);
    String_Delete(string);
}

void ov70_02243F00(UnkStruct_0201956C *arg0, MessagePrinter *arg1, Window *arg2, s32 arg3, u32 arg4);

void ov70_02243F00(UnkStruct_0201956C *arg0, MessagePrinter *messagePrinter, Window *window, s32 arg3, u32 num) {
    PrintUIntOnWindow(messagePrinter, arg3 + 1, 2U, PRINTING_MODE_RIGHT_ALIGN, window, 0U, 0U);
    sub_0200CDAC(messagePrinter, 0U, window, 0x10U, 0U);
    PrintUIntOnWindow(messagePrinter, num, 2U, PRINTING_MODE_LEFT_ALIGN, window, 0x18U, 0U);
    CopyWindowPixelsToVram_TextMode(window);
    sub_02019A60(arg0, 0, window);
}

void *ov70_0223F658(s32, s32, s32 *);

s32 ov70_0224342C(u8 *, struct Pokedex *, s32, u16 *);

s32 ov70_02243F54(struct UnkStruct_ov70_02242E58 *arg0, s32 arg1);

s32 ov70_02243F54(UnkStruct_ov70_02242E58 *arg0, s32 arg1) {
    s32 v0;
    s32 v1;
    u16 *ptr;

    ptr = ov70_0223F658(0x3D, arg1, &v0);
    v1 = ov70_0224342C(arg0->unk_30, arg0->unk_20, v0, ptr);
    Heap_Free(ptr);
    return v1;
}
