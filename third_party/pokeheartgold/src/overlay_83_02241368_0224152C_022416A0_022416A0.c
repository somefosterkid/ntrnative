#include "global.h"

#include "message_format.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "unk_02005D10.h"
#include "overlay_83_battle_castle_self_app.h"

u8 ov83_0223FD14(BattleCastleSelfApp *, s32, u8);

s32 ov83_02240C60(BattleCastleSelfApp *, s32, BoxPokemon *);

void ov83_02241E18(BattleCastleSelfApp *);

s32 ov83_022421E0(BattleCastleSelfApp *, s32);

s32 ov83_0224755C(struct BattleCastleAppSprite *, s32);

u8 ov83_02247768();

void ov83_022416A0(BattleCastleSelfApp *arg0, s32 arg1, u16 arg2);

void ov83_022416A0(BattleCastleSelfApp *arg0, s32 arg1, u16 arg2) {
    Pokemon *temp_r0;
    unsigned int new_var;
    new_var = arg1;
    temp_r0 = Party_GetMonByIndex(arg0->party, ov83_02247768(arg0->numSlots));
    SetMonData(temp_r0, 6, &arg2);
    if (arg0->selectedMonSlot == new_var) {
        ov83_02241E18(arg0);
        ov83_022421E0(arg0, 0);
    }
    ov83_0224755C(arg0->itemIconSprites[ov83_02247768(arg0->numSlots, new_var)], 1);
    ov83_02240C60(arg0, 0, Mon_GetBoxMon(temp_r0));
    BufferItemName(arg0->strTemplate, 1U, (u32)(arg2));
    arg0->printerID = ov83_0223FD14(arg0, 0x3B, 1);
    PlaySE(0x623U);
}
