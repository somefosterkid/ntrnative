// Black's namein.c (pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov280/namein.c): the name entry
// screen, ov194 0x021f1440-0x021f4ae4. The message, talk and icon functions at 0x021f3cd0-0x021f4450 were first ported
// from pokebw2's src/ov257/ctvt_game_cam.c, whose code they share; their types are namein's own (pokebw2's
// NameEntryMessage, NameEntryTalk, NameEntryTask and NameEntryIcon).
#include "types.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "pml/poke_party.h"
#include "system/app_menu_common.h"
#include "system/nsbtx_to_clwk.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/fs.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "system/bmp_oam.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "app/namein_data.h"
#include "app/namein_graphic.h"
#include "gfl/proc.h"
#include "struct_decls.h"
#include "system/prof_word.h"
#include "app/name_entry.h"
#include "nitro/mi.h"
#include "save/save_control_intr.h"
#include "constants/pokemon.h"
#include "nnsys/gfd.h"
#include "system/str_tool.h"
#include "gfl/fade.h"
#include "gfl/ui.h"
#include "pml/species_names.h"
#include "gfl/key.h"
#include "constants/sound.h"
// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they
// belong in the library's header
enum {
    NAMEIN_WORD_TABLE_DAKUTEN,
    NAMEIN_WORD_TABLE_HANDAKUTEN,
    NAMEIN_WORD_TABLE_COMPOSE,
    NAMEIN_WORD_TABLE_MAX,
};
#define NAME_SLOT_COUNT 16
typedef struct NameBox {
    u16 text[11];
    u16 pending[4];
    u16 length;
    u16 pendingLength;
    u16 maxLength;
    u16 centered;
    BOOL dirty;
    BmpWin *printWindow;
    u8 printing;
    Font *font;
    PrintQueue *queue;
    StrBuf *buf;
    BmpWin *window;
    ClActor *slots[NAME_SLOT_COUNT];
    NameinWordTable *words[NAMEIN_WORD_TABLE_MAX];
    NameinConvTable *conv;
} NameBox;

// The message at the bottom of the screen
typedef struct NameinMessage {
    MsgData *msgData;
    Font *font;
    BmpWin *printWindow;
    u8 printing;
    PrintQueue *printQueue;
    BmpWin *window;
    WordSet *wordSet;
    StrBuf *strbuf;
    u16 color;
} NameinMessage;

typedef struct NameinTask NameinTask;
typedef struct NameinWork NameinWork;

// A step of the screen's task, with its own state, starting at 0 each time the step is set
typedef void (*NameinTaskFunc)(NameinTask *task, int *state, NameinWork *work);

// The message that is printed a letter at a time before the screen closes, and the step to go on to when it is done
typedef struct NameinTalk {
    PrintStream *stream;
    TCBExManager *tcbManager;
    NameinTaskFunc next;
    u16 wait;
    u16 timer;
} NameinTalk;

// The icon of what is being named, which bounces when it is touched, with its label
typedef struct NameinIcon {
    ClActor *actor;
    u32 unk_4;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    BOOL touched;
    u32 state;
    BOOL bouncing;
    u16 frame;
    u32 labelPalette;
    StrBuf *label;
    BmpOamActor *labelActor;
    GFLBitmap *labelBitmap;
    BOOL labelPrinting;
    PrintQueue *labelQueue;
} NameinIcon;

typedef struct NameinKeyRect {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} NameinKeyRect;

typedef struct NameinTouchPos {
    u32 x;
    u32 y;
} NameinTouchPos;

typedef struct NameinKeyPos {
    s32 x;
    s32 y;
} NameinKeyPos;

BOOL func_ov194_021f4450(const NameinKeyRect *rect, const NameinTouchPos *point);
void func_ov194_021f4394(NameinIcon *icon);
BOOL func_ov194_021f43a0(NameinIcon *icon);

void func_ov194_021f46b4(NameinTask *task, int *state, NameinWork *work);
BOOL func_ov194_021f3e78(NameinMessage *win);
void func_ov194_021f3eb0(NameinTalk *msg, HeapID heapId);
void func_ov194_021f3ed0(NameinTalk *msg);

// The sound sequences the name entry preloads and their count (another overlay's code reads them), and its proc
// functions
const u32 data_ov194_021f558c[6] = { SEQ_SE_SELECT1, SEQ_SE_CANCEL3, SEQ_SE_DECIDE1,
                                     SEQ_SE_SYS_12,  SEQ_SE_DECIDE2, SEQ_SE_BEEP };

const u32 data_ov194_021f557c = 6;

BOOL func_ov194_021f1440(GameProc *proc, u32 *state, void *param, void *arg);
BOOL func_ov194_021f1744(GameProc *proc, u32 *state, void *param, void *arg);
BOOL func_ov194_021f168c(GameProc *proc, u32 *state, void *param, void *arg);

const GameProcFunctions data_ov194_021f5580 = { func_ov194_021f1440, func_ov194_021f1744, func_ov194_021f168c };

// How fast the key highlight blinks
u32 data_ov194_021f5780 = 0x51e;

// The kana keyboard's rows: where each starts and its top
const s32 data_ov194_021f5544[] = { 5, 6, 7 };
const s32 data_ov194_021f5550[] = { 8, 12, 16 };

// The palette of the icon of each type
const u8 data_ov194_021f5569[16] = { 2, 3, 6, 5, 4, 5, 2, 0, 1, 3, 6, 5, 4, 7, 7, 0 };

// Where the selection starts on each part of a keyboard layout
const u8 data_ov194_021f555c[13] = { 0, 2, 3, 5, 6, 10, 0, 1, 9, 8, 0, 4, 4 };

void func_ov194_021f3d54(NameinMessage *message);
void func_ov194_021f1f7c(NameBox *box);
void func_ov194_021f4608(NameinTask *task);
void func_ov194_021f42a0(NameinIcon *icon);
void func_ov194_021f1878(u32 *bgs);
void func_ov194_021f4614(NameinTask *task);
void func_ov194_021f436c(NameinIcon *icon);
BOOL func_ov194_021f4628(NameinTask *task);
typedef struct NameinObjs {
    u32 palette;
    u32 chars;
    u32 cellAnims;
    ClActor *actors[21];
} NameinObjs;
void func_ov194_021f1968(NameinObjs *objs);
void func_ov194_021f1888(NameinObjs *objs, ClActUnit *unit, HeapID heapId);
void func_ov194_021f3de0(NameinMessage *win, u32 messageId, u16 species, u16 form, HeapID heapId);
void func_ov194_021f3e18(NameinMessage *win, u32 messageId, PartyPkm *pkm);
void func_ov194_021f3d70(NameinMessage *win, u32 messageId);
struct NameinKeyView;
void func_ov194_021f3008(struct NameinKeyView *kb, u32 type, Font *font, PrintQueue *queue, NameinKeyboardSet *keyboards, NameinObjs *objs, HeapID heapId);
void func_ov194_021f3cd0(NameinMessage *win, Font *font, MsgData *msgData, PrintQueue *queue, WordSet *wordSet, HeapID heapId);
void func_ov194_021f1e28(NameBox *box, StrBuf *name, u8 maxLength, Font *font, PrintQueue *queue, NameinObjs *objs, HeapID heapId);
void func_ov194_021f45ec(NameinTask *task, NameinWork *work, NameinTaskFunc func);
u32 func_ov194_021f4408(u32 type, u32 value);
void func_ov194_021f3fbc(NameinIcon *icon, u32 kind, unsigned long index, u32 param, ClActUnit *unit, Font *font,
                         PrintQueue *printQueue, MsgData *msgData, BmpOamSys *bmpOamSys, HeapID heapId);
void func_ov194_021f1780(u32 *bgs, HeapID heapId);

void func_ov194_021f463c(NameinTask *task, int *state, NameinWork *work);

BOOL func_ov194_021f2150(NameBox *box);
BOOL func_ov194_021f23fc(NameBox *box, int set);
BOOL func_ov194_021f22e8(NameBox *box);

s32 func_ov194_021f2f68(const NameinKeyPos *pos, NameinKeyPos *cell);
s32 func_ov194_021f2f60(NameinKeyPos *pos, NameinKeyPos *cell);
s32 func_ov194_021f2f8c(NameinKeyPos *pos, NameinKeyPos *cell);
void func_ov194_021f2884(NameinKeyPos *pos, u32 from, u32 to);
void func_ov194_021f462c(NameinTask *task, NameinTaskFunc func);
void func_ov194_021f4674(NameinTask *task, int *state, NameinWork *work);
void func_ov194_021f47b4(NameinTask *task, int *state, NameinWork *work);
void func_ov194_021f4aa8(NameinTask *task, int *state, NameinWork *work);
void func_ov194_021f4a5c(NameinTask *task, int *state, NameinWork *work);
BOOL func_ov194_021f299c(NameinKeyPos *pos, NameinKeyRect *rect);
void func_ov194_021f2bc8(NameinKeyPos *pos, const NameinKeyPos *delta, u8 *memory);
BOOL func_ov194_021f28fc(const NameinKeyPos *pos, NameinKeyRect *rect);
void func_ov194_021f2a88(NameinKeyPos *pos, const NameinKeyPos *delta, u8 *memory);
typedef struct NameinKeyHighlight NameinKeyHighlight;
struct NameinKeyHighlight {
    u16 baseColor[16];
    u16 altColor[16];
    u16 color[16];
    u16 phase[16];
    u16 timer;
    NameinKeyRect rect;
    s32 mode;
    BOOL active;
    u32 kanaPage;
    u32 type;
    ClActor *actors[2];
    ClActorPos pos[2];
};

typedef struct NameinKeyLayout {
    NameinKeyboard *keyboard;
    u8 memory[0x10];
    BOOL (*getRect)(NameinKeyPos *pos, NameinKeyRect *rect);
    void (*move)();
    s32 (*getKind)(NameinKeyPos *pos, NameinKeyPos *cell);
    u16 width;
    u16 height;
} NameinKeyLayout;

typedef struct NameinKeyPrint {
    BmpWin *window;
    u8 printing;
} NameinKeyPrint;

typedef struct NameinKeyView {
    NameinKeyLayout layout;
    NameinKeyHighlight highlight;
    NameinKeyPos pos;
    BmpWin *window;
    s32 state;
    s32 result;
    NameinKeyPrint print;
    Font *font;
    PrintQueue *queue;
    u16 lastChar;
    HeapID heapId;
    u32 type;
    u32 kanaPage;
    u8 kanaGroup;
    u16 slideFrame;
    u16 slideStage;
    s32 sliding;
    s32 nextType;
    BOOL hasUpperLayer;
    void *screenData[2];
    NNSG2dScreenData *screens[2];
    NameinKeyboardSet *keyboards;
} NameinKeyView;

typedef struct NameinKeyInput {
    s32 button;
    BOOL moved;
    NameinKeyPos pos;
} NameinKeyInput;

void func_ov194_021f31b8(NameinKeyView *kb);

// The screen's task, a state machine that runs a function a frame until it chooses the next
struct NameinTask {
    NameinTaskFunc func;
    BOOL done;
    int state;
    NameinWork *work;
};

// The screen's work, 0x2d4 bytes (Black 2's adds two fields for its preset names)
struct NameinWork {
    NameinGraphic *graphic;
    NameinObjs objs;
    u32 bgs;
    NameBox box;
    NameinKeyView keyboard;
    NameinKeyboardSet *keyboards;
    NameinMessage message;
    NameinTalk talk;
    NameinIcon icon;
    NameinTask task;
    Font *font;
    PrintQueue *queue;
    MsgData *msgData;
    WordSet *wordSet;
    BmpOamSys *bmpOam;
    NameEntryParam *param;
    // The message to print again once the player goes on typing
    BOOL messagePending;
    ProfWordLists *profWord;
    StrBuf *name;
};
void func_ov194_021f1a1c(NameinKeyHighlight *highlight, s32 mode, const NameinKeyRect *rect, u32 kanaPage, u32 type);
void func_ov194_021f1bfc(NameinKeyHighlight *highlight, s32 type, NameinKeyRect *rect);
BOOL func_ov194_021f1fb4(NameBox *box);
BOOL func_ov194_021f1d58(u16 *color, u16 *phase, u16 step, u32 palette, u8 index, u16 from, u16 to);
u32 func_ov194_021f3358(u32 type, u32 page, u8 group);
void func_ov194_021f35a8(NameinKeyView *kb, s32 type);

BOOL func_ov194_021f1440(GameProc *proc, u32 *state, void *arg, void *unused) {
    NameEntryParam *param = arg;
    NameinWork *work;
    s32 kind = 0;
    ClActUnit *unit;
    u16 monsno;

    if (param->gameInfo != NULL) {
        kind = func_0200c104(param->gameInfo, param->mode);
        if (kind >= 5) {
            kind = 0;
        }
    }
    GFL_HeapCreateChild(1, 0x1e, 0x70000);
    work = GFL_ProcInitSubsystem(proc, sizeof(NameinWork), 0x1e);
    MI_CpuFill8(work, 0, sizeof(NameinWork));
    work->param = param;
    work->font = GFL_FontCreate(0x17, 0, 1, FALSE, 0x1e);
    work->queue = func_0201c450(0x1000, 0x1e);
    work->msgData = GFL_MsgSysLoadData(TRUE, 2, 0x51, 0x1e);
    work->wordSet = GFL_WordSetSystemCreateDefault(0x1e);
    work->graphic = func_ov194_021f4ae4(0, 0x1e);
    work->bmpOam = func_02025538(0x1e, func_ov194_021f4bec(work->graphic));
    func_ov194_021f1780(&work->bgs, 0x1e);
    unit = func_ov194_021f4bec(work->graphic);
    func_ov194_021f1888(&work->objs, unit, 0x1e);
    monsno = param->gender;
    func_ov194_021f3fbc(&work->icon, func_ov194_021f4408(param->mode, monsno), monsno, param->unk6, unit, work->font,
                        work->queue, work->msgData, work->bmpOam, 0x1e);
    work->keyboards = func_ov194_021f4d3c(0x1e);
    work->name = GFL_StrBufClone(param->name, 0x1e);
    if (GFL_StrBufGetCharCount(work->name) != 0) {
        if (GFL_StrBufGetCharCount(work->name) > param->maxLength
            || !func_ov194_021f5004(GFL_StrBufGetStringPtr(work->name), 0x1e)) {
            GFL_StrBufClear(work->name);
        }
    }
    func_ov194_021f45ec(&work->task, work, func_ov194_021f463c);
    func_ov194_021f1e28(&work->box, work->name, param->maxLength, work->font, work->queue, &work->objs, 0x1e);
    func_ov194_021f3008(&work->keyboard, kind, work->font, work->queue, work->keyboards, &work->objs, 0x1e);
    func_ov194_021f3cd0(&work->message, work->font, work->msgData, work->queue, work->wordSet, 0x1e);
    func_ov194_021f3eb0(&work->talk, 0x1e);
    work->profWord = func_ov194_021f5304(0x1e);
    if (param->mode == NAME_ENTRY_POKEMON) {
        if (param->pkm == NULL) {
            func_ov194_021f3de0(&work->message, param->mode, param->gender, param->unk6, 0x1e);
        } else {
            func_ov194_021f3e18(&work->message, param->mode, param->pkm);
        }
    } else {
        func_ov194_021f3d70(&work->message, param->mode);
    }
    if (func_0203cd98()) {
        func_0203d34c(TRUE, 0x1e);
    }
    if (work->param->saveTask != NULL) {
        func_ov088_021bd374(work->param->saveTask);
    }
    return TRUE;
}

BOOL func_ov194_021f168c(GameProc *proc, u32 *state, void *param, void *arg) {
    NameinWork *work = arg;

    func_ov194_021f5354(work->profWord);
    func_ov194_021f3ed0(&work->talk);
    func_ov194_021f3d54(&work->message);
    func_ov194_021f31b8(&work->keyboard);
    GFL_StrBufFree(work->name);
    func_ov194_021f4d78(work->keyboards);
    func_ov194_021f1f7c(&work->box);
    func_ov194_021f4608(&work->task);
    func_ov194_021f42a0(&work->icon);
    func_ov194_021f1968(&work->objs);
    func_ov194_021f1878(&work->bgs);
    func_02025588(work->bmpOam);
    func_ov194_021f4b84(work->graphic);
    GFL_WordSetSystemFree(work->wordSet);
    GFL_MsgDataFree(work->msgData);
    func_0201c6ec(work->queue);
    func_0201c4c0(work->queue);
    GFL_FontFree(work->font);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x1e);
    return TRUE;
}

BOOL func_ov194_021f1744(GameProc *proc, u32 *state, void *param, void *arg) {
    NameinWork *work = arg;

    func_ov194_021f4614(&work->task);
    func_ov194_021f4bd8(work->graphic);
    func_0201c4e4(work->queue);
    func_ov194_021f436c(&work->icon);
    if (func_ov194_021f4628(&work->task)) {
        return TRUE;
    }
    return FALSE;
}

void func_ov194_021f1780(u32 *bgs, HeapID heapId) {
    ArcTool *arc;

    arc = GFL_ArcSysCreateFileHandle(0x76, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 2, 4, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 1, 5, 0, 0, 0, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 3, 5, 0, 0, 0, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 2, 0, 0, 0, heapId);
    GFL_G2DIOLoadArcNCLR(arc, 2, 0, 0x40, 0x60, 0x20, heapId);
    GFL_G2DIOLoadArcNCLR(arc, 2, 0, 0x40, 0x80, 0x20, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 1, 0, 0, 0, 0, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0, 0, 0, 0, 0, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 4, 2, 0, 0, 0, heapId);
    GFL_ArcToolFree(arc);
    arc = GFL_ArcSysCreateFileHandle(0x17, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 6, 4, 0x1e0, 0x20, heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysFillChar(4, 0, 1, 0);
    LoadSysMsgBox(4, 1, 0xe, 0, heapId);
}

void func_ov194_021f1878(u32 *bgs) {
    GFL_BGSysFreeFilledChar(4, 1, 0);
}

void func_ov194_021f1888(NameinObjs *objs, ClActUnit *unit, HeapID heapId) {
    ArcTool *arc;
    ClActorSetup setup;
    int i;

    arc = GFL_ArcSysCreateFileHandle(0x76, heapId);
    objs->palette = func_0204abf0(arc, 9, 0, 0, heapId);
    objs->cellAnims = func_0204af28(arc, 7, 6, heapId);
    objs->chars = func_0204a6c8(arc, 8, FALSE, 0, heapId);
    GFL_ArcToolFree(arc);

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.sequence = 0;
    for (i = 0; i < 16; i++) {
        objs->actors[i] = func_0204b294(unit, objs->chars, objs->palette, objs->cellAnims, &setup, 0, heapId);
    }
    setup.sequence = 1;
    objs->actors[17] = func_0204b294(unit, objs->chars, objs->palette, objs->cellAnims, &setup, 0, heapId);
    func_0204b3dc(objs->actors[17], FALSE);

    for (i = 18; i < 20; i++) {
        setup.x = -32;
        setup.y = -32;
        setup.sequence = i - 15;
        objs->actors[i] = func_0204b294(unit, objs->chars, objs->palette, objs->cellAnims, &setup, 0, heapId);
        func_0204b92c(objs->actors[i], TRUE);
        func_0204b3dc(objs->actors[i], FALSE);
    }
}

void func_ov194_021f1968(NameinObjs *objs) {
    int i;

    for (i = 0; i < 21; i++) {
        if (objs->actors[i] != NULL) {
            func_0204b3b4(objs->actors[i]);
        }
    }
    func_0204ada4(objs->palette);
    func_0204a8d4(objs->chars);
    func_0204afd8(objs->cellAnims);
}

ClActor *func_ov194_021f1998(NameinObjs *objs, s32 index) {
    return objs->actors[index];
}

void func_ov194_021f19a0(NameinKeyHighlight *highlight, u32 type, NameinObjs *objs, HeapID heapId) {
    NNSG2dPaletteData *palette;
    u16 *colors;
    void *file;
    int i;
    int j;

    MI_CpuFill8(highlight, 0, sizeof(NameinKeyHighlight));
    highlight->type = type;
    for (i = 0; i < 2; i++) {
        highlight->actors[i] = func_ov194_021f1998(objs, 18 + i);
    }
    file = GFL_G2DIOReadNCLR(0x76, 2, &palette, heapId);
    colors = palette->pRawData;
    for (i = 0; i < 16; i++) {
        highlight->baseColor[i] = colors[16 + i];
    }
    for (j = 0; j < 16; j++) {
        highlight->altColor[j] = colors[32 + j];
    }
    GFL_HeapFree(file);
}

void func_ov194_021f1a10(NameinKeyHighlight *highlight) {
    MI_CpuFill8(highlight, 0, sizeof(NameinKeyHighlight));
}

void func_ov194_021f1a1c(NameinKeyHighlight *highlight, s32 mode, const NameinKeyRect *rect, u32 kanaPage, u32 type) {
    int palette;
    s16 y;

    highlight->active = FALSE;
    highlight->mode = mode;
    highlight->kanaPage = kanaPage;
    highlight->type = type;
    GFL_BGSysSetScrPaletteNo(2, 0, 0, 32, 24, 1);
    GFL_BGSysSetScrPaletteNo(1, 0, 0, 32, 24, 1);
    if (kanaPage && type == 3) {
        GFL_BGSysSetScrPaletteNo(1, 3, 16, 4, 3, 4);
        GFL_BGSysSetScrPaletteNo(1, 23, 16, 4, 3, 4);
    }
    palette = (type + 1) * 3;
    GFL_BGSysSetScrPaletteNo(1, palette, 20, 3, 3, 4);
    GFL_BGSysSetScrPaletteNo(2, palette, 20, 3, 3, 4);
    if (rect != NULL && mode != 0) {
        highlight->rect = *rect;
        if (mode == 2) {
            GFL_BGSysSetScrPaletteNo(2, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top,
                                     mode + 1);
            GFL_BGSysSetScrPaletteNo(1, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top,
                                     mode + 1);
        }
        if (!func_020355c4()) {
            highlight->pos[0].x = rect->left * 8 + 7;
            y = rect->top * 8 + (rect->bottom - rect->top) * 4 - 3;
            highlight->pos[0].y = y;
            highlight->pos[1].x = rect->right * 8 - 7;
            highlight->pos[1].y = y;
            func_0204b9a0(highlight->actors[0]);
            func_0204b9a0(highlight->actors[1]);
            func_0204b3dc(highlight->actors[0], TRUE);
            func_0204b3dc(highlight->actors[1], TRUE);
        }
    }
    if (mode == 0 || func_020355c4() == TRUE) {
        func_0204b3dc(highlight->actors[0], FALSE);
        func_0204b3dc(highlight->actors[1], FALSE);
    }
    GFL_BGSysLoadScr(2);
    GFL_BGSysLoadScr(1);
    if (mode != 0) {
        MI_CpuFill8(highlight->phase, 0, sizeof(highlight->phase));
        highlight->timer = 0;
        highlight->active = TRUE;
    }
}

void func_ov194_021f1bfc(NameinKeyHighlight *highlight, s32 type, NameinKeyRect *rect) {
    ClActorPos pos;
    u32 palette;
    u16 step;
    int i;

    if (highlight->active) {
        switch (highlight->mode) {
        case 0:
            break;
        case 1:
            palette = 2;
            step = data_ov194_021f5780;
            break;
        case 2:
            palette = 3;
            step = 0x1999;
            if (highlight->timer++ > 10) {
                if (!func_020355c4()) {
                    func_ov194_021f1a1c(highlight, 1, rect, highlight->kanaPage, type);
                } else {
                    func_ov194_021f1a1c(highlight, 0, rect, highlight->kanaPage, type);
                }
            }
            break;
        }
        for (i = 0; i < 16; i++) {
            func_ov194_021f1d58(&highlight->color[i], &highlight->phase[i], step, palette, i, highlight->baseColor[i],
                                highlight->altColor[i]);
        }
    }

    pos = highlight->pos[0];
    pos.y -= GFL_BGSysGetBGOffsetY(2) * 8;
    func_0204b404(highlight->actors[0], &pos, 0);
    pos = highlight->pos[1];
    pos.y -= GFL_BGSysGetBGOffsetY(2) * 8;
    func_0204b404(highlight->actors[1], &pos, 0);
    if (highlight->type != type) {
        if (!func_020355c4()) {
            func_ov194_021f1a1c(highlight, 1, &highlight->rect, highlight->kanaPage, type);
        } else {
            func_ov194_021f1a1c(highlight, 0, &highlight->rect, highlight->kanaPage, type);
        }
    }
}

BOOL func_ov194_021f1d58(u16 *color, u16 *phase, u16 step, u32 palette, u8 index, u16 from, u16 to) {
    int next = *phase + step;
    u8 fromR;
    u8 fromG;
    u8 fromB;
    u8 toR;
    u8 toG;
    u8 toB;
    u8 r;
    u8 g;
    u8 b;
    fx16 t;

    if (next >= 0x10000) {
        *phase = next - 0x10000;
    } else {
        *phase = next;
    }
    fromG = (from & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
    t = (FX_CosIdx(*phase) + FX32_ONE) / 2;
    fromR = from & GX_RGB_R_MASK;
    fromB = (from & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
    toB = (to & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
    b = fromB + ((toB - fromB) * t >> 12);
    toR = to & GX_RGB_R_MASK;
    r = fromR + ((toR - fromR) * t >> 12);
    toG = (to & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
    g = fromG + ((toG - fromG) * t >> 12);
    *color = GX_RGB(r, g, b);
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, palette * 32 + index * 2, color, 2);
    if (*color == to) {
        return TRUE;
    }
    return FALSE;
}

void func_ov194_021f1e28(NameBox *box, StrBuf *name, u8 maxLength, Font *font, PrintQueue *queue, NameinObjs *objs, HeapID heapId) {
    BmpWin *window;
    ClActorPos pos;
    int offset;
    int i;

    MI_CpuFill8(box, 0, sizeof(NameBox));
    box->font = font;
    box->maxLength = maxLength;
    box->queue = queue;
    window = BmpWin_CreateDynamic(0, 8, 2, 24, 2, 15, TRUE);
    box->window = window;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    box->printWindow = box->window;
    box->printing = FALSE;
    box->buf = GFL_StrBufCreate(14, heapId);
    for (i = 0; i < NAMEIN_WORD_TABLE_MAX; i++) {
        box->words[i] = func_ov194_021f4dbc(i, heapId);
    }
    box->conv = func_ov194_021f4f50(heapId);
    for (i = 0; i < NAME_SLOT_COUNT; i++) {
        box->slots[i] = func_ov194_021f1998(objs, i);
    }

    if (GFL_StrBufGetCharCount(name) == 0) {
        box->text[0] = GFL_StrBufGetTerminator();
    } else {
        GFL_StrBufStoreString(name, box->text, maxLength + 1);
        box->length = GFL_StrBufGetCharCount(name);
    }

    pos.y = 37;
    offset = 64 - box->maxLength * 14 / 2;
    if (offset < 0) {
        offset = 0;
    }
    offset += 64;
    for (i = 0; i < NAME_SLOT_COUNT; i++) {
        pos.x = offset + i * 14;
        func_0204b878(box->slots[i], 1);
        func_0204b92c(box->slots[i], TRUE);
        func_0204b404(box->slots[i], &pos, 0);
        func_0204b3dc(box->slots[i], TRUE);
    }
    for (i = box->maxLength; i < NAME_SLOT_COUNT; i++) {
        func_0204b3dc(box->slots[i], FALSE);
    }
    func_0204b95c(box->slots[box->length]);
    box->dirty = TRUE;
    func_ov194_021f1fb4(box);
}

void func_ov194_021f1f7c(NameBox *box) {
    int i;

    func_ov194_021f4f60(box->conv);
    for (i = 0; i < NAMEIN_WORD_TABLE_MAX; i++) {
        func_ov194_021f4dd0(box->words[i]);
    }
    GFL_StrBufFree(box->buf);
    BmpWin_Free(box->window);
    MI_CpuFill8(box, 0, sizeof(NameBox));
}

BOOL func_ov194_021f1fb4(NameBox *box) {
    // The name's text is printed from its window's top left edge
    const int left = 0;
    const int top = 0;
    u16 chars[14];
    u16 ch[2];
    PrintQueue *queue;
    int x;
    int i;

    if (box->dirty) {
        if (box->length != 0) {
            wcharsncpy(box->text, chars, box->length + 1);
        }
        if (box->pendingLength != 0) {
            wcharsncpy(box->pending, &chars[box->length], box->pendingLength + 1);
        }
        if (box->length + box->pendingLength > 0) {
            GFL_BitmapFill(BmpWin_GetBitmap(box->window), 0);
            for (i = 0; i < box->length + box->pendingLength; i++) {
                ch[0] = chars[i];
                ch[1] = GFL_StrBufGetTerminator();
                GFL_StrBufLoadFixedString(box->buf, ch, 2);
                x = 64 - box->maxLength * 14 / 2;
                if (x < left) {
                    x = left;
                }
                x += i * 14 + 7;
                x -= GFL_FontGetBlockWidth(box->buf, box->font, 0) / 2;
                if (i >= box->length) {
                    queue = box->queue;
                    func_0201c724(queue, BmpWin_GetBitmap(box->printWindow), x, top, box->buf, box->font, 0xc80);
                } else {
                    queue = box->queue;
                    func_0201c6fc(queue, BmpWin_GetBitmap(box->printWindow), x, top, box->buf, box->font);
                }
                box->printing = TRUE;
            }
        } else {
            GFL_BitmapFill(BmpWin_GetBitmap(box->window), 0);
            BmpWin_FlushChar(box->window);
        }

        for (i = 0; i < box->maxLength; i++) {
            func_0204b8c4(box->slots[i], 0);
            func_0204b878(box->slots[i], 1);
            func_0204b3dc(box->slots[i], TRUE);
        }
        for (; i < NAME_SLOT_COUNT; i++) {
            func_0204b8c4(box->slots[i], 0);
            func_0204b878(box->slots[i], 1);
            func_0204b3dc(box->slots[i], FALSE);
        }
        for (i = 0; i <= box->length && i < box->maxLength; i++) {
            func_0204b3dc(box->slots[i], TRUE);
        }
        func_0204b878(box->slots[box->length], 0);
        box->dirty = FALSE;
    }
    return func_ov194_021f22e8(box);
}

BOOL func_ov194_021f2150(NameBox *box) {
    if (box->pendingLength != 0) {
        box->pendingLength--;
        box->pending[box->pendingLength] = GFL_StrBufGetTerminator();
        box->dirty = TRUE;
        return TRUE;
    }
    if (box->length != 0) {
        box->length--;
        box->text[box->length] = GFL_StrBufGetTerminator();
        box->dirty = TRUE;
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov194_021f2190(NameBox *box, u16 c) {
    if (box->length < box->maxLength) {
        box->text[box->length] = c;
        box->length++;
        box->text[box->length] = GFL_StrBufGetTerminator();
        box->dirty = TRUE;
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov194_021f21bc(NameBox *box, u16 c, int set) {
    if (box->length < box->maxLength && box->pendingLength < 3) {
        box->pending[box->pendingLength] = c;
        box->pendingLength++;
        box->pending[box->pendingLength] = GFL_StrBufGetTerminator();
        func_ov194_021f23fc(box, set);
        box->dirty = TRUE;
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov194_021f21fc(NameBox *box, int which) {
    const NameinWordTable *table;
    u16 index;
    u16 added[4];
    u16 removed[4];
    u8 addCount;
    u8 removeCount;
    int i;

    switch (which) {
    case NAMEIN_WORD_TABLE_DAKUTEN:
        table = box->words[NAMEIN_WORD_TABLE_DAKUTEN];
        break;
    case NAMEIN_WORD_TABLE_HANDAKUTEN:
        table = box->words[NAMEIN_WORD_TABLE_HANDAKUTEN];
        break;
    default:
        return FALSE;
    }
    if (box->length != 0) {
        index = func_ov194_021f4ed8(table, &box->text[box->length - 1]);
        if (index != 0xffff) {
            if (func_ov194_021f4e18(table, index, added, 4, &addCount)) {
                func_ov194_021f4ddc(table, index, removed, 4, &removeCount);
                for (i = 0; i < removeCount; i++) {
                    func_ov194_021f2150(box);
                }
                for (i = 0; i < addCount; i++) {
                    func_ov194_021f2190(box, added[i]);
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL func_ov194_021f22ac(NameBox *box) {
    u16 next;

    if (box->length != 0) {
        if (func_ov194_021f4f7c(box->conv, &box->text[box->length - 1], &next)) {
            func_ov194_021f2150(box);
            func_ov194_021f2190(box, next);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov194_021f22e8(NameBox *box) {
    PrintQueue *queue = box->queue;

    if (box->printing) {
        if (!func_0201c6c4(queue, BmpWin_GetBitmap(box->printWindow))) {
            BmpWin_FlushChar(box->printWindow);
            box->printing = FALSE;
        }
    }
    if (!box->printing) {
        return TRUE;
    }
    return FALSE;
}

void func_ov194_021f2328(NameBox *box, StrBuf *dest) {
    GFL_StrBufLoadFixedString(dest, box->text, box->length + 1);
}

void func_ov194_021f233c(NameBox *box) {
    if (box->pendingLength != 0) {
        box->pendingLength = 0;
        box->pending[box->pendingLength] = GFL_StrBufGetTerminator();
        box->dirty = TRUE;
    }
}

void func_ov194_021f235c(NameBox *box) {
    int i;

    for (i = 0; i < box->pendingLength; i++) {
        func_ov194_021f2190(box, box->pending[i]);
    }
    box->pendingLength = 0;
    box->pending[box->pendingLength] = GFL_StrBufGetTerminator();
    box->dirty = TRUE;
}

void func_ov194_021f2394(NameBox *box) {
    if (box->length != 0) {
        box->length = 0;
        box->text[box->length] = GFL_StrBufGetTerminator();
        box->dirty = TRUE;
    }
    func_ov194_021f233c(box);
}

void func_ov194_021f23b8(NameBox *box, const u16 *str) {
    u16 c = *str;

    while (c != GFL_StrBufGetTerminator()) {
        if (!func_ov194_021f2190(box, c)) {
            break;
        }
        str++;
        c = *str;
    }
}

BOOL func_ov194_021f23e4(NameBox *box) {
    return box->length == box->maxLength;
}

u16 func_ov194_021f23f4(NameBox *box) {
    return box->length;
}

u16 func_ov194_021f23f8(NameBox *box) {
    return box->pendingLength;
}

BOOL func_ov194_021f23fc(NameBox *box, int set) {
    BOOL (*lookup)(const NameinWordTable *table, u32 index, u16 *dest, u32 n, u8 *len);
    u8 count;
    u16 chars[4];
    u16 index;
    int i;
    int j;

    if (set) {
        lookup = func_ov194_021f4e58;
    } else {
        lookup = func_ov194_021f4e18;
    }
    for (i = 0; i < box->pendingLength; i++) {
        index = func_ov194_021f4ed8(box->words[NAMEIN_WORD_TABLE_COMPOSE], &box->pending[i]);
        if (index != 0xffff) {
            for (j = 0; j < i; j++) {
                func_ov194_021f2190(box, box->pending[j]);
            }
            for (j = 0; j < i; j++) {
                func_ov194_021f2150(box);
            }
            if (lookup(box->words[NAMEIN_WORD_TABLE_COMPOSE], index, chars, 4, &count)) {
                for (i = 0; i < count; i++) {
                    func_ov194_021f2190(box, chars[i]);
                }
                func_ov194_021f4ddc(box->words[NAMEIN_WORD_TABLE_COMPOSE], index, chars, 4, &count);
                for (i = 0; i < count; i++) {
                    func_ov194_021f2150(box);
                }
                func_ov194_021f4e98(box->words[NAMEIN_WORD_TABLE_COMPOSE], index, chars, 4, &count);
                for (i = 0; i < count; i++) {
                    box->pending[box->pendingLength] = chars[i];
                    box->pendingLength++;
                    box->pending[box->pendingLength] = GFL_StrBufGetTerminator();
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL func_ov194_021f253c(const NameBox *box) {
    int i;
    int spaces;

    if (box->length == 0) {
        return FALSE;
    }
    spaces = 0;
    for (i = 0; i < box->length; i++) {
        if (box->text[i] == ' ') {
            spaces++;
        }
    }
    return i != spaces;
}

void func_ov194_021f256c(NameinKeyLayout *layout, u32 type, u32 index, NameinKeyboardSet *keyboards, s32 heapId) {
    int i;

    MI_CpuFill8(layout, 0, sizeof(NameinKeyLayout));
    layout->keyboard = func_ov194_021f4d94(keyboards, index);
    switch (type) {
    case 3:
        layout->width = 11;
        layout->height = 4;
        layout->getRect = func_ov194_021f299c;
        layout->move = func_ov194_021f2bc8;
        layout->getKind = func_ov194_021f2f8c;
        break;
    case 0:
    case 1:
    case 2:
        layout->width = 13;
        layout->height = 6;
        layout->getRect = (BOOL (*)(NameinKeyPos *, NameinKeyRect *)) func_ov194_021f28fc;
        layout->move = func_ov194_021f2a88;
        layout->getKind = (s32 (*)(NameinKeyPos *, NameinKeyPos *)) func_ov194_021f2f68;
        break;
    }
    for (i = 0; i < 13; i++) {
        layout->memory[i] = data_ov194_021f555c[i];
    }
}

void func_ov194_021f25f8(NameinKeyLayout *layout, u32 index, NameinKeyboardSet *keyboards) {
    layout->keyboard = func_ov194_021f4d94(keyboards, index);
}

void func_ov194_021f2608(NameinKeyLayout *layout) {
    MI_CpuFill8(layout, 0, sizeof(NameinKeyLayout));
}

void func_ov194_021f26f0(NameinKeyLayout *layout, NameinKeyPrint *print, PrintQueue *queue, Font *font, HeapID heapId);

BOOL func_ov194_021f2818(const NameinKeyLayout *layout, NameinKeyPos *pos, NameinKeyRect *rect);
void func_ov194_021f256c(NameinKeyLayout *layout, u32 type, u32 index, NameinKeyboardSet *keyboards, s32 heapId);

s32 func_ov194_021f2614(NameinKeyLayout *layout, NameinKeyPos *pos, u16 *c) {
    NameinKeyPos cell;
    s32 kind = layout->getKind(pos, &cell);

    if (kind == 1 || kind == 10 || kind == 11) {
        if (c != NULL) {
            *c = func_ov194_021f4da4(layout->keyboard, cell.x, cell.y);
        }
    } else if (kind == 9) {
        if (c != NULL) {
            *c = 0x20;
        }
    }
    return kind;
}

BOOL func_ov194_021f265c(const NameinKeyView *kb, NameinTouchPos *touch, NameinKeyPos *found) {
    NameinKeyPos pos;
    NameinKeyRect rect;

    pos.y = 0;
    while (pos.y < kb->layout.height) {
        pos.x = 0;
        while (pos.x < kb->layout.width) {
            if (func_ov194_021f2818(&kb->layout, &pos, &rect)) {
                rect.left *= 8;
                rect.top *= 8;
                rect.right *= 8;
                rect.bottom *= 8;
                if (func_ov194_021f4450(&rect, touch)) {
                    *found = pos;
                    return TRUE;
                }
            }
            pos.x++;
        }
        pos.y++;
    }
    return FALSE;
}

void func_ov194_021f26e0(NameinKeyLayout *layout, NameinKeyPos *pos, const NameinKeyPos *delta) {
    layout->move(pos, delta, layout->memory);
}

void func_ov194_021f26f0(NameinKeyLayout *layout, NameinKeyPrint *print, PrintQueue *queue, Font *font, HeapID heapId) {
    StrBuf *buf = GFL_StrBufCreate(2, heapId);
    NameinKeyPos pos;
    NameinKeyRect rect;
    u16 c;
    u16 str[2];
    u16 x;
    u16 y;
    int kind;

    for (pos.y = 0; pos.y < func_ov194_021f4da0(layout->keyboard); pos.y++) {
        for (pos.x = 0; pos.x < func_ov194_021f4d9c(layout->keyboard); pos.x++) {
            if (func_ov194_021f2818(layout, &pos, &rect)) {
                kind = func_ov194_021f2614(layout, &pos, &c);
                if (kind == 1 || kind == 10 || kind == 11) {
                    str[0] = c;
                    str[1] = GFL_StrBufGetTerminator();
                    GFL_StrBufLoadFixedString(buf, str, 2);
                    rect.left *= 8;
                    rect.top *= 8;
                    rect.right *= 8;
                    rect.bottom *= 8;
                    x = rect.left + (rect.right - rect.left) / 2 - 3 * 8;
                    y = rect.top + (rect.bottom - rect.top) / 2 - 5 * 8;
                    x -= GFL_FontGetBlockWidth(buf, font, 0) / 2;
                    y -= GFL_FontGetBlockHeight(buf, font) / 2;
                    func_0201c6fc(queue, BmpWin_GetBitmap(print->window), x - 1, y, buf, font);
                    print->printing = TRUE;
                }
            }
        }
    }
    GFL_StrBufFree(buf);
}

BOOL func_ov194_021f2818(const NameinKeyLayout *layout, NameinKeyPos *pos, NameinKeyRect *rect) {
    return layout->getRect(pos, rect);
}

BOOL func_ov194_021f2828(const NameinKeyLayout *layout, int kind, NameinKeyPos *found) {
    NameinKeyPos pos;

    for (pos.y = 0; pos.y <= layout->height; pos.y++) {
        for (pos.x = 0; pos.x <= layout->width; pos.x++) {
            if (kind == layout->getKind(&pos, NULL)) {
                *found = pos;
                return TRUE;
            }
        }
    }
    return FALSE;
}

void func_ov194_021f2884(NameinKeyPos *pos, u32 from, u32 to) {
    s32 (*oldKind)(NameinKeyPos *, NameinKeyPos *);
    s32 (*newKind)(NameinKeyPos *, NameinKeyPos *);
    s32 width;
    s32 height;
    s32 kind;
    NameinKeyPos next;

    if (from == 3) {
        oldKind = func_ov194_021f2f8c;
        newKind = func_ov194_021f2f60;
        width = 13;
        height = 5;
    } else if (to == 3) {
        oldKind = func_ov194_021f2f60;
        newKind = func_ov194_021f2f8c;
        width = 11;
        height = 4;
    }
    kind = oldKind(pos, NULL);
    for (next.y = 0; next.y <= height; next.y++) {
        for (next.x = 0; next.x <= width; next.x++) {
            if (kind == newKind(&next, NULL)) {
                *pos = next;
                return;
            }
        }
    }
}

BOOL func_ov194_021f28fc(const NameinKeyPos *pos, NameinKeyRect *rect) {
    NameinKeyPos cell;

    switch (func_ov194_021f2f60(pos, &cell)) {
    case 1:
    case 10:
    case 11:
        rect->left = cell.x * 2 + 3;
        rect->top = cell.y * 3 + 5;
        rect->right = rect->left + 2;
        rect->bottom = rect->top + 3;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        rect->left = (pos->x + 1) * 3;
        rect->top = pos->y * 3 + 5;
        rect->right = rect->left + 3;
        rect->bottom = rect->top + 3;
        break;
    case 6:
        rect->left = 15;
        rect->top = pos->y * 3 + 5;
        rect->right = rect->left + 7;
        rect->bottom = rect->top + 3;
        break;
    case 7:
        rect->left = 22;
        rect->top = pos->y * 3 + 5;
        rect->right = rect->left + 7;
        rect->bottom = rect->top + 3;
        break;
    case 0:
        return FALSE;
    }
    if (pos->y == 5 && pos->x > 6) {
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov194_021f299c(NameinKeyPos *pos, NameinKeyRect *rect) {
    NameinKeyPos cell;

    switch (func_ov194_021f2f8c(pos, &cell)) {
    case 1:
        rect->left = data_ov194_021f5544[cell.y] + cell.x * 2;
        rect->top = data_ov194_021f5550[cell.y];
        rect->right = rect->left + 2;
        rect->bottom = rect->top + 3;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        rect->left = (pos->x + 1) * 3;
        rect->top = 20;
        rect->right = rect->left + 3;
        rect->bottom = rect->top + 3;
        break;
    case 6:
        rect->left = 25;
        rect->top = 8;
        rect->right = rect->left + 4;
        rect->bottom = rect->top + 3;
        break;
    case 7:
        rect->left = 24;
        rect->top = 12;
        rect->right = rect->left + 5;
        rect->bottom = rect->top + 3;
        break;
    case 8:
        if (pos->x == 0) {
            rect->left = 3;
            rect->top = 16;
            rect->right = rect->left + 4;
            rect->bottom = rect->top + 3;
            break;
        } else {
            rect->left = 23;
            rect->top = 16;
            rect->right = rect->left + 4;
            rect->bottom = rect->top + 3;
            break;
        }
    case 9:
        rect->left = 15;
        rect->top = 20;
        rect->right = rect->left + 12;
        rect->bottom = rect->top + 3;
        break;
    case 0:
        return FALSE;
    }
    switch (pos->y) {
    case 1:
        if (pos->x > 9) {
            return FALSE;
        }
        break;
    case 2:
        if (pos->x > 9) {
            return FALSE;
        }
        break;
    case 3:
        if (pos->x > 4) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

void func_ov194_021f2a88(NameinKeyPos *pos, const NameinKeyPos *delta, u8 *memory) {
    BOOL toBottom = FALSE;
    BOOL fromBottom = FALSE;
    int x;
    int y;
    int slot;

    switch (pos->y) {
    case 0:
        x = pos->x + delta->x;
        pos->x = x;
        if (x < 0) {
            x = 12;
        } else if (x > 12) {
            x = 0;
        }
        pos->x = x;
        y = pos->y + delta->y;
        pos->y = y;
        if (y < 0) {
            y = 5;
        } else if (y > 5) {
            y = 0;
        }
        pos->y = y;
        if (y == 5) {
            toBottom = TRUE;
        }
        break;
    case 4:
        x = pos->x + delta->x;
        pos->x = x;
        if (x < 0) {
            x = 12;
        } else if (x > 12) {
            x = 0;
        }
        pos->x = x;
        y = pos->y + delta->y;
        pos->y = y;
        if (y < 0) {
            y = 5;
        } else if (y > 5) {
            y = 0;
        }
        pos->y = y;
        if (y == 5) {
            toBottom = TRUE;
        }
        break;
    case 5:
        x = pos->x + delta->x;
        pos->x = x;
        if (x < 0) {
            x = 5;
        } else if (x > 5) {
            x = 0;
        }
        pos->x = x;
        y = pos->y + delta->y;
        pos->y = y;
        if (y < 0) {
            y = 5;
        } else if (y > 5) {
            y = 0;
        }
        pos->y = y;
        if (y != 5) {
            fromBottom = TRUE;
        }
        break;
    default:
        x = pos->x + delta->x;
        pos->x = x;
        if (x < 0) {
            x = 12;
        } else if (x > 12) {
            x = 0;
        }
        pos->x = x;
        y = pos->y + delta->y;
        pos->y = y;
        if (y < 0) {
            y = 5;
        } else if (y > 5) {
            y = 0;
        }
        pos->y = y;
        break;
    }

    if (toBottom) {
        x = pos->x;
        if (x >= 6 && x <= 9) {
            memory[4] = x;
            pos->x = 4;
        } else if (x >= 10 && x <= 12) {
            memory[5] = x;
            pos->x = 5;
        } else if (x >= 0 && x <= 5) {
            slot = x * 2 / 3;
            memory[slot] = x;
            pos->x = slot;
        }
    }
    if (fromBottom) {
        if (pos->x == 4) {
            pos->x = memory[4];
        } else if (pos->x == 5) {
            pos->x = memory[5];
        } else if (pos->x >= 0 && pos->x <= 3) {
            pos->x = memory[pos->x];
        }
    }
}

void func_ov194_021f2bc8(NameinKeyPos *pos, const NameinKeyPos *delta, u8 *memory) {
    int clamped;
    int x;
    int y;

    switch (pos->y) {
    case 0:
        x = pos->x + delta->x;
        pos->x = x;
        if (x < 0) {
            x = 10;
        } else if (x > 10) {
            x = 0;
        }
        pos->x = x;
        y = pos->y + delta->y;
        pos->y = y;
        if (y < 0) {
            y = 3;
        } else if (y > 3) {
            y = 0;
        }
        pos->y = y;
        if (y == 3) {
            switch (pos->x) {
            case 0:
            case 1:
            case 2:
            case 3:
                break;
            case 8:
            case 9:
                memory[8] = pos->x;
                memory[12] = pos->x;
                memory[11] = 8;
                break;
            case 4:
            case 5:
            case 6:
            case 7:
                memory[12] = pos->x;
                memory[11] = pos->x;
                break;
            case 10:
                memory[12] = 10;
                memory[11] = 9;
                break;
            }
            switch (pos->x) {
            case 0:
                pos->x = memory[6];
                return;
            case 1:
            case 2:
                pos->x += 1;
                return;
            default:
                pos->x = 4;
                return;
            }
        } else if (y == 1) {
            switch (pos->x) {
            case 0:
            case 1:
            case 2:
            case 3:
                break;
            case 8:
            case 9:
                memory[8] = pos->x;
                memory[12] = pos->x;
                clamped = pos->x;
                if (clamped > 9) {
                    clamped = 9;
                } else if (clamped < 4) {
                    clamped = 4;
                }
                memory[11] = clamped;
                break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 10:
                memory[12] = pos->x;
                clamped = pos->x;
                if (clamped > 9) {
                    clamped = 9;
                } else if (clamped < 4) {
                    clamped = 4;
                }
                memory[11] = clamped;
                break;
            }
            switch (pos->x) {
            case 8:
            case 9:
                pos->x = 8;
                return;
            case 10:
                pos->x = 9;
                return;
            }
        }
        break;
    case 1:
        x = pos->x + delta->x;
        pos->x = x;
        if (x < 0) {
            x = 9;
        } else if (x > 9) {
            x = 0;
        }
        pos->x = x;
        y = pos->y + delta->y;
        pos->y = y;
        if (y < 0) {
            y = 3;
        } else if (y > 3) {
            y = 0;
        }
        pos->y = y;
        if (y == 0) {
            switch (pos->x) {
            case 7:
            case 8:
                memory[9] = pos->x;
                break;
            }
            switch (pos->x) {
            case 8:
                pos->x = memory[8];
                return;
            case 9:
                pos->x = 10;
                return;
            }
        } else if (y == 2) {
            switch (pos->x) {
            case 7:
            case 8:
                memory[9] = pos->x;
                break;
            }
            switch (pos->x) {
            case 0:
                pos->x = memory[7];
                return;
            case 7:
            case 8:
                pos->x = 8;
                return;
            case 9:
                pos->x = 9;
                return;
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            default:
                pos->x += 1;
                return;
            }
        }
        break;
    case 2:
        x = pos->x + delta->x;
        pos->x = x;
        if (x < 0) {
            x = 9;
        } else if (x > 9) {
            x = 0;
        }
        pos->x = x;
        y = pos->y + delta->y;
        pos->y = y;
        if (y < 0) {
            y = 3;
        } else if (y > 3) {
            y = 0;
        }
        pos->y = y;
        if (y == 1) {
            switch (pos->x) {
            case 0:
            case 1:
                memory[7] = pos->x;
                break;
            case 2:
            case 3:
                break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                memory[11] = pos->x;
                memory[12] = pos->x;
                break;
            }
            switch (pos->x) {
            case 0:
            case 1:
                pos->x = 0;
                return;
            case 8:
                pos->x = memory[9];
                return;
            case 9:
                pos->x = 9;
                return;
            default:
                pos->x -= 1;
                return;
            }
        } else if (y == 3) {
            switch (pos->x) {
            case 0:
            case 1:
                memory[7] = pos->x;
                break;
            case 2:
            case 3:
                break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                memory[11] = pos->x;
                memory[12] = pos->x;
                break;
            case 9:
                memory[11] = 9;
                memory[12] = 10;
                break;
            }
            switch (pos->x) {
            case 0:
                pos->x = memory[10];
                return;
            case 1:
            case 2:
            case 3:
                break;
            default:
                pos->x = 4;
                return;
            }
        }
        break;
    case 3:
        x = pos->x + delta->x;
        pos->x = x;
        if (x < 0) {
            x = 4;
        } else if (x > 4) {
            x = 0;
        }
        pos->x = x;
        y = pos->y + delta->y;
        pos->y = y;
        if (y < 0) {
            y = 3;
        } else if (y > 3) {
            y = 0;
        }
        pos->y = y;
        if (y == 2) {
            switch (pos->x) {
            case 0:
            case 1:
                memory[10] = pos->x;
                memory[6] = pos->x;
                break;
            }
            switch (pos->x) {
            case 0:
            case 1:
            case 2:
            case 3:
                break;
            case 4:
                pos->x = memory[11];
                return;
            }
        } else if (y == 0) {
            switch (pos->x) {
            case 0:
            case 1:
                memory[10] = pos->x;
                memory[6] = pos->x;
                break;
            }
            switch (pos->x) {
            case 0:
            case 1:
                pos->x = 0;
                return;
            case 2:
            case 3:
                pos->x -= 1;
                return;
            case 4:
                pos->x = memory[12];
                return;
            }
        }
        break;
    }
}

s32 func_ov194_021f2f60(NameinKeyPos *pos, NameinKeyPos *cell) {
    return func_ov194_021f2f68(pos, cell);
}

s32 func_ov194_021f2f68(const NameinKeyPos *pos, NameinKeyPos *cell) {
    if (pos->y < 5) {
        if (cell != NULL) {
            cell->y = pos->y;
            cell->x = pos->x;
        }
        return 1;
    }
    if (pos->x <= 5) {
        return pos->x + 2;
    }
    return 0;
}

s32 func_ov194_021f2f8c(NameinKeyPos *pos, NameinKeyPos *cell) {
    s32 kind = 0;
    NameinKeyPos found;

    switch (pos->y) {
    case 0:
        if (pos->x == 10) {
            kind = 6;
        } else {
            found = *pos;
            kind = 1;
        }
        break;
    case 1:
        if (pos->x == 9) {
            kind = 7;
        } else {
            found = *pos;
            kind = 1;
        }
        break;
    case 2:
        if (pos->x == 0 || pos->x == 9) {
            kind = 8;
        } else {
            found.x = pos->x - 1;
            found.y = pos->y;
            kind = 1;
        }
        break;
    case 3:
        kind = 9;
        if (pos->x != 4) {
            kind = pos->x + 2;
        }
        break;
    }
    if (kind == 1 && cell != NULL) {
        *cell = found;
    }
    return kind;
}

void func_ov194_021f3008(struct NameinKeyView *kb, u32 type, Font *font, PrintQueue *queue, NameinKeyboardSet *keyboards, NameinObjs *objs, HeapID heapId) {
    // The cursor starts on the top left key
    const int startX = 0;
    const int startY = 0;
    BmpWin *window;
    NameinKeyRect rect;

    MI_CpuFill8(kb, 0, sizeof(NameinKeyView));
    kb->pos.x = startX;
    kb->pos.y = startY;
    kb->font = font;
    kb->queue = queue;
    kb->heapId = heapId;
    kb->type = type;
    kb->nextType = type;
    kb->keyboards = keyboards;
    kb->screenData[0] = GFL_G2DIOReadNSCR(ARCID_NAMEIN_GRAPHIC, 10, FALSE, &kb->screens[0], heapId);
    kb->screenData[1] = GFL_G2DIOReadNSCR(ARCID_NAMEIN_GRAPHIC, 5, FALSE, &kb->screens[1], heapId);
    if (type == 3) {
        GFL_BGSysBufferScrDefault(1, kb->screens[0]->rawData, 0x800);
        GFL_BGSysSetBGEnabled(2, FALSE);
    } else {
        GFL_BGSysBufferScrDefault(1, kb->screens[1]->rawData, 0x800);
        GFL_BGSysSetBGEnabled(2, TRUE);
    }
    GFL_BGSysLoadScr(1);
    func_ov194_021f19a0(&kb->highlight, type, objs, heapId);
    func_ov194_021f256c(&kb->layout, type, func_ov194_021f3358(kb->type, kb->kanaPage, kb->kanaGroup), kb->keyboards, heapId);
    window = BmpWin_CreateDynamic(3, 3, 5, 26, 15, 15, TRUE);
    kb->window = window;
    BmpWin_TransferNow(window);
    kb->print.window = kb->window;
    kb->print.printing = FALSE;
    func_ov194_021f26f0(&kb->layout, &kb->print, queue, font, heapId);
    if (!func_020355c4()) {
        if (func_ov194_021f2818(&kb->layout, &kb->pos, &rect)) {
            func_ov194_021f1a1c(&kb->highlight, 1, &rect, kb->kanaPage, kb->type);
        }
    } else {
        if (func_ov194_021f2818(&kb->layout, &kb->pos, &rect)) {
            func_ov194_021f1a1c(&kb->highlight, 0, &rect, kb->kanaPage, kb->type);
        }
    }
}

void func_ov194_021f31b8(NameinKeyView *kb) {
    BmpWin_Free(kb->window);
    func_ov194_021f2608(&kb->layout);
    func_ov194_021f1a10(&kb->highlight);
    GFL_HeapFree(kb->screenData[0]);
    GFL_HeapFree(kb->screenData[1]);
    MI_CpuFill8(kb, 0, sizeof(NameinKeyView));
}

BOOL func_ov194_021f38cc(NameinKeyView *kb, NameinKeyInput *input);
BOOL func_ov194_021f37ec(NameinKeyView *kb, NameinKeyInput *input);
BOOL func_ov194_021f3bd4(NameinKeyView *kb, NameinKeyInput *input);
BOOL func_ov194_021f3b18(NameinKeyView *kb, NameinKeyInput *input);
void func_ov194_021f3c40(NameinKeyView *kb, NameinKeyInput *input);
BOOL func_ov194_021f36e8(NameinKeyView *kb, NameinKeyInput *input);
void func_ov194_021f391c(NameinKeyView *kb, NameinKeyInput *input);
BOOL func_ov194_021f3a0c(NameinKeyView *kb, NameinKeyInput *input);
BOOL func_ov194_021f33c0(NameinKeyView *kb);
u32 func_ov194_021f3304(NameinKeyView *kb);
void func_ov194_021f31f4(NameinKeyView *kb, NameBox *box) {
    NameinKeyInput input;
    NameinKeyInput slideInput;
    NameinKeyRect rect;
    BOOL acted;

    kb->result = 0;
    switch (kb->state) {
    case 0:
        MI_CpuFill8(&input, 0, sizeof(NameinKeyInput));
        acted = func_ov194_021f38cc(kb, &input);
        if (!acted) {
            func_ov194_021f36e8(kb, &input);
            acted = func_ov194_021f37ec(kb, &input);
        }
        if (acted) {
            func_ov194_021f391c(kb, &input);
        }
        break;
    case 2:
        MI_CpuFill8(&slideInput, 0, sizeof(NameinKeyInput));
        acted = func_ov194_021f3bd4(kb, &slideInput);
        if (!acted) {
            func_ov194_021f3a0c(kb, &slideInput);
            acted = func_ov194_021f3b18(kb, &slideInput);
        }
        if (acted) {
            func_ov194_021f3c40(kb, &slideInput);
        }
        if (func_ov194_021f33c0(kb)) {
            kb->state = 0;
        }
        break;
    case 1:
        if (func_ov194_021f23e4(box)) {
            func_ov194_021f2828(&kb->layout, 7, &kb->pos);
            kb->result = 11;
        }
        kb->state = 0;
        break;
    }
    func_ov194_021f2818(&kb->layout, &kb->pos, &rect);
    func_ov194_021f1bfc(&kb->highlight, kb->type, &rect);
    func_ov194_021f3304(kb);
}

u32 func_ov194_021f32d8(NameinKeyView *kb, u16 *c) {
    s32 result;

    result = kb->result;
    if ((((result == 1) || (result == 2)) || (result == 9)) && (c != NULL)) {
        *c = kb->lastChar;
    }
    return kb->result;
}

u32 func_ov194_021f32fc(NameinKeyView *kb) {
    return kb->kanaPage;
}

u32 func_ov194_021f3304(NameinKeyView *kb) {
    PrintQueue *queue = kb->queue;

    if (kb->print.printing) {
        if (!func_0201c6c4(queue, BmpWin_GetBitmap(kb->print.window))) {
            BmpWin_FlushChar(kb->print.window);
            kb->print.printing = FALSE;
        }
    }
    if (!kb->print.printing) {
        return TRUE;
    }
    return FALSE;
}

u32 func_ov194_021f3350(NameinKeyView *kb) {
    return kb->type;
}

u32 func_ov194_021f3358(u32 type, u32 page, u8 group) {
    if (type == 3) {
        return type + group * 2 + page;
    }
    return type;
}

BOOL func_ov194_021f3364(NameinKeyView *kb, s32 type) {
    BOOL hasUpperLayer;

    if (kb->nextType != type) {
        kb->nextType = type;
        hasUpperLayer = FALSE;
        kb->slideFrame = 0;
        kb->slideStage = 0;
        kb->sliding = TRUE;
        if (type == 3 || kb->type == 3) {
            hasUpperLayer = TRUE;
        }
        kb->hasUpperLayer = hasUpperLayer;
        if (hasUpperLayer) {
            G2_SetBlendAlpha(0xe, 0x2f, 16, 16);
        } else {
            G2_SetBlendAlpha(0xa, 0x2f, 16, 16);
        }
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov194_021f33c0(NameinKeyView *kb) {
    // The keys (BG 1) and their labels (BG 3) slide over 7 frames each way, with the upper layer (BG 2) when there
    // is one
    const u32 labelBg = 3;
    const u32 keyBg = 1;
    const u32 upperBg = 2;
    const int slideFrames = 7;
    int frame;
    s16 alpha;
    s16 offset;

    if (kb->sliding) {
        switch (kb->slideStage) {
        case 0:
            GFL_BGSysGetBGOffsetY(keyBg);
            frame = kb->slideFrame;
            alpha = -(frame * 16) / slideFrames + 16;
            reg_G2_BLDALPHA = alpha | (keyBg << 12);
            offset = -48 * frame / slideFrames;
            GFL_BGSysMoveBG(keyBg, BG_MOVE_SET_Y, offset);
            GFL_BGSysMoveBG(labelBg, BG_MOVE_SET_Y, offset);
            if (kb->hasUpperLayer) {
                GFL_BGSysMoveBG(upperBg, BG_MOVE_SET_Y, offset);
            }
            if (kb->slideFrame++ > slideFrames) {
                if (kb->hasUpperLayer) {
                    GFL_BGSysSetBGEnabled(keyBg, FALSE);
                    GFL_BGSysSetBGEnabled(upperBg, FALSE);
                    GFL_BGSysSetBGEnabled(labelBg, FALSE);
                } else {
                    GFL_BGSysSetBGEnabled(keyBg, FALSE);
                    GFL_BGSysSetBGEnabled(labelBg, FALSE);
                }
                kb->slideFrame = 0;
                kb->slideStage = 1;
            }
            break;
        case 1:
            offset = -48;
            GFL_BGSysMoveBG(keyBg, BG_MOVE_SET_Y, offset);
            GFL_BGSysMoveBG(labelBg, BG_MOVE_SET_Y, offset);
            if (kb->hasUpperLayer) {
                GFL_BGSysMoveBG(upperBg, BG_MOVE_SET_Y, offset);
            }
            func_ov194_021f35a8(kb, kb->nextType);
            kb->slideStage = 2;
            break;
        case 2:
            kb->slideFrame = slideFrames;
            kb->slideStage = 3;
            GFL_BGSysSetBGEnabled(keyBg, TRUE);
            GFL_BGSysSetBGEnabled(labelBg, TRUE);
            if (kb->type != 3) {
                GFL_BGSysSetBGEnabled(upperBg, TRUE);
            }
            reg_G2_BLDALPHA = 1 << 12;
            break;
        case 3:
            frame = kb->slideFrame;
            alpha = -(frame * 16) / slideFrames + 16;
            reg_G2_BLDALPHA = alpha | (1 << 12);
            offset = -48 * frame / slideFrames;
            GFL_BGSysMoveBG(keyBg, BG_MOVE_SET_Y, offset);
            GFL_BGSysMoveBG(labelBg, BG_MOVE_SET_Y, offset);
            if (kb->hasUpperLayer) {
                GFL_BGSysMoveBG(upperBg, BG_MOVE_SET_Y, offset);
            }
            if (kb->slideFrame-- == 0) {
                kb->slideStage = 4;
            }
            break;
        case 4:
            GFL_BGSysMoveBG(keyBg, BG_MOVE_SET_Y, 0);
            GFL_BGSysMoveBG(labelBg, BG_MOVE_SET_Y, 0);
            GFL_BGSysMoveBG(upperBg, BG_MOVE_SET_Y, 0);
            kb->hasUpperLayer = FALSE;
            reg_G2_BLDCNT = 0;
            kb->sliding = FALSE;
            return TRUE;
        }
    }
    return FALSE;
}

void func_ov194_021f35a8(NameinKeyView *kb, s32 type) {
    if (kb->type != type) {
        func_ov194_021f2608(&kb->layout);
        func_ov194_021f256c(&kb->layout, type, func_ov194_021f3358(type, kb->kanaPage, kb->kanaGroup), kb->keyboards,
                            kb->heapId);
        GFL_BitmapFill(BmpWin_GetBitmap(kb->window), 0);
        func_ov194_021f26f0(&kb->layout, &kb->print, kb->queue, kb->font, kb->heapId);
        if (type == 3) {
            GFL_BGSysBufferScrDefault(1, kb->screens[0]->rawData, 0x800);
            GFL_BGSysSetBGEnabled(2, FALSE);
        } else {
            GFL_BGSysBufferScrDefault(1, kb->screens[1]->rawData, 0x800);
        }
        GFL_BGSysLoadScr(1);
        if (type == 3 || kb->type == 3) {
            func_ov194_021f2884(&kb->pos, kb->type, type);
        }
        kb->type = type;
    }
}

static inline BOOL NameinKeyView_IsKana(const NameinKeyView *kb) {
    return kb->type == 3;
}

void func_ov194_021f366c(NameinKeyView *kb, u32 page, u32 group) {
    if (NameinKeyView_IsKana(kb)) {
        if (kb->kanaGroup == group && kb->kanaPage == page) {
            return;
        }
        func_ov194_021f25f8(&kb->layout, func_ov194_021f3358(kb->type, page, group), kb->keyboards);
        GFL_BitmapFill(BmpWin_GetBitmap(kb->window), 0);
        func_ov194_021f26f0(&kb->layout, &kb->print, kb->queue, kb->font, kb->heapId);
        kb->kanaPage = page;
        kb->kanaGroup = group;
    }
}

BOOL func_ov194_021f36e8(NameinKeyView *kb, NameinKeyInput *input) {
    NameinKeyPos delta;
    NameinKeyRect rect;
    BOOL moved = FALSE;

    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        delta.x = 0;
        delta.y = -1;
        moved = TRUE;
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        delta.x = 0;
        delta.y = 1;
        moved = TRUE;
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
        delta.x = -1;
        delta.y = 0;
        moved = TRUE;
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
        delta.x = 1;
        delta.y = 0;
        moved = TRUE;
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_START) {
        delta.x = 0;
        delta.y = 0;
        moved = TRUE;
        func_ov194_021f2828(&kb->layout, 7, &kb->pos);
        func_ov194_021f26e0(&kb->layout, &kb->pos, &delta);
    }
    if (moved) {
        if (func_020355c4() == TRUE) {
            if (func_ov194_021f2818(&kb->layout, &kb->pos, &rect)) {
                func_020355d8(FALSE);
                func_ov194_021f1a1c(&kb->highlight, 1, &rect, kb->kanaPage, kb->type);
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
        } else {
            func_ov194_021f26e0(&kb->layout, &kb->pos, &delta);
            if (func_ov194_021f2818(&kb->layout, &kb->pos, &rect)) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                func_ov194_021f1a1c(&kb->highlight, 1, &rect, kb->kanaPage, kb->type);
            }
        }
    }
    return FALSE;
}

BOOL func_ov194_021f37ec(NameinKeyView *kb, NameinKeyInput *input) {
    NameinKeyRect rect;
    BOOL acted = FALSE;

    if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A) {
        input->pos = kb->pos;
        acted = TRUE;
        input->moved = TRUE;
    }
    if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_B) {
        func_ov194_021f2828(&kb->layout, 6, &input->pos);
        input->button = 6;
        acted = TRUE;
    }
    if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_SELECT) {
        input->button = kb->type + 3;
        if (input->button > 5) {
            input->button = 2;
        }
        func_ov194_021f2828(&kb->layout, input->button, &kb->pos);
        input->pos = kb->pos;
        acted = TRUE;
    }
    if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_R) {
        kb->result = 10;
        acted = FALSE;
    }
    if (acted && func_020355c4() == TRUE) {
        if (func_ov194_021f2818(&kb->layout, &kb->pos, &rect)) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_020355d8(FALSE);
            func_ov194_021f1a1c(&kb->highlight, 1, &rect, kb->kanaPage, kb->type);
            return FALSE;
        }
    }
    return acted;
}

BOOL func_ov194_021f265c(const NameinKeyView *kb, NameinTouchPos *touch, NameinKeyPos *found);
BOOL func_ov194_021f38cc(NameinKeyView *kb, NameinKeyInput *input) {
    NameinTouchPos touch;
    u32 touchX;
    u32 touchY;
    s32 x;
    s32 y;

    if (func_02035d20(&touchX, &touchY)) {
        touch.x = touchX;
        touch.y = touchY;
        if (func_ov194_021f265c(kb, &touch, &kb->pos)) {
            x = kb->pos.x;
            y = kb->pos.y;
            input->pos.y = y;
            input->moved = TRUE;
            input->pos.x = x;
            func_020355d8(TRUE);
            return TRUE;
        }
    }
    return FALSE;
}

void func_ov194_021f391c(NameinKeyView *kb, NameinKeyInput *input) {
    NameinKeyRect rect;
    int kind;

    if (input->moved) {
        kind = func_ov194_021f2614(&kb->layout, &kb->pos, &kb->lastChar);
    } else {
        kind = input->button;
    }
    switch (kind) {
    case 1:
        kb->result = 1;
        kb->state = 1;
        break;
    case 9:
        kb->result = 9;
        kb->state = 1;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        if (func_ov194_021f3364(kb, kind - 2)) {
            GFL_SndSEPlay(SEQ_SE_SYS_12);
            kb->state = 2;
            kb->result = 6;
        }
        break;
    case 6:
        kb->result = 5;
        break;
    case 7:
        kb->result = 7;
        break;
    case 8:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        kb->result = 8;
        func_ov194_021f366c(kb, kb->kanaPage ^ 1, kb->kanaGroup);
        break;
    case 10:
        kb->result = 3;
        break;
    case 11:
        kb->result = 4;
        break;
    }
    if (func_ov194_021f2818(&kb->layout, &input->pos, &rect)) {
        func_ov194_021f1a1c(&kb->highlight, 2, &rect, kb->kanaPage, kb->type);
    }
}

BOOL func_ov194_021f3a0c(NameinKeyView *kb, NameinKeyInput *input) {
    NameinKeyRect rect;
    NameinKeyPos delta;
    NameinKeyPos pos;
    int kind;
    BOOL moved = FALSE;

    if (!kb->hasUpperLayer) {
        if (GCTX_HIDGetPressedKeys() & PAD_KEY_UP) {
            delta.x = 0;
            delta.y = -1;
            moved = TRUE;
        } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_DOWN) {
            delta.x = 0;
            delta.y = 1;
            moved = TRUE;
        } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_LEFT) {
            delta.x = -1;
            delta.y = 0;
            moved = TRUE;
        } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_RIGHT) {
            delta.x = 1;
            delta.y = 0;
            moved = TRUE;
        }
    }
    if (moved) {
        pos = kb->pos;
        func_ov194_021f26e0(&kb->layout, &pos, &delta);
        kind = func_ov194_021f2614(&kb->layout, &pos, NULL);
        if (kind == 2 || kind == 3 || kind == 4 || kind == 5) {
            if (func_020355c4() == TRUE) {
                if (func_ov194_021f2818(&kb->layout, &kb->pos, &rect)) {
                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                    func_020355d8(FALSE);
                    func_ov194_021f1a1c(&kb->highlight, 1, &rect, kb->kanaPage, kb->type);
                }
            } else {
                func_ov194_021f26e0(&kb->layout, &kb->pos, &delta);
                if (func_ov194_021f2818(&kb->layout, &kb->pos, &rect)) {
                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                    func_ov194_021f1a1c(&kb->highlight, 1, &rect, kb->kanaPage, kb->type);
                }
            }
        }
    }
    return FALSE;
}

BOOL func_ov194_021f3b18(NameinKeyView *kb, NameinKeyInput *input) {
    NameinKeyRect rect;
    BOOL acted = FALSE;

    if (!kb->hasUpperLayer) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            input->pos = kb->pos;
            acted = TRUE;
            input->moved = TRUE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
            input->button = kb->type + 3;
            if (input->button > 5) {
                input->button = 2;
            }
            func_ov194_021f2828(&kb->layout, input->button, &kb->pos);
            input->pos = kb->pos;
            acted = TRUE;
        }
        if (acted && func_020355c4() == TRUE) {
            if (func_ov194_021f2818(&kb->layout, &kb->pos, &rect)) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                func_020355d8(FALSE);
                func_ov194_021f1a1c(&kb->highlight, 1, &rect, kb->kanaPage, kb->type);
                return FALSE;
            }
        }
    }
    return acted;
}

BOOL func_ov194_021f3bd4(NameinKeyView *kb, NameinKeyInput *input) {
    NameinTouchPos touch;
    u32 touchX;
    u32 touchY;
    s32 x;
    s32 y;
    s32 kind;

    if (!kb->hasUpperLayer) {
        if (func_02035d20(&touchX, &touchY)) {
            touch.x = touchX;
            touch.y = touchY;
            if (func_ov194_021f265c(kb, &touch, &kb->pos)) {
                kind = func_ov194_021f2614(&kb->layout, &kb->pos, NULL);
                if (kind == 2 || kind == 3 || kind == 4 || kind == 5) {
                    x = kb->pos.x;
                    y = kb->pos.y;
                    input->pos.y = y;
                    input->moved = TRUE;
                    input->pos.x = x;
                    func_020355d8(TRUE);
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

void func_ov194_021f3c40(NameinKeyView *kb, NameinKeyInput *input) {
    NameinKeyRect rect;
    s32 kind;

    if (input->moved) {
        kind = func_ov194_021f2614(&kb->layout, &kb->pos, &kb->lastChar);
    } else {
        kind = input->button;
    }
    switch (kind) {
    case 2:
    case 3:
    case 4:
    case 5:
        if (func_ov194_021f3364(kb, kind - 2)) {
            GFL_SndSEPlay(0x64b);
            kb->state = 2;
            kb->result = 6;
        }
        break;
    }
    if (func_ov194_021f2818(&kb->layout, &input->pos, &rect)) {
        func_ov194_021f1a1c(&kb->highlight, 2, &rect, kb->kanaPage, kb->type);
    }
}

void func_ov194_021f3cd0(NameinMessage *win, Font *font, MsgData *msgData, PrintQueue *queue, WordSet *wordSet, HeapID heapId) {
    BmpWin *window;

    MI_CpuFill8(win, 0, sizeof(NameinMessage));
    win->color = 15;
    win->font = font;
    win->wordSet = wordSet;
    win->msgData = msgData;
    win->printQueue = queue;
    win->strbuf = GFL_StrBufCreate(255, heapId);
    win->window = BmpWin_CreateDynamic(4, 1, 19, 30, 4, 15, TRUE);
    BmpWin_DrawFrame(win->window, 2, 1, 14);
    win->printWindow = win->window;
    win->printing = FALSE;
    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    window = win->window;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
}

void func_ov194_021f3d54(NameinMessage *message) {
    BmpWin_Free(message->window);
    GFL_StrBufFree(message->strbuf);
    MI_CpuFill8(message, 0, sizeof(NameinMessage));
}

void func_ov194_021f3d70(NameinMessage *win, u32 messageId) {
    PrintQueue *queue;

    while (func_0201c6c4(win->printQueue, BmpWin_GetBitmap(win->window))) {
        func_0201c4e4(win->printQueue);
    }
    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    GFL_MsgDataLoadStrbuf(win->msgData, messageId, win->strbuf);
    queue = win->printQueue;
    func_0201c6fc(queue, BmpWin_GetBitmap(win->printWindow), 0, 0, win->strbuf, win->font);
    win->printing = TRUE;
}

void func_ov194_021f3de0(NameinMessage *win, u32 messageId, u16 species, u16 form, HeapID heapId) {
    PartyPkm *pkm = PokeParty_NewTempPkm(species, 1, 0, heapId);

    PokeParty_SetParam(pkm, PKM_PARAM_FORM, form);
    func_ov194_021f3e18(win, messageId, pkm);
    GFL_HeapFree(pkm);
}

void func_ov194_021f3e18(NameinMessage *win, u32 messageId, PartyPkm *pkm) {
    StrBuf *format;
    PrintQueue *queue;

    setPartyPokemonSpeciesNameToStrbuf(win->wordSet, 0, pkm);
    GFL_BitmapFill(BmpWin_GetBitmap(win->window), win->color);
    format = GFL_MsgDataLoadStrbufNew(win->msgData, messageId);
    GFL_WordSetFormatStrbuf(win->wordSet, win->strbuf, format);
    queue = win->printQueue;
    func_0201c6fc(queue, BmpWin_GetBitmap(win->printWindow), 0, 0, win->strbuf, win->font);
    win->printing = TRUE;
    GFL_StrBufFree(format);
}

// pokebw2's NameEntryMessage_IsPrintDone
BOOL func_ov194_021f3e78(NameinMessage *win) {
    PrintQueue *printQueue = win->printQueue;

    if (win->printing) {
        if (!func_0201c6c4(printQueue, BmpWin_GetBitmap(win->printWindow))) {
            BmpWin_FlushChar(win->printWindow);
            win->printing = FALSE;
        }
    }
    if (!win->printing) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's NameEntryTalk_Init
void func_ov194_021f3eb0(NameinTalk *msg, HeapID heapId) {
    MI_CpuFill8(msg, 0, sizeof(NameinTalk));
    msg->tcbManager = GFL_TCBExMgrCreate(heapId, heapId, 1, 0);
}

// pokebw2's NameEntryTalk_Free
void func_ov194_021f3ed0(NameinTalk *msg) {
    if (msg->stream != NULL) {
        func_0201ce48(msg->stream);
    }
    GFL_TCBExMgrFree(msg->tcbManager);
    MI_CpuFill8(msg, 0, sizeof(NameinTalk));
}

void func_ov194_021f3ef0(NameinTalk *talk, NameinMessage *message, NameEntryParam *param, HeapID heapId) {
    WordSet *wordSet = GFL_WordSetSystemCreateDefault(heapId);
    StrBuf *name;

    if (func_ov010_0216c0e4(param)) {
        loadPokemonNicknameToStrbuf(wordSet, 0, param->pkm);
    } else {
        name = GFL_StrBufClone(param->name, heapId);
        func_ov010_0216c0d4(param, name);
        func_0201ee0c(wordSet, 0, name, 0, 0, 0);
        GFL_StrBufFree(name);
    }
    loadBoxNameForDisplay(wordSet, 1, param->unk10, param->unk14);
    if (talk->stream != NULL) {
        func_0201ce48(talk->stream);
    }
    GFL_BitmapFill(BmpWin_GetBitmap(message->window), message->color);
    GFL_StrBufClear(message->strbuf);
    GFL_WordSetFormatStrbuf(wordSet, message->strbuf, param->unkC);
    talk->stream = func_0201cce4(message->window, 0, 0, message->strbuf, message->font, func_02012ffc(),
                                 talk->tcbManager, 2, heapId, message->color);
    talk->timer = 0;
    talk->next = func_ov194_021f46b4;
    talk->wait = 60;
    GFL_WordSetSystemFree(wordSet);
}

void func_ov194_021f3fbc(NameinIcon *icon, u32 kind, unsigned long index, u32 param, ClActUnit *unit, Font *font,
                         PrintQueue *printQueue, MsgData *msgData, BmpOamSys *bmpOamSys, HeapID heapId) {
    BmpOamActorSetup bmpSetup;
    ClActorSetup setup;
    BOOL bitmapNeeded;
    u32 arcId;
    u32 charFile;
    u32 paletteFile;
    u32 cellFile;
    u32 animFile;
    u16 start;
    ArcTool *arc;
    u32 setupX;
    u32 setupY;
    u32 x;
    BOOL singleActor;
    u32 msgId;
    u16 color;

    bitmapNeeded = FALSE;
    setupX = 0x22;
    setupY = 0x14;
    singleActor = FALSE;
    MI_CpuFill8(icon, 0, sizeof(NameinIcon));
    start = 0;
    switch (kind) {
    case 0:
    case 1:
        arcId = getUINarcIdx();
        charFile = func_020276a4();
        paletteFile = func_020276a8();
        cellFile = func_020276ac(2);
        animFile = func_020276b0(2);
        x = index;
        break;
    case 2:
        arcId = 0x20;
        charFile = 0;
        start = data_ov194_021f5569[index];
        paletteFile = index;
        paletteFile += 0x31;
        cellFile = 0x41;
        animFile = 0x42;
        x = index >= 8 ? 1 : 0;
        break;
    case 3:
        bitmapNeeded = TRUE;
        x = (param & 0xff00) >> 8;
        if (x == 2) {
            bitmapNeeded = FALSE;
        }
        arcId = 7;
        charFile = func_0201bc3c();
        paletteFile = PokeParty_GetIconIndex(index, param & 0xff, x, 0);
        cellFile = func_0201bc40();
        animFile = getOBJTileMapping_MainEng();
        setupX = 0x1e;
        setupY = 0x14;
        singleActor = TRUE;
        break;
    case 4:
    case 5:
        arcId = 0x76;
        charFile = 9;
        paletteFile = 8;
        cellFile = 7;
        animFile = 6;
        start = 1;
        break;
    }
    arc = GFL_ArcSysCreateFileHandle(arcId, heapId);
    if (singleActor) {
        icon->palette = func_0204acec(arc, charFile, 0, 0x40, heapId);
    } else {
        icon->palette = func_0204ac18(arc, charFile, 0, 0x40, start, 1, heapId);
    }
    icon->cellAnims = func_0204af28(arc, cellFile, animFile, heapId);
    icon->chars = func_0204a6c8(arc, paletteFile, 0, 0, heapId);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = setupX;
    setup.y = setupY;
    icon->actor = func_0204b294(unit, icon->chars, icon->palette, icon->cellAnims, &setup, 0, heapId);
    func_0204b92c(icon->actor, 1);
    switch (kind) {
    case 0:
        if (index == 0) {
            func_ov194_021f51e4(icon->actor, 0x31, 6, 3, 4, 4, 0, 0, heapId);
        } else {
            func_ov194_021f51e4(icon->actor, 0x31, 9, 3, 4, 4, 0, 0, heapId);
        }
        break;
    case 1:
        func_ov194_021f51e4(icon->actor, 0x31, 6, 3, 4, 4, 0, 0, heapId);
    case 2:
        func_0204b6f4(icon->actor, 0, 1);
        break;
    case 3:
        func_0204b878(icon->actor, 1);
        func_0204b6f4(icon->actor, func_0201bb5c(index, param & 0xff, (param & 0xff00) >> 8, 0), 0);
        break;
    case 5:
        func_0204b878(icon->actor, 5);
        func_0204b6f4(icon->actor, 0, 1);
        break;
    case 4:
        func_0204b878(icon->actor, 2);
        func_0204b6f4(icon->actor, 0, 1);
        break;
    }
    if (bitmapNeeded) {
        arc = GFL_ArcSysCreateFileHandle(0x17, heapId);
        icon->labelBitmap = GFL_BitmapCreate(2, 2, 0x20, heapId);
        icon->labelPalette = func_0204acec(arc, 5, 0, 0xa0, heapId);
        GFL_ArcToolFree(arc);
        MI_CpuFill8(&bmpSetup, 0, sizeof(BmpOamActorSetup));
        bmpSetup.bitmap = icon->labelBitmap;
        bmpSetup.x = 0x2e;
        bmpSetup.y = 0x14;
        bmpSetup.palette = icon->labelPalette;
        bmpSetup.priority = 0;
        bmpSetup.surface = 0;
        bmpSetup.vramType = 0;
        bmpSetup.bgPriority = 0;
        bmpSetup.paletteOffset = 0;
        icon->labelActor = func_020255a0(bmpOamSys, &bmpSetup);
        if (x == 1) {
            msgId = 0x11;
            color = PRINT_COLOR(3, 4, 0);
        } else {
            msgId = 0x10;
            color = PRINT_COLOR(5, 6, 0);
        }
        icon->label = GFL_MsgDataLoadStrbufNew(msgData, msgId);
        GFL_BitmapFill(icon->labelBitmap, 0);
        func_0201c724(printQueue, icon->labelBitmap, 0, 0, icon->label, font, color);
        icon->labelQueue = printQueue;
        icon->labelPrinting = TRUE;
    }
}

void func_ov194_021f42a0(NameinIcon *icon) {
    if (icon->labelActor != NULL) {
        func_0204ada4(icon->labelPalette);
        GFL_StrBufFree(icon->label);
        func_0202570c(icon->labelActor);
        GFL_BitmapFree(icon->labelBitmap);
    }
    func_0204ada4(icon->palette);
    func_0204a8d4(icon->chars);
    func_0204afd8(icon->cellAnims);
    func_0204b3b4(icon->actor);
    MI_CpuFill8(icon, 0, sizeof(NameinIcon));
}

void func_ov194_021f42e8(NameinIcon *icon) {
    u32 x;
    u32 y;
    ClActorPos pos;
    NameinTouchPos touch;
    NameinKeyRect rect;

    switch (icon->state) {
    case 0:
        func_0204b454(icon->actor, &pos, 0);
        icon->touched = FALSE;
        if (func_02035d20(&x, &y)) {
            rect.left = pos.x - 0x10;
            rect.top = pos.y - 0x10;
            rect.right = pos.x + 0x10;
            rect.bottom = pos.y + 0x10;
            touch.x = x;
            touch.y = y;
            if (func_ov194_021f4450(&rect, &touch)) {
                func_ov194_021f4394(icon);
                icon->state = 1;
            }
        }
        break;
    case 1:
        if (func_ov194_021f43a0(icon)) {
            icon->touched = TRUE;
            icon->state = 0;
        }
        break;
    }
}

void func_ov194_021f436c(NameinIcon *icon) {
    if (icon->labelPrinting) {
        if (!func_0201c6c4(icon->labelQueue, icon->labelBitmap)) {
            func_020257d0(icon->labelActor);
            icon->labelPrinting = FALSE;
        }
    }
}

BOOL func_ov194_021f4390(NameinIcon *icon) {
    return icon->touched;
}

void func_ov194_021f4394(NameinIcon *icon) {
    icon->frame = 0;
    icon->bouncing = TRUE;
}

BOOL func_ov194_021f43a0(NameinIcon *icon) {
    if (icon->bouncing) {
        ClActorPos pos;
        u16 timer = icon->frame;

        icon->frame++;
        func_0204b454(icon->actor, &pos, 0);
        pos.y = 0x14 - ((FX_SinIdx((u16)((timer * 0x7fff) / 8)) * 0xc) >> 12);
        func_0204b404(icon->actor, &pos, 0);
        if (icon->frame > 8) {
            icon->bouncing = FALSE;
        }
        return FALSE;
    }
    return TRUE;
}

u32 func_ov194_021f4408(u32 type, u32 value) {
    switch (type) {
    case 0:
    case 5:
    case 6:
    case 7:
        return 0;
    case 1:
        return 3;
    case 2:
        return 4;
    case 3:
        return 1;
    case 4:
        if (value == 0xffff) {
            return 5;
        }
        return 2;
    default:
        return 0;
    }
}

BOOL func_ov194_021f4450(const NameinKeyRect *rect, const NameinTouchPos *point) {
    return ((u32)(point->x - rect->left) <= (u32)(rect->right - rect->left)) &
           ((u32)(point->y - rect->top) <= (u32)(rect->bottom - rect->top));
}

StrBuf *func_ov194_021f4480(NameinWork *work, u32 mode, HeapID heapId) {
    switch (mode) {
    case 0:
        return NULL;
    case 1:
        if (GFL_StrBufGetCharCount(work->name) == 0) {
            return GFL_MsgDataLoadStrbufNew(g_PMLSpeciesNamesResident, work->param->gender);
        }
        return GFL_StrBufClone(work->name, heapId);
    case 2:
        return GFL_StrBufClone(work->name, heapId);
    case 3:
        return NULL;
    case 4:
        return GFL_StrBufClone(work->name, heapId);
    case 5:
        return GFL_StrBufClone(work->name, heapId);
    case 6:
        return GFL_StrBufClone(work->name, heapId);
    case 7:
        return GFL_StrBufClone(work->name, heapId);
    default:
        return NULL;
    }
}

void func_ov194_021f4524(NameinWork *work) {
    StrBuf *copy;

    work->param->unk1C = 0;
    copy = GFL_StrBufClone(work->param->name, 0x1e);
    func_ov194_021f2328(&work->box, work->param->name);
    if (GFL_StrBufCmp(copy, work->param->name)) {
        work->param->unk1C = 1;
    }
    if (!func_ov194_021f253c(&work->box)) {
        GFL_StrBufCopy(work->param->name, copy);
        work->param->unk1C = 1;
    }
    GFL_StrBufFree(copy);
    if (GFL_StrBufGetCharCount(work->param->name) == 0) {
        if (work->param->mode != 0 && work->param->mode != 3) {
            work->param->unk1C = 1;
        }
    }
}

u32 func_ov194_021f45a8(NameinWork *work) {
    u32 mode = work->param->mode;

    if (mode == 0 || mode == 3) {
        if (func_ov194_021f23f4(&work->box) == 0 && func_ov194_021f23f8(&work->box) == 0) {
            return FALSE;
        }
        if (!func_ov194_021f253c(&work->box)) {
            return FALSE;
        }
    }
    return TRUE;
}

void func_ov194_021f45ec(NameinTask *task, NameinWork *work, NameinTaskFunc func) {
    MI_CpuFill8(task, 0, sizeof(NameinTask));
    task->work = work;
    func_ov194_021f462c(task, func);
}

void func_ov194_021f4608(NameinTask *task) {
    MI_CpuFill8(task, 0, sizeof(NameinTask));
}

void func_ov194_021f4614(NameinTask *task) {
    if (!task->done) {
        task->func(task, &task->state, task->work);
    }
}

BOOL func_ov194_021f4628(NameinTask *task) {
    return task->done;
}

void func_ov194_021f462c(NameinTask *task, NameinTaskFunc func) {
    task->func = func;
    task->state = 0;
}

void func_ov194_021f4634(NameinTask *task) {
    task->done = TRUE;
}

void func_ov194_021f463c(NameinTask *task, int *state, NameinWork *work) {
    BOOL ready = func_ov194_021f3304(&work->keyboard);

    ready |= func_ov194_021f22e8(&work->box);
    if (func_ov194_021f3e78(&work->message) | ready) {
        func_ov194_021f462c(task, func_ov194_021f4674);
    }
}

void func_ov194_021f4674(NameinTask *task, int *state, NameinWork *work) {
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
        func_ov194_021f462c(task, func_ov194_021f47b4);
        break;
    }
}

void func_ov194_021f46b4(NameinTask *task, int *state, NameinWork *work) {
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
        func_ov194_021f462c(task, func_ov194_021f4a5c);
        break;
    }
}

void func_ov194_021f46f4(NameinTask *task, int *state, NameinWork *work) {
    NameinTalk *talk = &work->talk;

    switch (*state) {
    case 0:
        GFL_TCBExMgrUpdate(talk->tcbManager);
        switch (func_0201ce30(talk->stream)) {
        case PRINT_STREAM_RUNNING:
            if ((GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c38()) {
                func_0201ce5c(talk->stream, 0);
                if (GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
                    func_020355d8(FALSE);
                } else {
                    func_020355d8(TRUE);
                }
            }
            break;
        case PRINT_STREAM_DONE:
            *state = 1;
            break;
        case PRINT_STREAM_PAUSED:
            if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60()) {
                func_0201ce38(talk->stream);
                if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
                    func_020355d8(FALSE);
                } else {
                    func_020355d8(TRUE);
                }
            }
            break;
        }
        break;
    case 1:
        if (talk->timer >= talk->wait) {
            *state = 2;
        } else {
            talk->timer++;
        }
        break;
    case 2:
        func_ov194_021f462c(task, talk->next);
        break;
    }
}

void func_ov194_021f47b4(NameinTask *task, int *state, NameinWork *work) {
    u16 c;
    int result;
    u32 page;
    BOOL success;
    StrBuf *name;
    NameEntryParam *param;

    result = func_ov194_021f32d8(&work->keyboard, &c);
    page = func_ov194_021f32fc(&work->keyboard);
    if (work->messagePending == TRUE) {
        if (result != 0 || func_ov194_021f4390(&work->icon) || (GCTX_HIDGetPressedKeys() & ~PAD_BUTTON_A) != 0) {
            param = work->param;
            if (param->mode == NAME_ENTRY_POKEMON) {
                if (param->pkm == NULL) {
                    func_ov194_021f3de0(&work->message, param->mode, param->gender, param->unk6, 0x1e);
                } else {
                    func_ov194_021f3e18(&work->message, param->mode, param->pkm);
                }
            } else {
                func_ov194_021f3d70(&work->message, param->mode);
            }
            work->messagePending = FALSE;
        }
    }

    switch (result) {
    case 1:
        if (func_ov194_021f2190(&work->box, c)) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
        break;
    case 2:
        if (func_ov194_021f21bc(&work->box, c, page)) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
        break;
    case 3:
        success = TRUE;
        if (!func_ov194_021f21fc(&work->box, 0)) {
            success = FALSE;
        }
        if (success) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
        break;
    case 4:
        success = TRUE;
        if (!func_ov194_021f21fc(&work->box, 1)) {
            success = FALSE;
        }
        if (success) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
        break;
    case 10:
        if (func_ov194_021f22ac(&work->box)) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
        break;
    case 5:
        if (func_ov194_021f2150(&work->box)) {
            GFL_SndSEPlay(SEQ_SE_CANCEL3);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
        break;
    case 6:
        func_ov194_021f233c(&work->box);
        break;
    case 11:
        func_ov194_021f235c(&work->box);
        break;
    case 7:
        if (func_ov194_021f23f8(&work->box) == 0) {
            if (!func_ov194_021f45a8(work)) {
                GFL_SndSEPlay(SEQ_SE_BEEP);
                func_ov194_021f3d70(&work->message, 8);
                work->messagePending = TRUE;
                break;
            }
            if (func_ov194_021f5370(work->profWord, work->box.text, 0x1e)) {
                GFL_SndSEPlay(SEQ_SE_BEEP);
                break;
            }
            if (func_ov194_021f54cc(work->box.text, work->box.length, 0x1e)) {
                GFL_SndSEPlay(SEQ_SE_BEEP);
                func_ov194_021f3d70(&work->message, 9);
                work->messagePending = TRUE;
                break;
            }
            GFL_SndSEPlay(SEQ_SE_DECIDE2);
            func_ov194_021f4524(work);
            param = work->param;
            if (param->mode == NAME_ENTRY_POKEMON && param->unkC != NULL) {
                func_ov194_021f3ef0(&work->talk, &work->message, param, 0x1e);
                func_ov194_021f462c(task, func_ov194_021f46f4);
            } else {
                func_ov194_021f462c(task, func_ov194_021f46b4);
            }
        } else {
            func_ov194_021f235c(&work->box);
            GFL_SndSEPlay(SEQ_SE_DECIDE2);
        }
        break;
    case 0:
        if (func_ov194_021f4390(&work->icon)) {
            name = func_ov194_021f4480(work, work->param->mode, 0x1e);
            func_ov194_021f2394(&work->box);
            if (name != NULL) {
                func_ov194_021f23b8(&work->box, GFL_StrBufGetStringPtr(name));
                GFL_StrBufFree(name);
            }
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
        break;
    case 9:
        func_ov194_021f235c(&work->box);
        if (func_ov194_021f2190(&work->box, c)) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        } else {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
        break;
    }

    func_ov194_021f31f4(&work->keyboard, &work->box);
    func_ov194_021f1fb4(&work->box);
    func_ov194_021f42e8(&work->icon);
    func_ov194_021f3e78(&work->message);
}

void func_ov194_021f4a5c(NameinTask *task, int *state, NameinWork *work) {
    if (work->param->gameInfo != NULL) {
        u32 type = func_ov194_021f3350(&work->keyboard);

        func_0200c0fc(work->param->gameInfo, work->param->mode, type);
    }
    if (work->param->saveTask != NULL) {
        func_ov194_021f462c(task, func_ov194_021f4aa8);
        return;
    }
    func_ov194_021f4634(task);
}

void func_ov194_021f4aa8(NameinTask *task, int *state, NameinWork *work) {
    switch (*state) {
    case 0:
        func_ov088_021bd368(work->param->saveTask);
        *state = 1;
        break;
    case 1:
        if (func_ov088_021bd380(work->param->saveTask) == TRUE) {
            func_ov194_021f4634(task);
        }
        break;
    }
}
