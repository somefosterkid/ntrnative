#include "global.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"

struct ApplicationManager;

struct ApplicationManagerTemplate;

struct SaveData;

struct PlayerProfile;

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
} UnkStruct_ov66_02230DBC;

typedef struct {
    UnkStruct_ov66_02230DBC unk_00;
    SaveData *saveData;
    u8 unk_38;
    u8 unk_39;
    u8 padding[2];
} UnkStruct_ov117_02260440;

void UnloadOverlayByID(const FSOverlayID overlayID);

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

int ov45_0222D27C(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

int ov45_0222D27C(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1) {
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

void *Heap_Alloc(u32 heapID, u32 size);

void ov45_0222CE78(UnkStruct_ov66_02230DBC *param0, u32 param1);

void ov45_0222D2AC(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov45_0222D2AC(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID) {
    UnkStruct_ov117_02260440 *v0;

    param1->unk_00 = Heap_Alloc(heapID, sizeof(UnkStruct_ov117_02260440));
    memset(param1->unk_00, 0, sizeof(UnkStruct_ov117_02260440));

    v0 = param1->unk_00;
    v0->unk_38 = 0;
    v0->unk_39 = 1;
    v0->saveData = param0->saveData;

    ov45_0222CE78(&v0->unk_00, heapID);
}
