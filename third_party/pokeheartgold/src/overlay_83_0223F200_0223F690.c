#include "global.h"

#include "frontier/overlay_80_022340E8.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "palette.h"
#include "overlay_83_battle_castle_self_app.h"

s32 ov83_0223F7E4(void);

s32 ov83_0223F804(BgConfig *);

s32 ov83_0223FA00(void *, s32);

s32 ov83_0223FA74(void);

s32 ov83_0223FAA8(void *, s32);

s32 ov83_0223FAF0(void);

s32 ov83_0223FBEC(void *, s32);

s32 ov83_022477EC(s32, s32, void *);

void ov83_0223F690(BattleCastleSelfApp *arg0);

void ov83_0223F690(BattleCastleSelfApp *arg0) {
    reg_GX_POWCNT &= 0xFFFF7FFF;
    ov83_0223F7E4();
    ov83_0223F804(arg0->bgConfig);
    arg0->plttData = PaletteData_Init(HEAP_ID_107);
    PaletteData_AllocBuffers(arg0->plttData, PLTTBUF_MAIN_OBJ, 0x200U, HEAP_ID_107);
    PaletteData_AllocBuffers(arg0->plttData, PLTTBUF_MAIN_BG, 0x200U, HEAP_ID_107);
    ov83_0223FA00(arg0, 3);
    ov83_0223FA74();
    ov83_0223FAA8(arg0, 2);
    ov83_0223FAF0();
    ov83_022477EC(2, 0, &arg0->unk_868);
    ov83_0223FBEC(arg0, 4);
}

s32 ov83_02246E08(void *, Party *, s32);

void ov83_0223F70C(BattleCastleSelfApp *arg0);

void ov83_0223F70C(BattleCastleSelfApp *arg0) {
    ov83_02246E08(&arg0->spriteMan, arg0->party, (u8)ov80_02237D8C(arg0->challengeType));
}

void ov83_0223F734(BgConfig *arg0);

void ov83_0223F734(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(0x1FU, 0U);
    GfGfx_EngineBTogglePlanes(0x1FU, 0U);
    FreeBgTilemapBuffer(bgConfig, 3U);
    FreeBgTilemapBuffer(bgConfig, 2U);
    FreeBgTilemapBuffer(bgConfig, 0U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 7U);
    Heap_Free(bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
}
