#include "global.h"

s32 ov38_0221BE84(void);

s32 ov38_0221BFEC(void);

s32 ov72_02237D50(s32 *, s32 *, s32, void *, s32);

extern s32 _0223B820;

extern s32 ov72_0223B52C;

extern s32 ov72_0223B834;

extern s32 ov72_0223B920;

void ov72_02237BD0(s32 arg0, s32 arg1, void *arg2);

void ov72_02237BD0(s32 arg0, s32 arg1, void *arg2) {
    *(void **)((u8 *)(&ov72_0223B920) + 8) = arg2;
    ov38_0221BE84();
    *(s8 *)((u8 *)(&_0223B820) + 0x14) = (s8)(arg0 - 1);
    *(s8 *)((u8 *)(&_0223B820) + 0x15) = (s8)(arg1 - 1);
    if (ov72_02237D50(&ov72_0223B52C, &ov72_0223B834, 2, arg2, 0xA38) != 0) {
        *(s32 *)(&_0223B820) = 4;
        return;
    }
    *(s32 *)(&_0223B820) = 0xC;
    *(s32 *)((u8 *)(&_0223B820) + 4) = -5;
    ov38_0221BFEC();
}
