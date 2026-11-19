#include "global.h"

#include "move.h"
#include "party.h"
#include "pokemon.h"
#include "overlay_83_battle_castle_self_app.h"

u8 ov83_02247768(u8, u8);

void ov83_02241E18(BattleCastleSelfApp *arg0);

void ov83_02241E18(BattleCastleSelfApp *arg0) {
    s32 decrypt_result;
    Pokemon *mon;
    u16 i;
    u16 ppUps;
    u8 ppUpsByte;

    mon = Party_GetMonByIndex(arg0->party, ov83_02247768(arg0->numSlots, arg0->selectedMonSlot));
    decrypt_result = AcquireMonLock(mon);
    arg0->mon = mon;
    arg0->boxMon = Mon_GetBoxMon(mon);
    arg0->species = (s16)GetMonData(mon, MON_DATA_SPECIES, NULL);
    arg0->level = (u8)GetMonData(mon, 0xA1, NULL);
    arg0->unk_810 = (s8)GetMonData(mon, 0xA, NULL);
    arg0->nature = GetMonNature(mon);
    arg0->heldItem = (u16)GetMonData(mon, 6, NULL);
    arg0->stats[0] = (u16)GetMonData(mon, 0xA3, NULL);
    arg0->stats[1] = (u16)GetMonData(mon, 0xA4, NULL);
    arg0->stats[2] = (u16)GetMonData(mon, 0xA5, NULL);
    arg0->stats[3] = (u16)GetMonData(mon, 0xA8, NULL);
    arg0->stats[4] = (u16)GetMonData(mon, 0xA6, NULL);
    arg0->stats[5] = (u16)GetMonData(mon, 0xA9, NULL);
    arg0->stats[6] = (u16)GetMonData(mon, 0xA7, NULL);
    arg0->unk_826 = (s8)GetMonData(mon, 0x70, NULL);
    arg0->personality = GetMonData(mon, 0, NULL);
    if (GetMonData(mon, MON_DATA_NO_PRINT_GENDER, NULL) == 1) {
        arg0->noPrintGender = 0;
    } else {
        arg0->noPrintGender = 1;
    }
    arg0->gender = GetMonGender(mon);
    i = 0;
    do {
        arg0->moves[i] = (u16)GetMonData(mon, i + MON_DATA_MOVE1, NULL);
        arg0->movePP[i] = (s8)GetMonData(mon, i + 0x3A, NULL);
        ppUps = (u16)GetMonData(mon, i + 0x3E, NULL);
        ppUpsByte = (u8)ppUps;
        arg0->moveMaxPP[i] = GetMoveMaxPP(arg0->moves[i], ppUpsByte);
        i += 1;
    } while (((u32)i) < 4U);
    ReleaseMonLock(mon, decrypt_result);
}
