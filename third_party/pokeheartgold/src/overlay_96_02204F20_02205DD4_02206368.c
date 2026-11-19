#include "global.h"

#include "overlay_96_ov96hurdle_work.h"

void ov96_02206368(void *param_1, void *param_2, Ov96HurdleRunner *runner);

void ov96_02206368(void *param_1, void *param_2, Ov96HurdleRunner *runner) {
    if (runner->active != 0) {
        runner->active -= 1;
    }
    return;
}
