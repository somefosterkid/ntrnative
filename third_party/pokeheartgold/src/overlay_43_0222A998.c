#include "global.h"

#include "gf_gfx_loader.h"
#include "bg_window.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "render_text.h"
#include "text.h"
#include "overlay_43_palpad.h"

extern u8 ov43_0222F0C0[2];

void ov43_0222A998(PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222A9D8(PalPadGfx *gfx);
void ov43_0222A9F4(PalPadGfx *gfx, Window *window, s32 strno);
void ov43_0222AA70(PalPadGfx *gfx);

void ov43_0222A998(PalPadGfx *gfx, enum HeapID heapID) {
    s32 i;

    for (i = 0; i < 2; i++) {
        gfx->screens.files[i] = GfGfxLoader_GetScrnDataFromOpenNarc(gfx->narc, ov43_0222F0C0[i], 1, &gfx->screens.data[i], heapID);
    }
}

void ov43_0222A9D8(PalPadGfx *gfx) {
    s32 i;

    for (i = 0; i < 2; i++) {
        Heap_Free(gfx->screens.files[i]);
    }
}

void ov43_0222A9F4(PalPadGfx *gfx, Window *window, s32 strno) {
    if (TextPrinterCheckActive(gfx->titlePrinterId) == 1) {
        TextFlags_SetCanTouchSpeedUpPrint(0);
        RemoveTextPrinter(gfx->titlePrinterId);
    }
    FillWindowPixelBuffer(window, 0U);
    ReadMsgDataIntoString(gfx->msgData, strno, gfx->titleBuffer);
    StringExpandPlaceholders(gfx->msgFormat, gfx->titleText, gfx->titleBuffer);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    gfx->titlePrinterId = AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, gfx->titleText, 0U, 4U, 2U, 0xF0200U, NULL);
    ScheduleWindowCopyToVram(window);
}

void ov43_0222AA70(PalPadGfx *gfx) {
    if (TextPrinterCheckActive(gfx->titlePrinterId) == 1) {
        TextFlags_SetCanTouchSpeedUpPrint(0);
        RemoveTextPrinter(gfx->titlePrinterId);
    }
    ClearWindowTilemapAndScheduleTransfer(&gfx->titleWindow);
}
