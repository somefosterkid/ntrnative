#include "global.h"

#include "follow_mon.h"

s32 ov64_021E6E30(s32 arg0, s32 arg1, s32 arg2);

s32 ov64_021E6E30(s32 species, s32 arg1, s32 arg2) {
    s32 v0;
    s32 v1;

    v1 = arg1;
    if ((species <= 0) || (species > 0x1ED)) {
        v0 = 1;
    } else {
        v0 = SpeciesToOverworldModelIndexOffset(species) + 0x129;
        if (OverworldModelLookupHasFemaleForm(species) != 0) {
            if (arg2 == 1) {
                v0 += 1;
            }
        } else {
            if (v1 > OverworldModelLookupFormCount(species)) {
                v1 = 0;
            }
            v0 += v1;
        }
    }
    return v0;
}
