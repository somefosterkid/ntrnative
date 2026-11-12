// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/demo/egg_demo.h: the lines in use so far.
#ifndef BWPORT_DEMO_EGG_DEMO_H
#define BWPORT_DEMO_EGG_DEMO_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "gfl/clact.h"
#include "gfl/bg_sys.h"

// The egg demo's BG: one word, zeroed by its init
typedef struct {
    u32 unk0;
} EggDemoBG;

// One BG of the egg demo: its number, its setup, its mode and whether it is enabled
typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} EggDemoBGSetup;

typedef struct EggDemoGraphic EggDemoGraphic;

// pokebw2's EggDemoBG_Init (BG system, BG table, heap): Black's func_ov215_021f651c
void func_ov215_021f651c(EggDemoBG *bg, HeapID heapId);

// pokebw2's EggDemoGraphic_Create
// egg_demo_graphic.c
EggDemoGraphic *func_ov215_021f63cc(u32 layout, HeapID heapId);

typedef struct EggDemoView EggDemoView;

// pokebw2's EggDemoView_Create
// egg_demo_view.c: the egg and the Pokémon
EggDemoView *func_ov215_021f6720(HeapID heapId, PartyPkm *pkm);
// pokebw2's EggDemoView_Free
void func_ov215_021f67b0(EggDemoView *view);
// pokebw2's EggDemoView_Draw
void func_ov215_021f6a24(EggDemoView *view);
// pokebw2's EggDemoView_ShowPokemon
// Shows the hatched Pokémon, first as white
void func_ov215_021f6a48(EggDemoView *view, PartyPkm *pkm);
// pokebw2's EggDemoView_Reveal
// Fades the Pokémon in from white, then plays its cry
void func_ov215_021f6a98(EggDemoView *view);
// pokebw2's EggDemoView_IsDone
BOOL func_ov215_021f6aa4(EggDemoView *view);

// Black 2's; twinned code confirms the offsets of pkm in Black
struct EggDemoParam {
    GameData *gameData;
    PartyPkm *pkm;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's EggDemoView_Update
void func_ov215_021f67e0(EggDemoView *view);
// pokebw2's EggDemoView_Start
// Starts the egg shaking until it hatches
void func_ov215_021f6a3c(EggDemoView *view);
// pokebw2's EggDemoView_IsHatched
BOOL func_ov215_021f6a44(EggDemoView *view);
// pokebw2's EggDemoView_IsWhite
BOOL func_ov215_021f6a84(EggDemoView *view);
// pokebw2's EggDemoGraphic_Free
void func_ov215_021f6474(EggDemoGraphic *graphic);
// pokebw2's EggDemoGraphic_GetClActUnit
ClActUnit *func_ov215_021f64fc(EggDemoGraphic *graphic);

// Black's func_ov215_021f64d0, func_ov215_021f64e4, func_ov215_021f64f0: the graphic's update and 3D begin and end
void func_ov215_021f64d0(EggDemoGraphic *graphic);
void func_ov215_021f64e4(EggDemoGraphic *graphic);
void func_ov215_021f64f0(EggDemoGraphic *graphic);

#endif // BWPORT_DEMO_EGG_DEMO_H
