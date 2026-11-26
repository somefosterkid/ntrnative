#include "global.h"

#include "overlay_manager.h"
#include "field_system_def.h"

struct DWCstInetControl;


struct SaveData;

struct tagDWCWDSData;

typedef enum {
    DWC_WDS_STATE_IDLE = 0,
    DWC_WDS_STATE_PROCESS = 1,
    DWC_WDS_STATE_COMPLETED = 2,
    DWC_WDS_STATE_FAILED = 3,
    DWC_WDS_STATE_ERROR = 4
} DWCWDSState;

typedef struct tagDWCWDSData {
    u8 ssid[32];
    u8 wep[32];
    int wepMode;
    char apnum[10];
} DWCWDSData;

typedef struct DWCstInetControl {
    volatile int ac_state;
    u16 state;
    u16 online;
    u16 dmaNo;
    u16 powerMode;
    u8 wontExecuteWDS;
    DWCWDSState processWDSstate;
    DWCWDSData nspotInfo;

} DWCInetControl;

extern WMLinkLevel DWC_GetLinkLevel(void);

extern BOOL DWC_UpdateConnection(void);

typedef enum {
    DWC_ALLOCTYPE_AUTH,
    DWC_ALLOCTYPE_AC,
    DWC_ALLOCTYPE_BM,
    DWC_ALLOCTYPE_UTIL,
    DWC_ALLOCTYPE_BASE,
    DWC_ALLOCTYPE_GS,
    DWC_ALLOCTYPE_ND,

    DWC_ALLOCTYPE_ENC,
    DWC_ALLOCTYPE_MAIL,

    DWC_ALLOCTYPE_LAST
} DWCAllocType;

typedef void *(*DWCAllocEx)(DWCAllocType name, u32 size, int align);

typedef void (*DWCFreeEx)(DWCAllocType name, void *ptr, u32 size);

void DWC_SetMemFunc(DWCAllocEx alloc, DWCFreeEx free);


typedef struct SaveData SaveData;

typedef struct {
    FieldSystem *fieldSystem;
    SaveData *saveData;
    int unk_08;
    u8 unk_0C;
    u8 padding_0D[3];
} UnkStruct_0203E6C0;

typedef struct {
    UnkStruct_0203E6C0 *unk_00;
    BOOL unk_10;
    OverlayManager *appMan;
    void *unk_18;
    NNSFndHeapHandle unk_1C;
    DWCInetControl unk_20;
    int unk_88;
    BOOL unk_8C;
} UnkStruct_02017498;

int ov39_02228140(OverlayManager *appMan, int *param1);

int ov61_0222C0F8(OverlayManager *appMan, int *param1);


void sub_0203A930(int networkStrength);

BOOL sub_02034DB8(void);

const OverlayManagerTemplate *sub_02087E10(int param0);

extern NNSFndHeapHandle _021D1108;

void sub_020194B4(UnkStruct_02017498 *param0);

void *sub_02019520(DWCAllocType param0, u32 param1, int param2);

void sub_02019548(DWCAllocType param0, void *param1, u32 param2);

extern const OverlayManagerTemplate _020F6288;

int sub_0201935C(OverlayManager *appMan, int *param1);

int sub_0201935C(OverlayManager *appMan, int *param1) {
    UnkStruct_02017498 *v0 = OverlayManager_GetData(appMan);

    switch (*param1) {
    case 0:
        sub_020194B4(v0);
        *param1 = 1;
        break;
    case 1:
        if (sub_02034DB8()) {
            _021D1108 = v0->unk_1C;

            DWC_SetMemFunc(sub_02019520, sub_02019548);

            v0->unk_88 = 1;
            (*param1)++;
        }
        break;
    case 2:
        v0->appMan = OverlayManager_New(&_020F6288, v0, (enum HeapID)123);
        (*param1)++;
        break;
    case 3:
        if (OverlayManager_Run(v0->appMan) == 1) {
            OverlayManager_Delete(v0->appMan);

            if (v0->unk_8C == 1) {
                v0->unk_10 = 1;
                (*param1)++;
            } else {
                *param1 = 8;
            }
        }
        break;
    case 4: {
        const OverlayManagerTemplate *v1;

        v1 = sub_02087E10(v0->unk_00->unk_0C);
        v0->appMan = OverlayManager_New(v1, v0->unk_00->fieldSystem, (enum HeapID)123);
        (*param1)++;
    } break;
    case 5:
        if (OverlayManager_Run(v0->appMan) == 1) {
            OverlayManager_Delete(v0->appMan);
            (*param1)++;
        }
        break;
    case 6:
        v0->appMan = OverlayManager_New(&_020F6288, v0, (enum HeapID)123);
        (*param1)++;
        break;
    case 7:
        if (OverlayManager_Run(v0->appMan) == 1) {
            OverlayManager_Delete(v0->appMan);
            v0->unk_10 = 0;
            (*param1)++;
        }
        break;
    case 8:
        return 1;
    }

    if ((v0->unk_88 == 1) && (v0->unk_10 == 1) && (v0->unk_8C == 1)) {
        DWC_UpdateConnection();
        sub_0203A930(WM_LINK_LEVEL_3 - DWC_GetLinkLevel());
    }

    return 0;
}
