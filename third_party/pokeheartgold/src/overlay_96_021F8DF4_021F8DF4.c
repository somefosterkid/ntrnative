#include "global.h"

#include "bg_window.h"
#include "obj_char_transfer.h"
#include "sprite_transfer.h"
#include "text.h"
#include "unk_02013534.h"
#include "overlay_96_app_021f8df4.h"

typedef struct UnkStruct_ov96_021F8DF4 {
    s32 unk_0;
    u8 unk_4[0xC];
    s32 unk_10;
    u8 unk_14[0x168];
    s32 unk_17C;
    u8 unk_180[0x144];
    SpriteResource *unk_2C4;
} UnkStruct_ov96_021F8DF4;

void ov96_021F8DF4(UnkStruct_ov96_021F8DF4 *arg0, Window *arg1, String *arg2, u32 arg3, s32 arg4, s32 arg5, s32 arg6, void *arg7);

void ov96_021F8DF4(UnkStruct_ov96_021F8DF4 *arg0, Window *arg1, String *arg2, u32 arg3, s32 arg4, s32 arg5, s32 arg6, void *arg7) {
    u32 sp28[12];
    UnkStruct_02021AC8 sp1C;
    u32 sp18;
    TextOBJ *temp_r0;
    u8 *out = arg7;

    sp18 = arg3;
    FillWindowPixelBufferText_AssumeTileSize32(arg1, 0U);
    AddTextPrinterParameterizedWithColorAndSpacing(arg1, 0, arg2, sp18, 0U, 0xFFU, 0x10200U, 0U, 0U, NULL);
    sub_02021AC8(((Ov96App_021F8DF4 *)arg1)->unk_14, 1, NNS_G2D_VRAM_TYPE_2DMAIN, &sp1C);
    sp28[0] = arg0->unk_10;
    sp28[1] = (u32)arg1;
    sp28[2] = arg0->unk_17C;
    sp28[3] = (u32)SpriteTransfer_GetPaletteProxy(arg0->unk_2C4, NULL);
    sp28[4] = arg6;
    sp28[5] = sp1C.offset;
    sp28[6] = arg4;
    sp28[7] = arg5;
    sp28[8] = 0;
    sp28[9] = 3;
    sp28[10] = 1;
    sp28[11] = arg0->unk_0;
    temp_r0 = sub_020135D8((TextOBJTemplate *)sp28);
    TextOBJ_SetPaletteNum(temp_r0, 0);
    *(TextOBJ **)out = temp_r0;
    *(UnkStruct_02021AC8 *)(out + 4) = sp1C;
}
