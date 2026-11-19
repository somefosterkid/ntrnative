#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sys_task.h"
#include "unk_02013534.h"
#include "pm_string.h"
#include "mail_message_types.h"
#include "save.h"
#include "pokepic.h"
#include "message_format.h"
#include "overlay_39_unk_struct_ov62_022349_a8.h"
#include "overlay_40_unk_struct_ov62_02233310.h"
#include "overlay_40_unk_struct_ov62_0223118c.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite_system.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct BattleRegulation;

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

struct ManagedSpriteTemplate;

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

BOOL SpriteSystem_LoadCellResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

void ManagedSprite_TickFrame(ManagedSprite *managedSprite);

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
    UnkStruct_0202F41C *unk_04;
    u8 padding_08[4];
    ManagedSprite *unk_0C[12];
    Window unk_3C[8];
} UnkStruct_ov62_02233F74;

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

u32 GetMonIconNaixEx(u32 species, u32 isEgg, u32 form);

const u8 GetMonIconPaletteEx(u32 species, u32 form, u32 isEgg);

u32 sub_02074490(void);

u32 sub_0207449C(void);

u32 sub_020744A8(void);

u64 sub_0203088C(UnkStruct_0202F41C *param0, int param1, int param2);

MessageFormat *ov40_0222DAB0(enum HeapID heapID);

u16 *ov62_Pokedex_Alphabetical(enum HeapID heapID, int unused, int *pokedexLength);

BOOL ov40_02230404(UnkStruct_ov62_02233F74 *param0);

void ov62_022349E4(String *param0, enum HeapID heapID);

void ov40_02230424(UnkStruct_ov62_02233F74 *param0, UnkStruct_0208C06C *param1);

void ov40_02230424(UnkStruct_ov62_02233F74 *param0, UnkStruct_0208C06C *param1) {
    int v0;
    int v1;
    int v2;
    int v3 = 0;
    ManagedSpriteTemplate v4;
    int v5;
    int v6;
    int v7[12];
    int v8[12];
    int v9;
    SpriteSystem *v10;
    SpriteManager *v11;
    PaletteData *v12;
    s16 v13[][12][2] = {
        {
         { 11 * 8, 13 * 8 },
         { 14 * 8, 13 * 8 },
         { 17 * 8, 13 * 8 },
         { 20 * 8, 13 * 8 },
         { 23 * 8, 13 * 8 },
         { 26 * 8, 13 * 8 },
         { 3 * 8, 0x98 },
         { 6 * 8, 0x98 },
         { 9 * 8, 0x98 },
         { 12 * 8, 0x98 },
         { 15 * 8, 0x98 },
         { 18 * 8, 0x98 },
         },
        {
         { 3 * 8, 17 * 8 },
         { 6 * 8, 17 * 8 },
         { 9 * 8, 17 * 8 },
         { 5 * 8, 20 * 8 },
         { 8 * 8, 20 * 8 },
         { 11 * 8, 20 * 8 },
         { 18 * 8, 17 * 8 },
         { 21 * 8, 17 * 8 },
         { 24 * 8, 17 * 8 },
         { 20 * 8, 20 * 8 },
         { 23 * 8, 20 * 8 },
         { 26 * 8, 20 * 8 },
         },
    };

    v10 = param1->unk_14.unk_04;
    v11 = param1->unk_14.unk_08;
    v12 = param1->unk_14.unk_14;

    SpriteSystem_LoadPaletteBuffer(v12, (enum PaletteBufferId)(2), v10, v11, (enum NarcId)(20), sub_02074490(), 0, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 100000);

    {
        int v14 = sub_0207449C();
        SpriteSystem_LoadCellResObj(v10, v11, (enum NarcId)(20), v14, 0, 100000);
    }

    {
        int v15 = sub_020744A8();
        SpriteSystem_LoadAnimResObj(v10, v11, (enum NarcId)(20), v15, 0, 100000);
    }

    {
        int v16 = 0;
        int v17 = 0;
        int v18 = 0;
        int v19 = 6;
        int v20 = 0;
        int v21 = v19;

        v9 = 0;

        if (ov40_02230404(param0) == 1) {
            v19 = 3;
            v21 = 3;
            v9 = 1;
        }

        do {
            for (v0 = v20; v0 < v21; v0++) {
                v7[v16] = 0xFF;
                v17 = sub_0203088C(param0->unk_04, 0, v0);
                v18 = sub_0203088C(param0->unk_04, 1, v0);

                if (v17 == 0) {
                    continue;
                }

                v7[v16] = v17;
                v8[v16] = v18;

                v16++;
            }

            for (v0 = v16; v0 < v21; v0++) {
                v7[v16] = 0;
                v8[v16] = 0;
                v16++;
            }

            v20 += v19;
            v21 += v19;
        } while (v21 <= 12);
    }

    for (v0 = 0; v0 < 12; v0++) {
        param0->unk_0C[v0] = 0L;
        v1 = v7[v0];

        if (v1 == 0) {
            continue;
        }

        v2 = v8[v0];
        v6 = GetMonIconNaixEx(v1, 0, v2);

        SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(v10, v11, (enum NarcId)20, v6, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 100000 + v3);

        v4.x = v13[v9][v0][0] + 12 - 4;
        v4.y = v13[v9][v0][1] - 12;
        v4.z = 0;
        v4.animation = 0;
        v4.drawPriority = 0;
        v4.pal = 0;
        v4.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        v4.bgPriority = 0;
        v4.vramTransfer = 0;
        v4.resIdList[0] = 100000 + v3;
        v4.resIdList[1] = 100000;
        v4.resIdList[2] = 100000;
        v4.resIdList[3] = 100000;
        v4.resIdList[4] = -1;
        v4.resIdList[5] = -1;

        param0->unk_0C[v3] = SpriteSystem_NewSprite(param1->unk_14.unk_04, param1->unk_14.unk_08, &v4);

        v5 = GetMonIconPaletteEx(v1, v2, 0);
        ManagedSprite_SetPaletteOverrideOffset(param0->unk_0C[v3], 4 + v5);
        ManagedSprite_SetAnim(param0->unk_0C[v3], 1);
        ManagedSprite_TickFrame(param0->unk_0C[v3]);
        ManagedSprite_SetDrawPriority(param0->unk_0C[v3], 12 - v0);
        v3++;
    }
}
