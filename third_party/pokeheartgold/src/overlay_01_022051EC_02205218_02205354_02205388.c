#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "overlay_01_022051EC.h"

BOOL ov01_02205388(Field3DModelAnimation *anims, u32 count) {
    BOOL BVar1;
    u32 uVar2;
    u32 uVar3;

    uVar3 = 0;
    uVar2 = 0;
    while (uVar2 < count) {
        BVar1 = Field3dModelAnimation_FrameAdvanceAndCheck(&anims[uVar2], FX32_ONE);
        if (BVar1 != 0) {
            uVar3 = uVar3 + 1 & 0xff;
        }
        uVar2 = uVar2 + 1 & 0xff;
    }
    if (uVar3 == count) {
        return 1;
    }
    return 0;
}

void ov01_022053C4(Field3DModelAnimation *anims, u32 count, fx32 frame) {
    u32 uVar1;

    uVar1 = 0;
    while (uVar1 < count) {
        Field3dModelAnimation_FrameSet(&anims[uVar1], frame);
        uVar1 = uVar1 + 1 & 0xff;
    }
    return;
}
