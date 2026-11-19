#include "global.h"

#include "bg_window.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "pm_string.h"
#include "text.h"
#include "unk_02013534.h"
#include "overlay_96_app_021f9134.h"

s32 ov96_021EB5E8(void *);

s32 sub_02013880(TextOBJ *, s32);

void ov96_021F9134(void *arg0, Window *arg1, String *arg2, u32 arg3, s32 arg4, s32 arg5, s32 arg6, void *arg7);

void ov96_021F9134(void *arg0, Window *arg1, String *arg2, u32 arg3, s32 arg4, s32 arg5, s32 arg6, void *arg7) {
    u32 sp28[12];
    UnkStruct_02021AC8 sp1C;
    TextOBJ *temp;
    u8 *var_r6;

    var_r6 = arg7;
    FillWindowPixelBufferText_AssumeTileSize32(arg1, 0U);
    AddTextPrinterParameterizedWithColorAndSpacing(arg1, 0, arg2, arg3, 0U, 0xFFU, 0x10200U, 0U, 0U, NULL);
    sub_02021AC8(((Ov96App_021F9134 *)arg1)->unk_14, 1, NNS_G2D_VRAM_TYPE_2DSUB, &sp1C);
    sp28[0] = *(u32 *)((u8 *)arg0 + 8);
    sp28[1] = (u32)arg1;
    sp28[2] = (u32)ov96_021EB5E8(*(void **)((u8 *)arg0 + 4));
    sp28[3] = (u32)ObjPlttTransfer_GetPlttProxyByID(0x68);
    sp28[4] = arg6;
    sp28[5] = sp1C.offset;
    sp28[6] = arg4;
    sp28[7] = arg5;
    sp28[8] = 0;
    sp28[9] = 0;
    sp28[10] = 2;
    sp28[11] = *(u32 *)arg0;
    temp = sub_020135D8((TextOBJTemplate *)sp28);
    sub_02013880(temp, 4);
    *(TextOBJ **)var_r6 = temp;
    *(UnkStruct_02021AC8 *)(var_r6 + 4) = sp1C;
}
