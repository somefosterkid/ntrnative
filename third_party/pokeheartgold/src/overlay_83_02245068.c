#include "global.h"

u16 ov83_02245068(u8 adjustment);

u16 ov83_02245068(u8 adjustment) {
    if (adjustment == 1) {
        return 1;
    }

    return 15;
}
