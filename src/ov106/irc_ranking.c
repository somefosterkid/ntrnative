// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov180/irc_ranking.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/irc_appbar.h"
#include "app/irc_compatible.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/gx.h"
#include "nitro/math.h"
#include "nnsys/g2d.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/trainer_log.h"
#include "system/bmp_oam.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/btl_main_5.h"
#include "pml/poke_party.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
BmpOamSys *func_02025538(HeapID heapId, ClActUnit *unit);
IrcAppBar *func_ov101_021b70dc(u32 unused0, ClActUnit *unit, u8 bg, u8 palette, u8 objPalette, u32 unused5, Font *font, PrintQueue *queue, HeapID heapId);
int func_ov101_021b7250(IrcAppBar *bar);
BOOL func_ov101_021b727c(IrcAppBar *bar);

// The ranking of the compatibility checker: its graphics, the list of the trainers of the trainer log with their rows
// and the scrolling of the list, the stylus and the keys that scroll it, the fling it goes on with, the labels over
// the list and the steps of the screen. Names are ours

#define HEAP_ID 0x43
#define BG_COUNT 8
#define ROW_MAX TRAINER_LOG_MAX
#define ROW_HEIGHT 24
#define LABEL_COUNT 4
#define SCREEN_ROWS 3
#define SCREEN_COLUMNS 32

// The area of the list: how far it is scrolled when its first rows are at the top, and how many rows show at once
#define LIST_TOP_OFFSET (-40)
#define LIST_BOTTOM_OFFSET 24
#define LIST_VISIBLE_HEIGHT 288

// The frames that a key is held before the scroll it makes is at full speed, and the frames it takes to get there
#define KEY_SCROLL_START_FRAMES 20
#define KEY_SCROLL_FRAMES 40
#define KEY_SCROLL_FRAMES_FLING 12

// How the label of a message is placed on its bitmap
enum {
    LABEL_ALIGN_NONE,
    LABEL_ALIGN_CENTER,
    LABEL_ALIGN_CENTER_Y,
};

// The color of text, shadow and background of the labels
#define LABEL_COLOR PRINT_COLOR(1, 2, 0)

typedef struct {
    int x;
    int y;
} IrcRankingPoint;

// A trainer of the trainer log, ranked by the value the scripts match against the fortune
typedef struct {
    StrBuf *name;
    u32 value : 7;
    u32 count : 10;
    // The palette of the row, from whether the trainer is the player, the rank and whether the trainer was met today
    u32 palette : 8;
    u32 rank : 7;
    u16 species;
    u8 form;
    u8 sex;
    u8 isEgg;
    u8 isSelf;
} IrcRankingEntry;

typedef struct {
    u32 count;
    IrcRankingEntry entries[];
} IrcRankingData;

// The screen of the characters of a row, for the main engine's BGs and for the sub engine's
typedef struct {
    u16 main[SCREEN_ROWS][SCREEN_COLUMNS];
    u16 sub[SCREEN_ROWS][SCREEN_COLUMNS];
} IrcRankingScreens;

// Black 2's; twinned code confirms the offsets of mainChars, subChars and titleWindow in Black
typedef struct {
    ArcTool *arc;
    u32 mainChars;
    u32 subChars;
    BmpWin *titleWindow;
} IrcRankingBG;

// Black 2's; twinned code confirms the offsets of unit in Black
typedef struct {
    ClActUnit *unit;
    u32 unk4[4];
} IrcRankingClAct;

typedef struct {
    IrcRankingBG bg;
    IrcRankingClAct clact;
    TCB *vblankTask;
} IrcRankingGraphic;

// A trainer's row: the name, rank, value and count drawn on a bitmap, and the Pokémon's icon on each screen
typedef struct {
    const IrcRankingEntry *entry;
    GFLBitmap *bitmap;
    ClActor *icons[2];
    s16 iconY[2];
    u32 iconChars;
    u32 iconCellAnims;
    const IrcRankingScreens *screens;
} IrcRankingRow;

// Black 2's; twinned code confirms the offsets of iconPalettes, screens, rows, bgs, bg, ranking, count, scroll,
// scrollMin, scrollMax, topRow, delta, prevDirection, flag, offsetA, offsetB, windows, font, msgData, wordSet,
// vblankTask, transferPending, scrollDirty and bgDirty in Black
typedef struct {
    u32 iconPalettes[2];
    IrcRankingScreens screens;
    IrcRankingRow rows[ROW_MAX];
    // The BGs of the rows on each screen: two on the main engine and two on the sub engine
    u8 bgs[4];
    IrcRankingBG *bg;
    IrcRankingData *ranking;
    u16 count;
    s16 scroll;
    s16 scrollMin;
    s16 scrollMax;
    // The row at the top
    s16 topRow;
    // How far the last scroll went
    s16 delta;
    s16 prevDirection;
    s16 flag;
    // Where the BGs are scrolled to, which wraps around
    s16 offsetA;
    s16 offsetB;
    BmpWin *windows[2];
    Font *font;
    MsgData *msgData;
    u32 unk578[4];
    WordSet *wordSet;
    TCB *vblankTask;
    // Whether the rows have been redrawn and are to be sent to VRAM
    BOOL transferPending;
    // Whether the list was scrolled and is to be settled
    BOOL scrollDirty;
    // Whether the BGs are to be moved
    BOOL bgDirty;
} IrcRankingList;

// The stylus and the Control Pad that scroll the list
typedef struct {
    IrcRankingPoint start;
    IrcRankingPoint end;
    IrcRankingPoint last;
    IrcRankingPoint delta;
    u32 moveFrames;
    u32 idleFrames;
    u32 keyFrames;
} IrcRankingTouch;

// The scroll that goes on after the stylus is lifted, and slows down
typedef struct {
    fx32 strength;
    fx32 duration;
    fx32 speed;
    BOOL pending;
    s8 direction;
} IrcRankingFling;

// Text on a bitmap that shows as OAM sprites
typedef struct {
    GFLBitmap *bitmap;
    u16 color;
    BOOL busy;
    IrcRankingPoint offset;
    int align;
    StrBuf *strbuf;
    BmpOamActor *actor;
    PrintQueue *queue;
} IrcRankingLabel;

// Black 2's, not checked against Black
typedef struct {
    s16 x;
    s16 y;
    u16 width;
    u16 height;
} IrcRankingLabelDef;

// Black 2's, not checked against Black
typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
} IrcRankingBGInfo;

typedef struct IrcRankingStepMan IrcRankingStepMan;
typedef struct IrcRankingWork IrcRankingWork;
typedef void (*IrcRankingStepFunc)(IrcRankingStepMan *mgr, int *state, IrcRankingWork *wk);

// Black 2's, not checked against Black
struct IrcRankingStepMan {
    IrcRankingStepFunc step;
    BOOL done;
    int state;
    IrcRankingWork *work;
};

struct IrcRankingWork {
    IrcRankingGraphic graphic;
    IrcRankingStepMan stepMan;
    IrcRankingList list;
    IrcRankingTouch touch;
    IrcRankingFling fling;
    IrcAppBar *button;
    Font *font;
    PrintQueue *queue;
    MsgData *msgData;
    BmpOamSys *bmpOam;
    GameData *gameData;
    IrcRankingLabel *labels[LABEL_COUNT];
    IrcRankingData *ranking;
    // How long the list has scrolled to the player's row
    s16 autoScrollFrames;
    u32 palette;
};

static BOOL func_ov106_021b7560(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov106_021b7798(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov106_021b7848(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov106_021b7890(IrcRankingStepMan *mgr, IrcRankingWork *wk, IrcRankingStepFunc step);
static void func_ov106_021b78ac(IrcRankingStepMan *mgr);
static void func_ov106_021b78b8(IrcRankingStepMan *mgr);
static BOOL func_ov106_021b78cc(IrcRankingStepMan *mgr);
static void func_ov106_021b78d0(IrcRankingStepMan *mgr, IrcRankingStepFunc step);
static void func_ov106_021b78d8(IrcRankingStepMan *mgr);
static void func_ov106_021b78e0(IrcRankingStepMan *mgr, int *state, IrcRankingWork *wk);
static void func_ov106_021b7948(IrcRankingStepMan *mgr, int *state, IrcRankingWork *wk);
static void func_ov106_021b7984(IrcRankingStepMan *mgr, int *state, IrcRankingWork *wk);
static void func_ov106_021b7bc4(IrcRankingStepMan *mgr, int *state, IrcRankingWork *wk);
static void func_ov106_021b7d14(IrcRankingBG *bg, GameData *gameData, HeapID heapId);
static void func_ov106_021b7f00(IrcRankingBG *bg);
static void func_ov106_021b7f74(IrcRankingBG *bg);
static u32 func_ov106_021b7f7c(IrcRankingBG *bg, BOOL main);
static void func_ov106_021b7f88(IrcRankingClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov106_021b7fc8(IrcRankingClAct *clact);
static void func_ov106_021b7fe4(IrcRankingClAct *clact);
static void func_ov106_021b7fec(IrcRankingClAct *clact);
static ClActUnit *func_ov106_021b7ff4(IrcRankingClAct *clact);
static void func_ov106_021b7ff8(IrcRankingGraphic *graphic, GameData *gameData, HeapID heapId);
static void func_ov106_021b807c(IrcRankingGraphic *graphic);
static void func_ov106_021b80a0(IrcRankingGraphic *graphic);
static IrcRankingBG *func_ov106_021b80ac(IrcRankingGraphic *graphic);
static ClActUnit *func_ov106_021b80b0(IrcRankingGraphic *graphic);
static void func_ov106_021b80bc(TCB *tcb, void *data);
static void func_ov106_021b80d0(IrcRankingList *list, u8 bg1, u8 bg2, u8 bg5, u8 bg6, IrcRankingBG *bg, ClActUnit *unit,
                                MsgData *msgData, IrcRankingData *ranking, HeapID heapId);
static void func_ov106_021b8304(IrcRankingList *list);
static void func_ov106_021b838c(IrcRankingList *list);
static void func_ov106_021b8408(IrcRankingList *list, s16 delta);
static s16 func_ov106_021b84c4(IrcRankingList *list);
static s16 func_ov106_021b84d0(const IrcRankingList *list);
static s16 func_ov106_021b8518(IrcRankingList *list);
static void func_ov106_021b8524(IrcRankingList *list);
static void func_ov106_021b8648(IrcRankingList *list);
static void func_ov106_021b8690(TCB *tcb, void *data);
static void func_ov106_021b87b0(IrcRankingRow *row, const IrcRankingEntry *entry, ClActUnit *unit, u32 mainPalette,
                               u32 subPalette, MsgData *msgData, Font *font, WordSet *wordSet,
                               const IrcRankingScreens *screens, s16 mainY, s16 subY, HeapID heapId);
static void func_ov106_021b89f4(IrcRankingRow *row);
static void func_ov106_021b8a28(IrcRankingRow *row, BmpWin *window, u8 bg, u16 tileY);
static void func_ov106_021b8a6c(IrcRankingRow *row, s16 scroll);
static void func_ov106_021b8ad0(IrcRankingScreens *screens, IrcRankingBG *bg, HeapID heapId);
static void func_ov106_021b8b4c(IrcRankingScreens *screens);
static void func_ov106_021b8b5c(const IrcRankingScreens *screens, u8 bg, u32 tileY, u8 palette);
static IrcRankingData *func_ov106_021b8b9c(GameData *gameData, HeapID heapId);
static void func_ov106_021b8d5c(IrcRankingData *ranking);
static u32 func_ov106_021b8d8c(IrcRankingData *ranking);
static void func_ov106_021b8d90(IrcRankingTouch *touch, HeapID heapId);
static void func_ov106_021b8d9c(IrcRankingTouch *touch);
static void func_ov106_021b8da8(IrcRankingTouch *touch);
static BOOL func_ov106_021b8e50(IrcRankingTouch *touch, IrcRankingPoint *start, IrcRankingPoint *end,
                                    IrcRankingPoint *delta);
static BOOL func_ov106_021b8e90(IrcRankingTouch *touch, IrcRankingPoint *start, IrcRankingPoint *end,
                                       VecFx32 *vec, u32 *moveFrames);
static u32 func_ov106_021b8fa8(IrcRankingTouch *touch, u32 *keyFrames);
static void func_ov106_021b8fb8(IrcRankingFling *fling);
static void func_ov106_021b8fc4(IrcRankingFling *fling);
static void func_ov106_021b8fd0(IrcRankingFling *fling);
static void func_ov106_021b900c(IrcRankingFling *fling, fx32 speed, u32 frames);
static void func_ov106_021b9134(IrcRankingFling *fling);
static s16 func_ov106_021b913c(IrcRankingFling *fling);
static BOOL func_ov106_021b9198(IrcRankingFling *fling);
static void func_ov106_021b91a8(BmpWin *window, StrBuf *format, PlayerInfo *player, Font *font);
static void func_ov106_021b9298(GFLBitmap *src, GFLBitmap *dest, u32 srcX, u32 srcY, u16 destX, u16 destY, u16 width,
                                 u16 height);
static IrcRankingLabel *func_ov106_021b92d4(const ClActorSetup *setup, u8 width, u8 height, u32 palette,
                                               u8 paletteOffset, u32 surface, BmpOamSys *oam, PrintQueue *queue,
                                               HeapID heapId);
static void func_ov106_021b9360(IrcRankingLabel *label);
static void func_ov106_021b9380(IrcRankingLabel *label, MsgData *msgData, u32 msgId, Font *font);
static void func_ov106_021b93d8(IrcRankingLabel *label, u16 color);
static void func_ov106_021b93dc(IrcRankingLabel *label, int x, int y, int align);
static BOOL func_ov106_021b93e4(IrcRankingLabel *label);
static void func_ov106_021b9414(IrcRankingLabel *label, Font *font, IrcRankingPoint *pos);

// pokebw2's IRC_RANKING_PROC_FUNCTIONS
const GameProcFunctions data_ov106_021b9490 = {
    func_ov106_021b7560,
    func_ov106_021b7848,
    func_ov106_021b7798,
};

// pokebw2's sIrcRankingUpVector
static const VecFx32 data_ov106_021b949c = { 0, FX32_ONE, 0 };

// pokebw2's sIrcRankingLCDConfig
static const BGSysLCDConfig data_ov106_021b94a8 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sIrcRankingClActSetup
static const ClActSysSetup data_ov106_021b94b8 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 48, 48, 48, 48, 16, 16 };

// pokebw2's sIrcRankingLabelDefs
static const IrcRankingLabelDef data_ov106_021b94d4[LABEL_COUNT] = {
    { 8, 39, 6, 2 },
    { 64, 39, 5, 2 },
    { 112, 39, 6, 2 },
    { 168, 39, 6, 2 },
};

// pokebw2's sIrcRankingVRAMConfig
static const BGSysVRAMConfig data_ov106_021b94f4 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sIrcRankingBGInfos
static const IrcRankingBGInfo data_ov106_021b9524[BG_COUNT] = {
    { 0,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT },
    { 1,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT },
    { 2,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT },
    { 3,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1800, GX_BG_CHARBASE_0x14000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT },
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT },
    { 5,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT },
    { 6,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT },
    { 7,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1800, GX_BG_CHARBASE_0x14000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT },
};

// pokebw2's IrcRanking_Init
static BOOL func_ov106_021b7560(GameProc *proc, u32 *state, void *param, void *work) {
    IrcRankingWork *wk;
    IrcCompatibleParam4 *rankingParam = param;
    ArcTool *arc;
    ClActorSetup setup;
    int i;

    GFL_HeapCreateChild(HEAPID_USER, HEAP_ID, 0x60000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(IrcRankingWork), HEAP_ID);
    MI_CpuFill8(wk, 0, sizeof(IrcRankingWork));
    if (rankingParam->gsys != NULL) {
        wk->gameData = GSYS_GetGameData(rankingParam->gsys);
    } else {
        wk->gameData = GameData_Create(HEAPID_USER);
    }
    wk->queue = func_0201c440(HEAP_ID);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAP_ID);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_5, HEAP_ID);
    wk->ranking = func_ov106_021b8b9c(wk->gameData, HEAP_ID);
    func_ov106_021b7ff8(&wk->graphic, wk->gameData, HEAP_ID);
    wk->bmpOam = func_02025538(HEAP_ID, func_ov106_021b80b0(&wk->graphic));
    func_ov106_021b7890(&wk->stepMan, wk, func_ov106_021b78e0);
    func_ov106_021b8d90(&wk->touch, HEAP_ID);
    func_ov106_021b8fb8(&wk->fling);
    func_ov106_021b80d0(&wk->list, 1, 2, 5, 6, func_ov106_021b80ac(&wk->graphic),
                        func_ov106_021b80b0(&wk->graphic), wk->msgData, wk->ranking, HEAP_ID);
    wk->button = func_ov101_021b70dc(2, func_ov106_021b80b0(&wk->graphic), 4, 13, 0, 2, wk->font, wk->queue,
                                  HEAP_ID);
    arc = GFL_ArcSysCreateFileHandle(ARCID_FONT, HEAP_ID);
    wk->palette = func_0204acec(arc, 5, CLACT_VRAM_MAIN, 0x1c0, HEAP_ID);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 8;
    setup.y = 41;
    setup.priority = 0;
    setup.bgPriority = 0;
    for (i = 0; i < LABEL_COUNT; i++) {
        setup.x = data_ov106_021b94d4[i].x;
        setup.y = data_ov106_021b94d4[i].y;
        wk->labels[i] = func_ov106_021b92d4(&setup, data_ov106_021b94d4[i].width, data_ov106_021b94d4[i].height,
                                               wk->palette, 3, 0, wk->bmpOam, wk->queue, HEAP_ID);
        func_ov106_021b93d8(wk->labels[i], LABEL_COLOR);
        func_ov106_021b93dc(wk->labels[i], 0, 0, LABEL_ALIGN_CENTER);
        func_ov106_021b9380(wk->labels[i], wk->msgData, i + 1, wk->font);
    }
    return TRUE;
}

// pokebw2's IrcRanking_Exit
static BOOL func_ov106_021b7798(GameProc *proc, u32 *state, void *param, void *work) {
    IrcRankingWork *wk = work;
    IrcCompatibleParam4 *rankingParam = param;
    int i;

    for (i = 0; i < LABEL_COUNT; i++) {
        func_ov106_021b9360(wk->labels[i]);
    }
    func_0204ada4(wk->palette);
    func_ov101_021b71c0(wk->button);
    func_ov106_021b8304(&wk->list);
    func_ov106_021b8fc4(&wk->fling);
    func_ov106_021b8d9c(&wk->touch);
    func_ov106_021b78ac(&wk->stepMan);
    func_02025588(wk->bmpOam);
    func_ov106_021b807c(&wk->graphic);
    func_ov106_021b8d5c(wk->ranking);
    GFL_MsgDataFree(wk->msgData);
    GFL_FontFree(wk->font);
    func_0201c4c0(wk->queue);
    if (rankingParam->gsys == NULL) {
        GameData_Free(wk->gameData);
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAP_ID);
    return TRUE;
}

// pokebw2's IrcRanking_Main
static BOOL func_ov106_021b7848(GameProc *proc, u32 *state, void *param, void *work) {
    IrcRankingWork *wk = work;
    int i;

    func_ov106_021b78b8(&wk->stepMan);
    func_ov106_021b80a0(&wk->graphic);
    func_0201c4e4(wk->queue);
    for (i = 0; i < LABEL_COUNT; i++) {
        func_ov106_021b93e4(wk->labels[i]);
    }
    if (func_ov106_021b78cc(&wk->stepMan)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IrcRankingStepMan_Init
static void func_ov106_021b7890(IrcRankingStepMan *mgr, IrcRankingWork *wk, IrcRankingStepFunc step) {
    MI_CpuFill8(mgr, 0, sizeof(IrcRankingStepMan));
    mgr->work = wk;
    func_ov106_021b78d0(mgr, step);
}

// pokebw2's IrcRankingStepMan_Free
static void func_ov106_021b78ac(IrcRankingStepMan *mgr) {
    MI_CpuFill8(mgr, 0, sizeof(IrcRankingStepMan));
}

// pokebw2's IrcRankingStepMan_Run
static void func_ov106_021b78b8(IrcRankingStepMan *mgr) {
    if (!mgr->done) {
        mgr->step(mgr, &mgr->state, mgr->work);
    }
}

// pokebw2's IrcRankingStepMan_IsDone
static BOOL func_ov106_021b78cc(IrcRankingStepMan *mgr) {
    return mgr->done;
}

// pokebw2's IrcRankingStepMan_SetStep
static void func_ov106_021b78d0(IrcRankingStepMan *mgr, IrcRankingStepFunc step) {
    mgr->step = step;
    mgr->state = 0;
}

// pokebw2's IrcRankingStepMan_SetDone
static void func_ov106_021b78d8(IrcRankingStepMan *mgr) {
    mgr->done = TRUE;
}

// pokebw2's IrcRanking_StepFadeIn
static void func_ov106_021b78e0(IrcRankingStepMan *mgr, int *state, IrcRankingWork *wk) {
    switch (*state) {
    case 0:
        GFL_FadeSet(3, 16, 0, 0);
        *state = 1;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            *state = 2;
        }
        break;
    case 2:
        // BUG: the list reports -1 when the player is not in it, and this compares with 0xffff
#ifdef BUGFIX
        if (func_ov106_021b84d0(&wk->list) != -1) {
#else
        if (func_ov106_021b84d0(&wk->list) != 0xffff) {
#endif
            func_ov106_021b78d0(&wk->stepMan, func_ov106_021b7bc4);
        } else {
            func_ov106_021b78d0(mgr, func_ov106_021b7984);
        }
        break;
    }
}

// pokebw2's IrcRanking_StepFadeOut
static void func_ov106_021b7948(IrcRankingStepMan *mgr, int *state, IrcRankingWork *wk) {
    switch (*state) {
    case 0:
        GFL_FadeSet(3, 0, 16, 0);
        *state = 1;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            *state = 2;
        }
        break;
    case 2:
        func_ov106_021b78d8(mgr);
        break;
    }
}

// Scrolls the list with the stylus, the Control Pad and the fling that follows
// pokebw2's IrcRanking_StepInput
static void func_ov106_021b7984(IrcRankingStepMan *mgr, int *state, IrcRankingWork *wk) {
    u32 keyFrames;
    IrcRankingPoint delta;
    u32 moveFrames;
    VecFx32 release;

    if (!func_ov101_021b727c(wk->button)) {
        if (func_ov106_021b8fa8(&wk->touch, &keyFrames) & PAD_KEY_DOWN) {
            if (FX32_CONST(keyFrames) > FX32_CONST(KEY_SCROLL_START_FRAMES)) {
                func_ov106_021b900c(
                    &wk->fling,
                    FX_Mul(FX_Div(FX32_CONST(keyFrames) - FX32_CONST(KEY_SCROLL_START_FRAMES),
                                  FX32_CONST(KEY_SCROLL_FRAMES)),
                           FX32_CONST(60)),
                    KEY_SCROLL_FRAMES_FLING);
            } else {
                func_ov106_021b900c(&wk->fling, FX32_CONST(2), KEY_SCROLL_FRAMES_FLING);
            }
        } else if (func_ov106_021b8fa8(&wk->touch, &keyFrames) & PAD_KEY_UP) {
            if (FX32_CONST(keyFrames) > FX32_CONST(KEY_SCROLL_START_FRAMES)) {
                func_ov106_021b900c(
                    &wk->fling,
                    -FX_Mul(FX_Div(FX32_CONST(keyFrames) - FX32_CONST(KEY_SCROLL_START_FRAMES),
                                   FX32_CONST(KEY_SCROLL_FRAMES)),
                            FX32_CONST(60)),
                    KEY_SCROLL_FRAMES_FLING);
            } else {
                func_ov106_021b900c(&wk->fling, -FX32_CONST(2), KEY_SCROLL_FRAMES_FLING);
            }
        } else if (func_ov106_021b8e50(&wk->touch, NULL, NULL, &delta)) {
            func_ov106_021b8408(&wk->list, -delta.y);
            func_ov106_021b9134(&wk->fling);
        } else if (func_ov106_021b8e90(&wk->touch, NULL, NULL, &release, &moveFrames)) {
            func_ov106_021b900c(&wk->fling, VEC_DotProduct(&data_ov106_021b949c, &release), moveFrames);
        }
    }
    if (func_ov106_021b9198(&wk->fling)) {
        func_ov106_021b8408(&wk->list, func_ov106_021b913c(&wk->fling));
    }
    if (func_ov101_021b7250(wk->button) == 1) {
        func_ov106_021b78d0(mgr, func_ov106_021b7948);
    }
    func_ov106_021b838c(&wk->list);
    func_ov106_021b8da8(&wk->touch);
    func_ov106_021b8fd0(&wk->fling);
    func_ov101_021b71e4(wk->button);
}

// Scrolls the list to the player's row, faster the longer it takes
// pokebw2's IrcRanking_StepScrollToSelf
static void func_ov106_021b7bc4(IrcRankingStepMan *mgr, int *state, IrcRankingWork *wk) {
    s16 target = func_ov106_021b84d0(&wk->list) - ROW_HEIGHT * 8;
    s16 scroll;
    s16 scrollMax;
    s16 frames;

    target = MATH_MAX(target, 0);
    scroll = func_ov106_021b84c4(&wk->list);
    scrollMax = func_ov106_021b8518(&wk->list);
    if (target > scroll && scroll != scrollMax) {
        wk->autoScrollFrames++;
        frames = wk->autoScrollFrames;
        if (FX32_CONST(frames) > FX32_CONST(KEY_SCROLL_START_FRAMES)) {
            func_ov106_021b900c(&wk->fling,
                                  FX_Mul(FX_Div(FX32_CONST(frames) - FX32_CONST(KEY_SCROLL_START_FRAMES),
                                                FX32_CONST(KEY_SCROLL_FRAMES)),
                                         FX32_CONST(60)),
                                  KEY_SCROLL_FRAMES_FLING);
        } else {
            func_ov106_021b900c(&wk->fling, FX32_CONST(2), KEY_SCROLL_FRAMES_FLING);
        }
    } else {
        func_ov106_021b9134(&wk->fling);
        func_ov106_021b78d0(&wk->stepMan, func_ov106_021b7984);
    }
    if (func_ov106_021b9198(&wk->fling)) {
        func_ov106_021b8408(&wk->list, func_ov106_021b913c(&wk->fling));
    }
    if (func_ov101_021b7250(wk->button) == 1) {
        func_ov106_021b78d0(mgr, func_ov106_021b7948);
    }
    func_ov106_021b838c(&wk->list);
    func_ov106_021b8fd0(&wk->fling);
    func_ov101_021b71e4(wk->button);
}

// pokebw2's IrcRankingBG_Init
static void func_ov106_021b7d14(IrcRankingBG *bg, GameData *gameData, HeapID heapId) {
    MsgData *msgData;
    Font *font;
    StrBuf *format;
    int i;

    MI_CpuFill8(bg, 0, sizeof(IrcRankingBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov106_021b94a8);
    for (i = 0; i < BG_COUNT; i++) {
        GFL_BGSysCreateBG(data_ov106_021b9524[i].bg, &data_ov106_021b9524[i].setup, data_ov106_021b9524[i].mode);
        GFL_BGSysClearBG(data_ov106_021b9524[i].bg);
        GFL_BGSysSetBGEnabled(data_ov106_021b9524[i].bg, TRUE);
    }
    bg->arc = GFL_ArcSysCreateFileHandle(ARCID_IRC_RANKING, heapId);
    GFL_G2DIOLoadArcNCLRDefault(bg->arc, 0, PALTYPE_MAIN_BG, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(bg->arc, 1, 3, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(bg->arc, 3, 0, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(bg->arc, 4, 3, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(bg->arc, 6, 0, 0, 0, FALSE, heapId);
    GFL_G2DIOLoadArcNCLRDefault(bg->arc, 0, PALTYPE_SUB_BG, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(bg->arc, 3, 7, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(bg->arc, 7, 7, 0, 0, FALSE, heapId);
    GFL_BGSysFillChar(1, 0, 1, 0);
    GFL_BGSysFillChar(5, 0, 1, 0);
    bg->mainChars = GFL_BGSysLoadArcNCGRDynamic(bg->arc, 2, 1, 0, FALSE, heapId);
    bg->subChars = GFL_BGSysLoadArcNCGRDynamic(bg->arc, 2, 5, 0, FALSE, heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 0x1c0, 0x20, heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 0x1c0, 0x20, heapId);
    GFL_ArcToolFree(bg->arc);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_5, heapId);
    font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, heapId);
    bg->titleWindow = BmpWin_CreateDynamic(0, 3, 1, 26, 2, 14, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(bg->titleWindow), 15);
    format = GFL_MsgDataLoadStrbufNew(msgData, BtlMain5_Text_000);
    func_ov106_021b91a8(bg->titleWindow, format, GetGameDataPlayerInfo(gameData), font);
    GFL_StrBufFree(format);
    GFL_FontFree(font);
    GFL_MsgDataFree(msgData);
    BmpWin_FlushChar(bg->titleWindow);
    BmpWin_FlushMap(bg->titleWindow);
    GFL_BGSysLoadScr(0);
}

// pokebw2's IrcRankingBG_Free
static void func_ov106_021b7f00(IrcRankingBG *bg) {
    int i;

    BmpWin_Free(bg->titleWindow);
    GFL_BGSysFreeCharMemory(1, (u16)bg->mainChars, (u16)(bg->mainChars >> 16));
    GFL_BGSysFreeCharMemory(5, (u16)bg->subChars, (u16)(bg->subChars >> 16));
    GFL_BGSysFreeFilledChar(5, 1, 0);
    GFL_BGSysFreeFilledChar(1, 1, 0);
    for (i = 0; i < BG_COUNT; i++) {
        GFL_BGSysReleaseBG(data_ov106_021b9524[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(IrcRankingBG));
}

// pokebw2's IrcRankingBG_VBlank
static void func_ov106_021b7f74(IrcRankingBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's IrcRankingBG_GetChars
static u32 func_ov106_021b7f7c(IrcRankingBG *bg, BOOL main) {
    if (main) {
        return bg->mainChars;
    }
    return bg->subChars;
}

// pokebw2's IrcRankingClAct_Init
static void func_ov106_021b7f88(IrcRankingClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(IrcRankingClAct));
    ClActSys_Create(&data_ov106_021b94b8, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's IrcRankingClAct_Free
static void func_ov106_021b7fc8(IrcRankingClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(IrcRankingClAct));
}

// pokebw2's IrcRankingClAct_Update
static void func_ov106_021b7fe4(IrcRankingClAct *clact) {
    func_0204a600();
}

// pokebw2's IrcRankingClAct_VBlank
static void func_ov106_021b7fec(IrcRankingClAct *clact) {
    func_0204a648();
}

// pokebw2's IrcRankingClAct_GetUnit
static ClActUnit *func_ov106_021b7ff4(IrcRankingClAct *clact) {
    return clact->unit;
}

// pokebw2's IrcRankingGraphic_Create
static void func_ov106_021b7ff8(IrcRankingGraphic *graphic, GameData *gameData, HeapID heapId) {
    MI_CpuFill8(graphic, 0, sizeof(IrcRankingGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov106_021b94f4);
    GFL_BGSysSetDisplayLayout(1);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    func_0201dd60();
    func_ov106_021b7d14(&graphic->bg, gameData, heapId);
    func_ov106_021b7f88(&graphic->clact, &data_ov106_021b94f4, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov106_021b80bc, graphic, 0);
}

// pokebw2's IrcRankingGraphic_Free
static void func_ov106_021b807c(IrcRankingGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov106_021b7fc8(&graphic->clact);
    func_ov106_021b7f00(&graphic->bg);
    MI_CpuFill8(graphic, 0, sizeof(IrcRankingGraphic));
}

// pokebw2's IrcRankingGraphic_Update
static void func_ov106_021b80a0(IrcRankingGraphic *graphic) {
    func_ov106_021b7fe4(&graphic->clact);
}

// pokebw2's IrcRankingGraphic_GetBG
static IrcRankingBG *func_ov106_021b80ac(IrcRankingGraphic *graphic) {
    return &graphic->bg;
}

// pokebw2's IrcRankingGraphic_GetUnit
static ClActUnit *func_ov106_021b80b0(IrcRankingGraphic *graphic) {
    return func_ov106_021b7ff4(&graphic->clact);
}

// pokebw2's IrcRankingGraphic_VBlank
static void func_ov106_021b80bc(TCB *tcb, void *data) {
    IrcRankingGraphic *graphic = data;

    func_ov106_021b7f74(&graphic->bg);
    func_ov106_021b7fec(&graphic->clact);
}

static void func_ov106_021b80d0(IrcRankingList *list, u8 bg1, u8 bg2, u8 bg5, u8 bg6, IrcRankingBG *bg, ClActUnit *unit,
                                MsgData *msgData, IrcRankingData *ranking, HeapID heapId) {
    ArcTool *arc;
    u32 i;

    MI_CpuFill8(list, 0, sizeof(IrcRankingList));
    list->bg = bg;
    list->bgs[0] = bg1;
    list->bgs[1] = bg2;
    list->bgs[2] = bg5;
    list->bgs[3] = bg6;
    list->ranking = ranking;
    list->count = func_ov106_021b8d8c(ranking);
    list->font = GFL_FontCreate(ARCID_FONT, 0, 0, TRUE, heapId);
    list->msgData = msgData;
    list->wordSet = GFL_WordSetSystemCreateDefault(heapId);
    func_ov106_021b8ad0(&list->screens, bg, heapId);
    arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, heapId);
    list->iconPalettes[0] = func_0204acec(arc, func_0201bc3c(), CLACT_VRAM_MAIN, 0x20, heapId);
    list->iconPalettes[1] = func_0204acec(arc, func_0201bc3c(), CLACT_VRAM_SUB, 0x20, heapId);
    GFL_ArcToolFree(arc);
    for (i = 0; i < func_ov106_021b8d8c(ranking); i++) {
        func_ov106_021b87b0(&list->rows[i], &ranking->entries[i], unit, list->iconPalettes[0], list->iconPalettes[1],
                           list->msgData, list->font, list->wordSet, &list->screens, i * ROW_HEIGHT + 82,
                           i * ROW_HEIGHT - 112, heapId);
    }
    list->windows[0] = BmpWin_CreateDynamic(bg2, 0, 0, 32, 32, 14, TRUE);
    list->windows[1] = BmpWin_CreateDynamic(bg6, 0, 0, 32, 32, 14, TRUE);
    list->offsetA = LIST_TOP_OFFSET;
    list->offsetB = LIST_BOTTOM_OFFSET;
    GFL_BGSysMoveBG(list->bgs[0], BG_MOVE_SET_Y, list->offsetA);
    GFL_BGSysMoveBG(list->bgs[1], BG_MOVE_SET_Y, list->offsetA);
    GFL_BGSysMoveBG(list->bgs[2], BG_MOVE_SET_Y, list->offsetB);
    GFL_BGSysMoveBG(list->bgs[3], BG_MOVE_SET_Y, list->offsetB);
    list->scroll = 0;
    list->scrollMin = 0;
    list->scrollMax = list->count * ROW_HEIGHT - LIST_VISIBLE_HEIGHT;
    list->scrollMax = MATH_MAX(0, list->scrollMax);
    func_ov106_021b8524(list);
    func_ov106_021b8648(list);
    list->vblankTask = GFL_VBlankTCBAdd(func_ov106_021b8690, list, 0);
}

// pokebw2's IrcRankingList_Free
static void func_ov106_021b8304(IrcRankingList *list) {
    int i;
    u32 row;

    GFL_TCBRemove(list->vblankTask);
    for (i = 0; i < 2; i++) {
        BmpWin_Free(list->windows[i]);
    }
    for (row = 0; row < func_ov106_021b8d8c(list->ranking); row++) {
        func_ov106_021b89f4(&list->rows[row]);
    }
    func_0204ada4(list->iconPalettes[0]);
    func_0204ada4(list->iconPalettes[1]);
    func_ov106_021b8b4c(&list->screens);
    GFL_WordSetSystemFree(list->wordSet);
    GFL_FontFree(list->font);
    MI_CpuFill8(list, 0, sizeof(IrcRankingList));
}

// Once the list has stopped on a row, redraws it for the rows that show, and plays a sound for a new one
// pokebw2's IrcRankingList_Settle
static void func_ov106_021b838c(IrcRankingList *list) {
    s16 scroll;

    if (list->scrollDirty) {
        list->scrollDirty = FALSE;
        scroll = list->scroll;
        if (scroll % ROW_HEIGHT == 0) {
            if (list->topRow != scroll / ROW_HEIGHT) {
                GFL_SndSEPlay(SEQ_SE_SYS_06);
            }
            list->topRow = list->scroll / ROW_HEIGHT;
            list->offsetA = LIST_BOTTOM_OFFSET - 64;
            list->offsetB = ROW_HEIGHT;
            func_ov106_021b8524(list);
            list->transferPending = TRUE;
        } else {
            list->bgDirty = TRUE;
        }
    }
}

// pokebw2's IrcRankingList_Move
static void func_ov106_021b8408(IrcRankingList *list, s16 delta) {
    s16 scroll = list->scroll;
    int next = scroll + delta;
    int i;

    if (list->scrollMin > next) {
        list->scroll = list->scrollMin;
        list->delta = 0;
    } else if (list->scrollMax < next) {
        list->scroll = list->scrollMax;
        list->delta = 0;
    } else {
        if (scroll / ROW_HEIGHT != next / ROW_HEIGHT && scroll % ROW_HEIGHT != 0) {
            if (delta >= 0) {
                delta = delta - next % ROW_HEIGHT;
            } else {
                delta = delta + (ROW_HEIGHT - next % ROW_HEIGHT);
            }
        }
        list->scroll += delta;
        list->delta = delta;
    }
    for (i = 0; i < list->count; i++) {
        func_ov106_021b8a6c(&list->rows[i], list->scroll);
    }
    list->scrollDirty = TRUE;
}

// pokebw2's IrcRankingList_GetScroll
static s16 func_ov106_021b84c4(IrcRankingList *list) {
    return list->scroll;
}

// The scroll of the player's row, or -1
// pokebw2's IrcRankingList_FindSelf
static s16 func_ov106_021b84d0(const IrcRankingList *list) {
    int i;
    BOOL found = FALSE;

    for (i = 0; i < list->count; i++) {
        if (list->rows[i].entry->isSelf) {
            found = TRUE;
            break;
        }
    }
    if (found) {
        return ROW_HEIGHT * i;
    }
    return -1;
}

// pokebw2's IrcRankingList_GetScrollMax
static s16 func_ov106_021b8518(IrcRankingList *list) {
    return list->scrollMax;
}

// pokebw2's IrcRankingList_Redraw
static void func_ov106_021b8524(IrcRankingList *list) {
    int i;

    GFL_BGSysFillScrArea(list->bgs[0], 0, 0, 0, 32, 24, 0);
    GFL_BGSysFillScrArea(list->bgs[2], 0, 0, 0, 32, 24, 0);
    GFL_BitmapFill(BmpWin_GetBitmap(list->windows[0]), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(list->windows[1]), 0);
    for (i = list->topRow - 1; i < list->topRow + 10; i++) {
        if (i >= 0 && i < list->count) {
            int y = (i - list->topRow) * 3 + 4;

            if (y >= 0 && y <= 24) {
                func_ov106_021b8a28(&list->rows[i], list->windows[0], list->bgs[0], y);
            }
        }
    }
    for (i = list->topRow + 4; i < list->topRow + 17; i++) {
        if (i >= 0 && i < list->count) {
            int y = (i - (list->topRow + 4)) * 3;

            if (y >= 0 && y <= 24) {
                func_ov106_021b8a28(&list->rows[i], list->windows[1], list->bgs[2], y);
            }
        }
    }
}

// pokebw2's IrcRankingList_Transfer
static void func_ov106_021b8648(IrcRankingList *list) {
    int i;

    for (i = 0; i < 2; i++) {
        BmpWin_FlushChar(list->windows[i]);
        BmpWin_FlushMap(list->windows[i]);
    }
    GFL_BGSysLoadScr(list->bgs[0]);
    GFL_BGSysLoadScr(list->bgs[2]);
    GFL_BGSysLoadScr(list->bgs[1]);
    GFL_BGSysLoadScr(list->bgs[3]);
}

// pokebw2's IrcRankingList_VBlank
static void func_ov106_021b8690(TCB *tcb, void *data) {
    IrcRankingList *list = data;
    s16 x;
    s16 y;
    int rem;

    if (list->transferPending) {
        func_ov106_021b8648(list);
        list->transferPending = FALSE;
        list->bgDirty = TRUE;
    }
    if (list->bgDirty) {
        rem = (s16)(list->scroll % ROW_HEIGHT);
        if (rem == 0) {
            x = list->offsetA + rem;
            y = list->offsetB + rem;
            list->prevDirection = list->delta;
            list->flag = 1;
        } else if ((list->delta >= 0 && list->prevDirection >= 0) || (list->delta < 0 && list->prevDirection < 0) ||
                   list->flag != 0) {
            // The list goes on the way it went, or has just stood on a row: the way it goes now decides
            list->flag = 0;
            if (list->delta >= 0) {
                x = list->offsetA + rem;
                y = list->offsetB + rem;
                list->prevDirection = 1;
            } else {
                x = list->offsetA - (ROW_HEIGHT - 1 - rem);
                y = list->offsetB - (ROW_HEIGHT - 1 - rem);
                list->prevDirection = -1;
            }
        } else {
            // It has turned around within a row: the way it went before stays
            if (list->prevDirection >= 0) {
                x = list->offsetA + rem;
                y = list->offsetB + rem;
                list->prevDirection = 1;
            } else {
                x = list->offsetA - (ROW_HEIGHT - 1 - rem);
                y = list->offsetB - (ROW_HEIGHT - 1 - rem);
                list->prevDirection = -1;
            }
        }
        GFL_BGSysMoveBG(list->bgs[0], BG_MOVE_SET_Y, x);
        GFL_BGSysMoveBG(list->bgs[1], BG_MOVE_SET_Y, x);
        GFL_BGSysMoveBG(list->bgs[2], BG_MOVE_SET_Y, y);
        GFL_BGSysMoveBG(list->bgs[3], BG_MOVE_SET_Y, y);
        list->bgDirty = FALSE;
    }
}

static void func_ov106_021b87b0(IrcRankingRow *row, const IrcRankingEntry *entry, ClActUnit *unit, u32 mainPalette,
                               u32 subPalette, MsgData *msgData, Font *font, WordSet *wordSet,
                               const IrcRankingScreens *screens, s16 mainY, s16 subY, HeapID heapId) {
    StrBuf *rankFormat;
    StrBuf *valueFormat;
    StrBuf *countFormat;
    StrBuf *text;
    u32 iconFile;
    u32 iconCells;
    u32 iconAnims;
    ArcTool *arc;
    ClActorSetup setup;
    int i;
    u32 halfWidth;
    u32 palette;

    MI_CpuFill8(row, 0, sizeof(IrcRankingRow));
    row->entry = entry;
    row->screens = screens;
    row->bitmap = GFL_BitmapCreate(32, 3, 32, heapId);
    rankFormat = GFL_StrBufCreate(32, heapId);
    valueFormat = GFL_StrBufCreate(32, heapId);
    countFormat = GFL_StrBufCreate(32, heapId);
    text = GFL_StrBufCreate(32, heapId);
    GFL_MsgDataLoadStrbuf(msgData, 5, rankFormat);
    GFL_MsgDataLoadStrbuf(msgData, 7, valueFormat);
    GFL_MsgDataLoadStrbuf(msgData, 8, countFormat);
    WordSetNumber(wordSet, 0, entry->rank, 2, 0, TRUE);
    WordSetNumber(wordSet, 2, entry->value, 3, 0, TRUE);
    WordSetNumber(wordSet, 3, entry->count, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(wordSet, text, rankFormat);
    halfWidth = GFL_FontGetBlockWidth(text, font, 0) / 2;
    GFL_TextRendererDrawToBitmap(row->bitmap, 38 - halfWidth, 5, text, font);
    GFL_TextRendererDrawToBitmap(row->bitmap, 52, 5, entry->name, font);
    GFL_WordSetFormatStrbuf(wordSet, text, valueFormat);
    halfWidth = GFL_FontGetBlockWidth(text, font, 0) / 2;
    GFL_TextRendererDrawToBitmap(row->bitmap, 142 - halfWidth, 5, text, font);
    GFL_WordSetFormatStrbuf(wordSet, text, countFormat);
    halfWidth = GFL_FontGetBlockWidth(text, font, 0) / 2;
    GFL_TextRendererDrawToBitmap(row->bitmap, 194 - halfWidth, 5, text, font);
    GFL_StrBufFree(text);
    GFL_StrBufFree(countFormat);
    GFL_StrBufFree(valueFormat);
    GFL_StrBufFree(rankFormat);
    iconFile = PokeParty_GetIconIndex(entry->species, entry->form, entry->sex, entry->isEgg);
    iconCells = func_0201bc40();
    iconAnims = getOBJTileMapping_MainEng();
    arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, heapId);
    row->iconChars = func_0204a6c8(arc, iconFile, FALSE, CLACT_VRAM_BOTH, heapId);
    row->iconCellAnims = func_0204af28(arc, iconCells, iconAnims, heapId);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.sequence = 1;
    setup.bgPriority = 1;
    for (i = 0; i < 2; i++) {
        setup.x = 232;
        if (i == 0) {
            setup.y = mainY;
            palette = mainPalette;
        } else {
            setup.y = subY;
            palette = subPalette;
        }
        row->iconY[i] = setup.y;
        row->icons[i] = func_0204b294(unit, row->iconChars, palette, row->iconCellAnims,
                                      &setup, i, heapId);
        func_0204b6f4(row->icons[i], func_0201bb5c(entry->species, entry->form, entry->sex, entry->isEgg), FALSE);
        if (setup.y > -16 && setup.y < 208) {
            func_0204b3dc(row->icons[i], TRUE);
        } else {
            func_0204b3dc(row->icons[i], FALSE);
        }
    }
}

// pokebw2's IrcRankingRow_Free
static void func_ov106_021b89f4(IrcRankingRow *row) {
    int i;

    for (i = 0; i < 2; i++) {
        func_0204b3b4(row->icons[i]);
    }
    func_0204a8d4(row->iconChars);
    func_0204afd8(row->iconCellAnims);
    GFL_BitmapFree(row->bitmap);
    MI_CpuFill8(row, 0, sizeof(IrcRankingRow));
}

// pokebw2's IrcRankingRow_Draw
static void func_ov106_021b8a28(IrcRankingRow *row, BmpWin *window, u8 bg, u16 tileY) {
    func_ov106_021b9298(row->bitmap, BmpWin_GetBitmap(window), 0, 0, 0, tileY, 32, 3);
    func_ov106_021b8b5c(row->screens, bg, tileY, row->entry->palette);
}

// pokebw2's IrcRankingRow_SetScroll
static void func_ov106_021b8a6c(IrcRankingRow *row, s16 scroll) {
    ClActorPos pos;
    int offset[2];
    int i;

    offset[0] = -scroll;
    offset[1] = -scroll;
    for (i = 0; i < 2; i++) {
        func_0204b454(row->icons[i], &pos, i);
        pos.y = offset[i] + row->iconY[i];
        func_0204b404(row->icons[i], &pos, i);
        if (pos.y > -16 && pos.y < 208) {
            func_0204b3dc(row->icons[i], TRUE);
        } else {
            func_0204b3dc(row->icons[i], FALSE);
        }
    }
}

// pokebw2's IrcRankingScreens_Init
static void func_ov106_021b8ad0(IrcRankingScreens *screens, IrcRankingBG *bg, HeapID heapId) {
    NNSG2dScreenData *screen;
    void *file;
    u16 mainChars;
    u16 subChars;
    u16 *src;
    int y;
    int x;

    MI_CpuFill8(screens, 0, sizeof(IrcRankingScreens));
    mainChars = func_ov106_021b7f7c(bg, TRUE);
    subChars = func_ov106_021b7f7c(bg, FALSE);
    file = GFL_G2DIOReadNSCR(ARCID_IRC_RANKING, 5, FALSE, &screen, heapId);
    src = (u16 *)screen->rawData;
    for (y = 0; y < SCREEN_ROWS; y++) {
        for (x = 0; x < SCREEN_COLUMNS; x++) {
            screens->main[y][x] = src[y * SCREEN_COLUMNS + x] + mainChars;
            screens->sub[y][x] = src[y * SCREEN_COLUMNS + x] + subChars;
        }
    }
    GFL_HeapFree(file);
}

// pokebw2's IrcRankingScreens_Free
static void func_ov106_021b8b4c(IrcRankingScreens *screens) {
    MI_CpuFill8(screens, 0, sizeof(IrcRankingScreens));
}

// pokebw2's IrcRankingScreens_Load
static void func_ov106_021b8b5c(const IrcRankingScreens *screens, u8 bg, u32 tileY, u8 palette) {
    const u16 *src = screens->main[0];

    if (bg >= 4) {
        src = screens->sub[0];
    }
    GFL_BGSysLoadScrAreaAll(bg, src, 0, tileY, 32, 3);
    GFL_BGSysSetScrPaletteNo(bg, 0, tileY, 32, 3, palette);
}

// Reads the trainer log, and ranks its trainers by value
// pokebw2's IrcRankingData_Create
static IrcRankingData *func_ov106_021b8b9c(GameData *gameData, HeapID heapId) {
    TrainerLogSave *log = func_0200a8a0(GameData_GetSaveControl(gameData));
    u16 count = func_0200a8ac(log);
    IrcRankingData *ranking = GFL_HeapAllocate(heapId, sizeof(u32) + count * sizeof(IrcRankingEntry), FALSE,
                                               "irc_ranking.c", 2724);
    IrcRankingEntry *entry;
    u8 prevValue;
    u8 prevRank;
    u8 self;
    u32 i;

    MI_CpuFill8(ranking, 0, sizeof(u32) + count * sizeof(IrcRankingEntry));
    ranking->count = count;
    if (count != 0) {
        prevValue = 0x80;
        prevRank = 0;
        self = func_0200a930(log);
        for (i = 0; i < ranking->count; i++) {
            u8 met;

            entry = &ranking->entries[i];
            entry->name = GFL_StrBufCreate(8, heapId);
            GFL_StrBufLoadString(entry->name, func_0200a8c8(log, i));
            entry->value = func_0200a8d0(log, i);
            entry->count = func_0200a8e4(log, i);
            if (entry->value < prevValue) {
                entry->rank = i + 1;
            } else if (entry->value == prevValue) {
                entry->rank = prevRank;
            }
            met = func_0200a8f8(log, i);
            if (self == i) {
                u32 base = 1;

                if (met) {
                    base = 0;
                }
                entry->palette = (base + 1) * 5;
                entry->isSelf = TRUE;
            } else if (entry->rank == 1) {
                u32 base = 0;

                if (!met) {
                    base = 5;
                }
                entry->palette = base + 2;
            } else if (entry->rank == 2) {
                u32 base = 0;

                if (!met) {
                    base = 5;
                }
                entry->palette = base + 3;
            } else if (entry->rank == 3) {
                u32 base = 0;

                if (!met) {
                    base = 5;
                }
                entry->palette = base + 4;
            } else {
                u32 base = 0;

                if (!met) {
                    base = 5;
                }
                entry->palette = base + 1;
            }
            func_0200a908(log, i, &entry->species, &entry->form, &entry->sex, &entry->isEgg);
            prevValue = entry->value;
            prevRank = entry->rank;
        }
    }
    return ranking;
}

// pokebw2's IrcRankingData_Free
static void func_ov106_021b8d5c(IrcRankingData *ranking) {
    u32 i;

    if (ranking->count != 0) {
        for (i = 0; i < ranking->count; i++) {
            if (ranking->entries[i].name != NULL) {
                GFL_StrBufFree(ranking->entries[i].name);
            }
        }
    }
    GFL_HeapFree(ranking);
}

// pokebw2's IrcRankingData_GetCount
static u32 func_ov106_021b8d8c(IrcRankingData *ranking) {
    return ranking->count;
}

// pokebw2's IrcRankingTouch_Init
static void func_ov106_021b8d90(IrcRankingTouch *touch, HeapID heapId) {
    MI_CpuFill8(touch, 0, sizeof(IrcRankingTouch));
}

// pokebw2's IrcRankingTouch_Free
static void func_ov106_021b8d9c(IrcRankingTouch *touch) {
    MI_CpuFill8(touch, 0, sizeof(IrcRankingTouch));
}

// pokebw2's IrcRankingTouch_Update
static void func_ov106_021b8da8(IrcRankingTouch *touch) {
    u32 x;
    u32 y;

    if (func_02035d20(&x, &y)) {
        touch->start.x = x;
        touch->start.y = y;
        touch->end.x = x;
        touch->end.y = y;
        touch->last.x = x;
        touch->last.y = y;
        touch->delta.x = 0;
        touch->delta.y = 0;
        touch->moveFrames = 0;
    } else if (func_02035cb8(&x, &y)) {
        int diff = y - touch->start.y;

        if (MATH_ABS(diff) >= 1) {
            touch->delta.x = x - touch->last.x;
            touch->delta.y = y - touch->last.y;
            touch->last.x = x;
            touch->last.y = y;
        }
        touch->end.x = x;
        touch->end.y = y;
        touch->moveFrames++;
        touch->idleFrames = 0;
    } else {
        touch->idleFrames++;
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_KEY_UP | PAD_KEY_DOWN)) {
        touch->keyFrames = 0;
        return;
    }
    if (GCTX_HIDGetHeldKeys() & (PAD_KEY_UP | PAD_KEY_DOWN)) {
        touch->keyFrames++;
    }
}

static BOOL func_ov106_021b8e50(IrcRankingTouch *touch, IrcRankingPoint *start, IrcRankingPoint *end,
                                    IrcRankingPoint *delta) {
    if (func_02035c38()) {
        if (start != NULL) {
            *start = touch->start;
        }
        if (end != NULL) {
            *end = touch->end;
        }
        if (delta != NULL) {
            *delta = touch->delta;
        }
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov106_021b8e90(IrcRankingTouch *touch, IrcRankingPoint *start, IrcRankingPoint *end,
                                       VecFx32 *vec, u32 *moveFrames) {
    VecFx32 from;
    VecFx32 to;

    if (GCTX_HIDGetHeldKeys() == 0 && touch->idleFrames < 1) {
        touch->idleFrames = 1;
        if (start != NULL) {
            *start = touch->start;
        }
        if (end != NULL) {
            *end = touch->end;
        }
        if (vec != NULL) {
            VEC_Set(&from, FX32_CONST(touch->start.x), FX32_CONST(touch->start.y), 0);
            VEC_Set(&to, FX32_CONST(touch->end.x), FX32_CONST(touch->end.y), 0);
            VEC_Subtract(&from, &to, vec);
        }
        if (moveFrames != NULL) {
            *moveFrames = touch->moveFrames;
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IrcRankingTouch_GetHeldKeys
static u32 func_ov106_021b8fa8(IrcRankingTouch *touch, u32 *keyFrames) {
    if (keyFrames != NULL) {
        *keyFrames = touch->keyFrames;
    }
    return GCTX_HIDGetHeldKeys();
}

// pokebw2's IrcRankingFling_Init
static void func_ov106_021b8fb8(IrcRankingFling *fling) {
    MI_CpuFill8(fling, 0, sizeof(IrcRankingFling));
}

// pokebw2's IrcRankingFling_Free
static void func_ov106_021b8fc4(IrcRankingFling *fling) {
    MI_CpuFill8(fling, 0, sizeof(IrcRankingFling));
}

// Turns a fling that was started into its speed
// pokebw2's IrcRankingFling_Update
static void func_ov106_021b8fd0(IrcRankingFling *fling) {
    if (fling->pending) {
        fling->pending = FALSE;
        fling->speed = fling->strength + fling->duration / 2;
        fling->speed = FX_Mul(fling->speed, FX32_CONST(40));
    }
}

// Starts a fling at a speed, from a scroll that took some frames
// pokebw2's IrcRankingFling_Start
static void func_ov106_021b900c(IrcRankingFling *fling, fx32 speed, u32 frames) {
    fx32 limit;

    if (MATH_ABS(speed) > FX32_ONE) {
        fling->direction = speed / MATH_ABS(speed);
        speed = MATH_ABS(speed);
        speed = MATH_CLAMP(speed, FX32_ONE, FX32_CONST(60));
        if (FX32_CONST(frames) < FX32_CONST(13)) {
            limit = MATH_CLAMP(FX32_CONST(frames), 0, FX32_CONST(13));
            fling->strength = FX_Div(speed - FX32_ONE, FX32_CONST(59));
            fling->duration = FX32_ONE - FX_Div(limit, FX32_CONST(13));
            fling->pending = TRUE;
        }
    }
}

// pokebw2's IrcRankingFling_Stop
static void func_ov106_021b9134(IrcRankingFling *fling) {
    fling->speed = 0;
}

// Slows the fling down, and returns how far the list moves
// pokebw2's IrcRankingFling_Step
static s16 func_ov106_021b913c(IrcRankingFling *fling) {
    fx32 speed = fling->speed;
    fx32 step = speed * fling->direction;

    step = MATH_CLAMP(step, -0x8000, 0x8000);
    fling->speed -= FX_Mul(speed, 205);
    if (fling->speed > -FX32_ONE && fling->speed < FX32_ONE) {
        fling->speed = 0;
    }
    return FX_Whole(step);
}

// pokebw2's IrcRankingFling_IsActive
static BOOL func_ov106_021b9198(IrcRankingFling *fling) {
    if (fling->speed != 0) {
        return TRUE;
    }
    return FALSE;
}

// Prints a message with the player's name in it, in the middle of a window
// pokebw2's IrcRanking_PrintCentered
static void func_ov106_021b91a8(BmpWin *window, StrBuf *format, PlayerInfo *player, Font *font) {
    WordSet *wordSet;
    StrBuf *strbuf;
    u16 x;
    u16 y;

    if (player == NULL) {
        x = func_02045738(window) * 4;
        y = func_02045740(window) * 4;
        x -= (u16)(GFL_FontGetBlockWidth(format, font, 0) / 2);
        y -= (u16)(GFL_FontGetBlockHeight(format, font) / 2);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), x, y, format, font);
        return;
    }
    wordSet = GFL_WordSetSystemCreateDefault(HEAPID_TAIL(HEAP_ID));
    strbuf = GFL_StrBufCreate(128, HEAPID_TAIL(HEAP_ID));
    copyVarForText(wordSet, 0, player);
    GFL_WordSetFormatStrbuf(wordSet, strbuf, format);
    x = func_02045738(window) * 4;
    y = func_02045740(window) * 4;
    x -= (u16)(GFL_FontGetBlockWidth(strbuf, font, 0) / 2);
    y -= (u16)(GFL_FontGetBlockHeight(strbuf, font) / 2);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), x, y, strbuf, font);
    GFL_StrBufFree(strbuf);
    GFL_WordSetSystemFree(wordSet);
}

// Copies a rectangle of tiles from a bitmap to another of the same width
static void func_ov106_021b9298(GFLBitmap *src, GFLBitmap *dest, u32 srcX, u32 srcY, u16 destX, u16 destY, u16 width,
                                 u16 height) {
    u8 *srcPixels = GFL_BitmapGetPixelData(src);
    u8 *destPixels = GFL_BitmapGetPixelData(dest);

    MIi_CpuCopy32(srcPixels + (srcX + width * srcY) * 32, destPixels + (destX + width * destY) * 32, width * height * 32);
}

static IrcRankingLabel *func_ov106_021b92d4(const ClActorSetup *setup, u8 width, u8 height, u32 palette,
                                               u8 paletteOffset, u32 surface, BmpOamSys *oam, PrintQueue *queue,
                                               HeapID heapId) {
    IrcRankingLabel *label;
    BmpOamActorSetup actorSetup;

    label = GFL_HeapAllocate(heapId, sizeof(IrcRankingLabel), FALSE, "irc_ranking.c", 3402);
    MI_CpuFill8(label, 0, sizeof(IrcRankingLabel));
    label->queue = queue;
    label->strbuf = GFL_StrBufCreate(128, heapId);
    label->bitmap = GFL_BitmapCreate(width, height, 32, heapId);
    MI_CpuFill8(&actorSetup, 0, sizeof(BmpOamActorSetup));
    actorSetup.bitmap = label->bitmap;
    actorSetup.x = setup->x;
    actorSetup.y = setup->y;
    actorSetup.palette = palette;
    actorSetup.priority = setup->priority;
    actorSetup.surface = surface;
    actorSetup.vramType = surface;
    actorSetup.bgPriority = setup->bgPriority;
    actorSetup.paletteOffset = paletteOffset;
    label->actor = func_020255a0(oam, &actorSetup);
    return label;
}

// pokebw2's IrcRankingLabel_Free
static void func_ov106_021b9360(IrcRankingLabel *label) {
    func_0202570c(label->actor);
    GFL_BitmapFree(label->bitmap);
    GFL_StrBufFree(label->strbuf);
    GFL_HeapFree(label);
}

// pokebw2's IrcRankingLabel_PrintMsg
static void func_ov106_021b9380(IrcRankingLabel *label, MsgData *msgData, u32 msgId, Font *font) {
    IrcRankingPoint pos;

    GFL_BitmapFill(label->bitmap, label->color & 0x1f);
    GFL_MsgDataLoadStrbuf(msgData, msgId, label->strbuf);
    func_ov106_021b9414(label, font, &pos);
    func_0201c724(label->queue, label->bitmap, pos.x, pos.y, label->strbuf, font, label->color);
    label->busy = TRUE;
}

// pokebw2's IrcRankingLabel_SetColor
static void func_ov106_021b93d8(IrcRankingLabel *label, u16 color) {
    label->color = color;
}

// pokebw2's IrcRankingLabel_SetPos
static void func_ov106_021b93dc(IrcRankingLabel *label, int x, int y, int align) {
    label->offset.x = x;
    label->offset.y = y;
    label->align = align;
}

// pokebw2's IrcRankingLabel_Update
static BOOL func_ov106_021b93e4(IrcRankingLabel *label) {
    if (label->busy) {
        if (!func_0201c6c4(label->queue, label->bitmap)) {
            func_020257d0(label->actor);
            label->busy = FALSE;
        }
    }
    if (!label->busy) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IrcRankingLabel_GetPosition
static void func_ov106_021b9414(IrcRankingLabel *label, Font *font, IrcRankingPoint *pos) {
    u32 halfWidth;
    u32 halfHeight;
    int y;

    switch (label->align) {
    case LABEL_ALIGN_NONE:
        *pos = label->offset;
        break;
    case LABEL_ALIGN_CENTER:
        halfWidth = GFL_BitmapGetWidth(label->bitmap) / 2;
        halfHeight = GFL_BitmapGetHeight(label->bitmap) / 2;
        halfWidth -= GFL_FontGetBlockWidth(label->strbuf, font, 0) / 2;
        y = halfHeight - GFL_FontGetBlockHeight(label->strbuf, font) / 2;
        pos->x = halfWidth + label->offset.x;
        pos->y = y + label->offset.y;
        break;
    case LABEL_ALIGN_CENTER_Y:
        halfHeight = GFL_BitmapGetHeight(label->bitmap) / 2;
        y = halfHeight - GFL_FontGetBlockHeight(label->strbuf, font) / 2;
        pos->x = label->offset.x;
        pos->y = y + label->offset.y;
        break;
    }
}
