// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifibattlematch_view.h: the lines in use
// so far.
#ifndef BWPORT_APP_WIFIBATTLEMATCH_VIEW_H
#define BWPORT_APP_WIFIBATTLEMATCH_VIEW_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "system/printsys.h"

// The BG graphics and OBJ resources that the screens share, and their animated palette
typedef struct WifiBattleMatchRes WifiBattleMatchRes;

// pokebw2's WifiBattleMatchRes_Update
// Animates the backdrop's palette
void func_ov177_021e5980(WifiBattleMatchRes *res);
// pokebw2's WifiBattleMatchRes_Free
void func_ov177_021e5868(WifiBattleMatchRes *res);

// A player's digital card, of the Wi-Fi competition's or of the live one's, and the rank card
typedef struct WifiBattleMatchCard WifiBattleMatchCard;

// Black 2's; twinned code confirms the offsets of data, startDate, endDate, look, unkA0, unkA4, unkA8 and variant in
// Black
// What the digital card is made from, for the Wi-Fi competition and for the live one
typedef struct {
    // The entry's data, which starts with the player's name
    u8 data[0x92];
    u8 unk92[2];
    // Dates, as the year since 2000, the month and the day in the bytes from the top
    u32 startDate;
    u32 endDate;
    // The player's appearance (func_02008bf4)
    u32 look;
    u16 unkA0;
    u8 unkA2[2];
    u32 unkA4;
    u16 unkA8;
    // Field 11 of the entry, which picks the card's colors
    u16 variant;
} WifiBattleMatchWifiCardInfo;

// Black 2's; twinned code confirms the offsets of data, startDate, endDate, look, unkA0, unkA2, unkA4, unkA6 and
// variant in Black
typedef struct {
    u8 data[0x92];
    u8 unk92[2];
    u32 startDate;
    u32 endDate;
    u32 look;
    u16 unkA0;
    u16 unkA2;
    u16 unkA4;
    u16 unkA6;
    // Field 11 of the entry, which picks the card's colors
    u16 variant;
    u8 unkAA[2];
} WifiBattleMatchLiveCardInfo;

// pokebw2's WifiBattleMatchCard_SlideIn
// Slide the card in and out, each frame, and return TRUE once it is done
BOOL func_ov177_021e64c8(WifiBattleMatchCard *card);
// pokebw2's WifiBattleMatchCard_SlideOut
BOOL func_ov177_021e65fc(WifiBattleMatchCard *card);
// pokebw2's WifiBattleMatchWifiCard_ShowPage
// Shows a page of the card: its Pokémon, then two other pages that hide them
void func_ov177_021e5f24(WifiBattleMatchCard *card, u32 page, MsgData *msgData, PrintQueue *printQueue, Font *font,
                         HeapID heapId);
// pokebw2's WifiBattleMatchLiveCard_ShowPage
void func_ov177_021e62cc(WifiBattleMatchCard *card, u32 page, MsgData *msgData, PrintQueue *printQueue, Font *font,
                         HeapID heapId);

typedef struct WifiBattleMatchPartnerCard WifiBattleMatchPartnerCard;
typedef struct WifiBattleMatchTeamList WifiBattleMatchTeamList;
typedef struct WifiBattleMatchLinkIcon WifiBattleMatchLinkIcon;

// pokebw2's WifiBattleMatchLinkIcon_Create
// The icon of the link
WifiBattleMatchLinkIcon *func_ov177_021e7d20(ClActUnit *unit, WifiBattleMatchRes *res, HeapID heapId);
// pokebw2's WifiBattleMatchTeamList_Flush
BOOL func_ov177_021e84f8(WifiBattleMatchTeamList *list, PrintQueue *printQueue);
// pokebw2's WifiBattleMatchPartnerCard_Free
void func_ov177_021e7710(WifiBattleMatchPartnerCard *card);
// pokebw2's WifiBattleMatchPartnerCard_SlideIn
BOOL func_ov177_021e777c(WifiBattleMatchPartnerCard *card);
// pokebw2's WifiBattleMatchTeamList_Free
void func_ov177_021e82bc(WifiBattleMatchTeamList *list);
// pokebw2's WifiBattleMatchTeamList_SlideIn
BOOL func_ov177_021e8338(WifiBattleMatchTeamList *list);
// pokebw2's WifiBattleMatchTeamList_SlideOut
BOOL func_ov177_021e8420(WifiBattleMatchTeamList *list);

// Black 2's; twinned code confirms the offsets of titleMsg, unk4, look, unkC, unk10 and unk12 in Black
// What the rank card shows
struct WifiBattleMatchRankInfo {
    // The message of the title is this plus 2
    u32 titleMsg;
    u16 unk4;
    u8 unk6[2];
    // The player's appearance
    u32 look;
    u32 unkC;
    u16 unk10;
    u16 unk12;
};

// pokebw2's WifiBattleMatchRankCard_Create
// The rank card
WifiBattleMatchCard *func_ov177_021e5c20(const WifiBattleMatchRankInfo *info, u32 titleMsg, PlayerInfo *playerInfo,
                                         ClActUnit *unit, WifiBattleMatchRes *res, Font *font,
                                         PrintQueue *printQueue, MsgData *msgData, WordSet *wordSet, HeapID heapId);
// pokebw2's WifiBattleMatchRankCard_Free
void func_ov177_021e5d40(WifiBattleMatchCard *card);
// pokebw2's WifiBattleMatchWifiCard_Free
void func_ov177_021e5ef8(WifiBattleMatchCard *card);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WifiBattleMatchRes_Create
// The backdrop. WifiBattleMatchRes_Create takes the unit of the cell actors and a kind of backdrop, 0 to 3
WifiBattleMatchRes *func_ov177_021e567c(ClActUnit *unit, u32 kind, HeapID heapId);
// pokebw2's WifiBattleMatchCard_Flush
// Is TRUE once everything of the card is printed
BOOL func_ov177_021e64c0(WifiBattleMatchCard *card, PrintQueue *printQueue);
// pokebw2's WifiBattleMatchWifiCard_Create
// The digital card. The first of each pair is for the Wi-Fi competition's, the second for the live one's. ratio says
// whether the card's results are printed as two numbers with a message between them; main picks the main screen, or the
// sub screen
WifiBattleMatchCard *func_ov177_021e5d68(const WifiBattleMatchWifiCardInfo *info, u32 ratio, PlayerInfo *playerInfo,
                                         ClActUnit *unit, WifiBattleMatchRes *res, Font *font,
                                         PrintQueue *printQueue, MsgData *msgData, WordSet *wordSet, void *records,
                                         BOOL main, HeapID heapId);
// pokebw2's WifiBattleMatchLiveCard_Create
WifiBattleMatchCard *func_ov177_021e6118(const WifiBattleMatchLiveCardInfo *info, PlayerInfo *playerInfo,
                                         ClActUnit *unit, WifiBattleMatchRes *res, Font *font,
                                         PrintQueue *printQueue, MsgData *msgData, WordSet *wordSet, void *records,
                                         BOOL main, HeapID heapId);
// pokebw2's WifiBattleMatchLiveCard_Free
void func_ov177_021e62a0(WifiBattleMatchCard *card);
// pokebw2's WifiBattleMatchLinkIcon_Free
void func_ov177_021e7df0(WifiBattleMatchLinkIcon *icon);

// pokebw2's WifiBattleMatchLinkIcon_Update
void func_ov177_021e7e08(WifiBattleMatchLinkIcon *icon);
// pokebw2's WifiBattleMatchPartnerCard_Flush
BOOL func_ov177_021e7774(WifiBattleMatchPartnerCard *card, PrintQueue *printQueue);
// pokebw2's WifiBattleMatchPartnerCard_Create
// The other player's card
WifiBattleMatchPartnerCard *func_ov177_021e7520(const LiveBattleMatchPlayerData *data, ClActUnit *unit,
                                                WifiBattleMatchRes *res, Font *font, PrintQueue *printQueue,
                                                MsgData *msgData, WordSet *wordSet, u32 kind, u32 shown, u32 variant,
                                                HeapID heapId);
// pokebw2's WifiBattleMatchTeamList_Create
// The list of the team
WifiBattleMatchTeamList *func_ov177_021e7e3c(BattleBoxSave *battleBox, ClActUnit *unit, WifiBattleMatchRes *res,
                                             Font *font, PrintQueue *printQueue, MsgData *msgData, HeapID heapId);
// pokebw2's WifiBattleMatchRes_LoadBG
// Loads the BG screens of a kind of backdrop
void func_ov177_021e58a4(WifiBattleMatchRes *res, u32 kind, HeapID heapId);
// pokebw2's WifiBattleMatchLinkIcon_SetShown
void func_ov177_021e7e0c(WifiBattleMatchLinkIcon *icon, BOOL shown);

#endif // BWPORT_APP_WIFIBATTLEMATCH_VIEW_H
