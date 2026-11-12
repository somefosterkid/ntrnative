// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov310/research_top.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/research_radar/bg_font.h"
#include "app/research_radar/palette_anime.h"
#include "app/research_radar/queue.h"
#include "app/research_radar/research_common.h"
#include "app/research_radar/research_top.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/sound.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "nnsys/g2d.h"
#include "save/event_work.h"
#include "system/game_beacon.h"
#include "system/game_data.h"
#include "system/gf_font.h"

// The Research Radar's top screen, with a button for the list of surveys and one for the graph of the survey being
// run. The list's button is disabled until a second survey is unlocked. It is used by touch or with a cursor, and the
// screen it goes on to starts in the same mode

// The sequences of the screen, which run one after another from a queue
enum {
    SEQ_SETUP,
    SEQ_TOUCH,
    SEQ_CURSOR,
    SEQ_WAIT,
    SEQ_FADE_OUT,
    SEQ_TEARDOWN,
    SEQ_END,
};

// The buttons, in the order of the cursor
enum {
    BUTTON_LIST,
    BUTTON_GRAPH,
    BUTTON_COUNT,
};

// The BGs: the buttons on the main engine's BG 2 and their labels on BG 3, and the sub engine's title on BG 6 and its
// text on BG 7, in front of the proc's BGs
#define BG_MAIN_BUTTONS 2
#define BG_MAIN_TEXT 3
#define BG_SUB_TITLE 6
#define BG_SUB_TEXT 7

// The objects' resources, of the main engine and then of the sub engine
enum {
    OBJ_RES_MAIN_CHARS,
    OBJ_RES_MAIN_PALETTE,
    OBJ_RES_MAIN_CELL_ANIMS,
    OBJ_RES_SUB_CHARS,
    OBJ_RES_SUB_PALETTE,
    OBJ_RES_SUB_CELL_ANIMS,
    OBJ_RES_COUNT,
};

#define UNIT_COUNT 2
#define ACTOR_COUNT 1
#define MSG_DATA_COUNT 2
#define BG_FONT_COUNT 4
#define BUTTON_RECT_COUNT 3

// The palette animations: the cursor's button, a cursor move, a button pressed, and the list's disabled button
enum {
    ANIME_CURSOR,
    ANIME_MOVE,
    ANIME_DECIDE,
    ANIME_DISABLED_1,
    ANIME_DISABLED_2,
    ANIME_COUNT,
};

// The frames a pressed button flashes before the screen fades out
#define DECIDE_WAIT_FRAMES 15

// The first survey is always there, and each of these event flags unlocks one more
#define FIRST_SURVEY_FLAG 0xf0 // Black's
#define LAST_SURVEY_FLAG 0xf8 // Black's

struct ResearchTop {
    ResearchCommon *common;
    HeapID heapId;
    Font *font;
    MsgData *msgData[MSG_DATA_COUNT];
    Queue *queue;
    u32 seq;
    u32 seqState;
    u32 seqFrames;
    u32 waitFrames;
    int cursor;
    TouchRect buttonRects[BUTTON_RECT_COUNT];
    PaletteAnime *animes[ANIME_COUNT];
    BGFont *bgFonts[BG_FONT_COUNT];
    u32 objRes[OBJ_RES_COUNT];
    ClActUnit *units[UNIT_COUNT];
    ClActor *actors[ACTOR_COUNT];
    BOOL seqDone;
    // Whether the icon of a new survey result is shown
    BOOL newIconShown;
    BOOL isEnd;
    u32 next;
};

typedef struct {
    BGFontWindow window;
    u32 msgDataIndex;
    u32 strId;
    BOOL centered;
} BGFontEntry;

static void func_ov217_021b9b98(ResearchTop *wk);
static void func_ov217_021b9c54(ResearchTop *wk);
static void func_ov217_021b9e44(ResearchTop *wk);
static void func_ov217_021ba078(ResearchTop *wk);
static void func_ov217_021ba094(ResearchTop *wk);
static void func_ov217_021ba0c4(ResearchTop *wk);
static void func_ov217_021ba144(ResearchTop *wk, u32 seq);
static void func_ov217_021ba150(ResearchTop *wk);
static void func_ov217_021ba158(ResearchTop *wk);
static void func_ov217_021ba180(ResearchTop *wk, u32 seq);
static u32 func_ov217_021ba190(ResearchTop *wk);
static void func_ov217_021ba194(ResearchTop *wk);
static u32 func_ov217_021ba19c(ResearchTop *wk);
static void func_ov217_021ba1c0(ResearchTop *wk, BOOL touch);
static void func_ov217_021ba1cc(ResearchTop *wk, u32 next);
static void func_ov217_021ba1d4(ResearchTop *wk);
static void func_ov217_021ba1f8(ResearchTop *wk);
static void func_ov217_021ba228(ResearchTop *wk);
static void func_ov217_021ba254(ResearchTop *wk, int pos);
static void func_ov217_021ba284(int button);
static void func_ov217_021ba2bc(int button);
static void func_ov217_021ba2fc(ResearchTop *wk);
static void func_ov217_021ba31c(ResearchTop *wk);
static void func_ov217_021ba328(ResearchTop *wk);
static void func_ov217_021ba354(void);
static void func_ov217_021ba364(void);
static void func_ov217_021ba374(ResearchTop *wk);
static u32 func_ov217_021ba37c(ResearchTop *wk);
static void func_ov217_021ba380(ResearchTop *wk, u32 frames);
static u32 func_ov217_021ba384(ResearchTop *wk);
static void func_ov217_021ba388(ResearchTop *wk, int dir);
static void func_ov217_021ba3b0(ResearchTop *wk, int pos);
static BOOL func_ov217_021ba3b4(ResearchTop *wk, int button);
static u32 func_ov217_021ba3d0(ResearchTop *wk, u32 index);
static ClActUnit *func_ov217_021ba3d8(ResearchTop *wk, u32 index);
static ClActor *func_ov217_021ba3e0(ResearchTop *wk, u32 index);
static void func_ov217_021ba3e8(ResearchTop *wk, u32 index);
static void func_ov217_021ba40c(ResearchTop *wk, u32 index);
static void func_ov217_021ba41c(ResearchTop *wk);
static u8 func_ov217_021ba434(ResearchTop *wk);
static ResearchCommon *func_ov217_021ba4c8(ResearchTop *wk);
static void func_ov217_021ba4cc(ResearchTop *wk, ResearchCommon *common);
static void func_ov217_021ba4d0(ResearchTop *wk, HeapID heapId);
static BOOL func_ov217_021ba4d4(ResearchTop *wk);
static ResearchTop *func_ov217_021ba4e0(HeapID heapId);
static void func_ov217_021ba4fc(ResearchTop *wk);
static void func_ov217_021ba504(ResearchTop *wk);
static void func_ov217_021ba560(ResearchTop *wk);
static void func_ov217_021ba5b4(ResearchTop *wk);
static void func_ov217_021ba5bc(ResearchTop *wk);
static void func_ov217_021ba5d8(ResearchTop *wk);
static void func_ov217_021ba5e4(ResearchTop *wk);
static void func_ov217_021ba5f8(ResearchTop *wk);
static void func_ov217_021ba624(ResearchTop *wk);
static void func_ov217_021ba63c(ResearchTop *wk);
static void func_ov217_021ba674(ResearchTop *wk);
static void func_ov217_021ba67c(ResearchTop *wk);
static void func_ov217_021ba68c(ResearchTop *wk);
static void func_ov217_021ba698(ResearchTop *wk);
static void func_ov217_021ba748(ResearchTop *wk);
static void func_ov217_021ba768(ResearchTop *wk);
static void func_ov217_021ba7b4(ResearchTop *wk);
static void func_ov217_021ba7b8(ResearchTop *wk);
static void func_ov217_021ba7d0(ResearchTop *wk);
static void func_ov217_021ba7e0(ResearchTop *wk);
static void func_ov217_021ba82c(ResearchTop *wk);
static void func_ov217_021ba830(ResearchTop *wk);
static void func_ov217_021ba848(ResearchTop *wk);
static void func_ov217_021ba858(ResearchTop *wk);
static void func_ov217_021ba86c(ResearchTop *wk);
static void func_ov217_021ba8e0(ResearchTop *wk);
static void func_ov217_021ba8fc(ResearchTop *wk);
static void func_ov217_021ba910(ResearchTop *wk);
static void func_ov217_021ba960(ResearchTop *wk);
static void func_ov217_021ba978(ResearchTop *wk);
static void func_ov217_021ba9d0(ResearchTop *wk);
static void func_ov217_021ba9e8(ResearchTop *wk);
static void func_ov217_021ba9fc(ResearchTop *wk);
static void func_ov217_021baa28(ResearchTop *wk);
static void func_ov217_021baa40(ResearchTop *wk);
static void func_ov217_021baa48(ResearchTop *wk);
static void func_ov217_021baab8(ResearchTop *wk);
static void func_ov217_021baac4(ResearchTop *wk);
static void func_ov217_021baad8(ResearchTop *wk);
static void func_ov217_021baaf4(ResearchTop *wk);
static void func_ov217_021bab0c(ResearchTop *wk);
static void func_ov217_021bab38(ResearchTop *wk);
static void func_ov217_021bab50(void *data);
static void func_ov217_021bab58(ResearchTop *wk);
static void func_ov217_021bab68(ResearchTop *wk);
static void func_ov217_021bab70(ResearchTop *wk);

// pokebw2's sTopUnitPriorities
static const u8 data_ov217_021c2ed0[UNIT_COUNT] = { 0, 0 };

// pokebw2's sTopUnitCounts
static const u16 data_ov217_021c2ed2[UNIT_COUNT] = { 10, 10 };

// pokebw2's sTopMsgFiles
static const u32 data_ov217_021c2ed8[MSG_DATA_COUNT] = { 0xa9, 0xa4 };

// pokebw2's sTopButtonRects
static const ResearchRect data_ov217_021c2ee0[BUTTON_RECT_COUNT] = {
    { 24, 232, 40, 80 },
    { 24, 232, 96, 136 },
#ifdef BUGFIX
    { 0, 0, TOUCH_RECT_END, 0 },
#else
    // BUG: The end of the table is written as a TouchRect, with TOUCH_RECT_END in the first field, which is left here,
    // so the copy in the work has no end and the touch panel goes on reading the work after it
    { TOUCH_RECT_END, 0, 0, 0 },
#endif
};

// pokebw2's sTopLCDConfig
static const BGSysLCDConfig data_ov217_021c2eec = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// The icon of a new result
// pokebw2's sTopActors
static const ResearchActorSetup data_ov217_021c2efc[ACTOR_COUNT] = { { 32, 104, 5, 0, 0, 1, OBJ_RES_MAIN_CHARS,
                                                              OBJ_RES_MAIN_PALETTE, OBJ_RES_MAIN_CELL_ANIMS, 0 } };

// pokebw2's sTopBGSubText
static const BGSetup data_ov217_021c2f18 = { 0,
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

// pokebw2's sTopBGMainText
static const BGSetup data_ov217_021c2f58 = { 0,
                                        0,
                                        0x800,
                                        0,
                                        BGRES_256x256,
                                        GX_BG_COLORMODE_16,
                                        GX_BG_SCRBASE_0x1000,
                                        GX_BG_CHARBASE_0x10000,
                                        0x8000,
                                        GX_BG_EXTPLTT_01,
                                        1,
                                        GX_BG_AREAOVER_XLU,
                                        FALSE };

// pokebw2's sTopBGSubTitle
static const BGSetup data_ov217_021c2f78 = { 0,
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

// pokebw2's sTopBGMainButtons
static const BGSetup data_ov217_021c2f38 = { 0,
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

// pokebw2's sTopBGFonts
static const BGFontEntry data_ov217_021c2f98[BG_FONT_COUNT] = {
    { { BG_SUB_TEXT, 0, 17, 32, 6, 0, 16, 15, 3, 4, 0 }, 0, 1, FALSE },
    { { BG_MAIN_TEXT, 2, 0, 28, 4, 0, 4, 14, 3, 4, 0 }, 0, 0, FALSE },
    { { BG_MAIN_TEXT, 3, 5, 26, 5, 0, 16, 14, 1, 2, 0 }, 0, 2, FALSE },
    { { BG_MAIN_TEXT, 3, 12, 26, 5, 0, 16, 14, 1, 2, 0 }, 0, 3, FALSE },
};

// pokebw2's sTopPaletteAnimes
static const ResearchPaletteAnimeSetup data_ov217_021c2ff8[ANIME_COUNT] = {
    { (u16 *)(HW_BG_PLTT + 0x146), (const u16 *)(HW_BG_PLTT + 0x146), 3, 0, 0xffff },
    { (u16 *)(HW_BG_PLTT + 0x14c), (const u16 *)(HW_BG_PLTT + 0x14c), 1, 4, 0xffff },
    { (u16 *)(HW_BG_PLTT + 0x14c), (const u16 *)(HW_BG_PLTT + 0x14c), 1, 3, 0xffff },
    { (u16 *)(HW_BG_PLTT + 0xa0), (const u16 *)(HW_BG_PLTT + 0xa0), 16, 9, 0 },
    { (u16 *)(HW_BG_PLTT + 0x1e0), (const u16 *)(HW_BG_PLTT + 0x1e0), 16, 9, 0 },
};

// pokebw2's ResearchTop_Create
ResearchTop *func_ov217_021b9ad0(ResearchCommon *common) {
    HeapID heapId = func_ov217_021c09ac(common);
    ResearchTop *wk = func_ov217_021ba4e0(heapId);

    func_ov217_021ba504(wk);
    func_ov217_021ba4d0(wk, heapId);
    func_ov217_021ba4cc(wk, common);
    return wk;
}

// pokebw2's ResearchTop_Delete
void func_ov217_021b9af8(ResearchTop *wk) {
    func_ov217_021ba68c(wk);
    func_ov217_021ba4fc(wk);
}

// pokebw2's ResearchTop_Main
void func_ov217_021b9b08(ResearchTop *wk) {
    switch (wk->seq) {
    case SEQ_SETUP:
        func_ov217_021b9b98(wk);
        break;
    case SEQ_TOUCH:
        func_ov217_021b9c54(wk);
        break;
    case SEQ_CURSOR:
        func_ov217_021b9e44(wk);
        break;
    case SEQ_WAIT:
        func_ov217_021ba078(wk);
        break;
    case SEQ_FADE_OUT:
        func_ov217_021ba094(wk);
        break;
    case SEQ_TEARDOWN:
        func_ov217_021ba0c4(wk);
        break;
    case SEQ_END:
        return;
    }

    if (!func_ov217_021b9b80(wk)) {
        func_ov217_021ba1d4(wk);
        func_ov217_021ba41c(wk);
        func_ov217_021c09d0(wk->common);
        func_0204a600();
    }
    func_ov217_021ba374(wk);
    func_ov217_021ba158(wk);
}

// pokebw2's ResearchTop_IsEnd
BOOL func_ov217_021b9b80(ResearchTop *wk) {
    if (wk->isEnd) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ResearchTop_GetNext
u32 func_ov217_021b9b90(ResearchTop *wk) {
    return wk->next;
}

// pokebw2's ResearchTop_SeqSetup
static void func_ov217_021b9b98(ResearchTop *wk) {
    u32 startSeq;

    func_ov217_021ba67c(wk);
    func_ov217_021ba5bc(wk);
    func_ov217_021ba5f8(wk);
    func_ov217_021ba63c(wk);
    func_ov217_021ba560(wk);
    func_ov217_021ba698(wk);
    func_ov217_021ba768(wk);
    func_ov217_021ba7b8(wk);
    func_ov217_021ba7e0(wk);
    func_ov217_021ba830(wk);
    func_ov217_021ba86c(wk);
    func_ov217_021ba910(wk);
    func_ov217_021ba978(wk);
    func_ov217_021ba9fc(wk);
    func_ov217_021baa48(wk);
    func_ov217_021baad8(wk);
    func_ov217_021bab0c(wk);
    func_ov217_021bab70(wk);
    func_ov217_021bab58(wk);

    if (!func_ov217_021ba3b4(wk, BUTTON_LIST)) {
        func_ov217_021ba2fc(wk);
    }

    startSeq = func_ov217_021ba19c(wk);
    func_ov217_021ba144(wk, func_ov217_021ba19c(wk));
    if (startSeq == SEQ_CURSOR) {
        func_ov217_021ba284(wk->cursor);
        func_ov217_021ba3e8(wk, ANIME_CURSOR);
    }

    func_ov217_021ba354();
    func_ov217_021ba150(wk);
}

// pokebw2's ResearchTop_SeqTouch
static void func_ov217_021b9c54(ResearchTop *wk) {
    u32 keys = GCTX_HIDGetPressedKeys();
    s32 button = func_02035c08(wk->buttonRects);
    s32 commonButton = func_02035c08(func_ov217_021c09b8(wk->common));

    if (func_ov217_021ba4d4(wk) || commonButton == RESEARCH_COMMON_BUTTON_RETURN) {
        func_ov217_021ba1c0(wk, TRUE);
        func_ov217_021ba1cc(wk, RESEARCH_TOP_NEXT_EXIT);
        func_ov217_021ba31c(wk);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021ba380(wk, DECIDE_WAIT_FRAMES);
        func_ov217_021ba150(wk);
        func_ov217_021ba144(wk, SEQ_WAIT);
        func_ov217_021ba144(wk, SEQ_FADE_OUT);
        func_ov217_021ba144(wk, SEQ_TEARDOWN);
    } else if (keys & PAD_BUTTON_B) {
        func_ov217_021ba1c0(wk, FALSE);
        func_ov217_021ba1cc(wk, RESEARCH_TOP_NEXT_EXIT);
        func_ov217_021ba31c(wk);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021ba380(wk, DECIDE_WAIT_FRAMES);
        func_ov217_021ba150(wk);
        func_ov217_021ba144(wk, SEQ_WAIT);
        func_ov217_021ba144(wk, SEQ_FADE_OUT);
        func_ov217_021ba144(wk, SEQ_TEARDOWN);
    } else if ((keys & PAD_KEY_UP) || (keys & PAD_KEY_DOWN) || (keys & PAD_KEY_LEFT) || (keys & PAD_KEY_RIGHT) ||
               (keys & PAD_BUTTON_A)) {
        func_ov217_021ba284(wk->cursor);
        func_ov217_021ba3e8(wk, ANIME_CURSOR);
        func_ov217_021ba150(wk);
        func_ov217_021ba144(wk, SEQ_CURSOR);
    } else if (button == BUTTON_LIST) {
        if (func_ov217_021ba3b4(wk, BUTTON_LIST)) {
            func_ov217_021ba1c0(wk, TRUE);
            func_ov217_021ba1cc(wk, RESEARCH_TOP_NEXT_LIST);
            func_ov217_021ba254(wk, BUTTON_LIST);
            func_ov217_021ba284(wk->cursor);
            func_ov217_021ba40c(wk, ANIME_CURSOR);
            func_ov217_021ba3e8(wk, ANIME_DECIDE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov217_021ba380(wk, DECIDE_WAIT_FRAMES);
            func_ov217_021ba150(wk);
            func_ov217_021ba144(wk, SEQ_WAIT);
            func_ov217_021ba144(wk, SEQ_FADE_OUT);
            func_ov217_021ba144(wk, SEQ_TEARDOWN);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
    } else if (button == BUTTON_GRAPH) {
        if (func_ov217_021ba3b4(wk, BUTTON_GRAPH)) {
            func_ov217_021ba1c0(wk, TRUE);
            func_ov217_021ba1cc(wk, RESEARCH_TOP_NEXT_GRAPH);
            func_ov217_021ba254(wk, BUTTON_GRAPH);
            func_ov217_021ba284(wk->cursor);
            func_ov217_021ba40c(wk, ANIME_CURSOR);
            func_ov217_021ba3e8(wk, ANIME_DECIDE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov217_021ba380(wk, DECIDE_WAIT_FRAMES);
            func_ov217_021ba150(wk);
            func_ov217_021ba144(wk, SEQ_WAIT);
            func_ov217_021ba144(wk, SEQ_FADE_OUT);
            func_ov217_021ba144(wk, SEQ_TEARDOWN);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
    }
}

// pokebw2's ResearchTop_SeqCursor
static void func_ov217_021b9e44(ResearchTop *wk) {
    u32 keys = GCTX_HIDGetPressedKeys();
    s32 button = func_02035c08(wk->buttonRects);
    s32 commonButton = func_02035c08(func_ov217_021c09b8(wk->common));

    if (func_ov217_021ba4d4(wk) || commonButton == RESEARCH_COMMON_BUTTON_RETURN) {
        func_ov217_021ba1c0(wk, TRUE);
        func_ov217_021ba1cc(wk, RESEARCH_TOP_NEXT_EXIT);
        func_ov217_021ba31c(wk);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021ba380(wk, DECIDE_WAIT_FRAMES);
        func_ov217_021ba150(wk);
        func_ov217_021ba144(wk, SEQ_WAIT);
        func_ov217_021ba144(wk, SEQ_FADE_OUT);
        func_ov217_021ba144(wk, SEQ_TEARDOWN);
        return;
    }

    if (keys & PAD_KEY_UP) {
        func_ov217_021ba1f8(wk);
    }
    if (keys & PAD_KEY_DOWN) {
        func_ov217_021ba228(wk);
    }

    if (keys & PAD_BUTTON_A) {
        func_ov217_021ba1c0(wk, FALSE);
        func_ov217_021ba40c(wk, ANIME_CURSOR);
        func_ov217_021ba3e8(wk, ANIME_DECIDE);
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov217_021ba380(wk, DECIDE_WAIT_FRAMES);
        func_ov217_021ba150(wk);
        func_ov217_021ba144(wk, SEQ_WAIT);
        func_ov217_021ba144(wk, SEQ_FADE_OUT);
        func_ov217_021ba144(wk, SEQ_TEARDOWN);
        switch (wk->cursor) {
        case BUTTON_LIST:
            func_ov217_021ba1cc(wk, RESEARCH_TOP_NEXT_LIST);
            break;
        case BUTTON_GRAPH:
            func_ov217_021ba1cc(wk, RESEARCH_TOP_NEXT_GRAPH);
            break;
        default:
            func_ov217_021ba1cc(wk, RESEARCH_TOP_NEXT_EXIT);
            break;
        }
    } else if (button == BUTTON_LIST) {
        if (func_ov217_021ba3b4(wk, BUTTON_LIST)) {
            func_ov217_021ba1c0(wk, TRUE);
            func_ov217_021ba254(wk, BUTTON_LIST);
            func_ov217_021ba40c(wk, ANIME_CURSOR);
            func_ov217_021ba3e8(wk, ANIME_DECIDE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov217_021ba1cc(wk, RESEARCH_TOP_NEXT_LIST);
            func_ov217_021ba380(wk, DECIDE_WAIT_FRAMES);
            func_ov217_021ba150(wk);
            func_ov217_021ba144(wk, SEQ_WAIT);
            func_ov217_021ba144(wk, SEQ_FADE_OUT);
            func_ov217_021ba144(wk, SEQ_TEARDOWN);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
    } else if (button == BUTTON_GRAPH) {
        if (func_ov217_021ba3b4(wk, BUTTON_GRAPH)) {
            func_ov217_021ba1c0(wk, TRUE);
            func_ov217_021ba254(wk, BUTTON_GRAPH);
            func_ov217_021ba40c(wk, ANIME_CURSOR);
            func_ov217_021ba3e8(wk, ANIME_DECIDE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov217_021ba1cc(wk, RESEARCH_TOP_NEXT_GRAPH);
            func_ov217_021ba380(wk, DECIDE_WAIT_FRAMES);
            func_ov217_021ba150(wk);
            func_ov217_021ba144(wk, SEQ_WAIT);
            func_ov217_021ba144(wk, SEQ_FADE_OUT);
            func_ov217_021ba144(wk, SEQ_TEARDOWN);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
    } else if (keys & PAD_BUTTON_B) {
        func_ov217_021ba31c(wk);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov217_021ba1cc(wk, RESEARCH_TOP_NEXT_EXIT);
        func_ov217_021ba380(wk, DECIDE_WAIT_FRAMES);
        func_ov217_021ba150(wk);
        func_ov217_021ba144(wk, SEQ_WAIT);
        func_ov217_021ba144(wk, SEQ_FADE_OUT);
        func_ov217_021ba144(wk, SEQ_TEARDOWN);
    }
}

// pokebw2's ResearchTop_SeqWait
static void func_ov217_021ba078(ResearchTop *wk) {
    if (func_ov217_021ba384(wk) < func_ov217_021ba37c(wk)) {
        func_ov217_021ba150(wk);
    }
}

// pokebw2's ResearchTop_SeqFadeOut
static void func_ov217_021ba094(ResearchTop *wk) {
    switch (func_ov217_021ba190(wk)) {
    case 0:
        func_ov217_021ba364();
        func_ov217_021ba194(wk);
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            func_ov217_021ba150(wk);
        }
        break;
    }
}

// pokebw2's ResearchTop_SeqTeardown
static void func_ov217_021ba0c4(ResearchTop *wk) {
    func_ov217_021bab68(wk);
    func_ov217_021c09e0(wk->common);
    func_ov217_021c09ec(wk->common);
    func_ov217_021bab38(wk);
    func_ov217_021baaf4(wk);
    func_ov217_021baab8(wk);
    func_ov217_021baa28(wk);
    func_ov217_021ba960(wk);
    func_ov217_021ba9d0(wk);
    func_ov217_021ba8e0(wk);
    func_ov217_021ba848(wk);
    func_ov217_021ba82c(wk);
    func_ov217_021ba7d0(wk);
    func_ov217_021ba7b4(wk);
    func_ov217_021ba748(wk);
    func_ov217_021ba624(wk);
    func_ov217_021ba5d8(wk);
    wk->isEnd = TRUE;
    func_ov217_021ba144(wk, SEQ_END);
    func_ov217_021ba150(wk);
}

// pokebw2's ResearchTop_PushSeq
static void func_ov217_021ba144(ResearchTop *wk, u32 seq) {
    func_ov217_021c0e4c(wk->queue, seq);
}

// pokebw2's ResearchTop_EndSeq
static void func_ov217_021ba150(ResearchTop *wk) {
    wk->seqDone = TRUE;
}

// pokebw2's ResearchTop_UpdateSeq
static void func_ov217_021ba158(ResearchTop *wk) {
    if (wk->seqDone && !func_ov217_021c0e64(wk->queue)) {
        func_ov217_021ba180(wk, func_ov217_021c0e54(wk->queue));
    }
}

// pokebw2's ResearchTop_SetSeq
static void func_ov217_021ba180(ResearchTop *wk, u32 seq) {
    wk->seq = seq;
    wk->seqState = 0;
    wk->seqFrames = 0;
    wk->seqDone = FALSE;
}

// pokebw2's ResearchTop_GetSeqState
static u32 func_ov217_021ba190(ResearchTop *wk) {
    return wk->seqState;
}

// pokebw2's ResearchTop_NextSeqState
static void func_ov217_021ba194(ResearchTop *wk) {
    wk->seqState++;
}

// The sequence the screen starts with: the cursor's, when the screen was entered from another screen without touch
// pokebw2's ResearchTop_GetStartSeq
static u32 func_ov217_021ba19c(ResearchTop *wk) {
    ResearchCommon *common = wk->common;
    u32 prevSeq = func_ov217_021c09c8(common);
    BOOL touch = func_ov217_021c09cc(common);

    if (prevSeq != RESEARCH_SEQ_INIT && !touch) {
        return SEQ_CURSOR;
    }
    return SEQ_TOUCH;
}

// pokebw2's ResearchTop_SetTouchMode
static void func_ov217_021ba1c0(ResearchTop *wk, BOOL touch) {
    func_ov217_021c09c4(wk->common, touch);
}

// pokebw2's ResearchTop_SetNext
static void func_ov217_021ba1cc(ResearchTop *wk, u32 next) {
    wk->next = next;
}

// pokebw2's ResearchTop_CheckNewResult
static void func_ov217_021ba1d4(ResearchTop *wk) {
    if (!wk->newIconShown && func_020270e0() == TRUE) {
        wk->newIconShown = TRUE;
        func_ov217_021ba328(wk);
    }
}

// pokebw2's ResearchTop_CursorUp
static void func_ov217_021ba1f8(ResearchTop *wk) {
    func_ov217_021ba2bc(wk->cursor);
    func_ov217_021ba388(wk, -1);
    func_ov217_021ba284(wk->cursor);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
    func_ov217_021ba3e8(wk, ANIME_MOVE);
}

// pokebw2's ResearchTop_CursorDown
static void func_ov217_021ba228(ResearchTop *wk) {
    func_ov217_021ba2bc(wk->cursor);
    func_ov217_021ba388(wk, 1);
    func_ov217_021ba284(wk->cursor);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
    func_ov217_021ba3e8(wk, ANIME_MOVE);
}

// pokebw2's ResearchTop_MoveCursorTo
static void func_ov217_021ba254(ResearchTop *wk, int pos) {
    func_ov217_021ba2bc(wk->cursor);
    func_ov217_021ba3b0(wk, pos);
    func_ov217_021ba284(wk->cursor);
    GFL_SndSEPlay(SEQ_SE_SELECT1);
    func_ov217_021ba3e8(wk, ANIME_MOVE);
}

// pokebw2's ResearchTop_HighlightButton
static void func_ov217_021ba284(int button) {
    u8 bg, x, y, width, height, palette;

    switch (button) {
    case BUTTON_LIST:
        bg = BG_MAIN_BUTTONS;
        x = 3;
        y = 5;
        width = 26;
        height = 5;
        palette = 10;
        break;
    case BUTTON_GRAPH:
        bg = BG_MAIN_BUTTONS;
        x = 3;
        y = 12;
        width = 26;
        height = 5;
        palette = 10;
        break;
    default:
        return;
    }
    GFL_BGSysSetScrPaletteNo(bg, x, y, width, height, palette);
    GFL_BGSysLoadScr(bg);
}

// pokebw2's ResearchTop_UnhighlightButton
static void func_ov217_021ba2bc(int button) {
    u8 bg, x, y, width, height, palette;

    switch (button) {
    case BUTTON_LIST:
        bg = BG_MAIN_BUTTONS;
        x = 3;
        y = 5;
        width = 26;
        height = 5;
        palette = 5;
        break;
    case BUTTON_GRAPH:
        bg = BG_MAIN_BUTTONS;
        x = 3;
        y = 12;
        width = 26;
        height = 5;
        palette = 6;
        break;
    default:
        return;
    }
    GFL_BGSysSetScrPaletteNo(bg, x, y, width, height, palette);
    GFL_BGSysLoadScr(bg);
}

// pokebw2's ResearchTop_DisableListButton
static void func_ov217_021ba2fc(ResearchTop *wk) {
    func_ov217_021c1038(wk->bgFonts[2], 15);
    func_ov217_021ba3e8(wk, ANIME_DISABLED_1);
    func_ov217_021ba3e8(wk, ANIME_DISABLED_2);
}

// pokebw2's ResearchTop_StartReturnAnime
static void func_ov217_021ba31c(ResearchTop *wk) {
    func_ov217_021c09d8(wk->common, 0);
}

// pokebw2's ResearchTop_ShowNewIcon
static void func_ov217_021ba328(ResearchTop *wk) {
    ClActor *actor = func_ov217_021ba3e0(wk, 0);

    func_0204b3dc(actor, TRUE);
    func_0204b92c(actor, TRUE);
    func_0204b954(actor, FX32_ONE);
    func_0204b8c4(actor, 0);
}

// pokebw2's ResearchTop_FadeIn
static void func_ov217_021ba354(void) {
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
}

// pokebw2's ResearchTop_FadeOut
static void func_ov217_021ba364(void) {
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 0);
}

// pokebw2's ResearchTop_CountFrame
static void func_ov217_021ba374(ResearchTop *wk) {
    wk->seqFrames++;
}

// pokebw2's ResearchTop_GetSeqFrames
static u32 func_ov217_021ba37c(ResearchTop *wk) {
    return wk->seqFrames;
}

// pokebw2's ResearchTop_SetWait
static void func_ov217_021ba380(ResearchTop *wk, u32 frames) {
    wk->waitFrames = frames;
}

// pokebw2's ResearchTop_GetWait
static u32 func_ov217_021ba384(ResearchTop *wk) {
    return wk->waitFrames;
}

// pokebw2's ResearchTop_MoveCursor
static void func_ov217_021ba388(ResearchTop *wk, int dir) {
    int pos = wk->cursor;

    do {
        pos = (pos + dir + BUTTON_COUNT) % BUTTON_COUNT;
    } while (!func_ov217_021ba3b4(wk, pos));
    wk->cursor = pos;
}

// pokebw2's ResearchTop_SetCursor
static void func_ov217_021ba3b0(ResearchTop *wk, int pos) {
    wk->cursor = pos;
}

// pokebw2's ResearchTop_IsButtonEnabled
static BOOL func_ov217_021ba3b4(ResearchTop *wk, int button) {
    if (button == BUTTON_LIST) {
        if (func_ov217_021ba434(wk) > 1) {
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

// pokebw2's ResearchTop_GetObjRes
static u32 func_ov217_021ba3d0(ResearchTop *wk, u32 index) {
    return wk->objRes[index];
}

// pokebw2's ResearchTop_GetUnit
static ClActUnit *func_ov217_021ba3d8(ResearchTop *wk, u32 index) {
    return wk->units[index];
}

// pokebw2's ResearchTop_GetActor
static ClActor *func_ov217_021ba3e0(ResearchTop *wk, u32 index) {
    return wk->actors[index];
}

// pokebw2's ResearchTop_StartPaletteAnime
static void func_ov217_021ba3e8(ResearchTop *wk, u32 index) {
    func_ov217_021c29f8(wk->animes[index], data_ov217_021c2ff8[index].mode, data_ov217_021c2ff8[index].color);
}

// pokebw2's ResearchTop_StopPaletteAnime
static void func_ov217_021ba40c(ResearchTop *wk, u32 index) {
    func_ov217_021c2a00(wk->animes[index]);
}

// pokebw2's ResearchTop_UpdatePaletteAnimes
static void func_ov217_021ba41c(ResearchTop *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        func_ov217_021c29f0(wk->animes[i]);
    }
}

// pokebw2's ResearchTop_CountSurveys
static u8 func_ov217_021ba434(ResearchTop *wk) {
    EventWork *eventWork = GameData_GetEventWork(func_ov217_021c09b4(wk->common));
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
    return count;
}

// pokebw2's ResearchTop_GetCommon
static ResearchCommon *func_ov217_021ba4c8(ResearchTop *wk) {
    return wk->common;
}

// pokebw2's ResearchTop_SetCommon
static void func_ov217_021ba4cc(ResearchTop *wk, ResearchCommon *common) {
    wk->common = common;
}

// pokebw2's ResearchTop_SetHeapID
static void func_ov217_021ba4d0(ResearchTop *wk, HeapID heapId) {
    wk->heapId = heapId;
}

// pokebw2's ResearchTop_IsForceExit
static BOOL func_ov217_021ba4d4(ResearchTop *wk) {
    return func_ov217_021c09f8(func_ov217_021ba4c8(wk));
}

// pokebw2's ResearchTop_Alloc
static ResearchTop *func_ov217_021ba4e0(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(ResearchTop), FALSE, "research_top.c", 1486);
}

// pokebw2's ResearchTop_Free
static void func_ov217_021ba4fc(ResearchTop *wk) {
    GFL_HeapFree(wk);
}

// pokebw2's ResearchTop_InitWork
static void func_ov217_021ba504(ResearchTop *wk) {
    wk->seqDone = FALSE;
    wk->newIconShown = FALSE;
    wk->isEnd = FALSE;
    wk->seq = SEQ_SETUP;
    wk->seqFrames = 0;
    wk->waitFrames = 0;
    wk->cursor = BUTTON_LIST;
    wk->next = RESEARCH_TOP_NEXT_EXIT;
    wk->seqState = 0;
    func_ov217_021ba8fc(wk);
    func_ov217_021ba674(wk);
    func_ov217_021ba5e4(wk);
    func_ov217_021ba5b4(wk);
    func_ov217_021ba858(wk);
    func_ov217_021ba9e8(wk);
    func_ov217_021baa40(wk);
    func_ov217_021baac4(wk);
}

// Puts the cursor on the button of the screen it came back from, when that wasn't by touch
// pokebw2's ResearchTop_InitCursor
static void func_ov217_021ba560(ResearchTop *wk) {
    ResearchCommon *common = wk->common;
    u32 prevSeq = func_ov217_021c09c8(common);
    BOOL touch = func_ov217_021c09cc(common);
    int pos;

    if (prevSeq != RESEARCH_SEQ_INIT && !touch) {
        switch (prevSeq) {
        default:
        case RESEARCH_SEQ_LIST:
            pos = BUTTON_LIST;
            break;
        case RESEARCH_SEQ_GRAPH:
            pos = BUTTON_GRAPH;
            break;
        }
        func_ov217_021ba3b0(wk, pos);
        return;
    }

    if (func_ov217_021ba3b4(wk, BUTTON_LIST)) {
        func_ov217_021ba3b0(wk, BUTTON_LIST);
    } else {
        func_ov217_021ba3b0(wk, BUTTON_GRAPH);
    }
}

// pokebw2's ResearchTop_ClearFont
static void func_ov217_021ba5b4(ResearchTop *wk) {
    wk->font = NULL;
}

// pokebw2's ResearchTop_CreateFont
static void func_ov217_021ba5bc(ResearchTop *wk) {
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
}

// pokebw2's ResearchTop_DeleteFont
static void func_ov217_021ba5d8(ResearchTop *wk) {
    GFL_FontFree(wk->font);
}

// pokebw2's ResearchTop_ClearMsgData
static void func_ov217_021ba5e4(ResearchTop *wk) {
    int i;

    for (i = 0; i < MSG_DATA_COUNT; i++) {
        wk->msgData[i] = NULL;
    }
}

// pokebw2's ResearchTop_LoadMsgData
static void func_ov217_021ba5f8(ResearchTop *wk) {
    int i;

    for (i = 0; i < MSG_DATA_COUNT; i++) {
        wk->msgData[i] = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, data_ov217_021c2ed8[i], wk->heapId);
    }
}

// pokebw2's ResearchTop_FreeMsgData
static void func_ov217_021ba624(ResearchTop *wk) {
    int i;

    for (i = 0; i < MSG_DATA_COUNT; i++) {
        GFL_MsgDataFree(wk->msgData[i]);
    }
}

// pokebw2's ResearchTop_InitButtonRects
static void func_ov217_021ba63c(ResearchTop *wk) {
    int i;

    for (i = 0; i < BUTTON_RECT_COUNT; i++) {
        wk->buttonRects[i].left = data_ov217_021c2ee0[i].left;
        wk->buttonRects[i].right = data_ov217_021c2ee0[i].right;
        wk->buttonRects[i].top = data_ov217_021c2ee0[i].top;
        wk->buttonRects[i].bottom = data_ov217_021c2ee0[i].bottom;
    }
}

// pokebw2's ResearchTop_ClearQueue
static void func_ov217_021ba674(ResearchTop *wk) {
    wk->queue = NULL;
}

// pokebw2's ResearchTop_CreateQueue
static void func_ov217_021ba67c(ResearchTop *wk) {
    wk->queue = func_ov217_021c0e08(10, wk->heapId);
}

// pokebw2's ResearchTop_DeleteQueue
static void func_ov217_021ba68c(ResearchTop *wk) {
    func_ov217_021c0e3c(wk->queue);
}

// pokebw2's ResearchTop_InitBG
static void func_ov217_021ba698(ResearchTop *wk) {
    GFL_BGSysSetLCDConfig(&data_ov217_021c2eec);
    GFL_BGSysCreateBG(BG_SUB_TITLE, &data_ov217_021c2f78, BGMODE_TEXT);
    GFL_BGSysCreateBG(BG_SUB_TEXT, &data_ov217_021c2f18, BGMODE_TEXT);
    GFL_BGSysCreateBG(BG_MAIN_BUTTONS, &data_ov217_021c2f38, BGMODE_TEXT);
    GFL_BGSysCreateBG(BG_MAIN_TEXT, &data_ov217_021c2f58, BGMODE_TEXT);
    GFL_BGSysSetBGEnabled(RESEARCH_BG_SUB_BACK, TRUE);
    GFL_BGSysSetBGEnabled(RESEARCH_BG_SUB_PATTERN, TRUE);
    GFL_BGSysSetBGEnabled(BG_SUB_TITLE, TRUE);
    GFL_BGSysSetBGEnabled(BG_SUB_TEXT, TRUE);
    GFL_BGSysSetBGEnabled(RESEARCH_BG_MAIN_FRAME, TRUE);
    GFL_BGSysSetBGEnabled(BG_MAIN_BUTTONS, TRUE);
    GFL_BGSysSetBGEnabled(BG_MAIN_TEXT, TRUE);
    G2S_SetBlendAlpha(GX_PLANEMASK_BG1, GX_PLANEMASK_BG0, 7, 15);
    G2_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG1, 16, 5);
    BmpWin_InitAllocator(wk->heapId);
}

// pokebw2's ResearchTop_ExitBG
static void func_ov217_021ba748(ResearchTop *wk) {
    BmpWin_FreeAllocator();
    GFL_BGSysReleaseBG(BG_MAIN_TEXT);
    GFL_BGSysReleaseBG(BG_MAIN_BUTTONS);
    GFL_BGSysReleaseBG(BG_SUB_TEXT);
    GFL_BGSysReleaseBG(BG_SUB_TITLE);
}

// pokebw2's ResearchTop_LoadSubBG
static void func_ov217_021ba768(ResearchTop *wk) {
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, wk->heapId);
    void *file = GFL_ArcToolReadHeapNew(handle, 13, wk->heapId);
    NNSG2dScreenData *screen;

    NNS_G2dGetUnpackedScreenData(file, &screen);
    GFL_BGSysLoadScrAreaAll(BG_SUB_TITLE, screen->rawData, 0, 0, 32, 24);
    GFL_BGSysLoadScr(BG_SUB_TITLE);
    GFL_HeapFree(file);
    GFL_ArcToolFree(handle);
}

// pokebw2's ResearchTop_UnloadSubBG
static void func_ov217_021ba7b4(ResearchTop *wk) {
}

// pokebw2's ResearchTop_InitSubText
static void func_ov217_021ba7b8(ResearchTop *wk) {
    GFL_BGSysFillChar(BG_SUB_TEXT, 0, 1, 0);
    GFL_BGSysClearScr(BG_SUB_TEXT);
}

// pokebw2's ResearchTop_ExitSubText
static void func_ov217_021ba7d0(ResearchTop *wk) {
    GFL_BGSysFreeFilledChar(BG_SUB_TEXT, 1, 0);
}

// pokebw2's ResearchTop_LoadMainBG
static void func_ov217_021ba7e0(ResearchTop *wk) {
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, wk->heapId);
    void *file = GFL_ArcToolReadHeapNew(handle, 4, wk->heapId);
    NNSG2dScreenData *screen;

    NNS_G2dGetUnpackedScreenData(file, &screen);
    GFL_BGSysLoadScrAreaAll(BG_MAIN_BUTTONS, screen->rawData, 0, 0, 32, 24);
    GFL_BGSysLoadScr(BG_MAIN_BUTTONS);
    GFL_HeapFree(file);
    GFL_ArcToolFree(handle);
}

// pokebw2's ResearchTop_UnloadMainBG
static void func_ov217_021ba82c(ResearchTop *wk) {
}

// pokebw2's ResearchTop_InitMainText
static void func_ov217_021ba830(ResearchTop *wk) {
    GFL_BGSysFillChar(BG_MAIN_TEXT, 0, 1, 0);
    GFL_BGSysClearScr(BG_MAIN_TEXT);
}

// pokebw2's ResearchTop_ExitMainText
static void func_ov217_021ba848(ResearchTop *wk) {
    GFL_BGSysFreeFilledChar(BG_MAIN_TEXT, 1, 0);
}

// pokebw2's ResearchTop_ClearBGFonts
static void func_ov217_021ba858(ResearchTop *wk) {
    int i;

    for (i = 0; i < BG_FONT_COUNT; i++) {
        wk->bgFonts[i] = NULL;
    }
}

// pokebw2's ResearchTop_CreateBGFonts
static void func_ov217_021ba86c(ResearchTop *wk) {
    int i;

    for (i = 0; i < BG_FONT_COUNT; i++) {
        BGFontSetup setup;

        setup.window.bg = data_ov217_021c2f98[i].window.bg;
        setup.window.x = data_ov217_021c2f98[i].window.x;
        setup.window.y = data_ov217_021c2f98[i].window.y;
        setup.window.width = data_ov217_021c2f98[i].window.width;
        setup.window.height = data_ov217_021c2f98[i].window.height;
        setup.window.textX = data_ov217_021c2f98[i].window.textX;
        setup.window.textY = data_ov217_021c2f98[i].window.textY;
        setup.window.palette = data_ov217_021c2f98[i].window.palette;
        setup.window.letterColor = data_ov217_021c2f98[i].window.letterColor;
        setup.window.shadowColor = data_ov217_021c2f98[i].window.shadowColor;
        setup.window.backColor = data_ov217_021c2f98[i].window.backColor;
        setup.centered = data_ov217_021c2f98[i].centered;
        wk->bgFonts[i] = func_ov217_021c0ef4(&setup, wk->font, wk->msgData[data_ov217_021c2f98[i].msgDataIndex], wk->heapId);
        func_ov217_021c0f5c(wk->bgFonts[i], data_ov217_021c2f98[i].strId);
    }
}

// pokebw2's ResearchTop_DeleteBGFonts
static void func_ov217_021ba8e0(ResearchTop *wk) {
    int i;

    for (i = 0; i < BG_FONT_COUNT; i++) {
        func_ov217_021c0f48(wk->bgFonts[i]);
        wk->bgFonts[i] = NULL;
    }
}

// pokebw2's ResearchTop_ClearObjRes
static void func_ov217_021ba8fc(ResearchTop *wk) {
    int i;

    for (i = 0; i < OBJ_RES_COUNT; i++) {
        wk->objRes[i] = 0;
    }
}

// pokebw2's ResearchTop_LoadSubObjRes
static void func_ov217_021ba910(ResearchTop *wk) {
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

// pokebw2's ResearchTop_FreeSubObjRes
static void func_ov217_021ba960(ResearchTop *wk) {
    func_0204a8d4(wk->objRes[OBJ_RES_SUB_CHARS]);
    func_0204ada4(wk->objRes[OBJ_RES_SUB_PALETTE]);
    func_0204afd8(wk->objRes[OBJ_RES_SUB_CELL_ANIMS]);
}

// pokebw2's ResearchTop_LoadMainObjRes
static void func_ov217_021ba978(ResearchTop *wk) {
    HeapID heapId = wk->heapId;
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, heapId);
    u32 chars = func_0204a6c8(handle, 15, FALSE, CLACT_VRAM_MAIN, heapId);
    u32 palette = func_0204ac18(handle, 16, CLACT_VRAM_MAIN, 0xc0, 0, 4, heapId);
    u32 cellAnims = func_0204af28(handle, 14, 17, heapId);

    wk->objRes[OBJ_RES_MAIN_CHARS] = chars;
    wk->objRes[OBJ_RES_MAIN_PALETTE] = palette;
    wk->objRes[OBJ_RES_MAIN_CELL_ANIMS] = cellAnims;
    GFL_ArcToolFree(handle);
}

// pokebw2's ResearchTop_FreeMainObjRes
static void func_ov217_021ba9d0(ResearchTop *wk) {
    func_0204a8d4(wk->objRes[OBJ_RES_MAIN_CHARS]);
    func_0204ada4(wk->objRes[OBJ_RES_MAIN_PALETTE]);
    func_0204afd8(wk->objRes[OBJ_RES_MAIN_CELL_ANIMS]);
}

// pokebw2's ResearchTop_ClearUnits
static void func_ov217_021ba9e8(ResearchTop *wk) {
    int i;

    for (i = 0; i < UNIT_COUNT; i++) {
        wk->units[i] = NULL;
    }
}

// pokebw2's ResearchTop_CreateUnits
static void func_ov217_021ba9fc(ResearchTop *wk) {
    int i;

    for (i = 0; i < UNIT_COUNT; i++) {
        wk->units[i] = func_0204b100(data_ov217_021c2ed2[i], data_ov217_021c2ed0[i], wk->heapId);
    }
}

// pokebw2's ResearchTop_DeleteUnits
static void func_ov217_021baa28(ResearchTop *wk) {
    int i;

    for (i = 0; i < UNIT_COUNT; i++) {
        func_0204b1cc(wk->units[i]);
    }
}

// pokebw2's ResearchTop_ClearActors
static void func_ov217_021baa40(ResearchTop *wk) {
    wk->actors[0] = NULL;
}

// pokebw2's ResearchTop_CreateActors
static void func_ov217_021baa48(ResearchTop *wk) {
    int i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        const ResearchActorSetup *entry = &data_ov217_021c2efc[i];
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
        unit = func_ov217_021ba3d8(wk, entry->unit);
        chars = func_ov217_021ba3d0(wk, entry->chars);
        palette = func_ov217_021ba3d0(wk, entry->palette);
        cellAnims = func_ov217_021ba3d0(wk, entry->cellAnims);
        wk->actors[i] = func_0204b294(unit, chars, palette, cellAnims, &setup, entry->surface, wk->heapId);
        func_0204b3dc(wk->actors[i], FALSE);
    }
}

// pokebw2's ResearchTop_DeleteActors
static void func_ov217_021baab8(ResearchTop *wk) {
    func_0204b3b4(wk->actors[0]);
}

// pokebw2's ResearchTop_ClearPaletteAnimes
static void func_ov217_021baac4(ResearchTop *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        wk->animes[i] = NULL;
    }
}

// pokebw2's ResearchTop_CreatePaletteAnimes
static void func_ov217_021baad8(ResearchTop *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        wk->animes[i] = func_ov217_021c29c8(wk->heapId);
    }
}

// pokebw2's ResearchTop_DeletePaletteAnimes
static void func_ov217_021baaf4(ResearchTop *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        func_ov217_021c29d8(wk->animes[i]);
    }
}

// pokebw2's ResearchTop_SetupPaletteAnimes
static void func_ov217_021bab0c(ResearchTop *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        func_ov217_021c29e8(wk->animes[i], data_ov217_021c2ff8[i].dst, data_ov217_021c2ff8[i].src,
                           data_ov217_021c2ff8[i].count);
    }
}

// pokebw2's ResearchTop_RestorePalettes
static void func_ov217_021bab38(ResearchTop *wk) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        func_ov217_021c2a08(wk->animes[i]);
    }
}

// pokebw2's ResearchTop_VBlank
static void func_ov217_021bab50(void *data) {
    func_0204a648();
}

// pokebw2's ResearchTop_SetVBlank
static void func_ov217_021bab58(ResearchTop *wk) {
    GFL_VBlankSetCallback(func_ov217_021bab50, NULL);
}

// pokebw2's ResearchTop_ResetVBlank
static void func_ov217_021bab68(ResearchTop *wk) {
    GFL_VBlankResetCallback();
}

// pokebw2's ResearchTop_ShowCommIcon
static void func_ov217_021bab70(ResearchTop *wk) {
    func_0203d34c(TRUE, wk->heapId);
}
