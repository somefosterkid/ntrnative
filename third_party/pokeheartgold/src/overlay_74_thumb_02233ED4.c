#include "global.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov74_02233DBC(s16);
#else
s32 ov74_02233DBC(s32);
#endif

#include "pm_agb_save.h"

s32 ov74_02233ED4(void);

extern void ov74_02233B04(OSArenaId id);
void ov74_02233F5C(void);

s32 ov74_02233ED4(void) {
    switch (ov74_0223D33C.writeStatus) {
    case 2:
        ov74_02233DBC(ov74_0223D33C.sectorsWritten);
        return 0;
    case 3:
        ov74_02233DBC(ov74_0223D33C.sectorsWritten);
    case 1:
        return 0xA;
    case 4:
        return 8;
    default:
        return 0;
    }
}

s32 ov74_02233F14(void);

s32 ov74_02233F14(void) {
    switch (ov74_0223D33C.writeStatus) {
    case 2:
        return 0xB;
    case 3:
        return 0xA;
    case 1:
        return 9;
    case 4:
        return 8;
    default:
        return 0;
    }
}

void *ov74_02233F4C(void);

void *ov74_02233F4C(void) {
    return ov74_0223D33C.buffer + 0x6000;
}

void ov74_02233F5C(void) {
    ov74_02233B04(OS_ARENA_MAIN);
}
