#include "global.h"
struct FieldMenuManager;

#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_01_field_menu_manager.h"

void ov01_021EDD68(void *arg0, s32 arg1, s32 arg2);

void ov01_021EDD68(void *arg0, s32 strno, s32 arg2) {
    String *string;
    u8 v0;

    string = String_New(0x50U, HEAP_ID_FIELD1);
    ReadMsgDataIntoString(*(MsgData **)((u8 *)arg0 + 0x8C), strno, string);
    StringExpandPlaceholders(*(MessageFormat **)((u8 *)arg0 + 0x90), *(String **)((u8 *)(arg0 + (*(u8 *)((u8 *)arg0 + 0x9B) * 4)) + 0x1C), string);
    v0 = *(u8 *)((u8 *)arg0 + 0x9B);
    *(s32 *)((u8 *)(arg0 + (v0 * 8)) + 0xBC) = (s32) * (s32 *)((u8 *)(arg0 + (v0 * 4)) + 0x1C);
    String_Delete(string);
    *(s32 *)((u8 *)(arg0 + (*(u8 *)((u8 *)arg0 + 0x9B) * 8)) + 0xC0) = arg2;
    *(u8 *)((u8 *)arg0 + 0x9B) = (u8)(*(u8 *)((u8 *)arg0 + 0x9B) + 1);
}

s32 ov01_021EDDD8(struct FieldMenuManager *arg0);

s32 ov01_021EDDD8(FieldMenuManager *arg0) {
    String *string;
    s32 v1;
    u32 v0;
    u32 v3;

    v3 = 0;
    v1 = 0;
    if ((s32) arg0->optionsCount > 0) {
loop_2:
        string = arg0->menuChoicesStrings[v1].text;
        if (string != NULL) {
            v0 = FontID_String_GetWidth(FONT_SYSTEM, string, 0U);
            if (v3 < v0) {
                v3 = v0;
            }
            v1 += 1;
            if (v1 < (s32) arg0->optionsCount) {
                goto loop_2;
            }
        }
    }
    return v3 + 0xC;
}
