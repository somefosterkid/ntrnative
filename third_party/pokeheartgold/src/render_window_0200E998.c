#include "global.h"

#include "bg_window.h"
#include "render_text.h"
#include "constants/heap.h"

s32 sub_0200E948(Window *, u32, u32);

void DrawFrameAndWindow2(Window *window, s32 dont_copy_to_vram, u16 baseTile, u8 palette_num);

void ClearFrameAndWindow2(Window *window, u8 skipTransfer);
void sub_0200EA24(void *srcPixels, u16 srcX, u16 srcY, u16 srcWidth, u16 srcHeight, void *destPixels, u16 destWidth, u16 destHeight, u16 destX, u16 destY, u16 blitWidth, u16 blitHeight);

void DrawFrameAndWindow2(Window *window, s32 dont_copy_to_vram, u16 baseTile, u8 palette_num) {
    sub_0200E948(window, baseTile, palette_num);
    if (dont_copy_to_vram == 0) {
        CopyWindowToVram(window);
    }
    TextPrinter_SetDownArrowBaseTile((s32)baseTile);
}

void ClearFrameAndWindow2(Window *window, u8 skipTransfer) {
    FillBgTilemapRect(window->bgConfig,
        GetWindowBgId(window),
        0,
        GetWindowX(window) - 2,
        GetWindowY(window) - 1,
        GetWindowWidth(window) + 5,
        GetWindowHeight(window) + 2,
        0);

    if (skipTransfer == 0) {
        ClearWindowTilemapAndCopyToVram(window);
    }
}

void sub_0200EA24(void *srcPixels, u16 srcX, u16 srcY, u16 srcWidth, u16 srcHeight, void *destPixels, u16 destWidth, u16 destHeight, u16 destX, u16 destY, u16 blitWidth, u16 blitHeight) {
    Bitmap src, dest;

    src.pixels = (u8 *)srcPixels;
    src.width = srcWidth;
    src.height = srcHeight;

    dest.pixels = (u8 *)destPixels;
    dest.width = destWidth;
    dest.height = destHeight;

    BlitBitmapRect4Bit(&src, &dest, srcX, srcY, destX, destY, blitWidth, blitHeight, 0);
}
