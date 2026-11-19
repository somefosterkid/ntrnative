#include "global.h"
#include "overlay_14_box2_sys_work.h"
#include "overlay_14_box2_app_work.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"

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

typedef u16 HeapID;

enum {
    HEAPID_SYSTEM = 0x0,
    HEAPID_USER = 0x1,
    HEAPID_GAMESYSTEM = 0x3,
    HEAPID_GAMEEVENT = 0x4,
    HEAPID_BATTLE = 0x12,
    HEAPID_BATTLE_RECORDER = 0x5a,
    HEAPID_BATTLE_RECORDER_SYS = 0x59,
    HEAPID_NET = 0x8,
    HEAPID_DEVICE_ALLOC = 0x9,
    HEAPID_BAG = 0xe,
    HEAPID_DWC_UTILITY = 0xf,
    HEAPID_FIELDMAP = 0x15,
    HEAPID_MYSTERY = 0x1d,
    HEAPID_IRC_BATTLE_MENU = 0x1f,
    HEAPID_PMS_INPUT_SYS = 0x29,
    HEAPID_MB_PARENT = 0x5d,
    HEAPID_PMS_INPUT = 0x2a,
    HEAPID_MUSICAL_EVENT = 0x2c,
    HEAPID_MUSICAL_DRESSUP = 0x2d,
    HEAPID_IRC_COMPAT = 0x3c,
    HEAPID_P_STATUS = 0x42,
    HEAPID_WORLDTRADE = 0x48,
    HEAPID_BOX2 = 0x4b,
    HEAPID_BOX2_APP = 0x4c,
    HEAPID_GAME_CLEAR_SAVE = 0x54,
    HEAPID_MONOLITH = 0x61,
    HEAPID_VS_MULTI_LIST = 0x71,
    HEAPID_UN_SELECT = 0x7a,
    HEAPID_ZUKAN_OPEN = 0x7e,
    HEAPID_THE_END = 0x88,
    HEAPID_SUBWAY_MAP = 0x88,
    HEAPID_POKEMON_TRADE = 0x91,
    HEAPID_GAMESYNC = 0x67,
};

typedef struct ClActUnit ClActUnit;

typedef struct ClActor ClActor;

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

void *PML_PersonalLoad(u32 species, s16 form, HeapID heapId);

s8 PML_PersonalGetParam(void *personal, u32 param);

void PML_PersonalFree(void *personal);

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

u32 PML_PkmGetParam(BoxPkm *pkm, u32 param, void *buffer);

BOOL PML_PkmDecrypt(BoxPkm *pkm);

BOOL PML_PkmReEncrypt(BoxPkm *pkm, BOOL wasEncrypted);

BoxPkm *BoxSaveAccessor_GetPkm(BoxSaveAccessor *boxes, u32 box, u32 slot);

struct GFLBitmap {
    u8 *pixels;
    u16 width;
    u16 height;
    u16 tileSize;
    u16 storage;
};

void ov14_021F4A64(UnkStruct_ov14_boxapp *app, u32 tray, u8 *chr);

void *PCStorage_GetMonByIndexPair(void *, u32 boxno, u32 slotno);

BOOL AcquireBoxMonLock(void *);

u32 GetBoxMonData(void *, int attr, void *ptr);

BOOL ReleaseBoxMonLock(void *, BOOL decrypt_result);

int GetMonBaseStatEx_HandleAlternateForm(void *narc, int species, int form, int attr);

extern u8 ov14_021F8080[];

void ov14_021F4A64(UnkStruct_ov14_boxapp *app, u32 tray, u8 *chr) {
    BoxPkm *pkm;
    BOOL encrypted;
    u8 j;
    u8 i;
    u8 px;
    u8 py;
    u32 species;
    u8 y;
    u16 color;
    u16 form;

    py = 11;
    for (j = 0; j < 5; j++) {
        px = 10;
        for (i = 0; i < 6; i++) {
            pkm = PCStorage_GetMonByIndexPair(app->storage, tray, i + j * 6);
            encrypted = AcquireBoxMonLock(pkm);
            species = GetBoxMonData(pkm, 0x5, ((void *)0));
            if (GetBoxMonData(pkm, 0xAC, ((void *)0)) != 0) {
                if (GetBoxMonData(pkm, 0x4c, ((void *)0)) == 0) {
                    form = GetBoxMonData(pkm, 0x70, ((void *)0));
                    color = GetMonBaseStatEx_HandleAlternateForm(app->sys->unk_450, species, form, 27);
                } else if (species == 490) {
                    color = 1;
                } else {
                    color = 8;
                }
                color = ov14_021F8080[color];
                color += 32;
                color = (color << 8) | color;
                for (y = py; y < py + 2; y++) {
                    MIi_CpuClear16(color, &chr[((y & 7) << 3) + ((((y >> 3) << 2) + (px >> 3)) << 6) + (px & 7)], 2);
                }
            }
            ReleaseBoxMonLock(pkm, encrypted);
            px += 2;
        }
        py += 2;
    }
}
