// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_g3dobj.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_G3DOBJ_H
#define BWPORT_FIELD_FIELD_G3DOBJ_H

// The field's 3D objects: groups of resources and objects made from them. Names and layouts from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"

typedef struct FieldG3DObjSystem FieldG3DObjSystem;

// Black 2's, not checked against Black
// The archives and files of a resource group
struct FieldG3DObjResRequest {
    ArcTool *mdlArc;
    ArcTool *texArc;
    ArcTool *anmArc;
    u16 mdlDatID;
    u16 texDatID;
    u16 anmDatIDs[8];
    u8 anmCount;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void FieldG3DObjSystem_FreeResGroup(FieldG3DObjSystem *sys, u16 resGroupIdx);
u16 FieldG3DObjSystem_AddObj(FieldG3DObjSystem *sys, u16 resGroupIdx, u16 modelIdxInRes, VecFx32 *pos);
void FieldG3DObjSystem_FreeObj(FieldG3DObjSystem *sys, u16 objIdx);
SRTMatrix *FieldG3DObjSystem_GetObjTransform(FieldG3DObjSystem *sys, u16 objIdx);
void FieldG3DObjSystem_SetObjPosition(FieldG3DObjSystem *sys, u16 objIdx, VecFx32 *src);
void FieldG3DObjSystem_SetAnmFrame(FieldG3DObjSystem *sys, u16 objIdx, s8 anmIdx, fx32 frame);
// TRUE until the object's animation ends
BOOL FieldG3DObjSystem_StepObjAnm(FieldG3DObjSystem *sys, u16 objIdx, fx32 addend);
BOOL FieldG3DObjSystem_StepObjAnmLoop(FieldG3DObjSystem *sys, u16 objIdx, fx32 addend);
void FieldG3DObjSystem_DrawObj(FieldG3DObjSystem *sys, u16 objIdx);
void func_ov021_021c6828(FieldG3DObjSystem *sys, u16 objIdx, u32 flag);
u32 func_ov021_021c6844(FieldG3DObjSystem *sys, u16 objIdx);
u16 FieldG3DObjSystem_AddResGroup(FieldG3DObjSystem *sys, FieldG3DObjResRequest *req, BOOL preloadTextures);
void FieldG3DObjResRequest_Clear(FieldG3DObjResRequest *req);
void FieldG3DObjResRequest_SetModel(FieldG3DObjResRequest *req, ArcTool *arc, u16 datId);
void FieldG3DObjResRequest_SetAnmArc(FieldG3DObjResRequest *req, ArcTool *arc);
void FieldG3DObjResRequest_AddAnm(FieldG3DObjResRequest *req, u16 datId);
// Leave the object out of FieldG3DObjSystem_Draw, for its owner to draw with FieldG3DObjSystem_DrawObj, or not
void func_ov021_021c67d8(FieldG3DObjSystem *sys, u16 objIdx, BOOL flag);
// Hide the object, or not
void func_ov021_021c6678(FieldG3DObjSystem *sys, u16 objIdx, BOOL hidden);
void FieldG3DObjSystem_Free(FieldG3DObjSystem *sys);
void FieldG3DObjSystem_SetupResources(FieldG3DObjSystem *sys);

typedef struct FieldG3DObjResGroup FieldG3DObjResGroup;
typedef struct FieldG3DObj FieldG3DObj;

// The resources of one model group: the model, its textures and its animations
struct FieldG3DObjResGroup {
    void *mdlRes;
    void *texRes;
    void *anmRes[8];
    u8 anmCount;
    u8 texState; // 0 while uploading, 1 uploaded, 2 not uploaded yet
};

// One object made from a group: state 0 free, 1 has an actor, 2 waits for its group's textures
struct FieldG3DObj {
    u32 state : 2;
    u32 bboxCull : 1;
    u32 hidden : 1;
    u32 manualDraw : 1;
    u32 unk_5 : 1;
    u32 unk_6 : 26;
    u16 resGroupIdx;
    u16 modelIdx;
    SRTMatrix transform;
    G3DActor *actor;
    void *anms[8];
};

// Black's layout as the functions that reach it have it
struct FieldG3DObjSystem {
    u16 unk_00;
    u16 unk_02;
    u16 resGroupCount;
    u16 objCount;
    FieldG3DObjResGroup *resGroups;
    FieldG3DObj *objs;
    u8 *luminanceTable;
};

// pokebw2's FieldG3DObjSystem_Create: room for resGroupCount groups and objCount objects
FieldG3DObjSystem *func_ov021_021c63ac(HeapID heapId, s32 resGroupCount, s32 objCount);
u16 FieldG3DObjSystem_AddResGroupCore(FieldG3DObjSystem *sys, FieldG3DObjResRequest *req, BOOL preloadTextures, u8 *luminanceTable);
void FieldG3DObjResGroup_Free(FieldG3DObjResGroup *group);
void FieldG3DObjResGroup_LoadResources(FieldG3DObjResGroup *group, u32 unk, FieldG3DObjResRequest *req, BOOL preloadTextures, u8 *luminanceTable);
void FieldG3DObjResGroup_SetupTextures(FieldG3DObjResGroup *group);
void FieldG3DObj_Free(FieldG3DObj *obj, FieldG3DObjResGroup *group);
void FieldG3DObj_Setup(FieldG3DObj *obj, FieldG3DObjResGroup *group, u16 resGroupIdx, u16 modelIdx, u16 unk, VecFx32 *pos, BOOL initTransform);

#endif // BWPORT_FIELD_FIELD_G3DOBJ_H
