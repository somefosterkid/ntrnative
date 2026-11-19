#include "global.h"

#include "overlay_120.h"

BOOL ov120_0225F704(Ov120StepTask *task, Ov120StepWork *work) {
    return task->func(work, &task->state);
}
