#include "global.h"

#include "overlay_112_ir_state.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_manager.h"
#include "render_text.h"
#include "system.h"
#include "unk_020210A0.h"
#include "vram_transfer_manager.h"
#include "overlay_112_pokewalker_app.h"
struct Ov112PokewalkerApp;

s32 ov112_021E59A8(void);


s32 ov112_021E76A8(void);

void ov112_021E7768(Ov112PokewalkerApp *arg0);
s32 ov112_021E795C(Ov112PokewalkerApp *);
s32 ov112_021E76CC(OverlayManager *arg0);
void ov112_021E79A8(void *);
s32 ov112_021E79D0(void);
s32 ov112_021E79F0(BgConfig *);
s32 ov112_021E7B18(void *);
s32 ov112_021E7C18(void *);
s32 ov112_021E9A78(void *, u8);
s32 ov112_021E9C98(Ov112PokewalkerApp *);
s32 ov112_021EA1AC(Ov112PokewalkerApp *);
s32 ov112_021EA4F4(Ov112PokewalkerApp *);
s32 ov112_021E7AC8(BgConfig *);
s32 ov112_021E7C6C(struct Ov112PokewalkerApp *);
s32 ov112_021E9C10(void *, s32);
s32 ov112_021E9E30(Ov112PokewalkerApp *);
s32 ov112_021EA1F8(Ov112PokewalkerApp *arg0);
void ov112_021E77E4(Ov112PokewalkerApp *arg0);

s32 ov112_021E76A8(void) {
    s32 v0;
    s32 v1;

    v0 = ov112_021FFAA4.unk_14;
    if (ov112_021E59A8() != 0) {
        v1 = 0;
        ov112_021FFAA4.unk_14 = v1;
    } else {
        v1 = 1;
        ov112_021FFAA4.unk_14 = v1;
    }
    return v0;
}

s32 ov112_021E76CC(OverlayManager *man) {
    Ov112PokewalkerApp *v0;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT = (s32)(reg_GX_DISPCNT & 0xFFFFE0FF);
    reg_GXS_DB_DISPCNT = (s32)(0xFFFFE0FF & reg_GXS_DB_DISPCNT);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    sub_0201A728(0x10);
    Heap_Create(HEAP_ID_3, HEAP_ID_154, 0x70000U);
    v0 = OverlayManager_CreateAndGetData(man, sizeof(Ov112PokewalkerApp), HEAP_ID_154);
    memset(v0, 0, sizeof(Ov112PokewalkerApp));
    v0->saveData = *(SaveData **)((u8 *)(OverlayManager_GetArgs(man)) + 8);
    ov112_021E795C(v0);
    SetKeyRepeatTimers(3, 8);
    EnableTouchPad();
    InitializeTouchPad(4);
    ov112_021E7768(v0);
    TextFlags_SetCanABSpeedUpPrint(1);
    return 1;
}

void ov112_021E7768(Ov112PokewalkerApp *arg) {
    arg->bgConfig = BgConfig_Alloc(HEAP_ID_154);
    FontID_Alloc(FONT_UNK_4, HEAP_ID_154);
    ov112_021E7C18(arg);
    ov112_021E79D0();
    ov112_021E79F0(arg->bgConfig);
    ov112_021E7B18(arg);
    ov112_021E9A78(arg, 0);
    ov112_021E9A78(arg, 1);
    ov112_021EA1AC(arg);
    ov112_021EA4F4(arg);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    ov112_021E9C98(arg);
    Main_SetVBlankIntrCB(ov112_021E79A8, arg);
    reg_GX_POWCNT &= 0xFFFF7FFF;
    ToggleBgLayer(4U, 1U);
}

void ov112_021E77E4(Ov112PokewalkerApp *arg0) {
    ov112_021EA1F8(arg0);
    ov112_021E9C10(arg0, 1);
    ov112_021E9C10(arg0, 0);
    ov112_021E7AC8(arg0->bgConfig);
    DisableTouchPad();
    GF_DestroyVramTransferManager();
    ov112_021E9E30(arg0);
    ov112_021E7C6C(arg0);
    FontID_Release(FONT_UNK_4);
    Main_SetVBlankIntrCB(NULL, NULL);
    arg0->spriteManager = NULL;
}
