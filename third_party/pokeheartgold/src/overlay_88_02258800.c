#include "global.h"

#include "heap.h"
#include "overlay_88.h"
#include "overlay_88_wifi_plaza_news.h"
#include "overlay_manager.h"
#include "system.h"

void ov88_02258A64(void *);

void ov88_02258ABC(WiFiPlazaNewsGraphics *graphics, enum HeapID heapID);

void ov88_02258D64(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, enum HeapID heapID);

void ov88_02258EA8(WiFiPlazaNewsCountdown *countdown, WiFiPlazaNewsGraphics *graphics, WiFiPlazaNewsDigits *digits, struct UnkStruct_ov66_0223177C_t *playerList, const UnkStruct_ov66_02231428 *closeTime, struct UnkStruct_ov66_0222DFF8_t *plazaSystem, enum HeapID heapID);

void ov88_022590C0(WiFiPlazaNewsAnimState *animState, WiFiPlazaNewsGraphics *graphics, WiFiPlazaNewsDigits *digits, struct UnkStruct_ov66_0223177C_t *playerList, struct UnkStruct_ov66_0222DFF8_t *plazaSystem, enum HeapID heapID);

void ov88_022592EC(WiFiPlazaNewsNameList *nameList, WiFiPlazaNewsGraphics *graphics, enum HeapID heapID);

void ov88_0225967C(WiFiPlazaNewsCaptions *captions, WiFiPlazaNewsGraphics *graphics, enum HeapID heapID);

s32 ov88_02258800(OverlayManager *man, int *state) {
    WiFiPlazaNewsData *data;
    WiFiPlazaApplicationArgs *args;

    args = OverlayManager_GetArgs(man);
    Heap_Create(HEAP_ID_3, HEAP_ID_114, 0x50000U);
    data = OverlayManager_CreateAndGetData(man, sizeof(WiFiPlazaNewsData), HEAP_ID_114);
    memset(data, 0, sizeof(WiFiPlazaNewsData));
    data->playerList = args->playerList;
    data->closeTime = args->closeTime;
    ov88_02258ABC(&data->graphics, HEAP_ID_114);
    ov88_02258D64(&data->digits, &data->graphics, HEAP_ID_114);
    ov88_02258EA8(&data->countdown, &data->graphics, &data->digits, data->playerList, data->closeTime, args->plazaSystem, HEAP_ID_114);
    ov88_022590C0(&data->animState, &data->graphics, &data->digits, data->playerList, args->plazaSystem, HEAP_ID_114);
    ov88_022592EC(&data->nameList, &data->graphics, HEAP_ID_114);
    ov88_0225967C(&data->captions, &data->graphics, HEAP_ID_114);
    Main_SetVBlankIntrCB(ov88_02258A64, data);
    HBlankInterruptDisable();
    return 1;
}
