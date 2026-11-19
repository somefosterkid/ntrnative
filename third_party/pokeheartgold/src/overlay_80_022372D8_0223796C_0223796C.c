#include "global.h"

u16 ov80_022379C0();

extern s32 ov80_0223C5B8;

u8 ov80_0223796C(void);

u8 ov80_0223796C(void) {
    return *(u8 *)(&ov80_0223C5B8 + ov80_022379C0());
}
