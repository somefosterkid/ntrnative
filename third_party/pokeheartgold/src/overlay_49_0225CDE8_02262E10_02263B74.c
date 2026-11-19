#include "global.h"

#include "overlay_49_unk_struct_ov49_02264cf8.h"

#include "pm_string.h"
#include "unk_02005D10.h"
#include "bg_window.h"
#include "list_menu.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_45_wifi_plaza_system.h"

struct ColoredArrow;

struct ListMenu;

struct ListMenuTemplate;

struct String;

struct ListMenuItem;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_02263344_t;

struct WiFiPlazaProfile;

struct Window;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;

const WiFiPlazaProfile *ov45_0222A578(const WiFiPlazaSystem *param0, u32 param1);

WiFiPlazaProfile *ov45_0222A5C0(WiFiPlazaSystem *param0);

void ov45_0222A5E8(WiFiPlazaSystem *param0, int param1);

void ov45_0222A704(WiFiPlazaSystem *param0, int param1, s32 param2);

u32 ov45_0222AAC8(const WiFiPlazaProfile *profile);

void ov45_0222AE64(WiFiPlazaSystem *param0);

void ov45_0222AED8(WiFiPlazaSystem *param0, u16 param1);

void ov45_0222AFC4(WiFiPlazaSystem *param0);

u32 ov45_0222B020(const WiFiPlazaSystem *param0);

BOOL ov45_0222B028(const WiFiPlazaSystem *param0);

int ov45_0222B034(const WiFiPlazaSystem *param0);

u16 ov45_0222B040(const WiFiPlazaSystem *param0);

BOOL ov45_0222B06C(const WiFiPlazaSystem *param0);

BOOL ov45_0222B0B0(const WiFiPlazaSystem *param0);

void ov45_0222B0BC(WiFiPlazaSystem *param0);

void ov45_0222B0D8(WiFiPlazaSystem *param0, u32 param1);

void ov45_0222B0E8(WiFiPlazaSystem *param0, u32 param1);

void ov66_0222EEF4(WiFiPlazaSystem *param0, u32 param1);

void ov66_0222F020(WiFiPlazaSystem *param0);

void ov45_0222B118(WiFiPlazaSystem *param0, int param1);

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0);

void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2);

typedef struct ColoredArrow ColoredArrow;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

WiFiPlazaSystem *ov70_0225DEE8(UnkStruct_ov70_0225DEE8 *param0);

UnkStruct_ov70_0225CA20 *ov49_02259FF0(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A08C(UnkStruct_ov70_0225DEE8 *param0, const String *param1);

void ov49_0225A0BC(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A0CC(UnkStruct_ov70_0225DEE8 *param0);

BOOL ov49_0225A0DC(const UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A0EC(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A174(UnkStruct_ov70_0225DEE8 *param0, const ListMenuTemplate *param1, s32 param2, s32 param3);

u32 ov49_0225A1D4(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A1E4(UnkStruct_ov70_0225DEE8 *param0, u16 *param1, u16 *param2);

void ov49_0225A334(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);

void ov49_0225A428(UnkStruct_ov70_0225DEE8 *param0, u32 param1, BOOL param2);

void ov49_0225A478(UnkStruct_ov70_0225DEE8 *param0, u32 param1);

void ov49_0225A4D0(UnkStruct_ov70_0225DEE8 *param0);

void *ov49_0225EF40(UnkStruct_ov70_02263344 *param0, u32 param1);

void ov49_0225EF68(UnkStruct_ov70_02263344 *param0);

void *GridInputHandler_IsButtonInputMode(UnkStruct_ov70_02263344 *param0);

u32 ov49_0225EF88(const UnkStruct_ov70_02263344 *param0);

void ov49_0225EF8C(UnkStruct_ov70_02263344 *param0, u32 param1);

String *ov49_02264C04(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);

String *ov49_02264C50(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);

BOOL ov49_02264CA8(UnkStruct_ov49_02264CF8 *param0, WiFiPlazaSystem *param1, u32 param2);

void ov49_02264CF8(UnkStruct_ov49_02264CF8 *param0);

void ov49_02264CFC(UnkStruct_ov49_02264CF8 *param0, u8 param1, u8 param2, UnkStruct_ov70_02263344 *param3, u8 param4);

void ov49_02264D14(UnkStruct_ov49_02264CF8 *param0, UnkStruct_ov70_02263344 *param1);

void ov49_02264D30(UnkStruct_ov49_02264CF8 *param0, UnkStruct_ov70_02263344 *param1, UnkStruct_ov70_0225DEE8 *param2);

void ov49_02264D4C(UnkStruct_ov49_02264CF8 *param0, WiFiPlazaSystem *param1, UnkStruct_ov70_0225DEE8 *param2, u32 param3, const WiFiPlazaProfile *param4, const WiFiPlazaProfile *param5);

BOOL ov49_02264E20(UnkStruct_ov49_02264CF8 *param0, WiFiPlazaSystem *param1, UnkStruct_ov70_0225DEE8 *param2);

void ov49_02264E90(UnkStruct_ov49_02264CF8 *param0, WiFiPlazaSystem *param1, UnkStruct_ov70_0225DEE8 *param2, u32 param3, BOOL param4);

void ov49_02264EC8(UnkStruct_ov49_02264CF8 *param0, UnkStruct_ov70_0225DEE8 *param1);

void ov49_02264F10(UnkStruct_ov49_02264CF8 *param0);

void ov49_02264F1C(UnkStruct_ov49_02264CF8 *param0);

void ov49_02264F24(UnkStruct_ov49_02264CF8 *param0, UnkStruct_ov70_0225DEE8 *param1);

void ov49_02264F60(UnkStruct_ov49_02264CF8 *param0);

void ov49_02264F78(UnkStruct_ov49_02264CF8 *param0, const WiFiPlazaProfile *param1);

void ov49_02264F9C(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2, u32 param3, u32 param4);

void ov49_02265260(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1);

BOOL ov49_02263B74(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

WiFiPlazaSystem *ov49_02259FE8(UnkStruct_ov70_0225DEE8 *param0);

void *ov49_0225EF84(UnkStruct_ov70_02263344 *param0);

void IncrementGameStat47(WiFiPlazaSystem *param0);

u32 ov49_0225CB70(UnkStruct_ov70_0225DEE8 *param0);

BOOL ov49_02263B74(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    UnkStruct_ov49_02264CF8 *v0;
    WiFiPlazaSystem *v1;
    u32 v2;
    u32 v3;
    String *v4;
    const WiFiPlazaProfile *v5;
    const WiFiPlazaProfile *v6;

    v1 = ov49_02259FE8(param1);
    v0 = ov49_0225EF84(param0);
    v2 = ov45_0222B034(v1);
    v3 = ov45_0222B040(v1);

    if (ov45_0222B06C(v1)) {
        if (v0) {
            if (v0->unk_0A == 1) {
                ov49_0225EF8C(param0, 26);
                ov49_0225A0CC(param1);
                v0->unk_0A = 0;
            }
        }
    }

    if (v0) {
        v5 = ov45_0222A5C0(v1);
        v6 = ov45_0222A578(v1, v0->unk_03);
    }

    if (ov49_0225EF88(param0) != 0) {
        if (v0->unk_0A == 1) {
            if (v6 == 0L) {
                ov49_0225EF8C(param0, 26);
                ov49_0225A0CC(param1);
                v0->unk_0A = 0;
            }
        }
    }

    switch (ov49_0225EF88(param0)) {
    case 0:
        v0 = ov49_0225EF40(param0, sizeof(UnkStruct_ov49_02264CF8));
        v0->unk_44 = 0;
        v0->unk_46 = 0;
        v0->unk_48 = 0;

        {
            u32 v7;
            BOOL v8;

            v7 = ov45_0222B020(v1);
            v8 = ov49_02264CA8(v0, v1, v7);

            PlaySE(1508);
            ov45_0222A5E8(v1, 9);

            if (v8 == 1) {
                ov49_0225A428(param1, v7, 0);
                ov45_0222B0E8(v1, v7);
                ov49_0225EF8C(param0, 1);
            } else {
                ov49_0225EF8C(param0, 26);
                break;
            }
        }
        break;
    case 1:
        if (ov45_0222B0B0(v1) == 0) {
            break;
        }

        ov49_02264D4C(v0, v1, param1, v0->unk_05, v6, v5);
        ov49_02264CFC(v0, 128, 2, param0, 30);
        break;
    case 2:
        IncrementGameStat47(v1);
        ov45_0222B118(v1, 7);
        ov49_02264D4C(v0, v1, param1, v0->unk_04, v5, v6);
        ov49_02264CFC(v0, 128, 3, param0, 30);
        break;
    case 3:
        switch (v2) {
        case 3:
            ov49_0225A0CC(param1);
            ov45_0222AED8(v1, 0);

            if (ov45_0222B028(v1)) {
                ov49_0225EF8C(param0, 4);
            } else {
                ov49_0225EF8C(param0, 6);
            }
            break;
        case 2:
            if (ov49_0225A0DC(param1) == 0) {
                ov49_0225A0BC(param1);
            }

            ov49_02264EC8(v0, param1);
            break;
        case 1:
        case 4:
        case 0:
        default:
            ov49_0225EF8C(param0, 26);
            ov49_0225A0CC(param1);
            break;
        }
        break;
    case 4: {
        BOOL v9;

        v9 = ov49_02264E20(v0, v1, param1);

        if (v9 == 1) {
            ov49_02264CFC(v0, 128, 5, param0, 30);
        } else {
            ov49_0225EF8C(param0, 5);
        }
    } break;
    case 5:
        ov49_0225A334(param1, v0->unk_03, 0);

        v4 = ov49_02264C04(param1, v0->unk_03, 40);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 7, param0, 30);
        break;
    case 6:
        ov49_0225A334(param1, v0->unk_03, 0);
        ov49_0225A334(param1, v0->unk_08, 1);

        v4 = ov49_02264C50(param1, v0->unk_03, v0->unk_08);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 7, param0, 30);
        break;
    case 7:
        ov49_0225A334(param1, v0->unk_03, 0);

        v4 = ov49_02264C04(param1, v0->unk_03, 47);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 9, param0, 30);
        break;
    case 8:
        ov49_0225A334(param1, v0->unk_03, 0);

        v4 = ov49_02264C04(param1, v0->unk_03, 53);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 9, param0, 30);
        break;
    case 9:
        switch (v2) {
        case 3:
            switch (v3) {
            case 3:
                ov49_0225EF8C(param0, 10);
                ov45_0222B0BC(v1);

                v0->unk_4C = 1;

                ov49_02264F78(v0, v6);
                break;
            case 4:
                ov49_0225EF8C(param0, 14);
                ov45_0222B0BC(v1);
                v0->unk_4C = 1;
                ov49_02264F78(v0, v6);
                break;
            case 5:
                ov45_0222AED8(v1, 1);
                ov49_0225EF8C(param0, 15);
                break;
            default:
                ov49_0225EF8C(param0, 26);
                ov49_0225A0CC(param1);
                break;
            }

            ov49_0225A0CC(param1);
            break;
        case 2:
            if (ov49_0225A0DC(param1) == 0) {
                ov49_0225A0BC(param1);
            }

            ov49_02264EC8(v0, param1);
            break;
        case 0:
            ov49_0225EF8C(param0, 27);
            ov49_0225A0CC(param1);
            break;
        case 1:
        case 4:
        default:
            ov49_0225EF8C(param0, 26);
            ov49_0225A0CC(param1);
            break;
        }
        break;
    case 10:
        ov49_0225A334(param1, v0->unk_03, 0);
        ov49_02264E90(v0, v1, param1, 1, 1);

        v4 = ov49_02264C04(param1, v0->unk_03, 511);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 11, param0, 30);
        break;
    case 11:
        ov49_0225A334(param1, v0->unk_03, 0);
        ov49_02264E90(v0, v1, param1, 1, 1);

        v4 = ov49_02264C04(param1, v0->unk_03, 512);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 12, param0, 30);
        break;
    case 12:
        ov49_0225A334(param1, v0->unk_03, 0);
        ov49_02264E90(v0, v1, param1, 1, 1);

        v4 = ov49_02264C04(param1, v0->unk_03, 513);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 13, param0, 30);
        break;
    case 13:
        PlaySE(1471);
        ov49_0225A334(param1, v0->unk_03, 0);

        v4 = ov49_02264C04(param1, v0->unk_03, 514);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 8, param0, 30);
        ov45_0222AED8(v1, 2);

        {
            u32 v10;

            v10 = ov45_0222AAC8(ov45_0222A5C0(v1));
            ov49_0225A478(param1, v10);
        }
        break;
    case 14:
        ov49_0225A334(param1, v0->unk_03, 0);
        ov49_02264E90(v0, v1, param1, 1, 1);

        v4 = ov49_02264C04(param1, v0->unk_03, 507);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 27, param0, 30);
        ov45_0222AED8(v1, 2);
        break;
    case 15:
        ov49_0225A334(param1, v0->unk_03, 0);

        v4 = ov49_02264C04(param1, v0->unk_03, 687);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 16, param0, 30);
        break;
    case 16:
        switch (v2) {
        case 3:
            ov49_0225EF8C(param0, 17);
            ov49_0225A0CC(param1);
            ov45_0222B0BC(v1);
            v0->unk_4C = 1;
            break;
        case 2:
            if (ov49_0225A0DC(param1) == 0) {
                ov49_0225A0BC(param1);
            }

            ov49_02264EC8(v0, param1);
            break;
        case 1:
        case 4:
        case 0:
        default:
            ov49_0225EF8C(param0, 26);
            ov49_0225A0CC(param1);
            break;
        }
        break;
    case 17:
        if ((v3 < 6) || (v3 >= 126)) {
            ov49_0225EF8C(param0, 26);
            break;
        }

        v0->unk_02 = v3 - 6;
        v0->unk_00 = v0->unk_02 / 4;

        ov49_0225A334(param1, v0->unk_03, 0);

        v4 = ov49_02264C04(param1, v0->unk_03, 55 + v0->unk_00);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 18, param0, 30);
        break;
    case 18:
        ov49_0225A334(param1, v0->unk_03, 0);

        v4 = ov49_02264C04(param1, v0->unk_03, 85 + v0->unk_00);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 19, param0, 30);
        break;
    case 19:
        ov49_0225A334(param1, v0->unk_03, 0);

        v4 = ov49_02264C04(param1, v0->unk_03, 236 + v0->unk_02);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 20, param0, 30);
        break;
    case 20:
        ov49_0225A334(param1, v0->unk_03, 0);

        v4 = ov49_02264C04(param1, v0->unk_03, 477 + v0->unk_00);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 21, param0, 30);
        break;
    case 21:
        ov49_02264F9C(&v0->unk_14, param1, 5, 115 + (v0->unk_00 * 4), 2);
        ov49_0225A174(param1, &v0->unk_14.unk_00, 0, 0);
        ov49_02264F10(v0);
        ov49_0225EF8C(param0, 22);
        break;
    case 22: {
        u32 v11;
        BOOL v12;

        v11 = ov49_0225A1D4(param1);

        switch (v11) {
        case 0:
        case 1:
        case 2:
        case 3:
            v0->unk_01 = (v0->unk_00 * 4) + v11;

            ov45_0222AED8(v1, v0->unk_01 + 6);
            ov49_0225EF8C(param0, 23);

            v12 = 1;
            break;
        case 4:
            v0->unk_01 = 126;

            ov45_0222AED8(v1, v0->unk_01);
            ov49_0225EF8C(param0, 28);

            v12 = 1;
            break;
        default:
            ov49_02264F24(v0, param1);
            v12 = 0;
            break;
        }

        if (v12) {
            ov49_0225A1E4(param1, 0L, 0L);
            ov49_02265260(&v0->unk_14, param1);
            ov49_02264F1C(v0);
            ov49_02264F78(v0, v6);
        }
    } break;
    case 23:
        ov49_0225A334(param1, v0->unk_03, 0);
        v4 = ov49_02264C04(param1, v0->unk_03, 357 + v0->unk_01);
        ov49_0225A08C(param1, v4);

        if (v0->unk_02 == v0->unk_01) {
            ov49_02264CFC(v0, 128, 24, param0, 30);
        } else {
            ov49_02264CFC(v0, 128, 25, param0, 30);
        }
        break;
    case 24:
        ov49_0225A334(param1, v0->unk_03, 0);

        v4 = ov49_02264C04(param1, v0->unk_03, 51);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 25, param0, 30);
        break;
    case 25:
        ov49_0225A334(param1, v0->unk_03, 0);

        v4 = ov49_02264C04(param1, v0->unk_03, 52);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 8, param0, 30);
        break;
    case 26:
        ov49_0225A1E4(param1, 0L, 0L);
        ov49_02265260(&v0->unk_14, param1);

        v4 = ov49_02264C04(param1, v0->unk_03, 45);
        ov49_0225A08C(param1, v4);
        v0->unk_0A = 0;

        {
            u32 len;
            u32 value;

            len = String_GetLength(v4);
            value = ov49_0225CB70(param1) * len;
            value = (value >> 1) + 60;
            if (value < 0x80) {
                value = 0x80;
            } else if (value > 0xff) {
                value = 0xff;
            }
            ov49_02264CFC(v0, (u8)value, 31, param0, 29);
        }
        ov45_0222AFC4(v1);
        break;
    case 27:
        ov49_0225A334(param1, v0->unk_03, 0);

        v4 = ov49_02264C04(param1, v0->unk_03, 43);
        ov49_0225A08C(param1, v4);
        v0->unk_0A = 0;

        ov49_02264CFC(v0, 128, 31, param0, 29);
        break;
    case 28:
        ov49_0225A334(param1, v0->unk_03, 0);

        v4 = ov49_02264C04(param1, v0->unk_03, 49);

        ov49_0225A08C(param1, v4);
        ov49_02264CFC(v0, 128, 27, param0, 30);
        break;
    case 29:
        ov49_02264D14(v0, param0);
        break;
    case 30:
        ov49_02264D30(v0, param0, param1);
        break;
    case 31:
        if (v0->unk_44 == 1) {
            ov45_0222A704(v1, v0->unk_46, v0->unk_48);
        }

        if (v0->unk_4C == 0) {
            ov45_0222B0D8(v1, v0->unk_03);
        }

        ov45_0222AE64(v1);
        ov49_0225A0EC(param1);
        ov49_02264CF8(v0);
        ov49_0225EF68(param0);
        ov45_0222A5E8(v1, 1);

        {
            UnkStruct_ov70_0225CC54 *v13;
            UnkStruct_ov70_0225CA20 *v14;

            v14 = ov49_02259FF0(param1);
            v13 = ov49_02258DAC(v14);

            ov49_02258EEC(v14, v13, 1);
            ov49_0225A4D0(param1);
        }

        return 1;
    }

    ov49_02264F60(v0);

    return 0;
}
