#include "global.h"

u32 ov12_022395BC(s32 param_1);

u32 ov12_022395BC(s32 param_1) {
    switch (param_1) {
    case 0x42:
    case 0x43:
    case 0x46:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4b:
    case 0x4c:
    case 0x56:
    case 0x57:
    case 0x58:
    case 0x59:
    case 0x62:
    case 0x67:
    case 0x68:
    case 0x69:
    case 0x6a:
    case 0x6b:
    case 0x6c:
    case 0x6e:
    case 0x70:
        return 1;
    default:
        return 0;
    }
}
