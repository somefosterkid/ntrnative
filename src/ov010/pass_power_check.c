// Black's code at ov010 0x0216fe2c-0x0216fed8. pokebw2's ov012 pass_power_check.c follows g3d_text_draw.c there;
// Black's two functions are its CheckPassPowerUnlocked and GetUnlockedPassPowerCount (0xac bytes, as B2's)

#include "types.h"
#include "field/pass_power_check.h"
#include "save/high_link.h"

// The levels a pass power needs, or one of these
#define PASS_POWER_LEVEL_NONE 0xff
#define PASS_POWER_SPECIAL 0xfe

// Black has 48 pass powers. The loop over them tests its constant bound before the first pass, which MWCC only keeps
// for a counter of an enum type, so the pass powers were numbered by an enum (as in pokebw2's)
typedef enum {
    PASS_POWER_ID_FIRST,
    PASS_POWER_ID_COUNT = 0x30,
} PassPowerId;


u32 func_ov010_0216fe2c(const PassPowerData *table, int index, PassPowerLevel *have, u8 *flags) {
    int level1 = table[index].level1;
    u32 unlocked;

    if (level1 == PASS_POWER_LEVEL_NONE) {
        return PASS_POWER_LOCKED;
    }
    if (level1 == PASS_POWER_SPECIAL) {
        unlocked = flags[0] | (flags[1] << 8);
        if (unlocked & (1 << (index - PASS_POWER_SPECIAL_FIRST))) {
            return PASS_POWER_UNLOCKED;
        }
        return PASS_POWER_LOCKED;
    }
    if (level1 <= have->level1) {
        if (table[index].level2 <= have->level2) {
            return PASS_POWER_UNLOCKED;
        }
        if (table[index].level2 - have->level2 <= 3) {
            return PASS_POWER_NEAR;
        }
    } else if (table[index].level2 <= have->level2) {
        if (level1 <= have->level1) {
            return PASS_POWER_UNLOCKED;
        }
        if (level1 - have->level1 <= 3) {
            return PASS_POWER_NEAR;
        }
    }
    return PASS_POWER_LOCKED;
}

u32 func_ov010_0216feac(const PassPowerData *table, PassPowerLevel *levels, u8 *flags) {
    PassPowerId i;
    u32 count = 0;

    for (i = PASS_POWER_ID_FIRST; i < PASS_POWER_ID_COUNT; i++) {
        if (func_ov010_0216fe2c(table, i, levels, flags) == PASS_POWER_UNLOCKED) {
            count++;
        }
    }
    return count;
}


