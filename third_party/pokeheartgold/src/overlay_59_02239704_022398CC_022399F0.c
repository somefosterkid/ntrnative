#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "gf_gfx_planes.h"

void ov59_02239938(Ov59ApricornApp *);

void ov59_02239A24(Ov59ApricornApp *);

void ov59_022399F0(Ov59ApricornApp *arg0);

void ov59_022399F0(Ov59ApricornApp *arg0) {
    ov59_02239938(arg0);
    ov59_02239A24(arg0);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
}

s32 ov59_022399B4(Ov59ApricornApp *);

s32 ov59_02239C70(Ov59ApricornApp *);

void ov59_02239A08(Ov59ApricornApp *arg0);

void ov59_02239A08(Ov59ApricornApp *arg0) {
    GfGfx_EngineATogglePlanes(0x10U, 0U);
    ov59_02239C70(arg0);
    ov59_022399B4(arg0);
}
