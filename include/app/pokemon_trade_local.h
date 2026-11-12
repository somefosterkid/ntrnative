// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pokemon_trade_local.h, the lines in use
// so far.
#ifndef BWPORT_APP_POKEMON_TRADE_LOCAL_H
#define BWPORT_APP_POKEMON_TRADE_LOCAL_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "system/mcss.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/particle.h"
#include "spl/spl.h"
#include "app/ui/ui_scene.h"
#include "system/app_printsys_common.h"
#include "system/app_taskmenu.h"
#include "app/ui/touchbar.h"
#include "app/pokemon_trade.h"

// The Pokémon trade of overlay 119, local and over the wireless or Wi-Fi. The names are Black's dsd names; pokebw2's
// guesses are in the comments

// Cut short to the fields in use so far; the offsets are Black 2's, which Black's code shares
typedef struct PokemonTrade3DWork PokemonTrade3DWork;
typedef struct TradeFade TradeFade;
typedef struct TradeMcssMove TradeMcssMove;

struct ResSprite {
    UIObjRes res;
    ClActor *actor;
};

struct TradeMarkIcons {
    UIObjRes res;
    BOOL loaded;
    ClActor *icons[8];
};

// A cubic Bézier curve that a point follows over a number of frames: the point, the curve's four control
// the frame it is at
struct TradeCurve {
    VecFx32 pos;
    VecFx32 points[4];
    int frame;
    int frames;
};

// The copies of the save's data that a trade changes, to restore if saving fails
struct TradeBackup {
    void *mail;
    void *records;
    void *survey;
    void *pokedex;
    // The wifi list's block of the players met
    void *playersMet;
    PokeParty *party;
    void *box;
    u32 unk1C;
    BOOL chatot;
};

struct TradeBoxEntry {
    u16 species;
    u8 form;
    u8 sex;
};

typedef struct PokemonTradeWork {
    void *unk0;
    u8 unk4[0x4];
    PokemonTradeParam *param;
    // The 3D work of the demo's scenes
    PokemonTrade3DWork *work3D;
    u8 unk10[24][30];
    u8 unk2E0[6];
    u8 unk2E6[24][30];
    u8 unk5B6[6];
    // The two sides' message windows of the Pokémon's names and the ones of the Poké Balls' tiles
    BmpWin *unk5BC[2];
    BmpWin *unk5C4[2];
    BmpWin *unk5CC[2];
    BmpWin *unk5D4;
    BmpWin *unk5D8;
    PartyPkm *unk5DC[2];
    u32 unk5E4[2];
    void (*state)(struct PokemonTradeWork *wk);
    // The wait icon of the message window, and the message window's tcb manager
    WaitIcon *waitIcon;
    HeapID heapId;
    u8 unk5F6[0x16];
    TradeBackup backup;
    int unk630;
    int unk634;
    int unk638;
    int unk63C;
    u8 unk640[0x644 - 0x640];
    // The character of the message windows' frame
    u32 cursorImage;
    AppTaskMenuWin *menuWin;
    AppTaskMenu *menu;
    // The items of a task menu, and the resources of the task menus
    AppTaskMenuItem menuItems[8];
    AppTaskMenuRes *taskMenuRes;
    BmpWin *summaryWindow;
    BmpWin *unk6B8[0x19];
    BmpWin *msgWindow;
    MsgData *msgData;
    WordSet *wordSet;
    // The font and the printing stream of the messages, and the string they draw
    Font *font;
    PrintStream *printStream;
    StrBuf *drawStr;
    StrBuf *drawTemplate;
    StrBuf *strbuf;
    StrBuf *strbufTemplate;
    PrintQueue *printQueue;
    TCBExManager *tcbEx;
    u8 unk748[0x74c - 0x748];
    KeyCursor *keyCursor;
    BmpWin *unk750[0x1a];
    u8 unk7B8[0x800 - 0x7b8];
    u32 unk800;
    u8 unk804[0x810 - 0x804];
    G3DCamera *unk810;
    u8 unk814[0x818 - 0x814];
    TradeFade *fade;
    u8 unk81C[0x820 - 0x81C];
    G3DManager *sceneMgr;
    u16 scene;
    u8 unk826[0x828 - 0x826];
    BoxSaveAccessor *boxes;
    GameData *gameData;
    PlayerInfo *myInfo;
    PlayerInfo *partnerInfo;
    PokeParty *party;
    G3DCamera *camera;
    MCSSSystem *mcssSys;
    MCSS *mcss[4];
    u32 unk854;
    u32 unk858;
    TradeMcssMove *mcssMoves[4];
    u32 unk86C;
    u32 unk870;
    u32 unk874;
    u8 unk878[0x87c - 0x878];
    // Object resources (character, palette, cell) of the demo's sprites, by TRADE_OBJRES_*
    u32 objRes[17];
    u8 unk8C0[0x8c4 - 0x8c0];
    // The Poké Ball icons of the two sides' Pokémon, and their types' icons
    ResSprite ballIcons[3];
    ResSprite typeIcons[4];
    // The three icons of the Pokémon's information
    ResSprite infoIcons[3];
    TradeMarkIcons markIcons;
    TouchBar *touchBar;
    ClActUnit *clactUnit;
    TCB *unk9C8;
    u32 unk9CC[12][5];
    u8 unkABC[0xac0 - 0xabc];
    ClActor *unkAC0[12][5];
    ClActor *unkBB0[12][5];
    u16 iconSpecies[12][5];
    u8 iconForms[12][5];
    u8 iconSexes[12][5];
    u8 unkD90[12][5];
    ClActorPos unkDCC[12][5];
    u8 unkEBC[12][5];
    void *unkEF8;
    ClActor *actors[10];
    ClActor *unkF24;
    u8 unkF28[0xf2c - 0xf28];
    int timer;
    u8 unkF30[0xf34 - 0xf30];
    u32 unkF34;
    u8 unkF38[0xf54 - 0xf38];
    // The x of the touch screen's last touch, which picks the side of the summary
    int touchX;
    int unkF58;
    u32 unkF5C;
    ClActor *unkF60;
    ClActorPos unkF64;
    ClActorPos unkF68;
    ClActorPos unkF6C;
    u32 unkF70;
    int unkF74;
    int cursorRow;
    int heldSlot;
    int heldBox;
    int selectSlot;
    int selectBox;
    u8 unkF8C[0xf94 - 0xf8c];
    int unkF94;
    int unkF98;
    u32 unkF9C;
    s32 unkFA0;
    // The slots of the negotiation's Pokémon, by side and by the Pokémon's place
    int negoSlot[2][3];
    int negoBox[2][3];
    ClActor *unkFD4;
    PartyPkm *negoPkm[2][3];
    u32 unkFF0[2][3];
    ClActor *negoIcons[2][3];
    u32 negoIconChars[2][3];
    BmpWin *unk1038[4];
    int unk1048[2];
    int unk1050[2];
    u8 unk1058[0x1060 - 0x1058];
    int unk1060;
    void *unk1064[2];
    PartyPkm *unk106C;
    u8 unk1070[0x1074 - 0x1070];
    u32 unk1074;
    s16 unk1078;
    s16 unk107A;
    s16 unk107C;
    s16 unk107E;
    s16 firstColumn;
    u8 unk1082[0x1084 - 0x1082];
    int unk1084;
    u32 unk1088;
    int unk108C;
    int unk1090;
    int bgmTimer;
    // The page of the Pokémon's type icons: 0 or 1
    u16 typeIconPage;
    u16 unk109A;
    int sceneId;
    int unk10A0;
    void *savedObjPalette;
    void *savedBGPalette;
    ClActor *unk10AC[4];
    ClActor *unk10BC[4];
    ClActor *negoPanels[8];
    u32 negoPanelPltt;
    u32 negoPanelCells;
    // The bitmaps of the negotiation's messages: the two sides', four each
    GFLBitmap *negoBitmaps[8];
    u32 negoPanelChars[8];
    u16 boxPalette[16];
    u16 boxPaletteDim[16];
    u16 *unk1174[4];
    void *unk1184;
    u8 unk1188[0x118c - 0x1188];
    s32 type;
    struct TradeCurve curve;
    // The printing of the messages a character at a time
    AppPrintsysCommon printWait;
    s16 unk11DC;
    s16 unk11DE;
    u8 command[2];
    u8 unk11E2[2];
    u8 unk11E4[2];
    u8 unk11E6[2];
    u8 unk11E8[2];
    u8 unk11EA;
    u8 unk11EB;
    u8 unk11EC;
    // Frames to wait before going on, after a message
    u8 unk11ED;
    u8 unk11EE;
    u8 unk11EF;
    u8 unk11F0;
    u8 unk11F1;
    u8 unk11F2;
    u8 unk11F3;
    u8 unk11F4;
    u8 unk11F5;
    // Whether the trade shows the national Pokédex's numbers, not the regional
    u8 nationalDex;
    u8 unk11F7;
    u8 unk11F8;
    // Cleared when the trade's menus are hidden
    u8 unk11F9;
    u8 unk11FA;
    u8 unk11FB_0 : 4;
    u8 unk11FB_4 : 4;
    TradeBoxEntry boxEntries[726];
} PokemonTradeWork;

// A Pokémon's sprite moving to a place over some frames, straight, bouncing or along a path of offsets
struct TradeMcssMove {
    MCSS *mcss;
    int duration;
    int frame;
    u32 unkC;
    u8 unk10[4];
    u16 angle;
    u16 bounce;
    u8 unk18[4];
    VecFx32 start;
    VecFx32 end;
    const VecFx32 *path;
};

// pokemontrade_mcss.c
void func_ov119_021dfa60(int frame);
void func_ov119_021dfa98(int frame);
void func_ov119_021dfb4c(TradeMcssMove *move, int duration, const VecFx32 *end); // TradeMcssMove_Start
// TradeMcssMove_Create
TradeMcssMove *func_ov119_021dfb84(MCSS *mcss, int duration, const VecFx32 *end, HeapID heapId);
// TradeMcssMove_CreateWithPath
TradeMcssMove *func_ov119_021dfbb4(MCSS *mcss, int duration, const VecFx32 *end, const VecFx32 *path,
                                   HeapID heapId);
void func_ov119_021dfc68(TradeMcssMove *move, PokemonTradeWork *wk); // TradeMcssMove_Update
void func_ov119_021dfd0c(PokemonTradeWork *wk);
void func_ov119_021dfd24(TradeMcssMove *move); // TradeMcssMove_Free

// A fade of the screens' brightness over a number of frames, from one level to another
struct TradeFade {
    s8 frames;
    s8 frame;
    s8 start;
    s8 end;
};

// Black 2's, not checked against Black
typedef void (*PokemonTradeState)(PokemonTradeWork *wk);

// Black 2's; twinned code confirms the offsets of systems, systemWork, ircSpriteSizes, spriteSizes, cameraPosPath,
// cameraTargetPath, emitterPaths, emitters, cameraPosDone, cameraTargetDone, actor, resource and heapId in Black
struct PokemonTrade3DWork {
    ParticleSystem *systems[9];
    void *systemWork[9];
    // The sizes of the two sprites that the infrared trade's scene scales
    VecFx32 ircSpriteSizes[2];
    // The size of each of the four Pokémon sprites, as func_0201ab70 gives it
    VecFx32 spriteSizes[4];
    G3DCurve *cameraPosPath;
    G3DCurve *cameraTargetPath;
    // The paths that the two emitters follow
    G3DCurve *emitterPaths[2];
    SPLEmitter *emitters[2];
    // Whether the camera's paths have come to their end
    BOOL cameraPosDone;
    BOOL cameraTargetDone;
    // The sprite of the demo's scenes
    ClActor *actor;
    void *resource;
    u8 unkB8[2];
    HeapID heapId;
};

// The OBJ resources of the work's objRes: the palettes, the characters, then the cells with their animations, of the
// Pokémon icons, the lower screen's sprites (the touch bar's and the icons' cursors), the upper screen's, the common
// UI's, the trade demo's and the negotiation's icons
enum {
    TRADE_OBJRES_PLTT_ICON,
    TRADE_OBJRES_PLTT_SUB,
    TRADE_OBJRES_PLTT_MAIN,
    TRADE_OBJRES_PLTT_COMMON,
    TRADE_OBJRES_PLTT_DEMO,
    TRADE_OBJRES_PLTT_NEGO,
    TRADE_OBJRES_CHAR_SUB,
    TRADE_OBJRES_CHAR_MAIN,
    TRADE_OBJRES_CHAR_ICON0,
    TRADE_OBJRES_CHAR_ICON1,
    TRADE_OBJRES_CHAR_COMMON,
    TRADE_OBJRES_CHAR_DEMO,
    TRADE_OBJRES_CELL_ICON,
    TRADE_OBJRES_CELL_SUB,
    TRADE_OBJRES_CELL_MAIN,
    TRADE_OBJRES_CELL_COMMON,
    TRADE_OBJRES_CELL_DEMO,
    TRADE_OBJRES_CELL_NEGO,
    TRADE_OBJRES_COUNT,
};

// pokebw2's PokemonTrade_IsNegoType
BOOL func_ov119_021d8aa8(PokemonTradeWork *wk);
// pokebw2's PokemonTrade_GetPkm
// The Pokémon of a side of the trade: this machine's (0) or the other's (1)
PartyPkm *func_ov119_021d8fa4(PokemonTradeWork *wk, int side);
void func_ov119_021e3bb0(PokemonTradeWork *wk);
void func_ov119_021e3230(PokemonTradeWork *wk);
// pokebw2's PokemonTrade_IsNetwork
// Whether the trade is with another machine
BOOL func_ov119_021d8a24(PokemonTradeWork *wk);
// pokebw2's PokemonTrade_SetState
void func_ov119_021d8e84(PokemonTradeWork *wk, PokemonTradeState state);
void func_ov119_021e3034(PokemonTrade3DWork *work, int count);
void func_ov119_021e2f2c(PokemonTrade3DWork *work);
void func_ov119_021dfda0(PokemonTradeWork *wk);
void func_ov119_021e106c(PokemonTradeWork *wk);
// Set the planes of the main or the sub screen shown, in the next V-blank
void func_ov119_021e71f0(PokemonTradeWork *wk, int planes);
void func_ov119_021e64b8(PokemonTradeWork *wk);
void func_ov119_021e4888(PokemonTradeWork *wk);
void func_ov119_021e2680(PokemonTradeWork *wk);
void func_ov119_021e0ff4(PokemonTradeWork *wk, BOOL instant, BOOL bottom);
BOOL func_ov119_021e12ac(PokemonTradeWork *wk);
void func_ov119_021e1098(PokemonTradeWork *wk);
void func_ov119_021e36d8(PokemonTradeWork *wk, int side);
// pokemontrade_save.c
// pokebw2's PokemonTradeIrcDemo_Start
void func_ov117_021d4b88(PokemonTradeWork *wk);

BOOL func_ov119_021d8adc(PokemonTradeWork *wk);
void func_ov119_021e5ef8(ResSprite *sprite);
void func_ov119_021e625c(PokemonTradeWork *wk);
void func_ov119_021e5be4(PokemonTradeWork *wk);
void func_ov119_021e5998(PokemonTradeWork *wk);
void func_ov119_021e6294(PokemonTradeWork *wk, u32 palette, u32 type);
void func_ov119_021e5dfc(PokemonTradeWork *wk, int side, PartyPkm *pkm);
void func_ov119_021e5f14(PokemonTradeWork *wk, int side, BOOL summary, PartyPkm *pkm);
void func_ov119_021e6334(PokemonTradeWork *wk, int side, int index, PartyPkm *pkm, BOOL onMain, BOOL visible);
void func_ov119_021e6e7c(PokemonTradeWork *wk);
void func_ov119_021e6dec(PokemonTradeWork *wk);
BOOL func_ov119_021dd2f4(PokemonTradeWork *wk);
// pokemontrade_message.c, a descriptive name
void func_ov119_021e0ef4(PokemonTradeWork *wk, BOOL instant, u32 x, u32 y, u32 width, u32 height);
void func_ov119_021e1024(PokemonTradeWork *wk);
void func_ov119_021e1030(PokemonTradeWork *wk);
void func_ov119_021e11a8(PokemonTradeWork *wk);
void func_ov119_021e12f8(PokemonTradeWork *wk);
void func_ov119_021e131c(PokemonTradeWork *wk, const u32 *items, int count, u32 right, u32 bottom);
void func_ov119_021e1410(PokemonTradeWork *wk, const u32 *items, int count);
void func_ov119_021e1b14(PokemonTradeWork *wk, int side, PartyPkm *pkm);
void func_ov119_021e1ca8(PokemonTradeWork *wk);
void func_ov119_021e1ce8(PokemonTradeWork *wk, BOOL hideWindows);
void func_ov119_021e1d68(PokemonTradeWork *wk, PartyPkm *pkm);
void func_ov119_021e1e00(PokemonTradeWork *wk, int page, PartyPkm *pkm);
void func_ov119_021e219c(PokemonTradeWork *wk, PartyPkm *pkm, int side, BOOL reload);
void func_ov119_021e2438(PokemonTradeWork *wk, int side);
void func_ov119_021e2484(PokemonTradeWork *wk, BOOL clear);
void func_ov119_021e26ac(PokemonTradeWork *wk);
void func_ov119_021e272c(PokemonTradeWork *wk, int side, PartyPkm *pkm);
void func_ov119_021e293c(PokemonTradeWork *wk, int side);
AppTaskMenuWin *func_ov119_021e2984(PokemonTradeWork *wk, u32 msg);
void func_ov119_021e2a1c(PokemonTradeWork *wk, BOOL showActor, BOOL showButton);
BOOL func_ov119_021e2a80(PokemonTradeWork *wk);

#define TRADE_NET_CMD_BASE 0xc00
#define TRADE_NET_CMD_SELECT (TRADE_NET_CMD_BASE + 0)
#define TRADE_NET_CMD_UNK1 (TRADE_NET_CMD_BASE + 1)
#define TRADE_NET_CMD_UNK3 (TRADE_NET_CMD_BASE + 3)
#define TRADE_NET_CMD_UNK6 (TRADE_NET_CMD_BASE + 6)

// Black 2's, not checked against Black
#define TRADE_NET_CMD_UNKC (TRADE_NET_CMD_BASE + 0xc)

#define TRADE_NET_CMD_UNKE (TRADE_NET_CMD_BASE + 0xe)
#define TRADE_NET_CMD_UNKF (TRADE_NET_CMD_BASE + 0xf)
#define TRADE_NET_CMD_UNK10 (TRADE_NET_CMD_BASE + 0x10)
#define TRADE_NET_CMD_UNK13 (TRADE_NET_CMD_BASE + 0x13)
#define TRADE_NET_CMD_UNK16 (TRADE_NET_CMD_BASE + 0x16)
#define TRADE_NET_CMD_UNK17 (TRADE_NET_CMD_BASE + 0x17)

void func_ov119_021e66e8(PokemonTradeWork *wk);
// Greys an icon out or not
void func_ov119_021e4ab8(PokemonTradeWork *wk, ClActor *icon, BOOL grey);
void func_ov119_021e64dc(PokemonTradeWork *wk, int column, int row, int x, int y, BoxPkm *pkm);
void func_ov119_021df918(PokemonTradeWork *wk, int side, PartyPkm *pkm);void func_ov119_021dd4f4(PokemonTradeWork *wk, int side, int index);
void func_ov119_021e62d4(PokemonTradeWork *wk, int side, int index);
void func_ov119_021df8a0(PokemonTradeWork *wk, u8 index, int side);
void func_ov119_021e36a8(PokemonTradeWork *wk, int side, BOOL front, PartyPkm *pkm, BOOL mirror, BOOL a5);
void func_ov119_021e5b6c(PokemonTradeWork *wk, int side, BOOL level);
void func_ov119_021dd294(PokemonTradeWork *wk, u32 a1, u32 a2);
// The Pokémon icon at a point of the screen. If asked, the box and slot of its Pokémon, NULL if the icon shows none,
// and the column of the strip, the row and the icon column it is in
ClActor *func_ov119_021e51a4(PokemonTradeWork *wk, int x, int y, int *box, int *slot, int *column, int *row,
                             int *index);
void func_ov119_021df708(PokemonTradeWork *wk);
// Sets up the strip's icons for its scroll, loading their characters in the V-blank if asked
void func_ov119_021e4e64(BoxSaveAccessor *boxes, PokemonTradeWork *wk, BOOL async);
BOOL func_ov119_021dd5b4(PokemonTradeWork *wk, int slot, int box);
int func_ov119_021dd3d4(PokemonTradeWork *wk, int side, int slot, int box);
void func_ov119_021df7ac(PokemonTradeWork *wk, BOOL a1);
BOOL func_ov119_021dd2a8(PokemonTradeWork *wk);
void func_ov119_021e4174(PokemonTradeWork *wk);
void func_ov119_021dd1b8(PokemonTradeWork *wk);
void func_ov119_021dd528(PokemonTradeWork *wk, u32 vram);
void func_ov119_021e4420(PokemonTradeWork *wk);
void func_ov119_021e6700(PokemonTradeWork *wk);
void func_ov119_021e31fc(PokemonTradeWork *wk);
// Works out the colours of this machine's box, one a frame, and the party's after the last box; TRUE when that is
// done
BOOL func_ov119_021e3e00(PokemonTradeWork *wk, int box);
// Copies the characters of the icons of a box's Pokémon, or of the party's after the last box, one box a call
void func_ov119_021e5098(PokemonTradeWork *wk, int box);
int func_ov119_021e4dbc(PokemonTradeWork *wk);
// Whether a species' name starts with an initial, from 0 for A
BOOL func_ov119_021e4aa4(u32 species, u32 initial);
void func_ov119_021e5d84(PokemonTradeWork *wk);
void func_ov119_021e71e0(PokemonTradeWork *wk, BOOL dim);
void func_ov119_021e7058(PokemonTradeWork *wk);
BOOL func_ov119_021e707c(PokemonTradeWork *wk);
void func_ov119_021e6fd4(PokemonTradeWork *wk, u8 x, u8 y);
// Whether the cursor's column is on screen, and the third column on screen, where the cursor goes if not
BOOL func_ov119_021e4e0c(PokemonTradeWork *wk, int *column);
void func_ov119_021e42b4(PokemonTradeWork *wk);
void func_ov119_021e4598(PokemonTradeWork *wk);
// The index of a Pokémon among the ones this player offers, or -1
int func_ov119_021dd34c(PokemonTradeWork *wk, int box, int slot);
BOOL func_ov119_021e665c(PokemonTradeWork *wk);
void func_ov119_021df7f0(PokemonTradeWork *wk);
void func_ov119_021e4a1c(PokemonTradeWork *wk);
void func_ov119_021e6544(PokemonTradeWork *wk);
void func_ov119_021e55bc(PokemonTradeWork *wk);
void func_ov119_021e53f8(PokemonTradeWork *wk);
void func_ov119_021e5430(PokemonTradeWork *wk);
void func_ov119_021e5680(PokemonTradeWork *wk);
void func_ov119_021e4068(PokemonTradeWork *wk);
void func_ov119_021dd588(PokemonTradeWork *wk);
void func_ov119_021df8e0(PokemonTradeWork *wk);
void func_ov119_021dd9dc(PokemonTradeWork *wk);
void func_ov119_021e4570(PokemonTradeWork *wk);
void func_ov119_021e69a4(PokemonTradeWork *wk);
void func_ov119_021df7d0(PokemonTradeWork *wk);
void func_ov119_021e48e0(PokemonTradeWork *wk);
void func_ov119_021dd174(PokemonTradeWork *wk);
void func_ov119_021e3f08(PokemonTradeWork *wk);
void func_ov119_021e4000(PokemonTradeWork *wk);
void func_ov119_021e57a4(PokemonTradeWork *wk);
void func_ov119_021e30ec(PokemonTradeWork *wk);
void func_ov119_021e0b34(PokemonTradeWork *wk);
void func_ov119_021e3fe4(PokemonTradeWork *wk);
// pokemontrade_2d.c
void func_ov119_021e3c20(PokemonTradeWork *wk);
void func_ov119_021df868(PokemonTradeWork *wk);
void func_ov119_021df978(PokemonTradeWork *wk);
void func_ov119_021e31bc(PokemonTradeWork *wk);
// pokemontrade_proc.c
void func_ov119_021d8980(PokemonTradeWork *wk);
void func_ov119_021d89cc(PokemonTradeWork *wk);
BOOL func_ov119_021d89ec(const u8 *data);
u8 func_ov119_021d8a18(PokemonTradeWork *wk);
// pokebw2's PokemonTrade_GetColumnBox
int func_ov119_021d8af4(int a, PokemonTradeWork *wk);
// pokebw2's PokemonTrade_GetColumnSlot
int func_ov119_021d8b0c(int a, int b);
// pokebw2's PokemonTrade_CopyPartyPkm
PartyPkm *func_ov119_021d8d4c(BoxSaveAccessor *boxes, int box, int slot, PokemonTradeWork *wk);
// pokebw2's PokemonTrade_GetBoxPkm
BoxPkm *func_ov119_021d8dd0(BoxSaveAccessor *boxes, int box, int slot, PokemonTradeWork *wk);
// pokebw2's PokemonTrade_GetBoxEntry
TradeBoxEntry *func_ov119_021d8e24(int box, int slot, PokemonTradeWork *wk);
u32 func_ov119_021d8f20(PokemonTradeWork *wk, int side);
void func_ov119_021d8f60(PokemonTradeWork *wk, int side, u32 value);
// pokebw2's PokemonTrade_FadeOutToEnd
void func_ov119_021d9434(PokemonTradeWork *wk);
void func_ov119_021d9468(PokemonTradeWork *wk, int side, PartyPkm *pkm);
BOOL func_ov119_021d94d4(PokemonTradeWork *wk);
BOOL func_ov119_021d95d0(PokemonTradeWork *wk);
BOOL func_ov119_021d95f0(PokemonTradeWork *wk);
BOOL func_ov119_021d9610(PokemonTradeWork *wk);
void func_ov119_021da76c(PokemonTradeWork *wk);
void func_ov119_021dace4(PokemonTradeWork *wk);
void func_ov119_021daef0(PokemonTradeWork *wk);
void func_ov119_021dbbd0(PokemonTradeWork *wk);


// The colour of each slot of a machine's boxes and party, as indices into the box palette, which
// pokemontrade_3d.c draws into the textures of the boxes' cubes
struct TradeBoxColors {
    u8 boxes[24][30];
    u8 party[6];
};

// The first of each kind of resource in objRes
#define TRADE_OBJRES_PLTT TRADE_OBJRES_PLTT_ICON

#define TRADE_OBJRES_CHAR TRADE_OBJRES_CHAR_SUB
#define TRADE_OBJRES_CELL TRADE_OBJRES_CELL_ICON

BOOL func_ov119_021df614(PokemonTradeWork *wk, int column, int row);
BOOL func_ov119_021df6b4(PokemonTradeWork *wk, int column, int row);
void func_ov119_021e53cc(PokemonTradeWork *wk);
// Sets the display up for the trade, or for the trade demo
void func_ov119_021e57e8(int config);
void func_ov119_021e5868(PokemonTradeWork *wk);
void func_ov119_021e6308(PokemonTradeWork *wk, int side, BOOL visible);
void func_ov119_021e6440(PokemonTradeWork *wk, int side, int index);
void func_ov119_021e6718(PokemonTradeWork *wk, int index);
void func_ov119_021e6790(PokemonTradeWork *wk, int index, BOOL visible);
void func_ov119_021e67a8(PokemonTradeWork *wk, int index);
void func_ov119_021e67c4(PokemonTradeWork *wk, u32 index);
void func_ov119_021e67e0(PokemonTradeWork *wk, u32 index, int side);
void func_ov119_021e68b4(PokemonTradeWork *wk, int side);
void func_ov119_021e68f4(PokemonTradeWork *wk, int index);
BOOL func_ov119_021e69c0(PokemonTradeWork *wk);

#define TRADE_NET_CMD_OFFER (TRADE_NET_CMD_BASE + 7)

// Black 2's, not checked against Black
#define TRADE_NET_CMD_UNKA (TRADE_NET_CMD_BASE + 0xa)
#define TRADE_NET_CMD_WITHDRAW (TRADE_NET_CMD_BASE + 0xb)
#define TRADE_NET_CMD_UNKD (TRADE_NET_CMD_BASE + 0xd)
#define TRADE_NET_CMD_CHECK_RESULT (TRADE_NET_CMD_BASE + 0x11)

// Black 2's, not checked against Black
#define TRADE_NET_CMD_UNK12 (TRADE_NET_CMD_BASE + 0x12)

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov119_021e7208(PokemonTradeWork *wk, int planes);
void func_ov119_021e3f70(PokemonTradeWork *wk);
void func_ov119_021e5958(PokemonTradeWork *wk);
void func_ov119_021e3f30(PokemonTradeWork *wk);
void func_ov119_021e40ec(PokemonTradeWork *wk, int side, int page);
void func_ov119_021e60bc(PokemonTradeWork *wk, PartyPkm *pkm, BOOL isEgg);
void func_ov119_021e6cb8(PokemonTradeWork *wk);
void func_ov119_021e10d8(PokemonTradeWork *wk);
void func_ov119_021e24e8(PokemonTradeWork *wk, u32 a1);
void func_ov119_021dd380(PokemonTradeWork *wk, int index, PartyPkm *pkm);
void func_ov119_021e3e80(PokemonTradeWork *wk);
BOOL func_ov119_021dd24c(PokemonTradeWork *wk);
// pokemontrade_nego.c
// Whether a Pokémon can't be offered in a negotiation
BOOL func_ov119_021dd0cc(PokemonTradeWork *wk, BoxPkm *pkm);
void func_ov119_021df5d8(PokemonTradeWork *wk);
void func_ov119_021df904(PokemonTradeWork *wk);
void func_ov119_021dd68c(PokemonTradeWork *wk);
void func_ov119_021df78c(PokemonTradeWork *wk);
void func_ov119_021e5c64(PokemonTradeWork *wk);
void func_ov119_021e58a0(PokemonTradeWork *wk);
void func_ov119_021e467c(PokemonTradeWork *wk);
void func_ov119_021e3f74(PokemonTradeWork *wk);
void func_ov119_021dd90c(PokemonTradeWork *wk, int side, u32 msg, BOOL force);
void func_ov119_021e31b4(PokemonTradeWork *wk);
void func_ov119_021e3e60(PokemonTradeWork *wk);
void func_ov119_021e57fc(PokemonTradeWork *wk);
void func_ov119_021e6b90(PokemonTradeWork *wk);
void func_ov119_021e6f0c(PokemonTradeWork *wk, int side, PartyPkm *pkm);
void func_ov119_021e2efc(PokemonTrade3DWork *work);
void func_ov119_021e3070(PokemonTrade3DWork *work);

// Black 2's, not checked against Black

// pokebw2's PokemonTrade_IsNegoType
// pokebw2's PokemonTrade_GetPkm
// The Pokémon of a side of the trade: this machine's (0) or the other's (1)
// pokebw2's PokemonTrade_SetState
// pokemontrade_save.c
// Set the planes of the main or the sub screen shown, in the next V-blank
void func_ov119_021dc720(PokemonTradeWork *wk);
// Create the cell actor system for the trade demo, which has few sprites, and for the trade
void func_ov119_021e3e40(PokemonTradeWork *wk);
void func_ov119_021e3208(PokemonTradeWork *wk, int sceneId);
void func_ov119_021e2f34(PokemonTrade3DWork *work);
void func_ov119_021e3004(PokemonTrade3DWork *work);
void func_ov119_021e2fb0(PokemonTrade3DWork *work);
void func_ov119_021e36c8(PokemonTradeWork *wk, int side, BOOL front, PartyPkm *pkm, BOOL flag);
// pokebw2's PokemonTradeDemoDown_Start
void func_ov118_021d59b0(PokemonTradeWork *wk);

// pokebw2's PokemonTrade_IsNetwork
// Whether the trade is with another machine
// pokebw2's PokemonTradeDemoCommon_Start
void func_ov118_021d4ab4(PokemonTradeWork *wk);

// Adds the sprite of a Pokémon on a side, facing front or back, mirrored if asked and the species allows it
void func_ov119_021e35a0(PokemonTradeWork *wk, int side, BOOL front, PartyPkm *pkm, BOOL mirror);
// pokebw2's PokemonTradeDemoMiddle_Start
void func_ov118_021d6f2c(PokemonTradeWork *wk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Sets up the trade demo's BGs 6 and 7, with the graphics of the trade or of the other demo
void func_ov119_021e5284(PokemonTradeWork *wk, BOOL other);
// pokemontrade_3d.c
void func_ov119_021e2d70(PokemonTrade3DWork *work, int path);

#endif // BWPORT_APP_POKEMON_TRADE_LOCAL_H
