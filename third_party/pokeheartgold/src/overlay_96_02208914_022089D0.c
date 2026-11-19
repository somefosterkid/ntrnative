#include "global.h"

#include "overlay_96_ov96hurdle_hud.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"

SpriteResource **ov96_021EB5EC(void *, SpriteResource *, s32);

extern void *ov96_0221CBCC;

void ov96_022089D0(void *arg0, Ov96HurdleHud *hud);

void ov96_022089D0(void *arg0, Ov96HurdleHud *hud) {
    Ov96HurdleHud *sp0;
    SpriteResource *spC[2];
    NNSG2dImagePaletteProxy *sp8;
    NNSG2dImagePaletteProxy *temp_r0_2;
    SpriteResource *var_r5;
    SpriteResource *var_r6;
    void *var_r7;
    u8 var_r4;

    var_r7 = arg0;
    sp0 = hud;
    spC[0] = *(SpriteResource **)((u8 *)&ov96_0221CBCC + 0x18);
    spC[1] = *(SpriteResource **)((u8 *)&ov96_0221CBCC + 0x1c);
    var_r4 = 0;
    do {
        var_r5 = spC[var_r4];
        var_r6 = *ov96_021EB5EC(var_r7, var_r5, 0);
        var_r5 = *ov96_021EB5EC(var_r7, var_r5, 1);
        var_r6 = (SpriteResource *)SpriteTransfer_GetCharProxy(var_r6);
        temp_r0_2 = SpriteTransfer_GetPaletteProxy(var_r5, (NNSG2dImageProxy *)var_r6);
        sp8 = temp_r0_2;
        sp0->charLoc[var_r4] = NNS_G2dGetImageLocation((NNSG2dImageProxy *)var_r6, NNS_G2D_VRAM_TYPE_2DSUB);
        sp0->palLoc[var_r4] = NNS_G2dGetImagePaletteLocation(sp8, NNS_G2D_VRAM_TYPE_2DSUB);
        var_r4 += 1;
    } while ((u32)var_r4 < 2U);
}
