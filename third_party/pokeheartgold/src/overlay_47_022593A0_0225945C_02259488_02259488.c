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

struct BgConfig;

struct ListMenuCursor;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MessageBank;

struct MessageBankEntry;

struct NARC;

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

struct WindowTemplate;

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

void ov45_0222B1FC(WiFiPlazaSystem *param0, const UnkStruct_ov68_0225DC74 *param1);

BOOL ov45_0222B21C(const WiFiPlazaSystem *param0, UnkStruct_ov68_0225DC74 *param1);

typedef struct {
    SaveData *saveData;
    WiFiPlazaSystem *unk_04;
    int unk_08;
} UnkStruct_ov66_02231374;

void AddWindow(BgConfig *bgConfig, Window *window, const WindowTemplate *template);

u32 GfGfxLoader_LoadCharDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);

void GfGfxLoader_LoadScrnDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, enum HeapID heapID);

void GfGfxLoader_GXLoadPalFromOpenNarc(NARC *narc, u32 narcMemberIdx, enum GFPalLoadLocation loadLocation, u32 offset, u32 size, u32 heapID);

extern void *GfGfxLoader_GetCharDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BOOL compressed, NNSG2dCharacterData **outCharData, enum HeapID heapID);

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

extern void ov47_02259C8C(UnkStruct_ov68_0225DB8C *param0, BOOL param1, enum HeapID heapID);

void ov47_02259D74(UnkStruct_ov68_0225DC74 *param0);

void ov47_02259DCC(UnkStruct_ov68_0225DC74 *param0, const WiFiPlazaSystem *param1);

extern void ov47_0225999C(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, u32 param2, TextColor param3);

extern const WindowTemplate ov47_02259EE8[6];

void ov68_0225C91C(UnkStruct_ov68_0225C91C *param0, SaveData *saveData, enum HeapID heapID);

void ov68_0225C9A0(UnkStruct_ov68_0225C91C *param0, Options *options, enum HeapID heapID);

void ov47_02258BB4(UnkStruct_ov68_0225C91C *param0, enum HeapID heapID);

void ov47_02258D78(UnkStruct_ov68_0225CB70 *param0, u32 param1, enum HeapID heapID);

BOOL ov68_0225CCB4(UnkUnion_ov68_0225CCA8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, enum HeapID heapID);

void ov47_02259228(UnkStruct_ov68_0225D128 *param0, UnkStruct_ov68_0225C91C *param1, SaveData *saveData, enum HeapID heapID);



void ov68_0225D8F0(UnkStruct_ov68_0225D388 *param0, const UnkStruct_ov68_0225DB8C *param1, UnkStruct_ov68_0225CB70 *param2, UnkStruct_ov68_0225C91C *param3, const UnkStruct_ov66_0222E908 *param4, enum HeapID heapID, u32 param6, u32 param7, BOOL param8);

void ov47_02259488(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov66_02231374 *param3, enum HeapID heapID);

void ov47_02259488(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov66_02231374 *param3, enum HeapID heapID) {
    {
        GfGfxLoader_GXLoadPalFromOpenNarc(param2->unk_1A4, 199, (enum GFPalLoadLocation)(0), 0, 4 * 32, heapID);
        GfGfxLoader_LoadCharDataFromOpenNarc(param2->unk_1A4, 200, param2->unk_00, 0, 0, 0, 0, heapID);
        GfGfxLoader_LoadScrnDataFromOpenNarc(param2->unk_1A4, 202, param2->unk_00, 0, 0, 0, 0, heapID);
    }

    {
        int v0;

        for (v0 = 0; v0 < 6; v0++) {
            AddWindow(param2->unk_00, &param0->unk_00[v0], &ov47_02259EE8[v0]);
        }
    }

    ov47_0225999C(param0, param1, 109, ((TextColor)(((3 & 0xFF) << 16) | ((4 & 0xFF) << 8) | ((0 & 0xFF) << 0))));

    {
        param0->unk_80 = GfGfxLoader_GetCharDataFromOpenNarc(param2->unk_1A4, 200, 0, &param0->unk_84, heapID);
    }

    {
        ov47_02259C8C(&param0->unk_88, 1, heapID);
        ov47_02259C8C(&param0->unk_A8, 0, heapID);

        if (ov45_0222B21C(param3->unk_04, &param0->unk_C8) == 0) {
            ov47_02259DCC(&param0->unk_C8, param3->unk_04);
            ov45_0222B1FC(param3->unk_04, &param0->unk_C8);
        }

        ov47_02259D74(&param0->unk_DC);
    }
}
