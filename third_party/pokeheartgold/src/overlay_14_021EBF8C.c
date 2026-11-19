#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "item.h"
#include "party.h"
#include "pokemon.h"
#include "unk_02005D10.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
BoxPokemon *ov14_021E60C0(void *, u32, u32);
s32 ov14_021E6480(void *, s32);
s32 ov14_021F0254(void *, s16);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F34C8(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F43F4(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F685C(void *, s32, s32, s32);
struct UnkStruct_ov14_boxsys;

s32 ov14_021E94BC(UnkStruct_ov14_boxapp *app);

void ov14_021EBF8C(void *arg0) {
    ov14_021F0234(arg0, ov14_021E94BC, 0xE);
}

s32 ov14_021EBF9C(UnkStruct_ov14_boxapp *arg0) {
    Pokemon *mon;
    s32 slot;
    void *v0;
    void *v1;
    void *v2;
    void *v3;

    if ((u32) arg0->unk21 >= 0x1EU) {
        ov14_021F3488(arg0, 2, 1);
        ov14_021F34C8(*((void * *)&arg0->sys), arg0->unk21, 0);
        slot = arg0->unk21 - 0x1E;
        if (ov14_021E6480(arg0, slot) == 0) {
            PlaySE(0x5F3U);
            ov14_021F67B0(arg0, 6, 0x25);
            v0 = *((void * *)&arg0->sys);
            *(u8 *)((u8 *)v0 + 0x44E) = (u8)(0x80 | *(u8 *)((u8 *)v0 + 0x44E));
            *((s32 *)&arg0->unk30) = 0xE;
            return 6;
        }
        mon = Party_GetMonByIndex(arg0->party, slot);
        if (ItemIdIsMail((u16)GetMonData(mon, 6, NULL)) == 1) {
            PlaySE(0x5F3U);
            ov14_021F685C(arg0, 0, 6, 0x25);
            v1 = *((void * *)&arg0->sys);
            *(u8 *)((u8 *)v1 + 0x44E) = (u8)(0x80 | *(u8 *)((u8 *)v1 + 0x44E));
            *((s32 *)&arg0->unk30) = 0xE;
            return 6;
        }
        if (GetMonData(mon, 0xA2, NULL) != 0) {
            PlaySE(0x5F3U);
            ov14_021F685C(arg0, 0, 5, 0x25);
            v2 = *((void * *)&arg0->sys);
            *(u8 *)((u8 *)v2 + 0x44E) = (u8)(0x80 | *(u8 *)((u8 *)v2 + 0x44E));
            *((s32 *)&arg0->unk30) = 0xE;
            return 6;
        }
        goto block_8;
    }
    ov14_021F43F4(*((void * *)&arg0->sys), 0);
    ov14_021F3488(arg0, 1, 1);
    ov14_021F34C8(*((void * *)&arg0->sys), arg0->unk21, 0);
block_8:
    if (GetBoxMonData(ov14_021E60C0(arg0, arg0->unk1F, arg0->unk21), 0x4C, NULL) != 0) {
        PlaySE(0x5F3U);
        ov14_021F67B0(arg0, 3, 0x25);
        v3 = *((void * *)&arg0->sys);
        *(u8 *)((u8 *)v3 + 0x44E) = (u8)(0x80 | *(u8 *)((u8 *)v3 + 0x44E));
        *((s32 *)&arg0->unk30) = 0xE;
        return 6;
    }
    ov14_021F67B0(arg0, 0, 0x25);
    return ov14_021F0254(arg0, 1);
}
