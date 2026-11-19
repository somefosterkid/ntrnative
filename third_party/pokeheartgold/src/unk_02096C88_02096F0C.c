#include "global.h"
#include "field_system_def.h"

#include "player_data.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_02096C88.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "unk_struct_0209_c194.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

struct GameStats;

struct JournalEntry;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

BOOL sub_02096D4C(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

void sub_02096F0C(int param0, int param1, void *param2, void *param3) {
    u8 v0;
    UnkStruct_0209BDF8 *v1 = param3;

    if (sub_0203769C() == 0) {
        v0 = param0;
        sub_02096D4C(v1, 0, &v0, 1);
    }
}

void sub_02096F2C(int param0, int param1, void *param2, void *param3) {
    u16 *v0 = param2;
    UnkStruct_0209BDF8 *v1 = param3;

    v1->unk_40 |= *v0;
}

void sub_02096F3C(int param0, int param1, void *param2, void *param3) {
    UnkStruct_0209BDF8 *v0 = param3;
    v0->unk_42 |= 1 << param0;
}

void ov85_021E8680(void *, void *);

void sub_02096F50(u32 param_1, u32 param_2, void *param_3, u8 **param_4) {
    ov85_021E8680(*(void **)(*param_4 + 0x34), param_3);
    return;
}

void ov85_021E86AC(UnkStruct_ov109_021D0F70 *, u32);

void sub_02096F60(u32 param_1, u32 param_2, u32 *param_3, u8 **param_4) {
    ov85_021E86AC(*(void **)(*param_4 + 0x34), *param_3);
    return;
}

void ov85_021E85C4(UnkStruct_ov109_021D0F70 *, u32);

void sub_02096F70(u32 param_1, u32 param_2, u8 *param_3, u8 **param_4) {
    ov85_021E85C4(*(void **)(*param_4 + 0x34), *param_3);
    return;
}

void ov85_021E85CC(void *, void *);

void sub_02096F80(u32 param_1, u32 param_2, void *param_3, u8 **param_4);

void sub_02096F80(u32 param_1, u32 param_2, void *param_3, u8 **param_4) {
    u16 uVar1;

    uVar1 = sub_0203769C();
    if (uVar1 != 0) {
        ov85_021E85CC(*(void **)(*param_4 + 0x34), param_3);
    }
    return;
}

void ov85_021E8740(UnkStruct_ov109_021D0F70 *, u32, u32);

void sub_02096F9C(u32 param_1, u32 param_2, u32 *param_3, u8 **param_4) {
    ov85_021E8740(*(void **)(*param_4 + 0x34), param_1, *param_3);
    return;
}

void ov85_021E8748(void *, void *);

void sub_02096FAC(u32 param_1, u32 param_2, void *param_3, u8 **param_4) {
    ov85_021E8748(*(void **)(*param_4 + 0x34), param_3);
    return;
}

void sub_02096FBC(int param0, int param1, void *param2, void *param3) {
    u32 v0 = 1 << param0;
    UnkStruct_0209BDF8 *v1 = param3;

    v1->unk_48 |= v0;
}

void sub_02096FD0(int param0, int param1, void *param2, void *param3) {
    u32 v0 = 1 << param0;
    UnkStruct_0209BDF8 *v1 = param3;

    v1->unk_4A |= v0;
}

u32 sub_02096FE4(void) {
    return 0x18;
}

u32 sub_02096FE8(void) {
    return 0x590;
}

s32 sub_02096FF0(s32 param_1, u8 *param_2) {
    return *(s32 *)(param_2 + 0x4c) + param_1 * 0x590;
}

s32 sub_02096FFC(void);

s32 sub_02096FFC(void) {
    PlayerProfile *pPVar1;
    u32 a0;
    s32 iVar2;

    iVar2 = 0;
    a0 = 0;
    do {
        pPVar1 = sub_02034818(a0);
        if (pPVar1 != NULL) {
            iVar2 = iVar2 + 1;
        }
        a0 = a0 + 1;
    } while ((s32)a0 < 5);
    return iVar2;
}

s32 sub_02097018(UnkStruct_0209BDF8 *param_1, s32 param_2);

s32 sub_02097018(UnkStruct_0209BDF8 *param_1, s32 param_2) {
    return *((s32 *)&param_1->unk_50) + param_2 * 0x590;
}
