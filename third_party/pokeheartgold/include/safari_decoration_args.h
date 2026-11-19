#ifndef POKEHEARTGOLD_SAFARI_DECORATION_ARGS_H
#define POKEHEARTGOLD_SAFARI_DECORATION_ARGS_H

#include "menu_input_state.h"
#include "save_layout.h"
#include "save.h"

typedef struct SafariDecorationTarget {
    u8 valid;
    u8 x;
    u8 unk_2; // not set
    u8 y;
} SafariDecorationTarget;

typedef struct SafariDecorationArgs {
    SaveData *saveData;
    MenuInputStateMgr *menuInputStatePtr; // points to fieldSystem->menuInputState (0x10C)
    SafariDecorationTarget targets[4];    // 0x08
    u8 side;                              // 0x18
    u8 areaIndex;                         // 0x19
    u8 unk1A;
    u8 unk1B;
    u16 unk1C; // known from what's decompiled
    u8 *unk20; // points to fieldSystem->unk_111
} SafariDecorationArgs;

#ifndef SDK_PORT
LAYOUT_CHECK(SafariDecorationArgs_size, sizeof(SafariDecorationArgs) == 0x24);
#endif

#endif // POKEHEARTGOLD_SAFARI_DECORATION_ARGS_H
