#include "global.h"

s32 ov40_02230404();

s32 ov40_02230410(void);

s32 ov40_02230410(void) {
    if (ov40_02230404() == 1) {
        return 0x51;
    }
    return 0x52;
}
