#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_40_unk_struct_ov62_02236cbc.h"
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
#include "overlay_40_unk_struct_ov62_02233310.h"
#include "save_vars_flags.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite_system.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct BgConfig;

struct CharTransferAllocation;

struct MailMessage;

struct TextOBJ;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct PaletteData;

struct Pokepic;

struct PokepicManager;

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

struct SaveVarsFlags;

struct Window;

typedef struct UnkStruct_0202F41C_t UnkStruct_0202F41C;

typedef struct UnkStruct_02030A80_t UnkStruct_02030A80;

typedef struct {
    int unk_00;
    enum HeapID heapID;
    int unk_08;
    int unk_0C;
    fx32 unk_10;
} UnkStruct_0208B284;

u16 *PaletteData_GetFadedBuf(PaletteData *palette, enum PaletteBufferId bufferID);

void PaletteData_SetAutoTransparent(PaletteData *paletteData, BOOL val);

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
    int unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_ov61_0222B138;

typedef struct {
    u8 unk_00;
    u8 unk_01[3];
    u32 unk_04;
} UnkStruct_ov61_0222B2D8;

typedef struct {
    BOOL unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_ov62_0223CAA4;

typedef void (*UnkFuncPtr_ov61_0222B920)(void *, const UnkStruct_ov62_0223CAA4 *);

typedef BOOL (*UnkFuncPtr_ov61_0222B920_1)(void *, void *);

typedef struct {
    UnkFuncPtr_ov61_0222B920 unk_00;
    u16 unk_04;
    u16 unk_06;
    u8 unk_08;
    UnkFuncPtr_ov61_0222B920_1 unk_0C;
} UnkStruct_ov61_0222B920;

typedef void (*UnkFuncPtr_ov62_022349A8)(void *, String *);

typedef struct {
    void *unk_00;
    UnkFuncPtr_ov61_0222B920 unk_04;
    UnkFuncPtr_ov61_0222B920 unk_08;
    UnkFuncPtr_ov61_0222B920 unk_0C;
    UnkFuncPtr_ov61_0222B920 unk_10;
    UnkFuncPtr_ov61_0222B920 unk_14;
    UnkFuncPtr_ov61_0222B920 unk_18;
    UnkFuncPtr_ov61_0222B920 unk_1C;
    UnkFuncPtr_ov61_0222B920 unk_20;
    UnkFuncPtr_ov61_0222B920 unk_24;
    UnkFuncPtr_ov61_0222B920 unk_28;
} UnkStruct_ov62_022349A8_sub2;

typedef struct {
    u32 personality;
    u32 otID;
    u16 species;
    u16 trainerName[7 + 1];
    s8 priority;
    u8 xPos;
    u8 yPos;
    u8 form;
} UnkStruct_ov61_0222AE80_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    s8 unk_03;
} UnkStruct_ov61_0222AE80_sub2;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov62_02239DA4_sub1;

typedef struct {
    u32 integrity;
    UnkStruct_ov61_0222AE80_sub1 unk_04;
    u32 unk_24;
    MailMessage title;
    UnkStruct_ov61_0222AE80_sub2 unk_30[10];
    u8 unk_58;
    u8 language;
    u8 unk_5A[2];
    UnkStruct_ov62_02239DA4_sub1 unk_5C;
} UnkStruct_ov61_0222AE80;

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
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov61_0222AE80 unk_80;
} UnkStruct_ov62_022349A8_sub3_sub1;

typedef struct {
    u16 unk_00[20];
    u16 unk_28[30];
    u32 unk_64[30];
    u32 unk_DC[30];
    u32 unk_154;
    u8 unk_158[30];
    u8 unk_176;
    u8 unk_177;
    u8 unk_178[32];
    UnkStruct_ov62_02239DA4_sub1 unk_198;
} UnkStruct_ov62_02239DA4;

typedef struct {
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov62_02239DA4 unk_80;
} UnkStruct_ov62_022349A8_sub3_sub2;

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

typedef struct {
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov61_0222BED8_sub1 unk_80;
    UnkStruct_ov61_0222BED8_sub2 unk_E4;
} UnkStruct_ov62_022349A8_sub3_sub3;

typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 unk_05[3];
    u32 unk_08;
    u8 unk_0C[4];
} UnkStruct_ov62_022349A8_sub3_sub4;

typedef struct {
    u32 unk_00;
} UnkStruct_ov62_022349A8_sub3_sub5;

typedef struct {
    u8 version;
    u8 language;
    u8 month;
    u8 appearance;
    u16 species;
    u8 isEgg : 1;
    u8 form : 7;
    u8 unk_07;
} UnkStruct_ov61_0222AFCC;

typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov62_0223D518_sub1_sub1;

typedef struct {
    UnkStruct_ov61_0222AFCC unk_00;
    UnkStruct_ov62_0223D518_sub1_sub1 unk_08;
    UnkStruct_ov61_0222B2D8 unk_0C[3];
} UnkStruct_ov62_0223D518_sub1;

typedef struct {
    union {
        UnkStruct_ov62_022349A8_sub3_sub1 unk_00_val1;
        UnkStruct_ov62_022349A8_sub3_sub2 unk_00_val2;
        UnkStruct_ov62_0223D518_sub1 unk_00_val3;
        UnkStruct_ov62_022349A8_sub3_sub3 *unk_00_val4;
        UnkStruct_ov62_022349A8_sub3_sub4 unk_00_val5;
        UnkStruct_ov62_022349A8_sub3_sub5 unk_00_val6;
    };
    union {
        struct {
            u8 unk_00;
            u8 unk_01;
            u8 unk_02[2];
        } val1;
        struct {
            u16 unk_00;
            u16 unk_02;
        } val2;
        struct {
            u64 unk_00;
        } val3;
    } unk_21C;
} UnkStruct_ov62_022349A8_sub3;

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

typedef struct {
    s32 unk_00;
    u8 unk_04;
    u8 language;
    char unk_06[301];
    u8 padding[5];
} UnkStruct_ov62_022349A8_sub1;

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

typedef struct {
    SaveData *saveData;
    UnkFuncPtr_ov62_022349A8 unk_04;
    void *unk_08;
    UnkStruct_ov62_022349A8_sub1 unk_0C;
    enum HeapID heapID;
    int unk_148;
    int unk_14C;
    UnkStruct_ov62_022349A8_sub2 unk_150;
    UnkStruct_ov62_0223CAA4 unk_17C;
    void *unk_18C;
    UnkStruct_ov62_022349A8_sub3 unk_190;
    void *unk_3B4;
    int unk_3B8;
    int unk_3BC;
    int unk_3C0;
    BOOL unk_3C4;
    BOOL unk_3C8;
    UnkStruct_ov61_0222B138 unk_3CC;
    int unk_3D8;
    int unk_3DC;
    int unk_3E0;
    int unk_3E4;
    int unk_3E8;
    int unk_3EC;
    int unk_3F0;
    struct MsgData *unk_3F4;
    MessageFormat *unk_3F8;
    String *unk_3FC;
    UnkStruct_ov61_0222B920 unk_400;
    u8 unk_40D;
    u8 unk_40E;
} UnkStruct_ov62_022349A8;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    BOOL unk_18;
    u8 padding_1C[4];
} UnkStruct_ov62_0223118C_sub1;

typedef struct {
    int unk_00;
    BOOL unk_04;
    u32 unk_08;
    u32 unk_0C;
    s16 unk_10;
    u8 padding_12[2];
    BOOL unk_14;
    ManagedSprite *unk_18[2];
} UnkStruct_ov62_0223359C;

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
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    u8 padding_0C[4];
    BgConfig *unk_10;
    PaletteData *unk_14;
    TouchHitboxController *unk_18;
    TouchScreenRect unk_1C[5];
    BOOL unk_30;
    struct MsgData *unk_34;
    struct MsgData *unk_38;
    UnkStruct_02012744 *unk_3C;
    int unk_40;
    int unk_44;
    u8 unk_48;
    u8 padding_49[3];
    G3DPipelineBuffers *unk_4C;
    PokepicManager *unk_50;
    UnkStruct_ov62_022349A8 unk_54;
    UnkStruct_ov62_0223359C unk_46C;
    UnkStruct_ov62_02233310 unk_48C;
    u8 padding_4D8[24];
    UnkStruct_ov62_0223118C_sub1 unk_4F0;
    BOOL unk_510;
    BOOL unk_514;
} UnkStruct_ov62_0223118C;

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

SaveVarsFlags *Save_VarsFlags_Get(SaveData *saveData);

u16 Save_VarsFlags_GetBattleCastlePrintProgress(SaveVarsFlags *varsFlags);

u16 Save_VarsFlags_GetBattleArcadePrintProgress(SaveVarsFlags *varsFlags);

u16 Save_VarsFlags_GetVar4052(SaveVarsFlags *varsFlags);

u16 SystemVars_GetBattleArcadePrintState(SaveVarsFlags *varsFlags);

u16 Save_VarsFlags_GetBattleHallPrintProgress(SaveVarsFlags *varsFlags);

u16 Save_VarsFlags_GetBattleTowerPrintProgress(SaveVarsFlags *varsFlags);

u16 Save_VarsFlags_GetBattleFactoryPrintProgress(SaveVarsFlags *varsFlags);

void ov62_022370D4(u16 *param0, u16 param1, u16 param2);

void ov40_02233550(UnkStruct_0208C06C *param0);

void ov40_022334F8(u16 *param0, u16 param1, u16 param2);

typedef struct {
    int a[5];
} ov40_02245174_Array;

extern const ov40_02245174_Array ov40_02245174;

void ov40_02233550(UnkStruct_0208C06C *param0) {
    UnkStruct_ov62_02236CBC *v0 = param0->unk_860;
    BgConfig *v1 = param0->unk_14.unk_10;
    SpriteSystem *v2 = param0->unk_14.unk_04;
    SpriteManager *v3 = param0->unk_14.unk_08;
    PaletteData *v4 = param0->unk_14.unk_14;
    int v5;
    int v6[5];

    {
        SaveVarsFlags *v7 = Save_VarsFlags_Get(param0->saveData);

        v6[0] = Save_VarsFlags_GetBattleTowerPrintProgress(v7);
        v6[1] = Save_VarsFlags_GetBattleFactoryPrintProgress(v7);
        v6[2] = Save_VarsFlags_GetBattleArcadePrintProgress(v7);
        v6[3] = Save_VarsFlags_GetBattleCastlePrintProgress(v7);
        v6[4] = Save_VarsFlags_GetBattleHallPrintProgress(v7);
    }

    for (v5 = 0; v5 < 5; v5++) {
        if (v6[v5] != 0) {
            ov40_02245174_Array v8 = ov40_02245174;

            if ((u32)(v6[v5] - 2) <= 1) {
                PaletteData *v9 = param0->unk_14.unk_14;
                u16 *v10;

                v10 = PaletteData_GetFadedBuf(v9, (enum PaletteBufferId)(2));
                ov40_022334F8(v10, (4 + v8.a[v5]) * 16, 16);
            }
        }
    }

    PaletteData_SetAutoTransparent(v4, 1);
}

struct ManagedSpriteTemplate;

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

void ManagedSprite_TickFrame(ManagedSprite *managedSprite);

void ManagedSprite_SetDrawFlag(ManagedSprite *managedSprite, BOOL draw);

void ManagedSprite_SetPaletteOverride(ManagedSprite *managedSprite, int palette);

void ManagedSprite_SetAffineScale(ManagedSprite *managedSprite, f32 xScale, f32 yScale);

void ov40_0222D288(ManagedSprite *param0, s16 param1, s16 param2);

void ov40_022335F4(UnkStruct_0208C06C *param0);

typedef struct {
    int a[5];
} ov40_022451B0_Array;

extern const ov40_022451B0_Array ov40_022451B0;

void ov40_022335F4(UnkStruct_0208C06C *param0) {
    int v0;
    int v1 = 0;
    int v2[4 + 1];
    int v3[5];
    UnkStruct_ov62_02236CBC *v4 = param0->unk_860;
    BgConfig *v5 = param0->unk_14.unk_10;
    SpriteSystem *v6 = param0->unk_14.unk_04;
    SpriteManager *v7 = param0->unk_14.unk_08;
    PaletteData *v8 = param0->unk_14.unk_14;
    ManagedSpriteTemplate v9;

    v9.x = 0;
    v9.y = 256;
    v9.z = 0;
    v9.animation = 0;
    v9.drawPriority = 10;
    v9.pal = 0;
    v9.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    v9.bgPriority = 0;
    v9.vramTransfer = 0;
    v9.resIdList[4] = -1;
    v9.resIdList[5] = -1;
    v9.resIdList[0] = 165535;
    v9.resIdList[1] = 165535;
    v9.resIdList[2] = 165535;
    v9.resIdList[3] = 165535;

    for (v0 = 0; v0 < 5; v0++) {
        v4->unk_40[v0] = SpriteSystem_NewSprite(param0->unk_14.unk_04, param0->unk_14.unk_08, &v9);
        ManagedSprite_SetAnim(v4->unk_40[v0], 1);
        ManagedSprite_TickFrame(v4->unk_40[v0]);
        ManagedSprite_SetAffineScale(v4->unk_40[v0], v4->unk_04[v0], v4->unk_04[v0]);
    }

    for (v0 = 0; v0 < 5; v0++) {
        v9.resIdList[0] = 18181 + v0;
        v9.resIdList[1] = 18181 + v0;

        if (v0 == 3) {
            v9.resIdList[2] = 18181 + 1;
            v9.resIdList[3] = 18181 + 1;
        } else {
            v9.resIdList[2] = 18181;
            v9.resIdList[3] = 18181;
        }

        v4->unk_54[v0] = SpriteSystem_NewSprite(param0->unk_14.unk_04, param0->unk_14.unk_08, &v9);

        if (v0 == 3) {
            ManagedSprite_SetAnim(v4->unk_54[v0], 0);
        } else {
            ManagedSprite_SetAnim(v4->unk_54[v0], 1);
        }

        ManagedSprite_TickFrame(v4->unk_54[v0]);
        ManagedSprite_SetAffineScale(v4->unk_54[v0], v4->unk_04[v0], v4->unk_04[v0]);
        ManagedSprite_SetPaletteOverride(v4->unk_54[v0], 10 + v0);
    }

    v9.resIdList[0] = 28282;
    v9.resIdList[1] = 28282;
    v9.resIdList[2] = 28282;
    v9.resIdList[3] = 28282;
    v9.resIdList[4] = -1;
    v9.resIdList[5] = -1;
    v9.drawPriority = 20;

    {
        SaveVarsFlags *v10 = Save_VarsFlags_Get(param0->saveData);

        v3[0] = Save_VarsFlags_GetBattleTowerPrintProgress(v10);
        v3[1] = Save_VarsFlags_GetBattleFactoryPrintProgress(v10);
        v3[2] = Save_VarsFlags_GetBattleArcadePrintProgress(v10);
        v3[3] = Save_VarsFlags_GetBattleCastlePrintProgress(v10);
        v3[4] = Save_VarsFlags_GetBattleHallPrintProgress(v10);
    }

    for (v0 = 0; v0 < 5; v0++) {
        v4->unk_68[v0] = SpriteSystem_NewSprite(param0->unk_14.unk_04, param0->unk_14.unk_08, &v9);

        if ((u32)v3[v0] <= 1) {
            ManagedSprite_SetAnim(v4->unk_68[v0], 5);
            ManagedSprite_SetPaletteOverride(v4->unk_68[v0], 4 + 5);
            ManagedSprite_SetDrawFlag(v4->unk_54[v0], 0);
            ManagedSprite_SetDrawFlag(v4->unk_40[v0], 0);
        } else {
            ov40_022451B0_Array v11 = ov40_022451B0;

            ManagedSprite_SetAnim(v4->unk_68[v0], v11.a[v0]);
            ManagedSprite_SetPaletteOverride(v4->unk_68[v0], 4 + v11.a[v0]);
        }

        ManagedSprite_TickFrame(v4->unk_68[v0]);
        ManagedSprite_SetAffineScale(v4->unk_68[v0], v4->unk_04[v0], v4->unk_04[v0]);
    }

    v9.resIdList[0] = 29292;
    v9.resIdList[1] = 29292;
    v9.resIdList[2] = 29292;
    v9.resIdList[3] = 29292;
    v9.resIdList[4] = -1;
    v9.resIdList[5] = -1;
    v9.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    v9.drawPriority = 0;

    {
        int v12 = 1000;
        int v13 = v4->unk_90;
        BOOL v14 = 0;

        for (v0 = 0; v0 < 4; v0++) {
            v2[v0] = v13 / v12;
            v13 %= v12;
            v12 /= 10;

            if ((v2[v0] == 0) && (v14 == 0)) {
                v1++;
            } else {
                v14 = 1;
            }
        }

        v2[4] = 10;
    }

    for (v0 = 0; v0 < 4 + 1; v0++) {
        v4->unk_7C[v0] = SpriteSystem_NewSprite(param0->unk_14.unk_04, param0->unk_14.unk_08, &v9);

        if (v2[v0] == 0) {
            if (v0 < v1) {
                ManagedSprite_SetDrawFlag(v4->unk_7C[v0], 0);
            }
        }

        ManagedSprite_SetAnim(v4->unk_7C[v0], v2[v0]);
        ov40_0222D288(v4->unk_7C[v0], 88 + (16 * v0), 72);
        ManagedSprite_TickFrame(v4->unk_7C[v0]);
    }

    ManagedSprite_SetDrawFlag(v4->unk_7C[4 - 1], 1);
    ManagedSprite_SetDrawFlag(v4->unk_7C[4 - 0], 1);

    for (v0 = 0; v0 < 5; v0++) {
        ManagedSprite_SetAffineOverwriteMode(v4->unk_40[v0], AFFINE_OVERWRITE_MODE_DOUBLE);
        ManagedSprite_SetAffineOverwriteMode(v4->unk_54[v0], AFFINE_OVERWRITE_MODE_DOUBLE);
        ManagedSprite_SetAffineOverwriteMode(v4->unk_68[v0], AFFINE_OVERWRITE_MODE_DOUBLE);
    }
}
