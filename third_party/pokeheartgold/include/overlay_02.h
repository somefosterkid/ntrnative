#ifndef POKEHEARTGOLD_OVERLAY_02_H
#define POKEHEARTGOLD_OVERLAY_02_H

#include "battle/battle_setup.h"
#include "field/headbutt.h"

#include "field/field_3d_object_task.h"

#include "heap.h"
#include "menu_input_state.h"
#include "safari_decoration_args.h"
#include "script.h"
#include "start_menu.h"
#include "sys_task.h"
#include "unk_02062108.h"

typedef struct FieldMoveTaskEnvironment FieldMoveTaskEnvironment;

struct LocalMapObject;

// Environment of the field move tasks (Teleport, Dig, Escape Rope, Flash ...); allocated with sizeof (0x30 on the DS).
struct FieldMoveTaskEnvironment {
    s32 state;                                         // 0x00
    s32 frame;                                         // 0x04
    BOOL followMonActive;                              // 0x08
    s32 type;                                          // 0x0C
    EventObjectMovementMan *playerMovement;            // 0x10
    EventObjectMovementMan *followMonMovement;         // 0x14
    void *cutIn;                                       // 0x18
    SysTask *cameraTask;                               // 0x1C
    struct LocalMapObject *mapObject;                  // 0x20
    FieldSystem *fieldSystem;                          // 0x24
    Pokemon *mon;                                      // 0x28
    Field3dObjectTask *unk_2C;                         // 0x2C
};

SysTask *ov02_0224B418(FieldSystem *fieldSystem, int gender);
BOOL ov02_0224B43C(SysTask *unk);
void ov02_0224B448(SysTask *unk);
struct FieldMoveTaskEnvironment *CreateFieldEscapeRopeTaskEnv(FieldSystem *fieldSystem, enum HeapID heapID);
struct FieldMoveTaskEnvironment *FieldMoveTask_CreateTeleportEnvironment(FieldSystem *fieldSystem, Pokemon *mon, u8 slotno, enum HeapID heapID);
struct FieldMoveTaskEnvironment *FieldMoveTask_CreateDigEnvironment(FieldSystem *fieldSystem, Pokemon *mon, u8 slotno, enum HeapID heapID);
BOOL Task_FieldEscapeRope(TaskManager *taskManager);
BOOL Task_FieldTeleport(TaskManager *taskManager);
BOOL Task_FieldDig(TaskManager *taskManager);
BOOL ov02_0224C1F8(TaskManager *taskManager);
void *ov02_0224C1D8(FieldSystem *fieldSystem, enum HeapID heapID, int a2);
void *ov02_0224C660(enum HeapID heapID, u32 size);
Field3dObjectTask *ov02_0224DDE0(FieldSystem *fieldSystem);
Field3dObjectTask *ov02_0224DDF4(FieldSystem *fieldSystem);
void ov02_0224DE08(Field3dObjectTask *task);
Field3dObjectTask *ov02_0224D598(FieldSystem *fieldSystem);
void ov02_0224D5AC(Field3dObjectTask *task);
Field3dObjectTask *ov02_0224D67C(FieldSystem *fieldSystem);
void ov02_0224D690(Field3dObjectTask *task);
Field3dObjectTask *ov02_0224D9A4(FieldSystem *fieldSystem);
void ov02_0224D9B8(Field3dObjectTask *task);

// TODO: define the struct, figure out what a1 is
SysTask *ov02_02249458(FieldSystem *fieldSystem, int mode, Pokemon *mon, int gender);
BOOL ov02_0224953C(SysTask *task);
void ov02_02249548(SysTask *task);

void ov02_02245B80(TaskManager *taskManager);
void ov02_022460AC(TaskManager *taskMan, u8 a1);
void ShowLegendaryWing(TaskManager *taskMan, u8 a1);
void ov02_022462E8(TaskManager *taskMan);
void PokecenterAnimCreate(FieldSystem *fieldSystem, u8 kind);
void ov02_0224BDE8(FieldSystem *fieldSystem, u8 direction, u8 length);
int ov02_0224CD38(PlayerProfile *profile, u16 a, u16 b, u16 c, u16 d, enum HeapID heapID);
int ov02_0224CD74(PlayerProfile *profile, u16 a, u16 b, u16 c, u16 d, enum HeapID heapID);
void ov02_0224BF58(FieldSystem *fieldSystem, u8 a1);
void ov02_0224BFC0(FieldSystem *fieldSystem, u8 a1);
void ov02_0224BFCC(FieldSystem *fieldSystem, u8 a1);
void ov02_0224CDB0(FieldSystem *fieldSystem, u8 a1);
void ov02_0224E074(FieldSystem *fieldSystem, u16 *p_ret, int a2, enum HeapID heapID);
void ov02_0224E0BC(LocalMapObject *obj1, LocalMapObject *obj2, TaskManager *taskManager);

typedef struct MapObjectSwapPos {
    s32 x;
    s32 z;
} MapObjectSwapPos;

// The environment of ov02_0224E0EC (two map objects walking past each other); the object pointers sit at DS 0x4 / 0x8.
typedef struct MapObjectSwapTask {
    s32 state;
    LocalMapObject *obj1;
    LocalMapObject *obj2;
    MapObjectSwapPos pos1;
    s32 dir1;
    MapObjectSwapPos pos2;
} MapObjectSwapTask; // size 0x20 on the DS

MapObjectSwapTask *ov02_0224E0D4(LocalMapObject *obj1, LocalMapObject *obj2);
void FieldSystem_FollowMonInteract(FieldSystem *fieldSystem);
BOOL ov02_02250780(FieldSystem *fieldSystem, u8 a1);
void ov02_022507B4(FieldSystem *fieldSystem, u32 a1);
void ov02_022508B4(FieldSystem *fieldSystem);
void ov02_022523B4(TaskManager *taskManager);

void ov02_BattleExit_HandleRoamerAction(FieldSystem *fieldSystem, BattleSetup *setup);
void ov02_02246714(TaskManager *man, u32 a1, u32 a2, u32 a3, u32 a4);
void ov02_022469B4(TaskManager *man, LocalMapObject *obj1, LocalMapObject *obj2);
void OpenAlphHiddenRoom(TaskManager *man, u32 a1);
int ov02_0224E340(FieldSystem *fieldSystem);

int ov02_0224E698(FieldSystem *fieldSystem);
u16 ov02_0224E754(FieldSystem *fieldSystem, u16 *varPointer);

SafariDecorationArgs *SafariDecoration_CreateArgs(FieldSystem *fieldSystem, enum HeapID heapID);

#endif // POKEHEARTGOLD_OVERLAY_02_H
