#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_81.h"
#include "palette.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_02240210(void);

s32 ov81_02240230(BgConfig *);

s32 ov81_02240698(void *, s32);

s32 ov81_02240770(void);

s32 ov81_022407A8(void *, s32);

void ov81_022403C0(void *arg0);

void ov81_022403C0(void *arg0) {
    ov81_02240210();
    ov81_02240230(((BattleFactoryApp *)arg0)->bgConfig);
    ((BattleFactoryApp *)arg0)->plttData = PaletteData_Init(HEAP_ID_100);
    PaletteData_AllocBuffers(((BattleFactoryApp *)arg0)->plttData, PLTTBUF_MAIN_OBJ, 0x200U, HEAP_ID_100);
    PaletteData_AllocBuffers(((BattleFactoryApp *)arg0)->plttData, PLTTBUF_MAIN_BG, 0x200U, HEAP_ID_100);
    ov81_02240770();
    ov81_02240728(arg0, GF_BG_LYR_SUB_2);
    ov81_02240698(arg0, 3);
    ov81_022406E0(arg0, GF_BG_LYR_MAIN_2);
    if (((u32)(*((u8 *)((u8 *)&((BattleFactoryApp *)arg0)->numPokeballs + 0x1)) << 0x1B) >> 0x1F) != 0) {
        BgSetPosTextAndCommit(((BattleFactoryApp *)arg0)->bgConfig, 6U, BG_POS_OP_SET_ROT, *((s32 *)&((BattleFactoryApp *)arg0)->conveyorXOffset));
    }
    GfGfx_EngineATogglePlanes(2U, 1U);
    GfGfx_EngineATogglePlanes(4U, 0U);
    GfGfx_EngineATogglePlanes(8U, 1U);
    ov81_022407A8(arg0, 7);
}

s32 ov81_0224276C(void *);

void ov81_02240448(void *arg0);

void ov81_02240448(void *arg0) {
    ov81_0224276C(&((BattleFactoryApp *)arg0)->spriteMan);
}

void ov81_02240458(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(0x1FU, 0U);
    GfGfx_EngineBTogglePlanes(0x1FU, 0U);
    FreeBgTilemapBuffer(bgConfig, 3U);
    FreeBgTilemapBuffer(bgConfig, 2U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 7U);
    Heap_Free(bgConfig);
}

void ov81_022404AC(BattleFactoryApp *app, s32 *arg1, s32 arg2) {
    app->subState = 0;
    *arg1 = arg2;
}
