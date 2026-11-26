#include "global.h"

#include "bg_window.h"
#include "overlay_manager.h"
#include "render_window.h"
#include "save_arrays.h"
#include "overlay_74_mystery_gift_app_data.h"

void ov74_02229F60(Window *window, u8 skipTransfer);

s32 ov74_0222A89C(OverlayManager *, s32 *, s32, Window *, s32);

s32 ov74_0222CE10(void *);

extern s32 ov74_0223C2B0;

s32 ov74_0222A5CC(OverlayManager *arg0);

s32 ov74_0222A5CC(OverlayManager *man) {
    Window *window;
    MysteryGiftAppData *v0;

    v0 = OverlayManager_GetData(man);
    Save_MysteryGift_Get(v0->saveData);
    if (*((s32 *)&v0->unk_5C8) == 0x1E) {
        *((s32 *)&v0->unk_5CC) = 1;
        ov74_0222CE10(v0);
    }
    ov74_02229F60(&v0->menuWindows[0], 0);
    window = &v0->menuWindows[1];
    if (*((s32 *)&v0->menuWindows[1].bgConfig) == 0) {
        AddWindowParameterized(*(BgConfig **)v0, window, 0U, 0xFU, 9U, 0x10U, 8U, 0U, 0x124U);
    }
    DrawFrameAndWindow1(window, 1, 0x1FU, 3U);
    ov74_0222A89C(man, &ov74_0223C2B0, 4, window, 1);
    return 4;
}
