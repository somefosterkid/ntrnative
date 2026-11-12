// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_internal.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_FIELD_INTERNAL_H
#define BWPORT_FIELD_FIELD_INTERNAL_H

#include "types.h"
#include "struct_decls.h"
#include "field/field_g3d_mapper.h"
#include "field/zone.h"
#include "nitro/fx/fx.h"

// Black 2's, not checked against Black
struct FieldGimmickWorkBlock {
    u32 password;
    void *work;
};

// Black's layout (Field_Create allocates 0x154), from pokebw2's (swan's): Black has no unk34, fesGimmick,
// colorPostFX, unk_124 or lensFlare, and its fields keep Black 2's order
struct Field {
    u16 heapId;
    u16 unk2;
    GameSystem *gameSystem;
    GameData *gameData;
    AreaData *areaData;
    FieldCamera *cameraSystem;
    void *lightSystem;
    FieldFog *fog;
    void *fogCtrl;
    void *weatherSystem;
    FieldSubscreen *subscreen;
    void *msgBGSys;
    void *moneyWin;
    PlaceName *placeName;
    FieldExpObjSystem *expObjSystem;
    MMSys *actorSystem;
    NoGridMapper *noGridMapper;
    void *sceneArea;
    void *sceneAreaLoader;
    FieldG3DMapper *g3DMapper;
    u32 unk4C;
    FieldG3DMapperConfig mapperConfig;
    FieldPlayer *player;
    EncountSystem *encountSystem;
    void *fieldEffects;
    void *unk98;
    void *palaceSys;
    void *skillMapEff;
    void *g3dCamera;
    void *g3dLights;
    TCB *asyncActorMatLoadTCB;
    void *actorBlAct;
    void *effectBlAct;
    void *wildEffectBlAct;
    FieldAsyncProcManager *asyncProcManager;
    u32 routineState;
    u32 routineID;
    u8 subroutinePhase;
    u8 routineAlternator;
    u16 unkCA;
    u32 framesSinceStart;
    u16 zoneId;
    u16 unkD2;
    ZoneSpawnInfo playerZoneState;
    VecFx32 *playerPosPtr;
    VecFx32 playerPos;
    fx32 actorYOffset;
    fx32 objectProjectionMatrixOffset;
    const FieldmapCtrlVTable *ctrlVTable;
    void *controller;
    TCBManager *tcbManager;
    void *tcbManagerHeap;
    FieldGimmickWorkBlock gimmickWork;
    struct FieldParticle *particleSystem;
    void *g3dCi;
    DayCare *dayCare;
    u32 renderMode;
    void *g3DObjSystem;
    FieldTaskManager *taskManager;
    BOOL fadeFlag;
    EncEff *encEff;
    BOOL effectRunningFlag;
    BOOL seasonBannerOverdrawFlag;
    void *nDemoData;
    FieldDispControl *dispControl;
    CasteliaRush *casteliaRush;
};

void func_ov021_02189758(FieldGimmickWorkBlock *block);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void *func_ov021_02188ce0(Field *field);

#endif // BWPORT_FIELD_FIELD_INTERNAL_H
