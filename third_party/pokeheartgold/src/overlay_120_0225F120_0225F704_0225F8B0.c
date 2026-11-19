#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "overlay_120.h"

BOOL ov120_0225F8B0(Ov120StepWork *work, s32 *state) {
    switch (*state) {
    case 0:
        work->wipe = ov120_0225F0FC(HEAP_ID_FIELD1);
        ov120_0225F120(work->wipe, 0, work->window, 0xF);
        *state = 1;
        break;
    case 1: {
        s32 done = ov120_0225F14C(work->wipe);
        ScheduleWindowCopyToVram(work->window);
        if (done != 0) {
            *state = 2;
        }
        break;
    }
    case 2:
        ov120_0225F118(work->wipe);
        *state = 0;
        return TRUE;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}
