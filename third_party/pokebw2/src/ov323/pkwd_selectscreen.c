#include "types.h"
#include "app/pkwd_selectscreen.h"
#include "app/pkwd_selectscreen_graphic.h"
#include "app/ui/touchbar.h"
#include "app/ui/ui_scene.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
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
#include "nitro/gx.h"
#include "nitro/hw.h"
#include "pml/poke_party.h"
#include "pml/species_names.h"
#include "save/player_info.h"
#include "save/pokewood.h"
#include "save/save_control.h"
#include "system/app_taskmenu.h"
#include "system/blink_palanm.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/pokewood_lines.h"

// Pokéstar Studios' movie select screen: the eight screens of the theater, four to a page, with the poster, the
// tagline and the release date of the movie under the cursor on the top screen. Picking a movie asks whether to watch
// it or, when the screen was opened to make room, to remove it; its details list the cast and the box office. Named
// after the ROM's "pkwd_selectscreen.c" (the other file of the overlay is pkwd_selectscreen_graphic.c)

#define SCREEN_COUNT 8
#define PAGE_SIZE 4
#define CAST_COUNT 6
#define MOVIE_COUNT 41
#define MENU_ITEM_MAX 3
#define MARK_COUNT 1

// The poster and the frame color of a screen with no movie
#define MOVIE_NONE (-1)
#define FRAME_COLOR_EMPTY 4

#define CURSOR_NONE (-1)

// The files of the poster of a screen with no movie
#define POSTER_PALETTE_FILE_NONE 49
#define POSTER_CHARS_FILE_NONE 63
#define POSTER_SCREEN_FILE_NONE 78

enum {
    WINDOW_SCREEN,
    WINDOW_TITLE = PAGE_SIZE,
    WINDOW_TAGLINE,
    WINDOW_RELEASE,
    // The windows that stay
    WINDOW_STATIC_COUNT,
    WINDOW_MESSAGE = WINDOW_STATIC_COUNT,
    WINDOW_DETAILS,
    WINDOW_COUNT,
};

enum {
    MENU_WATCH,
    MENU_REMOVE,
    MENU_YES_NO,
};

enum {
    STATE_FADE_IN,
    STATE_WAIT_FADE_IN,
    STATE_START_INPUT,
    STATE_INPUT,
    STATE_WAIT_BLINK,
    STATE_WAIT_MESSAGE,
    STATE_ASK_REMOVE_SURE,
    STATE_OPEN_WATCH_MENU,
    STATE_WATCH_MENU,
    STATE_OPEN_REMOVE_MENU,
    STATE_REMOVE_MENU,
    STATE_OPEN_YES_NO_MENU,
    STATE_YES_NO_MENU,
    STATE_FADE_OUT,
    STATE_WAIT_FADE_OUT,
    STATE_OPEN_DETAILS,
    STATE_DETAILS,
};

typedef struct {
    u32 unk0;
} PkwdSelectScreenBG;

// The resources of a group of cell actors
typedef struct {
    u32 chars;
    u32 palette;
    u32 cellAnims;
} PkwdSelectScreenActorRes;

// A screen of the theater and its movie: the fields of the downloaded movie in its slot
typedef struct {
    u32 screen;
    // Pokewood_LoadDownloadedMovie's result, 1 when the slot has a movie
    u32 loaded;
    s32 movie;
    // In tenths of a billion
    s32 gross;
    u32 rank;
    // The year since 2000, the month and the day in the three high bytes
    u32 date;
    // The hour and the minute in the two middle bytes
    u32 time;
    u16 cast[CAST_COUNT];
    WordSet *wordSet;
    u32 watched;
} PkwdSelectScreenMovie;

typedef struct {
    u16 heapId;
    PkwdSelectScreenBG bg;
    PkwdSelectScreenParam *param;
    StrBuf *strbuf;
    PkwdSelectScreenGraphic *graphic;
    TouchBar *touchBar;
    PkwdSelectScreenActorRes touchBarRes;
    Font *font;
    WordSet *wordSet;
    PrintQueue *printQueue;
    PrintStream *stream;
    MsgData *msgData;
    TCBExManager *tcbManager;
    u32 state;
    u32 nextState;
    TCB *vblankTask;
    // Set to have the VBlank load the poster of the movie under the cursor
    BOOL posterDirty;
    AppTaskMenuRes *menuRes;
    AppTaskMenu *menu;
    PkwdSelectScreenActorRes newRes;
    // The marks of the screens whose movie has not been watched
    ClActor *newMarks[PAGE_SIZE];
    PkwdSelectScreenActorRes markRes;
    // The upper screen's mark of the movie's rank
    ClActor *marks[MARK_COUNT];
    PrintWindow windows[WINDOW_COUNT];
    s16 cursor;
    s16 lastCursor;
    // Whether the cursor was put by a touch, which the keys take back to the first screen
    BOOL touched;
    BlinkPalAnm *blinks[PAGE_SIZE];
    u32 page;
    u32 unkE4;
    PkwdSelectScreenMovie movies[SCREEN_COUNT];
    // The screens with a movie first
    PkwdSelectScreenMovie *sorted[SCREEN_COUNT];
} PkwdSelectScreenWork;

// A file of a poster: where it goes, the file, and as in pkwd_count.c the offset and the size or palette
typedef struct {
    u8 target;
    u8 fileId;
    u8 offset;
    u8 extra;
} PkwdSelectScreenLoad;

typedef struct {
    u8 x;
    u8 y;
} PkwdSelectScreenPos;

typedef struct {
    u32 x;
    u32 y;
} PkwdSelectScreenTextPos;

typedef struct {
    u16 bg;
    u16 palette;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} PkwdSelectScreenWindowSetup;

// A menu: the messages and types of its items, and its width in tiles
typedef struct {
    u32 *messages;
    s32 count;
    u32 width;
    u32 types[4];
} PkwdSelectScreenMenu;

typedef BOOL (*PkwdSelectScreenStateFunc)(PkwdSelectScreenWork *wk);

static BOOL PkwdSelectScreen_Init(GameProc *proc, u32 *state, void *param, void *work);
static int PkwdSelectScreen_GetScreen(PkwdSelectScreenWork *wk, s32 cursor);
static void PkwdSelectScreen_CreateBlinks(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_FreeBlinks(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_Exit(GameProc *proc, u32 *state, void *param, void *work);
static BOOL PkwdSelectScreen_Main(GameProc *proc, u32 *state, void *param, void *work);
static void PkwdSelectScreen_VBlank(TCB *tcb, void *data);
static void PkwdSelectScreenBG_Load(PkwdSelectScreenBG *bg, HeapID heapId);
static void PkwdSelectScreenBG_LoadPoster(PkwdSelectScreenBG *bg, s32 movie, s32 cursor);
static TouchBar *PkwdSelectScreen_CreateTouchBar(PkwdSelectScreenWork *wk, ClActUnit *unit, HeapID heapId);
static void PkwdSelectScreen_FreeTouchBar(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_UpdateTouchBar(TouchBar *touchBar);
static AppTaskMenu *PkwdSelectScreen_CreateMenu(AppTaskMenuRes *res, MsgData *msgData, HeapID heapId, u32 kind);
static void PkwdSelectScreen_FreeMenu(AppTaskMenu *menu);
static void PkwdSelectScreen_UpdateMenu(AppTaskMenu *menu);
static void PkwdSelectScreen_LoadNewMarkRes(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_FreeNewMarkRes(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_CreateNewMarks(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_ShowNewMarks(PkwdSelectScreenWork *wk, u32 page);
static void PkwdSelectScreen_FreeNewMarks(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_LoadMarkRes(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_FreeMarkRes(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_CreateMarks(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_FreeMarks(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_SetMarkVisible(PkwdSelectScreenWork *wk, u32 mark, BOOL visible);
static void PkwdSelectScreen_SetMarkSequence(PkwdSelectScreenWork *wk, u32 mark, s32 sequence);
static BOOL PkwdSelectScreen_FlushWindows(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_CreateWindows(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_FreeWindows(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_ShowWindow(PkwdSelectScreenWork *wk, u32 window);
static void PkwdSelectScreen_LoadMovies(PkwdSelectScreenWork *wk, PkwdSelectScreenParam *param);
static s32 PkwdSelectScreen_ReadTouch(PkwdSelectScreenWork *wk);
static s32 PkwdSelectScreen_ReadKeys(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_InitText(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_FreeText(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_PrintScreens(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_PrintCentered(PrintWindow *window, PrintQueue *queue, StrBuf *strbuf, s32 y, Font *font,
                                           u16 color);
static void PkwdSelectScreen_PrintMovie(PkwdSelectScreenWork *wk, s32 screen);
static void PkwdSelectScreen_SetBlink(PkwdSelectScreenWork *wk, s32 cursor);
static void PkwdSelectScreen_UpdateCursor(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_PrintMessage(PkwdSelectScreenWork *wk, u32 messageId);
static void PkwdSelectScreen_PrintDetails(PkwdSelectScreenWork *wk, int screen);
static BOOL PkwdSelectScreen_RunState(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_FadeIn(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_WaitFadeIn(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_StartInput(PkwdSelectScreenWork *wk);
static s32 PkwdSelectScreen_GetLastCursor(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_Input(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_WaitBlink(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_WaitMessage(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_AskRemoveSure(PkwdSelectScreenWork *wk);
static s32 PkwdSelectScreen_GetMenuResult(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_OpenWatchMenu(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_WatchMenu(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_OpenRemoveMenu(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_RemoveMenu(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_OpenYesNoMenu(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_YesNoMenu(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_FadeOut(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_WaitFadeOut(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_OpenDetails(PkwdSelectScreenWork *wk);
static BOOL PkwdSelectScreen_Details(PkwdSelectScreenWork *wk);
static void PkwdSelectScreen_ShowPageIcons(PkwdSelectScreenWork *wk, BOOL visible);

static u32 sPkwdSelectScreenYesNoMessages[2] = { PokewoodLines_Text_151, PokewoodLines_Text_152 };

static u32 sPkwdSelectScreenWatchMessages[3] = {
    PokewoodLines_Text_154,
    PokewoodLines_Text_153,
    PokewoodLines_Text_155,
};

static u32 sPkwdSelectScreenRemoveMessages[3] = {
    PokewoodLines_Text_156,
    PokewoodLines_Text_153,
    PokewoodLines_Text_155,
};

// clang-format off
static PkwdSelectScreenStateFunc sPkwdSelectScreenStates[] = {
    PkwdSelectScreen_FadeIn,        PkwdSelectScreen_WaitFadeIn,     PkwdSelectScreen_StartInput,
    PkwdSelectScreen_Input,         PkwdSelectScreen_WaitBlink,      PkwdSelectScreen_WaitMessage,
    PkwdSelectScreen_AskRemoveSure, PkwdSelectScreen_OpenWatchMenu,  PkwdSelectScreen_WatchMenu,
    PkwdSelectScreen_OpenRemoveMenu, PkwdSelectScreen_RemoveMenu,    PkwdSelectScreen_OpenYesNoMenu,
    PkwdSelectScreen_YesNoMenu,     PkwdSelectScreen_FadeOut,        PkwdSelectScreen_WaitFadeOut,
    PkwdSelectScreen_OpenDetails,   PkwdSelectScreen_Details,
};

// clang-format on

// Where the upper screen's rank mark goes
static const PkwdSelectScreenPos sPkwdSelectScreenMarkPos[MARK_COUNT] = { { 240, 152 } };

// The rank mark's animation for each rank
static const s16 sPkwdSelectScreenRankSequences[4] = { 11, 10, 12, 12 };

// The messages of the release date: the date, the time and, twice, the heading. The reads are folded, so the table
// is only in the ROM's data
static const u16 sPkwdSelectScreenReleaseMessages[4] = {
    PokewoodLines_Text_173,
    PokewoodLines_Text_174,
    PokewoodLines_Text_175,
    PokewoodLines_Text_175,
};

static const PkwdSelectScreenPos sPkwdSelectScreenNewMarkPos[PAGE_SIZE] = {
    { 14, 20 },
    { 14, 60 },
    { 14, 100 },
    { 14, 140 },
};

static const TouchRect sPkwdSelectScreenTouchRects[PAGE_SIZE + 1] = {
#include "asset/pkwd_selectscreen_sPkwdSelectScreenTouchRects.inc"
};

// The color of each movie's frame in the list, by its series
static const u8 sPkwdSelectScreenFrameColors[MOVIE_COUNT] = {
#include "asset/pkwd_selectscreen_sPkwdSelectScreenFrameColors.inc"
};

// The graphics of each movie's poster
static const u8 sPkwdSelectScreenPosterGraphics[MOVIE_COUNT] = {
#include "asset/pkwd_selectscreen_sPkwdSelectScreenPosterGraphics.inc"
};

// Where the details put the names of the cast
static const PkwdSelectScreenTextPos sPkwdSelectScreenCastPos[CAST_COUNT] = {
#include "asset/pkwd_selectscreen_sPkwdSelectScreenCastPos.inc"
};

static const PkwdSelectScreenLoad sPkwdSelectScreenPosterChars[14] = {
#include "asset/pkwd_selectscreen_PosterChars.inc"
};

static const PkwdSelectScreenLoad sPkwdSelectScreenPosterPalettes[14] = {
#include "asset/pkwd_selectscreen_PosterPalettes.inc"
};

static const PkwdSelectScreenLoad sPkwdSelectScreenPosterScreens[14] = {
#include "asset/pkwd_selectscreen_PosterScreens.inc"
};

const GameProcFunctions PKWD_SELECTSCREEN_PROC_FUNCTIONS = {
    PkwdSelectScreen_Init,
    PkwdSelectScreen_Main,
    PkwdSelectScreen_Exit,
};

static const PkwdSelectScreenWindowSetup sPkwdSelectScreenWindows[WINDOW_COUNT] = {
#include "asset/pkwd_selectscreen_sPkwdSelectScreenWindows.inc"
};

static const PkwdSelectScreenMenu sPkwdSelectScreenMenus[3] = {
#include "asset/pkwd_selectscreen_sPkwdSelectScreenMenus.inc"
};

// The lines that the upper screen shows under each movie's title for a boy and a girl, as in pkwd_count.c
static const s16 sPkwdSelectScreenTaglines[MOVIE_COUNT][2] = {
    { PokewoodLines_Text_177, PokewoodLines_Text_218 },
    { PokewoodLines_Text_178, PokewoodLines_Text_219 },
    { PokewoodLines_Text_179, PokewoodLines_Text_220 },
    { PokewoodLines_Text_180, PokewoodLines_Text_221 },
    { PokewoodLines_Text_181, PokewoodLines_Text_222 },
    { PokewoodLines_Text_182, PokewoodLines_Text_223 },
    { PokewoodLines_Text_183, PokewoodLines_Text_224 },
    { PokewoodLines_Text_184, PokewoodLines_Text_225 },
    { PokewoodLines_Text_185, PokewoodLines_Text_226 },
    { PokewoodLines_Text_186, PokewoodLines_Text_227 },
    { PokewoodLines_Text_187, PokewoodLines_Text_228 },
    { PokewoodLines_Text_188, PokewoodLines_Text_229 },
    { PokewoodLines_Text_189, PokewoodLines_Text_230 },
    { PokewoodLines_Text_190, PokewoodLines_Text_231 },
    { PokewoodLines_Text_191, PokewoodLines_Text_232 },
    { PokewoodLines_Text_192, PokewoodLines_Text_233 },
    { PokewoodLines_Text_193, PokewoodLines_Text_234 },
    { PokewoodLines_Text_194, PokewoodLines_Text_235 },
    { PokewoodLines_Text_195, PokewoodLines_Text_236 },
    { PokewoodLines_Text_196, PokewoodLines_Text_237 },
    { PokewoodLines_Text_197, PokewoodLines_Text_238 },
    { PokewoodLines_Text_198, PokewoodLines_Text_239 },
    { PokewoodLines_Text_199, PokewoodLines_Text_240 },
    { PokewoodLines_Text_200, PokewoodLines_Text_241 },
    { PokewoodLines_Text_201, PokewoodLines_Text_242 },
    { PokewoodLines_Text_202, PokewoodLines_Text_243 },
    { PokewoodLines_Text_203, PokewoodLines_Text_244 },
    { PokewoodLines_Text_204, PokewoodLines_Text_245 },
    { PokewoodLines_Text_205, PokewoodLines_Text_246 },
    { PokewoodLines_Text_206, PokewoodLines_Text_247 },
    { PokewoodLines_Text_207, PokewoodLines_Text_248 },
    { PokewoodLines_Text_208, PokewoodLines_Text_249 },
    { PokewoodLines_Text_209, PokewoodLines_Text_250 },
    { PokewoodLines_Text_210, PokewoodLines_Text_251 },
    { PokewoodLines_Text_211, PokewoodLines_Text_252 },
    { PokewoodLines_Text_212, PokewoodLines_Text_253 },
    { PokewoodLines_Text_213, PokewoodLines_Text_254 },
    { PokewoodLines_Text_214, PokewoodLines_Text_255 },
    { PokewoodLines_Text_215, PokewoodLines_Text_256 },
    { PokewoodLines_Text_216, PokewoodLines_Text_257 },
    { PokewoodLines_Text_217, PokewoodLines_Text_258 },
};

static BOOL PkwdSelectScreen_Init(GameProc *proc, u32 *state, void *param, void *work) {
    PkwdSelectScreenWork *wk;

    GFL_OvlLoad(OVERLAY_APP_UI);
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_PKWD_SELECTSCREEN, 0x60000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(PkwdSelectScreenWork), HEAPID_PKWD_SELECTSCREEN);
    MI_CpuFill8(wk, 0, sizeof(PkwdSelectScreenWork));
    wk->heapId = HEAPID_PKWD_SELECTSCREEN;
    wk->param = param;
    wk->graphic = PkwdSelectScreenGraphic_Create(0, wk->heapId);
    PkwdSelectScreen_InitText(wk);
    PkwdSelectScreenBG_Load(&wk->bg, wk->heapId);
    wk->touchBar = PkwdSelectScreen_CreateTouchBar(wk, PkwdSelectScreenGraphic_GetClActUnit(wk->graphic), wk->heapId);
    TouchBar_SetBgPriority(wk->touchBar, 1);
    wk->menuRes = AppTaskMenuRes_Create(0, 11, wk->font, wk->printQueue, wk->heapId);
    PkwdSelectScreen_LoadNewMarkRes(wk);
    PkwdSelectScreen_LoadMarkRes(wk);
    PkwdSelectScreen_LoadMovies(wk, param);
    PkwdSelectScreen_CreateWindows(wk);
    PkwdSelectScreen_CreateNewMarks(wk);
    PkwdSelectScreen_ShowNewMarks(wk, wk->page);
    PkwdSelectScreen_CreateMarks(wk);
    PkwdSelectScreen_ShowPageIcons(wk, TRUE);
    PkwdSelectScreen_PrintScreens(wk);
    PkwdSelectScreen_CreateBlinks(wk);
    wk->cursor = 0;
    if (GCTX_HIDIsUsingTouch() == TRUE) {
        wk->cursor = CURSOR_NONE;
        wk->touched = TRUE;
        PkwdSelectScreen_SetMarkVisible(wk, 0, FALSE);
    } else {
        PkwdSelectScreen_PrintMovie(wk, PkwdSelectScreen_GetScreen(wk, wk->cursor));
        wk->touched = FALSE;
    }
    wk->state = STATE_FADE_IN;
    wk->vblankTask = GFL_VBlankTCBAdd(PkwdSelectScreen_VBlank, wk, 1);
    GFL_BGSysMoveBG(6, BG_MOVE_SET_Y, 16);
    GFL_NetSetIconScreen(FALSE, HEAPID_PKWD_SELECTSCREEN);
    return TRUE;
}

// The screen of the theater at a row of the page
static int PkwdSelectScreen_GetScreen(PkwdSelectScreenWork *wk, s32 cursor) {
    return cursor + wk->page * PAGE_SIZE;
}

// Creates the blinking frames of the page's rows, in the colors of their movies
static void PkwdSelectScreen_CreateBlinks(PkwdSelectScreenWork *wk) {
    s32 i;
    u32 color;

    for (i = 0; i < PAGE_SIZE; i++) {
        PkwdSelectScreenMovie *movie = wk->sorted[i + wk->page * PAGE_SIZE];

        if (movie->loaded) {
            color = sPkwdSelectScreenFrameColors[movie->movie];
        } else {
            color = FRAME_COLOR_EMPTY;
        }
        wk->blinks[i] = BlinkPalAnm_Create(i * 16, 16, 3, HEAPID_PKWD_SELECTSCREEN);
        BlinkPalAnm_SetPalBufferArc(wk->blinks[i], ARCID_PKWD_COUNT, 6, color * 16, (color + 5) * 16);
        BlinkPalAnm_SetAnimeCount(wk->blinks[i], 0x4000);
        BlinkPalAnm_Main(wk->blinks[i]);
    }
}

static void PkwdSelectScreen_FreeBlinks(PkwdSelectScreenWork *wk) {
    s32 i;

    for (i = 0; i < PAGE_SIZE; i++) {
        BlinkPalAnm_Free(wk->blinks[i]);
    }
}

static BOOL PkwdSelectScreen_Exit(GameProc *proc, u32 *state, void *param, void *work) {
    PkwdSelectScreenWork *wk = work;
    HeapID heapId;

    GFL_TCBRemove(wk->vblankTask);
    PkwdSelectScreen_FreeWindows(wk);
    PkwdSelectScreen_FreeBlinks(wk);
    PkwdSelectScreen_FreeNewMarks(wk);
    PkwdSelectScreen_FreeNewMarkRes(wk);
    PkwdSelectScreen_FreeMarks(wk);
    PkwdSelectScreen_FreeMarkRes(wk);
    AppTaskMenuRes_Free(wk->menuRes);
    PkwdSelectScreen_FreeTouchBar(wk);
    GFL_StrBufFree(wk->strbuf);
    PkwdSelectScreen_FreeText(wk);
    PkwdSelectScreenGraphic_Free(wk->graphic);
    heapId = wk->heapId;
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(heapId);
    GFL_OvlUnload(OVERLAY_APP_UI);
    return TRUE;
}

static BOOL PkwdSelectScreen_Main(GameProc *proc, u32 *state, void *param, void *work) {
    PkwdSelectScreenWork *wk = work;

    if (PkwdSelectScreen_RunState(wk)) {
        return TRUE;
    }
    PkwdSelectScreen_UpdateTouchBar(wk->touchBar);
    if (wk->menu != NULL) {
        PkwdSelectScreen_UpdateMenu(wk->menu);
    }
    if (wk->tcbManager != NULL) {
        GFL_TCBExMgrUpdate(wk->tcbManager);
    }
    PrintQueue_Main(wk->printQueue);
    PkwdSelectScreen_FlushWindows(wk);
    if (wk->cursor >= 0) {
        BlinkPalAnm_Main(wk->blinks[wk->cursor]);
    }
    PkwdSelectScreenGraphic_Update(wk->graphic);
    return FALSE;
}

static void PkwdSelectScreen_VBlank(TCB *tcb, void *data) {
    PkwdSelectScreenWork *wk = data;

    if (wk->posterDirty) {
        s32 screen = PkwdSelectScreen_GetScreen(wk, wk->cursor);

        if (screen < 0) {
            PkwdSelectScreenBG_LoadPoster(&wk->bg, MOVIE_NONE, CURSOR_NONE);
        } else {
            PkwdSelectScreenBG_LoadPoster(&wk->bg, wk->sorted[screen]->movie, wk->cursor);
        }
        wk->posterDirty = FALSE;
    }
}

static void PkwdSelectScreenBG_Load(PkwdSelectScreenBG *bg, HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_PKWD_COUNT, heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, 6, 0, 0, 11 * 0x20, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 7, 4, 0, 15 * 0x20, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 16, 5, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 31, 5, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 15, 3, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 29, 3, 0, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
}

// Loads the poster of a movie to the upper screen while it is blacked out, or the empty one
static void PkwdSelectScreenBG_LoadPoster(PkwdSelectScreenBG *bg, s32 movie, s32 cursor) {
    ArcTool *arc;

    G2x_SetBlendBrightness_(REG_DB_BLDCNT_ADDR, GX_BLEND_PLANEMASK_BG2, 16);
    arc = GFL_ArcSysCreateFileHandle(ARCID_PKWD_COUNT, HEAPID_PKWD_SELECTSCREEN);
    if (cursor == CURSOR_NONE || movie == MOVIE_NONE) {
        GFL_G2DIOLoadArcNCLRDefault(arc, POSTER_PALETTE_FILE_NONE, 4, 0, 0x20, HEAPID_PKWD_SELECTSCREEN);
        loadBGScrToVramByFileNoReserveNegAlign(arc, POSTER_SCREEN_FILE_NONE, 6, 0, 0, FALSE, HEAPID_PKWD_SELECTSCREEN);
        GFL_BGSysLoadArcNCGRStatic(arc, POSTER_CHARS_FILE_NONE, 6, 0, 0, FALSE, HEAPID_PKWD_SELECTSCREEN);
    } else {
        u32 graphics;

        GFL_BGSysFillScr(6, 1);
        GFL_BGSysLoadScr(6);
        graphics = sPkwdSelectScreenPosterGraphics[movie];
        GFL_G2DIOLoadArcNCLRDefault(arc, sPkwdSelectScreenPosterPalettes[graphics].fileId, 4, 0, 0x20,
                                    HEAPID_PKWD_SELECTSCREEN);
        GFL_BGSysLoadArcNCGRStatic(arc, sPkwdSelectScreenPosterChars[graphics].fileId, 6, 0, 0, FALSE,
                                   HEAPID_PKWD_SELECTSCREEN);
        loadBGScrToVramByFileNoReserveNegAlign(arc, sPkwdSelectScreenPosterScreens[graphics].fileId, 6, 0, 0, FALSE,
                                               HEAPID_PKWD_SELECTSCREEN);
    }
    GFL_ArcToolFree(arc);
    G2S_BlendNone();
}

// Creates the bar at the bottom with its page arrows and its return button
static TouchBar *PkwdSelectScreen_CreateTouchBar(PkwdSelectScreenWork *wk, ClActUnit *unit, HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_TOWNMAP_GRA, heapId);

    wk->touchBarRes.chars = ClActSys_LoadChars(arc, 8, FALSE, 0, heapId);
    wk->touchBarRes.palette = ClActSys_LoadPlttRange(arc, 0, 0, 14 * 0x20, 2, 2, heapId);
    wk->touchBarRes.cellAnims = ClActSys_LoadCellAnims(arc, 12, 19, heapId);
    GFL_ArcToolFree(arc);
    {
        TouchBarItem items[3] = {
            { TOUCHBAR_ICON_LEFT, { 168, 168 } },
            { TOUCHBAR_ICON_RIGHT, { 200, 168 } },
            { TOUCHBAR_ICON_RETURN, { 232, 168 } },
        };
        TouchBarSetup setup;

        MI_CpuFill8(&setup, 0, sizeof(TouchBarSetup));
        setup.items = items;
        setup.count = NELEMS(items);
        setup.unit = unit;
        setup.bg = 1;
        setup.bgPalette = 13;
        setup.objPalette = 0;
        setup.vramType = 2;
        return TouchBar_Create(&setup, heapId);
    }
}

static void PkwdSelectScreen_FreeTouchBar(PkwdSelectScreenWork *wk) {
    TouchBar_Free(wk->touchBar);
    ClActSys_FreePltt(wk->touchBarRes.palette);
    ClActSys_FreeChars(wk->touchBarRes.chars);
    ClActSys_FreeCellAnims(wk->touchBarRes.cellAnims);
}

static void PkwdSelectScreen_UpdateTouchBar(TouchBar *touchBar) {
    TouchBar_Main(touchBar);
}

// Creates a menu at the bottom right
static AppTaskMenu *PkwdSelectScreen_CreateMenu(AppTaskMenuRes *res, MsgData *msgData, HeapID heapId, u32 kind) {
    AppTaskMenuInit init;
    AppTaskMenuItem items[MENU_ITEM_MAX];
    const PkwdSelectScreenMenu *setup = &sPkwdSelectScreenMenus[kind];
    AppTaskMenu *menu;
    s32 i;

    for (i = 0; i < setup->count; i++) {
        items[i].str = GFL_MsgDataLoadStrbufNew(msgData, setup->messages[i]);
        items[i].color = PRINT_COLOR(14, 15, 3);
        items[i].type = setup->types[i];
    }
    init.heapId = heapId;
    init.itemCount = setup->count;
    init.items = items;
    init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    init.x = 32;
    init.y = 23;
    init.width = setup->width;
    init.height = 3;
    menu = AppTaskMenu_Create(&init, res);
    for (i = 0; i < setup->count; i++) {
        GFL_StrBufFree(items[i].str);
    }
    return menu;
}

static void PkwdSelectScreen_FreeMenu(AppTaskMenu *menu) {
    AppTaskMenu_Free(menu);
}

static void PkwdSelectScreen_UpdateMenu(AppTaskMenu *menu) {
    AppTaskMenu_Update(menu);
}

static void PkwdSelectScreen_LoadNewMarkRes(PkwdSelectScreenWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_PKWD, HEAPID_PKWD_SELECTSCREEN);

    wk->newRes.chars = ClActSys_LoadChars(arc, 21, FALSE, 0, HEAPID_PKWD_SELECTSCREEN);
    wk->newRes.palette = ClActSys_LoadPlttRange(arc, 2, 0, 5 * 0x20, 0, 1, HEAPID_PKWD_SELECTSCREEN);
    wk->newRes.cellAnims = ClActSys_LoadCellAnims(arc, 27, 39, HEAPID_PKWD_SELECTSCREEN);
    GFL_ArcToolFree(arc);
}

static void PkwdSelectScreen_FreeNewMarkRes(PkwdSelectScreenWork *wk) {
    ClActSys_FreePltt(wk->newRes.palette);
    ClActSys_FreeChars(wk->newRes.chars);
    ClActSys_FreeCellAnims(wk->newRes.cellAnims);
}

static void PkwdSelectScreen_CreateNewMarks(PkwdSelectScreenWork *wk) {
    ClActorSetup setup;
    s32 i;

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    for (i = 0; i < PAGE_SIZE; i++) {
        setup.x = sPkwdSelectScreenNewMarkPos[i].x;
        setup.y = sPkwdSelectScreenNewMarkPos[i].y;
        setup.sequence = 1;
        setup.bgPriority = 2;
        wk->newMarks[i] = ClActor_Create(PkwdSelectScreenGraphic_GetClActUnit(wk->graphic), wk->newRes.chars,
                                         wk->newRes.palette, wk->newRes.cellAnims, &setup, 0, HEAPID_PKWD_SELECTSCREEN);
        ClActor_SetAutoAnim(wk->newMarks[i], TRUE);
    }
}

// Shows the marks of the page's movies that have not been watched
static void PkwdSelectScreen_ShowNewMarks(PkwdSelectScreenWork *wk, u32 page) {
    s32 i;

    for (i = 0; i < PAGE_SIZE; i++) {
        s32 screen = i + page * PAGE_SIZE;

        if (!PokewoodSave_IsSlotWatched(wk->param->save, screen)) {
            ClActor_SetVisible(wk->newMarks[i], wk->sorted[screen]->loaded);
        } else {
            ClActor_SetVisible(wk->newMarks[i], FALSE);
        }
    }
}

static void PkwdSelectScreen_FreeNewMarks(PkwdSelectScreenWork *wk) {
    s32 i;

    for (i = 0; i < PAGE_SIZE; i++) {
        ClActor_Delete(wk->newMarks[i]);
    }
}

static void PkwdSelectScreen_LoadMarkRes(PkwdSelectScreenWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_PKWD, HEAPID_PKWD_SELECTSCREEN);

    wk->markRes.chars = ClActSys_LoadChars(arc, 21, FALSE, 1, HEAPID_PKWD_SELECTSCREEN);
    wk->markRes.palette = ClActSys_LoadPlttRange(arc, 2, 1, 0, 0, 3, HEAPID_PKWD_SELECTSCREEN);
    wk->markRes.cellAnims = ClActSys_LoadCellAnims(arc, 27, 39, HEAPID_PKWD_SELECTSCREEN);
    GFL_ArcToolFree(arc);
}

static void PkwdSelectScreen_FreeMarkRes(PkwdSelectScreenWork *wk) {
    ClActSys_FreePltt(wk->markRes.palette);
    ClActSys_FreeChars(wk->markRes.chars);
    ClActSys_FreeCellAnims(wk->markRes.cellAnims);
}

static void PkwdSelectScreen_CreateMarks(PkwdSelectScreenWork *wk) {
    ClActorSetup setup;
    s32 i;

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    for (i = 0; i < MARK_COUNT; i++) {
        setup.x = sPkwdSelectScreenMarkPos[i].x;
        setup.y = sPkwdSelectScreenMarkPos[i].y;
        setup.sequence = 10;
        setup.bgPriority = 1;
        wk->marks[i] = ClActor_Create(PkwdSelectScreenGraphic_GetClActUnit(wk->graphic), wk->markRes.chars,
                                      wk->markRes.palette, wk->markRes.cellAnims, &setup, 1, HEAPID_PKWD_SELECTSCREEN);
        ClActor_SetAutoAnim(wk->marks[i], TRUE);
    }
}

static void PkwdSelectScreen_FreeMarks(PkwdSelectScreenWork *wk) {
    s32 i;

    for (i = 0; i < MARK_COUNT; i++) {
        ClActor_Delete(wk->marks[i]);
    }
}

static void PkwdSelectScreen_SetMarkVisible(PkwdSelectScreenWork *wk, u32 mark, BOOL visible) {
    ClActor_SetVisible(wk->marks[mark], visible);
}

static void PkwdSelectScreen_SetMarkSequence(PkwdSelectScreenWork *wk, u32 mark, s32 sequence) {
    ClActor_SetSequence(wk->marks[mark], sequence);
    ClActor_SetAutoAnim(wk->marks[mark], TRUE);
}

// Sends the windows whose text is printed to VRAM
static BOOL PkwdSelectScreen_FlushWindows(PkwdSelectScreenWork *wk) {
    s32 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        PrintWindow_Flush(&wk->windows[i], wk->printQueue);
    }
    return TRUE;
}

static void PkwdSelectScreen_CreateWindows(PkwdSelectScreenWork *wk) {
    s32 i;

    for (i = 0; i < WINDOW_STATIC_COUNT; i++) {
        const PkwdSelectScreenWindowSetup *setup = &sPkwdSelectScreenWindows[i];

        wk->windows[i].window =
            BmpWin_CreateDynamic(setup->bg, setup->x, setup->y, setup->width, setup->height, setup->palette, TRUE);
    }
    LoadSysMsgBoxPalette(0, 14, 0, HEAPID_PKWD_SELECTSCREEN);
    LoadSysMsgBox(0, 0x20, 14, 0, HEAPID_PKWD_SELECTSCREEN);
}

static void PkwdSelectScreen_FreeWindows(PkwdSelectScreenWork *wk) {
    s32 i;

    for (i = 0; i < WINDOW_STATIC_COUNT; i++) {
        BmpWin_Free(wk->windows[i].window);
    }
}

// Puts a window on its BG, to be sent to VRAM once its text is printed
static void PkwdSelectScreen_ShowWindow(PkwdSelectScreenWork *wk, u32 window) {
    BmpWin_FlushMap(wk->windows[window].window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(wk->windows[window].window));
    wk->windows[window].flushPending = TRUE;
}

// Reads the movie of each screen from the save, and sorts the screens with a movie first
static void PkwdSelectScreen_LoadMovies(PkwdSelectScreenWork *wk, PkwdSelectScreenParam *param) {
    int i;
    s32 j;
    s32 loadedCount = 0;
    s32 emptyIdx = SCREEN_COUNT - 1;

    BattleRec_LoadToolOverlay();
    allocatePokewoodBlk(HEAPID_PKWD_SELECTSCREEN);
    for (i = 0; i < SCREEN_COUNT; i++) {
        wk->movies[i].screen = i;
        wk->movies[i].loaded = Pokewood_LoadDownloadedMovie(param->saveControl, HEAPID_PKWD_SELECTSCREEN, i);
        if (wk->movies[i].loaded == 1) {
            PartyPkm *pkm = GFL_HeapAllocate(HEAPID_PKWD_SELECTSCREEN, PokeParty_GetPkmRawSize(), FALSE,
                                             "pkwd_selectscreen.c", 1570);

            wk->movies[i].movie = Pokewood_GetLoadedMovieField(POKEWOOD_BLOCK_FIELD_MOVIE, 0);
            wk->movies[i].gross = Pokewood_GetLoadedMovieField(POKEWOOD_BLOCK_FIELD_SCORE, 0);
            wk->movies[i].rank = Pokewood_GetLoadedMovieField(POKEWOOD_BLOCK_FIELD_ENDING, 0);
            wk->movies[i].date = Pokewood_GetLoadedMovieField(POKEWOOD_BLOCK_FIELD_DATE, 0);
            wk->movies[i].time = Pokewood_GetLoadedMovieField(POKEWOOD_BLOCK_FIELD_TIME, 0);
            wk->movies[i].watched = PokewoodSave_IsSlotWatched(param->save, i);
            for (j = 0; j < CAST_COUNT; j++) {
                Pokewood_GetLoadedMovieField(POKEWOOD_BLOCK_FIELD_PKM_0 + j, (uintptr_t)pkm);
                wk->movies[i].cast[j] = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
            }
            Pokewood_GetLoadedMovieField(POKEWOOD_BLOCK_FIELD_PKM_0, (uintptr_t)pkm);
            setPartyPokemonSpeciesNameToStrbuf(wk->movies[i].wordSet, 0, pkm);
            GFL_HeapFree(pkm);
        } else {
            wk->movies[i].movie = MOVIE_NONE;
        }
    }
    for (i = 0; i < SCREEN_COUNT; i++) {
        if (wk->movies[i].loaded == 1) {
            wk->sorted[loadedCount] = &wk->movies[i];
            loadedCount++;
        } else {
            wk->sorted[emptyIdx] = &wk->movies[i];
            emptyIdx--;
        }
    }
    freeAndClearPokewoodBlk();
    BattleRec_UnloadToolOverlay();
}

// The row with a movie that was touched this frame, which takes the cursor, or CURSOR_NONE
static s32 PkwdSelectScreen_ReadTouch(PkwdSelectScreenWork *wk) {
    s32 row = GFL_HIDGetTouchPressedRect(sPkwdSelectScreenTouchRects);

    if (row != TOUCH_RECT_NONE) {
        if (wk->sorted[PkwdSelectScreen_GetScreen(wk, row)]->loaded) {
            wk->touched = TRUE;
            wk->cursor = row;
            wk->posterDirty = TRUE;
            PkwdSelectScreen_PrintMovie(wk, PkwdSelectScreen_GetScreen(wk, wk->cursor));
            return row;
        }
    }
    return CURSOR_NONE;
}

// Moves the cursor with the keys. Returns the row that A picks, or CURSOR_NONE
static s32 PkwdSelectScreen_ReadKeys(PkwdSelectScreenWork *wk) {
    if (wk->touched == TRUE && GCTX_HIDGetPressedKeys()) {
        wk->touched = FALSE;
        wk->cursor = 0;
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_UP) {
        if (wk->cursor != 0 && wk->sorted[PkwdSelectScreen_GetScreen(wk, wk->cursor) - 1]->loaded) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->cursor--;
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_DOWN) {
        if (wk->cursor < PAGE_SIZE - 1 && wk->sorted[PkwdSelectScreen_GetScreen(wk, wk->cursor) + 1]->loaded) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->cursor++;
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        return wk->cursor;
    }
    return CURSOR_NONE;
}

static void PkwdSelectScreen_InitText(PkwdSelectScreenWork *wk) {
    s32 i;

    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 15 * 0x20, 0x20, HEAPID_PKWD_SELECTSCREEN);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_POKEWOOD_LINES, wk->heapId);
    wk->strbuf = GFL_StrBufCreate(104, HEAPID_PKWD_SELECTSCREEN);
    for (i = 0; i < SCREEN_COUNT; i++) {
        wk->movies[i].wordSet = GFL_WordSetSystemCreateDefault(HEAPID_PKWD_SELECTSCREEN);
    }
    wk->wordSet = GFL_WordSetSystemCreateDefault(HEAPID_PKWD_SELECTSCREEN);
    wk->printQueue = PrintQueue_CreateDefault(wk->heapId);
    wk->tcbManager = GFL_TCBExMgrCreate(wk->heapId, wk->heapId, 32, 32);
}

static void PkwdSelectScreen_FreeText(PkwdSelectScreenWork *wk) {
    s32 i;

    GFL_TCBExMgrFree(wk->tcbManager);
    GFL_MsgDataFree(wk->msgData);
    PrintQueue_Free(wk->printQueue);
    GFL_FontFree(wk->font);
    for (i = 0; i < SCREEN_COUNT; i++) {
        GFL_WordSetSystemFree(wk->sorted[i]->wordSet);
    }
    GFL_WordSetSystemFree(wk->wordSet);
}

// Prints the page's rows: the screen's name and the title of its movie
static void PkwdSelectScreen_PrintScreens(PkwdSelectScreenWork *wk) {
    StrBuf *strbuf = GFL_StrBufCreate(104, HEAPID_PKWD_SELECTSCREEN | HEAPID_TAIL_BIT);
    s32 i;

    for (i = 0; i < PAGE_SIZE; i++) {
        s32 screen = i + wk->page * PAGE_SIZE;

        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_SCREEN + i].window), 0);
        if (wk->sorted[screen]->loaded) {
            GFL_MsgDataLoadStrbuf(wk->msgData, PokewoodLines_Text_157 + screen, strbuf);
            PrintWindow_Print(&wk->windows[WINDOW_SCREEN + i], wk->printQueue, 0, 4, strbuf, wk->font,
                              PRINT_COLOR(1, 2, 0));
            GFL_MsgDataLoadStrbuf(wk->msgData, PokewoodLines_Text_042 + wk->sorted[screen]->movie, strbuf);
            PrintWindow_Print(&wk->windows[WINDOW_SCREEN + i], wk->printQueue, 0, 20, strbuf, wk->font,
                              PRINT_COLOR(1, 2, 0));
        }
        PkwdSelectScreen_ShowWindow(wk, WINDOW_SCREEN + i);
    }
    GFL_StrBufFree(strbuf);
}

static void PkwdSelectScreen_PrintCentered(PrintWindow *window, PrintQueue *queue, StrBuf *strbuf, s32 y, Font *font,
                                           u16 color) {
    s32 textWidth = GFL_FontGetBlockWidth(strbuf, font, 0);
    s32 width = BmpWin_GetSizeX(window->window) * 8;

    PrintWindow_Print(window, queue, width / 2 - textWidth / 2, y, strbuf, font, color);
}

// Prints the upper screen's text for the movie of a screen: its title, tagline and release date
static void PkwdSelectScreen_PrintMovie(PkwdSelectScreenWork *wk, s32 screen) {
    StrBuf *strbuf = GFL_StrBufCreate(104, HEAPID_PKWD_SELECTSCREEN | HEAPID_TAIL_BIT);
    StrBuf *formatted = GFL_StrBufCreate(104, HEAPID_PKWD_SELECTSCREEN | HEAPID_TAIL_BIT);
    u32 gender = getTrainerGender(wk->param->playerInfo);
    BOOL empty = FALSE;

    if (screen == CURSOR_NONE || wk->cursor == CURSOR_NONE) {
        empty = TRUE;
    } else if (!wk->sorted[screen]->loaded) {
        empty = TRUE;
    }
    if (empty) {
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_TITLE].window), 0);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_TAGLINE].window), 0);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_RELEASE].window), 0);
        PkwdSelectScreen_SetMarkVisible(wk, 0, FALSE);
        wk->posterDirty = TRUE;
    } else {
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_TITLE].window), 0);
        GFL_MsgDataLoadStrbuf(wk->msgData, PokewoodLines_Text_042 + wk->sorted[screen]->movie, strbuf);
        PkwdSelectScreen_PrintCentered(&wk->windows[WINDOW_TITLE], wk->printQueue, strbuf, 8, wk->font,
                                       PRINT_COLOR(13, 12, 0));
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_TAGLINE].window), 0);
        GFL_MsgDataLoadStrbuf(wk->msgData, sPkwdSelectScreenTaglines[wk->sorted[screen]->movie][gender], strbuf);
        GFL_WordSetFormatStrbuf(wk->sorted[screen]->wordSet, formatted, strbuf);
        PkwdSelectScreen_PrintCentered(&wk->windows[WINDOW_TAGLINE], wk->printQueue, formatted, 6, wk->font,
                                       PRINT_COLOR(13, 12, 0));
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_RELEASE].window), 0);
        GFL_MsgDataLoadStrbuf(wk->msgData, sPkwdSelectScreenReleaseMessages[2], strbuf);
        PrintWindow_Print(&wk->windows[WINDOW_RELEASE], wk->printQueue, 0, 0, strbuf, wk->font, PRINT_COLOR(15, 14, 0));
        GFL_MsgDataLoadStrbuf(wk->msgData, sPkwdSelectScreenReleaseMessages[0], strbuf);
        WordSetNumber(wk->wordSet, 0, 2000 + (u8)(wk->sorted[screen]->date >> 24), 4, 0, TRUE);
        WordSetNumber(wk->wordSet, 1, (u8)(wk->sorted[screen]->date >> 16), 2, 2, TRUE);
        WordSetNumber(wk->wordSet, 2, (u8)(wk->sorted[screen]->date >> 8), 2, 2, TRUE);
        GFL_WordSetFormatStrbuf(wk->wordSet, formatted, strbuf);
        PrintWindow_Print(&wk->windows[WINDOW_RELEASE], wk->printQueue, 96, 0, formatted, wk->font,
                          PRINT_COLOR(15, 14, 0));
        GFL_MsgDataLoadStrbuf(wk->msgData, sPkwdSelectScreenReleaseMessages[1], strbuf);
        WordSetNumber(wk->wordSet, 0, (u8)(wk->sorted[screen]->time >> 16), 2, 2, TRUE);
        WordSetNumber(wk->wordSet, 1, (u8)(wk->sorted[screen]->time >> 8), 2, 2, TRUE);
        GFL_WordSetFormatStrbuf(wk->wordSet, formatted, strbuf);
        PrintWindow_Print(&wk->windows[WINDOW_RELEASE], wk->printQueue, 184, 0, formatted, wk->font,
                          PRINT_COLOR(15, 14, 0));
        PkwdSelectScreen_SetMarkSequence(wk, 0, sPkwdSelectScreenRankSequences[wk->sorted[screen]->rank]);
        if (wk->sorted[screen]->watched == TRUE) {
            PkwdSelectScreen_SetMarkVisible(wk, 0, TRUE);
        } else {
            PkwdSelectScreen_SetMarkVisible(wk, 0, FALSE);
        }
        wk->posterDirty = TRUE;
    }
    GFL_StrBufFree(formatted);
    GFL_StrBufFree(strbuf);
    PkwdSelectScreen_ShowWindow(wk, WINDOW_TITLE);
    PkwdSelectScreen_ShowWindow(wk, WINDOW_TAGLINE);
    PkwdSelectScreen_ShowWindow(wk, WINDOW_RELEASE);
}

// Blinks the frame of the cursor's row, and stills the others
static void PkwdSelectScreen_SetBlink(PkwdSelectScreenWork *wk, s32 cursor) {
    s32 i;

    for (i = 0; i < PAGE_SIZE; i++) {
        if (i == cursor) {
            BlinkPalAnm_InitAnime(wk->blinks[cursor]);
        } else {
            BlinkPalAnm_SetAnimeCount(wk->blinks[i], 0x4000);
            BlinkPalAnm_Main(wk->blinks[i]);
        }
    }
}

// Shows the movie under the cursor when the cursor has moved
static void PkwdSelectScreen_UpdateCursor(PkwdSelectScreenWork *wk) {
    if (wk->lastCursor != wk->cursor && wk->cursor >= 0) {
        wk->posterDirty = TRUE;
        PkwdSelectScreen_PrintMovie(wk, PkwdSelectScreen_GetScreen(wk, wk->cursor));
        PkwdSelectScreen_SetBlink(wk, wk->cursor);
    }
    wk->lastCursor = wk->cursor;
}

// Opens the message window and starts printing a message in it
static void PkwdSelectScreen_PrintMessage(PkwdSelectScreenWork *wk, u32 messageId) {
    const PkwdSelectScreenWindowSetup *setup = &sPkwdSelectScreenWindows[WINDOW_MESSAGE];
    BmpWin *window =
        BmpWin_CreateDynamic(setup->bg, setup->x, setup->y, setup->width, setup->height, setup->palette, TRUE);

    wk->windows[WINDOW_MESSAGE].window = window;
    BmpWin_DrawFrame(window, 2, 0x20, 14);
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0xff);
    GFL_MsgDataLoadStrbuf(wk->msgData, messageId, wk->strbuf);
    wk->stream = PrintStream_Create(window, 0, 0, wk->strbuf, wk->font, TextSpeed_GetCurrentWait(), wk->tcbManager, 4,
                                    HEAPID_PKWD_SELECTSCREEN, 15);
    BmpWin_TransferNow(window);
}

// Opens the details of a screen's movie: its cast, its box office gross and its summary
static void PkwdSelectScreen_PrintDetails(PkwdSelectScreenWork *wk, int screen) {
    const PkwdSelectScreenWindowSetup *setup = &sPkwdSelectScreenWindows[WINDOW_DETAILS];
    BmpWin *window =
        BmpWin_CreateDynamic(setup->bg, setup->x, setup->y, setup->width, setup->height, setup->palette, TRUE);
    StrBuf *featuring;
    StrBuf *grosses;
    StrBuf *summary;
    StrBuf *summaryText;
    StrBuf *gross;
    s32 width;
    s32 i;

    wk->windows[WINDOW_DETAILS].window = window;
    BmpWin_DrawFrame(window, 2, 0x20, 14);
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0xff);
    featuring = GFL_MsgDataLoadStrbufNew(wk->msgData, PokewoodLines_Text_165);
    PrintWindow_PrintNoColor(&wk->windows[WINDOW_DETAILS], wk->printQueue, 4, 0, featuring, wk->font);
    grosses = GFL_MsgDataLoadStrbufNew(wk->msgData, PokewoodLines_Text_166);
    PrintWindow_PrintNoColor(&wk->windows[WINDOW_DETAILS], wk->printQueue, 4, 56, grosses, wk->font);
    summary = GFL_MsgDataLoadStrbufNew(wk->msgData, PokewoodLines_Text_167);
    PrintWindow_PrintNoColor(&wk->windows[WINDOW_DETAILS], wk->printQueue, 4, 80, summary, wk->font);
    summaryText = GFL_MsgDataLoadStrbufNew(wk->msgData, wk->sorted[screen]->movie);
    PrintWindow_PrintNoColor(&wk->windows[WINDOW_DETAILS], wk->printQueue, 8, 96, summaryText, wk->font);
    if (wk->sorted[screen]->watched) {
        s32 amount = wk->sorted[screen]->gross;
        s32 tenths;

        gross = GFL_MsgDataLoadStrbufNew(wk->msgData, PokewoodLines_Text_170);
        WordSetNumber(wk->sorted[screen]->wordSet, 3, amount / 10, 3, 0, TRUE);
        if (amount >= 0) {
            tenths = amount % 10;
        } else {
            tenths = -amount % 10;
        }
        WordSetNumber(wk->sorted[screen]->wordSet, 4, tenths, 1, 2, TRUE);
        WordSetNumber(wk->sorted[screen]->wordSet, 2, amount, 4, 0, TRUE);
        GFL_WordSetFormatStrbuf(wk->sorted[screen]->wordSet, wk->strbuf, gross);
    } else {
        gross = GFL_MsgDataLoadStrbufNew(wk->msgData, PokewoodLines_Text_176);
        GFL_StrBufCopy(wk->strbuf, gross);
    }
    width = GFL_FontGetBlockWidth(wk->strbuf, wk->font, 0);
    PrintWindow_PrintNoColor(&wk->windows[WINDOW_DETAILS], wk->printQueue, 216 - width, 56, wk->strbuf, wk->font);
    for (i = 0; i < CAST_COUNT; i++) {
        if (wk->sorted[screen]->cast[i] != 0) {
            StrBuf *name = GFL_MsgDataLoadStrbufNew(g_PMLSpeciesNamesResident, wk->sorted[screen]->cast[i]);

            PrintWindow_PrintNoColor(&wk->windows[WINDOW_DETAILS], wk->printQueue, sPkwdSelectScreenCastPos[i].x,
                                     sPkwdSelectScreenCastPos[i].y, name, wk->font);
            GFL_StrBufFree(name);
        }
    }
    GFL_StrBufFree(gross);
    GFL_StrBufFree(summaryText);
    GFL_StrBufFree(summary);
    GFL_StrBufFree(grosses);
    GFL_StrBufFree(featuring);
    BmpWin_TransferNow(window);
}

// Runs the state. Returns TRUE once the screen is done
static BOOL PkwdSelectScreen_RunState(PkwdSelectScreenWork *wk) {
    return sPkwdSelectScreenStates[wk->state](wk);
}

static BOOL PkwdSelectScreen_FadeIn(PkwdSelectScreenWork *wk) {
    wk->posterDirty = TRUE;
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 8, 1, HEAPID_PKWD_SELECTSCREEN);
    wk->state = STATE_WAIT_FADE_IN;
    return FALSE;
}

static BOOL PkwdSelectScreen_WaitFadeIn(PkwdSelectScreenWork *wk) {
    if (GFL_WipeIsFinished()) {
        wk->state = STATE_START_INPUT;
    }
    return FALSE;
}

static BOOL PkwdSelectScreen_StartInput(PkwdSelectScreenWork *wk) {
    wk->state = STATE_INPUT;
    return FALSE;
}

// The cursor's row, or the last row above it that has a movie
static s32 PkwdSelectScreen_GetLastCursor(PkwdSelectScreenWork *wk) {
    s32 cursor;

    for (cursor = wk->cursor; cursor >= 0; cursor--) {
        if (wk->sorted[PkwdSelectScreen_GetScreen(wk, cursor)]->loaded == 1) {
            break;
        }
    }
    return cursor;
}

// Picks a movie, turns the page or backs out
static BOOL PkwdSelectScreen_Input(PkwdSelectScreenWork *wk) {
    s32 picked = CURSOR_NONE;

    if (!PrintQueue_IsEmpty(wk->printQueue)) {
        return FALSE;
    }
    if (!TouchBar_IsTouching(wk->touchBar)) {
        picked = PkwdSelectScreen_ReadTouch(wk);
        if (picked == CURSOR_NONE) {
            picked = PkwdSelectScreen_ReadKeys(wk);
        }
    }
    PkwdSelectScreen_UpdateCursor(wk);
    if (picked != CURSOR_NONE) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        BlinkPalAnm_StartBlink(wk->blinks[wk->cursor]);
        TouchBar_SetActive(wk->touchBar, FALSE);
        wk->state = STATE_WAIT_BLINK;
    } else if (TouchBar_GetDecided(wk->touchBar) == TOUCHBAR_ICON_RETURN) {
        TouchBar_SetActive(wk->touchBar, FALSE);
        wk->state = STATE_FADE_OUT;
        *wk->param->result = PKWD_SELECTSCREEN_CANCEL;
    } else if (TouchBar_GetDecided(wk->touchBar) == TOUCHBAR_ICON_LEFT ||
               TouchBar_GetDecided(wk->touchBar) == TOUCHBAR_ICON_RIGHT) {
        wk->page ^= 1;
        wk->cursor = PkwdSelectScreen_GetLastCursor(wk);
        PkwdSelectScreen_PrintScreens(wk);
        PkwdSelectScreen_PrintMovie(wk, PkwdSelectScreen_GetScreen(wk, wk->cursor));
        PkwdSelectScreen_FreeBlinks(wk);
        PkwdSelectScreen_CreateBlinks(wk);
        PkwdSelectScreen_ShowNewMarks(wk, wk->page);
    }
    return FALSE;
}

// Once the picked row has blinked, asks what to do with its movie
static BOOL PkwdSelectScreen_WaitBlink(PkwdSelectScreenWork *wk) {
    if (BlinkPalAnm_IsBlinkEnd(wk->blinks[wk->cursor])) {
        if (wk->param->mode == PKWD_SELECTSCREEN_MODE_WATCH) {
            PkwdSelectScreen_PrintMessage(wk, PokewoodLines_Text_148);
            wk->state = STATE_WAIT_MESSAGE;
            wk->nextState = STATE_OPEN_WATCH_MENU;
        } else {
            PkwdSelectScreen_PrintMessage(wk, PokewoodLines_Text_149);
            wk->state = STATE_WAIT_MESSAGE;
            wk->nextState = STATE_OPEN_REMOVE_MENU;
        }
    }
    return FALSE;
}

static BOOL PkwdSelectScreen_WaitMessage(PkwdSelectScreenWork *wk) {
    switch (PrintStream_GetState(wk->stream)) {
    case PRINT_STREAM_DONE:
        PrintStream_Delete(wk->stream);
        wk->state = wk->nextState;
        break;
    case PRINT_STREAM_RUNNING:
        if ((GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || GFL_HIDIsTouchHeld()) {
            PrintStream_SkipWait(wk->stream, 0);
        }
        break;
    }
    return FALSE;
}

static BOOL PkwdSelectScreen_AskRemoveSure(PkwdSelectScreenWork *wk) {
    PkwdSelectScreen_PrintMessage(wk, PokewoodLines_Text_150);
    wk->state = STATE_WAIT_MESSAGE;
    wk->nextState = STATE_OPEN_YES_NO_MENU;
    return FALSE;
}

// Once an item of the menu is picked, closes the menu and the message. Returns the item, or -1 until then
static s32 PkwdSelectScreen_GetMenuResult(PkwdSelectScreenWork *wk) {
    s32 result = -1;

    if (AppTaskMenu_IsFlashFinished(wk->menu)) {
        result = AppTaskMenu_GetCursorPos(wk->menu);
        PkwdSelectScreen_FreeMenu(wk->menu);
        wk->menu = NULL;
        BmpWin_ClearFrame(wk->windows[WINDOW_MESSAGE].window, 2);
        BmpWin_ClearScreenNow(wk->windows[WINDOW_MESSAGE].window);
        BmpWin_Free(wk->windows[WINDOW_MESSAGE].window);
    }
    return result;
}

static BOOL PkwdSelectScreen_OpenWatchMenu(PkwdSelectScreenWork *wk) {
    wk->menu = PkwdSelectScreen_CreateMenu(wk->menuRes, wk->msgData, wk->heapId, MENU_WATCH);
    wk->state = STATE_WATCH_MENU;
    return FALSE;
}

static BOOL PkwdSelectScreen_WatchMenu(PkwdSelectScreenWork *wk) {
    s32 result = PkwdSelectScreen_GetMenuResult(wk);

    if (result >= 0) {
        if (result == 0) {
            wk->state = STATE_FADE_OUT;
            *wk->param->result = wk->sorted[PkwdSelectScreen_GetScreen(wk, wk->cursor)]->screen;
        } else if (result == 1) {
            TouchBar_SetActive(wk->touchBar, TRUE);
            wk->state = STATE_OPEN_DETAILS;
        } else {
            wk->state = STATE_START_INPUT;
            TouchBar_SetActive(wk->touchBar, TRUE);
            PkwdSelectScreen_SetBlink(wk, wk->cursor);
        }
    }
    return FALSE;
}

static BOOL PkwdSelectScreen_OpenRemoveMenu(PkwdSelectScreenWork *wk) {
    wk->menu = PkwdSelectScreen_CreateMenu(wk->menuRes, wk->msgData, wk->heapId, MENU_REMOVE);
    wk->state = STATE_REMOVE_MENU;
    return FALSE;
}

static BOOL PkwdSelectScreen_RemoveMenu(PkwdSelectScreenWork *wk) {
    s32 result = PkwdSelectScreen_GetMenuResult(wk);

    if (result >= 0) {
        if (result == 0) {
            wk->state = STATE_ASK_REMOVE_SURE;
        } else if (result == 1) {
            TouchBar_SetActive(wk->touchBar, TRUE);
            wk->state = STATE_OPEN_DETAILS;
        } else {
            wk->state = STATE_START_INPUT;
            TouchBar_SetActive(wk->touchBar, TRUE);
            PkwdSelectScreen_SetBlink(wk, wk->cursor);
        }
    }
    return FALSE;
}

static BOOL PkwdSelectScreen_OpenYesNoMenu(PkwdSelectScreenWork *wk) {
    wk->menu = PkwdSelectScreen_CreateMenu(wk->menuRes, wk->msgData, wk->heapId, MENU_YES_NO);
    wk->state = STATE_YES_NO_MENU;
    return FALSE;
}

static BOOL PkwdSelectScreen_YesNoMenu(PkwdSelectScreenWork *wk) {
    s32 result = PkwdSelectScreen_GetMenuResult(wk);

    if (result >= 0) {
        if (result == 0) {
            wk->state = STATE_FADE_OUT;
            *wk->param->result = wk->sorted[PkwdSelectScreen_GetScreen(wk, wk->cursor)]->screen;
        } else {
            wk->state = STATE_START_INPUT;
            TouchBar_SetActive(wk->touchBar, TRUE);
            PkwdSelectScreen_SetBlink(wk, wk->cursor);
        }
    }
    return FALSE;
}

static BOOL PkwdSelectScreen_FadeOut(PkwdSelectScreenWork *wk) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 8, 1,
                HEAPID_PKWD_SELECTSCREEN);
    wk->state = STATE_WAIT_FADE_OUT;
    return FALSE;
}

static BOOL PkwdSelectScreen_WaitFadeOut(PkwdSelectScreenWork *wk) {
    if (GFL_WipeIsFinished()) {
        return TRUE;
    }
    return FALSE;
}

static BOOL PkwdSelectScreen_OpenDetails(PkwdSelectScreenWork *wk) {
    PkwdSelectScreen_PrintDetails(wk, PkwdSelectScreen_GetScreen(wk, wk->cursor));
    PkwdSelectScreen_ShowPageIcons(wk, FALSE);
    wk->state = STATE_DETAILS;
    return FALSE;
}

// Closes the details when the return button is picked
static BOOL PkwdSelectScreen_Details(PkwdSelectScreenWork *wk) {
    if (TouchBar_GetDecided(wk->touchBar) == TOUCHBAR_ICON_RETURN) {
        BmpWin_ClearFrame(wk->windows[WINDOW_DETAILS].window, 2);
        BmpWin_ClearScreenNow(wk->windows[WINDOW_DETAILS].window);
        BmpWin_Free(wk->windows[WINDOW_DETAILS].window);
        PkwdSelectScreen_ShowPageIcons(wk, TRUE);
        PkwdSelectScreen_SetBlink(wk, wk->cursor);
        wk->state = STATE_START_INPUT;
    }
    return FALSE;
}

// Shows the page arrows, which stay hidden while no more than a page of screens has a movie
static void PkwdSelectScreen_ShowPageIcons(PkwdSelectScreenWork *wk, BOOL visible) {
    if (!wk->sorted[PAGE_SIZE]->loaded) {
        visible = FALSE;
    }
    TouchBar_SetIconVisible(wk->touchBar, TOUCHBAR_ICON_LEFT, visible);
    TouchBar_SetIconVisible(wk->touchBar, TOUCHBAR_ICON_RIGHT, visible);
}
