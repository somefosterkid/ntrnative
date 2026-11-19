#include "global.h"
struct TradeTubePhase;

#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "overlay_71_trade_tube_phase.h"

void *ov71_0224744C(s32, s32, s32, s32);

void *ov71_02247610(void *, s32, s32, s32, s32, s32, s32, s32);

s32 ov71_02247730(void *, s32);

s32 ov71_0224780C(void *, s32 *);

extern u16 ov71_0224BD0C;

extern s32 ov71_0224C0C4;

extern s32 ov71_0224C0C8;

void ov71_022488E4(struct TradeTubePhase *arg0);

void ov71_022488E4(TradeTubePhase *arg0) {
    NNS_G3dInit();
    G3X_InitMtxStack();
    *((u16 *)REG_DISP3DCNT_ADDR) &= 0xFFFFCFFD;
    *((u16 *)REG_DISP3DCNT_ADDR) = 0x10 | ((*((u16 *)REG_DISP3DCNT_ADDR)) & 0xFFFFCFFF);
    *((u16 *)REG_DISP3DCNT_ADDR) &= 0xCFFB;
    *((u16 *)REG_DISP3DCNT_ADDR) = 8 | ((*((u16 *)REG_DISP3DCNT_ADDR)) & 0xFFFFCFFF);
    *((u16 *)REG_DISP3DCNT_ADDR) = 0x20 | ((*((u16 *)REG_DISP3DCNT_ADDR)) & 0xFFFFCFFF);
    G3X_SetEdgeColorTable(&ov71_0224BD0C);
    G3X_SetClearColor(0x43FFU, 0, 0x7FFF, 0x3F, 0);
    reg_G3_SWAP_BUFFERS = 2;
    reg_G3_VIEWPORT = 0xBFFF0000;
    GF_3DVramMan_InitFrameTexVramManager(1U, 1);
    GF_3DVramMan_InitFramePlttVramManager(0x4000U, 1);
    GfGfx_EngineATogglePlanes(1U, 1U);
    *((u16 *)REG_BG0CNT_ADDR) = (*((u16 *)REG_BG0CNT_ADDR)) & (~3);
    *((void **)&arg0->scene) = ov71_0224744C(1, 0, 0, 0);
    *((void **)&arg0->model) = ov71_02247610(*((void **)&arg0->scene), 0, 0x59, 0x1B, 0, 0xFFFC8800, 0, 0);
    *((u16 *)(((u8 *)(&ov71_0224C0C4)) + 4)) = 0xEBE0;
    *((s16 *)(((u8 *)(&ov71_0224C0C4)) + 6)) = 0;
    *((s16 *)(((u8 *)(&ov71_0224C0C4)) + 8)) = 0;
    ov71_0224780C(*((void **)&arg0->scene), &ov71_0224C0C8);
    ov71_02247730(*((void **)&arg0->model), 0x1C00);
}
