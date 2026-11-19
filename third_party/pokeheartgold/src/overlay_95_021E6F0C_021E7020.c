#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "overlay_95_egg_hatch_app.h"

void ov95_021E72B8(EggHatchPrompt *);

EggHatchPrompt *ov95_021E7020(BgConfig *arg0, SpriteSystem *arg1, SpriteManager *arg2, enum HeapID arg3);

EggHatchPrompt *ov95_021E7020(BgConfig *arg0, SpriteSystem *arg1, SpriteManager *arg2, enum HeapID arg3) {
    enum HeapID heapID;
    EggHatchPrompt *dest;

    heapID = arg3;
    if (arg0 == 0) {
        GF_AssertFail();
    }
    if (arg1 == 0) {
        GF_AssertFail();
    }
    if (arg2 == 0) {
        GF_AssertFail();
    }
    dest = Heap_Alloc(heapID, sizeof(EggHatchPrompt));
    MI_CpuFill8(dest, 0U, sizeof(EggHatchPrompt));
    dest->heapID = heapID;
    dest->bgConfig = arg0;
    dest->spriteSys = arg1;
    dest->spriteMan = arg2;
    dest->msgFormat = MessageFormat_New(heapID);
    FontID_Alloc(FONT_UNK_4, dest->heapID);
    ov95_021E72B8(dest);
    return dest;
}

void ov95_021E7308(EggHatchPrompt *);

void ov95_021E7404(EggHatchPrompt *);

void ov95_021E7078(EggHatchPrompt *arg0);

void ov95_021E7078(EggHatchPrompt *ptr) {
    if (ptr == NULL) {
        GF_AssertFail();
    }
    ov95_021E7308(ptr);
    ov95_021E7404(ptr);
    FontID_Release(FONT_UNK_4);
    MessageFormat_Delete(ptr->msgFormat);
    FreeBgTilemapBuffer(ptr->bgConfig, 4U);
    FreeBgTilemapBuffer(ptr->bgConfig, 5U);
    FreeBgTilemapBuffer(ptr->bgConfig, 6U);
    Heap_Free(ptr);
}
