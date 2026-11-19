#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_82.h"
#include "palette.h"
#include "party.h"
#include "overlay_82_battle_hall_app.h"

s32 ov82_0223EC48(void);

s32 ov82_0223EC68(BgConfig *);

s32 ov82_0223ED94(void *);

s32 ov82_0223EDF0(void *, s32);

s32 ov82_0223EE38(void);

s32 ov82_0223EE6C(void *, s32);

void ov82_0223EB3C(void *arg0) {
    ov82_0223EC48();
    ov82_0223EC68(*((void **)&((BattleHallApp *)arg0)->bgConfig));
    ov82_0223ED94(arg0);
    ((BattleHallApp *)arg0)->plttData = PaletteData_Init(HEAP_ID_105);
    PaletteData_AllocBuffers(((BattleHallApp *)arg0)->plttData, PLTTBUF_MAIN_OBJ, 0x200U, HEAP_ID_105);
    PaletteData_AllocBuffers(((BattleHallApp *)arg0)->plttData, PLTTBUF_MAIN_BG, 0x200U, HEAP_ID_105);
    ov82_0223EDF0(arg0, 3);
    ov82_0223EE38();
    GfGfx_EngineATogglePlanes(4U, 0U);
    ov82_0223EE6C(arg0, 5);
}

s32 ov82_0223F95C(void *, Pokemon *);

void ov82_0223EB9C(void *arg0) {
    ov82_0223F95C(arg0 + 0xA8, Party_GetMonByIndex(((BattleHallApp *)arg0)->party, 0));
}

void ov82_0223EBB8(BgConfig *arg0);

void ov82_0223EBB8(BgConfig *bgConfig) {
    reg_GX_DISPCNT &= 0xFFFF1FFF;
    GfGfx_EngineATogglePlanes(0x1FU, 0U);
    GfGfx_EngineBTogglePlanes(0x1FU, 0U);
    FreeBgTilemapBuffer(bgConfig, 3U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 0U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    Heap_Free(bgConfig);
}
