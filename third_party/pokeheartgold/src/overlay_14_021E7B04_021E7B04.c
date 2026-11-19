#include "global.h"

struct AppTaskMenu;

struct AppTaskMenuRes;

struct ArcTool;

struct BGWinFrame;

struct BagSave;

struct BmpWin;

struct Box2AppWork;

struct Box2ButtonAnm;

struct Box2FontOam;

struct Box2IrqWork;

struct Box2ItemMoveWork;

struct Box2SysWork;

struct BoxPkm;

struct BoxSaveAccessor;

struct ClActUnit;

struct ClActor;

struct CursorMove;

struct Font;

struct GFLBitmap;

struct GameData;

struct GameProcManager;

struct MailData;

struct MsgData;

struct PMSData;

struct PaletteFade;

struct PartyPkm;

struct PlayerInfo;

struct PokeParty;

struct PrintQueue;

struct StrBuf;

struct TCB;

struct TrainerDataSave;

struct WordSet;

struct seq;

typedef struct AppTaskMenu AppTaskMenu;

typedef struct AppTaskMenuRes AppTaskMenuRes;

typedef struct ArcTool ArcTool;

typedef struct BagSave BagSave;

typedef struct BmpWin BmpWin;

typedef struct Box2AppWork Box2AppWork;

typedef struct Box2SysWork Box2SysWork;

typedef struct BoxPkm BoxPkm;

typedef struct BoxSaveAccessor BoxSaveAccessor;

typedef struct GameData GameData;

typedef struct GFLBitmap GFLBitmap;

typedef struct Font Font;

typedef struct GameProcManager GameProcManager;

typedef struct MsgData MsgData;

typedef struct PaletteFade PaletteFade;

typedef struct PartyPkm PartyPkm;

typedef struct PlayerInfo PlayerInfo;

typedef struct PMSData PMSData;

typedef struct PokeParty PokeParty;

typedef struct StrBuf StrBuf;

typedef struct TCB TCB;

typedef struct TrainerDataSave TrainerDataSave;

typedef struct WordSet WordSet;

typedef struct MailData MailData;

typedef struct BGWinFrame BGWinFrame;

typedef struct CursorMove CursorMove;

typedef struct Box2FontOam Box2FontOam;

typedef struct Box2IrqWork Box2IrqWork;

typedef struct Box2ButtonAnm Box2ButtonAnm;

typedef struct Box2ItemMoveWork Box2ItemMoveWork;

typedef struct {
    GameData *gameData;
    BoxSaveAccessor *boxes;
    PokeParty *party;
    BagSave *bag;
    PlayerInfo *playerInfo;
    u32 unk14;
    TrainerDataSave *trainerData;
    u32 unk1C;

    u8 *unk20;
    u32 mode;
    u16 unk28;

    u8 tray;
    u8 position;
} Box2Param;

typedef struct ClActUnit ClActUnit;

typedef struct ClActor ClActor;

typedef struct PrintQueue PrintQueue;

typedef struct {
    BmpWin *window;
    u8 flushPending;
} PrintWindow;

typedef struct {
    StrBuf *str;
    u16 color;
    u32 type;
} AppTaskMenuItem;

struct Box2SysWork {
    Box2Param *param;
    GameProcManager *procManager;
    BOOL procMgrResult;
    void *subProcWork;
    u16 subRet;
    u8 subProcType;
    u8 unk13;
    u8 tray;
    u8 trayMax;
    u8 pos;
    u8 getTray;
    u8 unk18;
    u8 trayScroll;
    u8 unk1A;
    u8 unk1B;
    u8 moveMode : 4;
    u8 unk1C_4 : 4;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u32 curRcvPos;
    int nextSeq;
    u8 pad_2C[8];
    Box2AppWork *app;
};

struct Box2FontOam {
    void *oam;
    GFLBitmap *bitmap;
};

typedef BOOL (*Box2VFunc)(Box2SysWork *syswk);

struct Box2IrqWork {
    Box2VFunc func;
    Box2VFunc freq;
    void *work;
    u16 seq;
    u16 cnt;
};

struct Box2ButtonAnm {
    u8 mode : 1;
    u8 id : 7;
    u8 pal1 : 4;
    u8 pal2 : 4;
    u8 seq;
    u8 cnt;
    u8 px;
    u8 py;
    u8 sx;
    u8 sy;
};

struct Box2AppWork {
    void *bmpOam;
    struct Box2IrqWork vfunk;
    u32 unk68;
    PaletteFade *palFade;
    CursorMove *cursorMove;
    BGWinFrame *bgWinFrame;
    Font *font;
    Font *smallFont;
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *expandBuf;
    PrintQueue *printQueue;
    u8 unk90[4];
    PrintWindow windows[28];
    u8 flushChar[4];
    u32 cursorChars;
    BOOL nationalDex;
    u16 *regionalDex;
    AppTaskMenuItem yesNoItems[2];
    AppTaskMenuRes *yesNoRes;
    AppTaskMenu *yesNoMenu;
    u16 ynID;
    struct Box2ButtonAnm bawk;
    ArcTool *pokeIconArc;
    u8 pokeIconChar[30][512];
    u8 unk3DB4[0x1E];
    u8 pokeIconId[0x25];
    u8 unk3DF7;
    u32 unk3DF8[(0x3E70 - 0x3DF8) / 4];
    u8 trayIconChar[3][0x400];
    u8 unk4A70[0x9E70 - 0x4A70];
    ClActUnit *unk9E70;
    ClActor *unk9E74[96];
    u32 unk9FF4[0x45];
    u32 unkA108[11];
    u32 unkA134[12];
    u16 unkA164;
    u8 unkA166[2];
    u32 curPos;
    u16 itemId;
    u16 unkA16E;
    u8 unkA170[0x2];
    u8 unkA172;
    u8 moveErr;
    u32 unkA174;
    s8 wallPx;
    u8 wallArea;
    u8 unkA17A[2];
    u8 unkA17C;
    u8 unkA17D;
    u8 pokePutKey;
    u8 unkA17F;
    u32 tpx;
    u32 tpy;
    u32 oldCurPos;
    u32 unkA18C;
    void *unkA190;
    u32 unkA194;
    u32 unkA198;
};

struct Box2ItemMoveWork {
    u16 putPos;
    u16 setPos;
    u32 mvMode;
    u32 nowX;
    u32 nowY;
    u32 mx;
    u32 my;
    u32 mvX : 1;
    u32 mvY : 1;
    u32 cnt : 30;
};

void ov14_021F396C(Box2AppWork *app, u32 pos, u32 mode);

void ov14_021F39A0(Box2AppWork *app, u32 pos, u32 mode);

void ov14_021F3B5C(Box2AppWork *app);

void ov14_021F395C(Box2AppWork *app, s16 x, s16 y);

struct PMSData {

    u16 type;
    u16 id;
    u16 words[2];
};

struct MailData {
    u32 trainerId;
    u8 trainerGender;
    u8 region;
    u8 gameVersion;
    u8 unk07;
    u16 trainerName[8];
    u16 unk18[3];

    u16 unk1E;
    PMSData message[3];
};

typedef struct {
    u8 rawData[32];
} PkmBufferChunk;

typedef struct {
    PkmBufferChunk chunks[4];
} PkmBuffer;

struct BoxPkm {
    u32 pid;
    u16 partyDecrypted : 1;
    u16 boxDecrypted : 1;
    u16 badEgg : 1;
    u16 unk4_3 : 13;
    u16 checksum;
    PkmBuffer contentBuffer;
};

struct PartyPkm {
    BoxPkm base;
    u32 statusCond;
    u8 level;
    u8 unk8D;
    u16 nowHP;
    u16 maxHP;
    u16 atk;
    u16 def;
    u16 spe;
    u16 spa;
    u16 spd;
    MailData mail;
    u32 unkD4;
    u32 unkD8;
};

struct PokeParty {
    s32 capacity;
    s32 count;
    PartyPkm pkms[6];
    u8 slotExists;
};

typedef struct seq seq;

BOOL ov14_021E7B04(Box2SysWork *syswk, BOOL hand);

BOOL ov14_021E7B04(Box2SysWork *syswk, BOOL hand) {
    Box2ItemMoveWork *work = syswk->app->vfunk.work;
    if (work->cnt == 8) {
        if (hand == 1) {
            ov14_021F396C(syswk->app, work->putPos, work->mvMode);
        } else {
            ov14_021F39A0(syswk->app, work->putPos, work->mvMode);
        }
        ov14_021F3B5C(syswk->app);
        return 0;
    }
    if (work->mvX == 0) {
        work->nowX = work->nowX + work->mx;
    } else {
        work->nowX -= work->mx;
    }
    if (work->mvY == 0) {
        work->nowY = work->nowY + work->my;
    } else {
        work->nowY -= work->my;
    }
    ov14_021F395C(syswk->app, (s16)(work->nowX >> 8), (s16)(work->nowY >> 8));
    ov14_021F3B5C(syswk->app);
    work->cnt++;
    return 1;
}
