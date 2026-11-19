#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_74_app_templates.h"
#include "overlay_74_wonder_cards_app2_data.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "sys_task.h"
#include "unk_0200A090.h"

const void *sub_02014A60(const BufferManager *bufferManager);
void sub_02014AA0(void);
void sub_02014AB0(const void *src, void *dst, int size, enum BufferManagerDMAType dmaType);

s32 ov74_022352A0(s32);

void ov74_0222F600(SysTask *task, void *param);

BOOL ov74_0222F598(OverlayManager *man, int *state) {
    WonderCardsApp2Data *appData;

    Heap_Create(HEAP_ID_3, HEAP_ID_85, 0x30000U);
    appData = OverlayManager_CreateAndGetData(man, sizeof(WonderCardsApp2Data), HEAP_ID_85);
    memset(appData, 0, sizeof(WonderCardsApp2Data));
    appData->bgConfig = BgConfig_Alloc(HEAP_ID_85);
    appData->heapID = HEAP_ID_85;
    sub_0200FBF4(PM_LCD_TOP, 0U);
    sub_0200FBF4(PM_LCD_BOTTOM, 0U);
    appData->unk_2BF4 = 1;
    ov74_022352A0(HEAP_ID_85);
    Heap_Create(HEAP_ID_DEFAULT, HEAP_ID_89, 0x570U);
    return TRUE;
}

void ov74_0222F600(SysTask *task, void *param) {
    WonderCardFlipAnimManager *animMan = param;

    sub_02014AA0();
    sub_02014AB0(sub_02014A60(animMan->bufferManager), (void *)REG_BG0OFS_ADDR, sizeof(u16) * 4, BUFFER_MANAGER_DMA_TYPE_32BIT);
}
