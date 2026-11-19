#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"
#include "overlay_02_hmcut_in.h"

struct HMCutIn;

BOOL ov02_02249838(HMCutIn *cutIn);

BOOL ov02_02249838(HMCutIn *cutIn) {
    cutIn->subState++;

    if (cutIn->subState >= 8) {
        cutIn->subState = 0;
        cutIn->pokemonSpriteDeltaX = (FX32_ONE * -1);
        cutIn->state++;
    }

    return FALSE;
}
