#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "save.h"
#include "unk_02034354.h"
#include "unk_020379A0.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "unk_02037C94.h"

struct OverlayManager;

struct PlayerProfile;

struct UnkStruct_ov114_0225D678_t;

struct UnkStruct_ov114_0225E854_t;

struct UnkStruct_ov115_0226095C_t;

struct UnkStruct_ov115_02260BE4_t;

struct UnkStruct_ov115_02260D78_t;

struct UnkStruct_ov66_0222DFF8_t;

typedef struct UnkStruct_ov115_0226095C_t UnkStruct_ov115_0226095C;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void *OverlayManager_GetData(OverlayManager *appMan);

void OverlayManager_FreeData(OverlayManager *appMan);

void *OverlayManager_GetArgs(OverlayManager *appMan);

typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

BOOL ov90_02258B54(UnkStruct_ov114_0225C76C *param0);

typedef struct UnkStruct_ov115_02260BE4_t UnkStruct_ov115_02260BE4;

typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
} UnkStruct_ov115_02262DC4;

typedef struct {
    u32 unk_00;
    UnkStruct_ov115_02262DC4 *unk_04;
    const PlayerProfile *unk_08[4];
    u32 unk_18[4];
} UnkStruct_ov115_02262F50;

typedef struct UnkStruct_ov115_02260D78_t UnkStruct_ov115_02260D78;

typedef struct {
    UnkStruct_ov66_02230DBC unk_00;
    SaveData *saveData;
    u8 unk_38;
    u8 unk_39;
    u8 padding[2];
} UnkStruct_ov115_02260440;


int sub_02037454(void);

struct UnkStruct_ov115_0226095C_t {
    UnkStruct_ov114_0225D678 *unk_00;
    UnkStruct_ov114_0225E854 *unk_04;
    UnkStruct_ov114_0225C76C unk_08;
    UnkStruct_ov114_0225C9A8 unk_18;
    UnkStruct_ov115_02260BE4 *unk_30;
    UnkStruct_ov115_02260D78 *unk_34;
    BOOL unk_38;
    BOOL unk_3C;
    BOOL unk_40;
    u16 unk_44;
    u16 unk_46;
    UnkStruct_ov115_02262F50 unk_48;
    u32 unk_70;
    u32 unk_74;
    u32 unk_78;
    u32 unk_7C;
    BOOL unk_80;
    u8 unk_84[4];
};

void ov91_0225CC38(UnkStruct_ov115_0226095C *param0);

int ov91_0225C9EC(OverlayManager *appMan, int *param1);

void ov91_0225CA64(void *arg0);
void ov91_0225CA6C(void *arg0);

int ov91_0225C9EC(OverlayManager *appMan, int *param1) {
    UnkStruct_ov115_0226095C *v0 = OverlayManager_GetData(appMan);
    UnkStruct_ov115_02260440 *v1 = OverlayManager_GetArgs(appMan);
    BOOL v2;

    switch (*param1) {
    case 0:
        v2 = ov90_02258B54(&v0->unk_08);

        ov91_0225CC38(v0);

        OverlayManager_FreeData(appMan);
        Heap_Destroy(HEAP_ID_106);
        sub_020398D4(0, 1);

        if (v2 == 1) {
            return 1;
        }

        sub_02037AC0(4);
        (*param1)++;
        break;
    case 1:
        if (sub_02037B38(4) || (sub_02037454() < sub_020347A0())) {
            return 1;
        }
        break;
    default:
        break;
    }

    return 0;
}

void ov91_0225CA64(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x38) = 1;
}

void ov91_0225CA6C(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x3C) = 1;
}
