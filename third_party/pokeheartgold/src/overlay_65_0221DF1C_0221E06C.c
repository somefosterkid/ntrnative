#include "global.h"

#include "bg_window.h"
#include "overlay_manager.h"
#include "party.h"
#include "save_special_ribbons.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_02088288.h"
#include "overlay_65_trade_room.h"

typedef struct UnkStruct_ov65_0221E144 {
    u8 unk_0[0x180];
    BgConfig *unk_180;
    void *unk_184;
    u8 unk_188[0x8];
    void *unk_190;
    u8 unk_194[0x420];
    u8 unk_5B4;
    u8 unk_5B5[0x1C6B];
    s32 *unk_2220;
} UnkStruct_ov65_0221E144;

extern u8 _0221FD34;

void ov65_0221E06C(TradeRoom *arg0, s32 arg1);

void ov65_0221E06C(TradeRoom *arg0, s32 arg1) {
    if (arg1 == 0) {
        *((Party **)&arg0->summaryArgs.party) = (Party *)arg0->playerParty;
        *((s8 *)&arg0->summaryArgs.partyCount) = (s8)Party_GetCount(*(Party **)((u8 *)(((void *)arg0->args)) + 8));
        *((void **)&arg0->summaryArgs.unk28) = NULL;
        sub_0208AD34(&arg0->summaryArgs, sub_02034818((u32)sub_0203769C()));
    } else {
        *((Party **)&arg0->summaryArgs.party) = (Party *)arg0->partnerParty;
        *((s8 *)&arg0->summaryArgs.partyCount) = (s8)Party_GetCount(arg0->partnerParty);
        *((void **)&arg0->summaryArgs.unk28) = (void *)arg0->chatotCryBuffer[1 ^ sub_0203769C()];
        sub_0208AD34(&arg0->summaryArgs, sub_02034818(sub_0203769C() ^ 1));
    }
    *((s8 *)&arg0->summaryArgs.unk11) = 1;
    *((s8 *)&arg0->summaryArgs.partySlot) = (s8)((s32) *((s32 *)&arg0->selectedSlot[0]) % 6);
    *((s8 *)&arg0->summaryArgs.unk12) = 1;
    *((s16 *)&arg0->summaryArgs.moveToLearn) = 0;
    *((u32 *)&arg0->summaryArgs.unk2C) = sub_02088288(*(SaveData **)((u8 *)(((void *)arg0->args)) + 0x10));
    *((s32 *)&arg0->summaryArgs.natDexEnabled) = (s32) * (s32 *)((u8 *)(((void *)arg0->args)) + 0x2C);
    *((s32 *)&arg0->summaryArgs.options) = (s32) * (s32 *)((u8 *)(((void *)arg0->args)) + 0x18);
    *((SaveSpecialRibbons **)&arg0->summaryArgs.ribbons) = Save_SpecialRibbons_Get(*(SaveData **)((u8 *)(((void *)arg0->args)) + 0x10));
    sub_02089D40(&arg0->summaryArgs, &_0221FD34);
    *((OverlayManager **)&arg0->appMan) = OverlayManager_New(&gOverlayTemplate_PokemonSummary, &arg0->summaryArgs, HEAP_ID_26);
    *((s32 *)&arg0->summarySide) = arg1;
}

u8 ov65_0221FB90(void *, s32, s32, MsgData *, MessageFormat *);

extern s32 ov65_0221E194;

s32 ov65_0221E144(UnkStruct_ov65_0221E144 *arg0);

s32 ov65_0221E144(UnkStruct_ov65_0221E144 *arg0) {
    FillBgTilemapRect(arg0->unk_180, 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
    ov65_0221FB90((void *)&arg0->unk_5B4, 0x19, 1, arg0->unk_190, arg0->unk_184);
    arg0->unk_2220 = &ov65_0221E194;
    return 0;
}
