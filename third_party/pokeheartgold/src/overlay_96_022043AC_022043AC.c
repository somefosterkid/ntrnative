#include "global.h"

void ov96_022043AC(void *param_1, u32 param_2, void *param_3, void *param_4);

void ov96_022043AC(void *param_1, u32 param_2, void *param_3, void *param_4) {
    *(void **)(param_1 + param_2 * 4) = param_3;
    *(void **)(param_1 + (param_2 * 4 + 0x30)) = param_4;
    return;
}
