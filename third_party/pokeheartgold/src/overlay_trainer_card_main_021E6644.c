#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "trainer_card_main.h"

void ov51_021E6644(BgConfig *arg0);

void ov51_021E6644(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(0x1FU, 0U);
    GfGfx_EngineBTogglePlanes(0x1FU, 0U);
    BgSetPosTextAndCommit(bgConfig, 5U, BG_POS_OP_SET_ROT, 0);
    BgSetPosTextAndCommit(bgConfig, 5U, BG_POS_OP_SET_XSCALE, 0);
    BgSetPosTextAndCommit(bgConfig, 3U, BG_POS_OP_SET_ROT, 0);
    BgSetPosTextAndCommit(bgConfig, 3U, BG_POS_OP_SET_XSCALE, 0);
    FreeBgTilemapBuffer(bgConfig, 7U);
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 2U);
    FreeBgTilemapBuffer(bgConfig, 3U);
    Heap_Free(bgConfig);
}

s32 ov51_021E6CCC(void *);

s32 ov51_021E74D4(void *, u8, u8);

void ov51_021E66C0(void *arg0);

void ov51_021E66C0(void *arg0) {
    if (*((s32 *)&((TrainerCardMain *)arg0)->showingBack) == 0) {
        GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 0x2F, *(BgConfig **)arg0, GF_BG_LYR_SUB_2, 0U, 0U, 0, HEAP_ID_25);
        ov51_021E74D4(((TrainerCardMain *)arg0)->windows, 7, 0xA);
        BgClearTilemapBufferAndCommit(*(BgConfig **)arg0, 7U);
        return;
    }
    GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 0x30, *(BgConfig **)arg0, GF_BG_LYR_SUB_2, 0U, 0U, 0, HEAP_ID_25);
    ov51_021E74D4(((TrainerCardMain *)arg0)->windows, 0, 6);
    ov51_021E6CCC(arg0);
    BgClearTilemapBufferAndCommit(*(BgConfig **)arg0, 7U);
}
