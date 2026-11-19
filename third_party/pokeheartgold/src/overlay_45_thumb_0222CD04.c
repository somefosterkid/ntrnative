#include "global.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"

typedef struct WiFiPlazaTrainerAppearance {
    u16 graphicsID;
    u16 gender;
} WiFiPlazaTrainerAppearance;

extern const WiFiPlazaTrainerAppearance ov45_02254A84[8 * 2];

u32 ov45_0222CD04(u32 trainerAppearance);

struct ApplicationManager;
struct ApplicationManagerTemplate;
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
void ov45_0222CDE4(UnkStruct_ov66_02230C90 *param0, u32 param1, u32 heapID);
UnkStruct_ov66_02230C90 *ov45_0222CD1C(BOOL param0, SaveData *saveData, UnkStruct_ov66_0222DCE0_sub1 *param2, WiFiPlazaSystem *param3, u32 heapID);

u32 ov45_0222CD04(u32 trainerAppearance) {
    if (trainerAppearance < ((int)(sizeof(ov45_02254A84) / sizeof(*(ov45_02254A84))))) {
        return ov45_02254A84[trainerAppearance].graphicsID;
    }

    return 0xffff;
}

UnkStruct_ov66_02230C90 *ov45_0222CD1C(BOOL param0, SaveData *saveData, UnkStruct_ov66_0222DCE0_sub1 *param2, WiFiPlazaSystem *param3, u32 heapID) {
    UnkStruct_ov66_02230C90 *v0 = Heap_Alloc(heapID, sizeof(UnkStruct_ov66_02230C90));
    memset(v0, 0, sizeof(UnkStruct_ov66_02230C90));

    v0->unk_3C = param3;
    v0->saveData = saveData;
    v0->unk_44 = param2;
    v0->unk_39 = param0;
    v0->heapID = heapID;

    {
        int v1;

        for (v1 = 0; v1 < 13; v1++) {
            ov45_0222CDE4(v0, v1, heapID);
        }
    }

    return v0;
}
