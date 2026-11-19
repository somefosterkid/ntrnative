#include "global.h"

#include "heap.h"
#include "menu_input_state.h"
#include "options.h"
#include "overlay_55.h"
#include "overlay_56.h"
#include "overlay_manager.h"
#include "render_text.h"
#include "overlay_56_mail_viewer_app.h"

s32 ov56_021E5C20(OverlayManager *man, int *state) {
    UnkStruct_ov55_021E5B08 *args;
    u8 selectionIndex;
    MailViewerApp *mailViewerApp;

    Heap_Create(HEAP_ID_3, HEAP_ID_41, 0x20000U);
    mailViewerApp = OverlayManager_CreateAndGetData(man, sizeof(MailViewerApp), HEAP_ID_41);
    memset(mailViewerApp, 0, sizeof(MailViewerApp));
    mailViewerApp->heapID = HEAP_ID_41;
    args = OverlayManager_GetArgs(man);
    mailViewerApp->args = args;
    mailViewerApp->mode = (u8)args->result;
    mailViewerApp->initialMode = mailViewerApp->mode;
    mailViewerApp->verticalSelectionIndex = mailViewerApp->args->mailMessageIdx;
    mailViewerApp->horizontalSelectionIndex = mailViewerApp->args->horizontalSelectionIndex;
    selectionIndex = mailViewerApp->verticalSelectionIndex;
    mailViewerApp->blendIndex = selectionIndex;
    mailViewerApp->prevBlendIndex = selectionIndex;
    mailViewerApp->args->result = 0xFFFF;
    mailViewerApp->textFrameDelay = Options_GetTextFrameDelay(mailViewerApp->args->options);
    mailViewerApp->frame = (s8)Options_GetFrame(mailViewerApp->args->options);
    TextFlags_SetCanABSpeedUpPrint(1);
    mailViewerApp->unkA = (s8)MenuInputStateMgr_GetState(mailViewerApp->args->menuInputStateMgr);
    return 1;
}

s32 ov56_021E6228(MailViewerApp *mailViewerApp);

s32 ov56_021E5C9C(OverlayManager *man, int *state) {
    if (ov56_021E6228(OverlayManager_GetData(man)) != 0) {
        return 1;
    }
    return 0;
}

s32 ov56_021E5CB4(OverlayManager *man, int *state) {
    enum HeapID heapID;
    MailViewerApp *mailViewerApp;

    mailViewerApp = OverlayManager_GetData(man);
    TextFlags_SetCanABSpeedUpPrint(0);
    heapID = mailViewerApp->heapID;
    MenuInputStateMgr_SetState(mailViewerApp->args->menuInputStateMgr, (enum MenuInputState)mailViewerApp->unkA);
    OverlayManager_FreeData(man);
    Heap_Destroy(heapID);
    return 1;
}
