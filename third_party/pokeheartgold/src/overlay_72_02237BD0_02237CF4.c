#include "global.h"

s32 ov38_0221BE84(void);

s32 ov38_0221BFEC(void);

s32 ov72_02237D50(s32 *, s32 *, s32, void *, s32);

extern s32 _0223B820;

extern s32 ov72_0223B60C;

extern s32 ov72_0223B834;

extern s32 ov72_0223B920;

void ov72_02237CF4(void *arg0, void *arg1);

void ov72_02237CF4(void *arg0, void *arg1) {
    OS_GetMacAddress(arg0 + 0x1C);
    memcpy(&ov72_0223B834, arg0, 0x64U);
    *(void **)((u8 *)(&ov72_0223B920) + 8) = arg1;
    ov38_0221BE84();
    if (ov72_02237D50(&ov72_0223B60C, &ov72_0223B834, 0x64, *(void **)((u8 *)(&ov72_0223B920) + 8), 8) != 0) {
        *(s32 *)(&_0223B820) = 0xA;
        return;
    }
    *(s32 *)(&_0223B820) = 0xC;
    *(s32 *)((u8 *)(&_0223B820) + 4) = -5;
    ov38_0221BFEC();
}
