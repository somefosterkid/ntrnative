#ifndef POKEHEARTGOLD_OVERLAY_02_HMCUT_IN_H
#define POKEHEARTGOLD_OVERLAY_02_HMCUT_IN_H

#include "g2d_renderer.h"
#include "pokemon_types_def.h"

#include "field/overlay_01_021FB878.h"

// HMCutIn: merged by tools/hg/promote.py from 23 m2c local copies (1 variants, base
// src/overlay_02_02248728_02249584_022495E8_02249658.c) and 120 accesses; fields at every accessed offset, pads elsewhere.

struct FieldSystem;
struct LocalMapObject;
struct NNSG2dCharacterData;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dPaletteData;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct NNSG2dScreenData;
struct OverworldAnimManager;
struct OverworldAnimManagerList;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct GF_2DGfxResMan;
struct SysTask;

// HGSS layout (hk220, from the asm of the three state tables ov02_02253550 / 02253588 / 022534F0): one layout for every
// mode, Platinum's fields plus playerSpriteSet, three anim managers, the two map objects (0x208/0x20C) and the 3D model block.
// mode 0x20: 0 = Pokemon cut-in (ov02_02249584), 1 = with Fly bird (ov02_02249984), 2 = follow mon (ov02_022499B8),
// 3 = follow mon by script (ov02_022494C4, also ov02_022499B8).
typedef struct HMCutInFieldModel {
    NNSFndAllocator allocator;
    void *modelData;
    Field3dModel model;
    Field3dObject object;
    Field3DModelAnimation anims[2];
} HMCutInFieldModel; // DS 0xC4

typedef struct HMCutIn {
    int state;
    int isFinished;
    int subState;
    u16 playerGender;
    u16 playerSpriteSet; // index into the player sprite resource tables (ov02_02253304...): gender, 2 in mode 3
    BOOL updateSprites;
    int windParticlesAnimState;
    BOOL screenFaded;
    BOOL forceDrawPlayerSprite;
    int mode;
    u16 bg0Priority;
    u16 bg3Priority;
    int _2;
    BOOL updateWindow;
    u32 window;
    u32 planeMaskInside;
    BOOL effectInside;
    u32 planeMaskOutside;
    BOOL effectOutside;
    fx32 windowX1;
    fx32 windowX2;
    fx32 windowY1;
    fx32 windowY2;
    fx32 windowDelta;
    fx32 pokemonSpriteDeltaX;
    Pokemon *mon;
    struct FieldSystem *fieldSystem;
    struct NNSG2dScreenData *g2dScreenData;
    struct NNSG2dCharacterData *g2dCharData;
    struct NNSG2dPaletteData *g2dPaletteData;
    struct SpriteList *spriteList;
    G2dRenderer g2dRenderer;
    struct GF_2DGfxResMan *charLocation;
    struct GF_2DGfxResMan *paletteLocation;
    struct GF_2DGfxResMan *cellLocation;
    struct GF_2DGfxResMan *animLocation;
    struct SpriteResource *charSource[4];
    struct SpriteResource *paletteSource[3];
    struct SpriteResource *cellSource[4];
    struct SpriteResource *animSource[2];
    struct OverworldAnimManagerList *animManagerList;
    struct Sprite *playerSprite;
    struct Sprite *pokemonSprite;
    struct OverworldAnimManager *flyBirdAnim; // data = FlyTaskEnv
    struct OverworldAnimManager *unk_1F0;     // modes 2/3 (ov02_0224B784, template ov02_02253490)
    struct OverworldAnimManager *unk_1F4;     // modes 2/3 (ov02_0224B72C, template ov02_02253440)
    PokepicTemplate pokemonSpriteTemplate;
    struct LocalMapObject *playerObject;
    struct LocalMapObject *followMonObject; // modes 2/3
    int resourcesLoaded;
    BOOL resourceSourcesFreed;
    void *pokemonCharSource;
    void *pokemonPaletteSource;
    struct SysTask *resourceTransferTask;
    struct SysTask *windowTask;
    HMCutInFieldModel fieldModel; // modes 2/3 (ov02_0224B88C)
    VecFx32 playerSpriteOffset; // modes 2/3: x, z added to the player sprite position
} HMCutIn;

// OverworldAnimManager context of the HMCutIn::unk_1F4 player-sprite visibility check (template ov02_02253440, DS 8).
typedef struct HMCutInWithPlayer2 {
    struct Sprite *playerSprite;
    HMCutIn *cutIn;
} HMCutInWithPlayer2;

// OverworldAnimManager context of the follow mon field model renderer (HMCutIn::unk_1F0, template ov02_02253490,
// DS 0xC): the cut-in, the animation step and the finished flag ov02_0224B964 waits for.
typedef struct FollowMonModelCtx {
    HMCutIn *cutIn;
    u32 state;
    BOOL finished;
} FollowMonModelCtx;

// The Fly bird animation's task env (OverworldAnimManager data of HMCutIn::flyBirdAnim, DS 0x68), read by
// ov02_02248DF0 / ov02_0224ACE0 / ov02_0224AC38 / ov02_0224ADF0 / ov02_0224B0E0 / ov02_0224B158 / ov02_0224ABCC.
typedef struct FlyTaskEnv {
    u8 stage;
    u8 state;
    u8 subState;
    u8 _1;
    int descendCount;
    VecFx32 birdStartPos;
    VecFx32 pathOffset;
    VecFx32 _2;
    VecFx32 birdScale;
    fx32 birdRotation;
    fx32 _3;
    fx32 angleParam;
    fx32 _4;
    fx32 pathParam;
    fx32 pathDelta;
    fx32 scaleDelta;
    fx32 playerOffsetY;
    struct Sprite *birdSprite;
    HMCutIn *cutIn;
    struct OverworldAnimManager *unk_60;
    struct SysTask *task;
} FlyTaskEnv; // DS 0x68

// sizeof(HMCutIn) == 0x2f8

#endif // POKEHEARTGOLD_OVERLAY_02_HMCUT_IN_H
