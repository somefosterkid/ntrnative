#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "text.h"
#include "unk_02005D10.h"
#include "overlay_112_sub_screen.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021F0314(void);

s32 ov112_021F0314(void) {
    if (IsPaletteFadeFinished() == 1) {
        return 0x22;
    }
    return 0x21;
}

s32 ov112_021F0328(Ov112PokewalkerApp *arg0) {
    s32 v0;

    if (arg0->unk_9D7A == 0) {
        v0 = ov112_021EA08C(arg0, 0, 0x2F);
    } else {
        PlayFanfare(0x4A1U);
        v0 = ov112_021EA08C(arg0, 0, 0x2D);
    }
    arg0->unk_1E524 = v0;
    arg0->unk_C = 8;
    return 0x23;
}

s32 ov112_021F0368(Ov112PokewalkerApp *arg0) {
    s32 v0;

    if ((TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) && (IsFanfarePlaying() == 0)) {
        v0 = arg0->unk_C;
        arg0->unk_8 = v0;
        return v0;
    }
    return 0x23;
}

s32 ov112_021F0394(void *arg0);

s32 ov112_021F0394(void *arg0) {
    *(s32 *)((u8 *)arg0 + 4) = 0;
    return 0x25;
}

Ov112SubScreen *ov112_021F039C(enum HeapID arg0);

Ov112SubScreen *ov112_021F039C(enum HeapID heapID) {
    Ov112SubScreen *dest;

    dest = Heap_Alloc(heapID, sizeof(Ov112SubScreen));
    MI_CpuFill8(dest, 0U, sizeof(Ov112SubScreen));
    dest->heapID = heapID;
    dest->sprites.heapID = heapID;
    return dest;
}

s32 ov112_021F05FC(BgConfig *, enum HeapID);

void ov112_021F09B4(Ov112SubScreenSprites *arg0);

void ov112_021F0B28(Ov112SubScreenSprites *arg0);

void ov112_021F03BC(Ov112SubScreen *arg0, BgConfig *arg1);

void ov112_021F03BC(Ov112SubScreen *arg0, BgConfig *arg1) {
    arg0->bgConfig = arg1;
    ov112_021F05FC(arg1, arg0->heapID);
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[0], 5U, 0xEU, 9U, 0xAU, 2U, 0xFU, 0x1DU);
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[1], 5U, 0x17U, 6U, 5U, 2U, 0xFU, 0x11U);
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[2], 5U, 0xEU, 0xCU, 8U, 2U, 0xFU, 0x31U);
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[3], 5U, 0xEU, 6U, 8U, 2U, 0xFU, 1U);
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[4], 5U, 0x16U, 6U, 1U, 2U, 0xFU, 0x1BU);
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[5], 5U, 0xEU, 0xEU, 0x10U, 2U, 0xFU, 0x41U);
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x114, arg0->heapID);
    arg0->msgFormat = MessageFormat_New(arg0->heapID);
    arg0->string = String_New(0xBU, arg0->heapID);
    ov112_021F09B4(&arg0->sprites);
    ov112_021F0B28(&arg0->sprites);
    GfGfx_EngineBTogglePlanes(2U, 0U);
    GfGfx_EngineBTogglePlanes(4U, 0U);
}
