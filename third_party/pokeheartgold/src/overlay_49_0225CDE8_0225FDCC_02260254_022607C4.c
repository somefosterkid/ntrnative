#include "global.h"

#include "overlay_45_wifi_plaza_api.h"
#include "overlay_49_unk_struct_ov70_0226da18.h"

#include "error_handling.h"
#include "overlay_42.h"
#include "list_menu_items.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "pm_string.h"
#include "save.h"

struct UnkStruct_ov70_022630A4_t;
typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;


void ov45_0222A4C8(WiFiPlazaSystem *param0, int param1);

void ov45_0222B118(WiFiPlazaSystem *param0, int param1);

u32 ov49_02258DAC(u32 arg0);

typedef struct {
    s16 x;
    s16 y;
} UnkStruct_ov49_022607C4;

UnkStruct_ov49_022607C4 ov49_02258E34(u32);

s32 ov49_02258E60(u32, u32);

void ov49_02258EAC(u32, u32, u32, int);

void ov49_02258EEC(u32, u32, u32);

WiFiPlazaSystem *ov49_02259FE8(void *);

u16 *ov49_02259FEC(void *);

u32 ov49_02259FF0(void *);

UnkStruct_ov70_022630A4 *ov49_0225A010(void *);

void ov49_0225A034(void *, u32);

void ov49_0225A038(void *, u32);

s32 ov49_0225A040(void *);

void ov49_0225A08C(void *, u32);

s32 ov49_0225A0AC(void *);

void ov49_0225A0EC(void *);

u32 ov49_0225A30C(void *, u32, u32);

void ov49_0225A37C(void *, int, u32);

void *ov49_0225EF40(void *, u32);

void ov49_0225EF68(void *);

void *ov49_0225EF84(void *);

u32 ov49_0225EF88(void *);

void ov49_0225EF8C(void *, u32);

void ov49_0225EF90(void *);

void ov49_0225EF98(UnkStruct_ov70_022630A4 *, u32, const UnkStruct_ov70_0226DA18 *, void *);

void ov49_0225EFC4(UnkStruct_ov70_022630A4 *, u32, const UnkStruct_ov70_0226DA18 *, void *);

extern const UnkStruct_ov70_0226DA18 ov49_02269B38;

extern const UnkStruct_ov70_0226DA18 ov49_02269B88;
#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov49_02262028(struct UnkStruct_ov70_02263344_t * param0, struct UnkStruct_ov70_0225DEE8_t * param1, u32 param2);

// asm keeps the DS copy (4-byte callback pointer and a literal 0); the 64-bit port emits 4-byte pointers (wordmix.py): the PC
// build uses this C definition instead.
const UnkStruct_ov70_0226DA18 ov49_02269B88 = { ov49_02262028, NULL };
#endif


u32 ov49_022607C4(void *param_1, void *param_2, u32 param_3);

u32 ov49_022607C4(void *param_1, void *param_2, u32 param_3) {
    UnkStruct_ov49_022607C4 sp24;
    u8 bVar2;
    u32 uVar6;
    UnkStruct_ov70_022630A4 *pUnk;
    u32 uVar5;
    u32 uVar7;
    WiFiPlazaSystem *uVar8;
    u32 uStack_38;
    BOOL bVar1;
    u8 uVar3;
    u8 *pcVar4;
    u32 uVar9;
    u8 *puVar10;
    u16 *puVar11;
    s32 sVar12;
    s32 iVar13;
    u32 unaff_r5;

    pcVar4 = (u8 *)ov49_0225EF84(param_1);
    pUnk = ov49_0225A010(param_2);
    uVar6 = ov49_02259FF0(param_2);
    uVar7 = ov49_02258DAC(uVar6);
    uVar8 = ov49_02259FE8(param_2);
    uVar9 = ov49_0225EF88(param_1);
    switch (uVar9) {
    case 0:
        puVar10 = ov49_0225EF40(param_1, 4);
        ov49_02258EEC(uVar6, uVar7, 0);
        ov49_0225EF90(param_1);
        *puVar10 = 0;
        iVar13 = ov49_0225A040(param_2);
        switch (iVar13) {
        case 30:
            puVar10[1] = 0;
            puVar10[2] = 0;
            puVar10[3] = 2;
            break;
        case 31:
            puVar10[1] = 1;
            puVar10[2] = 1;
            puVar10[3] = 3;
            break;
        case 32:
            puVar10[1] = 2;
            puVar10[2] = 2;
            puVar10[3] = 4;
            break;
        default:
            GF_AssertFail();
            break;
        }
        uVar8 = ov49_02259FE8(param_2);
        ov45_0222A5E8(uVar8, 0xc);
        ov49_0225EFC4(pUnk, param_3, &ov49_02269B88, puVar10);
        break;
    case 1:
        if (*pcVar4 == 8) {
            ov49_0225EF8C(param_1, 2);
        } else {
            ov49_0225EF8C(param_1, 3);
        }
        break;
    case 2:
        iVar13 = ov49_0225A040(param_2);
        switch (iVar13) {
        case 30:
            unaff_r5 = 3;
            uStack_38 = 0;
            break;
        case 31:
            unaff_r5 = 4;
            uStack_38 = 1;
            break;
        case 32:
            unaff_r5 = 5;
            uStack_38 = 2;
            break;
        }
        ov49_0225A034(param_2, 1);
        ov49_0225A038(param_2, unaff_r5 & 0xff);
        IncrementGameStat119(uVar8);
        ov45_0222B118(uVar8, uStack_38);
        sVar12 = ov49_02258E60(uVar7, 6);
        bVar2 = ov42_022282A4(sVar12);
        puVar11 = ov49_02259FEC(param_2);
        sp24 = ov49_02258E34(uVar7);
        puVar11[3] = 2;
        *puVar11 = sp24.x / 0x10;
        puVar11[1] = sp24.y / 0x10;
        puVar11[2] = (u16)bVar2;
        puVar11[4] = (u16)(u8)pcVar4[2];
        uVar8 = ov49_02259FE8(param_2);
        ov45_0222A4C8(uVar8, 1);
        ov49_0225EF68(param_1);
        return 1;
    case 3:
        sVar12 = ov49_02258E60(uVar7, 6);
        uVar3 = ov42_022282A4(sVar12);
        ov49_02258EAC(uVar6, uVar7, 2, uVar3);
        ov49_0225EF90(param_1);
        break;
    case 4:
        iVar13 = ov49_02258E60(uVar7, 5);
        if (iVar13 == 0) {
            ov49_0225EF90(param_1);
        }
        break;
    case 5:
        bVar1 = TRUE;
        switch (*pcVar4) {
        case 0:
            ov49_0225A37C(param_2, pcVar4[2], 0);
            unaff_r5 = 4;
            break;
        case 1:
            unaff_r5 = 5;
            break;
        case 2:
            ov49_0225A37C(param_2, pcVar4[2], 0);
            unaff_r5 = 6;
            break;
        case 3:
            unaff_r5 = 0x13;
            break;
        case 4:
            unaff_r5 = 0x11;
            break;
        case 6:
            unaff_r5 = 0x1d;
            break;
        case 7:
            unaff_r5 = 7;
            break;
        default:
            bVar1 = FALSE;
            break;
        }
        if (bVar1) {
            uVar5 = ov49_0225A30C(param_2, 0, unaff_r5);
            ov49_0225A08C(param_2, uVar5);
            ov49_0225EF90(param_1);
        } else {
            ov49_0225EF8C(param_1, 7);
        }
        break;
    case 6:
        iVar13 = ov49_0225A0AC(param_2);
        if (iVar13 == 1) {
            ov49_0225A0EC(param_2);
            ov49_0225EF90(param_1);
        }
        break;
    case 7:
        ov49_0225EF68(param_1);
        uVar8 = ov49_02259FE8(param_2);
        ov45_0222A5E8(uVar8, 1);
        ov49_02258EEC(uVar6, uVar7, 1);
        pUnk = ov49_0225A010(param_2);
        ov49_0225EF98(pUnk, param_3, &ov49_02269B38, 0);
    }
    return 0;
}
