// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/mailtool.h: the lines in use so far.
#ifndef BWPORT_FIELD_MAILTOOL_H
#define BWPORT_FIELD_MAILTOOL_H

#include "types.h"
#include "struct_decls.h"

#define MAIL_PARAM_MODE_VIEW 0
#define MAIL_PARAM_MODE_WRITE 1

// The parameter of the mail screen (mailsys.c's MailSys_ProcInit and the others): the mail to show or to write
struct MailParam {
    u16 mode;
    // The mail was written and is handed over to its owner
    BOOL written;
    // The caller's own value, always 2 here
    u32 unk08;
    u16 unk0C;
    u8 partySlot;
    // The mail's design, which is the mail item's mail ID
    u8 design;
    SaveControl *save;
    MailData *mail;
    // The save's block of mail
    void *mailBox;
};

// pokebw2's MailTool_CreateWriteParam
// Writes a new mail of the given design, to give to the party Pokémon in partySlot
MailParam *func_ov136_021c3cc0(GameData *gameData, u32 a1, u8 partySlot, u32 mailId, u32 heapId);
// pokebw2's MailTool_CreatePokemonViewParam
// Shows the mail that a party Pokémon holds, or a mail item's blank design
MailParam *func_ov136_021c3d44(GameData *gameData, PartyPkm *pkm, u32 heapId);
// pokebw2's MailTool_CreateItemViewParam
MailParam *func_ov136_021c3da4(GameData *gameData, u32 mailId, u32 heapId);
// pokebw2's MailTool_GiveMail
// Gives the written mail to the Pokémon, and returns whether there was one
BOOL func_ov136_021c3e04(MailParam *param, PartyPkm *pkm);
// pokebw2's MailTool_FreeParam
void func_ov136_021c3e24(MailParam *param);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MailTool_IsWritten
// Whether the mail was written
BOOL func_ov136_021c3e00(MailParam *param);

#endif // BWPORT_FIELD_MAILTOOL_H
