// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov215/mailtool.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/mailtool.h"
#include "constants/pokemon.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "system/game_data.h"
#include "nitro/mi.h"

// The mail screens' params (overlay 215's mailtool.c): what the field app call allocates, and reads the result of

// pokebw2's MailTool_CreateWriteParam
MailParam *func_ov136_021c3cc0(GameData *gameData, u32 a1, u8 partySlot, u32 mailId, u32 heapId) {
    void *mailBox = func_02008fec(gameData);
    MailParam *param = GFL_HeapAllocate(HEAPID_TAIL((HeapID)heapId), sizeof(MailParam), FALSE, "mailtool.c", 67);

    MIi_CpuClearFast(0, param, sizeof(MailParam));
    param->design = mailId;
    param->partySlot = partySlot;
    param->mailBox = mailBox;
    param->mode = MAIL_PARAM_MODE_WRITE;
    param->unk08 = a1;
    param->unk0C = 0;
    param->save = GameData_GetSaveControl(gameData);
    param->mail = CreateMailData(heapId);
    ResetMailData(param->mail);
    func_02008f48(param->mail, 0xff, partySlot, gameData);
    return param;
}

// pokebw2's MailTool_CreatePokemonViewParam
MailParam *func_ov136_021c3d44(GameData *gameData, PartyPkm *pkm, u32 heapId) {
    MailParam *param = GFL_HeapAllocate(HEAPID_TAIL((HeapID)heapId), sizeof(MailParam), FALSE, "mailtool.c", 151);

    MIi_CpuClearFast(0, param, sizeof(MailParam));
    param->mode = MAIL_PARAM_MODE_VIEW;
    param->save = GameData_GetSaveControl(gameData);
    param->mail = CreateMailData(heapId);
    PokeParty_GetParam(pkm, PKM_PARAM_MAIL, param->mail);
    return param;
}

// pokebw2's MailTool_CreateItemViewParam
MailParam *func_ov136_021c3da4(GameData *gameData, u32 mailId, u32 heapId) {
    MailParam *param = GFL_HeapAllocate(HEAPID_TAIL((HeapID)heapId), sizeof(MailParam), FALSE, "mailtool.c", 185);

    MIi_CpuClearFast(0, param, sizeof(MailParam));
    param->mode = MAIL_PARAM_MODE_VIEW;
    param->save = GameData_GetSaveControl(gameData);
    param->mail = CreateMailData(heapId);
    func_02008fa8(param->mail, mailId);
    return param;
}

// pokebw2's MailTool_IsWritten
BOOL func_ov136_021c3e00(MailParam *param) {
    return param->written;
}

// pokebw2's MailTool_GiveMail
BOOL func_ov136_021c3e04(MailParam *param, PartyPkm *pkm) {
    if (!func_ov136_021c3e00(param)) {
        return FALSE;
    }
    PokeParty_SetParam(pkm, PKM_PARAM_MAIL, (uintptr_t)param->mail);
    return TRUE;
}

// pokebw2's MailTool_FreeParam
void func_ov136_021c3e24(MailParam *param) {
    if (param->mail != NULL) {
        GFL_HeapFree(param->mail);
    }
    GFL_HeapFree(param);
}
