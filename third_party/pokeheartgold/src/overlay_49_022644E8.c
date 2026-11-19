#include "global.h"

#include "bg_window.h"
#include "list_menu.h"
#include "pm_string.h"
#include "unk_02005D10.h"
#include "overlay_45_wifi_plaza_system.h"
#include "overlay_49_unk_struct_ov49_02264cf8.h"

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_02263344_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};

BOOL ov45_0222A330(const WiFiPlazaSystem *param0);

BOOL ov45_0222A374(const WiFiPlazaSystem *param0);

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0);

void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2);

WiFiPlazaSystem *ov49_02259FE8(UnkStruct_ov70_0225DEE8 *param0);

UnkStruct_ov70_0225CA20 *ov49_02259FF0(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A08C(UnkStruct_ov70_0225DEE8 *param0, const String *param1);

BOOL ov49_0225A0AC(const UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A0EC(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A174(UnkStruct_ov70_0225DEE8 *param0, const ListMenuTemplate *param1, s32 param2, s32 param3);

void ov49_0225A1A4(UnkStruct_ov70_0225DEE8 *param0, const ListMenuTemplate *param1, u32 param2, u32 param3, u8 param4, u8 param5, u8 param6);

u32 ov49_0225A1D4(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A1E4(UnkStruct_ov70_0225DEE8 *param0, u16 *param1, u16 *param2);

String *ov49_0225A30C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2);

void ov49_0225A37C(UnkStruct_ov70_0225DEE8 *param0, enum PlazaMinigame param1, u32 param2);

void *ov49_0225EF40(UnkStruct_ov70_02263344 *param0, u32 param1);

void ov49_0225EF68(UnkStruct_ov70_02263344 *param0);

void *ov49_0225EF84(UnkStruct_ov70_02263344 *param0);

u32 ov49_0225EF88(const UnkStruct_ov70_02263344 *param0);

void ov49_0225EF8C(UnkStruct_ov70_02263344 *param0, u32 param1);

typedef struct {
    u32 unk_00;
    UnkStruct_ov70_02268E44 unk_04;
} UnkStruct_ov70_02268390;

void ov49_02265110(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1);

void ov49_02265170(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1);

void ov49_022651E8(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1);

void ov49_02265260(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1);

BOOL ov49_022644E8(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

int ov45_0222B134(const WiFiPlazaSystem *param0);
BOOL ov49_022649F4(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);
struct WiFiPlazaProfile;
UnkStruct_ov66_02231428 ov45_0222A9C8(const WiFiPlazaProfile *profile);
u32 ov49_02264B94(const WiFiPlazaProfile *param0);

BOOL ov49_022644E8(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    UnkStruct_ov70_02268390 *v0;
    String *v1;
    WiFiPlazaSystem *v2;

    v0 = ov49_0225EF84(param0);
    v2 = ov49_02259FE8(param1);

    switch (ov49_0225EF88(param0)) {
    case 0:
        v0 = ov49_0225EF40(param0, sizeof(UnkStruct_ov70_02268390));

        PlaySE(1500);

        if (ov45_0222A330(v2) == 1) {
            ov49_0225EF8C(param0, 2);
        } else {
            if (ov45_0222A374(v2) == 1) {
                ov49_0225EF8C(param0, 1);
            } else {
                ov49_0225EF8C(param0, 3);
            }
        }
        break;
    case 1:
        v1 = ov49_0225A30C(param1, 1, 78);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 24;
        ov49_0225EF8C(param0, 23);
        break;
    case 2:
        v1 = ov49_0225A30C(param1, 1, 3);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 24;
        ov49_0225EF8C(param0, 23);
        break;
    case 3:
        v1 = ov49_0225A30C(param1, 1, 15);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 4;
        ov49_0225EF8C(param0, 23);
        break;
    case 4:
        v1 = ov49_0225A30C(param1, 1, 16);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 5;
        ov49_0225EF8C(param0, 23);
        break;
    case 5:
        ov49_02265110(&v0->unk_04, param1);
        ov49_0225A1A4(param1, &v0->unk_04.unk_00, 0, 0, 16, 1, 15);
        ov49_0225EF8C(param0, 6);
        break;
    case 6: {
        u32 v3;
        BOOL v4 = 0;

        v3 = ov49_0225A1D4(param1);

        switch (v3) {
        case 0:
            ov49_0225EF8C(param0, 7);
            v4 = 1;
            break;
        case 1:
            ov49_0225EF8C(param0, 8);
            v4 = 1;
            break;
        case 2:
            ov49_0225EF8C(param0, 13);
            v4 = 1;
            break;
        case 3:
            ov49_0225EF8C(param0, 14);
            v4 = 1;
            break;
        case 4:
            ov49_0225EF8C(param0, 15);
            v4 = 1;
            break;
        case 5:
            ov49_0225EF8C(param0, 21);
            v4 = 1;
            break;
        case 6:
            ov49_0225EF8C(param0, 16);
            v4 = 1;
            break;
        case 0xfffffffe:
            PlaySE(1500);
        case 7:
            ov49_0225EF8C(param0, 22);
            v4 = 1;
            break;
        default:
            break;
        }

        if (v4 == 1) {
            ov49_0225A1E4(param1, 0L, 0L);
            ov49_02265260(&v0->unk_04, param1);
        }
    } break;
    case 7:
        v1 = ov49_0225A30C(param1, 1, 23);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 4;
        ov49_0225EF8C(param0, 23);
        break;
    case 8:
        ov49_022651E8(&v0->unk_04, param1);
        ov49_0225A174(param1, &v0->unk_04.unk_00, 0, 0);
        ov49_0225EF8C(param0, 9);
        break;
    case 9: {
        u32 v5;
        BOOL v6 = 0;

        v5 = ov49_0225A1D4(param1);

        switch (v5) {
        case 0:
            ov49_0225EF8C(param0, 10);
            v6 = 1;
            break;
        case 1:
            ov49_0225EF8C(param0, 11);
            v6 = 1;
            break;
        case 2:
            ov49_0225EF8C(param0, 12);
            v6 = 1;
            break;
        case 0xfffffffe:
            PlaySE(1500);
        case 3:
            ov49_0225EF8C(param0, 4);
            v6 = 1;
            break;
        default:
            break;
        }

        if (v6 == 1) {
            ov49_0225A1E4(param1, 0L, 0L);
            ov49_02265260(&v0->unk_04, param1);
        }
    } break;
    case 10:
        ov49_0225A37C(param1, UnkEnum_ov66_022324D0_00, 0);
        v1 = ov49_0225A30C(param1, 1, 24);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 4;
        ov49_0225EF8C(param0, 23);
        break;
    case 11:
        ov49_0225A37C(param1, UnkEnum_ov66_022324D0_01, 0);
        v1 = ov49_0225A30C(param1, 1, 25);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 4;
        ov49_0225EF8C(param0, 23);
        break;
    case 12:
        ov49_0225A37C(param1, UnkEnum_ov66_022324D0_02, 0);
        v1 = ov49_0225A30C(param1, 1, 26);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 4;
        ov49_0225EF8C(param0, 23);
        break;
    case 13:
        v1 = ov49_0225A30C(param1, 1, 18);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 4;
        ov49_0225EF8C(param0, 23);
        break;
    case 14:
        v1 = ov49_0225A30C(param1, 1, 19);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 4;
        ov49_0225EF8C(param0, 23);
        break;
    case 15:
        v1 = ov49_0225A30C(param1, 1, 20);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 4;
        ov49_0225EF8C(param0, 23);
        break;
    case 16:
        ov49_02265170(&v0->unk_04, param1);
        ov49_0225A174(param1, &v0->unk_04.unk_00, 0, 0);
        ov49_0225EF8C(param0, 17);
        break;
    case 17: {
        u32 v7;
        BOOL v8 = 0;

        v7 = ov49_0225A1D4(param1);

        switch (v7) {
        case 0:
            ov49_0225EF8C(param0, 18);
            v8 = 1;
            break;
        case 1:
            ov49_0225EF8C(param0, 19);
            v8 = 1;
            break;
        case 2:
            ov49_0225EF8C(param0, 20);
            v8 = 1;
            break;
        case 0xfffffffe:
            PlaySE(1500);
        case 3:
            ov49_0225EF8C(param0, 4);
            v8 = 1;
            break;
        default:
            break;
        }

        if (v8 == 1) {
            ov49_0225A1E4(param1, 0L, 0L);
            ov49_02265260(&v0->unk_04, param1);
        }
    } break;
    case 18:
        v1 = ov49_0225A30C(param1, 1, 27);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 4;
        ov49_0225EF8C(param0, 23);
        break;
    case 19:
        v1 = ov49_0225A30C(param1, 1, 28);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 4;
        ov49_0225EF8C(param0, 23);
        break;
    case 20:
        v1 = ov49_0225A30C(param1, 1, 29);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 4;
        ov49_0225EF8C(param0, 23);
        break;
    case 21:
        v1 = ov49_0225A30C(param1, 1, 30);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 4;
        ov49_0225EF8C(param0, 23);
        break;
    case 22:
        v1 = ov49_0225A30C(param1, 1, 22);
        ov49_0225A08C(param1, v1);
        v0->unk_00 = 24;
        ov49_0225EF8C(param0, 23);
        break;
    case 23:
        if (ov49_0225A0AC(param1)) {
            ov49_0225EF8C(param0, v0->unk_00);
        }
        break;
    case 24:
        ov49_0225EF68(param0);
        ov49_0225A0EC(param1);

        {
            UnkStruct_ov70_0225CA20 *v9;
            UnkStruct_ov70_0225CC54 *v10;

            v9 = ov49_02259FF0(param1);
            v10 = ov49_02258DAC(v9);

            ov49_02258EEC(v9, v10, 1);
        }

        return 1;
    }

    return 0;
}

BOOL ov49_022649F4(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    UnkStruct_ov70_02268390 *v0;
    WiFiPlazaSystem *v1;
    String *v2;

    v0 = ov49_0225EF84(param0);
    v1 = ov49_02259FE8(param1);

    switch (ov49_0225EF88(param0)) {
    case 0:
        PlaySE(1500);

        if (ov45_0222A330(v1) == 0) {
            if (ov45_0222A374(v1) == 1) {
                v2 = ov49_0225A30C(param1, 1, 78);

                ov49_0225A08C(param1, v2);
                ov49_0225EF8C(param0, 1);
                break;
            }
        } else {
            v2 = ov49_0225A30C(param1, 1, 3);

            ov49_0225A08C(param1, v2);
            ov49_0225EF8C(param0, 1);
            break;
        }

        {
            u32 v3;

            v3 = ov45_0222B134(v1);

            switch (v3) {
            case 0:
                ov49_0225A37C(param1, UnkEnum_ov66_022324D0_00, 0);
                v2 = ov49_0225A30C(param1, 1, 73);
                break;
            case 1:
                ov49_0225A37C(param1, UnkEnum_ov66_022324D0_01, 0);
                v2 = ov49_0225A30C(param1, 1, 73);
                break;
            case 2:
                ov49_0225A37C(param1, UnkEnum_ov66_022324D0_02, 0);
                v2 = ov49_0225A30C(param1, 1, 73);
                break;
            case 3:
                ov49_0225A37C(param1, UnkEnum_ov66_022324D0_05, 0);
                v2 = ov49_0225A30C(param1, 1, 77);
                break;
            case 4:
                ov49_0225A37C(param1, UnkEnum_ov66_022324D0_06, 0);
                v2 = ov49_0225A30C(param1, 1, 77);
                break;
            case 5:
                v2 = ov49_0225A30C(param1, 1, 74);
                break;
            case 6:
                v2 = ov49_0225A30C(param1, 1, 79);
                break;
            case 7:
                v2 = ov49_0225A30C(param1, 1, 75);
                break;
            default:
            case 8:
                v2 = ov49_0225A30C(param1, 1, 76);
                break;
            }

            ov49_0225A08C(param1, v2);
        }

        ov49_0225EF8C(param0, 1);
        break;
    case 1:
        if (ov49_0225A0AC(param1)) {
            ov49_0225EF8C(param0, 2);
        }
        break;
    case 2:
        ov49_0225A0EC(param1);

        {
            UnkStruct_ov70_0225CA20 *v4;
            UnkStruct_ov70_0225CC54 *v5;

            v4 = ov49_02259FF0(param1);
            v5 = ov49_02258DAC(v4);
            ov49_02258EEC(v4, v5, 1);
        }

        return 1;
    }

    return 0;
}

u32 ov49_02264B94(const WiFiPlazaProfile *param0) {
    UnkStruct_ov66_02231428 v0;

    v0 = ov45_0222A9C8(param0);

    switch (v0.unk_00_val2_unk_00) {
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        return 0;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        return 1;
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 0:
    case 1:
    case 2:
    case 3:
        return 2;
    }

    ((0) ? (void)0 : GF_AssertFail());
    return 1;
}
