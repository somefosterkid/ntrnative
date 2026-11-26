#include "global.h"

#include "constants/heap.h"
#include "overlay_47_unk_struct_ov68_0225_d128.h"
#include "overlay_45_wifi_plaza_system.h"
#include "bg_window.h"
#include "filesystem.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_47_unk_struct_ov68_0225c91c.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "struct_defs/resdat_narc_structs.h"

struct ListMenuCursor;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MessageBank;

struct MessageBankEntry;

struct Options;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct Sprite;

struct SpriteResource;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct Window;

typedef enum {
    UnkEnum_ov66_02233454_00,
    UnkEnum_ov66_02233454_01,
    UnkEnum_ov66_02233454_02,
    UnkEnum_ov66_02233454_03,
    UnkEnum_ov66_02233454_04,
    UnkEnum_ov66_02233454_05,
    UnkEnum_ov66_02233454_06,
    UnkEnum_ov66_02233454_07,
    UnkEnum_ov66_02233454_08,
    UnkEnum_ov66_02233454_09,
    UnkEnum_ov66_02233454_10,
    UnkEnum_ov66_02233454_11,
} UnkEnum_ov66_02233454;

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

s32 ov45_0222F544(UnkEnum_ov66_02233454 param0);

typedef struct {
    SaveData *saveData;
    WiFiPlazaSystem *unk_04;
    int unk_08;
} UnkStruct_ov66_02231374;

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

typedef struct {
    u32 unk_00;
    u32 unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    String *unk_10;
    String *unk_14[3];
} UnkStruct_ov68_0225DB8C;

typedef struct {
    Window unk_00[1];
    s32 unk_10;
    Sprite *unk_14;
    SpriteResource *unk_18[4];
    u16 unk_28;
    u16 unk_2A;
    UnkStruct_ov68_0225DB8C unk_2C;
} UnkStruct_ov68_0225D0F8;

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

typedef union {
    UnkStruct_ov68_0225D0F8 val1;
    UnkStruct_ov68_0225D388 val2;
} UnkUnion_ov68_0225CCA8;

typedef struct {
    MsgData *unk_00[9];
    MessageFormat *unk_24;
    String *unk_28;
    String *unk_2C;
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov68_0225CB70;

void ov68_0225C91C(UnkStruct_ov68_0225C91C *param0, SaveData *saveData, enum HeapID heapID);

void ov68_0225C9A0(UnkStruct_ov68_0225C91C *param0, Options *options, enum HeapID heapID);

void ov47_02258BB4(UnkStruct_ov68_0225C91C *param0, enum HeapID heapID);

void ov47_02258D78(UnkStruct_ov68_0225CB70 *param0, u32 param1, enum HeapID heapID);

BOOL ov68_0225CCB4(UnkUnion_ov68_0225CCA8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, enum HeapID heapID);

void ov47_02259228(UnkStruct_ov68_0225D128 *param0, UnkStruct_ov68_0225C91C *param1, SaveData *saveData, enum HeapID heapID);


void ov68_0225D388(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov66_02231374 *param3, enum HeapID heapID);


void ov68_0225D8F0(UnkStruct_ov68_0225D388 *param0, const UnkStruct_ov68_0225DB8C *param1, UnkStruct_ov68_0225CB70 *param2, UnkStruct_ov68_0225C91C *param3, const UnkStruct_ov66_0222E908 *param4, enum HeapID heapID, u32 param6, u32 param7, BOOL param8);

void ov68_0225DB8C(UnkStruct_ov68_0225DB8C *param0, BOOL param1, enum HeapID heapID);

void ov47_02259D74(UnkStruct_ov68_0225DC74 *param0);

void ov47_02259D74(UnkStruct_ov68_0225DC74 *param0) {
    int v0;

    param0->unk_0C = 0;

    for (v0 = 0; v0 < 3; v0++) {
        param0->unk_00[v0] = ov45_0222F544((UnkEnum_ov66_02233454)(UnkEnum_ov66_02233454_08 + v0));
        param0->unk_0C += param0->unk_00[v0];
    }
}
