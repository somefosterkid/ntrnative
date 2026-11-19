#include "global.h"

#include "gf_gfx_loader.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_0223B41C(void *, void *, void *, void *);

s32 ov70_0223B484(void *, void *, void *);

void ov70_0223B258(GTSApplicationState *arg0);

void ov70_0223B258(GTSApplicationState *arg0) {
    GfGfxLoader_LoadScrnData(NARC_a_1_0_0, 0x24, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0x600U, 1, HEAP_ID_61);
    ov70_0223B484(&arg0->infoWindows[23], arg0->trainerLocationStrings[0], arg0->trainerLocationStrings[1]);
    ov70_0223B41C(&arg0->infoWindows[28], arg0->gtsMessageLoader, arg0->speciesMessageLoader, &arg0->searchResults[arg0->selectedSearchResult].requirements);
}
