// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/mail.c, ported by tools/port_file.py.
#include "types.h"
#include "pml/mail.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/pms_data.h"
#include "system/version.h"
#include "nitro/mi.h"

// The mail: a mailbox of MAIL_COUNT mails, and the mail of one Pokémon. The ROM's string for the file is "mail.c".

#define SAVE_BLOCK_MAIL 0x28

void ResetMailData(MailData *mail) {
    int i;

    mail->trainerId = 0;
    mail->trainerGender = 0;
    mail->region = region;
    mail->gameVersion = game_version;
    mail->unk07 = 0xff;
    MIi_CpuClear16(GFL_StrBufGetTerminator(), mail->trainerName, sizeof(mail->trainerName));
    mail->unk1E = 0xffff;
    for (i = 0; i < 3; i++) {
        func_0202428c(&mail->message[i]);
    }
}

BOOL func_02008ef8(const MailData *mail) {
    if (mail->unk07 > MAIL_DESIGN_MAX) {
        return FALSE;
    }
    return TRUE;
}

MailData *CreateMailData(HeapID heapId) {
    MailData *mail = GFL_HeapAllocate(HEAPID_TAIL(heapId), sizeof(MailData), FALSE, "mail.c", 135);
    ResetMailData(mail);
    return mail;
}

void copyMailMsg(const MailData *src, MailData *dest) {
    MI_CpuCopy8(src, dest, sizeof(MailData));
}

void func_02008f48(MailData *mail, u32 design, u32 unused, GameData *gameData) {
    PlayerInfo *info;

    ResetMailData(mail);
    mail->unk07 = design;
    info = GetGameDataPlayerInfo(gameData);
    MI_CpuCopy8(GetPlayerName(info), mail->trainerName, sizeof(mail->trainerName));
    mail->trainerGender = getTrainerGender(info);
    mail->trainerId = getIDAsUInt(info);
}

u32 func_02008f80(const MailData *mail) {
    return mail->trainerId;
}

u16 *func_02008f84(MailData *mail) {
    return mail->trainerName;
}

void func_02008f88(MailData *mail, const u16 *name) {
    MI_CpuCopy8(name, mail->trainerName, sizeof(mail->trainerName));
}

u8 func_02008f9c(const MailData *mail) {
    return mail->trainerGender;
}

void func_02008fa0(MailData *mail, u8 gender) {
    mail->trainerGender = gender;
}

u8 func_02008fa4(const MailData *mail) {
    return mail->unk07;
}




void func_02008fa8(MailData *mail, u32 design) {
    if (design < MAIL_DESIGN_COUNT) {
        mail->unk07 = design;
    }
}

u8 func_02008fb0(const MailData *mail) {
    return mail->region;
}

void func_02008fb4(MailData *mail, u8 region) {
    mail->region = region;
}

u8 func_02008fb8(const MailData *mail) {
    return mail->gameVersion;
}

void func_02008fbc(MailData *mail, u8 gameVersion) {
    mail->gameVersion = gameVersion;
}

u16 func_02008fc0(const MailData *mail) {
    return mail->unk1E;
}






void func_02008fc4(MailData *mail, u16 value) {
    mail->unk1E = value;
}

PMSData *func_02008fc8(MailData *mail, u8 index) {
    if (index < 3) {
        return &mail->message[index];
    }
    return &mail->message[0];
}

void func_02008fd8(MailData *mail, const PMSData *src, u8 index) {
    if (index < 3) {
        func_02024570(&mail->message[index], src);
    }
}

void *func_02008fec(GameData *gameData) {
    return SaveControl_GetBlockPtr(GameData_GetSaveControl(gameData), SAVE_BLOCK_MAIL);
}

u32 func_02008ffc(void) {
    return MAIL_COUNT * sizeof(MailData);
}

void func_02009004(MailData *mails) {
    int i;

    for (i = 0; i < MAIL_COUNT; i++) {
        ResetMailData(&mails[i]);
    }
}

s32 func_02009020(MailData *mails, u32 box) {
    return func_02009074(mails, MAIL_COUNT);
}

void func_0200902c(MailData *mails, u32 box, u32 index) {
    MailData *mail = func_020090a0(mails, box, index);

    if (mail != NULL) {
        ResetMailData(mail);
    }
}

void func_0200903c(MailData *mails, u32 box, u32 index, const MailData *src) {
    MailData *mail = func_020090a0(mails, box, index);

    if (mail != NULL) {
        copyMailMsg(src, mail);
    }
}

MailData *func_02009050(MailData *mails, u32 box, u32 index, HeapID heapId) {
    MailData *mail = func_020090a0(mails, box, index);
    MailData *copy = CreateMailData(heapId);

    if (mail != NULL) {
        copyMailMsg(mail, copy);
    }
    return copy;
}

s32 func_02009074(MailData *mails, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        if (!func_02008ef8(&mails[i])) {
            return i;
        }
    }
    return -1;
}

MailData *func_020090a0(MailData *mails, u32 box, s32 index) {
    if (index < MAIL_COUNT) {
        return &mails[index];
    }
    return NULL;
}
