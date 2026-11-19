#include "global.h"

s32 ov38_0221BE84(void);

s32 ov38_0221BFEC(void);

s32 ov72_02237D50(s32 *, s32 *, s32, s32 *, s32);

extern s32 _0223B820;

extern s32 ov72_0223B5C4;

extern s32 ov72_0223B834;

extern s32 ov72_0223B923;

void ov72_02237CB0(void);

void ov72_02237CB0(void) {
    ov38_0221BE84();
    if (ov72_02237D50(&ov72_0223B5C4, &ov72_0223B834, 0, &ov72_0223B923, 2) != 0) {
        *(s32 *)(&_0223B820) = 8;
        return;
    }
    *(s32 *)(&_0223B820) = 0xC;
    *(s32 *)((u8 *)(&_0223B820) + 4) = -5;
    ov38_0221BFEC();
}
