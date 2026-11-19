#include "global.h"

#include "mail_message.h"
#include "overlay_34.h"

s32 ov34_0225E428(Ov34Work *work, MailMessage *mail, s32 id) {
    MailMessage *a;
    s32 count;
    s32 i;
    Ov34ChatLog *chatLog;
    Ov34ChatEntry *entry;

    a = mail;
    if (MailMsg_IsInit(mail) == 0) {
        return 0;
    }
    for (i = 0; i < work->chatLog->count; i++) {
        if ((id == work->chatLog->entries[i].id) && (MailMsg_Compare(a, &work->chatLog->entries[i].mail) != 0)) {
            break;
        }
    }
    chatLog = work->chatLog;
    count = chatLog->count;
    if ((i != count) && (count != 0)) {
        return 0;
    }
    entry = &chatLog->entries[i];
    if (id != entry->id) {
        MailMsg_Compare(a, &entry->mail);
    }
    return 1;
}

void ov34_0225E4A8(Ov34Work *work, PlayerProfile *profile, MailMessage *mail, s32 id) {
    s32 atEnd;

    atEnd = 0;
    if (work->scroll == (work->count - 3)) {
        atEnd = 1;
    }
    ov34_0225E348(work, id, mail, profile);
    if ((work->scrollbarVisible != 0) && (atEnd != 0)) {
        work->scroll = (u16)(work->chatLog->count - 3);
    }
    ov34_0225E560(work);
}
