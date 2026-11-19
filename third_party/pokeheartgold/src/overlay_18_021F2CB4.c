#include "global.h"

/* DS: the callers were built against the wider return type below; the PC takes the definition's (abilint RET) */
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov18_021F2C98();
#else
u32 ov18_021F2C98();
#endif

s32 ov18_021F2CB4(void);

s32 ov18_021F2CB4(void) {
    return (ov18_021F2C98() >> 1) + 0x15;
}

s32 ov18_021F2CC0(void);

s32 ov18_021F2CC0(void) {
    return 0x83 - (ov18_021F2C98() >> 1);
}
