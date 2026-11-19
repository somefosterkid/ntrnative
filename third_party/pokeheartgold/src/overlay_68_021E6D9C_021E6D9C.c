#include <stdint.h>

#include "global.h"
#include "overlay_68_move_relearner.h"
#include "msgdata.h"
struct AppTaskMenu;
struct AppTaskMenuRes;
struct AppTaskMenuWin;
struct ArcTool;
struct BmpWin;
struct ClActUnit;
struct ClActor;
struct CursorMove;
struct Font;
struct GameData;
struct KeyCursor;
struct ListMenuOption;
struct MsgData;
struct PrintQueue;
struct PrintStream;
struct StrBuf;
struct TCB;
struct TCBExManager;
struct TrainerDataSave;
struct WordSet;

struct AppTaskMenu;

struct AppTaskMenuRes;

struct AppTaskMenuWin;

struct ArcTool;

struct BmpWin;

struct BoxPkm;

struct ClActUnit;

struct ClActor;

struct CursorMove;

struct Field;

struct Font;

struct GameCommSys;

struct GameData;

struct GameEvent;

struct GameProcManager;

struct GameSystem;

struct GameSystemProcData;

struct ISS;

struct KeyCursor;

struct ListMenuOption;

struct MailData;

struct MsgData;

struct PMSData;

struct PartyPkm;

struct PlayerInfo;

struct PrintQueue;

struct PrintStream;

struct StrBuf;

struct TCB;

struct TCBExManager;

struct TrainerDataSave;

struct WordSet;

typedef struct AppTaskMenu AppTaskMenu;

typedef struct AppTaskMenuRes AppTaskMenuRes;

typedef struct ArcTool ArcTool;

typedef struct BmpWin BmpWin;

typedef struct BoxPkm BoxPkm;

typedef struct Field Field;

typedef struct GameCommSys GameCommSys;

typedef struct GameData GameData;

typedef struct GameEvent GameEvent;

typedef struct GameSystem GameSystem;

typedef struct Font Font;

typedef struct GameProcManager GameProcManager;

typedef struct ISS ISS;

typedef struct KeyCursor KeyCursor;

typedef struct PartyPkm PartyPkm;

typedef struct PlayerInfo PlayerInfo;

typedef struct PMSData PMSData;

typedef struct PrintStream PrintStream;

typedef struct StrBuf StrBuf;

typedef struct TCBExManager TCBExManager;

typedef struct TCB TCB;

typedef struct TrainerDataSave TrainerDataSave;

typedef struct WordSet WordSet;

typedef struct ListMenuOption ListMenuOption;

typedef struct MailData MailData;

typedef struct GameSystemProcData GameSystemProcData;

typedef struct CursorMove CursorMove;

typedef struct AppTaskMenuWin AppTaskMenuWin;

typedef u16 HeapID;

typedef struct {

    void *party;
    TrainerDataSave *trainerData;
    GameData *gameData;
    u8 dataType;
    u8 mode;
    u8 partyCount;
    u8 partyIndex;

    u8 page;

    u8 slot;
    u8 result;
    u16 move;
    BOOL isNationalDex;
    u8 unk1C[4];

    BOOL fromFieldMenu;

    BOOL forceExit;
} PStatusParam;

typedef struct ClActUnit ClActUnit;

typedef struct ClActor ClActor;

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

typedef struct PrintQueue PrintQueue;

typedef struct {
    StrBuf *str;
    u16 color;
    u32 type;
} AppTaskMenuItem;

struct ListMenuOption {
    StrBuf *text;
    intptr_t value;
};

typedef GameEvent *(*GameEventProvider)(GameSystem *gsys, void *args);

typedef enum {
    GAME_ENTRYPOINT_OPENING,
    GAME_ENTRYPOINT_FIELD_CONTINUE,
    GAME_ENTRYPOINT_DEBUG,
} GameEntryPoint;

struct GameSystemProcData {
    GameEntryPoint entryPoint;
    VecFx32 spawnPos;
    u16 zoneId;
    u16 unk12;
};

struct GameSystem {
    HeapID heapId;
    GameSystemProcData *procData;
    GameProcManager *procManager;

    u32 procManagerState;

    GameEventProvider eventProvider;
    void *eventProviderData;
    GameEvent *event;
    GameData *gameData;

    Field *field;
    GameCommSys *comm;
    u32 unk28;
    ISS *iss;
    u8 unk34;

    u8 eventRunning;

    u8 unk36;

    u8 unk37_0 : 1;
};

enum {
    SEQ_FADE_WAIT,
    SEQ_SELECT,
    SEQ_MESSAGE,
    SEQ_YES_NO_INIT,
    SEQ_YES_NO,
    SEQ_SET_MOVE,
    SEQ_FORGOT,
    SEQ_STATUS_RESULT,
    SEQ_FADE_OUT,
    SEQ_END,
    SEQ_STATUS_FADE_OUT,
    SEQ_STATUS_CALL,
    SEQ_STATUS_RETURN,
    SEQ_DECIDE_WAIT,
    SEQ_QUIT,
    SEQ_MOVE_CHOSEN,
};

enum {
    MSG_SELECT,
    MSG_LEARN,
    MSG_QUIT,
    MSG_LEARNED,
    MSG_REPLACE,

    MSG_FORGET,
    MSG_FORGOT,
    MSG_GIVE_UP,
    MSG_NOT_LEARNED,
    MSG_PAID,
    MSG_FORGET_CONFIRM,
    MESSAGE_COUNT,
};

enum {
    YES_NO_LEARN,
    YES_NO_QUIT,
    YES_NO_REPLACE,
    YES_NO_GIVE_UP,
    YES_NO_FORGET,
};

typedef struct {
    u32 type;
    BOOL dirty;
} WazaOshieTypeIcon;

typedef struct {
    u8 unk00[6];
    u16 index;
    u16 seq;
    u16 type;
} WazaOshieDecide;

void ov68_021E6C14(MoveRelearner *work, u32 msg);

s32 ov68_021E6D9C(MoveRelearner *work);

s32 ov68_021E6D9C(MoveRelearner *work) {
    ov68_021E6C14(work, MSG_REPLACE);
    work->question = YES_NO_REPLACE;
    work->nextState = SEQ_YES_NO_INIT;
    return SEQ_MESSAGE;
}
