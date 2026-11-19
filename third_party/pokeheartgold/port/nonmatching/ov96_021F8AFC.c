#include "sprite.h"
s32 ov96_021F8A98(s32 *, s32, s32, s32);
Sprite *ov96_021F8ACC(s32 *, s32, s32);

void ov96_021F8AFC(s32 arg0, u8 arg1, void *arg2, s32 arg3) {
    s32 buf[14];
    NNSG2dImageProxy *img;
    NNSG2dImagePaletteProxy *pal;
    enum NNS_G2D_VRAM_TYPE vram;
    Sprite *sprite;
    u32 loc;
    u32 off;

    ov96_021F8A98(buf, arg0, arg0 + 0x2D8, 1);
    *(Sprite **)arg2 = ov96_021F8ACC(buf, 0x11, 4);
    *(Sprite **)((u8 *)arg2 + 4) = ov96_021F8ACC(buf, 5, 0);
    *(Sprite **)((u8 *)arg2 + 8) = ov96_021F8ACC(buf, 0x12, 1);
    sprite = *(Sprite **)((u8 *)arg2 + 0xC) = ov96_021F8ACC(buf, 0, 2);
    vram = Sprite_GetVramType(sprite);
    img = Sprite_GetImageProxy(sprite);
    pal = Sprite_GetPaletteProxy(sprite);
    loc = NNS_G2dGetImageLocation(img, vram);
    off = arg1 << 9;
    NNS_G2dInitImageProxy(img);
    NNS_G2dSetImageLocation(img, vram, off + loc);
    loc = NNS_G2dGetImagePaletteLocation(pal, vram);
    off = (arg1 + 6) << 5;
    NNS_G2dInitImagePaletteProxy(pal);
    NNS_G2dSetImagePaletteLocation(pal, vram, off + loc);
    Sprite_SetImageProxy(sprite, img);
    Sprite_SetPaletteProxy(sprite, pal);
    Sprite_SetPalOffset(sprite, arg1);
}
