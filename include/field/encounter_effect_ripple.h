// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/encounter_effect_ripple.h: the lines
// in use so far.
#ifndef BWPORT_FIELD_ENCOUNTER_EFFECT_RIPPLE_H
#define BWPORT_FIELD_ENCOUNTER_EFFECT_RIPPLE_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// The mesh's vertices, one more than its quads in each direction
#define ENCEFF_RIPPLE_COLUMNS 33

#define ENCEFF_RIPPLE_ROWS 25
#define ENCEFF_RIPPLE_VERTICES (ENCEFF_RIPPLE_COLUMNS * ENCEFF_RIPPLE_ROWS)

// The effect's work area, 0x19e0 bytes
struct EncEffRipple {
    // z is the height of the water at the vertex
    VecFx16 vertices[ENCEFF_RIPPLE_VERTICES];
    // How fast each vertex's height changes
    s16 speeds[ENCEFF_RIPPLE_VERTICES];
    // Frames until the next drop, and then until the effect ends
    s16 timer;
    s16 drops;
    u16 dropInterval;
    // Frames to wait after the last drop
    u16 settleTime;
    // The height a drop sets the middle vertex to
    fx32 dropHeight;
    // The screens the fade goes to white or black on (FADE_ENGINE_*)
    u32 fadeMode;
    // Frames before the fade starts
    s32 fadeDelay;
    BOOL fadeStarted;
};

// pokebw2's EncEffRipple_CreateWhite
// Two white flashes, then drops that ripple the screen as it fades to white
GameEvent *func_ov078_021f5620(GameSystem *gsys, Field *field);
// pokebw2's EncEffRipple_ClearAndDraw
void func_ov078_021f57d4(EncEff *effect);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's EncEffRipple_CreateBlack
// Quicker and stronger drops, without the flashes, as the screen fades to black
GameEvent *func_ov078_021f565c(GameSystem *gsys, Field *field);

#endif // BWPORT_FIELD_ENCOUNTER_EFFECT_RIPPLE_H
