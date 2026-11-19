#include "global.h"
#include "overlay_45_wifi_plaza_system.h"

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_022630A4_t;

struct UnkStruct_ov70_02263344_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

BOOL ov45_0222A230(WiFiPlazaSystem *param0, u32 param1);

BOOL ov45_0222A550(const WiFiPlazaSystem *param0, u32 param1);

const WiFiPlazaProfile *ov45_0222A578(const WiFiPlazaSystem *param0, u32 param1);

u32 ov45_0222A920(const WiFiPlazaProfile *profile);

u32 ov45_0222AA5C(const WiFiPlazaProfile *profile);

BOOL ov45_0222AADC(const WiFiPlazaProfile *profile);

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

UnkStruct_ov70_0225CC54 *ov49_02258CB8(UnkStruct_ov70_0225CA20 *param0, u32 param1, u32 param2);

void ov49_02258D54(UnkStruct_ov70_0225CC54 *param0);

UnkStruct_ov70_0225CC54 *ov49_02258D70(UnkStruct_ov70_0225CA20 *param0, u32 param1);

void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2);

BOOL ov49_02258F38(const UnkStruct_ov70_0225CC54 *param0);

void ov49_02259130(UnkStruct_ov70_0225CC54 *param0, BOOL param1);

void ov49_022591C0(UnkStruct_ov70_0225CC54 *param0, BOOL param1);

typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;

WiFiPlazaSystem *ov49_02259FE8(UnkStruct_ov70_0225DEE8 *param0);

UnkStruct_ov70_0225CA20 *ov49_02259FF0(UnkStruct_ov70_0225DEE8 *param0);

UnkStruct_ov70_022630A4 *ov49_0225A010(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A04C(UnkStruct_ov70_0225DEE8 *param0, u8 param1, BOOL param2);

void ov49_0225A06C(UnkStruct_ov70_0225DEE8 *param0, u8 param1, BOOL param2);

typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;

typedef BOOL (*UnkFuncPtr_ov70_0226DA18)(UnkStruct_ov70_02263344 *, UnkStruct_ov70_0225DEE8 *, u32);

typedef struct {
    UnkFuncPtr_ov70_0226DA18 unk_00;
    UnkFuncPtr_ov70_0226DA18 unk_04;
} UnkStruct_ov70_0226DA18;

void *ov49_0225EF40(UnkStruct_ov70_02263344 *param0, u32 param1);

void ov49_0225EF68(UnkStruct_ov70_02263344 *param0);

void *ov49_0225EF84(UnkStruct_ov70_02263344 *param0);

u32 ov49_0225EF88(const UnkStruct_ov70_02263344 *param0);

void ov49_0225EF8C(UnkStruct_ov70_02263344 *param0, u32 param1);

void ov49_0225EF90(UnkStruct_ov70_02263344 *param0);

void ov49_0225EF98(UnkStruct_ov70_022630A4 *param0, u32 param1, const UnkStruct_ov70_0226DA18 *param2, void *param3);

void ov49_0225EFC4(UnkStruct_ov70_022630A4 *param0, u32 param1, const UnkStruct_ov70_0226DA18 *param2, void *param3);

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 unk_05[3];
} UnkStruct_ov70_02265CCC;

typedef struct {
    UnkStruct_ov70_02265CCC unk_00;
} UnkStruct_ov70_0226545C;

BOOL ov70_02265630(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_02265A84(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov49_02261DBC(UnkStruct_ov70_02265CCC *param0, WiFiPlazaSystem *param1, UnkStruct_ov70_0225DEE8 *param2, UnkStruct_ov70_0225CC54 *param3, u8 param4, int param5);

extern const UnkStruct_ov70_0226DA18 ov49_02269B40;

extern const UnkStruct_ov70_0226DA18 ov49_02269B58;

BOOL ov49_0226154C(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov49_0226154C(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    UnkStruct_ov70_0226545C *v0;
    UnkStruct_ov70_0225CC54 *v1;
    WiFiPlazaSystem *v2;
    UnkStruct_ov70_0225CA20 *v3;
    UnkStruct_ov70_022630A4 *v4;

    v2 = ov49_02259FE8(param1);
    v3 = ov49_02259FF0(param1);
    v1 = ov49_02258D70(v3, param2);
    v0 = ov49_0225EF84(param0);
    v4 = ov49_0225A010(param1);

    switch (ov49_0225EF88(param0)) {
    case 0:
        v0 = ov49_0225EF40(param0, sizeof(UnkStruct_ov70_0226545C));
        ov49_0225EF8C(param0, 1);
    case 1: {
        BOOL v5, v6;

        v5 = ov45_0222A230(v2, param2);
        v6 = ov45_0222A550(v2, param2);

        if ((v5 == 1) || (v6 == 1)) {
            ov49_0225EF8C(param0, 2);
        }
    } break;
    case 2: {
        const WiFiPlazaProfile *v7;

        v7 = ov45_0222A578(v2, param2);

        if (v7 == 0L) {
            ov49_0225EF8C(param0, 1);
            break;
        }

        if (ov45_0222AADC(v7) == 1) {
            v1 = ov49_02258CB8(v3, param2, ov45_0222AA5C(v7));

            if (v1) {
                ov49_022591C0(v1, 0);

                ov49_0225EF8C(param0, 3);
            }
        }
    } break;
    case 3: {
        u32 v9;
        const WiFiPlazaProfile *v10;
        BOOL v11;

        v10 = ov45_0222A578(v2, param2);

        if (v10 == 0L) {
            if (v1 != 0L) {
                ov49_02258D54(v1);
            }

            ov49_0225EF8C(param0, 1);
            break;
        }

        v9 = ov45_0222A920(v10);
        v11 = ov49_02261DBC(&v0->unk_00, v2, param1, v1, v9, 1);

        if (v11) {
            ov49_0225EF8C(param0, 5);
            ov49_02258EEC(v3, v1, 0);
            ov49_0225A06C(param1, param2, 1);
            ov49_0225A04C(param1, param2, 1);
            ov49_02259130(v1, 0);
            ov49_0225EFC4(v4, param2, &ov49_02269B58, &v0->unk_00);
        } else {
            ov49_02258EEC(v3, v1, 4);
            ov49_0225EF8C(param0, 4);
            ov49_0225A04C(param1, param2, 1);
            ov49_0225A06C(param1, param2, 1);
            ov49_022591C0(v1, 1);
        }
    } break;
    case 4: {
        BOOL v12;

        v12 = ov49_02258F38(v1);

        if (v12 == 1) {
            ov49_0225EF90(param0);
        }
    } break;
    case 5:
        ov49_0225EF68(param0);
        ov49_02258EEC(v3, v1, 2);
        ov49_0225EF98(v4, param2, &ov49_02269B40, 0L);
        ov49_0225A04C(param1, param2, 0);
        ov49_0225A06C(param1, param2, 0);
        break;
    }

    return 0;
}
