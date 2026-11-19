#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "camera.h"
#include "error_handling.h"
#include "filesystem.h"
#include "heap.h"
#include "overlay_85.h"
#include "party.h"
#include "pokemon.h"
#include "trainer_memo.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "game_stats.h"
#include "unk_struct_0209_c194.h"

void ov85_021E8530(fx32 *param0, fx32 param1) {
    (*param0) += param1;

    while ((*param0) < 0) {
        (*param0) += (FX32_ONE * 360);
    }

    (*param0) %= (FX32_ONE * 360);
}

s32 ov85_021E7644(void *);

s32 ov85_021E78A4(u8 *, s32);

void ov85_021E8558(u8 *arg0, s32 arg1);

void ov85_021E8558(u8 *arg0, s32 arg1) {
    ov85_021E7644(arg0 + 0xD4);
    ov85_021E78A4(arg0, arg1);
}

struct GameStats;

struct JournalEntry;

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

BOOL ov85_021E8570(UnkStruct_ov109_021D0F70 *param0);

BOOL ov85_021E8570(UnkStruct_ov109_021D0F70 *param0) {
    param0->unk_08++;

    if (param0->unk_08 < (30 * 20)) {
        return 0;
    }

    param0->unk_08 = (30 * 20);
    return 1;
}

void *ov85_021E8588(UnkStruct_ov109_021D0F70 *arg0, u32 arg1, s32 arg2);

void *ov85_021E8588(UnkStruct_ov109_021D0F70 *arg0, u32 file_id, s32 arg2) {
    u32 size;
    void *dest;

    size = NARC_GetMemberSize(arg0->unk_D80, file_id);
    if (arg2 == 1) {
        dest = Heap_Alloc(HEAP_ID_102, size);
    } else {
        dest = Heap_AllocAtEnd(HEAP_ID_102, size);
    }
    NARC_ReadWholeMember(arg0->unk_D80, file_id, dest);
    return dest;
}

void ov85_021E85C4(UnkStruct_ov109_021D0F70 *param0, u32 param1);

void ov85_021E85C4(UnkStruct_ov109_021D0F70 *param0, u32 param1) {
    if (param1 != 0) {
        param0->unk_2C.unk_08 = param1;
    }
}

void ov85_021E85CC(UnkStruct_ov109_021D0F70 *param0, const UnkStruct_0209C0F0 *param1);

void ov85_021E85CC(UnkStruct_ov109_021D0F70 *param0, const UnkStruct_0209C0F0 *param1) {
    param0->unk_2C.unk_10 = 1;
    param0->unk_2C.unk_2C = *param1;
}

void *ov85_021E85F0(UnkStruct_ov109_021D0F70 *param0, u32 param1);

void *ov85_021E85F0(UnkStruct_ov109_021D0F70 *param0, u32 param1) {
    GF_ASSERT(param1 < 32);
    memset(param0->unk_AC, 0, 32);
    return param0->unk_AC;
}

void *ov85_021E8610(UnkStruct_ov109_021D0F70 *param0);

void *ov85_021E8610(UnkStruct_ov109_021D0F70 *param0) {
    return param0->unk_AC;
}

BOOL ov85_021E8614(UnkStruct_ov109_021D0F70 *param0, u16 param1);

BOOL ov85_021E8614(UnkStruct_ov109_021D0F70 *param0, u16 param1) {
    u32 v0 = param0->unk_D0->unk_40;

    if (v0 & param1) {
        return 1;
    }

    return 0;
}

BOOL ov85_021E8628(UnkStruct_ov109_021D0F70 *param0);

BOOL ov85_021E8628(UnkStruct_ov109_021D0F70 *param0) {
    int v0 = 0;
    int v1 = param0->unk_CC->unk_08 - 1;
    u32 v2 = param0->unk_D0->unk_42;

    while (v2) {
        v0 += (v2 & 0x1);
        v2 >>= 1;
    }

    if (v0 >= v1) {
        return 1;
    }

    return 0;
}

int ov85_021E8660(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E8660(UnkStruct_ov109_021D0F70 *param0) {
    int v0 = 0;
    u32 v1 = param0->unk_D0->unk_42;

    while (v1) {
        v0 += (v1 & 0x1);
        v1 >>= 1;
    }

    return v0;
}

void ov85_021E8680(UnkStruct_ov109_021D0F70 *param0, const UnkStruct_ov109_021D1048 *param1);

void ov85_021E8680(UnkStruct_ov109_021D0F70 *param0, const UnkStruct_ov109_021D1048 *param1) {
    param0->unk_2C.unk_44[param1->unk_00] = *param1;

    if (param1->unk_02 == sub_0203769C()) {
        param0->unk_2C.unk_00 = param1->unk_00;
    }
}

void ov85_021E86AC(UnkStruct_ov109_021D0F70 *param0, int param1);

void ov85_021E86AC(UnkStruct_ov109_021D0F70 *param0, int param1) {
    param0->unk_2C.unk_04 = param1;
}

BOOL ov85_021E86B0(UnkStruct_ov109_021D0F70 *param0, int param1);

BOOL ov85_021E86B0(UnkStruct_ov109_021D0F70 *param0, int param1) {
    if (param1 != 0) {
        u32 v0 = 1 << (u32)param1;

        if ((param0->unk_D0->unk_42 & v0) == 0) {
            return 0;
        }
    }

    return 1;
}

void *sub_02097018(UnkStruct_0209BDF8 *param0, int param1);

void ov85_021E86CC(UnkStruct_ov109_021D0F70 *param0, int param1);

void ov85_021E86CC(UnkStruct_ov109_021D0F70 *param0, int param1) {
    int v0, v1;
    Party *v2, *v3;
    Pokemon *v4, *v5;

    v2 = param0->unk_24;
    v3 = (Party *)sub_02097018(param0->unk_D0, param1);

    v0 = param0->unk_CC->unk_04;
    v1 = param0->unk_2C.unk_18[param1];

    v4 = Party_GetMonByIndex(v2, v0);
    v5 = Party_GetMonByIndex(v3, v1);

    MonSetTrainerMemo(v5, sub_02034818(sub_0203769C()), 5, 0, HEAP_ID_FIELD2);
    CopyPokemonToPokemon(v5, v4);
}

int ov85_021E8720(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E8720(UnkStruct_ov109_021D0F70 *param0) {
    int v0 = 0;
    u32 v1 = param0->unk_D0->unk_46;

    while (v1) {
        v0 += (v1 & 0x1);
        v1 >>= 1;
    }

    return v0;
}

void ov85_021E8740(UnkStruct_ov109_021D0F70 *param0, int param1, int param2);

void ov85_021E8740(UnkStruct_ov109_021D0F70 *param0, int param1, int param2) {
    param0->unk_2C.unk_18[param1] = param2;
}

void ov85_021E8748(UnkStruct_ov109_021D0F70 *param0, const UnkStruct_ov109_021D17EC *param1);

void ov85_021E8748(UnkStruct_ov109_021D0F70 *param0, const UnkStruct_ov109_021D17EC *param1) {
    param0->unk_2C.unk_34 = *param1;
    param0->unk_2C.unk_14 = 1;
}
