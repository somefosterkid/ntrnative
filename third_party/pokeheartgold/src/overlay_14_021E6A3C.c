#include "global.h"

#include "item.h"
#include "party.h"
#include "pokemon.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E6480(void *, s32);

s32 ov14_021E6A3C(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2) {
    Pokemon *mon;

    if (ov14_021E6070(arg0, arg2, 0xAC, 0) != 0) {
        return 0;
    }
    if (ov14_021E6480(arg0, arg1 - 0x1E) == 0) {
        return 0;
    }
    mon = Party_GetMonByIndex(arg0->party, arg1 - 0x1E);
    if (ItemIdIsMail((u16)GetMonData(mon, 6, NULL)) == 1) {
        return 0;
    }
    if (GetMonData(mon, 0xA2, NULL) == 0) {
        return 1;
    }
    return 0;
}
