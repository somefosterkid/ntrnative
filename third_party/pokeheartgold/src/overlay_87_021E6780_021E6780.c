#include "global.h"

#include "filesystem.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "unk_020163E0.h"

#include "overlay_87_scratch_off_card_app.h"

s32 ov87_021E6BB8(BgConfig *);

s32 ov87_021E7E98(void *);

void *ov87_021E7FC0(struct ScratchOffCardsAppSprite *);

s32 ov87_021E80F0(void *);

void ov87_021E6780(ScratchOffCardApp *app);

void ov87_021E6780(ScratchOffCardApp *app) {
    int i;

    for (i = 0; i < 4; i++) {
        if (app->topScreenMonSprites[i] != NULL) {
            ov87_021E7FC0(app->topScreenMonSprites[i]);
        }
    }
    for (i = 0; i < 9; i++) {
        if (app->scratchMonSprites[i] != NULL) {
            ov87_021E7FC0(app->scratchMonSprites[i]);
        }
    }
    if (app->skipCardButtonSprite != NULL) {
        ov87_021E7FC0(app->skipCardButtonSprite);
    }
    if (app->centerMessageBoxSprite != NULL) {
        ov87_021E7FC0(app->centerMessageBoxSprite);
    }
    for (i = 0; i < 3; i++) {
        if (app->matchingCellBoxSprites[i] != NULL) {
            ov87_021E7FC0(app->matchingCellBoxSprites[i]);
        }
    }
    for (i = 0; i < 3; i++) {
        if (app->winMonSprites[i] != NULL) {
            ov87_021E7FC0(app->winMonSprites[i]);
        }
    }
    if (app->winPopupSprite != NULL) {
        ov87_021E7FC0(app->winPopupSprite);
    }
    FontID_Release(FONT_SUBSCREEN);
    PaletteData_FreeBuffers(app->plttData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->plttData, PLTTBUF_MAIN_BG);
    PaletteData_Free(app->plttData);
    app->plttData = NULL;
    ov87_021E7E98(&app->spriteMan);
    sub_020164C4(app->paletteAnimator);
    DestroyMsgData(app->msgLoader);
    MessageFormat_Delete(app->strTemplate);
    String_Delete(app->displayStr);
    String_Delete(app->fmtStr);
    MessagePrinter_Delete(app->specialChars);
    Heap_Free(app->scratchCellsRawCharData);
    Heap_Free(app->scratchCellsDataBackup);
    ov87_021E80F0(app->windows);
    ov87_021E6BB8(app->bgConfig);
    NARC_Delete(app->narc);
}
