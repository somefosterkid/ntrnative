// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/demo/shinka_demo.h, the lines in use so far.
// The proc table's name, SHINKA_DEMO_PROC_FUNCTIONS, is pokebw2's guess.
#ifndef BWPORT_DEMO_SHINKA_DEMO_H
#define BWPORT_DEMO_SHINKA_DEMO_H

#include "types.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "struct_decls.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "system/mcss.h"

// The evolution demo
#define OVERLAY_SHINKA_DEMO OVERLAY_ID(195)

typedef struct {
    GameData *gameData;
    PokeParty *party;
    // The species to evolve into
    u16 species;
    u8 partyIndex;
    // EVO_METHOD_*
    u8 method;
    // Set, the demo pushes and brings back the music that was playing
    u32 unkC;
    // Whether B stops the evolution. The demo clears it for an evolution by item
    BOOL canCancel;
} ShinkaDemoParam;

extern const GameProcFunctions data_ov195_02208c40;

typedef struct ShinkaDemoGraphic ShinkaDemoGraphic;

// pokebw2's ShinkaDemoGraphic_Create
// shinka_demo_graphic.c
ShinkaDemoGraphic *func_ov195_022058f8(u32 layout, HeapID heapId);
// pokebw2's ShinkaDemoGraphic_InitSubBG
// Creates and frees the BGs of the sub screen
void func_ov195_02205a34(ShinkaDemoGraphic *graphic);
// pokebw2's ShinkaDemoGraphic_FreeSubBG
void func_ov195_02205a84(ShinkaDemoGraphic *graphic);

typedef struct ShinkaDemoEffect ShinkaDemoEffect;

// pokebw2's ShinkaDemoEffect_Create
// shinka_demo_effect.c: the particles and 3D model around the evolving Pokémon
ShinkaDemoEffect *func_ov195_0220871c(HeapID heapId, BOOL played);
// pokebw2's ShinkaDemoEffect_Free
void func_ov195_02208770(ShinkaDemoEffect *effect);
// pokebw2's ShinkaDemoEffect_Update
void func_ov195_0220878c(ShinkaDemoEffect *effect);
// pokebw2's ShinkaDemoEffect_Draw
void func_ov195_02208804(ShinkaDemoEffect *effect);
// pokebw2's ShinkaDemoEffect_Cancel
void func_ov195_02208814(ShinkaDemoEffect *effect);
// pokebw2's ShinkaDemoEffect_Start
// Starts the particles
void func_ov195_0220882c(ShinkaDemoEffect *effect);
// pokebw2's ShinkaDemoEffect_Start3D
// Plays the 3D model once the particles have started
void func_ov195_02208844(ShinkaDemoEffect *effect);
// pokebw2's ShinkaDemoEffect_Is3DHeld
BOOL func_ov195_02208850(ShinkaDemoEffect *effect);
// pokebw2's ShinkaDemoEffect_Is3DReversing
BOOL func_ov195_02208860(ShinkaDemoEffect *effect);

typedef struct ShinkaDemoView ShinkaDemoView;

// pokebw2's ShinkaDemoView_Cancel
// Stops the evolution while the pieces are moving, and returns whether it did
BOOL func_ov195_02206078(ShinkaDemoView *view);
// pokebw2's ShinkaDemoView_Update
void func_ov195_02205d50(ShinkaDemoView *view);
// pokebw2's ShinkaDemoView_Draw
void func_ov195_0220602c(ShinkaDemoView *view);
// pokebw2's ShinkaDemoView_Create
// shinka_demo_view.c: the evolving Pokémon. played creates it after the evolution has been shown, with the evolved
// Pokémon
ShinkaDemoView *func_ov195_02205c74(HeapID heapId, BOOL played, PartyPkm *pkm, u16 species);
// pokebw2's ShinkaDemoView_Free
void func_ov195_02205d28(ShinkaDemoView *view);

// pokebw2's ShinkaDemoView_Evolve
// Turns the sprite white and breaks it into pieces
void func_ov195_02206050(ShinkaDemoView *view);
// pokebw2's ShinkaDemoView_Reveal
// Shows the evolved sprite, still white
void func_ov195_0220605c(ShinkaDemoView *view);
// pokebw2's ShinkaDemoView_FadeIn
// Fades the sprite in from white, then plays its cry
void func_ov195_02206064(ShinkaDemoView *view);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ShinkaDemoGraphic_Free
void func_ov195_022059a0(ShinkaDemoGraphic *graphic);
// pokebw2's ShinkaDemoGraphic_Update
void func_ov195_022059fc(ShinkaDemoGraphic *graphic);
// pokebw2's ShinkaDemoGraphic_Begin3D
void func_ov195_02205a10(ShinkaDemoGraphic *graphic);
// pokebw2's ShinkaDemoGraphic_End3D
void func_ov195_02205a1c(ShinkaDemoGraphic *graphic);
// pokebw2's ShinkaDemoGraphic_GetClActUnit
ClActUnit *func_ov195_02205a28(ShinkaDemoGraphic *graphic);
// pokebw2's ShinkaDemoView_Start
// Plays the Pokémon's cry
void func_ov195_02206044(ShinkaDemoView *view);
// pokebw2's ShinkaDemoView_IsCryDone
BOOL func_ov195_0220604c(ShinkaDemoView *view);
// pokebw2's ShinkaDemoView_HavePiecesReturned
BOOL func_ov195_02206058(ShinkaDemoView *view);
// pokebw2's ShinkaDemoView_IsDone
BOOL func_ov195_0220606c(ShinkaDemoView *view);
// pokebw2's ShinkaDemoView_HavePiecesStarted
BOOL func_ov195_02206070(ShinkaDemoView *view);
// pokebw2's ShinkaDemoView_GetUnk3C
// Set with HavePiecesReturned
BOOL func_ov195_02206074(ShinkaDemoView *view);

#define PIECE_ROWS 18
#define PIECE_COLUMNS 18

// Black 2's, not checked against Black
typedef struct {
    s32 unk0;
    // The texture's left, right, top and bottom
    fx32 s0;
    fx32 s1;
    fx32 t0;
    fx32 t1;
    u8 alpha;
    VecFx16 vertices[4];
    u32 frame;
    // Where the piece is in the sprite
    VecFx32 home;
    VecFx32 velocity;
    // The piece's place in the helix: its angle from the helix's, its distance from the helix's axis and its height
    s32 angle;
    fx32 radius;
    fx32 height;
    VecFx32 target;
    s32 state;
    VecFx32 pos;
    VecFx32 prevPos;
} SpritePiece;

// Black 2's, not checked against Black
typedef struct {
    NNSG2dCharacterData *character;
    NNSG2dPaletteData *palette;
    void *characterFile;
    void *paletteFile;
    GFLBitmap *bitmap;
    u32 texAddr;
    u32 plttAddr;
    VecFx32 pos;
    s32 state;
    GXRgb colors[16];
    // The palette fades toward fadeColor, from fadeStart to fadeEnd out of 31 by fadeStep every fadeWait frames
    GXRgb fadeColor;
    s16 fadeStart;
    s16 fadeEnd;
    s16 fadeWait;
    s16 fadeStep;
    s16 fadeValue;
    s16 fadeCounter;
    // The next pieces to hide while the helix spins, going back and forward from the middle of the sprite
    s16 hideColumnBack;
    s16 hideRowBack;
    s16 hideColumn;
    s16 hideRow;
    s32 moveState;
    s32 angle;
    s32 angleSpeed;
    u32 frame;
    SpritePiece pieces[PIECE_ROWS][PIECE_COLUMNS];
    // The rows that are drawn as pieces
    u8 rowCount;
} SpritePieces;

// Black 2's, not checked against Black
// The rows of the sprite that have not broken into pieces, as one quad
typedef struct {
    u32 texAddr;
    u32 plttAddr;
    VecFx32 pos;
    VecFx32 scale;
    u32 polygonId;
    fx32 s0;
    fx32 s1;
    fx32 t0;
    fx32 t1;
    u8 alpha;
    VecFx16 vertices[4];
    u8 rowCount;
} SpriteRest;

// Black 2's, not checked against Black
typedef struct {
    s32 state;
    G3DCamera *camera;
    // The old and new forms
    SpritePieces *pieces[2];
    SpriteRest *rest;
} PiecesManager;

// Black 2's, not checked against Black
struct ShinkaDemoView {
    HeapID heapId;
    PartyPkm *pkm;
    BOOL played;
    u16 species;
    // A copy of the Pokémon that has evolved
    PartyPkm *evolved;
    s32 state;
    u32 frame;
    BOOL started;
    BOOL cryDone;
    BOOL evolveRequested;
    BOOL piecesReturned;
    BOOL revealRequested;
    BOOL fadeInRequested;
    BOOL done;
    BOOL piecesStarted;
    // Set with piecesReturned
    BOOL unk3C;
    BOOL cancelled;
    f32 scale;
    // Which of the sprites is shown
    u8 current;
    MCSSSystem *mcssSystem;
    // The old and new forms
    MCSS *mcss[2];
    PiecesManager *pieces;
    // FALSE while the pieces are drawn instead of the sprites
    BOOL mcssVisible;
};

#endif // BWPORT_DEMO_SHINKA_DEMO_H
