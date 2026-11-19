#include "global.h"

#include "camera.h"
#include "gf_gfx_planes.h"
#include "overlay_68_move_relearner.h"

void ov68_021E71C4(Camera **camera, enum HeapID heapID);

void ov68_021E7224(MoveRelearnerMonPic *monPic, Pokemon *mon, enum HeapID heapID);

void ov68_021E7288(MoveRelearner *relearner);

void ov68_021E7288(MoveRelearner *relearner) {
    NNS_G3dInit();
    G3X_Init();
    G3X_InitMtxStack();
    reg_G3X_DISP3DCNT &= 0xFFFFCFFD;
    reg_G3X_DISP3DCNT &= 0xCFFB;
    reg_G3X_DISP3DCNT = 8 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    reg_G3X_DISP3DCNT = 0x10 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    reg_G3X_DISP3DCNT &= 0xCFDF;
    G3X_SetFog(0, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(0U, 0, 0x7FFF, 0x3F, 0);
    reg_G3_SWAP_BUFFERS = 2;
    reg_G3_VIEWPORT = 0xBFFF0000;
    MI_CpuFill8(&relearner->monPic, 0, sizeof(MoveRelearnerMonPic));
    ov68_021E71C4(&relearner->monPic.camera, HEAP_ID_66);
    ov68_021E7224(&relearner->monPic, relearner->args->mon, HEAP_ID_66);
    GfGfx_EngineATogglePlanes(1U, 1U);
    reg_G2_BG0CNT = 1 | (reg_G2_BG0CNT & ~3);
}
