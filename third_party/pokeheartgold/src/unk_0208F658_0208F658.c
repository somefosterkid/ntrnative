#include "global.h"

#include "unk_02033AE0.h"
#include "unk_0208F658.h"

extern s32 _02105430;

void sub_0208F658(void *fieldSystem) {
    sub_0203410C(&_02105430, 0xB, fieldSystem);
}

s32 sub_0208F66C(u32 param_1) {
    if (param_1 == 0xc) {
        return 0xc;
    }
    if (param_1 < 6) {
        return param_1 + 6;
    }
    return param_1 - 6;
}
