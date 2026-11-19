#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "overlay_102_app_021eb1e0.h"

void ov102_021EB29C(Ov102App_021EB1E0 *param_1);

void ov102_021EB1E0(void *arg0);

void ov102_021EB1E0(void *ptr) {
    ov102_021EB29C(ptr);
    DestroyMsgData(*(MsgData **)((u8 *)ptr + 0x14));
    Heap_Free(ptr);
}

BgConfig *ov102_021EA268(void *);

s32 ov102_021EB2EC(void *, s32, s32);

s32 ov102_021EB46C(void *, BgConfig *);

s32 ov102_021EB4D0(void *);

s32 ov102_021EB570(void *, s8, s8);

void ov102_021EB1F8(void *arg0, NARC *arg1);

void ov102_021EB1F8(void *arg0, NARC *narc) {
    BgConfig *bgConfig;

    bgConfig = ov102_021EA268(*(void **)arg0);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 4, bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_35);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_35);
    ((Ov102App_021EB1E0 *)arg0)->unk_54 = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 0xE, 0, arg0 + 0x5C, HEAP_ID_35);
    ((Ov102App_021EB1E0 *)arg0)->unk_58 = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 0xF, 0, arg0 + 0x60, HEAP_ID_35);
    ov102_021EB46C(arg0, bgConfig);
    ov102_021EB4D0(arg0);
    ov102_021EB2EC(arg0, 1, 0);
    ov102_021EB570(arg0, 0, 1);
    ov102_021EB570(arg0, 1, 0);
    BgCommitTilemapBufferToVram(bgConfig, 3U);
}
