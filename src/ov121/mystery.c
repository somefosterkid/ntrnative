// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov197/mystery.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mystery/mystery_album.h"
#include "app/mystery/mystery_check.h"
#include "app/mystery/mystery_gift_data.h"
#include "app/mystery/mystery_graphic.h"
#include "app/mystery/mystery_net.h"
#include "app/mystery/mystery_util.h"
#include "app/wifi_login.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/script_text_banks.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/particle.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "nitro/os.h"
#include "nitro/rtc.h"
#include "pml/item.h"
#include "pml/poke_graphic.h"
#include "pml/poke_party.h"
#include "pml/species_names.h"
#include "save/mystery_gift.h"
#include "save/player_info.h"
#include "system/bmp_oam.h"
#include "system/bmp_winframe.h"
#include "system/dsi.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/version.h"
#include "system/wordset.h"
#include "gfl/rtc_cache.h"
#include "work_area.h"

// Mystery Gift, from the start menu: receiving gifts by wireless, infrared or Wi-Fi, the effect of a received gift
// and the album of cards. Our names; swan has none for this overlay

// The menus of func_ov121_021d81b0
enum {
    LIST_TOP,
    LIST_RECEIVE,
    LIST_RECEIVED,
    LIST_ABOUT,
};

// How a gift is received, from the receive menu
enum {
    RECEIVE_WIRELESS,
    RECEIVE_WIFI,
    RECEIVE_INFRARED,
};

// The BGs' graphics: the main screens', the window of a received card's text, and the effect's
typedef struct {
    HeapID heapId;
} MysteryBg;

// The falling stars on both screens, the menu's cursor and the hint of the receive effect, and the colors the
// cursor's palette cycles through
typedef struct {
    u32 palette;
    u32 chars;
    u32 cellAnims;
    // The cursor, the stars, then the hint
    ClActor *actors[35];
    u16 palFrame;
    u16 colors[16];
    u16 toColors[16];
    u16 fromColors[16];
} MysteryActors;

#define ACTOR_CURSOR 0
#define ACTOR_STARS 1
#define STAR_COUNT 32
#define ACTOR_HINT 34

// The particles of a special gift's effect
typedef struct {
    ParticleSystem *sys;
    u32 unk4;
    void *work;
    BOOL active;
} MysteryParticle;

typedef struct MysteryBgScroll MysteryBgScroll;

// Scrolls the BGs behind the menus and moves the stars with them
struct MysteryBgScroll {
    BOOL active;
    BOOL enabled;
    u16 frame;
    fx32 pos;
    fx32 speed;
    // The speed when it last changed, which the speed eases from
    fx32 startSpeed;
    fx32 starY[STAR_COUNT];
    fx32 starSpeed[STAR_COUNT];
    MysteryActors *actors;
    void (*func)(MysteryBgScroll *scroll);
    u32 unk120;
    HeapID heapId;
};

typedef struct MysteryEffect MysteryEffect;

// The effect of a received gift: its Pokémon, item or other icon falls from the top screen, and the album opens
struct MysteryEffect {
    HeapID heapId;
    ClActor *actor;
    BOOL done;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    u32 frame;
    u32 subFrame;
    u32 state;
    MysteryBg *bg;
    MysteryBgScroll *scroll;
    void (*func)(MysteryEffect *effect);
    u16 blend;
    u16 bgFadeColors[16];
    u16 subFadeColors[16];
    // The palette of the gift's kind, and the normal one
    u16 kindColors[16];
    u16 baseColors[16];
    u16 objFadeColors[16];
    u16 whiteColors[16];
    u16 objColors[16];
    const MysteryGiftRecvData *recv;
    MysteryParticle particle;
    // Where the icon stops, from the bottom of the Pokémon's sprite
    s16 height;
    s16 offsetX;
};

// The button prompt shown as an OAM text over the album
typedef struct {
    BmpOamSys *bmpOam;
    MysteryOamText *text;
    const MysteryActors *actors;
} MysteryOamMsg;

typedef struct {
    MysteryAlbum *album;
    MysteryCardRes *cardRes;
    MysteryCardView *cardView;
    MysteryEffect effect;
    MysteryBgScroll scroll;
    MysteryYesNo *yesNo;
    MysteryOamMsg oamMsg;
    MysteryList *list;
    u32 listCursors[4];
    MysteryMsgWin *msgWin;
    MysteryTextWin *unk288;
    MysteryTextWin *cardWin;
    MysterySeq *seq;
    MysteryActors actors;
    MysteryBg bg;
    MysteryGraphic *graphic;
    Font *font;
    PrintQueue *queue;
    MsgData *scriptMsgData;
    MsgData *msgData;
    WordSet *wordSet;
    u32 receiveMode;
    MysteryNet *net;
    void *param;
    GameData *gameData;
    MysteryGiftSave *giftSave;
    WifiLoginParam *loginParam;
    u32 wait;
    // Set when the album was full and has a free slot again, to go on with receiving
    BOOL resumeReceive;
    MysteryGiftRecvData recv;
} MysteryWork;

BOOL func_ov121_021d6660(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov121_021d67d4(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov121_021d6888(GameProc *proc, u32 *state, void *param, void *work);
void func_ov121_021d6924(MysteryBg *bg, HeapID heapId);
void func_ov121_021d6948(MysteryBg *bg);
static void func_ov121_021d694c(MysteryBg *bg, u32 mode);
void func_ov121_021d6aec(MysteryActors *actors, ClActUnit *unit, HeapID heapId);
void func_ov121_021d6c28(MysteryActors *actors);
ClActor *func_ov121_021d6c58(const MysteryActors *actors, u32 index);
void func_ov121_021d6c60(MysteryActors *actors, HeapID heapId);
static void func_ov121_021d6c84(MysteryActors *actors);
void func_ov121_021d6cec(MysteryActors *actors);
void func_ov121_021d6cf4(MysteryParticle *particle, HeapID heapId);
void func_ov121_021d6d38(MysteryParticle *particle);
void func_ov121_021d6d50(MysteryParticle *particle, u32 arcId, u32 fileId, HeapID heapId);
void func_ov121_021d6d6c(MysteryParticle *particle, int emitter);
BOOL func_ov121_021d6d7c(MysteryParticle *particle);
void func_ov121_021d6d80(MysterySeq *seq, u32 *unused, void *work);
static void func_ov121_021d6db4(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021d6df4(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021d6e3c(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021d72f0(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021d73dc(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021d7814(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021d7b4c(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021d7cf4(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021d7eac(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021d80c0(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021d80f0(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021d8138(MysteryWork *wk, u32 unused, HeapID heapId);
void func_ov121_021d8198(MysteryWork *wk);
void func_ov121_021d81b0(MysteryWork *wk, u32 type, HeapID heapId);
void func_ov121_021d8380(MysteryWork *wk);
u32 func_ov121_021d83b0(MysteryWork *wk);
void func_ov121_021d83cc(void *work);
void func_ov121_021d83dc(MysteryWork *wk, HeapID heapId);
static void func_ov121_021d849c(MysteryWork *wk);
void func_ov121_021d84d4(MysteryEffect *effect, ClActUnit *unit, const MysteryGiftRecvData *recv, GameData *gameData, MysteryActors *actors, MysteryBg *bg, MysteryBgScroll *scroll, HeapID heapId);
void func_ov121_021d8c1c(MysteryEffect *effect, u32 mode);
void func_ov121_021d8ca8(MysteryEffect *effect);
void func_ov121_021d8d00(MysteryEffect *effect);
BOOL func_ov121_021d8d18(MysteryEffect *effect);
static void func_ov121_021d8d1c(MysteryEffect *effect);
void func_ov121_021d8ea4(MysteryEffect *effect);
void func_ov121_021d9138(MysteryEffect *effect);
void func_ov121_021d91bc(MysteryBgScroll *scroll, MysteryActors *actors, HeapID heapId);
void func_ov121_021d9228(MysteryBgScroll *scroll);
static void func_ov121_021d9238(MysteryBgScroll *scroll);
void func_ov121_021d9254(MysteryBgScroll *scroll, u32 mode);
void func_ov121_021d9288(MysteryBgScroll *scroll, BOOL enabled);
static void func_ov121_021d928c(MysteryBgScroll *scroll);
static void func_ov121_021d933c(MysteryBgScroll *scroll);
void func_ov121_021d93f4(MysteryOamMsg *msg, const MysteryActors *actors, ClActUnit *unit, PrintQueue *queue,
                               MsgData *msgData, u32 msgId, Font *font, HeapID heapId);
void func_ov121_021d9488(MysteryOamMsg *msg);
BOOL func_ov121_021d94b8(MysteryOamMsg *msg);
u32 func_ov121_021d94c4(BoxPkm *pkm, HeapID heapId);
BOOL func_ov121_021d9554(const MysteryGiftRecvData *recv, u32 errors);

// The BG palettes of the main and of the sub screen. No code loads from it: its reads have constant indices
static const u8 sBgPalTypes[2] = { PALTYPE_MAIN_BG, PALTYPE_SUB_BG };

// The palette of the stars and the effect for each kind of gift
// pokebw2's sKindPalettes
static const u16 data_ov121_021df4f2[5] = { 0, 1, 2, 6, 3 };

const GameProcFunctions data_ov121_021df538 = {
    func_ov121_021d6660,
    func_ov121_021d6888,
    func_ov121_021d67d4,
};

// pokebw2's MysteryProc_Main

BOOL func_ov121_021d6660(GameProc *proc, u32 *state, void *param, void *work) {
    MysteryWork *wk;

    GFL_OvlLoad(OVERLAY_ID(114));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_MYSTERY, 0xa0000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(MysteryWork), HEAPID_MYSTERY);
    MI_CpuFill8(wk, 0, sizeof(MysteryWork));
    wk->param = param;
    wk->gameData = GameData_Create(HEAPID_MYSTERY);
    wk->giftSave = mysteryGiftBlock(GameData_GetSaveControl(wk->gameData), 1, HEAPID_MYSTERY);
    wk->graphic = func_ov121_021d9624(1, HEAPID_MYSTERY);
    func_ov121_021d6924(&wk->bg, HEAPID_MYSTERY);
    func_ov121_021d6aec(&wk->actors, func_ov121_021d9744(wk->graphic), HEAPID_MYSTERY);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_MYSTERY);
    wk->queue = func_0201c440(HEAPID_MYSTERY);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_14, HEAPID_MYSTERY);
    wk->scriptMsgData = GFL_MsgSysLoadData(FALSE, ARCID_SCRIPT_MESSAGE, SCRIPT_TEXT_GLOBAL_10520, HEAPID_MYSTERY);
    wk->wordSet = GFL_WordSetSystemCreateDefault(HEAPID_MYSTERY);
    wk->seq = func_ov121_021da854(wk, func_ov121_021d6d80, HEAPID_MYSTERY);
    wk->msgWin = func_ov121_021d9ca4(1, 15, wk->queue, wk->font, HEAPID_MYSTERY);
    func_ov121_021d9ffc(wk->msgWin, 10, 14);
    wk->net = func_ov121_021de2a0(GameData_GetSaveControl(wk->gameData), HEAPID_MYSTERY);
    func_ov121_021d91bc(&wk->scroll, &wk->actors, HEAPID_MYSTERY);
    func_ov121_021d9254(&wk->scroll, 0);
    func_ov121_021d9288(&wk->scroll, TRUE);
    GFL_SndBGMPlay(SEQ_BGM_WIFI_PRESENT, 0xffff);
    return TRUE;
}

BOOL func_ov121_021d67d4(GameProc *proc, u32 *state, void *param, void *work) {
    MysteryWork *wk = work;

    func_ov121_021d9228(&wk->scroll);
    func_ov121_021de2f0(wk->net);
    if (wk->msgWin != NULL) {
        func_ov121_021d9e04(wk->msgWin);
    }
    func_ov121_021da890(wk->seq);
    GFL_WordSetSystemFree(wk->wordSet);
    GFL_MsgDataFree(wk->scriptMsgData);
    GFL_MsgDataFree(wk->msgData);
    func_0201c4c0(wk->queue);
    GFL_FontFree(wk->font);
    func_ov121_021d6c28(&wk->actors);
    func_ov121_021d6948(&wk->bg);
    func_ov121_021d96c4(wk->graphic);
    func_0200a338(wk->giftSave);
    GameData_Free(wk->gameData);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_MYSTERY);
    OS_ResetSystem(0);
    GFL_OvlUnload(OVERLAY_ID(114));
    func_02005d28();
    return TRUE;
}


static BOOL func_ov121_021d6888(GameProc *proc, u32 *state, void *param, void *work) {
    MysteryWork *wk = work;

    func_ov121_021de300(wk->net);
    func_ov121_021da898(wk->seq);
    if (wk->graphic != NULL) {
        func_ov121_021d9718(wk->graphic);
        func_ov121_021d972c(wk->graphic);
        func_ov121_021d9738(wk->graphic);
    }
    func_ov121_021d9238(&wk->scroll);
    func_0201c4e4(wk->queue);
    if (wk->msgWin != NULL) {
        func_ov121_021d9e50(wk->msgWin);
    }
    if (wk->cardWin != NULL) {
        func_ov121_021da2c8(wk->cardWin);
    }
    if (wk->unk288 != NULL) {
        func_ov121_021da2c8(wk->unk288);
    }
    if (wk->list != NULL) {
        func_ov121_021da840(wk->list);
    }
    if (func_ov121_021da8ac(wk->seq)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's MysteryBg_Init
void func_ov121_021d6924(MysteryBg *bg, HeapID heapId) {
    MI_CpuFill8(bg, 0, sizeof(MysteryBg));
    bg->heapId = heapId;
    func_ov121_021d694c(bg, 0);
    GFL_BGSysSetBGEnabled(6, TRUE);
}

// pokebw2's MysteryBg_Exit
void func_ov121_021d6948(MysteryBg *bg) {
}

// pokebw2's MysteryBg_Load
static void func_ov121_021d694c(MysteryBg *bg, u32 mode) {
    HeapID heapId = HEAPID_TAIL(bg->heapId);
    ArcTool *arc;

    switch (mode) {
    case 0:
        arc = GFL_ArcSysCreateFileHandle(0x22, heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, 6, sBgPalTypes[0], 0, 0, heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, 6, sBgPalTypes[1], 0, 0x80, heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, 7, sBgPalTypes[1], 0x20, 0x20, heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 15, 3, 0, 0, FALSE, heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 15, 7, 0, 0, FALSE, heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 31, 3, 0, 0, FALSE, heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 31, 7, 0, 0, FALSE, heapId);
        GFL_ArcToolFree(arc);
        arc = GFL_ArcSysCreateFileHandle(ARCID_FONT, heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, 5, sBgPalTypes[0], 15 * 0x20, 0x20, heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, 5, sBgPalTypes[1], 15 * 0x20, 0x20, heapId);
        GFL_ArcToolFree(arc);
        LoadSysMsgBox(1, 10, 14, 0, heapId);
        LoadSysMsgBox(0, 1, 13, 0, heapId);
        GFL_BGSysClearCharCore(1, 0x20, 0, heapId);
        GFL_BGSysClearCharCore(0, 0x20, 0, heapId);
        break;
    case 1:
        arc = GFL_ArcSysCreateFileHandle(0x22, heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, 6, sBgPalTypes[1], 0, 0xc0, heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 15, 6, 0, 0, FALSE, heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 21, 6, 0, 0, FALSE, heapId);
        GFL_ArcToolFree(arc);
        break;
    case 2:
        arc = GFL_ArcSysCreateFileHandle(0x22, heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 11, 2, 0, 0, FALSE, heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 28, 2, 0, 0, FALSE, heapId);
        GFL_ArcToolFree(arc);
        break;
    }
}

// pokebw2's MysteryActors_Init
void func_ov121_021d6aec(MysteryActors *actors, ClActUnit *unit, HeapID heapId) {
    ArcTool *arc;
    NNSG2dPaletteData *palette;
    void *buffer;
    u16 *colors;
    ClActorSetup setup;
    int i;

    arc = GFL_ArcSysCreateFileHandle(0x22, heapId);
    actors->palette = func_0204abf0(arc, 8, CLACT_VRAM_BOTH, 0, heapId);
    actors->cellAnims = func_0204af28(arc, 34, 37, heapId);
    actors->chars = func_0204a6c8(arc, 16, FALSE, CLACT_VRAM_BOTH, heapId);
    buffer = GFL_G2DIOReadNCLRArc(arc, 8, &palette, heapId);
    colors = palette->pRawData;
    MI_CpuCopy8(colors, actors->fromColors, sizeof(actors->fromColors));
    colors += 16;
    MI_CpuCopy8(colors, actors->toColors, sizeof(actors->toColors));
    GFL_HeapFree(buffer);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&setup, 0, sizeof(setup));
    actors->actors[ACTOR_CURSOR] =
        func_0204b294(unit, actors->chars, actors->palette, actors->cellAnims, &setup, 0, heapId);
    func_0204b3dc(actors->actors[ACTOR_CURSOR], FALSE);
    for (i = 0; i < STAR_COUNT; i++) {
        setup.x = GFL_RandomLCAlt(256);
        setup.y = GFL_RandomLCAlt(256) - 272;
        setup.bgPriority = 3;
        setup.sequence = GFL_RandomLCAlt(6) + 3;
        actors->actors[ACTOR_STARS + i] =
            func_0204b294(unit, actors->chars, actors->palette, actors->cellAnims, &setup, 0, heapId);
        func_0204b3dc(actors->actors[ACTOR_STARS + i], FALSE);
    }
    setup.x = 128;
    setup.y = 177;
    setup.bgPriority = 0;
    setup.priority = 1;
    setup.sequence = 9;
    actors->actors[ACTOR_HINT] =
        func_0204b294(unit, actors->chars, actors->palette, actors->cellAnims, &setup, 0, heapId);
    func_0204b3dc(actors->actors[ACTOR_HINT], FALSE);
}

// pokebw2's MysteryActors_Exit
void func_ov121_021d6c28(MysteryActors *actors) {
    int i;

    for (i = 0; i < 35; i++) {
        if (actors->actors[i] != NULL) {
            func_0204b3b4(actors->actors[i]);
        }
    }
    func_0204ada4(actors->palette);
    func_0204a8d4(actors->chars);
    func_0204afd8(actors->cellAnims);
}

// pokebw2's MysteryActors_GetActor
ClActor *func_ov121_021d6c58(const MysteryActors *actors, u32 index) {
    return actors->actors[index];
}

// pokebw2's MysteryActors_LoadPalette
void func_ov121_021d6c60(MysteryActors *actors, HeapID heapId) {
    NNSG2dPaletteData *palette;
    void *buffer = GFL_G2DIOReadNCLR(0x22, 8, &palette, heapId);

    func_0204adec(actors->palette, palette, 7);
    GFL_HeapFree(buffer);
}

// pokebw2's MysteryActors_Update
static void func_ov121_021d6c84(MysteryActors *actors) {
    int i;

    if (actors->palFrame + 0x400 >= 0x10000) {
        actors->palFrame = actors->palFrame + 0x400 - 0x10000;
    } else {
        actors->palFrame += 0x400;
    }
    for (i = 0; i < 5; i++) {
        func_ov121_021daaa4(14, &actors->colors[i], actors->palFrame, 0, i, actors->toColors[i], actors->fromColors[i]);
    }
}

// pokebw2's MysteryActors_ResetPalAnim
void func_ov121_021d6cec(MysteryActors *actors) {
    actors->palFrame = 0;
}

// pokebw2's MysteryParticle_Init
void func_ov121_021d6cf4(MysteryParticle *particle, HeapID heapId) {
    MI_CpuFill8(particle, 0, sizeof(MysteryParticle));
    particle->active = TRUE;
    // the SPL manager's work, whose emitters and particles hold pointers
    particle->work = GFL_HeapAllocate(heapId, WORK_AREA_SIZE(0x4800), FALSE, "mystery.c", 1154);
    particle->sys = func_02050c04(particle->work, WORK_AREA_SIZE(0x4800), TRUE, heapId);
}

// pokebw2's MysteryParticle_Exit
void func_ov121_021d6d38(MysteryParticle *particle) {
    GFL_HeapFree(particle->work);
    MI_CpuFill8(particle, 0, sizeof(MysteryParticle));
}

// pokebw2's MysteryParticle_Load
void func_ov121_021d6d50(MysteryParticle *particle, u32 arcId, u32 fileId, HeapID heapId) {
    func_02051230(particle->sys, func_0205121c(arcId, fileId, heapId), TRUE, NULL);
}

// pokebw2's MysteryParticle_Emit
void func_ov121_021d6d6c(MysteryParticle *particle, int emitter) {
    func_020515cc(particle->sys, emitter, NULL, particle);
}

// pokebw2's MysteryParticle_IsActive
BOOL func_ov121_021d6d7c(MysteryParticle *particle) {
    return particle->active;
}

void func_ov121_021d6d80(MysterySeq *seq, u32 *unused, void *work) {
    MysteryWork *wk = work;

    func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x4f, 0);
    func_ov121_021d81b0(wk, 0, HEAPID_MYSTERY);
    func_ov121_021da8b0(seq, func_ov121_021d6db4);
}


// pokebw2's MysterySeq_FadeIn
static void func_ov121_021d6db4(MysterySeq *seq, u32 *state, void *work) {
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
        func_ov121_021da8b0(seq, func_ov121_021d6e3c);
        break;
    }
}

// pokebw2's MysterySeq_FadeOut
void func_ov121_021d6df4(MysterySeq *seq, u32 *state, void *work) {
    switch (*state) {
    case 0:
        GFL_FadeSet(FADE_ENGINE_A_WHITE | FADE_ENGINE_B_WHITE, 0, 16, 0);
        GFL_SndBGMFadeOut(8);
        *state = 1;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            *state = 2;
        }
        break;
    case 2:
        func_ov121_021da8b0(seq, func_ov121_021d80c0);
        break;
    }
}

void func_ov121_021d6e3c(MysterySeq *seq, u32 *state, void *work) {
    MysteryWork *wk = work;
    u32 ret;
    u32 i;
    BOOL noneDelivered;
    BOOL full;

    switch (*state) {
    case 0:
        if (wk->resumeReceive) {
            wk->resumeReceive = FALSE;
            *state = 2;
            break;
        }
        func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x4f, 0);
        if (wk->list == NULL) {
            func_ov121_021d81b0(wk, 0, HEAPID_MYSTERY);
        }
        *state = 1;
        break;
    case 1:
        ret = func_ov121_021d83b0(wk);
        if (ret == MYSTERY_MENU_NONE) {
            break;
        }
        if (ret == 0) {
            noneDelivered = TRUE;
            func_ov121_021d8380(wk);
            full = func_0200a0c8(wk->giftSave) == 0 ? noneDelivered : FALSE;
            if (full) {
                for (i = 0; i < func_0200a348(wk->giftSave); i++) {
                    if (func_0200a104(wk->giftSave, i)) {
                        noneDelivered = FALSE;
                    }
                }
            }
            if (full) {
                if (noneDelivered) {
                    func_ov121_021d9f08(wk->msgWin, wk->msgData, func_0200a350(wk->giftSave) == 0 ? 0x92 : 0x89, 1);
                    func_ov121_021da8c0(seq, 0);
                    *state = 19;
                } else {
                    *state = 16;
                }
            } else if (!func_0202eb44()) {
                func_ov121_021da8b0(seq, func_ov121_021d80f0);
            } else {
                *state = 2;
            }
        } else if (ret == 1) {
            func_ov121_021d8380(wk);
            func_ov121_021da8b0(seq, func_ov121_021d7b4c);
        } else if (ret == 2) {
            func_ov121_021d8380(wk);
            func_ov121_021da8b0(seq, func_ov121_021d72f0);
        } else if (ret == 3) {
            func_ov121_021da8b0(seq, func_ov121_021d6df4);
        }
        break;
    case 2:
        func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x53, 1);
        func_ov121_021da8c0(seq, 3);
        *state = 19;
        break;
    case 3:
        func_ov121_021d8138(wk, 0, HEAPID_MYSTERY);
        *state = 4;
        break;
    case 4:
        ret = func_ov121_021da178(wk->yesNo);
        if (ret != MYSTERY_MENU_NONE) {
            func_ov121_021d8198(wk);
            if (ret == 0) {
                *state = 5;
            } else if (ret == 1) {
                *state = 0;
            }
        }
        break;
    case 5:
        func_ov121_021de308(wk->net, 1);
        *state = 6;
        break;
    case 6:
        if (func_ov121_021de3e0(wk->net) == 2) {
            *state = 7;
        }
        break;
    case 7:
        func_ov121_021d8380(wk);
        func_ov121_021d81b0(wk, 1, HEAPID_MYSTERY);
        func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x56, 0);
        *state = 8;
        break;
    case 8:
        if (func_ov121_021de4d8(wk->net) & 0x20) {
            func_ov121_021da84c(wk->list, 0, TRUE);
        } else {
            func_ov121_021da84c(wk->list, 0, FALSE);
        }
        ret = func_ov121_021d83b0(wk);
        if (ret == MYSTERY_MENU_NONE) {
            break;
        }
        func_ov121_021d8380(wk);
        switch (ret) {
        case 0:
            wk->receiveMode = 0;
            *state = 9;
            break;
        case 1:
            wk->receiveMode = 1;
            *state = 12;
            break;
        case 2:
            wk->receiveMode = 2;
            *state = 9;
            break;
        case 3:
            *state = 14;
            break;
        }
        break;
    case 9:
        switch (wk->receiveMode) {
        case 0:
            func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x60, 1);
            func_ov121_021da8c0(seq, 10);
            *state = 19;
            break;
        case 1:
            break;
        case 2:
            func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x66, 1);
            func_ov121_021da8c0(seq, 10);
            *state = 19;
            break;
        }
        break;
    case 10:
        func_ov121_021d8138(wk, 0, HEAPID_MYSTERY);
        *state = 11;
        break;
    case 11:
        ret = func_ov121_021da178(wk->yesNo);
        if (ret != MYSTERY_MENU_NONE) {
            func_ov121_021d8198(wk);
            if (ret == 0) {
                *state = 12;
            } else if (ret == 1) {
                *state = 7;
            }
        }
        break;
    case 12:
        if (func_ov121_021de3e0(wk->net) == 2) {
            func_ov121_021de308(wk->net, 3);
            *state = 13;
        }
        break;
    case 13:
        if (func_ov121_021de3e0(wk->net) == 0) {
            if (wk->receiveMode == 1) {
                func_ov121_021da8b0(seq, func_ov121_021d7eac);
            } else {
                func_ov121_021da8b0(seq, func_ov121_021d73dc);
            }
        }
        break;
    case 14:
        if (func_ov121_021de3e0(wk->net) == 2) {
            func_ov121_021de308(wk->net, 3);
            *state = 15;
        } else {
            *state = 0;
        }
        break;
    case 15:
        if (func_ov121_021de3e0(wk->net) == 0) {
            *state = 0;
        }
        break;
    case 16:
        func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x8d, 1);
        func_ov121_021da8c0(seq, 17);
        *state = 19;
        break;
    case 17:
        func_ov121_021d8138(wk, 0, HEAPID_MYSTERY);
        *state = 18;
        break;
    case 18:
        ret = func_ov121_021da178(wk->yesNo);
        if (ret != MYSTERY_MENU_NONE) {
            func_ov121_021d8198(wk);
            if (ret == 0) {
                func_ov121_021da8b0(seq, func_ov121_021d7cf4);
            } else if (ret == 1) {
                *state = 0;
            }
        }
        break;
    case 19:
        if (func_ov121_021d9ff8(wk->msgWin)) {
            func_ov121_021da8c4(seq);
        }
        break;
    }
}


void func_ov121_021d72f0(MysterySeq *seq, u32 *state, void *work) {
    MysteryWork *wk = work;
    u32 ret;

    switch (*state) {
    case 0:
        func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x7e, 0);
        func_ov121_021d81b0(wk, 3, HEAPID_MYSTERY);
        *state = 2;
        break;
    case 2:
        ret = func_ov121_021d83b0(wk);
        if (ret == MYSTERY_MENU_NONE) {
            break;
        }
        func_ov121_021d8380(wk);
        if (ret == 0) {
            func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x84, MYSTERY_PRINT_STREAM);
            func_ov121_021da8c0(seq, 0);
            *state = 3;
        } else if (ret == 1) {
            func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x85, MYSTERY_PRINT_STREAM);
            func_ov121_021da8c0(seq, 0);
            *state = 3;
        } else if (ret == 2) {
            func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x86, MYSTERY_PRINT_STREAM);
            func_ov121_021da8c0(seq, 0);
            *state = 3;
        } else if (ret == 3) {
            func_ov121_021da8b0(seq, func_ov121_021d6e3c);
        }
        break;
    case 3:
        if (func_ov121_021d9ff8(wk->msgWin)) {
            func_ov121_021da8c4(seq);
        }
        break;
    }
}

void func_ov121_021d73dc(MysterySeq *seq, u32 *state, void *work) {
    MysteryWork *wk = work;
    int ret;
    BOOL cancel;
    RTCDate date;

    switch (*state) {
    case 0:
        switch (wk->receiveMode) {
        case 1:
            func_ov121_021de308(wk->net, 4);
            break;
        case 0:
            func_ov121_021de308(wk->net, 7);
            break;
        case 2:
            func_ov121_021de308(wk->net, 10);
            break;
        }
        func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x69, 0);
        wk->wait = 0;
        *state = 1;
        break;
    case 1:
        if (func_ov121_021de3e0(wk->net) == 0) {
            ret = func_ov121_021de4dc(wk->net, &wk->recv, sizeof(wk->recv));
            if (ret == 1) {
                if (func_ov121_021d9554(&wk->recv, func_ov121_021def6c(&wk->recv, wk->gameData, HEAPID_TAIL(HEAPID_MYSTERY)))) {
                    *state = 5;
                } else {
                    *state = 12;
                }
            } else if (ret == 2) {
                *state = 2;
            }
        }
        switch (func_ov121_021de500(wk->net)) {
        case 1:
        case 2:
            func_ov121_021de550(wk->net);
            func_ov121_021da8b0(seq, func_ov121_021d6e3c);
            break;
        }
        if (!(GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) && wk->wait++ <= 7200) {
            break;
        }
        cancel = FALSE;
        switch (wk->receiveMode) {
        case 1:
            if (func_ov121_021de3e0(wk->net) == 4) {
                func_ov121_021de308(wk->net, 5);
                cancel = TRUE;
            }
            break;
        case 0:
            if (func_ov121_021de3e0(wk->net) == 8) {
                func_ov121_021de308(wk->net, 9);
                cancel = TRUE;
            }
            break;
        case 2:
            if (func_ov121_021de3e0(wk->net) == 11) {
                func_ov121_021de308(wk->net, 12);
                cancel = TRUE;
            }
            break;
        }
        if (cancel) {
            *state = 2;
            wk->wait = 0;
        }
        break;
    case 2:
        func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x88, 2);
        func_ov121_021da8c0(seq, 3);
        *state = 14;
        break;
    case 3:
        wk->wait++;
        if (func_ov121_021de3e0(wk->net) == 0 && wk->wait > 30) {
            wk->wait = 0;
            func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x8b, 1);
            func_ov121_021da8c0(seq, 4);
            *state = 14;
        }
        break;
    case 4:
        func_ov121_021da8b0(seq, func_ov121_021d6e3c);
        break;
    case 5:
        func_ov121_021d81b0(wk, 2, HEAPID_MYSTERY);
        func_ov121_021d83dc(wk, HEAPID_MYSTERY);
        func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x6a, 0);
        *state = 6;
        break;
    case 6:
        cancel = FALSE;
        ret = func_ov121_021d83b0(wk);
        if (ret != MYSTERY_MENU_NONE) {
            if (ret == 0) {
            func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x6e, 1);
            func_ov121_021da8c0(seq, 7);
            *state = 14;
            } else if (ret == MYSTERY_MENU_CANCEL) {
                cancel = TRUE;
            }
        } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
            cancel = TRUE;
        }

        if (cancel) {
            func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x6b, 1);
            func_ov121_021da8c0(seq, 9);
            *state = 14;
        }
        break;
    case 7:
        func_ov121_021d8138(wk, 0, HEAPID_MYSTERY);
        *state = 8;
        break;
    case 8:
        ret = func_ov121_021da178(wk->yesNo);
        if (ret == MYSTERY_MENU_NONE) {
            break;
        }
        func_ov121_021d8198(wk);
        if (ret == 0) {
            func_ov121_021d8380(wk);
            if (wk->recv.gift.once && func_0200a1a8(wk->giftSave, wk->recv.gift.id)) {
                *state = 11;
                break;
            }
            RTC_GetCachedDate(&date);
            wk->recv.gift.date = ((date.year + 2000) << 16) | ((date.month & 0xff) << 8) | (date.day & 0xff);
            func_0200a034(wk->giftSave, &wk->recv.gift);
            func_0200a1e4(wk->giftSave, wk->recv.gift.id);
            func_ov121_021da8b0(seq, func_ov121_021d7814);
        } else if (ret == 1) {
            *state = 5;
        }
        break;
    case 9:
        func_ov121_021d8138(wk, 0, HEAPID_MYSTERY);
        *state = 10;
        break;
    case 10:
        ret = func_ov121_021da178(wk->yesNo);
        if (ret == MYSTERY_MENU_NONE) {
            break;
        }
        func_ov121_021d8198(wk);
        if (ret == 0) {
            func_ov121_021d8380(wk);
            func_ov121_021d849c(wk);
            func_ov121_021da8b0(seq, func_ov121_021d6e3c);
        } else if (ret == 1) {
            *state = 5;
        }
        break;
    case 11:
        func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x8a, 1);
        func_ov121_021da8c0(seq, 13);
        *state = 14;
        break;
    case 12:
        func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x90, 1);
        func_ov121_021da8c0(seq, 13);
        *state = 14;
        break;
    case 13:
        func_ov121_021d8380(wk);
        func_ov121_021d849c(wk);
        func_ov121_021da8b0(seq, func_ov121_021d6e3c);
        break;
    case 14:
        if (func_ov121_021d9ff8(wk->msgWin)) {
            func_ov121_021da8c4(seq);
        }
        break;
    }
}


void func_ov121_021d7814(MysterySeq *seq, u32 *state, void *work) {
    MysteryWork *wk = work;
    MysteryCardResSetup setup;
    ClActUnit *unit;

    switch (*state) {
    case 0:
        func_ov121_021d9750(wk->graphic);
        GFL_BGSysSetBGEnabled(1, TRUE);
        GFL_BGSysSetBGEnabled(3, TRUE);
        unit = func_ov121_021d9744(wk->graphic);
        func_ov121_021d84d4(&wk->effect, unit, &wk->recv, wk->gameData, &wk->actors, &wk->bg, &wk->scroll, HEAPID_MYSTERY);
        func_ov121_021d8c1c(&wk->effect, 0);
        func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x71, 2);
        *state = 1;
        break;
    case 1:
        GFL_BGSysSetBGEnabled(0, TRUE);
        *state = 2;
        break;
    case 2:
        if (func_ov121_021d9ff8(wk->msgWin)) {
            *state = 3;
        }
        break;
    case 3:
        func_ov121_021d8d00(&wk->effect);
        if (func_ov121_021d8d18(&wk->effect)) {
            *state = 4;
        }
        break;
    case 4:
        func_0200a2b8(wk->giftSave, wk->gameData);
        *state = 5;
        break;
    case 5:
        if (func_0200a2d8(wk->giftSave, wk->gameData) == 2) {
            *state = 6;
        }
        break;
    case 6:
        func_ov121_021d8c1c(&wk->effect, 1);
        *state = 7;
        break;
    case 7:
        func_ov121_021d8d00(&wk->effect);
        if (func_ov121_021d8d18(&wk->effect)) {
            *state = 8;
        }
        break;
    case 8:
        func_ov121_021d9f08(wk->msgWin, wk->msgData, 0x72, 1);
        *state = 9;
        break;
    case 9:
        if (func_ov121_021d9ff8(wk->msgWin)) {
            *state = 10;
        }
        break;
    case 10:
        func_ov121_021d8ca8(&wk->effect);
        func_ov121_021d9e04(wk->msgWin);
        wk->msgWin = NULL;
        func_ov121_021d8380(wk);
        G2_BlendNone();
        func_ov121_021d9790(wk->graphic);
        GFL_BGSysSetBGEnabled(1, TRUE);
        GFL_BGSysSetBGEnabled(3, TRUE);
        *state = 11;
        break;
    case 11:
        GFL_BGSysSetBGEnabled(0, TRUE);
        *state = 12;
        break;
    case 12:
        GFL_BGSysSetBGEnabled(1, FALSE);
        MI_CpuFill8(&setup, 0, sizeof(setup));
        setup.bg = 2;
        setup.textBg = 0;
        setup.bgPalette = 8;
        setup.textPalette = 15;
        setup.iconPalette = 10;
        setup.pokePalette = 13;
        setup.unit = func_ov121_021d9744(wk->graphic);
        setup.giftSave = wk->giftSave;
        setup.msgData = wk->msgData;
        setup.font = wk->font;
        setup.queue = wk->queue;
        setup.wordSet = wk->wordSet;
        wk->cardRes = func_ov121_021dcfc8(&setup, HEAPID_MYSTERY);
        wk->album = func_ov121_021dd24c(&wk->recv.gift, wk->cardRes, wk->gameData, HEAPID_MYSTERY);
        func_ov121_021de0ac(wk->album, FALSE);
        *state = 13;
        break;
    case 13:
        func_ov121_021dd9cc(wk->album);
        func_ov121_021ddf5c(wk->album);
        *state = 14;
        break;
    case 14:
        func_ov121_021dd9cc(wk->album);
        if (func_ov121_021de014(wk->album)) {
            wk->wait = 0;
            *state = 15;
        }
        break;
    case 15:
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            *state = 18;
        }
        if (wk->wait++ > 240) {
            wk->wait = 0;
            *state = 16;
        }
        break;
    case 16:
        unit = func_ov121_021d9744(wk->graphic);
        func_ov121_021d93f4(&wk->oamMsg, &wk->actors, unit, wk->queue, wk->msgData, 0x9b, wk->font, HEAPID_MYSTERY);
        func_ov121_021d94b8(&wk->oamMsg);
        *state = 17;
        break;
    case 17:
        func_ov121_021d94b8(&wk->oamMsg);
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            func_ov121_021d9488(&wk->oamMsg);
            *state = 18;
        }
        break;
    case 18:
        func_ov121_021da8b0(seq, func_ov121_021d6df4);
        break;
    }
}


void func_ov121_021d7b4c(MysterySeq *seq, u32 *state, void *work) {
    MysteryWork *wk = work;
    MysteryCardViewSetup setup;

    switch (*state) {
    case 0:
        GFL_FadeSet(3, 0, 16, -1);
        func_ov121_021da8c0(seq, 1);
        *state = 8;
        break;
    case 1:
        func_ov121_021d9288(&wk->scroll, FALSE);
        func_ov121_021d8380(wk);
        func_ov121_021d9e04(wk->msgWin);
        wk->msgWin = NULL;
        MI_CpuFill8(&setup, 0, sizeof(setup));
        setup.mode = 0;
        setup.unit = func_ov121_021d9744(wk->graphic);
        setup.giftSave = wk->giftSave;
        setup.font = wk->font;
        setup.queue = wk->queue;
        setup.wordSet = wk->wordSet;
        setup.msgData = wk->msgData;
        setup.gameData = wk->gameData;
        wk->cardView = func_ov121_021dac18(&setup, HEAPID_MYSTERY);
        *state = 2;
        break;
    case 2:
        GFL_FadeSet(3, 16, 0, -1);
        func_ov121_021da8c0(seq, 3);
        *state = 8;
        break;
    case 3:
        func_ov121_021daef0(wk->cardView);
        if (func_ov121_021db008(wk->cardView)) {
            *state = 4;
        }
        break;
    case 4:
        GFL_FadeSet(3, 0, 16, 0);
        func_ov121_021da8c0(seq, 5);
        *state = 8;
        break;
    case 5:
        func_ov121_021d9288(&wk->scroll, TRUE);
        func_ov121_021dae1c(wk->cardView);
        wk->cardView = NULL;
        func_ov121_021d694c(&wk->bg, 0);
        func_ov121_021d6c60(&wk->actors, HEAPID_MYSTERY);
        wk->msgWin = func_ov121_021d9ca4(1, 15, wk->queue, wk->font, HEAPID_MYSTERY);
        func_ov121_021d9ffc(wk->msgWin, 10, 14);
        func_ov121_021d81b0(wk, 0, HEAPID_MYSTERY);
        G2_SetBlendAlpha(4, 0x18, 15, 3);
        *state = 6;
        break;
    case 6:
        GFL_FadeSet(3, 16, 0, 0);
        func_ov121_021da8c0(seq, 7);
        *state = 8;
        break;
    case 7:
        func_ov121_021da8b0(seq, func_ov121_021d6e3c);
        break;
    case 8:
        if (!GFL_FadeIsRunning()) {
            func_ov121_021da8c4(seq);
        }
        break;
    }
    if (wk->cardView != NULL) {
        func_ov121_021dafcc(wk->cardView);
    }
}


void func_ov121_021d7cf4(MysterySeq *seq, u32 *state, void *work) {
    MysteryWork *wk = work;
    MysteryCardViewSetup setup;

    switch (*state) {
    case 0:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, -1);
        func_ov121_021da8c0(seq, 1);
        *state = 8;
        break;
    case 1:
        func_ov121_021d9288(&wk->scroll, FALSE);
        func_ov121_021d8380(wk);
        func_ov121_021d9e04(wk->msgWin);
        wk->msgWin = NULL;
        MI_CpuFill8(&setup, 0, sizeof(setup));
        setup.mode = 1;
        setup.unit = func_ov121_021d9744(wk->graphic);
        setup.giftSave = wk->giftSave;
        setup.font = wk->font;
        setup.queue = wk->queue;
        setup.wordSet = wk->wordSet;
        setup.msgData = wk->msgData;
        setup.gameData = wk->gameData;
        wk->cardView = func_ov121_021dac18(&setup, HEAPID_MYSTERY);
        *state = 2;
        break;
    case 2:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, -1);
        func_ov121_021da8c0(seq, 3);
        *state = 8;
        break;
    case 3:
        func_ov121_021daef0(wk->cardView);
        if (func_ov121_021db008(wk->cardView)) {
            *state = 4;
        }
        break;
    case 4:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 0);
        func_ov121_021da8c0(seq, 5);
        *state = 8;
        break;
    case 5:
        func_ov121_021d9288(&wk->scroll, TRUE);
        func_ov121_021dae1c(wk->cardView);
        wk->cardView = NULL;
        func_ov121_021d694c(&wk->bg, 0);
        func_ov121_021d6c60(&wk->actors, HEAPID_MYSTERY);
        wk->msgWin = func_ov121_021d9ca4(1, 15, wk->queue, wk->font, HEAPID_MYSTERY);
        func_ov121_021d9ffc(wk->msgWin, 10, 14);
        G2_SetBlendAlpha(4, 0x18, 15, 3);
        func_ov121_021d81b0(wk, 0, HEAPID_MYSTERY);
        *state = 6;
        break;
    case 6:
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
        func_ov121_021da8c0(seq, 7);
        *state = 8;
        break;
    case 7:
        wk->resumeReceive = func_0200a0c8(wk->giftSave);
        func_ov121_021da8b0(seq, func_ov121_021d6e3c);
        break;
    case 8:
        if (!GFL_FadeIsRunning()) {
            func_ov121_021da8c4(seq);
        }
        break;
    }
    if (wk->cardView != NULL) {
        func_ov121_021dafcc(wk->cardView);
    }
}

void func_ov121_021d7eac(MysterySeq *seq, u32 *state, void *work) {
    MysteryWork *wk = work;

    switch (func_ov121_021de500(wk->net)) {
    case 1:
    case 2:
        func_ov121_021de550(wk->net);
        func_ov121_021da8b0(seq, func_ov121_021d6e3c);
        break;
    }
    switch (*state) {
    case 0:
        GFL_FadeSet(3, 0, 16, 0);
        *state = 1;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            func_ov121_021d9288(&wk->scroll, FALSE);
            *state = 2;
        }
        break;
    case 2:
        if (wk->msgWin != NULL) {
            func_ov121_021d9e04(wk->msgWin);
            wk->msgWin = NULL;
        }
        func_ov121_021d6c28(&wk->actors);
        func_ov121_021d6948(&wk->bg);
        func_ov121_021d96c4(wk->graphic);
        wk->graphic = NULL;
        *state = 3;
        break;
    case 3:
        wk->loginParam = GFL_HeapAllocate(HEAPID_MYSTERY, sizeof(WifiLoginParam), FALSE, "mystery.c", 2664);
        MI_CpuFill8(wk->loginParam, 0, sizeof(WifiLoginParam));
        wk->loginParam->gameData = wk->gameData;
        wk->loginParam->unk4 = 0;
        wk->loginParam->unk8 = 1;
        wk->loginParam->buffer = NULL;
        wk->loginParam->unk18 = 0;
        wk->loginParam->unkC = 9;
        if (func_0200a350(wk->giftSave) == 0) {
            wk->loginParam->unk14 = 2;
        } else {
            wk->loginParam->unk14 = 0;
        }
        GCTX_ProcMgrQueueProc(OVERLAY_WIFILOGIN, &data_ov115_021d6338, wk->loginParam);
        *state = 5;
        break;
    case 5:
        wk->graphic = func_ov121_021d9624(1, HEAPID_MYSTERY);
        func_ov121_021d6924(&wk->bg, HEAPID_MYSTERY);
        func_ov121_021d6aec(&wk->actors, func_ov121_021d9744(wk->graphic), HEAPID_MYSTERY);
        wk->msgWin = func_ov121_021d9ca4(1, 15, wk->queue, wk->font, HEAPID_MYSTERY);
        func_ov121_021d9ffc(wk->msgWin, 10, 14);
        G2_SetBlendAlpha(4, 0x18, 15, 3);
        func_0203d34c(TRUE, HEAPID_MYSTERY);
        GFL_SndBGMPlay(SEQ_BGM_WIFI_PRESENT, 0xffff);
        func_ov121_021d9288(&wk->scroll, TRUE);
        *state = 4;
        break;
    case 4:
        *state = 6;
        break;
    case 6:
        GFL_FadeSet(3, 16, 0, 0);
        *state = 7;
        break;
    case 7:
        if (!GFL_FadeIsRunning()) {
            *state = 8;
        }
        break;
    case 8:
        if (wk->loginParam->result == 0) {
            func_ov121_021da8b0(seq, func_ov121_021d73dc);
        } else {
            func_ov121_021da8b0(seq, func_ov121_021d6e3c);
        }
        GFL_HeapFree(wk->loginParam);
        break;
    }
}




// pokebw2's MysterySeq_Exit
void func_ov121_021d80c0(MysterySeq *seq, u32 *state, void *work) {
    MysteryWork *wk = work;

    func_ov121_021d8198(wk);
    func_ov121_021d8380(wk);
    func_ov121_021d849c(wk);
    if (wk->album != NULL) {
        func_ov121_021dd980(wk->album);
        func_ov121_021dd1fc(wk->cardRes);
    }
    func_ov121_021da8b8(seq);
}

// pokebw2's MysterySeq_WirelessOff
void func_ov121_021d80f0(MysterySeq *seq, u32 *state, void *work) {
    MysteryWork *wk = work;

    switch (*state) {
    case 0:
        func_ov121_021d9f08(wk->msgWin, wk->scriptMsgData, 0x16, MYSTERY_PRINT_STREAM);
        *state = 1;
        break;
    case 1:
        if (func_ov121_021d9ff8(wk->msgWin)) {
            func_ov121_021da8b0(seq, func_ov121_021d6e3c);
        }
        break;
    }
}

// pokebw2's Mystery_CreateYesNo
void func_ov121_021d8138(MysteryWork *wk, u32 unused, HeapID heapId) {
    MysteryYesNoSetup setup;

    if (wk->yesNo == NULL) {
        MI_CpuFill8(&setup, 0, sizeof(setup));
        setup.msgData = wk->msgData;
        setup.font = wk->font;
        setup.queue = wk->queue;
        setup.msgIds[0] = 0x54;
        setup.msgIds[1] = 0x55;
        setup.count = 2;
        setup.bg = 0;
        setup.palette = 15;
        setup.framePalette = 14;
        setup.frameChar = 1;
        wk->yesNo = func_ov121_021da038(&setup, heapId);
    }
}

// pokebw2's Mystery_DeleteYesNo
void func_ov121_021d8198(MysteryWork *wk) {
    if (wk->yesNo != NULL) {
        func_ov121_021da148(wk->yesNo);
        wk->yesNo = NULL;
    }
}

void func_ov121_021d81b0(MysteryWork *wk, u32 type, HeapID heapId) {
    MysteryListSetup setup;
    ArcTool *arc;
    int i;

    if (wk->list == NULL) {
        MI_CpuFill8(&setup, 0, sizeof(setup));
        setup.font = wk->font;
        setup.queue = wk->queue;
        setup.cursor = func_ov121_021d6c58(&wk->actors, ACTOR_CURSOR);
        setup.bg = 1;
        setup.palette = 15;
        setup.unk28 = 2;
        setup.bgPalette = 4;
        setup.onMove = func_ov121_021d83cc;
        setup.work = wk;
        setup.offsetY = 0;
        setup.offsetX = 0;
        setup.cursorSequence = 0;
        setup.cursorPos = &wk->listCursors[type];
        switch (type) {
        case 0:
            setup.msgData = wk->msgData;
            setup.items[0].msgId = 0x50;
            setup.items[1].msgId = 0x51;
            setup.items[2].msgId = 0x83;
            setup.items[3].msgId = 0x52;
            setup.count = 4;
            break;
        case 1:
            setup.msgData = wk->msgData;
            setup.items[0].msgId = 0x57;
            setup.items[1].msgId = 0x58;
            setup.items[2].msgId = 0x59;
            setup.items[3].msgId = 0x5a;
            setup.count = 4;
            break;
        case 2:
            setup.msgData = NULL;
            setup.items[0].str = GFL_StrBufCreate(37, heapId);
            GFL_StrBufLoadFixedString(setup.items[0].str, wk->recv.gift.title, 37);
            setup.count = 1;
            setup.offsetY = -1;
            setup.offsetX = -2;
            setup.cursorSequence = 10;
            break;
        case 3:
            setup.msgData = wk->msgData;
            setup.items[0].msgId = 0x7f;
            setup.items[1].msgId = 0x80;
            setup.items[2].msgId = 0x81;
            setup.items[3].msgId = 0x82;
            setup.count = 4;
            break;
        }
        arc = GFL_ArcSysCreateFileHandle(0x22, heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, 6, 0, 0, 0xa0, heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 15, 2, 0, 0, FALSE, heapId);
        if (type == 2) {
            loadBGScrToVramByNarcNoReserveNegAlign(0x22, 30, 2, 0, 0, FALSE, heapId);
        } else {
            loadBGScrToVramByNarcNoReserveNegAlign(0x22, 29, 2, 0, 0, FALSE, heapId);
        }
        GFL_ArcToolFree(arc);
        GFL_BGSysSetScrPaletteNo(2, 0, 0, 32, 24, 4);
        GFL_BGSysLoadScr(2);
        GFL_BGSysSetBGEnabled(2, TRUE);
        wk->list = func_ov121_021da4dc(&setup, heapId);
        if (setup.msgData == NULL) {
            for (i = 0; i < 4; i++) {
                if (setup.items[i].str != NULL) {
                    GFL_StrBufFree(setup.items[i].str);
                }
            }
        }
        G2_SetBlendAlpha(4, 0x18, 15, 3);
        func_ov121_021d6cec(&wk->actors);
    }
}


// pokebw2's Mystery_DeleteList
void func_ov121_021d8380(MysteryWork *wk) {
    if (wk->list != NULL) {
        GFL_BGSysClearScr(2);
        GFL_BGSysLoadScr(2);
        GFL_BGSysSetBGEnabled(2, FALSE);
        func_ov121_021da698(wk->list);
        wk->list = NULL;
    }
}

// pokebw2's Mystery_UpdateList
u32 func_ov121_021d83b0(MysteryWork *wk) {
    u32 ret = func_ov121_021da6d0(wk->list);

    func_ov121_021d6c84(&wk->actors);
    return ret;
}

// pokebw2's Mystery_OnListMove
void func_ov121_021d83cc(void *work) {
    MysteryWork *wk = work;

    func_ov121_021d6cec(&wk->actors);
}

// pokebw2's Mystery_CreateCardWin
void func_ov121_021d83dc(MysteryWork *wk, HeapID heapId) {
    if (wk->cardWin == NULL) {
        MysteryTextWinEntry entries[2] = {
            { 1, 2, 30, 2, 0, NULL, TRUE, 0, 0, PRINT_COLOR(15, 2, 0) },
            { 2, 6, 28, 15, 0, NULL, FALSE, 0, 4, PRINT_COLOR(15, 2, 0) },
        };

        entries[0].str = GFL_StrBufCreate(37, heapId);
        GFL_StrBufLoadFixedString(entries[0].str, wk->recv.gift.title, 37);
        entries[1].str = GFL_StrBufCreate(253, heapId);
        GFL_StrBufLoadFixedString(entries[1].str, wk->recv.text, 253);
        wk->cardWin = func_ov121_021da1bc(0, entries, 2, 4, 15, wk->queue, wk->msgData, wk->font, heapId);
        GFL_StrBufFree(entries[0].str);
        GFL_StrBufFree(entries[1].str);
        func_ov121_021d694c(&wk->bg, 1);
        GFL_BGSysLoadScr(6);
        GFL_BGSysSetBGEnabled(6, TRUE);
        G2S_SetBlendAlpha(4, 0x18, 15, 3);
    }
}

// pokebw2's Mystery_DeleteCardWin
static void func_ov121_021d849c(MysteryWork *wk) {
    if (wk->cardWin != NULL) {
        G2S_BlendNone();
        func_ov121_021da2a8(wk->cardWin);
        func_ov121_021da280(wk->cardWin);
        wk->cardWin = NULL;
        GFL_BGSysLoadScr(4);
        GFL_BGSysSetBGEnabled(6, FALSE);
    }
}

void func_ov121_021d84d4(MysteryEffect *effect, ClActUnit *unit, const MysteryGiftRecvData *recv, GameData *gameData,
                         MysteryActors *actors, MysteryBg *bg, MysteryBgScroll *scroll, HeapID heapId) {
    u32 palNo;
    NNSG2dPaletteData *palette;
    void *buffer;
    u16 *colors;
    int i;
    PartyPkm *pkm;
    ArcTool *arc;
    ClActorSetup setup;
    MysteryGift *gift = (MysteryGift *)&recv->gift;

    MI_CpuFill8(effect, 0, sizeof(MysteryEffect));
    effect->scroll = scroll;
    effect->bg = bg;
    effect->heapId = heapId;
    effect->recv = recv;
    effect->height = 0;
    effect->offsetX = 0;
    if (recv->special) {
        palNo = 3;
    } else {
        palNo = data_ov121_021df4f2[recv->gift.kind];
    }
    buffer = GFL_G2DIOReadNCLR(0x22, 2, &palette, heapId);
    colors = palette->pRawData;
    MI_CpuCopy8(colors, effect->baseColors, sizeof(effect->baseColors));
    MI_CpuCopy8(colors + palNo * 16, effect->kindColors, sizeof(effect->kindColors));
    GFL_HeapFree(buffer);
    for (i = 0; i < STAR_COUNT; i++) {
        func_0204b6f4(func_ov121_021d6c58(actors, ACTOR_STARS + i), (u8)palNo, 0);
    }
    switch (gift->kind) {
    case 1:
        pkm = Mystery_CreateGiftPokemon(gift, HEAPID_TAIL(heapId), gameData);
        arc = MakePokeGraArcHandle(heapId);
        effect->palette = func_0202d8d0(arc, func_0201d624(pkm), 0, 0, 0x1c0, heapId);
        effect->cellAnims = func_0202d9a4(func_0201d624(pkm), 0, 2, 0, heapId);
        effect->chars = func_0202d934(arc, func_0201d624(pkm), 0, 0, heapId);
        effect->height = func_ov121_021d94c4(func_0201d624(pkm), heapId) - 48;
        effect->height = MATH_CLAMP(effect->height, 0, 48);
        GFL_ArcToolFree(arc);
        GFL_HeapFree(pkm);
        break;
    case 2:
        arc = GFL_ArcSysCreateFileHandle(0x19, heapId);
        effect->palette = func_0204abf0(arc, GetItemGraphicsDatID(recv->gift.value, 2), CLACT_VRAM_MAIN, 0x1c0, heapId);
        effect->cellAnims = func_0204af28(arc, 1, 0, heapId);
        effect->chars = func_0204a6c8(arc, GetItemGraphicsDatID(recv->gift.value, 1), FALSE, CLACT_VRAM_MAIN, heapId);
        GFL_ArcToolFree(arc);
        effect->offsetX = 4;
        effect->height = 8;
        break;
    case 3:
        arc = GFL_ArcSysCreateFileHandle(0x22, heapId);
        effect->palette = func_0204abf0(arc, 3, CLACT_VRAM_MAIN, 0x1c0, heapId);
        effect->cellAnims = func_0204af28(arc, 33, 36, heapId);
        effect->chars = func_0204a6c8(arc, 12, FALSE, CLACT_VRAM_MAIN, heapId);
        GFL_ArcToolFree(arc);
        effect->height = 8;
        break;
    }
    if (recv->special) {
        MI_CpuCopy8((void *)(HW_OBJ_PLTT + 14 * 0x20), effect->objColors, sizeof(effect->objColors));
        MIi_CpuClear16(0x7fff, (void *)(HW_OBJ_PLTT + 14 * 0x20), 0x20);
        MI_CpuCopy8((void *)(HW_OBJ_PLTT + 14 * 0x20), effect->whiteColors, sizeof(effect->whiteColors));
    }
    MI_CpuFill8(&setup, 0, sizeof(setup));
    setup.x = effect->offsetX + 128;
    setup.y = -96;
    setup.bgPriority = 2;
    effect->actor = func_0204b294(unit, effect->chars, effect->palette, effect->cellAnims, &setup, 0, heapId);
    func_0204b3dc(effect->actor, FALSE);
    if (recv->special) {
        func_0204b688(effect->actor, 1);
    }
}


// pokebw2's MysteryEffect_Start
void func_ov121_021d8c1c(MysteryEffect *effect, u32 mode) {
    effect->done = FALSE;
    effect->state = 0;
    effect->frame = 0;
    switch (mode) {
    case 0:
        if (effect->recv->special) {
            effect->func = func_ov121_021d8ea4;
        } else {
            effect->func = func_ov121_021d8d1c;
        }
        break;
    case 1:
        effect->func = func_ov121_021d9138;
        break;
    }
    GX_SetVisibleWnd(GX_WNDMASK_W0);
    G2_SetWnd0InsidePlane(15, TRUE);
    G2_SetWndOutsidePlane(31, TRUE);
    G2_SetWnd0Position(0, 168, 255, 192);
}

// pokebw2's MysteryEffect_Exit
void func_ov121_021d8ca8(MysteryEffect *effect) {
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    if (func_ov121_021d6d7c(&effect->particle)) {
        func_ov121_021d6d38(&effect->particle);
    }
    GFL_BGSysSetBGEnabled(2, FALSE);
    func_0204b3b4(effect->actor);
    func_0204ada4(effect->palette);
    func_0204a8d4(effect->chars);
    func_0204afd8(effect->cellAnims);
    MI_CpuFill8(effect, 0, sizeof(MysteryEffect));
}

// pokebw2's MysteryEffect_Main
void func_ov121_021d8d00(MysteryEffect *effect) {
    if (effect->func != NULL) {
        effect->func(effect);
        if (effect->done) {
            effect->func = NULL;
        }
    }
}

// pokebw2's MysteryEffect_IsEnd
BOOL func_ov121_021d8d18(MysteryEffect *effect) {
    return effect->done;
}

static inline void Mystery_SetBlendAlpha(s16 ev1, s16 ev2) {
    reg_G2_BLDALPHA = ev1 | (ev2 << 8);
}

// pokebw2's MysteryEffect_Fall
static void func_ov121_021d8d1c(MysteryEffect *effect) {
    u32 alpha;
    s16 y;

    switch (effect->state) {
    case 0:
        GFL_BGSysSetBGEnabled(2, FALSE);
        func_ov121_021d694c(effect->bg, 2);
        effect->state = 1;
        break;
    case 1:
        G2_SetBlendAlpha(4, 8, 0, 16);
        GFL_BGSysSetBGEnabled(2, TRUE);
        effect->frame = 0;
        effect->state = 2;
        break;
    case 2:
        alpha = effect->frame * 16 / 120;
        Mystery_SetBlendAlpha(alpha, 16 - alpha);
        effect->blend = effect->frame * 0x7fff / 120;
        func_ov121_021dab50(15, effect->bgFadeColors, effect->blend, 0, effect->kindColors, effect->baseColors);
        func_ov121_021dab50(31, effect->subFadeColors, effect->blend, 0, effect->kindColors, effect->baseColors);
        if (effect->frame++ > 120) {
            G2_BlendNone();
            func_ov121_021d9254(effect->scroll, 1);
            effect->frame = 0;
            effect->state = 3;
        }
        break;
    case 3:
        if (effect->frame++ > 180) {
            effect->frame = 0;
            effect->state = 4;
        }
        break;
    case 4:
        y = (214 - effect->height) * effect->frame / 60 - 96;
        func_0204b49c(effect->actor, y, 0, 1);
        if (y >= -48 && y <= 192) {
            func_0204b3dc(effect->actor, TRUE);
        }
        if (effect->frame++ >= 60) {
            GFL_SndSEPlay(SEQ_SE_SYS_50);
            func_ov121_021d9254(effect->scroll, 0);
            effect->frame = 0;
            effect->state = 5;
        }
        break;
    case 5:
        if (effect->frame++ > 60) {
            effect->frame = 0;
            effect->state = 6;
        }
        break;
    case 6:
        effect->done = TRUE;
        break;
    }
}

// pokebw2's MysteryEffect_FallSpecial
void func_ov121_021d8ea4(MysteryEffect *effect) {
    u32 alpha;
    ClActorPos pos;

    switch (effect->state) {
    case 0:
        func_ov121_021d6cf4(&effect->particle, effect->heapId);
        func_ov121_021d6d50(&effect->particle, 0x22, 38, effect->heapId);
        GFL_BGSysSetBGEnabled(2, FALSE);
        func_ov121_021d694c(effect->bg, 2);
        effect->state = 1;
        break;
    case 1:
        G2_SetBlendAlpha(4, 8, 0, 16);
        GFL_BGSysSetBGEnabled(2, TRUE);
        effect->frame = 0;
        effect->state = 2;
        break;
    case 2:
        alpha = effect->frame * 16 / 120;
        Mystery_SetBlendAlpha(alpha, 16 - alpha);
        effect->blend = effect->frame * 0x7fff / 120;
        func_ov121_021dab50(15, effect->bgFadeColors, effect->blend, 0, effect->kindColors, effect->baseColors);
        func_ov121_021dab50(31, effect->subFadeColors, effect->blend, 0, effect->kindColors, effect->baseColors);
        if (effect->frame++ > 120) {
            G2_BlendNone();
            effect->frame = 0;
            effect->state = 3;
        }
        break;
    case 3:
        if (effect->frame++ > 180) {
            effect->frame = 0;
            effect->state = 6;
            G2_SetBlendAlpha(1, 31, 0, 16);
            func_ov121_021d6d6c(&effect->particle, 0);
            func_ov121_021d6d6c(&effect->particle, 1);
            func_ov121_021d6d6c(&effect->particle, 2);
            GFL_SndSEPlay(SEQ_SE_SYS_88);
        }
        break;
    case 6:
        if (effect->frame++ > 120) {
            GFL_SndSEPlay(SEQ_SE_SYS_89);
            effect->frame = 0;
            effect->state = 4;
            func_ov121_021d9254(effect->scroll, 1);
        }
        break;
    case 4:
        pos.x = 128;
        pos.y = 118 - effect->height;
        func_0204b404(effect->actor, &pos, 0);
        func_0204b3dc(effect->actor, TRUE);
        effect->subFrame = 0;
        effect->state = 5;
        break;
    case 5:
        alpha = effect->subFrame * 16 / 10;
        Mystery_SetBlendAlpha(alpha, 16 - alpha);
        effect->frame++;
        if (effect->subFrame++ > 10) {
            effect->subFrame = 0;
            effect->state = 7;
        }
        break;
    case 7:
        if (effect->frame++ > 90) {
            G2_BlendNone();
            effect->frame = 0;
            effect->state = 8;
        }
        break;
    case 8:
        if (effect->frame++ > 10) {
            effect->frame = 0;
            effect->state = 9;
        }
        break;
    case 9:
        effect->blend = effect->frame * 0x7fff / 120;
        func_ov121_021dab50(14, effect->objFadeColors, effect->blend, 14, effect->objColors, effect->whiteColors);
        if (effect->frame++ > 120) {
            func_ov121_021d9254(effect->scroll, 0);
            effect->frame = 0;
            effect->state = 10;
        }
        break;
    case 10:
        if (effect->frame++ > 60) {
            GFL_SndSEPlay(SEQ_SE_SYS_50);
            effect->frame = 0;
            effect->state = 11;
        }
        break;
    case 11:
        G2_BlendNone();
        effect->done = TRUE;
        break;
    }
}

// pokebw2's MysteryEffect_FadeBack
void func_ov121_021d9138(MysteryEffect *effect) {
    switch (effect->state) {
    case 0:
        effect->state = 1;
        break;
    case 1:
        effect->blend = effect->frame * 0x7fff / 60;
        func_ov121_021dab50(15, effect->bgFadeColors, effect->blend, 0, effect->baseColors, effect->kindColors);
        func_ov121_021dab50(31, effect->subFadeColors, effect->blend, 0, effect->baseColors, effect->kindColors);
        if (effect->frame++ > 60) {
            effect->frame = 0;
            effect->state = 2;
        }
        break;
    case 2:
        effect->done = TRUE;
        break;
    }
}

// pokebw2's MysteryBgScroll_Init
void func_ov121_021d91bc(MysteryBgScroll *scroll, MysteryActors *actors, HeapID heapId) {
    int i;
    ClActorPos pos;

    MI_CpuFill8(scroll, 0, sizeof(MysteryBgScroll));
    scroll->speed = -FX32_CONST(0.2);
    scroll->actors = actors;
    scroll->startSpeed = -FX32_CONST(0.2);
    scroll->heapId = heapId;
    for (i = 0; i < STAR_COUNT; i++) {
        func_0204b53c(func_ov121_021d6c58(scroll->actors, ACTOR_STARS + i), &pos);
        scroll->starY[i] = pos.y << FX32_SHIFT;
        scroll->starSpeed[i] = GFL_RandomLCAlt(FX32_ONE) + FX32_CONST(1.6);
    }
}

// pokebw2's MysteryBgScroll_Exit
void func_ov121_021d9228(MysteryBgScroll *scroll) {
    MI_CpuFill8(scroll, 0, sizeof(MysteryBgScroll));
}

// pokebw2's MysteryBgScroll_Main
static void func_ov121_021d9238(MysteryBgScroll *scroll) {
    if (scroll->enabled && scroll->active && scroll->func != NULL) {
        scroll->func(scroll);
    }
}

// pokebw2's MysteryBgScroll_SetSpeed
void func_ov121_021d9254(MysteryBgScroll *scroll, u32 mode) {
    scroll->active = TRUE;
    scroll->frame = 0;
    scroll->startSpeed = scroll->speed;
    switch (mode) {
    case 0:
        scroll->func = func_ov121_021d928c;
        break;
    case 1:
        scroll->func = func_ov121_021d933c;
        break;
    }
}

// pokebw2's MysteryBgScroll_SetEnabled
void func_ov121_021d9288(MysteryBgScroll *scroll, BOOL enabled) {
    scroll->enabled = enabled;
}

// pokebw2's MysteryBgScroll_Slow
static void func_ov121_021d928c(MysteryBgScroll *scroll) {
    int i;
    ClActor *actor;
    ClActorPos pos;

    if (scroll->speed != -FX32_CONST(0.2)) {
        scroll->speed = scroll->startSpeed + (-FX32_CONST(0.2) - scroll->startSpeed) * scroll->frame / 60;
        if (scroll->frame++ > 60) {
            scroll->speed = -FX32_CONST(0.2);
        }
    }
    for (i = 0; i < STAR_COUNT; i++) {
        actor = func_ov121_021d6c58(scroll->actors, ACTOR_STARS + i);
        if (func_0204b3f4(actor)) {
            scroll->starY[i] += scroll->starSpeed[i];
            func_0204b53c(actor, &pos);
            pos.y = scroll->starY[i] >> FX32_SHIFT;
            func_0204b528(actor, &pos);
            if (pos.y >= 0 && pos.y <= 208) {
                func_0204b3dc(actor, TRUE);
            } else {
                func_0204b3dc(actor, FALSE);
            }
        }
    }
    scroll->pos += scroll->speed;
    GFL_BGSysMoveBGReq(3, BG_MOVE_SET_Y, scroll->pos >> FX32_SHIFT);
    GFL_BGSysMoveBGReq(7, BG_MOVE_SET_Y, scroll->pos >> FX32_SHIFT);
}

// pokebw2's MysteryBgScroll_Fast
static void func_ov121_021d933c(MysteryBgScroll *scroll) {
    int i;
    ClActor *actor;
    ClActorPos pos;

    if (scroll->speed != -FX32_CONST(4)) {
        scroll->speed = scroll->startSpeed + (-FX32_CONST(4) - scroll->startSpeed) * scroll->frame / 60;
        if (scroll->frame++ > 60) {
            scroll->speed = -FX32_CONST(4);
        }
    }
    for (i = 0; i < STAR_COUNT; i++) {
        scroll->starY[i] += scroll->starSpeed[i];
        if (scroll->starY[i] >= FX32_CONST(208)) {
            scroll->starY[i] = -FX32_CONST(16);
        }
        actor = func_ov121_021d6c58(scroll->actors, ACTOR_STARS + i);
        func_0204b53c(actor, &pos);
        pos.y = scroll->starY[i] >> FX32_SHIFT;
        func_0204b528(actor, &pos);
        if (pos.y >= 0 && pos.y <= 208) {
            func_0204b3dc(actor, TRUE);
        } else {
            func_0204b3dc(actor, FALSE);
        }
    }
    scroll->pos += scroll->speed;
    GFL_BGSysMoveBGReq(3, BG_MOVE_SET_Y, scroll->pos >> FX32_SHIFT);
    GFL_BGSysMoveBGReq(7, BG_MOVE_SET_Y, scroll->pos >> FX32_SHIFT);
}

void func_ov121_021d93f4(MysteryOamMsg *msg, const MysteryActors *actors, ClActUnit *unit, PrintQueue *queue,
                         MsgData *msgData, u32 msgId, Font *font, HeapID heapId) {
    ClActorSetup setup;

    MI_CpuFill8(msg, 0, sizeof(MysteryOamMsg));
    msg->actors = actors;
    msg->bmpOam = func_02025538(heapId, unit);
    MI_CpuFill8(&setup, 0, sizeof(setup));
    setup.x = 96;
    setup.y = 169;
    setup.priority = 0;
    setup.bgPriority = 0;
    msg->text = func_ov121_021da8cc(&setup, 8, 2, actors->palette, 3, 0, msg->bmpOam, queue, heapId);
    func_ov121_021da9ec(msg->text, PRINT_COLOR(1, 3, 0));
    func_ov121_021da9f0(msg->text, 0, 0, 1);
    func_ov121_021da994(msg->text, msgData, msgId, font);
    func_0204b3dc(func_ov121_021d6c58(actors, 0x22), TRUE);
}

// pokebw2's MysteryOamMsg_Exit
void func_ov121_021d9488(MysteryOamMsg *msg) {
    func_ov121_021da978(msg->text);
    func_0204b3dc(func_ov121_021d6c58(msg->actors, 0x22), FALSE);
    func_ov121_021da958(msg->text);
    func_02025588(msg->bmpOam);
    MI_CpuFill8(msg, 0, sizeof(MysteryOamMsg));
}

// pokebw2's MysteryOamMsg_Update
BOOL func_ov121_021d94b8(MysteryOamMsg *msg) {
    return func_ov121_021da9f8(msg->text);
}

// The lowest row of the Pokémon's front sprite that has a pixel
// pokebw2's Mystery_GetSpriteBottom
u32 func_ov121_021d94c4(BoxPkm *pkm, HeapID heapId) {
    NNSG2dCharacterData *charData;
    void *buffer;
    u32 bottom = 48;
    BOOL found = FALSE;
    int row;
    int col;
    int y;
    u32 *tile;

    buffer = func_0202d6f4(&charData, pkm, 0, HEAPID_TAIL(heapId));
    func_0202d214(charData, HEAPID_TAIL(heapId));
    for (row = 11; row >= 0; row--) {
        for (col = 0; col < 12; col++) {
            tile = (u32 *)charData->pRawData + (row * 12 + col) * 8;
            for (y = 7; y >= 0; y--) {
                if (tile[y] != 0) {
                    if (bottom < y + row * 8) {
                        bottom = y + row * 8;
                    }
                    found = TRUE;
                }
            }
        }
        if (found) {
            break;
        }
    }
    GFL_HeapFree(buffer);
    return bottom;
}

// pokebw2's Mystery_IsRecvDataValid
BOOL func_ov121_021d9554(const MysteryGiftRecvData *recv, u32 errors) {
    if (func_0200a21c(recv) && errors == 0 && (recv->versions == 0 || (recv->versions & (1 << GAME_VERSION)))) {
        return TRUE;
    }
    return FALSE;
}
