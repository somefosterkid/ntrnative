#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "message_format.h"
#include "party.h"
#include "pokemon.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221D8C4(void *, s32, void *);

s32 ov65_0221F748(Sprite *, s32, s32);

extern void *ov65_0221FF4C;

void ov65_0221EF0C(void *arg0);

u8 ov65_0221FB90(void *, s32, s32, void *, MessageFormat *);
extern s32 ov65_0221F120;
s32 ov65_0221F028(void *arg0);

void ov65_0221EF0C(void *arg0) {
    void *v1;
    void *v0;
    void *new_var[2];
    s32 v3;
    void *v2;
    v0 = arg0;
    ClearWindowTilemapAndScheduleTransfer(arg0 + 0x574);
    ClearWindowTilemapAndScheduleTransfer(arg0 + 0x584);
    v2 = v0;
    new_var[0] = &ov65_0221FF4C;
    v3 = 0;
    new_var[1] = v2;
    v1 = v2 + 0x444;
    do {
        if ((*(u16 *)((u8 *)v2 + offsetof(TradeRoom, monDisplayData[0].species))) != 0) {
            ov65_0221F748(*((Sprite **)(((u8 *)new_var[1]) + 0x37C)), (*((s32 *)new_var[0])) + 0x10, (*((s32 *)(((u8 *)new_var[0]) + 4))) - 6);
            Sprite_SetDrawFlag(*((Sprite **)(((u8 *)new_var[1]) + 0x37C)), 1);
            if ((*(u16 *)((u8 *)v2 + offsetof(TradeRoom, monDisplayData[0].heldItemType))) != 0) {
                ov65_0221F748(*((Sprite **)(((u8 *)new_var[1]) + 0x3AC)), (*((s32 *)new_var[0])) + 0x24, (*((s32 *)(((u8 *)new_var[0]) + 4))) + 0x10);
                Sprite_SetDrawFlag(*((Sprite **)(((u8 *)new_var[1]) + 0x3AC)), 1);
            }
            if ((*(s32 *)((u8 *)v2 + offsetof(TradeRoom, monDisplayData[0].ballCapsuleId))) != 0) {
                ov65_0221F748(*((Sprite **)(((u8 *)new_var[1]) + 0x3DC)), (*((s32 *)new_var[0])) + 0x2C, (*((s32 *)(((u8 *)new_var[0]) + 4))) + 0x10);
                Sprite_SetDrawFlag(*((Sprite **)(((u8 *)new_var[1]) + 0x3DC)), 1);
            }
            ScheduleWindowCopyToVram(v1 + ((v3 + 7) * 0x10));
        }
        v3 += 1;
        v2 += sizeof(((TradeRoom *)0)->monDisplayData[0]);
        new_var[0] += 8;
        new_var[1] += 4;
    } while (v3 < 0xC);
    Sprite_SetDrawFlag(((TradeRoom *)v0)->cursorSprites[0], 1);
    Sprite_SetDrawFlag(((TradeRoom *)v0)->cursorSprites[1], 1);
    ov65_0221D8C4(v0 + 0x444, 1, v0);
    Sprite_SetDrawFlag(((TradeRoom *)v0)->detailCardSprites[3], 0);
    Sprite_SetDrawFlag(((TradeRoom *)v0)->convergeIndicatorSprite, 0);
}

s32 ov65_0221F028(void *arg0) {
    FillBgTilemapRect(((TradeRoom *)arg0)->bgConfig, 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
    BufferBoxMonNickname(((MessageFormat *)((TradeRoom *)arg0)->strTemplate2), 0U, Mon_GetBoxMon(Party_GetMonByIndex(((TradeRoom *)arg0)->playerParty, *(s32 *)((u8 *)arg0 + 0x94))));
    BufferBoxMonNickname(((MessageFormat *)((TradeRoom *)arg0)->strTemplate2), 1U, Mon_GetBoxMon(Party_GetMonByIndex(((TradeRoom *)arg0)->partnerParty, *(s32 *)((u8 *)arg0 + 0x98) - 6)));
    ov65_0221FB90(arg0 + 0x5B4, 0x15, 1, *((void **)&((TradeRoom *)arg0)->msgLoader), ((MessageFormat *)((TradeRoom *)arg0)->strTemplate2));
    *((s32 **)&((TradeRoom *)arg0)->subStepCallback) = &ov65_0221F120;
    return 0;
}
