// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/battle_box.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/box.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/text_banks.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "save/save_control.h"
#include "text/system/btl_pokeparam_box_main_save_init.h"

// The battle box: the box of six Pokémon kept for link and Wi-Fi battles, with its name and the flags that lock
// it. The ROM has no string for the file, so the name is a guess (swan's getBattleBox). Block 0x31.

#define SAVE_BLOCK_BATTLE_BOX 0x31

static void copyPkmIntoBattleBox(BattleBoxSave *battleBox, u32 box, u32 slot, const BoxPkm *pkm);

// pokebw2's BattleBoxSave_GetSize
u32 func_0200ba78(void) {
    return sizeof(BattleBoxSave);
}

// pokebw2's BattleBoxSave_Init
void func_0200ba80(BattleBoxSave *battleBox) {
    u32 i;
    MsgData *msgData;

    MI_CpuFill8(battleBox, 0, sizeof(BattleBoxSave));
    for (i = 0; i < BATTLE_BOX_SLOTS; i++) {
        PML_PkmInit(&battleBox->pkm[0][i]);
    }
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_POKEPARAM_BOX_MAIN_SAVE_INIT, HEAPID_USER);
    if (msgData != NULL) {
        GFL_MsgDataLoadRawStr(msgData, BtlPokeparamBoxMainSaveInit_Text_006, battleBox->name, BATTLE_BOX_NAME_LENGTH);
        GFL_MsgDataFree(msgData);
    }
}

BattleBoxSave *getBattleBox(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_BATTLE_BOX);
}

static void copyPkmIntoBattleBox(BattleBoxSave *battleBox, u32 box, u32 slot, const BoxPkm *pkm) {
    battleBox->pkm[box][slot] = *pkm;
}

PokeParty *convertBoxedPokeSetToParty(BattleBoxSave *battleBox, HeapID heapId) {
    u32 i;
    PokeParty *party = PokeParty_Create(heapId);

    PokeParty_InitCore(party, BATTLE_BOX_SLOTS);
    for (i = 0; i < BATTLE_BOX_SLOTS; i++) {
        BoxPkm *boxPkm = getBoxSlotAddress(battleBox, 0, i);
        PartyPkm *pkm;

        if (!PML_PkmGetParam(boxPkm, PKM_PARAM_SPECIES_VALID, NULL)) {
            break;
        }
        pkm = boxPkmRegenToPartyPkm(boxPkm, HEAPID_TAIL(heapId));
        PokeParty_AddPkm(party, pkm);
        GFL_HeapFree(pkm);
    }
    return party;
}

void copySelectedPkmToBattleBlk(BattleBoxSave *battleBox, PokeParty *party) {
    u32 i;

    for (i = 0; i < BATTLE_BOX_SLOTS; i++) {
        PML_PkmInit(&battleBox->pkm[0][i]);
    }
    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        copyPkmIntoBattleBox(battleBox, 0, i, func_0201d624(PokeParty_GetPkm(party, i)));
    }
}

// pokebw2's BattleBoxSave_HasPkm
BOOL func_0200bba8(BattleBoxSave *battleBox) {
    BOOL result = FALSE;

    if (PML_PkmGetParam(getBoxSlotAddress(battleBox, 0, 0), PKM_PARAM_SPECIES_VALID, NULL)) {
        result = TRUE;
    }
    return result;
}

// pokebw2's BattleBoxSave_SetFlags
void func_0200bbc8(BattleBoxSave *battleBox, u32 mask) {
    battleBox->flags |= (u16)mask;
}

// pokebw2's BattleBoxSave_ClearFlags
void func_0200bbd8(BattleBoxSave *battleBox, u32 mask) {
    battleBox->flags &= (u16)~mask;
}

BoxPkm *getBoxSlotAddress(BattleBoxSave *battleBox, u32 box, u32 slot) {
    return &battleBox->pkm[box][slot];
}

// pokebw2's BattleBoxSave_CheckFlags
BOOL func_0200bbfc(BattleBoxSave *battleBox, u16 flags) {
    if (battleBox->flags & flags) {
        return TRUE;
    }
    return FALSE;
}
