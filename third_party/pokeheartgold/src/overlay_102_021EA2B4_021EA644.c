#include "global.h"

#include "bg_window.h"
#include "overlay_102_easy_chat_sentence_view.h"

BgConfig *ov102_021EA268(struct UnkStruct_ov20_021D2128_t *parent);

void ov102_021EA644(void *view_) {
    EasyChatSentenceView *view = view_;
    BgConfig *bgConfig;

    bgConfig = ov102_021EA268(view->parent);
    if ((s32) * &reg_GX_VCOUNT < 0x30) {
        BgSetPosTextAndCommit(bgConfig, 0U, BG_POS_OP_SET_XSCALE, (s32) view->scrollX);
        return;
    }
    BgSetPosTextAndCommit(bgConfig, 0U, BG_POS_OP_SET_XSCALE, 0);
}
