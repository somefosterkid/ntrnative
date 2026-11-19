#include "global.h"

#include "overlay_05.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "party.h"
#include "pokemon.h"
#include "sprite_system.h"
#include "unk_02088288.h"

void ov05_0221D414(UnkStruct_ov10_0221FB28 *arg0, Party *party, s32 arg2, s32 arg3) {
    u32 v2;
    s32 v1;
    Party *v0;
    Pokemon *mon;
    u32 slot;
    u8 v3;
    u8 v5;

    v0 = party;
    v1 = arg2;
    v2 = (u32)(u8)Party_GetCount(party);
    v5 = 0;
    slot = 0;
    if (v1 > 0) {
        do {
            ManagedSprite_SetAnim(arg0->unk_198[arg3 + slot], 0);
            if (v2 > slot) {
                mon = Party_GetMonByIndex(v0, (s32)slot);
                if ((GetMonData(mon, 5, NULL) != 0) && (GetMonData(mon, 0x4C, NULL) == 0)) {
                    v3 = (u8)Pokemon_GetStatusIconId(mon);
                    if (v3 == 6) {
                        ManagedSprite_SetAnim(arg0->unk_198[arg3 + v5], 3);
                    } else if (v3 != 7) {
                        ManagedSprite_SetAnim(arg0->unk_198[arg3 + v5], 2);
                    } else {
                        ManagedSprite_SetAnim(arg0->unk_198[arg3 + v5], 1);
                    }
                    v5 += 1;
                }
            }
            slot = (u32)(u8)(slot + 1);
        } while ((s32)slot < v1);
    }
}
