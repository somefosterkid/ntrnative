#include "global.h"

#include "overlay_80_022324C4.h"
#include "party.h"
#include "pokemon.h"
#include "overlay_80_battle_castle.h"

u32 sub_02030D84(void *, u8, u8, u8, void *);

void ov80_022329B4(struct BattleCastle *castle) {
    s32 v1;
    s16 ptr[2];
    s32 v0;
    Pokemon *mon;
    s32 slot;
    u8 v2;
    v2 = (u8)Party_GetCount(castle->playersParty);
    slot = 0;
    v0 = (s32)v2;
    if (((s32)v2) > 0) {
        do {
            mon = Party_GetMonByIndex(castle->playersParty, slot);
            ptr[1] = sub_02030D84(castle->castleSave, 2, (u8)slot, 0, 0);
            SetMonData(mon, 0xA3, &ptr[1]);
            *((s8 *)(&ptr[0])) = (s8)sub_02030D84(castle->castleSave, 3, (u8)slot, 0, 0);
            SetMonData(mon, 0x3A, &ptr[0]);
            *((s8 *)(&ptr[0])) = (s8)sub_02030D84(castle->castleSave, 3, (u8)slot, 1, 0);
            SetMonData(mon, 0x3B, &ptr[0]);
            *((s8 *)(&ptr[0])) = (s8)sub_02030D84(castle->castleSave, 3, (u8)slot, 2, 0);
            SetMonData(mon, 0x3C, &ptr[0]);
            *((s8 *)(&ptr[0])) = (s8)sub_02030D84(castle->castleSave, 3, (u8)slot, 3, 0);
            SetMonData(mon, 0x3D, &ptr[0]);
            v1 = (s32)sub_02030D84(castle->castleSave, 4, (u8)slot, 0, 0);
            SetMonData(mon, 0xA0, &v1);
            ptr[1] = sub_02030D84(castle->castleSave, 5, (u8)slot, 0, 0);
            SetMonData(mon, 6, &ptr[1]);
            slot += 1;
        } while (slot < v0);
    }
}
