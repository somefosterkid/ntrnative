#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "overlay_27_ov27task.h"

s32 ov27_0225A4D0(Ov27Task *arg0);

s32 ov27_0225A9C0(Ov27Task *, s32);

void ov27_0225A690(Ov27Task *arg0, s32 arg1);

void ov27_0225A690(Ov27Task *arg0, s32 arg1) {
    if (arg1 != 0) {
        arg0->unk_51C_0 = 1;
        ov27_0225A4D0(arg0);
        ov27_0225A9C0(arg0, 1);
        Sprite_SetDrawFlag(arg0->unk_390[12], 1);
        return;
    }
    arg0->unk_51C_0 = 0;
    FillWindowPixelBuffer(&arg0->unk_3D0, 0U);
    CopyWindowToVram(&arg0->unk_3D0);
    Sprite_SetDrawFlag(arg0->unk_390[12], 0);
    Sprite_SetDrawFlag(arg0->unk_390[11], 0);
    Sprite_SetDrawFlag(arg0->unk_390[13], 0);
    ov27_0225A9C0(arg0, 0);
}
