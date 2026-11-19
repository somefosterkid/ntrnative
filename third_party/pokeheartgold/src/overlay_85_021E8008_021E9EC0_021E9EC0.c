#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"
#include "msgdata.h"
#include "message_format.h"
#include "player_data.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_41_sprite_resource_collection.h"
#include "font_types_def.h"
#include "overlay_85_ov109_021D0F70.h"
#include "list_menu_items.h"
#include "unk_struct_0209_c194.h"
#include "font.h"
#include "string_util.h"
#include "struct_defs/resdat_narc_structs.h"

struct ListMenuCursor;

struct EncodingSeed;

struct GameStats;

struct JournalEntry;

struct JournalEntryMon;

struct JournalEntryTitle;

struct JournalEntryTrainer;

struct ListMenu2DTemplate;

struct MessageBank;

struct MessageBankEntry;

struct SaveBlockInfo;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct String;

struct ListMenuItem;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct Window;

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

void String_Delete(String *string);

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

void CopyWindowToVram(Window *window);

void FillWindowPixelBuffer(Window *window, u8 val);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

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

typedef struct ListMenu2DTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} ListMenu2DTemplate;

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

u16 sub_0203769C(void);

BOOL ov85_021EA010(UnkStruct_ov109_021D5140 *param0);

BOOL ov85_021E9EC0(Window *param0, int param1, TextColor param2, UnkStruct_ov109_021D5140 *param3);

BOOL ov85_021E9EC0(Window *param0, int param1, TextColor param2, UnkStruct_ov109_021D5140 *param3) {
    int v0, v1 = sub_0203769C();
    String *v2 = 0L;

    if (!ov85_021EA010(param3)) {
        return 0;
    }

    FillWindowPixelBuffer(&param0[0], 0x0);

    for (v0 = 0; v0 < 5; v0++) {
        if (param3->unk_3D8[v0][0] != 0L) {
            u16 v3 = PlayerProfile_GetTrainerID_VisibleHalf(param3->unk_3D8[v0][0]);

            PlayerName_FlatToString(param3->unk_3D8[v0][0], param3->unk_3C[v0]);
            BufferIntegerAsString(param3->unk_34, 0, v3, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
            v2 = ReadMsgData_ExpandPlaceholders(param3->unk_34, param3->unk_38, 1, HEAP_ID_102);

            if (v1 == v0) {
                AddTextPrinterParameterizedWithColor(&param0[0], FONT_SYSTEM, param3->unk_3C[v0], 5, 1 + v0 * 18, 0xFF, ((TextColor)(((2 & 0xFF) << 16) | ((3 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
                AddTextPrinterParameterizedWithColor(&param0[0], FONT_SYSTEM, v2, 5 + 13 * 5, 1 + v0 * 18, 0xFF, ((TextColor)(((2 & 0xFF) << 16) | ((3 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
            } else {
                AddTextPrinterParameterizedWithColor(&param0[0], FONT_SYSTEM, param3->unk_3C[v0], 5, 1 + v0 * 18, 0xFF, param2, 0L);
                AddTextPrinterParameterizedWithColor(&param0[0], FONT_SYSTEM, v2, 5 + 13 * 5, 1 + v0 * 18, 0xFF, param2, 0L);
            }

            String_Delete(v2);
        }
    }

    CopyWindowToVram(&param0[0]);

    return 1;
}
