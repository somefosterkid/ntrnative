#include "global.h"

#include "pokemon.h"
#include "render_window.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238E50(void *, s32, s32);

s32 ov70_0223EDE4(void *, s32);

s32 ov70_0223F3D8(void *, void *, void *, void *, s32, s32, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov70_0223F960(void *, s32, s16, s16);
#else
s32 ov70_0223F960(void *, s32, u16, u16);
#endif

s16 ov70_02242144(void *);

s32 ov70_0223EE10(GTSApplicationState *arg0);

s32 ov70_02238D84(GTSApplicationState *, s32, s32);
s32 ov70_0223E8E8(GTSApplicationState *, s32, s32, s32, s32);
s32 ov70_0223EEE8(GTSApplicationState *arg0);

s32 ov70_0223EE10(GTSApplicationState *arg0) {
    s16 species;
    s8 v2;
    GTSSpeciesMenuState *v0;
    GTSSpeciesMenuState *v1;

    species = ov70_02242144(arg0->unk_11A8);
    switch (species) {
    case -1:
        break;

    case -2:
        ClearFrameAndWindow2(&arg0->bottomInstructionWindow, 0);
        ov70_02238E50(arg0, 5, 5);
        arg0->currentScreenInstruction = 2;
        v0 = arg0->speciesMenuState;
        ov70_0223F960(&arg0->charpadScrollState, (v0->charpadScrollPos) + (v0->charpadCursorPos), v0->speciesListScrollPos, v0->speciesListCursorPos);
        break;

    default:
        arg0->depositRequirements.species = species;
        arg0->speciesMenuState->selectedSpeciesGenderRatio = GetMonBaseStat((s32)species, 0x12);
        if (ov70_0223EDE4(&arg0->depositRequirements, arg0->speciesMenuState->selectedSpeciesGenderRatio) != 0) {
            arg0->currentScreenInstruction = 0xA;
            v2 = arg0->depositRequirements.gender;
        } else {
            arg0->currentScreenInstruction = 7;
            v2 = 3;
        }
        ov70_0223F3D8(arg0->gtsMessageLoader, arg0->speciesMessageLoader, arg0->stringTemplate, &arg0->infoWindows[13], (s32)(arg0->depositRequirements.species), (s32)v2, -1);
        v1 = arg0->speciesMenuState;
        ov70_0223F960(&arg0->charpadScrollState, v1->charpadScrollPos + v1->charpadCursorPos, v1->speciesListScrollPos, v1->speciesListCursorPos);
        break;
    }

    return 3;
}

s32 ov70_0223EEE8(GTSApplicationState *arg0) {
    ov70_0223E8E8(arg0, 0xA, 1, 0, 0xF0F);
    ov70_02238D84(arg0, 0x10, 8);
    return 3;
}
