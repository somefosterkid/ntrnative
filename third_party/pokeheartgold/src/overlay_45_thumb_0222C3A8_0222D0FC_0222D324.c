#include "global.h"

#include "poke_overlay.h"

struct ApplicationManager;

struct SaveData;

struct PlayerProfile;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov66_02230C90_t;

typedef struct {
    s64 unk_00;
} UnkStruct_ov66_0222DCE0_sub1;

typedef struct UnkStruct_ov66_0222DFF8_t WiFiPlazaSystem;

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
    u8 unk_00[4];
} UnkStruct_ov66_0222E294;

typedef struct {
    u8 unk_00[20];
} UnkStruct_ov66_02232068;

typedef struct PlayerProfile PlayerProfile;

void ov45_0222A430(WiFiPlazaSystem *param0, BOOL param1);

void ov45_0222A72C(WiFiPlazaSystem *param0, u32 param1);

typedef struct {
    u32 unk_00;
    UnkStruct_ov66_0222E294 unk_04;
    UnkStruct_ov66_02232068 unk_08;
    BOOL unk_1C;
    PlayerProfile *unk_20[4];
    WiFiPlazaSystem *unk_30;
    u32 unk_34;
    u8 unk_38;
    u8 unk_39;
    u8 pad_3A[2];
} UnkStruct_ov66_02230DBC;

typedef struct {
    UnkStruct_ov66_02230DBC unk_00;
    SaveData *saveData;
    u8 unk_38;
    u8 unk_39;
    u8 padding[2];
} UnkStruct_ov117_02260440;

FS_EXTERN_OVERLAY(OVY_90);

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

void ov45_0222CE2C(UnkStruct_ov66_02230C90 *param0, u32 param1);

int ov45_0222D324(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

int ov45_0222D324(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1) {
    FS_EXTERN_OVERLAY(OVY_90);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_90));

    {
        UnkStruct_ov117_02260440 *v0;

        v0 = param1->unk_00;

        ov45_0222A430(param0->unk_3C, v0->unk_00.unk_1C);
        ov45_0222A72C(param0->unk_3C, v0->unk_00.unk_00);
    }

    ov45_0222CE2C(param0, 0);

    return 0;
}

typedef struct {
    WiFiPlazaSystem *unk_00;
    u8 unk_04;
    u8 padding_05[3];
} UnkStruct_ov66_0223127C;

void *Heap_Alloc(u32 heapID, u32 size);

void ov45_0222D354(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov45_0222D354(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID) {
    UnkStruct_ov66_0223127C *v0;

    param1->unk_00 = Heap_Alloc(heapID, sizeof(UnkStruct_ov66_0223127C));
    memset(param1->unk_00, 0, sizeof(UnkStruct_ov66_0223127C));

    v0 = param1->unk_00;
    v0->unk_00 = param0->unk_3C;
}

struct Sprite;

struct SpriteResource;

struct SysTask;

struct SysTaskManager;

typedef struct SysTask SysTask;

typedef struct SysTaskManager SysTaskManager;

typedef void (*SysTaskFunc)(SysTask *, void *);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
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

void ov45_0222D380(ChooseStarterCursor *cursor);

void ov45_0222D380(ChooseStarterCursor *cursor) {
    Heap_Free(cursor->sprite);
}
