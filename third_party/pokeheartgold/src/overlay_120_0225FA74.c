#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_120.h"
#include "unk_0200FA24.h"

void ov120_0225FA74(SysTask *task, EncounterEffect *effect, BOOL reverse) {
    u16 plttData;
    BOOL finished;
    s32 color;
    Ov120TilesWork *newWork;
    Ov120TilesWork *work;

    work = effect->param;
    switch (effect->state) {
    case 0:
        newWork = Heap_Alloc(HEAP_ID_FIELD1, sizeof(Ov120TilesWork));
        effect->param = newWork;
        memset(newWork, 0, sizeof(Ov120TilesWork));
        newWork = effect->param;
        newWork->window = AllocWindows(HEAP_ID_FIELD1, 1);
        AddWindowParameterized(effect->fieldSystem->bgConfig, newWork->window, 3, 0, 0, 0x20, 0x20, 0, 0);
        plttData = 0;
        BG_LoadPlttData(3, &plttData, 2, 0x1E);
        FillWindowPixelBuffer(newWork->window, 0);
        ScheduleWindowCopyToVram(newWork->window);
        newWork->tiles = ov120_0225F268(HEAP_ID_FIELD1);
        effect->state = 1;
        return;
    case 1:
        if (reverse != FALSE) {
            color = 0x10;
        } else {
            color = -0x10;
        }
        ov01_021EFCF8(1, color, color, &effect->effectComplete, 2);
        effect->state = 2;
        return;
    case 2:
        if (effect->effectComplete != FALSE) {
            effect->state = 3;
            return;
        }
        return;
    case 3:
        ov120_0225F2B8(work->tiles, 0, 4, work->window, 0xF, reverse);
        effect->state = 4;
        return;
    case 4:
        finished = ov120_0225F318(work->tiles);
        ScheduleWindowCopyToVram(work->window);
        if (finished != FALSE) {
            effect->state = 5;
            return;
        }
        break;
    case 5:
        sub_0200FC20(0);
        if (effect->done != NULL) {
            *effect->done = TRUE;
        }
        ov120_0225F294(work->tiles);
        ClearWindowTilemapAndCopyToVram(work->window);
        RemoveWindow(work->window);
        WindowArray_Delete(work->window, 1);
        ov01_021EFCDC(effect, task);
        break;
    }
}

void ov120_0225FBAC(SysTask *task, EncounterEffect *effect) {
    ov120_0225FBC4(task, effect, TRUE);
}

void ov120_0225FBB8(SysTask *task, EncounterEffect *effect) {
    ov120_0225FBC4(task, effect, FALSE);
}
