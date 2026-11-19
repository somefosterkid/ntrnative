#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "pm_string.h"
#include "heap.h"
#include "overlay_88_wifi_plaza_news.h"

void ov88_02259508(WiFiPlazaNewsNameEntry *entry, WiFiPlazaNewsGraphics *graphics, s32 slot, enum HeapID heapID);

extern u32 _02259904;

void ov88_022592EC(WiFiPlazaNewsNameList *nameList, WiFiPlazaNewsGraphics *graphics, enum HeapID heapID);

void ov88_0225954C(WiFiPlazaNewsNameEntry *entry);
void ov88_022593D0(WiFiPlazaNewsNameList *nameList);

void ov88_022592EC(WiFiPlazaNewsNameList *nameList, WiFiPlazaNewsGraphics *graphics, enum HeapID heapID) {
    for (int i = 0; i < 3; i++) {
        ov88_02259508(&nameList->entries[i], graphics, i, heapID);
        AddWindowParameterized(graphics->bgConfig, &nameList->windows[i], 3, 1, ((u8 *)&_02259904)[i], 0x1e, 2, i + 7, 0x300 + i * 0x3c);
        FillWindowPixelBuffer(&nameList->windows[i], 0);
        ScheduleWindowCopyToVram(&nameList->windows[i]);
    }
    nameList->pendingName = String_New(0x100, heapID);
    nameList->paletteBuffer = GfGfxLoader_GetPlttDataFromOpenNarc(graphics->narc, 0, &nameList->paletteData, heapID);
    BG_LoadPlttData(0, (u8 *)nameList->paletteData->pRawData + 0xee, 4, 0xfa);
    BG_LoadPlttData(0, (u8 *)nameList->paletteData->pRawData + 0xee, 4, 0x11a);
    BG_LoadPlttData(0, (u8 *)nameList->paletteData->pRawData + 0xee, 4, 0x13a);
}

void ov88_022593D0(WiFiPlazaNewsNameList *nameList) {
    Heap_Free(nameList->paletteBuffer);
    String_Delete(nameList->pendingName);

    for (int i = 0; i < 3; i++) {
        RemoveWindow(&nameList->windows[i]);
        ov88_0225954C(&nameList->entries[i]);
    }
}
