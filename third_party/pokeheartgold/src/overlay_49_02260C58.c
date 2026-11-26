#include "global.h"

#include "overlay_45_wifi_plaza_api.h"

#include "overlay_49_getters.h"

#include "error_handling.h"
#include "overlay_49.h"
#include "pm_string.h"
#include "unk_02005D10.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"


struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
struct UnkStruct_ov70_0225DEE8_t;
struct UnkStruct_ov70_02263344_t;
typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0);
void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2);
void ov49_0225A08C(UnkStruct_ov70_0225DEE8 *param0, const String *param1);
BOOL ov49_0225A0AC(const UnkStruct_ov70_0225DEE8 *param0);
void ov49_0225A0EC(UnkStruct_ov70_0225DEE8 *param0);
String *ov49_0225A30C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2);
typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;
u32 ov49_0225EF88(const UnkStruct_ov70_02263344 *param0);
void ov49_0225EF90(UnkStruct_ov70_02263344 *param0);
BOOL ov49_02260D90(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

s32 ov49_02260C58(u32 arg0) {
    switch (arg0) {
    case 22:
    case 23:
    case 24:
    case 25:
        return 2;
    case 26:
    case 27:
    case 28:
    case 29:
        return 3;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        return 0;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        return 1;
    default:
        GF_AssertFail();
        return 0;
    }
}

s32 ov49_02260CC0(u32 arg0) {
    switch (arg0) {
    case 22:
    case 23:
    case 24:
    case 25:
        return 1;
    case 26:
    case 27:
    case 28:
    case 29:
        return 2;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        return 6;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        return 7;
    default:
        GF_AssertFail();
        return 0;
    }
}

s32 ov49_02260D28(u32 arg0) {
    switch (arg0) {
    case 22:
    case 23:
    case 24:
    case 25:
        return 7;
    case 26:
    case 27:
    case 28:
    case 29:
        return 8;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        return 5;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        return 6;
    default:
        GF_AssertFail();
        return 0;
    }
}

BOOL ov49_02260D90(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    switch (ov49_0225EF88(param0)) {
    case 0: {
        String *v0;
        WiFiPlazaSystem *v1;

        v1 = ov49_02259FE8(param1);

        if (ov45_0222A330(v1) == 1) {
            v0 = ov49_0225A30C(param1, 1, 3);
        } else {
            if (ov45_0222A374(v1) == 1) {
                v0 = ov49_0225A30C(param1, 1, 78);
            } else {
                v0 = ov49_0225A30C(param1, 1, 2);
            }
        }

        ov49_0225A08C(param1, v0);
        PlaySE(1500);
        ov49_0225EF90(param0);
    } break;
    case 1:
        if (ov49_0225A0AC(param1)) {
            ov49_0225A0EC(param1);
            {
                UnkStruct_ov70_0225CA20 *v2;
                UnkStruct_ov70_0225CC54 *v3;

                v2 = ov49_02259FF0(param1);
                v3 = ov49_02258DAC(v2);

                ov49_02258EEC(v2, v3, 1);
            }
            return 1;
        }

        break;
    }

    return 0;
}
