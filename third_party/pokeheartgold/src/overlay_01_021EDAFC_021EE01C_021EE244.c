#include "global.h"
struct FieldMenuManager;

#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_01_field_menu_manager.h"

void MoveTutorMenu_SetListItem_Internal(FieldMenuManager *menuManager, s32 strno, s32 altTextID, s32 value);

void MoveTutorMenu_SetListItem_Internal(FieldMenuManager *menuManager, s32 strno, s32 altTextID, s32 value) {
    s32 v0;
    String *string;
    u8 v1;

    v0 = strno;
    string = String_New(0x50U, HEAP_ID_FIELD1);
    ReadMsgDataIntoString(menuManager->messageLoader, strno, string);
    StringExpandPlaceholders(menuManager->stringTemplate, menuManager->choicesStringsBuffers[menuManager->optionsCount], string);
    v1 = menuManager->optionsCount;
    menuManager->listMenuChoicesStrings[v1].text = menuManager->choicesStringsBuffers[v1];
    String_Delete(string);
    if (value == 0xFA) {
        menuManager->listMenuChoicesStrings[menuManager->optionsCount].value = -3;
    } else {
        menuManager->listMenuChoicesStrings[menuManager->optionsCount].value = value;
    }
    menuManager->choicesAltTextStringIDs[menuManager->optionsCount] = altTextID;
    menuManager->optionsCount++;
}

s32 ov01_021EE2E4(struct FieldMenuManager *arg0);

s32 ov01_021EE2E4(FieldMenuManager *arg0) {
    String *string;
    s32 v1;
    u32 v0;
    u32 v3;

    v3 = 0;
    v1 = 0;
    if ((s32) arg0->optionsCount > 0) {
loop_2:
        string = arg0->listMenuChoicesStrings[v1].text;
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
