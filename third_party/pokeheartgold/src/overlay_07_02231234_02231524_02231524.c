#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "palette.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_stat_change_context.h"

u16 ov07_0221FB04(BattleAnimSystem *system, u32 arg1);

extern u32 ov07_0223680C;

extern u32 ov07_02236810;

extern u32 ov07_02236814;

void ov07_02231524(StatChangeContext *ctx, GFBgLayer bgLayer) {
    enum HeapID heapID = ov07_0221BFD0(ctx->common.battleAnimSys);
    u16 priority = ov07_0221FB04(ctx->common.battleAnimSys, 3);

    SetBgPriority(bgLayer, priority & 0xff);
    GfGfxLoader_LoadCharData(ctx->config.narcID, *(s32 *)(&ov07_0223680C + ctx->config.memberTableIndex * 4), ctx->common.bgConfig, bgLayer, 0, 0, 1, heapID);
    PaletteData_LoadNarc(ctx->common.paletteData, ctx->config.narcID, *(s32 *)(&ov07_02236810 + ctx->config.memberTableIndex * 4), heapID, PLTTBUF_MAIN_BG, 0x20, 0x80);
    BgClearTilemapBufferAndCommit(ctx->common.bgConfig, (u8)bgLayer);
    GfGfxLoader_LoadScrnData(ctx->config.narcID, *(s32 *)(&ov07_02236814 + ctx->config.memberTableIndex * 4), ctx->common.bgConfig, bgLayer, 0, 0, 1, heapID);
}
