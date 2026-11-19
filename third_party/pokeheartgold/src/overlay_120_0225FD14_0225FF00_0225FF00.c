#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_01_021EFB38.h"
#include "overlay_120.h"
#include "unk_0200FA24.h"

extern const BgTemplate ov120_022601BC;
extern const BgTemplate ov120_022601D8;

#if defined(SDK_PORT)
// On PC the overlay's asm rodata (asm/overlay_120.s) is not linked; these are byte-for-byte copies of it.
const BgTemplate ov120_022601BC = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x1000,
    .baseTile = 0,
    .size = 2,
    .colorMode = 0,
    .screenBase = 2,
    .charBase = 1,
    .bgExtPltt = 0,
    .priority = 0,
    .areaOver = 0,
    .dummy = 0,
    .mosaic = 0,
};

const BgTemplate ov120_022601D8 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x1000,
    .baseTile = 0,
    .size = 2,
    .colorMode = 0,
    .screenBase = 4,
    .charBase = 2,
    .bgExtPltt = 1,
    .priority = 0,
    .areaOver = 0,
    .dummy = 0,
    .mosaic = 0,
};
#endif

void ov120_0225FF00(SysTask *task, EncounterEffect *effect, BOOL reverse) {
    Ov120CrossWork *work;
    s32 color;

    work = effect->param;
    switch (effect->state) {
    case 0:
        work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(Ov120CrossWork));
        effect->param = work;
        memset(work, 0, sizeof(Ov120CrossWork));
        work = effect->param;
        GfGfx_EngineATogglePlanes(2, 1);
        GfGfx_EngineATogglePlanes(8, 1);
        reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffffe0ff | 0x1500;
        FreeBgTilemapBuffer(effect->fieldSystem->bgConfig, 1);
        InitBgFromTemplate(effect->fieldSystem->bgConfig, 1, &ov120_022601BC, 0);
        BgClearTilemapBufferAndCommit(effect->fieldSystem->bgConfig, 1);
        FreeBgTilemapBuffer(effect->fieldSystem->bgConfig, 3);
        InitBgFromTemplate(effect->fieldSystem->bgConfig, 3, &ov120_022601D8, 0);
        BgClearTilemapBufferAndCommit(effect->fieldSystem->bgConfig, 3);
        SetBgPriority(2, 0);
        SetBgPriority(8, 0);
        GfGfx_EngineASetPlanes(0x15);
        ov120_0225FECC(&work->loader, effect);
        effect->state = 1;
        break;
    case 1:
        if (ov120_0225FEE8(&work->loader) != FALSE) {
            BG_SetMaskColor(1, 0);
            ov01_021EFEC8(&work->scroll1, 0, 0x140000, 0, 0x12);
            ov01_021EFEC8(&work->scroll3, 0, 0x140000, 0x8000, 0x1e);
            ov120_0225F6AC(&work->wobble, 0, 0x10, 0xf);
            ScheduleSetBgPosText(effect->fieldSystem->bgConfig, 3, BG_POS_OP_SET_XSCALE, work->scroll1.currentValue >> 0xc);
            ScheduleSetBgPosText(effect->fieldSystem->bgConfig, 3, BG_POS_OP_SET_ROT, 0);
            ScheduleSetBgPosText(effect->fieldSystem->bgConfig, 1, BG_POS_OP_SET_XSCALE, work->scroll3.currentValue >> 0xc);
            ScheduleSetBgPosText(effect->fieldSystem->bgConfig, 1, BG_POS_OP_SET_ROT, 0);
            if (reverse != FALSE) {
                color = 0x10;
            } else {
                color = -0x10;
            }
            ov01_021EFCF8(1, color, color, &effect->effectComplete, 2);
            effect->state = 2;
        }
        break;
    case 2:
        if (effect->effectComplete != FALSE) {
            ov01_021F12B4(&work->shake, HEAP_ID_FIELD1);
            effect->state = 3;
        }
        break;
    case 3:
        work->delay = 10;
        ov01_021F12E8(&work->shake, 0, 0xbf, 0xd52, 0x3000, 400, (void *)REG_BG0OFS_ADDR, 0, 4);
        work->scroll3Active = 1;
        GfGfx_EngineATogglePlanes(8, 1);
        effect->state = 4;
    case 4:
        if (work->delay-- == 0) {
            effect->state = 5;
        }
        break;
    case 5:
        work->scroll1Active = 1;
        GfGfx_EngineATogglePlanes(2, 1);
        effect->state = 6;
        break;
    case 6:
        if (work->scroll1Done != 0) {
            effect->state = 7;
        }
        break;
    case 7:
        sub_0200FC20(0);
        if (effect->done != NULL) {
            *effect->done = TRUE;
        }
        ov01_021F12D0(&work->shake);
        ov01_021EFCDC(effect, task);
        return;
    }
    if (work->scroll1Active != 0) {
        work->scroll1Done = ov01_021EFF28(&work->scroll1);
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, 1, BG_POS_OP_SET_XSCALE, work->scroll1.currentValue >> 0xc);
    }
    if (work->scroll3Active != 0) {
        work->scroll3Done = ov01_021EFF28(&work->scroll3);
        ov120_0225F6BC(&work->wobble);
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, 3, BG_POS_OP_SET_XSCALE, work->scroll3.currentValue >> 0xc);
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, 3, BG_POS_OP_SET_ROT, work->wobble.value);
    }
}
