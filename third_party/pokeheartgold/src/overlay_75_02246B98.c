#include "global.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_70_gtsapplication_state.h"
#include "save.h"
#include "options.h"
#include "pm_string.h"
#include "overlay_manager.h"

struct OverlayManager;
struct OverlayManagerTemplate;
struct DWCstInetControl;
struct Options;
struct SaveData;
struct String;
struct WorldExchangeTrainer;
struct WorldExchangeTrainerError;
struct tagDWCWDSData;
typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);
void Heap_Free(void *ptr);
void UnloadOverlayByID(const FSOverlayID overlayID);
void sub_02034DE0(void);
void UnloadDwcOverlay(void);
void UnloadOVY38(void);
FS_EXTERN_OVERLAY(OVY_70);

void ov75_02246B98(UnkStruct_ov98_02246E88 *param0);
void ov75_02246BCC(UnkStruct_ov98_02246E88 *param0, int param1, int param2);
void ov75_02246BD8(UnkStruct_ov98_02246E88 *param0);

void ov75_02246B98(UnkStruct_ov98_02246E88 *param0) {
    if (param0->unk_80 == 1) {
        NNS_FndDestroyExpHeap(param0->unk_10);

        Heap_Free(param0->unk_0C);
        UnloadOVY38();
        UnloadDwcOverlay();
        sub_02034DE0();
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_70));

        param0->unk_80 = 0;
    }
}

void ov75_02246BCC(UnkStruct_ov98_02246E88 *param0, int param1, int param2) {
    param0->unk_8C = param1;
    param0->unk_90 = param2;
}

void ov75_02246BD8(UnkStruct_ov98_02246E88 *param0) {
    param0->unk_8C = 6;
}
