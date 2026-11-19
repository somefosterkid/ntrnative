#include "global.h"
struct TradeSendPhase;

#include "gf_3d_vramman.h"
#include "pokepic.h"
#include "overlay_71_trade_send_phase.h"

void ov71_02247ED0(struct TradeSendPhase *arg0);

void ov71_02247ED0(TradeSendPhase *arg0) {
    u32 v0;
    u32 v1;

    NNS_G3dInit();
    G3X_InitMtxStack();
    reg_G3X_DISP3DCNT &= 0xFFFFCFFD;
    reg_G3X_DISP3DCNT = 0x10 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    reg_G3X_DISP3DCNT &= 0xCFFB;
    reg_G3X_DISP3DCNT = 8 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    G3X_SetClearColor(0U, 0, (s32)(0xFFFFCFFDU >> 0x11), 0x3F, 0);
    reg_G3_SWAP_BUFFERS = 2;
    reg_G3_VIEWPORT = 0xBFFF0000;
    GF_3DVramMan_InitFrameTexVramManager(1U, 1);
    GF_3DVramMan_InitFramePlttVramManager(0x4000U, 1);
    v0 = NNS_GfdDefaultFuncAllocTexVram(0x4000U, 0, 0U);
    v1 = NNS_GfdDefaultFuncAllocPlttVram(0x80U, 0, 1U);
    PokepicManager_SetCharBaseAddrAndSize(*((PokepicManager * *)&arg0->spriteManager), (s32)((u32)(v0 << 0x10) >> 0xD), ((u32)(0x7FFF0000 & v0) >> 0x10) * 0x10);
    PokepicManager_SetPlttBaseAddrAndSize(*((PokepicManager * *)&arg0->spriteManager), (s32)((u32)(v1 << 0x10) >> 0xD), ((u32)(0xFFFF0000 & v1) >> 0x10) * 8);
}
