#include "global.h"

#include "overlay_49_unk_struct_ov49_02264cf8.h"

#include "bg_window.h"
#include "list_menu.h"
#include "pm_string.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_45_wifi_plaza_system.h"

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_0225DEE8_t;

struct WiFiPlazaProfile;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

const WiFiPlazaProfile *ov45_0222A578(const WiFiPlazaSystem *param0, u32 param1);

WiFiPlazaProfile *ov45_0222A5C0(WiFiPlazaSystem *param0);

BOOL ov45_0222AA28(const WiFiPlazaProfile *profile);

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

void ov49_0225A08C(UnkStruct_ov70_0225DEE8 *param0, const String *param1);

String *ov49_0225A30C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2);

BOOL ov49_02264E20(UnkStruct_ov49_02264CF8 *param0, WiFiPlazaSystem *param1, UnkStruct_ov70_0225DEE8 *param2);

BOOL ov49_02264E20(UnkStruct_ov49_02264CF8 *param0, WiFiPlazaSystem *param1, UnkStruct_ov70_0225DEE8 *param2) {
    u32 v0;
    const WiFiPlazaProfile *v1;
    const WiFiPlazaProfile *v2;

    v1 = ov45_0222A5C0(param1);
    v2 = ov45_0222A578(param1, param0->unk_03);

    if (ov45_0222AA28(v1) == 0) {
        return 0;
    }

    if (ov45_0222AA28(v2) == 0) {
        return 0;
    }

    if (param0->unk_04 != param0->unk_05) {
        switch (param0->unk_05) {
        case 0:
            v0 = 39;
            break;
        case 1:
            v0 = 37;
            break;
        case 2:
            v0 = 38;
            break;
        }

        ov49_0225A08C(param2, ov49_0225A30C(param2, 3, v0));
        return 1;
    }

    return 0;
}

u32 ov45_0222AAC8(const WiFiPlazaProfile *profile);

void ov49_0225A39C(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);

void ov49_02264E90(UnkStruct_ov49_02264CF8 *param0, WiFiPlazaSystem *param1, UnkStruct_ov70_0225DEE8 *param2, u32 param3, BOOL param4);

void ov49_02264E90(UnkStruct_ov49_02264CF8 *param0, WiFiPlazaSystem *param1, UnkStruct_ov70_0225DEE8 *param2, u32 param3, BOOL param4) {
    u32 v0;
    const WiFiPlazaProfile *v1;
    const WiFiPlazaProfile *v2;

    v1 = ov45_0222A5C0(param1);
    v2 = ov45_0222A578(param1, param0->unk_03);

    if (param4 == 0) {
        v0 = ov45_0222AAC8(v2);
    } else {
        v0 = ov45_0222AAC8(v1);
    }

    ov49_0225A39C(param2, v0, param3);
}
