#include "global.h"

#include "constants/heap.h"

#include "overlay_manager.h"
#include "party_menu.h"
#include "heap.h"
#include "mail_misc.h"
#include "party.h"
#include "overlay_103_mail_box_work.h"

s32 ov103_021EDCC8(MailBoxWork *arg0);

s32 ov103_021EDCE0(MailBoxWork *arg0);

s32 ov103_021EDCC8(MailBoxWork *arg0) {
    arg0->childMan = OverlayManager_New(&gOverlayTemplate_PartyMenu, arg0->childParam, HEAP_ID_156);
    return 0;
}

s32 ov103_021EDCE0(MailBoxWork *arg0) {
    PartyMenuArgs *args;
    u8 slot;

    args = arg0->childParam;
    slot = args->partySlot;
    if ((u32)slot <= 5U) {
        sub_02091004(arg0->mailbox->msgs, (s32) arg0->mailIndex, Party_GetMonByIndex(SaveArray_Party_Get(arg0->args->saveData), (s32)slot), HEAP_ID_156);
    }
    Heap_Free(arg0->childParam);
    return 8;
}
