#include "global.h"

#include "filesystem.h"
#include "font.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "unk_0203A3B0.h"
#include "overlay_83_battle_castle_self_app.h"
#include "battle_castle_app_sprite.h"
#include "overlay_80_02237A70.h"


void ov83_0223F734(BgConfig *);

void ov83_022471FC(BattleCastleAppSpriteManager *spriteMan);

void *ov83_0224753C(BattleCastleAppSprite *sprite);

void ov83_02247858(BattleCastleYesNoPrompt *yesNo);

void ov83_0224791C(Window *windows, u8 isOpponentApp);

void ov83_02247A18(struct GridInputHandler *inputHandler);

void ov83_02247CC4(struct TouchscreenListMenuSpawner *spawner);

void ov83_0223F058(BattleCastleSelfApp *param_1);

void ov83_0223F058(BattleCastleSelfApp *param_1) {
    s32 iVar1;
    s32 iVar3;
    ov83_02247858(&param_1->yesNoPrompt);
    ov83_02247CC4(param_1->touchMenuSpawner);
    ov83_02247A18(param_1->partyGridInput);
    ov83_0224753C(param_1->upArrowSprite);
    ov83_0224753C(param_1->downArrowSprite);
    ov83_0224753C(param_1->bigSparkles);
    ov83_0224753C(param_1->cursorSprite);
    ov83_0224753C(param_1->partnerCursorSprite);
    ov83_0224753C(param_1->itemSelectCursorSprite);
    ov83_0224753C(param_1->selectedMonSprite);
    ov83_0224753C(param_1->subScreenSprite);
    iVar1 = ov80_02237B24(param_1->challengeType, 1);
    iVar3 = 0;
    if (0 < iVar1) {
        do {
            ov83_0224753C(param_1->monIconSprites[iVar3]);
            ov83_0224753C(param_1->itemIconSprites[iVar3]);
            ov83_0224753C(param_1->barSprites[iVar3]);
            iVar3 = iVar3 + 1;
        } while (iVar3 < iVar1);
    }
    iVar1 = 0;
    do {
        ov83_0224753C(param_1->listItemSprites[iVar1]);
        iVar1 = iVar1 + 1;
    } while (iVar1 < 6);
    sub_0203A914();
    PaletteData_FreeBuffers(param_1->plttData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(param_1->plttData, PLTTBUF_MAIN_BG);
    PaletteData_Free(param_1->plttData);
    param_1->plttData = NULL;
    ov83_022471FC(&param_1->spriteMan);
    DestroyMsgData(param_1->msgLoader);
    DestroyMsgData(param_1->itemMsgLoader);
    MessageFormat_Delete(param_1->strTemplate);
    String_Delete(param_1->displayStr);
    String_Delete(param_1->fmtStr);
    MessagePrinter_Delete(param_1->msgPrinter);
    FontID_Release(FONT_UNK_4);
    iVar1 = 0;
    do {
        String_Delete(param_1->yesNoStrs[iVar1]);
        iVar1 = iVar1 + 1;
    } while (iVar1 < 3);
    ov83_0224791C(param_1->windows, 0);
    ov83_0223F734(param_1->bgConfig);
    NARC_Delete(param_1->narc);
    return;
}
