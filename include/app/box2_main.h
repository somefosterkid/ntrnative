// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/box2_main.h, the lines in use so far.
#ifndef BWPORT_APP_BOX2_MAIN_H
#define BWPORT_APP_BOX2_MAIN_H

#include "types.h"
#include "app/box2.h"
#include "struct_decls.h"
#include "gfl/clact.h"
#include "system/app_taskmenu.h"
#include "system/printsys.h"
#include "app/name_entry.h"

// The PC box's work. Names are pokebw2's

// The number of boxes once all are open
#define BOX2_TRAY_MAX 24
// No tray or position
#define BOX2_GET_NONE 0xff

// 0x30 bytes: Black 2's adds the Pokémon search's criteria at the end
struct Box2SysWork {
    Box2Param *param;
    GameProcManager *procManager;
    BOOL procMgrResult;
    void *subProcWork;
    u16 subRet;
    u8 subProcType;
    u8 unk13;
    u8 tray;
    u8 trayMax;
    u8 pos;
    // The tray of the held Pokémon, or BOX2_GET_NONE
    u8 getTray;
    u8 unk18;
    u8 trayScroll;
    u8 unk1A;
    u8 unk1B;
    u8 moveMode : 4;
    u8 unk1C_4 : 4;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u32 curRcvPos;
    int nextSeq;
    Box2AppWork *app;
};

// Black 2's; twinned code confirms the offsets of oam in Black
// A text object on OAM
struct Box2FontOam {
    void *oam;
    GFLBitmap *bitmap;
};

// Black 2's, not checked against Black
// What runs once per frame until it returns FALSE
typedef BOOL (*Box2VFunc)(Box2SysWork *syswk);

// Black 2's, not checked against Black
struct Box2IrqWork {
    Box2VFunc func;
    Box2VFunc freq;
    void *work;
    u16 seq;
    u16 cnt;
};

// Black 2's, not checked against Black
// A button's press animation, on an actor or on the BG
struct Box2ButtonAnm {
    u8 mode : 1;
    u8 id : 7;
    u8 pal1 : 4;
    u8 pal2 : 4;
    u8 seq;
    u8 cnt;
    u8 px;
    u8 py;
    u8 sx;
    u8 sy;
};

// Black 2's, not checked against Black
struct Box2RangeSelect {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    u16 anmMax;
    u16 anmCnt;
    u8 anm;
    u8 prevAnm;
    u8 startPos;
    u8 endPos;
};

// Black 2's up to the windows, which sit at 0x94 here (0xdc in Black 2); the rest of the Black 2's layout follows
// them 0x48 lower, which itemId at 0xa16c confirms
struct Box2AppWork {
    void * bmpOam;
    Box2FontOam fontOam[9];
    u32 keyRepeatWait;
    u32 keyRepeatStart;
    TCB * vtask;
    struct Box2IrqWork vfunk;
    u32 unk68;
    PaletteFade * palFade;
    CursorMove * cursorMove;
    BGWinFrame * bgWinFrame;
    Font * font;
    Font * smallFont;
    MsgData * msgData;
    WordSet * wordSet;
    StrBuf * expandBuf;
    PrintQueue * printQueue;
    u8 unk90[4];
    PrintWindow windows[28];
    u8 flushChar[4];
    u32 cursorChars;
    BOOL nationalDex;
    u16 * regionalDex;
    AppTaskMenuItem yesNoItems[2];
    AppTaskMenuRes * yesNoRes;
    AppTaskMenu * yesNoMenu;
    u16 ynID;
    struct Box2ButtonAnm bawk;
    ArcTool * pokeIconArc;
    u8 pokeIconChar[30][512];
    u8 unk3DB4[0x1E];
    u8 pokeIconId[0x25];
    u8 unk3DF7;
    u32 unk3DF8[(0x3E70 - 0x3DF8) / 4];
    u8 trayIconChar[3][0x400];
    u8 unk4A70[0x9E70 - 0x4A70];
    ClActUnit * unk9E70;
    ClActor * unk9E74[96];
    u32 unk9FF4[0x45];
    u32 unkA108[11];
    u32 unkA134[12];
    u16 unkA164;
    u8 unkA166[2];
    u32 curPos;
    // The item that the menu's message names
    u16 itemId;
    u16 unkA16E;
    u8 unkA170[0x2];
    u8 unkA172;
    // Why the last move was refused, 0 for none
    u8 moveErr;
    u32 unkA174;
    s8 wallPx;
    u8 wallArea;
    u8 unkA17A[2];
    u8 unkA17C;
    u8 unkA17D;
    u8 pokePutKey;
    u8 unkA17F;
    // The touch point, which the box's touch areas read
    u32 tpx;
    u32 tpy;
    u32 oldCurPos;
    u32 unkA18C;
    void * unkA190;
    u32 unkA194;
    u32 unkA198;
};

// Positions: a tray's 30 Pokémon, then the party's 6, then the boxes of the box list
#define BOX2_TRAY_POKE_MAX 30

// Black 2's, not checked against Black
// An item of a menu: its message, and 1 for the item that closes the menu
struct Box2MenuItem {
    u16 msgId;
    u16 type;
};

// Black 2's, not checked against Black
// What the upper screen shows of a Pokémon
struct Box2PokeInfo {
    BoxPkm *pkm;
    u16 species;
    u16 item;
    u32 pid;
    u8 type1;
    u8 type2;
    u8 ability;
    u8 nature;
    u16 mark;
    u8 level : 7;
    u8 egg : 1;
    u8 sex : 4;
    // 0 for Nidoran and eggs, whose sex isn't shown
    u8 sexPut : 1;
    u8 rare : 1;
    // 1 while infected, 2 once cured
    u8 pokerus : 2;
    u16 waza[4];
};

// pokebw2's Box2Main_GetBoxPkm
// The Pokémon at a position of a tray, or of the party when tray is BOX2_GET_NONE
BoxPkm *func_ov170_021dd2d0(Box2SysWork *syswk, u32 tray, u32 pos);

#define BOX2_PARTY_POS 30
#define BOX2_BOXLIST_POS 36

// The actors of the cursor and the item icon
#define BOX2_ACTOR_CURSOR 4

// Black 2's, not checked against Black
#define BOX2_ACTOR_ITEM_ICON 10 // Black's

// Black 2's, not checked against Black
// The Pokémon search's criteria, which the boxes' icons are filtered by
struct Box2SearchParam {
    u16 species;
    // 1 when the Pokémon must hold no item, 2 when it must hold one
    u8 item;
    u8 marks;
    // The nature and the sex, plus 1, or 0 for any
    u8 nature;
    u8 ability;
    u8 sex;
    u8 active;
};

// Black 2's; twinned code confirms the offsets of cap, scaleCnt and scale in Black
// Releasing a Pokémon
struct Box2PokeFreeWork {
    ClActor *cap;
    // The next box slot whose Pokémon's moves to check
    u16 checkCnt;
    u8 checkFlag;
    u8 scaleCnt;
    f32 scale;
};

// pokebw2's Box2Main_GetPokeParam
u32 func_ov170_021dd25c(Box2SysWork *syswk, u16 pos, u16 tray, u32 param, void *buf);
// pokebw2's Box2Main_IsSpeciesFlagged
BOOL func_ov170_021dd908(Box2SysWork *syswk, u32 pos);
// pokebw2's Box2Main_GetTrayScroll
u8 func_ov170_021deb14(Box2SysWork *syswk, s8 mv);

// Black 2's, not checked against Black
// The directions of a box change
#define BOX2_TRAY_SCROLL_L 0

// Black 2's, not checked against Black
#define BOX2_TRAY_SCROLL_R 1
#define BOX2_TRAY_SCROLL_NONE 2
// The frames the item icon takes to move
#define BOX2_ITEMMOVE_CNT 8

// Black 2's, not checked against Black
// The frames a Pokémon icon takes to move
#define BOX2_POKEMOVE_CNT 8

// Black 2's, not checked against Black
// Why a Pokémon can't be put where it was dropped
#define BOX2_MOVE_ERR_NONE 0

// Black 2's, not checked against Black
#define BOX2_MOVE_ERR_BOX_FULL 1
#define BOX2_MOVE_ERR_MAIL 2
#define BOX2_MOVE_ERR_LAST_BATTLER 3
#define BOX2_BTN_ANM_MODE_OBJ 0
#define BOX2_BTN_ANM_MODE_BG 1

// Black 2's; twinned code confirms the offsets of iconPos, mvPos and dfPos in Black
// A Pokémon icon that moves to a new position
struct Box2PokeMoveData {
    // The icon's index into pokeIconId
    u16 iconPos;
    u16 mvPos;
    u16 dfPos;
    // 0 when the entry is unused
    u16 flag;
    u32 mx;
    u32 my;
    s16 vx;
    s16 vy;
    s16 dx;
    s16 dy;
};

// Black 2's, not checked against Black
// The Pokémon that move at once, and where the move started and ends
struct Box2PokeMoveWork {
    Box2PokeMoveData data[7];
    u32 cnt;
    u32 getPos;
    u32 putPos;
    // Where the Pokémon was dropped
    u32 setPos;
};

// An area of the lower screen, with its right edge in it
struct Box2Area {
    u8 left;
    u8 right;
    u8 top;
    u8 bottom;
};

// Black 2's, not checked against Black
// The work of the name entry for a box's name
struct Box2NameInWork {
    NameEntryParam *param;
    StrBuf *name;
};

// Black 2's, not checked against Black
// The item icon's move
struct Box2ItemMoveWork {
    u16 putPos;
    u16 setPos;
    u32 mvMode;
    u32 nowX;
    u32 nowY;
    u32 mx;
    u32 my;
    u32 mvX : 1;
    u32 mvY : 1;
    u32 cnt : 30;
};

// Black 2's, not checked against Black
// The cursor's move to a position
struct Box2CursorMoveWork {
    u8 px;
    u8 py;
    u8 vx;
    u8 vy;
    u32 mx : 1;
    u32 my : 1;
    u32 cnt : 30;
};

BOOL func_ov170_021dd930(Box2SysWork *syswk);
BOOL func_ov170_021de158(Box2SysWork *syswk, u32 pos, u32 putPos);
void func_ov170_021de66c(Box2SysWork *syswk, Box2PokeMoveData *work);
u32 func_ov170_021df228(s16 x, s16 y, const Box2Area *areas);
void func_ov170_021de530(Box2SysWork *syswk, u32 pos, u32 putPos);
u16 func_ov170_021de5c8(Box2SysWork *syswk, u16 iconPos);
extern const Box2Area data_ov170_021ef79c[6];
void func_ov170_021dea4c(Box2SysWork *syswk, u32 putPos, u32 mvMode);
BOOL func_ov170_021deb08(Box2SysWork *syswk);
// pokebw2's Box2Main_InitBg
void func_ov170_021dc9e0(Box2SysWork *syswk);
// pokebw2's Box2Main_SetBlendAlpha
void func_ov170_021dcdf0(BOOL enabled);
// pokebw2's Box2Main_ScrollTray
// Shows the previous or next box
void func_ov170_021dd16c(Box2SysWork *syswk, BOOL right);
void func_ov170_021dd1c4(Box2SysWork *syswk, u32 a1);
// pokebw2's Box2Main_IsTrayScrollRight
BOOL func_ov170_021dd210(Box2SysWork *syswk, u32 from, u32 to);
// pokebw2's Box2Main_SetPokeParam
void func_ov170_021dd2b4(Box2SysWork *syswk, u32 pos, u32 tray, u32 param, u32 value);
// pokebw2's Box2Main_ClearPokeData
void func_ov170_021dd314(Box2SysWork *syswk, u32 tray, u32 pos);
// pokebw2's Box2Main_GetScrolledTray
// The box that a tray icon of the box list shows
u32 func_ov170_021dd684(Box2SysWork *syswk, u32 tray);
// pokebw2's Box2Main_PokeItemFormChange
BOOL func_ov170_021dd718(Box2SysWork *syswk, BoxPkm *pkm);
// pokebw2's Box2Main_RecalcPartyStats
// Recalculates the stats of the party Pokémon at pos, after a forme change
void func_ov170_021dd8a8(Box2SysWork *syswk, u32 pos);
// pokebw2's Box2Main_PokeItemMoveCheck
BOOL func_ov170_021dde80(Box2SysWork *syswk, u32 getPos, u32 putPos);
// pokebw2's Box2Main_UpdateChatter
void func_ov170_021de898(Box2SysWork *syswk);
// pokebw2's Box2Main_PokeInfoPutCore
BOOL func_ov170_021def98(Box2SysWork *syswk, u32 tray, u32 pos);
// pokebw2's Box2Main_PokeInfoPut
BOOL func_ov170_021defec(Box2SysWork *syswk, u32 pos);
// pokebw2's Box2Main_PokeSelectOff
void func_ov170_021df0bc(Box2SysWork *syswk);
// pokebw2's Box2Main_PokeStatusExit
int func_ov170_021df324(Box2SysWork *syswk);

// Black 2's, not checked against Black
// The scroll of the box list by touch
struct Box2BoxListDrag {
    s16 cnt;
    s16 dir;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Box2Main_GetVramBanks
const BGSysVRAMConfig *func_ov170_021dc9d8(void);
// pokebw2's Box2Main_GetWallPaperNumber
// The wallpaper of a box
u32 func_ov170_021dedcc(Box2SysWork *syswk, u32 tray);
// pokebw2's Box2Main_InitVBlank
void func_ov170_021dc94c(Box2SysWork *syswk);
// pokebw2's Box2Main_ExitVBlank
void func_ov170_021dc964(Box2SysWork *syswk);
// pokebw2's Box2Main_VFuncSet
// Sets the function that runs each frame
void func_ov170_021dc9a0(Box2AppWork *app, Box2VFunc func);
// pokebw2's Box2Main_VFuncReq
// Sets the function that Box2Main_VFuncReqSet makes the next to run
void func_ov170_021dc9b4(Box2AppWork *app, Box2VFunc func);
// pokebw2's Box2Main_VFuncReqSet
void func_ov170_021dc9b8(Box2AppWork *app);
// pokebw2's Box2Main_InitVramBanks
void func_ov170_021dc9c4(void);
// pokebw2's Box2Main_ExitBg
void func_ov170_021dcb48(Box2SysWork *syswk);
// pokebw2's Box2Main_LoadBgGraphics
void func_ov170_021dcb8c(Box2SysWork *syswk);
// pokebw2's Box2Main_InitPaletteFade
void func_ov170_021dcdb8(Box2SysWork *syswk);
// pokebw2's Box2Main_ExitPaletteFade
void func_ov170_021dcdd8(Box2SysWork *syswk);
void func_ov170_021dd1e8(Box2SysWork *syswk);
// pokebw2's Box2Main_ShowCursor
// Shows the cursor and leaves touch mode
void func_ov170_021dd238(Box2SysWork *syswk);
// pokebw2's Box2Main_BattlePokeCheck
BOOL func_ov170_021dd694(Box2SysWork *syswk, u32 pos);
// pokebw2's Box2Main_InitDexData
void func_ov170_021dd8d0(Box2SysWork *syswk);
// pokebw2's Box2Main_ExitDexData
void func_ov170_021dd8f8(Box2SysWork *syswk);
// pokebw2's Box2Main_WallPaperSet
// Shows a wallpaper, scrolling from the direction dir, a BOX2_TRAY_SCROLL_*
void func_ov170_021dec98(Box2SysWork *syswk, u32 wallpaper, u32 dir);
// pokebw2's Box2Main_WallPaperChange
void func_ov170_021ded58(Box2SysWork *syswk, u32 wallpaper);
// pokebw2's Box2Main_PokeInfoRewrite
void func_ov170_021deff8(Box2SysWork *syswk, u32 pos);
// pokebw2's Box2Main_PokeInfoOff
void func_ov170_021df040(Box2SysWork *syswk);
// pokebw2's Box2Main_MarkingPutMain
void func_ov170_021df110(Box2SysWork *syswk, u32 mark);
// pokebw2's Box2Main_MarkingPutSub
void func_ov170_021df11c(Box2SysWork *syswk, u32 mark);
// pokebw2's Box2Main_PokeStatusCall
// The sub procs: the summary, the bag, the name entry for a box's name and the search
int func_ov170_021df288(Box2SysWork *syswk);
// pokebw2's Box2Main_ButtonAnmMain
// Runs the press animation of a button; FALSE once it is done
BOOL func_ov170_021dcfc0(Box2SysWork *syswk);
// pokebw2's Box2Main_VFuncFrameMove
BOOL func_ov170_021e0190(Box2SysWork *syswk);
// pokebw2's Box2Main_VFuncTrayScrollLeft and _VFuncTrayScrollRight
// Scroll the trays a step; FALSE once they are done
BOOL func_ov170_021e00c4(Box2SysWork *syswk);
BOOL func_ov170_021e0128(Box2SysWork *syswk);
// pokebw2's Box2Main_VFuncMarkingFrameMove
BOOL func_ov170_021e01a8(Box2SysWork *syswk);
// pokebw2's Box2Main_VFuncBoxListScrollLeft and _VFuncBoxListScrollRight
// Scroll the box list a step; FALSE once it is done
BOOL func_ov170_021e19d4(Box2SysWork *syswk);
BOOL func_ov170_021e1a28(Box2SysWork *syswk);
void func_ov170_021dd14c(Box2SysWork *syswk);
// pokebw2's Box2Main_OpenYesNo
void func_ov170_021dcf58(Box2SysWork *syswk, u32 pos);
// pokebw2's Box2Main_PokeFreeCreate
void func_ov170_021de728(Box2SysWork *syswk);
// pokebw2's Box2Main_SetFrameButtonAnm
// Starts the press animation of a frame's button
void func_ov170_021dd090(Box2SysWork *syswk, u32 frame);
BOOL func_ov170_021e01c4(Box2SysWork *syswk);
BOOL func_ov170_021e01e0(Box2SysWork *syswk);

// pokebw2's Box2Main_VFuncCursorMove
BOOL func_ov170_021e066c(Box2SysWork *syswk);

void func_ov170_021ddefc(Box2SysWork *syswk, Box2PokeMoveData *move);
void func_ov170_021dd530(Box2SysWork *syswk);
void func_ov170_021de7b4(Box2SysWork *syswk);
BOOL func_ov170_021de7c8(Box2SysWork *syswk);
void func_ov170_021dcf30(Box2SysWork *syswk);
void func_ov170_021dce80(Box2SysWork *syswk);
void func_ov170_021dce14(Box2SysWork *syswk);
void func_ov170_021dcec0(Box2SysWork *syswk);
u32 func_ov170_021df1dc(s16 x, s16 y);
void func_ov170_021de980(Box2SysWork *syswk, u32 mvMode);
void func_ov170_021dea3c(Box2SysWork *syswk, u32 setPos, u32 pos, u32 type);
BOOL func_ov170_021deafc(Box2SysWork *syswk);
extern const Box2Area data_ov170_021ef7b4[6];
u32 func_ov170_021df254(Box2SysWork *syswk, s16 x, s16 y);
void func_ov170_021de5f8(Box2SysWork *syswk, u32 pos);

#endif // BWPORT_APP_BOX2_MAIN_H
