#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "item.h"
#include "party.h"
#include "pokemon.h"
#include "unk_02005D10.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E6480(void *, s32);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F685C(void *, s32, s32, s32);

s32 ov14_021F40DC(void *);

s32 ov14_021E96C8(UnkStruct_ov14_boxapp *app);

s32 ov14_021EE26C(UnkStruct_ov14_boxapp *arg0) {
    Pokemon *mon;
    s32 slot;

    slot = arg0->unk21 - 0x1E;
    if (ov14_021E6480(arg0, slot) == 0) {
        PlaySE(0x5F3U);
        ov14_021F67B0(arg0, 6, 0x25);
        *((s32 *)&arg0->unk30) = 0x5D;
        return 6;
    }
    mon = Party_GetMonByIndex(arg0->party, slot);
    if (ItemIdIsMail((u16)GetMonData(mon, 6, NULL)) == 1) {
        PlaySE(0x5F3U);
        ov14_021F685C(arg0, 0, 6, 0x25);
        *((s32 *)&arg0->unk30) = 0x5D;
        return 6;
    }
    if (GetMonData(mon, 0xA2, NULL) != 0) {
        PlaySE(0x5F3U);
        ov14_021F685C(arg0, 0, 5, 0x25);
        *((s32 *)&arg0->unk30) = 0x5D;
        return 6;
    }
    arg0->unk25 = (u8) arg0->unk1F;
    ov14_021F3488(arg0, 2, 1);
    ov14_021F40DC(arg0);
    return ov14_021F0234(arg0, ov14_021E96C8, 0x5E);
}
