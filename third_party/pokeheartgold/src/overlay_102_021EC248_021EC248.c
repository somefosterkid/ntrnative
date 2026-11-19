#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"

void ov102_021EC248(void *arg0, NARC *arg1);

void ov102_021EC248(void *arg0, NARC *narc) {
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0xB, *(BgConfig **)((u8 *)arg0 + 0xC), GF_BG_LYR_SUB_1, 0U, 0U, 1, HEAP_ID_35);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0xC, *(BgConfig **)((u8 *)arg0 + 0xC), GF_BG_LYR_SUB_1, 0U, 0U, 1, HEAP_ID_35);
    BgCommitTilemapBufferToVram(*(BgConfig **)((u8 *)arg0 + 0xC), 5U);
}

void ov102_021EC290(void);

void ov102_021EC290(void) {
}

void ov102_021EC294(void);

void ov102_021EC294(void) {
}
