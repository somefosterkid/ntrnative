#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

#include "camera.h"

s32 ov96_021F6398(UnkStruct_ov96_021F5018 *);

void ov96_021F6138(UnkStruct_ov96_021F5018 *arg0);

void ov96_021F6138(UnkStruct_ov96_021F5018 *arg0) {
    NNS_G3dInit();
    G3X_InitMtxStack();
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    *(u16 *)((void *)REG_BG0CNT_ADDR) = (u16)((*(u16 *)((void *)REG_BG0CNT_ADDR) & ~3) | 2);
    *(u16 *)((void *)REG_DISP3DCNT_ADDR) = (u16)(*(u16 *)((void *)REG_DISP3DCNT_ADDR) & 0xFFFFCFFD);
    *(u16 *)((void *)REG_DISP3DCNT_ADDR) = (u16)(*(u16 *)((void *)REG_DISP3DCNT_ADDR) & 0xCFEF);
    *(u16 *)((void *)REG_DISP3DCNT_ADDR) = (u16)(0xCFFB & *(u16 *)((void *)REG_DISP3DCNT_ADDR));
    *(u16 *)((void *)REG_DISP3DCNT_ADDR) = (u16)(0xCFF7 & *(u16 *)((void *)REG_DISP3DCNT_ADDR));
    *(u16 *)((void *)REG_DISP3DCNT_ADDR) = (u16)(*(u16 *)((void *)REG_DISP3DCNT_ADDR) & 0xCFDF);
    G3X_SetClearColor(0U, 0, (s32)(0xFFFFCFFDU >> 0x11), 0x3F, 0);
    G3_ViewPort(0, 0, 255, 191);
    arg0->unk_190 = Camera_New(arg0->heapID);
    ov96_021F6398(arg0);
}
