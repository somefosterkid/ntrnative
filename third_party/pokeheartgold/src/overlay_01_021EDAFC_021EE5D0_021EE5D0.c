#include "global.h"
struct FieldMenuManager;

#include "bg_window.h"
#include "list_menu.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_01_field_menu_manager.h"

static void ov01_021EE5D0(struct FieldMenuManager *arg0, s32 arg1, u32 arg2);

static void ov01_021EE5D0(FieldMenuManager *arg0, s32 strno, u32 textSpeed) {
    s32 v0;
    String *string;
    String *v1;

    v0 = strno;
    string = String_New(0x5AU, HEAP_ID_FIELD1);
    v1 = String_New(0x5AU, HEAP_ID_FIELD1);
    FillWindowPixelBuffer(arg0->parentWindow, 0xFU);
    ReadMsgDataIntoString(*((MsgData * *)&arg0->messageLoader), strno, string);
    StringExpandPlaceholders(*((MessageFormat * *)&arg0->stringTemplate), v1, string);
    AddTextPrinterParameterized(arg0->parentWindow, FONT_MESSAGE, v1, 0U, 0U, textSpeed, NULL);
    String_Delete(string);
    String_Delete(v1);
}

void ov01_021EE634(void *arg0);

void ov01_021EE634(void *arg0) {
    u16 v0;

    ListMenuGetCurrentItemArrayId(((FieldMenuManager *)arg0)->listMenu, arg0 + 0x1C2);
    v0 = *((u16 *)&((FieldMenuManager *)arg0)->choicesAltTextStringIDs[(((FieldMenuManager *)arg0)->listMenuAltTextIndex)]);
    if (v0 != 0xFF) {
        ov01_021EE5D0(arg0, v0, 0);
    }
}
