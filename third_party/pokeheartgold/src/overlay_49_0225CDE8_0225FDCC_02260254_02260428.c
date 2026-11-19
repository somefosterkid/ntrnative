#include "global.h"
#include "pm_string.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

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

/* DS: the callers were built against the wider return type below; the PC takes the definition's (abilint RET) */
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov42_022282A4(int param0);
#else
int ov42_022282A4(int param0);
#endif

typedef u16 charcode_t;

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

typedef enum {
    UnkEnum_ov66_022336B8_00,
    UnkEnum_ov66_022336B8_01,
    UnkEnum_ov66_022336B8_02,
    UnkEnum_ov66_022336B8_03,
    UnkEnum_ov66_022336B8_04,
} UnkEnum_ov66_022336B8;

typedef enum {
    UnkEnum_ov66_02232A84_00,
    UnkEnum_ov66_02232A84_01,
    UnkEnum_ov66_02232A84_02,
} UnkEnum_ov66_02232A84;

extern u8 ov45_0222A208(const WiFiPlazaSystem *param0);

BOOL ov45_0222A330(const WiFiPlazaSystem *param0);

extern void ov45_0222A5E8(WiFiPlazaSystem *param0, int param1);

extern u32 ov45_0222AB68(const WiFiPlazaSystem *param0);

extern u32 ov45_0222AB78(const WiFiPlazaSystem *param0, u32 param1);

extern void ov45_0222AC14(WiFiPlazaSystem *param0, enum PlazaMinigame param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, BOOL param7);

void ov66_0222EF94(WiFiPlazaSystem *param0, enum PlazaMinigame param1);

extern void ov45_0222B118(WiFiPlazaSystem *param0, int param1);

extern BOOL ov45_0222EB38(UnkEnum_ov66_022336B8 param0);

#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov45_0222EB74(void);
#else
UnkEnum_ov66_02232A84 ov45_0222EB74(void);
#endif

extern BOOL ov45_0222EBF0(UnkEnum_ov66_022336B8 param0);

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

UnkStruct_ov70_0225CC54 *ov49_02258D70(UnkStruct_ov70_0225CA20 *param0, u32 param1);

extern UnkStruct_ov63_0222CC3C ov49_02258E34(const UnkStruct_ov70_0225CC54 *param0);

extern u32 ov49_02258E60(const UnkStruct_ov70_0225CC54 *param0, int param1);

extern void ov49_02258EAC(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, int param2, int param3);

void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2);

BOOL ov49_02258F38(const UnkStruct_ov70_0225CC54 *param0);

typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
} UnkStruct_ov70_02263910;

WiFiPlazaSystem *ov49_02259FE8(UnkStruct_ov70_0225DEE8 *param0);

extern UnkStruct_ov70_02263910 *ov49_02259FEC(UnkStruct_ov70_0225DEE8 *param0);

UnkStruct_ov70_0225CA20 *ov49_02259FF0(UnkStruct_ov70_0225DEE8 *param0);

UnkStruct_ov70_022630A4 *ov49_0225A010(UnkStruct_ov70_0225DEE8 *param0);

extern void ov49_0225A034(UnkStruct_ov70_0225DEE8 *param0, BOOL param1);

extern void ov49_0225A038(UnkStruct_ov70_0225DEE8 *param0, BOOL param1);

int ov49_0225A040(const UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A08C(UnkStruct_ov70_0225DEE8 *param0, const String *param1);

BOOL ov49_0225A0AC(const UnkStruct_ov70_0225DEE8 *param0);

extern void ov49_0225A0EC(UnkStruct_ov70_0225DEE8 *param0);

String *ov49_0225A30C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2);

extern void ov49_0225A37C(UnkStruct_ov70_0225DEE8 *param0, enum PlazaMinigame param1, u32 param2);

typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;

typedef BOOL (*UnkFuncPtr_ov70_0226DA18)(UnkStruct_ov70_02263344 *, UnkStruct_ov70_0225DEE8 *, u32);

typedef struct {
    UnkFuncPtr_ov70_0226DA18 unk_00;
    UnkFuncPtr_ov70_0226DA18 unk_04;
} UnkStruct_ov70_0226DA18;

void *ov49_0225EF40(UnkStruct_ov70_02263344 *param0, u32 param1);

extern void ov49_0225EF68(UnkStruct_ov70_02263344 *param0);

void *ov49_0225EF84(UnkStruct_ov70_02263344 *param0);

u32 ov49_0225EF88(const UnkStruct_ov70_02263344 *param0);

void ov49_0225EF8C(UnkStruct_ov70_02263344 *param0, u32 param1);

void ov49_0225EF90(UnkStruct_ov70_02263344 *param0);

extern void ov49_0225EF98(UnkStruct_ov70_022630A4 *param0, u32 param1, const UnkStruct_ov70_0226DA18 *param2, void *param3);

typedef struct {
    s32 unk_00;
    u16 unk_04;
    u16 unk_06;
    u32 unk_08;
} UnkStruct_ov70_02264324;

BOOL ov70_02263CC8(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_0226412C(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

extern u32 ov49_02260C58(u32 param0);

extern u32 ov49_02260CC0(u32 param0);

extern u32 ov49_02260D28(u32 param0);

extern const UnkStruct_ov70_0226DA18 ov49_02269B38;

BOOL ov49_02260428(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov49_02260428(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    UnkStruct_ov70_0225CA20 *v0;
    UnkStruct_ov70_0225CC54 *v1;
    WiFiPlazaSystem *v2;
    u32 v3;
    UnkStruct_ov70_02264324 *v4;

    v0 = ov49_02259FF0(param1);
    v1 = ov49_02258D70(v0, param2);
    v3 = ov49_0225A040(param1);
    v4 = ov49_0225EF84(param0);
    v2 = ov49_02259FE8(param1);

    switch (ov49_0225EF88(param0)) {
    case 0:
        v4 = ov49_0225EF40(param0, sizeof(UnkStruct_ov70_02264324));

        {
            u8 v5;
            v5 = ov49_02260C58(v3);

            switch (v5) {
            case UnkEnum_ov66_022336B8_00:
                v4->unk_08 = UnkEnum_ov66_022324D0_03;
                break;
            case UnkEnum_ov66_022336B8_01:
                v4->unk_08 = UnkEnum_ov66_022324D0_04;
                break;
            case UnkEnum_ov66_022336B8_02:
                v4->unk_08 = UnkEnum_ov66_022324D0_05;
                break;
            case UnkEnum_ov66_022336B8_03:
                v4->unk_08 = UnkEnum_ov66_022324D0_06;
                break;
            default:
                ((0) ? (void)0 : GF_AssertFail());
                v4->unk_08 = UnkEnum_ov66_022324D0_05;
                break;
            }
        }

        if (ov45_0222A330(v2) == 1) {
            v4->unk_04 = 22;
            v4->unk_06 = 0;
            ov49_0225EF8C(param0, 5);
            break;
        }

        if (ov45_0222A208(v2) == 1) {
            v4->unk_04 = 72;
            v4->unk_06 = 1;
            ov49_0225EF8C(param0, 5);
            break;
        }

        {
            BOOL v6;
            u8 v7;

            v7 = ov49_02260C58(v3);

            if (ov45_0222EBF0((UnkEnum_ov66_022336B8)(v7)) == 0) {
                v4->unk_04 = 20;
                v4->unk_06 = 0;
                ov49_0225EF8C(param0, 3);
                break;
            }

            v6 = ov45_0222EB38((UnkEnum_ov66_022336B8)(v7));
            ((v6) ? (void)0 : GF_AssertFail());
            ov49_0225EF90(param0);
        }
        break;
    case 1: {
        UnkEnum_ov66_02232A84 v8;

        v8 = ov45_0222EB74();

        if (v8 == UnkEnum_ov66_02232A84_01) {
            ov49_0225EF8C(param0, 2);

            {
                u8 v9;
                int v11;
                u8 v12[4] = { 0 };
                u32 v13;
                u8 v14;
                WiFiPlazaSystem *v15;

                v15 = ov49_02259FE8(param1);
                v9 = ov45_0222AB68(v15);

                if ((v4->unk_08 == UnkEnum_ov66_022324D0_05) || (v4->unk_08 == UnkEnum_ov66_022324D0_06)) {
                    v14 = 0;

                    for (v11 = 0; v11 < 4; v11++) {
                        v13 = ov45_0222AB78(v15, v11);

                        if (v13 != 0xffffffff) {
                            if (v14 < 4) {
                                v12[v14] = v13;
                            }

                            v14++;
                        }
                    }
                } else {
                    v12[0] = param2;
                }

                ov45_0222AC14(v15, (enum PlazaMinigame)(v4->unk_08), v9, v12[0], v12[1], v12[2], v12[3], 0);
            }
        } else if (v8 == UnkEnum_ov66_02232A84_02) {
            v4->unk_04 = 20;
            v4->unk_06 = 0;
            ov49_0225EF8C(param0, 3);
        }
    } break;
    case 2: {
        u32 v16;
        u32 v17;
        u32 v18;

        v16 = ov49_02260CC0(v3);
        v17 = ov49_02260D28(v3);

        ov49_0225A034(param1, 1);
        ov49_0225A038(param1, (u8)v16);

        ov45_0222A5E8(ov49_02259FE8(param1), v17);

        switch (v4->unk_08) {
        case UnkEnum_ov66_022324D0_03:
        case UnkEnum_ov66_022324D0_04:
            v18 = 5;
            break;
        case UnkEnum_ov66_022324D0_05:
            v18 = 3;
            break;
        case UnkEnum_ov66_022324D0_06:
            v18 = 4;
            break;
        default:
            ((0) ? (void)0 : GF_AssertFail());
            break;
        }

        ov45_0222B118(v2, v18);

        {
            UnkStruct_ov63_0222CC3C v19;
            UnkStruct_ov70_02263910 *v20;
            int v21;

            v21 = ov49_02258E60(v1, 6);
            v21 = ov42_022282A4(v21);
            v20 = ov49_02259FEC(param1);
            v19 = ov49_02258E34(v1);

            v20->unk_06 = 1;
            v20->unk_00 = (v19.unk_00) / 16;
            v20->unk_02 = (v19.unk_02) / 16;
            v20->unk_04 = v21;
            v20->unk_08 = v4->unk_08;
        }

        ov49_0225EF68(param0);

        return 1;
    } break;
    case 3: {
        u8 v22;
        u8 v23;

        v23 = ov49_02258E60(v1, 6);

        switch (v23) {
        case 0:
            v22 = 6;
            break;
        case 1:
            v22 = 5;
            break;
        case 2:
            v22 = 8;
            break;
        case 3:
            v22 = 7;
            break;
        default:
            ((0) ? (void)0 : GF_AssertFail());
            break;
        }

        ov49_02258EEC(v0, v1, v22);
        ov49_0225EF90(param0);
    } break;
    case 4: {
        BOOL v24;

        v24 = ov49_02258F38(v1);

        if (v24 == 1) {
            v4->unk_00 = 8;
            ov49_0225EF8C(param0, 7);
        }
    } break;
    case 5: {
        u8 v25;

        v25 = ov49_02258E60(v1, 6);
        v25 = ov42_022282A4(v25);

        ov49_02258EAC(v0, v1, 2, v25);
        ov49_0225EF90(param0);
    } break;
    case 6: {
        u32 v26;

        v26 = ov49_02258E60(v1, 5);

        if (v26 == 0) {
            v4->unk_00 = 8;
            ov49_0225EF8C(param0, 7);
        }
    } break;
    case 7:
        v4->unk_00--;

        if (v4->unk_00 <= 0) {
            String *v27;

            ov49_0225A37C(param1, (enum PlazaMinigame)(v4->unk_08), 0);
            v27 = ov49_0225A30C(param1, v4->unk_06, v4->unk_04);
            ov49_0225A08C(param1, v27);
            ov49_0225EF90(param0);
        }
        break;
    case 8: {
        BOOL v28;

        v28 = ov49_0225A0AC(param1);

        if (v28) {
            ov49_0225EF68(param0);
            ov49_02258EEC(v0, v1, 1);
            ov49_0225A0EC(param1);

            {
                UnkStruct_ov70_022630A4 *v29;

                v29 = ov49_0225A010(param1);
                ov49_0225EF98(v29, param2, &ov49_02269B38, 0L);
            }
        }
    } break;
    }

    return 0;
}
