#include "global.h"
#include "pm_string.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_42.h"

struct String;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_022630A4_t;

struct UnkStruct_ov70_02263344_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;


typedef u16 charcode_t;

BOOL ov45_0222A2E0(const WiFiPlazaSystem *param0);

void ov45_0222A310(WiFiPlazaSystem *param0);

BOOL ov45_0222A330(const WiFiPlazaSystem *param0);

BOOL ov45_0222A3A0(const WiFiPlazaSystem *param0);

extern void ov45_0222A5E8(WiFiPlazaSystem *param0, int param1);

extern void ov45_0222A704(WiFiPlazaSystem *param0, int param1, s32 param2);

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

extern UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0);

extern UnkStruct_ov63_0222CC3C ov49_02258E34(const UnkStruct_ov70_0225CC54 *param0);

extern u32 ov49_02258E60(const UnkStruct_ov70_0225CC54 *param0, int param1);

extern void ov49_02258EAC(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, int param2, int param3);

void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2);

typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
} UnkStruct_ov70_02263910;

extern WiFiPlazaSystem *ov49_02259FE8(UnkStruct_ov70_0225DEE8 *param0);

extern UnkStruct_ov70_02263910 *ov49_02259FEC(UnkStruct_ov70_0225DEE8 *param0);

UnkStruct_ov70_0225CA20 *ov49_02259FF0(UnkStruct_ov70_0225DEE8 *param0);

UnkStruct_ov70_022630A4 *ov49_0225A010(UnkStruct_ov70_0225DEE8 *param0);

extern void ov49_0225A034(UnkStruct_ov70_0225DEE8 *param0, BOOL param1);

extern void ov49_0225A038(UnkStruct_ov70_0225DEE8 *param0, BOOL param1);

void ov49_0225A08C(UnkStruct_ov70_0225DEE8 *param0, const String *param1);

BOOL ov49_0225A0AC(const UnkStruct_ov70_0225DEE8 *param0);

extern void ov49_0225A0EC(UnkStruct_ov70_0225DEE8 *param0);

String *ov49_0225A30C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2);

typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;

typedef BOOL (*UnkFuncPtr_ov70_0226DA18)(UnkStruct_ov70_02263344 *, UnkStruct_ov70_0225DEE8 *, u32);

typedef struct {
    UnkFuncPtr_ov70_0226DA18 unk_00;
    UnkFuncPtr_ov70_0226DA18 unk_04;
} UnkStruct_ov70_0226DA18;

extern void *ov49_0225EF40(UnkStruct_ov70_02263344 *param0, u32 param1);

extern void ov49_0225EF68(UnkStruct_ov70_02263344 *param0);

extern void *ov49_0225EF84(UnkStruct_ov70_02263344 *param0);

extern u32 ov49_0225EF88(const UnkStruct_ov70_02263344 *param0);

void ov49_0225EF8C(UnkStruct_ov70_02263344 *param0, u32 param1);

void ov49_0225EF90(UnkStruct_ov70_02263344 *param0);

extern void ov49_0225EF98(UnkStruct_ov70_022630A4 *param0, u32 param1, const UnkStruct_ov70_0226DA18 *param2, void *param3);

typedef struct {
    s32 unk_00;
    u16 unk_04;
    u16 unk_06;
    u32 unk_08;
} UnkStruct_ov70_02264324;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov70_02264974;

BOOL ov70_02263CC8(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_0226412C(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

extern const UnkStruct_ov70_0226DA18 ov49_02269B38;

BOOL ov49_02260A68(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov49_02260A68(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    UnkStruct_ov70_02264974 *v0;
    UnkStruct_ov70_022630A4 *v1;
    UnkStruct_ov70_0225CA20 *v2;
    UnkStruct_ov70_0225CC54 *v3;
    WiFiPlazaSystem *v4;
    String *v5;

    v0 = ov49_0225EF84(param0);
    v1 = ov49_0225A010(param1);
    v2 = ov49_02259FF0(param1);
    v3 = ov49_02258DAC(v2);
    v4 = ov49_02259FE8(param1);

    switch (ov49_0225EF88(param0)) {
    case 0:
        v0 = ov49_0225EF40(param0, sizeof(UnkStruct_ov70_02264324));
        ov45_0222A5E8(ov49_02259FE8(param1), 13);

        if (ov45_0222A330(v4) == 1) {
            v0->unk_00 = 122;
            ov49_0225EF8C(param0, 2);
            break;
        }

        if (ov45_0222A3A0(v4) == 1) {
            v0->unk_02 = 10;
        } else {
            if (ov45_0222A2E0(v4) == 1) {
                v0->unk_00 = 98;
                ov49_0225EF8C(param0, 2);
                break;
            } else {
                v0->unk_02 = 9;
            }
        }

        ov49_0225EF8C(param0, 1);
        break;
    case 1: {
        u32 v6;

        if (v0->unk_02 == 9) {
            v6 = 9;
        } else {
            v6 = 10;

            ov45_0222A310(v4);
        }

        ov49_0225A034(param1, 1);
        ov49_0225A038(param1, (u8)v6);
        ov45_0222A704(v4, 23, -1);

        {
            UnkStruct_ov63_0222CC3C v8;
            UnkStruct_ov70_02263910 *v9;
            int v10;

            v10 = ov49_02258E60(v3, 6);
            v10 = ov42_022282A4(v10);
            v9 = ov49_02259FEC(param1);
            v8 = ov49_02258E34(v3);

            v9->unk_06 = 3;
            v9->unk_00 = (v8.unk_00) / 16;
            v9->unk_02 = (v8.unk_02) / 16;
            v9->unk_04 = v10;
            v9->unk_08 = 0;
        }

        ov49_0225EF68(param0);

        return 1;
    } break;
    case 2: {
        int v12;

        v12 = ov49_02258E60(v3, 6);
        v12 = ov42_022282A4(v12);

        ov49_02258EAC(v2, v3, 2, v12);
        ov49_0225EF90(param0);
    } break;
    case 3: {
        u32 v13;

        v13 = ov49_02258E60(v3, 5);

        if (v13 == 0) {
            ov49_0225EF90(param0);
        }
    } break;
    case 4:
        v5 = ov49_0225A30C(param1, 1, v0->unk_00);
        ov49_0225A08C(param1, v5);
        ov49_0225EF90(param0);
        break;
    case 5:
        if (ov49_0225A0AC(param1) == 1) {
            ov49_0225A0EC(param1);
            ov49_0225EF90(param0);
        }
        break;
    case 6:
        ov49_0225EF68(param0);
        ov45_0222A5E8(ov49_02259FE8(param1), 1);
        ov49_02258EEC(v2, v3, 1);

        {
            UnkStruct_ov70_022630A4 *v14;

            v14 = ov49_0225A010(param1);
            ov49_0225EF98(v14, param2, &ov49_02269B38, 0L);
        }
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return 0;
}
