#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "pm_string.h"
#include "save.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_47.h"
#include "sys_task.h"
#include "overlay_47_unk_struct_ov68_0225_d128.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_45_wifi_plaza_system.h"
#include "overlay_45_unk_struct_ov66_02231_e94.h"
#include "overlay_45_unk_struct_ov66_0223177_c_t.h"
#include "overlay_47_unk_struct_ov68_0225c91c.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "struct_defs/resdat_narc_structs.h"
#include "list_menu_2d.h"

struct BoxPkm;

struct MailData;

struct PMSData;

struct PartyPkm;

struct PokemonTradeWork;

typedef struct BoxPkm BoxPkm;

typedef struct PartyPkm PartyPkm;

typedef struct PMSData PMSData;

typedef struct MailData MailData;

typedef struct PokemonTradeWork PokemonTradeWork;

void ov47_02259488(PokemonTradeWork *wk, void *side, void *front, PartyPkm *pkm, BOOL mirror);

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

void ov47_0225945C(PokemonTradeWork *wk, void *side, void *front, PartyPkm *pkm, BOOL mirror);

SysTask *ov47_02259968(SysTaskFunc callback, void *param, void *priority);

void ov47_0225945C(PokemonTradeWork *wk, void *side, void *front, PartyPkm *pkm, BOOL mirror) {
    ov47_02259488(wk, side, front, pkm, mirror);
}

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct PasswordWordBank PasswordWordBank;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};

typedef u32 TextColor;

struct ListMenuCursor {
    TextColor color;
    String *string;
};

struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

struct PasswordWordBank {
    u32 length;
    u32 *words;
};

typedef struct {
    MsgData *unk_00[9];
    MessageFormat *unk_24;
    String *unk_28;
    String *unk_2C;
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov68_0225CB70;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    String *unk_10;
    String *unk_14[3];
} UnkStruct_ov68_0225DB8C;

typedef struct {
    SaveData *saveData;
    WiFiPlazaSystem *unk_04;
    int unk_08;
} UnkStruct_ov66_02231374;

typedef struct {
    Window unk_00[6];
    u16 unk_60;
    u16 unk_62;
    s32 unk_64;
    s32 unk_68[3];
    s32 unk_74[3];
    void *unk_80;
    NNSG2dCharacterData *unk_84;
    UnkStruct_ov68_0225DB8C unk_88;
    UnkStruct_ov68_0225DB8C unk_A8;
    UnkStruct_ov68_0225DC74 unk_C8;
    UnkStruct_ov68_0225DC74 unk_DC;
} UnkStruct_ov68_0225D388;

BOOL ov47_02259578(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, enum HeapID heapID);

void ov47_0225946C(UnkStruct_ov68_0225D388 *arg0, UnkStruct_ov68_0225CB70 *arg1, UnkStruct_ov68_0225C91C *arg2, UnkStruct_ov68_0225D128 *arg3, UnkStruct_ov66_02231374 *arg4, enum HeapID arg5);

void ov47_0225946C(UnkStruct_ov68_0225D388 *arg0, UnkStruct_ov68_0225CB70 *arg1, UnkStruct_ov68_0225C91C *arg2, UnkStruct_ov68_0225D128 *arg3, UnkStruct_ov66_02231374 *arg4, enum HeapID arg5) {
    ov47_02259578(arg0, arg1, arg2, arg3, arg4, arg5);
}

SysTask *ov47_02259480(SysTaskFunc callback, void *param, void *priority) {
    return ov47_02259968(callback, param, priority);
}
