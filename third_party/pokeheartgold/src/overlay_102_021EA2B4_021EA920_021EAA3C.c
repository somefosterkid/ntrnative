#include "global.h"

#include "bg_window.h"
#include "overlay_102_easy_chat_sentence_view.h"

s32 ov102_021E8F6C(struct EasyChatApp *app);

u16 ov102_021E8F90(struct EasyChatApp *app, s32 slot);

BgConfig *ov102_021EA268(struct UnkStruct_ov20_021D2128_t *parent);

void ov102_021EAA3C(EasyChatSentenceView *view) {
    s32 sp4;
    BgConfig *sp0;
    Window *var_r4;
    s32 temp_r0;
    s32 temp_r0_2;
    EasyChatSentenceViewPos *temp_r6;
    EasyChatSentenceViewPos *temp_r7;

    sp0 = ov102_021EA268(view->parent);
    ov102_021EA874(view);
    sp4 = 0;
    var_r4 = &view->windows[0];
    temp_r6 = &view->wordPos[0];
    temp_r7 = &view->wordPos[1];
    do {
        FillWindowPixelBuffer(var_r4, 0xDU);
        temp_r0_2 = ov102_021E8F6C(view->app);
        switch (temp_r0_2) {
        case 0:
            ov102_021EAD70(var_r4, temp_r6);
            ov102_021EAD98(view, var_r4, temp_r6, ov102_021E8F90(view->app, 0));
            break;
        case 1:
            ov102_021EAD70(var_r4, temp_r6);
            ov102_021EAD70(var_r4, temp_r7);
            ov102_021EAD98(view, var_r4, temp_r6, ov102_021E8F90(view->app, 0));
            ov102_021EAD98(view, var_r4, temp_r7, ov102_021E8F90(view->app, 1));
            break;
        case 2:
            view->wordBoxCount = ov102_021EAB30(view, var_r4);
            ov102_021EAC20(view);
            break;
        }
        CopyWindowPixelsToVram_TextMode(var_r4);
        var_r4++;
        temp_r0 = sp4 + 1;
        sp4 = temp_r0;
    } while (temp_r0 < 2);
    ScheduleBgTilemapBufferTransfer(sp0, 0U);
    ScheduleBgTilemapBufferTransfer(sp0, 4U);
}
