// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_local.h, the lines in use so
// far, with Black's names: pokebw2's WorldTrade_ names are its own guesses.
#ifndef BWPORT_OV135_WORLDTRADE_LOCAL_H
#define BWPORT_OV135_WORLDTRADE_LOCAL_H

#include "types.h"
#include "app/worldtrade.h"
#include "dpw/dpw_tr.h"
#include "gfl/clact.h"
#include "nnsys/g2d.h"
#include "struct_decls.h"
#include "system/app_taskmenu.h"
#include "system/printsys.h"

// The Global Trade Station's files

// What the proc's main function does
enum {
    WT_SEQ_INIT,
    WT_SEQ_FADEIN,
    WT_SEQ_MAIN,
    WT_SEQ_FADEOUT,
    WT_SEQ_OUT,
    WT_SEQ_END,
};

// The screens, each with an init, main and end function
enum {
    WORLDTRADE_ENTER,
    WORLDTRADE_TITLE,
    WORLDTRADE_MYPOKE,
    WORLDTRADE_PARTNER,
    WORLDTRADE_SEARCH,
    WORLDTRADE_MYBOX,
    WORLDTRADE_DEPOSIT,
    WORLDTRADE_UPLOAD,
    WORLDTRADE_STATUS,
    WORLDTRADE_DEMO,
};

// The rows and columns of WorldTradeWork's clactRes
enum {
    WT_CLACT_RES_MAIN,
    WT_CLACT_RES_SUB,
    WT_CLACT_RES_HERO,
    WT_CLACT_RES_MAIN2,
    WT_CLACT_RES_SETS,
};

enum {
    WT_CLACT_RES_CHAR,
    WT_CLACT_RES_PLTT,
    WT_CLACT_RES_CELL,
    WT_CLACT_RES_KINDS,
};

// The boxes: 24 of 30 Pokemon each
#define BOX_COUNT 24
#define BOX_POKE_NUM 30

typedef struct WorldTradeWork WorldTradeWork;

// The name check of the name the Dpw server checks (pokebw2's WorldTradeNameCheck)
typedef struct WorldTradeNameCheck {
    u16 name[8];
    u8 unk_10[8];
    const u16 *words;
    char result[4];
    int badWordCount;
    u16 unk24;
} WorldTradeNameCheck;

// Cut short to the fields in use, at Black's offsets
#define PRINT_ENTRY_COUNT 24

struct WorldTradePrintEntry {
    PrintWindow printWin;
    BOOL active;
};

struct WorldTradePrint {
    Font *font;
    TCBExManager *tcbManager;
    TrainerDataSave *config;
    PrintQueue *printQueue;
    PrintStream *stream;
    BmpWin *streamWin;
    WorldTradePrintEntry entries[PRINT_ENTRY_COUNT];
    KeyCursor *keyCursor;
};

// The cursor of each list of the name input, kept between the deposit and search screens
struct WorldTradeSelectListPos {
    u16 headList;
    u16 headPos;
    u16 nameList[10];
    u16 namePos[10];
};

// A box Pokemon's icon: where its graphics go in the VRAM, its palette, the actor, and the graphics themselves
typedef struct {
    u32 charOffset;
    u32 palNo;
    ClActor *icon;
    u8 charData[0x200];
} WorldTradePokeBuf;

typedef void (*WorldTradeVBlankFunc)(WorldTradeWork *wk);

struct WorldTradeWork {
    WorldTradeParam *param;
    u8 unk4[12];
    int subProcess;
    int subNextProcess;
    int subReturnProcess;
    int oldSubProcess;
    int subProcessMode;
    // The message printed for an error (pokebw2's errorMesNo)
    int errorMesNo;
    int subprocessSeq;
    // The step the next screen starts at (pokebw2's subprocessNextSeq)
    int subprocessNextSeq;
    u16 openingFlag;
    // Whether the Pokemon is deposited (pokebw2's depositFlag)
    u16 depositFlag;
    u8 unk34[2];
    // The wait for the server, in frames (pokebw2's serverWaitTime)
    u16 serverWaitTime;
    int connectErrorNo;
    u8 unk3C[124];
    // Set while the status screen or the trade demo runs, which takes the cell actors
    int subprocFlag;
    // The list position, set to -1 when a list is opened (pokebw2's listpos)
    u16 listPos;
    u16 unkBE;
    u16 titleCursorPos;
    u16 unkC2;
    u16 boxTrayNo;
    u16 boxCursorPos;
    u32 unkC8;
    // The Pokemon of the box cursor's position (pokebw2's WorldTrade_GetPokePtr's result)
    BoxPkm *pokePtr;
    // How many people a search found (pokebw2's searchResult)
    int searchResult;
    u32 unkD4;
    void *unkD8;
    u32 unkDC;
    u32 unkE0;
    // The upload data, whose isTrade says whether the trade was made
    Dpw_Tr_Data uploadData;
    // The trade data of the download screen, and the one the Pokemon is traded with (pokebw2's downloadPokemonData
    // and exchangePokemonData)
    Dpw_Tr_Data downloadData[7];
    Dpw_Tr_Data exchangeData;
    // The Pokemon being deposited, the one it is deposited for, and the search and the last one (pokebw2's post, want,
    // search and searchBackup)
    Dpw_Tr_PokemonDataSimple post;
    Dpw_Tr_PokemonSearchData want;
    Dpw_Tr_PokemonSearchData search;
    Dpw_Tr_PokemonSearchData searchBackup;
    u8 unkB62[2];
    int searchBackupCountryCode;
    // The word set and the string of the message printed on the window (pokebw2's wordSet and talkString)
    WordSet *wordSet;
    MsgData *msgManager;
    // The message files of the Pokemon names, the lobby, the system and the countries (pokebw2's monsNameManager,
    // lobbyMsgManager, systemMsgManager and countryNameManager)
    MsgData *monsNameManager;
    MsgData *lobbyMsgManager;
    MsgData *systemMsgManager;
    MsgData *countryNameManager;
    // The strings of a box's name, of the quit button and of the screen's title (pokebw2's boxTrayNameString,
    // endString and titleString)
    StrBuf *boxTrayNameString;
    StrBuf *endString;
    StrBuf *talkString;
    StrBuf *titleString;
    StrBuf *infoString[10];
    u8 unkBB8[8];
    ClActUnit *clactUnit;
    // The cell actors' resources of each screen: characters, palette and cell animations
    u32 clactRes[WT_CLACT_RES_SETS][WT_CLACT_RES_KINDS];
    // The actors of the box screen's cursor, of the sub cursor and of the finger that points at the quit button
    // (pokebw2's cursorAct, subCursorAct and fingerAct)
    ClActor *cursorAct;
    ClActor *subCursorAct;
    ClActor *fingerAct;
    // The box screen's Pokemon icons, their items and their balls, and the Pokemon's picture (pokebw2's pokeIconAct,
    // itemIconAct, cballAct and pokemonAct)
    ClActor *pokeIconAct[BOX_POKE_NUM];
    ClActor *itemIconAct[BOX_POKE_NUM];
    ClActor *cballAct[6];
    ClActor *pokemonAct;
    // The lower screen's actors: the player, then the people that stand for the trainers found
    ClActor *subAct[8];
    // The actors of the box arrows (pokebw2's boxArrowAct)
    ClActor *boxArrowAct[2];
    ClActor *partnerCursorAct;
    ClActor *promptDsAct;
    int drawOffset;
    // The message window (pokebw2's msgWin)
    BmpWin *msgWin;
    u8 unkD44[4];
    // The windows of the screen's title, its sub window, its menu, its info, its talk and back windows, the search's
    // country and the explanation (pokebw2's titleWin, subWin, menuWin, infoWin, talkWin, backWin, countryWin and
    // explainWin)
    BmpWin *titleWin;
    BmpWin *subWin;
    BmpWin *menuWin[16];
    BmpWin *infoWin[16];
    BmpWin *talkWin;
    BmpWin *backWin;
    BmpWin *countryWin[2];
    BmpWin *explainWin;
    // The input the name, sex, level and nation are picked with (pokebw2's inputWork)
    struct WorldTradeInputWork *inputWork;
    // The list of the menu, its list work and the wait icon (pokebw2's menuList, bmpListWork and timeWaitWork)
    ListMenuOption *menuList;
    u8 unkDEC[8];
    BmpMenuList *bmpListWork;
    WaitIcon *timeWaitWork;
    // Set to 0 when a message is printed (pokebw2's wait)
    int wait;
    // The deposit and search screens' work (pokebw2's dw)
    struct WorldTradeDepositWork *depositWork;
    // The buttons' resources and the buttons of the box and name screens (pokebw2's task_res and task_work), and the
    // steps after the two halves of a save (pokebw2's saveNextSeq1st and saveNextSeq2nd)
    AppTaskMenuRes *task_res;
    AppTaskMenu *task_work;
    u16 saveNextSeq1st;
    u16 saveNextSeq2nd;
    // The step the name check goes to once the name is checked (pokebw2's nameCheckNextSeq)
    int nameCheckNextSeq;
    TCB *demoTask;
    u16 demoEnd;
    u16 subLcdTouchOK;
    void *fieldObjCharaBuf[16];
    NNSG2dCharacterData *fieldObjCharaData[16];
    void *fieldObjPalBuf;
    NNSG2dPaletteData *fieldObjPalData;
    // A copy of the Pokemon the player trades away, and the Pokemon of the trade demo (pokebw2's sentPokemon and
    // demoPokemon)
    PartyPkm *sentPokemon;
    PartyPkm *demoPokemon;
    // The box page's Pokemon, as the server describes them (pokebw2's boxWork)
    Dpw_Tr_PokemonDataSimple *boxWork;
    // The Pokemon the boxes hold (pokebw2's boxPokeNum)
    u16 boxPokeNum;
    u16 boxSearchFlag;
    int subOutFlag;
    WorldTradePokeBuf *boxIcon;
    // Called once at the next VBlank, and at every VBlank (pokebw2's vfunc and vfunc2)
    WorldTradeVBlankFunc vfunc;
    WorldTradeVBlankFunc vfunc2;
    // Where each of subAct stands
    ClActorPos subActPos[10];
    // The cursor of each list of the name input
    WorldTradeSelectListPos selectListPos;
    // The player's profile, which the Dpw server keeps, and the result it gives back (pokebw2's dcProfile and
    // dcProfileResult)
    Dpw_Common_Profile dcProfile;
    Dpw_Common_ProfileResult dcProfileResult;
    // The country of the search (pokebw2's countryCode)
    u32 countryCode;
    s16 localSeq;
    s16 localWait;
    // The frames spent waiting for the server, and the task that runs at every VBlank (pokebw2's timeoutCount and
    // vblankTask)
    u32 timeoutCount;
    TCB *vblankTask;
    TCBManager *tcbManager;
    void *heroDemoWork;
    void *tcbBuffer;
    // The print state that the message printing takes (pokebw2's print)
    WorldTradePrint print;
    void *subProcParam;
    // The evil check of the Pokemon being uploaded (pokebw2's evilCheck)
    void *evilCheck;
    // The evil check's result: its status, its result and the signature it sends back
    u8 evilCheckStatus;
    u8 unk10E5[3];
    int evilCheckResult;
    u8 evilCheckSign[0x80];
    u32 wifiLoginBuffer;
    u8 unk1170[0x170];
    u32 unk12E0;
    u32 unk12E4;
    int unk12E8;
    int loginSeq;
    GameProcManager *procManager;
    BOOL procResult;
    // Set when the demo's evolution check is wanted
    BOOL checkEvolution;
    int partnerChange;
    WorldTradeNameCheck nameCheck;
    // The retries of the evil check (pokebw2's evilCheckRetry)
    int evilCheckRetry;
};

// The init, main and end function of each screen (pokebw2's WorldTrade_Enter_Init and the like), which worldtrade.c's
// table of screens lists. The demo's take the step as well, which they never read
typedef int (*WorldTradeSubProcFunc)(WorldTradeWork *wk, int seq);
int func_ov135_021f89b4(WorldTradeWork *wk, int seq);
int func_ov135_021f89f0(WorldTradeWork *wk, int seq);
int func_ov135_021f8a18(WorldTradeWork *wk, int seq);
int func_ov135_021fc06c(WorldTradeWork *wk, int seq);
int func_ov135_021fc174(WorldTradeWork *wk, int seq);
int func_ov135_021fc188(WorldTradeWork *wk, int seq);
int func_ov135_021fd924(WorldTradeWork *wk, int seq);
int func_ov135_021fdae4(WorldTradeWork *wk, int seq);
int func_ov135_021fdb74(WorldTradeWork *wk, int seq);
int func_ov135_021f5154(WorldTradeWork *wk, int seq);
int func_ov135_021f51f0(WorldTradeWork *wk, int seq);
int func_ov135_021f5204(WorldTradeWork *wk, int seq);
int func_ov135_02200c0c(WorldTradeWork *wk, int seq);
int func_ov135_02200c80(WorldTradeWork *wk, int seq);
int func_ov135_02200c94(WorldTradeWork *wk, int seq);
int func_ov135_021f6e58(WorldTradeWork *wk, int seq);
int func_ov135_021f6fd4(WorldTradeWork *wk, int seq);
int func_ov135_021f7268(WorldTradeWork *wk, int seq);

// worldtrade_sublcd.c: the lower screen's actors, and the people that stand for the trainers a search found
void func_ov135_021ff6a4(WorldTradeWork *wk);
void func_ov135_021ffb20(WorldTradeWork *wk, int count, int appear);

// pokebw2's WorldTrade_SelBoxInit (worldtrade.c)
// Makes the column of buttons of the menu list, count of them, with its bottom at y
void func_ov135_021f4d50(WorldTradeWork *wk, u8 frame, int count, int y);
// worldtrade.c: pokebw2's WorldTrade_SubProcessChange, WorldTrade_SubProcessUpdate, WorldTrade_InitGraphics and
// WorldTrade_ExitGraphics, and an actor's position set
void func_ov135_021f4ec0(WorldTradeWork *wk, int subProcess, int mode);
void func_ov135_021f4ec8(WorldTradeWork *wk);
void func_ov135_021f5074(WorldTradeWork *wk);
void func_ov135_021f50ec(WorldTradeWork *wk);
// Sets an actor's position on the lower screen
void func_ov135_021f4ea0(ClActor *actor, int x, int y);

// worldtrade_box.c: whether the tray is the party, a Pokémon of a tray and the number of them (pokebw2's
// WorldTrade_GetPPorPPP, WorldTrade_GetPokePtr and WorldTrade_GetBoxPokeNum)
BOOL func_ov135_021f6b34(int tray);
BoxPkm *func_ov135_021f6b40(PokeParty *party, BoxSaveAccessor *box, int tray, int pos);
// Whether the Pokemon holds mail (pokebw2's WorldTrade_PokemonMailCheck)
BOOL func_ov135_021f6de8(PartyPkm *pkm);
// Prints a message of the screen's message window (worldtrade_enter.c, pokebw2's Enter_MessagePrint)
void func_ov135_021f9088(WorldTradeWork *wk, MsgData *msgManager, int msgNo, int wait, u16 dat);
// The same, printed at once (pokebw2's Enter_MessagePrintNoStream)
void func_ov135_021f90a0(WorldTradeWork *wk, MsgData *msgManager, int msgNo, int wait, u16 dat);
// worldtrade_adapter.c: the party Pokemon made of a box Pokemon, and the box Pokemon copied into a party one
void func_ov135_022026a0(BoxPkm *pkm, PartyPkm *dest);
BoxPkm *func_ov135_022026bc(PartyPkm *pkm);
int func_ov135_021f6b80(PokeParty *party, BoxSaveAccessor *box, int tray);

// worldtrade_status.c: pokebw2's WorldTrade_Status_Init, WorldTrade_Status_Main and WorldTrade_Status_End
int func_ov135_021ff58c(WorldTradeWork *wk, int seq);
int func_ov135_021ff65c(WorldTradeWork *wk, int seq);
int func_ov135_021ff67c(WorldTradeWork *wk, int seq);


typedef struct WorldTradeInputWork WorldTradeInputWork;

// What worldtrade_input.c's input asks for; the modes from INPUT_MODE_HEADWORD_1 on are its own steps
enum {
    INPUT_MODE_POKEMON_NAME,
    INPUT_MODE_SEX,
    INPUT_MODE_LEVEL,
    INPUT_MODE_NATION,
    INPUT_MODE_HEADWORD_1,
    INPUT_MODE_HEADWORD_2,
    INPUT_MODE_NATION_HEAD1,
    INPUT_MODE_NATION_HEAD2,
};

// Black 2's; twinned code confirms the values of INPUT_SITUATION_SEARCH in Black
// The screen that uses the input
enum {
    INPUT_SITUATION_DEPOSIT,
    INPUT_SITUATION_SEARCH,
};

// worldtrade_adapter.c's numbers, printed with a font of digits
typedef struct WorldTradeNumFont WorldTradeNumFont;

// What worldtrade_input.c's input draws with
struct WorldTradeInputHeader {
    BmpWin **menuWin;
    BmpWin **backWin;
    ClActor *cursorAct;
    ClActor *arrowAct[2];
    // Only for the search screen
    ClActor *searchCursorAct;
    MsgData *msgManager;
    MsgData *monsNameManager;
    MsgData *countryNameManager;
    PokeDexSave *zukan;
    u8 *sinouTable;
    TrainerDataSave *config;
};

// pokebw2's WorldTrade_PrintInit
void func_ov135_022026d8(WorldTradePrint *print, TrainerDataSave *config);
// pokebw2's WorldTrade_PrintExit
void func_ov135_02202744(WorldTradePrint *print);
// pokebw2's WorldTrade_NumFontDelete
void func_ov135_022029d4(WorldTradeNumFont *numFont);
// pokebw2's WorldTrade_PrintMain
void func_ov135_02202770(WorldTradePrint *print);
// pokebw2's WorldTrade_NumFontMain
void func_ov135_022029f8(WorldTradeNumFont *numFont);
// pokebw2's WorldTrade_LevelListAdd
int func_ov135_021f87e0(ListMenuOption **menulist, MsgData *msgManager, int tblSelect);
// pokebw2's WorldTrade_NationSortListNumGet
int func_ov135_021f88d8(int start, int *number);
// pokebw2's WorldTrade_Input_Start
void func_ov135_021f9968(WorldTradeInputWork *wk, int type);
// pokebw2's WorldTrade_Input_Exit
void func_ov135_021f99d0(WorldTradeInputWork *wk);
// pokebw2's WorldTrade_Input_Main
// The value chosen, or -1 while the input runs or -2 when it was cancelled
u32 func_ov135_021f99f0(WorldTradeInputWork *wk);

// How many Pokémon a search returns
#define SEARCH_POKE_MAX 7

// Which table of levels a wanted level is from
#define LEVEL_PRINT_TBL_DEPOSIT 0

// pokebw2's WorldTrade_TransPokeGraphic
// Uploads the Pokémon's front sprite to the main screen's OBJ characters
void func_ov135_021fcb54(PartyPkm *pkm);
// pokebw2's WorldTrade_SetPartnerCursorPos
// Puts the partner cursor on a person of the lower screen, offset by y
void func_ov135_021ffd1c(WorldTradeWork *wk, int index, int offsetY);
// pokebw2's WorldTrade_SubLcdBgExit
void func_ov135_022000e4(WorldTradeWork *wk);
// pokebw2's WorldTrade_ExpandMessage
// A message with the word set's words put in
StrBuf *func_ov135_02202670(WordSet *wordSet, MsgData *msgData, u32 msgNo, HeapID heapId);
extern const u32 data_ov135_02202eb0[];
// pokebw2's WorldTrade_SetNextSeq
void func_ov135_021f4e98(WorldTradeWork *wk, int toSeq, int nextSeq);
// pokebw2's WorldTrade_SubLcdObjHitCheck
// The person of the lower screen that is touched, of the first count, or -1
int func_ov135_021ffb00(int count);
// pokebw2's WorldTrade_TouchSwMain
u8 func_ov135_021f4d14(WorldTradeWork *wk);
// pokebw2's WorldTrade_TouchWinYesNoDel
void func_ov135_021f4ce4(WorldTradeWork *wk);
s32 func_ov135_021f4e30(WorldTradeWork *wk);
void func_ov135_021f4e64(WorldTradeWork *wk);
// pokebw2's WorldTrade_PrintIsBusy
// Whether the message stream still prints
BOOL func_ov135_02202840(WorldTradePrint *print);
// pokebw2's WorldTrade_Print
void func_ov135_02202858(BmpWin *win, u8 font, StrBuf *str, int x, int y, WorldTradePrint *print);
void func_ov135_02200bac(WorldTradeWork *wk, s32 msgId);
void func_ov135_021fffe8(WorldTradeWork *wk, int a2, BOOL a3);
// pokebw2's WorldTrade_PokeNamePrintNoPut
void func_ov135_021f7fd4(BmpWin *win, MsgData *nameManager, int monsno, int y, u16 color,
                                   WorldTradePrint *print);
// pokebw2's WorldTrade_SexPrintNoPut
void func_ov135_021f8104(BmpWin *win, MsgData *msgManager, int sex, int flag, int x, int y, u16 color,
                              WorldTradePrint *print);
// pokebw2's WorldTrade_WantLevelPrint_XY
void func_ov135_021f8178(BmpWin *win, MsgData *msgManager, int level, int flag, int x, int y, u16 color,
                                  int tblSelect, WorldTradePrint *print);
// pokebw2's WorldTrade_Partner_Init
// worldtrade_partner.c
int func_ov135_021fcc40(WorldTradeWork *wk, int seq);
// pokebw2's WorldTrade_Partner_Main
int func_ov135_021fce64(WorldTradeWork *wk, int seq);
// pokebw2's WorldTrade_Partner_End
int func_ov135_021fced4(WorldTradeWork *wk, int seq);

// A search's gender that takes either
#define SEARCH_GENDER_ANY 3

// How many levels a search can ask for
#define SEARCH_LEVEL_SELECT_NUM 11

// Black 2's; twinned code confirms the offsets of headwordPos, headwordListPos, namePos, nameListPos, sinouTable,
// nameSortTable, nameSortNum, sexSelection, cursorSide, leftCursorPos and rightCursorPos in Black
// The deposit and search screens' work
struct WorldTradeDepositWork {
    ListMenuOption *pokename;
    u16 headwordPos;
    u16 headwordListPos;
    u16 namePos;
    u16 nameListPos;
    int sexPos;
    int levelPos;
    // Whether each species is of the regional Pokédex
    u8 *sinouTable;
    // The species in the order of their names
    u16 *nameSortTable;
    int nameSortNum;
    // The gender ratio of the Pokémon chosen
    int sexSelection;
    int cursorSide;
    int leftCursorPos;
    int rightCursorPos;
};

// pokebw2's WorldTrade_WifiIconAdd
void func_ov135_021f9254(WorldTradeWork *wk);
// pokebw2's WorldTrade_ClearWindow
// Clears a window from the screen, now or at the next VBlank
void func_ov135_02202644(BmpWin *win, int mode);
// pokebw2's WorldTrade_TouchWinYesNoMake
// worldtrade.c
void func_ov135_021f4be0(WorldTradeWork *wk, int y, int cgx, int palette, u8 passive);
// pokebw2's WorldTrade_Deposit_Init
// worldtrade_deposit.c
int func_ov135_021f73dc(WorldTradeWork *wk, int seq);
// pokebw2's WorldTrade_Deposit_End
int func_ov135_021f74f8(WorldTradeWork *wk, int seq);
// pokebw2's WorldTrade_SexSelectionCheck
BOOL func_ov135_021f7b9c(Dpw_Tr_PokemonSearchData *dtps, int sexSelection);
// pokebw2's WorldTrade_PokeNamePrint
// pokebw2's WorldTrade_PokeNamePrint
void func_ov135_021f7f9c(BmpWin *win, MsgData *msgManager, int msgNo, int flag, int y, u16 color, WorldTradePrint *print);
// pokebw2's WorldTrade_SysPrint (worldtrade_enter.c)
void func_ov135_021f91a0(BmpWin *win, StrBuf *str, int x, int y, int flag, u16 color, WorldTradePrint *print);
// pokebw2's WorldTrade_PrintColor (worldtrade_adapter.c)
void func_ov135_022028c8(BmpWin *win, u8 font, StrBuf *str, int x, int y, u8 align, u16 color, WorldTradePrint *print);
// pokebw2's WorldTrade_CountryPrint
void func_ov135_021f8010(BmpWin *win, MsgData *mgr, MsgData *mgr2, int a, int b, int c, u16 d, WorldTradePrint *print);
// pokebw2's WorldTrade_SexPrint
void func_ov135_021f8098(BmpWin *win, MsgData *mgr, int a, int b, int c, int d, u16 e, WorldTradePrint *print);
// pokebw2's WorldTrade_LevelTermGet
int func_ov135_021f886c(int min, int max, int tblSelect);
// pokebw2's WorldTrade_WantLevelPrint
void func_ov135_021f8154(BmpWin *win, MsgData *msgManager, int level, int flag, int y, u16 color, int tblSelect,
                             WorldTradePrint *print);
// pokebw2's WorldTrade_PokeWantPrint
void func_ov135_021f81c8(MsgData *msgManager, MsgData *monsNameManager, WordSet *wordSet, BmpWin **win, int monsno,
                              int sex, int level, WorldTradePrint *print);
// pokebw2's WorldTrade_MyPokeWantPrint
void func_ov135_021f8290(MsgData *msgManager, MsgData *monsNameManager, WordSet *wordSet, BmpWin **win, int monsno,
                         int sex, int level, WorldTradePrint *print);
// pokebw2's WorldTrade_PokeInfoPrint
void func_ov135_021f8358(MsgData *msgManager, WordSet *wordSet, BmpWin **win, BoxPkm *pkm,
                              Dpw_Tr_PokemonDataSimple *post, WorldTradePrint *print);
// pokebw2's WorldTrade_CountryCodeSet
void func_ov135_021f88b0(WorldTradeWork *wk, int countryCode);
// pokebw2's WorldTrade_SelectListPosInit
void func_ov135_021f8990(WorldTradeSelectListPos *slp);
// pokebw2's WorldTrade_SelectNameListBackup
void func_ov135_021f89a8(WorldTradeSelectListPos *slp, int head, int list, int pos);

// pokebw2's WorldTrade_SetPartnerExchangePos
// Puts the people back where they stand, or 32 pixels lower
void func_ov135_021ffd44(WorldTradeWork *wk);
// pokebw2's WorldTrade_SubLcdMatchObjHide
// Hides the people of the lower screen that a search found
void func_ov135_021ffbc8(WorldTradeWork *wk);
// pokebw2's WorldTrade_BoxPokeNumGetStart
void func_ov135_021f4f90(WorldTradeWork *wk);
// pokebw2's WorldTrade_ReturnHeroDemo
void func_ov135_021ff9e0(WorldTradeWork *wk);
// pokebw2's WorldTrade_ExplainPrint
void func_ov135_021f9218(BmpWin *win, MsgData *msgManager, int no, WorldTradePrint *print);
// pokebw2's WorldTrade_Title_Init
// worldtrade_title.c
int func_ov135_021ffda8(WorldTradeWork *wk, int seq);
// pokebw2's WorldTrade_Title_End
int func_ov135_021ffec8(WorldTradeWork *wk, int seq);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WorldTrade_CLACT_PosChange
void func_ov135_021f4ff4(ClActor *act, int x, int y);
// pokebw2's WorldTrade_NumFontCreate
WorldTradeNumFont *func_ov135_02202974(u32 unused0, u32 unused1, u32 unused2, HeapID heapId);
// pokebw2's WorldTrade_ZukanSortDataGet
u16 *func_ov135_021f8498(int heapId, int idx, int *num);
// pokebw2's WorldTrade_HeadwordRangeGet
void func_ov135_021f84bc(int select, int *start, int *end);
// pokebw2's WorldTrade_NationSortListMake
int func_ov135_021f8908(ListMenuOption **menulist, MsgData *countryNameManager, int start);
// pokebw2's WorldTrade_NumFontPrintNumber
// Prints a number of digits at x and y of a window
void func_ov135_02202a50(WorldTradeNumFont *numFont, int num, int digits, int dispType, BmpWin *win, int x,
                                   int y);
// pokebw2's WorldTrade_NumFontPrintSlash
// Prints the slash between two numbers
void func_ov135_02202b20(WorldTradeNumFont *numFont, int unused, BmpWin *win, int x, int y);
// pokebw2's WorldTrade_Input_Init
// worldtrade_input.c
WorldTradeInputWork *func_ov135_021f9890(WorldTradeInputHeader *header, int frame, int situation);
// pokebw2's WorldTrade_MyPokeInfoPrint
// Prints a Pokémon's nickname, gender, species, level and item into seven windows
void func_ov135_021fc8cc(MsgData *msgManager, MsgData *monsNameManager, WordSet *wordSet, BmpWin **win,
                                BoxPkm *pkm, Dpw_Tr_PokemonDataSimple *post, WorldTradePrint *print);
// pokebw2's WorldTrade_PokeInfoPrint2
// Prints the Pokémon's owner and its original trainer
void func_ov135_021fca8c(MsgData *param_1, BmpWin **param_2, u16 *param_3, PartyPkm *param_4, BmpWin **param_5, WorldTradePrint *param_6);
// pokebw2's WorldTrade_TouchWinYesNoMakeEx
void func_ov135_021f4bf8(WorldTradeWork *wk, int y, int cgx, int palette, int frame, u8 passive);
// pokebw2's WorldTrade_TouchPrint
void func_ov135_021f91dc(BmpWin *win, StrBuf *str, int x, int y, int flag, u16 color, WorldTradePrint *print);
// pokebw2's WorldTrade_SubLcdWinGraphicSet
void func_ov135_02200b34(WorldTradeWork *wk);
// pokebw2's WorldTrade_Deposit_Main
int func_ov135_021f74e4(WorldTradeWork *wk, int seq);
// pokebw2's WorldTrade_SinouZukanDataGet
u8 *func_ov135_021f84ec(int heapId);
// pokebw2's WorldTrade_PostPokemonBaseDataMake
void func_ov135_021f8554(Dpw_Tr_Data *dtd, WorldTradeWork *wk);
// pokebw2's WorldTrade_PokeNameListMake
BmpMenuList *func_ov135_021f875c(WorldTradeWork *wk, ListMenuOption **menulist, BmpWin *win,
                                         MsgData *msgManager, MsgData *monsNameManager, WorldTradeDepositWork *dw,
                                         PokeDexSave *zukan);
// pokebw2's WorldTrade_LevelMinMaxSet
void func_ov135_021f882c(Dpw_Tr_PokemonSearchData *dtps, int index, int tblSelect);
// pokebw2's WorldTrade_HeroDemo
// Walks the player into the trade room, or out of it
void func_ov135_021ff8b0(WorldTradeWork *wk);
// pokebw2's WorldTrade_GetTalkSpeed
int func_ov135_021f4ed4(WorldTradeWork *wk);
// pokebw2's WorldTrade_Title_Main
int func_ov135_021ffeb4(WorldTradeWork *wk, int seq);
// pokebw2's WorldTrade_SubLcdBgGraphicSet
void func_ov135_02200ab4(WorldTradeWork *wk);

#endif // BWPORT_OV135_WORLDTRADE_LOCAL_H
