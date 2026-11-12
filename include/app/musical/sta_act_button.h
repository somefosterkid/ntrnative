// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/sta_act_button.h: the lines in
// use so far.
#ifndef BWPORT_APP_MUSICAL_STA_ACT_BUTTON_H
#define BWPORT_APP_MUSICAL_STA_ACT_BUTTON_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// What StaActButton.selected holds when no button was touched
#define STA_ACT_BUTTON_NONE 2

// Black 2's; twinned code confirms the offsets of active, clactUnit, palettes, chars, cellAnims, itemIds, buttons,
// items and effect in Black
struct StaActButton {
    HeapID heapId;
    StaActing *stage;
    MusicalPoke *poke;
    // Whether the buttons can be touched
    BOOL active;
    // Whether a prop was touched and its use not yet started
    BOOL pressed;
    u32 selected;
    ClActUnit *clactUnit;
    // The button's, the prop's and the touch effect's resources
    u32 palettes[3];
    u32 chars[3];
    u32 cellAnims[3];
    // Whether each hand's prop was used
    BOOL used[2];
    u16 itemIds[2];
    ClActor *buttons[2];
    ClActor *items[2];
    ClActor *effect;
};

// pokebw2's StaActButton_TermSystem
void func_ov132_021fa64c(StaActButton *sys);
// pokebw2's StaActButton_SetShowFlg
void func_ov132_021faab8(StaActButton *sys, BOOL show);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's StaActButton_InitSystem
StaActButton *func_ov132_021fa5b0(HeapID heapId, StaActing *stage, MusicalPoke *poke);
// pokebw2's StaActButton_UpdateSystem
void func_ov132_021fa6cc(StaActButton *sys);

#endif // BWPORT_APP_MUSICAL_STA_ACT_BUTTON_H
