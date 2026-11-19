#include "global.h"
#include "sys_task.h"
#include "msgdata.h"
#include "overlay_70_sub_lcd_hero_demo.h"
#include "game_stats.h"

struct AdventureSave;

struct AppTaskMenu;

struct AppTaskMenuRes;

struct BagSave;

struct BmpMenuList;

struct BmpWin;

struct BoxPkm;

struct BoxSaveAccessor;

struct ClActUnit;

struct ClActor;

struct Font;

struct GameProcManager;

struct GameStats;

struct GameSystem;

struct KeyCursor;

struct ListMenuOption;

struct MsgData;

struct PartyPkm;

struct PlayerInfo;

struct PokeDexSave;

struct PokeParty;

struct PrintQueue;

struct PrintStream;

struct SaveControl;

struct StrBuf;

struct TCB;

struct TCBExManager;

struct TCBManager;

struct TrainerDataSave;

struct UnityTowerSurveySave;

struct WaitIcon;

struct WifiList;

struct WordSet;

struct WorldTradeData;

struct WorldTradeDepositWork;

struct WorldTradeInputWork;

struct WorldTradeNameCheck;

struct WorldTradePrint;

struct WorldTradePrintEntry;

struct WorldTradeSelectListPos;

struct WorldTradeWork;

typedef struct AdventureSave AdventureSave;

typedef struct AppTaskMenu AppTaskMenu;

typedef struct AppTaskMenuRes AppTaskMenuRes;

typedef struct BagSave BagSave;

typedef struct BmpWin BmpWin;

typedef struct BoxPkm BoxPkm;

typedef struct BoxSaveAccessor BoxSaveAccessor;

typedef struct GameSystem GameSystem;

typedef struct Font Font;

typedef struct GameProcManager GameProcManager;

typedef struct KeyCursor KeyCursor;

typedef struct PartyPkm PartyPkm;

typedef struct PlayerInfo PlayerInfo;

typedef struct PokeDexSave PokeDexSave;

typedef struct PokeParty PokeParty;

typedef struct PrintStream PrintStream;

typedef struct SaveControl SaveControl;

typedef struct StrBuf StrBuf;

typedef struct TCBExManager TCBExManager;

typedef struct TCBManager TCBManager;

typedef struct UnityTowerSurveySave UnityTowerSurveySave;

typedef struct WaitIcon WaitIcon;

typedef struct TCB TCB;

typedef struct TrainerDataSave TrainerDataSave;

typedef struct WifiList WifiList;

typedef struct WorldTradeData WorldTradeData;

typedef struct WordSet WordSet;

typedef struct ListMenuOption ListMenuOption;

typedef struct BmpMenuList BmpMenuList;

typedef struct WorldTradePrint WorldTradePrint;

typedef struct WorldTradePrintEntry WorldTradePrintEntry;

typedef struct WorldTradeDepositWork WorldTradeDepositWork;

typedef struct WorldTradeSelectListPos WorldTradeSelectListPos;

void GFL_HeapFree(void *ptr);

typedef struct ClActUnit ClActUnit;

typedef struct ClActor ClActor;

typedef struct {
    s16 x;
    s16 y;
} ClActorPos;

BOOL func_0204b98c(ClActor *actor);

void GFL_SndSEPlay(u32 se);

BOOL GFL_TCBRemove(TCB *tcb);

typedef struct {
    WorldTradeData *worldtrade_data;
    AdventureSave *systemdata;
    PokeParty *myparty;
    BoxSaveAccessor *mybox;
    PokeDexSave *pokedex;
    WifiList *wifilist;
    UnityTowerSurveySave *wifihistory;
    PlayerInfo *mystatus;
    TrainerDataSave *config;
    GameStats *record;
    BagSave *bag;
    BOOL isNationalDex;
    s32 profileId;
    u32 unk34;
    u32 unk38;
    SaveControl *savedata;
    GameSystem *gsys;
} WorldTradeParam;

typedef struct {
    s16 characterNo;
    s8 gender;
    s8 level_min;
    s8 level_max;
    s8 unused;
} Dpw_Tr_PokemonSearchData;

typedef struct {
    s16 characterNo;
    s8 gender;
    s8 level;
} Dpw_Tr_PokemonDataSimple;

typedef struct {

    u8 postData[0xec];
    Dpw_Tr_PokemonDataSimple postSimple;
    Dpw_Tr_PokemonSearchData wantSimple;

    u8 gender;
    u8 unkF7;
    u8 postDate[8];
    u8 tradeDate[8];
    s32 id;
    u32 trainerID;
    u16 name[8];
    u8 countryCode;
    u8 localCode;
    u8 trainerType;

    s8 isTrade;
    u8 versionCode;
    u8 langCode;
    u8 unk126;
    u8 unk127;
} Dpw_Tr_Data;

typedef struct {
    u8 version;
    u8 language;
    u8 country;
    u8 region;
    u32 playerId;
    u16 playerName[8];
    u32 unk18;
    u8 unk1C[8];

    char mailAddr[56];
    u32 unk5C;
    u8 unk60[4];
} Dpw_Common_Profile;

typedef struct {
    int code;
    int mailAddrAuthResult;
} Dpw_Common_ProfileResult;

typedef struct PrintQueue PrintQueue;

typedef struct {
    BmpWin *window;
    u8 flushPending;
} PrintWindow;

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

typedef struct WorldTradeWork WorldTradeWork;

typedef struct WorldTradeNameCheck {
    u16 name[8];
    u8 unk_10[8];
    const u16 *words;
    char result[4];
    int badWordCount;
    u16 unk24;
} WorldTradeNameCheck;

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
    WorldTradePrintEntry entries[24];
    KeyCursor *keyCursor;
};

struct WorldTradeSelectListPos {
    u16 headList;
    u16 headPos;
    u16 nameList[10];
    u16 namePos[10];
};

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
    int errorMesNo;
    int subprocessSeq;
    int subprocessNextSeq;
    u16 openingFlag;
    u16 depositFlag;
    u8 unk34[2];
    u16 serverWaitTime;
    int connectErrorNo;
    u8 unk3C[124];
    int subprocFlag;
    u16 listPos;
    u16 unkBE;
    u16 titleCursorPos;
    u16 unkC2;
    u16 boxTrayNo;
    u16 boxCursorPos;
    u32 unkC8;
    BoxPkm *pokePtr;
    int searchResult;
    u32 unkD4;
    void *unkD8;
    u32 unkDC;
    u32 unkE0;
    Dpw_Tr_Data uploadData;
    Dpw_Tr_Data downloadData[7];
    Dpw_Tr_Data exchangeData;
    Dpw_Tr_PokemonDataSimple post;
    Dpw_Tr_PokemonSearchData want;
    Dpw_Tr_PokemonSearchData search;
    Dpw_Tr_PokemonSearchData searchBackup;
    u8 unkB62[2];
    int searchBackupCountryCode;
    WordSet *wordSet;
    MsgData *msgManager;
    MsgData *monsNameManager;
    MsgData *lobbyMsgManager;
    MsgData *systemMsgManager;
    MsgData *countryNameManager;
    StrBuf *boxTrayNameString;
    StrBuf *endString;
    StrBuf *talkString;
    StrBuf *titleString;
    StrBuf *infoString[10];
    u8 unkBB8[8];
    ClActUnit *clactUnit;
    u32 clactRes[WT_CLACT_RES_SETS][WT_CLACT_RES_KINDS];
    ClActor *cursorAct;
    ClActor *subCursorAct;
    ClActor *fingerAct;
    ClActor *pokeIconAct[30];
    ClActor *itemIconAct[30];
    ClActor *cballAct[6];
    ClActor *pokemonAct;
    u8 pad_D0C[472];
    ClActor *subAct[8];
    ClActor *boxArrowAct[2];
    ClActor *partnerCursorAct;
    ClActor *promptDsAct;
    int drawOffset;
    BmpWin *msgWin;
    u8 unkD44[4];
    BmpWin *titleWin;
    BmpWin *subWin;
    BmpWin *menuWin[16];
    BmpWin *infoWin[16];
    BmpWin *talkWin;
    BmpWin *backWin;
    BmpWin *countryWin[2];
    BmpWin *explainWin;
    struct WorldTradeInputWork *inputWork;
    ListMenuOption *menuList;
    u8 unkDEC[8];
    BmpMenuList *bmpListWork;
    WaitIcon *timeWaitWork;
    int wait;
    struct WorldTradeDepositWork *depositWork;
    AppTaskMenuRes *task_res;
    AppTaskMenu *task_work;
    u16 saveNextSeq1st;
    u16 saveNextSeq2nd;
    int nameCheckNextSeq;
    TCB *demoTask;
    u8 pad_FF0[492];
    u16 demoEnd;
    u16 subLcdTouchOK;
    void *fieldObjCharaBuf[16];
    NNSG2dCharacterData *fieldObjCharaData[16];
    void *fieldObjPalBuf;
    NNSG2dPaletteData *fieldObjPalData;
    PartyPkm *sentPokemon;
    PartyPkm *demoPokemon;
    Dpw_Tr_PokemonDataSimple *boxWork;
    u16 boxPokeNum;
    u16 boxSearchFlag;
    int subOutFlag;
    WorldTradePokeBuf *boxIcon;
    WorldTradeVBlankFunc vfunc;
    WorldTradeVBlankFunc vfunc2;
    ClActorPos subActPos[10];
    WorldTradeSelectListPos selectListPos;
    Dpw_Common_Profile dcProfile;
    Dpw_Common_ProfileResult dcProfileResult;
    u32 countryCode;
    s16 localSeq;
    s16 localWait;
    u32 timeoutCount;
    TCB *vblankTask;
    TCBManager *tcbManager;
    void *heroDemoWork;
    void *tcbBuffer;
    WorldTradePrint print;
    void *subProcParam;
    void *evilCheck;
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
    BOOL checkEvolution;
    int partnerChange;
    WorldTradeNameCheck nameCheck;
    int evilCheckRetry;
};

typedef struct WorldTradeInputWork WorldTradeInputWork;

struct WorldTradeDepositWork {
    ListMenuOption *pokename;
    u16 headwordPos;
    u16 headwordListPos;
    u16 namePos;
    u16 nameListPos;
    int sexPos;
    int levelPos;

    u8 *sinouTable;

    u16 *nameSortTable;
    int nameSortNum;

    int sexSelection;
    int cursorSide;
    int leftCursorPos;
    int rightCursorPos;
};

void func_ov135_021ff914(SubLcdHeroDemo *demo, int seq);

void func_ov135_021ffaec(ClActor *act, int x, int y);

void ov70_02240F58(SysTask *tcb, void *data);

void ov70_02240F3C(SubLcdHeroDemo *demo, int seq);

void ov70_022410F0(ClActor *act, int x, int y);

int Sprite_IsAnimated(void *);

BOOL PlaySE(u16 sndseq);

void DestroySysTaskAndEnvironment(void *);

void ov70_02240F58(SysTask *tcb, void *data) {
    SubLcdHeroDemo *demo = data;
    WorldTradeWork *wk = demo->wk;

    switch (demo->seq) {
    case 0:
        if (demo->y > 160) {
            demo->y = 160;
            demo->seq = 1;
            ov70_02240F3C(demo, 1);
        }
        demo->y += 5;
        ov70_022410F0(wk->subAct[0], 128, demo->y);
        break;
    case 1:
        if (!Sprite_IsAnimated(wk->subAct[0])) {
            ov70_02240F3C(demo, 2);
            demo->seq = 2;
        }
        break;
    case 2:
        if (demo->y <= 138) {
            demo->y = 138;
            demo->seq = 3;
            ov70_02240F3C(demo, 3);
        } else {
            demo->y -= 2;
        }
        ov70_022410F0(wk->subAct[0], 128, demo->y);
        break;
    case 3:
        PlaySE(0x60C);
        demo->wk->demoEnd = 1;
        DestroySysTaskAndEnvironment(tcb);
        break;
    }
}
