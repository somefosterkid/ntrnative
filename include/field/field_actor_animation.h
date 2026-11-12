// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_actor_animation.h: the
// prototypes of Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_ACTOR_ANIMATION_H
#define BWPORT_FIELD_FIELD_ACTOR_ANIMATION_H

#include "types.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "system/game_event.h"
#include "struct_decls.h"

FieldActorAnmProc *FieldActorAnmProc_Create(Field *field, u16 actorId, const VecFx32 *pos, HeapID heapId);
void FieldActorAnmProc_Free(FieldActorAnmProc *proc);
GameEventReturnCode EventActorAnmProcWait_Callback(GameEvent *event, u32 *state, void *data);

// pokebw2's: the work of EventFieldSubscreenChange_Callback, which changes the subscreen to mode once it is ready
typedef struct {
    FieldSubscreen *subscreen;
    u32 mode;
} EventFieldSubscreenChange;

GameEventReturnCode EventFieldSubscreenChange_Callback(GameEvent *event, u32 *state, void *work);

// Black 2's; twinned code confirms the offsets of actor, curve, asyncProc, pos, animation, finished and heapId in Black
// Function names from swan.
// Only the fields needed by the current C accessors are known.
struct FieldActorAnmProc {
    FieldActor *actor;
    u8 unk4[0x180];
    G3DCurve *curve;
    FieldAsyncProc *asyncProc;
    VecFx32 pos;
    u8 unk198;
    u8 animation;
    u8 finished;
    u8 unk19b;
    HeapID heapId;
};

BOOL FieldActorAnmProc_IsPlaying(FieldActorAnmProc *proc);
void FieldActorAnmProc_Update(FieldAsyncProc *asyncProc, Field *field, void *data);
BOOL s0157_ActorAnimationInit(VM *vm, FieldScriptEnv *env);
BOOL s0158_ActorAnimationFree(VM *vm, FieldScriptEnv *env);
BOOL s0159_ActorAnimationPlay(VM *vm, FieldScriptEnv *env);
BOOL s015A_ActorAnimationWait(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void FieldActorAnmProc_Play(FieldActorAnmProc *proc, u16 animation);
void FieldActorAnmProc_CommitTransform(G3DCurve *curve, FieldActor *actor, VecFx32 *pos);

#endif // BWPORT_FIELD_FIELD_ACTOR_ANIMATION_H
