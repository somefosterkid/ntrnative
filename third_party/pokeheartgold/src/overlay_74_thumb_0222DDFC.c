#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "screen_fade.h"
#include "seal_case.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sys_task.h"
#include "unk_0200A090.h"

s32 ov74_022352A0(s32);
s32 ov74_0222DDFC(OverlayManager *arg0);
const void *sub_02014A60(const BufferManager *bufferManager);
void sub_02014AA0(void);
void sub_02014AB0(const void *src, void *dst, int size, enum BufferManagerDMAType dmaType);
void ov74_0222DE68(SysTask *task, void *param);
s32 ov74_0222DDFC(OverlayManager *man) {
    WonderCardsAppData *appData;

    Heap_Create(HEAP_ID_3, HEAP_ID_85, 0x30000U);
    appData = OverlayManager_CreateAndGetData(man, sizeof(WonderCardsAppData), HEAP_ID_85);
    memset(appData, 0, sizeof(WonderCardsAppData));
    appData->bgConfig = BgConfig_Alloc(HEAP_ID_85);
    appData->heapID = HEAP_ID_85;
    sub_0200FBF4(PM_LCD_TOP, 0U);
    sub_0200FBF4(PM_LCD_BOTTOM, 0U);
    appData->selectedWondercardSlot = 2;
    appData->numPlayerConnections = 1;
    ov74_022352A0(0x55);
    Heap_Create(HEAP_ID_DEFAULT, HEAP_ID_89, 0x570U);
    return 1;
}
void ov74_0222DE68(SysTask *task, void *param) {
    WonderCardFlipAnimManager *animMan = param;
    sub_02014AA0();
    sub_02014AB0(sub_02014A60(animMan->bufferManager), (void *)REG_BG0OFS_ADDR, sizeof(u16) * 4, BUFFER_MANAGER_DMA_TYPE_32BIT);
}
