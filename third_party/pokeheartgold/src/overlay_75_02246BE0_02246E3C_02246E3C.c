#include "global.h"
#include "heap.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "save.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"

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

extern UnkStruct_02089438 *sub_02085400(enum HeapID heapID, int param1, int param2[], Options *options, u32 param4, u32 param5);

void *ov75_02246E3C(UnkStruct_ov98_02246E88 *param0);

void *ov75_02246E3C(UnkStruct_ov98_02246E88 *param0) {
    int v0[4];

    v0[0] = 4;
    v0[1] = 0;
    v0[2] = 0;
    v0[3] = 0;

    param0->unk_98 = sub_02085400((enum HeapID)115, 4, v0, Save_PlayerData_GetOptionsAddr(param0->saveData), 6, 0);
    return param0->unk_98;
}
