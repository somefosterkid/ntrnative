// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_effect.h, the lines in use so
// far. pokebw2's func_ov036_021a3e74 is Black's func_ov021_021ac528, and its func_ov036_021a3bf0,
// func_ov036_021a3a70, func_ov036_021a3a94 and func_ov036_021a3ac8 are Black's func_ov021_021ac2c0,
// func_ov021_021ac154, func_ov021_021ac178 and func_ov021_021ac1ac.
#ifndef BWPORT_FIELD_FIELD_EFFECT_H
#define BWPORT_FIELD_FIELD_EFFECT_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

typedef struct FieldEffects FieldEffects;
typedef struct FieldEffectTask FieldEffectTask;

void *Field_GetEffectBlAct(Field *field);
// pokebw2's func_ov036_021b3f14: an emotion bubble over an actor's head, by kind (an exclamation mark, a question mark, a music note and an ellipsis);
// playSE plays its sound. It stays when it is done, until its owner ends it
FieldEffectTask *func_ov021_021bb528(FieldEffects *effects, FieldActor *actor, u32 kind, BOOL playSE);
// pokebw2's func_ov036_021a58e0 and func_ov036_021a5968
FieldEffectTask *func_ov021_021adf94(FieldEffects *effects, const VecFx32 *pos, u32 dir, u32 sameHeight);
void func_ov021_021ae01c(FieldEffectTask *task, u16 speed);

// The dust of an actor landing
void func_ov021_021ac528(FieldActor *actor, FieldEffects *effects);
// Give the actor a shadow
void func_ov021_021ac2c0(FieldActor *actor, FieldEffects *effects);

typedef void (*FieldEffectTaskFunc)(FieldEffectTask *task, void *work);

// What a task does when it starts, ends, updates and draws
typedef struct {
    // The size of the task's work
    u32 workSize;
    FieldEffectTaskFunc init;
    FieldEffectTaskFunc delete;
    FieldEffectTaskFunc update;
    FieldEffectTaskFunc draw;
} FieldEffectTaskVTable;

Field *FieldEffects_GetField(FieldEffects *effects);
ArcTool *FieldEffects_GetArc(FieldEffects *effects);
void *FieldEffects_GetHandleData(FieldEffects *effects, u32 id);
// Start a task, at pos if it isn't NULL
FieldEffectTask *FieldEffects_TCBCreate(FieldEffects *effects, const FieldEffectTaskVTable *vtable,
                                        const VecFx32 *pos, u32 param1, void *param2, u32 priority);
// End a task, update it, and its parameters
void func_ov021_021ac154(FieldEffectTask *task);
void func_ov021_021ac178(FieldEffectTask *task);
void *func_ov021_021ac1ac(FieldEffectTask *task);

// fldeff_shadow.c
void *FieldEffect_Shadow_Create(FieldEffects *effects, HeapID heapId);
void FieldEffect_Shadow_Free(FieldEffects *effects, void *data);

// field_saveanime.c, the icon over the player while the game saves: pokebw2's func_ov036_021c6cc8,
// func_ov036_021c6cf8, func_ov036_021c6d14 and func_ov036_021c6d3c (create, free, start, stop)
FieldSaveAnime *func_ov021_021d1164(HeapID heapId, Field *field);
void func_ov021_021d1194(FieldSaveAnime *effect);
void func_ov021_021d11b0(FieldSaveAnime *effect);
void func_ov021_021d11d8(FieldSaveAnime *effect);

u32 FieldEffects_GetSeason(FieldEffects *effects);
BOOL FieldEffects_GetAreaIsExterior(FieldEffects *effects);
void FieldEffects_ApplyLuminanceTable(FieldEffects *effects, void *resource);

void *func_ov021_021ac1e0(FieldEffectTask *task);
void func_ov021_021ac1cc(FieldEffectTask *task, const VecFx32 *pos);
void func_ov021_021ac1b8(FieldEffectTask *task, VecFx32 *pos);
void *func_ov021_021adedc(FieldEffects *effects, HeapID heapId);

void *playSmokeEffect(FieldEffects *effects, HeapID heapId);
// The dust in front of an actor
void func_ov021_021ac578(FieldActor *actor, FieldEffects *effects);

void *func_ov021_021adcb8(FieldEffects *effects, HeapID heapId);

void *func_ov021_021bbcbc(FieldEffects *effects, HeapID heapId);
// fldeff_footmark.c: the marks an actor leaves, by kind: footprints in sand (0) and snow (2), the tracks of actors
// whose footprint type is 2 in sand (1) and snow (3), and tracks in deep sand (4). Tracks turn with the actor
void func_ov021_021bbdd4(FieldActor *actor, FieldEffects *effects, u32 kind);

void *loadGrassEffects(FieldEffects *effects, HeapID heapId);
u32 func_ov021_021ac1a0(FieldEffectTask *task);

BOOL func_ov021_021bb5c8(FieldEffectTask *task);

void *func_ov021_021ad764(FieldEffects *effects, HeapID heapId);
void *func_ov021_021ad770(FieldEffects *effects, HeapID heapId);
void *func_ov021_021ad77c(FieldEffects *effects, HeapID heapId);
void *func_ov021_021ad788(FieldEffects *effects, HeapID heapId);
void *func_ov021_021ad794(FieldEffects *effects, HeapID heapId);
void func_ov021_021ad7a0(FieldEffects *effects, void *data);
// Pause and hide a phenomenon
void func_ov021_021adb4c(FieldEffectTask *task, BOOL paused);
void func_ov021_021adb5c(FieldEffectTask *task, BOOL hidden);

void *func_ov021_021aca38(FieldEffects *effects, HeapID heapId);
// fldeff_namipoke.c: the Pokémon an actor surfs on, facing dir at pos, and how it follows the actor (0: it doesn't, 1:
// it bobs under the actor, 2: the actor's offset is left alone), which can change; and whether its wake shows
FieldEffectTask *func_ov021_021acb38(FieldEffects *effects, u16 dir, const VecFx32 *pos, FieldActor *actor, u32 mode);
void func_ov021_021acbb8(FieldEffectTask *task, u8 mode);
void func_ov021_021acbc8(FieldEffectTask *task, BOOL showWake);
void *func_ov021_021ad3d4(FieldEffects *effects, HeapID heapId);
BOOL func_ov021_021ad5cc(FieldEffectTask *task);

// Create and free functions of the effects table
void *func_ov021_021bbf78(FieldEffects *effects, HeapID heapId);
void func_ov021_021bbf9c(FieldEffects *effects, void *data);
void *func_ov021_021c47b4(FieldEffects *effects, HeapID heapId);
void func_ov021_021c47e8(FieldEffects *effects, void *data);
void *func_ov021_021c4978(FieldEffects *effects, HeapID heapId);
void func_ov021_021c49ac(FieldEffects *effects, void *data);
void *func_ov021_021c8e68(FieldEffects *effects, HeapID heapId);
void func_ov021_021c8e8c(FieldEffects *effects, void *data);
void *func_ov021_021c94bc(FieldEffects *effects, HeapID heapId);
void func_ov021_021c94e0(FieldEffects *effects, void *data);
void *func_ov021_021d0140(FieldEffects *effects, HeapID heapId);
void func_ov021_021d0160(FieldEffects *effects, void *data);
// fldeff_btrain.c: pokebw2's FldeffBtrain_Start, a Battle Subway train of the kind, which starts at pos
FieldEffectTask *func_ov021_021d0218(FieldEffects *effects, u32 trainIdx, const VecFx32 *pos);
void *func_ov021_021d038c(FieldEffects *effects, HeapID heapId);
void func_ov021_021d03b0(FieldEffects *effects, void *data);
void *func_ov021_021d0610(FieldEffects *effects, HeapID heapId);
void func_ov021_021d0634(FieldEffects *effects, void *data);
void *func_ov021_021c4b28(FieldEffects *effects, HeapID heapId);
void func_ov021_021c4b50(FieldEffects *effects, void *data);

void FieldEffects_Free(FieldEffects *effects);

// field_effect.c: the effects a map has loaded, by ID, and the tasks that play them (Black has 24; Black 2 has 25)
enum {
    FLDEFF_MAX = 24,
    // The ID of an empty slot, and of the empty last entry of the effects' table
    FLDEFF_NONE = FLDEFF_MAX,
};

typedef struct FieldEffectTaskStore FieldEffectTaskStore;

void FieldEffects_SetLuminanceTable(FieldEffects *effects, const u8 *table);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void *Field_GetWildEffectBlAct(Field *field);
void func_ov021_021adf00(FieldEffects *effects, void *data);
void freeSmokeEffect(FieldEffects *effects, void *data);
// The field effects' 3D object system, archive, season and whether the area is outdoors
struct FieldG3DObjSystem *func_ov021_021abe18(FieldEffects *effects);
// Load count effects by ID, free one, and whether one is loaded and its data
void FieldEffects_Load(FieldEffects *effects, const u32 *ids, u32 count);
BOOL FieldEffects_IsLoaded(FieldEffects *effects, u32 id);
void func_ov021_021adcdc(FieldEffects *effects, void *data);
// Break the rock in front of an actor with Rock Smash
void func_ov021_021add7c(FieldActor *actor, FieldEffects *effects);
u8 func_ov021_021c90d0(FieldEffectTask *task);
// Chargestone Cave's stones (field_gimmick_d06.c): the effect over a stone at pos for actor, and the one at a mark
// at pos. Then lighting a stone's effect, starting and the state of its other animation, and stopping a mark's
FieldEffectTask *func_ov021_021c9064(FieldEffects *effects, const VecFx32 *pos, FieldActor *actor);
FieldEffectTask *func_ov021_021c9258(FieldEffects *effects, const VecFx32 *pos);
void func_ov021_021c909c(FieldEffectTask *task);
void func_ov021_021c90ac(FieldEffectTask *task);
void func_ov021_021c928c(FieldEffectTask *task);
void func_ov021_021bbcf8(FieldEffects *effects, void *data);
FieldEffectTask *func_ov021_021bb578(FieldEffects *effects, FieldActor *actor, u32 kind, BOOL playSE);
void freeGrassEffects(FieldEffects *effects, void *data);
void func_ov021_021ac760(FieldEffects *effects, FieldActor *actor, BOOL animate, int kind);
void *func_ov021_021bb484(FieldEffects *effects, HeapID heapId);
void func_ov021_021bb4a8(FieldEffects *effects, void *data);
void *func_ov021_021ad758(FieldEffects *effects, HeapID heapId);
// The field effect of a phenomenon, which fldeff_encount.c plays
FieldEffectTask *func_ov021_021adaac(EncountSystem *system, FieldEffects *effects, u16 x, u16 z, fx32 height,
                                     u32 kind);
void func_ov021_021bbfb8(MMSys *system, FieldActor *actor, FieldEffects *effects, u32 kind);
void func_ov021_021aca5c(FieldEffects *effects, void *data);
void func_ov021_021ad3fc(FieldEffects *effects, void *data);
// A splash of a kind that follows the surfed Pokémon's task, or one at pos that ends with its animation, and
// whether a splash's animation has ended
FieldEffectTask *func_ov021_021ad564(FieldEffects *effects, u32 kind, FieldEffectTask *parent);
FieldEffectTask *func_ov021_021ad598(FieldEffects *effects, u32 kind, const VecFx32 *pos);
FieldEffects *FieldEffects_Create(Field *field, int count, HeapID heapId);
void FieldEffects_Update(FieldEffects *effects);
void FieldEffects_Draw(FieldEffects *effects);
void FieldEffects_FreeEffect(s32 *param_1, s32 param_2);
void FieldEffects_TCBManagerInit(FieldEffects *effects, u32 count);

#endif // BWPORT_FIELD_FIELD_EFFECT_H
