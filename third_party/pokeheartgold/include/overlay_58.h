#ifndef POKEHEARTGOLD_OVY_58_H
#define POKEHEARTGOLD_OVY_58_H

#include "menu_input_state.h"
#include "overlay_manager.h"
#include "save.h"

typedef struct ApricornBoxArgs {
    u8 unk0;
    u8 unk1[0x7];
    u16 *unk8;
    u32 unkC;
    u32 steps;
    MenuInputStateMgr *menuInputStatePtr;
    SaveData *saveData;
} ApricornBoxArgs;

struct PartyMenuArgs;

// the ov58 Apricorn box parent object (OverlayManager data, DS size 0x20); ov59 runs as its child
typedef struct Ov58ApricornBoxWork {
    enum HeapID heapId;                  // 0x00
    ApricornBoxArgs *args;               // 0x04
    OverlayManager *childMan;            // 0x08
    u8 unk_0C[4];
    struct PartyMenuArgs *partyMenuArgs; // 0x10
    u32 unk_14;
    u8 unk_18;
    u8 unk_19;
    u8 unk_1A;
    u8 unk_1B;
    u8 unk_1C;
    u8 unk_1D[3];
} Ov58ApricornBoxWork;

BOOL ApricornBox_Init(OverlayManager *man, int *state);
BOOL ApricornBox_Main(OverlayManager *man, int *state);
BOOL ApricornBox_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_OVY_58_H
