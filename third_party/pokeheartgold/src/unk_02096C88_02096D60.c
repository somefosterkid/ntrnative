#include "global.h"
#include "field_system_def.h"

#include "error_handling.h"
#include "unk_02096C88.h"
#include "constants/heap.h"
#include "options.h"
#include "save.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "unk_struct_0209_c194.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

typedef void (*UnkFuncPtr_020F8E60)(int, int, void *, void *);

typedef struct {
    u32 unk_00;
    u8 unk_04[20];
} UnkStruct_0209BE84;

extern const UnkFuncPtr_020F8E60 _02108804[18];

struct GameStats;
struct JournalEntry;
struct UnkStruct_0205C22C_t;
struct UnkStruct_ov109_021D0F70_t;
struct UnkStruct_ov109_021D5140_t;
void *sub_02097018(UnkStruct_0209BDF8 *param0, int param1);
typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;
typedef struct JournalEntry JournalEntry;

void sub_02096D60(int param0, int param1, void *param2, void *param3) {
    UnkStruct_0209BE84 *v0 = param2;

    if (v0->unk_00 >= 18) {
        GF_ASSERT(FALSE);
        return;
    }

    _02108804[v0->unk_00](param0, param1, v0->unk_04, param3);
}

void sub_02096D80(int param0, int param1, void *param2, void *param3) {
    void *v0;
    UnkStruct_0209BDF8 *v1 = param3;

    v1->unk_46 |= 1 << param0;
    v0 = sub_02097018(v1, param0);
    memcpy(v0, param2, (236 * 6 + 4 * 2));
}

void sub_02096DA8(s32 param_1, u32 param_2, u8 *param_3, void *param_4) {
    if (param_1 == 0) {
        *(u32 *)((u8 *)param_4 + 0x28) = (u32)*param_3;
    }
    return;
}

void sub_02096DB4(int param0, int param1, void *param2, void *param3) {
    UnkStruct_0209BDF8 *v0 = param3;

    v0->unk_20++;
}
