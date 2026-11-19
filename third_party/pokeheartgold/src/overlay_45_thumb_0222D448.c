#include "global.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "sys_task.h"

struct OverlayManager;

typedef struct OverlayManager OverlayManager;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct OverlayManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} OverlayManagerTemplate;

struct OverlayManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    OverlayManager *parent;
    OverlayManager *child;
};

BOOL ov45_0222D448(OverlayManager *appMan, int *state);

s32 ov45_0222CDC4(void *, void **, s32 *);
extern s32 ov45_02254B44;
void ov45_0222D4CC(void *arg0, void *arg1);
s32 ov45_0222A33C(void *);
s32 ov45_0222CE2C(void *, s32);
s32 ov45_0222D4DC(void *arg0);

BOOL ov45_0222D448(OverlayManager *appMan, int *state) {
    return TRUE;
}

struct SaveData;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov66_02230C90_t;

typedef struct {
    s64 unk_00;
} UnkStruct_ov66_0222DCE0_sub1;

typedef struct UnkStruct_ov66_02230C90_t UnkStruct_ov66_02230C90;

typedef struct SaveData SaveData;

typedef struct {
    SaveData *saveData;
    WiFiPlazaSystem *unk_04;
    int unk_08;
} UnkStruct_ov66_02231374;

void *Heap_Alloc(u32 heapID, u32 size);

typedef struct {
    void *unk_00;
} UnkStruct_ov66_02230E4C;

struct UnkStruct_ov66_02230C90_t {
    OverlayManager *appMan;
    UnkStruct_ov66_02230E4C unk_04[13];
    u8 unk_38;
    u8 unk_39;
    u16 heapID;
    WiFiPlazaSystem *unk_3C;
    SaveData *saveData;
    UnkStruct_ov66_0222DCE0_sub1 *unk_44;
};

void ov45_0222D44C(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov45_0222D44C(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID) {
    UnkStruct_ov66_02231374 *v0;

    param1->unk_00 = Heap_Alloc(heapID, sizeof(UnkStruct_ov66_02231374));
    memset(param1->unk_00, 0, sizeof(UnkStruct_ov66_02231374));

    v0 = param1->unk_00;
    v0->saveData = param0->saveData;
    v0->unk_04 = param0->unk_3C;
    v0->unk_08 = 0;
}

void ov45_0222D484(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov45_0222D484(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID) {
    UnkStruct_ov66_02231374 *v0;

    param1->unk_00 = Heap_Alloc(heapID, sizeof(UnkStruct_ov66_02231374));
    memset(param1->unk_00, 0, sizeof(UnkStruct_ov66_02231374));

    v0 = param1->unk_00;
    v0->saveData = param0->saveData;
    v0->unk_04 = param0->unk_3C;
    v0->unk_08 = 1;
}

struct Sprite;

struct SpriteResource;

struct SysTask;

struct SysTaskQueue;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
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

void ov45_0222D4C0(ChooseStarterCursor *cursor);

void ov45_0222D4C0(ChooseStarterCursor *cursor) {
    Heap_Free(cursor->sprite);
}

void ov45_0222D4CC(void *arg0, void *arg1) {
    ov45_0222CDC4(arg0, arg1, &ov45_02254B44);
}

s32 ov45_0222D4DC(void *arg0) {
    if (ov45_0222A33C(*(void **)((u8 *)arg0 + 0x3C)) != 0) {
        ov45_0222CE2C(arg0, 0xA);
    } else {
        ov45_0222CE2C(arg0, 0);
    }
    return 0;
}
