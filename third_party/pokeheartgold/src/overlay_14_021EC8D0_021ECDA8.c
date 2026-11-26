#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "item.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021E637C(void *arg0);
void ov14_021E765C(void *arg0);
s32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E6480(void *, s32);
s32 ov14_021E6548(void *, s32, s32);
s32 ov14_021E7588(void *, s32);
s32 ov14_021F08F0(void *);
s32 ov14_021F57B8(void *);
s32 ov14_021F685C(void *, s32, s32, s32);

s32 ov14_021ECDA8(UnkStruct_ov14_boxapp *arg0) {
    s32 sp0;
    MoveSet *temp_r7;
    s32 temp_r6;
    u8 temp_r6_2;
    s32 temp_r6_3;
    s32 temp_r4;
    u8 temp_r1;
    s32 temp_r0;
    Pokemon *temp_r0_2;
    temp_r7 = arg0->sys->unk_C;
    temp_r6 = temp_r7->to;
    temp_r4 = temp_r7->unk_EC;
    PlaySE(0x5EAU);
    ov14_021E637C(arg0);
    temp_r0 = 0x80 & temp_r6;
    sp0 = temp_r0;
    if (temp_r0 == 0) {
        ov14_021E6548(arg0, temp_r7->from, temp_r7->to);
    }
    ov14_021F08F0(arg0);
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)(arg0->unk21));
    if ((*((u8 *)&arg0->unk23[1])) != 0) {
        ov14_021F57B8(arg0);
    }
    if (ov14_021E6070(arg0, arg0->unk21, 0xAC, 0) == 0) {
        ov14_021E765C(arg0);
    } else if (temp_r6 != 0xFF) {
        if (sp0 != 0) {
            ov14_021E7588(arg0, arg0->unk21);
        }
    } else {
        temp_r6_2 = arg0->unk21;
        if ((((u32)temp_r6_2) >= 0x1EU) && (temp_r4 != temp_r6_2)) {
            temp_r6_3 = temp_r6_2 - 0x1E;
            temp_r0_2 = Party_GetMonByIndex(arg0->party, temp_r6_3);
            if (ItemIdIsMail((u16)GetMonData(temp_r0_2, 6, 0L)) == 1) {
                PlaySE(0x5F3U);
                ov14_021F685C(arg0, 0, 6, 0x25);
                *((s32 *)&arg0->unk30) = 0x2C;
                return 6;
            }
            if (GetMonData(temp_r0_2, 0xA2, 0L) != 0) {
                PlaySE(0x5F3U);
                ov14_021F685C(arg0, 0, 5, 0x25);
                *((s32 *)&arg0->unk30) = 0x2C;
                return 6;
            }
            if (ov14_021E6480(arg0, temp_r6_3) == 0) {
                PlaySE(0x5F3U);
                ov14_021F67B0(arg0, 6, 0x25);
                *((s32 *)&arg0->unk30) = 0x2C;
                return 6;
            }
            goto block_18;
        }
block_18:
        if (((temp_r4 != 0xFF) && (0x80 & temp_r4)) && ((arg0->unk1F) != ((temp_r4 ^ 0x80) + (6 * (((s32)(arg0->unk25)) / 6))))) {
            temp_r1 = arg0->unk21;
            if (((u32)temp_r1) < 0x1EU) {
                ov14_021F685C(arg0, 0, 4, 0x25);
            } else if (ov14_021E6480(arg0, temp_r1 - 0x1E) == 1) {
                ov14_021F685C(arg0, 0, 4, 0x25);
            } else {
                ov14_021F67B0(arg0, 6, 0x25);
            }
            PlaySE(0x5F3U);
            *((s32 *)&arg0->unk30) = 0x2C;
            return 6;
        }
    }

    GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
    return 0x29;
}
