#include "global.h"

#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "system.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221CCB0(void *);

s32 ov65_0221CD0C(void *);

s32 ov65_0221D1C8(void *);

s32 ov65_0221FAE0(void *);

s32 WirelessTradeSelectMon_Exit(OverlayManager *man, s32 *state);

s32 WirelessTradeSelectMon_Exit(OverlayManager *man, s32 *state) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    *(s32 *)((u8 *)(OverlayManager_GetArgs(man)) + 0x24) = (s32) *((s32 *)&((TradeRoom *)v0)->didConfirmTrade);
    ov65_0221CD0C(v0);
    String_Delete(((TradeRoom *)v0)->itemLabelStr);
    Heap_Free(((void *)((TradeRoom *)v0)->partnerParty));
    ov65_0221CCB0(v0);
    ov65_0221FAE0(v0 + 0x444);
    ov65_0221D1C8(((void *)((TradeRoom *)v0)->bgConfig));
    Heap_FreeExplicit(HEAP_ID_26, ((void *)((TradeRoom *)v0)->bgConfig));
    DestroyMsgData(((MsgData *)((TradeRoom *)v0)->msgLoader));
    MessageFormat_Delete(((MessageFormat *)((TradeRoom *)v0)->unread_180));
    MessageFormat_Delete(((MessageFormat *)((TradeRoom *)v0)->strTemplate2));
    MessageFormat_Delete(((MessageFormat *)((TradeRoom *)v0)->strTemplate));
    String_Delete(((TradeRoom *)v0)->playerNameStr);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    FontID_Release(FONT_UNK_4);
    Heap_Destroy(HEAP_ID_26);
    return 1;
}
