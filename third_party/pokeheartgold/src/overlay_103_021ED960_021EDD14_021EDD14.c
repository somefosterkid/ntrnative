#include "global.h"

#include "heap.h"
#include "overlay_103_mail_box_work.h"
#include "party_menu.h"

u32 ov103_021EDD14(MailBoxWork *param_1);

u32 ov103_021EDD14(MailBoxWork *param_1) {
    PartyMenuArgs *p = param_1->childParam;

    param_1->partyPos = p->partySlot;
    param_1->partyResult = p->selectedAction;
    Heap_Free(param_1->childParam);
    return 0;
}
