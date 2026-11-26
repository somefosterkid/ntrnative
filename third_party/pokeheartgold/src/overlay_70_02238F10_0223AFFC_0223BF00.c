#include "global.h"

#include "unk_02005D10.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238D84(void *, s32, s32);

s32 ov70_0223CAC4(void *, s32, s32, s32, s32);

s32 ov70_0223CC68(GTSPokemonRequirements *, GTSPokemonRequirements *, s32, s32);

s32 ov70_02241234(void *);

s32 ov70_0223BF00(GTSApplicationState *arg0);

s32 ov70_0223BF00(GTSApplicationState *arg0) {
    if (arg0->searchRequirements.species == 0) {
        ov70_0223CAC4(arg0, 0xC, 1, 0, 0xF0F);
        ov70_02238D84(arg0, 0x15, 1);
        PlaySE(0x5F3U);
    } else if (ov70_0223CC68(&arg0->searchRequirements, &arg0->submittedRequirements, arg0->selectedCountryIndex, arg0->submittedCountryIndex) != 0) {
        ov70_0223CAC4(arg0, 0x21, 1, 0, 0xF0F);
        ov70_02238D84(arg0, 0x15, 1);
        PlaySE(0x5F3U);
    } else {
        PlaySE(0x5FEU);
        ov70_0223CAC4(arg0, 0xD, 1, 0, 0xF0F);
        ov70_02238D84(arg0, 0x15, 0x10);
        ov70_02241234(arg0);
    }
    return 3;
}
