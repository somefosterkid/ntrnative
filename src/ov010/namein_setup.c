// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/namein_setup.c, which is the same code in
// Black 2/White 2's overlay 12, except that Black's parameters lack Black 2's last fields and Black has no function
// comparing the name. The name is the file's own; setupNameEntry, setupPokemonNameEntry and pokemonNameEntry are
// swan's names (https://github.com/ds-pokemon-hacking/swan, GPL-3.0).
#include "types.h"
#include "app/name_entry.h"
#include "constants/pokemon.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "pml/poke_party.h"

// The parameters of the name entry

#define NAME_ENTRY_NAME_LENGTH 11

NameEntryParam *setupNameEntry(HeapID heapId, u32 mode, u32 a2, u32 a3, u32 maxLength, const StrBuf *name,
                               TrainerGameInfoSave *gameInfo) {
    NameEntryParam *param = GFL_HeapAllocate(heapId, sizeof(NameEntryParam), FALSE, "namein_setup.c", 55);

    MI_CpuFill8(param, 0, sizeof(NameEntryParam));
    param->mode = mode;
    param->maxLength = maxLength;
    param->gender = a2;
    param->unk6 = a3;
    param->gameInfo = gameInfo;
    param->name = GFL_StrBufCreate(NAME_ENTRY_NAME_LENGTH, heapId);
    if (name != NULL) {
        GFL_StrBufCopy(param->name, name);
    }
    return param;
}

NameEntryParam *setupPokemonNameEntry(HeapID heapId, PartyPkm *pkm, u32 maxLength, const StrBuf *name,
                                      TrainerGameInfoSave *gameInfo) {
    NameEntryParam *param = GFL_HeapAllocate(heapId, sizeof(NameEntryParam), FALSE, "namein_setup.c", 89);
    u32 form;

    MI_CpuFill8(param, 0, sizeof(NameEntryParam));
    param->mode = NAME_ENTRY_POKEMON;
    param->maxLength = maxLength;
    param->pkm = pkm;
    param->gameInfo = gameInfo;
    if (pkm != NULL) {
        param->gender = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
        form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
        param->unk6 = form | (PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL) << 8);
    }
    param->name = GFL_StrBufCreate(NAME_ENTRY_NAME_LENGTH, heapId);
    if (name != NULL) {
        GFL_StrBufCopy(param->name, name);
    }
    return param;
}

NameEntryParam *pokemonNameEntry(HeapID heapId, PartyPkm *pkm, u32 maxLength, const StrBuf *name, StrBuf *unkC,
                                 BoxSaveAccessor *unk10, u32 unk14, TrainerGameInfoSave *gameInfo) {
    NameEntryParam *param = setupPokemonNameEntry(heapId, pkm, maxLength, name, gameInfo);

    param->unkC = unkC;
    param->unk10 = unk10;
    param->unk14 = unk14;
    return param;
}

void func_ov010_0216c0c0(NameEntryParam *param) {
    GFL_StrBufFree(param->name);
    GFL_HeapFree(param);
}

void func_ov010_0216c0d4(NameEntryParam *param, StrBuf *dest) {
    GFL_StrBufCopy(dest, param->name);
}

BOOL func_ov010_0216c0e4(NameEntryParam *param) {
    return param->unk1C;
}
