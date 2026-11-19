#include "global.h"

#include "font.h"
#include "msgdata.h"
#include "overlay_01.h"
#include "pm_string.h"
#include "text.h"

u32 ov01_021F4234(Window *, String *, s32, s32);

extern s32 ov01_02206AD8;

extern s32 ov01_02206B04;

void ov01_021F426C(SaveInfoWindow *arg0);

void ov01_021F426C(SaveInfoWindow *arg0) {
    s32 v0;
    s32 *v3;
    String *string;
    u32 v1;
    u32 v4;
    u8 v2;

    v2 = GetFontAttribute(0U, 1);
    v0 = v2 + GetFontAttribute(0U, 3);
    v3 = &ov01_02206B04;
    v4 = 0;
    do {
        if ((arg0->saveInfo.pokedexCount != 0) || ((v1 = *(u32 *)v3, (v1 != 0xB)) && (v1 != 7))) {
            string = ReadMsgData_ExpandPlaceholders(arg0->strTemplate, arg0->msgLoader, *(u32 *)v3, arg0->heapID);
            AddTextPrinterParameterizedWithColor(arg0->window, FONT_SYSTEM, string, ov01_021F4234(arg0->window, string, *(s32 *)((u8 *)v3 + 4), 0), v0 * *(s32 *)((u8 *)v3 + 8), 0U, *(s32 *)(&ov01_02206AD8 + *(s32 *)((u8 *)v3 + 0xC)), NULL);
            String_Delete(string);
        }
        v4 += 1;
        v3 += 4;
    } while (v4 < 9U);
}
