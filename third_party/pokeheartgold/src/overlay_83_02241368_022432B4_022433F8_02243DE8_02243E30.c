#include "global.h"

#include "filesystem.h"
#include "font.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "unk_0203A3B0.h"

#include "overlay_83_battle_castle_opponent_app.h"

u8 ov80_02237B58(u8 challengeType, BOOL includePartnersMons);
void ov83_0224442C(BgConfig *bgConfig);
void ov83_022471FC(BattleCastleAppSpriteManager *spriteMan);
void *ov83_0224753C(struct BattleCastleAppSprite *sprite);
void ov83_02247858(BattleCastleYesNoPrompt *yesNo);
void ov83_0224791C(Window *windows, u8 isOpponentApp);
void ov83_02247A18(struct GridInputHandler *inputHandler);
void ov83_02247CC4(struct TouchscreenListMenuSpawner *spawner);

void ov83_02243E30(BattleCastleOpponentApp *app);

void ov83_02243E30(BattleCastleOpponentApp *app) {
    s32 i;
    s32 j;
    s32 n;

    ov83_02247858(&app->yesNoPrompt);
    ov83_02247CC4(app->touchMenuSpawner);
    ov83_02247A18(app->partyGridInput);
    ov83_0224753C(app->cursorSprite);
    ov83_0224753C(app->partnerCursorSprite);
    ov83_0224753C(app->sparklesSprite);
    ov83_0224753C(app->selectedMonSprite);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            ov83_0224753C(app->flagSprites[i][j]);
        }
    }
    n = ov80_02237B58(app->challengeType, TRUE);
    for (i = 0; i < n; i++) {
        ov83_0224753C(app->ballSprites[i]);
        ov83_0224753C(app->monSprites[i]);
        ov83_0224753C(app->barSprites[i]);
    }
    sub_0203A914();
    PaletteData_FreeBuffers(app->plttData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->plttData, PLTTBUF_MAIN_BG);
    PaletteData_Free(app->plttData);
    app->plttData = NULL;
    ov83_022471FC(&app->spriteMan);
    DestroyMsgData(app->msgLoader);
    MessageFormat_Delete(app->strTemplate);
    String_Delete(app->displayStr);
    String_Delete(app->fmtStr);
    MessagePrinter_Delete(app->msgPrinter);
    FontID_Release(FONT_UNK_4);
    for (i = 0; i < 3; i++) {
        String_Delete(app->simpleMenuStr[i]);
    }
    ov83_0224791C(app->windows, 1);
    ov83_0224442C(app->bgConfig);
    NARC_Delete(app->narc);
}
