#include "global.h"

#include "pokeathlon/pokeathlon.h"

void ov96_021E9858(int arg0, int arg1, void *arg2, void *arg3);

void ov96_021E98D0(int param_1, int param_2, void *param_3, void *param_4);
void ov96_021E98F4(int param_1, int param_2, void *param_3, void *param_4);

void ov96_021E9858(int arg0, int arg1, void *arg2, void *data) {
    PokeathlonCourse_SetStateTransitionType(data, 0xAU);
    PokeathlonCourse_SetStateField07(data, 0x12U);
}

void ov96_021E9870(int arg0, int arg1, void *arg2, void *arg3);

void ov96_021E9870(int arg0, int arg1, void *arg2, void *data) {
    void *v0;
    u32 v1;
    u8 *v3;
    u8 v2;

    v0 = arg2;
    v3 = PokeathlonCourse_GetDataCopyArea(data);
    if (ov96_021E5F24(data) == 0) {
        v1 = PokeathlonCourse_GetUnkConstant4(data);
        memcpy(ov96_021E8A20(v3 + 0x28) + (arg0 * v1), v0, v1);
        PokeathlonCourse_IncrementField1EF(data);
        v2 = PokeathlonCourse_GetParticipantCount(data);
        if (v2 == PokeathlonCourse_GetField1EF(data)) {
            PokeathlonCourse_ResetField1EF(data);
            PokeathlonCourse_SetStateField07(data, 0x15U);
        }
    }
}

void ov96_021E98D0(int param_1, int param_2, void *param_3, void *param_4) {
    u8 *puVar1;

    puVar1 = PokeathlonCourse_GetDataCopyArea(param_4);
    memcpy(puVar1 + 0xf0, param_3, param_2);
    PokeathlonCourse_SetStateField07(param_4, 0x16);
    return;
}

void ov96_021E98F4(int param_1, int param_2, void *param_3, void *param_4) {
    u8 *src = param_3;
    u8 uVar1;
    u8 uVar2;
    BOOL BVar3;
    PokeathlonFieldData *pPVar4;
    u8 *iVar5;
    s32 iVar6;
    u32 uVar7;

    PokeathlonCourse_GetDataCopyArea(param_4);
    BVar3 = ov96_021E5F24(param_4);
    if (BVar3 == 0) {
        pPVar4 = PokeathlonCourse_GetFieldData_AtIndex(param_4, param_1 & 0xff);
        uVar7 = 0;
        do {
            iVar6 = uVar7 * 0x20;
            iVar5 = src + iVar6;
            *(s32 *)(pPVar4->data + iVar6) = *(s32 *)(pPVar4->data + iVar6) + *(s32 *)(src + iVar6);
            *(s32 *)(pPVar4->data + iVar6 + 4) = *(s32 *)(pPVar4->data + iVar6 + 4) + *(s32 *)(iVar5 + 4);
            *(s32 *)(pPVar4->data + iVar6 + 8) = *(s32 *)(pPVar4->data + iVar6 + 8) + *(s32 *)(iVar5 + 8);
            *(s32 *)(pPVar4->data + iVar6 + 0xc) = *(s32 *)(pPVar4->data + iVar6 + 0xc) + *(s32 *)(iVar5 + 0xc);
            *(s32 *)(pPVar4->data + iVar6 + 0x10) = *(s32 *)(pPVar4->data + iVar6 + 0x10) + *(s32 *)(iVar5 + 0x10);
            *(s32 *)(pPVar4->data + iVar6 + 0x14) = *(s32 *)(pPVar4->data + iVar6 + 0x14) + *(s32 *)(iVar5 + 0x14);
            *(s32 *)(pPVar4->data + iVar6 + 0x18) = *(s32 *)(pPVar4->data + iVar6 + 0x18) + *(s32 *)(iVar5 + 0x18);
            *(s32 *)(pPVar4->data + iVar6 + 0x1c) = *(s32 *)(pPVar4->data + iVar6 + 0x1c) + *(s32 *)(iVar5 + 0x1c);
            uVar7 = uVar7 + 1 & 0xff;
        } while (uVar7 < 3);
        PokeathlonCourse_IncrementField1EF(param_4);
        uVar1 = PokeathlonCourse_GetParticipantCount(param_4);
        uVar2 = PokeathlonCourse_GetField1EF(param_4);
        if (uVar1 == uVar2) {
            PokeathlonCourse_ResetField1EF(param_4);
            PokeathlonCourse_SetStateField07(param_4, 0x1e);
        }
    }
    return;
}
