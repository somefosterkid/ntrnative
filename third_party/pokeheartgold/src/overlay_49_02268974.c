#include "global.h"

#include "error_handling.h"
#include "ppw_lobby.h"

u32 ov49_02268974(PPW_LOBBY_TIME_EVENT param0);

void ov49_02268A00(void);

u32 ov49_02268974(PPW_LOBBY_TIME_EVENT param0) {
    switch (param0) {
    case PPW_LOBBY_TIME_EVENT_NEON_A0:
    case PPW_LOBBY_TIME_EVENT_NEON_A1:
    case PPW_LOBBY_TIME_EVENT_NEON_A2:
    case PPW_LOBBY_TIME_EVENT_NEON_A3:
    case PPW_LOBBY_TIME_EVENT_NEON_A4:
    case PPW_LOBBY_TIME_EVENT_NEON_A5:
        return param0 - PPW_LOBBY_TIME_EVENT_NEON_A0;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    return 0;
}

u32 ov49_022689A0(PPW_LOBBY_TIME_EVENT param0);

u32 ov49_022689A0(PPW_LOBBY_TIME_EVENT param0) {
    switch (param0) {
    case PPW_LOBBY_TIME_EVENT_NEON_B0:
    case PPW_LOBBY_TIME_EVENT_NEON_B1:
    case PPW_LOBBY_TIME_EVENT_NEON_B2:
    case PPW_LOBBY_TIME_EVENT_NEON_B3:
        return param0 - PPW_LOBBY_TIME_EVENT_NEON_B0;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    return 0;
}

u32 ov49_022689D4(PPW_LOBBY_TIME_EVENT param0);

u32 ov49_022689D4(PPW_LOBBY_TIME_EVENT param0) {
    switch (param0) {
    case PPW_LOBBY_TIME_EVENT_NEON_C0:
    case PPW_LOBBY_TIME_EVENT_NEON_C1:
    case PPW_LOBBY_TIME_EVENT_NEON_C2:
    case PPW_LOBBY_TIME_EVENT_NEON_C3:
        return param0 - PPW_LOBBY_TIME_EVENT_NEON_C0;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    return 0;
}

void ov49_02268A00(void) {
    NNS_G3dGlbLightColor(GX_LIGHTID_2, 0U);
}
