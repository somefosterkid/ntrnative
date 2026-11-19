#include "global.h"

s32 ov01_021E9700(s32 arg0);

s32 ov01_021E9700(s32 arg0) {
    s32 v0;

    v0 = 0xFFFF;
    switch (arg0) {
    case 0x3D:
        v0 = 0;
        break;
    case 0xE0:
        v0 = 1;
        break;
    }
    return v0;
}
