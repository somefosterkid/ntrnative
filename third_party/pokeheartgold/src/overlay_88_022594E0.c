#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pm_string.h"
#include "error_handling.h"
#include "font.h"
#include "overlay_88.h"
#include "text.h"
#include "overlay_88_wifi_plaza_news.h"

void ov88_022595F8(const WiFiPlazaNewsNameEntry *entry, Window *window);

void ov88_022594E0(WiFiPlazaNewsNameList *nameList);

void ov88_02259508(WiFiPlazaNewsNameEntry *entry, WiFiPlazaNewsGraphics *graphics, s32 slot, enum HeapID heapID);
void ov88_0225954C(WiFiPlazaNewsNameEntry *entry);
void ov88_022592B8(const WiFiPlazaNewsNamePalette *namePalette, const NNSG2dPaletteData *plttData, u32 paletteSlot);

void ov88_022594E0(WiFiPlazaNewsNameList *nameList) {
    for (int i = 0; i < 3; i++) {
        if (nameList->entries[i].active == 1) {
            ov88_022595F8(&nameList->entries[i], &nameList->windows[i]);
        }
    }
}

void ov88_02259508(WiFiPlazaNewsNameEntry *entry, WiFiPlazaNewsGraphics *graphics, s32 slot, enum HeapID heapID) {
    entry->active = 0;
    entry->holdTimer = 0;
    entry->name = String_New(0x100U, heapID);
    entry->paletteSlot = (u8)(slot + 7);
    AddWindowParameterized(graphics->bgConfig, &entry->textWindow, 3U, 0U, 0U, 0xB4U, 2U, 7U, 0U);
}

void ov88_0225954C(WiFiPlazaNewsNameEntry *entry) {
    String_Delete(entry->name);
    RemoveWindow(&entry->textWindow);
}

void ov88_02259560(WiFiPlazaNewsNameEntry *entry, String *name, s32 holdDuration, const WiFiPlazaNewsNamePalette *palette, const NNSG2dPaletteData *plttData) {
    String_Copy(entry->name, name);
    entry->active = 1;
    entry->holdTimer = 0;
    entry->holdDuration = holdDuration;
    entry->scrollRange = (u16)FontID_String_GetWidth(FONT_MESSAGE, name, 0U);
    entry->scrollRange = (u16)(entry->scrollRange + 0x100);
    if ((u32)entry->scrollRange > 0x5A0U) {
        GF_AssertFail();
    }
    FillWindowPixelBuffer(&entry->textWindow, 0U);
    AddTextPrinterParameterized(&entry->textWindow, FONT_MESSAGE, entry->name, 0U, 0U, 0xFFU, NULL);
    entry->namePalette.paletteIndices[0] = palette->paletteIndices[0];
    entry->namePalette.paletteIndices[1] = palette->paletteIndices[1];
    entry->namePalette.paletteIndices[2] = palette->paletteIndices[2];
    entry->namePalette.paletteIndices[3] = palette->paletteIndices[3];
    ov88_022592B8(&entry->namePalette, plttData, entry->paletteSlot);
}

void ov88_022595DC(WiFiPlazaNewsNameEntry *entry) {
    entry->active = 0;
}

BOOL ov88_022595E4(WiFiPlazaNewsNameEntry *entry) {
    BOOL done;
    s32 next;

    done = 1;
    next = entry->holdTimer + 1;
    if (next <= (s32)entry->holdDuration) {
        entry->holdTimer = (u16)next;
        done = 0;
    }
    return done;
}

void ov88_022595F8(const WiFiPlazaNewsNameEntry *entry, Window *window) {
    s32 scrollOffset;
    s32 destX;
    s32 srcX;
    s32 blitWidth;

    scrollOffset = (entry->holdTimer * entry->scrollRange) / entry->holdDuration;

    if (scrollOffset < 256) {
        destX = (256 - 1) - scrollOffset;
        srcX = 0;
        blitWidth = scrollOffset;
    } else {
        destX = 0;
        srcX = scrollOffset - 256;
        blitWidth = (entry->scrollRange - scrollOffset);

        if (blitWidth > (256 - 1)) {
            blitWidth = 256 - 1;
        }
    }

    FillWindowPixelRect(window, 0, 0, 0, 256 - 1, 2 * 8);
    BlitBitmapRect(window, entry->textWindow.pixelBuffer, srcX, 0, 180 * 8, 2 * 8, destX, 0, blitWidth, 2 * 8, 15);
    ScheduleWindowCopyToVram(window);
}
