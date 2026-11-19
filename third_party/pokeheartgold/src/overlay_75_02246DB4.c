#include "global.h"
#include "heap.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct DWCstInetControl;

struct Options;

struct SaveData;

struct String;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;

struct tagDWCWDSData;

typedef struct SaveData SaveData;

typedef struct Options Options;

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

typedef struct String String;

typedef struct {
    int unk_00;
    int unk_04[5];
    int unk_18;
    String *unk_1C;
    Options *options;
    int unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
} UnkStruct_02089438;

Options *Save_PlayerData_GetOptionsAddr(SaveData *saveData);

extern UnkStruct_02089438 *sub_0208541C(enum HeapID heapID, int param1, int param2[], Options *options, u32 param4, u32 param5, u32 param6, u32 param7);

void *ov75_02246DB4(UnkStruct_ov98_02246E88 *param0);

u64 String_atoi(const String *src, BOOL *success);
void ov75_02246BCC(UnkStruct_ov98_02246E88 *param0, int param1, int param2);
void sub_02085438(UnkStruct_02089438 *param0);
void ov75_02246DFC(UnkStruct_ov98_02246E88 *param0);

void *ov75_02246DB4(UnkStruct_ov98_02246E88 *param0) {
    int v0[4];

    v0[0] = 3;
    v0[1] = 4;
    v0[2] = 0;
    v0[3] = 0;

    param0->unk_98 = sub_0208541C((enum HeapID)115, 7, v0, Save_PlayerData_GetOptionsAddr(param0->saveData), 5, 1, 1, param0->unk_7C);
    return param0->unk_98;
}

void ov75_02246DFC(UnkStruct_ov98_02246E88 *param0) {
    UnkStruct_02089438 *v0 = param0->unk_98;
    BOOL v1;
    param0->unk_10C = String_atoi(v0->unk_1C, &v1) % 10000;
    ((v1) ? (void)0 : GF_AssertFail());
    sub_02085438(v0);
    ov75_02246BCC(param0, 0, 0);
}
