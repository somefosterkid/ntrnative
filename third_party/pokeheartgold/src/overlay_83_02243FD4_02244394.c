#include "global.h"
struct BattleCastleOpponentApp;

#include "frontier/overlay_80_022340E8.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "palette.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_022444C0(void);

s32 ov83_022444E0(BgConfig *);

s32 ov83_0224465C(void *, s32);

s32 ov83_022446D0(void);

s32 ov83_02244704(void *, s32);

s32 ov83_0224474C(void);

s32 ov83_02244780(void *, s32);

void ov83_02244394(struct BattleCastleOpponentApp *arg0);

void ov83_02244394(BattleCastleOpponentApp *arg0) {
    reg_GX_POWCNT &= 0xFFFF7FFF;
    ov83_022444C0();
    ov83_022444E0(*((void **)&arg0->bgConfig));
    *((PaletteData * *)&arg0->unk_2B0) = PaletteData_Init(HEAP_ID_107);
    PaletteData_AllocBuffers(*((PaletteData * *)&arg0->unk_2B0), PLTTBUF_MAIN_OBJ, 0x200U, HEAP_ID_107);
    PaletteData_AllocBuffers(*((PaletteData * *)&arg0->unk_2B0), PLTTBUF_MAIN_BG, 0x200U, HEAP_ID_107);
    ov83_0224465C(arg0, 3);
    ov83_022446D0();
    ov83_02244704(arg0, 2);
    ov83_0224474C();
    GfGfx_EngineATogglePlanes(4U, 0U);
    ov83_02244780(arg0, 4);
}

s32 ov83_02246E08(void *, Party *, s32);

void ov83_02244408(void *arg0);

void ov83_02244408(void *arg0) {
    ov83_02246E08(arg0 + 0x2C8, *((void **)&((BattleCastleOpponentApp *)arg0)->opponentsParty), (u8)ov80_02237D8C(((BattleCastleOpponentApp *)arg0)->challengeType));
}

void ov83_0224442C(BgConfig *arg0);

void ov83_0224442C(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(0x1FU, 0U);
    GfGfx_EngineBTogglePlanes(0x1FU, 0U);
    FreeBgTilemapBuffer(bgConfig, 3U);
    FreeBgTilemapBuffer(bgConfig, 2U);
    FreeBgTilemapBuffer(bgConfig, 0U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    Heap_Free(bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
}
