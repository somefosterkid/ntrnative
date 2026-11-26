#include "global.h"

#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "save_arrays.h"
#include "string_util.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EF1F0(PCStorage *, u32, s32, s32 *);

void ov112_021EF31C(void *arg0, Ov112PokewalkerApp *arg1) {
    u16 sp18[11];
    s32 sp0[6];
    BoxPokemon *temp_r0_3;
    Pokemon *temp_r0_2;
    SaveData *temp_r0;
    u32 temp_r6;

    if (*(s32 *)((u8 *)arg0 + 0x14) == 0) {
        temp_r6 = *(u32 *)((u8 *)arg0 + 0xC);
        temp_r0 = arg1->saveData;
        if (temp_r6 == 0x12) {
            temp_r0_2 = Party_GetMonByIndex(SaveArray_Party_Get(temp_r0), *(s32 *)((u8 *)arg0 + 0x10));
            *(s16 *)((u8 *)arg0 + 0x18) = (s16)GetMonData(temp_r0_2, 5, NULL);
            *(s8 *)((u8 *)arg0 + 0x30) = (s8)GetMonData(temp_r0_2, 0x70, NULL);
            GetMonData(temp_r0_2, 0x75, arg0 + 0x1A);
            *(u8 *)((u8 *)arg0 + 0x31) = MonIsShiny(temp_r0_2);
            *(s8 *)((u8 *)arg0 + 0x32) = (s8)GetMonData(temp_r0_2, 0x6F, NULL);
            arg1->unk_1E42C = temp_r0_2;
            arg1->unk_1E430 = NULL;
            return;
        }
        temp_r0_3 = PCStorage_GetMonByIndexPair(SaveArray_PCStorage_Get(temp_r0), temp_r6, (u32) * (s32 *)((u8 *)arg0 + 0x10));
        if (GetBoxMonData(temp_r0_3, 0xAC, NULL) != 0) {
            arg1->unk_1E430 = temp_r0_3;
            arg1->unk_1E42C = NULL;
        }
        ov112_021EF1F0(SaveArray_PCStorage_Get(arg1->saveData), *(u32 *)((u8 *)arg0 + 0xC), *(s32 *)((u8 *)arg0 + 0x10), sp0);
        *(s16 *)((u8 *)arg0 + 0x18) = (s16)sp0[0];
        *(s8 *)((u8 *)arg0 + 0x30) = (s8) * (u16 *)((u8 *)(sp0) + 0xA);
        CopyU16StringArrayN(arg0 + 0x1A, sp18, 0xBU);
        *(u8 *)((u8 *)arg0 + 0x31) = (u8) * (u16 *)((u8 *)(sp0) + 0x10);
        *(s8 *)((u8 *)arg0 + 0x32) = (s8) * (u16 *)((u8 *)(sp0) + 0x12);
    }
}
