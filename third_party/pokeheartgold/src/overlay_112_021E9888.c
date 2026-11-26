#include "global.h"

#include "error_handling.h"
#include "touchscreen.h"
#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"

#include "overlay_112_hitbox.h"
#include "overlay_112_pokewalker_app.h"

// The asm table has 5 entries (the screens 0..4) and ov112_021FF528 follows it directly; the callers pass 5 too (the assert allows it), so
// on the DS entry 5 is the first word of ov112_021FF528 = ov112_021F3664, a hitbox list with only the back button. The PC table spells that
// 6th entry out (UBFIX note: the DS reads it past the end of the 5-entry table, no behaviour change).
extern TouchscreenHitbox *ov112_021FF514[6];

#if defined(SDK_PORT) || defined(NONMATCHING)
static TouchscreenHitbox ov112_021F3664[2] = {
    { .rect = { .top = 0xA0, .bottom = 0xBF, .left = 0xC0, .right = 0xFF } },
    { .rect = { .top = 0xFF, .bottom = 0, .left = 0, .right = 0 } },
};

static TouchscreenHitbox ov112_021F36B8[9] = {
    { .rect = { .top = 0, .bottom = 0x1F, .left = 0xB, .right = 0xEF } },
    { .rect = { .top = 0x20, .bottom = 0x3F, .left = 0xB, .right = 0xEF } },
    { .rect = { .top = 0x40, .bottom = 0x5F, .left = 0xB, .right = 0xEF } },
    { .rect = { .top = 0x60, .bottom = 0x7F, .left = 0xB, .right = 0xEF } },
    { .rect = { .top = 0x80, .bottom = 0x9F, .left = 0xB, .right = 0xEF } },
    { .rect = { .top = 0xA0, .bottom = 0xBF, .left = 0xC0, .right = 0xFF } },
    { .rect = { .top = 0xA2, .bottom = 0xBF, .left = 0xC, .right = 0x23 } },
    { .rect = { .top = 0xA2, .bottom = 0xBF, .left = 0x24, .right = 0x41 } },
    { .rect = { .top = 0xFF, .bottom = 0, .left = 0, .right = 0 } },
};

static TouchscreenHitbox ov112_021F36A4[5] = {
    { .rect = { .top = 0xB, .bottom = 0x34, .left = 0xB, .right = 0xEF } },
    { .rect = { .top = 0x35, .bottom = 0x5B, .left = 0xB, .right = 0xEF } },
    { .rect = { .top = 0x5C, .bottom = 0x84, .left = 0xB, .right = 0xEF } },
    { .rect = { .top = 0xA0, .bottom = 0xBF, .left = 0xC0, .right = 0xFF } },
    { .rect = { .top = 0xFF, .bottom = 0, .left = 0, .right = 0 } },
};

static TouchscreenHitbox ov112_021F3694[4] = {
    { .rect = { .top = 0x25, .bottom = 0x4C, .left = 0xD, .right = 0xEF } },
    { .rect = { .top = 0x4D, .bottom = 0x73, .left = 0xD, .right = 0xEF } },
    { .rect = { .top = 0xA0, .bottom = 0xBF, .left = 0xC0, .right = 0xFF } },
    { .rect = { .top = 0xFF, .bottom = 0, .left = 0, .right = 0 } },
};

static TouchscreenHitbox ov112_021F3678[3] = {
    { .rect = { .top = 0x3C, .bottom = 0x64, .left = 0xC, .right = 0xEF } },
    { .rect = { .top = 0xA0, .bottom = 0xBF, .left = 0xC0, .right = 0xFF } },
    { .rect = { .top = 0xFF, .bottom = 0, .left = 0, .right = 0 } },
};

static TouchscreenHitbox ov112_021F366C[3] = {
    { .rect = { .top = 0x25, .bottom = 0x4C, .left = 0xD, .right = 0xEF } },
    { .rect = { .top = 0x4D, .bottom = 0x73, .left = 0xD, .right = 0xEF } },
    { .rect = { .top = 0xFF, .bottom = 0, .left = 0, .right = 0 } },
};

TouchscreenHitbox *ov112_021FF514[6] = {
    ov112_021F366C, ov112_021F3678, ov112_021F3694, ov112_021F36A4, ov112_021F36B8,
    ov112_021F3664,
};
#endif

int ov112_021E9888(s32 param_1) {
    if ((5 + 1) <= param_1) {
        GF_AssertFail();
    }
    return TouchscreenHitbox_FindRectAtTouchNew(ov112_021FF514[param_1]);
}

void ov112_021E98A4(Ov112PokewalkerApp *arg0) {
    NARC *narc;

    narc = NARC_New(NARC_a_2_4_7, HEAP_ID_154);
    ToggleBgLayer(5U, 0U);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x12, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, HEAP_ID_154);
    ToggleBgLayer(5U, 1U);
    NARC_Delete(narc);
}

void ov112_021E98E8(Ov112PokewalkerApp *param_1, u16 param_2) {
    ToggleBgLayer(5, param_2);
    return;
}
