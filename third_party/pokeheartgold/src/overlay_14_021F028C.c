#include "global.h"
#include "overlay_14.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;
#include "item.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E783C(void *, s32, s32);
s32 ov14_021E7930(void *, u32);
s32 ov14_021E8248(void *);
s32 ov14_021E82A8(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2DE8(void *, u32);
void ov14_021F028C(void *arg0, s32 arg1);
s32 ov14_021E8234(void *);
s32 ov14_021E8294(void *);
s32 ov14_021E8314(void *);
s32 ov14_021F08BC(void *);
s32 ov14_021F3190(void *, u32, s32);
s32 ov14_021F3F6C(void *);
void ov14_021F039C(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);
void ov14_021F3044(void *);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F34C8(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F685C(void *, u32, s32, s32);
s32 ov14_021F6928(void *, s32, s32);
void ov14_021F0418(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);
s32 ov14_021E8434(void *);
s32 ov14_021E8824(UnkStruct_ov14_boxsys *);
void ov14_021F04D4(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);
void ov14_021F0530(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);
s32 ov14_021E6480(void *, s32);
void ov14_021F0594(struct UnkStruct_ov14_boxapp *arg0, u32 arg1);
void ov14_021F0314(void *arg0, s32 arg1);

s32 ov14_021E92AC(UnkStruct_ov14_boxapp *app);

s32 ov14_021E8BA4(UnkStruct_ov14_boxapp *app);
s32 ov14_021E9C88(UnkStruct_ov14_boxapp *app);
s32 ov14_021EA068(UnkStruct_ov14_boxapp *app);
s32 ov14_021EA0B8(UnkStruct_ov14_boxapp *app);
s32 ov14_021E8D20(UnkStruct_ov14_boxapp *app);

void ov14_021F028C(void *arg0, s32 arg1) {
    u8 v0;
    u32 v1;

    v0 = ((UnkStruct_ov14_boxapp *)arg0)->unk1F;
    if (v0 == 0) {
        v1 = 0x11;
    } else {
        v1 = v0 - 1;
    }
    ((UnkStruct_ov14_boxapp *)arg0)->unk1F = v1;
    ov14_021F2DE8(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F);
    ov14_021E783C(arg0, ov14_021E7930(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F), 0);
    ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0, 2);
    if (ov14_021E8544(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0) == 1) {
        ov14_021E84A4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
        if ((u32)(((UnkStruct_ov14_boxapp *)arg0)->unk_0->unk8 - 2) <= 1U) {
            ov14_021E8248(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
            ov14_021E82A8(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
        }
    }
    ov14_021F0234(arg0, ov14_021E92AC, arg1);
}

s32 ov14_021E9370(UnkStruct_ov14_boxapp *app);

void ov14_021F0314(void *arg0, s32 arg1) {
    u8 v0;
    u32 v1;

    v0 = ((UnkStruct_ov14_boxapp *)arg0)->unk1F;
    if (v0 == 0x11) {
        v1 = 0;
    } else {
        v1 = v0 + 1;
    }
    ((UnkStruct_ov14_boxapp *)arg0)->unk1F = v1;
    ov14_021F2DE8(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F);
    ov14_021E783C(arg0, ov14_021E7930(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F), 1);
    ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 1, 4);
    if (ov14_021E8544(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0) == 1) {
        ov14_021E84A4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
        if ((u32)(((UnkStruct_ov14_boxapp *)arg0)->unk_0->unk8 - 2) <= 1U) {
            ov14_021E8248(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
            ov14_021E82A8(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
        }
    }
    ov14_021F0234(arg0, ov14_021E9370, arg1);
}

void ov14_021F039C(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    arg0->unk21 = arg1;
    ov14_021F3190(((void *)arg0->sys), arg0->unk21, 0);
    ov14_021F3F6C(arg0);
    ov14_021F08BC(arg0);
    if (ov14_021E8544(arg0->sys->unk_2F0) == 0) {
        ov14_021E8234(*(void **)&arg0->sys->unk_2F0);
        ov14_021E8294(*(void **)&arg0->sys->unk_2F0);
        ov14_021E8314(*(void **)&arg0->sys->unk_2F0);
    }
    ov14_021E84A4(arg0->sys->unk_2F0);
    *((s8 *)&arg0->unk22) = 0;
    ov14_021F0234(arg0, ov14_021E8BA4, 0xD);
}

void ov14_021F0418(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    arg0->unk21 = arg1;
    ov14_021F3190(((void *)arg0->sys), arg0->unk21, 0);
    ov14_021F3F6C(arg0);
    ov14_021F3044(arg0);
    ov14_021F3488(arg0, 1, 1);
    ov14_021F3488(arg0, 2, 1);
    ov14_021F34C8(((void *)arg0->sys), arg0->unk21, 0);
    ov14_021F685C(arg0, arg0->unk21, 1, 0x27);
    if (Party_GetCount(arg0->party) != 6) {
        ov14_021F6928(arg0, 0x28, 1);
    } else {
        ov14_021F6928(arg0, 0x28, 3);
    }
    ov14_021F08BC(arg0);
    ov14_021E84A4(arg0->sys->unk_2F0);
    if (ov14_021E8544(arg0->sys->unk_2F0) == 0) {
        ov14_021E8314(*(void **)&arg0->sys->unk_2F0);
    }
    *((s8 *)&arg0->unk22) = 2;
    ov14_021F0234(arg0, ov14_021E9C88, 0x57);
}

void ov14_021F04D4(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    arg0->unk21 = arg1;
    ov14_021F3190(((void *)arg0->sys), arg0->unk21, 1);
    ov14_021F3F6C(arg0);
    ov14_021F685C(arg0, arg0->unk21, 1, 0x27);
    ov14_021E8434(*(void **)&arg0->sys->unk_2F0);
    ov14_021E8314(*(void **)&arg0->sys->unk_2F0);
    ov14_021E8824(((void *)arg0->sys));
    ov14_021F0234(arg0, ov14_021EA068, 0x58);
}

void ov14_021F0530(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    arg0->unk21 = arg1;
    ov14_021F3190(((void *)arg0->sys), arg0->unk21, 1);
    ov14_021F3F6C(arg0);
    ov14_021E8434(*(void **)&arg0->sys->unk_2F0);
    ov14_021E8234(*(void **)&arg0->sys->unk_2F0);
    ov14_021E8294(*(void **)&arg0->sys->unk_2F0);
    ov14_021E8314(*(void **)&arg0->sys->unk_2F0);
    ov14_021E8824(((void *)arg0->sys));
    ov14_021F0234(arg0, ov14_021EA0B8, 0x4A);
}

void ov14_021F0594(UnkStruct_ov14_boxapp *arg0, u32 arg1) {
    Pokemon *mon;

    arg0->unk21 = arg1;
    ov14_021F3190(arg0->sys, arg0->unk21, 0);
    ov14_021F3F6C(arg0);
    mon = Party_GetMonByIndex(arg0->party, arg1 - 0x1E);
    if ((ov14_021E6480(arg0, arg1 - 0x1E) == 0) && (PCStorage_CountMonsInBox(arg0->storage, (u32) arg0->unk1F) == 0)) {
        ov14_021F6928(arg0, 0x28, 8);
    } else if (ItemIdIsMail((u16)GetMonData(mon, 6, NULL)) == 1) {
        ov14_021F6928(arg0, 0x28, 6);
    } else if (GetMonData(mon, 0xA2, NULL) != 0) {
        ov14_021F6928(arg0, 0x28, 7);
    } else {
        ov14_021F6928(arg0, 0x28, 0);
    }
    ov14_021F08BC(arg0);
    ov14_021E84A4(arg0->sys->unk_2F0);
    ov14_021E8314(((u8 *)arg0->sys->unk_2F0));
    *((s8 *)&arg0->unk22) = 1;
    ov14_021F0234(arg0, ov14_021E8D20, 0x27);
}
