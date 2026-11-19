#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"
#include "overlay_02_hmcut_in.h"

struct HMCutIn;

void ov02_02249FD4(HMCutIn *cutIn);

BOOL ov02_02249940(HMCutIn *cutIn);

BOOL ov02_02249940(HMCutIn *cutIn) {
    ov02_02249FD4(cutIn);
    cutIn->updateSprites = FALSE;
    cutIn->state++;
    return FALSE;
}

void ov02_0224A028(HMCutIn *cutIn);

BOOL ov02_02249954(HMCutIn *cutIn);

BOOL ov02_02249954(HMCutIn *cutIn) {
    ov02_0224A028(cutIn);
    cutIn->updateSprites = FALSE;
    cutIn->state++;
    return FALSE;
}
