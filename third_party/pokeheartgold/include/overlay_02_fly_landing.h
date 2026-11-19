#ifndef POKEHEARTGOLD_OVERLAY_02_FLY_LANDING_H
#define POKEHEARTGOLD_OVERLAY_02_FLY_LANDING_H

// Task environment of the Fly takeoff/landing player animation (allocated in ov02_0224B418, 0x17C bytes on the DS).
// Pointers are tagged-forward so the header composes with the overlay 2 units that still carry local copies of the sprite types.

#include "global.h"

#include "g2d_renderer.h"

struct FieldSystem;
struct OverworldAnimManager;
struct OverworldAnimManagerList;
struct Sprite;
struct SpriteList;
struct SpriteResourceCollection;
struct SpriteResourceWithLocation;
struct SysTask;

typedef struct FlyResources {
    u8 resource1Count;
    u8 resource2Count;
    u8 resource3Count;
    u8 resource4Count;
    s8 resource1ID;
    s8 resource2ID;
    s8 resource3ID;
    s8 resource4ID;
    struct SpriteList *spriteList;
    G2dRenderer g2dRenderer;
    struct SpriteResourceCollection *charLocation;
    struct SpriteResourceCollection *paletteLocation;
    struct SpriteResourceCollection *cellLocation;
    struct SpriteResourceCollection *animLocation;
    struct SpriteResourceWithLocation *charSourceWrapper;
    struct SpriteResourceWithLocation *paletteSourceWrapper;
    struct SpriteResourceWithLocation *cellSourceWrapper;
    struct SpriteResourceWithLocation *animSourceWrapper;
} FlyResources;

typedef struct FlyLandingEnv {
    int state;                                      // 0x00
    int animationFinished;                          // 0x04
    int _1;                                         // 0x08
    int playerGender;                               // 0x0C
    BOOL animationInProgress;                       // 0x10
    struct FieldSystem *fieldSystem;                // 0x14
    FlyResources resources;                         // 0x18
    struct Sprite *playerSprite;                    // 0x16C
    struct OverworldAnimManagerList *animManagerList; // 0x170
    struct OverworldAnimManager *animManager;       // 0x174
    struct SysTask *_2;                             // 0x178
} FlyLandingEnv;

// What ov02_02248D58 hands to the anim manager (template ov02_02253454) as user data; the landing animation context embeds a copy.
typedef struct FlyResourcesWithPlayer {
    struct OverworldAnimManagerList *unk_00;
    struct FieldSystem *fieldSystem;
    struct Sprite *playerSprite;
    FlyResources *resources;
} FlyResourcesWithPlayer;

// Context of the Fly landing animation (OverworldAnimManager ctx, template ov02_02253454, ctxSize 0x74 on the DS).
// stage selects the state table (ov02_02253320): 0 = ov02_022532F8, 1 = ov02_022534D0 (landing: ov02_02248F88 ... ov02_022493EC).
typedef struct FlyLandingEnvExtended {
    u8 stage;                                   // 0x00
    u8 state;                                   // 0x01
    u8 hasEndedIfTwo;                           // 0x02
    u8 _1;                                      // 0x03
    int subState;                               // 0x04
    VecFx32 position;                           // 0x08
    VecFx32 pathOffset;                         // 0x14
    VecFx32 _2;                                 // 0x20
    VecFx32 scale;                              // 0x2C
    fx32 birdRotation;                          // 0x38
    fx32 _3;                                    // 0x3C
    fx32 angleParam;                            // 0x40
    fx32 _4;                                    // 0x44
    fx32 pathParam;                             // 0x48
    fx32 pathDelta;                             // 0x4C
    fx32 scaleDelta;                            // 0x50
    fx32 playerOffsetY;                         // 0x54
    FlyResourcesWithPlayer resourcesWithPlayer; // 0x58
    struct Sprite *birdSprite;                  // 0x68
    struct OverworldAnimManager *unk_6C;        // 0x6C
    struct SysTask *task;                       // 0x70
} FlyLandingEnvExtended;

#endif // POKEHEARTGOLD_OVERLAY_02_FLY_LANDING_H
