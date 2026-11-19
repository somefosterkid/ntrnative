#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "party.h"
#include "pokemon.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E6480(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);

s32 ov14_021E6480(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    Pokemon *mon;
    u32 i;

    for (i = 0; i < (u32)Party_GetCount(arg0->party); i++) {
        if (i != arg1) {
            mon = Party_GetMonByIndex(arg0->party, i);
            if (GetMonData(mon, 0x4C, 0) == 0 && GetMonData(mon, 0xA3, 0) != 0) {
                return 1;
            }
        }
    }
    return 0;
}
