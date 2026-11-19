#include "global.h"
#include "overlay_14_box2_sys_work.h"
#include "overlay_14_box2_app_work.h"
#include "msgdata.h"
#include "overlay_14_box_funcs.h"

struct AppTaskMenu;

struct AppTaskMenuRes;

struct ArcTool;

struct BGWinFrame;

struct BagItem;

struct BagSave;

struct BmpWin;

struct Box2AppWork;

struct Box2ButtonAnm;

struct Box2FontOam;

struct Box2IrqWork;

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

typedef struct AppTaskMenu AppTaskMenu;

typedef struct AppTaskMenuRes AppTaskMenuRes;

typedef struct ArcTool ArcTool;

typedef struct BagSave BagSave;

typedef struct BmpWin BmpWin;

typedef struct BoxPkm BoxPkm;

typedef struct BoxSaveAccessor BoxSaveAccessor;

typedef struct GameData GameData;

typedef struct GFLBitmap GFLBitmap;

typedef struct Font Font;

typedef struct GameProcManager GameProcManager;

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

typedef struct BagItem BagItem;

typedef struct BGWinFrame BGWinFrame;

typedef struct CursorMove CursorMove;

typedef struct Box2FontOam Box2FontOam;

typedef struct Box2IrqWork Box2IrqWork;

typedef struct Box2ButtonAnm Box2ButtonAnm;

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

int func_ov170_021df324(Box2SysWork *syswk);

int func_ov170_021df288(Box2SysWork *syswk);

enum {
    BOX2SEQ_INIT = 0,
    BOX2SEQ_RELEASE,
    BOX2SEQ_WIPE,
    BOX2SEQ_PALETTE_FADE,
    BOX2SEQ_WAIT,
    BOX2SEQ_VFUNC,
    BOX2SEQ_TRGWAIT,
    BOX2SEQ_YESNO,
    BOX2SEQ_BUTTON_ANM,
    BOX2SEQ_SUBPROC_CALL,
    BOX2SEQ_SUBPROC_MAIN,
    BOX2SEQ_START,
};

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

struct BagItem {
    u16 item;
    u16 count;
};

struct BagSave {
    BagItem items[310];
    BagItem keyItems[83];
    BagItem tmsHms[109];
    BagItem medicine[48];
    BagItem berries[64];
    u8 unk_998[0x28];
};

typedef int (*Box2SeqFunc)(Box2SysWork *syswk);

typedef struct {
    Box2SeqFunc call;
    Box2SeqFunc exit;
    int nextSeq;
} Box2SubProc;

int func_ov170_021df348(Box2SysWork *syswk);

int func_ov170_021df378(Box2SysWork *syswk);

BOOL func_ov170_021df38c(Box2SysWork *syswk);

int func_ov170_021df404(Box2SysWork *syswk);

extern const Box2SubProc ov14_021F7D50[3];

int ov14_021EB290(Box2SysWork *syswk) {
    ov14_021F7D50[syswk->subProcType].call(syswk);
    return BOX2SEQ_SUBPROC_MAIN;
}
