#include "global.h"

#include "error_handling.h"
#include "options.h"
#include "party.h"
#include "pokemon.h"
#include "unk_02005D10.h"
#include "overlay_83_battle_castle_self_app.h"

u8 ov83_0223FD14(void *, s32, u8);

s32 ov83_02240C60(void *, s32, BoxPokemon *);

s32 ov83_0224152C(Pokemon *, u16);

u8 ov83_02247768(u8);

u8 ov83_0224777C(SaveData *, u8, u8);

s32 ov83_02247944(void *, u32);

extern void *_02247D0C;

void ov83_022415F4(BattleCastleSelfApp *arg0, s32 arg1, s32 arg2);

void ov83_022415F4(BattleCastleSelfApp *arg0, s32 arg1, s32 arg2) {
    Pokemon *temp_r6;

    temp_r6 = Party_GetMonByIndex(arg0->party, ov83_02247768(arg0->numSlots));
    ov83_0224777C(arg0->saveData, arg0->challengeType, 0);
    ov83_02240C60(arg0, 0, Mon_GetBoxMon(temp_r6));
    ov83_02247944(&arg0->windows[6], Options_GetFrame(arg0->options));
    *((s8 *)&arg0->printerID) = ov83_0223FD14(arg0, *(u16 *)((u8 *)&_02247D0C + ((arg2 - 1) * 2)), 1);
    switch (arg2) {
    case 1:
        ov83_0224152C(temp_r6, 0x18);
        break;
    case 2:
        ov83_0224152C(temp_r6, 0x29);
        break;
    case 3:
        ov83_0224152C(temp_r6, 0x18);
        ov83_0224152C(temp_r6, 0x29);
        break;
    default:
        GF_AssertFail();
        break;
    }
    PlaySE(0x5ECU);
}
