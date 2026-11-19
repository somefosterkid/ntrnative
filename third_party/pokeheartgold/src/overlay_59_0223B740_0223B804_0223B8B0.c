#include "global.h"
#include "gf_gfx_planes.h"
#include "overlay_59_juice_feed_app.h"
struct Ov59JuiceFeedApp;

s32 ov59_0223B804(struct Ov59JuiceFeedApp *arg0);

s32 ov59_0223B8E4(Ov59JuiceFeedApp *);

void ov59_0223B8B0(Ov59JuiceFeedApp *arg0);

void ov59_0223B8B0(Ov59JuiceFeedApp *arg0) {
    ov59_0223B804(arg0);
    ov59_0223B8E4(arg0);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
}

s32 ov59_0223B878(struct Ov59JuiceFeedApp *);

s32 ov59_0223BA44(Ov59JuiceFeedApp *);

void ov59_0223B8C8(Ov59JuiceFeedApp *arg0);

void ov59_0223B8C8(Ov59JuiceFeedApp *arg0) {
    GfGfx_EngineATogglePlanes(0x10U, 0U);
    ov59_0223BA44(arg0);
    ov59_0223B878(arg0);
}
