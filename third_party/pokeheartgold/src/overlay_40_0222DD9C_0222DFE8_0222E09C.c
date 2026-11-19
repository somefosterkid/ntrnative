#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "pokepic.h"
#include "overlay_39_unk_struct_ov62_022349_a8.h"
#include "overlay_40_unk_struct_ov62_02233310.h"
#include "overlay_40_unk_struct_ov62_0223118c.h"
#include "font_types_def.h"
#include "font.h"
#include "struct_defs/resdat_narc_structs.h"

struct BattleRegulation;

struct BgConfig;

struct CharTransferAllocation;

struct MailMessage;

struct TextOBJ;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct NARC;

struct PaletteData;

struct Pokepic;

struct PokepicDrawParam;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct SpriteAnimFrame;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct TouchHitboxController;

struct TouchScreenHitTableActionStateInternal;

struct UnkStruct_02012B20_t;

struct UnkStruct_0202F41C_t;

struct UnkStruct_02030A80_t;

struct UnkStruct_020F3DCC_t;

struct Window;

typedef struct UnkStruct_0202F41C_t UnkStruct_0202F41C;

typedef struct {
    int unk_00;
    enum HeapID heapID;
    int unk_08;
    int unk_0C;
    fx32 unk_10;
} UnkStruct_0208B284;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    PaletteData *unk_0C;
} UnkStruct_0208B878_sub1;

typedef struct {
    ManagedSprite *unk_00;
    s16 unk_04;
    s16 unk_06;
    int unk_08;
    int unk_0C;
} UnkStruct_0208B878_sub2;

typedef struct {
    UnkStruct_0208B878_sub1 unk_00;
    UnkStruct_0208B284 unk_10;
    UnkStruct_0208B878_sub2 unk_24[12];
    int unk_E4;
    int unk_E8;
    int unk_EC;
    BOOL unk_F0;
    SysTask *unk_F4;
    int unk_F8;
    int unk_FC;
    int unk_100;
    int unk_104;
} UnkStruct_0208B878;

typedef struct UnkStruct_02030A80_t UnkStruct_02030A80;

typedef struct {
    BOOL unk_00;
    int unk_04;
    u8 padding_08[8];
} UnkStruct_0208BA84;

typedef struct {
    BOOL unk_00;
    int unk_04;
    int unk_08[8];
} UnkStruct_0208C06C_sub1;

typedef struct {
    BOOL unk_00;
    int unk_04;
    UnkStruct_0208B878 *unk_08;
    SysTask *unk_0C;
} UnkStruct_0208C06C_sub2;

typedef struct UnkStruct_020F3DCC_t UnkStruct_020F3DCC;

struct UnkStruct_020F3DCC_t {
    BOOL unk_00;
    BOOL unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    const UnkStruct_020F3DCC *unk_20;
};

typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;

enum TouchScreenButtonState {
    TOUCH_BUTTON_NOT_PRESSED = -1,
    TOUCH_BUTTON_PRESSED,
    TOUCH_BUTTON_RELEASED,
    TOUCH_BUTTON_HELD,
    TOUCH_BUTTON_HELD_OUT_OF_BOUNDS
};

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

typedef struct {
    ManagedSprite *unk_00;
    u8 padding_04[8];
    BOOL unk_0C;
    TouchScreenRect *unk_10;
    TextOBJ *unk_14;
    UnkStruct_02012B20 *unk_18;
    CharTransferAllocation unk_1C;
} UnkStruct_ov62_022307C0_sub1;

typedef struct {
    UnkStruct_ov62_022307C0_sub1 unk_00[5];
    UnkStruct_ov62_022307C0_sub1 unk_C8[5];
    const UnkStruct_020F3DCC *unk_190[5];
    int unk_1A4;
    u8 padding_1A8[4];
    int unk_1AC;
    int unk_1B0;
    u8 padding_1B4[8];
} UnkStruct_ov62_022307C0;

typedef struct {
    ManagedSprite *unk_00[2];
    SysTask *unk_08;
    fx32 unk_0C;
    f32 unk_10;
    s16 unk_14;
    s16 unk_16;
    s8 unk_18;
    u8 unk_19;
    u8 padding_1A[2];
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    int unk_2C;
} UnkStruct_ov62_02230C28;

void String_Delete(String *string);

String *MailMsg_GetExpandedString(const MailMessage *sentence, enum HeapID heapID);

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov62_02239DA4_sub1;

typedef struct {
    u16 unk_00[8];
    u32 unk_10;
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
    u8 unk_18;
    u8 unk_19;
    u8 unk_1A;
    u8 unk_1B_0 : 1;
    u8 unk_1B_1 : 7;
    u16 unk_1C;
    u8 unk_1E;
    u8 unk_1F;
    union {
        MailMessage unk_20_val1;
        u16 unk_20_val2[40];
    };
    u8 unk_70[12];
    UnkStruct_ov62_02239DA4_sub1 unk_7C;
} UnkStruct_ov61_0222AFC0;

typedef struct {
    u16 unk_00[12];
    u16 unk_18;
    u8 unk_1A;
    u8 unk_1B;
    s8 unk_1C;
    s8 unk_1D;
    u8 unk_1E_0 : 1;
    u8 unk_1E_1 : 1;
    u8 unk_1E_2 : 1;
    u8 unk_1E_3 : 1;
    u8 unk_1E_4 : 1;
    u8 : 3;
    u8 unk_1F;
} UnkStruct_ov61_0222BED8_sub1_sub1;

typedef struct {
    u16 unk_00[12];
    u8 unk_18[12];
    u16 unk_24;
    u8 unk_26;
    u8 unk_27;
    UnkStruct_ov61_0222BED8_sub1_sub1 unk_28;
    u16 unk_48;
    u8 unk_4A[14];
    u64 unk_58;
    UnkStruct_ov62_02239DA4_sub1 unk_60;
} UnkStruct_ov61_0222BED8_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u16 unk_04[4];
    u32 unk_0C;
    u32 unk_10;
    u16 unk_14[8];
    MailMessage unk_24;
    MailMessage unk_2C;
} UnkStruct_ov61_0222BED8_sub2_sub1_sub1;

typedef struct {
    u32 unk_00;
    int unk_04;
    int unk_08[4];
    UnkStruct_ov61_0222BED8_sub2_sub1_sub1 unk_18[4];
    int unk_E8;
    int unk_EC;
    int unk_F0;
    int unk_F4;
    int unk_F8;
    int unk_FC;
    int unk_100;
    int unk_104;
    int unk_108;
    int unk_10C;
    int unk_110;
    u32 unk_114[4];
    u32 unk_124;
    int unk_128;
    u32 unk_12C;
    u32 unk_130;
    int unk_134[4];
    u16 unk_144;
    u16 unk_146;
    int unk_148;
    u8 unk_14C[4];
} UnkStruct_ov61_0222BED8_sub2_sub1;

typedef struct {
    u8 unk_00[4][1024];
} UnkStruct_ov61_0222BED8_sub2_sub2;

typedef struct {
    u32 unk_00;
    u16 unk_04_0 : 1;
    u16 unk_04_1 : 1;
    u16 unk_04_2 : 1;
    u16 unk_04_3 : 13;
    u16 unk_06;
    u16 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
    u8 unk_18;
    u8 unk_19;
    u8 unk_1A;
    u8 unk_1B;
    u16 unk_1C[4];
    u8 unk_24[4];
    u8 unk_28[4];
    u32 unk_2C_0 : 5;
    u32 unk_2C_5 : 5;
    u32 unk_2C_10 : 5;
    u32 unk_2C_15 : 5;
    u32 unk_2C_20 : 5;
    u32 unk_2C_25 : 5;
    u32 unk_2C_30 : 1;
    u32 unk_2C_31 : 1;
    u8 unk_30_0 : 1;
    u8 unk_30_1 : 2;
    u8 unk_30_3 : 5;
    u16 unk_32[11];
    u16 unk_48[8];
    u8 unk_58;
    u8 unk_59;
    u32 unk_5C;
    u8 unk_60;
    u8 unk_61;
    u16 unk_62;
    u16 unk_64;
    u16 unk_66;
    u16 unk_68;
    u16 unk_6A;
    u16 unk_6C;
    u16 unk_6E;
} UnkStruct_ov61_0222BED8_sub2_sub3_sub1;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    UnkStruct_ov61_0222BED8_sub2_sub3_sub1 unk_04[6];
} UnkStruct_ov61_0222BED8_sub2_sub3;

typedef struct {
    u16 unk_00[8];
    u32 unk_10;
    u32 unk_14;
    u8 unk_18;
    u8 unk_19;
    u8 unk_1A;
    u8 unk_1B;
    u8 unk_1C;
    u8 unk_1D_0 : 1;
    u8 unk_1D_1 : 1;
    u8 unk_1D_2 : 6;
    u8 unk_1E;
    u8 unk_1F;
} UnkStruct_ov61_0222BED8_sub2_sub4;

typedef struct {
    u16 unk_00_0 : 4;
    u16 unk_00_4 : 2;
    u16 unk_00_6 : 1;
    u16 unk_00_7 : 1;
    u16 unk_00_8 : 2;
    u16 unk_00_10 : 5;
    u16 unk_00_15 : 1;
} UnkStruct_ov61_0222BED8_sub2_sub5;

typedef struct {
    UnkStruct_ov61_0222BED8_sub2_sub1 unk_00;
    UnkStruct_ov61_0222BED8_sub2_sub2 unk_150;
    UnkStruct_ov61_0222BED8_sub2_sub3 unk_1150[4];
    UnkStruct_ov61_0222BED8_sub2_sub4 unk_1B68[4];
    UnkStruct_ov61_0222BED8_sub2_sub5 unk_1BE8;
    u16 unk_1BEA;
    UnkStruct_ov62_02239DA4_sub1 unk_1BEC;
} UnkStruct_ov61_0222BED8_sub2;

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

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

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

void MessageFormat_Delete(MessageFormat *template);

void BufferCountryName(MessageFormat *template, u32 idx, u32 country);

void BufferCityName(MessageFormat *template, u32 idx, u32 country, u32 city);

void BufferMonthNameAbbr(MessageFormat *template, u32 idx, u32 month);

void MessageFormat_ResetBuffers(MessageFormat *template);

void InitWindow(Window *window);

void AddWindowParameterized(BgConfig *bgConfig, Window *window, u8 bgLayer, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 palette, u16 baseTile);

void ScheduleWindowCopyToVram(Window *window);

void FillWindowPixelBuffer(Window *window, u8 val);

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

typedef void (*TouchScreenHitTableAction)(u32, enum TouchScreenButtonState, void *);

typedef struct TouchScreenHitTableActionStateInternal TouchScreenHitTableActionStateInternal;

typedef struct TouchHitboxController {
    const TouchscreenHitbox *hitTables;
    u32 hitTablesLength;
    TouchScreenHitTableAction action;
    void *hitTableContext;
    TouchScreenHitTableActionStateInternal *actionStates;
} TouchHitboxController;

typedef struct {
    int unk_00;
    BOOL unk_04;
} UnkStruct_ov62_02233750;

typedef struct {
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov61_0222BED8_sub1 unk_80;
} UnkStruct_ov61_0222BE20;

typedef struct {
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov61_0222BED8_sub1 unk_80;
    UnkStruct_ov61_0222BED8_sub2 unk_E4;
} UnkStruct_ov61_0222BED8;

typedef struct {
    UnkStruct_ov61_0222BED8 *unk_00;
    UnkStruct_ov61_0222BED8 unk_04;
    u8 unk_1D54;
    u8 padding_1D55[3];
    UnkStruct_ov61_0222BE20 *unk_1D58[30];
    UnkStruct_ov61_0222BE20 unk_1DD0[30];
    int unk_3900;
    u8 unk_3904[32];
} UnkStruct_ov62_02233828;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int *unk_10;
    UnkStruct_ov62_0223118C unk_14;
    UnkStruct_ov62_02233750 unk_52C;
    UnkStruct_ov62_022307C0 unk_534;
    UnkStruct_0208B878 *unk_6F0;
    UnkStruct_0208B878 *unk_6F4;
    UnkStruct_ov62_02230C28 unk_6F8[6];
    const UnkStruct_020F3DCC *unk_818;
    const UnkStruct_020F3DCC *unk_81C[5];
    SaveData *saveData;
    u16 unk_834;
    u16 unk_836;
    UnkStruct_0208C06C_sub1 unk_838;
    void *unk_860;
    u8 padding_864[4];
    UnkStruct_0208BA84 *unk_868;
    int unk_86C;
    BOOL unk_870;
    u32 unk_874;
    u8 *unk_878;
    UnkStruct_0202F41C *unk_87C[4];
    UnkStruct_02030A80 *unk_88C[4];
    BOOL unk_89C;
    BOOL unk_8A0;
    Window unk_8A4;
    UnkStruct_ov62_02233828 unk_8B4;
    UnkStruct_0208C06C_sub2 unk_41D8;
    SysTask *unk_41E8;
} UnkStruct_0208C06C;

typedef struct {
    UnkStruct_02030A80 *unk_00;
    ManagedSprite *unk_04;
    ManagedSprite *unk_08;
    Window unk_0C[8];
} UnkStruct_ov62_022323B8;

MessageFormat *ov40_0222DAB0(enum HeapID heapID);

int ov40_022306C0(Window *param0, String *param1);

void ov40_02230DCC(UnkStruct_0208C06C *param0, String *param1);

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_0202F298_sub1;

struct UnkStruct_02030A80_t {
    u16 unk_00[8];
    u32 id;
    u8 gender;
    u8 month;
    u8 appearance;
    u8 country;
    u8 region;
    u8 version;
    u8 language;
    u8 isEgg : 1;
    u8 form : 7;
    u16 species;
    u8 unk_1E;
    u8 unk_1F;
    union {
        MailMessage unk_20_val1;
        u16 unk_20_val2[40];
    };
    u8 unk_70[12];
    UnkStruct_0202F298_sub1 unk_7C;
};

typedef struct BattleRegulation {
    u16 name[12];
    u16 maxTotalLevel;
    u8 teamSize;
    u8 maxLevel;
    s8 heightRestriction;
    s8 weightRestriction;
    u8 evolutionRestriction : 1;
    u8 specialSpeciesRestriction : 1;
    u8 speciesUniqueness : 1;
    u8 itemUniqueness : 1;
    u8 moveRestrictions : 1;
} BattleRegulation;

struct UnkStruct_0202F41C_t {
    u16 unk_00[12];
    u8 unk_18[12];
    u16 unk_24;
    u8 unk_26;
    u8 unk_27;
    BattleRegulation battleRegulation;
    u16 unk_48;
    u8 unk_4A[14];
    u64 unk_58;
    UnkStruct_0202F298_sub1 unk_60;
};

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

String *sub_020315B8(const UnkStruct_02030A80 *param0, enum HeapID heapID);

u8 sub_02031620(const UnkStruct_02030A80 *param0);

u8 sub_0203162C(const UnkStruct_02030A80 *param0);

String *sub_0203164C(const UnkStruct_02030A80 *param0, MailMessage *param1, enum HeapID heapID);

u8 sub_020316F0(const UnkStruct_02030A80 *param0);

u16 *ov62_Pokedex_Alphabetical(enum HeapID heapID, int unused, int *pokedexLength);

void ov62_022349E4(String *param0, enum HeapID heapID);

void ov40_0222E09C(UnkStruct_ov62_022323B8 *param0, UnkStruct_0208C06C *param1);

static s16 hgasset_ov40_0222E09C_Unk_ov62_02248A28[][4] __attribute__((section(BWPORT_DATA_SECTION))) = {
            { 0,  0,  0, 0 },
            { 0,  0,  0,  0 },
            { 0, 0,  0,  0 },
            { 0,  0,  0,  0 },
            { 0,  0,  0, 0 },
            { 0,  0,  0, 0 },
            { 0,  0, 0,  0 },
            { 0,  0, 0, 0 },
            { 0, 0,  0,  0 },
            { 0,  0, 0, 0 }
        };

void ov40_0222E09C(UnkStruct_ov62_022323B8 *param0, UnkStruct_0208C06C *param1) {
    int v0;
    int v1;
    Window *v2;
    String *v3;

    {
        __typeof__(hgasset_ov40_0222E09C_Unk_ov62_02248A28) Unk_ov62_02248A28; __builtin_memcpy((void *)&Unk_ov62_02248A28, &hgasset_ov40_0222E09C_Unk_ov62_02248A28, sizeof(Unk_ov62_02248A28));

        const int Unk_ov62_022489F8[] = {
            0x14,
            0x14,
            0x14,
            0x14,
            0x14,
            0x14,
            0x14,
            0x14,
            0x14,
            0x14,
            0x14,
            0x14
        };

        v1 = 1;

        for (v0 = 0; v0 < 8; v0++) {
            v2 = &param0->unk_0C[v0];
            v3 = NewString_ReadMsgData(param1->unk_14.unk_34, Unk_ov62_022489F8[v0]);
            InitWindow(v2);
            AddWindowParameterized(param1->unk_14.unk_10, v2, 2, Unk_ov62_02248A28[v0][0], Unk_ov62_02248A28[v0][1], Unk_ov62_02248A28[v0][2], Unk_ov62_02248A28[v0][3], 14, v1);
            FillWindowPixelBuffer(v2, 0x00);
            AddTextPrinterParameterizedWithColor(v2, FONT_SYSTEM, v3, ov40_022306C0(v2, v3), 0, (0xff), ((u32)(((15 & (0xff)) << 16) | ((13 & (0xff)) << 8) | ((0 & (0xff)) << 0))), 0L);
            ScheduleWindowCopyToVram(v2);
            v1 += (Unk_ov62_02248A28[v0][2] * Unk_ov62_02248A28[v0][3]);
            String_Delete(v3);
        }
    }

    {
        String *v6;
        String *v7;
        String *v8;
        MessageFormat *v9;
        Window *v10;
        UnkStruct_02030A80 *v11;

        v11 = param0->unk_00;
        v9 = ov40_0222DAB0(HEAP_ID_109);

        v10 = &param0->unk_0C[0];
        v6 = sub_020315B8(v11, HEAP_ID_109);
        ov40_02230DCC(param1, v6);
        v7 = NewString_ReadMsgData(param1->unk_14.unk_34, 13);
        v8 = String_New(255, HEAP_ID_109);

        BufferString(v9, 0, v6, 0, 1, LANGUAGE_ENGLISH);
        StringExpandPlaceholders(v9, v8, v7);

        FillWindowPixelBuffer(v10, 0x00);
        AddTextPrinterParameterizedWithColor(v10, FONT_SYSTEM, v8, ov40_022306C0(v10, v8), 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
        ScheduleWindowCopyToVram(v10);

        String_Delete(v6);
        String_Delete(v7);
        String_Delete(v8);
        MessageFormat_ResetBuffers(v9);

        {
            int v12 = sub_020316F0(v11);

            v10 = &param0->unk_0C[2];
            v6 = NewString_ReadMsgData(param1->unk_14.unk_34, 15);
            v7 = sub_020315B8(v11, HEAP_ID_109);
            ov40_02230DCC(param1, v7);
            v8 = String_New(255, HEAP_ID_109);

            BufferMonthNameAbbr(v9, 0, v12);
            StringExpandPlaceholders(v9, v8, v6);

            FillWindowPixelBuffer(v10, 0x00);
            AddTextPrinterParameterizedWithColor(v10, FONT_SYSTEM, v8, ov40_022306C0(v10, v8), 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
            ScheduleWindowCopyToVram(v10);

            String_Delete(v6);
            String_Delete(v7);
            String_Delete(v8);
            MessageFormat_ResetBuffers(v9);
        }

        {
            v10 = &param0->unk_0C[3];
            v8 = NewString_ReadMsgData(param1->unk_14.unk_34, 16);
            FillWindowPixelBuffer(v10, 0x00);
            AddTextPrinterParameterizedWithColor(v10, FONT_SYSTEM, v8, ov40_022306C0(v10, v8), 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
            ScheduleWindowCopyToVram(v10);
            String_Delete(v8);

            {
                int v13 = sub_02031620(v11);
                int v14 = sub_0203162C(v11);

                v10 = &param0->unk_0C[4];
                FillWindowPixelBuffer(v10, 0x00);
                if (v13 == 0) {
                    v8 = NewString_ReadMsgData(param1->unk_14.unk_34, 21);

                    AddTextPrinterParameterizedWithColor(v10, FONT_SYSTEM, v8, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
                    ScheduleWindowCopyToVram(v10);
                    String_Delete(v8);
                } else {
                    v8 = String_New(255, HEAP_ID_109);
                    v6 = NewString_ReadMsgData(param1->unk_14.unk_34, 22);
                    BufferCountryName(v9, 0, v13);
                    StringExpandPlaceholders(v9, v8, v6);

                    AddTextPrinterParameterizedWithColor(v10, FONT_SYSTEM, v8, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
                    ScheduleWindowCopyToVram(v10);
                    String_Delete(v8);
                    String_Delete(v6);
                    if (v14 != 0) {
                        v10 = &param0->unk_0C[5];
                        FillWindowPixelBuffer(v10, 0x00);
                        v8 = String_New(255, HEAP_ID_109);
                        v6 = NewString_ReadMsgData(param1->unk_14.unk_34, 23);
                        BufferCityName(v9, 0, v13, v14);
                        StringExpandPlaceholders(v9, v8, v6);

                        AddTextPrinterParameterizedWithColor(v10, FONT_SYSTEM, v8, 4, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
                        ScheduleWindowCopyToVram(v10);
                        String_Delete(v8);
                        String_Delete(v6);
                    }
                }

                MessageFormat_ResetBuffers(v9);
            }
        }

        {
            v10 = &param0->unk_0C[6];
            v8 = NewString_ReadMsgData(param1->unk_14.unk_34, 17);
            FillWindowPixelBuffer(v10, 0x00);
            AddTextPrinterParameterizedWithColor(v10, FONT_SYSTEM, v8, ov40_022306C0(v10, v8), 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
            ScheduleWindowCopyToVram(v10);
            String_Delete(v8);

            {
                MailMessage v15;

                v10 = &param0->unk_0C[7];

                v8 = sub_0203164C(v11, &v15, HEAP_ID_109);
                if (v8 == 0L) {
                    v8 = MailMsg_GetExpandedString(&v15, HEAP_ID_109);
                }

                FillWindowPixelBuffer(v10, 0x00);
                AddTextPrinterParameterizedWithColor(v10, FONT_SYSTEM, v8, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
                ScheduleWindowCopyToVram(v10);

                String_Delete(v8);
            }
        }

        MessageFormat_Delete(v9);
    }
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_overlay_40_0222DD9C_0222DFE8_0222E09C_c_0(void) {
    PORT_AssetAdd("IPKE", "src/overlay_40_0222DD9C_0222DFE8_0222E09C.c:hgasset_ov40_0222E09C_Unk_ov62_02248A28", (void *)&hgasset_ov40_0222E09C_Unk_ov62_02248A28, sizeof(hgasset_ov40_0222E09C_Unk_ov62_02248A28), "u8");
    PORT_AssetAdd("IPGE", "src/overlay_40_0222DD9C_0222DFE8_0222E09C.c:hgasset_ov40_0222E09C_Unk_ov62_02248A28", (void *)&hgasset_ov40_0222E09C_Unk_ov62_02248A28, sizeof(hgasset_ov40_0222E09C_Unk_ov62_02248A28), "u8");
}
