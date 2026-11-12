// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/clact.h, the lines in use so far.
#ifndef BWPORT_GFL_CLACT_H
#define BWPORT_GFL_CLACT_H

#include "types.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nnsys/g2d.h"
#include "struct_decls.h"

// Cell actors, the OAM sprites of the 2D engines

// Holds cell actors, 0xe4 bytes each
typedef struct ClActUnit ClActUnit;
typedef struct ClActor ClActor;

typedef struct {
    s16 x;
    s16 y;
    u16 sequence;
    u8 priority;
    u8 bgPriority;
} ClActorSetup;

// Where a resource is loaded: CLACT_VRAM_MAIN, _SUB or _BOTH
typedef enum {
    CLACT_VRAM_MAIN,
    CLACT_VRAM_SUB,
    CLACT_VRAM_BOTH,
} ClActVram;

// The surfaces of the default renderer, which the actors' positions are relative to. An enum, which MWCC does not
// propagate as a constant
typedef enum {
    CLACT_SURFACE_MAIN,
    CLACT_SURFACE_SUB,
} ClActSurface;

typedef struct {
    s16 x;
    s16 y;
} ClActorPos;

// The functions are named by Black's addresses; pokebw2 names them by Black 2's, given after each
void func_0204b3b4(ClActor *actor); // func_0204c108
void func_0204b3dc(ClActor *actor, BOOL visible); // func_0204c124
BOOL func_0204b3f4(ClActor *actor); // func_0204c138
// Positions are relative to a surface of the actor's renderer, or absolute with surface 0xffff
void func_0204b404(ClActor *actor, const ClActorPos *pos, u16 surface); // func_0204c140
void func_0204b454(ClActor *actor, ClActorPos *pos, u16 surface); // func_0204c178
void func_0204b528(ClActor *actor, const ClActorPos *pos); // func_0204c210
void func_0204b84c(ClActor *actor, u32 bgPriority); // func_0204c468
// Sets the actor's animation sequence
void func_0204b878(ClActor *actor, u16 sequence);
u16 func_0204b898(ClActor *actor); // func_0204c4a0
// pokebw2's, at its 0x0204c4a8; Black's link drops it (nothing calls it)
u16 ClActor_GetSequenceCount(ClActor *actor);
BOOL func_0204b98c(ClActor *actor); // func_0204c560

typedef struct {
    // Where the main and sub screens are, for actors positioned on them
    s16 mainX;
    s16 mainY;
    s16 subX;
    s16 subY;
    // The OAMs of each engine the system uses
    u8 oamStartMain;
    u8 oamCountMain;
    u8 oamStartSub;
    u8 oamCountSub;
    // How many VRAM transfer cell animations can play at once
    u32 transferCount;
    // How many resources of each kind can be loaded
    u16 charCount;
    u16 plttCount;
    u16 cellAnimCount;
    u16 unk16;
    // Where the resources start in OBJ VRAM, in 32-byte units
    u16 charOffsetMain;
    u16 charOffsetSub;
} ClActSysSetup;

typedef struct ClActRenderer ClActRenderer;

// A surface of a renderer: its view, its screen and how it culls cells
typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    u32 screen;
    u32 culling;
} ClActSurfaceSetup;

typedef struct {
    fx32 x;
    fx32 y;
} ClActorScale;

typedef struct {
    ClActorSetup base;
    ClActorPos affineCenter;
    fx32 scaleX;
    fx32 scaleY;
    u16 rotation;
    u16 affineMode;
} ClActorSetupEx;

// When an animation callback is called
enum {
    CLACT_CALLBACK_LAST_FRAME,
    CLACT_CALLBACK_FRAME,
    CLACT_CALLBACK_EVERY_FRAME,
};

// Called with the callback's parameter (a pointer at every call site) and the animation's frame. NNS's
// NNSG2dAnmCallBackPtr takes the parameter as a u32: clact.c casts at its calls
typedef void (*ClActorAnimCallback)(uintptr_t param, fx32 frame);

typedef struct {
    u16 type;
    // For CLACT_CALLBACK_FRAME
    u16 frame;
    uintptr_t param;
    ClActorAnimCallback func;
} ClActorCallback;

// Positions are relative to a surface of the unit's renderer, or absolute with surface 0xffff
ClActor *func_0204b294(ClActUnit *unit, u32 chars, u32 palette, u32 cellAnims, const ClActorSetup *setup, u16 surface,
                       HeapID heapId);
void func_0204b6f4(ClActor *actor, u32 palette, u32 a2);
void func_0204b92c(ClActor *actor, BOOL a1);

u32 func_0204ac18(ArcTool *arc, u32 fileId, u32 vramType, u16 offset, u16 start, u16 count, HeapID heapId);
u32 func_0204acec(ArcTool *arc, u32 fileId, u32 vramType, u16 offset, HeapID heapId);
u32 func_0204aef4(u32 palette, BOOL sub);
void func_0204ada4(u32 palette);

void func_0204b270(ClActUnit *unit);
void func_0204b1cc(ClActUnit *unit);
void func_0204a5ac(void);
void func_0204a600(void);
void func_0204a648(void);

// Load characters (NCGR), a palette (NCLR), and cells with their animations (NCER and NANR) from an archive, and
// return their resource indices
u32 func_0204a6c8(ArcTool *arc, u32 fileId, BOOL compressed, u32 vramType, HeapID heapId);
void func_0204a8d4(u32 chars);
void func_0204afd8(u32 cellAnims);

void func_0204b5ac(ClActor *actor, const ClActorScale *scale);

u32 func_0204abf0(ArcTool *arc, u32 fileId, u32 vramType, u16 offset, HeapID heapId);
void func_0204b8d4(ClActor *actor, fx32 speed);
// Act on the actor's animation controller
void func_0204b8c4(ClActor *actor, fx32 time);

void func_0204b800(ClActor *actor, u8 priority);

void func_0204b974(ClActor *actor);
void func_0204b95c(ClActor *actor);
void func_0204bfd4(ClActUnit *unit, BOOL active);
void func_0204b9a0(ClActor *actor);
void func_0204b570(ClActor *actor, u32 affineMode);

BOOL func_0204b944(ClActor *actor);
// Act on the actor's animation controller

// The actor's OBJ mode, GX_OAM_MODE_*
void func_0204b688(ClActor *actor, u32 mode);
void func_0204b954(ClActor *actor, fx32 a1);

// The actor's OBJ mode, GX_OAM_MODE_*
// Act on the actor's animation controller

u8 func_0204b714(ClActor *actor);
void func_0204b784(ClActor *actor, const NNSG2dImageProxy *proxy);
// Replace the data of loaded characters
void func_0204a9d0(u32 chars, NNSG2dCharacterData *character);
void func_0204ab84(u32 chars, NNSG2dImageProxy *proxy);

void func_0204b53c(const ClActor *actor, ClActorPos *pos);

// Replace the data of loaded characters

// The actor's OBJ mode, GX_OAM_MODE_*

void func_0204b09c(ClActRenderer *renderer, u32 surface, const ClActorPos *pos);

void func_0204b604(ClActor *actor, u32 axis, BOOL flip);
void func_0204b550(ClActor *actor, s16 value, u32 axis);

void func_0204aa94(u32 chars, void *src, u32 size, u32 offset, u32 vramType);

void func_0204ba20(ClActor *actor, BOOL a1);

// Sets it only when it changes
void func_0204b8a0(ClActor *actor, u16 sequence);

void func_0204b258(ClActUnit *unit, ClActRenderer *renderer);

u16 func_0204b918(ClActor *actor);
fx32 func_0204b8f4(ClActor *actor);
// Replace the data of loaded characters

u8 func_0204b838(ClActor *actor);

u16 func_0204b7f0(ClActor *actor);
u16 func_0204b7f8(ClActor *actor);
void func_0204b7c8(const ClActor *actor, NNSG2dImageProxy *proxy);

u32 func_0204abbc(u32 chars, BOOL sub);

void func_0204b9c4(ClActor *actor, u32 playMode);

void func_0204b5d4(ClActor *actor, fx32 value, u32 axis);
void func_0204b49c(ClActor *actor, s16 value, u16 surface, u32 axis);
s16 func_0204b4ec(ClActor *actor, u16 surface, u32 axis);
fx32 func_0204b5e4(ClActor *actor, u32 axis);

u32 func_0204b6e4(ClActor *actor);

u16 func_0204b5fc(ClActor *actor);
void func_0204b5f4(ClActor *actor, u16 rotation);

u8 func_0204b864(ClActor *actor);

void func_0204b5c0(ClActor *actor, ClActorScale *scale);

void func_0204b588(ClActor *actor, const ClActorPos *center);

void func_0204a698(void);
void func_0204aec0(u32 palette, NNSG2dImagePaletteProxy *proxy);
void func_0204b0c8(ClActRenderer *renderer, u32 surface, ClActorPos *pos);
s16 func_0204b560(ClActor *actor, u32 axis);
void func_0204b59c(ClActor *actor, s16 value, u32 axis);
BOOL func_0204b658(ClActor *actor, u32 axis);
void func_0204b728(ClActor *actor, const NNSG2dImagePaletteProxy *proxy);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void ClActSys_Create(const ClActSysSetup *setup, const BGSysVRAMConfig *vramConfig, HeapID heapId);
ClActUnit *func_0204b100(u16 count, u8 priority, HeapID heapId);
u32 func_0204af28(ArcTool *arc, u32 cellFileId, u32 animFileId, HeapID heapId);
// The setup that most apps create the system with, the same as the intro's copy
extern const ClActSysSetup data_020a1448;
void func_0204b908(ClActor *actor, u16 frame);
// A renderer of its own for units, with count surfaces
ClActRenderer *func_0204b034(const ClActSurfaceSetup *setups, u16 count, HeapID heapId);
void func_0204b084(ClActRenderer *renderer);
// Replace the data of a loaded palette, count palettes of it or all when 0
void func_0204adec(u32 palette, NNSG2dPaletteData *data, u32 count);
void func_0204b0f4(ClActRenderer *renderer, BOOL cull);
ClActor *func_0204b324(ClActUnit *unit, u32 chars, u32 palette, u32 cellAnims, const ClActorSetupEx *setup,
                       u16 surface, HeapID heapId);
void func_0204ba00(ClActor *actor, const ClActorCallback *callback);
void func_0204b764(const ClActor *actor, NNSG2dImagePaletteProxy *proxy);
void func_0204b22c(ClActUnit *unit, u32 a1);
u8 func_0204b248(ClActUnit *unit);
u32 func_0204a7a0(u32 size, u32 vramType, HeapID heapId);
// Game Sync's screens' setup, with 192 for the sub screen's y
extern const ClActSysSetup data_020a1464;
void func_0204ba10(ClActor *actor);

#endif // BWPORT_GFL_CLACT_H
