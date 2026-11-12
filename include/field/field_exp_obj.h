// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_exp_obj.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_FIELD_EXP_OBJ_H
#define BWPORT_FIELD_FIELD_EXP_OBJ_H

#include "types.h"
#include "gfl/g3d.h"
#include "struct_decls.h"

// The field's expansion objects: 3D models that zones such as gyms add, in scenes of actors with animations

void LoadFieldExpandObjData(FieldExpObjSystem *system, const G3DSceneSetup *setup, u16 scene);
void FieldExpObj_StepAllAnimations(FieldExpObjSystem *system);
SRTMatrix *FieldExpObj_GetActorMatrixPtr(FieldExpObjSystem *system, u16 scene, u16 actor);
FieldExpObjAnm *FieldExpObj_GetAnmInfo(FieldExpObjSystem *system, u32 scene, u16 actor, u16 anm);
void FieldExpObj_SetAnm(FieldExpObjSystem *system, u16 scene, u16 actor, u16 anm, BOOL a4);

void FieldExpObjAnm_SetPaused(FieldExpObjAnm *anm, u8 paused);
void FieldExpObjAnm_SetLooped(FieldExpObjAnm *anm, u8 looped);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void FieldExpObj_FreeScene(FieldExpObjSystem *system, u16 scene);
G3DActor *FieldExpObj_GetActor(FieldExpObjSystem *system, u32 scene, u16 actor);
BOOL FieldExpObjAnm_IsPlaybackFinished(FieldExpObjAnm *anm);
void FieldExpObj_SetAnmFrame(FieldExpObjSystem *system, u16 scene, u16 actor, u16 anm, fx32 frame);
fx32 func_ov021_021be748(FieldExpObjAnm *anm);
fx32 func_ov021_021be6e8(FieldExpObjSystem *system, u16 scene, u16 actor, u16 anm);
void func_ov021_021be4a4(FieldExpObjSystem *system, u16 scene, u16 actor, BOOL value);
void FieldExpObj_SetActorHidden(FieldExpObjSystem *system, u16 scene, u16 actor, BOOL value);
// The animation's last frame
void FieldExpObj_AddScene(FieldExpObjSystem *system, const G3DSceneSetup *setup, u8 scene);

// A slot of a scene's actor array: 0x44 bytes, the matrix at +8
typedef struct FieldExpObjActorSlot {
    u32 unk_00;
    u32 unk_04;
    SRTMatrix mtx;
} FieldExpObjActorSlot;

// An animation of a scene's actor: 0x10 bytes; fields named by offset
struct FieldExpObjAnm {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    fx32 unk_04;
    u32 unk_08;
    void *unk_0C; // the actor's animation, GFL_G3DActorGetAnm's
};

// The animations of one actor
typedef struct FieldExpObjActorAnms {
    s32 count;
    FieldExpObjAnm *anms;
} FieldExpObjActorAnms;

// A scene record of FieldExpObjSystem: 16 bytes, one per scene
typedef struct FieldExpObjScene {
    s32 active;
    FieldExpObjActorSlot *slots;
    FieldExpObjActorAnms *actorAnms;
    u16 sceneId;
    u16 actorCount;
} FieldExpObjScene;

// Black's layout
struct FieldExpObjSystem {
    u32 heapId;
    G3DManager *g3dManager;
    FieldExpObjScene scenes[4];
};

FieldExpObjSystem *func_ov021_021be2a8(u32 resourceLimit, u32 actorLimit, u32 heapId);

#endif // BWPORT_FIELD_FIELD_EXP_OBJ_H
