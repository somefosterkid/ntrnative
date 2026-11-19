#include "global.h"

#include "overlay_120.h"

void ov120_0225F6FC(Ov120StepTask *task, Ov120StepFunc func) {
    task->func = func;
    task->state = 0;
}
