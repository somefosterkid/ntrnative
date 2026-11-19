#include "global.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_88_unk_struct_ov66_02231428.h"
#include "sys_task.h"

s32 ov45_0222A33C(void *);

s32 ov45_0222CE2C(void *, s32);

s32 ov45_0222D054(void *arg0);

s32 ov45_0222CDC4(void *, void **, s32 *);
extern s32 ov45_02254B74;
void ov45_0222D0C8(void *arg0, void *arg1);
s32 ov45_0222D0D8(void *arg0);

s32 ov45_0222D054(void *arg0) {
    if (ov45_0222A33C(*(void **)((u8 *)arg0 + 0x3C)) != 0) {
        ov45_0222CE2C(arg0, 0xA);
    } else {
        ov45_0222CE2C(arg0, 0);
    }
    return 0;
}

struct ApplicationManager;

struct SaveData;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov66_02230C90_t;

struct UnkStruct_ov66_0223177C_t;

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

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

UnkStruct_ov66_0223177C *ov45_0222A210(WiFiPlazaSystem *param0);

const UnkStruct_ov66_02231428 *ov45_0222A22C(const WiFiPlazaSystem *param0);

typedef struct {
    UnkStruct_ov66_0223177C *unk_00;
    const UnkStruct_ov66_02231428 *unk_04;
    WiFiPlazaSystem *unk_08;
} WiFiPlazaApplicationArgs;

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

void ov45_0222D078(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov45_0222D078(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID) {
    WiFiPlazaApplicationArgs *v0;

    param1->unk_00 = Heap_Alloc(heapID, sizeof(WiFiPlazaApplicationArgs));
    memset(param1->unk_00, 0, sizeof(WiFiPlazaApplicationArgs));

    v0 = param1->unk_00;

    v0->unk_00 = ov45_0222A210(param0->unk_3C);
    v0->unk_04 = ov45_0222A22C(param0->unk_3C);
    v0->unk_08 = param0->unk_3C;
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

void ov45_0222D0BC(ChooseStarterCursor *cursor);

void ov45_0222D0BC(ChooseStarterCursor *cursor) {
    Heap_Free(cursor->sprite);
}

void ov45_0222D0C8(void *arg0, void *arg1) {
    ov45_0222CDC4(arg0, arg1, &ov45_02254B74);
}

s32 ov45_0222D0D8(void *arg0) {
    if (ov45_0222A33C(*(void **)((u8 *)arg0 + 0x3C)) != 0) {
        ov45_0222CE2C(arg0, 0xA);
    } else {
        ov45_0222CE2C(arg0, 0);
    }
    return 0;
}
