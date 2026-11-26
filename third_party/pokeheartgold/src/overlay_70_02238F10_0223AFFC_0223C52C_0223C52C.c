#include "global.h"

#include "bg_window.h"
#include "pokemon.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_0223CD28(s32);

s32 ov70_0223EDE4(void *, s32);

s32 ov70_0223F1D8(void *, void *, s32, s32, s32, s32);

s32 ov70_0223F2BC(void *, void *, int, s32, s32, s32, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov70_0223F960(void *, s32, s16, s16);
#else
s32 ov70_0223F960(void *, s32, u16, u16);
#endif

s16 ov70_02242144(void *);

s32 ov70_0223C52C(GTSApplicationState *arg0);

s32 ov70_0223C52C(GTSApplicationState *arg0) {
    short temp_r0;
    GTSSpeciesMenuState *temp_r3;
    GTSSpeciesMenuState *temp_r3_2;
    temp_r0 = ov70_02242144(arg0->unk_11A8);
    switch (temp_r0) {
    case -1:
        break;

    case -2:
        ov70_0223CD28(0);
        arg0->currentScreenInstruction = 0;
        temp_r3 = arg0->speciesMenuState;
        ov70_0223F960(&arg0->charpadScrollState, temp_r3->charpadScrollPos + temp_r3->charpadCursorPos, temp_r3->speciesListScrollPos, temp_r3->speciesListCursorPos);
        break;

    default:
        ov70_0223CD28(0);
        arg0->searchRequirements.species = temp_r0;
        arg0->currentScreenInstruction = 0;
        FillWindowPixelBuffer(&arg0->infoWindows[14], 0U);
        ov70_0223F1D8(&arg0->infoWindows[14], arg0->speciesMessageLoader, temp_r0, 0, 0, 0x10200);
        arg0->speciesMenuState->selectedSpeciesGenderRatio = GetMonBaseStat((s32)temp_r0, 0x12);
        temp_r3_2 = arg0->speciesMenuState;
        ov70_0223F960(&arg0->charpadScrollState, temp_r3_2->charpadScrollPos + temp_r3_2->charpadCursorPos, temp_r3_2->speciesListScrollPos, temp_r3_2->speciesListCursorPos);
        if (ov70_0223EDE4(&arg0->searchRequirements, arg0->speciesMenuState->selectedSpeciesGenderRatio) != 0) {
            FillWindowPixelBuffer(&arg0->infoWindows[16], 0U);
            ov70_0223F2BC(&arg0->infoWindows[16], arg0->gtsMessageLoader, arg0->searchRequirements.gender, 1, 0, 0, 0x10200);
        }
        break;
    }

    return 3;
}
