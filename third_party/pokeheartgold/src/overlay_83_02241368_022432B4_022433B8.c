#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"

s32 ov83_022448E4(u8 *, u8 *);

s32 ov83_022449D4(u8 *, u8 *);

s32 ov83_02244BEC(u8 *);

s32 ov83_022453DC(u8 *, Window *);

s32 ov83_02245584(u8 *, Window *);

void ov83_022433B8(u8 *arg0);

void ov83_022433B8(u8 *arg0) {
    Window *window;

    window = (struct Window *)(arg0 + 0x50);
    FillWindowPixelBuffer(window, 0U);
    ov83_02245584(arg0, window);
    ov83_022453DC(arg0, window);
    ov83_022448E4(arg0, arg0 + 0x80);
    ov83_022449D4(arg0, arg0 + 0x70);
    ov83_02244BEC(arg0);
    GfGfx_BothDispOn();
}
