#include "global.h"
#include "msgdata.h"

struct AppTaskMenu;

struct AppTaskMenuRes;

struct AppTaskMenuWin;

struct BmpWin;

struct BoxPkm;

struct ButtonMan;

struct ClActUnit;

struct ClActor;

struct DWCFriendData;

struct DWCUserData;

struct Font;

struct GameData;

struct IrcBattleMatch;

struct IrcBattleMatchParam;

struct MailData;

struct MsgData;

struct NetHandle;

struct PMSData;

struct PartyPkm;

struct PokeParty;

struct PrintQueue;

struct StrBuf;

struct TCB;

struct WordSet;

typedef struct AppTaskMenu AppTaskMenu;

typedef struct AppTaskMenuRes AppTaskMenuRes;

typedef struct BmpWin BmpWin;

typedef struct BoxPkm BoxPkm;

typedef struct ButtonMan ButtonMan;

typedef struct GameData GameData;

typedef struct Font Font;

typedef struct NetHandle NetHandle;

typedef struct PartyPkm PartyPkm;

typedef struct PMSData PMSData;

typedef struct PokeParty PokeParty;

typedef struct StrBuf StrBuf;

typedef struct TCB TCB;

typedef struct WordSet WordSet;

typedef struct MailData MailData;

typedef struct AppTaskMenuWin AppTaskMenuWin;

typedef struct IrcBattleMatchParam IrcBattleMatchParam;

typedef u16 HeapID;

typedef struct {
    u8 serverMac[6];
    u8 mac[6];
    u8 netId;
    u8 valid;
    u8 unkE;
    u8 unkF;
} NetNegotiation;

struct NetHandle {
    void *data;
    u32 dataSize;
    NetNegotiation *negotiations[9];
    struct {
        u8 netId;
        u8 pending;
    } replies[8];
    u8 unk3C[2];

    u16 negotiationsSent;
    u16 serverNegotiationsSent;
    u8 unk42[2];
    u16 timingRecv;
    u16 timingSend;
    u8 unk48;
    u8 timingSendPending;
    u8 state;
    u8 unk4B[3];
    u8 infoPending;
    u8 unk4F;
};

typedef struct {

    const void *commandTable;
    int commandNum;

    void (*disconnectCallback)(void *work, int netId);

    void (*negotiationCallback)(void *work, int netId);

    void *(*getInfo)(void *work);
    int (*getInfoSize)(void *work);
    void *(*unk18)(void *work);
    int (*unk1C)(void *work);
    void *unk20;
    void (*unk24)(NetHandle *handle, int a1, void *work);
    void (*unk28)(NetHandle *handle, int a1, void *work);
    void (*unk2C)(void *work);
    void (*unk30)(void *work);

    union {
        u8 unk34[0xc];
        struct {
            u8 unk34_0[8];
            void (*callback)(int code, int param, void *work);
        } wifiError;
    } unk34;

    struct DWCFriendData *(*getFriendData)(void *work);
    struct DWCUserData *(*getUserData)(void *work);

    u32 wifiHeapSize;

    union {
        u8 unk4C[8];
        struct {
            u32 unk4C_0;
            u32 gameId;
        } id;
    } unk4C;
    HeapID parentHeapId;
    HeapID heapId;
    HeapID wifiHeapId;
    HeapID ircHeapId;

    u16 iconX;
    u16 iconY;
    u8 maxConnectNum;

    u8 maxSendSize;
    u8 unk62;
    u8 unk63;

    u8 bMPMode;

    u8 bNetType;
    u8 unk66;

    u8 gameCommandBase;
    union {
        u8 bytes[4];
        u32 word;
    } unk68;
    u16 unk6C;
    u16 unk6E;
} GFLNetInitData;

struct IrcBattleMatchParam {
    GameData *gameData;
    PokeParty *party;

    PokeParty *parties[4];

    u8 order[4];

    int result;

    const GFLNetInitData *musicalNetInit;
};

typedef struct ClActUnit ClActUnit;

typedef struct ClActor ClActor;

void Sprite_DeleteAndFreeResources(ClActor *actor);

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

typedef struct DWCUserData DWCUserData;

typedef struct DWCFriendData DWCFriendData;

typedef struct PrintQueue PrintQueue;

typedef struct {
    StrBuf *str;
    u16 color;
    u32 type;
} AppTaskMenuItem;

enum {
    IRCMATCH_ACTOR_0,
    IRCMATCH_ACTOR_1,
    IRCMATCH_ACTOR_2,
    IRCMATCH_ACTOR_3,
    IRCMATCH_ACTOR_4,
    IRCMATCH_ACTOR_5,
    IRCMATCH_ACTOR_6,
    IRCMATCH_ACTOR_7,
    IRCMATCH_ACTOR_COUNT,
};

typedef struct IrcBattleMatch IrcBattleMatch;

typedef void (*IrcBattleMatchState)(IrcBattleMatch *work);

struct IrcBattleMatch {
    u8 unk_0[312];
    ClActor *actors[IRCMATCH_ACTOR_COUNT];
    u32 acceptedPlayers;
    BOOL enoughPlayers;
    u32 unk2F0[3];
    u32 bgChars;
    u32 unk300;
    u32 windowFrameChars;
    u32 menuBgChars;
    TCB *unk30C;
    u32 unk310;
    u8 unk314[0xc7c - 0x314];
    BOOL linked;
    BOOL messagePending;
    BOOL bannerActive;
    s32 bannerFrames;
    BOOL linkChanged;
    u32 friendResult;
    BOOL linkChangePending;
    s32 linkChangeFrames;
    s32 scrollY;
    BOOL connectDone;
    ButtonMan *buttonMan;
    u32 unkCA8;
    s32 counter;
    u32 currentMsgId;
    void *paletteBackup;
    PrintQueue *printQueue;
    BOOL backOnly;
    AppTaskMenu *menu;
    AppTaskMenuRes *menuRes;
    AppTaskMenuItem menuItems[2];
    AppTaskMenuWin *taskWin0;
    AppTaskMenuWin *taskWin1;
    s32 shownCount;
    BOOL palettePulse;
};

void ov64_021E6010(IrcBattleMatch *work, int slot);

void ov64_021E6010(IrcBattleMatch *work, int slot) {
    if (work->actors[slot] != ((void *)0)) {
        Sprite_DeleteAndFreeResources(work->actors[slot]);
        work->actors[slot] = ((void *)0);
    }
}
