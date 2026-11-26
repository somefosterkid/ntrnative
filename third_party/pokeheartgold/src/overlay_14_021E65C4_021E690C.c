#include "global.h"

#include "item.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_box_funcs.h"

u32 ov14_021E6464(void *, u32);

s32 ov14_021E6070(void *, u32, s32, void *);
s32 ov14_021E6480(void *, s32);

s32 ov14_021E690C(UnkStruct_ov14_boxapp *arg0, u32 arg1, u32 arg2) {
    u32 sp0;
    Pokemon *temp_r0_2;
    Pokemon *temp_r0_3;
    s32 var_r7;
    u32 temp_r0;
    u32 var_r4;

    var_r4 = arg2;
    ov14_021E6070(arg0, var_r4, 0xAC, 0);
    if (0x80 & var_r4) {
        temp_r0 = ov14_021E6464(arg0, var_r4);
        sp0 = temp_r0;
        if ((temp_r0 == arg0->unk1F) || (PCStorage_CountMonsAndEggsInBox(arg0->storage, sp0) == 0x1E)) {
            return 0;
        }
        var_r4 ^= 0x80;
        var_r7 = 0;
        goto block_6;
    }
    sp0 = (u32) arg0->unk1F;
    var_r7 = ov14_021E6070(arg0, var_r4, 0xAC, 0);
block_6:
    if (arg1 >= 0x1EU) {
        if (ov14_021E6480(arg0, arg1 - 0x1E) == 0) {
            if (var_r7 == 0) {
                if ((var_r4 < 0x1EU) || (sp0 != arg0->unk1F)) {
                    return 0;
                }
                goto block_15;
            }
            if ((ov14_021E6070(arg0, var_r4, 0x4C, 0) != 0) && (var_r4 < 0x1EU)) {
                return 0;
            }
            goto block_15;
        }
block_15:
        if (var_r4 < 0x1EU) {
            temp_r0_2 = Party_GetMonByIndex(arg0->party, arg1 - 0x1E);
            if (ItemIdIsMail((u16)GetMonData(temp_r0_2, 6, NULL)) == 1) {
                return 0;
            }
            if (GetMonData(temp_r0_2, 0xA2, NULL) != 0) {
                return 0;
            }
        }
    } else {
        if ((var_r4 >= 0x1EU) && (var_r7 != 0)) {
            temp_r0_3 = Party_GetMonByIndex(arg0->party, var_r4 - 0x1E);
            if (ItemIdIsMail((u16)GetMonData(temp_r0_3, 6, NULL)) == 1) {
                return 0;
            }
            if (GetMonData(temp_r0_3, 0xA2, NULL) != 0) {
                return 0;
            }
            if ((ov14_021E6070(arg0, arg1, 0x4C, 0) != 0) && (ov14_021E6480(arg0, var_r4 - 0x1E) == 0)) {
                return 0;
            }
        }
    }
    return 1;
}
