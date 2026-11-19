#include "global.h"

#include "mail_message.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "save_frontier.h"
#include "unk_0202D230.h"
#include "unk_02030A98.h"

void sub_0202D640(SaveData *saveData, int a1, MailMessage *mailMessage) {
    FrontierSave *pFVar1;

    pFVar1 = Save_Frontier_GetStatic(saveData);
    MailMsg_Copy(&pFVar1->unk_950.easyChatMessages.sentences[a1], mailMessage);
    return;
}

MailMessage *sub_0202D660(SaveData *saveData, u32 a1) {
    FrontierSave *pFVar1;

    pFVar1 = Save_Frontier_GetStatic(saveData);
    return &pFVar1->unk_950.easyChatMessages.sentences[a1];
}
