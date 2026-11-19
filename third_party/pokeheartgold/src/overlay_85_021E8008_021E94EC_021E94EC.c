#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "wild_encounter.h"
#include "list_menu_2d.h"
#include "overlay_85_ov109_021D5140.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_41_sprite_resource_collection.h"
#include "screen_fade_internal.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

struct ListMenuCursor;

struct EncodingSeed;

struct GameStats;

struct JournalEntry;

struct JournalEntryMon;

struct JournalEntryTitle;

struct JournalEntryTrainer;

struct MessageBank;

struct MessageBankEntry;

struct Options;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_0209BF64;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;

typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;

typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;

typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;

typedef struct EncodingSeed {
    u16 byteSum;
    u16 modifier;
} EncodingSeed;

typedef struct GameStats {
    u32 recordsU32[71];
    u16 recordsU16[77];
    u16 padding;
    EncodingSeed seed;
} GameStats;

typedef struct JournalEntryTitle {
    u32 year : 7;
    u32 month : 4;
    u32 week : 3;
    u32 day : 5;
    u32 mapID : 13;
} JournalEntryTitle;

typedef struct JournalEntryMon {
    u8 battleResult;
    u8 stringVariant : 2;
    u8 timeOfDay : 4;
    u8 gender : 2;
    u16 species;
} JournalEntryMon;

typedef struct JournalEntryTrainer {
    u16 standard : 1;
    u16 trainerID : 15;
    u16 mapID;
} JournalEntryTrainer;

typedef struct JournalEntry {
    JournalEntryTitle title;
    u32 locationEvents[4];
    JournalEntryMon mon;
    JournalEntryTrainer trainer;
    u8 onlineEvents[2][42];
} JournalEntry;

typedef u32 TextColor;

typedef struct ListMenuCursor {
    TextColor color;
    String *string;
} ListMenuCursor;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

int sub_02037454(void);

u16 sub_0203769C(void);

BOOL PlaySE(u16 seqID);

BOOL sub_02096D4C(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);

void ov85_021E943C(UnkStruct_ov109_021D5140 *param0);

void ov85_021EA0EC(UnkStruct_ov109_021D5140 *param0, int param1, int param2);

int ov85_021E9FD0(void);

void ov85_021E9458(UnkStruct_ov109_021D5140 *param0, int param1);

void ov85_021EA39C(UnkStruct_ov109_021D5140 *param0, int param1);

void *ov85_021E94EC(UnkStruct_ov109_021D5140 *param0, void *param1);

void *ov85_021E94EC(UnkStruct_ov109_021D5140 *param0, void *param1) {
    u32 v1;

    if (sub_0203769C()) {
        if (param0->unk_10->unk_28) {
            if (gSystem.newKeys & (0x0001 | 0x0002 | 0x0040 | 0x0080)) {
                PlaySE(1522);
            }

            ov85_021E943C(param0);
            return param1;
        }
    } else {
        if (param0->unk_10->unk_30 != 0) {
            if (gSystem.newKeys & (0x0001 | 0x0002 | 0x0040 | 0x0080)) {
                PlaySE(1522);
            }

            ov85_021E943C(param0);
            return param1;
        }
    }

    if (ov85_021E9FD0() != sub_02037454()) {
        ov85_021E943C(param0);
        return param1;
    }

    v1 = Handle2dMenuInput_DeleteOnFinish(param0->unk_394, HEAP_ID_102);

    if (v1 != 0xffffffff) {
        if (v1 == 0xfffffffe) {
            if (sub_0203769C() == 0) {
                int v2 = 0;

                sub_02096D4C(param0->unk_10, 7, &v2, 1);
                ov85_021EA39C(param0, 1);
            }

            ov85_021E9458(param0, 0);
        } else {
            if (sub_0203769C() == 0) {
                ov85_021E9458(param0, 11);
                ov85_021EA0EC(param0, 14, 0);
            } else {
                UnkStruct_0209BF64 v3;

                MI_CpuClear8(&v3, sizeof(UnkStruct_0209BF64));
                v3.unk_02 = 0;
                v3.unk_00 = sub_0203769C();

                param0->unk_4AB4 = 1;
                param0->unk_4AC0 = 0;
                param0->unk_3B8 = 6;

                sub_02096D4C(param0->unk_10, 2, &v3, sizeof(UnkStruct_0209BF64));
            }
        }

        param0->unk_394 = 0L;
    }

    ov85_021E943C(param0);

    return param1;
}
