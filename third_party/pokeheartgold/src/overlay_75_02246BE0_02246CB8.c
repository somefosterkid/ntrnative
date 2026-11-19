#include "global.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_70_gtsapplication_state.h"
#include "save.h"
#include "options.h"
#include "pm_string.h"
#include "overlay_manager.h"

s32 ov75_02249780(void *, void *, s32);

void ov75_02246CB8(void *arg0);

void ov75_02246CB8(void *arg0) {
    ov75_02249780(*((void **)&((UnkStruct_ov98_02246E88 *)arg0)->unk_114), arg0 + 0xBC, 0x73);
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

void ov75_02246CCC(UnkStruct_ov98_02246E88 *param0);

void ov75_02246CCC(UnkStruct_ov98_02246E88 *param0) {
    param0->unk_9C.unk_5A = param0->unk_10C;
}

void ov75_02246CD8(UnkStruct_ov98_02246E88 *arg0, s16 arg1);

void ov75_02246CD8(UnkStruct_ov98_02246E88 *arg0, s16 arg1) {
    *((s16 *)&arg0->unk_7C) = arg1;
}

u32 ov75_02246CE0(UnkStruct_ov98_02246E88 *param0);

u32 ov75_02246CE0(UnkStruct_ov98_02246E88 *param0) {
    return param0->unk_10C;
}

u32 ov75_02246CE8(UnkStruct_ov98_02246E88 *param0);

u32 ov75_02246CE8(UnkStruct_ov98_02246E88 *param0) {
    return param0->unk_108;
}

void ov75_02246CF0(UnkStruct_ov98_02246E88 *param0, int param1);

void ov75_02246CF0(UnkStruct_ov98_02246E88 *param0, int param1) {
    param0->unk_110 = param1;
}

int ov75_02246CF8(UnkStruct_ov98_02246E88 *param0);

int ov75_02246CF8(UnkStruct_ov98_02246E88 *param0) {
    return param0->unk_110;
}

void *ov75_02246D00(UnkStruct_ov98_02246E88 *param0);

void *ov75_02246D00(UnkStruct_ov98_02246E88 *param0) {
    return param0;
}

void ov75_02246D04(UnkStruct_ov98_02246E88 *param0);

void ov75_02246D04(UnkStruct_ov98_02246E88 *param0) {
}
