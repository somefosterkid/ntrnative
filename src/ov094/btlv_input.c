// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov168/btlv_input.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The battle's lower screen. The name is the ROM's string, from GFL_HeapAllocate's asserts. The names are ours; swan
// has none for this file. Another session's notes, ~/Projects/White2Decomp/docs/battle-ui-spec.md (White 2 addresses,
// +0x40), were the source of our understanding of the screens and their transitions, not of names or code

#include "battle/btlv_input.h"
#include "types.h"
#include "battle/btl_main.h"
#include "battle/btl_ov169.h"
#include "battle/btl_pokeparam.h"
#include "battle/btlv.h"
#include "battle/btlv_effect.h"
#include "battle/btlv_finger_cursor.h"
#include "battle/btlv_gauge.h"
#include "battle/btlv_mcss.h"
#include "constants/arc.h"
#include "constants/moves.h"
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
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "pml/poke_party.h"
#include "pml/waza.h"
#include "system/app_menu_common.h"
#include "system/gf_font.h"
#include "system/infowin.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/0021.h"

// The work of a screen task, 0x24 bytes; the fields past seq are set by the task's starter
typedef struct {
    BtlvInput *work;
    s32 seq;
    u32 pos;     // the battle position
    u32 mapFile; // the BG 4 map of the screen, func_ov094_02206a7c's or func_ov094_022050f0's
    s32 x;       // the recorder's BG 4 scroll
    s32 y;
    s32 wait;
    StrBuf *strbuf; // the recorder's message
    u32 prevScreen; // other to standby
} BtlvInputScreenTask;

// The work of a task that needs at most one value
typedef struct {
    BtlvInput *work;
    int value;
} BtlvInputSimpleTask;

// The work of a task that needs only the lower screen's work
typedef struct {
    BtlvInput *work;
} BtlvInputWorkTask;

typedef struct {
    BtlvInput *work;
    fx32 scale;
    fx32 target;
    fx32 step;
    int y;
} BtlvInputBallScale;

typedef struct {
    BtlvInput *work;
    int state;
    int x;
    int y;
    int step;
    int count;
} BtlvInputButtonRise;

typedef struct {
    BtlvInput *work;
    int step;
    int dir;
    int wait;
} BtlvInputFieldFrames;

typedef struct {
    BtlvInput *work;
    ClActUnit *unit;
    ClActor *actors[6];
} BtlvInputTileAnim;

typedef struct {
    BtlvInput *work;
    int dir;
} BtlvInputActorSlide;

typedef struct {
    BtlvInput *work;
    int bg;
    int x;
    int y;
    int bg4;
    int bg5;
    int bg6;
    int bg7;
} BtlvInputSetLayers;

// The press flash's task
typedef struct {
    BtlvInput *work;
    s32 seq;
    u32 rows;
    BOOL objOnly; // flashes the OBJ palette, for Struggle
} BtlvInputFlashWork;

// A BG to create, with its mode
typedef struct {
    BGSetup setup;
    u32 mode;
} BtlvInputBGSetup;

typedef struct {
    int x;
    int y;
} BtlvInputScroll;

typedef struct {
    ClActorPos pos[6];
    u32 sequences[2];
} BtlvInputTileSet;

// MWCC lays out the shared .rodata section by size and sorts the objects of one size (with a heap sort that is not
// stable) in an order that depends on where each object, the functions' local initializers too, is declared. The
// tables are in Black's address order but for three, moved to the places that give Black's layout (found with
// tools/data_order.py's model): data_ov094_0220a380 and data_ov094_0220a3fc after data_ov094_0220a450, and
// data_ov094_0220a8bc after data_ov094_0220a46c; each pointer table follows the tables it points to

// The move screen's slot of each party slot
static const u8 data_ov094_0220a383[3] = { 1, 2, 3 };

// Which of the two extra buttons of the move screen each party slot gets
static const u8 data_ov094_0220a386[3][2] = { { 1, 1 }, { 1, 0 }, { 0, 1 } };

// The pokeSlot that rotate buttons 5 and 6 switch to, by slot and button
static const u8 data_ov094_0220a38c[3][2] = { { 2, 1 }, { 0, 3 }, { 3, 0 } };

// The template of most actors, all zero: each actor is placed and animated after it is created
static const ClActorSetup data_ov094_0220a392 = { 0, 0, 0, 0, 0 };

// The party icons' heights, by row and direction
static const int data_ov094_0220a3b4[2][2] = { { 136, 136 }, { 24, 48 } };

// Where each move tile's type icon goes
static const ClActorPos data_ov094_0220a3c4[4] = { { 32, 64 }, { 160, 64 }, { 32, 112 }, { 160, 112 } };
static const BGSysLCDConfig data_ov094_0220a3d4 = { 1, 0, 3, TRUE };
static const u32 data_ov094_0220a3e4[3][2] = { { 0x247, 0x246 }, { 0x245, 0 }, { 0, 0x245 } };
static const s32 data_ov094_0220a418[7] = { 0, 0, 0, 0, 0x10, 0x4, 0x8 };
static const s32 data_ov094_0220a434[7] = { 0, 0, 0, 0, 1, 0, 0 };
static const s32 data_ov094_0220a450[7] = { 0x200, 0x400, 0x800, 0x1000, 0x10, 0x4, 0x8 };

// The stops the move screen's key cursor may come back to
static const u8 data_ov094_0220a380[3] = { 5, 6, 0xff };
static const s32 data_ov094_0220a3fc[7] = { 0, 0, 0, 0, 1, 0, 0 };
static const BtlvInputScroll data_ov094_0220a46c[2][2] = { { { 256, 0 }, { 0, 192 } }, { { 256, 0 }, { 0, 0 } } };

static const BtlvInputBGSetup data_ov094_0220a8bc[4] = {
#include "asset/btlv_input_data_ov094_0220a8bc.inc"
};

static const TouchRect data_ov094_0220a48c[8] = {
#include "asset/btlv_input_data_ov094_0220a48c.inc"
};

// The move screen's buttons, without a usable move and with one
static const BtlvInputButtonSet data_ov094_0220a3a8 = { data_ov094_0220a48c, data_ov094_0220a3fc, data_ov094_0220a418 };

// Where each move tile's name is centered
static const s32 data_ov094_0220a4ac[4][2] = { { 64, 10 }, { 192, 10 }, { 64, 58 }, { 192, 58 } };

static const TouchRect data_ov094_0220a4cc[8] = {
#include "asset/btlv_input_data_ov094_0220a4cc.inc"
};
static const BtlvInputButtonSet data_ov094_0220a39c = { data_ov094_0220a4cc, data_ov094_0220a434, data_ov094_0220a450 };

// The scroll of BG 4 for the button rise of each slot
static const BtlvInputScroll data_ov094_0220a4ec[4] = { { 256, 448 }, { 256, 192 }, { 0, 192 }, { 0, 448 } };

// Where each party icon goes, by rule
static const ClActorPos data_ov094_0220a50c[4][3] = {
#include "asset/btlv_input_data_ov094_0220a50c.inc"
};

// The palette file of each move type's tile
// Aligned: with an explicit alignment on an object it reads, MWCC enters func_ov094_02204cc0's loop at its test and
// shares no address across its calls, as Black does (pokebw2's matching.md, BtlvInput_DrawMoveTiles)
static const u32 data_ov094_0220a5f0[17] ATTRIBUTE_ALIGN(4) = {
#include "asset/btlv_input_data_ov094_0220a5f0.inc"
};

// The key cursor's stops of the move screen, without a usable move and with one, per slot
static const BtlvInputKeyStop data_ov094_0220a634[6] = {
#include "asset/btlv_input_data_ov094_0220a634.inc"
};

static const BtlvInputKeyStop data_ov094_0220a67c[6] = {
#include "asset/btlv_input_data_ov094_0220a67c.inc"
};

static const BtlvInputKeyStop data_ov094_0220a6c4[6] = {
#include "asset/btlv_input_data_ov094_0220a6c4.inc"
};

static const BtlvInputKeyStop data_ov094_0220a70c[6] = {
#include "asset/btlv_input_data_ov094_0220a70c.inc"
};

static const BtlvInputKeyStop data_ov094_0220a754[7] = {
#include "asset/btlv_input_data_ov094_0220a754.inc"
};

// The key cursor's stops of the move screen of each slot, without a usable move and with one
static const BtlvInputKeyStop *data_ov094_0220ae98[3] = { data_ov094_0220a754, data_ov094_0220a634,
                                                          data_ov094_0220a67c };

static const BtlvInputKeyStop data_ov094_0220a7a8[7] = {
#include "asset/btlv_input_data_ov094_0220a7a8.inc"
};

static const BtlvInputKeyStop *data_ov094_0220aea4[3] = { data_ov094_0220a7a8, data_ov094_0220a6c4,
                                                          data_ov094_0220a70c };

// Where each target panel's name is centered, by rule
static const s32 data_ov094_0220a7fc[2][6][2] = {
#include "asset/btlv_input_data_ov094_0220a7fc.inc"
};

// The tiles that grow or shrink, by set
static const BtlvInputTileSet data_ov094_0220a85c[3] = {
#include "asset/btlv_input_data_ov094_0220a85c.inc"
};

// The target screen's BG 4 map, by rule, attacker and move range
static const u32 data_ov094_0220a94c[2][3][15] = {
#include "asset/btlv_input_data_ov094_0220a94c.inc"
};

static BtlvInput *func_ov094_022020a0(GameData *gameData, u32 rule, u32 unk54, PaletteFade *paletteFade, Font *font,
                                      u8 *keyCursorFlag, u32 scdState, BOOL inBattle, HeapID heapId);
void func_ov094_02202238(TCB *tcb, void *data);
void func_ov094_02202248(BtlvInput *work);
static void func_ov094_022023d0(BtlvInput *work);
void func_ov094_0220255c(BtlvInput *work);
static void func_ov094_022025b4(void);
void func_ov094_022035c0(TCB *tcb, void *data);
void func_ov094_022036c4(TCB *tcb, void *data);
void func_ov094_02203880(TCB *tcb, void *data);
void func_ov094_02203978(TCB *tcb, void *data);
void func_ov094_02203bc0(TCB *tcb, void *data);
void func_ov094_02203c2c(TCB *tcb, void *data);
void func_ov094_02203b08(TCB *tcb, void *data);
void func_ov094_02203a78(TCB *tcb, void *data);
void func_ov094_02203e30(TCB *tcb, void *data);
void func_ov094_022041e0(TCB *tcb, void *data);
void func_ov094_02203fdc(TCB *tcb, void *data);
void func_ov094_022040fc(TCB *tcb, void *data);
void func_ov094_02203d40(TCB *tcb, void *data);
void func_ov094_022042e4(TCB *tcb, void *data);
void func_ov094_022043ac(TCB *tcb);
void func_ov094_022043c0(BtlvInput *work, fx32 from, fx32 to, fx32 step, int y);
void func_ov094_02204438(TCB *tcb, void *data);
void func_ov094_022044a0(TCB *tcb);
void func_ov094_022044c0(BtlvInput *work, int x, int y, int step, int count);
void func_ov094_0220453c(TCB *tcb, void *data);
void func_ov094_02204588(TCB *tcb);
void func_ov094_022045a8(BtlvInput *work, int unk1, int unk2);
void func_ov094_0220461c(TCB *tcb, void *data);
void func_ov094_02204678(TCB *tcb);
void func_ov094_02204698(BtlvInput *work, int unk1, int unk2);
void func_ov094_022047cc(TCB *tcb, void *data);
void func_ov094_022047f8(TCB *tcb);
void func_ov094_02204838(BtlvInput *work, int dir);
void func_ov094_022048a4(TCB *tcb, void *data);
void func_ov094_02204940(TCB *tcb);
void func_ov094_02204960(BtlvInput *work, int bg, int x, int y, int bg4, int bg5, int bg6, int bg7);
void func_ov094_022049c8(TCB *tcb, void *data);
static void func_ov094_02204a3c(TCB *tcb, void *data);
static void func_ov094_02204a70(TCB *tcb);
void func_ov094_02204a88(TCB *tcb, void *data);
void func_ov094_02204aec(TCB *tcb);
void func_ov094_02204b0c(BtlvInput *work, int screen);
static void func_ov094_02204bac(TCB *tcb, void *data);
void func_ov094_02204bcc(TCB *tcb, void *data);
static void func_ov094_02204c48(TCB *tcb, void *data);
void func_ov094_02204c98(const StrBuf *strbuf, Font *font, int *width, int *blocks);
void func_ov094_02204cc0(BtlvInput *work, const BtlvInputMoveParam *param);
void func_ov094_022050f0(BtlvInput *work, BtlvInputScreenTask *task, const BtlvInputTargetParam *param);
void func_ov094_02205340(BtlvInput *work, const BtlvInputYesNoParam *param);
void func_ov094_022053dc(BtlvInput *work);
void func_ov094_0220558c(BtlvInput *work, const BtlvInputRecorderParam *param);
void func_ov094_022056b4(BtlvInput *work);
s32 func_ov094_02205768(s32 pp, s32 maxPP);
void func_ov094_022057cc(BtlvInput *work, const BtlvInputCommandParam *param);
static void func_ov094_02205ab8(BtlvInput *work);
void func_ov094_02205b80(BtlvInput *work, BOOL active);
void func_ov094_02205b90(BtlvInput *work, int index);
static void func_ov094_02205d34(BtlvInput *work);
void func_ov094_02205d58(BtlvInput *work, int side);
static void func_ov094_02205e30(BtlvInput *work);
void func_ov094_02205e80(BtlvInput *work);
static void func_ov094_02205f40(BtlvInput *work);
static void func_ov094_02205f64(BtlvInput *work);
static void func_ov094_02205f88(BtlvInput *work);
void func_ov094_02206128(BtlvInput *work);
s32 func_ov094_02206140(BtlvInput *work, const BtlvInputButtonSet *set, const BtlvInputKeyStop *stops,
                              const u8 *lastStops, s32 result, BOOL noMoveInfo);
s32 func_ov094_0220648c(BtlvInput *work);
void func_ov094_022064cc(BtlvInput *work, const TouchRect *rects, const BtlvInputKeyStop *stops);
s32 func_ov094_02206578(BtlvInput *work, s32 index, s32 rows);
void func_ov094_0220663c(TCB *tcb, void *data);
void func_ov094_02206710(TCB *tcb);
void func_ov094_02206728(BtlvInput *work, BtlvInputCommandParam *param);
void func_ov094_022067e8(BtlvInput *work);
void func_ov094_0220680c(BtlvInput *work);
static void func_ov094_0220684c(BtlvInput *work, TCB *task, void (*endFunc)(TCB *tcb));
static int func_ov094_02206864(BtlvInput *work, TCB *task);
static void func_ov094_0220687c(BtlvInput *work, TCB *task);
static int func_ov094_022068c4(BtlvInput *work);
static void func_ov094_022068e8(BtlvInput *work);
void func_ov094_02206950(BtlvInput *work);
s32 func_ov094_02206908(BtlvInput *work, const BtlvInputButtonSet *set, u32 index);
void func_ov094_02206a14(BtlvInput *work);
void func_ov094_02206a58(BtlvInput *work);

BtlvInput *func_ov094_02202058(GameData *gameData, u32 rule, PaletteFade *paletteFade, Font *font, u8 *keyCursorFlag,
                               HeapID heapId) {
    return func_ov094_022020a0(gameData, rule, 0, paletteFade, font, keyCursorFlag, 0, FALSE, heapId);
}

BtlvInput *func_ov094_0220207c(GameData *gameData, u32 rule, u32 unk54, PaletteFade *paletteFade, Font *font,
                               u8 *keyCursorFlag, u32 scdState, HeapID heapId) {
    return func_ov094_022020a0(gameData, rule, unk54, paletteFade, font, keyCursorFlag, scdState, TRUE, heapId);
}

static BtlvInput *func_ov094_022020a0(GameData *gameData, u32 rule, u32 unk54, PaletteFade *paletteFade, Font *font,
                                      u8 *keyCursorFlag, u32 scdState, BOOL inBattle, HeapID heapId) {
    BtlvInput *work;
    BtlvInputSimpleTask *coreTask;
    int i;

    work = GFL_HeapAllocate(heapId, sizeof(BtlvInput), TRUE, "btlv_input.c", 0x39a);
    work->heapId = heapId;
    work->tcbMgrBuf = GFL_HeapAllocate(work->heapId, GFL_TCBMgrCalcAllocSize(8), TRUE, "btlv_input.c", 0x39e);
    work->tcbMgr = GFL_TCBMgrCreate(8, work->tcbMgrBuf);
    work->gameData = gameData;
    work->font = font;
    work->rule = rule;
    work->unk54 = unk54;
    work->keyCursorFlag = keyCursorFlag;
    work->prevCursor = 0xf;
    work->inBattle = inBattle;
    work->paletteFade = paletteFade;
    work->chars = -1;
    work->palette = -1;
    work->cellAnims = -1;
    work->chars2 = -1;
    work->cellAnims2 = -1;
    work->iconPalette = -1;
    work->iconCellAnims = -1;
    work->movePalette = -1;
    work->moveCellAnims = -1;
    for (i = 0; i < 3; i++) {
        work->iconChars[i] = -1;
    }
    for (i = 0; i < 4; i++) {
        work->moveChars[i] = -1;
    }
    work->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0021, work->heapId);
    work->pressedButton = -1;
    func_ov094_02202248(work);

    coreTask = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputSimpleTask), FALSE, "btlv_input.c", 0x3cc);
    coreTask->work = work;
    coreTask->value = scdState;
    func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02204bac, coreTask, 0), NULL);
    return work;
}

// pokebw2's BtlvInput_Delete
void func_ov094_022021f4(BtlvInput *work) {
    if (work != NULL) {
        func_ov094_022068e8(work);
        func_ov094_022023d0(work);
        GFL_MsgDataFree(work->msgData);
        if (work->fingerCursor != NULL) {
            func_ov094_02209b30(work->fingerCursor);
        }
        func_02030da4(work->tcbMgr);
        GFL_HeapFree(work->tcbMgrBuf);
        GFL_HeapFree(work);
    }
}

// pokebw2's BtlvInput_Update
void func_ov094_02202228(BtlvInput *work) {
    GFL_TCBMgrUpdate(work->tcbMgr);
    func_02024abc();
}

// pokebw2's BtlvInput_CoreTask
void func_ov094_02202238(TCB *tcb, void *data) {
    BtlvInput *work = data;

    GFL_TCBMgrUpdate(work->tcbMgr);
    func_02024abc();
}

void func_ov094_022033f4(BtlvInput *work);

ClActUnit *func_0204b100(u16 count, u8 priority, HeapID heapId);
void func_02024a20(u8 bg, u8 palette, GameData *gameData, HeapID heapId);
void func_0203d34c(BOOL top, HeapID heapId);
void func_020216d4(PaletteFade *fade, u32 vram, u16 offset, u32 size);
void func_ov094_0220270c(BtlvInput *work, u32 screen, void *param);
TCBManager *func_ov094_021f86d4(void);

void func_ov094_02202248(BtlvInput *work) {
    GFL_BGSysSetLCDConfigForEngine(&data_ov094_0220a3d4, 1);
    work->arc = GFL_ArcSysCreateFileHandle(11, work->heapId);
    work->moveUnit = func_0204b100(4, 2, work->heapId);
    work->pokeUnit = func_0204b100(12, 2, work->heapId);
    work->cursorUnit = func_0204b100(6, 0, work->heapId);
    work->iconUnit = func_0204b100(3, 2, work->heapId);
    work->unit20c = func_0204b100(2, 2, work->heapId);
    work->unit218 = func_0204b100(1, 1, work->heapId);
    func_ov094_0220255c(work);
    func_ov094_022033f4(work);
    func_ov094_02205e80(work);
    work->msgWin = BmpWin_CreateDynamic(6, 0, 4, 32, 12, 13, TRUE);
    work->msgBitmap = BmpWin_GetBitmap(work->msgWin);
    work->subWin = BmpWin_CreateDynamic(6, 3, 22, 3, 2, 13, TRUE);
    work->subBitmap = BmpWin_GetBitmap(work->subWin);
    GFL_BitmapFill(work->msgBitmap, 0);
    GFL_BitmapFill(work->subBitmap, 0);
    BmpWin_FlushChar(work->msgWin);
    BmpWin_FlushChar(work->subWin);
    BmpWin_FlushMap(work->msgWin);
    BmpWin_FlushMap(work->subWin);
    GFL_BGSysLoadScr(6);
    func_02024a20(6, 15, work->gameData, work->heapId);
    GFL_BGSysSetBGEnabled(6, TRUE);
    GFL_BGSysSetBGEnabledB(0x10, TRUE);
    func_0203d34c(FALSE, work->heapId);
    func_020216d4(work->paletteFade, 3, 0xe0, 0x20);
    work->cursorRefresh = TRUE;
    if (work->inBattle == TRUE) {
        work->coreTask = GFL_TCBMgrAddTask(func_ov094_021f86d4(), func_ov094_02202238, work, 0);
    }
    func_ov094_0220270c(work, 0, NULL);
}


// pokebw2's BtlvInput_FreeGraphics
static void func_ov094_022023d0(BtlvInput *work) {
    int i;

    func_ov094_02205f64(work);
    func_ov094_02205e30(work);
    func_ov094_02205f40(work);
    func_ov094_02205ab8(work);
    func_ov094_02205d34(work);
    if (work->chars != -1) {
        func_0204a8d4(work->chars);
        work->chars = -1;
    }
    if (work->palette != -1) {
        func_0204ada4(work->palette);
        work->palette = -1;
    }
    if (work->cellAnims != -1) {
        func_0204afd8(work->cellAnims);
        work->cellAnims = -1;
    }
    if (work->chars2 != -1) {
        func_0204a8d4(work->chars2);
        work->chars2 = -1;
    }
    if (work->cellAnims2 != -1) {
        func_0204afd8(work->cellAnims2);
        work->cellAnims2 = -1;
    }
    for (i = 0; i < 4; i++) {
        if (work->moveChars[i] != -1) {
            func_0204a8d4(work->moveChars[i]);
            work->moveChars[i] = -1;
        }
    }
    if (work->moveCellAnims != -1) {
        func_0204afd8(work->moveCellAnims);
        work->moveCellAnims = -1;
    }
    if (work->movePalette != -1) {
        func_0204ada4(work->movePalette);
        work->movePalette = -1;
    }
    if (work->moveUnit != NULL) {
        func_0204b1cc(work->moveUnit);
        work->moveUnit = NULL;
    }
    if (work->pokeUnit != NULL) {
        func_0204b1cc(work->pokeUnit);
        work->pokeUnit = NULL;
    }
    if (work->cursorUnit != NULL) {
        func_0204b1cc(work->cursorUnit);
        work->cursorUnit = NULL;
    }
    if (work->iconUnit != NULL) {
        func_0204b1cc(work->iconUnit);
        work->iconUnit = NULL;
    }
    if (work->unit20c != NULL) {
        func_0204b1cc(work->unit20c);
        work->unit20c = NULL;
    }
    if (work->unit218 != NULL) {
        func_0204b1cc(work->unit218);
        work->unit218 = NULL;
    }
    if (work->msgWin != NULL) {
        BmpWin_Free(work->msgWin);
        work->msgWin = NULL;
    }
    if (work->subWin != NULL) {
        BmpWin_Free(work->subWin);
        work->subWin = NULL;
    }
    func_02024dcc();
    func_ov094_022025b4();
    if (work->coreTask != NULL) {
        GFL_TCBRemove(work->coreTask);
        work->coreTask = NULL;
    }
    if (work->arc != NULL) {
        GFL_ArcToolFree(work->arc);
        work->arc = NULL;
    }
    work->screen = 0;
}

// pokebw2's BtlvInput_CreateBGs
void func_ov094_0220255c(BtlvInput *work) {
    u32 i;

    for (i = 0; i < 4; i++) {
        GFL_BGSysCreateBG(i + 4, &data_ov094_0220a8bc[i].setup, data_ov094_0220a8bc[i].mode);
        GFL_BGSysFillScr(i + 4, 0x3ff);
        func_ov094_02204960(work, i + 4, 0, 0, 2, 2, 2, 2);
    }
}

// pokebw2's BtlvInput_ReleaseBGs
static void func_ov094_022025b4(void) {
    u32 i;

    for (i = 0; i < 4; i++) {
        GFL_BGSysSetBGEnabled(i + 4, FALSE);
        GFL_BGSysReleaseBG(i + 4);
    }
}

// pokebw2's BtlvInput_StartPaletteFade
void func_ov094_022025d8(BtlvInput *work, u32 mode) {
    BtlvInputWorkTask *task;

    task = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputWorkTask), FALSE, "btlv_input.c", 0x4f7);
    if (mode == 1) {
        func_0202173c(work->paletteFade, 0xa, 0xffff, 1, 0, 16, 0, work->tcbMgr);
    } else {
        func_0202173c(work->paletteFade, 0xa, 0xffff, 1, 12, 16, 0, work->tcbMgr);
    }
    work->fadeMode = 1;
    task->work = work;
    func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02204a3c, task, 0), func_ov094_02204a70);
}

// pokebw2's BtlvInput_StartFadeInWithGraphics
void func_ov094_0220266c(BtlvInput *work) {
    BtlvInputWorkTask *task;

    task = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputWorkTask), FALSE, "btlv_input.c", 0x50c);
    if (GXx_GetMasterBrightness_(REG_DB_MASTER_BRIGHT_ADDR) <= 0) {
        GFL_FadeSet(FADE_ENGINE_B_BLACK, 16, 0, 0);
    } else {
        GFL_FadeSet(FADE_ENGINE_B_WHITE, 16, 0, 0);
    }
    func_ov094_02202248(work);
    work->fadeMode = 2;
    task->work = work;
    func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02204a3c, task, 0), func_ov094_02204a70);
}

// pokebw2's BtlvInput_IsFading
BOOL func_ov094_022026fc(BtlvInput *work) {
    if (work->fadeMode != 0) {
        return TRUE;
    }
    return FALSE;
}


void func_ov094_02206b30(BtlvInput *work);
void func_ov094_0220270c(BtlvInput *work, u32 screen, void *param) {
    func_ov094_022056b4(work);
    work->fingerSeq = 0;
    work->canCancel = TRUE;
    switch (screen) {
    case 0: {
        BtlvInputScreenTask *task;

        work->pressedButton = -1;
        if (work->screen == 0) {
            func_ov094_022043c0(work, 0x3000, 0x3000, 0, 0x98);
            func_ov094_02204960(work, -1, 0, 0, 0, 0, 2, 1);
            func_0202173c(work->paletteFade, 2, 3, 1, 12, 12, 0x842, work->tcbMgr);
        } else {
            task = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputScreenTask), TRUE, "btlv_input.c", 0x549);
            work->busy = TRUE;
            task->work = work;
            if (work->inBattle == 1) {
                func_ov094_021f8a78();
            }
            func_ov094_02205e30(work);
            func_ov094_02205ab8(work);
            if (work->screen == 1 || work->screen == 4) {
                func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02203bc0, task, 1), func_ov094_022043ac);
            } else {
                task->prevScreen = work->screen;
                func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02203c2c, task, 1), func_ov094_022043ac);
            }
                func_02021848(work->paletteFade, 2, 3, 1, 0, 12, 0x842, work->tcbMgr);
            if (func_0204b248(work->cursorUnit) != 0) {
                work->cursor = 0;
                work->prevCursor = 0xf;
                work->cursorRefresh = TRUE;
                func_0204b22c(work->cursorUnit, FALSE);
            }
        }
        break;
    }
    case 1: {
        BtlvInputCommandParam *command = param;
        BtlvInputScreenTask *task;
        int i;

        task = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputScreenTask), TRUE, "btlv_input.c", 0x570);
        work->pokePos = command->pokePos;
        func_ov094_022067e8(work);
        func_ov094_021f8a40(2);
        if (command->pokePos >= 2) {
            command->pokePos = (command->pokePos - 2) / 2;
        }
        work->busy = TRUE;
        work->laterPoke = command->laterPoke;
        task->work = work;
        task->pos = command->pokePos;
        work->pokeIndex = command->pokePos;
        if (work->screen == 0) {
            func_ov094_0220680c(work);
        }
        for (i = 0; i < 6; i++) {
            work->pokeEntries[0][i] = command->entries[0][i];
            work->pokeEntries[1][i] = command->entries[1][i];
        }
        for (i = 0; i < 4; i++) {
            work->buttonEnabled[i] = TRUE;
        }
        work->buttonEnabled[1] = func_ov094_021f8914();
        func_ov094_02205ab8(work);
        func_ov094_022057cc(work, command);
        func_ov094_02205b90(work, command->ballSlide);
        task->mapFile = func_ov094_02206a7c(work);
        func_ov094_02206728(work, command);
        if (work->screen == 2 || work->screen == 5) {
            func_ov094_02205b80(work, FALSE);
            func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02203978, task, 1), func_ov094_022043ac);
        } else {
            func_ov094_02205d58(work, 0);
            if (command->secondRow != 0) {
                func_ov094_02205d58(work, 1);
            }
            work->secondRow = command->secondRow;
            func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_022035c0, task, 1), func_ov094_022043ac);
        }
        break;
    }
    case 2: {
        BtlvInputMoveParam *move = param;
        BtlvInputScreenTask *task;
        int i;

        task = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputScreenTask), TRUE, "btlv_input.c", 0x5bd);
        func_ov094_02204cc0(work, move);
        work->busy = TRUE;
        task->work = work;
        task->pos = move->pos;
        work->noMoveInfo = move->noMoveInfo;
        task->mapFile = func_ov094_02206a7c(work);
        for (i = 0; i < 4; i++) {
            work->moves[i] = move->moves[i];
        }
        if (work->inBattle == 1 && work->rule - 1 <= 1) {
            func_ov094_021f8ac0(2);
            func_ov094_021f7998(0x244);
        }
        if (work->screen == 3) {
            func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02203b08, task, 1), func_ov094_022043ac);
        } else if (work->screen == 0) {
            func_ov094_02205d58(work, 0);
            if (work->secondRow) {
                func_ov094_02205d58(work, 1);
            }
            func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_022036c4, task, 1), func_ov094_022043ac);
        } else {
            func_ov094_02205ab8(work);
            func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02203880, task, 1), func_ov094_022043ac);
        }
        break;
    }
    case 3: {
        BtlvInputTargetParam *target = param;
        BtlvInputScreenTask *task;

        task = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputScreenTask), TRUE, "btlv_input.c", 0x5ec);
        func_ov094_022050f0(work, task, target);
        work->busy = TRUE;
        task->work = work;
        task->pos = target->attackerPos;
        func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02203a78, task, 1), func_ov094_022043ac);
        break;
    }
    case 4: {
        BtlvInputYesNoParam *yesNo = param;
        BtlvInputScreenTask *task;

        task = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputScreenTask), TRUE, "btlv_input.c", 0x5f8);
        func_ov094_02205340(work, yesNo);
        work->busy = TRUE;
        task->work = work;
        work->canCancel = yesNo->canCancel;
        func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02203d40, task, 1), func_ov094_022043ac);
        break;
    }
    case 5: {
        BtlvInputRotationParam *rotation = param;
        BtlvInputScreenTask *task;
        int i;
        int j;

        task = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputScreenTask), TRUE, "btlv_input.c", 0x605);
        if (work->screen != 0) {
            work->unk68_17 = 0;
            work->pokeSlot = 0;
        }
        for (i = 0; i < 3; i++) {
            work->mons[i] = rotation->mons[i];
            for (j = 0; j < 4; j++) {
                work->moveUsable[i][j] = rotation->moveUsable[i][j];
            }
        }
        func_ov094_022053dc(work);
        work->busy = TRUE;
        task->work = work;
        if (work->screen == 0) {
            func_ov094_02205d58(work, 0);
            if (work->secondRow) {
                func_ov094_02205d58(work, 1);
            }
            func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02203e30, task, 1), func_ov094_022043ac);
        } else {
            func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_022041e0, task, 1), func_ov094_022043ac);
        }
        break;
    }
    case 6: {
        BtlvInputRecorderParam *recorder = param;
        BtlvInputScreenTask *task;
        int i;
        StrBuf *strbuf = NULL;

        for (i = 0; i < 3; i++) {
            work->buttonEnabled[i] = TRUE;
        }
        func_ov094_0220558c(work, recorder);
        switch (recorder->mode) {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
            GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
            strbuf = GFL_MsgDataLoadStrbufNew(work->msgData, recorder->mode + 8);
            break;
        }
        if (work->screen == 0) {
            task = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputScreenTask), TRUE, "btlv_input.c", 0x65f);
            work->busy = TRUE;
            task->work = work;
            if (recorder->mode == 0) {
                task->x = 0;
                task->y = 0x100;
            } else {
                task->x = 0x100;
                task->y = 0;
            }
            task->strbuf = strbuf;
            func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02203fdc, task, 1), func_ov094_022043ac);
            } else {
            if (recorder->mode == 0) {
                func_ov094_02204960(work, 4, 0, 0x100, 2, 2, 2, 2);
            } else {
                func_ov094_02204960(work, 4, 0x100, 0, 2, 2, 2, 2);
            }
            if (strbuf != NULL) {
                GFL_TextRendererDrawToBitmap(work->msgBitmap, 0x18, 0x38, strbuf, work->font);
                GFL_StrBufFree(strbuf);
            }
            BmpWin_FlushMap(work->msgWin);
            GFL_BGSysLoadScr(6);
            BmpWin_FlushChar(work->msgWin);
        }
        break;
    }
    case 7: {
        BtlvInputScreenTask *task;

        task = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputScreenTask), TRUE, "btlv_input.c", 0x68c);
        work->busy = TRUE;
        task->work = work;
        func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_022040fc, task, 1), func_ov094_022043ac);
        work->buttonEnabled[0] = TRUE;
        work->buttonEnabled[1] = TRUE;
        break;
    }
    }
    work->screen = screen;
}

s32 func_ov094_02202dc4(BtlvInput *work, const BtlvInputButtonSet *set, const BtlvInputKeyStop *stops) {
    s32 result;
    s32 touch;
    u32 held;

    if (work->busy || func_02021ed8(work->paletteFade) != 0 || work->inputLocked) {
        return -1;
    }
    if (work->pressedButton != -1) {
        result = work->pressedButton;
        work->selectionLocked = FALSE;
        work->pressedButton = -1;
        return result;
    }
    if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_START) && work->screen - 1 <= 2) {
        func_ov094_021f8168();
    }
    if (work->screen == 6) {
        result = func_02035c08(set->rects);
        if (result != -1) {
            if (work->buttonEnabled[result] == 0) {
                result = -1;
            } else if (result == 2) {
                func_ov094_02206b58(work);
            } else {
                func_ov094_02206b30(work);
            }
        } else {
            result = func_ov094_0220648c(work);
        }
        if (result != -1) {
            result = func_ov094_02206578(work, result, set->flashRows[result]);
        }
        return result;
    }
    touch = func_02035c08(set->rects);
    result = func_ov094_02206140(work, set, stops, NULL, touch, work->noMoveInfo);
    if (result != -1) {
        held = GCTX_HIDGetHeldKeys();
        if (work->buttonEnabled[result] == 0 && (!(held & PAD_BUTTON_L) || work->screen != 2)) {
            if (work->moveExists[result] != 0 && work->screen == 2) {
                func_ov094_02206bac(work);
            }
            result = -1;
        } else if (result < 4 && (held & PAD_BUTTON_L) && work->screen == 2 && work->noMoveInfo == 1) {
            result = -1;
        } else if (result < 4 && (held & PAD_BUTTON_L) && work->screen == 2 && work->moveExists[result] == 0) {
            result = -1;
        } else if (touch != -1) {
            if (func_ov094_02206908(work, set, result) == 0 && !work->selectionLocked) {
                func_ov094_02206b30(work);
                switch (work->screen) {
                case 1:
                case 2:
                case 5:
                    work->lastCursor[work->pokeIndex][work->screen] = result;
                    break;
                case 3: {
                    const BtlvInputKeyStop *row;
                    u8 count;
                    int found = 0;
                    int i;

                    if (work->rule != 2) {
                        count = data_ov095_0689e090[work->pokeIndex][work->pos];
                    } else {
                        count = data_ov095_0689e0b0[work->pokeIndex][work->pos];
                    }
                    if (work->rule != 2) {
                        row = data_ov095_0689e560[work->pokeIndex][work->pos];
                    } else {
                        row = data_ov095_0689e704[work->pokeIndex][work->pos];
                    }
                    for (i = 0; i < count; i++) {
                        if (result == row[i].a) {
                            found = i;
                            break;
                        }
                    }
                    work->lastCursor[work->pokeIndex][work->screen] = found | (work->lastCursor[work->pokeIndex][2] << 4);
                    break;
                }
                }
            } else {
                func_ov094_02206b58(work);
            }
        }
    }
    if (result != -1) {
        result = func_ov094_02206578(work, result, set->flashRows[result]);
    }
    return result;
}

u32 func_ov094_02203040(BtlvInput *work) {
    u32 result = 0;

    switch (work->fingerSeq) {
    case 0: {
        u32 pos[3][2] = { { 0x80, 0x48 }, { 0x40, 0x30 }, { 0x28, 0x98 } };

        if (work->fingerCursor == NULL) {
            work->fingerCursor = BtlvFingerCursor_Create(work->paletteFade, 0xb, work->heapId);
        }
        if (func_ov094_02209b60(work->fingerCursor, pos[work->fingerCount][0], pos[work->fingerCount][1], 2, 7,
                                0x10)) {
            work->fingerSeq++;
            work->fingerCount++;
        }
        break;
    }
    case 1:
        result = func_ov094_02209c2c(work->fingerCursor);
        if (result == 1) {
            func_ov094_02206b30(work);
        }
        break;
    }
    return result;
}

BOOL func_ov094_022030f0(BtlvInput *work, u8 *slot, u32 *button) {
    s32 result;
    s32 touch;
    s32 cancel;
    u32 held;
    u8 enabled;
    int pressed;

    if (work->busy || func_02021ed8(work->paletteFade) != 0 || work->inputLocked) {
        return FALSE;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_START) {
        func_ov094_021f8168();
    }
    if (work->pressedButton != -1) {
        pressed = work->pressedButton & ~0x8000;
        if (pressed < 5) {
            if (pressed < 4 && work->noUsableMove == 1) {
                work->pressedButton = 7;
            }
            *button = work->pressedButton;
            *slot = data_ov094_0220a383[work->pokeSlot];
            work->pressedButton = -1;
            work->selectionLocked = FALSE;
            func_02021848(work->paletteFade, 2, 0x3e00, 0, 0, 0, 0, work->tcbMgr);
            func_02021848(work->paletteFade, 8, 0x700, 0, 0, 0, 0, work->tcbMgr);
            return TRUE;
        }
        work->cursor = 0;
        work->lastCursor[work->pokeIndex][work->screen] = 0;
        work->selectionLocked = FALSE;
        func_ov094_02206b74(work);
        func_ov094_02204b0c(work, pressed);
        work->pressedButton = -1;
        return FALSE;
    }
    if (work->noUsableMove == 1) {
        touch = func_02035c08(data_ov094_0220a3a8.rects);
        result = func_ov094_02206140(work, &data_ov094_0220a3a8, data_ov094_0220ae98[work->pokeSlot], NULL, touch,
                                     TransformCheck(work->mons[work->pokeSlot]));
    } else {
        touch = func_02035c08(data_ov094_0220a39c.rects);
        result = func_ov094_02206140(work, &data_ov094_0220a39c, data_ov094_0220aea4[work->pokeSlot],
                                     data_ov094_0220a380, touch, TransformCheck(work->mons[work->pokeSlot]));
    }
    if (result != -1) {
        held = GCTX_HIDGetHeldKeys();
        enabled = work->buttonEnabled[result];
        if (enabled == 0 && !(held & PAD_BUTTON_L)) {
            if (work->moveExists[result] != 0) {
                func_ov094_02206bac(work);
            }
            result = -1;
        } else if (result > 4 && enabled == 0) {
            result = -1;
        } else if (result < 4 && (held & PAD_BUTTON_L) &&
                   (TransformCheck(work->mons[work->pokeSlot]) == 1 || work->noUsableMove == 1)) {
            result = -1;
        } else if (result < 4 && (held & PAD_BUTTON_L) && work->moveExists[result] == 0) {
            result = -1;
        }
    }
    if (result != -1) {
        if (work->noUsableMove == 1) {
            cancel = func_ov094_02206908(work, &data_ov094_0220a3a8, result);
            if (cancel == 0 && !work->selectionLocked) {
                work->lastCursor[work->pokeIndex][work->screen] = result;
                func_ov094_02206b90(work);
            } else if (cancel == 1 && !work->selectionLocked) {
                func_ov094_02206b58(work);
            }
            result = func_ov094_02206578(work, result, data_ov094_0220a3a8.flashRows[result]);
        } else {
            cancel = func_ov094_02206908(work, &data_ov094_0220a39c, result);
            if (cancel == 0 && !work->selectionLocked) {
                work->lastCursor[work->pokeIndex][work->screen] = result;
                func_ov094_02206b90(work);
            } else if (cancel == 1 && !work->selectionLocked) {
                func_ov094_02206b58(work);
            }
            result = func_ov094_02206578(work, result, data_ov094_0220a39c.flashRows[result]);
        }
    }
    if (result == -1) {
        return FALSE;
    }
    return TRUE;
}



// Whether a screen change, a palette fade or a lock blocks the input
// pokebw2's BtlvInput_IsBusy
BOOL func_ov094_022033cc(BtlvInput *work) {
    if (work->busy || func_02021ed8(work->paletteFade) != 0 || work->inputLocked) {
        return TRUE;
    }
    return FALSE;
}

u32 func_ov094_022033f0(BtlvInput *work) {
    return work->screen;
}

void func_ov094_022033f4(BtlvInput *work) {
    ArcTool *arc;
    int i;

    GFL_BGSysLoadArcNCGRStatic(work->arc, 0x5b, 4, 0, 0, FALSE, work->heapId);
    GFL_BGSysLoadArcNCGRStatic(work->arc, 0x71, 7, 0, 0x8000, FALSE, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(work->arc, 0x72, 7, 0, 0, FALSE, work->heapId);
    func_020216b4(work->paletteFade, work->arc, 0x5c, work->heapId, 1, 0x1e0, 0);
    if (work->inBattle == 1) {
        MIi_CpuCopy16(func_02021728(func_ov094_021f86e0(), 1) + 0x20, work->savedPalette, 0x20);
    }
    work->chars = func_0204a6c8(work->arc, 0x73, FALSE, 1, work->heapId);
    work->cellAnims = func_0204af28(work->arc, 0x75, 0x76, work->heapId);
    work->palette = func_0204abf0(work->arc, 0x74, 1, 0, work->heapId);
    func_020216d4(work->paletteFade, 3, func_0204aef4(work->palette, TRUE) >> 1, 0x100);
    work->chars2 = func_0204a6c8(work->arc, 0x77, FALSE, 1, work->heapId);
    work->cellAnims2 = func_0204af28(work->arc, 0x78, 0x79, work->heapId);
    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_TAIL(work->heapId));
    work->moveCellAnims = func_0204af28(arc, func_020275fc(0), func_02027600(0), work->heapId);
    work->movePalette = func_0204abf0(arc, func_020275e8(), 1, 0x100, work->heapId);
    func_020216d4(work->paletteFade, 3, func_0204aef4(work->movePalette, TRUE) >> 1, 0x60);
    for (i = 0; i < 4; i++) {
        work->moveChars[i] = func_0204a6c8(arc, func_020275f8(0), FALSE, 1, work->heapId);
    }
    GFL_ArcToolFree(arc);
}


// Standby to command: the ball opens while the buttons rise
void func_ov094_022035c0(TCB *tcb, void *data) {
    BtlvInputScreenTask *param = data;

    switch (param->seq) {
    case 0:
        loadBGScrToVramByFileNoReserveNegAlign(param->work->arc, param->mapFile, 4, 0, 0, FALSE, param->work->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(param->work->arc, 0x68, 5, 0, 0, FALSE,
                                               param->work->heapId);
        func_ov094_02206af8(param->work);
        func_ov094_02204960(param->work, 5, 0, 0, 1, 0, 2, 1);
        func_ov094_022043c0(param->work, 0x3000, 0x1000, -0x400, 0x98);
        func_ov094_022044c0(param->work, 0, 0x1c0, 8, 8);
        func_ov094_02206a14(param->work);
        BmpWin_FlushMap(param->work->subWin);
        GFL_BGSysLoadScr(6);
        func_02021848(param->work->paletteFade, 2, 3, 1, 12, 0, 0x842, param->work->tcbMgr);
        param->seq++;
        break;
    case 1:
    default:
        if (param->work->subTaskCount == 0) {
            BmpWin_FlushChar(param->work->subWin);
            GFL_BGSysSetBGEnabled(4, 1);
            GFL_BGSysSetBGEnabled(5, 1);
            GFL_BGSysSetBGEnabled(7, 0);
            func_ov094_0220687c(param->work, tcb);
        }
        break;
    }
}

// Standby to moves: the ball opens, then the command to moves sequence runs
void func_ov094_022036c4(TCB *tcb, void *data) {
    BtlvInputScreenTask *param = data;

    switch (param->seq) {
    case 0:
        loadBGScrToVramByFileNoReserveNegAlign(param->work->arc, 0x5d, 4, 0, 0, FALSE, param->work->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(param->work->arc, 0x6f, 5, 0, 0, FALSE, param->work->heapId);
        func_ov094_02206af8(param->work);
        func_ov094_02204960(param->work, 5, 0, 0, 0, 0, 2, 1);
        func_ov094_022043c0(param->work, 0x3000, 0x1000, -0x400, 0x98);
        GFL_BGSysLoadScr(6);
        func_02021848(param->work->paletteFade, 2, 3, 1, 12, 0, 0x842, param->work->tcbMgr);
        param->seq++;
        break;
    case 1:
        if (param->work->subTaskCount != 0) {
            break;
        }
        if (param->work->rule == 2 && param->pos != 4) {
            loadBGScrToVramByFileNoReserveNegAlign(param->work->arc, 0x5e, 4, 0, 0,
                                                   FALSE, param->work->heapId);
        } else {
            loadBGScrToVramByFileNoReserveNegAlign(param->work->arc, 0x5d, 4, 0, 0,
                                                   FALSE, param->work->heapId);
        }
        BmpWin_FlushMap(param->work->msgWin);
        GFL_BGSysLoadScr(6);
        func_ov094_022044c0(param->work, 0x100, 0x1c0, 8, 8);
        func_ov094_022045a8(param->work, 0, 0);
        func_ov094_02204838(param->work, 0);
        func_ov094_02204960(param->work, -1, 0, 0, 1, 1, 2, 0);
        param->wait = 4;
        param->seq++;
        break;
    case 2:
        param->wait--;
        if (param->wait == 0) {
            func_ov094_02204698(param->work, 0, 0);
            param->seq++;
        }
        break;
    case 3:
    default:
        if (param->work->subTaskCount == 0) {
            BmpWin_FlushChar(param->work->msgWin);
            func_0204b22c(param->work->moveUnit, TRUE);
            func_ov094_02204960(param->work, 5, 0x100, 0xc0, 2, 2, 2, 2);
            func_ov094_0220687c(param->work, tcb);
        }
        break;
    }
}

// Command to moves: the cancel button rises, the field frames and the actors slide in, then the tiles grow
void func_ov094_02203880(TCB *tcb, void *data) {
    BtlvInputScreenTask *param = data;

    switch (param->seq) {
    case 0:
        loadBGScrToVramByFileNoReserveNegAlign(param->work->arc, param->mapFile, 4, 0, 0, FALSE, param->work->heapId);
        BmpWin_FlushMap(param->work->msgWin);
        GFL_BGSysLoadScr(6);
        func_ov094_022044c0(param->work, 0x100, 0x1c0, 8, 8);
        func_ov094_022045a8(param->work, 0, 0);
        func_ov094_02204838(param->work, 0);
        func_ov094_02206a58(param->work);
        func_ov094_02204960(param->work, -1, 0, 0, 1, 1, 2, 0);
        param->wait = 4;
        param->seq++;
        break;
    case 1:
        param->wait--;
        if (param->wait == 0) {
            func_ov094_02204698(param->work, 0, 0);
            param->seq++;
        }
        break;
    case 2:
    default:
        if (param->work->subTaskCount == 0) {
            BmpWin_FlushChar(param->work->msgWin);
            func_0204b22c(param->work->moveUnit, TRUE);
            func_ov094_02204960(param->work, 5, 0x100, 0xc0, 2, 2, 2, 2);
            func_ov094_0220687c(param->work, tcb);
        }
        break;
    }
}

// Moves to command: the tiles shrink, then the buttons rise while the field frames and the actors slide back
void func_ov094_02203978(TCB *tcb, void *data) {
    BtlvInputScreenTask *param = data;

    switch (param->seq) {
    case 0:
        loadBGScrToVramByFileNoReserveNegAlign(param->work->arc, param->mapFile, 4, 0, 0, FALSE, param->work->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(param->work->arc, 0x68, 5, 0, 0, FALSE, param->work->heapId);
        BmpWin_FlushMap(param->work->subWin);
        func_ov094_02206a14(param->work);
        GFL_BGSysLoadScr(6);
        func_ov094_02204960(param->work, 5, 0, 0xc0, 1, 1, 2, 0);
        param->seq++;
        break;
    case 1:
        func_ov094_02204698(param->work, 0, 1);
        param->wait = 4;
        param->seq++;
        break;
    case 2:
        param->wait--;
        if (param->wait == 0) {
            func_ov094_022044c0(param->work, 0, 0x1c0, 8, 8);
            func_ov094_022045a8(param->work, 0, 1);
            func_ov094_02204838(param->work, 1);
            param->seq++;
        }
        break;
    case 3:
    default:
        if (param->work->subTaskCount == 0) {
            func_ov094_02205b80(param->work, TRUE);
            BmpWin_FlushChar(param->work->subWin);
            func_ov094_0220687c(param->work, tcb);
        }
        break;
    }
}



void func_ov094_02203a78(TCB *tcb, void *data) {
    BtlvInputScreenTask *task = data;

    loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x5d, 4, 0, 0, FALSE, task->work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, task->mapFile, 4, 0x440, 0x380, FALSE,
                                           task->work->heapId);
    BmpWin_FlushMap(task->work->msgWin);
    GFL_BGSysLoadScr(6);
    if (task->work->rule == 2) {
        loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x6a, 5, 0, 0, FALSE, task->work->heapId);
    } else {
        loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x69, 5, 0, 0, FALSE, task->work->heapId);
    }
    BmpWin_FlushChar(task->work->msgWin);
    func_ov094_0220687c(task->work, tcb);
}


void func_ov094_02203b08(TCB *tcb, void *data) {
    BtlvInputScreenTask *task = data;

    switch (task->seq) {
    case 0:
        loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, task->mapFile, 4, 0, 0, FALSE, task->work->heapId);
        func_ov094_02204960(task->work, 5, 0, 0xc0, 2, 2, 2, 2);
        task->seq++;
        break;
    case 1:
        BmpWin_FlushMap(task->work->msgWin);
        GFL_BGSysLoadScr(6);
        loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x68, 5, 0, 0, FALSE, task->work->heapId);
        BmpWin_FlushChar(task->work->msgWin);
        func_0204b22c(task->work->moveUnit, TRUE);
        func_ov094_02204960(task->work, 5, 0x100, 0xc0, 2, 2, 2, 2);
        func_ov094_0220687c(task->work, tcb);
        break;
    }
}

// Command to standby: the ball closes over the buttons
// pokebw2's BtlvInput_CommandToStandbyTask
void func_ov094_02203bc0(TCB *tcb, void *data) {
    BtlvInputScreenTask *param = data;

    switch (param->seq) {
    case 0:
        func_ov094_022043c0(param->work, 0x1000, 0x3000, 0x400, 0xa8);
        param->seq++;
        break;
    case 1:
        GFL_BGSysSetBGEnabled(4, 0);
        GFL_BGSysSetBGEnabled(5, 0);
        GFL_BGSysSetBGEnabled(7, 1);
        param->seq++;
        break;
    case 2:
    default:
        if (param->work->subTaskCount == 0) {
            func_ov094_0220687c(param->work, tcb);
        }
        break;
    }
}


void func_ov094_02203c2c(TCB *tcb, void *data) {
    BtlvInputScreenTask *task = data;

    switch (task->seq) {
    case 0:
        loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x5d, 4, 0, 0, 0, task->work->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x6e, 5, 0, 0, 0, task->work->heapId);
        func_ov094_02204960(task->work, 5, 0, 0xc0, 1, 1, 2, 0);
        task->seq++;
        return;
    case 1:
        if (task->prevScreen == 2 || task->prevScreen == 5) {
            func_ov094_02204698(task->work, 0, 1);
        }
        task->seq++;
        task->wait = 4;
        return;
    case 2:
        task->wait--;
        if (task->wait == 0) {
            func_ov094_022045a8(task->work, 0, 1);
            task->seq++;
            return;
        }
        break;
    case 3:
        if (task->work->subTaskCount == 0) {
            func_ov094_022043c0(task->work, 0x1000, 0x3000, 0x400, 0xa8);
            task->seq++;
            return;
        }
        break;
    case 4:
        GFL_BGSysSetBGEnabled(4, 0);
        GFL_BGSysSetBGEnabled(5, 0);
        GFL_BGSysSetBGEnabled(7, 1);
        task->seq++;
        return;
    case 5:
    default:
        if (task->work->subTaskCount == 0) {
            func_ov094_0220687c(task->work, tcb);
        }
        break;
    }
}


// Yes or no: the ball opens on the question
void func_ov094_02203d40(TCB *tcb, void *data) {
    BtlvInputScreenTask *param = data;

    switch (param->seq) {
    case 0:
        func_ov094_02206af8(param->work);
        func_ov094_02204960(param->work, 5, 0, 0, 0, 0, 2, 1);
        func_ov094_022043c0(param->work, 0x3000, 0x1000, -0x400, 0x98);
        func_ov094_02206a58(param->work);
        func_0202173c(param->work->paletteFade, 2, 3, 1, 12, 0, 0x842, param->work->tcbMgr);
        param->seq++;
        break;
    case 1:
        if (param->work->subTaskCount == 0) {
            func_ov094_022045a8(param->work, 0, 0);
            GFL_BGSysSetBGEnabled(5, 1);
            GFL_BGSysSetBGEnabled(7, 0);
            param->seq++;
        }
        break;
    case 2:
    default:
        if (param->work->subTaskCount == 0) {
            BmpWin_FlushMap(param->work->msgWin);
            GFL_BGSysLoadScr(6);
            BmpWin_FlushChar(param->work->msgWin);
            func_ov094_02204960(param->work, 5, 0x100, 0xc0, 2, 2, 2, 2);
            func_ov094_0220687c(param->work, tcb);
        }
        break;
    }
}

void func_ov094_02203e30(TCB *tcb, void *data) {
    BtlvInputScreenTask *task = data;

    switch (task->seq) {
    case 0:
        loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x5d, 4, 0, 0, FALSE, task->work->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x6f, 5, 0, 0, FALSE, task->work->heapId);
        func_ov094_02206af8(task->work);
        func_ov094_02204960(task->work, 5, 0, 0, 0, 0, 2, 1);
        func_ov094_022043c0(task->work, 0x3000, 0x1000, -0x400, 0x98);
        GFL_BGSysLoadScr(6);
        func_02021848(task->work->paletteFade, 2, 3, 1, 12, 0, 0x842, task->work->tcbMgr);
        task->seq++;
        break;
    case 1:
        if (task->work->subTaskCount == 0) {
            loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x64, 4, 0, 0, FALSE, task->work->heapId);
            BmpWin_FlushMap(task->work->msgWin);
            GFL_BGSysLoadScr(6);
            func_ov094_022044c0(task->work, data_ov094_0220a4ec[task->work->pokeSlot].x,
                                data_ov094_0220a4ec[task->work->pokeSlot].y, 8, 8);
            func_ov094_022045a8(task->work, 0, 0);
            func_ov094_02204838(task->work, 0);
            func_ov094_02204960(task->work, -1, 0, 0, 1, 1, 2, 0);
            task->wait = 4;
            task->seq++;
        }
        break;
    case 2:
        task->wait--;
        if (task->wait == 0) {
            func_ov094_02204698(task->work, 0, 0);
            task->seq++;
        }
        break;
    case 3:
    default:
        if (task->work->subTaskCount == 0) {
            BmpWin_FlushChar(task->work->msgWin);
            func_0204b22c(task->work->moveUnit, TRUE);
            func_ov094_02204960(task->work, 5, 0x100, 0xc0, 2, 2, 2, 2);
            func_ov094_0220687c(task->work, tcb);
        }
        break;
    }
}

void func_ov094_02203fdc(TCB *tcb, void *data) {
    BtlvInputScreenTask *task = data;

    switch (task->seq) {
    case 0:
        loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x62, 4, 0, 0, FALSE, task->work->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x6d, 5, 0, 0, FALSE, task->work->heapId);
        func_ov094_02206af8(task->work);
        func_ov094_02204960(task->work, 5, 0, 0, TRUE, FALSE, 2, TRUE);
        func_ov094_022043c0(task->work, FX32_CONST(3), FX32_ONE, -FX32_CONST(0.25), 152);
        func_ov094_022044c0(task->work, 0, 0x1c0, 8, 8);
        func_0202173c(task->work->paletteFade, 2, 3, 1, 12, 0, 0x842, task->work->tcbMgr);
        task->seq++;
        break;
    case 1:
    default:
        if (task->work->subTaskCount == 0) {
            if (task->strbuf != NULL) {
                GFL_TextRendererDrawToBitmap(task->work->msgBitmap, 0x18, 0x38, task->strbuf, task->work->font);
                GFL_StrBufFree(task->strbuf);
            }
            BmpWin_FlushMap(task->work->msgWin);
            GFL_BGSysLoadScr(6);
            BmpWin_FlushChar(task->work->msgWin);
            func_ov094_02204960(task->work, 4, task->x, task->y, 1, 1, 2, 0);
            func_ov094_0220687c(task->work, tcb);
        }
        break;
    }
}

void func_ov094_022040fc(TCB *tcb, void *data) {
    BtlvInputScreenTask *task = data;

    switch (task->seq) {
    case 0:
        loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x63, 4, 0, 0, FALSE, task->work->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x6b, 5, 0, 0, FALSE, task->work->heapId);
        func_ov094_02206af8(task->work);
        func_ov094_02204960(task->work, 5, 0, 0, TRUE, FALSE, 2, TRUE);
        func_ov094_022043c0(task->work, FX32_CONST(3), FX32_ONE, -FX32_CONST(0.25), 152);
        func_ov094_022044c0(task->work, 0, 0x1c0, 8, 8);
        func_0202173c(task->work->paletteFade, 2, 3, 1, 12, 0, 0x842, task->work->tcbMgr);
        task->seq++;
        break;
    case 1:
    default:
        if (task->work->subTaskCount == 0) {
            GFL_BGSysSetBGEnabled(4, TRUE);
            GFL_BGSysSetBGEnabled(5, TRUE);
            GFL_BGSysSetBGEnabled(7, FALSE);
            func_ov094_0220687c(task->work, tcb);
        }
        break;
    }
}

void func_ov094_022041e0(TCB *tcb, void *data) {
    BtlvInputScreenTask *task = data;

    switch (task->seq) {
    case 0:
        loadBGScrToVramByFileNoReserveNegAlign(task->work->arc, 0x64, 4, 0, 0, FALSE, task->work->heapId);
        BmpWin_FlushMap(task->work->msgWin);
        func_ov094_02206a58(task->work);
        GFL_BGSysLoadScr(6);
        func_ov094_022044c0(task->work, data_ov094_0220a4ec[task->work->pokeSlot].x,
                            data_ov094_0220a4ec[task->work->pokeSlot].y, 8, 8);
        func_ov094_022045a8(task->work, 0, 0);
        func_ov094_02204838(task->work, 0);
        func_ov094_02204960(task->work, -1, 0, 0, 1, 1, 2, 0);
        task->wait = 4;
        task->seq++;
        break;
    case 1:
        task->wait--;
        if (task->wait == 0) {
            func_ov094_02204698(task->work, 0, 0);
            task->seq++;
        }
        break;
    case 2:
    default:
        if (task->work->subTaskCount == 0) {
            BmpWin_FlushChar(task->work->msgWin);
            func_0204b22c(task->work->moveUnit, TRUE);
            func_ov094_02204960(task->work, 5, 0x100, 0xc0, 2, 2, 2, 2);
            func_ov094_0220687c(task->work, tcb);
        }
        break;
    }
}





// The rotation switch's task: the move screen is redrawn for the new slot, its buttons rising from the slot's position
void func_ov094_022042e4(TCB *tcb, void *data) {
    BtlvInputScreenTask *task = data;

    switch (task->seq) {
    case 0:
        func_ov094_022056b4(task->work);
        task->seq++;
        break;
    case 1:
        func_ov094_022053dc(task->work);
        BmpWin_FlushMap(task->work->msgWin);
        GFL_BGSysLoadScr(6);
        func_ov094_022044c0(task->work, data_ov094_0220a4ec[task->work->pokeSlot].x,
                                  data_ov094_0220a4ec[task->work->pokeSlot].y, 8, 8);
        func_ov094_02204960(task->work, -1, 0, 0, TRUE, TRUE, 2, FALSE);
        task->seq++;
        break;
    case 2:
    default:
        if (task->work->subTaskCount == 0) {
            BmpWin_FlushChar(task->work->msgWin);
            func_0204b22c(task->work->moveUnit, TRUE);
            func_ov094_02204960(task->work, 5, 256, 192, 2, 2, 2, 2);
            func_ov094_0220687c(task->work, tcb);
        }
        break;
    }
}

// The end of a screen task
// pokebw2's BtlvInput_ScreenTaskEnd
void func_ov094_022043ac(TCB *tcb) {
    BtlvInputScreenTask *task = GFL_TCBGetData(tcb);
    task->work->busy = FALSE;
}

// Scales the ball picture of BG 7 from one scale to another, moving it by two pixels a frame
// pokebw2's BtlvInput_StartBallScale
void func_ov094_022043c0(BtlvInput *work, fx32 from, fx32 to, fx32 step, int y) {
    BtlvInputBallScale *param =
        GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputBallScale), FALSE, "btlv_input.c", 0xb97);
    param->work = work;
    param->scale = from;
    param->target = to;
    param->step = step;
    param->y = y;
    func_ov094_0220684c(work, GFL_VBlankTCBAdd(func_ov094_02204438, param, 0), func_ov094_022044a0);
    work->subTaskCount++;
}

// pokebw2's BtlvInput_BallScaleTask
void func_ov094_02204438(TCB *tcb, void *data) {
    BtlvInputBallScale *param = data;
    MtxFx22 mtx;

    param->scale += param->step;
    if (param->step > 0) {
        param->y -= 2;
    } else if (param->step < 0) {
        param->y += 2;
    }
    MTX_Scale22_(&mtx, param->scale, param->scale);
    GFL_BGSysSetBGTransformEx(7, BG_MOVE_SET_X, 128, &mtx, 256, 256);
    GFL_BGSysSetBGTransformEx(7, BG_MOVE_SET_Y, param->y, &mtx, 256, 256);
    if (param->scale == param->target) {
        func_ov094_0220687c(param->work, tcb);
    }
}

// pokebw2's BtlvInput_BallScaleEnd
void func_ov094_022044a0(TCB *tcb) {
    BtlvInputBallScale *param = GFL_TCBGetData(tcb);
    param->work->subTaskCount--;
}

// Scrolls BG 4 to (x, y), then by step for count frames
// pokebw2's BtlvInput_StartButtonRise
void func_ov094_022044c0(BtlvInput *work, int x, int y, int step, int count) {
    BtlvInputButtonRise *param =
        GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputButtonRise), FALSE, "btlv_input.c", 0xbd5);
    param->work = work;
    param->state = 0;
    param->x = x;
    param->y = y;
    param->step = step;
    param->count = count;
    func_ov094_0220684c(work, GFL_VBlankTCBAdd(func_ov094_0220453c, param, 0), func_ov094_02204588);
    work->subTaskCount++;
}

// pokebw2's BtlvInput_ButtonRiseTask
void func_ov094_0220453c(TCB *tcb, void *data) {
    BtlvInputButtonRise *param = data;

    if (param->state == 0) {
        GFL_BGSysMoveBG(4, BG_MOVE_SET_X, param->x);
        GFL_BGSysMoveBG(4, BG_MOVE_SET_Y, param->y);
        param->state++;
        return;
    }
    param->y += param->step;
    GFL_BGSysMoveBG(4, BG_MOVE_SET_Y, param->y);
    param->count--;
    if (param->count == 0) {
        func_ov094_0220687c(param->work, tcb);
    }
}

// pokebw2's BtlvInput_ButtonRiseEnd
void func_ov094_02204588(TCB *tcb) {
    BtlvInputButtonRise *param = GFL_TCBGetData(tcb);
    param->work->subTaskCount--;
}

// Scrolls BG 5 through the two positions of a table, one every two frames
// pokebw2's BtlvInput_StartFieldFrames
void func_ov094_022045a8(BtlvInput *work, int unused, int dir) {
    BtlvInputFieldFrames *param =
        GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputFieldFrames), FALSE, "btlv_input.c", 0xc10);
    param->work = work;
    param->step = 0;
    param->dir = dir;
    param->wait = 2;
    func_ov094_0220684c(work, GFL_VBlankTCBAdd(func_ov094_0220461c, param, 0), func_ov094_02204678);
    work->subTaskCount++;
}

// pokebw2's BtlvInput_FieldFramesTask
void func_ov094_0220461c(TCB *tcb, void *data) {
    BtlvInputFieldFrames *param = data;

    param->wait--;
    if (param->wait != 0) {
        return;
    }
    param->wait = 2;
    GFL_BGSysMoveBG(5, BG_MOVE_SET_X, data_ov094_0220a46c[param->dir][param->step].x);
    GFL_BGSysMoveBG(5, BG_MOVE_SET_Y, data_ov094_0220a46c[param->dir][param->step++].y);
    if (param->step >= 2) {
        func_ov094_0220687c(param->work, tcb);
    }
}

// pokebw2's BtlvInput_FieldFramesEnd
void func_ov094_02204678(TCB *tcb) {
    BtlvInputFieldFrames *param = GFL_TCBGetData(tcb);
    param->work->subTaskCount--;
}

// Plays the grow (which 0) or shrink (1) animation on an actor at each tile of the set
// pokebw2's BtlvInput_StartTileAnim
void func_ov094_02204698(BtlvInput *work, int set, int which) {
    BtlvInputTileAnim *param =
        GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputTileAnim), FALSE, "btlv_input.c", 0xc53);
    ClActorSetup setup = { 0, 0, 0, 0, 2 };
    int i;

    param->work = work;
    param->unit = func_0204b100(6, 0, work->heapId);
    for (i = 0; i < 6; i++) {
        if (data_ov094_0220a85c[set].pos[i].x == -1) {
            param->actors[i] = NULL;
            continue;
        }
        param->actors[i] =
            func_0204b294(param->unit, work->chars, work->palette, work->cellAnims, &setup, 1, work->heapId);
        func_0204b404(param->actors[i], &data_ov094_0220a85c[set].pos[i], 1);
        func_0204b570(param->actors[i], 2);
        func_0204b92c(param->actors[i], TRUE);
        func_0204b878(param->actors[i], data_ov094_0220a85c[set].sequences[which]);
    }
    func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_022047cc, param, 0), func_ov094_022047f8);
    work->subTaskCount++;
}

// pokebw2's BtlvInput_TileAnimTask
void func_ov094_022047cc(TCB *tcb, void *data) {
    BtlvInputTileAnim *param = data;
    int i;

    for (i = 0; i < 6; i++) {
        if (param->actors[i] != NULL && func_0204b98c(param->actors[i])) {
            return;
        }
    }
    func_ov094_0220687c(param->work, tcb);
}

// pokebw2's BtlvInput_TileAnimEnd
void func_ov094_022047f8(TCB *tcb) {
    BtlvInputTileAnim *param = GFL_TCBGetData(tcb);
    int i;

    for (i = 0; i < 6; i++) {
        if (param->actors[i] != NULL) {
            func_0204b3b4(param->actors[i]);
        }
    }
    func_0204b1cc(param->unit);
    param->work->subTaskCount--;
}

// Slides the party icons to the row's height for the direction, four pixels a frame
// pokebw2's BtlvInput_StartActorSlide
void func_ov094_02204838(BtlvInput *work, int dir) {
    BtlvInputActorSlide *param =
        GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputActorSlide), FALSE, "btlv_input.c", 0xcc9);
    param->work = work;
    param->dir = dir;
    func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_022048a4, param, 0), func_ov094_02204940);
    work->subTaskCount++;
}

// pokebw2's BtlvInput_ActorSlideTask
void func_ov094_022048a4(TCB *tcb, void *data) {
    BtlvInputActorSlide *param = data;
    int row;
    BOOL moving = FALSE;
    BtlvInputActor *actors;
    int i;
    ClActorPos pos;

    for (row = 0; row < 2; row++) {
        int target = data_ov094_0220a3b4[row][param->dir];

        if (row == 0) {
            actors = param->work->pokeActors[0];
        } else if (param->work->secondRow) {
            actors = param->work->pokeActors[1];
        } else {
            break;
        }
        for (i = 0; i < 6; i++) {
            func_0204b454(actors[i].actor, &pos, 1);
            if (pos.y > target) {
                pos.y -= 4;
                moving = TRUE;
            } else if (pos.y < target) {
                pos.y += 4;
                moving = TRUE;
            }
            func_0204b404(actors[i].actor, &pos, 1);
        }
    }
    if (!moving) {
        func_ov094_0220687c(param->work, tcb);
    }
}

// pokebw2's BtlvInput_ActorSlideEnd
void func_ov094_02204940(TCB *tcb) {
    BtlvInputActorSlide *param = GFL_TCBGetData(tcb);
    param->work->subTaskCount--;
}

// Next frame, scrolls a BG (none with -1) and switches BGs 4 to 7 on (TRUE), off (FALSE) or leaves them (2)
// pokebw2's BtlvInput_StartSetLayers
void func_ov094_02204960(BtlvInput *work, int bg, int x, int y, int bg4, int bg5, int bg6, int bg7) {
    BtlvInputSetLayers *param =
        GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputSetLayers), FALSE, "btlv_input.c", 0xd1d);
    param->work = work;
    param->bg = bg;
    param->x = x;
    param->y = y;
    param->bg4 = bg4;
    param->bg5 = bg5;
    param->bg6 = bg6;
    param->bg7 = bg7;
    func_ov094_0220684c(work, GFL_VBlankTCBAdd(func_ov094_022049c8, param, 0), NULL);
}

// pokebw2's BtlvInput_SetLayersTask
void func_ov094_022049c8(TCB *tcb, void *data) {
    BtlvInputSetLayers *param = data;

    if (param->bg != -1) {
        GFL_BGSysMoveBG(param->bg, BG_MOVE_SET_X, param->x);
        GFL_BGSysMoveBG(param->bg, BG_MOVE_SET_Y, param->y);
    }
    if (param->bg4 != 2) {
        GFL_BGSysSetBGEnabled(4, param->bg4);
    }
    if (param->bg5 != 2) {
        GFL_BGSysSetBGEnabled(5, param->bg5);
    }
    if (param->bg6 != 2) {
        GFL_BGSysSetBGEnabled(6, param->bg6);
    }
    if (param->bg7 != 2) {
        GFL_BGSysSetBGEnabled(7, param->bg7);
    }
    func_ov094_0220687c(param->work, tcb);
}

// Waits for the fades to end, freeing the graphics when the fade was one out of the screen
// pokebw2's BtlvInput_FadeWaitTask
static void func_ov094_02204a3c(TCB *tcb, void *data) {
    BtlvInputWorkTask *param = data;

    if (func_02021ed8(param->work->paletteFade) == 0 && !GFL_FadeIsRunning()) {
        if (param->work->fadeMode == 1) {
            func_ov094_022023d0(param->work);
        }
        func_ov094_0220687c(param->work, tcb);
    }
}

// pokebw2's BtlvInput_FadeWaitEnd
static void func_ov094_02204a70(TCB *tcb) {
    BtlvInputWorkTask *param = GFL_TCBGetData(tcb);
    param->work->fadeMode = 0;
}

// Slides the two actors left eight pixels a frame until they reach x 224
// pokebw2's BtlvInput_SlideActorsLeftTask
void func_ov094_02204a88(TCB *tcb, void *data) {
    BtlvInputWorkTask *param = data;
    ClActorPos pos;

    func_0204b454(param->work->actors210[0], &pos, 1);
    pos.x -= 8;
    func_0204b404(param->work->actors210[0], &pos, 1);
    func_0204b454(param->work->actors210[1], &pos, 1);
    pos.x -= 8;
    func_0204b404(param->work->actors210[1], &pos, 1);
    if (pos.x == 224) {
        func_ov094_0220687c(param->work, tcb);
    }
}

// pokebw2's BtlvInput_SlideActorsLeftEnd
void func_ov094_02204aec(TCB *tcb) {
    BtlvInputWorkTask *param = GFL_TCBGetData(tcb);
    param->work->subTaskCount--;
}

// Starts the rotation switch for rotate button 5 or 6, which rotates another mon to the front
// pokebw2's BtlvInput_StartRotationSwitch
void func_ov094_02204b0c(BtlvInput *work, int screen) {
    BtlvInputScreenTask *task =
        GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputScreenTask), TRUE, "btlv_input.c", 0xd8c);
    int slot = work->pokeSlot;
    int index = screen - 5;

    work->pokeSlot = data_ov094_0220a38c[slot][index];
    work->busy = TRUE;
    task->work = work;
    func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_022042e4, task, 1),
                      func_ov094_022043ac);
    func_ov094_021f8ac0(2);
    func_ov094_021f7998(data_ov094_0220a3e4[slot][index]);
}

// Waits for the touch screen's work and sets its state
// pokebw2's BtlvInput_SetScdStateTask
static void func_ov094_02204bac(TCB *tcb, void *data) {
    BtlvInputSimpleTask *param = data;

    if (func_ov094_021f8678()) {
        func_ov094_021f8884(param->value);
        func_ov094_0220687c(param->work, tcb);
    }
}

// Darkens the sub screen's OBJ outside the windows, window 1 only with three or more of them
// pokebw2's BtlvInput_WindowDarkenTask
void func_ov094_02204bcc(TCB *tcb, void *data) {
    BtlvInputSimpleTask *param = data;

    GXS_SetVisibleWnd(GX_WNDMASK_W0 | GX_WNDMASK_W1);
    G2S_SetWnd0InsidePlane(0x1f, param->value > 1);
    G2S_SetWnd1InsidePlane(0x1f, param->value == 3);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetBlendBrightness(GX_PLANEMASK_OBJ, -8);
    func_ov094_0220687c(param->work, tcb);
}

// Undoes it
// pokebw2's BtlvInput_WindowClearTask
static void func_ov094_02204c48(TCB *tcb, void *data) {
    BtlvInputSimpleTask *param = data;

    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    G2S_SetWnd0InsidePlane(0, FALSE);
    G2S_SetWnd1InsidePlane(0, FALSE);
    G2S_SetWndOutsidePlane(0, FALSE);
    G2S_SetBlendBrightness(0, 0);
    func_ov094_0220687c(param->work, tcb);
}

// A string's width in pixels and in 8-pixel blocks
// pokebw2's BtlvInput_GetStringWidth
void func_ov094_02204c98(const StrBuf *strbuf, Font *font, int *width, int *blocks) {
    int w = GFL_FontGetBlockWidth(strbuf, font, 0);
    int b = w / 8;

    if (FX_ModS32(w, 8) != 0) {
        b++;
    }
    *width = w;
    *blocks = b;
}

static inline u8 BtlvInput_ColorLetter(s32 color) {
    return (color >> 10) & 0x1f;
}

static inline u8 BtlvInput_ColorShadow(s32 color) {
    return (color >> 5) & 0x1f;
}

static inline u8 BtlvInput_ColorBackground(s32 color) {
    return color & 0x1f;
}

static inline HeapID BtlvInput_TempHeapId(const BtlvInput *work) {
    return HEAPID_TAIL(work->heapId);
}
void func_ov094_02204cc0(BtlvInput *work, const BtlvInputMoveParam *param) {
    const s32 labelPos[4][2] = { { 0x33, 0x1a }, { 0xb3, 0x1a }, { 0x33, 0x4a }, { 0xb3, 0x4a } };
    const s32 ppPos[4][2] = { { 0x5a, 0x1a }, { 0xda, 0x1a }, { 0x5a, 0x4a }, { 0xda, 0x4a } };
    NNSG2dImageProxy proxy;
    s32 width;
    s32 blocks;
    NNSG2dCharacterData *character;
    NNSG2dPaletteData *palette;
    u8 letter;
    u8 shadow;
    u8 background;
    WordSet *wordSet;
    u32 paletteFile;
    StrBuf *nameBuf;
    StrBuf *nameFmt;
    StrBuf *ppBuf;
    StrBuf *ppFmt;
    StrBuf *labelFmt;
    void *charBuf;
    void *palBuf;
    s32 type;
    s32 color;
    int i;

    nameBuf = GFL_StrBufCreate(16, work->heapId);
    nameFmt = GFL_MsgDataLoadStrbufNew(work->msgData, 0);
    labelFmt = GFL_MsgDataLoadStrbufNew(work->msgData, 1);
    wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
    ppBuf = GFL_StrBufCreate(16, work->heapId);
    ppFmt = GFL_MsgDataLoadStrbufNew(work->msgData, 2);
    func_0204b22c(work->moveUnit, FALSE);

    for (i = 0; i < 4; i++) {
        if (param->moves[i] != 0) {
            type = PML_MoveGetParam(param->moves[i], MOVE_PARAM_TYPE);
            paletteFile = data_ov094_0220a5f0[type];
            work->moveActors[i] = func_0204b294(work->moveUnit, work->moveChars[i], work->movePalette,
                                                work->moveCellAnims, &data_ov094_0220a392, 1, work->heapId);
            func_0204b404(work->moveActors[i], &data_ov094_0220a3c4[i], 1);
            charBuf = GFL_G2DIOReadOBJNCGR(getUINarcIdx(), func_020275f8(type), FALSE, &character,
                                           BtlvInput_TempHeapId(work));
            func_0204b6f4(work->moveActors[i], func_020275ec(type), 1);
            func_0204b7c8(work->moveActors[i], &proxy);
            MIi_CpuCopy16(character->pRawData,
                         (u8 *)G2S_GetOBJCharPtr() + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DSUB], 0x100);
            GFL_HeapFree(charBuf);

            GFL_TextRndGetGlobalColors(&letter, &shadow, &background);
            GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
            loadMoveNameToStrbuf(wordSet, 0, param->moves[i]);
            GFL_WordSetFormatStrbuf(wordSet, nameBuf, nameFmt);
            func_ov094_02204c98(nameBuf, work->font, &width, &blocks);
            GFL_TextRendererDrawToBitmap(work->msgBitmap, data_ov094_0220a4ac[i][0] - width / 2,
                                         data_ov094_0220a4ac[i][1], nameBuf, work->font);

            WordSetNumber(wordSet, 0, param->pp[i], 2, 1, TRUE);
            WordSetNumber(wordSet, 1, param->maxPP[i], 2, 1, TRUE);
            GFL_WordSetFormatStrbuf(wordSet, ppBuf, ppFmt);
            color = func_ov094_02205768(param->pp[i], param->maxPP[i]);
            GFL_TextRndUpdateColorIndexLUT(BtlvInput_ColorLetter(color), BtlvInput_ColorShadow(color),
                                           BtlvInput_ColorBackground(color));
            GFL_TextRendererDrawToBitmap(work->msgBitmap, labelPos[i][0], labelPos[i][1], labelFmt, work->font);
            func_ov094_02204c98(ppBuf, work->font, &width, &blocks);
            GFL_TextRendererDrawToBitmap(work->msgBitmap, ppPos[i][0] - width / 2, ppPos[i][1], ppBuf, work->font);
            GFL_TextRndUpdateColorIndexLUT(letter, shadow, background);

            work->buttonEnabled[i] = param->pp[i] != 0;
            work->moveExists[i] = param->moves[i] != 0;
        } else {
            paletteFile = 0xc1;
        }
        palBuf = GFL_G2DIOReadNCLR(11, paletteFile, &palette, BtlvInput_TempHeapId(work));
        func_020215bc(work->paletteFade, palette->pRawData, 1, i * 16 + 0x90, 0x20);
        GFL_HeapFree(palBuf);
    }
    work->buttonEnabled[i] = TRUE;
    if (work->rule == 2) {
        work->buttonEnabled[i + 1] = TRUE;
    }

    GFL_WordSetSystemFree(wordSet);
    GFL_StrBufFree(nameBuf);
    GFL_StrBufFree(labelFmt);
    GFL_StrBufFree(nameFmt);
    GFL_StrBufFree(ppFmt);
    GFL_StrBufFree(ppBuf);
}


// Draws the target panels' names, picks the screen's layout and loads the panel palettes
// pokebw2's BtlvInput_DrawTargetPanels
void func_ov094_022050f0(BtlvInput *work, BtlvInputScreenTask *task, const BtlvInputTargetParam *param) {
    s32 width;
    s32 blocks;
    NNSG2dPaletteData *paletteData;
    u8 letter;
    u8 shadow;
    u8 background;
    StrBuf *buf;
    StrBuf *fmt;
    WordSet *wordSet;
    void *palBuf;
    int count;
    BOOL triple;
    int attacker;
    int i;

    count = (work->rule == 2) ? 6 : 4;
    triple = (work->rule == 2) ? TRUE : FALSE;
    GFL_TextRndGetGlobalColors(&letter, &shadow, &background);
    buf = GFL_StrBufCreate(12, HEAPID_TAIL(work->heapId));
    wordSet = GFL_WordSetSystemCreateDefault(HEAPID_TAIL(work->heapId));
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
    for (i = 0; i < count; i++) {
        if (param->mons[i].hp != 0) {
            fmt = GFL_MsgDataLoadStrbufNew(work->msgData, param->mons[i].msgForm + 3);
            loadPokemonNicknameToStrbuf(wordSet, 0, param->mons[i].pkm);
            GFL_WordSetFormatStrbuf(wordSet, buf, fmt);
            func_ov094_02204c98(buf, work->font, &width, &blocks);
            GFL_TextRendererDrawToBitmap(work->msgBitmap, data_ov094_0220a7fc[triple][i][0] - width / 2,
                                         data_ov094_0220a7fc[triple][i][1], buf, work->font);
            GFL_StrBufFree(fmt);
        }
        work->buttonEnabled[i] = param->mons[i].selectable;
    }
    work->buttonEnabled[i] = TRUE;
    GFL_TextRndUpdateColorIndexLUT(letter, shadow, background);
    GFL_WordSetSystemFree(wordSet);
    GFL_StrBufFree(buf);

    attacker = (param->attackerPos - 2) / 2;
    work->pos = param->range;
    task->mapFile = data_ov094_0220a94c[triple][attacker][param->range];

    count = (work->rule == 2) ? 3 : 2;
    for (i = 0; i < count; i++) {
        palBuf = GFL_G2DIOReadNCLR(11, 0xd3, &paletteData, HEAPID_TAIL(work->heapId));
        func_020215bc(work->paletteFade, paletteData->pRawData, 1, i * 16 + 0x70, 0x20);
        GFL_HeapFree(palBuf);
    }
    for (; i < count * 2; i++) {
        palBuf = GFL_G2DIOReadNCLR(11, 0xd4, &paletteData, HEAPID_TAIL(work->heapId));
        func_020215bc(work->paletteFade, paletteData->pRawData, 1, i * 16 + 0x70, 0x20);
        GFL_HeapFree(palBuf);
    }
}

// Draws the two strings of the Yes / No screen and loads its BG screen
// pokebw2's BtlvInput_DrawYesNo
void func_ov094_02205340(BtlvInput *work, const BtlvInputYesNoParam *param) {
    s32 width;
    s32 blocks;

    GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
    func_ov094_02204c98(param->strs[0], work->font, &width, &blocks);
    GFL_TextRendererDrawToBitmap(work->msgBitmap, 128 - width / 2, 16, param->strs[0], work->font);
    func_ov094_02204c98(param->strs[1], work->font, &width, &blocks);
    GFL_TextRendererDrawToBitmap(work->msgBitmap, 128 - width / 2, 64, param->strs[1], work->font);
    work->buttonEnabled[0] = TRUE;
    work->buttonEnabled[1] = TRUE;
    loadBGScrToVramByFileNoReserveNegAlign(work->arc, 0x6b, 5, 0, 0, FALSE, work->heapId);
}

// Draws the move screen for the current mon and starts its fade
// pokebw2's BtlvInput_SetupMoveScreen
void func_ov094_022053dc(BtlvInput *work) {
    BtlvInputMoveParam param;
    int i;

    for (i = 0; i < 4; i++) {
        param.moves[i] = 0;
        param.pp[i] = 0;
        param.maxPP[i] = 0;
    }
    for (i = 0; i < GetBattleMonMoveCount(work->mons[work->pokeSlot]); i++) {
        param.moves[i] = func_ov093_021d5528(work->mons[work->pokeSlot], i, &param.pp[i], &param.maxPP[i]);
    }
    func_ov094_02204cc0(work, &param);
    work->buttonEnabled[5] = data_ov094_0220a386[work->pokeSlot][0];
    work->buttonEnabled[6] = data_ov094_0220a386[work->pokeSlot][1];
    work->noUsableMove = TRUE;
    for (i = 0; i < 4; i++) {
        if (work->moveUsable[work->pokeSlot][i] == TRUE) {
            work->noUsableMove = FALSE;
        }
    }
    if (IsFainted(work->mons[work->pokeSlot])) {
        for (i = 0; i < 4; i++) {
            work->buttonEnabled[i] = FALSE;
        }
        work->noUsableMove = FALSE;
        func_02021848(work->paletteFade, 2, 0x3e00, 0, 8, 8, 0, work->tcbMgr);
        func_02021848(work->paletteFade, 8, 0x700, 0, 8, 8, 0, work->tcbMgr);
    } else if (work->noUsableMove == TRUE) {
        work->buttonEnabled[0] = TRUE;
        func_ov094_02205f88(work);
        func_02021848(work->paletteFade, 2, 0x3e00, 0, 8, 8, 0, work->tcbMgr);
        func_02021848(work->paletteFade, 8, 0x700, 0, 8, 8, 0, work->tcbMgr);
    } else {
        func_02021848(work->paletteFade, 2, 0x3e00, 0, 0, 0, 0, work->tcbMgr);
        func_02021848(work->paletteFade, 8, 0x700, 0, 0, 0, 0, work->tcbMgr);
    }
}

// Draws the recorder screen's count line, unk4 / unk8, unk4 in red when it differs from unk0
// pokebw2's BtlvInput_DrawRecorderCount
void func_ov094_0220558c(BtlvInput *work, const BtlvInputRecorderParam *param) {
    u8 letter;
    u8 shadow;
    u8 background;
    StrBuf *buf;
    WordSet *wordSet;
    StrBuf *fmt;
    StrBuf *label;

    wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
    buf = GFL_StrBufCreate(8, work->heapId);
    fmt = GFL_MsgDataLoadStrbufNew(work->msgData, Bank0021_Text_006);
    GFL_TextRndGetGlobalColors(&letter, &shadow, &background);
    GFL_TextRndUpdateColorIndexLUT(9, 10, 0);
    WordSetNumber(wordSet, 0, param->unk8, 3, 2, TRUE);
    GFL_WordSetFormatStrbuf(wordSet, buf, fmt);
    GFL_TextRendererDrawToBitmap(work->msgBitmap, 140, 8, buf, work->font);
    if (param->unk0 != param->unk4) {
        GFL_TextRndUpdateColorIndexLUT(7, 8, 0);
    }
    WordSetNumber(wordSet, 0, param->unk4, 3, 2, TRUE);
    GFL_WordSetFormatStrbuf(wordSet, buf, fmt);
    GFL_TextRendererDrawToBitmap(work->msgBitmap, 96, 8, buf, work->font);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
    label = GFL_MsgDataLoadStrbufNew(work->msgData, Bank0021_Text_007);
    GFL_TextRendererDrawToBitmap(work->msgBitmap, 124, 8, label, work->font);
    GFL_StrBufFree(label);
    GFL_TextRndUpdateColorIndexLUT(letter, shadow, background);
    GFL_WordSetSystemFree(wordSet);
    GFL_StrBufFree(fmt);
    GFL_StrBufFree(buf);
    func_020215bc(work->paletteFade, func_02021728(func_ov094_021f86e0(), 1) + 0x20, 1, 0x70,
                         0x20);
}

// Clears the screen's text, buttons and actors and fades its palettes out
// pokebw2's BtlvInput_ClearScreen
void func_ov094_022056b4(BtlvInput *work) {
    int i;

    work->selectionLocked = FALSE;
    for (i = 0; i < 8; i++) {
        work->buttonEnabled[i] = FALSE;
    }
    for (i = 0; i < 4; i++) {
        work->moveExists[i] = FALSE;
    }
    GFL_BitmapFill(work->msgBitmap, 0);
    BmpWin_FlushChar(work->msgWin);
    GFL_BitmapFill(work->subBitmap, 0);
    BmpWin_FlushChar(work->subWin);
    func_ov094_02205f64(work);
    func_ov094_02206128(work);
    func_ov094_02205d34(work);
    GFL_BGSysFillScrArea(4, 0, 32, 2, 32, 32, 0);
    func_02021848(work->paletteFade, 2, 0x3e00, 0, 0, 0, 0, work->tcbMgr);
    func_02021848(work->paletteFade, 8, 0x700, 0, 0, 0, 0, work->tcbMgr);
}

// The text color of a move's PP count
// pokebw2's BtlvInput_GetPPColor
s32 func_ov094_02205768(s32 pp, s32 maxPP) {
    if (pp == 0) {
        return PRINT_COLOR(7, 8, 0);
    }
    if (maxPP == pp) {
        return PRINT_COLOR(1, 2, 0);
    }
    if (maxPP <= 2) {
        if (pp == 1) {
            return PRINT_COLOR(5, 6, 0);
        }
    } else if (maxPP <= 7) {
        switch (pp) {
        case 1:
            return PRINT_COLOR(5, 6, 0);
        case 2:
            return PRINT_COLOR(3, 4, 0);
        }
    } else {
        if (pp <= maxPP / 4) {
            return PRINT_COLOR(5, 6, 0);
        }
        if (pp <= maxPP / 2) {
            return PRINT_COLOR(3, 4, 0);
        }
    }
    return PRINT_COLOR(1, 2, 0);
}

void func_ov094_022057cc(BtlvInput *work, const BtlvInputCommandParam *param) {
    ArcTool *arc;
    BtlvInputSimpleTask *task;
    int count;
    int i;
    int placed;

    count = (work->rule == 3) ? 2 : work->rule + 1;
    placed = 0;
    if (work->rule == 0 || work->rule == 3) {
        return;
    }
    arc = GFL_ArcSysCreateFileHandle(7, HEAPID_TAIL(work->heapId));
    work->iconCellAnims = func_0204af28(arc, func_0201bc78(), getOBJTileMapping_SubEng(), work->heapId);
    work->iconPalette = func_0204acec(arc, func_0201bc3c(), 1, 0x160, work->heapId);
    func_020216d4(work->paletteFade, 3, func_0204aef4(work->iconPalette, TRUE) >> 1, 0x60);
    G2S_SetWnd0Position(1, 0, 0, 0);
    G2S_SetWnd1Position(1, 0, 0, 0);
    if (count > 1) {
        task = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputSimpleTask), FALSE, "btlv_input.c", 0x1145);
        task->work = work;
        task->value = count;
        func_ov094_0220684c(work, GFL_VBlankTCBAdd(func_ov094_02204bcc, task, 0), NULL);
    }
    for (i = 0; i < count; i++) {
        if (param->species[i] == 0) {
            continue;
        }
        work->iconChars[i] = func_0204a6c8(arc, PokeParty_GetIconIndex(param->species[i], param->forms[i], param->sexes[i], FALSE),
                                           FALSE, 1, work->heapId);
        work->iconActors[i].actor = func_0204b294(work->iconUnit, work->iconChars[i], work->iconPalette,
                                                  work->iconCellAnims, &data_ov094_0220a392, 1, work->heapId);
        func_0204b404(work->iconActors[i].actor, &data_ov094_0220a50c[work->rule][i], 1);
        func_0204b92c(work->iconActors[i].actor, TRUE);
        if (param->pokePos == i) {
            func_0204b878(work->iconActors[i].actor, 1);
        } else if (placed == 0) {
            G2S_SetWnd0Position(data_ov094_0220a50c[work->rule][i].x - 16, data_ov094_0220a50c[work->rule][i].y - 16,
                                 data_ov094_0220a50c[work->rule][i].x + 16, data_ov094_0220a50c[work->rule][i].y + 16);
            placed++;
        } else {
            G2S_SetWnd1Position(data_ov094_0220a50c[work->rule][i].x - 16, data_ov094_0220a50c[work->rule][i].y - 16,
                                 data_ov094_0220a50c[work->rule][i].x + 16, data_ov094_0220a50c[work->rule][i].y + 16);
        }
        func_0204b6f4(work->iconActors[i].actor, func_0201bb5c(param->species[i], param->forms[i], param->sexes[i], FALSE), 0);
    }
    GFL_ArcToolFree(arc);
}


// Frees the party icons and their resources; a task clears their windows when any was freed
// pokebw2's BtlvInput_DeletePartyIcons
static void func_ov094_02205ab8(BtlvInput *work) {
    BtlvInputSimpleTask *data;
    BOOL none;
    int i;

    none = TRUE;
    for (i = 0; i < 3; i++) {
        if (work->iconActors[i].actor != NULL) {
            func_0204b3b4(work->iconActors[i].actor);
            func_0204a8d4(work->iconChars[i]);
            work->iconActors[i].actor = NULL;
            work->iconChars[i] = -1;
            none = FALSE;
        }
    }
    if (work->iconCellAnims != -1) {
        func_0204afd8(work->iconCellAnims);
        work->iconCellAnims = -1;
    }
    if (work->iconPalette != -1) {
        func_0204ada4(work->iconPalette);
        work->iconPalette = -1;
    }
    if (!none) {
        data = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputSimpleTask), FALSE, "btlv_input.c", 0x11b2);
        data->work = work;
        func_ov094_0220684c(work, GFL_VBlankTCBAdd(func_ov094_02204c48, data, 0), NULL);
    }
}

// Shows or hides the party icons
// pokebw2's BtlvInput_SetPartyIconsActive
void func_ov094_02205b80(BtlvInput *work, BOOL active) {
    func_0204b22c(work->iconUnit, active);
}

// Creates the two actors of the ball count's slide for the ball index and starts its task
// pokebw2's BtlvInput_CreateBallSlideActors
void func_ov094_02205b90(BtlvInput *work, int index) {
    const u32 palettes[5] = { 0, 1, 3, 2, 1 };
    const u32 xs[5] = { 5, 5, 1, 1, 1 };
    const u32 ys[5] = { -12, -12, -8, -8, -12 };
    ClActorPos pos;
    BtlvInputWorkTask *data;

    if (index == 0) {
        return;
    }
    work->actors210[0] = func_0204b294(work->unit20c, work->chars, work->palette, work->cellAnims, &data_ov094_0220a392,
                                       1, work->heapId);
    pos.x = xs[index - 1] + 304;
    pos.y = ys[index - 1] + 32;
    func_0204b404(work->actors210[0], &pos, 1);
    func_0204b92c(work->actors210[0], TRUE);
    func_0204b878(work->actors210[0], index + 14);
    work->actors210[1] = func_0204b294(work->unit20c, work->chars, work->palette, work->cellAnims, &data_ov094_0220a392,
                                       1, work->heapId);
    pos.x = 304;
    pos.y = 32;
    func_0204b404(work->actors210[1], &pos, 1);
    func_0204b92c(work->actors210[1], TRUE);
    func_0204b878(work->actors210[1], 14);
    func_0204b6f4(work->actors210[1], (u8)palettes[index - 1], 0);
    data = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputWorkTask), FALSE, "btlv_input.c", 0x11ec);
    data->work = work;
    func_ov094_0220684c(work, GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02204a88, data, 0),
                      func_ov094_02204aec);
    work->subTaskCount++;
}

// Frees the slide's actors
// pokebw2's BtlvInput_DeleteBallSlideActors
static void func_ov094_02205d34(BtlvInput *work) {
    int i;

    for (i = 0; i < 2; i++) {
        if (work->actors210[i] != NULL) {
            func_0204b3b4(work->actors210[i]);
            work->actors210[i] = NULL;
        }
    }
}

// Creates the six party icons of one side of the party screen, in a row
// pokebw2's BtlvInput_CreatePokeListIcons
void func_ov094_02205d58(BtlvInput *work, int side) {
    BtlvInputActor *actors;
    ClActorPos pos;
    int step;
    int base;
    int i;

    if (side == 1) {
        actors = work->pokeActors[1];
        pos.x = 148;
        pos.y = 48;
        step = -8;
        base = 4;
    } else {
        actors = work->pokeActors[0];
        pos.x = 88;
        pos.y = 136;
        step = 16;
        base = 0;
    }
    for (i = 0; i < 6; i++) {
        actors[i].actor = func_0204b294(work->pokeUnit, work->chars, work->palette, work->cellAnims,
                                        &data_ov094_0220a392, 1, work->heapId);
        func_0204b404(actors[i].actor, &pos, 1);
        func_0204b92c(actors[i].actor, TRUE);
        func_0204b878(actors[i].actor, base + work->pokeEntries[side][i].iconAnim);
        pos.x += step;
    }
}

// Frees both sides' party icons
// pokebw2's BtlvInput_DeletePokeListIcons
static void func_ov094_02205e30(BtlvInput *work) {
    BtlvInputActor *actors;
    int side;
    int i;

    for (side = 0; side < 2; side++) {
        actors = (side != 0) ? work->pokeActors[1] : work->pokeActors[0];
        for (i = 0; i < 6; i++) {
            if (actors[i].actor != NULL) {
                func_0204b3b4(actors[i].actor);
                actors[i].actor = NULL;
            }
        }
    }
}

// Creates the six hidden cursor actors
// pokebw2's BtlvInput_CreateCursorActors
void func_ov094_02205e80(BtlvInput *work) {
    const u32 sequences[6] = { 0, 2, 1, 3, 1, 3 };
    int i;

    for (i = 0; i < 6; i++) {
        work->cursorActors[i].actor = func_0204b294(work->cursorUnit, work->chars2, work->palette, work->cellAnims2,
                                                    &data_ov094_0220a392, 1, work->heapId);
        func_0204b92c(work->cursorActors[i].actor, TRUE);
        func_0204b878(work->cursorActors[i].actor, sequences[i]);
        func_0204b3dc(work->cursorActors[i].actor, FALSE);
    }
}

// Frees the cursor actors
// pokebw2's BtlvInput_DeleteCursorActors
static void func_ov094_02205f40(BtlvInput *work) {
    int i;

    for (i = 0; i < 6; i++) {
        if (work->cursorActors[i].actor != NULL) {
            func_0204b3b4(work->cursorActors[i].actor);
            work->cursorActors[i].actor = NULL;
        }
    }
}

// Frees the move tiles' type icons
// pokebw2's BtlvInput_DeleteMoveTypeIcons
static void func_ov094_02205f64(BtlvInput *work) {
    int i;

    for (i = 0; i < 4; i++) {
        if (work->moveActors[i] != NULL) {
            func_0204b3b4(work->moveActors[i]);
            work->moveActors[i] = NULL;
        }
    }
}

static void func_ov094_02205f88(BtlvInput *work) {
    StrBuf *buf;
    StrBuf *fmt;
    WordSet *wordSet;
    GFLBitmap *bitmap;
    NNSG2dImageProxy proxy;
    ClActorPos pos;
    s32 width;
    s32 blocks;
    u8 letter;
    u8 shadow;
    u8 background;
    u8 *pixels;
    void *vram = G2S_GetOBJCharPtr();
    int i;

    work->actor21c = func_0204b294(work->unit218, work->chars, work->palette, work->cellAnims, &data_ov094_0220a392,
                                   1, work->heapId);
    pos.x = 128;
    pos.y = 80;
    func_0204b404(work->actor21c, &pos, 1);
    func_0204b92c(work->actor21c, TRUE);
    func_0204b878(work->actor21c, 0x14);
    buf = GFL_StrBufCreate(16, work->heapId);
    fmt = GFL_MsgDataLoadStrbufNew(work->msgData, 0);
    wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
    bitmap = GFL_BitmapCreate(14, 4, 0x20, work->heapId);
    GFL_BitmapFill(bitmap, 0);
    GFL_TextRndGetGlobalColors(&letter, &shadow, &background);
    GFL_TextRndUpdateColorIndexLUT(14, 10, 0);
    loadMoveNameToStrbuf(wordSet, 0, 0xa5);
    GFL_WordSetFormatStrbuf(wordSet, buf, fmt);
    func_ov094_02204c98(buf, work->font, &width, &blocks);
    GFL_TextRendererDrawToBitmap(bitmap, 56 - width / 2, 8, buf, work->font);
    GFL_TextRndUpdateColorIndexLUT(letter, shadow, background);
    pixels = GFL_BitmapGetPixelData(bitmap);
    func_0204b7c8(work->actor21c, &proxy);
    for (i = 0; i < 4; i++) {
        MI_CpuCopy16(&pixels[i * 0x1c0],
                     (void *)((uintptr_t)vram + (i * 8 + 0xcd) * 0x20 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DSUB]),
                     0x100);
        MI_CpuCopy16(&pixels[i * 0x1c0 + 0x100],
                     (void *)((uintptr_t)vram + (i * 4 + 0xed) * 0x20 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DSUB]),
                     0x80);
        MI_CpuCopy16(&pixels[i * 0x1c0 + 0x180],
                     (void *)((uintptr_t)vram + (i * 2 + 0xfd) * 0x20 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DSUB]),
                     0x40);
    }
    GFL_StrBufFree(buf);
    GFL_StrBufFree(fmt);
    GFL_WordSetSystemFree(wordSet);
    GFL_BitmapFree(bitmap);
}


// pokebw2's BtlvInput_DeleteStruggleActor
void func_ov094_02206128(BtlvInput *work) {
    if (work->actor21c != NULL) {
        func_0204b3b4(work->actor21c);
        work->actor21c = NULL;
    }
}

// Reads the keys for a screen: moves the cursor along its stops and returns the stop's result when A or B is
// pressed, or result as it came in. lastStops lists the stops the cursor may come back to. noMoveInfo says that a
// move's information can't be shown with L held
s32 func_ov094_02206140(BtlvInput *work, const BtlvInputButtonSet *set, const BtlvInputKeyStop *stops,
                              const u8 *lastStops, s32 result, BOOL noMoveInfo) {
    int pressed = GCTX_HIDGetPressedKeys();
    u32 held = GCTX_HIDGetHeldKeys();
    BOOL decided = FALSE;
    s8 next;

    if (!work->canCancel) {
        pressed &= ~PAD_BUTTON_B;
    }
    if (work->cursorRefresh) {
        work->cursorRefresh = 0;
        func_ov094_02206950(work);
        func_ov094_022064cc(work, set->rects, stops);
        func_0204b22c(work->cursorUnit, TRUE);
    }
    if (result != -1) {
        *work->keyCursorFlag = 0;
        work->cursor = 0;
        work->prevCursor = 0xf;
        func_ov094_022064cc(work, set->rects, stops);
        return result;
    }

    if (pressed != 0) {
        if (*work->keyCursorFlag != 0) {
            const BtlvInputKeyStop *stop;

            next = KEY_CURSOR_NONE;
            stop = &stops[work->cursor];

            switch (pressed) {
            case PAD_KEY_UP:
                next = stop->up;
                break;
            case PAD_KEY_DOWN:
                next = stop->down;
                break;
            case PAD_KEY_LEFT:
                next = stop->left;
                break;
            case PAD_KEY_RIGHT:
                next = stop->right;
                break;
            case PAD_BUTTON_B:
                if (stop->b < 0) {
                    if (work->laterPoke == 1) {
                        next = -stop->b;
                    }
                } else {
                    next = stop->b;
                }
                break;
            }

            if (pressed == PAD_BUTTON_A) {
                result = stop->a;
                if (!(result < 4 && (held & PAD_BUTTON_L) && work->screen == 5 &&
                      (noMoveInfo == 1 || work->noUsableMove == 1)) &&
                    !(result < 4 && (held & PAD_BUTTON_L) && work->screen == 2 && noMoveInfo == 1)) {
                    if (work->buttonEnabled[result] == 1) {
                        if (!func_ov094_02206908(work, set, result)) {
                            func_ov094_02206b30(work);
                        } else {
                            func_ov094_02206b58(work);
                        }
                        decided = TRUE;
                    } else if (result < 4 && (held & PAD_BUTTON_L) && work->screen == 2 &&
                               work->moveExists[result] == 1) {
                        func_ov094_02206b30(work);
                    }
                }
            } else if (next != KEY_CURSOR_NONE) {
                func_ov094_02206b14(work);
                if (next < 0) {
                    if (-next != work->prevCursor && work->prevCursor != 0xf) {
                        next = work->prevCursor;
                    } else {
                        next *= -1;
                    }
                } else if (next & KEY_CURSOR_LAST) {
                    int i = 0;

                    next &= ~KEY_CURSOR_LAST;
                    for (; lastStops[i] != 0xff; i++) {
                        if (work->prevCursor == lastStops[i]) {
                            next = work->prevCursor;
                            break;
                        }
                    }
                }
                work->prevCursor = work->cursor;
                work->cursor = next;
                if (pressed == PAD_BUTTON_B) {
                    if (next != 0xf) {
                        func_ov094_02206b58(work);
                    }
                    result = next;
                    decided = TRUE;
                }
            }
        } else if (pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y | PAD_PLUS_KEY_MASK)) {
            *work->keyCursorFlag = 1;
            func_ov094_02206950(work);
            func_ov094_02206b14(work);
        }
        func_ov094_022064cc(work, set->rects, stops);
    }

    if (decided == TRUE) {
        switch (work->screen) {
        case 2:
            if (!func_ov094_02206908(work, set, work->cursor)) {
                work->lastMove[work->pokeIndex] = work->moves[work->cursor];
            }
            // fallthrough
        case 1:
        case 5:
            if (!func_ov094_02206908(work, set, work->cursor)) {
                work->lastCursor[work->pokeIndex][work->screen] = work->cursor;
            }
            work->selectionLocked = 1;
            break;
        case 3:
            if (!func_ov094_02206908(work, set, result)) {
                work->lastCursor[work->pokeIndex][work->screen] =
                    work->cursor | (work->lastCursor[work->pokeIndex][2] << 4);
            }
            work->selectionLocked = 1;
            break;
        }
        work->cursor = 0;
        work->prevCursor = 0xf;
        work->cursorRefresh = 1;
        func_0204b22c(work->cursorUnit, FALSE);
    }
    return result;
}

// Keys of the yes / no screen: 0 for left, 1 for right, 2 for B
// pokebw2's BtlvInput_YesNoKeyInput
s32 func_ov094_0220648c(BtlvInput *work) {
    u32 keys = GCTX_HIDGetTypedKeys();

    if (keys & PAD_BUTTON_B) {
        func_ov094_02206b58(work);
        return 2;
    }
    if (keys & PAD_KEY_LEFT) {
        func_ov094_02206b30(work);
        return 0;
    }
    if (keys & PAD_KEY_RIGHT) {
        func_ov094_02206b30(work);
        return 1;
    }
    return -1;
}

// Puts the cursor actors on the corners of the current stop's rectangles
// pokebw2's BtlvInput_UpdateKeyCursorActors
void func_ov094_022064cc(BtlvInput *work, const TouchRect *rects, const BtlvInputKeyStop *stops) {
    ClActorPos pos;
    int i;

    for (i = 0; i < 6; i++) {
        s8 corner = stops[work->cursor].corners[i];

        if (corner >= 0) {
            const TouchRect *rect = &rects[corner];

            switch (i) {
            case 0:
                pos.x = rect->left;
                pos.y = rect->top;
                break;
            case 1:
                pos.x = rect->right;
                pos.y = rect->top;
                break;
            case 2:
                pos.x = rect->left;
                pos.y = rect->bottom;
                break;
            case 3:
                pos.x = rect->right;
                pos.y = rect->bottom;
                break;
            case 4:
                pos.x = rect->left;
                pos.y = rect->bottom;
                break;
            case 5:
                pos.x = rect->right;
                pos.y = rect->bottom;
                break;
            }
            func_0204b3dc(work->cursorActors[i].actor, *work->keyCursorFlag);
            func_0204b404(work->cursorActors[i].actor, &pos, 1);
        } else {
            func_0204b3dc(work->cursorActors[i].actor, FALSE);
        }
    }
}

// Starts the flash of a pressed button and locks the input until it ends. Returns -1, or index when there are no
// rows to flash
// pokebw2's BtlvInput_StartPressFlash
s32 func_ov094_02206578(BtlvInput *work, s32 index, s32 rows) {
    BtlvInputFlashWork *flash;
    TCB *task;
    BOOL lock;

    if (rows < 0) {
        return index;
    }
    if ((GCTX_HIDGetHeldKeys() & PAD_BUTTON_L) && index < 4 && (work->screen == 2 || work->screen == 5)) {
        work->pressedButton = index | 0x8000;
    } else {
        work->pressedButton = index;
    }

    flash = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvInputFlashWork), FALSE, "btlv_input.c", 0x1440);
    lock = FALSE;
    if (!(rows & 0x10000)) {
        lock = TRUE;
    }
    work->inputLocked = lock;
    flash->work = work;
    flash->seq = 0;
    flash->rows = rows & 0xffff;
    if (work->noUsableMove == 1 && flash->rows == 0) {
        flash->objOnly = TRUE;
    } else {
        flash->objOnly = FALSE;
    }
    task = GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_0220663c, flash, 0);
    func_ov094_0220684c(work, task, func_ov094_02206710);
    return -1;
}

// pokebw2's BtlvInput_PressFlashTask
void func_ov094_0220663c(TCB *tcb, void *data) {
    BtlvInputFlashWork *flash = data;

    switch (flash->seq) {
    case 0:
        if (flash->objOnly == 1) {
            func_0202173c(flash->work->paletteFade, 8, 2, 0, 0, 8, 0x7fff, flash->work->tcbMgr);
        } else {
            func_0202173c(flash->work->paletteFade, 2, flash->rows, 0, 0, 8, 0x7fff, flash->work->tcbMgr);
        }
        flash->seq++;
        break;
    case 1:
        if (func_02021ed8(flash->work->paletteFade) == 0) {
            if (flash->objOnly == 1) {
                func_0202173c(flash->work->paletteFade, 8, 2, 0, 8, 0, 0x7fff, flash->work->tcbMgr);
            } else {
                func_0202173c(flash->work->paletteFade, 2, flash->rows, 0, 8, 0, 0x7fff, flash->work->tcbMgr);
            }
            flash->seq++;
        }
        break;
    case 2:
        if (func_02021ed8(flash->work->paletteFade) == 0) {
            func_ov094_0220687c(flash->work, tcb);
        }
        break;
    }
}

// pokebw2's BtlvInput_PressFlashEnd
void func_ov094_02206710(TCB *tcb) {
    BtlvInputFlashWork *flash = GFL_TCBGetData(tcb);

    flash->work->inputLocked = 0;
}

// Prints the Wonder Launcher's points, from the command's parameter, into the small window
// pokebw2's BtlvInput_PrintLauncherPoints
void func_ov094_02206728(BtlvInput *work, BtlvInputCommandParam *param) {
    u8 letter, shadow, background;
    s32 width, tiles;
    WordSet *wordSet;
    StrBuf *strbuf;
    StrBuf *src;

    if (!func_ov094_021f88f4()) {
        return;
    }
    wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
    strbuf = GFL_StrBufCreate(6, work->heapId);
    src = GFL_MsgDataLoadStrbufNew(work->msgData, Bank0021_Text_008);
    GFL_TextRndGetGlobalColors(&letter, &shadow, &background);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
    WordSetNumber(wordSet, 0, param->launcherPoints, 2, 0, TRUE);
    GFL_WordSetFormatStrbuf(wordSet, strbuf, src);
    func_ov094_02204c98(strbuf, work->font, &width, &tiles);
    GFL_TextRendererDrawToBitmap(work->subBitmap, 12 - width / 2, 0, strbuf, work->font);
    GFL_TextRndUpdateColorIndexLUT(letter, shadow, background);
    GFL_WordSetSystemFree(wordSet);
    GFL_StrBufFree(src);
    GFL_StrBufFree(strbuf);
}

// pokebw2's BtlvInput_StartFingerCursor
void func_ov094_022067e8(BtlvInput *work) {
    func_ov094_021f8ac0(2);
    func_ov094_021f8554(work->pokePos, 1, 10, 0, 8);
}

// Forgets the Pokémon's cursor stops when it changed
// pokebw2's BtlvInput_ForgetCursorOnPokeChange
void func_ov094_0220680c(BtlvInput *work) {
    BOOL changed = func_ov094_02207d78(func_ov094_021f86b4(), work->pokePos);

    if ((changed | func_ov094_022005c4(func_ov094_021f8694(), work->pokePos)) == TRUE) {
        int i;

        for (i = 0; i < 8; i++) {
            work->lastCursor[work->pokeIndex][i] = 0;
        }
    }
}

// pokebw2's BtlvInput_AddTask
static void func_ov094_0220684c(BtlvInput *work, TCB *task, void (*endFunc)(TCB *tcb)) {
    int slot = func_ov094_022068c4(work);

    work->tasks[slot] = task;
    work->taskEndFuncs[slot] = endFunc;
}

// pokebw2's BtlvInput_FindTask
static int func_ov094_02206864(BtlvInput *work, TCB *task) {
    int i;

    for (i = 0; i < 8; i++) {
        if (work->tasks[i] == task) {
            break;
        }
    }
    return i;
}

// pokebw2's BtlvInput_EndTask
static void func_ov094_0220687c(BtlvInput *work, TCB *task) {
    int slot = func_ov094_02206864(work, task);
    void *data;

    if (task == NULL) {
        return;
    }
    data = GFL_TCBGetData(task);
    if (work->taskEndFuncs[slot] != NULL) {
        work->taskEndFuncs[slot](task);
    }
    if (data != NULL) {
        GFL_HeapFree(data);
    }
    GFL_TCBRemove(task);
    work->tasks[slot] = NULL;
    work->taskEndFuncs[slot] = NULL;
}

// A free slot, ending the first task when there is none
// pokebw2's BtlvInput_FreeTaskSlot
static int func_ov094_022068c4(BtlvInput *work) {
    int i;

    for (i = 0; i < 8; i++) {
        if (work->tasks[i] == NULL) {
            break;
        }
    }
    if (i == 8) {
        func_ov094_0220687c(work, work->tasks[0]);
        i = 0;
    }
    return i;
}

// pokebw2's BtlvInput_EndAllTasks
static void func_ov094_022068e8(BtlvInput *work) {
    int i;

    for (i = 0; i < 8; i++) {
        if (work->tasks[i] != NULL) {
            func_ov094_0220687c(work, work->tasks[i]);
        }
    }
}

s32 func_ov094_02206908(BtlvInput *work, const BtlvInputButtonSet *set, u32 index) {
    s32 cancel;

    if (!work->laterPoke || work->screen != 1) {
        return set->flags[index] & 1;
    }
    cancel = (set->flags[index] & 2) >> 1;
    if (cancel == 1) {
        work->lastCursor[work->pokeIndex][work->screen] = 0;
    }
    return cancel;
}



// Puts the cursor on the stop remembered for the screen
void func_ov094_02206950(BtlvInput *work) {
    switch (work->screen) {
    case 2:
        if (work->lastMove[work->pokeIndex] != work->moves[work->lastCursor[work->pokeIndex][work->screen]]) {
            work->lastCursor[work->pokeIndex][3] = 0;
        }
        // fallthrough
    case 1:
    case 5:
        work->cursor = work->lastCursor[work->pokeIndex][work->screen];
        break;
    case 3:
        if ((work->lastCursor[work->pokeIndex][work->screen] & 0xf0) >> 4 == work->lastCursor[work->pokeIndex][2]) {
            work->cursor = work->lastCursor[work->pokeIndex][work->screen];
        } else {
            work->cursor = 0;
        }
        break;
    default:
        work->cursor = 0;
        break;
    }
}



void func_ov094_02206a14(BtlvInput *work) {
    u16 palette[16];

    if (BtlSetup_GetBattleType(func_ov094_021f8754()) != 4 && func_ov094_021f8914() == 0) {
        func_02021f64((u16 *)&work->savedPalette[0] , palette, 16, 8, 0);
        func_020215bc(func_ov094_021f86e0(), palette, 1, 0x20, 0x20);
    }
}

void func_ov094_02206a58(BtlvInput *work) {
    if (work->inBattle == 1) {
        func_020215bc(func_ov094_021f86e0(), work->savedPalette, 1, 0x20, 0x20);
    }
}
