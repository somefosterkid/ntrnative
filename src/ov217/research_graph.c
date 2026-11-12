// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov310/research_graph.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/research_radar/arrow.h"
#include "app/research_radar/bg_font.h"
#include "app/research_radar/circle_graph.h"
#include "app/research_radar/palette_anime.h"
#include "app/research_radar/percentage.h"
#include "app/research_radar/queue.h"
#include "app/research_radar/research_common.h"
#include "app/research_radar/research_data.h"
#include "app/research_radar/research_graph.h"
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
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "nnsys/g2d.h"
#include "save/save_control.h"
#include "system/bmp_oam.h"
#include "system/game_beacon.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "save/hall_of_fame.h"
#include "save/playtime.h"
#include "save/survey_savedata.h"

// The Research Radar's graph of the survey being run: a circle graph of the answers to each of its three questions,
// today's or in all, with the answer chosen pointed at by an arrow, and the player's own answer marked. The results are
// hidden until the player opens them, and they are updated when new ones come in by beacon. The screen is used by touch
// or with a cursor, which moves between the question, the answer, the player's answer, the count and the button that
// opens the results

// The sequences of the screen, which run one after another from a queue
enum {
    SEQ_SETUP,
    // Restarts the beacon communication, which brings in new results
    SEQ_RESTART_COMM,
    SEQ_TOUCH,
    SEQ_CURSOR,
    // Shows the results for the first time, growing the graph
    SEQ_OPEN,
    // Shows the percentages of the answers one after another
    SEQ_SHOW_PERCENTAGES,
    // Flashes the screen white and back
    SEQ_FLASH_IN,
    SEQ_FLASH_OUT,
    // Shows the results updated by new ones, growing the new graph over the old one
    SEQ_UPDATE_TOUCH,
    SEQ_UPDATE_CURSOR,
    SEQ_FADE_OUT,
    SEQ_WAIT,
    SEQ_TEARDOWN,
    SEQ_END,
};

// The BGs: the graph on the main engine's BG 0, in 3D, its frame on BG 2 and its text on BG 3, and the sub engine's
// title on BG 6 and its text on BG 7, in front of the proc's BGs
#define BG_MAIN_3D 0
#define BG_MAIN_GRAPH 2
#define BG_MAIN_TEXT 3
#define BG_SUB_TITLE 6
#define BG_SUB_TEXT 7

// The palettes of the graph's BG for the areas of the cursor, and for the one the cursor is on
#define BG_PALETTE_NORMAL 6
#define BG_PALETTE_CURSOR 10

// The objects' resources: the main engine's, the palette of the button, which comes from the graphics the apps share,
// and the sub engine's
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

// The units: the actors', the bitmaps' and the percentages'
#define UNIT_COUNT 4
#define UNIT_ACTORS 1
#define UNIT_BMP_OAM 2
#define UNIT_PERCENTAGES 3

// The actors: the arrows on either side of the cursor, and the mark of the player's answer in the graph, with the one
// that explains it
enum {
    ACTOR_LEFT,
    ACTOR_RIGHT,
    ACTOR_LEGEND,
    ACTOR_MARKER,
    ACTOR_COUNT,
};

// The bitmaps shown as actors: the message shown while the graph grows, and the button that opens the results
enum {
    BMP_MESSAGE,
    BMP_BUTTON,
    BMP_COUNT,
};

// The texts: the sub screen's title, survey and question, and the main screen's question, answer, player's answer,
// count of people, the message that nobody has answered, and the one that the results are being updated
enum {
    BG_FONT_TITLE,
    BG_FONT_SURVEY,
    BG_FONT_QUESTION,
    BG_FONT_QUESTION_TITLE,
    BG_FONT_ANSWER,
    BG_FONT_PLAYER_ANSWER,
    BG_FONT_TOTAL,
    BG_FONT_NO_ANSWERS,
    BG_FONT_UPDATING,
    BG_FONT_COUNT,
};

// The palette animations: the cursor, a cursor move and the button pressed, the cursor on the button and moved to
// it, the button disabled and enabled, and the frame
enum {
    ANIME_CURSOR,
    ANIME_MOVE,
    ANIME_DECIDE,
    ANIME_BUTTON_CURSOR,
    ANIME_BUTTON_MOVE,
    ANIME_BUTTON_DISABLE,
    ANIME_BUTTON_ENABLE,
    ANIME_FRAME,
    ANIME_COUNT,
};

// The places of the cursor, in its order
enum {
    CURSOR_QUESTION,
    CURSOR_ANSWER,
    CURSOR_PLAYER_ANSWER,
    CURSOR_MODE,
    CURSOR_BUTTON,
    CURSOR_COUNT,
};

// The touch rectangles: the arrows on either side of the cursor, which move with it, the places of the cursor, the
// graph and the button
enum {
    TOUCH_LEFT,
    TOUCH_RIGHT,
    TOUCH_QUESTION,
    TOUCH_ANSWER,
    TOUCH_PLAYER_ANSWER,
    TOUCH_MODE,
    TOUCH_GRAPH,
    TOUCH_BUTTON,
    TOUCH_END,
    TOUCH_RECT_COUNT,
};

// The counts the graph shows: today's answers or all of them
enum {
    MODE_TODAY,
    MODE_TOTAL,
    MODE_COUNT,
};

#define MSG_DATA_COUNT 3
#define MSG_DATA_ANSWERS 2
#define PERCENTAGE_COUNT 10
#define SURVEY_COUNT 10

// The answers' IDs, which index the colors of their slices
#define ANSWER_ID_COUNT 148

// The counts are shown up to this
#define COUNT_MAX 999999

// A percentage under this isn't shown on the graph
#define PERCENTAGE_MIN 10

// The question of the time played, whose answer the player doesn't give: it is worked out from the hours played, one
// answer for each 10 hours up to 100
#define QUESTION_PLAY_TIME 29
#define ANSWER_PLAY_TIME_FIRST 0x87

// The frames the graph grows for when it is opened and updated, playing a drum roll
#define OPEN_FRAMES 120
#define UPDATE_FRAMES 60

// The frames after the results are updated before new results are shown
#define UPDATE_WAIT_FRAMES 240

// The frames between two percentages being shown
#define PERCENTAGE_INTERVAL 10

struct ResearchGraph {
    ResearchCommon *common;
    HeapID heapId;
    Font *font;
    MsgData *msgData[MSG_DATA_COUNT];
    WordSet *wordSet;
    Queue *queue;
    u32 seq;
    u32 seqState;
    u32 seqFrames;
    u32 waitFrames;
    // The frames since the results were last updated, up to UPDATE_WAIT_FRAMES
    u32 updateFrames;
    int cursor;
    ResearchData data;
    // The question shown and its answer chosen, by their indices in the data
    u8 question;
    u8 answer;
    // The graph of each mode, and the graphs that grow over them when the results are updated
    CircleGraph *graphs[MODE_COUNT];
    CircleGraph *nextGraphs[MODE_COUNT];
    u32 mode;
    Arrow *arrow;
    Percentage *percentages[PERCENTAGE_COUNT];
    // How many percentages are set up, and how many of them are shown
    u8 percentageCount;
    u8 percentagesShown;
    TouchRect touchRects[TOUCH_RECT_COUNT];
    PaletteAnime *animes[ANIME_COUNT];
    BGFont *bgFonts[BG_FONT_COUNT];
    u32 objRes[OBJ_RES_COUNT];
    ClActUnit *units[UNIT_COUNT];
    ClActor *actors[ACTOR_COUNT];
    BmpOamSys *bmpOamSys;
    BmpOamActor *bmpOamActors[BMP_COUNT];
    GFLBitmap *bitmaps[BMP_COUNT];
    PaletteFade *paletteFade;
    TCBManager *tcbMgr;
    TCB *vblankTask;
    // Whether the graphs are drawn and animated, while the screen is set up
    BOOL active;
    // Whether the results have been opened
    BOOL resultsShown;
    // Whether they were opened by touch
    BOOL touchOpened;
    // Whether the results are being updated
    BOOL updating;
    // Whether UPDATE_WAIT_FRAMES have passed since the last update, and whether new results came in
    BOOL updateReady;
    BOOL newResults;
    BOOL seqDone;
    BOOL isEnd;
    u32 next;
};

// Black 2's, not checked against Black
typedef struct {
    BGFontWindow window;
    u32 msgDataIndex;
    u32 strId;
    BOOL centered;
} BGFontEntry;

// A bitmap shown as an actor, with the text drawn into it, over a picture loaded from an archive when hasBase is set
// Black 2's; twinned code confirms the offsets of width, height, msgDataIndex, strId, textX, textY, letterColor,
// shadowColor, backColor, arcId, fileId and hasBase in Black
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
// Black 2's, not checked against Black
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

// A place of the cursor: the area of the graph's BG it highlights, in tiles, its rectangle in pixels, and where the
// arrows on either side of it go, from the rectangle's corner
// Black 2's; twinned code confirms the offsets of x, y, leftX, leftY, rightX and rightY in Black
typedef struct {
    u8 tileX;
    u8 tileY;
    u8 tileWidth;
    u8 tileHeight;
    int x;
    int y;
    int width;
    int height;
    int leftX;
    int leftY;
    int rightX;
    int rightY;
} CursorEntry;

static void func_ov217_021bd718(ResearchGraph *wk);
static void func_ov217_021bd868(ResearchGraph *wk);
static void func_ov217_021bd89c(ResearchGraph *wk);
static void func_ov217_021bdc18(ResearchGraph *wk);
static void func_ov217_021bdf58(ResearchGraph *wk);
static void func_ov217_021be040(ResearchGraph *wk);
static void func_ov217_021be094(ResearchGraph *wk);
static void func_ov217_021be0d0(ResearchGraph *wk);
static void func_ov217_021be108(ResearchGraph *wk);
static void func_ov217_021be21c(ResearchGraph *wk);
static void func_ov217_021be33c(ResearchGraph *wk);
static void func_ov217_021be36c(ResearchGraph *wk);
static void func_ov217_021be384(ResearchGraph *wk);
static void func_ov217_021be448(ResearchGraph *wk);
static void func_ov217_021be454(ResearchGraph *wk, BOOL touch);
static void func_ov217_021be460(ResearchGraph *wk, u32 next);
static u32 func_ov217_021be468(ResearchGraph *wk);
static u32 func_ov217_021be46c(ResearchGraph *wk);
static void func_ov217_021be490(ResearchGraph *wk);
static void func_ov217_021be498(ResearchGraph *wk, u32 seq);
static void func_ov217_021be4a4(ResearchGraph *wk);
static void func_ov217_021be4cc(ResearchGraph *wk, u32 seq);
static u32 func_ov217_021be4dc(ResearchGraph *wk);
static void func_ov217_021be4e0(ResearchGraph *wk);
static void func_ov217_021be4e8(ResearchGraph *wk);
static void func_ov217_021be580(ResearchGraph *wk);
static void func_ov217_021be614(ResearchGraph *wk, int pos);
static void func_ov217_021be630(ResearchGraph *wk, int pos);
static void func_ov217_021be678(ResearchGraph *wk);
static void func_ov217_021be704(ResearchGraph *wk);
static void func_ov217_021be790(ResearchGraph *wk);
static void func_ov217_021be7b4(ResearchGraph *wk);
static void func_ov217_021be7d8(ResearchGraph *wk);
static void func_ov217_021be7f0(ResearchGraph *wk);
static void func_ov217_021be808(ResearchGraph *wk);
static void func_ov217_021be8fc(void);
static void func_ov217_021be90c(void);
static void func_ov217_021be91c(ResearchGraph *wk, int dir);
static void func_ov217_021be930(ResearchGraph *wk, int pos);
static void func_ov217_021be934(ResearchGraph *wk);
static void func_ov217_021be9b8(ResearchGraph *wk);
static void func_ov217_021bea3c(ResearchGraph *wk, int dir);
static void func_ov217_021bea54(ResearchGraph *wk);
static void func_ov217_021bea8c(ResearchGraph *wk, int dir);
static void func_ov217_021beaec(ResearchGraph *wk);
static void func_ov217_021beaf8(ResearchGraph *wk);
static void func_ov217_021beb40(ResearchGraph *wk);
static void func_ov217_021beb6c(ResearchGraph *wk);
static void func_ov217_021beb88(ResearchGraph *wk);
static void func_ov217_021beba4(ResearchGraph *wk);
static void func_ov217_021bebc0(ResearchGraph *wk);
static void func_ov217_021bebe4(ResearchGraph *wk);
static void func_ov217_021bec00(ResearchGraph *wk);
static void func_ov217_021bec0c(ResearchGraph *wk);
static void func_ov217_021bec34(CircleGraphData *item);
static void func_ov217_021bec64(ResearchGraph *wk);
static void func_ov217_021bec84(ResearchGraph *wk, u32 mode);
static void func_ov217_021bed10(ResearchGraph *wk, u32 mode);
static void func_ov217_021bed9c(ResearchGraph *wk);
static void func_ov217_021bedc4(ResearchGraph *wk);
static void func_ov217_021bedec(ResearchGraph *wk);
static void func_ov217_021beea4(ResearchGraph *wk);
static void func_ov217_021bef30(ResearchGraph *wk);
static void func_ov217_021bef98(ResearchGraph *wk);
static void func_ov217_021befc0(ResearchGraph *wk, u8 index);
static void func_ov217_021befd4(ResearchGraph *wk);
static void func_ov217_021beff8(ResearchGraph *wk);
static void func_ov217_021bf08c(ResearchGraph *wk);
static void func_ov217_021bf100(ResearchGraph *wk);
static void func_ov217_021bf11c(ResearchGraph *wk);
static void func_ov217_021bf13c(ResearchGraph *wk);
static void func_ov217_021bf15c(ResearchGraph *wk);
static void func_ov217_021bf264(ResearchGraph *wk);
static void func_ov217_021bf2fc(ResearchGraph *wk);
static void func_ov217_021bf38c(ResearchGraph *wk);
static void func_ov217_021bf3b4(ResearchGraph *wk);
static void func_ov217_021bf3d8(ResearchGraph *wk);
static void func_ov217_021bf47c(ResearchGraph *wk);
static void func_ov217_021bf4ac(ResearchGraph *wk);
static void func_ov217_021bf55c(ResearchGraph *wk);
static void func_ov217_021bf568(ResearchGraph *wk);
static void func_ov217_021bf574(ResearchGraph *wk);
static void func_ov217_021bf580(ResearchGraph *wk, u32 index, BOOL visible);
static void func_ov217_021bf594(ResearchGraph *wk, u32 index);
static void func_ov217_021bf5bc(ResearchGraph *wk, u32 index);
static BOOL func_ov217_021bf5d0(ResearchGraph *wk, u32 index);
static void func_ov217_021bf5e4(ResearchGraph *wk);
static void func_ov217_021bf600(ResearchGraph *wk);
static void func_ov217_021bf60c(ResearchGraph *wk);
static void func_ov217_021bf658(ResearchGraph *wk);
static BOOL func_ov217_021bf6a4(ResearchGraph *wk);
static void func_ov217_021bf6bc(TCB *tcb, void *data);
static void func_ov217_021bf6d4(ResearchGraph *wk);
static void func_ov217_021bf704(ResearchGraph *wk);
static void func_ov217_021bf710(ResearchGraph *wk);
static void func_ov217_021bf728(ResearchGraph *wk);
static GameSystem *func_ov217_021bf734(ResearchGraph *wk);
static GameData *func_ov217_021bf740(ResearchGraph *wk);
static void func_ov217_021bf74c(ResearchGraph *wk, HeapID heapId);
static ResearchCommon *func_ov217_021bf750(ResearchGraph *wk);
static void func_ov217_021bf754(ResearchGraph *wk, ResearchCommon *common);
static BOOL func_ov217_021bf758(ResearchGraph *wk);
static void func_ov217_021bf764(ResearchGraph *wk, u32 mode);
static CircleGraph *func_ov217_021bf76c(ResearchGraph *wk);
static CircleGraph *func_ov217_021bf77c(ResearchGraph *wk);
static BOOL func_ov217_021bf78c(ResearchGraph *wk);
static u8 func_ov217_021bf7cc(ResearchGraph *wk);
static u8 func_ov217_021bf7e0(ResearchGraph *wk);
static u16 func_ov217_021bf7f4(ResearchGraph *wk);
static u32 func_ov217_021bf810(ResearchGraph *wk);
static u32 func_ov217_021bf834(ResearchGraph *wk);
static u32 func_ov217_021bf844(ResearchGraph *wk);
static u32 func_ov217_021bf854(ResearchGraph *wk);
static u32 func_ov217_021bf878(ResearchGraph *wk);
static u32 func_ov217_021bf894(ResearchGraph *wk);
u8 func_ov217_021bf8b0(ResearchGraph *wk);
static u8 func_ov217_021bf910(ResearchGraph *wk);
static u8 func_ov217_021bf968(ResearchGraph *wk);
static u32 func_ov217_021bf9cc(ResearchGraph *wk);
static u32 func_ov217_021bf9f0(ResearchGraph *wk, u32 index);
static ClActUnit *func_ov217_021bf9fc(ResearchGraph *wk, u32 index);
static ClActor *func_ov217_021bfa08(ResearchGraph *wk, u32 index);
static ResearchGraph *func_ov217_021bfa14(HeapID heapId);
static void func_ov217_021bfa30(ResearchGraph *wk);
static void func_ov217_021bfb04(ResearchGraph *wk);
static void func_ov217_021bfb0c(ResearchGraph *wk);
static void func_ov217_021bfb14(ResearchGraph *wk);
static void func_ov217_021bfb24(ResearchGraph *wk);
static void func_ov217_021bfb30(ResearchGraph *wk);
static void func_ov217_021bfb38(ResearchGraph *wk);
static void func_ov217_021bfb54(ResearchGraph *wk);
static void func_ov217_021bfb60(ResearchGraph *wk);
static void func_ov217_021bfb74(ResearchGraph *wk);
static void func_ov217_021bfba0(ResearchGraph *wk);
static void func_ov217_021bfbb8(ResearchGraph *wk);
static void func_ov217_021bfbc0(ResearchGraph *wk);
static void func_ov217_021bfbd0(ResearchGraph *wk);
static void func_ov217_021bfbdc(ResearchGraph *wk);
static void func_ov217_021bfbfc(ResearchGraph *wk);
static void func_ov217_021bfc28(ResearchGraph *wk);
static void func_ov217_021bfc50(ResearchGraph *wk);
void func_ov217_021bfc60(ResearchGraph *wk);
static void func_ov217_021bfe40(ResearchGraph *wk);
static void func_ov217_021bfe80(ResearchGraph *wk);
static void func_ov217_021bfe8c(ResearchGraph *wk);
static void func_ov217_021bfed4(ResearchGraph *wk);
static void func_ov217_021bfee4(ResearchGraph *wk);
static void func_ov217_021bff00(ResearchGraph *wk);
static void func_ov217_021bff5c(ResearchGraph *wk);
static void func_ov217_021bff78(void);
static void func_ov217_021bffc8(ResearchGraph *wk);
static void func_ov217_021c0084(ResearchGraph *wk);
static void func_ov217_021c00ac(ResearchGraph *wk);
static void func_ov217_021c00f8(ResearchGraph *wk);
static void func_ov217_021c00fc(ResearchGraph *wk);
static void func_ov217_021c0114(ResearchGraph *wk);
static void func_ov217_021c0124(ResearchGraph *wk);
static void func_ov217_021c0170(ResearchGraph *wk);
static void func_ov217_021c0174(ResearchGraph *wk);
static void func_ov217_021c018c(ResearchGraph *wk);
static void func_ov217_021c019c(ResearchGraph *wk);
static void func_ov217_021c01b8(ResearchGraph *wk);
static void func_ov217_021c0230(ResearchGraph *wk);
static void func_ov217_021c0250(ResearchGraph *wk);
static void func_ov217_021c02a8(ResearchGraph *wk);
static void func_ov217_021c02c8(ResearchGraph *wk);
static void func_ov217_021c0358(ResearchGraph *wk);
static void func_ov217_021c0384(ResearchGraph *wk);
static void func_ov217_021c03a0(ResearchGraph *wk);
static void func_ov217_021c03d0(ResearchGraph *wk);
static void func_ov217_021c03ec(ResearchGraph *wk);
static void func_ov217_021c0408(ResearchGraph *wk);
static void func_ov217_021c0494(ResearchGraph *wk);
static void func_ov217_021c04b0(ResearchGraph *wk);
static void func_ov217_021c04cc(ResearchGraph *wk);
static void func_ov217_021c04fc(ResearchGraph *wk);
static void func_ov217_021c05b0(ResearchGraph *wk);
static void func_ov217_021c06ac(ResearchGraph *wk);
static void func_ov217_021c06c8(ResearchGraph *wk);
static void func_ov217_021c06e0(ResearchGraph *wk);
static void func_ov217_021c06f0(ResearchGraph *wk);
static void func_ov217_021c0794(ResearchGraph *wk);
static void func_ov217_021c07b0(ResearchGraph *wk);
static void func_ov217_021c07bc(ResearchGraph *wk);
static void func_ov217_021c0800(ResearchGraph *wk);
static void func_ov217_021c0820(ResearchGraph *wk);
static void func_ov217_021c083c(ResearchGraph *wk);
static void func_ov217_021c085c(ResearchGraph *wk);
static void func_ov217_021c0878(ResearchGraph *wk);
static void func_ov217_021c08a8(ResearchGraph *wk);
static void func_ov217_021c08c4(ResearchGraph *wk);
static void func_ov217_021c08d4(ResearchGraph *wk);
static void func_ov217_021c08f0(ResearchGraph *wk);
static u8 func_ov217_021c0900(int value);
void func_ov217_021c0918(ResearchGraph *wk);
static BOOL func_ov217_021c093c(ResearchGraph *wk);
static void func_ov217_021c0968(ResearchGraph *wk);

// Not referenced
const u32 ResearchGraph_Unused1 = 31;

// Not referenced
const u32 ResearchGraph_Unused3 = 3;

static const u8 data_ov217_021c3618[UNIT_COUNT] = { 0, 1, 0, 0 };

static const u16 data_ov217_021c3620[UNIT_COUNT] = { 1, 4, 34, 68 };

// Not referenced; see research_graph.h. Black 2's { 3, 0 } is Black's ResearchGraph_Unused3, before the graph's
// shared data, and this 0 inside it
const u32 ResearchGraph_Unused2[1] = { 0 };

// The names of the surveys
static const u8 data_ov217_021c3628[SURVEY_COUNT] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

static const u8 data_ov217_021c3632[SURVEY_COUNT] = { 29, 26, 28, 11, 14, 24, 22, 17, 23, 27 };

// The three questions of each survey
static const u8 data_ov217_021c363c[SURVEY_COUNT] = { 0, 1, 2, 3, 4, 12, 20, 10, 9, 15 };

static const u8 data_ov217_021c3646[SURVEY_COUNT] = { 8, 25, 7, 5, 6, 13, 21, 16, 19, 18 };

static const u32 data_ov217_021c3650[MSG_DATA_COUNT] = { 0xa9, 0xa4, 0 };

static const GXRgb data_ov217_021c365c[8] = { 0 };

static const BGSysLCDConfig data_ov217_021c366c = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };

static const u8 data_ov217_021c3d44[30][RESEARCH_ANSWER_MAX] = {
#include "asset/research_graph_data_ov217_021c3d44.inc"
};

// The titles of the questions, and their texts, by their IDs
static const u8 data_ov217_021c367c[30] = {
#include "asset/research_graph_data_ov217_021c367c.inc"
};

static const u8 data_ov217_021c369a[30] = {
#include "asset/research_graph_data_ov217_021c369a.inc"
};

static const u8 data_ov217_021c36b8[30] = {
#include "asset/research_graph_data_ov217_021c36b8.inc"
};

static const BGSetup data_ov217_021c36d8 = { 0,
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

static const BGSetup data_ov217_021c36f8 = { 0,
                                           0,
                                           0x800,
                                           0,
                                           BGRES_256x256,
                                           GX_BG_COLORMODE_16,
                                           GX_BG_SCRBASE_0x0800,
                                           GX_BG_CHARBASE_0x04000,
                                           0x8000,
                                           GX_BG_EXTPLTT_01,
                                           1,
                                           GX_BG_AREAOVER_XLU,
                                           FALSE };

static const BGSetup data_ov217_021c3718 = { 0,
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

static const BGSetup data_ov217_021c3738 = { 0,
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

// The arrows' rectangles are set from the cursor
static const ResearchRect data_ov217_021c3758[TOUCH_RECT_COUNT] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 16, 240, 0, 32 },
    { 104, 255, 32, 88 },
    { 136, 255, 88, 128 },
    { 104, 255, 128, 168 },
    { 8, 112, 48, 152 },
    { 0, 160, 160, 192 },
#ifdef BUGFIX
    { 0, 0, TOUCH_RECT_END, 0 },
#else
    // BUG: The end of the table is written as a TouchRect, with TOUCH_RECT_END in the first field, which is left here,
    // so the copy in the work has no end and the touch panel goes on reading the work after it
    { TOUCH_RECT_END, 0, 0, 0 },
#endif
};

static const BmpOamEntry data_ov217_021c377c[BMP_COUNT] = {
#include "asset/research_graph_data_ov217_021c377c.inc"
};

static const BitmapEntry data_ov217_021c37a4[BMP_COUNT] = {
#include "asset/research_graph_data_ov217_021c37a4.inc"
};

static const ResearchActorSetup data_ov217_021c37fc[ACTOR_COUNT] = {
#include "asset/research_graph_data_ov217_021c37fc.inc"
};

// The colors of the answers' slices, by the answers' IDs
static const u8 data_ov217_021c386c[148] = {
#include "asset/research_graph_data_ov217_021c386c.inc"
};

static const u8 data_ov217_021c3900[148] = {
#include "asset/research_graph_data_ov217_021c3900.inc"
};

static const u8 data_ov217_021c3994[148] = {
#include "asset/research_graph_data_ov217_021c3994.inc"
};

static const ResearchPaletteAnimeSetup data_ov217_021c3a28[ANIME_COUNT] = {
    { (u16 *)(HW_BG_PLTT + 0x146), (const u16 *)(HW_BG_PLTT + 0x146), 3, 0, 0xffff },
    { (u16 *)(HW_BG_PLTT + 0x140), (const u16 *)(HW_BG_PLTT + 0x140), 16, 4, 0xffff },
    { (u16 *)(HW_OBJ_PLTT + 0x80), (const u16 *)(HW_OBJ_PLTT + 0x80), 16, 3, 0xffff },
    { (u16 *)(HW_OBJ_PLTT + 0x8c), (const u16 *)(HW_OBJ_PLTT + 0x8c), 1, 0, 0xffff },
    { (u16 *)(HW_OBJ_PLTT + 0x8c), (const u16 *)(HW_OBJ_PLTT + 0x8c), 1, 4, 0xffff },
    { (u16 *)(HW_OBJ_PLTT + 0xa0), (const u16 *)(HW_OBJ_PLTT + 0xa0), 16, 7, 0 },
    { (u16 *)(HW_OBJ_PLTT + 0xa0), (const u16 *)(HW_OBJ_PLTT + 0xa0), 16, 8, 0 },
    { (u16 *)(HW_BG_PLTT + 0x160), (const u16 *)(HW_BG_PLTT + 0x160), 16, 1, 0xffff },
};

static const CursorEntry data_ov217_021c3ac8[CURSOR_COUNT] = {
#include "asset/research_graph_data_ov217_021c3ac8.inc"
};

static const BGFontEntry data_ov217_021c3b7c[BG_FONT_COUNT] = {
#include "asset/research_graph_data_ov217_021c3b7c.inc"
};

// pokebw2's ResearchGraph_Create
ResearchGraph *func_ov217_021bd5f4(ResearchCommon *common) {
    HeapID heapId = func_ov217_021c09ac(common);
    ResearchGraph *wk = func_ov217_021bfa14(heapId);

    func_ov217_021bfa30(wk);
    func_ov217_021bf74c(wk, heapId);
    func_ov217_021bf754(wk, common);
    return wk;
}

// pokebw2's ResearchGraph_Delete
void func_ov217_021bd61c(ResearchGraph *wk) {
    func_ov217_021bfb24(wk);
    func_ov217_021bfb04(wk);
}

// pokebw2's ResearchGraph_Main
void func_ov217_021bd62c(ResearchGraph *wk) {
    switch (wk->seq) {
    case SEQ_SETUP:
        func_ov217_021bd718(wk);
        break;
    case SEQ_RESTART_COMM:
        func_ov217_021bd868(wk);
        break;
    case SEQ_TOUCH:
        func_ov217_021bd89c(wk);
        break;
    case SEQ_CURSOR:
        func_ov217_021bdc18(wk);
        break;
    case SEQ_OPEN:
        func_ov217_021bdf58(wk);
        break;
    case SEQ_SHOW_PERCENTAGES:
        func_ov217_021be040(wk);
        break;
    case SEQ_FLASH_IN:
        func_ov217_021be094(wk);
        break;
    case SEQ_FLASH_OUT:
        func_ov217_021be0d0(wk);
        break;
    case SEQ_UPDATE_TOUCH:
        func_ov217_021be108(wk);
        break;
    case SEQ_UPDATE_CURSOR:
        func_ov217_021be21c(wk);
        break;
    case SEQ_FADE_OUT:
        func_ov217_021be33c(wk);
        break;
    case SEQ_WAIT:
        func_ov217_021be36c(wk);
        break;
    case SEQ_TEARDOWN:
        func_ov217_021be384(wk);
        break;
    case SEQ_END:
        return;
    }

    func_ov217_021bf6d4(wk);
    func_ov217_021bf710(wk);
    if (wk->active) {
        func_0204a600();
        func_ov217_021bed9c(wk);
        func_ov217_021c20c4(wk->arrow);
        func_ov217_021bf600(wk);
        func_ov217_021bf5e4(wk);
        func_ov217_021bedc4(wk);
        func_ov217_021bedec(wk);
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
    func_ov217_021be490(wk);
    func_ov217_021be4a4(wk);
}

// pokebw2's ResearchGraph_IsEnd
BOOL func_ov217_021bd708(ResearchGraph *wk) {
    return wk->isEnd;
}

// pokebw2's ResearchGraph_GetNext
u32 func_ov217_021bd710(ResearchGraph *wk) {
    return wk->next;
}

// pokebw2's ResearchGraph_SeqSetup
static void func_ov217_021bd718(ResearchGraph *wk) {
    u32 startSeq;

    func_ov217_021bfb14(wk);
    func_ov217_021bfb38(wk);
    func_ov217_021bfb74(wk);
    func_ov217_021bfbc0(wk);
    func_ov217_021bfe40(wk);
    func_ov217_021beff8(wk);
    func_ov217_021bff78();
    func_ov217_021bffc8(wk);
    func_ov217_021c00ac(wk);
    func_ov217_021c00fc(wk);
    func_ov217_021c0124(wk);
    func_ov217_021c0174(wk);
    func_ov217_021c01b8(wk);
    func_ov217_021c0250(wk);
    func_ov217_021c02c8(wk);
    func_ov217_021c03a0(wk);
    func_ov217_021c0408(wk);
    func_ov217_021c04cc(wk);
    func_ov217_021c04fc(wk);
    func_ov217_021c05b0(wk);
    func_ov217_021c06c8(wk);
    func_ov217_021c06f0(wk);
    func_ov217_021bfc60(wk);
    func_ov217_021bfbfc(wk);
    func_ov217_021bfe8c(wk);
    func_ov217_021bff00(wk);
    func_ov217_021c07bc(wk);
    func_ov217_021c083c(wk);
    func_ov217_021c0878(wk);
    func_ov217_021c08d4(wk);
    func_ov217_021c08c4(wk);
    func_ov217_021bf100(wk);
    func_ov217_021bf11c(wk);
    func_ov217_021bf13c(wk);
    func_ov217_021bf15c(wk);
    func_ov217_021bf264(wk);
    func_ov217_021bf2fc(wk);
    func_ov217_021bf38c(wk);
    func_ov217_021bf3b4(wk);
    func_ov217_021bf3d8(wk);
    func_ov217_021bf08c(wk);
    func_ov217_021bf47c(wk);
    func_ov217_021bf55c(wk);
    func_ov217_021bf594(wk, ANIME_CURSOR);
    func_ov217_021bf594(wk, ANIME_FRAME);
    func_ov217_021beaf8(wk);
    wk->active = TRUE;
    func_ov217_021be8fc();

    startSeq = func_ov217_021be46c(wk);
    func_ov217_021be498(wk, SEQ_RESTART_COMM);
    func_ov217_021be498(wk, startSeq);
    if (startSeq == SEQ_CURSOR) {
        func_ov217_021be934(wk);
    }
    func_ov217_021be448(wk);
}

// pokebw2's ResearchGraph_SeqRestartComm
static void func_ov217_021bd868(ResearchGraph *wk) {
    switch (func_ov217_021be4dc(wk)) {
    case 0:
        func_ov217_021c0918(wk);
        func_ov217_021be4e0(wk);
        break;
    case 1:
        if (func_ov217_021c093c(wk)) {
            func_ov217_021be448(wk);
        }
        break;
    }
}

// pokebw2's ResearchGraph_SeqTouch
static void func_ov217_021bd89c(ResearchGraph *wk) {
    u32 keys;
    s32 rect;
    s32 commonButton;

    GCTX_HIDGetHeldKeys();
    keys = GCTX_HIDGetPressedKeys();
    rect = func_02035c08(wk->touchRects);
    commonButton = func_02035c08(func_ov217_021c09b8(wk->common));

    if (wk->resultsShown && wk->newResults && wk->updateReady) {
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_UPDATE_TOUCH);
        func_ov217_021be498(wk, SEQ_FLASH_IN);
        func_ov217_021be498(wk, SEQ_FLASH_OUT);
        func_ov217_021be498(wk, SEQ_SHOW_PERCENTAGES);
        func_ov217_021be498(wk, SEQ_TOUCH);
        return;
    }

    if (func_ov217_021bf758(wk) || commonButton == RESEARCH_COMMON_BUTTON_RETURN) {
        func_ov217_021be454(wk, TRUE);
        func_ov217_021bec00(wk);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_WAIT);
        func_ov217_021be498(wk, SEQ_FADE_OUT);
        func_ov217_021be498(wk, SEQ_TEARDOWN);
    } else if ((keys & PAD_KEY_UP) || (keys & PAD_KEY_DOWN) || (keys & PAD_KEY_LEFT) || (keys & PAD_KEY_RIGHT) ||
               (keys & PAD_BUTTON_A)) {
        func_ov217_021be934(wk);
        func_ov217_021beaf8(wk);
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_CURSOR);
    } else if (rect == TOUCH_GRAPH && !wk->resultsShown && func_ov217_021bf810(wk)) {
        wk->touchOpened = TRUE;
        func_ov217_021beb40(wk);
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_OPEN);
        func_ov217_021be498(wk, SEQ_FLASH_IN);
        func_ov217_021be498(wk, SEQ_FLASH_OUT);
        func_ov217_021be498(wk, SEQ_SHOW_PERCENTAGES);
        func_ov217_021be498(wk, SEQ_TOUCH);
    } else if (rect == TOUCH_BUTTON) {
        if (!wk->resultsShown && func_ov217_021bf810(wk)) {
            wk->touchOpened = TRUE;
            func_ov217_021beb40(wk);
            func_ov217_021be448(wk);
            func_ov217_021be498(wk, SEQ_OPEN);
            func_ov217_021be498(wk, SEQ_FLASH_IN);
            func_ov217_021be498(wk, SEQ_FLASH_OUT);
            func_ov217_021be498(wk, SEQ_SHOW_PERCENTAGES);
            func_ov217_021be498(wk, SEQ_TOUCH);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
    } else if (rect == TOUCH_QUESTION) {
        if (!wk->resultsShown && func_ov217_021bf810(wk)) {
            wk->touchOpened = TRUE;
            func_ov217_021beb40(wk);
            func_ov217_021be448(wk);
            func_ov217_021be498(wk, SEQ_OPEN);
            func_ov217_021be498(wk, SEQ_FLASH_IN);
            func_ov217_021be498(wk, SEQ_FLASH_OUT);
            func_ov217_021be498(wk, SEQ_SHOW_PERCENTAGES);
            func_ov217_021be498(wk, SEQ_TOUCH);
        } else {
            func_ov217_021be614(wk, CURSOR_QUESTION);
        }
    } else if (wk->resultsShown && func_ov217_021bf810(wk) && rect == TOUCH_ANSWER) {
        func_ov217_021be614(wk, CURSOR_ANSWER);
        func_ov217_021be934(wk);
        func_ov217_021beaf8(wk);
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_CURSOR);
    } else if (wk->resultsShown && func_ov217_021bf810(wk) && rect == TOUCH_PLAYER_ANSWER) {
        func_ov217_021be614(wk, CURSOR_PLAYER_ANSWER);
        func_ov217_021be934(wk);
        func_ov217_021beaf8(wk);
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_CURSOR);
    } else if (rect == TOUCH_MODE) {
        func_ov217_021be614(wk, CURSOR_MODE);
        func_ov217_021be934(wk);
        func_ov217_021beaf8(wk);
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_CURSOR);
    } else if (rect == TOUCH_LEFT) {
        func_ov217_021be934(wk);
        func_ov217_021beaf8(wk);
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_CURSOR);
        switch (wk->cursor) {
        case CURSOR_QUESTION:
            func_ov217_021be704(wk);
            break;
        case CURSOR_ANSWER:
            func_ov217_021be7b4(wk);
            break;
        case CURSOR_MODE:
            func_ov217_021be808(wk);
            break;
        }
    } else if (rect == TOUCH_RIGHT) {
        func_ov217_021be934(wk);
        func_ov217_021beaf8(wk);
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_CURSOR);
        switch (wk->cursor) {
        case CURSOR_QUESTION:
            func_ov217_021be678(wk);
            break;
        case CURSOR_ANSWER:
            func_ov217_021be790(wk);
            break;
        case CURSOR_MODE:
            func_ov217_021be808(wk);
            break;
        }
    } else if (keys & PAD_BUTTON_B) {
        func_ov217_021be454(wk, FALSE);
        func_ov217_021bec00(wk);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_WAIT);
        func_ov217_021be498(wk, SEQ_FADE_OUT);
        func_ov217_021be498(wk, SEQ_TEARDOWN);
    }
}

// pokebw2's ResearchGraph_SeqCursor
static void func_ov217_021bdc18(ResearchGraph *wk) {
    u32 keys;
    s32 rect;
    s32 commonButton;

    GCTX_HIDGetHeldKeys();
    keys = GCTX_HIDGetPressedKeys();
    rect = func_02035c08(wk->touchRects);
    commonButton = func_02035c08(func_ov217_021c09b8(wk->common));

    if (wk->resultsShown && wk->newResults && wk->updateReady) {
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_UPDATE_CURSOR);
        func_ov217_021be498(wk, SEQ_FLASH_IN);
        func_ov217_021be498(wk, SEQ_FLASH_OUT);
        func_ov217_021be498(wk, SEQ_SHOW_PERCENTAGES);
        func_ov217_021be498(wk, SEQ_CURSOR);
        return;
    }

    if (func_ov217_021bf758(wk) || commonButton == RESEARCH_COMMON_BUTTON_RETURN) {
        func_ov217_021be454(wk, TRUE);
        func_ov217_021bec00(wk);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_WAIT);
        func_ov217_021be498(wk, SEQ_FADE_OUT);
        func_ov217_021be498(wk, SEQ_TEARDOWN);
    } else if (keys & PAD_KEY_UP) {
        func_ov217_021be4e8(wk);
    } else if (keys & PAD_KEY_DOWN) {
        func_ov217_021be580(wk);
    } else if (rect == TOUCH_GRAPH && !wk->resultsShown && func_ov217_021bf810(wk)) {
        wk->touchOpened = TRUE;
        func_ov217_021beb40(wk);
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_OPEN);
        func_ov217_021be498(wk, SEQ_FLASH_IN);
        func_ov217_021be498(wk, SEQ_FLASH_OUT);
        func_ov217_021be498(wk, SEQ_SHOW_PERCENTAGES);
        func_ov217_021be498(wk, SEQ_TOUCH);
    } else if (rect == TOUCH_BUTTON) {
        if (!wk->resultsShown && func_ov217_021bf810(wk)) {
            wk->touchOpened = TRUE;
            func_ov217_021beb40(wk);
            func_ov217_021be448(wk);
            func_ov217_021be498(wk, SEQ_OPEN);
            func_ov217_021be498(wk, SEQ_FLASH_IN);
            func_ov217_021be498(wk, SEQ_FLASH_OUT);
            func_ov217_021be498(wk, SEQ_SHOW_PERCENTAGES);
            func_ov217_021be498(wk, SEQ_TOUCH);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
    } else if (rect == TOUCH_QUESTION) {
        if (!wk->resultsShown && func_ov217_021bf810(wk)) {
            wk->touchOpened = TRUE;
            func_ov217_021beb40(wk);
            func_ov217_021be448(wk);
            func_ov217_021be498(wk, SEQ_OPEN);
            func_ov217_021be498(wk, SEQ_FLASH_IN);
            func_ov217_021be498(wk, SEQ_FLASH_OUT);
            func_ov217_021be498(wk, SEQ_SHOW_PERCENTAGES);
            func_ov217_021be498(wk, SEQ_TOUCH);
        } else {
            func_ov217_021be614(wk, CURSOR_QUESTION);
        }
    } else if (wk->resultsShown && func_ov217_021bf810(wk) && rect == TOUCH_ANSWER) {
        func_ov217_021be614(wk, CURSOR_ANSWER);
    } else if (wk->resultsShown && func_ov217_021bf810(wk) && rect == TOUCH_PLAYER_ANSWER) {
        func_ov217_021be614(wk, CURSOR_PLAYER_ANSWER);
    } else if (rect == TOUCH_MODE) {
        func_ov217_021be614(wk, CURSOR_MODE);
    } else if ((keys & PAD_KEY_LEFT) || rect == TOUCH_LEFT) {
        switch (wk->cursor) {
        case CURSOR_QUESTION:
            func_ov217_021be704(wk);
            break;
        case CURSOR_ANSWER:
            func_ov217_021be7b4(wk);
            break;
        case CURSOR_MODE:
            func_ov217_021be808(wk);
            break;
        }
    } else if ((keys & PAD_KEY_RIGHT) || rect == TOUCH_RIGHT) {
        switch (wk->cursor) {
        case CURSOR_QUESTION:
            func_ov217_021be678(wk);
            break;
        case CURSOR_ANSWER:
            func_ov217_021be790(wk);
            break;
        case CURSOR_MODE:
            func_ov217_021be808(wk);
            break;
        }
    } else if ((keys & PAD_BUTTON_A) && !wk->resultsShown && func_ov217_021bf810(wk) &&
               (wk->cursor == CURSOR_QUESTION || wk->cursor == CURSOR_BUTTON)) {
        wk->touchOpened = FALSE;
        func_ov217_021beb40(wk);
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_OPEN);
        func_ov217_021be498(wk, SEQ_FLASH_IN);
        func_ov217_021be498(wk, SEQ_FLASH_OUT);
        func_ov217_021be498(wk, SEQ_SHOW_PERCENTAGES);
        func_ov217_021be498(wk, SEQ_CURSOR);
    } else if (keys & PAD_BUTTON_B) {
        func_ov217_021be454(wk, FALSE);
        func_ov217_021bec00(wk);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021be448(wk);
        func_ov217_021be498(wk, SEQ_WAIT);
        func_ov217_021be498(wk, SEQ_FADE_OUT);
        func_ov217_021be498(wk, SEQ_TEARDOWN);
    }
}

// pokebw2's ResearchGraph_SeqOpen
static void func_ov217_021bdf58(ResearchGraph *wk) {
    switch (func_ov217_021be4dc(wk)) {
    case 0:
        func_ov217_021bfc60(wk);
        func_ov217_021bf704(wk);
        func_ov217_021bf568(wk);
        func_ov217_021bec84(wk, MODE_TODAY);
        func_ov217_021bec84(wk, MODE_TOTAL);
        func_ov217_021c1184(func_ov217_021bf76c(wk), TRUE);
        func_ov217_021c1154(func_ov217_021bf76c(wk));
        func_ov217_021be4e0(wk);
        break;
    case 1:
        if (!GFL_SndPlayerIsActiveAny()) {
            if (wk->seqFrames >= OPEN_FRAMES) {
                func_ov217_021be4e0(wk);
            } else {
                GFL_SndSEPlay(SEQ_SE_SYS_81);
            }
        }
        break;
    case 2:
        wk->resultsShown = TRUE;
        func_ov217_021be930(wk, CURSOR_ANSWER);
        func_ov217_021bf3d8(wk);
        func_ov217_021beff8(wk);
        func_ov217_021be7f0(wk);
        func_ov217_021bf574(wk);
        func_ov217_021bf08c(wk);
        func_ov217_021bf15c(wk);
        func_ov217_021bf264(wk);
        func_ov217_021bf3b4(wk);
        func_ov217_021bf2fc(wk);
        func_ov217_021beea4(wk);
        func_ov217_021bf47c(wk);
        if (!wk->touchOpened) {
            func_ov217_021be934(wk);
        }
        GFL_SndSEPlay(SEQ_SE_SYS_82);
        func_ov217_021be448(wk);
        break;
    }
}

// pokebw2's ResearchGraph_SeqShowPercentages
static void func_ov217_021be040(ResearchGraph *wk) {
    switch (func_ov217_021be4dc(wk)) {
    case 0:
        func_ov217_021bef30(wk);
        func_ov217_021be4e0(wk);
        break;
    case 1:
        if (wk->seqFrames % PERCENTAGE_INTERVAL == 0) {
            func_ov217_021befc0(wk, wk->percentagesShown++);
        }
        if (wk->percentageCount <= wk->percentagesShown) {
            func_ov217_021be448(wk);
        }
        break;
    }
}

// pokebw2's ResearchGraph_SeqFlashIn
static void func_ov217_021be094(ResearchGraph *wk) {
    switch (func_ov217_021be4dc(wk)) {
    case 0:
        func_ov217_021bf5bc(wk, ANIME_BUTTON_DISABLE);
        func_ov217_021bf60c(wk);
        func_ov217_021be4e0(wk);
        break;
    case 1:
        if (func_ov217_021bf6a4(wk)) {
            func_ov217_021be448(wk);
        }
        break;
    }
}

// pokebw2's ResearchGraph_SeqFlashOut
static void func_ov217_021be0d0(ResearchGraph *wk) {
    switch (func_ov217_021be4dc(wk)) {
    case 0:
        func_ov217_021bf658(wk);
        func_ov217_021be4e0(wk);
        break;
    case 1:
        if (func_ov217_021bf6a4(wk)) {
            func_ov217_021beaf8(wk);
            func_ov217_021be448(wk);
        }
        break;
    }
}

// pokebw2's ResearchGraph_SeqUpdateTouch
static void func_ov217_021be108(ResearchGraph *wk) {
    switch (func_ov217_021be4dc(wk)) {
    case 0:
        wk->updating = TRUE;
        func_ov217_021bf704(wk);
        func_ov217_021bf728(wk);
        func_ov217_021bfc60(wk);
        func_ov217_021bf08c(wk);
        func_ov217_021bf15c(wk);
        func_ov217_021bf3b4(wk);
        func_ov217_021bf38c(wk);
        func_ov217_021beea4(wk);
        func_ov217_021bf3d8(wk);
        func_ov217_021bef98(wk);
        func_ov217_021bed10(wk, MODE_TODAY);
        func_ov217_021bed10(wk, MODE_TOTAL);
        func_ov217_021bec64(wk);
        func_ov217_021c1184(func_ov217_021bf77c(wk), TRUE);
        func_ov217_021c1178(func_ov217_021bf77c(wk));
        func_ov217_021be4e0(wk);
        break;
    case 1:
        if (!GFL_SndPlayerIsActiveAny()) {
            if (wk->seqFrames >= UPDATE_FRAMES) {
                func_ov217_021be4e0(wk);
            } else {
                GFL_SndSEPlay(SEQ_SE_SYS_81);
            }
        }
        break;
    case 2:
        wk->updating = FALSE;
        func_ov217_021bec0c(wk);
        func_ov217_021c1184(func_ov217_021bf77c(wk), FALSE);
        func_ov217_021bf08c(wk);
        func_ov217_021bf3b4(wk);
        func_ov217_021bf15c(wk);
        func_ov217_021bf264(wk);
        func_ov217_021bf2fc(wk);
        func_ov217_021beea4(wk);
        func_ov217_021bf3d8(wk);
        func_ov217_021bf4ac(wk);
        func_ov217_021bf47c(wk);
        GFL_SndSEPlay(SEQ_SE_SYS_82);
        func_ov217_021be448(wk);
        break;
    }
}

// pokebw2's ResearchGraph_SeqUpdateCursor
static void func_ov217_021be21c(ResearchGraph *wk) {
    switch (func_ov217_021be4dc(wk)) {
    case 0:
        wk->updating = TRUE;
        func_ov217_021bf704(wk);
        func_ov217_021bf728(wk);
        func_ov217_021bfc60(wk);
        func_ov217_021be9b8(wk);
        func_ov217_021bf08c(wk);
        func_ov217_021bf15c(wk);
        func_ov217_021bf3b4(wk);
        func_ov217_021bf38c(wk);
        func_ov217_021beea4(wk);
        func_ov217_021bf3d8(wk);
        func_ov217_021bef98(wk);
        func_ov217_021bed10(wk, MODE_TODAY);
        func_ov217_021bed10(wk, MODE_TOTAL);
        func_ov217_021bec64(wk);
        func_ov217_021c1184(func_ov217_021bf77c(wk), TRUE);
        func_ov217_021c1178(func_ov217_021bf77c(wk));
        func_ov217_021be4e0(wk);
        break;
    case 1:
        if (!GFL_SndPlayerIsActiveAny()) {
            if (wk->seqFrames >= UPDATE_FRAMES) {
                func_ov217_021be4e0(wk);
            } else {
                GFL_SndSEPlay(SEQ_SE_SYS_81);
            }
        }
        break;
    case 2:
        wk->updating = FALSE;
        func_ov217_021bec0c(wk);
        func_ov217_021c1184(func_ov217_021bf77c(wk), FALSE);
        func_ov217_021bf08c(wk);
        func_ov217_021be934(wk);
        func_ov217_021bf3b4(wk);
        func_ov217_021bf15c(wk);
        func_ov217_021bf264(wk);
        func_ov217_021bf2fc(wk);
        func_ov217_021beea4(wk);
        func_ov217_021bf3d8(wk);
        func_ov217_021bf4ac(wk);
        func_ov217_021bf47c(wk);
        GFL_SndSEPlay(SEQ_SE_SYS_82);
        func_ov217_021be448(wk);
        break;
    }
}

// pokebw2's ResearchGraph_SeqFadeOut
static void func_ov217_021be33c(ResearchGraph *wk) {
    switch (func_ov217_021be4dc(wk)) {
    case 0:
        func_ov217_021be90c();
        func_ov217_021be4e0(wk);
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            func_ov217_021be448(wk);
        }
        break;
    }
}

// pokebw2's ResearchGraph_SeqWait
static void func_ov217_021be36c(ResearchGraph *wk) {
    if (func_ov217_021be468(wk) < wk->seqFrames) {
        func_ov217_021be448(wk);
    }
}

// pokebw2's ResearchGraph_SeqTeardown
static void func_ov217_021be384(ResearchGraph *wk) {
    func_ov217_021c08a8(wk);
    func_ov217_021c085c(wk);
    func_ov217_021c09e0(wk->common);
    func_ov217_021c09ec(wk->common);
    func_ov217_021c08f0(wk);
    func_ov217_021bff5c(wk);
    func_ov217_021bfed4(wk);
    func_ov217_021bfc28(wk);
    func_ov217_021c0800(wk);
    func_ov217_021c06ac(wk);
    func_ov217_021c0794(wk);
    func_ov217_021c06e0(wk);
    func_ov217_021c0494(wk);
    func_ov217_021c03d0(wk);
    func_ov217_021c02a8(wk);
    func_ov217_021c0358(wk);
    func_ov217_021c0230(wk);
    func_ov217_021c018c(wk);
    func_ov217_021c0170(wk);
    func_ov217_021c0114(wk);
    func_ov217_021c00f8(wk);
    func_ov217_021c0084(wk);
    func_ov217_021bfbd0(wk);
    func_ov217_021bfba0(wk);
    func_ov217_021bfb54(wk);
    func_ov217_021be460(wk, RESEARCH_GRAPH_NEXT_TOP);
    func_ov217_021be498(wk, SEQ_END);
    func_ov217_021be448(wk);
    func_ov217_021c0968(wk);
    wk->active = FALSE;
    wk->isEnd = TRUE;
}

// pokebw2's ResearchGraph_EndSeq
static void func_ov217_021be448(ResearchGraph *wk) {
    wk->seqDone = TRUE;
}

// pokebw2's ResearchGraph_SetTouchMode
static void func_ov217_021be454(ResearchGraph *wk, BOOL touch) {
    func_ov217_021c09c4(wk->common, touch);
}

// pokebw2's ResearchGraph_SetNext
static void func_ov217_021be460(ResearchGraph *wk, u32 next) {
    wk->next = next;
}

// pokebw2's ResearchGraph_GetWait
static u32 func_ov217_021be468(ResearchGraph *wk) {
    return wk->waitFrames;
}

// The cursor is shown from the start when the screen before was left with it
// pokebw2's ResearchGraph_GetStartSeq
static u32 func_ov217_021be46c(ResearchGraph *wk) {
    ResearchCommon *common = wk->common;
    u32 prevSeq = func_ov217_021c09c8(common);
    BOOL touch = func_ov217_021c09cc(common);

    if (prevSeq != RESEARCH_SEQ_INIT && !touch) {
        return SEQ_CURSOR;
    }
    return SEQ_TOUCH;
}

// pokebw2's ResearchGraph_CountFrame
static void func_ov217_021be490(ResearchGraph *wk) {
    wk->seqFrames++;
}

// pokebw2's ResearchGraph_PushSeq
static void func_ov217_021be498(ResearchGraph *wk, u32 seq) {
    func_ov217_021c0e4c(wk->queue, seq);
}

// pokebw2's ResearchGraph_UpdateSeq
static void func_ov217_021be4a4(ResearchGraph *wk) {
    if (wk->seqDone && !func_ov217_021c0e64(wk->queue)) {
        func_ov217_021be4cc(wk, func_ov217_021c0e54(wk->queue));
    }
}

// pokebw2's ResearchGraph_SetSeq
static void func_ov217_021be4cc(ResearchGraph *wk, u32 seq) {
    wk->seq = seq;
    wk->seqState = 0;
    wk->seqFrames = 0;
    wk->seqDone = FALSE;
}

// pokebw2's ResearchGraph_GetSeqState
static u32 func_ov217_021be4dc(ResearchGraph *wk) {
    return wk->seqState;
}

// pokebw2's ResearchGraph_NextSeqState
static void func_ov217_021be4e0(ResearchGraph *wk) {
    wk->seqState++;
}

// Moves the cursor up, past the places that can't be chosen: the answer and the player's answer until the results are
// shown, and the button after they are
// pokebw2's ResearchGraph_CursorUp
static void func_ov217_021be4e8(ResearchGraph *wk) {
    BOOL moving = TRUE;

    func_ov217_021be9b8(wk);
    func_ov217_021be91c(wk, -1);
    while (moving) {
        if (wk->cursor == CURSOR_ANSWER || wk->cursor == CURSOR_PLAYER_ANSWER) {
            if (!wk->resultsShown || !func_ov217_021bf810(wk)) {
                func_ov217_021be91c(wk, -1);
            } else {
                moving = FALSE;
            }
        } else if (wk->cursor == CURSOR_BUTTON) {
            if (wk->resultsShown == TRUE || !func_ov217_021bf810(wk)) {
                func_ov217_021be91c(wk, -1);
            } else {
                moving = FALSE;
            }
        } else {
            moving = FALSE;
        }
    }
    func_ov217_021be934(wk);
    func_ov217_021bf3d8(wk);
    func_ov217_021bf4ac(wk);
    func_ov217_021bf594(wk, ANIME_MOVE);
    if (wk->cursor == CURSOR_BUTTON) {
        func_ov217_021beba4(wk);
    }
    GFL_SndSEPlay(SEQ_SE_SELECT1);
    func_ov217_021beff8(wk);
}

// pokebw2's ResearchGraph_CursorDown
static void func_ov217_021be580(ResearchGraph *wk) {
    BOOL moving = TRUE;

    func_ov217_021be9b8(wk);
    func_ov217_021be91c(wk, 1);
    while (moving) {
        if (wk->cursor == CURSOR_ANSWER || wk->cursor == CURSOR_PLAYER_ANSWER) {
            if (!wk->resultsShown || !func_ov217_021bf810(wk)) {
                func_ov217_021be91c(wk, 1);
            } else {
                moving = FALSE;
            }
        } else if (wk->cursor == CURSOR_BUTTON) {
            if (wk->resultsShown == TRUE || !func_ov217_021bf810(wk)) {
                func_ov217_021be91c(wk, 1);
            } else {
                moving = FALSE;
            }
        } else {
            moving = FALSE;
        }
    }
    func_ov217_021be934(wk);
    func_ov217_021bf3d8(wk);
    func_ov217_021bf4ac(wk);
    func_ov217_021bf594(wk, ANIME_MOVE);
    if (wk->cursor == CURSOR_BUTTON) {
        func_ov217_021beba4(wk);
    }
    GFL_SndSEPlay(SEQ_SE_SELECT1);
    func_ov217_021beff8(wk);
}

// pokebw2's ResearchGraph_SelectCursor
static void func_ov217_021be614(ResearchGraph *wk, int pos) {
    func_ov217_021be630(wk, pos);
    func_ov217_021bf594(wk, ANIME_MOVE);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
}

// pokebw2's ResearchGraph_MoveCursorTo
static void func_ov217_021be630(ResearchGraph *wk, int pos) {
    if ((pos == CURSOR_ANSWER || pos == CURSOR_PLAYER_ANSWER) &&
        (!wk->resultsShown || !func_ov217_021bf810(wk))) {
        return;
    }
    func_ov217_021be9b8(wk);
    func_ov217_021be930(wk, pos);
    func_ov217_021be934(wk);
    func_ov217_021bf3d8(wk);
    func_ov217_021bf4ac(wk);
    func_ov217_021beff8(wk);
}

// pokebw2's ResearchGraph_NextQuestion
static void func_ov217_021be678(ResearchGraph *wk) {
    if (wk->resultsShown && func_ov217_021bf810(wk)) {
        func_ov217_021c116c(func_ov217_021bf76c(wk));
    }
    func_ov217_021bfc60(wk);
    func_ov217_021bf704(wk);
    func_ov217_021bea3c(wk, 1);
    wk->resultsShown = FALSE;
    func_ov217_021bf08c(wk);
    func_ov217_021be934(wk);
    func_ov217_021bf11c(wk);
    func_ov217_021bf13c(wk);
    func_ov217_021be7d8(wk);
    func_ov217_021bf264(wk);
    func_ov217_021bf2fc(wk);
    func_ov217_021bf38c(wk);
    func_ov217_021bef98(wk);
    func_ov217_021bf47c(wk);
    func_ov217_021beaf8(wk);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
}

// pokebw2's ResearchGraph_PrevQuestion
static void func_ov217_021be704(ResearchGraph *wk) {
    if (wk->resultsShown && func_ov217_021bf810(wk)) {
        func_ov217_021c116c(func_ov217_021bf76c(wk));
    }
    func_ov217_021bfc60(wk);
    func_ov217_021bf704(wk);
    func_ov217_021bea3c(wk, -1);
    wk->resultsShown = FALSE;
    func_ov217_021bf08c(wk);
    func_ov217_021be934(wk);
    func_ov217_021bf11c(wk);
    func_ov217_021bf13c(wk);
    func_ov217_021be7d8(wk);
    func_ov217_021bf264(wk);
    func_ov217_021bf2fc(wk);
    func_ov217_021bf38c(wk);
    func_ov217_021bef98(wk);
    func_ov217_021bf47c(wk);
    func_ov217_021beaf8(wk);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
}

// pokebw2's ResearchGraph_NextAnswer
static void func_ov217_021be790(ResearchGraph *wk) {
    func_ov217_021bea8c(wk, 1);
    func_ov217_021bf15c(wk);
    func_ov217_021beea4(wk);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
}

// pokebw2's ResearchGraph_PrevAnswer
static void func_ov217_021be7b4(ResearchGraph *wk) {
    func_ov217_021bea8c(wk, -1);
    func_ov217_021bf15c(wk);
    func_ov217_021beea4(wk);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
}

// pokebw2's ResearchGraph_ResetAnswer
static void func_ov217_021be7d8(ResearchGraph *wk) {
    func_ov217_021beaec(wk);
    func_ov217_021bf15c(wk);
    func_ov217_021beea4(wk);
}

// pokebw2's ResearchGraph_SelectFirstAnswer
static void func_ov217_021be7f0(ResearchGraph *wk) {
    func_ov217_021bea54(wk);
    func_ov217_021bf15c(wk);
    func_ov217_021beea4(wk);
}

// Switches the graph between today's answers and all of them, shrinking the one shown and growing the other
// pokebw2's ResearchGraph_ToggleMode
static void func_ov217_021be808(ResearchGraph *wk) {
    BOOL shrunk = FALSE;
    u32 mode;

    if (!func_ov217_021bf78c(wk)) {
        return;
    }

    if (wk->resultsShown && func_ov217_021bf810(wk)) {
        func_ov217_021c116c(func_ov217_021bf76c(wk));
        shrunk = TRUE;
    }

    switch (wk->mode) {
    case MODE_TODAY:
        mode = MODE_TOTAL;
        break;
    case MODE_TOTAL:
        mode = MODE_TODAY;
        break;
    }
    func_ov217_021bf764(wk, mode);
    func_ov217_021bf08c(wk);
    func_ov217_021be934(wk);
    func_ov217_021bf15c(wk);
    func_ov217_021bf264(wk);
    func_ov217_021bf2fc(wk);
    func_ov217_021bf38c(wk);
    func_ov217_021bf3b4(wk);
    func_ov217_021bf47c(wk);
    func_ov217_021beaf8(wk);
    func_ov217_021beea4(wk);
    func_ov217_021bef98(wk);

    if (wk->resultsShown && func_ov217_021bf810(wk)) {
        func_ov217_021bef30(wk);
        func_ov217_021befd4(wk);
    }

    if (wk->resultsShown && func_ov217_021bf810(wk)) {
        if (shrunk) {
            func_ov217_021c118c(func_ov217_021bf76c(wk), 20);
        }
        func_ov217_021c1160(func_ov217_021bf76c(wk));
        func_ov217_021c1184(func_ov217_021bf76c(wk), TRUE);
    }
    GFL_SndSEPlay(SEQ_SE_SELECT1);
}

// pokebw2's ResearchGraph_FadeIn
static void func_ov217_021be8fc(void) {
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
}

// pokebw2's ResearchGraph_FadeOut
static void func_ov217_021be90c(void) {
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 0);
}

// pokebw2's ResearchGraph_MoveCursor
static void func_ov217_021be91c(ResearchGraph *wk, int dir) {
    wk->cursor = (wk->cursor + dir + CURSOR_COUNT) % CURSOR_COUNT;
}

// pokebw2's ResearchGraph_SetCursor
static void func_ov217_021be930(ResearchGraph *wk, int pos) {
    wk->cursor = pos;
}

// The answer's and the count's areas take the cursor's palette in two parts, around their labels
// pokebw2's ResearchGraph_HighlightCursor
static void func_ov217_021be934(ResearchGraph *wk) {
    switch (wk->cursor) {
    case CURSOR_QUESTION:
    case CURSOR_PLAYER_ANSWER:
    case CURSOR_MODE:
        GFL_BGSysSetScrPaletteNo(BG_MAIN_GRAPH, data_ov217_021c3ac8[wk->cursor].tileX, data_ov217_021c3ac8[wk->cursor].tileY,
                                 data_ov217_021c3ac8[wk->cursor].tileWidth, data_ov217_021c3ac8[wk->cursor].tileHeight,
                                 BG_PALETTE_CURSOR);
        break;
    case CURSOR_ANSWER:
        GFL_BGSysSetScrPaletteNo(BG_MAIN_GRAPH, 13, 4, 19, 4, BG_PALETTE_CURSOR);
        GFL_BGSysSetScrPaletteNo(BG_MAIN_GRAPH, 16, 8, 16, 3, BG_PALETTE_CURSOR);
        break;
    case CURSOR_BUTTON:
        func_ov217_021beb6c(wk);
        break;
    }
    GFL_BGSysLoadScr(BG_MAIN_GRAPH);
}

// pokebw2's ResearchGraph_UnhighlightCursor
static void func_ov217_021be9b8(ResearchGraph *wk) {
    switch (wk->cursor) {
    case CURSOR_QUESTION:
    case CURSOR_PLAYER_ANSWER:
    case CURSOR_MODE:
        GFL_BGSysSetScrPaletteNo(BG_MAIN_GRAPH, data_ov217_021c3ac8[wk->cursor].tileX, data_ov217_021c3ac8[wk->cursor].tileY,
                                 data_ov217_021c3ac8[wk->cursor].tileWidth, data_ov217_021c3ac8[wk->cursor].tileHeight,
                                 BG_PALETTE_NORMAL);
        break;
    case CURSOR_ANSWER:
        GFL_BGSysSetScrPaletteNo(BG_MAIN_GRAPH, 13, 4, 19, 4, BG_PALETTE_NORMAL);
        GFL_BGSysSetScrPaletteNo(BG_MAIN_GRAPH, 16, 8, 16, 3, BG_PALETTE_NORMAL);
        break;
    case CURSOR_BUTTON:
        func_ov217_021beb88(wk);
        break;
    }
    GFL_BGSysLoadScr(BG_MAIN_GRAPH);
}

// pokebw2's ResearchGraph_ChangeQuestion
static void func_ov217_021bea3c(ResearchGraph *wk, int dir) {
    wk->question = (wk->question + dir + RESEARCH_QUESTION_COUNT) % RESEARCH_QUESTION_COUNT;
}

// Chooses the answer of the graph's first slice, the most given
// pokebw2's ResearchGraph_SetFirstAnswer
static void func_ov217_021bea54(ResearchGraph *wk) {
    CircleGraph *graph = func_ov217_021bf76c(wk);
    int questionId = func_ov217_021bf7cc(wk);
    int id = func_ov217_021c11d0(graph, 0);
    u8 answer = func_ov217_021c26ac(&wk->data, questionId, id);

    wk->answer = answer;
}

// Chooses the answer of the slice before or after the chosen one's in the graph
// pokebw2's ResearchGraph_ChangeAnswer
static void func_ov217_021bea8c(ResearchGraph *wk, int dir) {
    CircleGraph *graph = func_ov217_021bf76c(wk);
    int questionId = func_ov217_021bf7cc(wk);
    int count = func_ov217_021bf7e0(wk);
    int index = func_ov217_021c11c8(graph, func_ov217_021bf7f4(wk));
    int id;

    index = (count + (index + dir)) % count;
    id = func_ov217_021c11d0(graph, index);
    wk->answer = func_ov217_021c26ac(&wk->data, questionId, id);
}

// pokebw2's ResearchGraph_SetFirstAnswerIndex
static void func_ov217_021beaec(ResearchGraph *wk) {
    wk->answer = 0;
}

// The button can be pressed until the results are shown, and only if somebody answered the question
// pokebw2's ResearchGraph_UpdateButton
static void func_ov217_021beaf8(ResearchGraph *wk) {
    if (wk->resultsShown == TRUE || wk->updating == TRUE || !func_ov217_021bf810(wk)) {
        if (!func_ov217_021bf5d0(wk, ANIME_BUTTON_DISABLE)) {
            func_ov217_021bebe4(wk);
        }
    } else {
        if (func_ov217_021bf5d0(wk, ANIME_BUTTON_DISABLE) == TRUE) {
            func_ov217_021bebc0(wk);
        }
    }
}

// pokebw2's ResearchGraph_PressButton
static void func_ov217_021beb40(ResearchGraph *wk) {
    func_02025a54(wk->bmpOamActors[BMP_BUTTON], 0);
    func_ov217_021bf5bc(wk, ANIME_BUTTON_DISABLE);
    func_ov217_021bf5bc(wk, ANIME_BUTTON_ENABLE);
    func_ov217_021bf594(wk, ANIME_DECIDE);
}

// pokebw2's ResearchGraph_HighlightButton
static void func_ov217_021beb6c(ResearchGraph *wk) {
    func_02025a54(wk->bmpOamActors[BMP_BUTTON], 0);
    func_ov217_021bf594(wk, ANIME_BUTTON_CURSOR);
}

// pokebw2's ResearchGraph_UnhighlightButton
static void func_ov217_021beb88(ResearchGraph *wk) {
    func_02025a54(wk->bmpOamActors[BMP_BUTTON], 1);
    func_ov217_021bf5bc(wk, ANIME_BUTTON_CURSOR);
}

// pokebw2's ResearchGraph_MoveToButton
static void func_ov217_021beba4(ResearchGraph *wk) {
    func_02025a54(wk->bmpOamActors[BMP_BUTTON], 0);
    func_ov217_021bf594(wk, ANIME_BUTTON_MOVE);
}

// pokebw2's ResearchGraph_EnableButton
static void func_ov217_021bebc0(ResearchGraph *wk) {
    func_02025a54(wk->bmpOamActors[BMP_BUTTON], 1);
    func_ov217_021bf5bc(wk, ANIME_BUTTON_DISABLE);
    func_ov217_021bf594(wk, ANIME_BUTTON_ENABLE);
}

// pokebw2's ResearchGraph_DisableButton
static void func_ov217_021bebe4(ResearchGraph *wk) {
    func_02025a54(wk->bmpOamActors[BMP_BUTTON], 1);
    func_ov217_021bf594(wk, ANIME_BUTTON_DISABLE);
}

// pokebw2's ResearchGraph_StartReturnAnime
static void func_ov217_021bec00(ResearchGraph *wk) {
    func_ov217_021c09d8(wk->common, 0);
}

// pokebw2's ResearchGraph_SwapGraphs
static void func_ov217_021bec0c(ResearchGraph *wk) {
    int i;

    for (i = 0; i < MODE_COUNT; i++) {
        CircleGraph *graph = wk->graphs[i];

        wk->graphs[i] = wk->nextGraphs[i];
        wk->nextGraphs[i] = graph;
    }
}

// The centre of a slice is its color blended a third of the way toward white
// pokebw2's ResearchGraph_SetCenterColor
static void func_ov217_021bec34(CircleGraphData *item) {
    u8 green = (item->color[1] + 62) / 3;
    u8 blue = (item->color[2] + 62) / 3;

    item->centerColor[0] = (item->color[0] + 62) / 3;
    item->centerColor[1] = green;
    item->centerColor[2] = blue;
}

// The old graph is drawn behind the new one that grows over it
// pokebw2's ResearchGraph_SetGraphDepths
static void func_ov217_021bec64(ResearchGraph *wk) {
    func_ov217_021c11a4(func_ov217_021bf76c(wk), -2 * FX16_ONE);
    func_ov217_021c11a4(func_ov217_021bf77c(wk), 0);
}

// pokebw2's ResearchGraph_SetGraphData
static void func_ov217_021bec84(ResearchGraph *wk, u32 mode) {
    CircleGraphData data[RESEARCH_ANSWER_MAX];
    CircleGraph *graph = wk->graphs[mode];
    int count = func_ov217_021bf7e0(wk);
    ResearchQuestion *question = &wk->data.questions[wk->question];
    int i;

    for (i = 0; i < count; i++) {
        ResearchAnswer *answer = &question->answers[i];
        CircleGraphData *item = &data[i];

        item->id = answer->id;
        item->color[0] = answer->red;
        item->color[1] = answer->green;
        item->color[2] = answer->blue;
        func_ov217_021bec34(item);
        switch (mode) {
        case MODE_TODAY:
            item->count = answer->todayCount;
            break;
        case MODE_TOTAL:
            item->count = answer->totalCount;
            break;
        }
    }
    func_ov217_021c10e8(graph, data, count);
}

// pokebw2's ResearchGraph_SetNextGraphData
static void func_ov217_021bed10(ResearchGraph *wk, u32 mode) {
    CircleGraphData data[RESEARCH_ANSWER_MAX];
    CircleGraph *graph = wk->nextGraphs[mode];
    int count = func_ov217_021bf7e0(wk);
    ResearchQuestion *question = &wk->data.questions[wk->question];
    int i;

    for (i = 0; i < count; i++) {
        ResearchAnswer *answer = &question->answers[i];
        CircleGraphData *item = &data[i];

        item->id = answer->id;
        item->color[0] = answer->red;
        item->color[1] = answer->green;
        item->color[2] = answer->blue;
        func_ov217_021bec34(item);
        switch (mode) {
        case MODE_TODAY:
            item->count = answer->todayCount;
            break;
        case MODE_TOTAL:
            item->count = answer->totalCount;
            break;
        }
    }
    func_ov217_021c10e8(graph, data, count);
}

// pokebw2's ResearchGraph_UpdateGraphs
static void func_ov217_021bed9c(ResearchGraph *wk) {
    int i;

    for (i = 0; i < MODE_COUNT; i++) {
        func_ov217_021c1124(wk->graphs[i]);
        func_ov217_021c1124(wk->nextGraphs[i]);
    }
}

// pokebw2's ResearchGraph_DrawGraphs
static void func_ov217_021bedc4(ResearchGraph *wk) {
    int i;

    for (i = 0; i < MODE_COUNT; i++) {
        func_ov217_021c112c(wk->graphs[i]);
        func_ov217_021c112c(wk->nextGraphs[i]);
    }
}

// Draws a square of the chosen answer's color next to its name
// pokebw2's ResearchGraph_DrawAnswerColor
static void func_ov217_021bedec(ResearchGraph *wk) {
    if (wk->resultsShown && wk->updating != TRUE && func_ov217_021bf810(wk)) {
        ResearchData *data = &wk->data;
        u8 red = func_ov217_021c2654(data, wk->question, wk->answer);
        u8 green = func_ov217_021c2664(data, wk->question, wk->answer);
        u8 blue = func_ov217_021c2674(data, wk->question, wk->answer);

        G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, 31, 0);
        G3_Begin(GX_BEGIN_QUADS);
        G3_Color(GX_RGB(red, green, blue));
        G3_Vtx(-0x398, 0x96c, 0);
        G3_Vtx(-0x398, 0xa7a, 0);
        G3_Vtx(-0x25f, 0xa7a, 0);
        G3_Vtx(-0x25f, 0x96c, 0);
        G3_End();
    }
}

// Points the arrow from the answer's name to its slice
// pokebw2's ResearchGraph_ShowArrow
static void func_ov217_021beea4(ResearchGraph *wk) {
    int x, y;

    func_ov217_021c20d8(wk->arrow);
    // resultsShown is tested twice, as the original reads it again at the end
    if (wk->updating != TRUE && wk->resultsShown && func_ov217_021bf810(wk) &&
        func_ov217_021c11dc(func_ov217_021bf76c(wk), func_ov217_021bf7f4(wk)) && wk->resultsShown) {
        func_ov217_021c11fc(func_ov217_021bf76c(wk), func_ov217_021bf7f4(wk), &x, &y);
        func_ov217_021c20b4(wk->arrow, 104, 34, x, y);
        func_ov217_021c20cc(wk->arrow);
    }
}

// Sets up a percentage on each slice of PERCENTAGE_MIN or more, from the last
// pokebw2's ResearchGraph_SetupPercentages
static void func_ov217_021bef30(ResearchGraph *wk) {
    // The count goes in the loop's counter first: a loop that starts from a call's result is longer
    int i = func_ov217_021bf7e0(wk);
    CircleGraph *graph = func_ov217_021bf76c(wk);
    int count = 0;
    int x, y;

    for (i = i - 1; i >= 0; i--) {
        int percent = func_ov217_021c11e8(graph, i);

        func_ov217_021c1224(graph, i, &x, &y);
        if (percent >= PERCENTAGE_MIN) {
            Percentage *percentage = wk->percentages[count];

            func_ov217_021c272c(percentage, percent);
            func_ov217_021c271c(percentage, x, y);
            count++;
        }
    }
    wk->percentageCount = count;
}

// pokebw2's ResearchGraph_HidePercentages
static void func_ov217_021bef98(ResearchGraph *wk) {
    int i;

    for (i = 0; i < PERCENTAGE_COUNT; i++) {
        func_ov217_021c2740(wk->percentages[i], FALSE);
    }
    wk->percentagesShown = 0;
}

// pokebw2's ResearchGraph_ShowPercentage
static void func_ov217_021befc0(ResearchGraph *wk, u8 index) {
    func_ov217_021c2740(wk->percentages[index], TRUE);
}

// pokebw2's ResearchGraph_ShowPercentages
static void func_ov217_021befd4(ResearchGraph *wk) {
    int i;
    int count = wk->percentageCount;

    for (i = 0; i < count; i++) {
        func_ov217_021befc0(wk, i);
    }
}

// The arrows' touch rectangles are 32 pixels square around them
// pokebw2's ResearchGraph_UpdateArrowRects
static void func_ov217_021beff8(ResearchGraph *wk) {
    const CursorEntry *entry = &data_ov217_021c3ac8[wk->cursor];
    TouchRect *rect = &wk->touchRects[TOUCH_LEFT];

    rect->left = func_ov217_021c0900(entry->x + entry->leftX - 16);
    rect->top = func_ov217_021c0900(entry->y + entry->leftY - 16);
    rect->right = func_ov217_021c0900(rect->left + 32);
    rect->bottom = func_ov217_021c0900(rect->top + 32);

    // The original looks the entry up again for the right arrow
    entry = &data_ov217_021c3ac8[wk->cursor];
    rect = &wk->touchRects[TOUCH_RIGHT];
    rect->left = func_ov217_021c0900(entry->x + entry->rightX - 16);
    rect->top = func_ov217_021c0900(entry->y + entry->rightY - 16);
    rect->right = func_ov217_021c0900(rect->left + 32);
    rect->bottom = func_ov217_021c0900(rect->top + 32);
}

// The graph's frame: without the labels of the results until they are shown or while they are updated
// pokebw2's ResearchGraph_LoadGraphBG
static void func_ov217_021bf08c(ResearchGraph *wk) {
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, wk->heapId);
    u32 fileId;
    void *file;
    NNSG2dScreenData *screen;

    if (wk->updating) {
        fileId = 8;
    } else if (!func_ov217_021bf810(wk)) {
        fileId = 6;
    } else if (!wk->resultsShown) {
        fileId = 6;
    } else {
        fileId = 7;
    }
    file = GFL_ArcToolReadHeapNew(handle, fileId, wk->heapId);
    NNS_G2dGetUnpackedScreenData(file, &screen);
    GFL_BGSysLoadScrAreaAll(BG_MAIN_GRAPH, screen->rawData, 0, 0, 32, 32);
    GFL_BGSysLoadScr(BG_MAIN_GRAPH);
    GFL_HeapFree(file);
    GFL_ArcToolFree(handle);
}

// pokebw2's ResearchGraph_PrintSurvey
static void func_ov217_021bf100(ResearchGraph *wk) {
    func_ov217_021c0f5c(wk->bgFonts[BG_FONT_SURVEY], data_ov217_021c3628[wk->data.surveyId]);
}

// pokebw2's ResearchGraph_PrintQuestion
static void func_ov217_021bf11c(ResearchGraph *wk) {
    func_ov217_021c0f5c(wk->bgFonts[BG_FONT_QUESTION], data_ov217_021c36b8[func_ov217_021bf7cc(wk)]);
}

// pokebw2's ResearchGraph_PrintQuestionTitle
static void func_ov217_021bf13c(ResearchGraph *wk) {
    BGFont *bgFont = wk->bgFonts[BG_FONT_QUESTION_TITLE];

    func_ov217_021c0f5c(bgFont, data_ov217_021c369a[func_ov217_021bf7cc(wk)]);
}

// The chosen answer's rank in the graph, name, count and percentage
// pokebw2's ResearchGraph_PrintAnswer
static void func_ov217_021bf15c(ResearchGraph *wk) {
    BGFont *bgFont = wk->bgFonts[BG_FONT_ANSWER];
    CircleGraph *graph = func_ov217_021bf76c(wk);
    u16 answerId;
    u8 index;
    u32 total;
    u8 percent;
    StrBuf *format;
    StrBuf *name;
    StrBuf *str;

    if (!wk->resultsShown || wk->updating == TRUE) {
        func_ov217_021c0ff8(bgFont, FALSE);
        return;
    }
    if (!func_ov217_021bf810(wk)) {
        func_ov217_021c0ff8(bgFont, FALSE);
        return;
    }

    answerId = func_ov217_021bf7f4(wk);
    index = func_ov217_021c11c8(graph, answerId);
    total = func_ov217_021bf854(wk);
    percent = func_ov217_021c11dc(graph, answerId);
    format = GFL_MsgDataLoadStrbufNew(wk->msgData[0], 13);
    name = GFL_MsgDataLoadStrbufNew(wk->msgData[MSG_DATA_ANSWERS], answerId);
    str = GFL_StrBufCreate(0x80, wk->heapId);
    WordSetNumber(wk->wordSet, 0, index + 1, 2, 1, 1);
    func_0201ee0c(wk->wordSet, 1, name, 0, 1, 2);
    WordSetNumber(wk->wordSet, 2, total, 5, 1, 1);
    WordSetNumber(wk->wordSet, 3, percent, 3, 1, 1);
    GFL_WordSetFormatStrbuf(wk->wordSet, str, format);
    func_ov217_021c0f78(bgFont, str);
    func_ov217_021c0ff8(bgFont, TRUE);
    GFL_StrBufFree(format);
    GFL_StrBufFree(str);
    GFL_StrBufFree(name);
}

// pokebw2's ResearchGraph_PrintPlayerAnswer
static void func_ov217_021bf264(ResearchGraph *wk) {
    u8 answerId;
    StrBuf *format;
    StrBuf *name;
    StrBuf *str;

    if (!wk->resultsShown || !func_ov217_021bf810(wk)) {
        func_ov217_021c0ff8(wk->bgFonts[BG_FONT_PLAYER_ANSWER], FALSE);
        return;
    }

    answerId = func_ov217_021bf910(wk);
    format = GFL_MsgDataLoadStrbufNew(wk->msgData[0], 14);
    name = GFL_MsgDataLoadStrbufNew(wk->msgData[MSG_DATA_ANSWERS], answerId);
    str = GFL_StrBufCreate(0x80, wk->heapId);
    func_0201ee0c(wk->wordSet, 0, name, 0, 1, 2);
    GFL_WordSetFormatStrbuf(wk->wordSet, str, format);
    func_ov217_021c0ff8(wk->bgFonts[BG_FONT_PLAYER_ANSWER], TRUE);
    func_ov217_021c0f78(wk->bgFonts[BG_FONT_PLAYER_ANSWER], str);
    GFL_StrBufFree(format);
    GFL_StrBufFree(str);
    GFL_StrBufFree(name);
}

// How many people answered, today or in all
// pokebw2's ResearchGraph_PrintTotal
static void func_ov217_021bf2fc(ResearchGraph *wk) {
    u32 strId;
    u32 total;
    WordSet *wordSet;
    StrBuf *format;
    StrBuf *str;

    if (wk->resultsShown) {
        switch (wk->mode) {
        case MODE_TODAY:
            strId = 15;
            break;
        case MODE_TOTAL:
            strId = 17;
            break;
        }
    } else {
        switch (wk->mode) {
        case MODE_TODAY:
            strId = 16;
            break;
        case MODE_TOTAL:
            strId = 18;
            break;
        }
    }

    total = func_ov217_021bf810(wk);
    wordSet = wk->wordSet;
    format = GFL_MsgDataLoadStrbufNew(wk->msgData[0], strId);
    str = GFL_StrBufCreate(0x80, wk->heapId);
    WordSetNumber(wordSet, 0, total, 6, 1, 1);
    GFL_WordSetFormatStrbuf(wordSet, str, format);
    func_ov217_021c0f78(wk->bgFonts[BG_FONT_TOTAL], str);
    GFL_StrBufFree(format);
    GFL_StrBufFree(str);
}

// pokebw2's ResearchGraph_ShowNoAnswers
static void func_ov217_021bf38c(ResearchGraph *wk) {
    if (!func_ov217_021bf810(wk)) {
        func_ov217_021c0ff8(wk->bgFonts[BG_FONT_NO_ANSWERS], TRUE);
    } else {
        func_ov217_021c0ff8(wk->bgFonts[BG_FONT_NO_ANSWERS], FALSE);
    }
}

// pokebw2's ResearchGraph_ShowUpdating
static void func_ov217_021bf3b4(ResearchGraph *wk) {
    if (wk->updating) {
        func_ov217_021c0ff8(wk->bgFonts[BG_FONT_UPDATING], TRUE);
    } else {
        func_ov217_021c0ff8(wk->bgFonts[BG_FONT_UPDATING], FALSE);
    }
}

// The arrows on either side of the cursor, except where there is nothing to choose
// pokebw2's ResearchGraph_UpdateArrows
static void func_ov217_021bf3d8(ResearchGraph *wk) {
    const CursorEntry *entry = &data_ov217_021c3ac8[wk->cursor];
    ClActor *left = func_ov217_021bfa08(wk, ACTOR_LEFT);
    ClActor *right = func_ov217_021bfa08(wk, ACTOR_RIGHT);
    ClActorPos pos;

    if (wk->updating == TRUE || wk->cursor == CURSOR_PLAYER_ANSWER || wk->cursor == CURSOR_BUTTON) {
        func_0204b3dc(left, FALSE);
        func_0204b3dc(right, FALSE);
        return;
    }

    pos.x = entry->x + entry->leftX;
    pos.y = entry->y + entry->leftY;
    func_0204b404(left, &pos, 0);
    pos.x = entry->x + entry->rightX;
    pos.y = entry->y + entry->rightY;
    func_0204b404(right, &pos, 0);
    func_0204b3dc(left, TRUE);
    func_0204b3dc(right, TRUE);
    func_0204b92c(left, TRUE);
    func_0204b92c(right, TRUE);
}

// pokebw2's ResearchGraph_UpdateLegend
static void func_ov217_021bf47c(ResearchGraph *wk) {
    if (!wk->resultsShown || !func_ov217_021bf810(wk)) {
        func_0204b3dc(wk->actors[ACTOR_LEGEND], FALSE);
        return;
    }
    func_0204b3dc(wk->actors[ACTOR_LEGEND], TRUE);
}

// Marks the player's answer in the graph while the cursor is on it
// pokebw2's ResearchGraph_UpdateMarker
static void func_ov217_021bf4ac(ResearchGraph *wk) {
    ClActor *marker = wk->actors[ACTOR_MARKER];
    const ResearchActorSetup *entry = &data_ov217_021c37fc[ACTOR_MARKER];
    u32 answerId;
    int x, y;
    ClActorPos pos;

    if (!wk->resultsShown) {
        func_0204b3dc(marker, FALSE);
        return;
    }
    if (wk->cursor != CURSOR_PLAYER_ANSWER) {
        func_0204b3dc(marker, FALSE);
        return;
    }

    answerId = func_ov217_021bf910(wk);
    if (!func_ov217_021c11dc(func_ov217_021bf76c(wk), answerId)) {
        func_0204b3dc(marker, FALSE);
        return;
    }
    if (answerId == 0) {
        func_0204b3dc(marker, FALSE);
        return;
    }

    func_ov217_021c11fc(func_ov217_021bf76c(wk), answerId, &x, &y);
    pos.x = x;
    pos.y = y;
    func_0204b404(marker, &pos, entry->surface);
    func_0204b3dc(marker, TRUE);
    func_0204b92c(marker, TRUE);
    func_0204b9c4(marker, 2);
}

// pokebw2's ResearchGraph_ShowButton
static void func_ov217_021bf55c(ResearchGraph *wk) {
    func_ov217_021bf580(wk, BMP_BUTTON, TRUE);
}

// pokebw2's ResearchGraph_ShowMessage
static void func_ov217_021bf568(ResearchGraph *wk) {
    func_ov217_021bf580(wk, BMP_MESSAGE, TRUE);
}

// pokebw2's ResearchGraph_HideMessage
static void func_ov217_021bf574(ResearchGraph *wk) {
    func_ov217_021bf580(wk, BMP_MESSAGE, FALSE);
}

// pokebw2's ResearchGraph_SetBmpOamVisible
static void func_ov217_021bf580(ResearchGraph *wk, u32 index, BOOL visible) {
    func_02025774(wk->bmpOamActors[index], visible);
}

// pokebw2's ResearchGraph_StartPaletteAnime
static void func_ov217_021bf594(ResearchGraph *wk, u32 index) {
    func_ov217_021c29f8(wk->animes[index], data_ov217_021c3a28[index].mode, data_ov217_021c3a28[index].color);
}

// pokebw2's ResearchGraph_StopPaletteAnime
static void func_ov217_021bf5bc(ResearchGraph *wk, u32 index) {
    func_ov217_021c2a00(wk->animes[index]);
}

// pokebw2's ResearchGraph_IsPaletteAnimeActive
static BOOL func_ov217_021bf5d0(ResearchGraph *wk, u32 index) {
    return func_ov217_021c2a10(wk->animes[index]);
}

// pokebw2's ResearchGraph_UpdatePaletteAnimes
static void func_ov217_021bf5e4(ResearchGraph *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        func_ov217_021c29f0(wk->animes[i]);
    }
}

// pokebw2's ResearchGraph_UpdateCommonPaletteAnime
static void func_ov217_021bf600(ResearchGraph *wk) {
    func_ov217_021c09d0(wk->common);
}

// pokebw2's ResearchGraph_FlashIn
static void func_ov217_021bf60c(ResearchGraph *wk) {
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_MAIN_BG, 0xffff, 3, 0, 7, 0xffff, wk->tcbMgr);
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_MAIN_OBJ, 0xffff, 3, 0, 7, 0xffff, wk->tcbMgr);
}

// pokebw2's ResearchGraph_FlashOut
static void func_ov217_021bf658(ResearchGraph *wk) {
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_MAIN_BG, 0xffff, 3, 7, 0, 0xffff, wk->tcbMgr);
    func_0202173c(wk->paletteFade, 1 << PALFADE_BUFFER_MAIN_OBJ, 0xffff, 3, 7, 0, 0xffff, wk->tcbMgr);
}

// pokebw2's ResearchGraph_IsPaletteFadeDone
static BOOL func_ov217_021bf6a4(ResearchGraph *wk) {
    if (!func_02021ed8(wk->paletteFade)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ResearchGraph_VBlank
static void func_ov217_021bf6bc(TCB *tcb, void *data) {
    ResearchGraph *wk = data;

    GFL_BGSysUpdate();
    func_0204a648();
    func_02021d50(wk->paletteFade);
}

// New results came in by beacon if today's count of answers saved is no longer the one shown
// pokebw2's ResearchGraph_CheckNewResults
static void func_ov217_021bf6d4(ResearchGraph *wk) {
    if (func_020270e0() == TRUE) {
        u16 shown = func_ov217_021bf834(wk);
        u16 saved = func_ov217_021bf9cc(wk);

        if (shown != saved) {
            wk->newResults = TRUE;
        }
    }
}

// pokebw2's ResearchGraph_ClearNewResults
static void func_ov217_021bf704(ResearchGraph *wk) {
    wk->newResults = FALSE;
}

// pokebw2's ResearchGraph_CountUpdateFrames
static void func_ov217_021bf710(ResearchGraph *wk) {
    if (wk->updateFrames < UPDATE_WAIT_FRAMES) {
        wk->updateFrames++;
    } else {
        wk->updateReady = TRUE;
    }
}

// pokebw2's ResearchGraph_ResetUpdateFrames
static void func_ov217_021bf728(ResearchGraph *wk) {
    wk->updateFrames = 0;
    wk->updateReady = FALSE;
}

// pokebw2's ResearchGraph_GetGameSystem
static GameSystem *func_ov217_021bf734(ResearchGraph *wk) {
    return func_ov217_021c09b0(wk->common);
}

// pokebw2's ResearchGraph_GetGameData
static GameData *func_ov217_021bf740(ResearchGraph *wk) {
    return func_ov217_021c09b4(wk->common);
}

// pokebw2's ResearchGraph_SetHeapID
static void func_ov217_021bf74c(ResearchGraph *wk, HeapID heapId) {
    wk->heapId = heapId;
}

// pokebw2's ResearchGraph_GetCommon
static ResearchCommon *func_ov217_021bf750(ResearchGraph *wk) {
    return wk->common;
}

// pokebw2's ResearchGraph_SetCommon
static void func_ov217_021bf754(ResearchGraph *wk, ResearchCommon *common) {
    wk->common = common;
}

// pokebw2's ResearchGraph_IsForceExit
static BOOL func_ov217_021bf758(ResearchGraph *wk) {
    return func_ov217_021c09f8(func_ov217_021bf750(wk));
}

// pokebw2's ResearchGraph_SetMode
static void func_ov217_021bf764(ResearchGraph *wk, u32 mode) {
    wk->mode = mode;
}

// pokebw2's ResearchGraph_GetGraph
static CircleGraph *func_ov217_021bf76c(ResearchGraph *wk) {
    return wk->graphs[wk->mode];
}

// pokebw2's ResearchGraph_GetNextGraph
static CircleGraph *func_ov217_021bf77c(ResearchGraph *wk) {
    return wk->nextGraphs[wk->mode];
}

// pokebw2's ResearchGraph_AreGraphsStill
static BOOL func_ov217_021bf78c(ResearchGraph *wk) {
    if (func_ov217_021c11f4(wk->graphs[MODE_TODAY]) || func_ov217_021c11f4(wk->graphs[MODE_TOTAL]) ||
        func_ov217_021c11f4(wk->nextGraphs[MODE_TODAY]) || func_ov217_021c11f4(wk->nextGraphs[MODE_TOTAL])) {
        return FALSE;
    }
    return TRUE;
}

// pokebw2's ResearchGraph_GetQuestionID
static u8 func_ov217_021bf7cc(ResearchGraph *wk) {
    return wk->data.questions[wk->question].id;
}

// pokebw2's ResearchGraph_GetAnswerCount
static u8 func_ov217_021bf7e0(ResearchGraph *wk) {
    return wk->data.questions[wk->question].answerCount;
}

// pokebw2's ResearchGraph_GetAnswerID
static u16 func_ov217_021bf7f4(ResearchGraph *wk) {
    return wk->data.questions[wk->question].answers[wk->answer].id;
}

// How many people answered the question, today or in all
// pokebw2's ResearchGraph_GetQuestionTotal
static u32 func_ov217_021bf810(ResearchGraph *wk) {
    u32 total = 0;

    switch (wk->mode) {
    case MODE_TODAY:
        total = func_ov217_021bf834(wk);
        break;
    case MODE_TOTAL:
        total = func_ov217_021bf844(wk);
        break;
    }
    return total;
}

// pokebw2's ResearchGraph_GetQuestionTodayTotal
static u32 func_ov217_021bf834(ResearchGraph *wk) {
    return wk->data.questions[wk->question].todayCount;
}

// pokebw2's ResearchGraph_GetQuestionAllTotal
static u32 func_ov217_021bf844(ResearchGraph *wk) {
    return wk->data.questions[wk->question].totalCount;
}

// How many people gave the chosen answer, today or in all
// pokebw2's ResearchGraph_GetAnswerTotal
static u32 func_ov217_021bf854(ResearchGraph *wk) {
    switch (wk->mode) {
    case MODE_TODAY:
        return func_ov217_021bf878(wk);
    case MODE_TOTAL:
        return func_ov217_021bf894(wk);
    }
    return 0;
}

// pokebw2's ResearchGraph_GetAnswerTodayTotal
static u32 func_ov217_021bf878(ResearchGraph *wk) {
    return wk->data.questions[wk->question].answers[wk->answer].todayCount;
}

// pokebw2's ResearchGraph_GetAnswerAllTotal
static u32 func_ov217_021bf894(ResearchGraph *wk) {
    return wk->data.questions[wk->question].answers[wk->answer].totalCount;
}

// The survey being run, found from its questions, or the first if none matches
// pokebw2's ResearchGraph_GetSurveyID
u8 func_ov217_021bf8b0(ResearchGraph *wk) {
    int i;
    u8 questions[RESEARCH_QUESTION_COUNT];
    u8 id;
    void *survey = func_0200e394(GameData_GetSaveControl(func_ov217_021bf740(wk)));

    for (i = 0; i < RESEARCH_QUESTION_COUNT; i++) {
        questions[i] = func_0200e44c(survey, i);
    }
    for (id = 0; id < SURVEY_COUNT; id++) {
        if (questions[0] == data_ov217_021c363c[id] && questions[1] == data_ov217_021c3646[id] &&
            questions[2] == data_ov217_021c3632[id]) {
            return id;
        }
    }
    return 0;
}

// The ID of the player's answer to the question, or 0 if the player hasn't answered it
// pokebw2's ResearchGraph_GetPlayerAnswer
static u8 func_ov217_021bf910(ResearchGraph *wk) {
    void *answers = func_0200e3a0(func_0200e394(GameData_GetSaveControl(func_ov217_021bf740(wk))));
    u8 questionId = func_ov217_021bf7cc(wk);
    u16 answer;

    if (questionId == QUESTION_PLAY_TIME) {
        return func_ov217_021bf968(wk);
    }

    answer = func_0200e3a4(answers, questionId);
    if (answer == 0) {
        return 0;
    }
    answer--;
    return func_ov217_021c2644(&wk->data, wk->question, answer);
}

// pokebw2's ResearchGraph_GetPlayTimeAnswer
static u8 func_ov217_021bf968(ResearchGraph *wk) {
    u16 hours = func_0200864c(func_02012fcc(func_ov217_021bf740(wk)));

    if (hours < 10) {
        return ANSWER_PLAY_TIME_FIRST;
    } else if (hours < 20) {
        return ANSWER_PLAY_TIME_FIRST + 1;
    } else if (hours < 30) {
        return ANSWER_PLAY_TIME_FIRST + 2;
    } else if (hours < 40) {
        return ANSWER_PLAY_TIME_FIRST + 3;
    } else if (hours < 50) {
        return ANSWER_PLAY_TIME_FIRST + 4;
    } else if (hours < 60) {
        return ANSWER_PLAY_TIME_FIRST + 5;
    } else if (hours < 70) {
        return ANSWER_PLAY_TIME_FIRST + 6;
    } else if (hours < 80) {
        return ANSWER_PLAY_TIME_FIRST + 7;
    } else if (hours < 90) {
        return ANSWER_PLAY_TIME_FIRST + 8;
    } else if (hours < 100) {
        return ANSWER_PLAY_TIME_FIRST + 9;
    }
    return ANSWER_PLAY_TIME_FIRST + 10;
}

// pokebw2's ResearchGraph_GetSavedTodayTotal
static u32 func_ov217_021bf9cc(ResearchGraph *wk) {
    void *survey = func_0200e394(GameData_GetSaveControl(func_ov217_021bf740(wk)));

    return func_0200e458(survey, func_ov217_021bf7cc(wk));
}

// pokebw2's ResearchGraph_GetObjRes
static u32 func_ov217_021bf9f0(ResearchGraph *wk, u32 index) {
    return wk->objRes[index];
}

// pokebw2's ResearchGraph_GetUnit
static ClActUnit *func_ov217_021bf9fc(ResearchGraph *wk, u32 index) {
    return wk->units[index];
}

// pokebw2's ResearchGraph_GetActor
static ClActor *func_ov217_021bfa08(ResearchGraph *wk, u32 index) {
    return wk->actors[index];
}

// pokebw2's ResearchGraph_Alloc
static ResearchGraph *func_ov217_021bfa14(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(ResearchGraph), FALSE, "research_graph.c", 4096);
}

// pokebw2's ResearchGraph_InitWork
static void func_ov217_021bfa30(ResearchGraph *wk) {
    int i;

    wk->seq = SEQ_SETUP;
    wk->seqState = 0;
    wk->seqDone = FALSE;
    wk->waitFrames = 15;
    wk->seqFrames = 0;
    wk->updateFrames = 0;
    wk->cursor = CURSOR_QUESTION;
    wk->active = FALSE;
    wk->resultsShown = FALSE;
    wk->touchOpened = FALSE;
    wk->updating = FALSE;
    wk->updateReady = FALSE;
    wk->newResults = FALSE;
    wk->question = 0;
    wk->answer = 0;
    wk->mode = MODE_TODAY;
    wk->tcbMgr = GFL_VBlankGetTCBMgr();
    wk->percentageCount = 0;
    wk->percentagesShown = 0;
    wk->next = RESEARCH_GRAPH_NEXT_TOP;
    wk->isEnd = FALSE;
    for (i = 0; i < OBJ_RES_COUNT; i++) {
        wk->objRes[i] = 0;
    }
    func_ov217_021bfc50(wk);
    func_ov217_021bfb0c(wk);
    func_ov217_021bfbdc(wk);
    func_ov217_021bfe80(wk);
    func_ov217_021bfee4(wk);
    func_ov217_021bfb60(wk);
    func_ov217_021bfbb8(wk);
    func_ov217_021bfb30(wk);
    func_ov217_021c019c(wk);
    func_ov217_021c0384(wk);
    func_ov217_021c03ec(wk);
    func_ov217_021c04b0(wk);
    func_ov217_021c07b0(wk);
    func_ov217_021c0820(wk);
}

// pokebw2's ResearchGraph_Free
static void func_ov217_021bfb04(ResearchGraph *wk) {
    GFL_HeapFree(wk);
}

// pokebw2's ResearchGraph_ClearQueue
static void func_ov217_021bfb0c(ResearchGraph *wk) {
    wk->queue = NULL;
}

// pokebw2's ResearchGraph_CreateQueue
static void func_ov217_021bfb14(ResearchGraph *wk) {
    wk->queue = func_ov217_021c0e08(10, wk->heapId);
}

// pokebw2's ResearchGraph_DeleteQueue
static void func_ov217_021bfb24(ResearchGraph *wk) {
    func_ov217_021c0e3c(wk->queue);
}

// pokebw2's ResearchGraph_ClearFont
static void func_ov217_021bfb30(ResearchGraph *wk) {
    wk->font = NULL;
}

// pokebw2's ResearchGraph_CreateFont
static void func_ov217_021bfb38(ResearchGraph *wk) {
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
}

// pokebw2's ResearchGraph_DeleteFont
static void func_ov217_021bfb54(ResearchGraph *wk) {
    GFL_FontFree(wk->font);
}

// pokebw2's ResearchGraph_ClearMsgData
static void func_ov217_021bfb60(ResearchGraph *wk) {
    int i;

    for (i = 0; i < MSG_DATA_COUNT; i++) {
        wk->msgData[i] = NULL;
    }
}

// pokebw2's ResearchGraph_LoadMsgData
static void func_ov217_021bfb74(ResearchGraph *wk) {
    int i;

    for (i = 0; i < MSG_DATA_COUNT; i++) {
        wk->msgData[i] = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, data_ov217_021c3650[i], wk->heapId);
    }
}

// pokebw2's ResearchGraph_FreeMsgData
static void func_ov217_021bfba0(ResearchGraph *wk) {
    int i;

    for (i = 0; i < MSG_DATA_COUNT; i++) {
        GFL_MsgDataFree(wk->msgData[i]);
    }
}

// pokebw2's ResearchGraph_ClearWordSet
static void func_ov217_021bfbb8(ResearchGraph *wk) {
    wk->wordSet = NULL;
}

// pokebw2's ResearchGraph_CreateWordSet
static void func_ov217_021bfbc0(ResearchGraph *wk) {
    wk->wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
}

// pokebw2's ResearchGraph_DeleteWordSet
static void func_ov217_021bfbd0(ResearchGraph *wk) {
    GFL_WordSetSystemFree(wk->wordSet);
}

// pokebw2's ResearchGraph_ClearGraphs
static void func_ov217_021bfbdc(ResearchGraph *wk) {
    int i;

    for (i = 0; i < MODE_COUNT; i++) {
        wk->graphs[i] = NULL;
        wk->nextGraphs[i] = NULL;
    }
}

// pokebw2's ResearchGraph_CreateGraphs
static void func_ov217_021bfbfc(ResearchGraph *wk) {
    int i;

    for (i = 0; i < MODE_COUNT; i++) {
        wk->graphs[i] = func_ov217_021c10c8(wk->heapId);
        wk->nextGraphs[i] = func_ov217_021c10c8(wk->heapId);
    }
}

// pokebw2's ResearchGraph_DeleteGraphs
static void func_ov217_021bfc28(ResearchGraph *wk) {
    int i;

    for (i = 0; i < MODE_COUNT; i++) {
        func_ov217_021c10d8(wk->graphs[i]);
        func_ov217_021c10d8(wk->nextGraphs[i]);
    }
}

// pokebw2's ResearchGraph_ClearData
static void func_ov217_021bfc50(ResearchGraph *wk) {
    MI_CpuFill8(&wk->data, 0, sizeof(ResearchData));
}

static inline u8 GetSurveyAnswerMsgIDCount(u8 question) {
    return data_ov217_021c367c[question];
}

static inline u16 GetSurveyAnswerMsgID(u8 question, int answer) {
    return data_ov217_021c3d44[question][answer];
}
void func_ov217_021bfc60(ResearchGraph *wk) {
    void *survey = func_0200e394(GameData_GetSaveControl(func_ov217_021bf740(wk)));
    int count;
    int q, i;
    u8 surveyId = func_ov217_021bf8b0(wk);
    u16 answerIds[RESEARCH_QUESTION_COUNT][RESEARCH_ANSWER_MAX];
    u32 todayCounts[RESEARCH_QUESTION_COUNT][RESEARCH_ANSWER_MAX];
    u32 pastCounts[RESEARCH_QUESTION_COUNT][RESEARCH_ANSWER_MAX];
    u32 todayTotals[RESEARCH_QUESTION_COUNT];
    u32 pastTotals[RESEARCH_QUESTION_COUNT];
    u8 questions[RESEARCH_QUESTION_COUNT];
    u8 answerCounts[RESEARCH_QUESTION_COUNT];

    questions[0] = func_0200e44c(survey, 0);
    questions[1] = func_0200e44c(survey, 1);
    questions[2] = func_0200e44c(survey, 2);
    answerCounts[0] = data_ov217_021c367c[questions[0]];
    answerCounts[1] = data_ov217_021c367c[questions[1]];
    answerCounts[2] = data_ov217_021c367c[questions[2]];
    todayTotals[0] = func_0200e458(survey, questions[0]);
    todayTotals[1] = func_0200e458(survey, questions[1]);
    todayTotals[2] = func_0200e458(survey, questions[2]);
    pastTotals[0] = func_0200e47c(survey, questions[0]);
    pastTotals[1] = func_0200e47c(survey, questions[1]);
    pastTotals[2] = func_0200e47c(survey, questions[2]);

    for (q = 0; q < RESEARCH_QUESTION_COUNT; q++) {
        u8 question = questions[q];

        count = answerCounts[q];
        for (i = 0; i < count; i++) {
            answerIds[q][i] = data_ov217_021c3d44[question][i];
            todayCounts[q][i] = func_0200e4b0(survey, question, i + 1);
            pastCounts[q][i] = func_0200e4f8(survey, question, i + 1);
        }
    }

    wk->data.surveyId = surveyId;
    for (q = 0; q < RESEARCH_QUESTION_COUNT; q++) {
        wk->data.questions[q].id = questions[q];
        wk->data.questions[q].answerCount = answerCounts[q];
        wk->data.questions[q].todayCount = todayTotals[q];
        wk->data.questions[q].totalCount = todayTotals[q] + pastTotals[q];
        if (wk->data.questions[q].totalCount > 999999) {
            wk->data.questions[q].totalCount = 999999;
        }
        for (i = 0; i < RESEARCH_ANSWER_MAX; i++) {
            u16 id = answerIds[q][i];

            wk->data.questions[q].answers[i].id = id;
            wk->data.questions[q].answers[i].red = data_ov217_021c386c[id];
            wk->data.questions[q].answers[i].green = data_ov217_021c3900[id];
            wk->data.questions[q].answers[i].blue = data_ov217_021c3994[id];
            wk->data.questions[q].answers[i].todayCount = todayCounts[q][i];
            wk->data.questions[q].answers[i].totalCount = todayCounts[q][i] + pastCounts[q][i];
        }
    }
}


// pokebw2's ResearchGraph_InitTouchRects
static void func_ov217_021bfe40(ResearchGraph *wk) {
    int i;

    for (i = 0; i < TOUCH_RECT_COUNT; i++) {
        wk->touchRects[i].left = data_ov217_021c3758[i].left;
        wk->touchRects[i].right = data_ov217_021c3758[i].right;
        wk->touchRects[i].top = data_ov217_021c3758[i].top;
        wk->touchRects[i].bottom = data_ov217_021c3758[i].bottom;
    }
}

// pokebw2's ResearchGraph_ClearArrow
static void func_ov217_021bfe80(ResearchGraph *wk) {
    wk->arrow = NULL;
}

// pokebw2's ResearchGraph_CreateArrow
static void func_ov217_021bfe8c(ResearchGraph *wk) {
    ArrowResources resources;

    resources.chars = func_ov217_021bf9f0(wk, OBJ_RES_MAIN_CHARS);
    resources.palette = func_ov217_021bf9f0(wk, OBJ_RES_MAIN_PALETTE);
    resources.cellAnims = func_ov217_021bf9f0(wk, OBJ_RES_MAIN_CELL_ANIMS);
    resources.surface = 0;
    resources.sequences[0] = 0;
    resources.sequences[1] = 1;
    resources.sequences[2] = 2;
    resources.sequences[3] = 3;
    wk->arrow = func_ov217_021c2070(wk->heapId, &resources);
}

// pokebw2's ResearchGraph_DeleteArrow
static void func_ov217_021bfed4(ResearchGraph *wk) {
    func_ov217_021c209c(wk->arrow);
}

// pokebw2's ResearchGraph_ClearPercentages
static void func_ov217_021bfee4(ResearchGraph *wk) {
    int i;

    for (i = 0; i < PERCENTAGE_COUNT; i++) {
        wk->percentages[i] = NULL;
    }
}

// pokebw2's ResearchGraph_CreatePercentages
static void func_ov217_021bff00(ResearchGraph *wk) {
    int i;
    PercentageResources resources;

    resources.chars = func_ov217_021bf9f0(wk, OBJ_RES_MAIN_CHARS);
    resources.palette = func_ov217_021bf9f0(wk, OBJ_RES_MAIN_PALETTE);
    resources.cellAnims = func_ov217_021bf9f0(wk, OBJ_RES_MAIN_CELL_ANIMS);
    resources.surface = 0;
    resources.digitSequence = 12;
    resources.percentSequence = 13;
    for (i = 0; i < PERCENTAGE_COUNT; i++) {
        wk->percentages[i] = func_ov217_021c26e8(wk->heapId, &resources, func_ov217_021bf9fc(wk, UNIT_PERCENTAGES));
        func_ov217_021c2740(wk->percentages[i], FALSE);
    }
}

// pokebw2's ResearchGraph_DeletePercentages
static void func_ov217_021bff5c(ResearchGraph *wk) {
    int i;

    for (i = 0; i < PERCENTAGE_COUNT; i++) {
        func_ov217_021c270c(wk->percentages[i]);
    }
}

// pokebw2's ResearchGraph_Init3D
static void func_ov217_021bff78(void) {
    G3X_Init();
    G3X_InitMtxStack();
    G3_MtxMode(GX_MTXMODE_PROJECTION);
    G3_Identity();
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_Identity();
    G3X_AntiAlias(TRUE);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(data_ov217_021c365c);
}

// pokebw2's ResearchGraph_InitBG
static void func_ov217_021bffc8(ResearchGraph *wk) {
    GFL_BGSysSetLCDConfig(&data_ov217_021c366c);
    GFL_BGSysSet3DBGPriority(2);
    GFL_BGSysCreateBG(BG_SUB_TITLE, &data_ov217_021c36d8, BGMODE_TEXT);
    GFL_BGSysCreateBG(BG_SUB_TEXT, &data_ov217_021c3738, BGMODE_TEXT);
    GFL_BGSysCreateBG(BG_MAIN_GRAPH, &data_ov217_021c36f8, BGMODE_TEXT);
    GFL_BGSysCreateBG(BG_MAIN_TEXT, &data_ov217_021c3718, BGMODE_TEXT);
    GFL_BGSysSetBGEnabled(RESEARCH_BG_SUB_BACK, TRUE);
    GFL_BGSysSetBGEnabled(RESEARCH_BG_SUB_PATTERN, TRUE);
    GFL_BGSysSetBGEnabled(BG_SUB_TITLE, TRUE);
    GFL_BGSysSetBGEnabled(BG_SUB_TEXT, TRUE);
    GFL_BGSysSetBGEnabled(BG_MAIN_3D, TRUE);
    GFL_BGSysSetBGEnabled(RESEARCH_BG_MAIN_FRAME, TRUE);
    GFL_BGSysSetBGEnabled(BG_MAIN_GRAPH, TRUE);
    GFL_BGSysSetBGEnabled(BG_MAIN_TEXT, TRUE);
    G2S_SetBlendAlpha(GX_PLANEMASK_BG1, GX_PLANEMASK_BG0, 7, 15);
    G2_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG1, 16, 5);
    BmpWin_InitAllocator(wk->heapId);
}

// pokebw2's ResearchGraph_ExitBG
static void func_ov217_021c0084(ResearchGraph *wk) {
    BmpWin_FreeAllocator();
    GFL_BGSysReleaseBG(BG_MAIN_3D);
    GFL_BGSysReleaseBG(BG_MAIN_TEXT);
    GFL_BGSysReleaseBG(BG_MAIN_GRAPH);
    GFL_BGSysReleaseBG(BG_SUB_TEXT);
    GFL_BGSysReleaseBG(BG_SUB_TITLE);
}

// pokebw2's ResearchGraph_LoadSubBG
static void func_ov217_021c00ac(ResearchGraph *wk) {
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, wk->heapId);
    void *file = GFL_ArcToolReadHeapNew(handle, 13, wk->heapId);
    NNSG2dScreenData *screen;

    NNS_G2dGetUnpackedScreenData(file, &screen);
    GFL_BGSysLoadScrAreaAll(BG_SUB_TITLE, screen->rawData, 0, 0, 32, 24);
    GFL_BGSysLoadScr(BG_SUB_TITLE);
    GFL_HeapFree(file);
    GFL_ArcToolFree(handle);
}

// pokebw2's ResearchGraph_UnloadSubBG
static void func_ov217_021c00f8(ResearchGraph *wk) {
}

// pokebw2's ResearchGraph_InitSubText
static void func_ov217_021c00fc(ResearchGraph *wk) {
    GFL_BGSysFillChar(BG_SUB_TEXT, 0, 1, 0);
    GFL_BGSysClearScr(BG_SUB_TEXT);
}

// pokebw2's ResearchGraph_ExitSubText
static void func_ov217_021c0114(ResearchGraph *wk) {
    GFL_BGSysFreeFilledChar(BG_SUB_TEXT, 1, 0);
}

// pokebw2's ResearchGraph_LoadMainBG
static void func_ov217_021c0124(ResearchGraph *wk) {
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, wk->heapId);
    void *file = GFL_ArcToolReadHeapNew(handle, 7, wk->heapId);
    NNSG2dScreenData *screen;

    NNS_G2dGetUnpackedScreenData(file, &screen);
    GFL_BGSysLoadScrAreaAll(BG_MAIN_GRAPH, screen->rawData, 0, 0, 32, 32);
    GFL_BGSysLoadScr(BG_MAIN_GRAPH);
    GFL_HeapFree(file);
    GFL_ArcToolFree(handle);
}

// pokebw2's ResearchGraph_UnloadMainBG
static void func_ov217_021c0170(ResearchGraph *wk) {
}

// pokebw2's ResearchGraph_InitMainText
static void func_ov217_021c0174(ResearchGraph *wk) {
    GFL_BGSysFillChar(BG_MAIN_TEXT, 0, 1, 0);
    GFL_BGSysClearScr(BG_MAIN_TEXT);
}

// pokebw2's ResearchGraph_ExitMainText
static void func_ov217_021c018c(ResearchGraph *wk) {
    GFL_BGSysFreeFilledChar(BG_MAIN_TEXT, 1, 0);
}

// pokebw2's ResearchGraph_ClearBGFonts
static void func_ov217_021c019c(ResearchGraph *wk) {
    int i;

    for (i = 0; i < BG_FONT_COUNT; i++) {
        wk->bgFonts[i] = NULL;
    }
}

// pokebw2's ResearchGraph_CreateBGFonts
static void func_ov217_021c01b8(ResearchGraph *wk) {
    int i;

    for (i = 0; i < BG_FONT_COUNT; i++) {
        BGFontSetup setup;

        setup.window.bg = data_ov217_021c3b7c[i].window.bg;
        setup.window.x = data_ov217_021c3b7c[i].window.x;
        setup.window.y = data_ov217_021c3b7c[i].window.y;
        setup.window.width = data_ov217_021c3b7c[i].window.width;
        setup.window.height = data_ov217_021c3b7c[i].window.height;
        setup.window.textX = data_ov217_021c3b7c[i].window.textX;
        setup.window.textY = data_ov217_021c3b7c[i].window.textY;
        setup.window.palette = data_ov217_021c3b7c[i].window.palette;
        setup.window.letterColor = data_ov217_021c3b7c[i].window.letterColor;
        setup.window.shadowColor = data_ov217_021c3b7c[i].window.shadowColor;
        setup.window.backColor = data_ov217_021c3b7c[i].window.backColor;
        setup.centered = data_ov217_021c3b7c[i].centered;
        wk->bgFonts[i] = func_ov217_021c0ef4(&setup, wk->font, wk->msgData[data_ov217_021c3b7c[i].msgDataIndex], wk->heapId);
        func_ov217_021c0f5c(wk->bgFonts[i], data_ov217_021c3b7c[i].strId);
    }
}

// pokebw2's ResearchGraph_DeleteBGFonts
static void func_ov217_021c0230(ResearchGraph *wk) {
    int i;

    for (i = 0; i < BG_FONT_COUNT; i++) {
        func_ov217_021c0f48(wk->bgFonts[i]);
        wk->bgFonts[i] = NULL;
    }
}

// pokebw2's ResearchGraph_LoadSubObjRes
static void func_ov217_021c0250(ResearchGraph *wk) {
    HeapID heapId = wk->heapId;
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, heapId);
    u32 chars = func_0204a6c8(handle, 15, FALSE, CLACT_VRAM_SUB, heapId);
    u32 palette = func_0204abf0(handle, 16, CLACT_VRAM_SUB, 0, heapId);
    u32 cellAnims = func_0204af28(handle, 14, 17, heapId);

    wk->objRes[OBJ_RES_SUB_CHARS] = chars;
    wk->objRes[OBJ_RES_SUB_PALETTE] = palette;
    wk->objRes[OBJ_RES_SUB_CELL_ANIMS] = cellAnims;
    GFL_ArcToolFree(handle);
}

// pokebw2's ResearchGraph_FreeSubObjRes
static void func_ov217_021c02a8(ResearchGraph *wk) {
    func_0204a8d4(wk->objRes[OBJ_RES_SUB_CHARS]);
    func_0204ada4(wk->objRes[OBJ_RES_SUB_PALETTE]);
    func_0204afd8(wk->objRes[OBJ_RES_SUB_CELL_ANIMS]);
}

// pokebw2's ResearchGraph_LoadMainObjRes
static void func_ov217_021c02c8(ResearchGraph *wk) {
    HeapID heapId = wk->heapId;
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, heapId);
    u32 chars = func_0204a6c8(handle, 15, FALSE, CLACT_VRAM_MAIN, heapId);
    u32 palette = func_0204ac18(handle, 16, CLACT_VRAM_MAIN, 0xc0, 0, 3, heapId);
    u32 cellAnims = func_0204af28(handle, 14, 17, heapId);
    ArcTool *commonHandle;
    u32 buttonPalette;

    GFL_ArcToolFree(handle);

    commonHandle = GFL_ArcSysCreateFileHandle(ARCID_APP_MENU_COMMON, heapId);
    buttonPalette = func_0204ac18(commonHandle, 31, CLACT_VRAM_MAIN, 0x80, 0, 2, heapId);
    GFL_ArcToolFree(commonHandle);

    wk->objRes[OBJ_RES_MAIN_CHARS] = chars;
    wk->objRes[OBJ_RES_MAIN_PALETTE] = palette;
    wk->objRes[OBJ_RES_BUTTON_PALETTE] = buttonPalette;
    wk->objRes[OBJ_RES_MAIN_CELL_ANIMS] = cellAnims;
}

// pokebw2's ResearchGraph_FreeMainObjRes
static void func_ov217_021c0358(ResearchGraph *wk) {
    func_0204a8d4(wk->objRes[OBJ_RES_MAIN_CHARS]);
    func_0204ada4(wk->objRes[OBJ_RES_MAIN_PALETTE]);
    func_0204ada4(wk->objRes[OBJ_RES_BUTTON_PALETTE]);
    func_0204afd8(wk->objRes[OBJ_RES_MAIN_CELL_ANIMS]);
}

// pokebw2's ResearchGraph_ClearUnits
static void func_ov217_021c0384(ResearchGraph *wk) {
    int i;

    for (i = 0; i < UNIT_COUNT; i++) {
        wk->units[i] = NULL;
    }
}

// pokebw2's ResearchGraph_CreateUnits
static void func_ov217_021c03a0(ResearchGraph *wk) {
    int i;

    for (i = 0; i < UNIT_COUNT; i++) {
        wk->units[i] = func_0204b100(data_ov217_021c3620[i], data_ov217_021c3618[i], wk->heapId);
    }
}

// pokebw2's ResearchGraph_DeleteUnits
static void func_ov217_021c03d0(ResearchGraph *wk) {
    int i;

    for (i = 0; i < UNIT_COUNT; i++) {
        func_0204b1cc(wk->units[i]);
    }
}

// pokebw2's ResearchGraph_ClearActors
static void func_ov217_021c03ec(ResearchGraph *wk) {
    int i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        wk->actors[i] = NULL;
    }
}

// pokebw2's ResearchGraph_CreateActors
static void func_ov217_021c0408(ResearchGraph *wk) {
    int i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        const ResearchActorSetup *entry = &data_ov217_021c37fc[i];
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
        unit = func_ov217_021bf9fc(wk, entry->unit);
        chars = func_ov217_021bf9f0(wk, entry->chars);
        palette = func_ov217_021bf9f0(wk, entry->palette);
        cellAnims = func_ov217_021bf9f0(wk, entry->cellAnims);
        wk->actors[i] = func_0204b294(unit, chars, palette, cellAnims, &setup, entry->surface, wk->heapId);
        func_0204b3dc(wk->actors[i], FALSE);
    }
}

// pokebw2's ResearchGraph_DeleteActors
static void func_ov217_021c0494(ResearchGraph *wk) {
    int i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        func_0204b3b4(wk->actors[i]);
    }
}

// pokebw2's ResearchGraph_ClearBitmaps
static void func_ov217_021c04b0(ResearchGraph *wk) {
    int i;

    for (i = 0; i < BMP_COUNT; i++) {
        wk->bitmaps[i] = NULL;
    }
}

// pokebw2's ResearchGraph_CreateBitmaps
static void func_ov217_021c04cc(ResearchGraph *wk) {
    int i;

    for (i = 0; i < BMP_COUNT; i++) {
        wk->bitmaps[i] =
            GFL_BitmapCreate(data_ov217_021c37a4[i].width, data_ov217_021c37a4[i].height, data_ov217_021c37a4[i].tileSize, wk->heapId);
    }
}

// pokebw2's ResearchGraph_DrawBitmaps
static void func_ov217_021c04fc(ResearchGraph *wk) {
    int i;

    for (i = 0; i < BMP_COUNT; i++) {
        const BitmapEntry *entry = &data_ov217_021c37a4[i];

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

// Draws the button from the tiles of the apps' button, 3 by 3 of them for its corners, edges and middle
// pokebw2's ResearchGraph_DrawButton
static void func_ov217_021c05b0(ResearchGraph *wk) {
    int tiles[3][20] = {
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
        { 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5 },
        { 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8 },
    };
    const BitmapEntry *entry = &data_ov217_021c37a4[BMP_BUTTON];
    int x, y;
    GFLBitmap *base;
    u16 color;
    StrBuf *str;

    base = GFL_G2DIOLoadBitmap(entry->arcId, entry->fileId, FALSE, wk->heapId);
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 20; x++) {
            GFL_BitmapCopyArea(base, wk->bitmaps[BMP_BUTTON], tiles[y][x] % 3 * 8, tiles[y][x] / 3 * 8, x * 8, y * 8, 8,
                               8, 0);
        }
    }
    GFL_BitmapFree(base);

    color = PRINT_COLOR(entry->letterColor, entry->shadowColor, entry->backColor);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData[entry->msgDataIndex], entry->strId);
    GFL_TextRendererDrawToBitmapEx(wk->bitmaps[BMP_BUTTON], entry->textX, entry->textY, str, wk->font, color);
    GFL_HeapFree(str);
}

// pokebw2's ResearchGraph_FreeBitmaps
static void func_ov217_021c06ac(ResearchGraph *wk) {
    int i;

    for (i = 0; i < BMP_COUNT; i++) {
        GFL_BitmapFree(wk->bitmaps[i]);
    }
}

// pokebw2's ResearchGraph_CreateBmpOam
static void func_ov217_021c06c8(ResearchGraph *wk) {
    wk->bmpOamSys = func_02025538(wk->heapId, wk->units[UNIT_BMP_OAM]);
}

// pokebw2's ResearchGraph_DeleteBmpOam
static void func_ov217_021c06e0(ResearchGraph *wk) {
    func_02025588(wk->bmpOamSys);
}

// pokebw2's ResearchGraph_CreateBmpOamActors
static void func_ov217_021c06f0(ResearchGraph *wk) {
    int i;

    for (i = 0; i < BMP_COUNT; i++) {
        BmpOamActorSetup setup;

        setup.bitmap = wk->bitmaps[i];
        setup.x = data_ov217_021c377c[i].x;
        setup.y = data_ov217_021c377c[i].y;
        setup.palette = func_ov217_021bf9f0(wk, data_ov217_021c377c[i].palette);
        setup.paletteOffset = data_ov217_021c377c[i].palOffset;
        setup.priority = data_ov217_021c377c[i].priority;
        setup.bgPriority = data_ov217_021c377c[i].bgPriority;
        setup.surface = data_ov217_021c377c[i].surface;
        setup.vramType = data_ov217_021c377c[i].vramType;
        wk->bmpOamActors[i] = func_020255a0(wk->bmpOamSys, &setup);
        func_02025774(wk->bmpOamActors[i], FALSE);
        func_020257d0(wk->bmpOamActors[i]);
    }
}

// pokebw2's ResearchGraph_DeleteBmpOamActors
static void func_ov217_021c0794(ResearchGraph *wk) {
    int i;

    for (i = 0; i < BMP_COUNT; i++) {
        func_0202570c(wk->bmpOamActors[i]);
    }
}

// pokebw2's ResearchGraph_ClearPaletteFade
static void func_ov217_021c07b0(ResearchGraph *wk) {
    wk->paletteFade = NULL;
}

// pokebw2's ResearchGraph_CreatePaletteFade
static void func_ov217_021c07bc(ResearchGraph *wk) {
    wk->paletteFade = func_02021518(wk->heapId);
    func_0202155c(wk->paletteFade, PALFADE_BUFFER_MAIN_BG, 0x200, wk->heapId);
    func_0202155c(wk->paletteFade, PALFADE_BUFFER_MAIN_OBJ, 0x200, wk->heapId);
    func_020216d4(wk->paletteFade, PALFADE_VRAM_MAIN_BG, 0, 0x200);
    func_020216d4(wk->paletteFade, PALFADE_VRAM_MAIN_OBJ, 0, 0x200);
}

// pokebw2's ResearchGraph_DeletePaletteFade
static void func_ov217_021c0800(ResearchGraph *wk) {
    func_020215a0(wk->paletteFade, PALFADE_BUFFER_MAIN_BG);
    func_020215a0(wk->paletteFade, PALFADE_BUFFER_MAIN_OBJ);
    func_02021540(wk->paletteFade);
}

// pokebw2's ResearchGraph_ClearPaletteAnimes
static void func_ov217_021c0820(ResearchGraph *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        wk->animes[i] = NULL;
    }
}

// pokebw2's ResearchGraph_CreatePaletteAnimes
static void func_ov217_021c083c(ResearchGraph *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        wk->animes[i] = func_ov217_021c29c8(wk->heapId);
    }
}

// pokebw2's ResearchGraph_DeletePaletteAnimes
static void func_ov217_021c085c(ResearchGraph *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        func_ov217_021c29d8(wk->animes[i]);
    }
}

// pokebw2's ResearchGraph_SetupPaletteAnimes
static void func_ov217_021c0878(ResearchGraph *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        func_ov217_021c29e8(wk->animes[i], data_ov217_021c3a28[i].dst, data_ov217_021c3a28[i].src,
                           data_ov217_021c3a28[i].count);
    }
}

// pokebw2's ResearchGraph_RestorePalettes
static void func_ov217_021c08a8(ResearchGraph *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        func_ov217_021c2a08(wk->animes[i]);
    }
}

// pokebw2's ResearchGraph_ShowCommIcon
static void func_ov217_021c08c4(ResearchGraph *wk) {
    func_0203d34c(TRUE, wk->heapId);
}

// pokebw2's ResearchGraph_SetVBlank
static void func_ov217_021c08d4(ResearchGraph *wk) {
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov217_021bf6bc, wk, 0);
}

// pokebw2's ResearchGraph_ResetVBlank
static void func_ov217_021c08f0(ResearchGraph *wk) {
    GFL_TCBRemove(wk->vblankTask);
}

// pokebw2's ResearchGraph_ClampU8
static u8 func_ov217_021c0900(int value) {
    if (value < 0) {
        return 0;
    }
    if (value > 255) {
        return 255;
    }
    return value;
}

void func_ov217_021c0918(ResearchGraph *wk) {
    GameCommSys *comm = GSYS_GetGameCommSystem(func_ov217_021bf734(wk));

    func_02026498(comm, TRUE);
    if (GameCommSys_BootCheck(comm) == GAME_COMM_NO_INTRUDE) {
        func_02026400(comm);
    }
}

static BOOL func_ov217_021c093c(ResearchGraph *wk) {
    GameSystem *gsys = func_ov217_021bf734(wk);

    switch (GameCommSys_BootCheck(GSYS_GetGameCommSystem(gsys))) {
    case GAME_COMM_NO_NULL:
        func_020121cc(gsys);
        return TRUE;
    default:
        return TRUE;
    case GAME_COMM_NO_INTRUDE:
        return FALSE;
    }
}



// pokebw2's ResearchGraph_EndComm
static void func_ov217_021c0968(ResearchGraph *wk) {
    func_02026498(GSYS_GetGameCommSystem(func_ov217_021bf734(wk)), FALSE);
}
