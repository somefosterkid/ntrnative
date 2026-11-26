#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "pm_string.h"
#include "overlay_90_app_0225a2b0.h"
#include "overlay_90_app_0225a2b0_2.h"
#include "player_data.h"

s32 ov90_0225927C(void *, s32);

s32 ov90_0225928C(void *, String *, s32);

void ov90_0225A2B0(Window *arg0, BgConfig **arg1, void *arg2, s32 arg3, NARC *arg4, enum HeapID arg5);

void ov90_0225A2B0(Window *window, BgConfig **arg1, void *arg2, s32 arg3, NARC *narc, enum HeapID heapID) {
    AddWindowParameterized(*arg1, window, 1U, 6U, 0xBU, 0x14U, 0xAU, 8U, 0x201U);
    FillWindowPixelBuffer(window, 0U);
    ((Ov90App_0225A2B0 *)window)->unk_14 = String_New(0x80U, heapID);
    ov90_0225927C(arg2, arg3);
    ov90_0225928C(arg2, ((Ov90App_0225A2B0 *)window)->unk_14, 0x10);
    ((Ov90App_0225A2B0 *)window)->unk_10 = arg3;
    GfGfx_EngineATogglePlanes(1U, 0U);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 7, *arg1, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0xA, *arg1, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 0, heapID);
}

void ov90_0225A340(Window *arg0);

void ov90_0225A340(Window *window) {
    RemoveWindow(window);
    String_Delete(((Ov90App_0225A2B0_2 *)window)->unk_14);
}
