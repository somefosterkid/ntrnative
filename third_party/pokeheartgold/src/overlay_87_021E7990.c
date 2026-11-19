#include "global.h"
struct ScratchOffCardApp;

#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "error_handling.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_87_scratch_off_card_app.h"
#include "message_printer.h"
#include "msgdata.h"
#include "yes_no_prompt.h"

struct OverlayManager;
struct MessagePrinter;
struct MsgData;
struct PaletteAnimator;
struct ScratchOffCardsAppSprite;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct YesNoPrompt;
typedef struct PaletteAnimator PaletteAnimator;
typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);
struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};
void ov87_021E7FD4(ScratchOffCardsAppSprite *appSprite, int draw);

void ov87_021E7990(ScratchOffCardApp *app);
void ov87_021E7998(ScratchOffCardApp *app);
void ov87_021E79A0(ScratchOffCardApp *app);

void ov87_021E79C4(ScratchOffCardApp *app);
void ov87_021E7A04(struct ScratchOffCardApp *arg0);
void ov87_021E7A2C(struct ScratchOffCardApp *arg0);

void ov87_021E7990(ScratchOffCardApp *app) {
    app->currentCard++;
}

void ov87_021E7998(ScratchOffCardApp *app) {
    app->currentCard++;
}

void ov87_021E79A0(ScratchOffCardApp *app) {
    ov87_021E7FD4(app->centerMessageBoxSprite, 0);
    FillWindowPixelBuffer(&app->windows[SCRATCH_WINDOW_START_CARD], 0);
    ScheduleWindowCopyToVram(&app->windows[SCRATCH_WINDOW_START_CARD]);
}

void ov87_021E79C4(ScratchOffCardApp *app) {
    NNSG2dCharacterData *charData;

    app->scratchCellsRawCharData = GfGfxLoader_GetCharData(NARC_a_2_0_5, 7, FALSE, &app->scratchCellsCharData, HEAP_ID_122);
    charData = app->scratchCellsCharData;
    BG_LoadCharTilesData(app->bgConfig, 0U, charData->pRawData, charData->szByte, 0U);
    BgCommitTilemapBufferToVram(app->bgConfig, 0U);
}

void ov87_021E7A04(ScratchOffCardApp *arg0) {
    void *v0;

    arg0->scratchCellsDataBackup = Heap_Alloc(HEAP_ID_122, arg0->scratchCellsCharData->szByte);
    v0 = arg0->scratchCellsDataBackup;
    if ((v0 == NULL) && (v0 == NULL)) {
        GF_AssertFail();
    }
}

void ov87_021E7A2C(ScratchOffCardApp *arg0) {
    NNSG2dCharacterData *v0;

    v0 = arg0->scratchCellsCharData;
    memcpy(arg0->scratchCellsDataBackup, v0->pRawData, v0->szByte);
}
