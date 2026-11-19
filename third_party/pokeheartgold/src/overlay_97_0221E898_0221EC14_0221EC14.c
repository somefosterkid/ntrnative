#include "global.h"

#include "pokeathlon/pokeathlon_save.h"

#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "save_arrays.h"
#include "string_util.h"

typedef struct UnkStruct_ov97_0221EC14_arg2 {
    u8 unk_0[0xC];
    void *unk_C;
    u8 unk_10[0x1C];
    SaveData *unk_2C;
} UnkStruct_ov97_0221EC14_arg2;

s32 ov97_0221E898(PCStorage *, u32, s32, s32 *);

s32 ov97_0221EA88(Party *, s32, s32 *);

s32 ov97_0221EB38(BoxPokemon *, void *);

s32 ov97_0221EBD8(BoxPokemon *, s32 *);

s32 ov97_0221EDE4(s32 *, s32 *, s32 *, u32 *);

s32 ov97_0221EFD0(void *, void *);

extern s32 ov97_0221FCB8;

typedef struct UnkStruct_ov97_0221EC14 {
    s32 w[5];
} UnkStruct_ov97_0221EC14;

typedef struct Sp70 {
    u32 w0;
    u16 h4;
    u8 b6;
    s8 b7;
    s8 b8;
    s8 b9;
    u16 name[11];
    s8 flags[5];
    u8 pad[0x2B];
} Sp70;

void ov97_0221EC14(u32 arg0, s32 arg1, UnkStruct_ov97_0221EC14_arg2 *arg2);

void ov97_0221EC14(u32 arg0, s32 arg1, UnkStruct_ov97_0221EC14_arg2 *arg2) {
    Sp70 sp70;
    UnkStruct_ov97_0221EC14 words;
    u16 sp44[12];
    s32 sp2c[6];
    s32 sp24[2];
    s32 sp1c[2];
    u8 sp14[8];
    u8 spc[8];
    u8 sp6[6];
    u8 sp0[6];
    Sp70 *var_r6;
    s32 *var_r7;
    BoxPokemon *temp_r0_4;
    BoxPokemon *temp_r6;
    Party *temp_r0_2;
    Pokemon *temp_r0_3;
    u8 var_r4;
    u8 var_r4_2;
    void *temp_r3;

    temp_r3 = &ov97_0221FCB8;
    words = *(UnkStruct_ov97_0221EC14 *)temp_r3;
    if (arg0 != -1U) {
        if (arg1 == -1) {
            goto block_2;
        }
        sp70.b9 = 2;
        if (arg0 == 0x12) {
            temp_r0_2 = SaveArray_Party_Get(arg2->unk_2C);
            temp_r0_3 = Party_GetMonByIndex(temp_r0_2, arg1);
            sp70.w0 = GetMonData(temp_r0_3, 0, NULL);
            sp70.h4 = (u16)GetMonData(temp_r0_3, 5, NULL);
            sp70.b7 = (u8)GetMonData(temp_r0_3, 0x70, NULL);
            GetMonData(temp_r0_3, 0x75, sp70.name);
            sp70.b6 = MonIsShiny(temp_r0_3);
            sp70.b8 = (u8)GetMonData(temp_r0_3, 0x6F, NULL);
            temp_r6 = Mon_GetBoxMon(Party_GetMonByIndex(temp_r0_2, arg1));
            ov97_0221EA88(temp_r0_2, (u8)arg1, sp24);
            ov97_0221EBD8(temp_r6, sp1c);
            Party_GetMonAprijuiceModifiers(temp_r0_2, (struct PartyExtraSub *)(sp14 + 2), arg1);
            ov97_0221EDE4(sp1c, sp24, (s32 *)(sp14 + 2), (u32 *)&sp70);
            var_r4 = 0;
            var_r6 = &sp70;
            var_r7 = words.w;
            do {
                if (PokeathlonSave_GetUnkDC_AtIndex(Save_Pokeathlon_Get(arg2->unk_2C), var_r7[var_r4], (s32)var_r6->h4) != 0) {
                    var_r6->flags[var_r4] = 1;
                } else {
                    var_r6->flags[var_r4] = 0;
                }
                var_r4 += 1;
            } while ((u32)var_r4 < 5U);
            ov97_0221EFD0(arg2->unk_C, &sp70);
            return;
        }
        ov97_0221E898(SaveArray_PCStorage_Get(arg2->unk_2C), arg0, arg1, sp2c);
        sp70.w0 = sp2c[1];
        sp70.h4 = (u16)sp2c[0];
        sp70.b7 = (s8) * (u16 *)((u8 *)sp2c + 0xA);
        CopyU16StringArrayN(sp70.name, sp44, 0xBU);
        sp70.b6 = (u8) * (u16 *)((u8 *)sp2c + 0x10);
        sp70.b8 = (s8) * (u16 *)((u8 *)sp2c + 0x12);
        temp_r0_4 = PCStorage_GetMonByIndexPair(SaveArray_PCStorage_Get(arg2->unk_2C), arg0, (u32)arg1);
        ov97_0221EB38(temp_r0_4, spc + 2);
        ov97_0221EBD8(temp_r0_4, (s32 *)sp6);
        MI_CpuFill8(sp0, 0U, 5U);
        ov97_0221EDE4((s32 *)sp6, (s32 *)(spc + 2), (s32 *)sp0, (u32 *)&sp70);
        var_r4_2 = 0;
        var_r6 = &sp70;
        var_r7 = words.w;
        do {
            if (PokeathlonSave_GetUnkDC_AtIndex(Save_Pokeathlon_Get(arg2->unk_2C), var_r7[var_r4_2], (s32)var_r6->h4) != 0) {
                var_r6->flags[var_r4_2] = 1;
            } else {
                var_r6->flags[var_r4_2] = 0;
            }
            var_r4_2 += 1;
        } while ((u32)var_r4_2 < 5U);
        ov97_0221EFD0(arg2->unk_C, &sp70);
        return;
    }
block_2:
    sp70.b9 = 0;
    ov97_0221EFD0(arg2->unk_C, &sp70);
}
