#ifndef BWPORT_FIELD_PASS_POWER_CHECK_H
#define BWPORT_FIELD_PASS_POWER_CHECK_H

// Overlay 10's pass_power_check.c (pokebw2's name, a descriptive one): whether a pass power is unlocked, nearly or
// not, by the Entralink levels and the special pass powers' flags, and how many are

#include "types.h"
#include "struct_decls.h"
#include "save/high_link.h"

enum {
    PASS_POWER_UNLOCKED,
    PASS_POWER_NEAR,
    PASS_POWER_LOCKED,
};

// CheckPassPowerUnlocked
u32 func_ov010_0216fe2c(const PassPowerData *table, int index, PassPowerLevel *have, u8 *flags);
// GetUnlockedPassPowerCount
u32 func_ov010_0216feac(const PassPowerData *table, PassPowerLevel *levels, u8 *flags);

#endif // BWPORT_FIELD_PASS_POWER_CHECK_H
