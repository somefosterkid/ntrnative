#include "global.h"

#include "battle/battle_system.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "render_window.h"
#include "constants/heap.h"
#include "battle/battle.h"
#include "sprite.h"
#include "sprite_system.h"
#include "battle/battle_bag_context.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"

void ov08_022230CC(BgConfig *arg0);

struct BattleBagContext;
struct MessagePrinter;
struct Indicator;
struct MsgData;
struct MessageFormat;
typedef struct Indicator Indicator;
enum BattleBagScreen {
    BATTLE_BAG_SCREEN_MENU = 0,
    BATTLE_BAG_SCREEN_POCKET_MENU,
    BATTLE_BAG_SCREEN_USE_ITEM,
};
void ov08_022232AC(BattleBag *battleBag, enum BattleBagScreen screen);

void ov08_022230CC(BgConfig *bgConfig) {
    GfGfx_EngineBTogglePlanes(0x17U, 0U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 6U);
}

void ov08_02224254(BattleBag *, void *);

s32 sub_0200E63C(u32 messageBoxFrame);

void ov08_022230F4(void *arg0);

void ov08_022230F4(void *arg0) {
    NNSG2dScreenData *ppScrData;
    NARC *narc;
    u32 frame;
    void *v0;

    narc = NARC_New(NARC_a_0_7_7, ((BattleBag *)arg0)->context->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, ((BattleBag *)arg0)->background, GF_BG_LYR_SUB_2, 0U, 0U, 0, ((BattleBag *)arg0)->context->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, ((BattleBag *)arg0)->background, GF_BG_LYR_SUB_2, 0U, 0U, 0, ((BattleBag *)arg0)->context->heapID);
    v0 = NARC_AllocAndReadWholeMember(narc, 1U, ((BattleBag *)arg0)->context->heapID);
    NNS_G2dGetUnpackedScreenData(v0, &ppScrData);
    ov08_02224254(arg0, ppScrData->rawData);
    Heap_Free(v0);
    NARC_Delete(narc);
    PaletteData_LoadNarc(((BattleBag *)arg0)->palette, NARC_a_0_7_7, 3, ((BattleBag *)arg0)->context->heapID, PLTTBUF_SUB_BG, 0x180U, 0U);
    PaletteData_LoadNarc(((BattleBag *)arg0)->palette, NARC_graphic_font, 8, ((BattleBag *)arg0)->context->heapID, PLTTBUF_SUB_BG, 0x20U, 0xF0U);
    frame = BattleSystem_GetFrame(((BattleBag *)arg0)->context->battleSys);
    GfGfxLoader_LoadCharData(NARC_a_0_3_8, sub_0200E63C(frame), ((BattleBag *)arg0)->background, GF_BG_LYR_SUB_2_F, 0x3E2U, 0U, 0, ((BattleBag *)arg0)->context->heapID);
    PaletteData_LoadNarc(((BattleBag *)arg0)->palette, NARC_a_0_3_8, (s32)sub_0200E640((s32)frame), ((BattleBag *)arg0)->context->heapID, PLTTBUF_SUB_BG, 0x20U, 0xE0U);
}

void ov08_022231E8(void *arg0);

void ov08_022231E8(void *arg0) {
    *((MsgData **)&((BattleBag *)arg0)->messageLoader) = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 5, ((BattleBag *)arg0)->context->heapID);
    *((MessagePrinter **)&((BattleBag *)arg0)->sLotoId) = MessagePrinter_New(0xFU, 0xEU, 0U, ((BattleBag *)arg0)->context->heapID);
    *((MessageFormat **)&((BattleBag *)arg0)->stringTemplate) = MessageFormat_New(((BattleBag *)arg0)->context->heapID);
    ((BattleBag *)arg0)->string = String_New(0x200U, ((BattleBag *)arg0)->context->heapID);
}

void ov08_02223228(BattleBag *arg0);

void ov08_02223228(BattleBag *arg0) {
    DestroyMsgData(*((MsgData * *)&arg0->messageLoader));
    MessagePrinter_Delete(*((MessagePrinter * *)&arg0->sLotoId));
    MessageFormat_Delete(*((MessageFormat * *)&arg0->stringTemplate));
    String_Delete(arg0->string);
}

void ov08_02223248(BattleBag *arg0, s32 arg1);

void ov08_02223248(BattleBag *arg0, s32 arg1) {
    switch (arg1) {
    case 0:
        ScheduleSetBgPosText(arg0->background, 6U, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(arg0->background, 6U, BG_POS_OP_SET_XSCALE, 0);
        return;
    case 1:
        ScheduleSetBgPosText(arg0->background, 6U, BG_POS_OP_SET_ROT, 0x100);
        ScheduleSetBgPosText(arg0->background, 6U, BG_POS_OP_SET_XSCALE, 0);
        return;
    case 2:
        ScheduleSetBgPosText(arg0->background, 6U, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(arg0->background, 6U, BG_POS_OP_SET_XSCALE, 0x100);
        return;
    }
}

void ov08_022232AC(BattleBag *battleBag, enum BattleBagScreen screen) {
    if (screen != BATTLE_BAG_SCREEN_USE_ITEM) {
        return;
    }

    BgTilemapRectChangePalette(battleBag->background, GF_BG_LYR_SUB_2, 2, 35, 28, 4, 8 + battleBag->currentBattlePocket);
    BgTilemapRectChangePalette(battleBag->background, GF_BG_LYR_SUB_2, 2, 40, 28, 8, 8 + battleBag->currentBattlePocket);
}
