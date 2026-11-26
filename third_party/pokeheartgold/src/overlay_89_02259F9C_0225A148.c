#include "global.h"

s32 ov89_0225C724(void *, void *, void *, s32, s32);

void ov89_0225A148(void *arg0, s32 arg1);

void ov89_0225A148(void *arg0, s32 arg1) {
    ov89_0225C724(*(void **)((u8 *)arg0 + 0x30), *(void **)((u8 *)arg0 + 0x2C), arg0 + 0x34, **(s32 **)arg0, arg1);
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov89_0225C818(void *);
#else
s32 ov89_0225C818(void *);
#endif

void ov89_0225A160(u8 *arg0);

void ov89_0225A160(u8 *arg0) {
    ov89_0225C818(arg0 + 0x34);
}
