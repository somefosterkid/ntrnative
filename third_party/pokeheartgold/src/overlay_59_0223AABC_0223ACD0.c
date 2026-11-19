#include "global.h"
#include "gf_gfx_planes.h"
#include "screen_fade.h"
#include "system.h"
#include "unk_struct_ov59_0223c5b0.h"
#include "overlay_59_juice_feed_app.h"

void ov59_0223ACD0(void);

void ov59_0223ACD0(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    sub_0200FBF4(PM_LCD_TOP, 0U);
    sub_0200FBF4(PM_LCD_BOTTOM, 0U);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
}

void ov59_0223B3DC(void *);

s32 ov59_0223B434(Ov59JuiceFeedApp *param_1);

s32 ov59_0223B590(Ov59JuiceFeedApp *);

s32 ov59_0223B68C(Ov59JuiceFeedApp *);

s32 ov59_0223B740(Ov59JuiceFeedApp *);

s32 ov59_0223B8B0(Ov59JuiceFeedApp *);

void *ov59_0223C298(s32);

void ov59_0223AD20(Ov59JuiceFeedApp *arg0);

void ov59_0223AD20(Ov59JuiceFeedApp *arg0) {
    ov59_0223B434(arg0);
    ov59_0223B590(arg0);
    ov59_0223B68C(arg0);
    ov59_0223B740(arg0);
    ov59_0223B8B0(arg0);
    arg0->monSprite = ov59_0223C298(arg0->heapId);
    Main_SetVBlankIntrCB(ov59_0223B3DC, arg0);
}

s32 ov59_0223B554(Ov59JuiceFeedApp *);

s32 ov59_0223B66C(Ov59JuiceFeedApp *);

s32 ov59_0223B6FC(Ov59JuiceFeedApp *);

s32 ov59_0223B7D8(Ov59JuiceFeedApp *);

s32 ov59_0223B8C8(Ov59JuiceFeedApp *);

s32 ov59_0223C350(UnkStruct_ov59_0223C5B0 *);

void ov59_0223AD58(Ov59JuiceFeedApp *arg0);

void ov59_0223AD58(Ov59JuiceFeedApp *arg0) {
    ov59_0223C350(arg0->monSprite);
    ov59_0223B8C8(arg0);
    ov59_0223B7D8(arg0);
    ov59_0223B6FC(arg0);
    ov59_0223B66C(arg0);
    ov59_0223B554(arg0);
}
