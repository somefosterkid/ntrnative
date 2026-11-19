#include "global.h"
#include "overlay_85_ov109_021D5140.h"
#include "msgdata.h"
#include "game_stats.h"

struct ArcTool;

struct BmpCursor;

struct BmpMenu;

struct BmpWin;

struct ClActUnit;

struct ClActor;

struct Font;

struct GameData;

struct GameStats;

struct Guru2CommWork;

struct Guru2MainWork;

struct Guru2ProcWork;

struct ListMenuOption;

struct MsgData;

struct PlayerInfo;

struct PrintQueue;

struct PrintStream;

struct StrBuf;

struct TCB;

struct TCBExManager;

struct UnionApp;

struct UnionAppStatus;

struct WordSet;

typedef struct ArcTool ArcTool;

typedef struct BmpCursor BmpCursor;

typedef struct BmpMenu BmpMenu;

typedef struct BmpWin BmpWin;

typedef struct GameData GameData;

typedef struct Font Font;

typedef struct PlayerInfo PlayerInfo;

typedef struct PrintStream PrintStream;

typedef struct StrBuf StrBuf;

typedef struct TCBExManager TCBExManager;

typedef struct TCB TCB;

typedef struct WordSet WordSet;

typedef struct ListMenuOption ListMenuOption;

typedef struct UnionApp UnionApp;

typedef struct UnionAppStatus UnionAppStatus;

struct PlayerInfo {
    u16 name[8];
    u32 id;

    s32 profileId;
    u8 country;
    u8 province;
    u8 region;

    u8 version;
    u8 unk1C;
    u8 gender;
    u8 unk1E[2];
};

typedef u16 HeapID;

typedef struct {
    PlayerInfo info;
    u8 mac[6];
    u8 pad[2];
} UnionAppMember;

typedef void (*UnionAppMemberCallback)(u8 netId, UnionAppMember *member, void *work);

struct UnionAppStatus {
    u8 memberMax;

    u8 joined;
    u8 unk2[2];
};

struct UnionApp {
    UnionAppStatus status;

    u8 entering;
    u8 unk5[2];

    u8 confirmed;
    u8 unk8;
    u8 entryMode;

    u8 entryCount;
    u8 entryLimit;

    u8 statusSendTo;

    u8 profileSendTo;

    u8 hasProfile;

    u8 queued;
    u8 current;
    u8 currentPending;
    u8 unk12;
    u8 unk13;
    u32 unk14;
    u8 unk18[8];
    BOOL startRequested;
    BOOL starting;
    BOOL started;
    UnionAppMember members[5];
    UnionAppMemberCallback onJoin;
    UnionAppMemberCallback onLeave;
    void *callbackWork;
};

typedef struct {
    UnionApp *app;
    GameData *gameData;
    u8 unk08[8];
    GameStats *records;

    BOOL started;
} Guru2Param;

typedef struct Guru2ProcWork Guru2ProcWork;

typedef struct Guru2CommWork Guru2CommWork;

typedef struct Guru2MainWork Guru2MainWork;

struct Guru2ProcWork {
    int state;

    int partyIndex;

    u32 memberCount;
    u32 memberBits;

    BOOL receiptDone;
    Guru2Param *param;
    Guru2CommWork *g2c;
    Guru2MainWork *g2m;
    struct UnkStruct_ov109_021D5140_t *g2r;
};

typedef struct {
    u32 unk0;
    u8 data[0x18];
} Guru2CommSendBuffer;

struct Guru2CommWork {
    Guru2ProcWork *g2p;
    Guru2CommSendBuffer send;

    int startCount;
    BOOL started;
    u32 unk28;

    int joinCount;
    u32 leavingBits;
    u32 unk34;

    u16 signalCount;
    u8 unk3A[2];

    u32 unk3C;

    u16 unk40;
    u16 unk42;
    u8 unk44[2];

    s16 blockBits;
    u16 unk48;
    u16 unk4A;

    u8 *blockRecvBuffer;
    u8 *blocks;
    u8 unk54[4];
};

enum {
    GURU2_RECEIPT_SEQ_INIT,
    GURU2_RECEIPT_SEQ_1,
    GURU2_RECEIPT_SEQ_2,
    GURU2_RECEIPT_SEQ_MAIN,
    GURU2_RECEIPT_SEQ_CANCEL_YESNO_INIT,
    GURU2_RECEIPT_SEQ_CANCEL_YESNO,
    GURU2_RECEIPT_SEQ_CANCEL_SIGNAL_WAIT,
    GURU2_RECEIPT_SEQ_CANCEL_OK,
    GURU2_RECEIPT_SEQ_CANCEL_REFUSED,
    GURU2_RECEIPT_SEQ_END_MSG,
    GURU2_RECEIPT_SEQ_END_WAIT,
    GURU2_RECEIPT_SEQ_PARENT_CANCEL_YESNO_INIT,
    GURU2_RECEIPT_SEQ_PARENT_CANCEL_YESNO,
    GURU2_RECEIPT_SEQ_PARENT_CANCEL_MSG,
    GURU2_RECEIPT_SEQ_PARENT_CANCEL_WAIT,
    GURU2_RECEIPT_SEQ_PARENT_CANCEL_SYNC,
    GURU2_RECEIPT_SEQ_PARENT_CANCEL_SYNC_WAIT,
    GURU2_RECEIPT_SEQ_NO_MEMBERS_MSG_WAIT,
    GURU2_RECEIPT_SEQ_NO_MEMBERS_END,
    GURU2_RECEIPT_SEQ_MEMBER_LEFT,
    GURU2_RECEIPT_SEQ_MEMBER_LEFT_2,
    GURU2_RECEIPT_SEQ_MEMBER_LEFT_WAIT,
    GURU2_RECEIPT_SEQ_START_YESNO_INIT,
    GURU2_RECEIPT_SEQ_START_YESNO,
    GURU2_RECEIPT_SEQ_START_SEND,
    GURU2_RECEIPT_SEQ_START_WAIT,
    GURU2_RECEIPT_SEQ_MSG_WAIT,
    GURU2_RECEIPT_SEQ_START,
    GURU2_RECEIPT_SEQ_COUNT,
};

typedef struct ClActUnit ClActUnit;

typedef struct ClActor ClActor;

typedef struct PrintQueue PrintQueue;

typedef struct {
    BmpWin *window;
    u8 flushPending;
} PrintWindow;

typedef struct {
    ListMenuOption *options;

    BmpWin *window;

    u8 wait;
    u8 xCount;
    u8 yCount;
    u8 lineSpacing : 4;

    u8 cursorDisplay : 2;

    u8 loop : 2;
    u16 fontSizeX;
    u16 fontSizeY;
    u32 unkC;
    PrintWindow *printWindow;
    PrintQueue *queue;
    Font *font;

    BOOL printed;
} BmpMenuHeader;

struct BmpMenu {
    BmpMenuHeader header;
    BmpCursor *cursor;

    u32 cancelKeys;
    u8 unk28;
    u8 cursorPos;

    u8 maxWidth;
    u8 x;
    u8 y;
    u8 fontSizeX;
    u8 fontSizeY;

    u8 moveDir;
    HeapID heapId;

    u8 wait;
};

enum {
    GURU2_RECEIPT_RES_PLTT,
    GURU2_RECEIPT_RES_CHAR = GURU2_RECEIPT_RES_PLTT + 5,
    GURU2_RECEIPT_RES_CELL = GURU2_RECEIPT_RES_CHAR + 5,
    GURU2_RECEIPT_RES_COUNT,
};

void ov85_021E943C(UnkStruct_ov109_021D5140 *wk);

void ov85_021E9458(UnkStruct_ov109_021D5140 *wk, int nextSeq);

int ov85_021E96B0(UnkStruct_ov109_021D5140 *wk, int procSeq);

int ov85_021E96B0(UnkStruct_ov109_021D5140 *wk, int procSeq) {
    wk->unk_4AB4 = 0;
    ov85_021E9458(wk, GURU2_RECEIPT_SEQ_INIT);
    ov85_021E943C(wk);
    return procSeq;
}
