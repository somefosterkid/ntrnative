#include "global.h"
struct FieldMenuManager;

#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_01_field_menu_manager.h"

void ov01_021EDD68(FieldMenuManager *arg0, s32 arg1, s32 arg2);

void ov01_021EDD68(FieldMenuManager *arg0, s32 strno, s32 arg2) {
    String *string;
    u8 v0;

    string = String_New(0x50U, HEAP_ID_FIELD1);
    ReadMsgDataIntoString(arg0->messageLoader, strno, string);
    StringExpandPlaceholders(arg0->stringTemplate, arg0->choicesStringsBuffers[arg0->optionsCount], string);
    v0 = arg0->optionsCount;
    arg0->menuChoicesStrings[v0].text = arg0->choicesStringsBuffers[v0];
    String_Delete(string);
    arg0->menuChoicesStrings[arg0->optionsCount].value = arg2;
    arg0->optionsCount = (u8)(arg0->optionsCount + 1);
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
