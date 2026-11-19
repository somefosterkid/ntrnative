#include "global.h"

#include "fashion_case.h"

s32 sub_0202BAB0(const FashionCase *fashionCase, u32 backdropID);

int sub_0202BA5C(FashionCase *fashionCase, int a1) {
    if (sub_0202BAB0(fashionCase, a1) != 0x12) {
        return 1;
    }
    return 0;
}
