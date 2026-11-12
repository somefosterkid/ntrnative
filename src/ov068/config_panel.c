// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov140/config_panel.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/config_panel.h"
#include "app/ui/touchbar.h"
#include "app/ui/ui_scene.h"
#include "constants/arc.h"
#include "constants/script_text_banks.h"
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
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/math.h"
#include "nnsys/g2d.h"
#include "nnsys/gfd.h"
#include "save/config.h"
#include "save/save_control.h"
#include "save/trainer_data_savedata.h"
#include "system/app_printsys_common.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "text/script/global_10520.h"
#include "text/system/title_2.h"

// The options screen of the menu: five options in rows that scroll, each with two or three values to pick with the
// keys or the touch screen, an explanation of the row under the cursor, and the buttons that confirm the changes or
// quit. The ROM has no name for this file; "config_panel.c" is a guess

#define HEAPID_CONFIG_PANEL 0x1c
// The screen's graphics
#define ARCID_CONFIG_PANEL 27
// The options screen's shortcut
#define SHORTCUT_CONFIG_PANEL 24

#define BG_COUNT 6
#define BG_BAR 0
#define BG_TEXT 1
#define BG_ROWS 2
#define BG_CONFIRM 3
#define BG_MESSAGE 4

// The windows: the values, the options' names, the title, the explanation and the question
#define WINDOW_COUNT 19
#define LABEL_COUNT 16
#define WINDOW_OPTION 11
#define WINDOW_TITLE 16
#define WINDOW_MESSAGE 17
#define WINDOW_CONFIRM 18

#define ACTOR_ITEM 2
// The actors: the values' frames from ACTOR_ITEM on
#define ACTOR_COUNT 14

// The options and all their values
#define OPTION_COUNT 5
#define OPTION_TEXT_SPEED 0
#define OPTION_BATTLE_SCENE 1
#define OPTION_BATTLE_STYLE 2
#define OPTION_SOUND 3
#define OPTION_SAVE_BEFORE_IR 4
#define ITEM_COUNT 11

// The cursor: an option, or one of the buttons
#define CURSOR_CONFIRM 5
#define CURSOR_QUIT 6
#define CURSOR_MAX 6

// The top and bottom of the rows on the screen
#define ROWS_TOP 24
#define ROWS_BOTTOM 168

// What the player did this frame
enum {
    INPUT_NONE,
    INPUT_SLIDE,
    INPUT_2,
    INPUT_TOUCH,
    INPUT_UP,
    INPUT_DOWN,
    INPUT_RIGHT,
    INPUT_LEFT,
    INPUT_DECIDE,
    INPUT_CANCEL,
    INPUT_UP_HELD,
    INPUT_DOWN_HELD,
    INPUT_Y,
    INPUT_X,
    // The first key after the touch screen, which only brings the cursor back
    INPUT_KEY_MODE,
};

// The points the input keeps: where the touch began, how far it moved this frame, and where it is
enum {
    POINT_START,
    POINT_SLIDE,
    POINT_NOW,
};

// The buttons of the bar. The touch bar's close icon is BUTTON_CLOSE
enum {
    BUTTON_CONFIRM,
    BUTTON_QUIT,
    BUTTON_CLOSE,
    BUTTON_NONE,
};

// How the message window prints
enum {
    MSG_MODE_NONE,
    MSG_MODE_STREAM,
    MSG_MODE_QUEUE,
};

// The answer to the question
enum {
    CONFIRM_NONE,
    CONFIRM_YES,
    CONFIRM_NO,
};

// The touch bar's own icons that the screen uses
#define TOUCHBAR_ICON_CLOSE 0
#define TOUCHBAR_ICON_CHECK 6

typedef struct {
    s32 x;
    s32 y;
} ConfigPanelPoint;

typedef struct {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} ConfigPanelRect;

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
} ConfigPanelBgSetup;

// A text of the screen: its message and its window, in tiles. The width is set from the text
typedef struct {
    u32 msgId;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} ConfigPanelLabel;

// An option: its explanation and its values
typedef struct {
    u16 msgId;
    u16 count;
    u8 items[4];
} ConfigPanelOption;

// A value of an option: where its frame is in the rows, and the area that picks it
typedef struct {
    u32 option;
    u32 value;
    s32 x;
    s32 y;
    s16 hitX;
    s16 hitY;
    s16 hitWidth;
    s16 hitHeight;
} ConfigPanelItem;

// The options as the screen edits them. changed is the value last picked, ITEM_COUNT when none
typedef struct {
    u8 values[OPTION_COUNT];
    u8 changed;
    Config *config;
} ConfigPanelOptions;

typedef struct {
    BmpWin *windows[WINDOW_COUNT];
    u16 pulseAngle;
    u16 pad4E;
    u16 pulseColor;
    void *titleCharBuf;
    NNSG2dCharacterData *titleChars;
    u16 cursorPalette[16];
} ConfigPanelBg;

typedef struct {
    ClActUnit *unit;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    u32 knobChars;
    u32 knobCellAnims;
    ClActor *actors[ACTOR_COUNT];
    u16 frame;
} ConfigPanelObj;

typedef struct {
    ConfigPanelBg bg;
    ConfigPanelObj obj;
    TCB *vblankTcb;
} ConfigPanelGraphic;

typedef struct ConfigPanelSeq ConfigPanelSeq;
typedef void (*ConfigPanelSeqFunc)(ConfigPanelSeq *seq, u32 *state, void *data);

struct ConfigPanelSeq {
    ConfigPanelSeqFunc func;
    BOOL ended;
    u32 state;
    void *data;
};

typedef struct {
    u32 input;
    ConfigPanelPoint start;
    ConfigPanelPoint slide;
    ConfigPanelPoint now;
    BOOL touching;
} ConfigPanelUi;

typedef struct {
    MsgData *msgData;
    Font *font;
    PrintStream *stream;
    TCBExManager *tcbManager;
    BmpWin *window;
    StrBuf *strbuf;
    u16 clearColor;
    u16 heapId;
    PrintWindow printWindow;
    PrintQueue *queue;
    u32 mode;
    AppPrintsysCommon common;
    u32 unk34;
} ConfigPanelMsg;

typedef struct {
    Font *font;
    PrintQueue *queue;
    TouchBar *touchBar;
    s32 button;
    BOOL decided;
    AppTaskMenuRes *res;
    MsgData *msgData;
    AppTaskMenuItem items[2];
    AppTaskMenuWin *wins[2];
    GameData *gameData;
    // Whether a button or an icon was touched this frame, which hides the cursor
    BOOL touched;
} ConfigPanelAppBar;

typedef struct {
    u16 textBg;
    u16 rowsBg;
    s32 cursor;
    ClActor *actors[ITEM_COUNT];
    s32 y;
    u32 repeat;
    u32 unk3C;
    u32 unk40;
    ConfigPanelOptions options;
    BOOL update;
    BOOL cursorVisible;
} ConfigPanelScroll;

typedef struct {
    ConfigPanelMsg msg;
    AppTaskMenu *menu;
    AppTaskMenuItem items[2];
    u16 result;
    u16 state;
    AppTaskMenuInit init;
    AppTaskMenuRes *res;
} ConfigPanelConfirm;

typedef struct {
    ConfigPanelGraphic graphic;
    ConfigPanelSeq seq;
    ConfigPanelUi ui;
    ConfigPanelAppBar appBar;
    ConfigPanelScroll scroll;
    ConfigPanelMsg msg;
    ConfigPanelConfirm confirm;
    // The options as they were
    ConfigPanelOptions saved;
    Font *font;
    PrintQueue *queue;
    MsgData *msgData;
    AppTaskMenuRes *buttonRes;
    AppTaskMenuRes *confirmRes;
    TCBExManager *tcbManager;
    ConfigPanelParam *param;
} ConfigPanelWork;

BOOL func_ov068_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov068_021b9768(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov068_021b980c(GameProc *proc, u32 *state, void *param, void *work);
void func_ov068_021b9850(ConfigPanelGraphic *graphic, HeapID heapId);
void func_ov068_021b98e0(ConfigPanelGraphic *graphic);
// Not static: overlay 169's data has two words equal to this function's address that dsd took for relocations
// (`overlays(140,167,...)`, really overlay 167's), and config_fixes.py can't resolve them in White 2, so the symbol
// must exist for the link.
void func_ov068_021b9930(ConfigPanelGraphic *graphic);
ClActor *func_ov068_021b9944(ConfigPanelGraphic *graphic, u32 index);
ClActUnit *func_ov068_021b9950(ConfigPanelGraphic *graphic);
BmpWin *func_ov068_021b995c(ConfigPanelGraphic *graphic, u32 index);
void func_ov068_021b9964(ConfigPanelGraphic *graphic);
void func_ov068_021b996c(TCB *tcb, void *data);
void func_ov068_021b9980(ConfigPanelBg *bg, HeapID heapId);
void func_ov068_021b9b6c(ConfigPanelBg *bg);
static void func_ov068_021b9bd0(ConfigPanelBg *bg);
BmpWin *func_ov068_021b9bfc(ConfigPanelBg *bg, u32 index);
void func_ov068_021b9c04(ConfigPanelBg *bg);
void func_ov068_021b9d58(ConfigPanelBg *bg);
void func_ov068_021b9d60(ConfigPanelBg *bg);
static void func_ov068_021b9d68(u16 *dest, u16 *angle, u16 speed, u8 palette, u8 index, u16 from, u16 to);
void func_ov068_021b9e28(ConfigPanelObj *obj, const BGSysVRAMConfig *vramConfig, HeapID heapId);
void func_ov068_021b9f70(ConfigPanelObj *obj);
static void func_ov068_021b9fc0(ConfigPanelObj *obj);
void func_ov068_021ba010(ConfigPanelObj *obj);
ClActor *func_ov068_021ba018(ConfigPanelObj *obj, u32 index);
ClActUnit *func_ov068_021ba020(ConfigPanelObj *obj);
void func_ov068_021ba024(ConfigPanelUi *ui, HeapID heapId);
void func_ov068_021ba030(ConfigPanelUi *ui);
void func_ov068_021ba03c(ConfigPanelUi *ui);
u32 func_ov068_021ba190(ConfigPanelUi *ui);
void func_ov068_021ba194(ConfigPanelUi *ui, u32 kind, ConfigPanelPoint *point);
void func_ov068_021ba1c4(ConfigPanelMsg *msg, BmpWin *window, Font *font, MsgData *msgData,
                                TCBExManager *tcbManager, PrintQueue *queue, u16 heapId);
void func_ov068_021ba254(ConfigPanelMsg *msg);
void func_ov068_021ba278(ConfigPanelMsg *msg, u32 msgId, s32 wait, BOOL stream);
void func_ov068_021ba28c(ConfigPanelMsg *msg, MsgData *msgData, u32 msgId, s32 wait, BOOL stream);
BOOL func_ov068_021ba31c(ConfigPanelMsg *msg);
void func_ov068_021ba37c(ConfigPanelAppBar *appBar, ClActUnit *unit, Font *font, PrintQueue *queue, MsgData *msgData, AppTaskMenuRes *res, GameData *gameData, HeapID heapId);
void func_ov068_021ba410(ConfigPanelAppBar *appBar);
void func_ov068_021ba448(ConfigPanelAppBar *appBar, ConfigPanelUi *ui, ConfigPanelScroll *scroll);
static void func_ov068_021ba644(ConfigPanelAppBar *appBar);
BOOL func_ov068_021ba65c(ConfigPanelAppBar *appBar, s32 *button);
BOOL func_ov068_021ba668(ConfigPanelAppBar *appBar);
void func_ov068_021ba694(ConfigPanelAppBar *appBar);
void func_ov068_021ba6b8(ConfigPanelAppBar *appBar, u16 heapId);
BOOL func_ov068_021ba730(ConfigPanelAppBar *appBar);
void func_ov068_021ba734(ConfigPanelScroll *scroll, u16 textBg, u16 rowsBg, ConfigPanelGraphic *graphic,
                                   const ConfigPanelOptions *options, HeapID heapId);
void func_ov068_021ba7b4(ConfigPanelScroll *scroll);
void func_ov068_021ba7c0(ConfigPanelScroll *scroll, ConfigPanelUi *ui, ConfigPanelMsg *msg,
                                   ConfigPanelGraphic *graphic, ConfigPanelAppBar *appBar);
void func_ov068_021ba9c0(ConfigPanelScroll *scroll, ConfigPanelGraphic *graphic);
s32 func_ov068_021ba9dc(ConfigPanelScroll *scroll);
void func_ov068_021ba9e0(ConfigPanelScroll *scroll, ConfigPanelOptions *options);
void func_ov068_021ba9f0(ConfigPanelScroll *scroll, BOOL unused);
void func_ov068_021ba9fc(ConfigPanelScroll *scroll, u8 value);
void func_ov068_021baa0c(ConfigPanelScroll *scroll, BOOL cursorVisible);
void func_ov068_021babe0(ConfigPanelScroll *scroll, int dy, int min, int max);
void func_ov068_021bac70(ConfigPanelScroll *scroll, const ConfigPanelPoint *point);
void func_ov068_021bacf4(ConfigPanelScroll *scroll, s8 sound, BOOL force);
void func_ov068_021bad14(ConfigPanelScroll *scroll, int dir);
s32 func_ov068_021bad38(ConfigPanelScroll *scroll, const ConfigPanelPoint *point);
void func_ov068_021bad58(ConfigPanelScroll *scroll);
void func_ov068_021bad60(ConfigPanelConfirm *confirm, BmpWin *window, Font *font, MsgData *msgData,
                                    TCBExManager *tcbManager, AppTaskMenuRes *res, PrintQueue *queue, u16 heapId);
void func_ov068_021badfc(ConfigPanelConfirm *confirm);
void func_ov068_021bae28(ConfigPanelConfirm *confirm);
void func_ov068_021bae90(ConfigPanelConfirm *confirm, s32 wait);
u32 func_ov068_021baebc(ConfigPanelConfirm *confirm);
void func_ov068_021baec4(ConfigPanelConfirm *confirm, u32 msgId, s32 wait);
void func_ov068_021baf0c(ConfigPanelConfirm *confirm, u32 msgId, s32 wait);
BOOL func_ov068_021baf38(ConfigPanelConfirm *confirm);
void func_ov068_021baf5c(ConfigPanelOptions *options, Config *config);
void func_ov068_021baf9c(ConfigPanelOptions *options);
void func_ov068_021bafa8(const ConfigPanelOptions *options);
void func_ov068_021bafd8(ConfigPanelOptions *options, u32 item);
void func_ov068_021bb000(ConfigPanelOptions *options, u32 option, int dir);
BOOL func_ov068_021bb0ac(const ConfigPanelOptions *options, u32 item);
s32 func_ov068_021bb0d0(const ConfigPanelOptions *options);
u32 func_ov068_021bb0d8(ConfigPanelOptions *options);
BOOL func_ov068_021bb0e4(const ConfigPanelOptions *a, const ConfigPanelOptions *b);
void func_ov068_021bb0f8(ConfigPanelSeq *seq, void *data, ConfigPanelSeqFunc func);
void func_ov068_021bb114(ConfigPanelSeq *seq);
static void func_ov068_021bb120(ConfigPanelSeq *seq);
static BOOL func_ov068_021bb134(ConfigPanelSeq *seq);
void func_ov068_021bb138(ConfigPanelSeq *seq, ConfigPanelSeqFunc func);
void func_ov068_021bb140(ConfigPanelSeq *seq);
void func_ov068_021bb148(ConfigPanelSeq *seq, u32 *state, void *data);
void func_ov068_021bb188(ConfigPanelSeq *seq, u32 *state, void *data);
void func_ov068_021bb1c4(ConfigPanelSeq *seq, u32 *state, void *data);
void func_ov068_021bb418(ConfigPanelSeq *seq, u32 *state, void *data);
void func_ov068_021bb4a4(ConfigPanelSeq *seq, u32 *state, void *data);
void func_ov068_021bb4d4(ConfigPanelSeq *seq, u32 *state, void *data);
BOOL func_ov068_021bb508(const ConfigPanelRect *rect, const ConfigPanelPoint *point);

// pokebw2's CONFIG_PANEL_PROC_FUNCTIONS
// pokebw2's ConfigPanel_Exit
// pokebw2's ConfigPanel_Init
// pokebw2's ConfigPanel_Main
const GameProcFunctions data_ov068_021bb5c8 = { func_ov068_021b95a0, func_ov068_021b980c, func_ov068_021b9768 };

// pokebw2's sConfigPanelVRAMConfig
static const BGSysVRAMConfig data_ov068_021bb598 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sConfigPanelLCDConfig
static const BGSysLCDConfig data_ov068_021bb540 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sConfigPanelBgSetups
static const ConfigPanelBgSetup data_ov068_021bb614[BG_COUNT] = {
    { 0,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 1, 0, 0 },
      BGMODE_TEXT },
    { 1,
      { 0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 2, 0, 0 },
      BGMODE_TEXT },
    { 2,
      { 0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1800, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 3, 0, 0 },
      BGMODE_TEXT },
    { 3,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2800, GX_BG_CHARBASE_0x14000, 0x8000,
        GX_BG_EXTPLTT_01, 0, 0, 0 },
      BGMODE_TEXT },
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, 0, 0 },
      BGMODE_TEXT },
    { 5,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 0, 0, 0 },
      BGMODE_TEXT },
};

// The values' texts in the order of data_ov068_021bb704, then the options' names
// pokebw2's sConfigPanelLabels
static ConfigPanelLabel data_ov068_021bb820[LABEL_COUNT] = {
    { Title2_Text_010, 14, 5, 6, 2 },        { Title2_Text_011, 20, 5, 6, 2 },
    { Title2_Text_012, 26, 5, 6, 2 },        { Title2_Text_013, 15, 8, 4, 2 },
    { Title2_Text_014, 24, 8, 6, 2 },         { Title2_Text_015, 15, 11, 8, 2 },
    { Title2_Text_016, 24, 11, 8, 2 },        { Title2_Text_017, 15, 14, 6, 2 },
    { Title2_Text_018, 24, 14, 6, 2 },       { Title2_Text_023, 15, 17, 6, 2 },
    { Title2_Text_024, 24, 17, 7, 2 },     { Title2_Text_001, 1, 5, 12, 2 },
    { Title2_Text_002, 1, 8, 10, 2 }, { Title2_Text_003, 1, 11, 12, 2 },
    { Title2_Text_004, 1, 14, 6, 2 },       { Title2_Text_007, 1, 17, 10, 2 },
};

// pokebw2's sConfigPanelItems
static const ConfigPanelItem data_ov068_021bb704[ITEM_COUNT] = {
    { OPTION_TEXT_SPEED, 0, 120, 0, 112, 0, 48, 24 },       { OPTION_TEXT_SPEED, 1, 165, 0, 160, 0, 48, 24 },
    { OPTION_TEXT_SPEED, 2, 211, 0, 208, 0, 48, 24 },       { OPTION_BATTLE_SCENE, 0, 120, 24, 112, 24, 72, 24 },
    { OPTION_BATTLE_SCENE, 1, 192, 24, 184, 24, 72, 24 },   { OPTION_BATTLE_STYLE, 0, 120, 48, 112, 48, 72, 24 },
    { OPTION_BATTLE_STYLE, 1, 192, 48, 184, 48, 72, 24 },   { OPTION_SOUND, 0, 120, 72, 112, 72, 72, 24 },
    { OPTION_SOUND, 1, 192, 72, 184, 72, 72, 24 },          { OPTION_SAVE_BEFORE_IR, 1, 120, 96, 112, 96, 72, 24 },
    { OPTION_SAVE_BEFORE_IR, 0, 192, 96, 184, 96, 72, 24 },
};

// The buttons' areas of the touch screen
// pokebw2's sConfigPanelConfirmRect
static const ConfigPanelRect data_ov068_021bb550 = { 112, 168, 184, 192 };
// pokebw2's sConfigPanelQuitRect
static const ConfigPanelRect data_ov068_021bb560 = { 184, 168, 256, 192 };

// pokebw2's sConfigPanelOptions
static const ConfigPanelOption data_ov068_021bb570[OPTION_COUNT] = {
    { Title2_Text_025, 3, { 0, 1, 2 } },  { Title2_Text_026, 2, { 3, 4 } },
    { Title2_Text_027, 2, { 5, 6 } }, { Title2_Text_028, 2, { 7, 8 } },
    { Title2_Text_031, 2, { 9, 10 } },
};

// The palettes of the rows on the screen, from the top
// pokebw2's sConfigPanelRowPalettes
static const u8 data_ov068_021bb538[OPTION_COUNT] = { 3, 1, 0, 1, 3 };



BOOL func_ov068_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    ConfigPanelWork *wk;
    GameData *gameData;

    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_CONFIG_PANEL, 0x21000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(ConfigPanelWork), HEAPID_CONFIG_PANEL);
    MI_CpuFill8(wk, 0, sizeof(ConfigPanelWork));
    wk->param = param;
    wk->param->result = FALSE;
    func_ov068_021b9850(&wk->graphic, HEAPID_CONFIG_PANEL);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_CONFIG_PANEL);
    wk->queue = func_0201c440(HEAPID_CONFIG_PANEL);
    wk->buttonRes = func_02027f60(BG_BAR, 13, wk->font, wk->queue, HEAPID_CONFIG_PANEL);
    wk->confirmRes = func_02027f60(BG_CONFIRM, 9, wk->font, wk->queue, HEAPID_CONFIG_PANEL);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_2, HEAPID_CONFIG_PANEL);
    wk->tcbManager = GFL_TCBExMgrCreate(HEAPID_CONFIG_PANEL, HEAPID_CONFIG_PANEL, 32, 32);
    func_ov068_021baf5c(
        &wk->saved, (Config *)getTrainerDataBlkAddress(GameData_GetSaveControl(GSYS_GetGameData(wk->param->gsys))));
    func_ov068_021bb0f8(&wk->seq, wk, func_ov068_021bb148);
    gameData = GSYS_GetGameData(wk->param->gsys);
    func_ov068_021ba37c(&wk->appBar, func_ov068_021b9950(&wk->graphic), wk->font, wk->queue, wk->msgData,
                        wk->buttonRes, gameData, HEAPID_CONFIG_PANEL);
    func_ov068_021ba734(&wk->scroll, BG_TEXT, BG_ROWS, &wk->graphic, &wk->saved, HEAPID_CONFIG_PANEL);
    func_ov068_021ba024(&wk->ui, HEAPID_CONFIG_PANEL);
    func_ov068_021ba1c4(&wk->msg, func_ov068_021b995c(&wk->graphic, WINDOW_MESSAGE), wk->font, wk->msgData,
                        wk->tcbManager, wk->queue, HEAPID_CONFIG_PANEL);
    func_ov068_021bad60(&wk->confirm, func_ov068_021b995c(&wk->graphic, WINDOW_CONFIRM), wk->font,
                        wk->msgData, wk->tcbManager, wk->confirmRes, wk->queue, HEAPID_CONFIG_PANEL);
    return TRUE;
}

BOOL func_ov068_021b9768(GameProc *proc, u32 *state, void *param, void *work) {
    ConfigPanelWork *wk = work;

    func_ov068_021badfc(&wk->confirm);
    func_ov068_021ba254(&wk->msg);
    func_ov068_021ba030(&wk->ui);
    func_ov068_021ba7b4(&wk->scroll);
    func_ov068_021ba410(&wk->appBar);
    func_ov068_021bb114(&wk->seq);
    func_ov068_021baf9c(&wk->saved);
    GFL_TCBExMgrFree(wk->tcbManager);
    GFL_MsgDataFree(wk->msgData);
    func_02027fd8(wk->confirmRes);
    func_02027fd8(wk->buttonRes);
    func_0201c4c0(wk->queue);
    GFL_FontFree(wk->font);
    func_ov068_021b98e0(&wk->graphic);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_CONFIG_PANEL);
    GFL_OvlUnload(OVERLAY_ID(67));
    return TRUE;
}

BOOL func_ov068_021b980c(GameProc *proc, u32 *state, void *param, void *work) {
    ConfigPanelWork *wk = work;

    func_ov068_021bb120(&wk->seq);
    func_ov068_021b9930(&wk->graphic);
    func_0201c4e4(wk->queue);
    GFL_TCBExMgrUpdate(wk->tcbManager);
    func_ov068_021ba644(&wk->appBar);
    if (func_ov068_021bb134(&wk->seq)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ConfigPanelGraphic_Init
void func_ov068_021b9850(ConfigPanelGraphic *graphic, HeapID heapId) {
    MI_CpuFill8(graphic, 0, sizeof(ConfigPanelGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(0);
    GFL_BGSysSetVRAMBanks(&data_ov068_021bb598);
    GFL_BGSysSetDisplayLayout(0);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    GFL_TextRndUpdateColorIndexLUT(15, 14, 0);
    func_ov068_021b9980(&graphic->bg, heapId);
    func_ov068_021b9e28(&graphic->obj, &data_ov068_021bb598, heapId);
    graphic->vblankTcb = GFL_VBlankTCBAdd(func_ov068_021b996c, graphic, 0);
}

// pokebw2's ConfigPanelGraphic_Exit
void func_ov068_021b98e0(ConfigPanelGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTcb);
    func_ov068_021b9f70(&graphic->obj);
    func_ov068_021b9b6c(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(ConfigPanelGraphic));
}

// pokebw2's ConfigPanelGraphic_Main
void func_ov068_021b9930(ConfigPanelGraphic *graphic) {
    func_ov068_021b9fc0(&graphic->obj);
    func_ov068_021b9bd0(&graphic->bg);
}

// pokebw2's ConfigPanelGraphic_GetActor
ClActor *func_ov068_021b9944(ConfigPanelGraphic *graphic, u32 index) {
    return func_ov068_021ba018(&graphic->obj, index);
}

// pokebw2's ConfigPanelGraphic_GetUnit
ClActUnit *func_ov068_021b9950(ConfigPanelGraphic *graphic) {
    return func_ov068_021ba020(&graphic->obj);
}

// pokebw2's ConfigPanelGraphic_GetWindow
BmpWin *func_ov068_021b995c(ConfigPanelGraphic *graphic, u32 index) {
    return func_ov068_021b9bfc(&graphic->bg, index);
}

// pokebw2's ConfigPanelGraphic_ResetPulse
void func_ov068_021b9964(ConfigPanelGraphic *graphic) {
    func_ov068_021b9d58(&graphic->bg);
}

// pokebw2's ConfigPanelGraphic_VBlank
void func_ov068_021b996c(TCB *tcb, void *data) {
    ConfigPanelGraphic *graphic = data;

    func_ov068_021b9d60(&graphic->bg);
    func_ov068_021ba010(&graphic->obj);
}

// pokebw2's ConfigPanelBg_Init
void func_ov068_021b9980(ConfigPanelBg *bg, HeapID heapId) {
    int i;
    ArcTool *arc;
    NNSG2dPaletteData *palette;
    void *buf;

    MI_CpuFill8(bg, 0, sizeof(ConfigPanelBg));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov068_021bb540);
    for (i = 0; i < BG_COUNT; i++) {
        u32 index = data_ov068_021bb614[i].bg;

        GFL_BGSysCreateBG(index, &data_ov068_021bb614[i].setup, data_ov068_021bb614[i].mode);
        GFL_BGSysClearBG(index);
        GFL_BGSysSetBGEnabled(index, TRUE);
    }
    GFL_BGSysSetBGEnabled(BG_CONFIRM, FALSE);
    GFL_BGSysFillChar(BG_MESSAGE, 0, 1, 0);
    GFL_BGSysFillChar(BG_CONFIRM, 0, 1, 0);

    arc = GFL_ArcSysCreateFileHandle(ARCID_CONFIG_PANEL, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, 0, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 13, BG_BAR, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 14, BG_BAR, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0, BG_ROWS, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 2, BG_ROWS, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 16, BG_CONFIRM, 1, 0, FALSE, heapId);
    GFL_G2DIOLoadArcNCLR(arc, 8, 0, 4 * 0x20, 15 * 0x20, 0x20, heapId);
    bg->titleCharBuf = GFL_G2DIOReadBGNCGRArc(arc, 15, FALSE, &bg->titleChars, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 8, 4, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 7, 5, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 9, 5, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 16, BG_MESSAGE, 1, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);

    buf = GFL_G2DIOReadNCLR(ARCID_CONFIG_PANEL, 1, &palette, heapId);
    MI_CpuCopy8((u16 *)palette->pRawData + 7 * 16, bg->cursorPalette, sizeof(bg->cursorPalette));
    GFL_HeapFree(buf);

    bg->windows[WINDOW_MESSAGE] = BmpWin_CreateDynamic(BG_MESSAGE, 1, 19, 30, 4, 4, TRUE);
    bg->windows[WINDOW_CONFIRM] = BmpWin_CreateDynamic(BG_CONFIRM, 1, 1, 30, 4, 15, TRUE);
    bg->windows[WINDOW_TITLE] = BmpWin_CreateDynamic(BG_BAR, 0, 0, 16, 3, 8, TRUE);
    func_ov068_021b9c04(bg);
    GFL_BGSysSetBGEnabled(BG_MESSAGE, FALSE);
}

// pokebw2's ConfigPanelBg_Exit
void func_ov068_021b9b6c(ConfigPanelBg *bg) {
    int i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        if (bg->windows[i] != NULL) {
            BmpWin_Free(bg->windows[i]);
        }
    }
    GFL_HeapFree(bg->titleCharBuf);
    GFL_BGSysFreeFilledChar(BG_MESSAGE, 1, 0);
    GFL_BGSysFreeFilledChar(BG_CONFIRM, 1, 0);
    for (i = 0; i < BG_COUNT; i++) {
        GFL_BGSysReleaseBG(data_ov068_021bb614[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(ConfigPanelBg));
}

// Pulses the cursor's color
// pokebw2's ConfigPanelBg_Main
static void func_ov068_021b9bd0(ConfigPanelBg *bg) {
    func_ov068_021b9d68(&bg->pulseColor, &bg->pulseAngle, 0x400, 7, 1, bg->cursorPalette[11], bg->cursorPalette[12]);
}

// pokebw2's ConfigPanelBg_GetWindow
BmpWin *func_ov068_021b9bfc(ConfigPanelBg *bg, u32 index) {
    return bg->windows[index];
}

// pokebw2's ConfigPanelBg_PrintLabels
void func_ov068_021b9c04(ConfigPanelBg *bg) {
    int i;
    Font *font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_CONFIG_PANEL);
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_2, HEAPID_CONFIG_PANEL);
    StrBuf *str;
    int x;
    const void *chars;

    for (i = 0; i < LABEL_COUNT; i++) {
        ConfigPanelLabel *label;
        u8 width;

        x = 0;
        switch (i) {
        case 0:
            x = 7;
            break;
        case 1:
            x = 4;
            break;
        case 2:
            x = 2;
            break;
        }
        label = &data_ov068_021bb820[i];
        str = GFL_MsgDataLoadStrbufNew(msgData, data_ov068_021bb820[i].msgId);
        width = (x + GFL_FontGetBlockWidth(str, font, 0) + 7) / 8;
        label->width = width;
        bg->windows[i] = BmpWin_CreateDynamic(BG_TEXT, label->x, label->y, width, label->height, 0, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(bg->windows[i]), 0);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(bg->windows[i]), x, 0, str, font);
        BmpWin_Transfer(bg->windows[i]);
        GFL_StrBufFree(str);
    }

    // The title's window starts as the bar's graphic of it
    chars = bg->titleChars->pRawData;
    MIi_CpuCopy32(chars, GFL_BitmapGetPixelData(BmpWin_GetBitmap(bg->windows[WINDOW_TITLE])), 16 * 3 * 0x20);
    str = GFL_MsgDataLoadStrbufNew(msgData, Title2_Text_000);
    x = func_02045738(bg->windows[WINDOW_TITLE]) * 4;
    x -= GFL_FontGetBlockWidth(str, font, 0) / 2;
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(bg->windows[WINDOW_TITLE]), x, 4, str, font,
                                   PRINT_COLOR(15, 14, 1));
    BmpWin_Transfer(bg->windows[WINDOW_TITLE]);
    GFL_StrBufFree(str);
    GFL_MsgDataFree(msgData);
    GFL_FontFree(font);
}

// pokebw2's ConfigPanelBg_ResetPulse
void func_ov068_021b9d58(ConfigPanelBg *bg) {
    bg->pulseAngle = 0;
}

// pokebw2's ConfigPanelBg_VBlank
void func_ov068_021b9d60(ConfigPanelBg *bg) {
    GFL_BGSysUpdate();
}

// Moves a color of a main engine BG palette between two colors, one way and back in a full turn of the angle
// pokebw2's ConfigPanel_PulseColor
static void func_ov068_021b9d68(u16 *dest, u16 *angle, u16 speed, u8 palette, u8 index, u16 from, u16 to) {
    if (*angle + speed >= 0x10000) {
        *angle = *angle + speed - 0x10000;
    } else {
        *angle += speed;
    }
    {
        u8 fromG = (from & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
        s16 t = (FX_CosIdx(*angle) + FX32_ONE) / 2;
        u8 fromR = (from & GX_RGB_R_MASK) >> GX_RGB_R_SHIFT;
        u8 fromB = (from & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
        u8 toB = (to & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
        u8 b = fromB + ((toB - fromB) * t >> FX32_SHIFT);
        u8 toR = (to & GX_RGB_R_MASK) >> GX_RGB_R_SHIFT;
        u8 r = fromR + ((toR - fromR) * t >> FX32_SHIFT);
        u8 toG = (to & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
        u8 g = fromG + ((toG - fromG) * t >> FX32_SHIFT);

        *dest = GX_RGB(r, g, b);
        NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, palette * 32 + index * 2, dest, sizeof(u16));
    }
}

// pokebw2's ConfigPanelObj_Init
void func_ov068_021b9e28(ConfigPanelObj *obj, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    int i;
    ArcTool *arc;
    ClActorSetup setup;

    MI_CpuFill8(obj, 0, sizeof(ConfigPanelObj));
    ClActSys_Create(&data_020a1448, vramConfig, heapId);
    obj->unit = func_0204b100(128, 0, heapId);
    func_0204b270(obj->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);

    arc = GFL_ArcSysCreateFileHandle(ARCID_CONFIG_PANEL, heapId);
    obj->palette = func_0204abf0(arc, 6, CLACT_VRAM_MAIN, 0, heapId);
    obj->chars = func_0204a6c8(arc, 5, FALSE, CLACT_VRAM_MAIN, heapId);
    obj->cellAnims = func_0204af28(arc, 4, 3, heapId);
    obj->knobChars = func_0204a6c8(arc, 12, FALSE, CLACT_VRAM_MAIN, heapId);
    obj->knobCellAnims = func_0204af28(arc, 11, 10, heapId);
    GFL_ArcToolFree(arc);

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.bgPriority = 2;
    setup.priority = 2;
    for (i = ACTOR_ITEM; i < ACTOR_ITEM + ITEM_COUNT; i++) {
        setup.x = data_ov068_021bb704[i - ACTOR_ITEM].x;
        setup.y = data_ov068_021bb704[i - ACTOR_ITEM].y;
        obj->actors[i] = func_0204b294(obj->unit, obj->chars, obj->palette, obj->cellAnims, &setup, 0, heapId);
        func_0204b92c(obj->actors[i], FALSE);
    }

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 128;
    setup.y = 96;
    setup.bgPriority = 2;
    setup.priority = 1;
    obj->actors[1] = func_0204b294(obj->unit, obj->knobChars, obj->palette, obj->knobCellAnims, &setup, 0, heapId);
    func_0204b6f4(obj->actors[1], 6, 1);

    if (func_0203cd98()) {
        func_0203d34c(TRUE, heapId);
    }
}

// pokebw2's ConfigPanelObj_Exit
void func_ov068_021b9f70(ConfigPanelObj *obj) {
    int i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        if (obj->actors[i] != NULL) {
            func_0204b3b4(obj->actors[i]);
        }
    }
    func_0204ada4(obj->palette);
    func_0204a8d4(obj->chars);
    func_0204afd8(obj->cellAnims);
    func_0204a8d4(obj->knobChars);
    func_0204afd8(obj->knobCellAnims);
    func_0204b1cc(obj->unit);
    func_0204a5ac();
    MI_CpuFill8(obj, 0, sizeof(ConfigPanelObj));
}

// Animates the values' frames together
// pokebw2's ConfigPanelObj_Main
static void func_ov068_021b9fc0(ConfigPanelObj *obj) {
    int i;

    if (obj->frame++ > 96) {
        obj->frame = 0;
    }
    for (i = ACTOR_ITEM; i < ACTOR_ITEM + ITEM_COUNT; i++) {
        func_0204b908(obj->actors[i], obj->frame / 12);
    }
    func_0204a600();
}

// pokebw2's ConfigPanelObj_VBlank
void func_ov068_021ba010(ConfigPanelObj *obj) {
    func_0204a648();
}

// pokebw2's ConfigPanelObj_GetActor
ClActor *func_ov068_021ba018(ConfigPanelObj *obj, u32 index) {
    return obj->actors[index];
}

// pokebw2's ConfigPanelObj_GetUnit
ClActUnit *func_ov068_021ba020(ConfigPanelObj *obj) {
    return obj->unit;
}

// pokebw2's ConfigPanelUi_Init
void func_ov068_021ba024(ConfigPanelUi *ui, HeapID heapId) {
    MI_CpuFill8(ui, 0, sizeof(ConfigPanelUi));
}

// pokebw2's ConfigPanelUi_Exit
void func_ov068_021ba030(ConfigPanelUi *ui) {
    MI_CpuFill8(ui, 0, sizeof(ConfigPanelUi));
}

// pokebw2's ConfigPanelUi_Main
void func_ov068_021ba03c(ConfigPanelUi *ui) {
    u32 x;
    u32 y;
    u32 pressed;
    u32 held;

    ui->input = INPUT_NONE;
    if (func_02035d20(&x, &y)) {
        ui->input = INPUT_TOUCH;
        ui->start.x = x;
        ui->start.y = y;
        ui->now.x = x;
        ui->now.y = y;
        ui->touching = TRUE;
    } else if (func_02035cb8(&x, &y) && ui->touching) {
        if (MATH_ABS((s32)y - ui->start.y) >= 1) {
            ui->input = INPUT_SLIDE;
            ui->slide.x = x - ui->now.x;
            ui->slide.y = y - ui->now.y;
            ui->now.x = x;
            ui->now.y = y;
        }
    } else {
        ui->touching = FALSE;
    }

    pressed = GCTX_HIDGetPressedKeys();
    held = GCTX_HIDGetHeldKeys();
    if (pressed != 0 && func_020355c4() == TRUE && !(pressed & (PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y))) {
        func_020355d8(FALSE);
        ui->input = INPUT_KEY_MODE;
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return;
    }
    if (pressed & PAD_KEY_UP) {
        ui->input = INPUT_UP;
    } else if (pressed & PAD_KEY_DOWN) {
        ui->input = INPUT_DOWN;
    } else if (pressed & PAD_KEY_LEFT) {
        ui->input = INPUT_LEFT;
    } else if (pressed & PAD_KEY_RIGHT) {
        ui->input = INPUT_RIGHT;
    } else if (held & PAD_KEY_UP) {
        ui->input = INPUT_UP_HELD;
    } else if (held & PAD_KEY_DOWN) {
        ui->input = INPUT_DOWN_HELD;
    } else if (pressed & PAD_BUTTON_A) {
        ui->input = INPUT_DECIDE;
    } else if (pressed & PAD_BUTTON_B) {
        func_020355d8(FALSE);
        ui->input = INPUT_CANCEL;
    } else if (pressed & PAD_BUTTON_Y) {
        func_020355d8(FALSE);
        ui->input = INPUT_Y;
    } else if (pressed & PAD_BUTTON_X) {
        func_020355d8(FALSE);
        ui->input = INPUT_X;
    }
}

// pokebw2's ConfigPanelUi_GetInput
u32 func_ov068_021ba190(ConfigPanelUi *ui) {
    return ui->input;
}

// pokebw2's ConfigPanelUi_GetPoint
void func_ov068_021ba194(ConfigPanelUi *ui, u32 kind, ConfigPanelPoint *point) {
    if (point != NULL) {
        switch (kind) {
        case POINT_START:
            *point = ui->start;
            break;
        case POINT_SLIDE:
            *point = ui->slide;
            break;
        case POINT_NOW:
            *point = ui->now;
            break;
        }
    }
}

void func_ov068_021ba1c4(ConfigPanelMsg *msg, BmpWin *window, Font *font, MsgData *msgData,
                                TCBExManager *tcbManager, PrintQueue *queue, u16 heapId) {
    MI_CpuFill8(msg, 0, sizeof(ConfigPanelMsg));
    msg->clearColor = 4;
    msg->window = window;
    msg->heapId = heapId;
    msg->font = font;
    msg->msgData = msgData;
    msg->strbuf = GFL_StrBufCreate(255, heapId);
    msg->tcbManager = tcbManager;
    msg->stream = NULL;
    msg->queue = queue;
    msg->mode = MSG_MODE_NONE;
    func_02028470(&msg->common,
                           APP_PRINTSYS_COMMON_KEYS | APP_PRINTSYS_COMMON_TOUCH | APP_PRINTSYS_COMMON_NO_SKIP);
    BmpWin_DrawFrame(msg->window, 2, 1, func_02045778(window));
    GFL_BitmapFill(BmpWin_GetBitmap(msg->window), msg->clearColor);
    BmpWin_TransferNow(msg->window);
    PrintWindow_Init(&msg->printWindow, msg->window);
}

// pokebw2's ConfigPanelMsg_Exit
void func_ov068_021ba254(ConfigPanelMsg *msg) {
    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
        msg->stream = NULL;
    }
    GFL_StrBufFree(msg->strbuf);
    MI_CpuFill8(msg, 0, sizeof(ConfigPanelMsg));
}

// pokebw2's ConfigPanelMsg_Print
void func_ov068_021ba278(ConfigPanelMsg *msg, u32 msgId, s32 wait, BOOL stream) {
    func_ov068_021ba28c(msg, msg->msgData, msgId, wait, stream);
}

// Prints a message at once, or a character at a time with the wait
// pokebw2's ConfigPanelMsg_PrintFrom
void func_ov068_021ba28c(ConfigPanelMsg *msg, MsgData *msgData, u32 msgId, s32 wait, BOOL stream) {
    GFL_BitmapFill(BmpWin_GetBitmap(msg->window), msg->clearColor);
    GFL_MsgDataLoadStrbuf(msgData, msgId, msg->strbuf);
    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
        msg->stream = NULL;
    }
    if (!stream) {
        PrintWindow_PrintNoColor(&msg->printWindow, msg->queue, 0, 0, msg->strbuf, msg->font);
        msg->mode = MSG_MODE_QUEUE;
    } else {
        msg->stream = func_0201cce4(msg->window, 0, 0, msg->strbuf, msg->font, wait, msg->tcbManager, 0, msg->heapId,
                                    msg->clearColor);
        msg->mode = MSG_MODE_STREAM;
    }
}

// pokebw2's ConfigPanelMsg_IsDone
BOOL func_ov068_021ba31c(ConfigPanelMsg *msg) {
    switch (msg->mode) {
    case MSG_MODE_NONE:
    default:
        return TRUE;
    case MSG_MODE_QUEUE:
        PrintWindow_Flush(&msg->printWindow, msg->queue);
        return PrintWindow_IsPrinted(&msg->printWindow);
    case MSG_MODE_STREAM:
        return func_02028484(&msg->common, msg->stream);
    }
}

void func_ov068_021ba37c(ConfigPanelAppBar *appBar, ClActUnit *unit, Font *font, PrintQueue *queue, MsgData *msgData, AppTaskMenuRes *res, GameData *gameData, HeapID heapId) {
    MI_CpuFill8(appBar, 0, sizeof(ConfigPanelAppBar));
    appBar->button = BUTTON_NONE;
    appBar->font = font;
    appBar->res = res;
    appBar->msgData = msgData;
    appBar->queue = queue;
    appBar->gameData = gameData;

    {
        TouchBarItem items[2] = {
            { TOUCHBAR_ICON_CLOSE, { 80, 168 } },
            { TOUCHBAR_ICON_CHECK, { 48, 172 } },
        };
        TouchBarSetup setup;

        MI_CpuFill8(&setup, 0, sizeof(TouchBarSetup));
        setup.items = items;
        setup.count = 2;
        setup.unit = unit;
        setup.objPalette = 8;
        setup.vramType = CLACT_VRAM_BOTH;
        setup.unk1C = TRUE;
        appBar->touchBar = func_ov067_021b62e0(&setup, heapId);
    }
    func_ov068_021ba6b8(appBar, heapId);

    if (GameData_IsShortcutRegistered(appBar->gameData, SHORTCUT_CONFIG_PANEL)) {
        func_ov067_021b6598(appBar->touchBar, 6, TRUE);
    } else {
        func_ov067_021b6598(appBar->touchBar, 6, FALSE);
    }
}


// pokebw2's ConfigPanelAppBar_Exit
void func_ov068_021ba410(ConfigPanelAppBar *appBar) {
    int i;

    func_ov067_021b639c(appBar->touchBar);
    for (i = 0; i < 2; i++) {
        GFL_StrBufFree(appBar->items[i].str);
        func_02028148(appBar->wins[i]);
    }
    MI_CpuFill8(appBar, 0, sizeof(ConfigPanelAppBar));
}

// pokebw2's ConfigPanelAppBar_Main
void func_ov068_021ba448(ConfigPanelAppBar *appBar, ConfigPanelUi *ui, ConfigPanelScroll *scroll) {
    BOOL update = FALSE;
    u32 input;
    s32 cursor;
    ConfigPanelPoint point;

    appBar->touched = FALSE;
    if (appBar->decided) {
        input = INPUT_NONE;
    } else {
        input = func_ov068_021ba190(ui);
    }
    cursor = func_ov068_021ba9dc(scroll);

    switch (input) {
    case INPUT_KEY_MODE:
        return;
    case INPUT_TOUCH:
        func_ov068_021ba194(ui, POINT_START, &point);
        if (func_ov068_021bb508(&data_ov068_021bb550, &point) && appBar->button != BUTTON_CONFIRM) {
            GFL_BGSysSetBGEnabled(BG_MESSAGE, FALSE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            appBar->button = BUTTON_CONFIRM;
            appBar->decided = TRUE;
            appBar->touched = TRUE;
            if (!func_020355c4()) {
                func_020355d8(TRUE);
            }
        } else if (func_ov068_021bb508(&data_ov068_021bb560, &point) && appBar->button != BUTTON_QUIT) {
            GFL_BGSysSetBGEnabled(BG_MESSAGE, FALSE);
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            appBar->button = BUTTON_QUIT;
            appBar->decided = TRUE;
            appBar->touched = TRUE;
            if (!func_020355c4()) {
                func_020355d8(TRUE);
            }
        } else if (appBar->button != BUTTON_NONE) {
            appBar->button = BUTTON_NONE;
        }
        update = TRUE;
        break;
    case INPUT_UP:
    case INPUT_DOWN:
    case INPUT_RIGHT:
    case INPUT_LEFT:
    case INPUT_UP_HELD:
    case INPUT_DOWN_HELD:
        switch (cursor) {
        case CURSOR_CONFIRM:
            appBar->button = BUTTON_CONFIRM;
            break;
        case CURSOR_QUIT:
            appBar->button = BUTTON_QUIT;
            break;
        default:
            appBar->button = BUTTON_NONE;
            break;
        }
        update = TRUE;
        break;
    case INPUT_DECIDE:
        if (appBar->button != BUTTON_NONE) {
            if (appBar->button == BUTTON_QUIT) {
                GFL_SndSEPlay(SEQ_SE_CANCEL1);
            } else {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
            }
            update = TRUE;
            appBar->decided = TRUE;
        } else {
            appBar->button = BUTTON_CONFIRM;
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            update = TRUE;
            appBar->decided = TRUE;
        }
        break;
    case INPUT_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        update = TRUE;
        appBar->button = BUTTON_QUIT;
        appBar->decided = TRUE;
        break;
    }

    if (update) {
        if (appBar->button == BUTTON_NONE) {
            func_02028218(appBar->wins[BUTTON_CONFIRM], FALSE);
            func_02028218(appBar->wins[BUTTON_QUIT], FALSE);
        } else {
            if (appBar->decided) {
                func_02028218(appBar->wins[appBar->button], FALSE);
                func_0202822c(appBar->wins[appBar->button], TRUE);
                func_02028218(appBar->wins[appBar->button ^ 1], FALSE);
            } else {
                func_02028218(appBar->wins[appBar->button], TRUE);
                func_02028218(appBar->wins[appBar->button ^ 1], FALSE);
            }
        }
    }

    if (!appBar->decided) {
        func_ov067_021b63d0(appBar->touchBar);
        switch (func_ov067_021b6470(appBar->touchBar)) {
        case TOUCHBAR_ICON_CHECK:
            if (func_020355c4() == TRUE) {
                appBar->touched = TRUE;
            }
            break;
        case TOUCHBAR_ICON_CLOSE:
            if (func_020355c4() == TRUE) {
                appBar->touched = TRUE;
            }
            break;
        }
        switch (func_ov067_021b6448(appBar->touchBar)) {
        case TOUCHBAR_ICON_CHECK:
            if (func_ov067_021b65a8(appBar->touchBar, TOUCHBAR_ICON_CHECK)) {
                GameData_SetKeyItemRegistration(appBar->gameData, SHORTCUT_CONFIG_PANEL, TRUE);
            } else {
                GameData_SetKeyItemRegistration(appBar->gameData, SHORTCUT_CONFIG_PANEL, FALSE);
            }
            appBar->button = BUTTON_NONE;
            break;
        case TOUCHBAR_ICON_CLOSE:
            appBar->button = BUTTON_CLOSE;
            appBar->decided = TRUE;
            break;
        }
    }
}

// pokebw2's ConfigPanelAppBar_UpdateButtons
static void func_ov068_021ba644(ConfigPanelAppBar *appBar) {
    int i;

    for (i = 0; i < 2; i++) {
        func_02028178(appBar->wins[i]);
    }
}

// Returns whether a button was picked, and which
// pokebw2's ConfigPanelAppBar_GetDecided
BOOL func_ov068_021ba65c(ConfigPanelAppBar *appBar, s32 *button) {
    if (button != NULL) {
        *button = appBar->button;
    }
    return appBar->decided;
}

// pokebw2's ConfigPanelAppBar_IsFlashFinished
BOOL func_ov068_021ba668(ConfigPanelAppBar *appBar) {
    BOOL finished = FALSE;

    if (appBar->decided && appBar->button != BUTTON_NONE) {
        finished = func_02028230(appBar->wins[appBar->button]);
        if (finished) {
            func_ov068_021ba694(appBar);
        }
    }
    return finished;
}

// pokebw2's ConfigPanelAppBar_ResetButtons
void func_ov068_021ba694(ConfigPanelAppBar *appBar) {
    int i;

    for (i = 0; i < 2; i++) {
        func_020282a4(appBar->wins[i]);
        func_02028218(appBar->wins[i], FALSE);
    }
}

// pokebw2's ConfigPanelAppBar_CreateButtons
void func_ov068_021ba6b8(ConfigPanelAppBar *appBar, u16 heapId) {
    int i;

    for (i = 0; i < 2; i++) {
        if (appBar->items[i].str != NULL) {
            GFL_StrBufFree(appBar->items[i].str);
        }
        appBar->items[i].str = GFL_MsgDataLoadStrbufNew(appBar->msgData, Title2_Text_008 + i);
        appBar->items[i].color = PRINT_COLOR(14, 15, 3);
        appBar->items[i].type = i;
        if (appBar->wins[i] != NULL) {
            func_02028148(appBar->wins[i]);
        }
        appBar->wins[i] = func_02027fec(appBar->res, &appBar->items[i], 14 + i * 9, 21, 9, heapId);
    }
}

// pokebw2's ConfigPanelAppBar_IsTouched
BOOL func_ov068_021ba730(ConfigPanelAppBar *appBar) {
    return appBar->touched;
}

void func_ov068_021ba734(ConfigPanelScroll *scroll, u16 textBg, u16 rowsBg, ConfigPanelGraphic *graphic,
                                   const ConfigPanelOptions *options, HeapID heapId) {
    int i;

    MI_CpuFill8(scroll, 0, sizeof(ConfigPanelScroll));
    scroll->textBg = textBg;
    scroll->rowsBg = rowsBg;
    scroll->y = 16;
    scroll->options = *options;
    for (i = 0; i < ITEM_COUNT; i++) {
        scroll->actors[i] = func_ov068_021b9944(graphic, ACTOR_ITEM + i);
    }
    func_ov068_021babe0(scroll, -8, -192, 192);
    if (func_020355c4() == TRUE) {
        scroll->cursor = -1;
        func_ov068_021baa0c(scroll, FALSE);
        scroll->update = FALSE;
    } else {
        scroll->cursor = 0;
        func_ov068_021baa0c(scroll, TRUE);
        scroll->update = TRUE;
    }
}

// pokebw2's ConfigPanelScroll_Exit
void func_ov068_021ba7b4(ConfigPanelScroll *scroll) {
    MI_CpuFill8(scroll, 0, sizeof(ConfigPanelScroll));
}

void func_ov068_021ba7c0(ConfigPanelScroll *scroll, ConfigPanelUi *ui, ConfigPanelMsg *msg,
                                   ConfigPanelGraphic *graphic, ConfigPanelAppBar *appBar) {
    u32 input = func_ov068_021ba190(ui);
    ConfigPanelPoint point;

    GFL_BGSysGetBGOffsetY(scroll->rowsBg);
    switch (input) {
    case INPUT_KEY_MODE:
        scroll->cursor = MATH_CLAMP(scroll->cursor, 0, CURSOR_MAX);
        func_ov068_021baa0c(scroll, TRUE);
        scroll->update = TRUE;
        break;
    case INPUT_UP_HELD:
        if (scroll->repeat++ < 10) {
            break;
        }
        // fallthrough
    case INPUT_UP:
        scroll->repeat = 0;
        if (scroll->cursor == 0) {
            scroll->cursor = CURSOR_MAX;
        } else {
            scroll->cursor--;
        }
        scroll->update = TRUE;
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov068_021baa0c(scroll, TRUE);
        func_ov068_021b9964(graphic);
        break;
    case INPUT_DOWN_HELD:
        if (scroll->repeat++ < 10) {
            break;
        }
        // fallthrough
    case INPUT_DOWN:
        scroll->repeat = 0;
        if (scroll->cursor == CURSOR_MAX) {
            scroll->cursor = 0;
        } else {
            scroll->cursor++;
        }
        scroll->update = TRUE;
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov068_021baa0c(scroll, TRUE);
        func_ov068_021b9964(graphic);
        break;
    case INPUT_TOUCH:
        func_ov068_021ba194(ui, POINT_START, &point);
        if (point.y >= ROWS_TOP && point.y <= ROWS_BOTTOM) {
            scroll->cursor = func_ov068_021bad38(scroll, &point);
            func_ov068_021bac70(scroll, &point);
            func_ov068_021baa0c(scroll, FALSE);
            func_ov068_021b9964(graphic);
            scroll->update = TRUE;
            if (!func_020355c4()) {
                func_020355d8(TRUE);
            }
        }
        break;
    case INPUT_RIGHT:
        if (scroll->cursor < OPTION_COUNT) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov068_021bad14(scroll, 1);
            func_ov068_021baa0c(scroll, TRUE);
            func_ov068_021b9964(graphic);
        } else if (scroll->cursor == CURSOR_CONFIRM) {
            scroll->cursor++;
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            scroll->cursor = MATH_CLAMP(scroll->cursor, 0, CURSOR_MAX);
        }
        break;
    case INPUT_LEFT:
        if (scroll->cursor < OPTION_COUNT) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov068_021bad14(scroll, -1);
            func_ov068_021baa0c(scroll, TRUE);
            func_ov068_021b9964(graphic);
        } else if (scroll->cursor == CURSOR_QUIT) {
            scroll->cursor--;
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            scroll->cursor = MATH_CLAMP(scroll->cursor, 0, CURSOR_MAX);
        }
        break;
    case INPUT_Y:
        func_ov068_021baa0c(scroll, TRUE);
        break;
    case INPUT_DECIDE:
    case INPUT_CANCEL:
        func_ov068_021baa0c(scroll, FALSE);
        break;
    }

    // A new text speed shows in the explanation
    switch (func_ov068_021bb0d8(&scroll->options)) {
    case 0:
    case 1:
    case 2:
        scroll->update = TRUE;
        break;
    }

    if (scroll->update) {
        if (scroll->cursor >= 0 && scroll->cursor < OPTION_COUNT) {
            BOOL stream;
            s32 wait;

            if (scroll->cursor == OPTION_TEXT_SPEED) {
                stream = TRUE;
                wait = func_ov068_021bb0d0(&scroll->options);
            } else {
                stream = FALSE;
                wait = 0;
            }
            GFL_BGSysSetBGEnabled(BG_MESSAGE, TRUE);
            func_ov068_021ba278(msg, data_ov068_021bb570[scroll->cursor].msgId, wait, stream);
        } else {
            GFL_BGSysSetBGEnabled(BG_MESSAGE, FALSE);
        }
        scroll->update = FALSE;
    }
    func_ov068_021ba31c(msg);
    func_ov068_021bad58(scroll);
}

// pokebw2's ConfigPanelScroll_HideCursor
void func_ov068_021ba9c0(ConfigPanelScroll *scroll, ConfigPanelGraphic *graphic) {
    func_ov068_021baa0c(scroll, FALSE);
    GFL_BGSysSetBGEnabled(BG_MESSAGE, FALSE);
    func_ov068_021b9964(graphic);
}

// pokebw2's ConfigPanelScroll_GetCursor
s32 func_ov068_021ba9dc(ConfigPanelScroll *scroll) {
    return scroll->cursor;
}

// pokebw2's ConfigPanelScroll_GetOptions
void func_ov068_021ba9e0(ConfigPanelScroll *scroll, ConfigPanelOptions *options) {
    *options = scroll->options;
}

// pokebw2's ConfigPanelScroll_Redraw
void func_ov068_021ba9f0(ConfigPanelScroll *scroll, BOOL unused) {
    func_ov068_021baa0c(scroll, scroll->cursorVisible);
}

// pokebw2's ConfigPanelScroll_SetSaveBeforeIr
void func_ov068_021ba9fc(ConfigPanelScroll *scroll, u8 value) {
    scroll->options.values[OPTION_SAVE_BEFORE_IR] = value;
    func_ov068_021baa0c(scroll, scroll->cursorVisible);
}

// Sets the palettes of the rows, the texts and the values' frames for the cursor and the values picked
// pokebw2's ConfigPanelScroll_Draw
void func_ov068_021baa0c(ConfigPanelScroll *scroll, BOOL cursorVisible) {
    int row;
    u32 j;
    int i;

    if (scroll->cursor >= OPTION_COUNT) {
        cursorVisible = FALSE;
    }
    scroll->cursorVisible = cursorVisible;

    row = (GFL_BGSysGetBGOffsetY(scroll->rowsBg) + 20) / 24;
    GFL_BGSysSetScrPaletteNo(scroll->rowsBg, 0, 0, 32, 32, 5);
    for (j = 0; j < OPTION_COUNT; j++) {
        GFL_BGSysSetScrPaletteNo(scroll->rowsBg, 0, (row + j) * 3 + 2, 32, 3, data_ov068_021bb538[j]);
    }

    for (i = 0; i < ITEM_COUNT; i++) {
        if (scroll->cursor == data_ov068_021bb704[i].option && cursorVisible) {
            if (func_ov068_021bb0ac(&scroll->options, i)) {
                GFL_BGSysSetScrPaletteNo(scroll->textBg, data_ov068_021bb820[i].x, data_ov068_021bb820[i].y,
                                         data_ov068_021bb820[i].width, data_ov068_021bb820[i].height, 7);
            } else {
                GFL_BGSysSetScrPaletteNo(scroll->textBg, data_ov068_021bb820[i].x, data_ov068_021bb820[i].y,
                                         data_ov068_021bb820[i].width, data_ov068_021bb820[i].height, 12);
            }
        } else {
            if (func_ov068_021bb0ac(&scroll->options, i)) {
                GFL_BGSysSetScrPaletteNo(scroll->textBg, data_ov068_021bb820[i].x, data_ov068_021bb820[i].y,
                                         data_ov068_021bb820[i].width, data_ov068_021bb820[i].height, 0);
            } else {
                GFL_BGSysSetScrPaletteNo(scroll->textBg, data_ov068_021bb820[i].x, data_ov068_021bb820[i].y,
                                         data_ov068_021bb820[i].width, data_ov068_021bb820[i].height, 3);
            }
        }
    }
    for (i = WINDOW_OPTION; i < LABEL_COUNT; i++) {
        if (i - WINDOW_OPTION == scroll->cursor && cursorVisible) {
            GFL_BGSysSetScrPaletteNo(scroll->textBg, data_ov068_021bb820[i].x, data_ov068_021bb820[i].y,
                                     data_ov068_021bb820[i].width, data_ov068_021bb820[i].height, 7);
        } else {
            GFL_BGSysSetScrPaletteNo(scroll->textBg, data_ov068_021bb820[i].x, data_ov068_021bb820[i].y,
                                     data_ov068_021bb820[i].width, data_ov068_021bb820[i].height, 0);
        }
    }
    if (cursorVisible) {
        GFL_BGSysSetScrPaletteNo(scroll->rowsBg, 0, scroll->cursor * 3 + 2, 32, 3, 7);
    }
    GFL_BGSysQueueScrLoad(scroll->rowsBg);
    GFL_BGSysQueueScrLoad(scroll->textBg);

    for (i = 0; i < ITEM_COUNT; i++) {
        if (scroll->cursor == data_ov068_021bb704[i].option && cursorVisible) {
            if (func_ov068_021bb0ac(&scroll->options, i)) {
                func_0204b8a0(scroll->actors[i], 3);
            } else {
                func_0204b8a0(scroll->actors[i], 2);
            }
        } else {
            if (func_ov068_021bb0ac(&scroll->options, i)) {
                func_0204b8a0(scroll->actors[i], 0);
            } else {
                func_0204b8a0(scroll->actors[i], 1);
            }
        }
    }
}

// Scrolls the rows by dy, within min and max, and moves the values' frames with them
// pokebw2's ConfigPanelScroll_Move
void func_ov068_021babe0(ConfigPanelScroll *scroll, int dy, int min, int max) {
    int i;
    ClActorPos pos;
    int y = GFL_BGSysGetBGOffsetY(scroll->rowsBg);

    y += dy;
    y = MATH_CLAMP(y, min, max);
    GFL_BGSysMoveBGReq(scroll->textBg, 3, y + 20);
    GFL_BGSysMoveBGReq(scroll->rowsBg, 3, y);

    scroll->y -= dy;
    scroll->y = MATH_CLAMP(scroll->y, 16 - max, 16 - min);
    for (i = 0; i < ITEM_COUNT; i++) {
        pos.x = data_ov068_021bb704[i].x;
        pos.y = scroll->y + data_ov068_021bb704[i].y;
        func_0204b404(scroll->actors[i], &pos, 0);
    }
}

// Picks the value at a point of the touch screen
// pokebw2's ConfigPanelScroll_TouchItems
void func_ov068_021bac70(ConfigPanelScroll *scroll, const ConfigPanelPoint *point) {
    int i;
    ConfigPanelRect rect;

    for (i = 0; i < ITEM_COUNT; i++) {
        rect.top = scroll->y + data_ov068_021bb704[i].hitY;
        rect.left = data_ov068_021bb704[i].hitX;
        rect.bottom = rect.top + data_ov068_021bb704[i].hitHeight;
        rect.right = rect.left + data_ov068_021bb704[i].hitWidth;
        if (rect.top >= ROWS_TOP && rect.top <= ROWS_BOTTOM && rect.bottom >= ROWS_TOP && rect.bottom <= ROWS_BOTTOM &&
            func_ov068_021bb508(&rect, point)) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov068_021bafd8(&scroll->options, i);
            func_ov068_021bacf4(scroll, scroll->options.values[OPTION_SOUND], FALSE);
            return;
        }
    }
}

// Switches the sound output while the cursor is on the option, so the change is heard, or always when forced
// pokebw2's ConfigPanelScroll_ApplySound
void func_ov068_021bacf4(ConfigPanelScroll *scroll, s8 sound, BOOL force) {
    if (scroll->cursor == OPTION_SOUND || force == TRUE) {
        if (sound == 0) {
            func_02005c28(TRUE);
        } else {
            func_02005c28(FALSE);
        }
    }
}

// pokebw2's ConfigPanelScroll_Step
void func_ov068_021bad14(ConfigPanelScroll *scroll, int dir) {
    if (scroll->cursor < OPTION_COUNT) {
        func_ov068_021bb000(&scroll->options, scroll->cursor, dir);
        func_ov068_021bacf4(scroll, scroll->options.values[scroll->cursor], FALSE);
    }
}

// The row at a point of the touch screen, or the cursor when it is on none
// pokebw2's ConfigPanelScroll_GetTouchedRow
s32 func_ov068_021bad38(ConfigPanelScroll *scroll, const ConfigPanelPoint *point) {
    int y = GFL_BGSysGetBGOffsetY(scroll->rowsBg);
    u32 row = (y + point->y - 16) / 24;

    if (row >= OPTION_COUNT) {
        row = scroll->cursor;
    }
    return row;
}

// pokebw2's ConfigPanelScroll_Tick
void func_ov068_021bad58(ConfigPanelScroll *scroll) {
    scroll->unk3C *= 2;
}

void func_ov068_021bad60(ConfigPanelConfirm *confirm, BmpWin *window, Font *font, MsgData *msgData,
                                    TCBExManager *tcbManager, AppTaskMenuRes *res, PrintQueue *queue, u16 heapId) {
    int i;

    MI_CpuFill8(confirm, 0, sizeof(ConfigPanelConfirm));
    confirm->result = CONFIRM_NONE;
    confirm->res = res;
    func_ov068_021ba1c4(&confirm->msg, window, font, msgData, tcbManager, queue, heapId);
    for (i = 0; i < 2; i++) {
        confirm->items[i].str = GFL_MsgDataLoadStrbufNew(msgData, Title2_Text_034 + i);
        confirm->items[i].color = PRINT_COLOR(14, 15, 3);
        confirm->items[i].type = 0;
    }
    confirm->init.heapId = heapId;
    confirm->init.itemCount = 2;
    confirm->init.items = confirm->items;
    confirm->init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    confirm->init.x = 32;
    confirm->init.y = 12;
    confirm->init.width = 13;
    confirm->init.height = 3;
}

// pokebw2's ConfigPanelConfirm_Exit
void func_ov068_021badfc(ConfigPanelConfirm *confirm) {
    int i;

    for (i = 0; i < 2; i++) {
        GFL_StrBufFree(confirm->items[i].str);
    }
    func_ov068_021ba254(&confirm->msg);
    MI_CpuFill8(confirm, 0, sizeof(ConfigPanelConfirm));
}

// pokebw2's ConfigPanelConfirm_Main
void func_ov068_021bae28(ConfigPanelConfirm *confirm) {
    switch (confirm->state) {
    case 0:
        if (func_ov068_021ba31c(&confirm->msg)) {
            confirm->state = 1;
        }
        break;
    case 1:
        confirm->menu = func_02027778(&confirm->init, confirm->res);
        confirm->state = 2;
        break;
    case 2:
        func_02027974(confirm->menu);
        if (func_020279e8(confirm->menu)) {
            confirm->result = func_02027a04(confirm->menu) + 1;
            confirm->state = 3;
            func_02027858(confirm->menu);
        }
        break;
    }
}

// Asks whether to make the changes
// pokebw2's ConfigPanelConfirm_Start
void func_ov068_021bae90(ConfigPanelConfirm *confirm, s32 wait) {
    G2_SetBlendBrightness(
        GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    GFL_BGSysSetBGEnabled(BG_CONFIRM, TRUE);
    func_ov068_021ba278(&confirm->msg, Title2_Text_033, wait, FALSE);
}

// pokebw2's ConfigPanelConfirm_GetResult
u32 func_ov068_021baebc(ConfigPanelConfirm *confirm) {
    return confirm->result;
}

// Prints a message of the script's text over the faded screen
// pokebw2's ConfigPanelConfirm_PrintScriptMsg
void func_ov068_021baec4(ConfigPanelConfirm *confirm, u32 msgId, s32 wait) {
    MsgData *msgData;

    G2_SetBlendBrightness(
        GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    GFL_BGSysSetBGEnabled(BG_CONFIRM, TRUE);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SCRIPT_MESSAGE, SCRIPT_TEXT_GLOBAL_10520,
                                 HEAPID_CONFIG_PANEL | HEAPID_TAIL_BIT);
    func_ov068_021ba28c(&confirm->msg, msgData, msgId, wait, TRUE);
    GFL_MsgDataFree(msgData);
}

// pokebw2's ConfigPanelConfirm_PrintMsg
void func_ov068_021baf0c(ConfigPanelConfirm *confirm, u32 msgId, s32 wait) {
    G2_SetBlendBrightness(
        GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    GFL_BGSysSetBGEnabled(BG_CONFIRM, TRUE);
    func_ov068_021ba278(&confirm->msg, msgId, wait, TRUE);
}

// pokebw2's ConfigPanelConfirm_IsMsgDone
BOOL func_ov068_021baf38(ConfigPanelConfirm *confirm) {
    if (func_ov068_021ba31c(&confirm->msg)) {
        G2_BlendNone();
        GFL_BGSysSetBGEnabled(BG_CONFIRM, FALSE);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ConfigPanelOptions_Init
void func_ov068_021baf5c(ConfigPanelOptions *options, Config *config) {
    MI_CpuFill8(options, 0, sizeof(ConfigPanelOptions));
    options->changed = ITEM_COUNT;
    options->config = config;
    options->values[OPTION_TEXT_SPEED] = func_02008374(config);
    options->values[OPTION_BATTLE_SCENE] = func_020083ac(config);
    options->values[OPTION_BATTLE_STYLE] = func_020083c8(config);
    options->values[OPTION_SOUND] = getSoundFlags(config);
    options->values[OPTION_SAVE_BEFORE_IR] = func_02008428(config);
}

// pokebw2's ConfigPanelOptions_Exit
void func_ov068_021baf9c(ConfigPanelOptions *options) {
    MI_CpuFill8(options, 0, sizeof(ConfigPanelOptions));
}

// Writes the options to the save's
// pokebw2's ConfigPanelOptions_Apply
void func_ov068_021bafa8(const ConfigPanelOptions *options) {
    func_0200837c(options->config, options->values[OPTION_TEXT_SPEED]);
    func_020083b4(options->config, options->values[OPTION_BATTLE_SCENE]);
    func_020083d0(options->config, options->values[OPTION_BATTLE_STYLE]);
    func_02008398(options->config, options->values[OPTION_SOUND]);
    func_02008430(options->config, options->values[OPTION_SAVE_BEFORE_IR]);
}

// pokebw2's ConfigPanelOptions_SetItem
void func_ov068_021bafd8(ConfigPanelOptions *options, u32 item) {
    if (options->values[data_ov068_021bb704[item].option] != data_ov068_021bb704[item].value) {
        options->values[data_ov068_021bb704[item].option] = data_ov068_021bb704[item].value;
        options->changed = item;
        func_ov068_021bafa8(options);
    }
}

// Moves an option to the value before or after its own
// pokebw2's ConfigPanelOptions_Step
void func_ov068_021bb000(ConfigPanelOptions *options, u32 option, int dir) {
    int i;
    int item = -1;
    s8 pos;

    for (i = 0; i < data_ov068_021bb570[option].count; i++) {
        item = data_ov068_021bb570[option].items[i];
        if (data_ov068_021bb704[item].value == options->values[option]) {
            break;
        }
    }
    pos = item - data_ov068_021bb570[option].items[0];
    pos = pos + dir;
    pos = MATH_CLAMP(pos, 0, data_ov068_021bb570[option].count - 1);
    item = data_ov068_021bb570[option].items[pos];
    if (options->values[option] != data_ov068_021bb704[item].value) {
        options->values[option] = data_ov068_021bb704[item].value;
        options->changed = item;
        func_ov068_021bafa8(options);
    }
}

// pokebw2's ConfigPanelOptions_IsSet
BOOL func_ov068_021bb0ac(const ConfigPanelOptions *options, u32 item) {
    if (options->values[data_ov068_021bb704[item].option] == data_ov068_021bb704[item].value) {
        return TRUE;
    }
    return FALSE;
}

// The wait between characters of the text speed, which func_ov068_021bafa8 has already written to the save
// pokebw2's ConfigPanelOptions_GetTextWait
s32 func_ov068_021bb0d0(const ConfigPanelOptions *options) {
    return func_02012ffc();
}

// pokebw2's ConfigPanelOptions_PopChanged
u32 func_ov068_021bb0d8(ConfigPanelOptions *options) {
    u32 changed = options->changed;

    options->changed = ITEM_COUNT;
    return changed;
}

// pokebw2's ConfigPanelOptions_Equals
BOOL func_ov068_021bb0e4(const ConfigPanelOptions *a, const ConfigPanelOptions *b) {
    if (GFL_STD_MemCmp(a, b, OPTION_COUNT) == 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ConfigPanelSeq_Init
void func_ov068_021bb0f8(ConfigPanelSeq *seq, void *data, ConfigPanelSeqFunc func) {
    MI_CpuFill8(seq, 0, sizeof(ConfigPanelSeq));
    seq->data = data;
    func_ov068_021bb138(seq, func);
}

// pokebw2's ConfigPanelSeq_Exit
void func_ov068_021bb114(ConfigPanelSeq *seq) {
    MI_CpuFill8(seq, 0, sizeof(ConfigPanelSeq));
}

// pokebw2's ConfigPanelSeq_Main
static void func_ov068_021bb120(ConfigPanelSeq *seq) {
    if (!seq->ended) {
        seq->func(seq, &seq->state, seq->data);
    }
}

// pokebw2's ConfigPanelSeq_IsEnded
static BOOL func_ov068_021bb134(ConfigPanelSeq *seq) {
    return seq->ended;
}

// pokebw2's ConfigPanelSeq_SetNext
void func_ov068_021bb138(ConfigPanelSeq *seq, ConfigPanelSeqFunc func) {
    seq->func = func;
    seq->state = 0;
}

// pokebw2's ConfigPanelSeq_End
void func_ov068_021bb140(ConfigPanelSeq *seq) {
    seq->ended = TRUE;
}

// pokebw2's ConfigPanelSeq_FadeIn
void func_ov068_021bb148(ConfigPanelSeq *seq, u32 *state, void *data) {
    switch (*state) {
    case 0:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
        *state = 1;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            *state = 2;
        }
        break;
    case 2:
        func_ov068_021bb138(seq, func_ov068_021bb1c4);
        break;
    }
}

// pokebw2's ConfigPanelSeq_FadeOut
void func_ov068_021bb188(ConfigPanelSeq *seq, u32 *state, void *data) {
    switch (*state) {
    case 0:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 0);
        *state = 1;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            *state = 2;
        }
        break;
    case 2:
        func_ov068_021bb140(seq);
        break;
    }
}

// pokebw2's ConfigPanelSeq_Input
void func_ov068_021bb1c4(ConfigPanelSeq *seq, u32 *state, void *data) {
    ConfigPanelWork *wk = data;
    s32 button;

    if (!func_0201c6b4(wk->queue)) {
        return;
    }

    switch (*state) {
    case 0:
        func_ov068_021ba03c(&wk->ui);
        if (!func_ov068_021ba65c(&wk->appBar, NULL)) {
            func_ov068_021ba7c0(&wk->scroll, &wk->ui, &wk->msg, &wk->graphic, &wk->appBar);
        }
        func_ov068_021ba448(&wk->appBar, &wk->ui, &wk->scroll);
        if (func_ov068_021ba730(&wk->appBar)) {
            func_ov068_021ba9c0(&wk->scroll, &wk->graphic);
        }

        if (func_ov068_021ba65c(&wk->appBar, &button)) {
            switch (button) {
            case BUTTON_CONFIRM:
                if (func_ov068_021ba668(&wk->appBar)) {
                    func_ov068_021bb138(seq, func_ov068_021bb4d4);
                }
                break;
            case BUTTON_CLOSE: {
                ConfigPanelOptions options;

                wk->param->result = TRUE;
                func_ov068_021ba9e0(&wk->scroll, &options);
                if (func_ov068_021bb0e4(&wk->saved, &options)) {
                    func_ov068_021bb138(seq, func_ov068_021bb4a4);
                } else {
                    func_ov068_021ba694(&wk->appBar);
                    func_ov068_021bb138(seq, func_ov068_021bb418);
                }
                break;
            }
            case BUTTON_QUIT:
                if (func_ov068_021ba668(&wk->appBar)) {
                    ConfigPanelOptions options;

                    func_ov068_021ba9e0(&wk->scroll, &options);
                    if (func_ov068_021bb0e4(&wk->saved, &options)) {
                        func_ov068_021bb138(seq, func_ov068_021bb4a4);
                    } else {
                        func_ov068_021ba694(&wk->appBar);
                        func_ov068_021bb138(seq, func_ov068_021bb418);
                    }
                }
                break;
            }
        }

        // The save of another game can't be overwritten, so there is nothing to save before infrared
        {
            BOOL otherSave = func_020071f0(GameData_GetSaveControl(GSYS_GetGameData(wk->param->gsys)));
            ConfigPanelOptions options;

            func_ov068_021ba9e0(&wk->scroll, &options);
            if (options.values[OPTION_SAVE_BEFORE_IR] == 0 && otherSave) {
                *state = 1;
            }
        }
        break;
    case 1: {
        ConfigPanelOptions options;

        func_ov068_021ba9e0(&wk->scroll, &options);
        func_ov068_021baf0c(&wk->confirm, Title2_Text_032,
                                    func_ov068_021bb0d0(&options));
        func_ov068_021ba9fc(&wk->scroll, 1);
        *state = 5;
        break;
    }
    case 2: {
        ConfigPanelOptions options;

        func_ov068_021ba9e0(&wk->scroll, &options);
        func_ov068_021baec4(&wk->confirm, Global10520_Text_022,
                                          func_ov068_021bb0d0(&options));
        func_ov068_021ba9f0(&wk->scroll, FALSE);
        *state = 5;
        break;
    }
    case 3: {
        ConfigPanelOptions options;

        func_ov068_021ba9e0(&wk->scroll, &options);
        func_ov068_021baec4(&wk->confirm, Global10520_Text_022,
                                          func_ov068_021bb0d0(&options));
        func_ov068_021ba9f0(&wk->scroll, FALSE);
        *state = 5;
        break;
    }
    case 4: {
        ConfigPanelOptions options;

        func_ov068_021ba9e0(&wk->scroll, &options);
        func_ov068_021baf0c(&wk->confirm, Title2_Text_036, func_ov068_021bb0d0(&options));
        func_ov068_021ba9f0(&wk->scroll, TRUE);
        *state = 5;
        break;
    }
    case 5:
        if (func_ov068_021baf38(&wk->confirm)) {
            *state = 0;
        }
        break;
    }
}

// Asks whether to make the changes, then saves them or quits
// pokebw2's ConfigPanelSeq_Confirm
void func_ov068_021bb418(ConfigPanelSeq *seq, u32 *state, void *data) {
    ConfigPanelWork *wk = data;

    switch (*state) {
    case 0: {
        ConfigPanelOptions options;

        func_ov068_021ba9e0(&wk->scroll, &options);
        func_ov068_021bae90(&wk->confirm, func_ov068_021bb0d0(&options));
        GFL_BGSysSetBGEnabled(BG_MESSAGE, FALSE);
        *state = 1;
        break;
    }
    case 1:
        switch (func_ov068_021baebc(&wk->confirm)) {
        case CONFIRM_YES:
            *state = 2;
            break;
        case CONFIRM_NO:
            *state = 3;
            break;
        }
        break;
    case 2:
        func_ov068_021bb138(seq, func_ov068_021bb4d4);
        break;
    case 3:
        func_ov068_021bb138(seq, func_ov068_021bb4a4);
        break;
    }
    func_ov068_021bae28(&wk->confirm);
}

// Puts the options back as they were
// pokebw2's ConfigPanelSeq_Quit
void func_ov068_021bb4a4(ConfigPanelSeq *seq, u32 *state, void *data) {
    ConfigPanelWork *wk = data;

    func_ov068_021bafa8(&wk->saved);
    func_ov068_021bacf4(&wk->scroll, wk->saved.values[OPTION_SOUND], TRUE);
    func_ov068_021bb138(seq, func_ov068_021bb188);
}

// pokebw2's ConfigPanelSeq_Save
void func_ov068_021bb4d4(ConfigPanelSeq *seq, u32 *state, void *data) {
    ConfigPanelWork *wk = data;
    ConfigPanelOptions options;

    func_ov068_021ba9e0(&wk->scroll, &options);
    func_ov068_021bafa8(&options);
    GFL_SndSEPlay(SEQ_SE_SAVE);
    func_ov068_021bb138(seq, func_ov068_021bb188);
}

// pokebw2's ConfigPanel_IsInRect
BOOL func_ov068_021bb508(const ConfigPanelRect *rect, const ConfigPanelPoint *point) {
    return ((u32)(point->x - rect->left) <= (u32)(rect->right - rect->left)) &
           ((u32)(point->y - rect->top) <= (u32)(rect->bottom - rect->top));
}
