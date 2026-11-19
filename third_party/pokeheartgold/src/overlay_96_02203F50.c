#include "global.h"

#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "bg_window.h"
#include "font.h"

typedef struct UnkStruct_ov96_02203FBC {
    enum HeapID unk_0;
    u8 unk_4[0x4];
    BgConfig *unk_8;
    u8 unk_C;
} UnkStruct_ov96_02203FBC;

s32 *ov96_021EB5EC(u8 *param_1, s32 param_2, u32 param_3);

void ov96_02203F50(void *param_1, void *param_2);

extern WindowTemplate ov96_0221CA28;
void ov96_02203FBC(UnkStruct_ov96_02203FBC *arg0);

void ov96_02203F50(void *param_1, void *param_2) {
    s32 *piVar1;
    int new_var;
    NNSG2dImageProxy *imageProxy;
    NNSG2dImagePaletteProxy *pImg;
    u32 uVar2;
    SpriteResource *charResObj;
    u32 uVar3;
    SpriteResource *plttResObj;
    void *iVar4;
    unsigned int new_var2;
    piVar1 = ov96_021EB5EC(param_1, 0x65, 0);
    charResObj = (SpriteResource *)(*piVar1);
    piVar1 = ov96_021EB5EC(param_1, 0x65, 1);
    plttResObj = (SpriteResource *)(*piVar1);
    imageProxy = SpriteTransfer_GetCharProxy(charResObj);
    pImg = SpriteTransfer_GetPaletteProxy(plttResObj, imageProxy);
    uVar3 = 0;
    do {
        iVar4 = param_2 + (uVar3 * 4);
        uVar2 = NNS_G2dGetImageLocation(imageProxy, NNS_G2D_VRAM_TYPE_2DSUB);
        new_var2 = (uVar3 * 0xd00) + 0x300;
        *((u32 *)(iVar4 + 0x3c)) = new_var2 + uVar2;
        uVar2 = NNS_G2dGetImagePaletteLocation(pImg, NNS_G2D_VRAM_TYPE_2DSUB);
        new_var = (uVar3 * 0x20) + 0x20;
        *((u32 *)(iVar4 + 0x48)) = new_var + uVar2;
        uVar3 = (uVar3 + 1) & 0xff;
    } while (uVar3 < 3);
    return;
}

void ov96_02203FBC(UnkStruct_ov96_02203FBC *arg0) {
    AddWindow(arg0->unk_8, (void *)&arg0->unk_C, &ov96_0221CA28);
    BG_FillCharDataRange(arg0->unk_8, GF_BG_LYR_SUB_2_F, 0U, 1U, 0U);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, arg0->unk_0);
    FillWindowPixelBuffer((void *)&arg0->unk_C, 0U);
}
