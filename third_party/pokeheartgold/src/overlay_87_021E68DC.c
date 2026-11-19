#include "global.h"

#include "filesystem.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "pm_string.h"
#include "system.h"
#include "unk_020163E0.h"
#include "unk_020210A0.h"
#include "yes_no_prompt.h"
#include "overlay_87_scratch_off_card_app.h"

void ov87_021E6704(ScratchOffCardApp *app);
void ov87_021E6B38(ScratchOffCardApp *app);
void ov87_021E6BA8(ScratchOffCardApp *app);
void ov87_021E6C04(void *arg);
void ov87_021E79C4(ScratchOffCardApp *app);
void ov87_021E7A04(ScratchOffCardApp *app);
void ov87_021E7A2C(ScratchOffCardApp *app);
ScratchOffCardsAppSprite *ov87_021E7F6C(ScratchOffCardsAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u16 x, u16 y, u8 onSubScreen, u32 resourcePriority, u32 priority);
void ov87_021E7FD4(ScratchOffCardsAppSprite *appSprite, BOOL draw);
void ov87_021E7FE0(ScratchOffCardsAppSprite *appSprite, u32 priority);
void ov87_021E7FEC(ScratchOffCardsAppSprite *appSprite, int x, int y);
void ov87_021E80C0(BgConfig *bgConfig, Window *windows);

extern const CoordinatesS16 ov87_021E81A0[4];
extern const CoordinatesS16 ov87_021E82E4[9];

void ov87_021E68DC(ScratchOffCardApp *app);
void ov87_021E6AE0(ScratchOffCardApp *app);

void ov87_021E68DC(ScratchOffCardApp *app) {
    s32 i;
    u32 y;

    app->narc = NARC_New(NARC_a_2_0_5, HEAP_ID_122);
    ov87_021E6B38(app);
    ov87_021E6BA8(app);
    app->msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1B0, HEAP_ID_122);
    app->strTemplate = MessageFormat_New(HEAP_ID_122);
    app->displayStr = String_New(0x258U, HEAP_ID_122);
    app->fmtStr = String_New(0x258U, HEAP_ID_122);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_122);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_122);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_122);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_122);
    FontID_Alloc(FONT_SUBSCREEN, HEAP_ID_122);
    app->specialChars = MessagePrinter_New(0xFU, 0xEU, 0U, HEAP_ID_122);
    ov87_021E80C0(app->bgConfig, app->windows);
    app->paletteAnimator = sub_020163E0(NULL, PM_LCD_BOTTOM, 0xCU, HEAP_ID_122);
    ov87_021E79C4(app);
    ov87_021E7A04(app);
    ov87_021E7A2C(app);
    GfGfx_BothDispOn();
    i = 0;
    y = 0x1A;
    do {
        app->topScreenMonSprites[i] = ov87_021E7F6C(&app->spriteMan, 0, i, 0x3CU, y, 1, 0, 0);
        ov87_021E7FD4(app->topScreenMonSprites[i], 0);
        y += 0x2A;
        i += 1;
    } while (i < 4);
    i = 0;
    do {
        app->scratchMonSprites[i] = ov87_021E7F6C(&app->spriteMan, 1, 0, ov87_021E82E4[i].x, ov87_021E82E4[i].y, 0, 2, 0xA);
        ov87_021E7FD4(app->scratchMonSprites[i], 0);
        i += 1;
    } while (i < 9);
    ov87_021E6704(app);
    i = 0;
    do {
        app->cardSprites[i] = ov87_021E7F6C(&app->spriteMan, 2, i, ov87_021E81A0[i].x, ov87_021E81A0[i].y, 0, 1, 0);
        ov87_021E7FEC(app->cardSprites[i], ov87_021E81A0[i].x - 0x100, ov87_021E81A0[i].y);
        ov87_021E7FE0(app->cardSprites[i], i + 0x14);
        i += 1;
    } while (i < 4);
    app->winPopupSprite = ov87_021E7F6C(&app->spriteMan, 4, 0, 0x26U, 0x38, 0, 0, 0xA);
    ov87_021E7FD4(app->winPopupSprite, 0);
    reg_GX_POWCNT &= 0xFFFF7FFF;
    EnableTouchPad();
    InitializeTouchPad(1);
    Main_SetVBlankIntrCB(ov87_021E6C04, app);
}

void ov87_021E6AE0(ScratchOffCardApp *arg0) {
    arg0->yesNoMenu = YesNoPrompt_Create(HEAP_ID_122);
}
