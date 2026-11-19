#include "global.h"

void ov96_021FE538(void *param_1, void *param_2, void *param_3);

void ov96_021FE538(void *param_1, void *param_2, void *param_3) {
    u8 *p = (u8 *)param_3;
    if (p[0xa9] != 0) {
        p[0xa9]--;
    }
}
