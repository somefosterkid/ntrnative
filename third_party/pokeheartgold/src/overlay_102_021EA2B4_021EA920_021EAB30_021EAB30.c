#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_102_easy_chat_sentence_view.h"

u16 ov102_021E8F90(struct EasyChatApp *app, s32 slot);

String *ov102_021E8FA8(struct EasyChatApp *app, s32 id);

s32 ov102_021EAB30(EasyChatSentenceView *view, Window *window) {
    String *sp20;
    s32 sp1C;
    s32 sp18;
    u32 var_r5;
    u32 var_r4;
    u32 temp_r0;
    u16 temp_r3;
    EasyChatSentenceViewPos *pos;

    sp20 = ov102_021E8FA8(view->app, 0x23);
    sp18 = 1;
    sp1C = 0;
    var_r4 = 0;
    var_r5 = 0;
    ov102_021EAC44(&view->reader, view);
    do {
        temp_r0 = ov102_021EAC7C(&view->reader, sp20);
        switch (temp_r0) {
        case 0:
            AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, sp20, var_r5, var_r4, 0xFFU, 0x3040DU, NULL);
            var_r5 += FontID_String_GetWidth(FONT_MESSAGE, sp20, 0U);
            break;
        case 1:
            view->wordPos[sp1C].x = (s16)(var_r5 + 0x32);
            view->wordPos[sp1C].y = (s16)(var_r4 + 8);
            pos = &view->wordPos[sp1C];
            ov102_021EAD70(window, pos);
            temp_r3 = ov102_021E8F90(view->app, sp1C);
            if (temp_r3 != 0xFFFF) {
                ov102_021EAD98(view, window, pos, temp_r3);
            }
            sp1C += 1;
            var_r5 += 0x64;
            break;
        case 2:
            var_r4 += 0x10;
            var_r5 = 0;
            break;
        case 3:
            sp18 = 0;
            break;
        }
    } while (sp18 != 0);
    ov102_021EAC70(&view->reader);
    String_Delete(sp20);
    return sp1C;
}
