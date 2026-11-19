#include "global.h"
#include "mail.h"
#include "pokemon_types_def.h"
#include "bag.h"
#include "heap.h"
#include "party.h"
#include "party_menu.h"
#include "overlay_103_mail_box_work.h"

void ov103_021EDC58(MailBoxWork *arg0);

void ov103_021EDC68(MailBoxWork *arg0, s8 arg1, s16 arg2, s32 arg3);

void ov103_021EDC58(MailBoxWork *arg0) {
    Mailbox_DeleteSlotI(arg0->mailbox->msgs, 0, (s32) arg0->mailIndex);
}

void ov103_021EDC68(MailBoxWork *arg0, s8 arg1, s16 arg2, s32 arg3) {
    s32 v0;
    PartyMenuArgs *dest;

    v0 = arg3;
    dest = Heap_Alloc(HEAP_ID_156, sizeof(PartyMenuArgs));
    MI_CpuFill8(dest, 0U, sizeof(PartyMenuArgs));
    dest->party = SaveArray_Party_Get(arg0->args->saveData);
    dest->bag = Save_Bag_Get(arg0->args->saveData);
    dest->options = arg0->options;
    dest->mailbox = arg0->mailbox;
    dest->unk_25 = 0;
    dest->context = arg1;
    dest->itemId = arg2;
    dest->fieldSystem = arg0->args->fieldSystem;
    dest->menuInputStatePtr = arg0->args->menuInputStatePtr;
    dest->partySlot = (s8)v0;
    arg0->childParam = dest;
}
