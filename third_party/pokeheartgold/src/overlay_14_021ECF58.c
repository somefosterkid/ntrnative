#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "item.h"
#include "party.h"
#include "pokemon.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E637C(void *);
s32 ov14_021E6480(void *, s32);
s32 ov14_021E6548(void *, s32, s32);
s32 ov14_021E7588(void *, s32);
void ov14_021E76B8(void *);
s32 ov14_021F08F0(void *);
s32 ov14_021F0CD8(void *);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F57B8(void *);
s32 ov14_021F685C(void *, s32, s32, s32);

s32 ov14_021ECF58(UnkStruct_ov14_boxapp *arg0) {
    Pokemon *mon;
    s32 v2;
    s32 v3;
    s32 slot;
    u8 v0;
    u8 v4;
    MoveSet *v1;

    v1 = arg0->sys->unk_C;
    v3 = v1->to;
    PlaySE(0x5EAU);
    ov14_021E637C(arg0);
    if (!(0x80 & v3)) {
        ov14_021E6548(arg0, v1->from, v1->to);
    }
    ov14_021F08F0(arg0);
    if (*((u8 *)&arg0->unk23[1]) != 0) {
        ov14_021F57B8(arg0);
    }
    v2 = GridInputHandler_GetNextInput(arg0->sys->unk_2C);
    if ((*((s32 *)&arg0->sys->unk_88CC) != 0) || (v3 != 0xFF) || (v4 = arg0->unk21, (v2 == v4))) {
        goto block_26;
    }
    if ((u32)v4 >= 0x1EU) {
        slot = v4 - 0x1E;
        mon = Party_GetMonByIndex(arg0->party, slot);
        if (ItemIdIsMail((u16)GetMonData(mon, 6, NULL)) == 1) {
            PlaySE(0x5F3U);
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32) arg0->unk21);
            GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 0);
            ov14_021F29E4(arg0->sys, 9, 8);
            ov14_021F685C(arg0, 0, 6, 0x25);
            *((s32 *)&arg0->unk30) = 0x2C;
            return 6;
        }
        if (GetMonData(mon, 0xA2, NULL) != 0) {
            PlaySE(0x5F3U);
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32) arg0->unk21);
            GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 0);
            ov14_021F29E4(arg0->sys, 9, 8);
            ov14_021F685C(arg0, 0, 5, 0x25);
            *((s32 *)&arg0->unk30) = 0x2C;
            return 6;
        }
        if (ov14_021E6480(arg0, slot) == 0) {
            PlaySE(0x5F3U);
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32) arg0->unk21);
            GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 0);
            ov14_021F29E4(arg0->sys, 9, 8);
            ov14_021F67B0(arg0, 6, 0x25);
            *((s32 *)&arg0->unk30) = 0x2C;
            return 6;
        }
        goto block_15;
    }
block_15:
    if (((u32)v2 >= 0x25U) && ((u32)v2 <= 0x2AU) && (arg0->unk1F != ((v2 - 0x25) + (6 * ((s32) arg0->unk25 / 6))))) {
        v0 = arg0->unk21;
        if ((u32)v0 < 0x1EU) {
            ov14_021F685C(arg0, 0, 4, 0x25);
        } else if (ov14_021E6480(arg0, v0 - 0x1E) == 1) {
            ov14_021F685C(arg0, 0, 4, 0x25);
        } else {
            ov14_021F67B0(arg0, 6, 0x25);
        }
        PlaySE(0x5F3U);
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32) arg0->unk21);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 0);
        ov14_021F29E4(arg0->sys, 9, 8);
        *((s32 *)&arg0->unk30) = 0x2C;
        return 6;
    }
    if (arg0->unk2A == 0) {
        ov14_021E7588(arg0, GridInputHandler_GetNextInput(arg0->sys->unk_2C));
    }
block_26:
    if (arg0->unk2A != 0) {
        ov14_021F29E4(arg0->sys, 9, 8);
        ov14_021E76B8(arg0);
        return ov14_021F0CD8(arg0);
    }
    if (((u32)v2 >= 0x25U) && ((u32)v2 <= 0x2AU)) {
        ov14_021F29E4(arg0->sys, 9, 0xE);
    } else {
        ov14_021F29E4(arg0->sys, 9, 8);
    }
    return 0x29;
}
