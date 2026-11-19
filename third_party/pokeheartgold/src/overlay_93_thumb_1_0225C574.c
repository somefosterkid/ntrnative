#include "global.h"

#include "constants/heap.h"
#include "save.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_manager.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "unk_02037C94.h"

// ov91-93 view of the Wi-Fi Plaza launch record: 0x34 here (embedded by value), the header copy is 0x3c
typedef struct UnkStruct_ov66_02230DBC {
    u32 unk_00;
    UnkStruct_ov66_0222E294 unk_04;
    UnkStruct_ov66_02232068 unk_08;
    BOOL unk_1C;
    struct PlayerProfile *unk_20[4];
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov66_02230DBC;

struct OverlayManager;

struct OverlayManagerTemplate;

struct SaveData;

struct PlayerProfile;

struct UnkStruct_ov66_0222DFF8_t;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

OverlayManager *OverlayManager_New(const OverlayManagerTemplate *template, void *args, const enum HeapID heapID);

void OverlayManager_Delete(OverlayManager *appMan);

void *OverlayManager_GetData(OverlayManager *appMan);

void *OverlayManager_GetArgs(OverlayManager *appMan);

BOOL OverlayManager_Run(OverlayManager *appMan);

void ov00_021E6A4C(void);



int ov93_0225C768(OverlayManager *appMan, int *param1);

int ov93_0225CA8C(OverlayManager *appMan, int *param1);

int ov93_0225CD10(OverlayManager *appMan, int *param1);

int ov93_02262598(OverlayManager *appMan, int *param1);

int ov93_022625BC(OverlayManager *appMan, int *param1);

int AlphPuzzle_OverlayExitStep(OverlayManager *appMan, int *param1);

typedef struct {
    UnkStruct_ov66_02230DBC unk_00;
    SaveData *saveData;
    u8 unk_38;
    u8 unk_39;
    u8 padding[2];
} UnkStruct_ov117_02260440;

typedef struct {
    UnkStruct_ov114_0225C76C unk_00;
    UnkStruct_ov114_0225C9A8 unk_10;
    OverlayManager *appMan;
    u8 unk_2C[4];
    u8 unk_30;
    u8 unk_31;
    u8 padding_32[2];
    int unk_34;
    BOOL unk_38;
    u8 unk_3C;
    u8 unk_3D;
    u8 unk_3E;
    u8 padding_3F;
} UnkStruct_ov117_022605C0;


int sub_020347A0(void);

int sub_02037454(void);

void sub_02037AC0(u8 syncNo);

BOOL sub_02037B38(u8 syncState);

extern const OverlayManagerTemplate _022629F8;

extern const OverlayManagerTemplate ov93_02262A08;

BOOL ov93_0225C730(UnkStruct_ov117_022605C0 *param0);

int ov93_0225C574(OverlayManager *appMan, int *param1);

int ov93_0225C574(OverlayManager *appMan, int *param1) {
    UnkStruct_ov117_022605C0 *v0 = OverlayManager_GetData(appMan);
    UnkStruct_ov117_02260440 *v1 = OverlayManager_GetArgs(appMan);

    if (ov93_0225C730(v0) == 1) {
        return 1;
    }

    switch (*param1) {
    case 0:
        v0->unk_31 = 0;
        v0->appMan = OverlayManager_New(&ov93_02262A08, v0, HEAP_ID_117);
        (*param1)++;
        break;
    case 1:
        if (OverlayManager_Run(v0->appMan) == 1) {
            OverlayManager_Delete(v0->appMan);
            v0->appMan = NULL;
            (*param1)++;
        }
        break;
    case 2:
        v0->unk_31 = 1;
        v0->appMan = OverlayManager_New(&_022629F8, v0, HEAP_ID_117);
        (*param1)++;
        break;
    case 3:
        if (OverlayManager_Run(v0->appMan) == 1) {
            OverlayManager_Delete(v0->appMan);
            v0->appMan = NULL;
            (*param1)++;
        }
        break;
    case 4:
        if (v1->unk_38) {
            ov00_021E6A4C();
        }

        v0->unk_31 = 2;
        v0->appMan = OverlayManager_New(&ov93_02262A08, v0, HEAP_ID_117);
        (*param1)++;
        break;
    case 5:
        if (OverlayManager_Run(v0->appMan) == 1) {
            OverlayManager_Delete(v0->appMan);
            v0->appMan = NULL;

            if (v0->unk_38 == 1) {
                (*param1) = 0;
            } else {
                (*param1)++;
            }
        }
        break;
    case 6:
        sub_020398D4(0, 1);
        sub_02037AC0(222);
        (*param1)++;
        break;
    case 7:
        if ((sub_02037B38(222) == 1) || (sub_02037454() < sub_020347A0())) {
            (*param1)++;
        }

        break;
    case 8:
    default:
        return 1;
    }

    return 0;
}
