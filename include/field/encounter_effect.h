// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/encounter_effect.h, the lines in use
// so far.
#ifndef BWPORT_FIELD_ENCOUNTER_EFFECT_H
#define BWPORT_FIELD_ENCOUNTER_EFFECT_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// Overlay 77 runs the encounter effects that split the screen into a grid of cells, which overlays 79 to 81 set up.
// Overlay 75 runs the event around such an effect. pokebw2's guessed names are in the comments

// A cell of the screen, 0x40 bytes
struct EncEffCell {
    VecFx32 pos;
    u8 unk0C[0xc];
    u32 unk18;
    s32 offset;
    u32 unk20;
    // Added to pos.x each frame
    fx32 speed;
    u8 unk28[0x8];
    s32 frame;
    BOOL visible;
    BOOL done;
    // Called for each visible cell, until it returns TRUE
    BOOL (*update)(EncEffCell *cell);
};

struct EncEffGrid {
    EncEffCell cells[192];
    // For the grid's update function
    s32 work[2];
    u16 columns;
    u16 rows;
    u8 unk300C[0x64];
    void (*update)(EncEffGrid *grid);
    u32 unk3074;
    u32 unk3078;
};

struct EncEffGridParam {
    // The size of a cell, in tiles
    u32 cellWidth;
    u32 cellHeight;
    void (*init)(EncEffGrid *grid);
    void (*update)(EncEffGrid *grid);
    BOOL (*cellUpdate)(EncEffCell *cell);
};

void *Field_Get3DCi(Field *field);
GameEvent *EventFieldEffect_CreatePokeSprite(GameSystem *gsys, void *g3DCi, u8 partySlot);
// Loads overlays 75 and 77 (pokebw2's func_ov036_021c5ea0)
void func_ov021_021cf580(EncEff *effect);
void *EncEff_AllocWorkArea(EncEff *effect, u32 size, HeapID heapId);
void *EncEff_GetWorkArea(EncEff *effect);
void *EncEff_GetEventData(EncEff *effect);
// EncEffCapture_CreateFlashEvent: the event of an effect, where init makes the effect's event and render draws it
GameEvent *func_ov075_021f5260(GameSystem *gsys, const VecFx32 *pos, GameEvent *(*init)(GameSystem *gsys),
                               void (*render)(EncEffGrid *grid));
// EncEffCapture_Draw
void func_ov075_021f52d8(void *data);
// EncEffGrid_CreateEvent: mode picks the fade at the end, to white or to black
GameEvent *func_ov077_021f5620(GameSystem *gsys, const EncEffGridParam *heapId, u32 mode);
// EncEffGrid_ClearAndDraw: clears the screen to the grid's color, then draws the effect with overlay 75
void func_ov077_021f57f8(EncEff *effect);
// EncEffGrid_Draw
void func_ov077_021f582c(EncEffGrid *grid);

GameEvent *func_ov079_021f5a80(GameSystem *gsys, Field *field);
GameEvent *func_ov080_021f5a80(GameSystem *gsys, Field *field);

// struct EncEff is enceff.c's own (as in pokebw2)

// An encounter effect of enceff_data.c's table (pokebw2's EncEffInfo, ENC_EFFECT_CONTROLLERS)
typedef struct EncEffEntry {
    GameEvent *(*create)();
    void (*update)(EncEff *effect);
    u32 overlayId;
    u32 param;
    // The most memory the effect needs
    u32 heapSize;
} EncEffEntry;

extern const EncEffEntry data_ov021_021db4fc[37];

#endif // BWPORT_FIELD_ENCOUNTER_EFFECT_H
