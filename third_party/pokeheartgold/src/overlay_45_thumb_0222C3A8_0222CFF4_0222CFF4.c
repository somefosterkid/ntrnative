#include "global.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_88_unk_struct_ov66_02231428.h"
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
    u16 unk_00;
    u8 unk_02;
    u8 unk_03_0 : 4;
    u8 unk_03_4 : 4;
} UnkStruct_ov66_02231560_sub1;

typedef struct {
    UnkStruct_ov66_02231560_sub1 unk_00[50];
} UnkStruct_ov66_02231560;

typedef struct {
    const UnkStruct_ov66_02231560 *unk_00;
    SaveData *saveData;
    UnkStruct_ov66_02231428 unk_08;
    WiFiPlazaSystem *unk_0C;
} UnkStruct_ov66_02230F50;

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

void ov45_0222CFF4(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov45_0222CFF4(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID) {
    UnkStruct_ov66_02230F50 *v0;

    param1->unk_00 = Heap_Alloc(heapID, sizeof(UnkStruct_ov66_02230F50));
    memset(param1->unk_00, 0, sizeof(UnkStruct_ov66_02230F50));

    v0 = param1->unk_00;

    v0->saveData = param0->saveData;
    v0->unk_0C = param0->unk_3C;
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

void ov45_0222D01C(ChooseStarterCursor *cursor);

void ov45_0222D01C(ChooseStarterCursor *cursor) {
    Heap_Free(cursor->sprite);
}

u8 *ov45_0222A214(u8 *);

s32 *ov45_0222A22C(const WiFiPlazaSystem *);

s32 ov45_0222CDC4(void *, void **, s32 *);

extern s32 ov45_02254B84;

void ov45_0222D028(void *arg0, void **arg1);

void ov45_0222D028(void *arg0, void **arg1) {
    void *v1;

    v1 = *arg1;
    *(void **)v1 = ov45_0222A214(*(void **)((u8 *)arg0 + 0x3C));
    *(s32 *)((u8 *)v1 + 8) = (s32)*ov45_0222A22C(*(void **)((u8 *)arg0 + 0x3C));
    ov45_0222CDC4(arg0, arg1, &ov45_02254B84);
}
