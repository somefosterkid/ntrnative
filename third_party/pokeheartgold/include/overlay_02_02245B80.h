#ifndef POKEHEARTGOLD_OVERLAY_02_02245B80_H
#define POKEHEARTGOLD_OVERLAY_02_02245B80_H

#include "global.h"
#include "constants/heap.h"
#include "field_types_def.h"
#include "field/overlay_01_021FB878.h"
#include "bg_window.h"
#include "camera.h"
#include "pokemon_types_def.h"
#include "nnsys/fnd/allocator.h"
#include "overlay_02_app_02245e68.h"

struct SysTask;

// Environment of the legendary demo scene task (ov02_02245B80): a 3D model with two animations from NARC_demo_legend.
// sizeof == 0xD0 on the DS.
typedef struct LegendDemoScene {
    enum HeapID heapID;                      // 0x00
    FieldSystem *fieldSystem;                // 0x04
    u16 state;                               // 0x08
    u16 timer;                               // 0x0A
    Field3dModel model;                      // 0x0C
    Field3DModelAnimation animations[2];     // 0x1C
    Field3dObject object;                    // 0x44
    NNSFndAllocator allocator;               // 0xBC
    s32 soundFrame;                          // 0xCC
} LegendDemoScene;

void ov02_02245D18(LegendDemoScene *scene);
void ov02_02245DB0(LegendDemoScene *scene);
void ov02_02245DE0(LegendDemoScene *scene);

// Environment of ov02_022460FC (a legendary effect played once; created by ov02_022460CC). sizeof == 0x14 on the DS.
typedef struct LegendEffectTask {
    enum HeapID heapID;              // 0x00
    FieldSystem *fieldSystem;        // 0x04
    Ov02App_02245E68 *effect;        // 0x08
    u16 state;                       // 0x0C
    u16 padding_E;
    u16 effectIdx;                   // 0x10
} LegendEffectTask;

// Environment of ov02_022461DC (ShowLegendaryWing; also the data of the ov02_022462DC SysTask). sizeof == 0x18 on the DS.
typedef struct LegendWingTask {
    enum HeapID heapID;              // 0x00
    FieldSystem *fieldSystem;        // 0x04
    Ov02App_02245E68 *effect;        // 0x08
    struct SysTask *sysTask;         // 0x0C
    u8 wingIdx;                      // 0x10
    u8 padding_11;
    u16 state;                       // 0x12
    u32 padding_14;
} LegendWingTask;

// Environment of ov02_02246330 (legend demo BG fade-in). sizeof == 0x10 on the DS.
typedef struct LegendFadeTask {
    enum HeapID heapID;              // 0x00
    FieldSystem *fieldSystem;        // 0x04
    u8 state;                        // 0x08
    u8 fadePhase;                    // 0x09 (ov02_02246490's own state)
    u8 fadeStep;                     // 0x0A
    u8 fadeWait;                     // 0x0B
    u8 count;                        // 0x0C
    u8 alpha;                        // 0x0D
    u8 bgPriority0;                  // 0x0E
    u8 bgPriority1;                  // 0x0F
} LegendFadeTask;

s32 ov02_02246490(LegendFadeTask *env);
void ov02_02246398(LegendFadeTask *env);
void ov02_02246444(LegendFadeTask *env);

// A camera that sways around the target of the field camera (ov02_02246548 sets it up, ov02_0224663C steps it). sizeof == 0x38 on the DS.
typedef struct LegendCameraShake {
    enum HeapID heapID;       // 0x00
    FieldSystem *fieldSystem; // 0x04
    s16 amplitudeX;           // 0x08
    s16 amplitudeY;           // 0x0A
    u8 padding_C[2];
    u16 repeat;               // 0x0E
    u16 frame;                // 0x10
    u16 frames;               // 0x12
    fx32 angleStep;           // 0x14
    Camera *camera;           // 0x18
    u8 padding_1C[4];
    VecFx32 target;           // 0x20
    VecFx32 origin;           // 0x2C
} LegendCameraShake;

// Environment of ov02_02246798 (created by ov02_02246714). sizeof == 0xC on the DS.
typedef struct LegendCameraShakeTask {
    enum HeapID heapID;        // 0x00
    FieldSystem *fieldSystem;  // 0x04
    LegendCameraShake *shake;  // 0x08
} LegendCameraShakeTask;

LegendCameraShake *ov02_0224650C(FieldSystem *fieldSystem, enum HeapID heapID);
void ov02_02246534(LegendCameraShake *shake);
void ov02_02246548(LegendCameraShake *shake, s32 amplitudeX, s32 amplitudeY, u16 repeat, u16 frames);
void ov02_0224662C(LegendCameraShake *shake);
s32 ov02_0224663C(LegendCameraShake *shake);

// Environment of ov02_02246818 (created by ov02_022467E8 for OpenAlphHiddenRoom): the legend demo background and a window with an Unown word.
// sizeof == 0x1C on the DS.
typedef struct AlphUnownTextTask {
    enum HeapID heapID;        // 0x00
    FieldSystem *fieldSystem;  // 0x04
    Window window;             // 0x08
    u8 state;                  // 0x18
    u8 msgNo;                  // 0x19
    u8 bgPriority;             // 0x1A
} AlphUnownTextTask;

AlphUnownTextTask *ov02_022467E8(FieldSystem *fieldSystem, u8 msgNo);
void ov02_0224686C(AlphUnownTextTask *env);

void ov02_02246964(AlphUnownTextTask *env);

BOOL ov02_022469A0(void);

// Environment of ov02_02246A34 (created by ov02_022469D8): the follow mon cut-in (HMCut mode 3) shown from ov02_022469B4. sizeof == 0x1C on the DS.
typedef struct FollowMonCutInTask {
    enum HeapID heapID;              // 0x00
    int state;                       // 0x04
    FieldSystem *fieldSystem;        // 0x08
    Pokemon *mon;                    // 0x0C
    LocalMapObject *playerObject;    // 0x10
    LocalMapObject *followMonObject; // 0x14
    struct SysTask *cutInTask;       // 0x18
} FollowMonCutInTask;

FollowMonCutInTask *ov02_022469D8(FieldSystem *fieldSystem, LocalMapObject *playerObject, LocalMapObject *followMonObject);

void ov02_02245B80(TaskManager *taskManager);
LegendDemoScene *ov02_02245B9C(FieldSystem *fieldSystem);
BOOL ov02_02245BC8(TaskManager *taskManager);

#endif // POKEHEARTGOLD_OVERLAY_02_02245B80_H
