#include "global.h"
#include "overlay_102_easy_chat_sentence_view.h"

s32 ov102_021E8F6C(struct EasyChatApp *app);

s32 ov102_021E8F84(struct EasyChatApp *app);

void ov102_021EA874(EasyChatSentenceView *view) {
    s32 v1;

    if (ov102_021E8F6C(view->app) == 2) {
        v1 = ov102_021E8F84(view->app) + 1;
        GX_LoadBGPltt(view->palette[v1], 0U, 0x20U);
        GXS_LoadBGPltt(view->palette[v1], 0U, 0x20U);
        return;
    }
    GX_LoadBGPltt(view->palette[0], 0U, 0x20U);
    GXS_LoadBGPltt(view->palette[0], 0U, 0x20U);
}

void ov102_021EA8C0(EasyChatSentenceView *view) {
    s32 v0;

    v0 = ov102_021E8F6C(view->app);
    switch (v0) {
    case 0:
        view->wordPos[0].x = 0x68;
        view->wordPos[0].y = 0x10;
        view->wordBoxCount = 1;
        return;
    case 1:
        view->wordPos[0].x = 0x30;
        view->wordPos[0].y = 0x10;
        view->wordPos[1].x = 0xA0;
        view->wordPos[1].y = 0x10;
        view->wordBoxCount = 2;
        return;
    case 2:
        view->wordBoxCount = 0;
        return;
    }
}
