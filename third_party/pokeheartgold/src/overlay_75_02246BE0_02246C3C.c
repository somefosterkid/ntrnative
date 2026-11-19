#include "global.h"
struct UnkStruct_ov98_02246E88;

#include "heap.h"
#include "unk_0203170C.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_70_gtsapplication_state.h"
#include "options.h"
#include "pm_string.h"
#include "overlay_manager.h"

s32 ov75_02249780(void *, s8 *, s32);

void ov75_02246C3C(struct UnkStruct_ov98_02246E88 *arg0);

void ov75_02246C3C(UnkStruct_ov98_02246E88 *arg0) {
    s8 *v0;

    v0 = Heap_Alloc(HEAP_ID_115, 0x64U);
    ov75_02249780(*((void **)&arg0->unk_114), v0, 0x73);
    sub_0203175C(arg0->saveData, v0);
    Heap_Free(v0);
}

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

void ov75_02246C68(UnkStruct_ov98_02246E88 *param0);

void ov75_02246C68(UnkStruct_ov98_02246E88 *param0) {
    sub_02031780(param0->saveData, 1, param0->unk_7C);
    sub_02031780(param0->saveData, 2, param0->unk_10C);
}
