#include "global.h"

u32 ov96_0220DE90(u8 *param_1, u8 *param_2);

u32 ov96_0220DE90(u8 *param_1, u8 *param_2) {
    if (*param_1 == *param_2) {
        return 0;
    }
    if (*param_1 < *param_2) {
        return 1;
    }
    return 0xffffffff;
}
