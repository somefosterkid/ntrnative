#include "global.h"

#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "save_arrays.h"
#include "string_util.h"

void ov97_0221E898(PCStorage *, s32, u32, u32 *);

void ov97_0221EA88(Party *param_1, s32 param_2, u8 *param_3);

void ov97_0221EB38(BoxPokemon *param_1, u8 *param_2);

void ov97_0221E98C(u8 *param_1, u32 *param_2);

void ov97_0221E98C(u8 *param_1, u32 *param_2) {
    u8 uVar1;
    Party *pPVar2;
    Pokemon *mon;
    u32 uVar3;
    PCStorage *pPVar4;
    BoxPokemon *pBVar5;
    u32 *puVar6;
    s32 iVar7;
    s32 iStack_48;
    u16 *puStack_4c;
    u8 *pbStack_50;
    u16 auStack_24[11];
    u32 auStack_0c[6];

    param_2[1] = *(u32 *)(param_1 + 0x28);
    if (*(s32 *)(param_1 + 0x28) == 0) {
        iStack_48 = 0;
        puStack_4c = (u16 *)((u8 *)param_2 + 0x1e);
        puVar6 = param_2;
        pbStack_50 = (u8 *)(param_2 + 5);
        do {
            iVar7 = *(s32 *)(param_1 + 0x10);
            if (iVar7 == 0x12) {
                pPVar2 = SaveArray_Party_Get(*(SaveData **)*param_2);
                mon = Party_GetMonByIndex(pPVar2, *(s32 *)(param_1 + 0x14));
                uVar3 = GetMonData(mon, 5, (void *)0x0);
                *(s16 *)(puVar6 + 3) = (s16)uVar3;
                uVar3 = GetMonData(mon, 0, (void *)0x0);
                puVar6[4] = uVar3;
                uVar3 = GetMonData(mon, 0x70, (void *)0x0);
                *(s16 *)((u8 *)puVar6 + 0xe) = (s16)uVar3;
                GetMonData(mon, 0x75, puStack_4c);
                uVar1 = MonIsShiny(mon);
                *(u8 *)(puVar6 + 7) = uVar1;
                uVar3 = GetMonData(mon, 0x6f, (void *)0x0);
                *(s8 *)((u8 *)puVar6 + 0x1d) = (s8)uVar3;
                pPVar2 = SaveArray_Party_Get(*(SaveData **)*param_2);
                ov97_0221EA88(pPVar2, *(u32 *)(param_1 + 0x14) & 0xff, pbStack_50);
            } else {
                pPVar4 = SaveArray_PCStorage_Get(*(SaveData **)*param_2);
                ov97_0221E898(pPVar4, iVar7, *(u32 *)(param_1 + 0x14), auStack_0c);
                *(s16 *)(puVar6 + 3) = (s16)auStack_0c[0];
                puVar6[4] = auStack_0c[1];
                *(u16 *)((u8 *)puVar6 + 0xe) = ((u16 *)auStack_0c)[5];
                CopyU16StringArrayN(puStack_4c, auStack_24, 0xb);
                *(s8 *)(puVar6 + 7) = (s8)((u16 *)auStack_0c)[8];
                *(s8 *)((u8 *)puVar6 + 0x1d) = (s8)((u16 *)auStack_0c)[9];
                pPVar4 = SaveArray_PCStorage_Get(*(SaveData **)*param_2);
                pBVar5 = PCStorage_GetMonByIndexPair(pPVar4, *(u32 *)(param_1 + 0x10), *(u32 *)(param_1 + 0x14));
                ov97_0221EB38(pBVar5, pbStack_50);
            }
            param_1 = param_1 + 8;
            puStack_4c = puStack_4c + 0x14;
            puVar6 = puVar6 + 10;
            pbStack_50 = pbStack_50 + 0x28;
            iStack_48 = iStack_48 + 1;
        } while (iStack_48 < 3);
    }
    return;
}
