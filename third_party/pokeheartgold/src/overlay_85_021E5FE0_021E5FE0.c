#include "global.h"
#include "simple3dmodel.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "player_data.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

BOOL sub_02096D4C(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);

BOOL ov85_021E750C(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E7644(UnkStruct_ov109_021D2AE4 *param0, fx32 param1);

void ov85_021E8128(UnkStruct_ov109_021D0F70 *param0);

BOOL ov85_021E8144(UnkStruct_ov109_021D0F70 *param0);

BOOL ov85_021E8150(UnkStruct_ov109_021D0F70 *param0);

BOOL ov85_021E834C(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E8358(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E8558(UnkStruct_ov109_021D0F70 *param0, fx32 param1);

BOOL ov85_021E8570(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5FE0(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5FE0(UnkStruct_ov109_021D0F70 *param0) {
    {
        BOOL v0 = ov85_021E8570(param0);

        if ((v0 == 1) && (ov85_021E8150(param0) == 0)) {
            param0->unk_00 = 29;
            return 1;
        }
    }

    if (param0->unk_2C.unk_08) {
        if (ov85_021E8150(param0) == 0) {
            ov85_021E8128(param0);
        }
    }

    if (ov85_021E834C(param0) == 0) {
        if (ov85_021E750C(param0) == 1) {
            if (ov85_021E8150(param0) == 0) {
                ov85_021E8128(param0);
                param0->unk_2C.unk_0C = 1;
            }

            ov85_021E8358(param0);
        }
    }

    if (param0->unk_2C.unk_0C) {
        if (sub_02096D4C(param0->unk_CC->unk_30, 10, &param0->unk_2C.unk_0C, 1) == 1) {
            param0->unk_2C.unk_0C = 0;
        }
    }

    {
        fx32 v1;
        UnkStruct_ov109_021D2AE4 *v2 = &param0->unk_D4;

        if (param0->unk_08 < ((30 * 20) / 2)) {
            v2->unk_48 += (((fx32)0x0000000000001000L) * 14 / ((30 * 20) / 2));

            if (v2->unk_48 >= (((fx32)0x0000000000001000L) * 14)) {
                v2->unk_48 = (((fx32)0x0000000000001000L) * 14);
            }
        } else {
            v2->unk_48 -= (((fx32)0x0000000000001000L) * 14 / ((30 * 20) / 2));

            if (v2->unk_48 < (((fx32)0x0000000000001000L) * 4)) {
                v2->unk_48 = (((fx32)0x0000000000001000L) * 4);
            }
        }

        v1 = v2->unk_48;

        if (ov85_021E8144(param0) == 1) {
            ov85_021E7644(&param0->unk_D4, (v1) << 1);
        } else {
            ov85_021E8558(param0, v1);
        }
    }

    {
        UnkStruct_0209C0F0 v3;

        v3.unk_02 = param0->unk_08;
        v3.unk_04 = ((param0->unk_D4.unk_48) / ((fx32)0x0000000000001000L));
        v3.unk_06 = ((param0->unk_D4.unk_3C) / ((fx32)0x0000000000001000L));

        sub_02096D4C(param0->unk_D0, 11, &v3, sizeof(UnkStruct_0209C0F0));
    }

    return 0;
}
