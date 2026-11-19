#include "global.h"

#include "constants/heap.h"

#include "mail_message.h"
#include "overlay_34.h"
#include "player_data.h"
#include "pm_string.h"
#include "save_palpad.h"

void ov34_0225E348(Ov34Work *work, u32 id, MailMessage *mail, PlayerProfile *profile) {
    Ov34ChatLog *chatLog = work->chatLog;
    SavePalPad *palPad = work->unk4->palPad;
    int *slot;

    if (chatLog->count == 30) {
        slot = &chatLog->start;
    } else {
        slot = &chatLog->count;
    }

    if (chatLog->entries[*slot].sentence != NULL) {
        String_Delete(chatLog->entries[*slot].sentence);
    }

    if (chatLog->entries[*slot].friendName != NULL) {
        String_Delete(chatLog->entries[*slot].friendName);
    }

    CopyU16ArrayToString(chatLog->entries[*slot].name, PlayerProfile_GetNamePtr(profile));

    chatLog->entries[*slot].mail = *mail;
    chatLog->entries[*slot].id = id;
    chatLog->entries[*slot].gender = PlayerProfile_GetTrainerGender(profile);
    chatLog->entries[*slot].sentence = MailMsg_GetExpandedString(mail, HEAP_ID_87);
    chatLog->entries[*slot].friendName = ov34_0225E2BC(palPad, id, work->msgFormat, work->msgData, work->profile);

    (*slot)++;

    if (chatLog->start == 30) {
        chatLog->start = 0;
    }
}
