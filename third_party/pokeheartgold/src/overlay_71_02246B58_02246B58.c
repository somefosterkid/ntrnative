#include "global.h"

#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
#include "overlay_71_trade_sequence_data.h"

s32 TradeSequence_Exit(OverlayManager *man, s32 *state);

s32 TradeSequence_Exit(OverlayManager *man, s32 *state) {
    u32 v1;
    TradeSequenceData *v0;

    v1 = OS_DisableInterrupts();
    v0 = OverlayManager_GetData(man);
    SysTask_Destroy(v0->task);
    sub_0203A914();
    MessageFormat_Delete(v0->strTemplate);
    DestroyMsgData(v0->msgLoader);
    String_Delete(v0->strBuffer);
    Heap_Free(v0->bgConfig);
    SpriteList_Delete(v0->spriteList);
    OamManager_Free();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_56);
    Heap_Destroy(HEAP_ID_57);
    OS_RestoreInterrupts(v1);
    return 1;
}
