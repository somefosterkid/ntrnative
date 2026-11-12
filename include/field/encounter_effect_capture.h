// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/encounter_effect_capture.h: the lines
// in use so far.
#ifndef BWPORT_FIELD_ENCOUNTER_EFFECT_CAPTURE_H
#define BWPORT_FIELD_ENCOUNTER_EFFECT_CAPTURE_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "nnsys/gfd.h"
#include "struct_decls.h"

// The event's data, which the effects' render functions get back from EncEff_GetEventData
struct EncEffCaptureWork {
    // How many times the screen has flashed white
    s32 flashes;
    u32 texBanks;
    // Whether bank D was added to the texture banks, to be removed again at the end
    BOOL addedTexBank;
    u32 renderMode;
    // Set by the V-blank task once the capture is on
    BOOL captured;
    // Where the captured screen is in the texture VRAM
    NNSGfdTexKey texKey;
    VecFx32 camPos;
    // The effect's work area
    void *effectWork;
    GameEvent *(*init)(GameSystem *gsys);
    void (*render)(EncEffGrid *grid);
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's EncEffCapture_CreateEvent
// The event of an effect without the flashes: init makes the effect's event, and render draws it. The event with two
// white flashes first, EncEffCapture_CreateFlashEvent, and the draw function, EncEffCapture_Draw, are declared in
// field/encounter_effect.h
GameEvent *func_ov075_021f529c(GameSystem *gsys, const VecFx32 *pos, GameEvent *(*init)(GameSystem *gsys),
                               void (*render)(EncEffGrid *grid));

#endif // BWPORT_FIELD_ENCOUNTER_EFFECT_CAPTURE_H
