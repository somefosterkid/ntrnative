// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov296/zukan_info.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/zukan_info.h"
#include "types.h"
#include "app/ui/print_msg.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "constants/types.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/g2d.h"
#include "nnsys/gfd.h"
#include "pml/personal.h"
#include "pml/poke_graphic.h"
#include "pml/poke_party.h"
#include "pml/species_names.h"
#include "system/app_menu_common.h"
#include "system/bmp_winframe.h"
#include "system/printsys.h"
#include "system/str_tool.h"
#include "system/wordset.h"
#include "text/system/btl_server_flow_title.h"

// zukan_info.c by the ROM's own string: a Pokémon's Pokédex entry on one screen. Its sprite, its types, its footprint,
// and in windows its name, number, category, height, weight and description, in the game's language or in one of the
// six others that the Pokédex has seen it in. The sprite, types and footprint are loaded into a slot of two, so that
// showing another Pokémon swaps slots instead of waiting for the old one to be freed

// The Pokédex's graphics (ARCID_ZUKAN_GRA) members
#define ZUKAN_GRA_PALETTE 9
#define ZUKAN_GRA_PALETTE_EXTRA 2
#define ZUKAN_GRA_CHARS 0x19
#define ZUKAN_GRA_CHARS_EXTRA 0xc
#define ZUKAN_GRA_SCREEN_MID 0x28
#define ZUKAN_GRA_SCREEN_BACK 0x1f
#define ZUKAN_GRA_SCREEN_EXTRA 0x24

// The archives it reads (our names): the table of the next form of each species and form, the footprints of each
// species, and of the forms that have a footprint of their own
#define ARCID_FORM_NAME_TABLE 98
#define ARCID_POKE_FOOTPRINT 167
#define ARCID_POKE_FOOTPRINT_FORM 233

enum {
    ZUKAN_INFO_MODE_MAIN,
    ZUKAN_INFO_MODE_SUB,
    ZUKAN_INFO_MODE_DETAIL,
};

enum {
    ZUKAN_INFO_STATE_WAIT_START,
    ZUKAN_INFO_STATE_DELAY,
    ZUKAN_INFO_STATE_OPEN_WINDOW,
    ZUKAN_INFO_STATE_WAIT_READY,
    ZUKAN_INFO_STATE_SLIDE_IN,
    ZUKAN_INFO_STATE_WAIT_CRY,
    ZUKAN_INFO_STATE_DONE,
    ZUKAN_INFO_STATE_STATIC,
};

enum {
    ZUKAN_INFO_ARC_ZUKAN_GRA,
    ZUKAN_INFO_ARC_FORM_NAME_TABLE,
    ZUKAN_INFO_ARC_FONT,
    ZUKAN_INFO_ARC_APP_MENU_COMMON,
    ZUKAN_INFO_ARC_POKEGRA,
    ZUKAN_INFO_ARC_FOOTPRINT,
    ZUKAN_INFO_ARC_FOOTPRINT_FORM,
    ZUKAN_INFO_ARC_COUNT,
};

// The windows that print the entry
enum {
    ZUKAN_INFO_WINDOW_LABEL,
    ZUKAN_INFO_WINDOW_NAME,
    ZUKAN_INFO_WINDOW_SIZE,
    ZUKAN_INFO_WINDOW_TEXT,
    ZUKAN_INFO_WINDOW_COUNT,
};

// The message files: the game's language, then the categories, heights, weights, entries and names of each language
enum {
    ZUKAN_INFO_MSG_LABELS,
    ZUKAN_INFO_MSG_CATEGORIES,
    ZUKAN_INFO_MSG_HEIGHTS,
    ZUKAN_INFO_MSG_WEIGHTS,
    ZUKAN_INFO_MSG_ENTRIES,
    ZUKAN_INFO_MSG_OTHER_CATEGORIES,
    ZUKAN_INFO_MSG_OTHER_HEIGHTS = ZUKAN_INFO_MSG_OTHER_CATEGORIES + 6,
    ZUKAN_INFO_MSG_OTHER_WEIGHTS = ZUKAN_INFO_MSG_OTHER_HEIGHTS + 6,
    ZUKAN_INFO_MSG_OTHER_ENTRIES = ZUKAN_INFO_MSG_OTHER_WEIGHTS + 6,
    ZUKAN_INFO_MSG_OTHER_NAMES = ZUKAN_INFO_MSG_OTHER_ENTRIES + 6,
    ZUKAN_INFO_MSG_COUNT = ZUKAN_INFO_MSG_OTHER_NAMES + 6,
};

#define ZUKAN_INFO_STRBUF_COUNT 16
// Japanese, French, German, Italian, Spanish and Korean. The game's own language, which has its own files, comes after
#define ZUKAN_INFO_LANGUAGE_COUNT 6
#define ZUKAN_INFO_LANGUAGE_GAME ZUKAN_INFO_LANGUAGE_COUNT

#define COLOR_LABEL PRINT_COLOR(1, 2, 0)
#define COLOR_TEXT PRINT_COLOR(15, 2, 0)

struct ZukanInfo {
    /* 0x000 */ HeapID heapId;
    /* 0x004 */ BOOL national;
    /* 0x008 */ BOOL caught;
    /* 0x00c */ u16 species;
    /* 0x00e */ u16 form;
    /* 0x010 */ u16 sex;
    /* 0x012 */ u16 rare;
    /* 0x014 */ u32 personality;
    /* 0x018 */ u32 mode;
    /* 0x01c */ u32 engine;
    /* 0x020 */ u8 bgPriority;
    /* 0x024 */ ClActUnit *unit;
    /* 0x028 */ Font *font;
    /* 0x02c */ PrintQueue *printQueue;
    /* 0x030 */ u32 midChars;
    /* 0x034 */ u32 backChars;
    /* 0x038 */ u8 unk38;
    /* 0x039 */ u8 scrollRequest;
    /* 0x03c */ u32 extraChars;
    /* 0x040 */ BOOL hasExtraChars;
    /* 0x044 */ BOOL hidden;
    /* 0x048 */ u32 palType;
    /* 0x04c */ u8 frontBG;
    /* 0x04d */ u8 midBG;
    /* 0x04e */ u8 backBG;
    /* 0x050 */ TCB *tcb;
    /* 0x054 */ u32 state;
    /* 0x058 */ BOOL ready;
    /* 0x05c */ BOOL cryRequested;
    /* 0x060 */ u32 delay;
    /* 0x064 */ u32 tick;
    /* 0x068 */ int windowEdge;
    /* 0x06c */ BmpWin *windows[ZUKAN_INFO_WINDOW_COUNT];
    /* 0x07c */ BOOL flushed[ZUKAN_INFO_WINDOW_COUNT];
    /* 0x08c */ BmpWin *labelWindows[2];
    /* 0x094 */ u32 chars[2];
    /* 0x09c */ u32 palettes[2];
    /* 0x0a4 */ u32 cellAnims[2];
    /* 0x0ac */ ClActor *sprites[2];
    /* 0x0b4 */ u32 spriteSlot;
    /* 0x0b8 */ u32 typeChars[2][2];
    /* 0x0c8 */ u32 typePalette;
    /* 0x0cc */ u32 typeCellAnims;
    /* 0x0d0 */ ClActor *typeActors[2][2];
    /* 0x0e0 */ u32 typeSlot;
    /* 0x0e4 */ BOOL allTypesLoaded;
    /* 0x0e8 */ u32 allTypeChars[TYPE_NULL];
    /* 0x12c */ ClActor *allTypeActors[TYPE_NULL];
    /* 0x170 */ u8 shownTypes[2];
    /* 0x174 */ u32 footprintChars[2];
    /* 0x17c */ u32 footprintPalette;
    /* 0x180 */ u32 footprintCellAnimsOld;
    /* 0x184 */ u32 footprintCellAnims;
    /* 0x188 */ ClActor *footprints[2];
    /* 0x190 */ u32 footprintSlot;
    /* 0x194 */ u16 glowPhase;
    /* 0x196 */ u16 glowColors[5];
    /* 0x1a0 */ u16 glowColorsA[5];
    /* 0x1aa */ u16 glowColorsB[5];
    /* 0x1b4 */ int scrollY;
    /* 0x1b8 */ MsgData *givenMsgs[3];
    /* 0x1c4 */ ArcTool *arcs[ZUKAN_INFO_ARC_COUNT];
    /* 0x1e0 */ MsgData *msgs[ZUKAN_INFO_MSG_COUNT];
    /* 0x26c */ void *personal;
    /* 0x270 */ StrBuf *labels[ZUKAN_INFO_STRBUF_COUNT];
    /* 0x2b0 */ u16 *formNameTable;
};

static void func_ov206_021ba4f4(ZukanInfo *info);
static void func_ov206_021ba864(TCB *tcb, void *data);
static void func_ov206_021ba890(ZukanInfo *info);
static void func_ov206_021ba934(ZukanInfo *info);
static void func_ov206_021ba960(HeapID heapId, BmpWin *window, PrintQueue *printQueue, Font *font, StrBuf *strbuf,
                                  u16 x, u16 y, u16 color, u32 align, WordSet *wordSet);
static void func_ov206_021baa08(HeapID heapId, BmpWin *window, MsgData *msgData, PrintQueue *printQueue, Font *font,
                                   u32 messageId, u16 x, u16 y, u16 color, u32 align, WordSet *wordSet);
static void func_ov206_021baa4c(ZukanInfo *info);
static void func_ov206_021bacb0(ZukanInfo *info, u32 language);
static void func_ov206_021baf50(ZukanInfo *info);
static void func_ov206_021baf7c(ZukanInfo *info);
static void func_ov206_021bafc4(ZukanInfo *info);
static void func_ov206_021bb0e4(ZukanInfo *info);
static void func_ov206_021bb0fc(ZukanInfo *info);
static void func_ov206_021bb13c(ZukanInfo *info, int x, u16 y, u32 slot);
static void func_ov206_021bb160(ZukanInfo *info, u32 slot);
static void func_ov206_021bb184(ZukanInfo *info, u32 slot);
static void func_ov206_021bb214(ZukanInfo *info, u32 slot);
static void func_ov206_021bb23c(ZukanInfo *info, int x, u16 y, u32 slot);
static void func_ov206_021bb2c4(ZukanInfo *info, u32 slot);
static void func_ov206_021bb2d4(ZukanInfo *info);
static void func_ov206_021bb304(ZukanInfo *info);
static void func_ov206_021bb480(ZukanInfo *info);
static void func_ov206_021bb4e0(ZukanInfo *info, u32 language, u32 slot);
static void func_ov206_021bb534(ZukanInfo *info, u32 type1, u32 type2, u32 slot);
static void func_ov206_021bb79c(ZukanInfo *info, u32 type1, u32 type2, u32 language, u32 slot);
static void func_ov206_021bb92c(ZukanInfo *info, u32 slot);
static void func_ov206_021bb980(ZukanInfo *info);
static void func_ov206_021bb9e0(ZukanInfo *info);
static void func_ov206_021bba7c(ZukanInfo *info);
static void func_ov206_021bbab0(ZukanInfo *info, u32 species, u32 form, u32 slot);
static void func_ov206_021bbbd8(ZukanInfo *info, u32 slot);
static void func_ov206_021bbc04(ZukanInfo *info);
static void func_ov206_021bbc38(ZukanInfo *info);
static void func_ov206_021bbc68(ZukanInfo *info, u32 language);
static void func_ov206_021bbc9c(ZukanInfo *info);
static void func_ov206_021bbda8(ZukanInfo *info, BOOL visible);
static void func_ov206_021bbe34(ZukanInfo *info, BOOL visible);
static void func_ov206_021bbe5c(ZukanInfo *info);
static void func_ov206_021bc008(ZukanInfo *info);
static u32 func_ov206_021bc0ac(void);
static u32 func_ov206_021bc0b0(u32 species);
static u32 func_ov206_021bc0b4(void);
static u32 func_ov206_021bc0b8(int species);
static u32 func_ov206_021bc0cc(int species);
static u32 func_ov206_021bc0e0(void);
u32 func_ov206_021bc0e4(u32 species, u32 form);

// The x of the weight and the height, right aligned, and of the entry, in each of the other languages
// pokebw2's sWeightX
static const u16 data_ov206_021bc11c[ZUKAN_INFO_LANGUAGE_COUNT] = { 100, 100, 100, 100, 100, 100 };
// pokebw2's sEntryX
static const u16 data_ov206_021bc134[ZUKAN_INFO_LANGUAGE_COUNT] = { 4, 2, 2, 2, 2, 4 };
// pokebw2's sHeightX
static const u16 data_ov206_021bc110[ZUKAN_INFO_LANGUAGE_COUNT] = { 93, 94, 94, 94, 94, 94 };
// The type icons of each language in the Pokédex's graphics
// pokebw2's sTypeIconFiles
static const u16 data_ov206_021bc128[ZUKAN_INFO_LANGUAGE_COUNT] = { 21, 18, 19, 20, 23, 22 };
static const u32 sArcIds[ZUKAN_INFO_ARC_COUNT] = { ARCID_ZUKAN_GRA, ARCID_FORM_NAME_TABLE, ARCID_FONT, 0, 0, 0, 0 };
// Where each type's icon is in its language's file, as its tile row and column
// pokebw2's sTypeIconTiles
static const u8 data_ov206_021bc150[TYPE_NULL + 1][2] = {
    { 0, 0 }, { 2, 1 }, { 2, 0 }, { 3, 0 }, { 1, 2 }, { 1, 1 }, { 2, 3 }, { 2, 2 }, { 3, 2 },
    { 0, 1 }, { 0, 2 }, { 0, 3 }, { 1, 0 }, { 3, 1 }, { 1, 3 }, { 4, 0 }, { 3, 3 }, { 0, 0 },
};
// pokebw2's sLabelMessages
static const u32 data_ov206_021bc184[ZUKAN_INFO_STRBUF_COUNT] = {
#include "asset/zukan_info_data_ov206_021bc184.inc"
};
// pokebw2's sMsgBanks
static const u16 data_ov206_021bc1c4[ZUKAN_INFO_MSG_COUNT] = {
    TEXT_BANK_BTL_SERVER_FLOW_TITLE,
    TEXT_BANK_SPECIES_CATEGORIES,
    TEXT_BANK_POKEDEX_HEIGHTS,
    TEXT_BANK_POKEDEX_WEIGHTS,
    TEXT_BANK_POKEDEX_ENTRIES,
    TEXT_BANK_SPECIES_CATEGORIES_JA,
    TEXT_BANK_SPECIES_CATEGORIES_FR,
    TEXT_BANK_SPECIES_CATEGORIES_DE,
    TEXT_BANK_SPECIES_CATEGORIES_IT,
    TEXT_BANK_SPECIES_CATEGORIES_ES,
    TEXT_BANK_SPECIES_CATEGORIES_KO,
    TEXT_BANK_POKEDEX_HEIGHTS_JA,
    TEXT_BANK_POKEDEX_HEIGHTS_FR,
    TEXT_BANK_POKEDEX_HEIGHTS_DE,
    TEXT_BANK_POKEDEX_HEIGHTS_IT,
    TEXT_BANK_POKEDEX_HEIGHTS_ES,
    TEXT_BANK_POKEDEX_HEIGHTS_KO,
    TEXT_BANK_POKEDEX_WEIGHTS_JA,
    TEXT_BANK_POKEDEX_WEIGHTS_FR,
    TEXT_BANK_POKEDEX_WEIGHTS_DE,
    TEXT_BANK_POKEDEX_WEIGHTS_IT,
    TEXT_BANK_POKEDEX_WEIGHTS_ES,
    TEXT_BANK_POKEDEX_WEIGHTS_KO,
    TEXT_BANK_POKEDEX_ENTRIES_JA,
    TEXT_BANK_POKEDEX_ENTRIES_FR,
    TEXT_BANK_POKEDEX_ENTRIES_DE,
    TEXT_BANK_POKEDEX_ENTRIES_IT,
    TEXT_BANK_POKEDEX_ENTRIES_ES,
    TEXT_BANK_POKEDEX_ENTRIES_KO,
    TEXT_BANK_SPECIES_NAMES_JA,
    TEXT_BANK_SPECIES_NAMES_FR,
    TEXT_BANK_SPECIES_NAMES_DE,
    TEXT_BANK_SPECIES_NAMES_IT,
    TEXT_BANK_SPECIES_NAMES_ES,
    TEXT_BANK_SPECIES_NAMES_KO,
};

ZukanInfo *func_ov206_021b9de0(HeapID heapId, PartyPkm *pkm, BOOL national, BOOL caught, u32 mode, u32 engine,
                               u8 bgPriority, ClActUnit *unit, Font *font, PrintQueue *printQueue, MsgData *msgData0,
                               MsgData *msgData1, MsgData *msgData2) {
    u16 species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    u16 form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
    u8 sex = PokeParty_GetSex(pkm);
    u16 rare = PokeParty_IsRare(pkm);
    u32 personality = PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL);

    return func_ov206_021b9e68(heapId, species, form, sex, rare, personality, national, caught, mode, engine,
                               bgPriority, unit, font, printQueue, msgData0, msgData1, msgData2);
}

ZukanInfo *func_ov206_021b9e68(HeapID heapId, u16 species, u16 form, u16 sex, u16 rare, u32 personality, BOOL national,
                               BOOL caught, u32 mode, u32 engine, u8 bgPriority, ClActUnit *unit, Font *font,
                               PrintQueue *printQueue, MsgData *msgData0, MsgData *msgData1, MsgData *msgData2) {
    ZukanInfo *info = GFL_HeapAllocate(heapId, sizeof(ZukanInfo), FALSE, "zukan_info.c", 1071);
    void *file;
    int x;
    int y;
    u8 i;
    ArcTool *arc;
    u32 size;
    NNSG2dPaletteData *palette;
    u16 *colors;
    u16 formCount;

    MI_CpuFill8(info, 0, sizeof(ZukanInfo));
    info->heapId = heapId;
    info->species = species;
    info->form = form;
    info->sex = sex;
    info->rare = rare;
    info->personality = personality;
    info->national = national;
    info->caught = caught;
    info->mode = mode;
    info->engine = engine;
    info->bgPriority = bgPriority;
    info->unit = unit;
    info->font = font;
    info->printQueue = func_0201c440(info->heapId);
    info->givenMsgs[0] = msgData0;
    info->givenMsgs[1] = msgData1;
    info->givenMsgs[2] = msgData2;

    if (info->engine == 0) {
        info->palType = PALTYPE_MAIN_BG;
        info->frontBG = 3;
        info->midBG = 2;
        info->backBG = 0;
    } else {
        info->palType = PALTYPE_SUB_BG;
        info->frontBG = 7;
        info->midBG = 6;
        info->backBG = 4;
    }
    GFL_BGSysSetBGPriority(info->frontBG, info->bgPriority);
    GFL_BGSysSetBGPriority(info->midBG, (u8)(info->bgPriority + 1));
    GFL_BGSysSetBGPriority(info->backBG, (u8)(info->bgPriority + 2));
    info->unk38 = 0;
    info->scrollRequest = 0;

    switch (info->mode) {
    case ZUKAN_INFO_MODE_MAIN:
        info->state = ZUKAN_INFO_STATE_WAIT_START;
        break;
    case ZUKAN_INFO_MODE_SUB:
        info->state = ZUKAN_INFO_STATE_WAIT_CRY;
        break;
    case ZUKAN_INFO_MODE_DETAIL:
        info->state = ZUKAN_INFO_STATE_STATIC;
        break;
    }
    info->ready = FALSE;
    info->cryRequested = FALSE;
    info->delay = 0;
    info->tick = 0;
    info->glowPhase = 0;
    if (info->mode == ZUKAN_INFO_MODE_DETAIL && info->engine == 0) {
        info->scrollY = -24;
    } else {
        info->scrollY = 0;
    }

    func_ov206_021bbe5c(info);
    formCount = PML_PersonalGetParam(info->personal, PERSONAL_FORM_COUNT);
    if (info->form >= formCount) {
        info->form = 0;
    }

    arc = info->arcs[ZUKAN_INFO_ARC_ZUKAN_GRA];
    GFL_G2DIOLoadArcNCLRDefault(arc, ZUKAN_GRA_PALETTE, info->palType, 0, 0x60, info->heapId);
    info->midChars = GFL_BGSysLoadArcNCGRDynamic(arc, ZUKAN_GRA_CHARS, info->midBG, 0x1000, FALSE, info->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, ZUKAN_GRA_SCREEN_MID, info->midBG, CHAR_POS(info->midChars), 0x800,
                                           FALSE, info->heapId);
    info->backChars = GFL_BGSysLoadArcNCGRDynamic(arc, ZUKAN_GRA_CHARS, info->backBG, 0x1000, FALSE, info->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, ZUKAN_GRA_SCREEN_BACK, info->backBG, CHAR_POS(info->backChars), 0x800,
                                           FALSE, info->heapId);
    info->hasExtraChars = FALSE;
    info->hidden = FALSE;

    if (info->mode == ZUKAN_INFO_MODE_DETAIL && info->engine == 1) {
        GFL_BGSysFillScrArea(info->midBG, 0, 0, 0, 0x20, 3, 0x11);
        arc = info->arcs[ZUKAN_INFO_ARC_ZUKAN_GRA];
        GFL_G2DIOLoadArcNCLR(arc, ZUKAN_GRA_PALETTE_EXTRA, info->palType, 0, 0x80, 0x40, info->heapId);
        info->extraChars =
            GFL_BGSysLoadArcNCGRDynamic(arc, ZUKAN_GRA_CHARS_EXTRA, info->backBG, 0x2000, FALSE, info->heapId);
        info->hasExtraChars = TRUE;
        GFL_G2DIOLoadNSCRSync(arc, ZUKAN_GRA_SCREEN_EXTRA, info->backBG, 0x400, CHAR_POS(info->extraChars), 0x800,
                              FALSE, info->heapId);
        GFL_BGSysSetScrPaletteNo(info->backBG, 0, 0x20, 0x20, 0x15, 4);
        GFL_BGSysSetScrPaletteNo(info->backBG, 0, 0x35, 0x20, 3, 5);
    }

    arc = info->arcs[ZUKAN_INFO_ARC_ZUKAN_GRA];
    size = GFL_ArcToolGetDataLength(arc, ZUKAN_GRA_PALETTE);
    file = GFL_HeapAllocate(info->heapId, size, FALSE, "zukan_info.c", 1271);
    GFL_ArcToolRead(arc, ZUKAN_GRA_PALETTE, file);
    NNS_G2dGetUnpackedPaletteData(file, &palette);
    colors = palette->pRawData;
    for (i = 0; i < 5; i++) {
        info->glowColorsA[i] = colors[0x11 + i];
        info->glowColorsB[i] = colors[0x21 + i];
    }
    GFL_HeapFree(file);

    func_ov206_021bafc4(info);
    func_ov206_021bb0fc(info);
    info->tcb = GFL_VBlankTCBAdd(func_ov206_021ba864, info, 1);
    func_ov206_021bb304(info);
    func_ov206_021bb9e0(info);
    if (info->mode == ZUKAN_INFO_MODE_MAIN || info->mode == ZUKAN_INFO_MODE_DETAIL) {
        func_ov206_021ba890(info);
        func_ov206_021bbc38(info);
    }

    for (i = 0; i < 2; i++) {
        info->sprites[i] = NULL;
    }
    info->spriteSlot = 0;
    switch (info->mode) {
    case ZUKAN_INFO_MODE_MAIN:
    case ZUKAN_INFO_MODE_DETAIL:
        x = 0x32;
        y = 0x48;
        break;
    case ZUKAN_INFO_MODE_SUB:
        x = 0x80;
        y = 0x48;
        break;
    }
    func_ov206_021bb13c(info, x, y + info->scrollY, info->spriteSlot);

    if (info->mode == ZUKAN_INFO_MODE_MAIN) {
        info->windowEdge = 0x88;
        GX_SetVisibleWnd(GX_WNDMASK_W0);
        G2_SetWnd0Position(0, 8, 100, info->windowEdge);
        G2_SetWnd0InsidePlane(0x1f, TRUE);
        G2_SetWndOutsidePlane(0x1f, FALSE);
        G2_SetBlendBrightness(0x10, -16);
    }

    GFL_BGSysMoveBG(info->midBG, BG_MOVE_SET_Y, -info->scrollY);
    GFL_BGSysMoveBG(info->backBG, BG_MOVE_SET_Y, -info->scrollY);
    GFL_BGSysMoveBG(info->frontBG, BG_MOVE_SET_Y, -info->scrollY);
    GFL_BGSysLoadScr(info->midBG);
    GFL_BGSysLoadScr(info->backBG);
    return info;
}

void func_ov206_021ba2cc(ZukanInfo *info) {
    u8 i;

    func_ov206_021ba814(info);
    info->hidden = FALSE;
    for (i = 0; i < 2; i++) {
        func_ov206_021bb160(info, i);
    }
    if (info->mode == ZUKAN_INFO_MODE_DETAIL) {
        func_ov206_021ba4cc(info);
    }
    func_ov206_021bba7c(info);
    func_ov206_021bb480(info);
    GFL_TCBRemove(info->tcb);
    func_ov206_021bb0e4(info);
    if (info->hasExtraChars) {
        GFL_BGSysFreeCharMemory(info->backBG, CHAR_POS(info->extraChars), CHAR_SIZE(info->extraChars));
        info->hasExtraChars = FALSE;
    }
    GFL_BGSysFreeCharMemory(info->backBG, CHAR_POS(info->backChars), CHAR_SIZE(info->backChars));
    GFL_BGSysFreeCharMemory(info->midBG, CHAR_POS(info->midChars), CHAR_SIZE(info->midChars));
    func_ov206_021bc008(info);
    func_0201c6ec(info->printQueue);
    func_0201c4c0(info->printQueue);
    GFL_HeapFree(info);
}

void func_ov206_021ba374(ZukanInfo *info) {
    ClActorPos pos;
    BOOL xDone;
    BOOL yDone;

    if (info->mode == ZUKAN_INFO_MODE_MAIN) {
        func_ov206_021bbc9c(info);
    }

    info->tick++;
    if (info->tick == 4) {
        info->scrollRequest |= 1;
        info->tick = 0;
    }

    switch (info->state) {
    case ZUKAN_INFO_STATE_WAIT_START:
    case ZUKAN_INFO_STATE_DONE:
    case ZUKAN_INFO_STATE_STATIC:
        break;
    case ZUKAN_INFO_STATE_DELAY:
        info->delay++;
        if (info->delay == 15) {
            info->state = ZUKAN_INFO_STATE_OPEN_WINDOW;
        }
        break;
    case ZUKAN_INFO_STATE_OPEN_WINDOW:
        info->windowEdge--;
        G2_SetWnd0Position(0, 8, 100, info->windowEdge);
        if (info->windowEdge == 0x10) {
            GX_SetVisibleWnd(GX_WNDMASK_NONE);
            G2_SetBlendBrightness(0x10, 0);
            PokeVoice_Play(info->species, info->form, 0x40, 0, 0, 0, 0, NULL);
            info->state = ZUKAN_INFO_STATE_WAIT_READY;
        }
        break;
    case ZUKAN_INFO_STATE_WAIT_READY:
        if (info->ready) {
            info->state = ZUKAN_INFO_STATE_SLIDE_IN;
        }
        break;
    case ZUKAN_INFO_STATE_SLIDE_IN:
        xDone = FALSE;
        yDone = FALSE;
        func_0204b53c(info->sprites[info->spriteSlot], &pos);
        pos.x++;
        pos.y++;
        if (pos.x > 0x80) {
            pos.x = 0x80;
            xDone = TRUE;
        }
        if (pos.y > 0x48) {
            pos.y = 0x48;
            yDone = TRUE;
        }
        func_0204b528(info->sprites[info->spriteSlot], &pos);
        if (xDone && yDone) {
            info->state = ZUKAN_INFO_STATE_DONE;
        }
        break;
    case ZUKAN_INFO_STATE_WAIT_CRY:
        if (info->cryRequested) {
            PokeVoice_Play(info->species, info->form, 0x40, 0, 0, 0, 0, NULL);
            info->state = ZUKAN_INFO_STATE_DONE;
        }
        break;
    }

    if (info->mode == ZUKAN_INFO_MODE_MAIN || info->mode == ZUKAN_INFO_MODE_DETAIL) {
        func_ov206_021baf7c(info);
    }
    func_0201c4e4(info->printQueue);
}

void func_ov206_021ba4cc(ZukanInfo *info) {
    func_ov206_021baf50(info);
    func_ov206_021ba934(info);
    func_ov206_021bbbd8(info, info->footprintSlot);
    func_ov206_021bb92c(info, info->typeSlot);
}

// pokebw2's ZukanInfo_ClearDisplay
static void func_ov206_021ba4f4(ZukanInfo *info) {
    func_ov206_021baf50(info);
    func_ov206_021bbbd8(info, info->footprintSlot);
    func_ov206_021bb92c(info, info->typeSlot);
}

void func_ov206_021ba514(ZukanInfo *info) {
    info->ready = TRUE;
}

void func_ov206_021ba51c(ZukanInfo *info) {
    info->cryRequested = TRUE;
}

BOOL func_ov206_021ba524(ZukanInfo *info) {
    BOOL sliding = FALSE;

    if (info->state == ZUKAN_INFO_STATE_SLIDE_IN) {
        sliding = TRUE;
    }
    return sliding;
}

BOOL func_ov206_021ba534(ZukanInfo *info) {
    BOOL done = FALSE;

    if (info->state == ZUKAN_INFO_STATE_DONE) {
        done = TRUE;
    }
    return done;
}

void func_ov206_021ba544(ZukanInfo *info) {
    if (info->state == ZUKAN_INFO_STATE_WAIT_START) {
        info->state = ZUKAN_INFO_STATE_DELAY;
    }
}

void func_ov206_021ba550(ZukanInfo *info, u16 species, u16 form, u16 sex, u16 rare, u32 personality, BOOL caught) {
    int x;
    int y;
    u16 formCount;

    if (info->personal != NULL) {
        PML_PersonalFree(info->personal);
    }
    info->personal = PML_PersonalLoad(species, form, info->heapId);

    if (info->spriteSlot != 0) {
        info->spriteSlot = 0;
    } else {
        info->spriteSlot = 1;
    }
    if (info->typeSlot != 0) {
        info->typeSlot = 0;
    } else {
        info->typeSlot = 1;
    }
    if (info->footprintSlot != 0) {
        info->footprintSlot = 0;
    } else {
        info->footprintSlot = 1;
    }
    func_ov206_021bb160(info, info->spriteSlot);
    func_ov206_021ba4f4(info);

    info->species = species;
    info->form = form;
    info->sex = sex;
    info->rare = rare;
    info->personality = personality;
    info->caught = caught;
    formCount = PML_PersonalGetParam(info->personal, PERSONAL_FORM_COUNT);
    if (info->form >= formCount) {
        info->form = 0;
    }

    func_ov206_021bb0fc(info);
    func_ov206_021bbc38(info);
    switch (info->mode) {
    case ZUKAN_INFO_MODE_MAIN:
    case ZUKAN_INFO_MODE_DETAIL:
        x = 0x32;
        y = 0x48;
        break;
    case ZUKAN_INFO_MODE_SUB:
        x = 0x80;
        y = 0x48;
        break;
    }
    func_ov206_021bb13c(info, x, y + info->scrollY, info->spriteSlot);
    func_ov206_021bbe34(info, !info->hidden);
}

void func_ov206_021ba64c(ZukanInfo *info, u32 language) {
    if (info->typeSlot != 0) {
        info->typeSlot = 0;
    } else {
        info->typeSlot = 1;
    }
    func_ov206_021baf50(info);
    func_ov206_021bb92c(info, info->typeSlot);
    func_ov206_021bb4e0(info, language, info->typeSlot);
    func_ov206_021bbda8(info, info->caught);
    if (language == ZUKAN_INFO_LANGUAGE_GAME || info->species > SPECIES_ARCEUS) {
        func_ov206_021baa4c(info);
    } else {
        func_ov206_021bacb0(info, language);
    }
}

void func_ov206_021ba6b0(ZukanInfo *info, u16 species, u16 form, u16 sex, u16 rare, u32 personality, BOOL caught,
                         u32 language) {
    int x;
    int y;
    u16 formCount;

    if (info->personal != NULL) {
        PML_PersonalFree(info->personal);
    }
    info->personal = PML_PersonalLoad(species, form, info->heapId);

    if (info->spriteSlot != 0) {
        info->spriteSlot = 0;
    } else {
        info->spriteSlot = 1;
    }
    if (info->typeSlot != 0) {
        info->typeSlot = 0;
    } else {
        info->typeSlot = 1;
    }
    if (info->footprintSlot != 0) {
        info->footprintSlot = 0;
    } else {
        info->footprintSlot = 1;
    }
    func_ov206_021bb160(info, info->spriteSlot);
    func_ov206_021ba4f4(info);

    info->species = species;
    info->form = form;
    info->sex = sex;
    info->rare = rare;
    info->personality = personality;
    info->caught = caught;
    formCount = PML_PersonalGetParam(info->personal, PERSONAL_FORM_COUNT);
    if (info->form >= formCount) {
        info->form = 0;
    }

    func_ov206_021bb0fc(info);
    if (language == ZUKAN_INFO_LANGUAGE_GAME || info->species > SPECIES_ARCEUS) {
        func_ov206_021bbc38(info);
    } else {
        func_ov206_021bbc68(info, language);
    }
    switch (info->mode) {
    case ZUKAN_INFO_MODE_MAIN:
    case ZUKAN_INFO_MODE_DETAIL:
        x = 0x32;
        y = 0x48;
        break;
    case ZUKAN_INFO_MODE_SUB:
        x = 0x80;
        y = 0x48;
        break;
    }
    func_ov206_021bb13c(info, x, y + info->scrollY, info->spriteSlot);
    func_ov206_021bbe34(info, !info->hidden);
}

void func_ov206_021ba7c8(ZukanInfo *info) {
    BOOL visible;

    if (!info->hidden) {
        visible = TRUE;
        info->hidden = TRUE;
        GFL_BGSysSetBGEnabled(info->frontBG, FALSE);
        GFL_BGSysSetBGEnabled(info->midBG, FALSE);
        GFL_BGSysMoveBGReq(info->backBG, BG_MOVE_SET_Y, -0x100);
        if (info->hidden) {
            visible = FALSE;
        }
        func_ov206_021bbe34(info, visible);
    }
}

void func_ov206_021ba814(ZukanInfo *info) {
    BOOL visible;

    if (info->hidden) {
        info->hidden = FALSE;
        visible = TRUE;
        GFL_BGSysSetBGEnabled(info->frontBG, TRUE);
        GFL_BGSysSetBGEnabled(info->midBG, TRUE);
        GFL_BGSysMoveBGReq(info->backBG, BG_MOVE_SET_Y, -info->scrollY);
        if (info->hidden) {
            visible = FALSE;
        }
        func_ov206_021bbe34(info, visible);
    }
}

// pokebw2's ZukanInfo_VBlankTask
static void func_ov206_021ba864(TCB *tcb, void *data) {
    ZukanInfo *info = data;

    if (info->scrollRequest & 1) {
        GFL_BGSysMoveBGReq(info->backBG, BG_MOVE_RIGHT, 1);
    }
    info->unk38 = 0;
    info->scrollRequest = 0;
}

// pokebw2's ZukanInfo_InitWindows
static void func_ov206_021ba890(ZukanInfo *info) {
    u32 palType = PALTYPE_MAIN_BG;
    u32 size;
    int i;

    info->formNameTable = GFL_ArcToolReadHeapNewLZGetLen(info->arcs[ZUKAN_INFO_ARC_FORM_NAME_TABLE], 0, FALSE,
                                                         info->heapId, &size);
    if (info->engine != 0) {
        palType = PALTYPE_SUB_BG;
    }
    GFL_G2DIOLoadArcNCLRDefault(info->arcs[ZUKAN_INFO_ARC_FONT], 5, palType, 0x60, 0x20, info->heapId);
    {
        // The position and size of each window
        u8 rects[ZUKAN_INFO_WINDOW_COUNT][4] = {
            { 2, 0, 28, 3 },
            { 16, 4, 15, 5 },
            { 18, 12, 13, 5 },
            { 2, 17, 28, 7 },
        };

        for (i = 0; i < ZUKAN_INFO_WINDOW_COUNT; i++) {
            info->windows[i] =
                BmpWin_CreateDynamic(info->frontBG, rects[i][0], rects[i][1], rects[i][2], rects[i][3], 3, TRUE);
            GFL_BitmapFill(BmpWin_GetBitmap(info->windows[i]), 0);
        }
    }
    for (i = 0; i < ZUKAN_INFO_WINDOW_COUNT; i++) {
        info->flushed[i] = TRUE;
    }
}

// pokebw2's ZukanInfo_FreeWindows
static void func_ov206_021ba934(ZukanInfo *info) {
    int i;

    func_0201c6ec(info->printQueue);
    for (i = 0; i < ZUKAN_INFO_WINDOW_COUNT; i++) {
        info->flushed[i] = TRUE;
        BmpWin_Free(info->windows[i]);
    }
    GFL_HeapFree(info->formNameTable);
}

// Prints a string. x is where it starts, or where it ends for a right alignment; a centered one ignores it. With a
// word set, the string is a format for it
static void func_ov206_021ba960(HeapID heapId, BmpWin *window, PrintQueue *printQueue, Font *font, StrBuf *strbuf,
                                  u16 x, u16 y, u16 color, u32 align, WordSet *wordSet) {
    u16 left;
    u16 width;
    GFLBitmap *bitmap = BmpWin_GetBitmap(window);
    StrBuf *formatted;

    if (wordSet != NULL) {
        formatted = GFL_StrBufCreate(0x80, heapId);
        GFL_WordSetFormatStrbuf(wordSet, formatted, strbuf);
    } else {
        formatted = strbuf;
    }

    switch (align) {
    case PRINT_ALIGN_LEFT:
        left = x;
        break;
    case PRINT_ALIGN_RIGHT:
        width = GFL_FontGetBlockWidth(formatted, font, 0);
        left = x - width;
        break;
    case PRINT_ALIGN_CENTER:
        width = GFL_FontGetBlockWidth(formatted, font, 0);
        left = (GFL_BitmapGetWidth(bitmap) - width) / 2;
        break;
    }
    func_0201c724(printQueue, bitmap, left, y, formatted, font, color);

    if (wordSet != NULL) {
        GFL_StrBufFree(formatted);
    }
}

static void func_ov206_021baa08(HeapID heapId, BmpWin *window, MsgData *msgData, PrintQueue *printQueue, Font *font,
                                   u32 messageId, u16 x, u16 y, u16 color, u32 align, WordSet *wordSet) {
    StrBuf *strbuf = GFL_MsgDataLoadStrbufNew(msgData, messageId);

    func_ov206_021ba960(heapId, window, printQueue, font, strbuf, x, y, color, align, wordSet);
    GFL_StrBufFree(strbuf);
}

// pokebw2's ZukanInfo_PrintEntry
static void func_ov206_021baa4c(ZukanInfo *info) {
    MsgData *labels = info->msgs[ZUKAN_INFO_MSG_LABELS];
    MsgData *categories = info->msgs[ZUKAN_INFO_MSG_CATEGORIES];
    MsgData *heights = info->msgs[ZUKAN_INFO_MSG_HEIGHTS];
    MsgData *weights = info->msgs[ZUKAN_INFO_MSG_WEIGHTS];
    MsgData *entries = info->msgs[ZUKAN_INFO_MSG_ENTRIES];
    u16 message = info->species;
    u16 n = 0;
    u8 i;
    u16 number;
    WordSet *wordSet;
    StrBuf *name;
    u32 messageId;

    // The messages of a form follow its species's through the table of next forms
    for (; n != info->form; n++) {
        message = info->formNameTable[message];
        if (message == 0) {
            break;
        }
    }
    if (message == 0) {
        message = info->species;
    }

    if (info->mode != ZUKAN_INFO_MODE_DETAIL) {
        func_ov206_021baa08(info->heapId, info->windows[ZUKAN_INFO_WINDOW_LABEL], labels, info->printQueue,
                               info->font, BtlServerFlowTitle_Text_144, 0, 5, COLOR_TEXT,
                               PRINT_ALIGN_CENTER, NULL);
    }

    number = info->species;
    if (!info->national) {
        u16 *table = PML_PersonalLoadRegionalDexTable(info->heapId, 0);

        number = table[info->species];
        GFL_HeapFree(table);
    }
    if (number != 999) {
        wordSet = GFL_WordSetSystemCreateDefault(info->heapId);
        WordSetNumber(wordSet, 0, number, 3, NUM_PAD_ZERO, TRUE);
        func_ov206_021ba960(info->heapId, info->windows[ZUKAN_INFO_WINDOW_NAME], info->printQueue, info->font,
                              info->labels[0], 8, 5, COLOR_LABEL, PRINT_ALIGN_LEFT, wordSet);
        GFL_WordSetSystemFree(wordSet);
    } else {
        func_ov206_021ba960(info->heapId, info->windows[ZUKAN_INFO_WINDOW_NAME], info->printQueue, info->font,
                              info->labels[1], 8, 5, COLOR_LABEL, PRINT_ALIGN_LEFT, NULL);
    }

    name = GFL_MsgDataLoadStrbufNew(g_PMLSpeciesNamesResident, info->species);
    func_0201c724(info->printQueue, BmpWin_GetBitmap(info->windows[ZUKAN_INFO_WINDOW_NAME]), 0x30, 5, name, info->font,
                  COLOR_LABEL);
    GFL_StrBufFree(name);

    messageId = 0;
    if (info->caught) {
        messageId = info->species;
    }
    func_ov206_021baa08(info->heapId, info->windows[ZUKAN_INFO_WINDOW_NAME], categories, info->printQueue,
                           info->font, messageId, 0, 22, COLOR_LABEL, PRINT_ALIGN_CENTER, NULL);

    func_ov206_021ba960(info->heapId, info->windows[ZUKAN_INFO_WINDOW_SIZE], info->printQueue, info->font,
                          info->labels[2], 0, 4, COLOR_LABEL, PRINT_ALIGN_LEFT, NULL);
    func_ov206_021ba960(info->heapId, info->windows[ZUKAN_INFO_WINDOW_SIZE], info->printQueue, info->font,
                          info->labels[3], 0, 20, COLOR_LABEL, PRINT_ALIGN_LEFT, NULL);

    messageId = 0;
    if (info->caught) {
        messageId = message;
    }
    func_ov206_021baa08(info->heapId, info->windows[ZUKAN_INFO_WINDOW_SIZE], heights, info->printQueue, info->font,
                           messageId, 101, 4, COLOR_LABEL, PRINT_ALIGN_RIGHT, NULL);
    messageId = 0;
    if (info->caught) {
        messageId = message;
    }
    func_ov206_021baa08(info->heapId, info->windows[ZUKAN_INFO_WINDOW_SIZE], weights, info->printQueue, info->font,
                           messageId, 102, 20, COLOR_LABEL, PRINT_ALIGN_RIGHT, NULL);
    messageId = 0;
    if (info->caught) {
        messageId = info->species;
    }
    func_ov206_021baa08(info->heapId, info->windows[ZUKAN_INFO_WINDOW_TEXT], entries, info->printQueue, info->font,
                           messageId, 4, 5, COLOR_TEXT, PRINT_ALIGN_LEFT, NULL);

    for (i = 0; i < ZUKAN_INFO_WINDOW_COUNT; i++) {
        info->flushed[i] = FALSE;
    }
    func_ov206_021baf7c(info);
}

// pokebw2's ZukanInfo_PrintEntryInLanguage
static void func_ov206_021bacb0(ZukanInfo *info, u32 language) {
    MsgData *labels = info->msgs[ZUKAN_INFO_MSG_LABELS];
    MsgData *categories = info->msgs[ZUKAN_INFO_MSG_OTHER_CATEGORIES + language];
    MsgData *heights = info->msgs[ZUKAN_INFO_MSG_OTHER_HEIGHTS + language];
    MsgData *weights = info->msgs[ZUKAN_INFO_MSG_OTHER_WEIGHTS + language];
    MsgData *entries = info->msgs[ZUKAN_INFO_MSG_OTHER_ENTRIES + language];
    MsgData *names = info->msgs[ZUKAN_INFO_MSG_OTHER_NAMES + language];
    u16 message = info->species;
    u16 number;
    u16 messageId;
    u8 i;
    WordSet *wordSet;

    // Giratina's Origin Forme and Shaymin's Sky Forme have messages after the last species of the generations
    if (message == SPECIES_GIRATINA && info->form != 0) {
        message = SPECIES_ARCEUS + 1;
    } else if (message == SPECIES_SHAYMIN && info->form != 0) {
        message = SPECIES_ARCEUS + 2;
    }

    if (info->mode != ZUKAN_INFO_MODE_DETAIL) {
        func_ov206_021baa08(info->heapId, info->windows[ZUKAN_INFO_WINDOW_LABEL], labels, info->printQueue,
                               info->font, BtlServerFlowTitle_Text_144, 0, 5, COLOR_TEXT,
                               PRINT_ALIGN_CENTER, NULL);
    }

    number = info->species;
    if (!info->national) {
        u16 *table = PML_PersonalLoadRegionalDexTable(info->heapId, 0);

        number = table[info->species];
        GFL_HeapFree(table);
    }
    if (number != 999) {
        wordSet = GFL_WordSetSystemCreateDefault(info->heapId);
        if (language == 0) {
            WordSetNumber(wordSet, 0, number, 3, NUM_PAD_ZERO, FALSE);
        } else {
            WordSetNumber(wordSet, 0, number, 3, NUM_PAD_ZERO, TRUE);
        }
        func_ov206_021ba960(info->heapId, info->windows[ZUKAN_INFO_WINDOW_NAME], info->printQueue, info->font,
                              info->labels[0], 8, 5, COLOR_LABEL, PRINT_ALIGN_LEFT, wordSet);
        GFL_WordSetSystemFree(wordSet);
    } else {
        func_ov206_021ba960(info->heapId, info->windows[ZUKAN_INFO_WINDOW_NAME], info->printQueue, info->font,
                              info->labels[1], 8, 5, COLOR_LABEL, PRINT_ALIGN_LEFT, NULL);
    }

    func_ov206_021baa08(info->heapId, info->windows[ZUKAN_INFO_WINDOW_NAME], names, info->printQueue, info->font,
                           info->species, 0x30, 5, COLOR_LABEL, PRINT_ALIGN_LEFT, NULL);

    messageId = 0;
    if (info->caught) {
        messageId = info->species;
    }
    func_ov206_021baa08(info->heapId, info->windows[ZUKAN_INFO_WINDOW_NAME], categories, info->printQueue,
                           info->font, messageId, 0, 22, COLOR_LABEL, PRINT_ALIGN_CENTER, NULL);

    func_ov206_021ba960(info->heapId, info->windows[ZUKAN_INFO_WINDOW_SIZE], info->printQueue, info->font,
                          info->labels[4 + language], 0, 4, COLOR_LABEL, PRINT_ALIGN_LEFT, NULL);
    func_ov206_021ba960(info->heapId, info->windows[ZUKAN_INFO_WINDOW_SIZE], info->printQueue, info->font,
                          info->labels[10 + language], 0, 20, COLOR_LABEL, PRINT_ALIGN_LEFT, NULL);

    messageId = 0;
    if (info->caught) {
        messageId = message;
    }
    func_ov206_021baa08(info->heapId, info->windows[ZUKAN_INFO_WINDOW_SIZE], heights, info->printQueue, info->font,
                           messageId, data_ov206_021bc110[language], 4, COLOR_LABEL, PRINT_ALIGN_RIGHT, NULL);
    messageId = 0;
    if (info->caught) {
        messageId = message;
    }
    func_ov206_021baa08(info->heapId, info->windows[ZUKAN_INFO_WINDOW_SIZE], weights, info->printQueue, info->font,
                           messageId, data_ov206_021bc11c[language], 20, COLOR_LABEL, PRINT_ALIGN_RIGHT, NULL);
    messageId = 0;
    if (info->caught) {
        messageId = info->species;
    }
    func_ov206_021baa08(info->heapId, info->windows[ZUKAN_INFO_WINDOW_TEXT], entries, info->printQueue, info->font,
                           messageId, 4, 5, COLOR_TEXT, PRINT_ALIGN_LEFT, NULL);

    for (i = 0; i < ZUKAN_INFO_WINDOW_COUNT; i++) {
        info->flushed[i] = FALSE;
    }
    func_ov206_021baf7c(info);
}

// pokebw2's ZukanInfo_ClearWindows
static void func_ov206_021baf50(ZukanInfo *info) {
    int i;

    func_0201c6ec(info->printQueue);
    for (i = 0; i < ZUKAN_INFO_WINDOW_COUNT; i++) {
        info->flushed[i] = TRUE;
        GFL_BitmapFill(BmpWin_GetBitmap(info->windows[i]), 0);
    }
}

// pokebw2's ZukanInfo_FlushWindows
static void func_ov206_021baf7c(ZukanInfo *info) {
    int i;

    for (i = 0; i < ZUKAN_INFO_WINDOW_COUNT; i++) {
        if (!info->flushed[i] && !func_0201c6c4(info->printQueue, BmpWin_GetBitmap(info->windows[i]))) {
            BmpWin_Transfer(info->windows[i]);
            info->flushed[i] = TRUE;
        }
    }
}

// pokebw2's ZukanInfo_InitLabelWindows
static void func_ov206_021bafc4(ZukanInfo *info) {
    GFLBitmap *src = GFL_G2DIOLoadBitmap(ARCID_ZUKAN_GRA, ZUKAN_GRA_CHARS, FALSE, info->heapId);
    GFLBitmap *dest;

    // The caught label's window
    info->labelWindows[0] = BmpWin_CreateDynamic(info->midBG, 12, 4, 4, 3, 0, FALSE);
    dest = BmpWin_GetBitmap(info->labelWindows[0]);
    GFL_BitmapCopyArea(src, dest, 8, 0, 0, 0, 0x20, 0x18, 0);

    // The not caught label's window, with its pieces
    info->labelWindows[1] = BmpWin_CreateDynamic(info->midBG, 12, 4, 4, 3, 0, FALSE);
    dest = BmpWin_GetBitmap(info->labelWindows[1]);
    GFL_BitmapCopyArea(src, dest, 0xa8, 8, 0, 0, 0x20, 8, 0);
    GFL_BitmapCopyArea(src, BmpWin_GetBitmap(info->labelWindows[1]), 0xf8, 8, 0, 8, 8, 8, 0);
    GFL_BitmapCopyArea(src, BmpWin_GetBitmap(info->labelWindows[1]), 0x28, 0x10, 8, 8, 0x18, 8, 0);
    GFL_BitmapCopyArea(src, BmpWin_GetBitmap(info->labelWindows[1]), 0x80, 0, 0, 0x10, 8, 8, 0);
    GFL_BitmapCopyArea(src, BmpWin_GetBitmap(info->labelWindows[1]), 0x58, 0x10, 8, 0x10, 0x18, 8, 0);
    GFL_BitmapFree(src);
}

// pokebw2's ZukanInfo_FreeLabelWindows
static void func_ov206_021bb0e4(ZukanInfo *info) {
    BmpWin_Free(info->labelWindows[0]);
    BmpWin_Free(info->labelWindows[1]);
}

// pokebw2's ZukanInfo_FlushLabelWindow
static void func_ov206_021bb0fc(ZukanInfo *info) {
    if (info->caught) {
        BmpWin_Transfer(info->labelWindows[0]);
    } else {
        BmpWin_Transfer(info->labelWindows[1]);
    }
}

// pokebw2's ZukanInfo_LoadSprite
static void func_ov206_021bb13c(ZukanInfo *info, int x, u16 y, u32 slot) {
    func_ov206_021bb184(info, slot);
    func_ov206_021bb23c(info, x, y, slot);
    func_ov206_021bb2d4(info);
}

// pokebw2's ZukanInfo_UnloadSprite
static void func_ov206_021bb160(ZukanInfo *info, u32 slot) {
    if (info->sprites[slot] != NULL) {
        func_ov206_021bb2c4(info, slot);
        func_ov206_021bb214(info, slot);
        info->sprites[slot] = NULL;
    }
}

// pokebw2's ZukanInfo_LoadSpriteResources
static void func_ov206_021bb184(ZukanInfo *info, u32 slot) {
    BOOL sub = FALSE;
    ArcTool *arc;

    if (info->engine != 0) {
        sub = TRUE;
    }
    arc = info->arcs[ZUKAN_INFO_ARC_POKEGRA];

    info->chars[slot] = func_0202d81c(arc, info->species, info->form, info->sex, info->rare, 0, FALSE,
                                               info->personality, sub, info->heapId);
    info->palettes[slot] = func_0202d7d8(arc, info->species, info->form, info->sex, info->rare, 0, FALSE,
                                                    sub, slot * 32, info->heapId);
    info->cellAnims[slot] = func_0202d898(info->species, info->form, info->sex, info->rare, 0, FALSE, 2,
                                                       sub, info->heapId);
}

// pokebw2's ZukanInfo_FreeSpriteResources
static void func_ov206_021bb214(ZukanInfo *info, u32 slot) {
    func_0204ada4(info->palettes[slot]);
    func_0204a8d4(info->chars[slot]);
    func_0204afd8(info->cellAnims[slot]);
}

// pokebw2's ZukanInfo_CreateSpriteActor
static void func_ov206_021bb23c(ZukanInfo *info, int x, u16 y, u32 slot) {
    BOOL sub = FALSE;
    ClActorSetup setup;

    if (info->engine != 0) {
        sub = TRUE;
    }

    MI_CpuFill8(&setup, 0, sizeof(setup));
    setup.x = x;
    setup.y = y;
    info->sprites[slot] = func_0204b294(info->unit, info->chars[slot], info->palettes[slot], info->cellAnims[slot],
                                        &setup, sub, info->heapId);
    func_0204b800(info->sprites[slot], 0);
    if (info->mode == ZUKAN_INFO_MODE_DETAIL) {
        func_0204b688(info->sprites[slot], 1);
    }
    if (PML_PersonalGetParam(info->personal, PERSONAL_SPRITE_FLIP) == 0) {
        func_0204b604(info->sprites[slot], 1, TRUE);
    }
}

// pokebw2's ZukanInfo_DeleteSpriteActor
static void func_ov206_021bb2c4(ZukanInfo *info, u32 slot) {
    func_0204b3b4(info->sprites[slot]);
}

// pokebw2's ZukanInfo_HideOtherSprites
static void func_ov206_021bb2d4(ZukanInfo *info) {
    u8 i;

    for (i = 0; i < 2; i++) {
        if (i != info->spriteSlot && info->sprites[i] != NULL) {
            func_0204b3dc(info->sprites[i], FALSE);
        }
    }
}

// pokebw2's ZukanInfo_InitTypeIcons
static void func_ov206_021bb304(ZukanInfo *info) {
    ClActVram vram = info->engine == 0 ? CLACT_VRAM_MAIN : CLACT_VRAM_SUB;
    BOOL sub = FALSE;
    ArcTool *arc;
    ClActorSetup setup;
    u8 i;
    u8 j;

    if (info->engine != 0) {
        sub = TRUE;
    }
    arc = info->arcs[ZUKAN_INFO_ARC_APP_MENU_COMMON];
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            info->typeChars[i][j] = -1;
        }
    }
    info->typeSlot = 0;
    info->typePalette = func_0204ac18(arc, func_020275e8(), vram, 0x40, 0, 3, info->heapId);
    info->typeCellAnims = func_0204af28(arc, func_020275fc(2), func_02027600(2), info->heapId);

    if (info->mode == ZUKAN_INFO_MODE_DETAIL && info->engine == 0) {
        setup.x = 0;
        setup.y = 0;
        setup.sequence = 0;
        setup.priority = 0;
        setup.bgPriority = 0;
        info->allTypesLoaded = TRUE;
        info->shownTypes[0] = TYPE_NULL;
        info->shownTypes[1] = TYPE_NULL;
        for (i = 0; i < TYPE_NULL; i++) {
            info->allTypeChars[i] = func_0204a6c8(arc, func_020275f8(i), FALSE, vram, info->heapId);
            info->allTypeActors[i] = func_0204b294(info->unit, info->allTypeChars[i], info->typePalette,
                                                   info->typeCellAnims, &setup, sub, info->heapId);
            func_0204b6f4(info->allTypeActors[i], func_020275ec(i), TRUE);
            func_0204b800(info->allTypeActors[i], 2);
            func_0204b688(info->allTypeActors[i], 1);
            func_0204b3dc(info->allTypeActors[i], FALSE);
        }
    } else {
        info->allTypesLoaded = FALSE;
    }
}

// pokebw2's ZukanInfo_FreeTypeIcons
static void func_ov206_021bb480(ZukanInfo *info) {
    u8 i;

    for (i = 0; i < 2; i++) {
        func_ov206_021bb92c(info, i);
    }
    if (info->allTypesLoaded) {
        for (i = 0; i < TYPE_NULL; i++) {
            func_0204b3b4(info->allTypeActors[i]);
            func_0204a8d4(info->allTypeChars[i]);
        }
        info->allTypesLoaded = FALSE;
    }
    func_0204afd8(info->typeCellAnims);
    func_0204ada4(info->typePalette);
}

// Shows the Pokémon's types in the slot, in the game's language or in another one
// pokebw2's ZukanInfo_ShowTypes
static void func_ov206_021bb4e0(ZukanInfo *info, u32 language, u32 slot) {
    void *personal = info->personal;
    u8 type1 = PML_PersonalGetParam(personal, PERSONAL_TYPE_1);
    u8 type2 = PML_PersonalGetParam(personal, PERSONAL_TYPE_2);

    if (type1 == type2) {
        type2 = TYPE_NULL;
    }
    if (language == ZUKAN_INFO_LANGUAGE_GAME) {
        func_ov206_021bb534(info, type1, type2, slot);
    } else {
        func_ov206_021bb79c(info, type1, type2, language, slot);
    }
    func_ov206_021bb980(info);
}

// pokebw2's ZukanInfo_ShowTypesInGameLanguage
static void func_ov206_021bb534(ZukanInfo *info, u32 type1, u32 type2, u32 slot) {
    if (info->allTypesLoaded) {
        BOOL sub = FALSE;
        ClActorPos pos[2];

        if (info->engine != 0) {
            sub = TRUE;
        }
        pos[0].x = 168;
        pos[0].y = info->scrollY + 84;
        pos[1].x = 208;
        pos[1].y = info->scrollY + 84;

        if (info->shownTypes[0] != TYPE_NULL) {
            if (info->shownTypes[0] != type1 && info->shownTypes[0] != type2) {
                func_0204b3dc(info->allTypeActors[info->shownTypes[0]], FALSE);
                info->shownTypes[0] = TYPE_NULL;
            } else {
                if (info->shownTypes[0] == type2) {
                    func_0204b404(info->allTypeActors[info->shownTypes[0]], &pos[1], sub);
                }
                func_0204b3dc(info->allTypeActors[info->shownTypes[0]], TRUE);
            }
        }
        if (info->shownTypes[1] != TYPE_NULL) {
            if (info->shownTypes[1] != type1 && info->shownTypes[1] != type2) {
                func_0204b3dc(info->allTypeActors[info->shownTypes[1]], FALSE);
                info->shownTypes[1] = TYPE_NULL;
            } else {
                if (info->shownTypes[1] == type1) {
                    func_0204b404(info->allTypeActors[info->shownTypes[1]], &pos[0], sub);
                }
                func_0204b3dc(info->allTypeActors[info->shownTypes[1]], TRUE);
            }
        }
        if (type1 != TYPE_NULL) {
            info->shownTypes[0] = type1;
            func_0204b3dc(info->allTypeActors[info->shownTypes[0]], TRUE);
            func_0204b404(info->allTypeActors[info->shownTypes[0]], &pos[0], sub);
        }
        if (type2 != TYPE_NULL) {
            info->shownTypes[1] = type2;
            func_0204b3dc(info->allTypeActors[info->shownTypes[1]], TRUE);
            func_0204b404(info->allTypeActors[info->shownTypes[1]], &pos[1], sub);
        }
    } else {
        ClActorSetup setups[2] = { { 0 }, { 0 } };
        BOOL sub = FALSE;
        u8 types[2];
        ClActSurface vram;
        ArcTool *arc;
        int i;

        if (info->engine != 0) {
            sub = TRUE;
        }
        vram = info->engine == 0 ? CLACT_SURFACE_MAIN : CLACT_SURFACE_SUB;
        types[0] = type1;
        types[1] = type2;
        setups[0].x = 168;
        setups[0].y = info->scrollY + 84;
        setups[1].x = 208;
        setups[1].y = info->scrollY + 84;
        arc = info->arcs[ZUKAN_INFO_ARC_APP_MENU_COMMON];

        for (i = 0; i < 2; i++) {
            info->typeChars[slot][i] = -1;
            if (types[i] != TYPE_NULL) {
                info->typeChars[slot][i] = func_0204a6c8(arc, func_020275f8(types[i]), FALSE, sub, info->heapId);
            }
        }
        for (i = 0; i < 2; i++) {
            if (info->typeChars[slot][i] != -1) {
                info->typeActors[slot][i] = func_0204b294(info->unit, info->typeChars[slot][i], info->typePalette,
                                                          info->typeCellAnims, &setups[i], vram, info->heapId);
                func_0204b6f4(info->typeActors[slot][i], func_020275ec(types[i]), TRUE);
                func_0204b800(info->typeActors[slot][i], 2);
                func_0204b688(info->typeActors[slot][i], 1);
            }
        }
    }
}

// pokebw2's ZukanInfo_ShowTypesInLanguage
static void func_ov206_021bb79c(ZukanInfo *info, u32 type1, u32 type2, u32 language, u32 slot) {
    ClActorSetup setups[2] = { { 0 }, { 0 } };
    BOOL sub = FALSE;
    u8 types[2];
    ClActSurface vram;
    ArcTool *arc;
    NNSG2dCharacterData *character;
    void *file;
    u8 *tiles;
    int i;

    if (info->engine != 0) {
        sub = TRUE;
    }
    vram = info->engine == 0 ? CLACT_SURFACE_MAIN : CLACT_SURFACE_SUB;
    types[0] = type1;
    types[1] = type2;
    setups[0].x = 168;
    setups[0].y = info->scrollY + 84;
    setups[1].x = 208;
    setups[1].y = info->scrollY + 84;
    arc = info->arcs[ZUKAN_INFO_ARC_APP_MENU_COMMON];

    file = GFL_G2DIOReadBGNCGRArc(info->arcs[ZUKAN_INFO_ARC_ZUKAN_GRA], data_ov206_021bc128[language], FALSE, &character,
                                  info->heapId);
    tiles = character->pRawData;
    for (i = 0; i < 2; i++) {
        u32 x;
        u32 y;
        u8 type;

        info->typeChars[slot][i] = -1;
        type = types[i];
        if (type != TYPE_NULL) {
            info->typeChars[slot][i] = func_0204a6c8(arc, func_020275f8(type), FALSE, sub, info->heapId);
            x = data_ov206_021bc150[type][1] << 7;
            y = data_ov206_021bc150[type][0] << 11;
            func_0204aa94(info->typeChars[slot][i], tiles + (y + x), 0x80, 0, sub);
            func_0204aa94(info->typeChars[slot][i], tiles + (y + 0x400 + x), 0x80, 0x80, sub);
        }
    }
    GFL_HeapFree(file);

    for (i = 0; i < 2; i++) {
        if (info->typeChars[slot][i] != -1) {
            info->typeActors[slot][i] = func_0204b294(info->unit, info->typeChars[slot][i], info->typePalette,
                                                      info->typeCellAnims, &setups[i], vram, info->heapId);
            func_0204b6f4(info->typeActors[slot][i], func_020275ec(types[i]), TRUE);
            func_0204b800(info->typeActors[slot][i], 2);
            func_0204b688(info->typeActors[slot][i], 1);
        }
    }
}

// pokebw2's ZukanInfo_FreeTypeActors
static void func_ov206_021bb92c(ZukanInfo *info, u32 slot) {
    int i;

    if (!info->allTypesLoaded) {
        for (i = 0; i < 2; i++) {
            if (info->typeChars[slot][i] != -1) {
                func_0204b3b4(info->typeActors[slot][i]);
            }
        }
        for (i = 0; i < 2; i++) {
            if (info->typeChars[slot][i] != -1) {
                func_0204a8d4(info->typeChars[slot][i]);
                info->typeChars[slot][i] = -1;
            }
        }
    }
}

// pokebw2's ZukanInfo_HideOtherTypes
static void func_ov206_021bb980(ZukanInfo *info) {
    u8 slot;
    u8 i;

    if (!info->allTypesLoaded) {
        for (slot = 0; slot < 2; slot++) {
            if (slot != info->typeSlot) {
                for (i = 0; i < 2; i++) {
                    if (info->typeChars[slot][i] != -1) {
                        func_0204b3dc(info->typeActors[slot][i], FALSE);
                    }
                }
            }
        }
    }
}

// pokebw2's ZukanInfo_InitFootprints
static void func_ov206_021bb9e0(ZukanInfo *info) {
    BOOL sub;
    u8 i;
    ArcTool *arc;

    for (i = 0; i < 2; i++) {
        info->footprints[i] = NULL;
    }
    info->footprintSlot = 0;
    sub = FALSE;
    if (info->engine != 0) {
        sub = TRUE;
    }
    arc = info->arcs[ZUKAN_INFO_ARC_FOOTPRINT];
    info->footprintPalette =
        func_0204ac18(arc, func_ov206_021bc0b4(), sub, 0xa0, 0, 1, info->heapId);
    info->footprintCellAnimsOld = func_0204af28(arc, func_ov206_021bc0b8(0),
                                                func_ov206_021bc0cc(0), info->heapId);
    info->footprintCellAnims = func_0204af28(arc, func_ov206_021bc0b8(SPECIES_ARCEUS + 1),
                                             func_ov206_021bc0cc(SPECIES_ARCEUS + 1), info->heapId);
}

// pokebw2's ZukanInfo_FreeFootprints
static void func_ov206_021bba7c(ZukanInfo *info) {
    u8 i;

    for (i = 0; i < 2; i++) {
        func_ov206_021bbbd8(info, i);
    }
    func_0204afd8(info->footprintCellAnims);
    func_0204afd8(info->footprintCellAnimsOld);
    func_0204ada4(info->footprintPalette);
}

// pokebw2's ZukanInfo_LoadFootprint
static void func_ov206_021bbab0(ZukanInfo *info, u32 species, u32 form, u32 slot) {
    BOOL sub = FALSE;
    ClActSurface vram;
    ArcTool *arc;
    u32 formFile;
    ClActorSetup setup;

    if (info->engine != 0) {
        sub = TRUE;
    }
    vram = info->engine == 0 ? CLACT_SURFACE_MAIN : CLACT_SURFACE_SUB;
    formFile = func_ov206_021bc0e4(species, form);
    if (formFile == 0xffff) {
        arc = info->arcs[ZUKAN_INFO_ARC_FOOTPRINT];
        info->footprintChars[slot] = func_0204a6c8(arc, func_ov206_021bc0b0(species), TRUE, sub, info->heapId);
    } else {
        arc = info->arcs[ZUKAN_INFO_ARC_FOOTPRINT_FORM];
        info->footprintChars[slot] = func_0204a6c8(arc, formFile, TRUE, sub, info->heapId);
    }

    setup.x = 120;
    setup.y = info->scrollY + 88;
    setup.sequence = 0;
    setup.priority = 1;
    setup.bgPriority = 0;
    if (species <= SPECIES_ARCEUS && formFile != 0xffff) {
        info->footprints[slot] = func_0204b294(info->unit, info->footprintChars[slot], info->footprintPalette,
                                               info->footprintCellAnimsOld, &setup, vram, info->heapId);
    } else {
        info->footprints[slot] = func_0204b294(info->unit, info->footprintChars[slot], info->footprintPalette,
                                               info->footprintCellAnims, &setup, vram, info->heapId);
    }
    func_0204b800(info->footprints[slot], 1);
    func_0204b688(info->footprints[slot], 1);
    func_ov206_021bbc04(info);
}

// pokebw2's ZukanInfo_UnloadFootprint
static void func_ov206_021bbbd8(ZukanInfo *info, u32 slot) {
    if (info->footprints[slot] != NULL) {
        func_0204b3b4(info->footprints[slot]);
        func_0204a8d4(info->footprintChars[slot]);
        info->footprints[slot] = NULL;
    }
}

// pokebw2's ZukanInfo_HideOtherFootprints
static void func_ov206_021bbc04(ZukanInfo *info) {
    u8 i;

    for (i = 0; i < 2; i++) {
        if (i != info->footprintSlot && info->footprints[i] != NULL) {
            func_0204b3dc(info->footprints[i], FALSE);
        }
    }
}

// pokebw2's ZukanInfo_ShowEntry
static void func_ov206_021bbc38(ZukanInfo *info) {
    func_ov206_021bb4e0(info, ZUKAN_INFO_LANGUAGE_GAME, info->typeSlot);
    func_ov206_021bbab0(info, info->species, info->form, info->footprintSlot);
    func_ov206_021bbda8(info, info->caught);
    func_ov206_021baa4c(info);
}

// pokebw2's ZukanInfo_ShowEntryInLanguage
static void func_ov206_021bbc68(ZukanInfo *info, u32 language) {
    func_ov206_021bb4e0(info, language, info->typeSlot);
    func_ov206_021bbab0(info, info->species, info->form, info->footprintSlot);
    func_ov206_021bbda8(info, info->caught);
    func_ov206_021bacb0(info, language);
}

// The footprint's frame glows: a few of the palette's colors go back and forth between two sets
// pokebw2's ZukanInfo_UpdateFootprintGlow
static void func_ov206_021bbc9c(ZukanInfo *info) {
    s16 ratio;
    u8 i;

    if (info->glowPhase + 0x400 >= 0x10000) {
        info->glowPhase = info->glowPhase + 0x400 - 0x10000;
    } else {
        info->glowPhase += 0x400;
    }
    ratio = (FX_CosIdx(info->glowPhase) + 0x1000) / 2;

    for (i = 0; i < 5; i++) {
        u16 a = info->glowColorsA[i];
        u16 b = info->glowColorsB[i];
        u8 aR = a & 0x1f;
        u8 aG = (a & 0x3e0) >> 5;
        u8 aB = (a & 0x7c00) >> 10;
        u8 bR = b & 0x1f;
        u8 bG = (b & 0x3e0) >> 5;
        u8 bB = (b & 0x7c00) >> 10;
        u8 r = aR + ((bR - aR) * ratio >> 12);
        u8 g = aG + ((bG - aG) * ratio >> 12);
        u8 blue = aB + ((bB - aB) * ratio >> 12);

        info->glowColors[i] = r | (g << 5) | (blue << 10);
    }
    NNS_GfdRegisterNewVramTransferTask(info->engine == 0 ? NNS_GFD_DST_2D_BG_PLTT_MAIN : NNS_GFD_DST_2D_BG_PLTT_SUB,
                                       0x22, info->glowColors, sizeof(info->glowColors));
}

// pokebw2's ZukanInfo_SetFootprintsVisible
static void func_ov206_021bbda8(ZukanInfo *info, BOOL visible) {
    u8 i;

    func_0204b3dc(info->footprints[info->footprintSlot], visible);
    if (info->allTypesLoaded) {
        if (info->shownTypes[0] != TYPE_NULL) {
            func_0204b3dc(info->allTypeActors[info->shownTypes[0]], visible);
        }
        if (info->shownTypes[1] != TYPE_NULL) {
            func_0204b3dc(info->allTypeActors[info->shownTypes[1]], visible);
        }
    } else {
        for (i = 0; i < 2; i++) {
            if (info->typeChars[info->typeSlot][i] != -1) {
                func_0204b3dc(info->typeActors[info->typeSlot][i], visible);
            }
        }
    }
}

// pokebw2's ZukanInfo_SetVisible
static void func_ov206_021bbe34(ZukanInfo *info, BOOL visible) {
    func_0204b3dc(info->sprites[info->spriteSlot], visible);
    if (!info->caught) {
        visible = FALSE;
    }
    func_ov206_021bbda8(info, visible);
}

// pokebw2's ZukanInfo_LoadResources
static void func_ov206_021bbe5c(ZukanInfo *info) {
    u8 i;

    for (i = 0; i < ZUKAN_INFO_ARC_COUNT; i++) {
        switch (i) {
        case ZUKAN_INFO_ARC_APP_MENU_COMMON:
            info->arcs[i] = GFL_ArcSysCreateFileHandle(getUINarcIdx(), info->heapId);
            break;
        case ZUKAN_INFO_ARC_POKEGRA:
            info->arcs[i] = MakePokeGraArcHandle(info->heapId);
            break;
        case ZUKAN_INFO_ARC_FOOTPRINT:
            info->arcs[i] = GFL_ArcSysCreateFileHandle(func_ov206_021bc0ac(), info->heapId);
            break;
        case ZUKAN_INFO_ARC_FOOTPRINT_FORM:
            info->arcs[i] = GFL_ArcSysCreateFileHandle(func_ov206_021bc0e0(), info->heapId);
            break;
        default:
            info->arcs[i] = GFL_ArcSysCreateFileHandle(sArcIds[i], info->heapId);
            break;
        }
    }

    for (i = 0; i < ZUKAN_INFO_MSG_COUNT; i++) {
        switch (i) {
        case ZUKAN_INFO_MSG_CATEGORIES:
            if (info->givenMsgs[0] != NULL) {
                info->msgs[i] = info->givenMsgs[0];
            } else {
                info->msgs[i] = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, data_ov206_021bc1c4[i], info->heapId);
            }
            break;
        case ZUKAN_INFO_MSG_HEIGHTS:
            if (info->givenMsgs[1] != NULL) {
                info->msgs[i] = info->givenMsgs[1];
            } else {
                info->msgs[i] = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, data_ov206_021bc1c4[i], info->heapId);
            }
            break;
        case ZUKAN_INFO_MSG_WEIGHTS:
            if (info->givenMsgs[2] != NULL) {
                info->msgs[i] = info->givenMsgs[2];
            } else {
                info->msgs[i] = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, data_ov206_021bc1c4[i], info->heapId);
            }
            break;
        default:
            info->msgs[i] = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, data_ov206_021bc1c4[i], info->heapId);
            break;
        }
    }

    info->personal = PML_PersonalLoad(info->species, info->form, info->heapId);
    for (i = 0; i < ZUKAN_INFO_STRBUF_COUNT; i++) {
        info->labels[i] = GFL_MsgDataLoadStrbufNew(info->msgs[ZUKAN_INFO_MSG_LABELS], data_ov206_021bc184[i]);
    }
}

// pokebw2's ZukanInfo_FreeResources
static void func_ov206_021bc008(ZukanInfo *info) {
    u8 i;

    for (i = 0; i < ZUKAN_INFO_STRBUF_COUNT; i++) {
        GFL_StrBufFree(info->labels[i]);
    }
    if (info->personal != NULL) {
        PML_PersonalFree(info->personal);
    }
    for (i = 0; i < ZUKAN_INFO_MSG_COUNT; i++) {
        switch (i) {
        case ZUKAN_INFO_MSG_CATEGORIES:
            if (info->givenMsgs[0] == NULL) {
                GFL_MsgDataFree(info->msgs[i]);
            }
            break;
        case ZUKAN_INFO_MSG_HEIGHTS:
            if (info->givenMsgs[1] == NULL) {
                GFL_MsgDataFree(info->msgs[i]);
            }
            break;
        case ZUKAN_INFO_MSG_WEIGHTS:
            if (info->givenMsgs[2] == NULL) {
                GFL_MsgDataFree(info->msgs[i]);
            }
            break;
        default:
            GFL_MsgDataFree(info->msgs[i]);
            break;
        }
    }
    for (i = 0; i < ZUKAN_INFO_ARC_COUNT; i++) {
        GFL_ArcToolFree(info->arcs[i]);
    }
}

// pokebw2's ZukanInfo_GetFootprintArcId
static u32 func_ov206_021bc0ac(void) {
    return ARCID_POKE_FOOTPRINT;
}

// The first files of the archive are not species'
// pokebw2's ZukanInfo_GetFootprintFile
static u32 func_ov206_021bc0b0(u32 species) {
    return species + 5;
}

// pokebw2's ZukanInfo_GetFootprintPaletteFile
static u32 func_ov206_021bc0b4(void) {
    return 0;
}

// The cells of the generations before the fifth are smaller than those of the fifth
// pokebw2's ZukanInfo_GetFootprintCellFile
static u32 func_ov206_021bc0b8(int species) {
    if (species <= SPECIES_ARCEUS) {
        return 2;
    }
    return 4;
}

// pokebw2's ZukanInfo_GetFootprintAnimFile
static u32 func_ov206_021bc0cc(int species) {
    if (species <= SPECIES_ARCEUS) {
        return 1;
    }
    return 3;
}

// pokebw2's ZukanInfo_GetFootprintFormArcId
static u32 func_ov206_021bc0e0(void) {
    return ARCID_POKE_FOOTPRINT_FORM;
}

// The file of a form that has a footprint of its own, or 0xffff
// pokebw2's ZukanInfo_GetFootprintFormFile
u32 func_ov206_021bc0e4(u32 species, u32 form) {
    if (species == SPECIES_GIRATINA && form == 1) {
        return 0;
    }
    if (species == SPECIES_SHAYMIN && form == 1) {
        return 1;
    }
    return 0xffff;
}
