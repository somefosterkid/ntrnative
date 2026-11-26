#include "global.h"

#include "error_handling.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_45_wifi_plaza_api.h"
#include "overlay_49.h"
#include "overlay_49_getters.h"
#include "overlay_49_plaza_api.h"
#include "overlay_49_unk_struct_ov49_02264cf8.h"

void ov49_02264C50(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2) {
    WiFiPlazaSystem *v0;
    BOOL v1;
    BOOL v2;
    u32 v3;

    v0 = ov49_02259FE8(param0);
    v2 = ov45_0222AB28(v0, param1);
    v1 = ov45_0222AB28(v0, param2);
    v3 = 0x29;
    if ((v2 == 1) && (v1 == 1)) {
        v3 = 0x2AA;
    } else if ((v2 == 1) && (v1 == 0)) {
        v3 = 0x226;
    } else if ((v2 == 0) && (v1 == 1)) {
        v3 = 0x2A9;
    }
    ov49_0225A30C(param0, 3, v3);
}

u32 ov49_02264B94(const WiFiPlazaProfile *param0);

BOOL ov49_02264CA8(UnkStruct_ov49_02264CF8 *arg0, WiFiPlazaSystem *arg1, u8 arg2) {
    const WiFiPlazaProfile *v0;
    WiFiPlazaProfile *v1;
    if (arg2 == -1U) {
        GF_AssertFail();
    }
    arg0->unk_03 = arg2;
    arg0->unk_08 = ov45_0222A53C(arg1);
    arg0->unk_0A = 1;
    v1 = ov45_0222A5C0(arg1);
    v0 = ov45_0222A578(arg1, arg0->unk_03);
    if (v0 == 0) {
        return 0;
    }
    arg0->unk_04 = ov49_02264B94(v1);
    arg0->unk_05 = ov49_02264B94(v0);
    return 1;
}

void ov49_02264CF8(void) {
}
