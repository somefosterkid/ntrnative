#include "global.h"
#include "overlay_08_battle_party.h"
struct BattleParty;

u8 ov08_0221D5D0(void *, s32 *);

s32 ov08_022217C8(void *);

u32 ov08_02224C94(void *);

extern s32 ov08_02224E54;

u8 ov08_0221D4B0(struct BattleParty *arg0);

u8 ov08_0221D4B0(BattleParty *arg0) {
    u32 v0;

    v0 = ov08_0221D5D0(arg0, &ov08_02224E54);
    if (v0 == -1U) {
        v0 = ov08_02224C94(*((void **)&arg0->cursor));
        if (v0 == -2U) {
            v0 = 3;
            goto error;
        }
        if (v0 == -1U) {
            return 0xFFU;
        }
        goto error;
    }
    ov08_022217C8(arg0);
error:
    return v0;
}

extern s32 ov08_02224E68;

u8 ov08_0221D4F8(struct BattleParty *arg0);

u8 ov08_0221D4F8(BattleParty *arg0) {
    u32 v0;

    v0 = ov08_0221D5D0(arg0, &ov08_02224E68);
    if (v0 == -1U) {
        v0 = ov08_02224C94(*((void **)&arg0->cursor));
        if (v0 == -2U) {
            v0 = 3;
            goto error;
        }
        if (v0 == -1U) {
            return 0xFFU;
        }
        goto error;
    }
    ov08_022217C8(arg0);
error:
    return v0;
}

extern s32 ov08_02224F5C;

u8 ov08_0221D540(struct BattleParty *arg0);

u8 ov08_0221D540(BattleParty *arg0) {
    u32 v0;

    v0 = ov08_0221D5D0(arg0, &ov08_02224F5C);
    if (v0 == -1U) {
        v0 = ov08_02224C94(*((void **)&arg0->cursor));
        if (v0 == -2U) {
            v0 = 7;
            goto error;
        }
        if (v0 == -1U) {
            return 0xFFU;
        }
        goto error;
    }
    ov08_022217C8(arg0);
error:
    return v0;
}

extern s32 ov08_02224E7C;

u8 ov08_0221D588(struct BattleParty *arg0);

u8 ov08_0221D588(BattleParty *arg0) {
    u32 v0;

    v0 = ov08_0221D5D0(arg0, &ov08_02224E7C);
    if (v0 == -1U) {
        v0 = ov08_02224C94(*((void **)&arg0->cursor));
        if (v0 == -2U) {
            v0 = 4;
            goto error;
        }
        if (v0 == -1U) {
            return 0xFFU;
        }
        goto error;
    }
    ov08_022217C8(arg0);
error:
    return v0;
}
