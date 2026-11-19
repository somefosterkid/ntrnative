#include "global.h"

#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_battle_bg_anim.h"
#include "overlay_07_battle_bg_switch.h"
#include "bg_window.h"
#include "heap.h"
#include "palette.h"
#include "sys_task_api.h"

s32 ov07_0221E868(BattleBgSwitch *bgSwitch);

struct BufferManager;
struct CustomBgScrollContext;
void ov07_02222C60(CustomBgScrollContext *ctx);
void *ov07_02222C84(const CustomBgScrollContext *ctx);
u32 ov07_02222D88(s32 x, s32 y);
void ov07_0221E87C(SysTask *task, void *param);
void ov07_0221E914(SysTask *arg0, void *arg1);

s32 ov07_0221E868(BattleBgSwitch *bgSwitch) {
    bgSwitch->bgWaveAnimActive = 1;
    return 0;
}

void ov07_0221E87C(SysTask *task, void *param) {
    int stripIdx, scanline;
    WaveAnimContext *waveCtx = ((BattleBgAnim *)param)->waveAnimCtx;
    BattleBgAnim *bgAnim = param;

    if (bgAnim->cancel == 1) {
        ov07_02222C60(bgAnim->waveAnimCtx->bgScrollCtx);
        Heap_Free(bgAnim->waveAnimCtx);
        Heap_Free(bgAnim);
        SysTask_Destroy(task);
        return;
    }

    u32 *buffer = ov07_02222C84(waveCtx->bgScrollCtx);

    for (stripIdx = 0; stripIdx < ((192 - 64) / 8); stripIdx++) {
        waveCtx->strips[stripIdx].scrollAccum += waveCtx->strips[stripIdx].scrollSpeed;

        for (scanline = waveCtx->strips[stripIdx].startScanline; scanline < waveCtx->strips[stripIdx].endScanline; scanline++) {
            s16 baseX, baseY;

            baseX = waveCtx->strips[stripIdx].baseOffset & 0xffff;
            baseY = waveCtx->strips[stripIdx].baseOffset >> 16;

            buffer[scanline] = ov07_02222D88((u16)(baseX + waveCtx->strips[stripIdx].scrollAccum), (u16)baseY);
        }
    }
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov07_0221E914(SysTask *arg0, void *ptr) {
    BattleBgAnim *anim = ptr;
    BattleBgPaletteAnim *pal = anim->paletteAnim;
    u8 v0;
    u8 v1;
    if (anim->cancel == 1) {
        Heap_Free(pal);
        Heap_Free(anim);
        SysTask_Destroy(arg0);
        return;
    }
    v0 = pal->state;
    switch (v0) {
    case 0:
        pal->timer = 0xFFU;
        pal->state = (u8)(pal->state + 1);

    case 1:
        v1 = pal->curPalette;
        if (((s32)pal->timer) < ((s32)(pal->frameCount[v1] - 2))) {
            pal->timer = (u8)(pal->timer + 1);
            return;
        }
        PaletteData_LoadFromNarc(pal->paletteData, NARC_a_0_0_9, (s32)pal->narcMemberBase, pal->heapID, PLTTBUF_MAIN_BG, 0x20U, 0x90U, (u16)((u16)(pal->paletteIdx[v1] * 0x10)));
        pal->timer = 0U;
        if (((s32)pal->curPalette) >= ((s32)(pal->numPalettes - 1))) {
            pal->curPalette = 0U;
            return;
        }
        pal->curPalette = (u8)(pal->curPalette + 1);
        return;
    }
}
#else
void ov07_0221E914(SysTask *arg0, void *ptr) {
    u8 v0;
    u8 v1;
    u8 *new_var;
    void *v2;
    v2 = *((void **)(((u8 *)ptr) + 0x24));
    if ((*((s32 *)(((u8 *)ptr) + 0x18))) == 1) {
        Heap_Free(v2);
        Heap_Free(ptr);
        SysTask_Destroy(arg0);
        return;
    }
    v0 = *((u8 *)(((u8 *)v2) + 0x18B));
    new_var = (u8 *)v2;
    switch (v0) {
    case 0:
        *((u8 *)(new_var + 0x18A)) = 0xFFU;
        *((u8 *)(new_var + 0x18B)) = (u8)((*((u8 *)(((u8 *)v2) + 0x18B))) + 1);

    case 1:
        v1 = *((u8 *)(((u8 *)v2) + 0x18C));
        if (((s32)(*((u8 *)(((u8 *)v2) + 0x18A)))) < ((s32)((*((u16 *)(((u8 *)(v2 + (v1 * 2))) + 0x88))) - 2))) {
            *((u8 *)(((u8 *)v2) + 0x18A)) = (u8)((*((u8 *)(new_var + 0x18A))) + 1);
            return;
        }
        PaletteData_LoadFromNarc(*((PaletteData **)(((u8 *)v2) + 4)), NARC_a_0_0_9, (s32)(*((u8 *)(((u8 *)v2) + 0x189))), *((enum HeapID *)v2), PLTTBUF_MAIN_BG, 0x20U, 0x90U, (u16)((u16)((*((u8 *)(((u8 *)(v2 + v1)) + 8))) * 0x10)));
        *((u8 *)(((u8 *)v2) + 0x18A)) = 0U;
        if (((s32)(*((u8 *)(((u8 *)v2) + 0x18C)))) >= ((s32)((*((u8 *)(((u8 *)v2) + 0x188))) - 1))) {
            *((u8 *)(((u8 *)v2) + 0x18C)) = 0U;
            return;
        }
        *((u8 *)(((u8 *)v2) + 0x18C)) = (u8)((*((u8 *)(new_var + 0x18C))) + 1);
        return;
    }
}
#endif
