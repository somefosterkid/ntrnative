#include "global.h"

#include "constants/heap.h"

#include "frontier/frontier_map.h"
#include "frontier/overlay_80_022340E8.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"

void ov80_02234A38(ArcadeContext *ctx, FrontierMap *map) {
    NARC *narc;

    narc = NARC_New(NARC_a_1_8_3, HEAP_ID_101);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x39, (BgConfig *)map->unk0, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_101);
    ScheduleBgTilemapBufferTransfer((BgConfig *)map->unk0, 3U);
    NARC_Delete(narc);
}
