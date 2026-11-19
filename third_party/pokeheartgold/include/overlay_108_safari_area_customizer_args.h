#ifndef POKEHEARTGOLD_OVERLAY_108_SAFARI_AREA_CUSTOMIZER_ARGS_H
#define POKEHEARTGOLD_OVERLAY_108_SAFARI_AREA_CUSTOMIZER_ARGS_H

#include "menu_input_state.h"
#include "save.h"

typedef struct SafariAreaCustomizerArgs {
    SaveData *saveData;
    MenuInputStateMgr *menuInputStatePtr;
    u8 unk8[0x11];
    u8 unk19;
    u8 unk1A[0x2];
    s16 unk1C;
    u8 unk1E[0x2];
    u8 *unk20;
} SafariAreaCustomizerArgs;

#endif // POKEHEARTGOLD_OVERLAY_108_SAFARI_AREA_CUSTOMIZER_ARGS_H
