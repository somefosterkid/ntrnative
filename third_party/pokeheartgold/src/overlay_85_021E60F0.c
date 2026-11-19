#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "camera.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "unk_02035900.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "game_stats.h"
#include "unk_struct_0209_c194.h"

s32 ov85_021E750C(void *);

s32 ov85_021E7644(void *, s32);

s32 ov85_021E8128(void *);

s32 ov85_021E8144(void *);

s32 ov85_021E8150(void *);

s32 ov85_021E834C(void *);

s32 ov85_021E8358(void *);

s32 ov85_021E8558(void *, s32);

s32 ov85_021E8570(void *);

s32 sub_02096D4C(void *, s32, void *, s32);

s32 ov85_021E60F0(UnkStruct_ov109_021D0F70 *arg0);

s32 ov85_021E60F0(UnkStruct_ov109_021D0F70 *arg0) {
    if (arg0->unk_2C.unk_10 == 1) {
        arg0->unk_D4.unk_3C = (s32)(arg0->unk_2C.unk_2C.unk_06 << 0xC);
        arg0->unk_D4.unk_48 = (s32)(arg0->unk_2C.unk_2C.unk_04 << 0xC);
        arg0->unk_08 = (s32) arg0->unk_2C.unk_2C.unk_02;
    }
    if ((ov85_021E8570(arg0) == 1) && (ov85_021E8150(arg0) == 0)) {
        arg0->unk_00 = 0x1D;
        return 1;
    }
    if ((arg0->unk_2C.unk_08 != 0) && (ov85_021E8150(arg0) == 0)) {
        ov85_021E8128(arg0);
    }
    if ((ov85_021E834C(arg0) == 0) && (ov85_021E750C(arg0) == 1)) {
        if (ov85_021E8150(arg0) == 0) {
            ov85_021E8128(arg0);
            arg0->unk_2C.unk_0C = 1;
        }
        ov85_021E8358(arg0);
    }
    if ((arg0->unk_2C.unk_0C != 0) && (sub_02096D4C(arg0->unk_CC->unk_30, 0xA, &arg0->unk_2C.unk_0C, 1) == 1)) {
        arg0->unk_2C.unk_0C = 0;
    }
    if (ov85_021E8144(arg0) == 1) {
        ov85_021E7644(&arg0->unk_D4, arg0->unk_D4.unk_48 * 2);
    } else {
        ov85_021E8558(arg0, arg0->unk_D4.unk_48);
    }
    return 0;
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

UnkStruct_ov109_021D2D78 *ov85_021E7958(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E84EC(UnkStruct_ov109_021D0F70 *param0, int param1);

int ov85_021E61C8(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E61C8(UnkStruct_ov109_021D0F70 *param0) {
    ov85_021E84EC(param0, 0);

    param0->unk_28 = ov85_021E7958(param0);

    if (param0->unk_28->unk_0C == sub_0203769C()) {
        param0->unk_00 = 30;
    } else {
        param0->unk_00 = 31;
    }

    PlaySE(1521);
    return 1;
}

s32 ov85_021E61FC(UnkStruct_ov109_021D0F70 *arg0);

s32 ov85_021E61FC(UnkStruct_ov109_021D0F70 *arg0) {
    ov85_021E8558(arg0, 0x4000);
    arg0->unk_28 = ov85_021E7958(arg0);
    if (arg0->unk_28->unk_0C != sub_0203769C()) {
        arg0->unk_00 = 0x1F;
    }
    return 0;
}
