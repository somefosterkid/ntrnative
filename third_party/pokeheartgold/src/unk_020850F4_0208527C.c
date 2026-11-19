#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_manager.h"
#include "palette.h"
#include "sprite_system.h"
#include "touch_hitbox_controller.h"
#include "unk_020210A0.h"
#include "unk_0203A3B0.h"
#include "unk_020850F4.h"
#include "constants/heap.h"
#include "options.h"
#include "pm_string.h"
#include "unk_struct_02089688.h"

s32 sub_020866CC(void *);

typedef struct {
    int unk_00;
    int unk_04[5];
    int unk_18;
    String *unk_1C;
    Options *options;
    int unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
} UnkStruct_02089438;
extern UnkStruct_02089438 *sub_020853B4(enum HeapID heapID, int param1, int param2[], Options *options, u32 param4, u32 param5);

s32 sub_0208527C(OverlayManager *man) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    if (*((s32 *)&((UnkStruct_02089688 *)v0)->unk_3B8.unk_30) != 0) {
        sub_0203A914();
    }
    sub_020866CC(v0);
    GfGfx_EngineATogglePlanes(1U, 0U);
    GfGfx_EngineATogglePlanes(2U, 0U);
    GfGfx_EngineATogglePlanes(4U, 0U);
    GfGfx_EngineATogglePlanes(8U, 0U);
    GfGfx_EngineBTogglePlanes(1U, 0U);
    GfGfx_EngineBTogglePlanes(2U, 0U);
    GfGfx_EngineBTogglePlanes(4U, 0U);
    GfGfx_EngineBTogglePlanes(8U, 0U);
    FreeBgTilemapBuffer(((UnkStruct_02089688 *)v0)->unk_2EC.unk_0C, 1U);
    FreeBgTilemapBuffer(((UnkStruct_02089688 *)v0)->unk_2EC.unk_0C, 2U);
    FreeBgTilemapBuffer(((UnkStruct_02089688 *)v0)->unk_2EC.unk_0C, 3U);
    FreeBgTilemapBuffer(((UnkStruct_02089688 *)v0)->unk_2EC.unk_0C, 4U);
    FreeBgTilemapBuffer(((UnkStruct_02089688 *)v0)->unk_2EC.unk_0C, 5U);
    FreeBgTilemapBuffer(((UnkStruct_02089688 *)v0)->unk_2EC.unk_0C, 6U);
    FreeBgTilemapBuffer(((UnkStruct_02089688 *)v0)->unk_2EC.unk_0C, 7U);
    Heap_Free(((UnkStruct_02089688 *)v0)->unk_2EC.unk_0C);
    PaletteData_FreeBuffers(((UnkStruct_02089688 *)v0)->unk_2EC.unk_10, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(((UnkStruct_02089688 *)v0)->unk_2EC.unk_10, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(((UnkStruct_02089688 *)v0)->unk_2EC.unk_10, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(((UnkStruct_02089688 *)v0)->unk_2EC.unk_10, PLTTBUF_SUB_OBJ);
    PaletteData_Free(((UnkStruct_02089688 *)v0)->unk_2EC.unk_10);
    NARC_Delete(((UnkStruct_02089688 *)v0)->unk_2EC.unk_00);
    DisableTouchPad();
    SpriteSystem_FreeResourcesAndManager(((UnkStruct_02089688 *)v0)->unk_2EC.unk_04, ((UnkStruct_02089688 *)v0)->unk_2EC.unk_08);
    SpriteSystem_Free(((UnkStruct_02089688 *)v0)->unk_2EC.unk_04);
    TouchHitboxController_Destroy(((TouchHitboxController *)((UnkStruct_02089688 *)v0)->unk_2EC.unk_14));
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_108);
    return 1;
}

UnkStruct_02089438 *sub_020853B4(enum HeapID heapID, int param1, int param2[], Options *options, u32 param4, u32 param5) {
    int i;
    UnkStruct_02089438 *v1 = 0L;

    v1 = Heap_Alloc(heapID, sizeof(UnkStruct_02089438));

    v1->unk_00 = param1;
    v1->unk_1C = String_New(param1 + 1, heapID);
    v1->options = options;

    for (i = 0; i < 4; i++) {
        v1->unk_04[i] = param2[i];
    }

    v1->unk_04[i] = param2[i - 1];
    v1->unk_2C = param4;
    v1->unk_30 = param5;

    return v1;
}
