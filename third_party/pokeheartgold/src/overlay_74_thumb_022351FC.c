#include "global.h"

BOOL ov74_0223514C(void);
static BOOL ov74_022351FC(void);
int ov74_02235230(void);

static BOOL ov74_022351FC(void);
int ov74_02235230(void);
BOOL ov74_02235258(void *dest, int size);

static BOOL ov74_022351FC(void) {
    CTRDG_Init();

    if (CTRDG_IsAgbCartridge() == 0) {
        return 0;
    }

    if (CTRDG_GetAgbMakerCode() != 0x3130) {
        return 0;
    }

    if (ov74_0223514C() == 0) {
        return 0;
    }

    return 1;
}

int ov74_02235230(void) {
    u32 length;

    if (ov74_022351FC() == 0) {
        return 0;
    }

    CTRDG_Enable(1);
    CTRDG_Read32((void *)0x8100000, &length);
    CTRDG_Enable(0);

    return length;
}

BOOL ov74_02235258(void *dest, int size) {
    BOOL success;

    if (ov74_022351FC() == 0) {
        return 0;
    }

    if (size == 0) {
        size = ov74_02235230();
    }

    CTRDG_Enable(1);
    success = CTRDG_CpuCopy16((void *)0x8100000 + 0x100, dest, size);
    CTRDG_Enable(0);

    if (CTRDG_IsExisting() == 0) {
        return 0;
    }

    return success;
}
