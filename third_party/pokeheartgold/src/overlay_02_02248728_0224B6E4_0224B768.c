#include "global.h"

#include "overlay_02_hmcut_in.h"
#include "unk_020689C8.h"

void ov02_0224B768(HMCutIn *cutIn);

void ov02_0224B768(HMCutIn *cutIn) {
    OverworldAnimManager *a0;

    a0 = cutIn->unk_1F4;
    if (a0 != 0) {
        sub_02068B48(a0);
        cutIn->unk_1F4 = 0;
    }
}
