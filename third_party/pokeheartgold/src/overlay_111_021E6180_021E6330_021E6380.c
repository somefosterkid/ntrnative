#include "global.h"

#include "error_handling.h"
#include "sprite_system.h"

extern u32 ov111_021E6C00;

void ov111_021E6380(SpriteSystem *param_1, SpriteManager *param_2, s16 param_3, u32 param_4, u8 param_5);

void ov111_021E6380(SpriteSystem *param_1, SpriteManager *param_2, s16 param_3, u32 param_4, u8 param_5) {
    ManagedSpriteTemplate MStack_4c;

    MStack_4c = *(ManagedSpriteTemplate *)&ov111_021E6C00;
    if (param_5 >= 2) {
        GF_AssertFail();
    }
    MStack_4c.resIdList[0] = param_5 + 1;
    MStack_4c.x = param_3;
    MStack_4c.y = (s16)param_4;
    SpriteSystem_NewSpriteWithYOffset(param_1, param_2, &MStack_4c, FX32_CONST(524));
    return;
}
