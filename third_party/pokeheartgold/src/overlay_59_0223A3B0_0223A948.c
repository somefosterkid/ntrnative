#include "global.h"

#include "overlay_manager.h"
#include "overlay_59_juice_feed_app.h"
#include "sprite_system.h"
struct UnkStruct_ov59_0223C5B0;

s32 ov59_0223AB3C(Ov59JuiceFeedApp *arg0);

s32 ov59_0223AB90(Ov59JuiceFeedApp *arg0);

void ov59_0223C304(struct UnkStruct_ov59_0223C5B0 *arg0);

s32 ov59_0223ABE4(Ov59JuiceFeedApp *arg0);

s32 ov59_0223ABEC(Ov59JuiceFeedApp *arg0);

u32 ov59_0223A948(OverlayManager *param_1, s32 *param_2);

u32 ov59_0223A948(OverlayManager *param_1, s32 *param_2) {
    Ov59JuiceFeedApp *arg0;
    s32 sVar1;

    arg0 = OverlayManager_GetData(param_1);
    switch (*param_2) {
    case 0:
        sVar1 = ov59_0223AB3C(arg0);
        *param_2 = sVar1;
        break;
    case 1:
        sVar1 = ov59_0223AB90(arg0);
        *param_2 = sVar1;
        break;
    case 2:
        sVar1 = ov59_0223ABE4(arg0);
        *param_2 = sVar1;
        break;
    case 4:
        sVar1 = ov59_0223ABEC(arg0);
        *param_2 = sVar1;
        break;
    case 5:
        sVar1 = ov59_0223AC48(arg0);
        *param_2 = sVar1;
        break;
    case 8:
        return 1;
    }
    SpriteSystem_DrawSprites(arg0->spriteManager);
    ov59_0223C304(arg0->monSprite);
    return 0;
}
