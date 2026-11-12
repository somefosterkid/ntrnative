// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_input.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "save/pokedex.h"
#include "system/bgwinfrm.h"
#include "system/bmp_menulist.h"
#include "worldtrade_local.h"
#include "system/bmp_menuwork.h"

// The Global Trade Station's input of a Pokémon's name, gender and level and of a country, in a window that slides
// over the lower screen, for the deposit and search screens. A name is chosen by its initial's group, then the
// initial, then the name, and a country the same way. The names are ours, guessed

// The steps of the input, by data_ov135_02203c48
enum {
    WI_SEQ_NONE,
    WI_SEQ_WININ,
    WI_SEQ_WINWAIT,
    WI_SEQ_WINOUT,
    WI_SEQ_WINOFF,
    WI_SEQ_WINOUT_ALL,
    WI_SEQ_HEAD1_INIT,
    WI_SEQ_HEAD1_MAIN,
    WI_SEQ_HEAD1_EXIT,
    WI_SEQ_HEAD1_RETURN,
    WI_SEQ_HEAD2_INIT,
    WI_SEQ_HEAD2_MAIN,
    WI_SEQ_HEAD2_EXIT,
    WI_SEQ_HEAD2_RETURN,
    WI_SEQ_POKENAME_INIT,
    WI_SEQ_POKENAME_MAIN,
    WI_SEQ_POKENAME_CANCEL_EXIT,
    WI_SEQ_POKENAME_EXIT,
    WI_SEQ_NATION_HEAD1_INIT,
    WI_SEQ_NATION_HEAD1_MAIN,
    WI_SEQ_NATION_HEAD1_EXIT,
    WI_SEQ_NATION_HEAD1_RETURN,
    WI_SEQ_NATION_HEAD2_INIT,
    WI_SEQ_NATION_HEAD2_MAIN,
    WI_SEQ_NATION_HEAD2_EXIT,
    WI_SEQ_NATION_HEAD2_RETURN,
    WI_SEQ_NATION_INIT,
    WI_SEQ_NATION_MAIN,
    WI_SEQ_NATION_CANCEL_EXIT,
    WI_SEQ_NATION_EXIT,
    WI_SEQ_SEX_INIT,
    WI_SEQ_SEX_MAIN,
    WI_SEQ_SEX_EXIT,
    WI_SEQ_LEVEL_INIT,
    WI_SEQ_LEVEL_MAIN,
    WI_SEQ_LEVEL_EXIT,
};

// What the select functions return besides a choice
#define INPUT_NULL ((u32) - 1)
#define INPUT_CANCEL ((u32) - 2)

// The Back button of the initial groups
#define INPUT_HEAD_CANCEL 9

// The colors of a choice that can be taken and of one that can't
#define INPUT_COLOR_NORMAL 0x3dc2
#define INPUT_COLOR_GRAY 0x2122

// The BG the window frame is drawn on
#define INPUT_FRAME_BG 2

// Black 2's; twinned code confirms the offsets of menuWin, cursorAct, arrowAct, bgWinFrm, zukan, sinouTable, nameList,
// numFont, head1, head2, poke, nation, sex, level, listpos, seq, next, bgFrame, type, page, listMax, seeCheck,
// listposBackupX and print in Black
struct WorldTradeInputWork {
    BmpWin **menuWin;
    BmpWin **backWin;
    ClActor *cursorAct;
    ClActor *arrowAct[2];
    ClActor *searchCursorAct;
    // The window's screen, which slides in and out
    BGWinFrame *bgWinFrm;
    PokeDexSave *zukan;
    MsgData *msgManager;
    MsgData *monsNameManager;
    MsgData *countryNameManager;
    u8 *sinouTable;
    // The Pokémon names, levels or countries listed
    ListMenuOption *nameList;
    WorldTradeNumFont *numFont;
    // The initial's group and the initial chosen, and the results
    s16 head1;
    s16 head2;
    s16 poke;
    s16 nation;
    s8 sex;
    s8 level;
    int listpos;
    int seq;
    int next;
    int bgFrame;
    s16 type;
    s16 page;
    int listMax;
    int situation;
    // Whether any Pokémon of each initial has been seen
    u8 seeCheck[26];
    // The column the cursor left the grid from
    u8 listposBackupX;
    WorldTradePrint print;
};

// Black 2's, not checked against Black
typedef struct {
    int start;
    int count;
} WorldTradeHeadwordRange;

static void func_ov135_021f9260(WorldTradeInputWork *wk, int mode);
static void func_ov135_021f9660(WorldTradeInputWork *wk, int mode);
static u32 func_ov135_021f9a1c(WorldTradeInputWork *wk, int mode);
static u32 func_ov135_021f9a88(WorldTradeInputWork *wk, u8 *seeCheck);
static u32 func_ov135_021f9cb0(WorldTradeInputWork *wk, int decide);
static u32 func_ov135_021f9cdc(WorldTradeInputWork *wk, u8 *seeCheck);
static int func_ov135_021f9eec(int num, int inPage);
static u32 func_ov135_021f9f00(WorldTradeInputWork *wk, int decide);
static void func_ov135_021f9f58(WorldTradeInputWork *wk, int move);
static u32 func_ov135_021f9ff0(WorldTradeInputWork *wk);
static u32 func_ov135_021fa15c(WorldTradeInputWork *wk);
static void func_ov135_021fa2b0(WorldTradeInputWork *wk, int move);
static u32 func_ov135_021fa330(WorldTradeInputWork *wk, int decide);
static u32 func_ov135_021fa394(WorldTradeInputWork *wk);
static u32 func_ov135_021fa4fc(WorldTradeInputWork *wk, int decide);
static u32 func_ov135_021fa514(WorldTradeInputWork *wk, u8 *seeCheck);
static u32 func_ov135_021fa760(WorldTradeInputWork *wk, int decide);
static void func_ov135_021fa7c4(WorldTradeInputWork *wk, int move);
static u32 func_ov135_021fa85c(WorldTradeInputWork *wk);
static void func_ov135_021fa9c8(WorldTradeInputWork *wk, int type, int x);
static void func_ov135_021faad4(WorldTradeInputWork *wk);
static void func_ov135_021fab70(WorldTradeInputWork *wk);
static void func_ov135_021fac50(void *frm, BmpWin *win, StrBuf *str, int x, u16 color, WorldTradePrint *print);
static u32 func_ov135_021fac84(WorldTradeInputWork *wk);
static u32 func_ov135_021fac8c(WorldTradeInputWork *wk);
static u32 func_ov135_021fad08(WorldTradeInputWork *wk);
static u32 func_ov135_021fad24(WorldTradeInputWork *wk);
static u32 func_ov135_021fad84(WorldTradeInputWork *wk);
static u32 func_ov135_021fada8(WorldTradeInputWork *wk);
static u32 func_ov135_021fadf0(WorldTradeInputWork *wk);
static u32 func_ov135_021fae2c(WorldTradeInputWork *wk);
static u32 func_ov135_021fae8c(WorldTradeInputWork *wk);
static u32 func_ov135_021faee0(WorldTradeInputWork *wk);
static u32 func_ov135_021faff0(WorldTradeInputWork *wk);
static u32 func_ov135_021fb164(WorldTradeInputWork *wk);
static u32 func_ov135_021fb1e4(WorldTradeInputWork *wk);
static u32 func_ov135_021fb220(WorldTradeInputWork *wk);
static int func_ov135_021fb240(u8 *sinou, PokeDexSave *zukan, int num, u16 *sortList, int start, int end);
static int func_ov135_021fb26c(ListMenuOption **menulist, MsgData *monsNameManager, MsgData *msgManager, u8 *sinou,
                                  int head, PokeDexSave *zukan);
static void func_ov135_021fb2f0(WorldTradeInputWork *wk, int page, int max);
static void func_ov135_021fb364(WorldTradeInputWork *wk, int page, int max);
static u32 func_ov135_021fb3d8(WorldTradeInputWork *wk);
static u32 func_ov135_021fb510(WorldTradeInputWork *wk);
static u32 func_ov135_021fb588(WorldTradeInputWork *wk);
static u32 func_ov135_021fb5e4(WorldTradeInputWork *wk);
static u32 func_ov135_021fb614(WorldTradeInputWork *wk);
static u32 func_ov135_021fb674(WorldTradeInputWork *wk);
static u32 func_ov135_021fb704(WorldTradeInputWork *wk);
static u32 func_ov135_021fb7a0(WorldTradeInputWork *wk);
static u32 func_ov135_021fb800(WorldTradeInputWork *wk);
u32 func_ov135_021fb964(WorldTradeInputWork *wk);
static u32 func_ov135_021fb9e4(WorldTradeInputWork *wk);
static u32 func_ov135_021fba5c(WorldTradeInputWork *wk);
static u32 func_ov135_021fba84(WorldTradeInputWork *wk);
static u32 func_ov135_021fbba0(WorldTradeInputWork *wk);
static u32 func_ov135_021fbc1c(WorldTradeInputWork *wk);
static u32 func_ov135_021fbc78(WorldTradeInputWork *wk);
static u32 func_ov135_021fbcb8(WorldTradeInputWork *wk);
static u32 func_ov135_021fbcf4(WorldTradeInputWork *wk);
static u32 func_ov135_021fbd68(WorldTradeInputWork *wk);
static u32 func_ov135_021fbd94(WorldTradeInputWork *wk);
static u32 func_ov135_021fbdd0(WorldTradeInputWork *wk);
u32 func_ov135_021fbe54(WorldTradeInputWork *wk);
static void func_ov135_021fbe84(void *frm, MsgData *msgManager, BmpWin *win, int msgNo, int x, u16 color,
                              WorldTradePrint *print);
static void func_ov135_021fbeb8(void *frm, MsgData *msgManager, BmpWin *win, int msgNo, WorldTradePrint *print);
static void func_ov135_021fbf04(void *frm, WorldTradeNumFont *numFont, BmpWin *win, int page, int max);
static int func_ov135_021fbf6c(WorldTradeInputWork *wk, int siin);
static BOOL func_ov135_021fbfb0(WorldTradeInputWork *wk, int select);
static int func_ov135_021fc008(WorldTradeInputWork *wk, int siin);
static BOOL func_ov135_021fc014(WorldTradeInputWork *wk, int select);

// Where the windows and the cursor's stops of each list are, in characters or pixels
// pokebw2's sSexWinPos
static const u8 data_ov135_02203200[3][2] = { { 3, 3 }, { 3, 7 }, { 3, 11 } };
// pokebw2's sLevelWinPos
static const u8 data_ov135_02203206[4][2] = { { 3, 2 }, { 3, 5 }, { 3, 8 }, { 3, 11 } };
// pokebw2's sSexCursorPos
static const u8 data_ov135_0220320e[4][2] = { { 192, 32 }, { 192, 64 }, { 192, 96 }, { 192, 136 } };
// pokebw2's sLevelCursorPos
static const u8 data_ov135_02203216[5][2] = { { 192, 24 }, { 192, 48 }, { 192, 72 }, { 192, 96 }, { 192, 136 } };
// pokebw2's sNameCursorPos
static const u8 data_ov135_02203220[5][2] = { { 192, 24 }, { 192, 48 }, { 192, 72 }, { 192, 96 }, { 192, 136 } };
// pokebw2's sNameWinPos
static const u8 data_ov135_0220322a[4][3] = {
#include "asset/worldtrade_input_data_ov135_0220322a.inc"
};
// pokebw2's sNationCursorPos
static const u8 data_ov135_02203236[6][2] = {
#include "asset/worldtrade_input_data_ov135_02203236.inc"
};
// pokebw2's sNationWinPos
static const u8 data_ov135_02203242[6][2] = {
#include "asset/worldtrade_input_data_ov135_02203242.inc"
};

// The gender each choice searches for: either, male or female, and the Back button
// pokebw2's sSexSelectTable
static const int data_ov135_02203250[4] = { 2, 0, 1, INPUT_CANCEL };

// Where the cursor goes from each choice of the initials with up, down, left and right
// pokebw2's sHead2KeyTable
static const u8 data_ov135_02203260[4][4] = {
#include "asset/worldtrade_input_data_ov135_02203260.inc"
};

// pokebw2's sSexTouchTbl
static const TouchRect data_ov135_02203270[5] = {
    { 24, 44, 136, 248 }, { 56, 76, 136, 248 }, { 88, 108, 136, 248 }, { 133, 154, 197, 250 }, { TOUCH_RECT_END },
};

// pokebw2's sHead1WinPos
static const u8 data_ov135_022032ae[12][2] = {
#include "asset/worldtrade_input_data_ov135_022032ae.inc"
};
// pokebw2's sHead1CursorPos
static const u8 data_ov135_022032c6[12][2] = {
#include "asset/worldtrade_input_data_ov135_022032c6.inc"
};

// pokebw2's sHead2TouchTbl
static const TouchRect data_ov135_02203284[5] = {
    { 39, 56, 142, 159 }, { 39, 56, 174, 191 }, { 39, 56, 206, 223 }, { 133, 154, 197, 250 }, { TOUCH_RECT_END },
};
// pokebw2's sNameTouchTbl
static const TouchRect data_ov135_022032de[8] = {
    { 16, 35, 136, 248 },   { 40, 60, 136, 248 },   { 64, 84, 136, 248 },   { 88, 108, 136, 248 },
    { 133, 154, 198, 248 }, { 108, 129, 143, 160 }, { 108, 126, 223, 240 }, { TOUCH_RECT_END },
};

// pokebw2's sNationHead1CursorPos
static const u8 data_ov135_02203298[11][2] = {
#include "asset/worldtrade_input_data_ov135_02203298.inc"
};

// pokebw2's sNationTouchTbl
static const TouchRect data_ov135_022032fe[9] = {
    { 8, 28, 8, 247 },      { 32, 52, 8, 247 },   { 56, 76, 8, 247 },     { 80, 100, 8, 247 }, { 104, 124, 8, 247 },
    { 133, 154, 195, 248 }, { 124, 145, 77, 96 }, { 124, 145, 173, 190 }, { TOUCH_RECT_END },
};

// Where each group's initials start among the 26
// pokebw2's sHeadOffset
static const int data_ov135_02203324[9] = { 0, 3, 6, 9, 12, 15, 18, 21, 24 };

// pokebw2's sHead1KeyTable
static const u8 data_ov135_02203348[10][4] = {
#include "asset/worldtrade_input_data_ov135_02203348.inc"
};
// pokebw2's sHead1TouchTbl
static const TouchRect data_ov135_02203370[11] = {
    { 39, 56, 142, 167 },  { 39, 56, 174, 199 },   { 39, 56, 206, 231 },  { 63, 80, 142, 167 },
    { 63, 80, 174, 199 },  { 63, 80, 206, 231 },   { 87, 104, 142, 167 }, { 87, 104, 174, 199 },
    { 87, 104, 206, 231 }, { 133, 154, 197, 250 }, { TOUCH_RECT_END },
};
// pokebw2's sNationHead1KeyTable
static const u8 data_ov135_0220339c[11][4] = {
#include "asset/worldtrade_input_data_ov135_0220339c.inc"
};
// pokebw2's sNationHead1TouchTbl
static const TouchRect data_ov135_022033c8[12] = {
    { 39, 56, 142, 167 },  { 39, 56, 174, 199 },   { 39, 56, 206, 231 },  { 63, 80, 142, 167 },
    { 63, 80, 174, 199 },  { 63, 80, 206, 231 },   { 87, 104, 142, 167 }, { 87, 104, 174, 199 },
    { 87, 104, 206, 231 }, { 133, 154, 197, 250 }, { 16, 35, 136, 248 },  { TOUCH_RECT_END },
};

// The initials of each group, as the first and the count
// pokebw2's sHeadwordRange
static const WorldTradeHeadwordRange data_ov135_022033f8[9] = {
#include "asset/worldtrade_input_data_ov135_022033f8.inc"
};

// The message of each initial
// pokebw2's sHeadwordMsg
static const u32 data_ov135_02203440[27] = {
#include "asset/worldtrade_input_data_ov135_02203440.inc"
};

static u32 (*data_ov135_02203c48[])(WorldTradeInputWork *wk) = {
    func_ov135_021fac84,
    func_ov135_021fac8c,
    func_ov135_021fad08,
    func_ov135_021fad24,
    func_ov135_021fad84,
    func_ov135_021fada8,
    func_ov135_021fadf0,
    func_ov135_021fae2c,
    func_ov135_021fae8c,
    func_ov135_021faee0,
    func_ov135_021faff0,
    func_ov135_021fb164,
    func_ov135_021fb1e4,
    func_ov135_021fb220,
    func_ov135_021fb3d8,
    func_ov135_021fb510,
    func_ov135_021fb588,
    func_ov135_021fb5e4,
    func_ov135_021fb614,
    func_ov135_021fb674,
    func_ov135_021fb704,
    func_ov135_021fb7a0,
    func_ov135_021fb800,
    func_ov135_021fb964,
    func_ov135_021fb9e4,
    func_ov135_021fba5c,
    func_ov135_021fba84,
    func_ov135_021fbba0,
    func_ov135_021fbc1c,
    func_ov135_021fbc78,
    func_ov135_021fbcb8,
    func_ov135_021fbcf4,
    func_ov135_021fbd68,
    func_ov135_021fbd94,
    func_ov135_021fbdd0,
    func_ov135_021fbe54,
};

// pokebw2's Input_SelectBmpWinAdd
static void func_ov135_021f9260(WorldTradeInputWork *wk, int mode) {
    int i;

    GFL_BGSysFillChar(wk->bgFrame, 0, 1, 0);
    switch (mode) {
    case INPUT_MODE_HEADWORD_1:
        wk->menuWin[14] = BmpWin_CreateDynamic(wk->bgFrame, 9, 17, 6, 2, 1, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[14]), 10);
        for (i = 0; i < 9; i++) {
            wk->menuWin[i] = BmpWin_CreateDynamic(wk->bgFrame, data_ov135_022032ae[i][0], data_ov135_022032ae[i][1], 3, 2, 1, 1);
            GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[i]), 10);
        }
        break;
    case INPUT_MODE_HEADWORD_2:
        wk->menuWin[14] = BmpWin_CreateDynamic(wk->bgFrame, 9, 17, 6, 2, 1, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[14]), 10);
        wk->menuWin[0] = BmpWin_CreateDynamic(wk->bgFrame, 2, 2, 3, 2, 1, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[0]), 7);
        for (i = 1; i < 4; i++) {
            wk->menuWin[i] =
                BmpWin_CreateDynamic(wk->bgFrame, data_ov135_022032ae[i - 1][0], data_ov135_022032ae[i - 1][1], 2, 2, 1, 1);
            GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[i]), 10);
        }
        break;
    case INPUT_MODE_POKEMON_NAME:
        wk->menuWin[14] = BmpWin_CreateDynamic(wk->bgFrame, 9, 17, 6, 2, 1, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[14]), 10);
        for (i = 0; i < 4; i++) {
            wk->menuWin[i] = BmpWin_CreateDynamic(wk->bgFrame, data_ov135_0220322a[i][0], data_ov135_0220322a[i][1], 8, 2, 1, 1);
            GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[i]), 10);
        }
        wk->menuWin[4] = BmpWin_CreateDynamic(wk->bgFrame, 5, 14, 5, 1, 1, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[4]), 8);
        break;
    case INPUT_MODE_NATION_HEAD1:
        wk->menuWin[14] = BmpWin_CreateDynamic(wk->bgFrame, 9, 17, 6, 2, 1, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[14]), 10);
        for (i = 0; i < 9; i++) {
            wk->menuWin[i] = BmpWin_CreateDynamic(wk->bgFrame, data_ov135_022032ae[i][0], data_ov135_022032ae[i][1], 3, 2, 1, 1);
            GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[i]), 10);
        }
        wk->menuWin[15] = BmpWin_CreateDynamic(wk->bgFrame, 1, 2, 9, 2, 1, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[15]), 10);
        break;
    case INPUT_MODE_NATION:
        wk->menuWin[14] = BmpWin_CreateDynamic(wk->bgFrame, 24, 17, 6, 2, 1, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[14]), 10);
        for (i = 0; i < 5; i++) {
            wk->menuWin[i] = BmpWin_CreateDynamic(wk->bgFrame, data_ov135_02203242[i][0], data_ov135_02203242[i][1], 23, 2, 1, 1);
            GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[i]), 10);
        }
        wk->menuWin[5] = BmpWin_CreateDynamic(wk->bgFrame, 14, 16, 5, 1, 1, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[5]), 10);
        break;
    case INPUT_MODE_SEX:
        wk->menuWin[14] = BmpWin_CreateDynamic(wk->bgFrame, 9, 17, 6, 2, 1, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[14]), 10);
        for (i = 0; i < 3; i++) {
            wk->menuWin[i] = BmpWin_CreateDynamic(wk->bgFrame, data_ov135_02203200[i][0], data_ov135_02203200[i][1], 9, 2, 1, 1);
            GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[i]), 10);
        }
        break;
    case INPUT_MODE_LEVEL:
        wk->menuWin[14] = BmpWin_CreateDynamic(wk->bgFrame, 9, 17, 6, 2, 1, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[14]), 10);
        for (i = 0; i < 4; i++) {
            wk->menuWin[i] = BmpWin_CreateDynamic(wk->bgFrame, data_ov135_02203206[i][0], data_ov135_02203206[i][1], 11, 2, 1, 1);
            GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[i]), 10);
        }
        wk->menuWin[4] = BmpWin_CreateDynamic(wk->bgFrame, 5, 14, 5, 1, 1, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[4]), 8);
        break;
    }
}

// Clears the windows' characters, and the frame's edge when the window was moved to its place
static inline void Input_ClearFrameEdge(s8 x) {
    if (x == 16) {
        GFL_BGSysFillScrArea(INPUT_FRAME_BG, 5, 16, 1, 1, 15, 16);
        GFL_BGSysFillScrArea(INPUT_FRAME_BG, 6, 17, 1, 15, 15, 16);
        GFL_BGSysLoadScr(INPUT_FRAME_BG);
    }
}

static inline void Input_ClearMenuWin(WorldTradeInputWork *wk, int i) {
    GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[i]), 10);
    BmpWin_FlushChar(wk->menuWin[i]);
    BmpWin_Free(wk->menuWin[i]);
}

// pokebw2's Input_SelectBmpWinDel
static void func_ov135_021f9660(WorldTradeInputWork *wk, int mode) {
    int i;
    s8 x;
    s8 y;

    func_0202cf98(wk->bgWinFrm, 0, &x, &y);
    switch (mode) {
    case INPUT_MODE_HEADWORD_1:
        for (i = 0; i < 9; i++) {
            Input_ClearMenuWin(wk, i);
        }
        Input_ClearFrameEdge(x);
        BmpWin_Free(wk->menuWin[14]);
        break;
    case INPUT_MODE_HEADWORD_2:
        for (i = 0; i < 4; i++) {
            Input_ClearMenuWin(wk, i);
        }
        Input_ClearFrameEdge(x);
        BmpWin_Free(wk->menuWin[14]);
        break;
    case INPUT_MODE_POKEMON_NAME:
        for (i = 0; i < 5; i++) {
            Input_ClearMenuWin(wk, i);
        }
        Input_ClearFrameEdge(x);
        BmpWin_Free(wk->menuWin[14]);
        break;
    case INPUT_MODE_NATION_HEAD1:
        for (i = 0; i < 9; i++) {
            Input_ClearMenuWin(wk, i);
        }
        Input_ClearFrameEdge(x);
        BmpWin_Free(wk->menuWin[14]);
        BmpWin_Free(wk->menuWin[15]);
        break;
    case INPUT_MODE_NATION:
        for (i = 0; i < 6; i++) {
            Input_ClearMenuWin(wk, i);
        }
        BmpWin_Free(wk->menuWin[14]);
        break;
    case INPUT_MODE_SEX:
        for (i = 0; i < 3; i++) {
            BmpWin_Free(wk->menuWin[i]);
        }
        BmpWin_Free(wk->menuWin[14]);
        break;
    case INPUT_MODE_LEVEL:
        for (i = 0; i < 5; i++) {
            BmpWin_Free(wk->menuWin[i]);
        }
        BmpWin_Free(wk->menuWin[14]);
        break;
    }
    GFL_BGSysFreeFilledChar(wk->bgFrame, 1, 0);
}

// pokebw2's WorldTrade_Input_Init
WorldTradeInputWork *func_ov135_021f9890(WorldTradeInputHeader *header, int frame, int situation) {
    WorldTradeInputWork *wk =
        GFL_HeapAllocate(HEAPID_WORLDTRADE, sizeof(WorldTradeInputWork), FALSE, "worldtrade_input.c", 822);

    wk->menuWin = header->menuWin;
    wk->backWin = header->backWin;
    wk->cursorAct = header->cursorAct;
    wk->arrowAct[0] = header->arrowAct[0];
    wk->arrowAct[1] = header->arrowAct[1];
    wk->searchCursorAct = header->searchCursorAct;
    wk->msgManager = header->msgManager;
    wk->monsNameManager = header->monsNameManager;
    wk->countryNameManager = header->countryNameManager;
    wk->zukan = header->zukan;
    wk->sinouTable = header->sinouTable;
    wk->bgFrame = frame;
    wk->situation = situation;
    wk->head1 = 0;
    wk->head2 = 0;
    wk->poke = 0;
    wk->nation = 0;
    wk->sex = 0;
    wk->level = 0;
    wk->listpos = 0;
    wk->listposBackupX = 0;
    wk->seq = WI_SEQ_NONE;

    wk->bgWinFrm = func_0202c9b4(BGWINFRAME_TRANSFER_VBLANK, 1, HEAPID_WORLDTRADE);
    func_0202ca3c(wk->bgWinFrm, 0, 2, 32, 20);
    func_ov135_021f4ff4(wk->cursorAct, 144, 40);
    func_0204b878(wk->cursorAct, 4);
    func_0204b92c(wk->cursorAct, TRUE);
    func_0204b3dc(wk->cursorAct, FALSE);
    wk->numFont = func_ov135_02202974(15, 14, 2, HEAPID_WORLDTRADE);
    func_ov135_022026d8(&wk->print, header->config);
    return wk;
}

// pokebw2's WorldTrade_Input_Start
void func_ov135_021f9968(WorldTradeInputWork *wk, int type) {
    wk->type = type;
    switch (type) {
    case INPUT_MODE_POKEMON_NAME:
        wk->listposBackupX = 0;
        wk->listpos = 0;
        wk->head1 = 0;
        wk->head2 = 0;
        wk->seq = WI_SEQ_HEAD1_INIT;
        break;
    case INPUT_MODE_NATION:
        wk->listposBackupX = 0;
        wk->listpos = 0;
        wk->head1 = 0;
        wk->head2 = 0;
        wk->seq = WI_SEQ_NATION_HEAD1_INIT;
        break;
    case INPUT_MODE_SEX:
        wk->listposBackupX = 0;
        wk->listpos = 0;
        wk->seq = WI_SEQ_SEX_INIT;
        break;
    case INPUT_MODE_LEVEL:
        wk->listposBackupX = 0;
        wk->listpos = 0;
        wk->seq = WI_SEQ_LEVEL_INIT;
        break;
    }
}

// pokebw2's WorldTrade_Input_Exit
void func_ov135_021f99d0(WorldTradeInputWork *wk) {
    func_ov135_02202744(&wk->print);
    func_ov135_022029d4(wk->numFont);
    func_0202ca0c(wk->bgWinFrm);
    GFL_HeapFree(wk);
}

// pokebw2's WorldTrade_Input_Main
u32 func_ov135_021f99f0(WorldTradeInputWork *wk) {
    u32 ret = data_ov135_02203c48[wk->seq](wk);

    func_0202cd94(wk->bgWinFrm);
    func_ov135_02202770(&wk->print);
    func_ov135_022029f8(wk->numFont);
    return ret;
}

// pokebw2's Input_TouchPanelFunc
static u32 func_ov135_021f9a1c(WorldTradeInputWork *wk, int mode) {
    switch (mode) {
    case INPUT_MODE_HEADWORD_1:
        return func_02035c08(data_ov135_02203370);
    case INPUT_MODE_NATION_HEAD1:
        return func_02035c08(data_ov135_022033c8);
    case INPUT_MODE_POKEMON_NAME:
    case INPUT_MODE_LEVEL:
        return func_02035c08(data_ov135_022032de);
    case INPUT_MODE_SEX:
        return func_02035c08(data_ov135_02203270);
    case INPUT_MODE_NATION:
        return func_02035c08(data_ov135_022032fe);
    case INPUT_MODE_HEADWORD_2:
    case INPUT_MODE_NATION_HEAD2:
        return func_02035c08(data_ov135_02203284);
    }
}

// pokebw2's Input_WordHeadSelectMain
static u32 func_ov135_021f9a88(WorldTradeInputWork *wk, u8 *seeCheck) {
    int pos = wk->listpos;
    u32 ret;
    u32 hit;

    if (pos < INPUT_HEAD_CANCEL) {
        wk->listposBackupX = pos;
    }
    if (GCTX_HIDGetPressedKeys() != 0 && func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, TRUE);
        func_020355d8(FALSE);
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return INPUT_NULL;
    }
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        wk->listpos = data_ov135_02203348[wk->listpos][0];
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        wk->listpos = data_ov135_02203348[wk->listpos][1];
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
        wk->listpos = data_ov135_02203348[wk->listpos][2];
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
        wk->listpos = data_ov135_02203348[wk->listpos][3];
    }
    // Back into the grid from the Back button, at the column it was left from
    if (pos >= INPUT_HEAD_CANCEL && wk->listpos < INPUT_HEAD_CANCEL) {
        if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
            wk->listpos = wk->listposBackupX;
            while (wk->listpos + 3 < INPUT_HEAD_CANCEL) {
                wk->listpos += 3;
            }
        } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
            wk->listpos = wk->listposBackupX;
            while (wk->listpos - 3 >= 0) {
                wk->listpos -= 3;
            }
        }
    }
    if (pos != wk->listpos) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov135_021f4ff4(wk->cursorAct, (data_ov135_022032c6[wk->listpos][0] + 16) * 8,
                            data_ov135_022032c6[wk->listpos][1] * 8);
        if (wk->listpos == INPUT_HEAD_CANCEL) {
            func_0204b878(wk->cursorAct, 5);
        } else {
            func_0204b878(wk->cursorAct, 18);
        }
    }

    ret = INPUT_NULL;
    hit = func_ov135_021f9a1c(wk, 4);
    if (hit != (u32)TOUCH_RECT_NONE) {
        func_ov135_021f4ff4(wk->cursorAct, (data_ov135_022032c6[hit][0] + 16) * 8, data_ov135_022032c6[hit][1] * 8);
        if (hit == INPUT_HEAD_CANCEL) {
            func_0204b878(wk->cursorAct, 5);
        } else {
            wk->listpos = hit;
            func_0204b878(wk->cursorAct, 18);
        }
        if (hit == INPUT_HEAD_CANCEL) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            ret = INPUT_CANCEL;
        } else if (seeCheck == NULL || seeCheck[hit] != 0) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            ret = hit;
        }
        if (ret == INPUT_NULL && func_020355c4() == FALSE) {
            func_0204b3dc(wk->cursorAct, FALSE);
        }
        func_020355d8(TRUE);
        return ret;
    }

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        if (wk->listpos == INPUT_HEAD_CANCEL) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            return INPUT_CANCEL;
        }
        GFL_ASSERT(wk->listpos < INPUT_HEAD_CANCEL);
        if (seeCheck == NULL || seeCheck[wk->listpos] != 0) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return wk->listpos;
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_0204b878(wk->cursorAct, 5);
        func_ov135_021f4ff4(wk->cursorAct, 192, 136);
        return INPUT_CANCEL;
    }
    return INPUT_NULL;
}

// pokebw2's Input_Head2DecideFunc
static u32 func_ov135_021f9cb0(WorldTradeInputWork *wk, int decide) {
    if (decide != 3) {
        if (wk->head1 <= 8) {
            if (decide >= data_ov135_022033f8[wk->head1].count) {
                decide = INPUT_NULL;
            }
            return decide;
        }
        return decide;
    }
    return INPUT_CANCEL;
}

// pokebw2's Input_WordHead2SelectMain
static u32 func_ov135_021f9cdc(WorldTradeInputWork *wk, u8 *seeCheck) {
    int pos = wk->listpos;
    long hit;

    if (pos < 3) {
        wk->listposBackupX = pos;
    }
    if (GCTX_HIDGetPressedKeys() != 0 && func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, TRUE);
        func_020355d8(FALSE);
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return INPUT_NULL;
    }
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        wk->listpos = data_ov135_02203260[wk->listpos][0];
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        wk->listpos = data_ov135_02203260[wk->listpos][1];
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
        wk->listpos = data_ov135_02203260[wk->listpos][2];
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
        wk->listpos = data_ov135_02203260[wk->listpos][3];
    }
    if (pos == 3 && wk->listpos < 3) {
        wk->listpos = wk->listposBackupX;
    }
    if (pos != wk->listpos) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        if (wk->listpos == 3) {
            func_ov135_021f4ff4(wk->cursorAct, 192, 136);
            func_0204b878(wk->cursorAct, 5);
        } else {
            func_ov135_021f4ff4(wk->cursorAct, (data_ov135_022032c6[wk->listpos][0] + 16) * 8,
                                data_ov135_022032c6[wk->listpos][1] * 8);
            func_0204b878(wk->cursorAct, 4);
        }
    }

    hit = func_ov135_021f9a1c(wk, 5);
    if (hit != (u32)TOUCH_RECT_NONE) {
        if (hit == 3) {
            func_ov135_021f4ff4(wk->cursorAct, 192, 136);
            func_0204b878(wk->cursorAct, 5);
        } else {
            wk->listpos = hit;
            func_ov135_021f4ff4(wk->cursorAct, (data_ov135_022032c6[hit][0] + 16) * 8, data_ov135_022032c6[hit][1] * 8);
            func_0204b878(wk->cursorAct, 4);
        }
        hit = func_ov135_021f9cb0(wk, hit);
        if (hit == INPUT_CANCEL) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
        } else if (seeCheck == NULL || seeCheck[data_ov135_02203324[wk->head1] + hit] != 0) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        } else {
            hit = INPUT_NULL;
            if (func_020355c4() == FALSE) {
                func_0204b3dc(wk->cursorAct, FALSE);
            }
        }
        func_020355d8(TRUE);
        return hit;
    }

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        hit = func_ov135_021f9cb0(wk, wk->listpos);
        if (hit == INPUT_NULL) {
            return hit;
        }
        if (hit == INPUT_CANCEL) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            return hit;
        }
        if (seeCheck == NULL || seeCheck[data_ov135_02203324[wk->head1] + hit] != 0) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return hit;
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_0204b878(wk->cursorAct, 5);
        func_ov135_021f4ff4(wk->cursorAct, 192, 136);
        return INPUT_CANCEL;
    }
    return INPUT_NULL;
}

// pokebw2's Input_ListPageNum
static int func_ov135_021f9eec(int num, int inPage) {
    if (num == 0) {
        return 1;
    }
    return (num + (inPage - 1)) / inPage;
}

// pokebw2's Input_NameDecideFunc
static u32 func_ov135_021f9f00(WorldTradeInputWork *wk, int decide) {
    switch (decide) {
    case 4:
        return INPUT_CANCEL;
    case 5:
        func_ov135_021f9f58(wk, -1);
        return INPUT_NULL;
    case 6:
        func_ov135_021f9f58(wk, 1);
        return INPUT_NULL;
    default: {
        int index = decide + wk->page * 4;

        if (index < wk->listMax) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return wk->nameList[index].value;
        }
        return INPUT_NULL;
    }
    }
}

// pokebw2's Input_NamePageRefresh
static void func_ov135_021f9f58(WorldTradeInputWork *wk, int move) {
    int max = func_ov135_021f9eec(wk->listMax, 4) - 1;

    if (max == 0) {
        return;
    }
    if (move < 0) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_0204b92c(wk->arrowAct[1], TRUE);
        func_0204b878(wk->arrowAct[1], 39);
        if (wk->page != 0) {
            wk->page--;
        } else {
            wk->page = max;
        }
    } else {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_0204b92c(wk->arrowAct[0], TRUE);
        func_0204b878(wk->arrowAct[0], 38);
        if (wk->page < max) {
            wk->page++;
        } else {
            wk->page = 0;
        }
    }
    func_ov135_021fb2f0(wk, wk->page, wk->listMax);
    func_ov135_021fbf04(wk->bgWinFrm, wk->numFont, wk->menuWin[4], wk->page, func_ov135_021f9eec(wk->listMax, 4));
}

// pokebw2's Input_PokeNameSelectMain
static u32 func_ov135_021f9ff0(WorldTradeInputWork *wk) {
    int pos = wk->listpos;
    u32 hit;

    if (GCTX_HIDGetPressedKeys() != 0 && func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, TRUE);
        func_020355d8(FALSE);
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return INPUT_NULL;
    }
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        if (wk->listpos != 0) {
            wk->listpos--;
        } else {
            wk->listpos = 4;
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        if (wk->listpos != 4) {
            wk->listpos++;
        } else {
            wk->listpos = 0;
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
        func_ov135_021f9f58(wk, -1);
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
        func_ov135_021f9f58(wk, 1);
    }
    if (pos != wk->listpos) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov135_021f4ff4(wk->cursorAct, data_ov135_02203220[wk->listpos][0], data_ov135_02203220[wk->listpos][1]);
        if (wk->listpos == 4) {
            func_0204b878(wk->cursorAct, 5);
        } else {
            func_0204b878(wk->cursorAct, 6);
        }
    }

    hit = func_ov135_021f9a1c(wk, 0);
    if (hit != (u32)TOUCH_RECT_NONE) {
        if (hit < 5) {
            func_ov135_021f4ff4(wk->cursorAct, data_ov135_02203220[hit][0], data_ov135_02203220[hit][1]);
            if (hit == 4) {
                func_0204b878(wk->cursorAct, 5);
            } else {
                wk->listpos = hit;
                func_0204b878(wk->cursorAct, 6);
            }
        }
        hit = func_ov135_021f9f00(wk, hit);
        if (hit == INPUT_NULL && func_020355c4() == FALSE) {
            func_0204b3dc(wk->cursorAct, FALSE);
        }
        func_020355d8(TRUE);
        return hit;
    }

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        return func_ov135_021f9f00(wk, wk->listpos);
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_ov135_021f4ff4(wk->cursorAct, 192, 136);
        func_0204b878(wk->cursorAct, 5);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return INPUT_CANCEL;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SexSelectMain
static u32 func_ov135_021fa15c(WorldTradeInputWork *wk) {
    int pos = wk->listpos;
    u32 hit;
    u32 ret;

    if (GCTX_HIDGetPressedKeys() != 0 && func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, TRUE);
        func_020355d8(FALSE);
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return INPUT_NULL;
    }
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        if (wk->listpos != 0) {
            wk->listpos--;
        } else {
            wk->listpos = 3;
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        if (wk->listpos != 3) {
            wk->listpos++;
        } else {
            wk->listpos = 0;
        }
    }
    if (pos != wk->listpos) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov135_021f4ff4(wk->cursorAct, data_ov135_0220320e[wk->listpos][0], data_ov135_0220320e[wk->listpos][1]);
        if (wk->listpos == 3) {
            func_0204b878(wk->cursorAct, 5);
        } else {
            func_0204b878(wk->cursorAct, 6);
        }
    }

    hit = func_ov135_021f9a1c(wk, 1);
    if (hit != (u32)TOUCH_RECT_NONE) {
        func_020355d8(TRUE);
        func_ov135_021f4ff4(wk->cursorAct, data_ov135_0220320e[hit][0], data_ov135_0220320e[hit][1]);
        if (hit == 3) {
            func_0204b878(wk->cursorAct, 5);
        } else {
            func_0204b878(wk->cursorAct, 6);
        }
        ret = data_ov135_02203250[hit];
        if (ret == INPUT_CANCEL) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
        } else {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
        return ret;
    }

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        if (data_ov135_02203250[wk->listpos] == INPUT_CANCEL) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
        } else {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
        return data_ov135_02203250[wk->listpos];
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov135_021f4ff4(wk->cursorAct, 192, 136);
        func_0204b878(wk->cursorAct, 5);
        return INPUT_CANCEL;
    }
    return INPUT_NULL;
}

// pokebw2's Input_LevelPageRefresh
static void func_ov135_021fa2b0(WorldTradeInputWork *wk, int move) {
    if (move < 0) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_0204b92c(wk->arrowAct[1], TRUE);
        func_0204b878(wk->arrowAct[1], 39);
        if (wk->page != 0) {
            wk->page--;
        } else {
            wk->page = 2;
        }
    } else {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_0204b92c(wk->arrowAct[0], TRUE);
        func_0204b878(wk->arrowAct[0], 38);
        if (wk->page < 2) {
            wk->page++;
        } else {
            wk->page = 0;
        }
    }
    func_ov135_021fb2f0(wk, wk->page, wk->listMax);
    func_ov135_021fbf04(wk->bgWinFrm, wk->numFont, wk->menuWin[4], wk->page, func_ov135_021f9eec(wk->listMax, 4));
}

// pokebw2's Input_LevelDecideFunc
static u32 func_ov135_021fa330(WorldTradeInputWork *wk, int decide) {
    switch (decide) {
    case 4:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return INPUT_CANCEL;
    case 5:
        func_ov135_021fa2b0(wk, -1);
        return INPUT_NULL;
    case 6:
        func_ov135_021fa2b0(wk, 1);
        return INPUT_NULL;
    default:
        if (decide + wk->page * 4 < wk->listMax) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return wk->nameList[decide].value;
        }
        return INPUT_NULL;
    }
}

// pokebw2's Input_LevelSelectMain
static u32 func_ov135_021fa394(WorldTradeInputWork *wk) {
    int pos = wk->listpos;
    u32 hit;

    if (GCTX_HIDGetPressedKeys() != 0 && func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, TRUE);
        func_020355d8(FALSE);
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return INPUT_NULL;
    }
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        if (wk->listpos != 0) {
            wk->listpos--;
        } else {
            wk->listpos = 4;
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        if (wk->listpos != 4) {
            wk->listpos++;
        } else {
            wk->listpos = 0;
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
        func_ov135_021fa2b0(wk, -1);
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
        func_ov135_021fa2b0(wk, 1);
    }
    if (pos != wk->listpos) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov135_021f4ff4(wk->cursorAct, data_ov135_02203216[wk->listpos][0], data_ov135_02203216[wk->listpos][1]);
        if (wk->listpos == 4) {
            func_0204b878(wk->cursorAct, 5);
        } else {
            func_0204b878(wk->cursorAct, 6);
        }
    }

    hit = func_ov135_021f9a1c(wk, 2);
    if (hit != (u32)TOUCH_RECT_NONE) {
        if (hit < 5) {
            func_ov135_021f4ff4(wk->cursorAct, data_ov135_02203216[hit][0], data_ov135_02203216[hit][1]);
            if (hit == 4) {
                func_0204b878(wk->cursorAct, 5);
            } else {
                wk->listpos = hit;
                func_0204b878(wk->cursorAct, 6);
            }
        }
        hit = func_ov135_021fa330(wk, hit);
        if (hit == INPUT_NULL && func_020355c4() == FALSE) {
            func_0204b3dc(wk->cursorAct, FALSE);
        }
        func_020355d8(TRUE);
        return hit;
    }

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        return func_ov135_021fa330(wk, wk->listpos);
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_ov135_021f4ff4(wk->cursorAct, 192, 136);
        func_0204b878(wk->cursorAct, 5);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return INPUT_CANCEL;
    }
    return INPUT_NULL;
}

// pokebw2's Input_NationHead1DecideFunc
static u32 func_ov135_021fa4fc(WorldTradeInputWork *wk, int decide) {
    switch (decide) {
    case 9:
        return INPUT_CANCEL;
    case 10:
        return 10;
    }
    return decide;
}

// pokebw2's Input_NationHead1SelectMain
static u32 func_ov135_021fa514(WorldTradeInputWork *wk, u8 *seeCheck) {
    int pos = wk->listpos;
    u32 hit;

    if (pos < 9) {
        wk->listposBackupX = pos;
    }
    if (GCTX_HIDGetPressedKeys() != 0 && func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, TRUE);
        func_020355d8(FALSE);
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return INPUT_NULL;
    }
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        wk->listpos = data_ov135_0220339c[wk->listpos][0];
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        wk->listpos = data_ov135_0220339c[wk->listpos][1];
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
        wk->listpos = data_ov135_0220339c[wk->listpos][2];
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
        wk->listpos = data_ov135_0220339c[wk->listpos][3];
    }
    if (pos >= 9 && wk->listpos < 9) {
        if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
            wk->listpos = wk->listposBackupX;
            while (wk->listpos + 3 < 9) {
                wk->listpos += 3;
            }
        } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
            wk->listpos = wk->listposBackupX;
            while (wk->listpos - 3 >= 0) {
                wk->listpos -= 3;
            }
        }
    }
    if (pos != wk->listpos) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov135_021f4ff4(wk->cursorAct, (data_ov135_02203298[wk->listpos][0] + 16) * 8,
                            data_ov135_02203298[wk->listpos][1] * 8);
        switch (wk->listpos) {
        case 9:
            func_0204b878(wk->cursorAct, 5);
            break;
        case 10:
            func_0204b878(wk->cursorAct, 6);
            break;
        default:
            func_0204b878(wk->cursorAct, 18);
            break;
        }
    }

    hit = func_ov135_021f9a1c(wk, 6);
    if (hit != (u32)TOUCH_RECT_NONE) {
        func_ov135_021f4ff4(wk->cursorAct, (data_ov135_02203298[hit][0] + 16) * 8, data_ov135_02203298[hit][1] * 8);
        switch (hit) {
        case 9:
            func_0204b878(wk->cursorAct, 5);
            break;
        case 10:
            func_0204b878(wk->cursorAct, 6);
            break;
        default:
            func_0204b878(wk->cursorAct, 18);
            wk->listpos = hit;
            break;
        }
        hit = func_ov135_021fa4fc(wk, hit);
        if (hit == INPUT_CANCEL) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
        } else if (hit == 10 || seeCheck == NULL || seeCheck[hit] != 0) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        } else {
            hit = INPUT_NULL;
        }
        if (hit == INPUT_NULL && func_020355c4() == FALSE) {
            func_0204b3dc(wk->cursorAct, FALSE);
        }
        func_020355d8(TRUE);
        return hit;
    }

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        hit = func_ov135_021fa4fc(wk, wk->listpos);
        if (hit == INPUT_CANCEL) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            return hit;
        }
        if (hit == 10 || seeCheck == NULL || seeCheck[hit] != 0) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return hit;
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_ov135_021f4ff4(wk->cursorAct, 192, 136);
        func_0204b878(wk->cursorAct, 5);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return INPUT_CANCEL;
    }
    return INPUT_NULL;
}

// pokebw2's Input_NationDecideFunc
static u32 func_ov135_021fa760(WorldTradeInputWork *wk, int decide) {
    switch (decide) {
    case 5:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return INPUT_CANCEL;
    case 6:
        func_ov135_021fa7c4(wk, -1);
        return INPUT_NULL;
    case 7:
        func_ov135_021fa7c4(wk, 1);
        return INPUT_NULL;
    default: {
        int index = decide + wk->page * 5;

        if (index < wk->listMax) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return wk->nameList[index].value;
        }
        return INPUT_NULL;
    }
    }
}

// pokebw2's Input_NationPageRefresh
static void func_ov135_021fa7c4(WorldTradeInputWork *wk, int move) {
    int max = func_ov135_021f9eec(wk->listMax, 5) - 1;

    if (max == 0) {
        return;
    }
    if (move < 0) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_0204b92c(wk->arrowAct[1], TRUE);
        func_0204b878(wk->arrowAct[1], 39);
        if (wk->page != 0) {
            wk->page--;
        } else {
            wk->page = max;
        }
    } else {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_0204b92c(wk->arrowAct[0], TRUE);
        func_0204b878(wk->arrowAct[0], 38);
        if (wk->page < max) {
            wk->page++;
        } else {
            wk->page = 0;
        }
    }
    func_ov135_021fb364(wk, wk->page, wk->listMax);
    func_ov135_021fbf04(wk->bgWinFrm, wk->numFont, wk->menuWin[5], wk->page, func_ov135_021f9eec(wk->listMax, 5));
}

// pokebw2's Input_NationSelectMain
static u32 func_ov135_021fa85c(WorldTradeInputWork *wk) {
    int pos = wk->listpos;
    u32 hit;

    if (GCTX_HIDGetPressedKeys() != 0 && func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, TRUE);
        func_020355d8(FALSE);
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return INPUT_NULL;
    }
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        if (wk->listpos != 0) {
            wk->listpos--;
        } else {
            wk->listpos = 5;
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        if (wk->listpos != 5) {
            wk->listpos++;
        } else {
            wk->listpos = 0;
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
        func_ov135_021fa7c4(wk, -1);
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
        func_ov135_021fa7c4(wk, 1);
    }
    if (pos != wk->listpos) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov135_021f4ff4(wk->cursorAct, data_ov135_02203236[wk->listpos][0], data_ov135_02203236[wk->listpos][1]);
        if (wk->listpos == 5) {
            func_0204b878(wk->cursorAct, 5);
        } else {
            func_0204b878(wk->cursorAct, 7);
        }
    }

    hit = func_ov135_021f9a1c(wk, 3);
    if (hit != (u32)TOUCH_RECT_NONE) {
        if (hit < 6) {
            func_ov135_021f4ff4(wk->cursorAct, data_ov135_02203236[hit][0], data_ov135_02203236[hit][1]);
            if (hit == 5) {
                func_0204b878(wk->cursorAct, 5);
            } else {
                wk->listpos = hit;
                func_0204b878(wk->cursorAct, 7);
            }
        }
        hit = func_ov135_021fa760(wk, hit);
        if (hit == INPUT_NULL && func_020355c4() == FALSE) {
            func_0204b3dc(wk->cursorAct, FALSE);
        }
        func_020355d8(TRUE);
        return hit;
    }

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        return func_ov135_021fa760(wk, wk->listpos);
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov135_021f4ff4(wk->cursorAct, 192, 136);
        func_0204b878(wk->cursorAct, 5);
        return INPUT_CANCEL;
    }
    return INPUT_NULL;
}

// pokebw2's Input_HeadWord1Init
static void func_ov135_021fa9c8(WorldTradeInputWork *wk, int type, int x) {
    int i;
    StrBuf *str;
    u16 color;

    func_0202cae4(wk->bgWinFrm, 0, ARCID_WORLDTRADE, type == INPUT_MODE_NATION_HEAD1 ? 0x1b : 0x1c, TRUE);
    func_0202cf84(wk->bgWinFrm, 0);
    func_ov135_021f9260(wk, type);
    func_ov135_021fbeb8(wk->bgWinFrm, wk->msgManager, wk->menuWin[14], 0x4a, &wk->print);
    MI_CpuFill8(wk->seeCheck, 1, sizeof(wk->seeCheck));
    for (i = 0; i < 9; i++) {
        str = GFL_MsgDataLoadStrbufNew(wk->msgManager, i + 0x74);
        if (type == INPUT_MODE_HEADWORD_1) {
            if (func_ov135_021fbfb0(wk, i) == TRUE) {
                color = INPUT_COLOR_NORMAL;
                wk->seeCheck[i] = 1;
            } else {
                color = INPUT_COLOR_GRAY;
                wk->seeCheck[i] = 0;
            }
        } else {
            if (func_ov135_021fc014(wk, i) == TRUE) {
                color = INPUT_COLOR_NORMAL;
                wk->seeCheck[i] = 1;
            } else {
                color = INPUT_COLOR_GRAY;
                wk->seeCheck[i] = 0;
            }
        }
        func_ov135_021fac50(wk->bgWinFrm, wk->menuWin[i], str, 2, color, &wk->print);
        GFL_StrBufFree(str);
    }
    if (type == INPUT_MODE_NATION_HEAD1) {
        str = GFL_MsgDataLoadStrbufNew(wk->msgManager, 0xbc);
        func_ov135_021fac50(wk->bgWinFrm, wk->menuWin[15], str, 2, INPUT_COLOR_NORMAL, &wk->print);
        GFL_StrBufFree(str);
    }
    func_0202cb4c(wk->bgWinFrm, 0, x, 0);
    func_0202cc58(wk->bgWinFrm, 0);
}

// pokebw2's Input_SexSelectInit
static void func_ov135_021faad4(WorldTradeInputWork *wk) {
    int i;
    StrBuf *str;

    func_0202cae4(wk->bgWinFrm, 0, ARCID_WORLDTRADE, 0x19, TRUE);
    func_0202cf84(wk->bgWinFrm, 0);
    func_ov135_021f9260(wk, INPUT_MODE_SEX);
    for (i = 0; i < 3; i++) {
        str = GFL_MsgDataLoadStrbufNew(wk->msgManager, i + 0x82);
        func_ov135_021fac50(wk->bgWinFrm, wk->menuWin[i], str, 2, INPUT_COLOR_NORMAL, &wk->print);
        GFL_StrBufFree(str);
    }
    func_ov135_021fbeb8(wk->bgWinFrm, wk->msgManager, wk->menuWin[14], 0x4a, &wk->print);
    func_0202cb4c(wk->bgWinFrm, 0, 32, 0);
    func_0202cc58(wk->bgWinFrm, 0);
    func_0202cd64(wk->bgWinFrm, 0, -4, 0, 4);
}

// pokebw2's Input_LevelSelectInit
static void func_ov135_021fab70(WorldTradeInputWork *wk) {
    func_0202cae4(wk->bgWinFrm, 0, ARCID_WORLDTRADE, 0x1d, TRUE);
    func_0202cf84(wk->bgWinFrm, 0);
    func_ov135_021f9260(wk, INPUT_MODE_LEVEL);
    if (wk->situation == INPUT_SITUATION_SEARCH) {
        wk->listMax = func_ov135_021f87e0(&wk->nameList, wk->msgManager, 1);
    } else if (wk->situation == INPUT_SITUATION_DEPOSIT) {
        wk->listMax = func_ov135_021f87e0(&wk->nameList, wk->msgManager, 0);
    }
    func_ov135_021fb2f0(wk, 0, wk->listMax);
    wk->page = 0;
    func_ov135_021fbf04(wk->bgWinFrm, wk->numFont, wk->menuWin[4], wk->page, func_ov135_021f9eec(wk->listMax, 4));
    func_ov135_021fbeb8(wk->bgWinFrm, wk->msgManager, wk->menuWin[14], 0x4a, &wk->print);
    func_0202cb4c(wk->bgWinFrm, 0, 32, 0);
    func_0204b878(wk->cursorAct, 6);
    wk->listposBackupX = 0;
    wk->listpos = 0;
    func_ov135_021f4ff4(wk->cursorAct, data_ov135_02203220[wk->listpos][0], data_ov135_02203220[wk->listpos][1]);
    func_0202cc58(wk->bgWinFrm, 0);
    func_0202cd64(wk->bgWinFrm, 0, -4, 0, 4);
    wk->seq = WI_SEQ_LEVEL_MAIN;
}

static void func_ov135_021fac50(void *frm, BmpWin *win, StrBuf *str, int x, u16 color, WorldTradePrint *print) {
    func_ov135_022028c8(win, 0, str, x, 0, 0, color, print);
    BmpWin_FlushChar(win);
    func_0202cebc(frm, 0, win);
}

// pokebw2's Input_SeqNone
static u32 func_ov135_021fac84(WorldTradeInputWork *wk) {
    return INPUT_NULL;
}

// pokebw2's Input_SeqWinIn
static u32 func_ov135_021fac8c(WorldTradeInputWork *wk) {
    if (func_0202ce40(wk->bgWinFrm, 0) == FALSE) {
        if (func_020355c4() == TRUE) {
            func_0204b3dc(wk->cursorAct, FALSE);
        } else {
            func_0204b3dc(wk->cursorAct, TRUE);
        }
        if (wk->type == INPUT_MODE_LEVEL) {
            func_0204b92c(wk->arrowAct[0], FALSE);
            func_0204b92c(wk->arrowAct[1], FALSE);
            func_0204b878(wk->arrowAct[0], 38);
            func_0204b878(wk->arrowAct[1], 39);
            func_ov135_021f4ff4(wk->arrowAct[0], 228, 120);
            func_ov135_021f4ff4(wk->arrowAct[1], 154, 120);
            func_0204b3dc(wk->arrowAct[0], TRUE);
            func_0204b3dc(wk->arrowAct[1], TRUE);
        }
        wk->seq = wk->next;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqWinWait
static u32 func_ov135_021fad08(WorldTradeInputWork *wk) {
    if (func_0202ce40(wk->bgWinFrm, 0) == FALSE) {
        wk->seq = wk->next;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqWinOut
static u32 func_ov135_021fad24(WorldTradeInputWork *wk) {
    if (func_0204b98c(wk->cursorAct)) {
        return INPUT_NULL;
    }
    func_0204b3dc(wk->cursorAct, FALSE);
    func_0204b3dc(wk->arrowAct[0], FALSE);
    func_0204b3dc(wk->arrowAct[1], FALSE);
    if (wk->type == INPUT_MODE_NATION && wk->next == WI_SEQ_NATION_CANCEL_EXIT) {
        func_0202cd64(wk->bgWinFrm, 0, 6, 0, 6);
    } else {
        func_0202cd64(wk->bgWinFrm, 0, 4, 0, 4);
    }
    wk->seq = WI_SEQ_WINOFF;
    return INPUT_NULL;
}

// pokebw2's Input_SeqWinOff
static u32 func_ov135_021fad84(WorldTradeInputWork *wk) {
    if (func_0202ce40(wk->bgWinFrm, 0) == FALSE) {
        func_0202cc70(wk->bgWinFrm, 0);
        wk->seq = wk->next;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqWinOutAll
static u32 func_ov135_021fada8(WorldTradeInputWork *wk) {
    if (func_0204b98c(wk->cursorAct)) {
        return INPUT_NULL;
    }
    func_0204b3dc(wk->cursorAct, FALSE);
    func_0204b3dc(wk->arrowAct[0], FALSE);
    func_0204b3dc(wk->arrowAct[1], FALSE);
    func_0202cd64(wk->bgWinFrm, 0, 4, 0, 4);
    wk->seq = WI_SEQ_WINWAIT;
    return INPUT_NULL;
}

// pokebw2's Input_SeqHead1Init
static u32 func_ov135_021fadf0(WorldTradeInputWork *wk) {
    func_ov135_021fa9c8(wk, INPUT_MODE_HEADWORD_1, 32);
    func_0204b878(wk->cursorAct, 18);
    func_ov135_021f4ff4(wk->cursorAct, 144, 40);
    func_0202cd64(wk->bgWinFrm, 0, -4, 0, 4);
    wk->seq = WI_SEQ_WININ;
    wk->next = WI_SEQ_HEAD1_MAIN;
    wk->head1 = -1;
    return INPUT_NULL;
}

// pokebw2's Input_SeqHead1Main
static u32 func_ov135_021fae2c(WorldTradeInputWork *wk) {
    u32 ret = func_ov135_021f9a88(wk, wk->seeCheck);

    if (ret != INPUT_CANCEL) {
        if (ret <= 8) {
            func_0204b3dc(wk->cursorAct, TRUE);
            func_0204b878(wk->cursorAct, 19);
            wk->seq = WI_SEQ_HEAD1_EXIT;
            wk->head1 = ret;
        }
    } else {
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_WINOUT;
        wk->next = WI_SEQ_HEAD1_EXIT;
        wk->head1 = -1;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqHead1Exit
static u32 func_ov135_021fae8c(WorldTradeInputWork *wk) {
    if (func_0204b98c(wk->cursorAct)) {
        return INPUT_NULL;
    }
    func_ov135_021f9660(wk, INPUT_MODE_HEADWORD_1);
    func_0204b3dc(wk->cursorAct, FALSE);
    if (wk->head1 >= 0) {
        if (data_ov135_022033f8[wk->head1].count != 1) {
            wk->seq = WI_SEQ_HEAD2_INIT;
        } else {
            wk->head2 = 0;
            wk->seq = WI_SEQ_POKENAME_INIT;
        }
    } else {
        return INPUT_CANCEL;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqHead1Return
static u32 func_ov135_021faee0(WorldTradeInputWork *wk) {
    int i;
    StrBuf *str;
    u16 color;

    func_0202cae4(wk->bgWinFrm, 0, ARCID_WORLDTRADE, 0x1c, TRUE);
    func_0202cf84(wk->bgWinFrm, 0);
    func_ov135_021f9260(wk, INPUT_MODE_HEADWORD_1);
    func_ov135_021fbeb8(wk->bgWinFrm, wk->msgManager, wk->menuWin[14], 0x4a, &wk->print);
    MI_CpuFill8(wk->seeCheck, 1, sizeof(wk->seeCheck));
    for (i = 0; i < 9; i++) {
        str = GFL_MsgDataLoadStrbufNew(wk->msgManager, i + 0x74);
        if (func_ov135_021fbfb0(wk, i) == TRUE) {
            color = INPUT_COLOR_NORMAL;
            wk->seeCheck[i] = 1;
        } else {
            color = INPUT_COLOR_GRAY;
            wk->seeCheck[i] = 0;
        }
        func_ov135_021fac50(wk->bgWinFrm, wk->menuWin[i], str, 2, color, &wk->print);
        GFL_StrBufFree(str);
    }
    func_0202cb4c(wk->bgWinFrm, 0, 16, 0);
    func_0202cc58(wk->bgWinFrm, 0);
    func_0204b878(wk->cursorAct, 18);
    func_ov135_021f4ff4(wk->cursorAct, (data_ov135_022032c6[wk->head1][0] + 16) * 8,
                               data_ov135_022032c6[wk->head1][1] * 8);
    wk->listpos = wk->head1;
    func_0204b3dc(wk->cursorAct, TRUE);
    wk->seq = WI_SEQ_HEAD1_MAIN;
    if (func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, FALSE);
    } else {
        func_0204b3dc(wk->cursorAct, TRUE);
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqHead2Init
static u32 func_ov135_021faff0(WorldTradeInputWork *wk) {
    int i;
    u16 color;

    func_0202cae4(wk->bgWinFrm, 0, ARCID_WORLDTRADE, 0x1a, TRUE);
    func_0202cf84(wk->bgWinFrm, 0);
    func_ov135_021f9260(wk, INPUT_MODE_HEADWORD_2);
    MI_CpuFill8(wk->seeCheck, 1, sizeof(wk->seeCheck));
    func_ov135_021fbe84(wk->bgWinFrm, wk->msgManager, wk->menuWin[0], wk->head1 + 0x74, 2, INPUT_COLOR_NORMAL,
                      &wk->print);
    for (i = 1; i < data_ov135_022033f8[wk->head1].count + 1; i++) {
        if (func_ov135_021fbf6c(wk, data_ov135_02203324[wk->head1] + i - 1) > 0) {
            color = INPUT_COLOR_NORMAL;
            wk->seeCheck[data_ov135_02203324[wk->head1] + i - 1] = 1;
        } else {
            color = INPUT_COLOR_GRAY;
            wk->seeCheck[data_ov135_02203324[wk->head1] + i - 1] = 0;
        }
        func_ov135_021fbe84(wk->bgWinFrm, wk->msgManager, wk->menuWin[i],
                          data_ov135_02203440[data_ov135_022033f8[wk->head1].start + i - 1], 5, color, &wk->print);
    }
    func_ov135_021fbeb8(wk->bgWinFrm, wk->msgManager, wk->menuWin[14], 0x4a, &wk->print);
    func_0202cb4c(wk->bgWinFrm, 0, 16, 0);
    func_0204b878(wk->cursorAct, 4);
    if (wk->head2 < 0) {
        wk->listpos = 0;
    } else {
        wk->listpos = wk->head2;
    }
    func_ov135_021f4ff4(wk->cursorAct, (data_ov135_022032c6[wk->listpos][0] + 16) * 8,
                               data_ov135_022032c6[wk->listpos][1] * 8);
    if (func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, FALSE);
    } else {
        func_0204b3dc(wk->cursorAct, TRUE);
    }
    wk->seq = WI_SEQ_HEAD2_MAIN;
    return INPUT_NULL;
}

// pokebw2's Input_SeqHead2Main
static u32 func_ov135_021fb164(WorldTradeInputWork *wk) {
    u32 ret = func_ov135_021f9cdc(wk, wk->seeCheck);

    switch (ret) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_HEAD2_EXIT;
        wk->head2 = ret;
        break;
    case INPUT_CANCEL:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_HEAD2_EXIT;
        wk->head2 = -1;
        break;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqHead2Exit
static u32 func_ov135_021fb1e4(WorldTradeInputWork *wk) {
    if (func_0204b98c(wk->cursorAct)) {
        return INPUT_NULL;
    }
    func_0204b3dc(wk->cursorAct, FALSE);
    func_ov135_021f9660(wk, INPUT_MODE_HEADWORD_2);
    if (wk->head2 >= 0) {
        wk->seq = WI_SEQ_POKENAME_INIT;
    } else {
        wk->seq = WI_SEQ_HEAD1_RETURN;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqHead2Return
static u32 func_ov135_021fb220(WorldTradeInputWork *wk) {
    if (func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, FALSE);
    } else {
        func_0204b3dc(wk->cursorAct, TRUE);
    }
    wk->seq = WI_SEQ_HEAD2_INIT;
    return INPUT_NULL;
}

// pokebw2's Input_PokeNameCountSeen
static int func_ov135_021fb240(u8 *sinou, PokeDexSave *zukan, int num, u16 *sortList, int start, int end) {
    int i;
    int count = 0;

    for (i = start; i < end; i++) {
        if (PokeDex_IsSeen(zukan, sortList[i])) {
            count++;
        }
    }
    return count;
}

static int func_ov135_021fb26c(ListMenuOption **menulist, MsgData *monsNameManager, MsgData *msgManager, u8 *sinou,
                                  int head, PokeDexSave *zukan) {
    int i;
    int num;
    int start;
    int end;
    int count;
    u16 *sortList = func_ov135_021f8498(HEAPID_WORLDTRADE, 0, &num);

    func_ov135_021f84bc(head, &start, &end);
    num = end - start;
    count = func_ov135_021fb240(sinou, zukan, num, sortList, start, end);
    *menulist = ListMenuCore_CreateOptionList(count + 1, HEAPID_WORLDTRADE);
    for (i = start; i < end; i++) {
        if (PokeDex_IsSeen(zukan, sortList[i])) {
            ListMenuCore_AppendMsgOption(*menulist, monsNameManager, sortList[i], sortList[i], HEAPID_WORLDTRADE);
        }
    }
    GFL_HeapFree(sortList);
    return count;
}

// pokebw2's Input_PokeNameListPrint
static void func_ov135_021fb2f0(WorldTradeInputWork *wk, int page, int max) {
    int i;

    for (i = 0; i < 4; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[i]), 10);
        if (i + page * 4 < max) {
            func_ov135_021fac50(wk->bgWinFrm, wk->menuWin[i], wk->nameList[i + page * 4].text, 0, INPUT_COLOR_NORMAL,
                           &wk->print);
        } else {
            BmpWin_FlushChar(wk->menuWin[i]);
            func_0202cebc(wk->bgWinFrm, 0, wk->menuWin[i]);
        }
    }
}

// pokebw2's Input_NationListPrint
static void func_ov135_021fb364(WorldTradeInputWork *wk, int page, int max) {
    int i;

    for (i = 0; i < 5; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(wk->menuWin[i]), 10);
        if (i + page * 5 < max) {
            func_ov135_021fac50(wk->bgWinFrm, wk->menuWin[i], wk->nameList[i + page * 5].text, 0, INPUT_COLOR_NORMAL,
                           &wk->print);
        } else {
            BmpWin_FlushChar(wk->menuWin[i]);
            func_0202cebc(wk->bgWinFrm, 0, wk->menuWin[i]);
        }
    }
}

// pokebw2's Input_SeqPokeNameInit
static u32 func_ov135_021fb3d8(WorldTradeInputWork *wk) {
    u8 pageMax;

    func_0202cae4(wk->bgWinFrm, 0, ARCID_WORLDTRADE, 0x1d, TRUE);
    func_0202cf84(wk->bgWinFrm, 0);
    func_ov135_021f9260(wk, INPUT_MODE_POKEMON_NAME);
    wk->listMax = func_ov135_021fb26c(&wk->nameList, wk->monsNameManager, wk->msgManager, wk->sinouTable,
                                         wk->head2 + data_ov135_02203324[wk->head1], wk->zukan);
    wk->page = 0;
    func_ov135_021fb2f0(wk, 0, wk->listMax);
    pageMax = func_ov135_021f9eec(wk->listMax, 4);
    func_ov135_021fbf04(wk->bgWinFrm, wk->numFont, wk->menuWin[4], wk->page, pageMax);
    func_ov135_021fbeb8(wk->bgWinFrm, wk->msgManager, wk->menuWin[14], 0x4a, &wk->print);
    func_0202cb4c(wk->bgWinFrm, 0, 16, 0);
    func_0204b878(wk->cursorAct, 6);
    wk->listpos = 0;
    func_ov135_021f4ff4(wk->cursorAct, data_ov135_02203220[wk->listpos][0], data_ov135_02203220[wk->listpos][1]);
    func_ov135_021f4ff4(wk->arrowAct[0], 228, 120);
    func_ov135_021f4ff4(wk->arrowAct[1], 154, 120);
    if (func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, FALSE);
    } else {
        func_0204b3dc(wk->cursorAct, TRUE);
    }
    if (pageMax == 1) {
        func_0204b878(wk->arrowAct[0], 61);
        func_0204b878(wk->arrowAct[1], 62);
    } else {
        func_0204b878(wk->arrowAct[0], 38);
        func_0204b878(wk->arrowAct[1], 39);
    }
    func_0204b92c(wk->arrowAct[0], FALSE);
    func_0204b92c(wk->arrowAct[1], FALSE);
    func_0204b3dc(wk->arrowAct[0], TRUE);
    func_0204b3dc(wk->arrowAct[1], TRUE);
    wk->seq = WI_SEQ_POKENAME_MAIN;
    return INPUT_NULL;
}

// pokebw2's Input_SeqPokeNameMain
static u32 func_ov135_021fb510(WorldTradeInputWork *wk) {
    u32 ret = func_ov135_021f9ff0(wk);

    switch (ret) {
    case INPUT_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_POKENAME_CANCEL_EXIT;
        wk->poke = -1;
        break;
    case INPUT_NULL:
        break;
    default:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_WINOUT;
        wk->next = WI_SEQ_POKENAME_EXIT;
        wk->poke = ret;
        break;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqPokeNameCancelExit
static u32 func_ov135_021fb588(WorldTradeInputWork *wk) {
    if (func_0204b98c(wk->cursorAct)) {
        return INPUT_NULL;
    }
    func_0204b3dc(wk->cursorAct, FALSE);
    func_0204b3dc(wk->arrowAct[0], FALSE);
    func_0204b3dc(wk->arrowAct[1], FALSE);
    func_ov135_021f9660(wk, INPUT_MODE_POKEMON_NAME);
    ListMenuCore_FreeOptionList(wk->nameList);
    if (data_ov135_022033f8[wk->head1].count != 1) {
        wk->seq = WI_SEQ_HEAD2_RETURN;
    } else {
        wk->seq = WI_SEQ_HEAD1_RETURN;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqPokeNameExit
static u32 func_ov135_021fb5e4(WorldTradeInputWork *wk) {
    if (func_0204b98c(wk->cursorAct)) {
        return INPUT_NULL;
    }
    func_0204b3dc(wk->cursorAct, FALSE);
    func_ov135_021f9660(wk, INPUT_MODE_POKEMON_NAME);
    ListMenuCore_FreeOptionList(wk->nameList);
    return wk->poke;
}

// pokebw2's Input_SeqNationHead1Init
static u32 func_ov135_021fb614(WorldTradeInputWork *wk) {
    func_ov135_021fa9c8(wk, INPUT_MODE_NATION_HEAD1, 32);
    func_0204b878(wk->cursorAct, 18);
    func_0202cd64(wk->bgWinFrm, 0, -4, 0, 4);
    wk->seq = WI_SEQ_WININ;
    wk->next = WI_SEQ_NATION_HEAD1_MAIN;
    wk->listpos = 10;
    wk->head1 = -1;
    func_ov135_021f4ff4(wk->cursorAct, (data_ov135_02203298[wk->listpos][0] + 16) * 8,
                               data_ov135_02203298[wk->listpos][1] * 8);
    func_0204b878(wk->cursorAct, 6);
    return INPUT_NULL;
}

// pokebw2's Input_SeqNationHead1Main
static u32 func_ov135_021fb674(WorldTradeInputWork *wk) {
    u32 ret = func_ov135_021fa514(wk, wk->seeCheck);

    switch (ret) {
    case INPUT_CANCEL:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_WINOUT;
        wk->next = WI_SEQ_NATION_HEAD1_EXIT;
        wk->head1 = -1;
        break;
    case 10:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_WINOUT;
        wk->next = WI_SEQ_NATION_HEAD1_EXIT;
        wk->head1 = -2;
        break;
    case INPUT_NULL:
        break;
    default:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, 19);
        wk->head1 = ret;
        wk->seq = WI_SEQ_NATION_HEAD1_EXIT;
        break;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqNationHead1Exit
static u32 func_ov135_021fb704(WorldTradeInputWork *wk) {
    if (func_0204b98c(wk->cursorAct)) {
        return INPUT_NULL;
    }
    func_0204b3dc(wk->cursorAct, FALSE);
    func_ov135_021f9660(wk, INPUT_MODE_NATION_HEAD1);
    if (wk->head1 < 0) {
        if (wk->head1 == -1) {
            return INPUT_CANCEL;
        }
        // Any country
        if (wk->head1 == -2) {
            return 0;
        }
    } else if (data_ov135_022033f8[wk->head1].count != 1) {
        wk->seq = WI_SEQ_NATION_HEAD2_INIT;
    } else {
        // Only one initial: straight to the countries
        wk->head2 = 0;
        func_0202cd64(wk->bgWinFrm, 0, -4, 0, 3);
        if (wk->searchCursorAct != NULL) {
            func_0204b3dc(wk->searchCursorAct, FALSE);
        }
        func_0204b878(wk->cursorAct, 7);
        wk->seq = WI_SEQ_WINWAIT;
        func_0202cae4(wk->bgWinFrm, 0, ARCID_WORLDTRADE, 0x18, TRUE);
        wk->next = WI_SEQ_NATION_INIT;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqNationHead1Return
static u32 func_ov135_021fb7a0(WorldTradeInputWork *wk) {
    func_ov135_021fa9c8(wk, INPUT_MODE_NATION_HEAD1, 16);
    func_0204b878(wk->cursorAct, 18);
    func_0204b3dc(wk->cursorAct, TRUE);
    wk->listpos = wk->head1;
    func_ov135_021f4ff4(wk->cursorAct, (data_ov135_02203298[wk->listpos][0] + 16) * 8,
                               data_ov135_02203298[wk->listpos][1] * 8);
    if (func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, FALSE);
    } else {
        func_0204b3dc(wk->cursorAct, TRUE);
    }
    wk->seq = WI_SEQ_NATION_HEAD1_MAIN;
    return INPUT_NULL;
}

// pokebw2's Input_SeqNationHead2Init
static u32 func_ov135_021fb800(WorldTradeInputWork *wk) {
    int i;
    u16 color;

    func_0202cae4(wk->bgWinFrm, 0, ARCID_WORLDTRADE, 0x1a, TRUE);
    func_0202cf84(wk->bgWinFrm, 0);
    func_ov135_021f9260(wk, INPUT_MODE_HEADWORD_2);
    MI_CpuFill8(wk->seeCheck, 1, sizeof(wk->seeCheck));
    func_ov135_021fbe84(wk->bgWinFrm, wk->msgManager, wk->menuWin[0], wk->head1 + 0x74, 2, INPUT_COLOR_NORMAL,
                      &wk->print);
    for (i = 1; i < 4; i++) {
        if (func_ov135_021fc008(wk, data_ov135_02203324[wk->head1] + i - 1) > 0) {
            color = INPUT_COLOR_NORMAL;
            wk->seeCheck[data_ov135_02203324[wk->head1] + i - 1] = 1;
        } else {
            color = INPUT_COLOR_GRAY;
            wk->seeCheck[data_ov135_02203324[wk->head1] + i - 1] = 0;
        }
        func_ov135_021fbe84(wk->bgWinFrm, wk->msgManager, wk->menuWin[i],
                          data_ov135_02203440[data_ov135_022033f8[wk->head1].start + i - 1], 5, color, &wk->print);
    }
    func_ov135_021fbeb8(wk->bgWinFrm, wk->msgManager, wk->menuWin[14], 0x4a, &wk->print);
    func_0202cb4c(wk->bgWinFrm, 0, 16, 0);
    func_0204b878(wk->cursorAct, 4);
    if (wk->head2 < 0) {
        wk->listpos = 0;
    } else {
        wk->listpos = wk->head2;
    }
    func_ov135_021f4ff4(wk->cursorAct, (data_ov135_022032c6[wk->listpos][0] + 16) * 8,
                               data_ov135_022032c6[wk->listpos][1] * 8);
    if (func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, FALSE);
    } else {
        func_0204b3dc(wk->cursorAct, TRUE);
    }
    if (wk->searchCursorAct != NULL) {
        func_0204b3dc(wk->searchCursorAct, TRUE);
    }
    wk->seq = WI_SEQ_NATION_HEAD2_MAIN;
    return INPUT_NULL;
}

// pokebw2's Input_SeqNationHead2Main
u32 func_ov135_021fb964(WorldTradeInputWork *wk) {
    u32 ret = func_ov135_021f9cdc(wk, wk->seeCheck);

    switch (ret) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_NATION_HEAD2_EXIT;
        wk->head2 = ret;
        break;
    case INPUT_CANCEL:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_NATION_HEAD2_EXIT;
        wk->head2 = -1;
        break;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqNationHead2Exit
static u32 func_ov135_021fb9e4(WorldTradeInputWork *wk) {
    if (func_0204b98c(wk->cursorAct)) {
        return INPUT_NULL;
    }
    func_0204b3dc(wk->cursorAct, FALSE);
    func_ov135_021f9660(wk, INPUT_MODE_HEADWORD_2);
    if (wk->head2 < 0) {
        wk->seq = WI_SEQ_NATION_HEAD1_RETURN;
    } else {
        func_0202cd64(wk->bgWinFrm, 0, -4, 0, 3);
        if (wk->searchCursorAct != NULL) {
            func_0204b3dc(wk->searchCursorAct, FALSE);
        }
        func_0204b878(wk->cursorAct, 7);
        wk->seq = WI_SEQ_WINWAIT;
        func_0202cae4(wk->bgWinFrm, 0, ARCID_WORLDTRADE, 0x18, TRUE);
        wk->next = WI_SEQ_NATION_INIT;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqNationHead2Return
static u32 func_ov135_021fba5c(WorldTradeInputWork *wk) {
    wk->seq = WI_SEQ_NATION_HEAD2_INIT;
    wk->listpos = wk->head2;
    if (func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, FALSE);
    } else {
        func_0204b3dc(wk->cursorAct, TRUE);
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqNationInit
static u32 func_ov135_021fba84(WorldTradeInputWork *wk) {
    u8 pageMax;

    func_0202cae4(wk->bgWinFrm, 0, ARCID_WORLDTRADE, 0x18, TRUE);
    func_ov135_021f9260(wk, INPUT_MODE_NATION);
    wk->listMax =
        func_ov135_021f8908(&wk->nameList, wk->countryNameManager, wk->head2 + data_ov135_02203324[wk->head1]);
    func_ov135_021fb364(wk, 0, wk->listMax);
    wk->page = 0;
    pageMax = func_ov135_021f9eec(wk->listMax, 5);
    func_ov135_021fbf04(wk->bgWinFrm, wk->numFont, wk->menuWin[5], wk->page, pageMax);
    func_ov135_021fbeb8(wk->bgWinFrm, wk->msgManager, wk->menuWin[14], 0x4a, &wk->print);
    func_0202cb4c(wk->bgWinFrm, 0, 1, 0);
    wk->listpos = 0;
    func_ov135_021f4ff4(wk->cursorAct, data_ov135_02203236[wk->listpos][0], data_ov135_02203236[wk->listpos][1]);
    func_ov135_021f4ff4(wk->arrowAct[0], 176, 136);
    func_ov135_021f4ff4(wk->arrowAct[1], 88, 136);
    if (func_020355c4() == TRUE) {
        func_0204b3dc(wk->cursorAct, FALSE);
    } else {
        func_0204b3dc(wk->cursorAct, TRUE);
    }
    if (pageMax == 1) {
        func_0204b878(wk->arrowAct[0], 61);
        func_0204b878(wk->arrowAct[1], 62);
    } else {
        func_0204b878(wk->arrowAct[0], 38);
        func_0204b878(wk->arrowAct[1], 39);
    }
    func_0204b92c(wk->arrowAct[0], FALSE);
    func_0204b92c(wk->arrowAct[1], FALSE);
    func_0204b3dc(wk->arrowAct[0], TRUE);
    func_0204b3dc(wk->arrowAct[1], TRUE);
    wk->seq = WI_SEQ_NATION_MAIN;
    return INPUT_NULL;
}

// pokebw2's Input_SeqNationMain
static u32 func_ov135_021fbba0(WorldTradeInputWork *wk) {
    u32 ret = func_ov135_021fa85c(wk);

    switch (ret) {
    case INPUT_CANCEL:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        func_0202cae4(wk->bgWinFrm, 0, ARCID_WORLDTRADE, 0x18, TRUE);
        wk->seq = WI_SEQ_WINOUT_ALL;
        wk->next = WI_SEQ_NATION_CANCEL_EXIT;
        break;
    case INPUT_NULL:
        break;
    default:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_WINOUT;
        wk->next = WI_SEQ_NATION_EXIT;
        wk->nation = ret;
        break;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqNationCancelExit
static u32 func_ov135_021fbc1c(WorldTradeInputWork *wk) {
    if (func_0204b98c(wk->cursorAct)) {
        return INPUT_NULL;
    }
    func_0204b3dc(wk->cursorAct, FALSE);
    func_0204b3dc(wk->arrowAct[0], FALSE);
    func_0204b3dc(wk->arrowAct[1], FALSE);
    func_ov135_021f9660(wk, INPUT_MODE_NATION);
    ListMenuCore_FreeOptionList(wk->nameList);
    if (data_ov135_022033f8[wk->head1].count != 1) {
        wk->seq = WI_SEQ_NATION_HEAD2_RETURN;
    } else {
        wk->seq = WI_SEQ_NATION_HEAD1_RETURN;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqNationExit
static u32 func_ov135_021fbc78(WorldTradeInputWork *wk) {
    if (func_0204b98c(wk->cursorAct)) {
        return INPUT_NULL;
    }
    func_0204b3dc(wk->cursorAct, FALSE);
    func_0204b3dc(wk->arrowAct[0], FALSE);
    func_0204b3dc(wk->arrowAct[1], FALSE);
    func_ov135_021f9660(wk, INPUT_MODE_NATION);
    ListMenuCore_FreeOptionList(wk->nameList);
    return wk->nation;
}

// pokebw2's Input_SeqSexInit
static u32 func_ov135_021fbcb8(WorldTradeInputWork *wk) {
    wk->listpos = 0;
    func_ov135_021faad4(wk);
    func_0204b878(wk->cursorAct, 6);
    func_ov135_021f4ff4(wk->cursorAct, data_ov135_0220320e[wk->listpos][0], data_ov135_0220320e[wk->listpos][1]);
    wk->seq = WI_SEQ_WININ;
    wk->next = WI_SEQ_SEX_MAIN;
    return INPUT_NULL;
}

// pokebw2's Input_SeqSexMain
static u32 func_ov135_021fbcf4(WorldTradeInputWork *wk) {
    u32 ret = func_ov135_021fa15c(wk);

    switch (ret) {
    case INPUT_CANCEL:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_WINOUT;
        wk->next = WI_SEQ_SEX_EXIT;
        wk->sex = INPUT_CANCEL;
        break;
    case INPUT_NULL:
        break;
    default:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_WINOUT;
        wk->next = WI_SEQ_SEX_EXIT;
        wk->sex = ret;
        break;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqSexExit
static u32 func_ov135_021fbd68(WorldTradeInputWork *wk) {
    if (func_0204b98c(wk->cursorAct)) {
        return INPUT_NULL;
    }
    func_0204b3dc(wk->cursorAct, FALSE);
    func_ov135_021f9660(wk, INPUT_MODE_SEX);
    return wk->sex;
}

// pokebw2's Input_SeqLevelInit
static u32 func_ov135_021fbd94(WorldTradeInputWork *wk) {
    wk->listpos = 0;
    func_ov135_021fab70(wk);
    func_0204b878(wk->cursorAct, 6);
    func_ov135_021f4ff4(wk->cursorAct, data_ov135_02203216[wk->listpos][0], data_ov135_02203216[wk->listpos][1]);
    wk->seq = WI_SEQ_WININ;
    wk->next = WI_SEQ_LEVEL_MAIN;
    return INPUT_NULL;
}

// pokebw2's Input_SeqLevelMain
static u32 func_ov135_021fbdd0(WorldTradeInputWork *wk) {
    u32 ret = func_ov135_021fa394(wk);

    switch (ret) {
    case INPUT_CANCEL:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_WINOUT;
        wk->next = WI_SEQ_LEVEL_EXIT;
        wk->level = INPUT_CANCEL;
        break;
    case INPUT_NULL:
        break;
    default:
        func_0204b3dc(wk->cursorAct, TRUE);
        func_0204b878(wk->cursorAct, func_0204b898(wk->cursorAct) + 9);
        wk->seq = WI_SEQ_WINOUT;
        wk->next = WI_SEQ_LEVEL_EXIT;
        wk->level = wk->nameList[ret + wk->page * 4].value;
        break;
    }
    return INPUT_NULL;
}

// pokebw2's Input_SeqLevelExit
u32 func_ov135_021fbe54(WorldTradeInputWork *wk) {
    if (func_0204b98c(wk->cursorAct)) {
        return INPUT_NULL;
    }
    func_0204b3dc(wk->cursorAct, FALSE);
    func_ov135_021f9660(wk, INPUT_MODE_LEVEL);
    ListMenuCore_FreeOptionList(wk->nameList);
    return wk->level;
}

static void func_ov135_021fbe84(void *frm, MsgData *msgManager, BmpWin *win, int msgNo, int x, u16 color,
                              WorldTradePrint *print) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(msgManager, msgNo);

    func_ov135_021fac50(frm, win, str, x, color, print);
    GFL_StrBufFree(str);
}

static void func_ov135_021fbeb8(void *frm, MsgData *msgManager, BmpWin *win, int msgNo, WorldTradePrint *print) {
    StrBuf *str;

    str = GFL_MsgDataLoadStrbufNew(msgManager, msgNo);
    func_ov135_022028c8(win, 0, str, 0, 0, 0, 0x3dc2, print);
    BmpWin_FlushChar(win);
    func_0202cebc(frm, 0, win);
    GFL_StrBufFree(str);
}

// pokebw2's Input_PagePrint
static void func_ov135_021fbf04(void *frm, WorldTradeNumFont *numFont, BmpWin *win, int page, int max) {
    GFL_BitmapFill(BmpWin_GetBitmap(win), 8);
    func_ov135_02202a50(numFont, page + 1, 2, 1, win, 0, 0);
    func_ov135_02202b20(numFont, 0, win, 16, 0);
    func_ov135_02202a50(numFont, max, 2, 0, win, 24, 0);
    BmpWin_FlushChar(win);
    func_0202cebc(frm, 0, win);
}

// pokebw2's Input_PokeSeeCountSiin
static int func_ov135_021fbf6c(WorldTradeInputWork *wk, int siin) {
    int num;
    int start;
    int end;
    int count;
    u16 *sortList = func_ov135_021f8498(HEAPID_WORLDTRADE, 0, &num);

    func_ov135_021f84bc(siin, &start, &end);
    num = end - start;
    count = func_ov135_021fb240(wk->sinouTable, wk->zukan, num, sortList, start, end);
    GFL_HeapFree(sortList);
    return count;
}

// pokebw2's Input_PokeSeeCountCategory
static BOOL func_ov135_021fbfb0(WorldTradeInputWork *wk, int select) {
    int start = data_ov135_02203324[select];
    int i;

    if (data_ov135_022033f8[select].count > 0) {
        for (i = 0; i < data_ov135_022033f8[select].count; i++) {
            if (func_ov135_021fbf6c(wk, start + i) > 0) {
                return TRUE;
            }
        }
    } else if (func_ov135_021fbf6c(wk, start) > 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Input_NationCountSiin
static int func_ov135_021fc008(WorldTradeInputWork *wk, int siin) {
    int number;

    return func_ov135_021f88d8(siin, &number);
}

// pokebw2's Input_NationCountCategory
static BOOL func_ov135_021fc014(WorldTradeInputWork *wk, int select) {
    int start = data_ov135_02203324[select];
    int i;

    if (data_ov135_022033f8[select].count > 0) {
        for (i = 0; i < data_ov135_022033f8[select].count; i++) {
            if (func_ov135_021fc008(wk, start + i) > 0) {
                return TRUE;
            }
        }
    } else if (func_ov135_021fc008(wk, start) > 0) {
        return TRUE;
    }
    return FALSE;
}
