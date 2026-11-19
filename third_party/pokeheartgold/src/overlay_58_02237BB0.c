#include "global.h"
#include "constants/heap.h"
#include "bag.h"
#include "bag_types_def.h"
#include "heap.h"
#include "mail.h"
#include "options.h"
#include "overlay_58.h"
#include "overlay_manager.h"
#include "party.h"
#include "party_menu.h"
#include "player_data.h"
#include "pokemon_types_def.h"
#include "save.h"

BOOL ov58_022378C0(OverlayManager **appManPtr);

u32 ov58_02237BB0(Ov58ApricornBoxWork *param_1);

s32 ov58_02237BD4(Ov58ApricornBoxWork *arg0);

u32 ov58_02237BB0(Ov58ApricornBoxWork *param_1) {
    s32 iVar1;

    iVar1 = ov58_022378C0(&param_1->childMan);
    if (iVar1 == 0) {
        return 2;
    }
    if (param_1->unk_18 == 3) {
        param_1->unk_1C = 0;
        return 3;
    }
    return 7;
}

s32 ov58_02237BD4(Ov58ApricornBoxWork *arg0) {
    PartyMenuArgs *v0;

    v0 = Heap_Alloc(arg0->heapId, sizeof(PartyMenuArgs));
    MI_CpuFill8(v0, 0U, sizeof(PartyMenuArgs));
    v0->party = SaveArray_Party_Get(arg0->args->saveData);
    v0->bag = Save_Bag_Get(arg0->args->saveData);
    v0->unk_25 = 0;
    v0->context = 0x14;
    v0->options = Save_PlayerData_GetOptionsAddr(arg0->args->saveData);
    v0->mailbox = Save_Mailbox_Get(arg0->args->saveData);
    v0->fieldSystem = NULL;
    v0->menuInputStatePtr = arg0->args->menuInputStatePtr;
    v0->partySlot = (u8)arg0->unk_1C;
    arg0->childMan = OverlayManager_New(&gOverlayTemplate_PartyMenu, v0, arg0->heapId);
    arg0->partyMenuArgs = v0;
    return 4;
}
