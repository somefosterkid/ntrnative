#include "global.h"
struct TradeReceivePhase;

#include "gf_3d_vramman.h"
#include "pokepic.h"
#include "unk_02005D10.h"
#include "overlay_71_trade_receive_phase.h"

s32 ov71_0224B6F0(TradeReceivePhase *arg0, s32 arg1);

s32 ov71_0224B6F0(TradeReceivePhase *arg0, s32 arg1) {
    s32 v0;

    if (arg0->fanfarePlaying != 0) {
        arg0->fanfarePlaying = IsFanfarePlaying();
    }
    v0 = arg0->timer;
    if (v0 < arg1) {
        arg0->timer = v0 + 1;
        return 0;
    }
    if (arg0->fanfarePlaying == 0) {
        return 1;
    }
    return 0;
}

void ov71_0224B720(struct TradeReceivePhase *arg0);

void ov71_0224B720(TradeReceivePhase *arg0) {
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
