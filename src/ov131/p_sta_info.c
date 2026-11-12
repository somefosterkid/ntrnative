// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov207/p_sta_info.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "constants/arc.h"
#include "constants/met_locations.h"
#include "constants/pokemon.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/heap.h"
#include "gfl/str.h"
#include "p_status_local.h"
#include "pml/met_data.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "pml/species_names.h"
#include "system/app_menu_common.h"
#include "system/game_data.h"
#include "app/p_status.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/msg.h"
#include "system/printsys.h"
#include "system/wordset.h"

// The summary screen's info page: the Pokédex number, species, original trainer, ID and experience, and the trainer
// memo, which says where and when the Pokémon was met and what its nature and best individual value say about it. The
// names of the fields and functions are guesses

#define INFO_WINDOW_COUNT 10
// What a regional Pokédex number is for a species that isn't in it
#define DEX_NUMBER_NONE 999
#define LEVEL_MAX 100
// The width in pixels of the experience bar
#define EXP_BAR_WIDTH 64

// The message files of the memo and of the place names
enum {
    INFO_MSG_MEMO,
    INFO_MSG_PLACES_UNOVA,
    INFO_MSG_PLACES_SPECIAL,
    INFO_MSG_PLACES_EVENT,
    INFO_MSG_PLACES_EXTERNAL,
    INFO_MSG_COUNT,
};

// Black 2's, not checked against Black
typedef struct {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} InfoWindowSetup;

struct PStaInfoWork {
    BOOL isPrinted;
    BOOL isShown;
    u16 *regionalDex;
    BmpWin *windows[INFO_WINDOW_COUNT];
    BmpWin *memoWindow;
    MsgData *msgData[INFO_MSG_COUNT];
    PStaScreen screens[4];
};

static void func_ov131_021d7f04(PStatusWork *wk, PStaInfoWork *info, BoxPkm *pkm);
static void func_ov131_021d8288(PStatusWork *wk, PStaInfoWork *info, BoxPkm *pkm);
static StrBuf *func_ov131_021d8784(PStatusWork *wk, PStaInfoWork *info, u32 location);

// pokebw2's sWindows
static const PStaWindowSetup data_ov131_021dc288[INFO_WINDOW_COUNT] = {
    { 2, 1, 16, 2 },  { 2, 3, 16, 2 },  { 2, 5, 8, 2 },   { 2, 7, 16, 2 },  { 2, 9, 16, 2 },
    { 2, 11, 17, 2 }, { 10, 13, 8, 2 }, { 2, 15, 16, 2 }, { 6, 17, 12, 2 }, { 10, 19, 8, 1 },
};

void func_ov131_021d6214(PStatusWork *wk, BmpWin *window, WordSet *wordSet, u32 msgId, u16 x, u16 y, u16 color);

// pokebw2's PStaInfo_Create
PStaInfoWork *func_ov131_021d7b00(PStatusWork *wk) {
    PStaInfoWork *info = GFL_HeapAllocate(wk->heapId, sizeof(PStaInfoWork), FALSE, "p_sta_info.c", 193);

    info->isPrinted = FALSE;
    info->isShown = FALSE;
    info->regionalDex = PML_PersonalLoadRegionalDexTable(wk->heapId, 0);
    return info;
}

// pokebw2's PStaInfo_Free
void func_ov131_021d7b34(PStatusWork *wk, PStaInfoWork *info) {
    GFL_HeapFree(info->regionalDex);
    GFL_HeapFree(info);
}

// pokebw2's PStaInfo_Main
void func_ov131_021d7b48(PStatusWork *wk, PStaInfoWork *info) {
}

void func_ov131_021d7b4c(PStatusWork *wk, PStaInfoWork *info, ArcTool *arc) {
    info->screens[0].file = GFL_G2DIOReadNSCRArc(arc, 63, FALSE, &info->screens[0].screen, wk->heapId);
    info->screens[1].file = GFL_G2DIOReadNSCRArc(arc, 72, FALSE, &info->screens[1].screen, wk->heapId);
    info->screens[2].file = GFL_G2DIOReadNSCRArc(arc, 64, FALSE, &info->screens[2].screen, wk->heapId);
    info->screens[3].file = GFL_G2DIOReadNSCRArc(arc, 65, FALSE, &info->screens[3].screen, wk->heapId);
    info->msgData[INFO_MSG_MEMO] = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_MEMO, wk->heapId);
    info->msgData[INFO_MSG_PLACES_UNOVA] =
        GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, PLACE_FILE_UNOVA, wk->heapId);
    info->msgData[INFO_MSG_PLACES_SPECIAL] =
        GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, PLACE_FILE_SPECIAL, wk->heapId);
    info->msgData[INFO_MSG_PLACES_EVENT] =
        GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, PLACE_FILE_EVENT, wk->heapId);
    info->msgData[INFO_MSG_PLACES_EXTERNAL] =
        GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, PLACE_FILE_EXTERNAL, wk->heapId);
}


// pokebw2's PStaInfo_FreeResources
void func_ov131_021d7bf0(PStatusWork *wk, PStaInfoWork *info) {
    if (info->isShown == TRUE) {
        func_ov131_021d7e84(wk, info);
    }
    GFL_MsgDataFree(info->msgData[INFO_MSG_MEMO]);
    GFL_MsgDataFree(info->msgData[INFO_MSG_PLACES_UNOVA]);
    GFL_MsgDataFree(info->msgData[INFO_MSG_PLACES_SPECIAL]);
    GFL_MsgDataFree(info->msgData[INFO_MSG_PLACES_EVENT]);
    GFL_MsgDataFree(info->msgData[INFO_MSG_PLACES_EXTERNAL]);
    GFL_HeapFree(info->screens[0].file);
    GFL_HeapFree(info->screens[1].file);
    GFL_HeapFree(info->screens[2].file);
    GFL_HeapFree(info->screens[3].file);
}

// pokebw2's PStaInfo_Load
void func_ov131_021d7c38(PStatusWork *wk, PStaInfoWork *info) {
    BoxPkm *pkm = func_ov131_021d6064(wk);
    u8 i;

    for (i = 0; i < INFO_WINDOW_COUNT; i++) {
        info->windows[i] =
            BmpWin_CreateDynamic(1, data_ov131_021dc288[i].x, data_ov131_021dc288[i].y, data_ov131_021dc288[i].width, data_ov131_021dc288[i].height, 14, 1);
    }
    info->memoWindow = BmpWin_CreateDynamic(4, 4, 5, 24, 20, 14, 1);
    func_ov131_021d7f04(wk, info, pkm);
    func_ov131_021d8288(wk, info, pkm);
    info->isShown = TRUE;
}

void func_ov131_021d7cac(PStatusWork *wk, PStaInfoWork *info) {
    BoxPkm *pkm = func_ov131_021d6064(wk);
    u8 i;
    BmpWin *window;
    u32 type1;
    u32 type2;
    ClActorPos pos;
    NNSG2dImageProxy proxy1;
    NNSG2dImageProxy proxy2;

    if (wk->isEgg == FALSE) {
        GFL_BGSysLoadScrArea(2, 0, 0, 32, 24, info->screens[0].screen->rawData, 0, 0, 32, 32);
    } else {
        GFL_BGSysLoadScrArea(2, 0, 0, 32, 24, info->screens[1].screen->rawData, 0, 0, 32, 32);
    }
    GFL_BGSysQueueScrLoad(2);
    GFL_BGSysBufferScrDefault(6, info->screens[2].screen->rawData, info->screens[2].screen->szByte);
    GFL_BGSysQueueScrLoad(6);
    GFL_BGSysLoadScrArea(5, 0, 0, 32, 3, info->screens[3].screen->rawData, 0, 0, 32, 32);
    GFL_BGSysQueueScrLoad(5);
    for (i = 0; i < INFO_WINDOW_COUNT; i++) {
        window = info->windows[i];
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    }
    window = info->memoWindow;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));

    if (wk->isEgg == FALSE) {
        type1 = PML_PkmGetParam(pkm, PKM_PARAM_TYPE1, NULL);
        type2 = PML_PkmGetParam(pkm, PKM_PARAM_TYPE2, NULL);
        func_0204ab84(wk->typeIconChars[type1], &proxy1);
        func_0204b784(wk->typeIcons[0], &proxy1);
        func_0204b6f4(wk->typeIcons[0], func_020275ec(type1), 1);
        pos.x = 96;
        pos.y = 48;
        func_0204b404(wk->typeIcons[0], &pos, 0);
        func_0204b3dc(wk->typeIcons[0], TRUE);
        func_0204b84c(wk->typeIcons[0], 1);
        if (type1 != type2) {
            func_0204ab84(wk->typeIconChars[type2], &proxy2);
            func_0204b784(wk->typeIcons[1], &proxy2);
            func_0204b6f4(wk->typeIcons[1], func_020275ec(type2), 1);
            pos.x = 130;
            pos.y = 48;
            func_0204b404(wk->typeIcons[1], &pos, 0);
            func_0204b3dc(wk->typeIcons[1], TRUE);
            func_0204b84c(wk->typeIcons[1], 1);
        } else {
            func_0204b3dc(wk->typeIcons[1], FALSE);
        }
    } else {
        func_0204b3dc(wk->typeIcons[0], FALSE);
        func_0204b3dc(wk->typeIcons[1], FALSE);
    }
    func_0204b878(wk->unk178[0], 3);
}


void func_ov131_021d7e84(PStatusWork *wk, PStaInfoWork *info) {
    u8 i;

    for (i = 0; i < INFO_WINDOW_COUNT; i++) {
        BmpWin_Free(info->windows[i]);
    }
    BmpWin_Free(info->memoWindow);
    info->isShown = FALSE;
}

void func_ov131_021d7eac(PStatusWork *wk, PStaInfoWork *info) {
    GFL_BGSysFillScrArea(1, 0, 0, 0, 19, 21, 16);
    GFL_BGSysQueueScrLoad(1);
    GFL_BGSysFillScrAsync(4, 0);
    GFL_BGSysQueueScrLoad(4);
    func_0204b3dc(wk->typeIcons[0], FALSE);
    func_0204b3dc(wk->typeIcons[1], FALSE);
    func_0204b878(wk->unk178[0], 0);
}

inline StrBuf *PStaInfo_LoadSpeciesName(u32 species) {
    return GFL_MsgDataLoadStrbufNew(g_PMLSpeciesNamesResident, species);
}
static void func_ov131_021d7f04(PStatusWork *wk, PStaInfoWork *info, BoxPkm *pkm) {
    WordSet *wordSet;
    u32 dexNumber;
    u16 color;
    StrBuf *str;
    StrBuf *name;
    WordSet *nameWordSet;
    u32 sex;
    u16 id;
    u32 level;
    u32 exp;
    u32 nextExp;
    u32 levelExp;
    u32 toNext;
    u32 gained;
    u32 width;

    if (wk->isEgg == FALSE) {
        func_ov131_021d618c(wk, info->windows[0], 8, 1, 1, PRINT_COLOR(15, 2, 0));
        wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
        dexNumber = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
        color = PRINT_COLOR(1, 2, 0);
        if (wk->param->isNationalDex == FALSE) {
            dexNumber = info->regionalDex[dexNumber];
        }
        if (PML_PkmIsRare(pkm) == TRUE) {
            color = PRINT_COLOR(3, 4, 0);
        }
        if (dexNumber == DEX_NUMBER_NONE) {
            func_ov131_021d6214(wk, info->windows[0], wordSet, 22, 65, 1, color);
        } else {
            WordSetNumber(wordSet, 0, dexNumber, 3, 2, 1);
            func_ov131_021d6214(wk, info->windows[0], wordSet, 9, 65, 1, color);
        }
        GFL_WordSetSystemFree(wordSet);

        func_ov131_021d618c(wk, info->windows[1], 10, 1, 1, PRINT_COLOR(15, 2, 0));
        if (wk->isEgg == FALSE) {
            str = PStaInfo_LoadSpeciesName(PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL));
        } else {
            str = PStaInfo_LoadSpeciesName(SPECIES_EGG);
        }
        func_0201c724(wk->printQueue, BmpWin_GetBitmap(info->windows[1]), 65, 1, str, wk->font, PRINT_COLOR(1, 2, 0));
        GFL_StrBufFree(str);

        func_ov131_021d618c(wk, info->windows[2], 12, 1, 1, PRINT_COLOR(15, 2, 0));
        func_ov131_021d618c(wk, info->windows[3], 13, 1, 1, PRINT_COLOR(15, 2, 0));
        name = GFL_StrBufCreate(32, wk->heapId);
        nameWordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
        color = PRINT_COLOR(1, 2, 0);
        sex = PML_PkmGetParam(pkm, PKM_PARAM_OT_GENDER, name);
        if (sex == GENDER_MALE) {
            color = PRINT_COLOR(5, 6, 0);
        } else if (sex == GENDER_FEMALE) {
            color = PRINT_COLOR(3, 4, 0);
        }
        PML_PkmGetParam(pkm, PKM_PARAM_OT_NAME, name);
        func_0201ee0c(nameWordSet, 0, name, 0, 1, 2);
        func_ov131_021d6214(wk, info->windows[3], nameWordSet, 14, 65, 1, color);
        GFL_WordSetSystemFree(nameWordSet);
        GFL_StrBufFree(name);

        func_ov131_021d618c(wk, info->windows[4], 15, 1, 1, PRINT_COLOR(15, 2, 0));
        wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
        id = PML_PkmGetParam(pkm, PKM_PARAM_ID, NULL);
        WordSetNumber(wordSet, 0, id, 5, 2, 1);
        func_ov131_021d6214(wk, info->windows[4], wordSet, 16, 65, 1, PRINT_COLOR(1, 2, 0));
        GFL_WordSetSystemFree(wordSet);

        level = PML_PkmGetParam(pkm, PKM_PARAM_LEVEL, NULL);
        exp = PML_PkmGetParam(pkm, PKM_PARAM_EXP, NULL);
        nextExp = PML_UtilGetPkmLvExp(PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL),
                                      PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL), level + 1);
        levelExp = PML_UtilGetPkmLvExp(PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL),
                                       PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL), level);
        toNext = nextExp - exp;
        if (level == LEVEL_MAX) {
            toNext = 0;
        }
        func_ov131_021d618c(wk, info->windows[5], 17, 1, 1, PRINT_COLOR(15, 2, 0));
        wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
        WordSetNumber(wordSet, 0, exp, 7, 0, 1);
        func_ov131_021d6214(wk, info->windows[6], wordSet, 18, 1, 1, PRINT_COLOR(1, 2, 0));
        GFL_WordSetSystemFree(wordSet);
        func_ov131_021d618c(wk, info->windows[7], 19, 1, 1, PRINT_COLOR(15, 2, 0));
        func_ov131_021d618c(wk, info->windows[8], 20, 1, 1, PRINT_COLOR(1, 2, 0));
        wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
        WordSetNumber(wordSet, 0, toNext, 6, 0, 1);
        func_ov131_021d6214(wk, info->windows[8], wordSet, 21, 33, 1, PRINT_COLOR(1, 2, 0));
        GFL_WordSetSystemFree(wordSet);
        if (level != LEVEL_MAX) {
            gained = nextExp - levelExp - toNext;
            width = gained * EXP_BAR_WIDTH / (nextExp - levelExp);
            if (gained != 0 && width == 0) {
                width = 1;
            }
            if (width == EXP_BAR_WIDTH) {
                width--;
            }
            GFL_BitmapFillArea(BmpWin_GetBitmap(info->windows[9]), 0, 3, width, 3, 5);
        }
    }
    info->isPrinted = TRUE;
}




// pokebw2's PStaInfo_PrintMemo
static void func_ov131_021d8288(PStatusWork *wk, PStaInfoWork *info, BoxPkm *pkm) {
    u32 y = 0;
    BOOL foreignOT;
    BOOL isEgg = PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    u32 nature = PML_PkmGetParam(pkm, PKM_PARAM_NATURE, NULL);
    u32 happiness = PML_PkmGetParam(pkm, PKM_PARAM_HAPPINESS, NULL);
    u32 eggYear = PML_PkmGetParam(pkm, PKM_PARAM_EGG_YEAR, NULL);
    u32 eggMonth = PML_PkmGetParam(pkm, PKM_PARAM_EGG_MONTH, NULL);
    u32 eggDay = PML_PkmGetParam(pkm, PKM_PARAM_EGG_DAY, NULL);
    u32 metYear = PML_PkmGetParam(pkm, PKM_PARAM_MET_YEAR, NULL);
    u32 metMonth = PML_PkmGetParam(pkm, PKM_PARAM_MET_MONTH, NULL);
    u32 metDay = PML_PkmGetParam(pkm, PKM_PARAM_MET_DAY, NULL);
    u32 metLevel = PML_PkmGetParam(pkm, PKM_PARAM_MET_LEVEL, NULL);
    u32 fateful = PML_PkmGetParam(pkm, PKM_PARAM_FATEFUL_ENCOUNTER, NULL);
    u32 originGame = PML_PkmGetParam(pkm, PKM_PARAM_ORIGIN_GAME, NULL);
    u32 pid = PML_PkmGetParam(pkm, PKM_PARAM_PID, NULL);
    u32 eggLocation = PML_PkmGetParam(pkm, PKM_PARAM_EGG_LOCATION, NULL);
    u32 metLocation = PML_PkmGetParam(pkm, PKM_PARAM_MET_LOCATION, NULL);
    BOOL transfer0 = special_transfers(pkm, 0);
    BOOL transfer1 = special_transfers(pkm, 1);
    BOOL transfer2 = special_transfers(pkm, 2);
    BOOL transfer3 = special_transfers(pkm, 3);
    BOOL transferred = transfer0 | transfer1 | transfer2 | transfer3;
    StrBuf *str;
    StrBuf *buf;
    WordSet *wordSet;
    u16 msg;
    u16 originMsg;
    StrBuf *eggPlace;
    StrBuf *metPlace;

    foreignOT = PML_UtilCheckForeignOT(pkm, GetGameDataPlayerInfo(wk->param->gameData));
    if (isEgg == FALSE) {
        str = GFL_MsgDataLoadStrbufNew(info->msgData[INFO_MSG_MEMO], nature);
        func_0201c724(wk->printQueue, BmpWin_GetBitmap(info->memoWindow), 1, 1, str, wk->font, PRINT_COLOR(1, 2, 0));
        GFL_StrBufFree(str);
        y = 16;
    }
    buf = GFL_StrBufCreate(256, wk->heapId);
    wordSet = GFL_WordSetSystemCreate(10, 32, wk->heapId);
    if (isEgg == FALSE) {
        if (metLocation == LOCATION_POKE_TRANSFER) {
            originMsg = 0x29;
            msg = 0x2b;
            if (fateful == TRUE) {
                originMsg = 0x2d;
                msg = 0x2f;
            }
            switch (originGame) {
            case VERSION_SAPPHIRE:
            case VERSION_RUBY:
            case VERSION_EMERALD:
                metLocation = LOCATION_HOENN;
                break;
            case VERSION_FIRERED:
            case VERSION_LEAFGREEN:
                metLocation = LOCATION_KANTO;
                break;
            case VERSION_HEARTGOLD:
            case VERSION_SOULSILVER:
                metLocation = LOCATION_JOHTO;
                msg = originMsg;
                break;
            case VERSION_DIAMOND:
            case VERSION_PEARL:
            case VERSION_PLATINUM:
                metLocation = LOCATION_SINNOH;
                msg = originMsg;
                break;
            case VERSION_COLOSSEUM:
                metLocation = LOCATION_FARAWAY;
                break;
            default:
                // Sets the egg's place, though the memo names the met place. Both names are blank, so it shows the same
                eggLocation = LOCATION_UNKNOWN;
                msg = originMsg;
                break;
            }
        } else if (metLocation >= LOCATION_EVENT_CELEBI && metLocation <= LOCATION_EVENT_BEASTS_USED &&
                   transferred == TRUE) {
            switch (metLocation - LOCATION_EVENT_CELEBI) {
            case 0:
                msg = 0x31;
                break;
            case 1:
                msg = 0x32;
                break;
            case 2:
                msg = 0x33;
                break;
            case 3:
                msg = 0x34;
                break;
            }
        } else if (fateful == FALSE) {
            if (eggLocation == LOCATION_NONE) {
                if (metLocation == LOCATION_IN_GAME_TRADE) {
                    msg = 0x1b;
                } else {
                    msg = 0x19;
                }
            } else if (eggLocation <= LOCATION_EXTERNAL_BASE) {
                msg = 0x1d;
            } else {
                msg = 0x1f;
            }
        } else {
            msg = 0x21;
            if (eggLocation != LOCATION_NONE) {
                msg = 0x23;
            }
        }
        if (foreignOT == FALSE && (metLocation < LOCATION_EVENT_CELEBI || metLocation > LOCATION_EVENT_BEASTS_USED)) {
            msg++;
        }
    } else if (fateful == FALSE) {
        if (foreignOT == TRUE) {
            msg = 0x53;
        } else {
            msg = 0x54;
        }
    } else {
        msg = 0x55;
        if (foreignOT != TRUE) {
            msg = 0x56;
        }
    }
    eggPlace = func_ov131_021d8784(wk, info, eggLocation);
    metPlace = func_ov131_021d8784(wk, info, metLocation);
    {
        u8 best;

        WordSetNumber(wordSet, 0, metYear, 2, 2, 1);
        // The highest IV's zero is copied from best's stack slot, so best is cleared here and maxIV starts from it
        best = 0;
        WordSetNumber(wordSet, 1, metMonth, 2, 0, 1);
        WordSetNumber(wordSet, 2, metDay, 2, 0, 1);
        WordSetNumber(wordSet, 3, metLevel, 3, 0, 1);
        func_0201ee0c(wordSet, 4, metPlace, 0, 1, 2);
        WordSetNumber(wordSet, 5, eggYear, 2, 2, 1);
        WordSetNumber(wordSet, 6, eggMonth, 2, 0, 1);
        WordSetNumber(wordSet, 7, eggDay, 2, 0, 1);
        func_0201ee0c(wordSet, 8, eggPlace, 0, 1, 2);
        str = GFL_MsgDataLoadStrbufNew(info->msgData[INFO_MSG_MEMO], msg);
        GFL_WordSetFormatStrbuf(wordSet, buf, str);
        func_0201c724(wk->printQueue, BmpWin_GetBitmap(info->memoWindow), 1, y + 1, buf, wk->font,
                      PRINT_COLOR(1, 2, 0));
        GFL_WordSetSystemFree(wordSet);
        y += func_0201d2c8(buf) * 16;
        GFL_StrBufFree(eggPlace);
        GFL_StrBufFree(metPlace);
        GFL_StrBufFree(str);
        GFL_StrBufFree(buf);
        if (isEgg == FALSE) {
            u8 maxIV = best;
            u32 params[6] = {
                PKM_PARAM_IV_HP,    PKM_PARAM_IV_ATTACK,    PKM_PARAM_IV_DEFENSE,
                PKM_PARAM_IV_SPEED, PKM_PARAM_IV_SP_ATTACK, PKM_PARAM_IV_SP_DEFENSE,
            };
            // The order in which the values are compared, by personality, so that a tie goes to a different stat
            u8 orders[6][6] = {
                { 0, 1, 2, 3, 4, 5 }, { 1, 2, 3, 4, 5, 0 }, { 2, 3, 4, 5, 0, 1 },
                { 3, 4, 5, 0, 1, 2 }, { 4, 5, 0, 1, 2, 3 }, { 5, 0, 1, 2, 3, 4 },
            };
            u8 i;
            u8 row = pid % 6;
            u32 iv;

            for (i = 0; i < 6; i++) {
                iv = PML_PkmGetParam(pkm, params[orders[row][i]], NULL);
                if (maxIV < iv) {
                    maxIV = iv;
                    best = orders[row][i];
                }
            }
            str = GFL_MsgDataLoadStrbufNew(info->msgData[INFO_MSG_MEMO], maxIV % 5 + (0x35 + best * 5));
            func_0201c724(wk->printQueue, BmpWin_GetBitmap(info->memoWindow), 1, y + 1, str, wk->font,
                          PRINT_COLOR(1, 2, 0));
            GFL_StrBufFree(str);
        } else {
            u32 hatchMsg;

            if (happiness <= 5) {
                hatchMsg = 0x57;
            } else if (happiness <= 10) {
                hatchMsg = 0x58;
            } else if (happiness <= 40) {
                hatchMsg = 0x59;
            } else {
                hatchMsg = 0x5a;
            }
            str = GFL_MsgDataLoadStrbufNew(info->msgData[INFO_MSG_MEMO], hatchMsg);
            func_0201c724(wk->printQueue, BmpWin_GetBitmap(info->memoWindow), 1, y + 1, str, wk->font,
                          PRINT_COLOR(1, 2, 0));
            GFL_StrBufFree(str);
        }
    }
}

// pokebw2's PStaInfo_GetPlaceName
static StrBuf *func_ov131_021d8784(PStatusWork *wk, PStaInfoWork *info, u32 location) {
    u32 file = func_0202f688(location);
    u32 index = func_0202f6d8(location);

    switch (file) {
    case PLACE_FILE_UNOVA:
        return GFL_MsgDataLoadStrbufNew(info->msgData[INFO_MSG_PLACES_UNOVA], index);
    case PLACE_FILE_SPECIAL:
        return GFL_MsgDataLoadStrbufNew(info->msgData[INFO_MSG_PLACES_SPECIAL], index);
    case PLACE_FILE_EVENT:
        return GFL_MsgDataLoadStrbufNew(info->msgData[INFO_MSG_PLACES_EVENT], index);
    case PLACE_FILE_EXTERNAL:
        return GFL_MsgDataLoadStrbufNew(info->msgData[INFO_MSG_PLACES_EXTERNAL], index);
    }
    return GFL_MsgDataLoadStrbufNew(info->msgData[INFO_MSG_PLACES_UNOVA], 0);
}
