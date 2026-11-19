#include "global.h"

#include "overlay_102_easy_chat_sentence_view.h"

void ov102_021EA678(EasyChatSentenceView *view, s32 enable) {
    reg_G2_WINOUT = (reg_G2_WINOUT & ~0x3f) | 0x1f;
    if (enable != 0) {
        reg_G2_WININ = (reg_G2_WININ & ~0x3f) | 0x1a;
        reg_G2_WININ = (reg_G2_WININ & ~0x3f00) | 0x1b00;
        reg_G2_WIN0H = 0xff;
        reg_G2_WIN0V = 0x30;
        reg_G2_WIN1H = 0xff;
        reg_G2_WIN1V = 0xa0c0;
        reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffff1fff | 0x6000;
        return;
    }
    reg_G2_WININ = (reg_G2_WININ & ~0x3f) | 0x1f;
    reg_G2_WININ = (reg_G2_WININ & ~0x3f00) | 0x1f00;
    reg_G2_WIN0H = 0;
    reg_G2_WIN0V = 0;
    reg_G2_WIN1H = 0;
    reg_G2_WIN1V = 0;
    reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffff1fff;
    return;
}
