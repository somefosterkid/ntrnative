#include "global.h"

#include "bag.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "pokemon_types_def.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E6094(void *, u32, s32, s16 *);
s32 ov14_021E64D0(BoxPokemon *boxMon);
s32 ov14_021F2ED0(void *, u32, u32, s32);

void *ov14_021E60C0(void *, u32, u32);
s32 ov14_021E7588(void *, s32);
s32 ov14_021E88F8(void *);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F6654(void *, s32);
s32 ov14_021F68C0(void *, s32, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F43F4(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F685C(void *, s32, s32, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F40DC(void *);
#else
s32 ov14_021F40DC(void *);
#endif

s32 ov14_021F20F4(void *arg0) {
    s16 v0;
    u8 v1;

    if (Bag_AddItem(Save_Bag_Get(**(SaveData ***)arg0), ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_88C8, 1U, HEAP_ID_10) == 0) {
        PlaySE(0x5F3U);
        ov14_021F68C0(arg0, 6, 0x25);
        *((s32 *)&((UnkStruct_ov14_boxapp *)arg0)->unk30) = 0x7A;
        return 6;
    }
    v0 = 0;
    ov14_021E6094(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk21, 6, &v0);
    if (ov14_021E64D0(ov14_021E60C0(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F, ((UnkStruct_ov14_boxapp *)arg0)->unk21)) == 1) {
        v1 = ((UnkStruct_ov14_boxapp *)arg0)->unk21;
        ov14_021F2ED0(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F, v1, ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_4094[v1]);
    }
    ov14_021E7588(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk21);
    ov14_021F40DC(arg0);
    ov14_021F6654(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0x25);
    ov14_021F391C(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 1);
    ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0xB, 2);
    ov14_021E88F8(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    return 0x79;
}

void ov14_021F21B4(void *arg0) {
    PlaySE(0x60DU);
    *((s32 *)&((UnkStruct_ov14_boxapp *)arg0)->unk30) = 0xB3;
    ov14_021F0204(arg0, 1);
}

s32 ov14_021F21D0(void *arg0) {
    u32 v0;

    ov14_021F6654(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0x25);
    GridInputHandler_SetButtonInputMode(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, 1);
    v0 = *(u32 *)((u8 *)(*(void **)arg0) + 8);
    switch (v0) {
    case 0:
        ov14_021F685C(arg0, 0, 0, 0x27);
        ov14_021F3488(arg0, 2, 0);
        return 0x5B;
    case 1:
        ov14_021F685C(arg0, 0, 0, 0x27);
        ov14_021F43F4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 1);
        ov14_021F3488(arg0, 1, 0);
        return 0x51;
    case 2:
        ov14_021F43F4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 1);
        ov14_021F3488(arg0, 1, 0);
        return 0xC;
    case 3:
        ov14_021F43F4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 1);
        ov14_021F3488(arg0, 0x81, 0);
        return 0x75;
    default:
        return 0xC;
    }
}
