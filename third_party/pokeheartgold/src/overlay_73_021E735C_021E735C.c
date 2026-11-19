#include "global.h"
#include "overlay_73_unk_struct_020961_e8_t.h"
#include "string_util.h"
#include "struct_defs/resdat_narc_structs.h"
#include "font.h"
#include "save.h"
#include "options.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "msgdata.h"
#include "list_menu_items.h"

struct ListMenuCursor;

struct EncodingSeed;

struct GameStats;

struct JournalEntry;

struct JournalEntryMon;

struct JournalEntryTitle;

struct JournalEntryTrainer;

struct ListMenu2D;

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

struct SysTask;

struct SysTaskQueue;

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

enum CharsetMode {

    CHARSET_MODE_JP,

    CHARSET_MODE_EN,
};

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

typedef struct ListMenu2D {
    ListMenu2DTemplate template;
    ListMenuCursor *cursor;
    u32 cancelKeys;
    u8 dummy14;
    u8 cursorPos;
    u8 width;
    u8 xOffset;
    u8 yOffset;
    u8 letterWidth;
    u8 lineHeight;
    u8 lastAction;
    u8 heapID;
} ListMenu2D;

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

u16 sub_0203769C(Window *, int);

BOOL ov59_021D254C(UnkStruct_020961E8 *param0);

BOOL ov73_021E735C(Window *param0, int unused, u32 param2, UnkStruct_020961E8 *param3);

BOOL ov73_021E7490(UnkStruct_020961E8 *param0);

BOOL ov73_021E735C(Window *param0, int unused, u32 param2, UnkStruct_020961E8 *param3) {
    int i;
    const int v1 = sub_0203769C(param0, unused);
    String *v2 = 0L;

    if (!ov73_021E7490(param3)) {
        return 0;
    }

    FillWindowPixelBuffer(&param0[0], 0x0);

    for (i = 0; i < 5; i++) {
        if (param3->unk_3C8[i][0] != 0L) {
            u16 v3 = PlayerProfile_GetTrainerID_VisibleHalf(param3->unk_3C8[i][0]);

            PlayerName_FlatToString(param3->unk_3C8[i][0], param3->unk_2C[i]);
            BufferIntegerAsString(param3->unk_24, 0, v3, 5, (enum PrintingMode)(2), (enum CharsetMode)(1));

            v2 = ReadMsgData_ExpandPlaceholders(param3->unk_24, param3->unk_28, 1, HEAP_ID_50);

            if (v1 == i) {
                AddTextPrinterParameterizedWithColor(&param0[0], FONT_SYSTEM, param3->unk_2C[i], 5, 1 + i * 18, 0xFF, ((TextColor)(((2 & 0xFF) << 16) | ((3 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
                AddTextPrinterParameterizedWithColor(&param0[0], FONT_SYSTEM, v2, 5 + 13 * 5, 1 + i * 18, 0xFF, ((TextColor)(((2 & 0xFF) << 16) | ((3 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
            } else {
                AddTextPrinterParameterizedWithColor(&param0[0], FONT_SYSTEM, param3->unk_2C[i], 5, 1 + i * 18, 0xFF, param2, 0L);
                AddTextPrinterParameterizedWithColor(&param0[0], FONT_SYSTEM, v2, 5 + 13 * 5, 1 + i * 18, 0xFF, param2, 0L);
            }

            String_Delete(v2);
        }
    }

    CopyWindowToVram(&param0[0]);

    return 1;
}
