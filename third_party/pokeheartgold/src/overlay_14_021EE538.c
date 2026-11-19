#include "global.h"
struct UnkStruct_ov14_boxapp;
#include "pokemon_storage_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E8314(void *);
s32 ov14_021E8588(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F4CA0(void *);
s32 ov14_021F6654(u8 *, s32);
s32 ov14_021F685C(void *, s32, s32, s32);
s32 ov14_021E8048(void *);
void ov14_021EE5C8(void *arg0);

s32 ov14_021E99F0(UnkStruct_ov14_boxapp *app);

s32 ov14_021E98AC(UnkStruct_ov14_boxapp *app);
s32 ov14_021E952C(UnkStruct_ov14_boxapp *app);

void ov14_021EE538(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F29E4(arg0->sys, 9, 8);
    ov14_021F2A18(arg0->sys, 9, 0);
    ov14_021E8588(((u8 *)arg0->sys->unk_2F0));
    ov14_021E8314(((u8 *)arg0->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E99F0, 0x67);
}

s32 ov14_021EE578(UnkStruct_ov14_boxapp *arg0) {
    if (PCStorage_CountMonsAndEggsInBox(*((PCStorage * *)&arg0->storage), (u32) arg0->unk25) == 0x1E) {
        ov14_021F685C(arg0, 0, 4, 0x25);
        *((s32 *)&arg0->unk30) = 0x5E;
        return 6;
    }
    ov14_021F3488(arg0, 2, 0);
    ov14_021F6654(*((void **)&arg0->sys), 0x27);
    ov14_021F4CA0(arg0);
    return ov14_021F0234(arg0, ov14_021E98AC, 0x68);
}

void ov14_021EE5C8(void *arg0) {
    ov14_021E8048(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E952C, 0x69);
}
