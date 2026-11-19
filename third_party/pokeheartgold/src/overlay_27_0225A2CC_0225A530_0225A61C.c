#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "text.h"
#include "overlay_27_ov27task.h"

extern const u8 ov27_0225D108[];

void ov27_0225A61C(Ov27Task *arg0, s32 arg1);

void ov27_0225A61C(Ov27Task *arg0, s32 arg1) {
    FillWindowPixelBuffer(&arg0->unk_3D0, 0U);
    AddTextPrinterParameterizedWithColor(&arg0->unk_3D0, FONT_UNK_4, arg0->unk_4B0[ov27_0225D108[arg1]], 0U, 0U, 0U, 0xF0100U, NULL);
    Sprite_SetDrawFlag(arg0->unk_390[12], 1);
}
