// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov310/research_list.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/research_radar/bg_font.h"
#include "app/research_radar/palette_anime.h"
#include "app/research_radar/queue.h"
#include "app/research_radar/research_common.h"
#include "app/research_radar/research_list.h"
#include "app/research_radar/research_list_recovery.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "field/survey.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "nitro/math.h"
#include "nnsys/g2d.h"
#include "save/event_work.h"
#include "system/bmp_oam.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "save/survey_savedata.h"

// The Research Radar's list of surveys, one for each that is unlocked, which scrolls by dragging its bar or with the
// cursor. Choosing a survey other than the one being run asks whether to run it instead, and running it makes it the
// survey of the save. The survey being run is drawn a tile to the left of the others, with an icon

// The sequences of the screen, which run one after another from a queue
enum {
    SEQ_SETUP,
    SEQ_TOUCH,
    SEQ_CURSOR,
    // Moves the cursor to the next survey, scrolling the list to it
    SEQ_CURSOR_SCROLL,
    // Scrolls the list while its bar is held, and on after it is released
    SEQ_DRAG,
    SEQ_RETURN_TOUCH,
    SEQ_RETURN_CURSOR,
    // Scrolls the chosen survey to the middle and asks whether to run it
    SEQ_CONFIRM_OPEN_TOUCH,
    SEQ_CONFIRM_TOUCH,
    SEQ_CONFIRM_OPEN_CURSOR,
    SEQ_CONFIRM_CURSOR,
    SEQ_CONFIRM_CANCEL,
    // Runs the chosen survey
    SEQ_DECIDE,
    SEQ_TEARDOWN,
    SEQ_END,
};

// The BGs: the main engine's frame on BG 0, the list on BG 2 and its text on BG 3, and the sub engine's title on BG 6
// and its text on BG 7, in front of the proc's BGs
#define BG_MAIN_FRAME 0
#define BG_MAIN_LIST 2
#define BG_MAIN_TEXT 3
#define BG_SUB_TITLE 6
#define BG_SUB_TEXT 7

// The objects' resources: the main engine's, the palette of the buttons, which comes from the graphics the apps
// share, and the sub engine's
enum {
    OBJ_RES_MAIN_CHARS,
    OBJ_RES_MAIN_PALETTE,
    OBJ_RES_MAIN_CELL_ANIMS,
    OBJ_RES_BUTTON_PALETTE,
    OBJ_RES_SUB_CHARS,
    OBJ_RES_SUB_PALETTE,
    OBJ_RES_SUB_CELL_ANIMS,
    OBJ_RES_COUNT,
};

// The units of the actors, and the one of the bitmaps' actors
#define UNIT_COUNT 3
#define UNIT_BMP_OAM 2

// The actors: the scroll bar's knob, an animation at the middle of the screen, and the icon of the survey being run
enum {
    ACTOR_SCROLL_BAR,
    ACTOR_CENTER,
    ACTOR_CURRENT,
    ACTOR_COUNT,
};

// The bitmaps shown as actors: the question whether to run a survey, its answers, and the message that it runs
enum {
    BMP_QUESTION,
    BMP_YES,
    BMP_NO,
    BMP_DECIDED,
    BMP_COUNT,
};

// The buttons of the question, in the order of its cursor
enum {
    BUTTON_YES,
    BUTTON_NO,
    BUTTON_COUNT,
};

// The palette animations: the list's cursor, a cursor move and a survey chosen, and the same for the question's
// buttons
enum {
    ANIME_CURSOR,
    ANIME_MOVE,
    ANIME_DECIDE,
    ANIME_BUTTON_CURSOR,
    ANIME_BUTTON_MOVE,
    ANIME_BUTTON_DECIDE,
    ANIME_COUNT,
};

// The texts of the sub screen: its title and help, and the chosen survey's name, description and three questions
enum {
    BG_FONT_TITLE,
    BG_FONT_HELP,
    BG_FONT_NAME,
    BG_FONT_DESCRIPTION,
    BG_FONT_QUESTION_1,
    BG_FONT_QUESTION_2,
    BG_FONT_QUESTION_3,
    BG_FONT_COUNT,
};

#define MSG_DATA_COUNT 2
#define SURVEY_COUNT 10
#define QUESTION_COUNT 3
#define BUTTON_RECT_COUNT 3
#define LIST_RECT_COUNT 11
#define SCROLL_RECT_COUNT 2

// The value of a survey index when there is none
#define SURVEY_NONE 0xff

// The list's layout: each survey is 24 pixels high, 3 rows of tiles, and the list shows from y 24 to 168
#define ITEM_HEIGHT 24
#define ITEM_ROWS 3
#define VIEW_TOP 24
#define VIEW_BOTTOM 168
// The BG scroll that shows the end of the list of all the surveys, which the saved scroll is kept within
#define MAX_BG_SCROLL (SURVEY_COUNT * ITEM_HEIGHT - VIEW_BOTTOM)

// The scroll bar's knob moves between these y
#define BAR_TOP 32
#define BAR_BOTTOM 160

// The fastest the list goes on scrolling after its bar is released
#define DRAG_SPEED_MAX 9

// The frames a direction is held before the cursor repeats
#define KEY_REPEAT_FRAMES 10

// The frames after a survey is run before the screen fades out
#define DECIDE_WAIT_FRAMES 60

// The first survey is always there, and each of these event flags unlocks one more
#define FIRST_SURVEY_FLAG 0xf0 // Black's
#define LAST_SURVEY_FLAG 0xf8 // Black's

// A text color of the text renderer, from each color's index

struct ResearchList {
    ResearchCommon *common;
    ResearchListRecovery *recovery;
    HeapID heapId;
    Font *font;
    MsgData *msgData[MSG_DATA_COUNT];
    Queue *queue;
    u32 seq;
    u32 seqState;
    u32 seqFrames;
    u32 waitFrames;
    // The cursor of the question whether to run a survey
    int buttonCursor;
    u8 surveyCount;
    // The survey chosen, and the one being run once the screen ends
    u8 selected;
    u8 cursor;
    // Where the cursor goes when it scrolls the list
    u8 cursorTarget;
    // The y in the list that the view follows, and its last value
    int scrollY;
    int prevScrollY;
    // A scroll in progress, from scrollFrom to scrollTo over scrollFrames frames
    int scrollFrom;
    int scrollTo;
    int scrollFrames;
    int scrollFrame;
    // The speed the list goes on scrolling at after its bar is released, and how it slows down
    int dragSpeed;
    int dragAccel;
    u32 keyRepeatFrames;
    TouchRect buttonRects[BUTTON_RECT_COUNT];
    TouchRect listRects[LIST_RECT_COUNT];
    TouchRect scrollRects[SCROLL_RECT_COUNT];
    PaletteFade *paletteFade;
    PaletteAnime *animes[ANIME_COUNT];
    BGFont *itemFonts[SURVEY_COUNT];
    BGFont *bgFonts[BG_FONT_COUNT];
    u32 objRes[OBJ_RES_COUNT];
    ClActUnit *units[UNIT_COUNT];
    ClActor *actors[ACTOR_COUNT];
    BmpOamSys *bmpOamSys;
    BmpOamActor *bmpOamActors[BMP_COUNT];
    GFLBitmap *bitmaps[BMP_COUNT];
    TCBManager *tcbMgr;
    TCB *vblankTask;
    BOOL seqDone;
    // Whether the rest of the screen is darkened, behind the question
    BOOL darkened;
    BOOL cursorShown;
    BOOL isEnd;
    u32 next;
};

typedef struct {
    BGFontWindow window;
    u32 msgDataIndex;
    u32 strId;
    BOOL centered;
} BGFontEntry;

// A bitmap shown as an actor, with the text drawn into it, over a picture loaded from an archive when hasBase is set
typedef struct {
    u32 width;
    u32 height;
    u32 tileSize;
    u32 msgDataIndex;
    u32 strId;
    u32 textX;
    u32 textY;
    u8 letterColor;
    u8 shadowColor;
    u8 backColor;
    u32 arcId;
    u32 fileId;
    BOOL hasBase;
} BitmapEntry;

// The actor of a bitmap: where it is, and its palette resource and offset
typedef struct {
    s16 x;
    s16 y;
    u32 palette;
    u32 palOffset;
    u8 priority;
    u8 bgPriority;
    u16 surface;
    u32 vramType;
} BmpOamEntry;

static void func_ov217_021baca0(ResearchList *wk);
static void func_ov217_021bae00(ResearchList *wk);
static void func_ov217_021baf70(ResearchList *wk);
static void func_ov217_021bb190(ResearchList *wk);
static void func_ov217_021bb200(ResearchList *wk);
static void func_ov217_021bb2b4(ResearchList *wk);
static void func_ov217_021bb37c(ResearchList *wk);
static void func_ov217_021bb484(ResearchList *wk);
static void func_ov217_021bb534(ResearchList *wk);
static void func_ov217_021bb5f4(ResearchList *wk);
static void func_ov217_021bb69c(ResearchList *wk);
static void func_ov217_021bb710(ResearchList *wk);
static void func_ov217_021bb784(ResearchList *wk);
static void func_ov217_021bb84c(ResearchList *wk);
static void func_ov217_021bb908(ResearchList *wk);
static void func_ov217_021bb910(ResearchList *wk, u32 seq);
static void func_ov217_021bb91c(ResearchList *wk);
static BOOL func_ov217_021bb928(ResearchList *wk);
static void func_ov217_021bb934(ResearchList *wk, BOOL touch);
static void func_ov217_021bb940(ResearchList *wk, u32 next);
static u32 func_ov217_021bb948(ResearchList *wk);
static void func_ov217_021bb94c(ResearchList *wk);
static u32 func_ov217_021bb974(ResearchList *wk);
static void func_ov217_021bb978(ResearchList *wk, u32 seq);
static u32 func_ov217_021bb98c(ResearchList *wk);
static void func_ov217_021bb990(ResearchList *wk);
static void func_ov217_021bb998(ResearchList *wk);
static void func_ov217_021bb9a0(ResearchList *wk);
static void func_ov217_021bb9d0(ResearchList *wk);
static void func_ov217_021bba00(ResearchList *wk, int pos);
static void func_ov217_021bba30(ResearchList *wk);
static void func_ov217_021bbaa4(ResearchList *wk, u8 pos);
static void func_ov217_021bbad0(ResearchList *wk);
static void func_ov217_021bbadc(ResearchList *wk);
static void func_ov217_021bbb60(ResearchList *wk);
static void func_ov217_021bbbe4(ResearchList *wk);
static u8 func_ov217_021bbc50(ResearchList *wk);
static void func_ov217_021bbcb0(ResearchList *wk);
static void func_ov217_021bbcfc(ResearchList *wk, int dir);
static void func_ov217_021bbd14(ResearchList *wk, int pos);
static void func_ov217_021bbd18(ResearchList *wk, u32 button);
static void func_ov217_021bbd28(ResearchList *wk, u32 button);
static void func_ov217_021bbd38(ResearchList *wk, u8 pos);
static void func_ov217_021bbd40(ResearchList *wk, int dir);
static void func_ov217_021bbd68(ResearchList *wk);
static void func_ov217_021bbdac(ResearchList *wk);
static void func_ov217_021bbdf0(ResearchList *wk, u8 index);
static void func_ov217_021bbe7c(ResearchList *wk, u8 index);
static u8 func_ov217_021bbf04(ResearchList *wk, u8 index);
static u8 func_ov217_021bbf1c(ResearchList *wk, u8 index);
static int func_ov217_021bbf28(ResearchList *wk, u8 index);
static int func_ov217_021bbf3c(ResearchList *wk, u8 index);
static int func_ov217_021bbf4c(ResearchList *wk, u8 index);
static void func_ov217_021bbf60(ResearchList *wk);
static void func_ov217_021bc014(ResearchList *wk);
static void func_ov217_021bc06c(ResearchList *wk);
static void func_ov217_021bc07c(ResearchList *wk, u8 index);
static void func_ov217_021bc0e8(ResearchList *wk);
static void func_ov217_021bc128(ResearchList *wk);
static void func_ov217_021bc168(ResearchList *wk);
static int func_ov217_021bc1ac(ResearchList *wk);
static int func_ov217_021bc200(ResearchList *wk, int y);
static void func_ov217_021bc250(ResearchList *wk);
static BOOL func_ov217_021bc264(ResearchList *wk);
static void func_ov217_021bc274(ResearchList *wk);
static void func_ov217_021bc280(ResearchList *wk);
static void func_ov217_021bc2b8(ResearchList *wk);
static void func_ov217_021bc2cc(ResearchList *wk, int from, int to, int frames);
static void func_ov217_021bc2d8(ResearchList *wk);
static BOOL func_ov217_021bc2fc(ResearchList *wk);
static void func_ov217_021bc30c(ResearchList *wk, int y);
static void func_ov217_021bc330(ResearchList *wk, int y);
static void func_ov217_021bc338(ResearchList *wk);
static void func_ov217_021bc380(ResearchList *wk);
static int func_ov217_021bc3ac(void);
static void func_ov217_021bc3b8(int y);
static int func_ov217_021bc3d4(void);
static int func_ov217_021bc3e0(void);
static int func_ov217_021bc3ec(int y);
static int func_ov217_021bc3f8(int index);
static int func_ov217_021bc400(int index);
static int func_ov217_021bc40c(ResearchList *wk);
static int func_ov217_021bc41c(ResearchList *wk);
static BOOL func_ov217_021bc428(ResearchList *wk);
void func_ov217_021bc43c(ResearchList *wk);
static void func_ov217_021bc480(void);
static void func_ov217_021bc490(void);
static BOOL func_ov217_021bc4a0(void);
static void func_ov217_021bc50c(ResearchList *wk);
static void func_ov217_021bc4b4(ResearchList *wk);
static void ResearchList_UpdateListRects(ResearchList *wk);
static void ResearchList_FadeIn(void);
static void ResearchList_FadeOut(void);
static BOOL ResearchList_IsFadeDone(void);
static void ResearchList_Darken(ResearchList *wk);
static void ResearchList_Brighten(ResearchList *wk);
static void ResearchList_BrightenCurrentIcon(ResearchList *wk);
static BOOL func_ov217_021bc55c(ResearchList *wk);
static void func_ov217_021bc574(ResearchList *wk, u32 index);
static void func_ov217_021bc580(ResearchList *wk, u32 index);
static void func_ov217_021bc5a8(ResearchList *wk, u32 index);
static BOOL func_ov217_021bc5b8(ResearchList *wk, u32 index);
static void func_ov217_021bc5d4(ResearchList *wk);
static void func_ov217_021bc5f0(ResearchList *wk);
static u32 func_ov217_021bc5fc(ResearchList *wk);
static void func_ov217_021bc600(ResearchList *wk);
static void func_ov217_021bc608(ResearchList *wk);
static void func_ov217_021bc610(TCB *tcb, void *data);
static GameData *func_ov217_021bc628(ResearchList *wk);
static void func_ov217_021bc634(ResearchList *wk, HeapID heapId);
static void func_ov217_021bc638(ResearchList *wk, ResearchCommon *common);
static ResearchCommon *func_ov217_021bc63c(ResearchList *wk);
static void func_ov217_021bc640(ResearchList *wk, ResearchListRecovery *recovery);
static void func_ov217_021bc644(ResearchList *wk);
static void func_ov217_021bc688(ResearchList *wk);
static BOOL func_ov217_021bc6d0(ResearchList *wk);
static u8 func_ov217_021bc6dc(ResearchList *wk);
static u8 func_ov217_021bc6e4(ResearchList *wk);
static void func_ov217_021bc6ec(ResearchList *wk, u8 index);
static BOOL func_ov217_021bc6f4(ResearchList *wk, u8 index);
static u32 func_ov217_021bc708(ResearchList *wk, u32 index);
static ClActUnit *func_ov217_021bc714(ResearchList *wk, u32 index);
static ClActor *func_ov217_021bc720(ResearchList *wk, u32 index);
static void func_ov217_021bc72c(ResearchList *wk, u32 index, BOOL visible);
static BmpOamActor *func_ov217_021bc740(ResearchList *wk, u32 button);
static ResearchList *func_ov217_021bc758(HeapID heapId);
static void func_ov217_021bc774(ResearchList *wk);
static void func_ov217_021bc818(ResearchList *wk);
static void func_ov217_021bc820(ResearchList *wk);
static void func_ov217_021bc8b4(ResearchList *wk);
static void func_ov217_021bc8d0(ResearchList *wk);
static void func_ov217_021bc8d8(ResearchList *wk);
static void func_ov217_021bc8e4(ResearchList *wk);
static void func_ov217_021bc910(ResearchList *wk);
static void func_ov217_021bc924(ResearchList *wk);
static void func_ov217_021bc93c(ResearchList *wk);
static void func_ov217_021bc94c(ResearchList *wk);
static void func_ov217_021bc954(ResearchList *wk);
static void func_ov217_021bc960(ResearchList *wk);
static void func_ov217_021bc9fc(ResearchList *wk);
static void func_ov217_021bcac0(ResearchList *wk);
static void func_ov217_021bcae8(ResearchList *wk);
static void func_ov217_021bcb44(ResearchList *wk);
static void func_ov217_021bcb48(ResearchList *wk);
static void func_ov217_021bcb60(ResearchList *wk);
static void func_ov217_021bcb70(ResearchList *wk);
static void func_ov217_021bcbcc(ResearchList *wk);
static void func_ov217_021bcbd0(ResearchList *wk);
static void func_ov217_021bcc3c(ResearchList *wk);
static void func_ov217_021bcc40(ResearchList *wk);
static void func_ov217_021bcc58(ResearchList *wk);
static void func_ov217_021bcc68(ResearchList *wk);
static void func_ov217_021bcd6c(ResearchList *wk);
static void func_ov217_021bcd90(ResearchList *wk);
static void func_ov217_021bcddc(ResearchList *wk);
static void func_ov217_021bcdf0(ResearchList *wk);
static void func_ov217_021bce5c(ResearchList *wk);
static void func_ov217_021bce7c(ResearchList *wk);
static void func_ov217_021bcf20(ResearchList *wk);
static void func_ov217_021bcf4c(ResearchList *wk);
static void func_ov217_021bcf68(ResearchList *wk);
static void func_ov217_021bcf98(ResearchList *wk);
static void func_ov217_021bcfb4(ResearchList *wk);
static void func_ov217_021bcfd0(ResearchList *wk);
static void func_ov217_021bd05c(ResearchList *wk);
static void func_ov217_021bd078(ResearchList *wk);
static void func_ov217_021bd094(ResearchList *wk);
static void func_ov217_021bd0c4(ResearchList *wk);
static void func_ov217_021bd178(ResearchList *wk);
static void func_ov217_021bd274(ResearchList *wk);
static void func_ov217_021bd370(ResearchList *wk);
static void func_ov217_021bd38c(ResearchList *wk);
static void func_ov217_021bd3a4(ResearchList *wk);
static void func_ov217_021bd3b4(ResearchList *wk);
static void func_ov217_021bd458(ResearchList *wk);
static void func_ov217_021bd474(ResearchList *wk);
static void func_ov217_021bd47c(ResearchList *wk);
static void func_ov217_021bd4cc(ResearchList *wk);
static void func_ov217_021bd4f0(ResearchList *wk);
static void func_ov217_021bd50c(ResearchList *wk);
static void func_ov217_021bd520(ResearchList *wk);
static void func_ov217_021bd53c(ResearchList *wk);
static void func_ov217_021bd56c(ResearchList *wk);
static void func_ov217_021bd588(ResearchList *wk);
static void func_ov217_021bd5b4(ResearchList *wk);
static void func_ov217_021bd5c4(ResearchList *wk);
static void func_ov217_021bd5e0(ResearchList *wk);

static const u8 data_ov217_021c305c[UNIT_COUNT] = { 0, 0, 0 };

static const u16 data_ov217_021c3060[UNIT_COUNT] = { 10, 10, 60 };

// The scroll bar
static const ResearchRect data_ov217_021c3070[SCROLL_RECT_COUNT] = {
    { 232, 255, VIEW_TOP, VIEW_BOTTOM },
#ifdef BUGFIX
    { 0, 0, TOUCH_RECT_END, 0 },
#else
    // BUG: As in research_top.c, the end of the table is written as a TouchRect, so the copy in the work has no end
    { TOUCH_RECT_END, 0, 0, 0 },
#endif
};

static const u32 data_ov217_021c3078[MSG_DATA_COUNT] = { 0xa9, 0xa4 };

static const u32 data_ov217_021c3068[BUTTON_COUNT] = { BMP_YES, BMP_NO };

// The three questions of each survey
static const u8 data_ov217_021c3080[SURVEY_COUNT] = { 8, 25, 7, 5, 6, 13, 21, 16, 19, 18 };

static const u8 data_ov217_021c30a8[SURVEY_COUNT] = { 0, 1, 2, 3, 4, 12, 20, 10, 9, 15 };

static const u8 data_ov217_021c309e[SURVEY_COUNT] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

static const u8 data_ov217_021c308a[SURVEY_COUNT] = { 29, 26, 28, 11, 14, 24, 22, 17, 23, 27 };

static const u8 data_ov217_021c3094[SURVEY_COUNT] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

// The buttons of the question
static const ResearchRect data_ov217_021c30b2[BUTTON_RECT_COUNT] = {
    { 160, 240, 142, 166 },
    { 160, 240, 168, 192 },
#ifdef BUGFIX
    { 0, 0, TOUCH_RECT_END, 0 },
#else
    // BUG: The same as data_ov217_021c3070' end
    { TOUCH_RECT_END, 0, 0, 0 },
#endif
};

static const BGSysLCDConfig data_ov217_021c30c0 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// The message of each question
static const u8 data_ov217_021c30d0[30] = {
#include "asset/research_list_data_ov217_021c30d0.inc"
};

static const BGSetup data_ov217_021c30f0 = { 0,
                                          0,
                                          0x800,
                                          0,
                                          BGRES_256x256,
                                          GX_BG_COLORMODE_16,
                                          GX_BG_SCRBASE_0x1000,
                                          GX_BG_CHARBASE_0x04000,
                                          0x8000,
                                          GX_BG_EXTPLTT_01,
                                          1,
                                          GX_BG_AREAOVER_XLU,
                                          FALSE };

static const BGSetup data_ov217_021c3110 = { 0,
                                         0,
                                         0x800,
                                         0,
                                         BGRES_256x256,
                                         GX_BG_COLORMODE_16,
                                         GX_BG_SCRBASE_0x1000,
                                         GX_BG_CHARBASE_0x04000,
                                         0x8000,
                                         GX_BG_EXTPLTT_01,
                                         1,
                                         GX_BG_AREAOVER_XLU,
                                         FALSE };

static const BGSetup data_ov217_021c3130 = { 0,
                                        0,
                                        0x800,
                                        0,
                                        BGRES_256x256,
                                        GX_BG_COLORMODE_16,
                                        GX_BG_SCRBASE_0x1800,
                                        GX_BG_CHARBASE_0x10000,
                                        0x8000,
                                        GX_BG_EXTPLTT_01,
                                        0,
                                        GX_BG_AREAOVER_XLU,
                                        FALSE };

static const BGSetup data_ov217_021c3150 = { 0,
                                         0,
                                         0x800,
                                         0,
                                         BGRES_256x256,
                                         GX_BG_COLORMODE_16,
                                         GX_BG_SCRBASE_0x0800,
                                         GX_BG_CHARBASE_0x04000,
                                         0x8000,
                                         GX_BG_EXTPLTT_01,
                                         2,
                                         GX_BG_AREAOVER_XLU,
                                         FALSE };

static const BGSetup data_ov217_021c3170 = { 0,
                                         0,
                                         0x800,
                                         0,
                                         BGRES_256x256,
                                         GX_BG_COLORMODE_16,
                                         GX_BG_SCRBASE_0x1800,
                                         GX_BG_CHARBASE_0x10000,
                                         0x8000,
                                         GX_BG_EXTPLTT_01,
                                         1,
                                         GX_BG_AREAOVER_XLU,
                                         FALSE };

// The surveys, which ResearchList_UpdateListRects fills in as the list scrolls
static const ResearchRect data_ov217_021c3190[LIST_RECT_COUNT] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
#ifdef BUGFIX
    { 0, 0, TOUCH_RECT_END, 0 },
#else
    // BUG: The same as data_ov217_021c3070' end
    { TOUCH_RECT_END, 0, 0, 0 },
#endif
};

static const BmpOamEntry data_ov217_021c31bc[BMP_COUNT] = {
#include "asset/research_list_data_ov217_021c31bc.inc"
};

static const ResearchActorSetup data_ov217_021c320c[ACTOR_COUNT] = {
#include "asset/research_list_data_ov217_021c320c.inc"
};

static const ResearchPaletteAnimeSetup data_ov217_021c3260[ANIME_COUNT] = {
    { (u16 *)(HW_BG_PLTT + 0x146), (const u16 *)(HW_BG_PLTT + 0x146), 3, 0, 0xffff },
    { (u16 *)(HW_BG_PLTT + 0x140), (const u16 *)(HW_BG_PLTT + 0x140), 16, 4, 0xffff },
    { (u16 *)(HW_BG_PLTT + 0x140), (const u16 *)(HW_BG_PLTT + 0x140), 16, 3, 0xffff },
    { (u16 *)(HW_OBJ_PLTT + 0x8c), (const u16 *)(HW_OBJ_PLTT + 0x8c), 1, 0, 0xffff },
    { (u16 *)(HW_OBJ_PLTT + 0x80), (const u16 *)(HW_OBJ_PLTT + 0x80), 16, 4, 0xffff },
    { (u16 *)(HW_OBJ_PLTT + 0x80), (const u16 *)(HW_OBJ_PLTT + 0x80), 16, 3, 0xffff },
};

static const BGFontEntry data_ov217_021c33c8[BG_FONT_COUNT] = {
#include "asset/research_list_data_ov217_021c33c8.inc"
};

static const BitmapEntry data_ov217_021c3470[BMP_COUNT] = {
#include "asset/research_list_data_ov217_021c3470.inc"
};

// The names of the surveys in the list
static const BGFontEntry data_ov217_021c3520[SURVEY_COUNT] = {
#include "asset/research_list_data_ov217_021c3520.inc"
};

// pokebw2's ResearchList_Create
ResearchList *func_ov217_021bab80(ResearchCommon *common, ResearchListRecovery *recovery) {
    HeapID heapId = func_ov217_021c09ac(common);
    ResearchList *wk = func_ov217_021bc758(heapId);

    func_ov217_021bc774(wk);
    func_ov217_021bc634(wk, heapId);
    func_ov217_021bc638(wk, common);
    func_ov217_021bc640(wk, recovery);
    return wk;
}

// pokebw2's ResearchList_Delete
void func_ov217_021babb4(ResearchList *wk) {
    func_ov217_021bc954(wk);
    func_ov217_021bc818(wk);
}

// pokebw2's ResearchList_Main
void func_ov217_021babc4(ResearchList *wk) {
    switch (func_ov217_021bb974(wk)) {
    case SEQ_SETUP:
        func_ov217_021baca0(wk);
        break;
    case SEQ_TOUCH:
        func_ov217_021bae00(wk);
        break;
    case SEQ_CURSOR:
        func_ov217_021baf70(wk);
        break;
    case SEQ_CURSOR_SCROLL:
        func_ov217_021bb190(wk);
        break;
    case SEQ_DRAG:
        func_ov217_021bb200(wk);
        break;
    case SEQ_RETURN_CURSOR:
        func_ov217_021bb69c(wk);
        break;
    case SEQ_RETURN_TOUCH:
        func_ov217_021bb710(wk);
        break;
    case SEQ_CONFIRM_OPEN_TOUCH:
        func_ov217_021bb484(wk);
        break;
    case SEQ_CONFIRM_TOUCH:
        func_ov217_021bb2b4(wk);
        break;
    case SEQ_CONFIRM_OPEN_CURSOR:
        func_ov217_021bb534(wk);
        break;
    case SEQ_CONFIRM_CURSOR:
        func_ov217_021bb37c(wk);
        break;
    case SEQ_CONFIRM_CANCEL:
        func_ov217_021bb784(wk);
        break;
    case SEQ_DECIDE:
        func_ov217_021bb5f4(wk);
        break;
    case SEQ_TEARDOWN:
        func_ov217_021bb84c(wk);
        break;
    case SEQ_END:
        break;
    }

    if (!func_ov217_021bac90(wk)) {
        func_ov217_021bc5f0(wk);
        func_ov217_021bc5d4(wk);
        func_0204a600();
    }
    func_ov217_021bb908(wk);
    func_ov217_021bb94c(wk);
}

// pokebw2's ResearchList_IsEnd
BOOL func_ov217_021bac90(ResearchList *wk) {
    return wk->isEnd;
}

// pokebw2's ResearchList_GetNext
u32 func_ov217_021bac98(ResearchList *wk) {
    return wk->next;
}

// pokebw2's ResearchList_SeqSetup
static void func_ov217_021baca0(ResearchList *wk) {
    u8 current;

    func_ov217_021bc93c(wk);
    func_ov217_021bc8b4(wk);
    func_ov217_021bc8e4(wk);
    func_ov217_021bc960(wk);
    func_ov217_021bc43c(wk);
    func_ov217_021bc820(wk);
    func_ov217_021bc9fc(wk);
    func_ov217_021bcae8(wk);
    func_ov217_021bcb48(wk);
    func_ov217_021bcb70(wk);
    func_ov217_021bcbd0(wk);
    func_ov217_021bcc40(wk);
    func_ov217_021bcc68(wk);
    func_ov217_021bcdf0(wk);
    func_ov217_021bce7c(wk);
    func_ov217_021bcf68(wk);
    func_ov217_021bcfd0(wk);
    func_ov217_021bd094(wk);
    func_ov217_021bd0c4(wk);
    func_ov217_021bd178(wk);
    func_ov217_021bd274(wk);
    func_ov217_021bd38c(wk);
    func_ov217_021bd3b4(wk);
    func_ov217_021bd47c(wk);
    func_ov217_021bd4f0(wk);
    func_ov217_021bd53c(wk);
    func_ov217_021bd5c4(wk);
    func_ov217_021bd5b4(wk);
    func_ov217_021bd588(wk);
    func_ov217_021bc580(wk, ANIME_CURSOR);
    func_ov217_021bc72c(wk, BMP_QUESTION, FALSE);
    func_ov217_021bc72c(wk, BMP_YES, FALSE);
    func_ov217_021bc72c(wk, BMP_NO, FALSE);
    func_ov217_021bc644(wk);

    current = func_ov217_021bbc50(wk);
    if (current != SURVEY_NONE) {
        func_ov217_021bbdf0(wk, current);
        func_ov217_021bc6ec(wk, current);
        func_ov217_021bc014(wk);
    }

    func_ov217_021bc43c(wk);
    func_ov217_021bc168(wk);
    func_ov217_021bc014(wk);
    func_ov217_021bbf60(wk);
    func_ov217_021bc06c(wk);

    if (!func_ov217_021bb928(wk)) {
        func_ov217_021bbd68(wk);
        func_ov217_021bc128(wk);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_CURSOR);
    } else {
        func_ov217_021bbdac(wk);
        func_ov217_021bc0e8(wk);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_TOUCH);
    }

    func_ov217_021bc480();
}

// pokebw2's ResearchList_SeqTouch
static void func_ov217_021bae00(ResearchList *wk) {
    u32 keys = GCTX_HIDGetPressedKeys();
    s32 item = func_02035c08(wk->listRects);
    s32 commonButton = func_02035c08(func_ov217_021c09b8(wk->common));
    u32 x, y;

    func_02035cb8(&x, &y);

    if (func_ov217_021bc6d0(wk) || commonButton == RESEARCH_COMMON_BUTTON_RETURN) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021bbad0(wk);
        func_ov217_021bb934(wk, TRUE);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_RETURN_TOUCH);
    } else if (keys & PAD_BUTTON_B) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021bbad0(wk);
        func_ov217_021bb934(wk, FALSE);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_RETURN_TOUCH);
    } else if ((keys & PAD_KEY_UP) || (keys & PAD_KEY_DOWN) || (keys & PAD_KEY_LEFT) || (keys & PAD_KEY_RIGHT) ||
               (keys & PAD_BUTTON_A)) {
        func_ov217_021bbd68(wk);
        func_ov217_021bc128(wk);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_CURSOR);
    } else if (item >= 0 && item <= SURVEY_COUNT - 1) {
        if (y >= VIEW_TOP && y <= VIEW_BOTTOM && func_ov217_021bc6f4(wk, item) == TRUE) {
            func_ov217_021bbaa4(wk, item);
            func_ov217_021bc6ec(wk, wk->cursor);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov217_021bc580(wk, ANIME_DECIDE);
            func_ov217_021bc06c(wk);
            func_ov217_021bc128(wk);
            func_ov217_021bb91c(wk);
            func_ov217_021bb910(wk, SEQ_CONFIRM_OPEN_TOUCH);
        }
    } else if ((func_02035ba4(wk->scrollRects) == 0 || func_02035c08(wk->scrollRects) == 0) &&
               func_ov217_021bc428(wk) == TRUE) {
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_DRAG);
        func_ov217_021bb910(wk, SEQ_TOUCH);
    }
}

// pokebw2's ResearchList_SeqCursor
static void func_ov217_021baf70(ResearchList *wk) {
    u32 held = GCTX_HIDGetHeldKeys();
    u32 keys = GCTX_HIDGetPressedKeys();
    s32 item = func_02035c08(wk->listRects);
    s32 commonButton = func_02035c08(func_ov217_021c09b8(wk->common));
    u32 x, y;

    func_02035cb8(&x, &y);

    if (held & (PAD_KEY_UP | PAD_KEY_DOWN)) {
        func_ov217_021bc600(wk);
    } else {
        func_ov217_021bc608(wk);
    }

    if (func_ov217_021bc6d0(wk) || commonButton == RESEARCH_COMMON_BUTTON_RETURN) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021bbad0(wk);
        func_ov217_021bb934(wk, TRUE);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_RETURN_CURSOR);
    } else if ((keys & PAD_KEY_UP) ||
               ((held & PAD_KEY_UP) && wk->cursor != 0 && func_ov217_021bc5fc(wk) > KEY_REPEAT_FRAMES)) {
        func_ov217_021bbdac(wk);
        func_ov217_021bbd40(wk, -1);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_CURSOR_SCROLL);
        func_ov217_021bb910(wk, SEQ_CURSOR);
    } else if ((keys & PAD_KEY_DOWN) || ((held & PAD_KEY_DOWN) && wk->cursor < func_ov217_021bc6dc(wk) - 1 &&
                                         func_ov217_021bc5fc(wk) > KEY_REPEAT_FRAMES)) {
        func_ov217_021bbdac(wk);
        func_ov217_021bbd40(wk, 1);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_CURSOR_SCROLL);
        func_ov217_021bb910(wk, SEQ_CURSOR);
    } else if (keys & PAD_BUTTON_A) {
        func_ov217_021bc580(wk, ANIME_DECIDE);
        func_ov217_021bc6ec(wk, wk->cursor);
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov217_021bb910(wk, SEQ_CONFIRM_OPEN_CURSOR);
        func_ov217_021bb91c(wk);
    } else if (item >= 0 && item <= SURVEY_COUNT - 1) {
        if (y >= VIEW_TOP && y <= VIEW_BOTTOM && func_ov217_021bc6f4(wk, item) == TRUE) {
            func_ov217_021bbaa4(wk, item);
            func_ov217_021bc6ec(wk, wk->cursor);
            func_ov217_021bc580(wk, ANIME_DECIDE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov217_021bc06c(wk);
            func_ov217_021bb91c(wk);
            func_ov217_021bb910(wk, SEQ_CONFIRM_OPEN_TOUCH);
        }
    } else if (keys & PAD_BUTTON_B) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021bbad0(wk);
        func_ov217_021bb934(wk, FALSE);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_RETURN_CURSOR);
    } else if ((func_02035ba4(wk->scrollRects) == 0 || func_02035c08(wk->scrollRects) == 0) &&
               func_ov217_021bc428(wk) == TRUE) {
        func_ov217_021bbdac(wk);
        func_ov217_021bc0e8(wk);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_DRAG);
        func_ov217_021bb910(wk, SEQ_TOUCH);
    }
}

// pokebw2's ResearchList_SeqCursorScroll
static void func_ov217_021bb190(ResearchList *wk) {
    switch (func_ov217_021bb98c(wk)) {
    case 0:
        func_ov217_021bbadc(wk);
        func_ov217_021bbaa4(wk, wk->cursorTarget);
        func_ov217_021bc06c(wk);
        func_ov217_021bb990(wk);
        break;
    case 1:
        func_ov217_021bc2d8(wk);
        func_ov217_021bc338(wk);
        func_ov217_021bc43c(wk);
        func_ov217_021bc168(wk);
        func_ov217_021bc014(wk);
        func_ov217_021bbf60(wk);
        if (func_ov217_021bc2fc(wk)) {
            func_ov217_021bbf60(wk);
            func_ov217_021bb91c(wk);
        }
        break;
    }
}

// pokebw2's ResearchList_SeqDrag
static void func_ov217_021bb200(ResearchList *wk) {
    u32 x, y;
    BOOL touching = func_02035cb8(&x, &y);

    switch (func_ov217_021bb98c(wk)) {
    case 0:
        if (!touching) {
            func_ov217_021bc280(wk);
            func_ov217_021bc2b8(wk);
            func_ov217_021bb990(wk);
        } else {
            func_ov217_021bc30c(wk, func_ov217_021bc200(wk, y));
            func_ov217_021bc380(wk);
            func_ov217_021bc43c(wk);
            func_ov217_021bc168(wk);
            func_ov217_021bc014(wk);
            func_ov217_021bc274(wk);
        }
        break;
    case 1:
        func_ov217_021bc30c(wk, wk->scrollY + wk->dragSpeed);
        func_ov217_021bc380(wk);
        func_ov217_021bc43c(wk);
        func_ov217_021bc168(wk);
        func_ov217_021bc014(wk);
        func_ov217_021bc250(wk);
        if (!func_ov217_021bc264(wk)) {
            func_ov217_021bc06c(wk);
            func_ov217_021bb91c(wk);
        }
        break;
    }

    func_ov217_021bba30(wk);
}

// pokebw2's ResearchList_SeqConfirmTouch
static void func_ov217_021bb2b4(ResearchList *wk) {
    u32 keys = GCTX_HIDGetPressedKeys();
    s32 button = func_02035c08(wk->buttonRects);

    if ((keys & PAD_KEY_UP) || (keys & PAD_KEY_DOWN) || (keys & PAD_KEY_LEFT) || (keys & PAD_KEY_RIGHT) ||
        (keys & PAD_BUTTON_A)) {
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_CONFIRM_OPEN_CURSOR);
    } else if (button == BUTTON_YES) {
        func_ov217_021bba00(wk, BUTTON_YES);
        func_ov217_021bc5a8(wk, ANIME_BUTTON_CURSOR);
        func_ov217_021bc580(wk, ANIME_BUTTON_DECIDE);
        func_ov217_021bb934(wk, TRUE);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_DECIDE);
    } else if (func_ov217_021bc6d0(wk) || (keys & PAD_BUTTON_B) || button == BUTTON_NO) {
        func_ov217_021bba00(wk, BUTTON_NO);
        func_ov217_021bc5a8(wk, ANIME_BUTTON_CURSOR);
        func_ov217_021bc580(wk, ANIME_BUTTON_DECIDE);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_CONFIRM_CANCEL);
        func_ov217_021bb910(wk, SEQ_CURSOR);
    }
}

// pokebw2's ResearchList_SeqConfirmCursor
static void func_ov217_021bb37c(ResearchList *wk) {
    u32 keys = GCTX_HIDGetPressedKeys();
    s32 button = func_02035c08(wk->buttonRects);

    if (keys & PAD_KEY_UP) {
        func_ov217_021bb9a0(wk);
    }
    if (keys & PAD_KEY_DOWN) {
        func_ov217_021bb9d0(wk);
    }

    if (keys & PAD_BUTTON_A) {
        func_ov217_021bc5a8(wk, ANIME_BUTTON_CURSOR);
        func_ov217_021bc580(wk, ANIME_BUTTON_DECIDE);
        switch (wk->buttonCursor) {
        case BUTTON_YES:
            func_ov217_021bb934(wk, FALSE);
            func_ov217_021bb91c(wk);
            func_ov217_021bb910(wk, SEQ_DECIDE);
            break;
        case BUTTON_NO:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov217_021bb91c(wk);
            func_ov217_021bb910(wk, SEQ_CONFIRM_CANCEL);
            func_ov217_021bb910(wk, SEQ_CURSOR);
            break;
        }
    } else if (button == BUTTON_YES) {
        func_ov217_021bba00(wk, BUTTON_YES);
        func_ov217_021bc5a8(wk, ANIME_BUTTON_CURSOR);
        func_ov217_021bc580(wk, ANIME_BUTTON_DECIDE);
        func_ov217_021bb934(wk, TRUE);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_DECIDE);
    } else if (func_ov217_021bc6d0(wk) || (keys & PAD_BUTTON_B) || button == BUTTON_NO) {
        func_ov217_021bba00(wk, BUTTON_NO);
        func_ov217_021bc5a8(wk, ANIME_BUTTON_CURSOR);
        func_ov217_021bc580(wk, ANIME_BUTTON_DECIDE);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021bb91c(wk);
        func_ov217_021bb910(wk, SEQ_CONFIRM_CANCEL);
        func_ov217_021bb910(wk, SEQ_CURSOR);
    }
}

// pokebw2's ResearchList_SeqConfirmOpenTouch
static void func_ov217_021bb484(ResearchList *wk) {
    switch (func_ov217_021bb98c(wk)) {
    case 0:
        func_ov217_021bbd28(wk, BUTTON_NO);
        func_ov217_021bbd28(wk, BUTTON_YES);
        func_ov217_021bc72c(wk, BMP_QUESTION, TRUE);
        func_ov217_021bc72c(wk, BMP_YES, TRUE);
        func_ov217_021bc72c(wk, BMP_NO, TRUE);
        if (!wk->darkened) {
            func_ov217_021c1038(wk->itemFonts[wk->cursor], 14);
            func_ov217_021bc4b4(wk);
        }
        func_ov217_021bbbe4(wk);
        func_ov217_021bb990(wk);
        break;
    case 1:
        func_ov217_021bc2d8(wk);
        func_ov217_021bc338(wk);
        func_ov217_021bc43c(wk);
        func_ov217_021bc168(wk);
        func_ov217_021bc014(wk);
        func_ov217_021bbf60(wk);
        if (func_ov217_021bc2fc(wk)) {
            func_ov217_021bb91c(wk);
            func_ov217_021bb910(wk, SEQ_CONFIRM_TOUCH);
        }
        break;
    }
}

// pokebw2's ResearchList_SeqConfirmOpenCursor
static void func_ov217_021bb534(ResearchList *wk) {
    switch (func_ov217_021bb98c(wk)) {
    case 0:
        func_ov217_021bbd14(wk, BUTTON_YES);
        func_ov217_021bbd28(wk, BUTTON_NO);
        func_ov217_021bbd18(wk, BUTTON_YES);
        func_ov217_021bc580(wk, ANIME_BUTTON_CURSOR);
        func_ov217_021bc72c(wk, BMP_QUESTION, TRUE);
        func_ov217_021bc72c(wk, BMP_YES, TRUE);
        func_ov217_021bc72c(wk, BMP_NO, TRUE);
        if (!wk->darkened) {
            func_ov217_021c1038(wk->itemFonts[wk->cursor], 14);
            func_ov217_021bc4b4(wk);
        }
        func_ov217_021bbbe4(wk);
        func_ov217_021bb990(wk);
        break;
    case 1:
        func_ov217_021bc2d8(wk);
        func_ov217_021bc338(wk);
        func_ov217_021bc43c(wk);
        func_ov217_021bc168(wk);
        func_ov217_021bc014(wk);
        func_ov217_021bbf60(wk);
        if (func_ov217_021bc2fc(wk)) {
            func_ov217_021bb91c(wk);
            func_ov217_021bb910(wk, SEQ_CONFIRM_CURSOR);
        }
        break;
    }
}

// pokebw2's ResearchList_SeqDecide
static void func_ov217_021bb5f4(ResearchList *wk) {
    switch (func_ov217_021bb98c(wk)) {
    case 0:
        func_ov217_021bbe7c(wk, func_ov217_021bbc50(wk));
        func_ov217_021bbcb0(wk);
        func_ov217_021bbdf0(wk, func_ov217_021bc6e4(wk));
        func_ov217_021bc014(wk);
        func_ov217_021bc72c(wk, BMP_DECIDED, TRUE);
        GFL_SndSEPlay(SEQ_SE_SYS_80);
        func_ov217_021bb990(wk);
        break;
    case 1:
        if (wk->seqFrames >= DECIDE_WAIT_FRAMES) {
            func_ov217_021bc490();
            func_ov217_021bb990(wk);
        }
        break;
    case 2:
        if (func_ov217_021bc4a0()) {
            func_ov217_021bc50c(wk);
            func_ov217_021bb990(wk);
        }
        break;
    case 3:
        if (func_ov217_021bc55c(wk)) {
            func_ov217_021bb91c(wk);
            func_ov217_021bb910(wk, SEQ_TEARDOWN);
        }
        break;
    }
}

// pokebw2's ResearchList_SeqReturnCursor
static void func_ov217_021bb69c(ResearchList *wk) {
    switch (func_ov217_021bb98c(wk)) {
    case 0:
        if (func_ov217_021bb948(wk) < wk->seqFrames) {
            func_ov217_021bc490();
            func_ov217_021bb990(wk);
        }
        break;
    case 1:
        if (func_ov217_021bc4a0()) {
            func_ov217_021bc50c(wk);
            func_ov217_021bb990(wk);
        }
        break;
    case 2:
        if (func_ov217_021bc55c(wk)) {
            func_ov217_021c1038(wk->itemFonts[wk->cursor], 15);
            func_ov217_021bb91c(wk);
            func_ov217_021bb910(wk, SEQ_TEARDOWN);
        }
        break;
    }
}

// pokebw2's ResearchList_SeqReturnTouch
static void func_ov217_021bb710(ResearchList *wk) {
    switch (func_ov217_021bb98c(wk)) {
    case 0:
        if (func_ov217_021bb948(wk) < wk->seqFrames) {
            func_ov217_021bc490();
            func_ov217_021bb990(wk);
        }
        break;
    case 1:
        if (func_ov217_021bc4a0()) {
            func_ov217_021bc50c(wk);
            func_ov217_021bb990(wk);
        }
        break;
    case 2:
        if (func_ov217_021bc55c(wk)) {
            func_ov217_021c1038(wk->itemFonts[wk->cursor], 15);
            func_ov217_021bb91c(wk);
            func_ov217_021bb910(wk, SEQ_TEARDOWN);
        }
        break;
    }
}

// pokebw2's ResearchList_SeqConfirmCancel
static void func_ov217_021bb784(ResearchList *wk) {
    switch (func_ov217_021bb98c(wk)) {
    case 0:
        func_ov217_021bbb60(wk);
        func_ov217_021bb990(wk);
        break;
    case 1:
        func_ov217_021bc2d8(wk);
        func_ov217_021bc338(wk);
        func_ov217_021bc43c(wk);
        func_ov217_021bc168(wk);
        func_ov217_021bc014(wk);
        func_ov217_021bbf60(wk);
        if (func_ov217_021bc2fc(wk)) {
            func_ov217_021bb990(wk);
        }
        break;
    case 2:
        if (func_ov217_021bc5b8(wk, ANIME_BUTTON_DECIDE)) {
            func_ov217_021bc72c(wk, BMP_QUESTION, FALSE);
            func_ov217_021bc72c(wk, BMP_YES, FALSE);
            func_ov217_021bc72c(wk, BMP_NO, FALSE);
            func_ov217_021bbf60(wk);
            func_ov217_021bc50c(wk);
            func_ov217_021bb990(wk);
        }
        break;
    case 3:
        if (func_ov217_021bc55c(wk)) {
            func_ov217_021c1038(wk->itemFonts[wk->cursor], 15);
            func_ov217_021bb91c(wk);
        }
        break;
    }
}

// pokebw2's ResearchList_SeqTeardown
static void func_ov217_021bb84c(ResearchList *wk) {
    func_ov217_021bc688(wk);
    func_ov217_021bd5e0(wk);
    func_ov217_021bd56c(wk);
    func_ov217_021bd520(wk);
    func_ov217_021c09e0(wk->common);
    func_ov217_021c09ec(wk->common);
    func_ov217_021bd4cc(wk);
    func_ov217_021bd370(wk);
    func_ov217_021bd458(wk);
    func_ov217_021bd3a4(wk);
    func_ov217_021bd05c(wk);
    func_ov217_021bcf98(wk);
    func_ov217_021bce5c(wk);
    func_ov217_021bcf20(wk);
    func_ov217_021bcd90(wk);
    func_ov217_021bcc58(wk);
    func_ov217_021bcbcc(wk);
    func_ov217_021bcc3c(wk);
    func_ov217_021bcb60(wk);
    func_ov217_021bcb44(wk);
    func_ov217_021bcac0(wk);
    func_ov217_021bc924(wk);
    func_ov217_021bc8d8(wk);
    func_ov217_021bb940(wk, RESEARCH_LIST_NEXT_TOP);
    func_ov217_021bb91c(wk);
    func_ov217_021bb910(wk, SEQ_END);
    wk->isEnd = TRUE;
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
}

// pokebw2's ResearchList_CountFrame
static void func_ov217_021bb908(ResearchList *wk) {
    wk->seqFrames++;
}

// pokebw2's ResearchList_PushSeq
static void func_ov217_021bb910(ResearchList *wk, u32 seq) {
    func_ov217_021c0e4c(wk->queue, seq);
}

// pokebw2's ResearchList_EndSeq
static void func_ov217_021bb91c(ResearchList *wk) {
    wk->seqDone = TRUE;
}

// pokebw2's ResearchList_GetTouchMode
static BOOL func_ov217_021bb928(ResearchList *wk) {
    return func_ov217_021c09cc(wk->common);
}

// pokebw2's ResearchList_SetTouchMode
static void func_ov217_021bb934(ResearchList *wk, BOOL touch) {
    func_ov217_021c09c4(wk->common, touch);
}

// pokebw2's ResearchList_SetNext
static void func_ov217_021bb940(ResearchList *wk, u32 next) {
    wk->next = next;
}

// pokebw2's ResearchList_GetWait
static u32 func_ov217_021bb948(ResearchList *wk) {
    return wk->waitFrames;
}

// pokebw2's ResearchList_UpdateSeq
static void func_ov217_021bb94c(ResearchList *wk) {
    if (wk->seqDone && !func_ov217_021c0e64(wk->queue)) {
        func_ov217_021bb978(wk, func_ov217_021c0e54(wk->queue));
    }
}

// pokebw2's ResearchList_GetSeq
static u32 func_ov217_021bb974(ResearchList *wk) {
    return wk->seq;
}

// pokebw2's ResearchList_SetSeq
static void func_ov217_021bb978(ResearchList *wk, u32 seq) {
    wk->seq = seq;
    wk->seqFrames = 0;
    wk->seqDone = FALSE;
    func_ov217_021bb998(wk);
}

// pokebw2's ResearchList_GetSeqState
static u32 func_ov217_021bb98c(ResearchList *wk) {
    return wk->seqState;
}

// pokebw2's ResearchList_NextSeqState
static void func_ov217_021bb990(ResearchList *wk) {
    wk->seqState++;
}

// pokebw2's ResearchList_ResetSeqState
static void func_ov217_021bb998(ResearchList *wk) {
    wk->seqState = 0;
}

// pokebw2's ResearchList_ButtonCursorUp
static void func_ov217_021bb9a0(ResearchList *wk) {
    func_ov217_021bbd28(wk, wk->buttonCursor);
    func_ov217_021bbcfc(wk, -1);
    func_ov217_021bbd18(wk, wk->buttonCursor);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
    func_ov217_021bc580(wk, ANIME_BUTTON_MOVE);
}

// pokebw2's ResearchList_ButtonCursorDown
static void func_ov217_021bb9d0(ResearchList *wk) {
    func_ov217_021bbd28(wk, wk->buttonCursor);
    func_ov217_021bbcfc(wk, 1);
    func_ov217_021bbd18(wk, wk->buttonCursor);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
    func_ov217_021bc580(wk, ANIME_BUTTON_MOVE);
}

// pokebw2's ResearchList_MoveButtonCursorTo
static void func_ov217_021bba00(ResearchList *wk, int pos) {
    func_ov217_021bbd28(wk, wk->buttonCursor);
    func_ov217_021bbd14(wk, pos);
    func_ov217_021bbd18(wk, wk->buttonCursor);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
    func_ov217_021bc580(wk, ANIME_BUTTON_MOVE);
}

// Keeps the cursor on a survey in the view, moving it to the next one in when the view scrolls past it
// pokebw2's ResearchList_FollowScroll
static void func_ov217_021bba30(ResearchList *wk) {
    int top = func_ov217_021bc3d4();
    int bottom = func_ov217_021bc3e0();
    int itemTop = func_ov217_021bc3f8(wk->cursor);
    int itemBottom = func_ov217_021bc400(wk->cursor);
    u8 pos;

    if (itemBottom <= top) {
        pos = func_ov217_021bc3ec(itemBottom + 1);
        if (wk->cursorShown) {
            func_ov217_021bbaa4(wk, pos);
        } else {
            func_ov217_021bbd38(wk, pos);
        }
    }

    if (bottom <= itemTop) {
        pos = func_ov217_021bc3ec(itemTop - 1);
        if (wk->cursorShown) {
            func_ov217_021bbaa4(wk, pos);
        } else {
            func_ov217_021bbd38(wk, pos);
        }
    }
}

// pokebw2's ResearchList_MoveCursorTo
static void func_ov217_021bbaa4(ResearchList *wk, u8 pos) {
    func_ov217_021bbdac(wk);
    func_ov217_021bbd38(wk, pos);
    func_ov217_021bbd68(wk);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
    func_ov217_021bc580(wk, ANIME_MOVE);
}

// pokebw2's ResearchList_StartReturnAnime
static void func_ov217_021bbad0(ResearchList *wk) {
    func_ov217_021bc574(wk, 0);
}

// Scrolls from the view's edge to the survey the cursor moves to, at once when both are in the view
// pokebw2's ResearchList_StartCursorScroll
static void func_ov217_021bbadc(ResearchList *wk) {
    int from = wk->scrollY;
    int to;
    int top;
    int bottom;
    int frames;

    if (wk->cursor < wk->cursorTarget) {
        to = func_ov217_021bc400(wk->cursorTarget) + 1;
    } else {
        to = func_ov217_021bc3f8(wk->cursorTarget);
    }

    top = func_ov217_021bc3d4();
    bottom = func_ov217_021bc3e0();
    if (to < top || bottom < to) {
        if (from < to) {
            from = bottom;
        } else {
            from = top;
        }
    }

    frames = (MATH_MAX(from, to) - MATH_MIN(from, to)) / 4;
    top = func_ov217_021bc3d4();
    bottom = func_ov217_021bc3e0();
    if (top <= from && from <= bottom && top <= to && to <= bottom) {
        frames = 0;
    }
    func_ov217_021bc2cc(wk, from, to, frames);
}

// Scrolls the list back inside its ends after the question
// pokebw2's ResearchList_StartScrollBack
static void func_ov217_021bbb60(ResearchList *wk) {
    int from = wk->scrollY;
    int to = from;
    int top;
    int bottom;
    int frames;

    if (func_ov217_021bc41c(wk) < from) {
        to = func_ov217_021bc40c(wk) + VIEW_TOP;
    } else if (from < 0) {
        to = VIEW_BOTTOM - VIEW_TOP;
    }

    top = func_ov217_021bc3d4();
    bottom = func_ov217_021bc3e0();
    if (to < top || bottom < to) {
        if (from < to) {
            from = bottom;
        } else {
            from = top;
        }
    }

    frames = (MATH_MAX(from, to) - MATH_MIN(from, to)) / 3;
    top = func_ov217_021bc3d4();
    bottom = func_ov217_021bc3e0();
    if (top <= from && from <= bottom && top <= to && to <= bottom) {
        frames = 1;
    }
    func_ov217_021bc2cc(wk, from, to, frames);
}

// Scrolls the survey of the cursor to the middle of the view
// pokebw2's ResearchList_StartScrollToCursor
static void func_ov217_021bbbe4(ResearchList *wk) {
    int center = (func_ov217_021bc3d4() + func_ov217_021bc3e0()) / 2;
    int y = func_ov217_021bbf1c(wk, wk->cursor) * 8;
    int from;
    int to;

    if (center < y) {
        from = func_ov217_021bc3e0();
        to = func_ov217_021bc3e0() + (y - center);
    } else {
        from = func_ov217_021bc3d4();
        to = func_ov217_021bc3d4() - (center - y);
    }
    func_ov217_021bc2cc(wk, from, to, (MATH_MAX(from, to) - MATH_MIN(from, to)) / 3);
}

// The survey being run, from its questions in the save
// pokebw2's ResearchList_GetCurrentSurvey
static u8 func_ov217_021bbc50(ResearchList *wk) {
    u8 questions[QUESTION_COUNT];
    int i;
    void *survey = func_0200e394(GameData_GetSaveControl(func_ov217_021bc628(wk)));
    u8 index;

    for (i = 0; i < QUESTION_COUNT; i++) {
        questions[i] = func_0200e44c(survey, i);
    }

    for (index = 0; index < SURVEY_COUNT; index++) {
        if (questions[0] == data_ov217_021c30a8[index] && questions[1] == data_ov217_021c3080[index] &&
            questions[2] == data_ov217_021c308a[index]) {
            return index;
        }
    }
    // Questions of no survey give the first, though some callers check for SURVEY_NONE. Returning SURVEY_NONE instead
    // would not fix it: func_ov217_021bb5f4 passes the result on to func_ov217_021bbe7c unchecked
    return 0;
}

// Makes the chosen survey the one being run
// pokebw2's ResearchList_SetCurrentSurvey
static void func_ov217_021bbcb0(ResearchList *wk) {
    u8 index = func_ov217_021bc6e4(wk);
    void *survey = func_0200e394(GameData_GetSaveControl(func_ov217_021bc628(wk)));

    func_0200e440(survey, data_ov217_021c30a8[index], 0);
    func_0200e440(survey, data_ov217_021c3080[index], 1);
    func_0200e440(survey, data_ov217_021c308a[index], 2);
}

// pokebw2's ResearchList_MoveButtonCursor
static void func_ov217_021bbcfc(ResearchList *wk, int dir) {
    wk->buttonCursor = (wk->buttonCursor + dir + BUTTON_COUNT) % BUTTON_COUNT;
}

// pokebw2's ResearchList_SetButtonCursor
static void func_ov217_021bbd14(ResearchList *wk, int pos) {
    wk->buttonCursor = pos;
}

// pokebw2's ResearchList_HighlightButton
static void func_ov217_021bbd18(ResearchList *wk, u32 button) {
    func_02025a54(func_ov217_021bc740(wk, button), 0);
}

// pokebw2's ResearchList_UnhighlightButton
static void func_ov217_021bbd28(ResearchList *wk, u32 button) {
    func_02025a54(func_ov217_021bc740(wk, button), 1);
}

// pokebw2's ResearchList_SetCursor
static void func_ov217_021bbd38(ResearchList *wk, u8 pos) {
    wk->cursor = pos;
}

// pokebw2's ResearchList_SetCursorTarget
static void func_ov217_021bbd40(ResearchList *wk, int dir) {
    u8 cursor = wk->cursor;

    wk->cursorTarget = (cursor + dir + func_ov217_021bc6dc(wk)) % func_ov217_021bc6dc(wk);
}

// pokebw2's ResearchList_ShowCursor
static void func_ov217_021bbd68(ResearchList *wk) {
    u8 cursor = wk->cursor;

    GFL_BGSysSetScrPaletteNo(BG_MAIN_LIST, func_ov217_021bbf04(wk, cursor), func_ov217_021bbf1c(wk, cursor),
                             28, ITEM_ROWS, 10);
    GFL_BGSysQueueScrLoad(BG_MAIN_LIST);
    wk->cursorShown = TRUE;
}

// pokebw2's ResearchList_HideCursor
static void func_ov217_021bbdac(ResearchList *wk) {
    u8 cursor = wk->cursor;

    GFL_BGSysSetScrPaletteNo(BG_MAIN_LIST, func_ov217_021bbf04(wk, cursor), func_ov217_021bbf1c(wk, cursor),
                             28, ITEM_ROWS, 9);
    GFL_BGSysQueueScrLoad(BG_MAIN_LIST);
    wk->cursorShown = FALSE;
}

// Moves a survey's tiles a tile to the left, as the survey being run is drawn
// pokebw2's ResearchList_IndentItem
static void func_ov217_021bbdf0(ResearchList *wk, u8 index) {
    u16 *listScreen = GFL_BGSysIsScrHeapExists(BG_MAIN_LIST);
    u16 *textScreen = GFL_BGSysIsScrHeapExists(BG_MAIN_TEXT);
    int top;
    int i;
    int x;
    int src;
    int dst;

    for (i = 0; i < ITEM_ROWS; i++) {
        for (x = 0; x < 28; x++) {
            top = func_ov217_021bbf1c(wk, index);
            src = (x + 2) + (top + i) * 32;
            dst = src - 1;
            listScreen[dst] = listScreen[src];
            textScreen[dst] = textScreen[src];
        }
    }

    top = func_ov217_021bbf1c(wk, index);
    listScreen[top * 32 + 29] = listScreen[0];
    textScreen[top * 32 + 29] = textScreen[0];
    listScreen[(top + 1) * 32 + 29] = listScreen[0];
    textScreen[(top + 1) * 32 + 29] = textScreen[0];
    listScreen[(top + 2) * 32 + 29] = listScreen[0];
    textScreen[(top + 2) * 32 + 29] = textScreen[0];
    GFL_BGSysQueueScrLoad(BG_MAIN_LIST);
    GFL_BGSysQueueScrLoad(BG_MAIN_TEXT);
}

// pokebw2's ResearchList_UnindentItem
static void func_ov217_021bbe7c(ResearchList *wk, u8 index) {
    u16 *listScreen = GFL_BGSysIsScrHeapExists(BG_MAIN_LIST);
    u16 *textScreen = GFL_BGSysIsScrHeapExists(BG_MAIN_TEXT);
    int top;
    int i;
    int x;
    int src;
    int dst;

    for (i = ITEM_ROWS - 1; i >= 0; i--) {
        for (x = 27; x >= 0; x--) {
            top = func_ov217_021bbf1c(wk, index);
            src = (x + 1) + (top + i) * 32;
            dst = src + 1;
            listScreen[dst] = listScreen[src];
            textScreen[dst] = textScreen[src];
        }
    }

    top = func_ov217_021bbf1c(wk, index);
    listScreen[top * 32 + 1] = listScreen[0];
    textScreen[top * 32 + 1] = textScreen[0];
    listScreen[(top + 1) * 32 + 1] = listScreen[0];
    textScreen[(top + 1) * 32 + 1] = textScreen[0];
    listScreen[(top + 2) * 32 + 1] = listScreen[0];
    textScreen[(top + 2) * 32 + 1] = textScreen[0];
    GFL_BGSysQueueScrLoad(BG_MAIN_LIST);
    GFL_BGSysQueueScrLoad(BG_MAIN_TEXT);
}

// The tile column of a survey in the list, a tile to the left for the survey being run
// pokebw2's ResearchList_GetItemColumn
static u8 func_ov217_021bbf04(ResearchList *wk, u8 index) {
    u8 x = 2;

    if (index == func_ov217_021bbc50(wk)) {
        x--;
    }
    return x;
}

// pokebw2's ResearchList_GetItemRow
static u8 func_ov217_021bbf1c(ResearchList *wk, u8 index) {
    return index * ITEM_ROWS;
}

// pokebw2's ResearchList_GetItemScreenX
static int func_ov217_021bbf28(ResearchList *wk, u8 index) {
    int x = func_ov217_021bbf04(wk, index) * 8;

    return x - GFL_BGSysGetBGOffsetX2(BG_MAIN_LIST);
}

// pokebw2's ResearchList_GetItemScreenTop
static int func_ov217_021bbf3c(ResearchList *wk, u8 index) {
    int y = func_ov217_021bbf1c(wk, index) * 8;

    return y - func_ov217_021bc3ac();
}

// pokebw2's ResearchList_GetItemScreenBottom
static int func_ov217_021bbf4c(ResearchList *wk, u8 index) {
    int y = func_ov217_021bbf1c(wk, index) * 8 + ITEM_HEIGHT - 1;

    return y - func_ov217_021bc3ac();
}

// Shows the parts of the screen above and below the list, where it has been scrolled off its ends, without the list
// pokebw2's ResearchList_UpdateWindow
static void func_ov217_021bbf60(ResearchList *wk) {
    BOOL enable = FALSE;
    int top = func_ov217_021bbf3c(wk, 0);
    int bottom = func_ov217_021bbf4c(wk, SURVEY_COUNT - 1);
    int x1, y1, x2, y2;

    if (top > 0) {
        enable = TRUE;
        x1 = 0;
        x2 = 240;
        y1 = 0;
        y2 = top - 1;
    } else if (bottom < 192) {
        enable = TRUE;
        x1 = 0;
        x2 = 240;
        y1 = bottom + 1;
        y2 = 192;
    }

    if (enable) {
        GX_SetVisibleWnd(GX_WNDMASK_W0);
        G2_SetWnd0Position(x1, y1, x2, y2);
        G2_SetWndOutsidePlane(GX_PLANEMASK_ALL, TRUE);
        G2_SetWnd0InsidePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_OBJ, TRUE);
    } else {
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
    }
}

// pokebw2's ResearchList_UpdateCurrentIcon
static void func_ov217_021bc014(ResearchList *wk) {
    int current = func_ov217_021bbc50(wk);
    ClActor *actor = func_ov217_021bc720(wk, ACTOR_CURRENT);
    ClActorPos pos;

    if (current == SURVEY_NONE) {
        func_0204b3dc(actor, FALSE);
        return;
    }

    pos.x = func_ov217_021bbf28(wk, current);
    pos.y = func_ov217_021bbf3c(wk, current) + 10;
    func_0204b404(actor, &pos, 0);
    func_0204b92c(actor, TRUE);
    func_0204b3dc(actor, TRUE);
}

// pokebw2's ResearchList_PrintCursorSurvey
static void func_ov217_021bc06c(ResearchList *wk) {
    func_ov217_021bc07c(wk, wk->cursor);
}

// pokebw2's ResearchList_PrintSurvey
static void func_ov217_021bc07c(ResearchList *wk, u8 index) {
    func_ov217_021c0f5c(wk->bgFonts[BG_FONT_NAME], data_ov217_021c309e[index]);
    func_ov217_021c0f5c(wk->bgFonts[BG_FONT_DESCRIPTION], data_ov217_021c3094[index]);
    func_ov217_021c0f5c(wk->bgFonts[BG_FONT_QUESTION_1], data_ov217_021c30d0[data_ov217_021c30a8[index]]);
    func_ov217_021c0f5c(wk->bgFonts[BG_FONT_QUESTION_2], data_ov217_021c30d0[data_ov217_021c3080[index]]);
    func_ov217_021c0f5c(wk->bgFonts[BG_FONT_QUESTION_3], data_ov217_021c30d0[data_ov217_021c308a[index]]);
}

// pokebw2's ResearchList_HideSurvey
static void func_ov217_021bc0e8(ResearchList *wk) {
    func_ov217_021c0ff8(wk->bgFonts[BG_FONT_NAME], FALSE);
    func_ov217_021c0ff8(wk->bgFonts[BG_FONT_DESCRIPTION], FALSE);
    func_ov217_021c0ff8(wk->bgFonts[BG_FONT_QUESTION_1], FALSE);
    func_ov217_021c0ff8(wk->bgFonts[BG_FONT_QUESTION_2], FALSE);
    func_ov217_021c0ff8(wk->bgFonts[BG_FONT_QUESTION_3], FALSE);
}

// pokebw2's ResearchList_ShowSurvey
static void func_ov217_021bc128(ResearchList *wk) {
    func_ov217_021c0ff8(wk->bgFonts[BG_FONT_NAME], TRUE);
    func_ov217_021c0ff8(wk->bgFonts[BG_FONT_DESCRIPTION], TRUE);
    func_ov217_021c0ff8(wk->bgFonts[BG_FONT_QUESTION_1], TRUE);
    func_ov217_021c0ff8(wk->bgFonts[BG_FONT_QUESTION_2], TRUE);
    func_ov217_021c0ff8(wk->bgFonts[BG_FONT_QUESTION_3], TRUE);
}

// pokebw2's ResearchList_UpdateScrollBar
static void func_ov217_021bc168(ResearchList *wk) {
    ClActor *actor = func_ov217_021bc720(wk, ACTOR_SCROLL_BAR);
    ClActorPos pos;

    if (!func_ov217_021bc428(wk)) {
        func_0204b3dc(actor, FALSE);
        return;
    }

    pos.x = 248;
    pos.y = func_ov217_021bc1ac(wk);
    func_0204b404(actor, &pos, 0);
    func_0204b3dc(actor, TRUE);
}

// pokebw2's ResearchList_GetBarY
static int func_ov217_021bc1ac(ResearchList *wk) {
    int scroll = func_ov217_021bc3ac();
    int max = func_ov217_021bc40c(wk);
    f32 ratio;
    int y;

    scroll += VIEW_TOP;
    max += VIEW_TOP;
    ratio = (f32)scroll / (f32)max;
    y = ratio * (BAR_BOTTOM - BAR_TOP) + BAR_TOP;
    if (y < BAR_TOP) {
        y = BAR_TOP;
    }
    if (y > BAR_BOTTOM) {
        y = BAR_BOTTOM;
    }
    return y;
}

// pokebw2's ResearchList_BarYToScroll
static int func_ov217_021bc200(ResearchList *wk, int y) {
    f32 ratio;
    int min;
    int max;

    if (y < BAR_TOP) {
        y = BAR_TOP;
    }
    if (y > BAR_BOTTOM) {
        y = BAR_BOTTOM;
    }

    ratio = (f32)(y - BAR_TOP) / (BAR_BOTTOM - BAR_TOP);
    min = 0;
    max = func_ov217_021bc40c(wk) + VIEW_TOP;
    return min + ratio * (max - min);
}

// pokebw2's ResearchList_SlowDrag
static void func_ov217_021bc250(ResearchList *wk) {
    int speed = wk->dragSpeed + wk->dragAccel;

    if (wk->dragSpeed * speed <= 0) {
        speed = 0;
    }
    wk->dragSpeed = speed;
}

// pokebw2's ResearchList_IsDragging
static BOOL func_ov217_021bc264(ResearchList *wk) {
    if (wk->dragSpeed != 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ResearchList_SetDragSpeed
static void func_ov217_021bc274(ResearchList *wk) {
    wk->dragSpeed = wk->scrollY - wk->prevScrollY;
}

// pokebw2's ResearchList_ReleaseDrag
static void func_ov217_021bc280(ResearchList *wk) {
    wk->dragSpeed *= 1.5;
    if (wk->dragSpeed > DRAG_SPEED_MAX) {
        wk->dragSpeed = DRAG_SPEED_MAX;
    } else if (wk->dragSpeed < -DRAG_SPEED_MAX) {
        wk->dragSpeed = -DRAG_SPEED_MAX;
    }
}

// pokebw2's ResearchList_SetDragAccel
static void func_ov217_021bc2b8(ResearchList *wk) {
    if (wk->dragSpeed < 0) {
        wk->dragAccel = 1;
    } else {
        wk->dragAccel = -1;
    }
}

// pokebw2's ResearchList_StartScroll
static void func_ov217_021bc2cc(ResearchList *wk, int from, int to, int frames) {
    wk->scrollFrom = from;
    wk->scrollTo = to;
    wk->scrollFrames = frames;
    wk->scrollFrame = 0;
}

// pokebw2's ResearchList_UpdateScroll
static void func_ov217_021bc2d8(ResearchList *wk) {
    wk->scrollFrame++;
    func_ov217_021bc330(wk, wk->scrollFrom + (wk->scrollTo - wk->scrollFrom) * wk->scrollFrame / wk->scrollFrames);
}

// pokebw2's ResearchList_IsScrollEnd
static BOOL func_ov217_021bc2fc(ResearchList *wk) {
    if (wk->scrollFrames <= wk->scrollFrame) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ResearchList_DragTo
static void func_ov217_021bc30c(ResearchList *wk, int y) {
    if (y < 0) {
        y = 0;
    }
    if (func_ov217_021bc41c(wk) < y) {
        y = func_ov217_021bc41c(wk);
    }
    wk->prevScrollY = wk->scrollY;
    wk->scrollY = y;
}

// pokebw2's ResearchList_SetScrollY
static void func_ov217_021bc330(ResearchList *wk, int y) {
    wk->prevScrollY = y;
    wk->scrollY = y;
}

// Scrolls the view to scrollY when it is outside
// pokebw2's ResearchList_ScrollToView
static void func_ov217_021bc338(ResearchList *wk) {
    int y = wk->scrollY;
    int top = func_ov217_021bc3d4();
    int bottom = func_ov217_021bc3e0();
    int bgY;

    if (y < top) {
        bgY = y - VIEW_TOP;
        GFL_BGSysMoveBGReq(BG_MAIN_LIST, BG_MOVE_SET_Y, bgY);
        GFL_BGSysMoveBGReq(BG_MAIN_TEXT, BG_MOVE_SET_Y, bgY);
    } else if (bottom < y) {
        bgY = y - VIEW_BOTTOM;
        GFL_BGSysMoveBGReq(BG_MAIN_LIST, BG_MOVE_SET_Y, bgY);
        GFL_BGSysMoveBGReq(BG_MAIN_TEXT, BG_MOVE_SET_Y, bgY);
    }
}

// pokebw2's ResearchList_ScrollToDrag
static void func_ov217_021bc380(ResearchList *wk) {
    int y = wk->scrollY - VIEW_TOP;

    if (y < -VIEW_TOP) {
        y = -VIEW_TOP;
    }
    if (func_ov217_021bc40c(wk) < y) {
        y = func_ov217_021bc40c(wk);
    }
    func_ov217_021bc3b8(y);
}

// pokebw2's ResearchList_GetBGScroll
static int func_ov217_021bc3ac(void) {
    return GFL_BGSysGetBGOffsetY(BG_MAIN_LIST);
}

// pokebw2's ResearchList_SetBGScroll
static void func_ov217_021bc3b8(int y) {
    GFL_BGSysMoveBG(BG_MAIN_LIST, BG_MOVE_SET_Y, y);
    GFL_BGSysMoveBG(BG_MAIN_TEXT, BG_MOVE_SET_Y, y);
}

// pokebw2's ResearchList_GetViewTop
static int func_ov217_021bc3d4(void) {
    return func_ov217_021bc3ac() + VIEW_TOP;
}

// pokebw2's ResearchList_GetViewBottom
static int func_ov217_021bc3e0(void) {
    return func_ov217_021bc3ac() + VIEW_BOTTOM;
}

// pokebw2's ResearchList_GetItemAt
static int func_ov217_021bc3ec(int y) {
    return y / ITEM_HEIGHT;
}

// pokebw2's ResearchList_GetItemTop
static int func_ov217_021bc3f8(int index) {
    return index * ITEM_HEIGHT;
}

// pokebw2's ResearchList_GetItemBottom
static int func_ov217_021bc400(int index) {
    return func_ov217_021bc3f8(index) + ITEM_HEIGHT - 1;
}

// pokebw2's ResearchList_GetMaxBGScroll
static int func_ov217_021bc40c(ResearchList *wk) {
    return func_ov217_021bc6dc(wk) * ITEM_HEIGHT - VIEW_BOTTOM;
}

// pokebw2's ResearchList_GetListHeight
static int func_ov217_021bc41c(ResearchList *wk) {
    return func_ov217_021bc40c(wk) + VIEW_BOTTOM;
}

// pokebw2's ResearchList_CanScroll
static BOOL func_ov217_021bc428(ResearchList *wk) {
    if (func_ov217_021bc6dc(wk) * ITEM_HEIGHT > VIEW_BOTTOM - VIEW_TOP) {
        return TRUE;
    }
    return FALSE;
}

void func_ov217_021bc43c(ResearchList *wk) {
    int i;

    for (i = 0; i <= 9; i++) {
        int x = func_ov217_021bbf28(wk, i);
        int y = func_ov217_021bbf3c(wk, i);

        wk->listRects[i].left = x;
        wk->listRects[i].right = x + 0xd8;
        wk->listRects[i].top = y;
        wk->listRects[i].bottom = y + 0x18;
    }
}


void func_ov217_021bc480(void) {
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
}

void func_ov217_021bc490(void) {
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 0);
}

BOOL func_ov217_021bc4a0(void) {
    if (GFL_FadeIsRunning()) {
        return FALSE;
    }
    return TRUE;
}

void func_ov217_021bc4b4(ResearchList *wk) {
    func_0202173c(wk->paletteFade, 1, 0xbbff, 2, 0, 10, 0, wk->tcbMgr);
    func_0202173c(wk->paletteFade, 4, 0xcf, 2, 0, 10, 0, wk->tcbMgr);
    wk->darkened = TRUE;
}

void func_ov217_021bc50c(ResearchList *wk) {
    func_0202173c(wk->paletteFade, 1, 0xbbff, 2, 10, 0, 0, wk->tcbMgr);
    func_0202173c(wk->paletteFade, 4, 0xcf, 2, 10, 0, 0, wk->tcbMgr);
    wk->darkened = FALSE;
}






// Moves the surveys' touch rectangles with the list, clipped at the screen's top and left
static inline void ResearchList_UpdateListRects(ResearchList *wk) {
    int i;

    for (i = 0; i <= SURVEY_COUNT - 1; i++) {
        int x = func_ov217_021bbf28(wk, i);
        int y = func_ov217_021bbf3c(wk, i);
        int left = x;
        int right;
        int top;
        int bottom;

        if (x < 0) {
            left = 0;
        }
        right = x + 216;
        if (right < 0) {
            right = 0;
        }
        top = y;
        if (y < 0) {
            top = 0;
        }
        bottom = y + ITEM_HEIGHT;
        if (bottom < 0) {
            bottom = 0;
        }
        wk->listRects[i].left = left;
        wk->listRects[i].right = right;
        wk->listRects[i].top = top;
        wk->listRects[i].bottom = bottom;
    }
}

static inline void ResearchList_FadeIn(void) {
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
}

static inline void ResearchList_FadeOut(void) {
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 0);
}

static inline BOOL ResearchList_IsFadeDone(void) {
    if (!GFL_FadeIsRunning()) {
        return TRUE;
    }
    return FALSE;
}

// Darkens the main engine's BG palettes but the list's and its cursor's, and its OBJ palettes but the question's and
// the icon of the survey being run, unless that is the one chosen
static inline void ResearchList_Darken(ResearchList *wk) {
    u16 objMask;

    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_MAIN_BG, 0xbbff, 2, 0, 10, 0, wk->tcbMgr);
    if (func_ov217_021bbc50(wk) == func_ov217_021bc6e4(wk)) {
        objMask = 0xcf;
    } else {
        objMask = 0x4cf;
    }
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_MAIN_OBJ, objMask, 2, 0, 10, 0, wk->tcbMgr);
    wk->darkened = TRUE;
}

static inline void ResearchList_Brighten(ResearchList *wk) {
    u16 objMask;

    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_MAIN_BG, 0xbbff, 2, 10, 0, 0, wk->tcbMgr);
    if (func_ov217_021bbc50(wk) == func_ov217_021bc6e4(wk)) {
        objMask = 0xcf;
    } else {
        objMask = 0x4cf;
    }
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_MAIN_OBJ, objMask, 2, 10, 0, 0, wk->tcbMgr);
    wk->darkened = FALSE;
}

static inline void ResearchList_BrightenCurrentIcon(ResearchList *wk) {
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_MAIN_OBJ, 0x400, 0, 10, 0, 0, wk->tcbMgr);
}

// pokebw2's ResearchList_IsPaletteFadeDone
static BOOL func_ov217_021bc55c(ResearchList *wk) {
    if (!func_02021ed8(wk->paletteFade)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ResearchList_StartCommonPaletteAnime
static void func_ov217_021bc574(ResearchList *wk, u32 index) {
    func_ov217_021c09d8(wk->common, index);
}

// pokebw2's ResearchList_StartPaletteAnime
static void func_ov217_021bc580(ResearchList *wk, u32 index) {
    func_ov217_021c29f8(wk->animes[index], data_ov217_021c3260[index].mode, data_ov217_021c3260[index].color);
}

// pokebw2's ResearchList_StopPaletteAnime
static void func_ov217_021bc5a8(ResearchList *wk, u32 index) {
    func_ov217_021c2a00(wk->animes[index]);
}

// pokebw2's ResearchList_IsPaletteAnimeEnd
static BOOL func_ov217_021bc5b8(ResearchList *wk, u32 index) {
    if (!func_ov217_021c2a10(wk->animes[index])) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ResearchList_UpdatePaletteAnimes
static void func_ov217_021bc5d4(ResearchList *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        func_ov217_021c29f0(wk->animes[i]);
    }
}

// pokebw2's ResearchList_UpdateCommonPaletteAnime
static void func_ov217_021bc5f0(ResearchList *wk) {
    func_ov217_021c09d0(wk->common);
}

// pokebw2's ResearchList_GetKeyRepeatFrames
static u32 func_ov217_021bc5fc(ResearchList *wk) {
    return wk->keyRepeatFrames;
}

// pokebw2's ResearchList_CountKeyRepeat
static void func_ov217_021bc600(ResearchList *wk) {
    wk->keyRepeatFrames++;
}

// pokebw2's ResearchList_ResetKeyRepeat
static void func_ov217_021bc608(ResearchList *wk) {
    wk->keyRepeatFrames = 0;
}

// pokebw2's ResearchList_VBlank
static void func_ov217_021bc610(TCB *tcb, void *data) {
    ResearchList *wk = data;

    GFL_BGSysUpdate();
    func_0204a648();
    func_02021d50(wk->paletteFade);
}

// pokebw2's ResearchList_GetGameData
static GameData *func_ov217_021bc628(ResearchList *wk) {
    return func_ov217_021c09b4(wk->common);
}

// pokebw2's ResearchList_SetHeapID
static void func_ov217_021bc634(ResearchList *wk, HeapID heapId) {
    wk->heapId = heapId;
}

// pokebw2's ResearchList_SetCommon
static void func_ov217_021bc638(ResearchList *wk, ResearchCommon *common) {
    wk->common = common;
}

// pokebw2's ResearchList_GetCommon
static ResearchCommon *func_ov217_021bc63c(ResearchList *wk) {
    return wk->common;
}

// pokebw2's ResearchList_SetRecovery
static void func_ov217_021bc640(ResearchList *wk, ResearchListRecovery *recovery) {
    wk->recovery = recovery;
}

// Puts the list back where it was when another screen was shown
// pokebw2's ResearchList_LoadRecovery
static void func_ov217_021bc644(ResearchList *wk) {
    int bgScroll;
    int scrollY;

    if (wk->recovery != NULL) {
        bgScroll = func_ov217_021c0df8(wk->recovery);
        if (bgScroll < -VIEW_TOP) {
            bgScroll = -VIEW_TOP;
        }
        if (bgScroll > MAX_BG_SCROLL) {
            bgScroll = MAX_BG_SCROLL;
        }
        func_ov217_021bc3b8(bgScroll);

        scrollY = func_ov217_021c0df4(wk->recovery);
        if (scrollY < 0) {
            scrollY = 0;
        }
        if (scrollY > SURVEY_COUNT * ITEM_HEIGHT) {
            scrollY = SURVEY_COUNT * ITEM_HEIGHT;
        }
        wk->scrollY = scrollY;
        wk->cursor = func_ov217_021c0df0(wk->recovery);
    }
}

// pokebw2's ResearchList_SaveRecovery
static void func_ov217_021bc688(ResearchList *wk) {
    int bgScroll;
    int scrollY;

    if (wk->recovery != NULL) {
        bgScroll = func_ov217_021bc3ac();
        if (bgScroll < -VIEW_TOP) {
            bgScroll = -VIEW_TOP;
        }
        if (bgScroll > MAX_BG_SCROLL) {
            bgScroll = MAX_BG_SCROLL;
        }
        func_ov217_021c0e04(wk->recovery, bgScroll);

        scrollY = wk->scrollY;
        if (scrollY < 0) {
            scrollY = 0;
        }
        if (scrollY > SURVEY_COUNT * ITEM_HEIGHT) {
            scrollY = SURVEY_COUNT * ITEM_HEIGHT;
        }
        func_ov217_021c0e00(wk->recovery, scrollY);
        func_ov217_021c0dfc(wk->recovery, wk->cursor);
    }
}

// pokebw2's ResearchList_IsForceExit
static BOOL func_ov217_021bc6d0(ResearchList *wk) {
    return func_ov217_021c09f8(func_ov217_021bc63c(wk));
}

// pokebw2's ResearchList_GetSurveyCount
static u8 func_ov217_021bc6dc(ResearchList *wk) {
    return wk->surveyCount;
}

// pokebw2's ResearchList_GetSelected
static u8 func_ov217_021bc6e4(ResearchList *wk) {
    return wk->selected;
}

// pokebw2's ResearchList_SetSelected
static void func_ov217_021bc6ec(ResearchList *wk, u8 index) {
    wk->selected = index;
}

// pokebw2's ResearchList_IsSurvey
static BOOL func_ov217_021bc6f4(ResearchList *wk, u8 index) {
    if (index < func_ov217_021bc6dc(wk)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ResearchList_GetObjRes
static u32 func_ov217_021bc708(ResearchList *wk, u32 index) {
    return wk->objRes[index];
}

// pokebw2's ResearchList_GetUnit
static ClActUnit *func_ov217_021bc714(ResearchList *wk, u32 index) {
    return wk->units[index];
}

// pokebw2's ResearchList_GetActor
static ClActor *func_ov217_021bc720(ResearchList *wk, u32 index) {
    return wk->actors[index];
}

// pokebw2's ResearchList_SetBmpOamVisible
static void func_ov217_021bc72c(ResearchList *wk, u32 index, BOOL visible) {
    func_02025774(wk->bmpOamActors[index], visible);
}

// pokebw2's ResearchList_GetButtonBmpOam
static BmpOamActor *func_ov217_021bc740(ResearchList *wk, u32 button) {
    return wk->bmpOamActors[data_ov217_021c3068[button]];
}

// pokebw2's ResearchList_Alloc
static ResearchList *func_ov217_021bc758(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(ResearchList), FALSE, "research_list.c", 3687);
}

// pokebw2's ResearchList_InitWork
static void func_ov217_021bc774(ResearchList *wk) {
    wk->recovery = NULL;
    wk->seq = SEQ_SETUP;
    wk->seqFrames = 0;
    wk->seqDone = FALSE;
    wk->waitFrames = 15;
    wk->cursor = 0;
    wk->cursorTarget = 0;
    wk->surveyCount = 0;
    wk->buttonCursor = BUTTON_YES;
    wk->selected = SURVEY_NONE;
    wk->tcbMgr = GFL_VBlankGetTCBMgr();
    wk->scrollY = 0;
    wk->prevScrollY = 0;
    wk->scrollFrom = 0;
    wk->scrollTo = 0;
    wk->scrollFrames = 0;
    wk->scrollFrame = 0;
    wk->darkened = FALSE;
    wk->cursorShown = FALSE;
    wk->next = RESEARCH_LIST_NEXT_TOP;
    wk->isEnd = FALSE;
    wk->vblankTask = NULL;
    func_ov217_021bc94c(wk);
    func_ov217_021bc910(wk);
    func_ov217_021bc8d0(wk);
    func_ov217_021bcd6c(wk);
    func_ov217_021bcf4c(wk);
    func_ov217_021bcfb4(wk);
    func_ov217_021bcddc(wk);
    func_ov217_021bd474(wk);
    func_ov217_021bd078(wk);
    func_ov217_021bd50c(wk);
}

// pokebw2's ResearchList_Free
static void func_ov217_021bc818(ResearchList *wk) {
    GFL_HeapFree(wk);
}

// pokebw2's ResearchList_CountSurveys
static void func_ov217_021bc820(ResearchList *wk) {
    EventWork *eventWork = GameData_GetEventWork(func_ov217_021bc628(wk));
    int count = 1;

    if (EventWork_FlagGet(eventWork, FIRST_SURVEY_FLAG) == TRUE) {
        count++;
    }
    if (EventWork_FlagGet(eventWork, FIRST_SURVEY_FLAG + 1) == TRUE) {
        count++;
    }
    if (EventWork_FlagGet(eventWork, FIRST_SURVEY_FLAG + 2) == TRUE) {
        count++;
    }
    if (EventWork_FlagGet(eventWork, FIRST_SURVEY_FLAG + 3) == TRUE) {
        count++;
    }
    if (EventWork_FlagGet(eventWork, FIRST_SURVEY_FLAG + 4) == TRUE) {
        count++;
    }
    if (EventWork_FlagGet(eventWork, FIRST_SURVEY_FLAG + 5) == TRUE) {
        count++;
    }
    if (EventWork_FlagGet(eventWork, FIRST_SURVEY_FLAG + 6) == TRUE) {
        count++;
    }
    if (EventWork_FlagGet(eventWork, FIRST_SURVEY_FLAG + 7) == TRUE) {
        count++;
    }
    if (EventWork_FlagGet(eventWork, LAST_SURVEY_FLAG) == TRUE) {
        count++;
    }
    wk->surveyCount = count;
}

// pokebw2's ResearchList_CreateFont
static void func_ov217_021bc8b4(ResearchList *wk) {
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
}

// pokebw2's ResearchList_ClearFont
static void func_ov217_021bc8d0(ResearchList *wk) {
    wk->font = NULL;
}

// pokebw2's ResearchList_DeleteFont
static void func_ov217_021bc8d8(ResearchList *wk) {
    GFL_FontFree(wk->font);
}

// pokebw2's ResearchList_LoadMsgData
static void func_ov217_021bc8e4(ResearchList *wk) {
    int i;

    for (i = 0; i < MSG_DATA_COUNT; i++) {
        wk->msgData[i] = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, data_ov217_021c3078[i], wk->heapId);
    }
}

// pokebw2's ResearchList_ClearMsgData
static void func_ov217_021bc910(ResearchList *wk) {
    int i;

    for (i = 0; i < MSG_DATA_COUNT; i++) {
        wk->msgData[i] = NULL;
    }
}

// pokebw2's ResearchList_FreeMsgData
static void func_ov217_021bc924(ResearchList *wk) {
    int i;

    for (i = 0; i < MSG_DATA_COUNT; i++) {
        GFL_MsgDataFree(wk->msgData[i]);
    }
}

// pokebw2's ResearchList_CreateQueue
static void func_ov217_021bc93c(ResearchList *wk) {
    wk->queue = func_ov217_021c0e08(10, wk->heapId);
}

// pokebw2's ResearchList_ClearQueue
static void func_ov217_021bc94c(ResearchList *wk) {
    wk->queue = NULL;
}

// pokebw2's ResearchList_DeleteQueue
static void func_ov217_021bc954(ResearchList *wk) {
    func_ov217_021c0e3c(wk->queue);
}

// pokebw2's ResearchList_InitTouchRects
static void func_ov217_021bc960(ResearchList *wk) {
    int i;

    for (i = 0; i < BUTTON_RECT_COUNT; i++) {
        wk->buttonRects[i].left = data_ov217_021c30b2[i].left;
        wk->buttonRects[i].right = data_ov217_021c30b2[i].right;
        wk->buttonRects[i].top = data_ov217_021c30b2[i].top;
        wk->buttonRects[i].bottom = data_ov217_021c30b2[i].bottom;
    }
    for (i = 0; i < LIST_RECT_COUNT; i++) {
        wk->listRects[i].left = data_ov217_021c3190[i].left;
        wk->listRects[i].right = data_ov217_021c3190[i].right;
        wk->listRects[i].top = data_ov217_021c3190[i].top;
        wk->listRects[i].bottom = data_ov217_021c3190[i].bottom;
    }
    for (i = 0; i < SCROLL_RECT_COUNT; i++) {
        wk->scrollRects[i].left = data_ov217_021c3070[i].left;
        wk->scrollRects[i].right = data_ov217_021c3070[i].right;
        wk->scrollRects[i].top = data_ov217_021c3070[i].top;
        wk->scrollRects[i].bottom = data_ov217_021c3070[i].bottom;
    }
}

// pokebw2's ResearchList_InitBG
static void func_ov217_021bc9fc(ResearchList *wk) {
    GFL_BGSysSetLCDConfig(&data_ov217_021c30c0);
    GFL_BGSysCreateBG(BG_SUB_TITLE, &data_ov217_021c3110, BGMODE_TEXT);
    GFL_BGSysCreateBG(BG_SUB_TEXT, &data_ov217_021c3130, BGMODE_TEXT);
    GFL_BGSysCreateBG(BG_MAIN_FRAME, &data_ov217_021c30f0, BGMODE_TEXT);
    GFL_BGSysCreateBG(BG_MAIN_LIST, &data_ov217_021c3150, BGMODE_TEXT);
    GFL_BGSysCreateBG(BG_MAIN_TEXT, &data_ov217_021c3170, BGMODE_TEXT);
    GFL_BGSysSetBGEnabled(RESEARCH_BG_SUB_BACK, TRUE);
    GFL_BGSysSetBGEnabled(RESEARCH_BG_SUB_PATTERN, TRUE);
    GFL_BGSysSetBGEnabled(BG_SUB_TITLE, TRUE);
    GFL_BGSysSetBGEnabled(BG_SUB_TEXT, TRUE);
    GFL_BGSysSetBGEnabled(RESEARCH_BG_MAIN_FRAME, TRUE);
    GFL_BGSysSetBGEnabled(BG_MAIN_FRAME, TRUE);
    GFL_BGSysSetBGEnabled(BG_MAIN_LIST, TRUE);
    GFL_BGSysSetBGEnabled(BG_MAIN_TEXT, TRUE);
    G2S_SetBlendAlpha(GX_PLANEMASK_BG1, GX_PLANEMASK_BG0, 16, 5);
    G2_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG1, 16, 5);
    BmpWin_InitAllocator(wk->heapId);
}

// pokebw2's ResearchList_ExitBG
static void func_ov217_021bcac0(ResearchList *wk) {
    BmpWin_FreeAllocator();
    GFL_BGSysReleaseBG(BG_MAIN_TEXT);
    GFL_BGSysReleaseBG(BG_MAIN_LIST);
    GFL_BGSysReleaseBG(BG_MAIN_FRAME);
    GFL_BGSysReleaseBG(BG_SUB_TEXT);
    GFL_BGSysReleaseBG(BG_SUB_TITLE);
}

// pokebw2's ResearchList_LoadSubBG
static void func_ov217_021bcae8(ResearchList *wk) {
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, HEAPID_TAIL(wk->heapId));
    void *file = GFL_ArcToolReadHeapNew(handle, 13, wk->heapId);
    NNSG2dScreenData *screen;

    NNS_G2dGetUnpackedScreenData(file, &screen);
    GFL_BGSysLoadScrAreaAll(BG_SUB_TITLE, screen->rawData, 0, 0, 32, 24);
    GFL_BGSysQueueScrLoad(BG_SUB_TITLE);
    GFL_HeapFree(file);
    GFL_ArcToolFree(handle);
}

// pokebw2's ResearchList_UnloadSubBG
static void func_ov217_021bcb44(ResearchList *wk) {
}

// pokebw2's ResearchList_InitSubText
static void func_ov217_021bcb48(ResearchList *wk) {
    GFL_BGSysFillChar(BG_SUB_TEXT, 0, 1, 0);
    GFL_BGSysClearScr(BG_SUB_TEXT);
}

// pokebw2's ResearchList_ExitSubText
static void func_ov217_021bcb60(ResearchList *wk) {
    GFL_BGSysFreeFilledChar(BG_SUB_TEXT, 1, 0);
}

// pokebw2's ResearchList_LoadFrameBG
static void func_ov217_021bcb70(ResearchList *wk) {
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, HEAPID_TAIL(wk->heapId));
    void *file = GFL_ArcToolReadHeapNew(handle, 3, wk->heapId);
    NNSG2dScreenData *screen;

    NNS_G2dGetUnpackedScreenData(file, &screen);
    GFL_BGSysLoadScrAreaAll(BG_MAIN_FRAME, screen->rawData, 0, 0, 32, 24);
    GFL_BGSysQueueScrLoad(BG_MAIN_FRAME);
    GFL_HeapFree(file);
    GFL_ArcToolFree(handle);
}

// pokebw2's ResearchList_UnloadFrameBG
static void func_ov217_021bcbcc(ResearchList *wk) {
}

// Loads the rows of the list's screen for the surveys that are unlocked
// pokebw2's ResearchList_LoadListBG
static void func_ov217_021bcbd0(ResearchList *wk) {
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, HEAPID_TAIL(wk->heapId));
    int rows = func_ov217_021bc6dc(wk) * ITEM_ROWS;
    void *file = GFL_ArcToolReadHeapNew(handle, 5, wk->heapId);
    NNSG2dScreenData *screen;

    NNS_G2dGetUnpackedScreenData(file, &screen);
    GFL_BGSysLoadScrAreaAll(BG_MAIN_LIST, screen->rawData, 0, 0, 32, rows);
    GFL_BGSysQueueScrLoad(BG_MAIN_LIST);
    GFL_HeapFree(file);
    GFL_ArcToolFree(handle);
}

// pokebw2's ResearchList_UnloadListBG
static void func_ov217_021bcc3c(ResearchList *wk) {
}

// pokebw2's ResearchList_InitMainText
static void func_ov217_021bcc40(ResearchList *wk) {
    GFL_BGSysFillChar(BG_MAIN_TEXT, 0, 1, 0);
    GFL_BGSysClearScr(BG_MAIN_TEXT);
}

// pokebw2's ResearchList_ExitMainText
static void func_ov217_021bcc58(ResearchList *wk) {
    GFL_BGSysFreeFilledChar(BG_MAIN_TEXT, 1, 0);
}

// pokebw2's ResearchList_CreateBGFonts
static void func_ov217_021bcc68(ResearchList *wk) {
    int i;

    for (i = 0; i < BG_FONT_COUNT; i++) {
        BGFontSetup setup;

        setup.window.bg = data_ov217_021c33c8[i].window.bg;
        setup.window.x = data_ov217_021c33c8[i].window.x;
        setup.window.y = data_ov217_021c33c8[i].window.y;
        setup.window.width = data_ov217_021c33c8[i].window.width;
        setup.window.height = data_ov217_021c33c8[i].window.height;
        setup.window.textX = data_ov217_021c33c8[i].window.textX;
        setup.window.textY = data_ov217_021c33c8[i].window.textY;
        setup.window.palette = data_ov217_021c33c8[i].window.palette;
        setup.window.letterColor = data_ov217_021c33c8[i].window.letterColor;
        setup.window.shadowColor = data_ov217_021c33c8[i].window.shadowColor;
        setup.window.backColor = data_ov217_021c33c8[i].window.backColor;
        setup.centered = data_ov217_021c33c8[i].centered;
        wk->bgFonts[i] = func_ov217_021c0ef4(&setup, wk->font, wk->msgData[data_ov217_021c33c8[i].msgDataIndex], wk->heapId);
        func_ov217_021c0f5c(wk->bgFonts[i], data_ov217_021c33c8[i].strId);
    }

    for (i = 0; i < func_ov217_021bc6dc(wk); i++) {
        BGFontSetup setup;

        setup.window.bg = data_ov217_021c3520[i].window.bg;
        setup.window.x = data_ov217_021c3520[i].window.x;
        setup.window.y = data_ov217_021c3520[i].window.y;
        setup.window.width = data_ov217_021c3520[i].window.width;
        setup.window.height = data_ov217_021c3520[i].window.height;
        setup.window.textX = data_ov217_021c3520[i].window.textX;
        setup.window.textY = data_ov217_021c3520[i].window.textY;
        setup.window.palette = data_ov217_021c3520[i].window.palette;
        setup.window.letterColor = data_ov217_021c3520[i].window.letterColor;
        setup.window.shadowColor = data_ov217_021c3520[i].window.shadowColor;
        setup.window.backColor = data_ov217_021c3520[i].window.backColor;
        setup.centered = data_ov217_021c3520[i].centered;
        wk->itemFonts[i] = func_ov217_021c0ef4(&setup, wk->font, wk->msgData[data_ov217_021c3520[i].msgDataIndex], wk->heapId);
        func_ov217_021c0f5c(wk->itemFonts[i], data_ov217_021c3520[i].strId);
    }
}

// pokebw2's ResearchList_ClearBGFonts
static void func_ov217_021bcd6c(ResearchList *wk) {
    int i;

    for (i = 0; i < BG_FONT_COUNT; i++) {
        wk->bgFonts[i] = NULL;
    }
    for (i = 0; i < SURVEY_COUNT; i++) {
        wk->itemFonts[i] = NULL;
    }
}

// pokebw2's ResearchList_DeleteBGFonts
static void func_ov217_021bcd90(ResearchList *wk) {
    int i;

    for (i = 0; i < BG_FONT_COUNT; i++) {
        func_ov217_021c0f48(wk->bgFonts[i]);
        wk->bgFonts[i] = NULL;
    }
    for (i = 0; i < func_ov217_021bc6dc(wk); i++) {
        func_ov217_021c0f48(wk->itemFonts[i]);
        wk->itemFonts[i] = NULL;
    }
}

// pokebw2's ResearchList_ClearObjRes
static void func_ov217_021bcddc(ResearchList *wk) {
    int i;

    for (i = 0; i < OBJ_RES_COUNT; i++) {
        wk->objRes[i] = 0;
    }
}

// pokebw2's ResearchList_LoadSubObjRes
static void func_ov217_021bcdf0(ResearchList *wk) {
    HeapID heapId = wk->heapId;
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, HEAPID_TAIL(heapId));
    u32 chars = func_0204a6c8(handle, 15, FALSE, CLACT_VRAM_SUB, heapId);
    u32 palette = func_0204abf0(handle, 16, CLACT_VRAM_SUB, 0, heapId);
    u32 cellAnims = func_0204af28(handle, 14, 17, heapId);

    wk->objRes[OBJ_RES_SUB_CHARS] = chars;
    wk->objRes[OBJ_RES_SUB_PALETTE] = palette;
    wk->objRes[OBJ_RES_SUB_CELL_ANIMS] = cellAnims;
    GFL_ArcToolFree(handle);
}

// pokebw2's ResearchList_FreeSubObjRes
static void func_ov217_021bce5c(ResearchList *wk) {
    func_0204a8d4(wk->objRes[OBJ_RES_SUB_CHARS]);
    func_0204ada4(wk->objRes[OBJ_RES_SUB_PALETTE]);
    func_0204afd8(wk->objRes[OBJ_RES_SUB_CELL_ANIMS]);
}

// pokebw2's ResearchList_LoadMainObjRes
static void func_ov217_021bce7c(ResearchList *wk) {
    HeapID heapId = wk->heapId;
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, HEAPID_TAIL(heapId));
    u32 chars = func_0204a6c8(handle, 15, FALSE, CLACT_VRAM_MAIN, heapId);
    u32 palette = func_0204ac18(handle, 16, CLACT_VRAM_MAIN, 0xc0, 0, 4, heapId);
    u32 cellAnims = func_0204af28(handle, 14, 17, heapId);
    ArcTool *commonHandle;
    u32 buttonPalette;

    GFL_ArcToolFree(handle);

    commonHandle = GFL_ArcSysCreateFileHandle(ARCID_APP_MENU_COMMON, heapId);
    buttonPalette = func_0204ac18(commonHandle, 31, CLACT_VRAM_MAIN, 0x80, 0, 2, heapId);
    GFL_ArcToolFree(commonHandle);

    wk->objRes[OBJ_RES_MAIN_CHARS] = chars;
    wk->objRes[OBJ_RES_MAIN_PALETTE] = palette;
    wk->objRes[OBJ_RES_MAIN_CELL_ANIMS] = cellAnims;
    wk->objRes[OBJ_RES_BUTTON_PALETTE] = buttonPalette;
}

// pokebw2's ResearchList_FreeMainObjRes
static void func_ov217_021bcf20(ResearchList *wk) {
    func_0204a8d4(wk->objRes[OBJ_RES_MAIN_CHARS]);
    func_0204ada4(wk->objRes[OBJ_RES_MAIN_PALETTE]);
    func_0204afd8(wk->objRes[OBJ_RES_MAIN_CELL_ANIMS]);
    func_0204ada4(wk->objRes[OBJ_RES_BUTTON_PALETTE]);
}

// pokebw2's ResearchList_ClearUnits
static void func_ov217_021bcf4c(ResearchList *wk) {
    int i;

    for (i = 0; i < UNIT_COUNT; i++) {
        wk->units[i] = NULL;
    }
}

// pokebw2's ResearchList_CreateUnits
static void func_ov217_021bcf68(ResearchList *wk) {
    int i;

    for (i = 0; i < UNIT_COUNT; i++) {
        wk->units[i] = func_0204b100(data_ov217_021c3060[i], data_ov217_021c305c[i], wk->heapId);
    }
}

// pokebw2's ResearchList_DeleteUnits
static void func_ov217_021bcf98(ResearchList *wk) {
    int i;

    for (i = 0; i < UNIT_COUNT; i++) {
        func_0204b1cc(wk->units[i]);
    }
}

// pokebw2's ResearchList_ClearActors
static void func_ov217_021bcfb4(ResearchList *wk) {
    int i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        wk->actors[i] = NULL;
    }
}

// pokebw2's ResearchList_CreateActors
static void func_ov217_021bcfd0(ResearchList *wk) {
    int i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        const ResearchActorSetup *entry = &data_ov217_021c320c[i];
        ClActorSetup setup;
        ClActUnit *unit;
        u32 chars;
        u32 palette;
        u32 cellAnims;

        setup.x = entry->x;
        setup.y = entry->y;
        setup.sequence = entry->sequence;
        setup.priority = entry->priority;
        setup.bgPriority = entry->bgPriority;
        unit = func_ov217_021bc714(wk, entry->unit);
        chars = func_ov217_021bc708(wk, entry->chars);
        palette = func_ov217_021bc708(wk, entry->palette);
        cellAnims = func_ov217_021bc708(wk, entry->cellAnims);
        wk->actors[i] = func_0204b294(unit, chars, palette, cellAnims, &setup, entry->surface, wk->heapId);
        func_0204b3dc(wk->actors[i], FALSE);
    }
}

// pokebw2's ResearchList_DeleteActors
static void func_ov217_021bd05c(ResearchList *wk) {
    int i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        func_0204b3b4(wk->actors[i]);
    }
}

// pokebw2's ResearchList_ClearBitmaps
static void func_ov217_021bd078(ResearchList *wk) {
    int i;

    for (i = 0; i < BMP_COUNT; i++) {
        wk->bitmaps[i] = NULL;
    }
}

// pokebw2's ResearchList_CreateBitmaps
static void func_ov217_021bd094(ResearchList *wk) {
    int i;

    for (i = 0; i < BMP_COUNT; i++) {
        wk->bitmaps[i] =
            GFL_BitmapCreate(data_ov217_021c3470[i].width, data_ov217_021c3470[i].height, data_ov217_021c3470[i].tileSize, wk->heapId);
    }
}

// pokebw2's ResearchList_DrawBitmaps
static void func_ov217_021bd0c4(ResearchList *wk) {
    int i;

    for (i = 0; i < BMP_COUNT; i++) {
        const BitmapEntry *entry = &data_ov217_021c3470[i];

        if (entry->hasBase) {
            GFLBitmap *base = GFL_G2DIOLoadBitmap(entry->arcId, entry->fileId, FALSE, wk->heapId);
            u16 color;
            StrBuf *str;

            GFL_BitmapCopyArea(base, wk->bitmaps[i], 0, 0, 0, 0, entry->width * 8, entry->height * 8, entry->backColor);
            color = PRINT_COLOR(entry->letterColor, entry->shadowColor, entry->backColor);
            str = GFL_MsgDataLoadStrbufNew(wk->msgData[entry->msgDataIndex], entry->strId);
            GFL_TextRendererDrawToBitmapEx(wk->bitmaps[i], entry->textX, entry->textY, str, wk->font, color);
            GFL_HeapFree(str);
            GFL_BitmapFree(base);
        }
    }
}

// Draws the Yes button from the tiles of the apps' button, 3 by 3 of them for its corners, edges and middle
// pokebw2's ResearchList_DrawYesButton
static void func_ov217_021bd178(ResearchList *wk) {
    int tiles[3][10] = {
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
        { 3, 4, 4, 4, 4, 4, 4, 4, 4, 5 },
        { 6, 7, 7, 7, 7, 7, 7, 7, 7, 8 },
    };
    const BitmapEntry *entry = &data_ov217_021c3470[BMP_YES];
    int x, y;
    GFLBitmap *base;
    u16 color;
    StrBuf *str;

    base = GFL_G2DIOLoadBitmap(entry->arcId, entry->fileId, FALSE, wk->heapId);
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 10; x++) {
            GFL_BitmapCopyArea(base, wk->bitmaps[BMP_YES], tiles[y][x] % 3 * 8, tiles[y][x] / 3 * 8, x * 8, y * 8, 8, 8,
                               0);
        }
    }
    GFL_BitmapFree(base);

    color = PRINT_COLOR(entry->letterColor, entry->shadowColor, entry->backColor);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData[entry->msgDataIndex], entry->strId);
    GFL_TextRendererDrawToBitmapEx(wk->bitmaps[BMP_YES], entry->textX, entry->textY, str, wk->font, color);
    GFL_HeapFree(str);
}

// pokebw2's ResearchList_DrawNoButton
static void func_ov217_021bd274(ResearchList *wk) {
    int tiles[3][10] = {
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
        { 3, 4, 4, 4, 4, 4, 4, 4, 4, 5 },
        { 6, 7, 7, 7, 7, 7, 7, 7, 7, 8 },
    };
    const BitmapEntry *entry = &data_ov217_021c3470[BMP_NO];
    int x, y;
    GFLBitmap *base;
    u16 color;
    StrBuf *str;

    base = GFL_G2DIOLoadBitmap(entry->arcId, entry->fileId, FALSE, wk->heapId);
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 10; x++) {
            GFL_BitmapCopyArea(base, wk->bitmaps[BMP_NO], tiles[y][x] % 3 * 8, tiles[y][x] / 3 * 8, x * 8, y * 8, 8, 8,
                               0);
        }
    }
    GFL_BitmapFree(base);

    color = PRINT_COLOR(entry->letterColor, entry->shadowColor, entry->backColor);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData[entry->msgDataIndex], entry->strId);
    GFL_TextRendererDrawToBitmapEx(wk->bitmaps[BMP_NO], entry->textX, entry->textY, str, wk->font, color);
    GFL_HeapFree(str);
}

// pokebw2's ResearchList_FreeBitmaps
static void func_ov217_021bd370(ResearchList *wk) {
    int i;

    for (i = 0; i < BMP_COUNT; i++) {
        GFL_BitmapFree(wk->bitmaps[i]);
    }
}

// pokebw2's ResearchList_CreateBmpOam
static void func_ov217_021bd38c(ResearchList *wk) {
    wk->bmpOamSys = func_02025538(wk->heapId, wk->units[UNIT_BMP_OAM]);
}

// pokebw2's ResearchList_DeleteBmpOam
static void func_ov217_021bd3a4(ResearchList *wk) {
    func_02025588(wk->bmpOamSys);
}

// pokebw2's ResearchList_CreateBmpOamActors
static void func_ov217_021bd3b4(ResearchList *wk) {
    int i;

    for (i = 0; i < BMP_COUNT; i++) {
        BmpOamActorSetup setup;

        setup.bitmap = wk->bitmaps[i];
        setup.x = data_ov217_021c31bc[i].x;
        setup.y = data_ov217_021c31bc[i].y;
        setup.palette = func_ov217_021bc708(wk, data_ov217_021c31bc[i].palette);
        setup.paletteOffset = data_ov217_021c31bc[i].palOffset;
        setup.priority = data_ov217_021c31bc[i].priority;
        setup.bgPriority = data_ov217_021c31bc[i].bgPriority;
        setup.surface = data_ov217_021c31bc[i].surface;
        setup.vramType = data_ov217_021c31bc[i].vramType;
        wk->bmpOamActors[i] = func_020255a0(wk->bmpOamSys, &setup);
        func_02025774(wk->bmpOamActors[i], FALSE);
        func_020257d0(wk->bmpOamActors[i]);
    }
}

// pokebw2's ResearchList_DeleteBmpOamActors
static void func_ov217_021bd458(ResearchList *wk) {
    int i;

    for (i = 0; i < BMP_COUNT; i++) {
        func_0202570c(wk->bmpOamActors[i]);
    }
}

// pokebw2's ResearchList_ClearPaletteFade
static void func_ov217_021bd474(ResearchList *wk) {
    wk->paletteFade = NULL;
}

// pokebw2's ResearchList_CreatePaletteFade
static void func_ov217_021bd47c(ResearchList *wk) {
    wk->paletteFade = func_02021518(wk->heapId);
    func_0202155c(wk->paletteFade, PALFADE_BUFFER_MAIN_BG, 0x200, wk->heapId);
    func_0202155c(wk->paletteFade, PALFADE_BUFFER_MAIN_OBJ, 0x200, wk->heapId);
    func_020216d4(wk->paletteFade, PALFADE_VRAM_MAIN_BG, 0, 0x200);
    func_020216d4(wk->paletteFade, PALFADE_VRAM_MAIN_OBJ, 0, 0x200);
}

// pokebw2's ResearchList_DeletePaletteFade
static void func_ov217_021bd4cc(ResearchList *wk) {
    func_020215a0(wk->paletteFade, PALFADE_BUFFER_MAIN_BG);
    func_020215a0(wk->paletteFade, PALFADE_BUFFER_MAIN_OBJ);
    func_02021540(wk->paletteFade);
}

// pokebw2's ResearchList_CreatePaletteAnimes
static void func_ov217_021bd4f0(ResearchList *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        wk->animes[i] = func_ov217_021c29c8(wk->heapId);
    }
}

// pokebw2's ResearchList_ClearPaletteAnimes
static void func_ov217_021bd50c(ResearchList *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        wk->animes[i] = NULL;
    }
}

// pokebw2's ResearchList_DeletePaletteAnimes
static void func_ov217_021bd520(ResearchList *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        func_ov217_021c29d8(wk->animes[i]);
    }
}

// pokebw2's ResearchList_SetupPaletteAnimes
static void func_ov217_021bd53c(ResearchList *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        func_ov217_021c29e8(wk->animes[i], data_ov217_021c3260[i].dst, data_ov217_021c3260[i].src,
                           data_ov217_021c3260[i].count);
    }
}

// pokebw2's ResearchList_RestorePalettes
static void func_ov217_021bd56c(ResearchList *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        func_ov217_021c2a08(wk->animes[i]);
    }
}

// pokebw2's ResearchList_ShowCenterActor
static void func_ov217_021bd588(ResearchList *wk) {
    ClActor *actor = func_ov217_021bc720(wk, ACTOR_CENTER);
    ClActorPos pos;

    pos.x = 128;
    pos.y = 96;
    func_0204b404(actor, &pos, 0);
    func_0204b3dc(actor, TRUE);
}

// pokebw2's ResearchList_ShowCommIcon
static void func_ov217_021bd5b4(ResearchList *wk) {
    func_0203d34c(TRUE, wk->heapId);
}

// pokebw2's ResearchList_SetVBlank
static void func_ov217_021bd5c4(ResearchList *wk) {
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov217_021bc610, wk, 0);
}

// pokebw2's ResearchList_ResetVBlank
static void func_ov217_021bd5e0(ResearchList *wk) {
    GFL_TCBRemove(wk->vblankTask);
    wk->vblankTask = NULL;
}
