#include "global.h"

#include "overlay_45_wifi_plaza_api.h"

#include "overlay_49_getters.h"

#include "overlay_49_unk_struct_ov49_02264cf8.h"

#include "overlay_42.h"
#include "pm_string.h"
#include "unk_02005D10.h"
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

struct UnkStruct_ov70_022630A4_t;

struct UnkStruct_ov70_02263344_t;

struct WiFiPlazaProfile;

struct Window;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;




void ov45_0222A704(WiFiPlazaSystem *param0, int param1, s32 param2);

void ov45_0222A72C(WiFiPlazaSystem *param0, u32 param1);

u32 ov45_0222AAC8(const WiFiPlazaProfile *profile);

void ov45_0222AB94(WiFiPlazaSystem *param0, u32 param1, u32 param2);

void ov45_0222ABD0(WiFiPlazaSystem *param0, u32 param1, u32 param2, u32 param3);

void ov45_0222AE64(WiFiPlazaSystem *param0);

BOOL ov45_0222AE74(WiFiPlazaSystem *param0, u32 param1);

void ov45_0222AED8(WiFiPlazaSystem *param0, u16 param1);

void ov45_0222AF80(WiFiPlazaSystem *param0);

void ov45_0222AFC4(WiFiPlazaSystem *param0);

BOOL ov45_0222B028(const WiFiPlazaSystem *param0);

int ov45_0222B034(const WiFiPlazaSystem *param0);

u16 ov45_0222B040(const WiFiPlazaSystem *param0);

BOOL ov45_0222B06C(const WiFiPlazaSystem *param0);

void ov45_0222B0E8(WiFiPlazaSystem *param0, u32 param1);

void ov66_0222EEF4(WiFiPlazaSystem *param0, u32 param1);

void ov66_0222EF44(WiFiPlazaSystem *param0, u32 param1);

void ov66_0222F020(WiFiPlazaSystem *param0);

void IncrementGameStat47(WiFiPlazaSystem *param0);

int ov49_0225CB70(UnkStruct_ov70_0225DEE8 *param0);

void ov45_0222B118(WiFiPlazaSystem *param0, int param1);

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0);

u32 ov49_02258E60(const UnkStruct_ov70_0225CC54 *param0, int param1);

void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2);

UnkStruct_ov70_0225CC54 *ov49_02258F40(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1);

void ov49_02259130(UnkStruct_ov70_0225CC54 *param0, BOOL param1);

void ov49_02259160(UnkStruct_ov70_0225CC54 *param0, int param1);

typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;

typedef struct ColoredArrow ColoredArrow;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);




void ov49_0225A08C(UnkStruct_ov70_0225DEE8 *param0, const String *param1);

void ov49_0225A0BC(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A0CC(UnkStruct_ov70_0225DEE8 *param0);

BOOL ov49_0225A0DC(const UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A0EC(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A174(UnkStruct_ov70_0225DEE8 *param0, const ListMenuTemplate *param1, s32 param2, s32 param3);

void ov49_0225A1A4(UnkStruct_ov70_0225DEE8 *param0, const ListMenuTemplate *param1, u32 param2, u32 param3, u8 param4, u8 param5, u8 param6);

u32 ov49_0225A1D4(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A1E4(UnkStruct_ov70_0225DEE8 *param0, u16 *param1, u16 *param2);

void ov49_0225A334(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);

void ov49_0225A428(UnkStruct_ov70_0225DEE8 *param0, u32 param1, BOOL param2);

void ov49_0225A4D0(UnkStruct_ov70_0225DEE8 *param0);

void *ov49_0225EF40(UnkStruct_ov70_02263344 *param0, u32 param1);

void ov49_0225EF68(UnkStruct_ov70_02263344 *param0);

void *ov49_0225EF84(UnkStruct_ov70_02263344 *param0);

u32 ov49_0225EF88(const UnkStruct_ov70_02263344 *param0);

void ov49_0225EF8C(UnkStruct_ov70_02263344 *param0, u32 param1);

void ov49_0225EFF0(UnkStruct_ov70_022630A4 *param0, u32 param1, BOOL param2);

String *ov49_02264C04(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);

String *ov49_02264C50(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov49_02264CA8(UnkStruct_ov49_02264CF8 *param0, WiFiPlazaSystem *param1, u8 param2);
#else
BOOL ov49_02264CA8(UnkStruct_ov49_02264CF8 *param0, WiFiPlazaSystem *param1, u32 param2);
#endif

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

void ov49_02265044(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2, u32 param3, u32 param4, u32 param5);

void ov49_02265260(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1);

void ov49_0226526C(UnkStruct_ov70_02269114 *param0);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov49_02265274(UnkStruct_ov70_02269114 *param0, const WiFiPlazaSystem *param1, s16 param2, s16 param3);
#else
void ov49_02265274(UnkStruct_ov70_02269114 *param0, const WiFiPlazaSystem *param1, u16 param2, u16 param3);
#endif

void ov49_0226529C(UnkStruct_ov70_02269114 *param0, WiFiPlazaSystem *param1);

BOOL ov49_022652D0(const UnkStruct_ov70_02269114 *param0);

u16 ov49_022652E0(const UnkStruct_ov70_02269114 *param0);

BOOL ov49_02262FB4(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov49_02262FB4(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    UnkStruct_ov49_02264CF8 *v0;
    WiFiPlazaSystem *v1;
    UnkStruct_ov70_022630A4 *v2;
    u32 v3;
    u32 v4;
    String *v5;
    const WiFiPlazaProfile *v6;
    const WiFiPlazaProfile *v7;

    v1 = ov49_02259FE8(param1);
    v2 = ov49_0225A010(param1);
    v0 = ov49_0225EF84(param0);
    v3 = ov45_0222B034(v1);
    v4 = ov45_0222B040(v1);

    if (ov45_0222B06C(v1)) {
        if (v0) {
            if (v0->unk_0A) {
                ov49_0225EF8C(param0, 32);
                ov49_0225A0CC(param1);
                v0->unk_0A = 0;
            }
        }
    }

    if (v0) {
        v6 = ov45_0222A5C0(v1);

        if (v0->unk_10) {
            v7 = ov45_0222A578(v1, v0->unk_03);
        } else {
            v7 = 0L;
        }
    }

    if (ov49_0225EF88(param0) > 0) {
        if (v0->unk_0A) {
            if (v7 == 0L) {
                ov49_0225EF8C(param0, 32);
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

        ov49_0226526C(&v0->unk_38);

        {
            UnkStruct_ov70_0225CC54 *v8;
            UnkStruct_ov70_0225CA20 *v9;
            UnkStruct_ov70_0225CC54 *v10;
            u32 v11;
            u32 v12;
            BOOL v13;

            v9 = ov49_02259FF0(param1);
            v10 = ov49_02258DAC(v9);
            v8 = ov49_02258F40(v9, v10);

            if (v8 == 0L) {
                v0->unk_0A = 0;
                ov49_0225EF8C(param0, 38);
                break;
            }

            v11 = ov49_02258E60(v8, 4);
            v12 = ov49_02258E60(v10, 6);
            v12 = ov42_022282A4(v12);

            ov45_0222AE64(v1);

            v13 = ov49_02264CA8(v0, v1, v11);
            v0->unk_10 = v8;

            ov49_02259130(v8, 0);
            ov49_02259160(v8, v12);
            ov49_0225EFF0(v2, v11, 0);
            PlaySE(1508);
            ov45_0222A5E8(v1, 9);

            if (v13 == 1) {
                ov49_0225A428(param1, v11, 0);
                ov49_0225EF8C(param0, 1);
            } else {
                ov49_0225EF8C(param0, 34);
                break;
            }
        }
        break;
    case 1: {
        BOOL v14;

        v14 = ov45_0222AE74(v1, v0->unk_03);

        if (v14 == 1) {
            ov49_0225EF8C(param0, 2);
        } else {
            ov49_0225EF8C(param0, 34);
        }
    } break;
    case 2:
        switch (v3) {
        case 2:
            ov45_0222B0E8(v1, v0->unk_03);
            ov49_0225EF8C(param0, 3);
            break;
        case 0:
            ov49_0225EF8C(param0, 34);
            break;
        case 1:
            break;
        case 3:
        case 4:
        default:
            ov49_0225EF8C(param0, 32);
            break;
        }
        break;
    case 3:
        IncrementGameStat47(v1);
        ov45_0222B118(v1, 7);

        ov49_02264D4C(v0, v1, param1, v0->unk_04, v6, v7);
        ov49_02264CFC(v0, 128, 4, param0, 37);
        break;
    case 4:
        ov49_02264D4C(v0, v1, param1, v0->unk_05, v7, v6);
        ov49_02264CFC(v0, 128, 5, param0, 37);
        ov45_0222AED8(v1, 0);
        break;
    case 5:
        switch (v3) {
        case 2:
            ov49_0225A0CC(param1);

            if (ov45_0222B028(v1)) {
                ov49_0225EF8C(param0, 6);
            } else {
                ov49_0225EF8C(param0, 8);
            }
            break;
        case 3:
            if (ov49_0225A0DC(param1) == 0) {
                ov49_0225A0BC(param1);
            }

            ov49_02264EC8(v0, param1);
            break;
        case 0:
        case 4:
        case 1:
        default:
            ov49_0225EF8C(param0, 32);
            ov49_0225A0CC(param1);
            break;
        }
        break;
    case 6: {
        BOOL v15;

        v15 = ov49_02264E20(v0, v1, param1);

        if (v15 == 1) {
            ov49_02264CFC(v0, 128, 7, param0, 37);
        } else {
            ov49_0225EF8C(param0, 7);
        }
    } break;
    case 7:
        ov49_0225A334(param1, v0->unk_03, 0);

        v5 = ov49_02264C04(param1, v0->unk_03, 40);

        ov49_0225A08C(param1, v5);
        ov49_02264CFC(v0, 128, 9, param0, 37);
        break;
    case 8:
        ov49_0225A334(param1, v0->unk_03, 0);
        ov49_0225A334(param1, v0->unk_08, 1);

        v5 = ov49_02264C50(param1, v0->unk_03, v0->unk_08);

        ov49_0225A08C(param1, v5);
        ov49_02264CFC(v0, 128, 9, param0, 37);
        break;
    case 9:
        ov49_0225A334(param1, v0->unk_03, 0);

        v5 = ov49_02264C04(param1, v0->unk_03, 46);

        ov49_0225A08C(param1, v5);
        ov49_02264CFC(v0, 128, 11, param0, 37);
        break;
    case 10:
        ov49_0225A334(param1, v0->unk_03, 0);

        v5 = ov49_02264C04(param1, v0->unk_03, 54);

        ov49_0225A08C(param1, v5);
        ov49_02264CFC(v0, 128, 11, param0, 37);
        break;
    case 11:
        ov49_02264F9C(&v0->unk_14, param1, 3, 515, 0);
        ov49_0225A174(param1, &v0->unk_14.unk_00, 0, 0);
        ov49_02264F10(v0);
        ov49_0225EF8C(param0, 12);
        break;
    case 12: {
        u32 v16;
        BOOL v17;

        v16 = ov49_0225A1D4(param1);

        switch (v16) {
        case 0:
            ov49_0225EF8C(param0, 13);
            v17 = 1;
            break;
        case 1:
            ov49_0225EF8C(param0, 19);

            {
                u32 v18;

                v18 = ov49_02258E60(v0->unk_10, 4);
                ov45_0222AB94(v1, param2, v18);
            }

            v17 = 1;
            break;
        case 2:
            ov45_0222AF80(v1);
            ov49_0225EF8C(param0, 33);
            v17 = 1;
            break;
        default:
            ov49_02264F24(v0, param1);
            v17 = 0;
            break;
        }

        if (v17 == 1) {
            ov49_0225A1E4(param1, 0L, 0L);
            ov49_02265260(&v0->unk_14, param1);
            ov49_02264F1C(v0);
        }
    } break;
    case 13: {
        u32 v27;

        v27 = ov45_0222AAC8(v6);

        if (v27 == ov45_0222AAC8(v7)) {
            v0->unk_0C = 1;

            ov49_0225A334(param1, v0->unk_03, 0);
            ov49_02264E90(v0, v1, param1, 1, 0);

            v5 = ov49_02264C04(param1, v0->unk_03, 507);

            ov49_0225A08C(param1, v5);
            ov49_02264CFC(v0, 128, 18, param0, 37);
            ov45_0222AED8(v1, 4);
        } else {
            v0->unk_0C = 0;

            ov49_0225A334(param1, v0->unk_03, 0);
            ov49_02264E90(v0, v1, param1, 1, 1);
            ov49_02264E90(v0, v1, param1, 2, 0);

            v5 = ov49_02264C04(param1, v0->unk_03, 508);

            ov49_0225A08C(param1, v5);
            ov49_02264CFC(v0, 128, 14, param0, 37);
        }
    } break;
    case 14:
        ov49_02264F9C(&v0->unk_14, param1, 2, 518, 1);
        ov49_0225A174(param1, &v0->unk_14.unk_00, 0, 0);
        ov49_02264F10(v0);
        ov49_0225EF8C(param0, 15);
        break;
    case 15: {
        u32 v19;
        BOOL v20;

        v19 = ov49_0225A1D4(param1);

        switch (v19) {
        case 0:
            ov45_0222AED8(v1, 3);
            ov49_0225EF8C(param0, 16);

            {
                u32 v21;

                v21 = ov49_02258E60(v0->unk_10, 4);
                ov45_0222ABD0(v1, param2, v21, ov45_0222AAC8(v7));
            }
            v20 = 1;
            break;
        case 1:
            ov49_0225EF8C(param0, 10);
            v20 = 1;
            break;
        default:
            ov49_02264F24(v0, param1);
            v20 = 0;
            break;
        }

        if (v20 == 1) {
            ov49_0225A1E4(param1, 0L, 0L);
            ov49_02265260(&v0->unk_14, param1);
            ov49_02264F1C(v0);
        }
    } break;
    case 16:
        ov49_0225A334(param1, v0->unk_03, 0);
        ov49_02264E90(v0, v1, param1, 1, 0);

        v5 = ov49_02264C04(param1, v0->unk_03, 510);

        ov49_0225A08C(param1, v5);
        ov49_02264CFC(v0, 128, 17, param0, 37);

        {
            u32 v22;

            v22 = ov45_0222AAC8(v7);
            ov45_0222A72C(v1, v22);
        }
        break;
    case 17:
        PlaySE(1471);
        ov49_0225EF8C(param0, 18);
        break;
    case 18:
        switch (v3) {
        case 2:
            ov49_02264F78(v0, v7);

            if (v0->unk_0C == 1) {
                ov49_0225EF8C(param0, 33);
            } else {
                ov49_0225EF8C(param0, 10);
            }

            ov49_0225A0CC(param1);
            break;
        case 3:
            if (ov49_0225A0DC(param1) == 0) {
                ov49_0225A0BC(param1);
            }

            ov49_02264EC8(v0, param1);
            break;
        case 0:
        case 4:
        case 1:
        default:
            ov49_0225EF8C(param0, 32);
            ov49_0225A0CC(param1);
            break;
        }
        break;
    case 19:
        ov45_0222AED8(v1, 5);
        ov49_0225EF8C(param0, 20);
        break;
    case 20:
        ov49_02265044(&v0->unk_14, param1, 30, 4, 519, 0);
        ov49_0225A1A4(param1, &v0->unk_14.unk_00, 0, 0, 8, 3, 23);
        ov49_0225EF8C(param0, 21);
        ov49_02264F10(v0);
        break;
    case 21: {
        u32 v23;

        v23 = ov49_0225A1D4(param1);

        switch (v23) {
        case 0xffffffff:
        case 0xfffffffe:
            ov49_02264F24(v0, param1);
            break;
        default:
            ov49_0225A1E4(param1, 0L, 0L);
            ov49_02265260(&v0->unk_14, param1);
            v0->unk_00 = v23;
            ov49_0225EF8C(param0, 22);
            break;
        }
    } break;
    case 22:
        ov49_0225A334(param1, v0->unk_03, 0);

        v5 = ov49_02264C04(param1, v0->unk_03, 477 + v0->unk_00);

        ov49_0225A08C(param1, v5);
        ov49_02264CFC(v0, 128, 23, param0, 37);
        break;
    case 23:
        ov49_02264F9C(&v0->unk_14, param1, 4, 115 + (v0->unk_00 * 4), 0);
        ov49_0225A174(param1, &v0->unk_14.unk_00, 0, 0);
        ov49_0225EF8C(param0, 24);
        break;
    case 24: {
        u32 v24;

        v24 = ov49_0225A1D4(param1);

        switch (v24) {
        case 0:
        case 1:
        case 2:
        case 3:
            ov49_0225A1E4(param1, 0L, 0L);
            ov49_02265260(&v0->unk_14, param1);
            ov49_02264F1C(v0);

            v0->unk_01 = (v0->unk_00 * 4) + v24;

            ov49_0225EF8C(param0, 25);
            break;
        default:
            ov49_02264F24(v0, param1);
            break;
        }
    } break;
    case 25:
        ov49_02265274(&v0->unk_38, v1, 6 + v0->unk_01, 2);
        ov49_0225A334(param1, v0->unk_03, 0);

        v5 = ov49_02264C04(param1, v0->unk_03, 357 + v0->unk_01);

        ov49_0225A08C(param1, v5);
        ov49_02264CFC(v0, 128, 26, param0, 37);
        break;
    case 26:
        ov49_0225A334(param1, v0->unk_03, 0);

        v5 = ov49_02264C04(param1, v0->unk_03, 85 + v0->unk_00);

        ov49_0225A08C(param1, v5);
        ov49_02264CFC(v0, 128, 27, param0, 37);
        break;
    case 27: {
        BOOL v25;
        u16 v26;

        v25 = ov49_022652D0(&v0->unk_38);

        if (v25 == 1) {
            v26 = ov49_022652E0(&v0->unk_38);

            if (v26 == 2) {
                ov49_0225EF8C(param0, 28);
            } else {
                ov49_0225EF8C(param0, 32);
                ov49_0225A0CC(param1);
            }
        } else {
            if (ov49_0225A0DC(param1) == 0) {
                ov49_0225A0BC(param1);
            }
        }
    } break;
    case 28:
        switch (v3) {
        case 2:
            ov49_02264F78(v0, v7);
            ov49_0225EF8C(param0, 29);
            ov49_0225A0CC(param1);
            break;
        case 3:
            if (ov49_0225A0DC(param1) == 0) {
                ov49_0225A0BC(param1);
            }

            ov49_02264EC8(v0, param1);
            break;
        case 0:
        case 1:
        case 4:
        default:
            ov49_0225EF8C(param0, 32);
            ov49_0225A0CC(param1);
            break;
        }
        break;
    case 29: {
        u16 v27 = (v4 - 6);

        if ((v4 >= 126) || (v4 < 6)) {
            ov49_0225EF8C(param0, 35);
            break;
        }

        ov49_0225A334(param1, v0->unk_03, 0);
        v5 = ov49_02264C04(param1, v0->unk_03, 236 + v27);
        ov49_0225A08C(param1, v5);

        if (v27 == v0->unk_01) {
            ov49_02264CFC(v0, 128, 30, param0, 37);
        } else {
            ov49_02264CFC(v0, 128, 31, param0, 37);
        }
    } break;
    case 30:
        ov49_0225A334(param1, v0->unk_03, 0);

        v5 = ov49_02264C04(param1, v0->unk_03, 51);

        ov49_0225A08C(param1, v5);
        ov49_02264CFC(v0, 128, 10, param0, 37);
        break;
    case 31:
        ov49_0225A334(param1, v0->unk_03, 0);

        v5 = ov49_02264C04(param1, v0->unk_03, 50);

        ov49_0225A08C(param1, v5);
        ov49_02264CFC(v0, 128, 10, param0, 37);
        break;
    case 32: {
        u32 v31;
        u32 v32;

        ov49_0225A1E4(param1, 0L, 0L);
        ov49_02265260(&v0->unk_14, param1);

        v5 = ov49_02264C04(param1, v0->unk_03, 45);
        ov49_0225A08C(param1, v5);
        v0->unk_0A = 0;

        v32 = String_GetLength(v5);
        v31 = ov49_0225CB70(param1) * v32;
        v31 = (v31 >> 1) + 0x3c;

        if (v31 < 0x80) {
            v31 = 0x80;
        } else if (v31 > 0xff) {
            v31 = 0xff;
        }

        ov49_02264CFC(v0, (u8)v31, 38, param0, 36);
        ov45_0222AFC4(v1);
    } break;
    case 33:
        ov49_0225A334(param1, v0->unk_03, 0);

        v5 = ov49_02264C04(param1, v0->unk_03, 43);
        ov49_0225A08C(param1, v5);
        v0->unk_0A = 0;

        ov49_02264CFC(v0, 128, 38, param0, 36);
        break;
    case 34:
        v5 = ov49_02264C04(param1, v0->unk_03, 42);
        ov49_0225A08C(param1, v5);

        v0->unk_0A = 0;

        ov49_02264CFC(v0, 128, 38, param0, 36);
        break;
    case 35:
        ov49_0225A334(param1, v0->unk_03, 0);

        v5 = ov49_02264C04(param1, v0->unk_03, 356);
        ov49_0225A08C(param1, v5);
        v0->unk_0A = 0;

        ov49_02264CFC(v0, 128, 38, param0, 36);
        break;
    case 36:
        ov49_02264D14(v0, param0);
        break;
    case 37:
        ov49_02264D30(v0, param0, param1);
        break;
    case 38:
        if (v0->unk_44 == 1) {
            ov45_0222A704(v1, v0->unk_46, v0->unk_48);
        }

        ov45_0222AE64(v1);
        ov49_0225A0EC(param1);
        ov45_0222A5E8(v1, 1);

        {
            UnkStruct_ov70_0225CC54 *v28;
            UnkStruct_ov70_0225CA20 *v29;
            u32 v30;

            v29 = ov49_02259FF0(param1);
            v28 = ov49_02258DAC(v29);

            ov49_02258EEC(v29, v28, 1);

            if (v0->unk_10 != 0L) {
                ov49_02259130(v0->unk_10, 1);

                v30 = ov49_02258E60(v0->unk_10, 4);

                ov49_0225EFF0(v2, v30, 1);
                ov49_0225A4D0(param1);
            }
        }

        ov49_02264CF8(v0);
        ov49_0225EF68(param0);

        return 1;
    }

    ov49_02264F60(v0);
    ov49_0226529C(&v0->unk_38, v1);

    return 0;
}
