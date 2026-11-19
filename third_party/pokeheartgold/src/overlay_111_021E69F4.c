#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "message_format.h"
#include "overlay_111.h"
#include "render_window.h"
#include "text.h"
#include "unk_0200A090.h"

void ov111_021E69F4(SwapMonText *text) {
    s32 printerId;

    printerId = text->printerId;
    if (printerId != -1) {
        RemoveTextPrinter((u8)printerId);
    }
    if (text->window.bgConfig != NULL) {
        ClearFrameAndWindow2(&text->window, 0);
        ClearWindowTilemapAndCopyToVram(&text->window);
        RemoveWindow(&text->window);
    }
}

MessageFormat *ov111_021E6A2C(SwapMonText *text) {
    ((text) ? (void)0 : GF_AssertFail());
    ((text->msgFormat) ? (void)0 : GF_AssertFail());

    return text->msgFormat;
}

void ov111_021E6A44(BgConfig *bgConfig, u8 layer, enum HeapID heapID) {
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, heapID);
    LoadUserFrameGfx2(bgConfig, (enum GFBgLayer)layer, 0x3D2U, 0xDU, 0U, heapID);
}
