#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "render_window.h"
#include "sprite.h"
#include "system.h"
#include "party.h"
#include "overlay_65_trade_room.h"

void ov65_0221BFEC(TradeRoom *);

void ov65_0221C1C4(TradeRoom *);

void ov65_0221C46C(Party *, u32, void *);

void ov65_0221CE98(void);

void ov65_0221CEB8(BgConfig *);

void ov65_0221D280(void *, NARC *);

void ov65_0221D3B8(void);

void ov65_0221D3E8(void *, NARC *);

void ov65_0221D930(void *);

void ov65_0221FB90(Window *, u32, u32, void *, void *);

void ov65_0221CE1C(void *param);

void ov65_0221C08C(TradeRoom *param_1);

s32 ov65_0221C3DC(void *, Party *, s32);
s32 ov65_0221C414(Window *, void *, void *, Party *, s32);
s32 ov65_0221D25C(BgConfig *, s32, s32);
void ov65_0221C1C4(TradeRoom *arg0);

void ov65_0221C08C(TradeRoom *param_1) {
    NARC *narc;
    u32 uVar1;

    narc = NARC_New(NARC_a_0_5_2, HEAP_ID_26);
    ov65_0221CE98();
    ov65_0221CEB8(((void *)param_1->bgConfig));
    ov65_0221D280(param_1, narc);
    ov65_0221C1C4(param_1);
    ov65_0221BFEC(param_1);
    uVar1 = Options_GetFrame(*(Options **)(((u8 *)param_1->args) + 0x18));
    LoadUserFrameGfx2(param_1->bgConfig, GF_BG_LYR_MAIN_0, 0x3d9, 0xa, (u8)uVar1, HEAP_ID_26);
    LoadUserFrameGfx1(param_1->bgConfig, GF_BG_LYR_MAIN_0, 0x3f7, 0xb, 0, HEAP_ID_26);
    ov65_0221FB90(&param_1->windows[21], 0xf, 1, ((void *)param_1->msgLoader), ((void *)param_1->strTemplate));
    ToggleBgLayer(4, 1);
    ToggleBgLayer(5, 1);
    ToggleBgLayer(6, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
    ToggleBgLayer(0, 1);
    ToggleBgLayer(1, 1);
    ToggleBgLayer(2, 1);
    ToggleBgLayer(3, 1);
    GfGfx_EngineATogglePlanes(0x10, 1);
    ov65_0221D3B8();
    ov65_0221D3E8(param_1, narc);
    ov65_0221D930(param_1);
    ov65_0221C46C(((void *)param_1->playerParty), 0, param_1);
    ov65_0221C46C(((void *)param_1->partnerParty), 6, param_1);
    Sprite_SetDrawFlag(param_1->cursorSprites[0], 1);
    Sprite_SetDrawFlag(param_1->cursorSprites[1], 1);
    Main_SetVBlankIntrCB(ov65_0221CE1C, param_1);
    NARC_Delete(narc);
    return;
}

void ov65_0221C1C4(TradeRoom *arg0) {
    s32 v0;

    ov65_0221C3DC(*((void **)&arg0->strTemplate), arg0->playerParty, 0);
    ov65_0221C3DC(*((void **)&arg0->strTemplate), arg0->partnerParty, 6);
    ov65_0221C414(&arg0->windows[7], *((void **)&arg0->strTemplate), *((void **)&arg0->msgLoader), arg0->playerParty, 0);
    ov65_0221C414(&arg0->windows[13], *((void **)&arg0->strTemplate), *((void **)&arg0->msgLoader), arg0->partnerParty, 6);
    v0 = Party_GetCount(arg0->playerParty);
    ov65_0221D25C(*((void **)&arg0->bgConfig), v0, Party_GetCount(arg0->partnerParty));
}
