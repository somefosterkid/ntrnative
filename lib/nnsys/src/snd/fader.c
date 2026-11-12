// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nnsys/src/snd/snd_fader.c.
#include "snd_internal.h"

// NitroSystem's faders (NNSi_Snd), which the players, the capture and the streams move their volumes with. The SDK's
// file is fader.c, and the names are NitroSystem's

void NNSi_SndFaderInit(NNSSndFader *fader) {
    fader->origin = fader->target = 0;
    fader->counter = fader->frame = 0;
}

void NNSi_SndFaderSet(NNSSndFader *fader, int target, int frame) {
    fader->origin = NNSi_SndFaderGet(fader);
    fader->target = target;
    fader->frame = frame;
    fader->counter = 0;
}

int NNSi_SndFaderGet(const NNSSndFader *fader) {
    if (fader->counter >= fader->frame) {
        return fader->target;
    }
    return fader->origin + (fader->target - fader->origin) * fader->counter / fader->frame;
}

void NNSi_SndFaderUpdate(NNSSndFader *fader) {
    if (fader->counter < fader->frame) {
        fader->counter++;
    }
}

BOOL NNSi_SndFaderIsFinished(const NNSSndFader *fader) {
    return fader->counter >= fader->frame;
}
