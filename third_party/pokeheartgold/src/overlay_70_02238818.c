#include "global.h"

#include "overlay_manager.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238E50(void *, s32, s32);

void ov70_02238818(GTSApplicationState *arg0, OverlayManager *arg1);

void ov70_02238880(void);

void ov70_02238818(GTSApplicationState *arg0, OverlayManager *man) {
    *(void **)arg0 = OverlayManager_GetArgs(man);
    arg0->screenId = 0;
    ov70_02238E50(arg0, 0, 0);
    arg0->mainMenuSelectedOption = 0;
    arg0->searchRequirements.species = 0;
    arg0->searchRequirements.gender = 3;
    arg0->searchRequirements.level = 0;
    arg0->searchRequirements.level2 = 0;
    arg0->submittedRequirements.species = 0;
    arg0->hasAvatarFinishedMoving = 0;
    arg0->selectedBoxId = 0x12;
    arg0->deferredBoxPokemonCount = 0;
    arg0->deferredBoxId = 0;
    arg0->searchResultsVisible = 0;
    arg0->unk_11BC = NULL;
    arg0->selectedCountryIndex = 0;
}

void ov70_0223887C(void);

void ov70_0223887C(void) {
}

void ov70_02238880(void) {
    GXS_SetVisibleWnd(0x00);
    GX_SetVisibleWnd(0x01);
    G2_SetWnd0InsidePlane(0x01 | 0x02 | 0x04 | 0x08 | 0x10, 0);
    G2_SetWndOutsidePlane(0x01 | 0x02 | 0x04 | 0x08 | 0x10, 1);
    G2_SetWnd0Position(256 - 16, 0, 255, 16);
}
