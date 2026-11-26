#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "message_format.h"
#include "msgdata.h"
#include "party.h"
#include "pokemon.h"
#include "overlay_65_trade_room.h"

void ov65_0221D8C4(Window *window, int side, TradeRoom *tradeRoom);

void ov65_0221F748(Sprite *sprite, int x, int y);

extern const int ov65_0221FF4C[12][2];

void ov65_0221EF0C(TradeRoom *tradeRoom);

u8 ov65_0221FB90(Window *window, s32 msgno, s32 fontId, MsgData *msgData, MessageFormat *messageFormat);
extern s32 ov65_0221F120;
s32 ov65_0221F028(TradeRoom *tradeRoom);

void ov65_0221EF0C(TradeRoom *tradeRoom) {
    int i;

    ClearWindowTilemapAndScheduleTransfer(&tradeRoom->windows[19]);
    ClearWindowTilemapAndScheduleTransfer(&tradeRoom->windows[20]);
    for (i = 0; i < 12; i++) {
        if (tradeRoom->monDisplayData[i].species != 0) {
            ov65_0221F748(tradeRoom->iconSprites[i], ov65_0221FF4C[i][0] + 0x10, ov65_0221FF4C[i][1] - 6);
            Sprite_SetDrawFlag(tradeRoom->iconSprites[i], TRUE);
            if (tradeRoom->monDisplayData[i].heldItemType != 0) {
                ov65_0221F748(tradeRoom->itemIconSprites[i], ov65_0221FF4C[i][0] + 0x24, ov65_0221FF4C[i][1] + 0x10);
                Sprite_SetDrawFlag(tradeRoom->itemIconSprites[i], TRUE);
            }
            if (tradeRoom->monDisplayData[i].ballCapsuleId != 0) {
                ov65_0221F748(tradeRoom->capsuleIconSprites[i], ov65_0221FF4C[i][0] + 0x2C, ov65_0221FF4C[i][1] + 0x10);
                Sprite_SetDrawFlag(tradeRoom->capsuleIconSprites[i], TRUE);
            }
            ScheduleWindowCopyToVram(&tradeRoom->windows[i + 7]);
        }
    }
    Sprite_SetDrawFlag(tradeRoom->cursorSprites[0], TRUE);
    Sprite_SetDrawFlag(tradeRoom->cursorSprites[1], TRUE);
    ov65_0221D8C4(tradeRoom->windows, 1, tradeRoom);
    Sprite_SetDrawFlag(tradeRoom->detailCardSprites[3], FALSE);
    Sprite_SetDrawFlag(tradeRoom->convergeIndicatorSprite, FALSE);
}

s32 ov65_0221F028(TradeRoom *tradeRoom) {
    FillBgTilemapRect(tradeRoom->bgConfig, 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
    BufferBoxMonNickname((MessageFormat *)tradeRoom->strTemplate2, 0U, Mon_GetBoxMon(Party_GetMonByIndex(tradeRoom->playerParty, tradeRoom->selectedSlot[0])));
    BufferBoxMonNickname((MessageFormat *)tradeRoom->strTemplate2, 1U, Mon_GetBoxMon(Party_GetMonByIndex(tradeRoom->partnerParty, tradeRoom->selectedSlot[1] - 6)));
    ov65_0221FB90(&tradeRoom->windows[23], 0x15, 1, tradeRoom->msgLoader, (MessageFormat *)tradeRoom->strTemplate2);
    tradeRoom->subStepCallback = &ov65_0221F120;
    return 0;
}
