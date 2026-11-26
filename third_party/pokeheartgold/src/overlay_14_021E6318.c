#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E62C8(void *, void *);

s32 ov14_021E6100(void *, u32, s32);
s32 ov14_021E6464(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);
s32 ov14_021F4A20(void *, u32);

s32 ov14_021F4958(void *, u32);
s32 ov14_021E6070(void *, u32, s32, void *);
void ov14_021E611C(struct UnkStruct_ov14_boxapp *, MoveEntry *, MoveEntry *);
s32 ov14_021E61BC(void *, void *);
s32 ov14_021E6210(void *, void *);
void ov14_021E637C(struct UnkStruct_ov14_boxapp *arg0);

static void ov14_021E6318(struct UnkStruct_ov14_boxapp *arg0, MoveEntry *arg1);

static void ov14_021E6318(UnkStruct_ov14_boxapp *arg0, MoveEntry *arg1) {
    u32 boxno;

    boxno = ov14_021E6464(arg0, arg1->arg);
    CopyPokemonToPokemon(Party_GetMonByIndex(arg0->party, arg1->idx - 0x1E), arg1->mon);
    if (GetMonData(arg1->mon, 5, NULL) == 0x1EC) {
        Mon_UpdateShayminForm(arg1->mon, 0);
    }
    PCStorage_PlaceMonInBoxFirstEmptySlot(arg0->storage, boxno, Mon_GetBoxMon(arg1->mon));
    ov14_021E6100(arg0, arg0->unk1F, arg1->idx);
    ov14_021F4958(arg0, boxno);
    ov14_021F4A20(arg0, boxno);
}

void ov14_021E637C(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    u32 v1;
    u32 from;
    u32 to;
    MoveSet *v2;

    v2 = arg0->sys->unk_C;
    if (v2->from != 0xFF) {
        v0 = Party_GetCount(arg0->party);
        to = v2->to;
        if (0x80 & to) {
            ov14_021E6464(arg0, to);
            v1 = v2->from;
            if (v1 < 0x1EU) {
                ov14_021E62C8(arg0, v2);
                return;
            }
            ov14_021E6318(arg0, &v2->e[v1 - 0x1E]);
            return;
        }
        from = v2->from;
        if (from < 0x1EU) {
            if (to < 0x1EU) {
                PCStorage_SwapMonsInBoxByIndexPair(arg0->storage, (u32) arg0->unk1F, from, to);
                ov14_021F4958(arg0, arg0->unk1F);
                ov14_021F4A20(arg0, arg0->unk1F);
                return;
            }
            if ((u32)(to - 0x1E) < (u32)v0) {
                ov14_021E611C(arg0, &v2->e[0], &v2->e[1]);
                return;
            }
            ov14_021E6184(arg0, v2);
            return;
        }
        if (to < 0x1EU) {
            if (ov14_021E6070(arg0, to, 0xAC, 0) != 0) {
                ov14_021E611C(arg0, &v2->e[1], &v2->e[0]);
                return;
            }
            ov14_021E61BC(arg0, &v2->e[v2->from - 0x1E]);
            return;
        }
        if ((u32)(to - 0x1E) < (u32)v0) {
            ov14_021E6210(arg0, v2);
            return;
        }
        ov14_021E627C(arg0, &v2->e[from - 0x1E]);
    }
}

s32 ov14_021E6464(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    return (arg1 & 0x7F) + (6 * ((s32) arg0->unk25 / 6));
}
