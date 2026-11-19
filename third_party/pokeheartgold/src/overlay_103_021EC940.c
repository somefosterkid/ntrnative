#include "global.h"

#include "heap.h"
#include "mail.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "overlay_103_mail_box_work.h"

BOOL ov103_021EC940(OverlayManager *man, int *state) {
    UnkStruct_0203F074 *v0;
    MailBoxWork *dest;

    Heap_Create(HEAP_ID_3, HEAP_ID_156, 0x10000U);
    dest = OverlayManager_CreateAndGetData(man, sizeof(MailBoxWork), HEAP_ID_156);
    MI_CpuFill8(dest, 0U, sizeof(MailBoxWork));
    v0 = OverlayManager_GetArgs(man);
    dest->args = v0;
    dest->options = Save_PlayerData_GetOptionsAddr(v0->saveData);
    dest->mailbox = Save_Mailbox_Get(dest->args->saveData);
    dest->nextState = 8;
    return 1;
}

s32 ov103_021ED2D4(void *, s32 *);

BOOL ov103_021EC988(OverlayManager *man, int *state) {
    if (ov103_021ED2D4(OverlayManager_GetData(man), state) == 0) {
        return 1;
    }
    return 0;
}

BOOL ov103_021EC9A4(OverlayManager *man, int *state) {
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_156);
    return 1;
}
