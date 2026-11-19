#include "global.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "sys_task.h"

struct ApplicationManager;

struct SaveData;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov66_02230C90_t;

typedef struct {
    s64 unk_00;
} UnkStruct_ov66_0222DCE0_sub1;

typedef struct UnkStruct_ov66_02230C90_t UnkStruct_ov66_02230C90;

typedef struct SaveData SaveData;

typedef struct ApplicationManager ApplicationManager;

typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);

typedef struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} ApplicationManagerTemplate;

struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

typedef struct {
    SaveData *saveData;
    WiFiPlazaSystem *unk_04;
    BOOL unk_08;
    UnkStruct_ov66_0222DCE0_sub1 *unk_0C;
    BOOL unk_10;
} UnkStruct_ov66_02231024;

void *Heap_Alloc(u32 heapID, u32 size);

typedef struct {
    void *unk_00;
} UnkStruct_ov66_02230E4C;

struct UnkStruct_ov66_02230C90_t {
    ApplicationManager *appMan;
    UnkStruct_ov66_02230E4C unk_04[13];
    u8 unk_38;
    u8 unk_39;
    u16 heapID;
    WiFiPlazaSystem *unk_3C;
    SaveData *saveData;
    UnkStruct_ov66_0222DCE0_sub1 *unk_44;
};

void ov45_0222D0FC(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

struct ApplicationManagerTemplate;
struct PlayerProfile;
typedef struct PlayerProfile PlayerProfile;
typedef struct {
    u32 unk_00;
    UnkStruct_ov66_0222E294 unk_04;
    UnkStruct_ov66_02232068 unk_08;
    BOOL unk_1C;
    PlayerProfile *unk_20[4];
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov66_02230DBC;
typedef struct {
    UnkStruct_ov66_02230DBC unk_00;
    SaveData *saveData;
    u8 unk_38;
    u8 unk_39;
    u8 padding[2];
} UnkStruct_ov117_02260440;
void ov45_0222CE78(UnkStruct_ov66_02230DBC *param0, u32 param1);
void ov45_0222D164(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov45_0222D0FC(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID) {
    UnkStruct_ov66_02231024 *v0;

    param1->unk_00 = Heap_Alloc(heapID, sizeof(UnkStruct_ov66_02231024));
    memset(param1->unk_00, 0, sizeof(UnkStruct_ov66_02231024));

    v0 = param1->unk_00;

    v0->saveData = param0->saveData;
    v0->unk_04 = param0->unk_3C;
    v0->unk_08 = param0->unk_39;
    v0->unk_0C = param0->unk_44;
}

struct Sprite;

struct SpriteResource;

struct SysTask;

struct SysTaskManager;

typedef struct SysTaskManager SysTaskManager;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

typedef struct Sprite Sprite;

void Heap_Free(Sprite *gfxElem);

typedef struct SpriteResource SpriteResource;

typedef struct ChooseStarterRotation {
    fx32 verticalOffset;
    fx32 verticalOffsetMultiplier;
    int frameCountMax;
    int frameCount;
} ChooseStarterRotation;

typedef struct ChooseStarterCursor {
    Sprite *sprite;
    SpriteResource *resources[6];
    VecFx32 position;
    SysTask *movementTask;
    ChooseStarterRotation starterRotation;
} ChooseStarterCursor;

void ov45_0222D130(ChooseStarterCursor *cursor);

void ov45_0222D130(ChooseStarterCursor *cursor) {
    Heap_Free(cursor->sprite);
}

s32 ov45_0222CDC4(void *, void **, s32 *);

extern s32 ov45_02254B64;

void ov45_0222D13C(void *arg0, void *arg1);

void ov45_0222D13C(void *arg0, void *arg1) {
    ov45_0222CDC4(arg0, arg1, &ov45_02254B64);
}

void ov45_0222CE2C(UnkStruct_ov66_02230C90 *param0, u32 param1);

int ov45_0222D14C(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

int ov45_0222D14C(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1) {
    UnkStruct_ov66_02231024 *v0 = param1->unk_00;

    if (v0->unk_10 == 0) {
        return 1;
    }

    ov45_0222CE2C(param0, 0);
    return 0;
}

void ov45_0222D164(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID) {
    UnkStruct_ov117_02260440 *v0;

    param1->unk_00 = Heap_Alloc(heapID, sizeof(UnkStruct_ov117_02260440));
    memset(param1->unk_00, 0, sizeof(UnkStruct_ov117_02260440));

    v0 = param1->unk_00;
    v0->unk_38 = 0;
    v0->unk_39 = 1;
    v0->saveData = param0->saveData;

    ov45_0222CE78(&v0->unk_00, heapID);
}
