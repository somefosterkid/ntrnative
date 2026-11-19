#include "global.h"
#include "field_system_def.h"
#include "unk_struct_02017498.h"
#include "save.h"
#include "overlay_manager.h"

struct OverlayManager;
struct OverlayManagerTemplate;
struct DWCstInetControl;
struct SaveData;
struct tagDWCWDSData;
typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);
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
typedef struct {
    FieldSystem *fieldSystem;
    SaveData *saveData;
    int unk_08;
    u8 unk_0C;
    u8 padding_0D[3];
} UnkStruct_0203E6C0;
void Heap_Free(void *ptr);
void sub_02034DE0(void);
void UnloadDwcOverlay(void);
void UnloadOVY38(void);

void sub_020194F8(UnkStruct_02017498 *param0);

void sub_020194F8(UnkStruct_02017498 *param0) {
    if (param0->unk_88 == 1) {
        NNS_FndDestroyExpHeap(param0->unk_1C);

        Heap_Free(param0->unk_18);
        UnloadOVY38();
        UnloadDwcOverlay();
        sub_02034DE0();

        param0->unk_88 = 0;
    }
}
