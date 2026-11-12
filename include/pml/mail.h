// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/pml/mail.h: the prototypes of Black's named
// functions and the types they need.
#ifndef BWPORT_PML_MAIL_H
#define BWPORT_PML_MAIL_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "system/pms_data.h"

// Allocates a blank mail
MailData *CreateMailData(HeapID heapId);
void copyMailMsg(const MailData *src, MailData *dest);

// Black 2's; twinned code confirms the offsets of trainerId, trainerGender, region, gameVersion, unk07, trainerName,
// unk1E and message, and its size in Black
// Names from swan
struct MailData {
    u32 trainerId;
    u8 trainerGender;
    u8 region;
    u8 gameVersion;
    u8 unk07;
    u16 trainerName[8];
    u16 unk18[3];
    // 0xffff when there is none
    u16 unk1E;
    PMSData message[3];
};

#define MAIL_COUNT 20
#define MAIL_DESIGN_COUNT 12

// Black 2's, not checked against Black
#define MAIL_DESIGN_MAX 11

// Whether the mail is filled in, which is when its design is one of the MAIL_DESIGN_COUNT
BOOL func_02008ef8(const MailData *mail);
// Empties the mail, then sets its design and the author's info from the game data
void func_02008f48(MailData *mail, u32 design, u32 unused, GameData *gameData);
// Sets the mail's author's name
void func_02008f88(MailData *mail, const u16 *name);
void func_02008fa8(MailData *mail, u32 design);
void func_02008fc4(MailData *mail, u16 value);
PMSData *func_02008fc8(MailData *mail, u8 index);
void func_02008fd8(MailData *mail, const PMSData *src, u8 index);
// The save block of mail, and its size
void *func_02008fec(GameData *gameData);
u32 func_02008ffc(void);
// Empties all of the mails of a mailbox
void func_02009004(MailData *mails);
// The mail of the save's mailbox (box 0) or of a Pokémon: the free slot, and clearing, copying and reading a slot's
// mail
s32 func_02009020(MailData *mails, u32 box);
void func_0200903c(MailData *mails, u32 box, u32 index, const MailData *mail);
MailData *func_02009050(MailData *mails, u32 box, u32 index, HeapID heapId);
s32 func_02009074(MailData *mails, s32 count);
MailData *func_020090a0(MailData *mails, u32 box, s32 index);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Empties the mail
void ResetMailData(MailData *mail);
u32 func_02008f80(const MailData *mail);
u16 *func_02008f84(MailData *mail);
void func_0200902c(MailData *mails, u32 box, u32 index);
u8 func_02008f9c(const MailData *mail);
u16 func_02008fc0(const MailData *mail);
u8 func_02008fa4(const MailData *mail);
u8 func_02008fb0(const MailData *mail);
u8 func_02008fb8(const MailData *mail);

#endif // BWPORT_PML_MAIL_H
