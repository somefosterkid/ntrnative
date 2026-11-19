#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "options.h"
#include "overlay_75.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "unk_02037C94.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_70_gtsapplication_state.h"
#include "save_vars_flags.h"

struct ApplicationManager;

typedef struct ApplicationManager ApplicationManager;

struct ApplicationManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

void ov75_02246BCC(UnkStruct_ov98_02246E88 *param0, int param1, int param2);

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

void sub_02085438(UnkStruct_02089438 *param0);

void ov75_02246E78(UnkStruct_ov98_02246E88 *param0);

void ov75_02246E78(UnkStruct_ov98_02246E88 *param0) {
    UnkStruct_02089438 *v0 = param0->unk_98;
    BOOL v1;
    param0->unk_108 = String_atoi(v0->unk_1C, &v1);
    GF_ASSERT(v1);
    sub_02085438(v0);
    ov75_02246BCC(param0, 0, 0);
}

typedef struct {
    SaveData *saveData;
    int unk_04;
    int unk_08;
} UnkStruct_ov98_02247168;

void *ov75_02246EAC(UnkStruct_ov98_02246E88 *arg0) {
    UnkStruct_ov98_02247168 *dest;

    dest = Heap_Alloc(HEAP_ID_115, sizeof(UnkStruct_ov98_02247168));
    MI_CpuFill8(dest, 0U, sizeof(UnkStruct_ov98_02247168));
    dest->saveData = arg0->saveData;
    dest->unk_04 = 1;
    arg0->unk_80 = 0;
    arg0->unk_98 = dest;
    return arg0->unk_98;
}

struct SaveVarsFlags;

SaveVarsFlags *Save_VarsFlags_Get(SaveData *saveData);

void SetFlag970(SaveVarsFlags *varsFlags);

void ov75_02246EDC(UnkStruct_ov98_02246E88 *param0);

void ov75_02246EDC(UnkStruct_ov98_02246E88 *param0) {
    UnkStruct_ov98_02247168 *v0 = param0->unk_98;

    if (sub_0203A05C(param0->saveData)) {
        SetFlag970(Save_VarsFlags_Get(param0->saveData));
    }

    Heap_Free(param0->unk_98);
    ov75_02246BCC(param0, 0, 0);
}
