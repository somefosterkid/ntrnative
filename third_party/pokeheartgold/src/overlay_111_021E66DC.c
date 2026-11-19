#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "error_handling.h"
#include "message_printer.h"
#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "text.h"
#include "pm_string.h"
#include "overlay_111.h"
#include "pokemon.h"

SwapMonPanels *ov111_021E66DC(enum HeapID heapID, BgConfig *bgConfig, SwapMonText *text, SpriteSystem *spriteSystem, SpriteManager *spriteManager, s8 iconPalette) {
    SwapMonPanels *dest;

    dest = Heap_Alloc(heapID, sizeof(SwapMonPanels));
    MI_CpuFill8(dest, 0, sizeof(SwapMonPanels));
    dest->bgConfig = bgConfig;
    dest->heapID = heapID;
    dest->text = text;
    dest->spriteSystem = spriteSystem;
    dest->spriteManager = spriteManager;
    dest->iconPalette = iconPalette;
    return dest;
}

void ov111_021E6710(SwapMonPanels *ptr) {
    s32 v0;

    if (ptr == NULL) {
        GF_AssertFail();
    }
    v0 = 0;
    do {
        if (ptr->panels[v0] != NULL) {
            ov111_021E6694(ptr->panels[v0]);
        }
        v0 += 1;
    } while (v0 < 2);
    Heap_Free(ptr);
}

void ov111_021E6738(SwapMonPanels *arg0, s32 arg1, u8 arg2, u8 arg3, Pokemon *mon) {
    SwapMonPanel *v0;

    v0 = ov111_021E63D0(arg0, arg2, arg3, arg1, mon);
    ov111_021E6268(v0->sprites[1], v0->mon, arg0->iconPalette, arg0->heapID);
    ov111_021E64C8(v0, arg0->text);
    arg0->panels[arg1] = v0;
}

void ov111_021E6770(SwapMonPanels *arg0, s32 arg1, s32 arg2, s32 arg3) {
    ov111_021E65CC(arg0->panels[arg1], arg2, arg3);
}

void ov111_021E6784(SwapMonPanels *arg0) {
    s32 v0;

    v0 = 0;
    do {
        if (ov111_021E6684(arg0->panels[v0]) == 0) {
            ov111_021E65D4(arg0->panels[v0]);
        }
        v0 += 1;
    } while (v0 < 2);
}

s32 ov111_021E67A4(SwapMonPanels *arg0) {
    s32 v0;

    v0 = 0;
loop_1:
    if (ov111_021E6684(arg0->panels[v0]) == 0) {
        return 0;
    }
    v0 += 1;
    if (v0 >= 2) {
        return 1;
    }
    goto loop_1;
}

SwapMonText *ov111_021E67C4(enum HeapID heapID) {
    SwapMonText *dest;

    dest = Heap_Alloc(heapID, sizeof(SwapMonText));
    MI_CpuFill8(dest, 0U, sizeof(SwapMonText));
    dest->heapID = heapID;
    dest->msgPrinter = MessagePrinter_New(1U, 2U, 0U, heapID);
    return dest;
}

void ov111_021E67EC(SwapMonText *text, BgConfig *bgConfig, s32 bgLayer, Options *options) {
    text->bgConfig = bgConfig;
    text->printerId = -1;
    text->bgLayer = bgLayer;
    text->options = options;
    text->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1D, text->heapID);
    text->msgFormat = MessageFormat_New(text->heapID);
    ResetAllTextPrinters();
    ov111_021E6A44(text->bgConfig, (u8)text->bgLayer, text->heapID);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, text->heapID);
    BG_FillCharDataRange(text->bgConfig, (enum GFBgLayer)text->bgLayer, 0U, 1U, 0U);
}

void ov111_021E685C(SwapMonText *text) {
    String *string;

    ov111_021E69F4(text);
    DestroyMsgData(text->msgData);
    MessageFormat_Delete(text->msgFormat);
    string = text->string;
    if (string != NULL) {
        String_Delete(string);
    }
    MessagePrinter_Delete(text->msgPrinter);
    Heap_Free(text);
}
