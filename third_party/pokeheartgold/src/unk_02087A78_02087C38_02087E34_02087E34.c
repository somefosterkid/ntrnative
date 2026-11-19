#include "global.h"
#include "pokemon_summary_args.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"
#include "constants/heap.h"
#include "overlay_40_unk_struct_0208_b878.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "sav_chatot.h"
#include "pokepic.h"
#include "overlay_39_unk_struct_ov62_022349_a8.h"
#include "overlay_40_unk_struct_ov62_02233310.h"
#include "overlay_40_unk_struct_ov62_0223118c.h"
#include "map_object.h"
#include "bag_cursor.h"
#include "save_palpad.h"
#include "overlay_12_field_battle_dto.h"
#include "script.h"
#include "unk_0202C034.h"
#include "pokemon_summary_app.h"
#include "list_menu.h"
#include "overlay_65_trade_room.h"
#include "field/model_attributes.h"
#include "field/area_light.h"
#include "field/dynamic_terrain_height.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_land_data_manager.h"
#include "field/map_prop.h"
#include "field/signpost.h"
#include "map_matrix.h"
#include "terrain_attributes.h"
#include "field/terrain_collision_manager.h"
#include "trainer_case.h"
#include "union_room_manager.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "unk_struct_0208bc3c.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct AreaDataManager;

struct AreaLightManager;

struct BagCursor;

struct BattleRegulation;

struct BattleTower;

struct BgConfig;

struct Camera;

struct CharTransferAllocation;

struct SOUND_CHATOT;

struct ColoredArrow;

struct DynamicTerrainHeightManager;

struct MailMessage;

struct FieldBattleDTO;

struct FieldEffectManager;

struct FieldProcessManager;



struct FieldTask;

struct FieldWildBattleMetadata;

struct FogManager;

struct TextOBJ;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct HoneyTreeShakeList;

struct JournalEntry;

struct LandDataManager;

struct ListMenu;

struct ListMenuTemplate;

struct Location;

struct ManagedSprite;

struct MapHeaderData;

struct MapLoadMode;

struct MapMatrix;

struct MapObjectManager;

struct MapObjectsToPreload;

struct MapPropAnimationManager;

struct MapPropManager;

struct MapPropOneShotAnimationManager;

struct Menu;

struct MsgData;

struct ModelAttributes;

struct NARC;

struct Options;

struct SavePalPad;

struct PaletteData;

struct Party;

struct PlayerAvatar;

struct PokedexMemory;

struct PokepicManager;

struct PokepicTemplate;

struct PokemonSummary;

struct RadarChain;

struct SaveData;

struct Signpost;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct SpriteSystem;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct TerrainAttributes;

struct TerrainCollisionManager;

struct TouchScreenActions;

struct TradeRoom;

struct TradeRoomArgs;

struct TradeRoomMonDisplayData;

struct TrainerCase;

struct PlayerProfile;

struct UndergroundTopScreenContext_t;

struct UnionRoomManager;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct UnkStruct_0202F41C_t;

struct UnkStruct_02030A80_t;

struct UnkStruct_0205C22C_t;

struct UnkStruct_020F3DCC_t;

struct UnkStruct_ov56_02256468_t;

struct UnkStruct_ov5_021EB0E0;

struct WiFiList;

struct Window;

typedef struct {
    BOOL unk_00;
    int unk_04;
    u8 padding_08[8];
} UnkStruct_0208BA84;

typedef struct UnkStruct_0202F41C_t UnkStruct_0202F41C;

typedef struct UnkStruct_02030A80_t UnkStruct_02030A80;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

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

typedef u16 charcode_t;

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

typedef struct {
    s32 unk_00;
    u8 unk_04;
    u8 language;
    char unk_06[301];
    u8 padding[5];
} UnkStruct_ov62_022349A8_sub1;

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
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_ov62_02233310_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    u64 unk_08;
} UnkStruct_ov62_02249380;

typedef struct {
    const UnkStruct_ov62_02249380 *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    u32 unk_24;
    const TouchScreenRect *unk_28;
} UnkStruct_ov62_02248CDC;

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

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct TouchScreenActions TouchScreenActions;

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
    u32 unk_14_;
    struct { struct NARC *unk_00; struct SpriteSystem *unk_04; struct SpriteManager *unk_08; u8 padding_0C[4]; struct BgConfig *unk_10; struct PaletteData *unk_14; struct TouchScreenActions *unk_18; u8 unk_1C[20]; BOOL unk_30; struct MsgData *unk_34; struct MsgData *unk_38; struct UnkStruct_02012744_t *unk_3C; int unk_40; int unk_44; u8 unk_48; u8 padding_49[3]; struct G3DPipelineBuffers *unk_4C; struct PokepicManager *unk_50; u8 unk_58_pad[4]; struct { struct SaveData *saveData; void *unk_04; void *unk_08; struct { s32 unk_00; u8 unk_04; u8 language; s8 unk_06[301]; u8 padding[5]; } unk_0C; u32 heapID; int unk_148; int unk_14C; struct { void *unk_00; void *unk_04; void *unk_08; void *unk_0C; void *unk_10; void *unk_14; void *unk_18; void *unk_1C; void *unk_20; void *unk_24; void *unk_28; } unk_150; struct { BOOL unk_00; int unk_04; int unk_08; int unk_0C; } unk_17C; void *unk_18C; struct { u8 unk_0_anon[540]; u8 unk_220_pad[4]; u8 unk_21C[8]; } unk_190; void *unk_3B4; int unk_3B8; int unk_3BC; int unk_3C0; BOOL unk_3C4; BOOL unk_3C8; struct { int unk_00; int unk_04; int unk_08; } unk_3CC; int unk_3D8; int unk_3DC; int unk_3E0; int unk_3E4; int unk_3E8; int unk_3EC; int unk_3F0; struct MsgData *unk_3F4; struct MessageFormat *unk_3F8; struct String *unk_3FC; struct { void *unk_00; u16 unk_04; u16 unk_06; u8 unk_08; u8 unk_C_pad[3]; void *unk_0C; } unk_400; u8 unk_40D; u8 unk_40E; u8 unk_416_tail[2]; } unk_54; struct { int unk_00; BOOL unk_04; u32 unk_08; u32 unk_0C; s16 unk_10; u8 padding_12[2]; BOOL unk_14; struct ManagedSprite *unk_18[2]; } unk_46C; struct { int unk_00; int unk_04; s16 unk_08; u8 padding_0A[2]; int unk_0C; int unk_10; int unk_14; Window unk_18; void *unk_28; struct ManagedSprite *unk_2C[2]; struct MsgData *unk_34; struct { int unk_00; int unk_04; int unk_08; int unk_0C; } unk_38; BOOL unk_48; u8 padding_4C[4]; } unk_48C; u8 padding_4D8[24]; struct { int unk_00; int unk_04; int unk_08; int unk_0C; int unk_10; int unk_14; BOOL unk_18; u8 padding_1C[4]; } unk_4F0; BOOL unk_510; BOOL unk_514; } unk_14;
    UnkStruct_ov62_02233750 unk_52C;
    UnkStruct_ov62_022307C0 unk_534;
    void *unk_6F0;
    void *unk_6F4;
    UnkStruct_ov62_02230C28 unk_6F8[6];
    struct UnkStruct_020F3DCC_t *unk_818;
    struct UnkStruct_020F3DCC_t *unk_81C[5];
    struct SaveData *saveData;
    u16 unk_834;
    u16 unk_836;
    UnkStruct_0208C06C_sub1 unk_838;
    void *unk_860;
    void *unk_870_;
    void *unk_868;
    int unk_86C;
    BOOL unk_870;
    u32 unk_874;
    u8 *unk_878;
    struct UnkStruct_0202F41C_t *unk_87C[4];
    struct UnkStruct_02030A80_t *unk_88C[4];
    BOOL unk_89C;
    BOOL unk_8A0;
    Window unk_8A4;
    struct { void *unk_00; u8 unk_8_pad[4]; struct { struct { u16 unk_00[8]; u32 unk_10; u8 unk_14; u8 unk_15; u8 unk_16; u8 unk_17; u8 unk_18; u8 unk_19; u8 unk_1A; u8 unk_1B_0 : 1; u8 unk_1B_1 : 7; u16 unk_1C; u8 unk_1E; u8 unk_1F; u8 unk_20_anon[80]; u8 unk_70[12]; struct { u16 unk_00; u16 unk_02; } unk_7C; } unk_00; struct { u16 unk_00[12]; u8 unk_18[12]; u16 unk_24; u8 unk_26; u8 unk_27; u8 unk_28[32]; u16 unk_48; u8 unk_4A[14]; u64 unk_58; struct { u16 unk_00; u16 unk_02; } unk_60; u8 unk_64_tail[4]; } unk_80; struct { struct { u32 unk_00; int unk_04; int unk_08[4]; struct { u8 unk_00; u8 unk_01; u8 unk_02; u8 unk_03; u16 unk_04[4]; u32 unk_0C; u32 unk_10; u16 unk_14[8]; struct { u16 type; u16 id; u16 words[2]; } unk_24; struct { u16 type; u16 id; u16 words[2]; } unk_2C; } unk_18[4]; int unk_E8; int unk_EC; int unk_F0; int unk_F4; int unk_F8; int unk_FC; int unk_100; int unk_104; int unk_108; int unk_10C; int unk_110; u32 unk_114[4]; u32 unk_124; int unk_128; u32 unk_12C; u32 unk_130; int unk_134[4]; u16 unk_144; u16 unk_146; int unk_148; u8 unk_14C[4]; } unk_00; struct { u8 unk_00[1024][4]; } unk_150; struct { u16 unk_00; u16 unk_02; struct { u32 unk_00; u16 unk_04_0 : 1; u16 unk_04_1 : 1; u16 unk_04_2 : 1; u16 unk_04_3 : 13; u16 unk_06; u16 unk_08; u8 unk_C_pad[2]; u32 unk_0C; u32 unk_10; u8 unk_14; u8 unk_15; u8 unk_16; u8 unk_17; u8 unk_18; u8 unk_19; u8 unk_1A; u8 unk_1B; u16 unk_1C[4]; u8 unk_24[4]; u8 unk_28[4]; u32 unk_2C_0 : 5; u32 unk_2C_5 : 5; u32 unk_2C_10 : 5; u32 unk_2C_15 : 5; u32 unk_2C_20 : 5; u32 unk_2C_25 : 5; u32 unk_2C_30 : 1; u32 unk_2C_31 : 1; u8 unk_30_0 : 1; u8 unk_30_1 : 2; u8 unk_30_3 : 5; u16 unk_32[11]; u16 unk_48[8]; u8 unk_58; u8 unk_59; u8 unk_5C_pad[2]; u32 unk_5C; u8 unk_60; u8 unk_61; u16 unk_62; u16 unk_64; u16 unk_66; u16 unk_68; u16 unk_6A; u16 unk_6C; u16 unk_6E; } unk_04[6]; } unk_1150[4]; struct { u16 unk_00[8]; u32 unk_10; u32 unk_14; u8 unk_18; u8 unk_19; u8 unk_1A; u8 unk_1B; u8 unk_1C; u8 unk_1D_0 : 1; u8 unk_1D_1 : 1; u8 unk_1D_2 : 6; u8 unk_1E; u8 unk_1F; } unk_1B68[4]; struct { u16 unk_00_0 : 4; u16 unk_00_4 : 2; u16 unk_00_6 : 1; u16 unk_00_7 : 1; u16 unk_00_8 : 2; u16 unk_00_10 : 5; u16 unk_00_15 : 1; } unk_1BE8; u16 unk_1BEA; struct { u16 unk_00; u16 unk_02; } unk_1BEC; } unk_E4; } unk_04; u8 unk_1D54; u8 padding_1D55[3]; void *unk_1D58[30]; u8 unk_1DD8_pad[4]; struct { struct { u16 unk_00[8]; u32 unk_10; u8 unk_14; u8 unk_15; u8 unk_16; u8 unk_17; u8 unk_18; u8 unk_19; u8 unk_1A; u8 unk_1B_0 : 1; u8 unk_1B_1 : 7; u16 unk_1C; u8 unk_1E; u8 unk_1F; u8 unk_20_anon[80]; u8 unk_70[12]; struct { u16 unk_00; u16 unk_02; } unk_7C; } unk_00; struct { u16 unk_00[12]; u8 unk_18[12]; u16 unk_24; u8 unk_26; u8 unk_27; u8 unk_28[32]; u16 unk_48; u8 unk_4A[14]; u64 unk_58; struct { u16 unk_00; u16 unk_02; } unk_60; u8 unk_64_tail[4]; } unk_80; } unk_1DD0[30]; int unk_3900; u8 unk_3904[32]; u8 unk_392C_tail[4]; } unk_8B4;
    UnkStruct_0208C06C_sub2 unk_41D8;
    struct SysTask *unk_41E8;
    u8 pad_4204[4];
} UnkStruct_0208C06C;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

enum MapHeaderID {
    MAP_HEADER_EVERYWHERE,
    MAP_HEADER_NOTHING,
    MAP_HEADER_UNDERGROUND,
    MAP_HEADER_JUBILIFE_CITY,
    MAP_HEADER_JUBILIFE_CITY_MART,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_HOUSE_1,
    MAP_HEADER_JUBILIFE_CITY_POKECENTER_1F,
    MAP_HEADER_JUBILIFE_CITY_POKECENTER_2F,
    MAP_HEADER_POKETCH_CO_1F,
    MAP_HEADER_POKETCH_CO_2F,
    MAP_HEADER_POKETCH_CO_3F,
    MAP_HEADER_JUBILIFE_TV_1F,
    MAP_HEADER_JUBILIFE_TV_2F,
    MAP_HEADER_JUBILIFE_TV_3F,
    MAP_HEADER_JUBILIFE_TV_4F,
    MAP_HEADER_JUBILIFE_TV_2F_GALLERY,
    MAP_HEADER_JUBILIFE_TV_3F_GLOBAL_RANKING_ROOM,
    MAP_HEADER_JUBILIFE_TV_3F_GROUP_RANKING_ROOM,
    MAP_HEADER_JUBILIFE_TV_ELEVATOR,
    MAP_HEADER_JUBILIFE_CITY_SOUTH_HOUSE_1F,
    MAP_HEADER_JUBILIFE_CITY_SOUTH_HOUSE_2F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_SOUTH_HOUSE_3F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_SOUTH_HOUSE_4F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_HOUSE_2,
    MAP_HEADER_JUBILIFE_CITY_CONDOMINIUMS_1F,
    MAP_HEADER_JUBILIFE_CITY_CONDOMINIUMS_2F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_CONDOMINIUMS_3F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_CONDOMINIUMS_4F,
    MAP_HEADER_GLOBAL_TERMINAL_1F,
    MAP_HEADER_TRAINERS_SCHOOL,
    MAP_HEADER_JUBILIFE_CITY_SOUTHWEST_HOUSE_1F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_HOUSE_3,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_HOUSE_4,
    MAP_HEADER_CANALAVE_CITY,
    MAP_HEADER_CANALAVE_CITY_MART,
    MAP_HEADER_CANALAVE_CITY_GYM,
    MAP_HEADER_CANALAVE_CITY_POKECENTER_1F,
    MAP_HEADER_CANALAVE_CITY_POKECENTER_2F,
    MAP_HEADER_CANALAVE_LIBRARY_1F,
    MAP_HEADER_CANALAVE_LIBRARY_2F,
    MAP_HEADER_CANALAVE_LIBRARY_3F,
    MAP_HEADER_CANALAVE_CITY_SOUTHEAST_HOUSE,
    MAP_HEADER_CANALAVE_CITY_EAST_HOUSE,
    MAP_HEADER_CANALAVE_CITY_HARBOR_INN,
    MAP_HEADER_CANALAVE_CITY_SAILOR_ELDRITCH_HOUSE,
    MAP_HEADER_OREBURGH_CITY,
    MAP_HEADER_OREBURGH_CITY_MART,
    MAP_HEADER_OREBURGH_CITY_GYM,
    MAP_HEADER_OREBURGH_CITY_POKECENTER_1F,
    MAP_HEADER_OREBURGH_CITY_POKECENTER_2F,
    MAP_HEADER_OREBURGH_CITY_NORTHWEST_HOUSE_1F,
    MAP_HEADER_OREBURGH_CITY_NORTHWEST_HOUSE_2F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_NORTHWEST_HOUSE_3F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_NORTHWEST_HOUSE_4F,
    MAP_HEADER_OREBURGH_CITY_NORTH_HOUSE_1F,
    MAP_HEADER_OREBURGH_CITY_NORTH_HOUSE_2F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_NORTH_HOUSE_3F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_NORTH_HOUSE_4F,
    MAP_HEADER_OREBURGH_CITY_MIDDLE_HOUSE,
    MAP_HEADER_MINING_MUSEUM,
    MAP_HEADER_OREBURGH_CITY_WEST_HOUSE,
    MAP_HEADER_OREBURGH_CITY_EAST_HOUSE_1F,
    MAP_HEADER_OREBURGH_CITY_EAST_HOUSE_2F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_EAST_HOUSE_3F,
    MAP_HEADER_OREBURGH_CITY_SOUTH_HOUSE,
    MAP_HEADER_ETERNA_CITY,
    MAP_HEADER_ETERNA_CITY_MART,
    MAP_HEADER_ETERNA_CITY_GYM,
    MAP_HEADER_ETERNA_CITY_DP_GYM,
    MAP_HEADER_ETERNA_CITY_POKECENTER_1F,
    MAP_HEADER_ETERNA_CITY_POKECENTER_2F,
    MAP_HEADER_CYCLE_SHOP,
    MAP_HEADER_TEAM_GALACTIC_ETERNA_BUILDING_1F,
    MAP_HEADER_TEAM_GALACTIC_ETERNA_BUILDING_2F,
    MAP_HEADER_TEAM_GALACTIC_ETERNA_BUILDING_3F,
    MAP_HEADER_TEAM_GALACTIC_ETERNA_BUILDING_4F,
    MAP_HEADER_ETERNA_CITY_CONDOMINIUMS_1F,
    MAP_HEADER_ETERNA_CITY_CONDOMINIUMS_2F,
    MAP_HEADER_ETERNA_CITY_CONDOMINIUMS_3F,
    MAP_HEADER_UNUSED_ETERNA_CITY_CONDOMINIUMS_4F,
    MAP_HEADER_ROUTE_206_CYCLING_ROAD_NORTH_GATE,
    MAP_HEADER_ETERNA_CITY_HERB_SHOP,
    MAP_HEADER_ETERNA_CITY_SOUTH_HOUSE,
    MAP_HEADER_ETERNA_CITY_EAST_HOUSE,
    MAP_HEADER_ETERNA_CITY_UNDERGROUND_MAN_HOUSE,
    MAP_HEADER_UNUSED_ETERNA_CITY_HOUSE,
    MAP_HEADER_HEARTHOME_CITY,
    MAP_HEADER_HEARTHOME_CITY_MART,
    MAP_HEADER_HEARTHOME_CITY_GYM_ENTRANCE_ROOM,
    MAP_HEADER_HEARTHOME_CITY_GYM_TRAINER_ROOM_1,
    MAP_HEADER_HEARTHOME_CITY_GYM_TRAINER_ROOM_2,
    MAP_HEADER_HEARTHOME_CITY_GYM_LEADER_ROOM,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_1,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_ELEVATOR_ROOM_1,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_2,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_ELEVATOR_ROOM_2,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_3,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_4,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_5,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_6,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_LEADER_ROOM,
    MAP_HEADER_HEARTHOME_CITY_POKECENTER_1F,
    MAP_HEADER_HEARTHOME_CITY_POKECENTER_2F,
    MAP_HEADER_HEARTHOME_CITY_SOUTHEAST_HOUSE_1F,
    MAP_HEADER_HEARTHOME_CITY_SOUTHEAST_HOUSE_2F,
    MAP_HEADER_HEARTHOME_CITY_SOUTHEAST_HOUSE_ELEVATOR,
    MAP_HEADER_HEARTHOME_CITY_POKEMON_FAN_CLUB,
    MAP_HEADER_HEARTHOME_CITY_WEST_GATE_TO_AMITY_SQUARE,
    MAP_HEADER_HEARTHOME_CITY_EAST_GATE_TO_AMITY_SQUARE,
    MAP_HEADER_ROUTE_208_GATE_TO_HEARTHOME_CITY,
    MAP_HEADER_ROUTE_209_GATE_TO_HEARTHOME_CITY,
    MAP_HEADER_ROUTE_212_GATE_TO_HEARTHOME_CITY,
    MAP_HEADER_HEARTHOME_CITY_NORTHEAST_HOUSE_1F,
    MAP_HEADER_HEARTHOME_CITY_NORTHEAST_HOUSE_2F,
    MAP_HEADER_HEARTHOME_CITY_NORTHEAST_HOUSE_ELEVATOR,
    MAP_HEADER_HEARTHOME_CITY_NORTHWEST_HOUSE,
    MAP_HEADER_POFFIN_HOUSE,
    MAP_HEADER_CONTEST_HALL_LOBBY,
    MAP_HEADER_CONTEST_HALL_STAGE_ONGOING_CONTEST,
    MAP_HEADER_FOREIGN_BUILDING,
    MAP_HEADER_PASTORIA_CITY,
    MAP_HEADER_PASTORIA_CITY_MART,
    MAP_HEADER_PASTORIA_CITY_GYM,
    MAP_HEADER_PASTORIA_CITY_POKECENTER_1F,
    MAP_HEADER_PASTORIA_CITY_POKECENTER_2F,
    MAP_HEADER_PASTORIA_CITY_OBSERVATORY_GATE_1F,
    MAP_HEADER_PASTORIA_CITY_OBSERVATORY_GATE_2F,
    MAP_HEADER_PASTORIA_CITY_SOUTHWEST_HOUSE,
    MAP_HEADER_PASTORIA_CITY_MIDDLE_HOUSE,
    MAP_HEADER_PASTORIA_CITY_EAST_HOUSE,
    MAP_HEADER_PASTORIA_CITY_NORTH_HOUSE,
    MAP_HEADER_PASTORIA_CITY_NORTHEAST_HOUSE,
    MAP_HEADER_VEILSTONE_CITY,
    MAP_HEADER_VEILSTONE_CITY_GYM,
    MAP_HEADER_VEILSTONE_CITY_POKECENTER_1F,
    MAP_HEADER_VEILSTONE_CITY_POKECENTER_2F,
    MAP_HEADER_GAME_CORNER,
    MAP_HEADER_VEILSTONE_STORE_1F,
    MAP_HEADER_VEILSTONE_STORE_2F,
    MAP_HEADER_VEILSTONE_STORE_3F,
    MAP_HEADER_VEILSTONE_STORE_4F,
    MAP_HEADER_VEILSTONE_STORE_5F,
    MAP_HEADER_VEILSTONE_STORE_ELEVATOR,
    MAP_HEADER_VEILSTONE_CITY_GALACTIC_WAREHOUSE,
    MAP_HEADER_VEILSTONE_CITY_PRIZE_EXCHANGE,
    MAP_HEADER_VEILSTONE_CITY_SOUTHEAST_HOUSE,
    MAP_HEADER_VEILSTONE_CITY_NORTHWEST_HOUSE,
    MAP_HEADER_VEILSTONE_CITY_NORTHEAST_HOUSE,
    MAP_HEADER_VEILSTONE_CITY_SOUTHWEST_HOUSE,
    MAP_HEADER_ROUTE_215_GATE_TO_VEILSTONE_CITY,
    MAP_HEADER_SUNYSHORE_CITY,
    MAP_HEADER_SUNYSHORE_CITY_POKECENTER_1F,
    MAP_HEADER_SUNYSHORE_CITY_POKECENTER_2F,
    MAP_HEADER_SUNYSHORE_CITY_MART,
    MAP_HEADER_SUNYSHORE_CITY_GYM_ROOM_1,
    MAP_HEADER_SUNYSHORE_CITY_GYM_ROOM_2,
    MAP_HEADER_SUNYSHORE_CITY_GYM_ROOM_3,
    MAP_HEADER_SUNYSHORE_MARKET,
    MAP_HEADER_SUNYSHORE_CITY_NORTHEAST_HOUSE,
    MAP_HEADER_SUNYSHORE_CITY_WEST_HOUSE,
    MAP_HEADER_SUNYSHORE_CITY_NORTHWEST_HOUSE,
    MAP_HEADER_UNUSED_SUNYSHORE_CITY_HOUSE_1,
    MAP_HEADER_UNUSED_SUNYSHORE_CITY_HOUSE_2,
    MAP_HEADER_SUNYSHORE_CITY_EAST_HOUSE,
    MAP_HEADER_VISTA_LIGHTHOUSE,
    MAP_HEADER_SNOWPOINT_CITY,
    MAP_HEADER_SNOWPOINT_CITY_MART,
    MAP_HEADER_SNOWPOINT_CITY_GYM,
    MAP_HEADER_SNOWPOINT_CITY_POKECENTER_1F,
    MAP_HEADER_SNOWPOINT_CITY_POKECENTER_2F,
    MAP_HEADER_SNOWPOINT_CITY_WEST_HOUSE,
    MAP_HEADER_SNOWPOINT_CITY_EAST_HOUSE,
    MAP_HEADER_POKEMON_LEAGUE,
    MAP_HEADER_POKEMON_LEAGUE_SOUTH_POKECENTER_1F,
    MAP_HEADER_POKEMON_LEAGUE_SOUTH_POKECENTER_2F,
    MAP_HEADER_POKEMON_LEAGUE_NORTH_POKECENTER_1F,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_AARON_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_AARON_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_BERTHA_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_BERTHA_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_FLINT_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_FLINT_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_LUCIAN_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_LUCIAN_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_CHAMPION_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_CHAMPION_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_HALLWAY_TO_HALL_OF_FAME,
    MAP_HEADER_POKEMON_LEAGUE_HALL_OF_FAME,
    MAP_HEADER_FIGHT_AREA,
    MAP_HEADER_FIGHT_AREA_POKECENTER_1F,
    MAP_HEADER_FIGHT_AREA_POKECENTER_2F,
    MAP_HEADER_FIGHT_AREA_MART,
    MAP_HEADER_UNUSED_BATTLE_PARK_GATE_TO_FIGHT_AREA,
    MAP_HEADER_ROUTE_225_GATE_TO_FIGHT_AREA,
    MAP_HEADER_FIGHT_AREA_MIDDLE_HOUSE,
    MAP_HEADER_FIGHT_AREA_SOUTH_HOUSE,
    MAP_HEADER_UNUSED_FIGHT_AREA_HOUSE,
    MAP_HEADER_UNKNOWN_197,
    MAP_HEADER_OREBURGH_MINE_B1F,
    MAP_HEADER_OREBURGH_MINE_B2F,
    MAP_HEADER_VALLEY_WINDWORKS_OUTSIDE,
    MAP_HEADER_VALLEY_WINDWORKS_BUILDING,
    MAP_HEADER_ETERNA_FOREST_OUTSIDE,
    MAP_HEADER_ETERNA_FOREST,
    MAP_HEADER_FUEGO_IRONWORKS_OUTSIDE,
    MAP_HEADER_FUEGO_IRONWORKS_BUILDING,
    MAP_HEADER_UNKNOWN_206,
    MAP_HEADER_MT_CORONET_1F_SOUTH,
    MAP_HEADER_MT_CORONET_2F,
    MAP_HEADER_MT_CORONET_3F,
    MAP_HEADER_MT_CORONET_OUTSIDE_NORTH,
    MAP_HEADER_MT_CORONET_OUTSIDE_SOUTH,
    MAP_HEADER_MT_CORONET_4F_ROOMS_1_AND_2,
    MAP_HEADER_MT_CORONET_4F_ROOM_3,
    MAP_HEADER_MT_CORONET_5F,
    MAP_HEADER_MT_CORONET_6F,
    MAP_HEADER_MT_CORONET_1F_TUNNEL_ROOM,
    MAP_HEADER_MT_CORONET_1F_NORTH_ROOM_2,
    MAP_HEADER_MT_CORONET_1F_NORTH_ROOM_1,
    MAP_HEADER_MT_CORONET_B1F,
    MAP_HEADER_SPEAR_PILLAR,
    MAP_HEADER_SPEAR_PILLAR_DISTORTED,
    MAP_HEADER_UNKNOWN_222,
    MAP_HEADER_PASTORIA_CITY_DP_GREAT_MARSH,
    MAP_HEADER_UNKNOWN_224,
    MAP_HEADER_SOLACEON_RUINS_MANIAC_TUNNEL_ROOM,
    MAP_HEADER_SOLACEON_RUINS_ROOM_1,
    MAP_HEADER_SOLACEON_RUINS_ROOM_2_NORTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_1_NORTHWEST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_2,
    MAP_HEADER_SOLACEON_RUINS_ROOM_1_SOUTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_3,
    MAP_HEADER_SOLACEON_RUINS_ROOM_2_SOUTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_6_SOUTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_5_SOUTHWEST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_3_NORTHWEST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_3_SOUTHWEST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_4,
    MAP_HEADER_SOLACEON_RUINS_ROOM_6,
    MAP_HEADER_SOLACEON_RUINS_ROOM_5,
    MAP_HEADER_SOLACEON_RUINS_ROOM_7,
    MAP_HEADER_SOLACEON_RUINS_ROOM_4_SOUTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_6_NORTHWEST_DEAD_END,
    MAP_HEADER_UNKNOWN_243,
    MAP_HEADER_VICTORY_ROAD_1F,
    MAP_HEADER_VICTORY_ROAD_2F,
    MAP_HEADER_VICTORY_ROAD_B1F,
    MAP_HEADER_VICTORY_ROAD_1F_ROOM_2,
    MAP_HEADER_VICTORY_ROAD_1F_ROOM_1,
    MAP_HEADER_VICTORY_ROAD_1F_ROOM_3,
    MAP_HEADER_UNKNOWN_250,
    MAP_HEADER_PAL_PARK,
    MAP_HEADER_UNKNOWN_252,
    MAP_HEADER_AMITY_SQUARE,
    MAP_HEADER_RAVAGED_PATH,
    MAP_HEADER_UNKNOWN_255,
    MAP_HEADER_FLOAROMA_MEADOW,
    MAP_HEADER_FLOAROMA_MEADOW_HOUSE,
    MAP_HEADER_OREBURGH_GATE_1F,
    MAP_HEADER_OREBURGH_GATE_B1F,
    MAP_HEADER_FULLMOON_ISLAND,
    MAP_HEADER_FULLMOON_ISLAND_FOREST,
    MAP_HEADER_STARK_MOUNTAIN_OUTSIDE,
    MAP_HEADER_STARK_MOUNTAIN_ROOM_1,
    MAP_HEADER_STARK_MOUNTAIN_ROOM_2,
    MAP_HEADER_STARK_MOUNTAIN_ROOM_3,
    MAP_HEADER_UNKNOWN_266,
    MAP_HEADER_SENDOFF_SPRING,
    MAP_HEADER_TURNBACK_CAVE_ENTRANCE,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_ROOM,
    MAP_HEADER_TURNBACK_CAVE_GIRATINA_ROOM,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_1,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_2,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_3,
    MAP_HEADER_FLOWER_PARADISE,
    MAP_HEADER_UNKNOWN_275,
    MAP_HEADER_UNKNOWN_276,
    MAP_HEADER_UNKNOWN_277,
    MAP_HEADER_SNOWPOINT_TEMPLE_1F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B1F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B2F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B3F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B4F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B5F,
    MAP_HEADER_WAYWARD_CAVE_1F,
    MAP_HEADER_WAYWARD_CAVE_B1F,
    MAP_HEADER_RUIN_MANIAC_CAVE_SHORT,
    MAP_HEADER_TROPHY_GARDEN,
    MAP_HEADER_IRON_ISLAND,
    MAP_HEADER_IRON_ISLAND_1F,
    MAP_HEADER_IRON_ISLAND_B1F_LEFT_ROOM,
    MAP_HEADER_IRON_ISLAND_B1F_RIGHT_ROOM,
    MAP_HEADER_IRON_ISLAND_B2F_RIGHT_ROOM,
    MAP_HEADER_IRON_ISLAND_B2F_LEFT_ROOM,
    MAP_HEADER_IRON_ISLAND_B3F,
    MAP_HEADER_OLD_CHATEAU,
    MAP_HEADER_OLD_CHATEAU_DINING_AREA,
    MAP_HEADER_OLD_CHATEAU_SIDE_ROOMS,
    MAP_HEADER_OLD_CHATEAU_CORRIDOR,
    MAP_HEADER_OLD_CHATEAU_BACK_WEST_ROOM,
    MAP_HEADER_OLD_CHATEAU_BACK_MIDDLE_WEST_ROOM,
    MAP_HEADER_OLD_CHATEAU_BACK_MIDDLE_ROOM,
    MAP_HEADER_OLD_CHATEAU_BACK_MIDDLE_EAST_ROOM,
    MAP_HEADER_OLD_CHATEAU_BACK_EAST_ROOM,
    MAP_HEADER_UNKNOWN_304,
    MAP_HEADER_GALACTIC_HQ_1F,
    MAP_HEADER_GALACTIC_HQ_2F,
    MAP_HEADER_GALACTIC_HQ_3F,
    MAP_HEADER_GALACTIC_HQ_4F,
    MAP_HEADER_GALACTIC_HQ_B1F,
    MAP_HEADER_GALACTIC_HQ_B2F,
    MAP_HEADER_LAKE_VERITY_LOW_WATER,
    MAP_HEADER_LAKE_VERITY,
    MAP_HEADER_VERITY_CAVERN,
    MAP_HEADER_LAKE_VALOR_DRAINED,
    MAP_HEADER_LAKE_VALOR,
    MAP_HEADER_VALOR_CAVERN,
    MAP_HEADER_LAKE_ACUITY_LOW_WATER,
    MAP_HEADER_LAKE_ACUITY,
    MAP_HEADER_ACUITY_CAVERN,
    MAP_HEADER_NEWMOON_ISLAND,
    MAP_HEADER_NEWMOON_ISLAND_FOREST,
    MAP_HEADER_UNUSED_BATTLE_PARK,
    MAP_HEADER_UNUSED_BATTLE_PARK_EXCHANGE_SERVICE_CORNER,
    MAP_HEADER_UNKNOWN_324,
    MAP_HEADER_UNKNOWN_325,
    MAP_HEADER_BATTLE_TOWER,
    MAP_HEADER_BATTLE_TOWER_ELEVATOR,
    MAP_HEADER_BATTLE_TOWER_CORRIDOR,
    MAP_HEADER_BATTLE_TOWER_CORRIDOR_MULTI,
    MAP_HEADER_BATTLE_TOWER_BATTLE_ROOM,
    MAP_HEADER_BATTLE_TOWER_MULTI_BATTLE_ROOM,
    MAP_HEADER_COMMUNICATION_CLUB_COLOSSEUM_2P,
    MAP_HEADER_COMMUNICATION_CLUB_COLOSSEUM_4P,
    MAP_HEADER_VERITY_LAKEFRONT,
    MAP_HEADER_UNUSED_VERITY_LAKEFRONT_HOUSE,
    MAP_HEADER_VALOR_LAKEFRONT,
    MAP_HEADER_RESTAURANT,
    MAP_HEADER_GRAND_LAKE_VALOR_LAKEFRONT_EAST_HOUSE,
    MAP_HEADER_GRAND_LAKE_VALOR_LAKEFRONT_WEST_HOUSE,
    MAP_HEADER_ACUITY_LAKEFRONT,
    MAP_HEADER_SPRING_PATH,
    MAP_HEADER_ROUTE_201,
    MAP_HEADER_ROUTE_202,
    MAP_HEADER_ROUTE_203,
    MAP_HEADER_ROUTE_204_SOUTH,
    MAP_HEADER_ROUTE_204_NORTH,
    MAP_HEADER_ROUTE_205_SOUTH,
    MAP_HEADER_ROUTE_205_HOUSE,
    MAP_HEADER_ROUTE_205_NORTH,
    MAP_HEADER_ROUTE_206,
    MAP_HEADER_ROUTE_206_CYCLING_ROAD_SOUTH_GATE,
    MAP_HEADER_UNUSED_GATE_BETWEEN_ETERNA_CITY_ROUTE_206,
    MAP_HEADER_ROUTE_207,
    MAP_HEADER_ROUTE_208,
    MAP_HEADER_ROUTE_208_HOUSE,
    MAP_HEADER_ROUTE_209,
    MAP_HEADER_ROUTE_209_LOST_TOWER_1F,
    MAP_HEADER_ROUTE_209_LOST_TOWER_2F,
    MAP_HEADER_ROUTE_209_LOST_TOWER_3F,
    MAP_HEADER_ROUTE_209_LOST_TOWER_4F,
    MAP_HEADER_ROUTE_209_LOST_TOWER_5F,
    MAP_HEADER_ROUTE_210_SOUTH,
    MAP_HEADER_ROUTE_210_NORTH,
    MAP_HEADER_ROUTE_210_GRANDMA_WILMA_HOUSE,
    MAP_HEADER_ROUTE_211_WEST,
    MAP_HEADER_ROUTE_211_EAST,
    MAP_HEADER_ROUTE_212_NORTH,
    MAP_HEADER_POKEMON_MANSION,
    MAP_HEADER_POKEMON_MANSION_MAIDS_ROOM,
    MAP_HEADER_POKEMON_MANSION_OFFICE,
    MAP_HEADER_ROUTE_212_SOUTH,
    MAP_HEADER_ROUTE_212_HOUSE,
    MAP_HEADER_ROUTE_213,
    MAP_HEADER_ROUTE_213_GATE_TO_PASTORIA_CITY,
    MAP_HEADER_FOOTSTEP_HOUSE,
    MAP_HEADER_GRAND_LAKE_ROUTE_213_LOBBY,
    MAP_HEADER_GRAND_LAKE_ROUTE_213_EAST_HOUSE,
    MAP_HEADER_GRAND_LAKE_ROUTE_213_NORTHWEST_HOUSE,
    MAP_HEADER_GRAND_LAKE_ROUTE_213_NORTHEAST_HOUSE,
    MAP_HEADER_ROUTE_214,
    MAP_HEADER_ROUTE_214_GATE_TO_VEILSTONE_CITY,
    MAP_HEADER_ROUTE_215,
    MAP_HEADER_ROUTE_216,
    MAP_HEADER_ROUTE_216_HOUSE,
    MAP_HEADER_ROUTE_217,
    MAP_HEADER_ROUTE_217_WEST_HOUSE,
    MAP_HEADER_ROUTE_217_NORTHEAST_HOUSE,
    MAP_HEADER_ROUTE_218,
    MAP_HEADER_ROUTE_218_GATE_TO_JUBILIFE_CITY,
    MAP_HEADER_ROUTE_218_GATE_TO_CANALAVE_CITY,
    MAP_HEADER_ROUTE_219,
    MAP_HEADER_ROUTE_221,
    MAP_HEADER_PAL_PARK_LOBBY,
    MAP_HEADER_ROUTE_221_HOUSE,
    MAP_HEADER_ROUTE_222,
    MAP_HEADER_ROUTE_222_WEST_HOUSE,
    MAP_HEADER_ROUTE_222_EAST_HOUSE,
    MAP_HEADER_ROUTE_222_GATE_TO_SUNYSHORE_CITY,
    MAP_HEADER_ROUTE_224,
    MAP_HEADER_ROUTE_225,
    MAP_HEADER_UNKNOWN_401,
    MAP_HEADER_UNKNOWN_402,
    MAP_HEADER_ROUTE_227,
    MAP_HEADER_UNKNOWN_404,
    MAP_HEADER_UNKNOWN_405,
    MAP_HEADER_ROUTE_228,
    MAP_HEADER_ROUTE_229,
    MAP_HEADER_UNKNOWN_408,
    MAP_HEADER_UNKNOWN_409,
    MAP_HEADER_RECORD_MIXING_ROOM,
    MAP_HEADER_TWINLEAF_TOWN,
    MAP_HEADER_TWINLEAF_TOWN_RIVAL_HOUSE_1F,
    MAP_HEADER_TWINLEAF_TOWN_RIVAL_HOUSE_2F,
    MAP_HEADER_TWINLEAF_TOWN_PLAYER_HOUSE_1F,
    MAP_HEADER_TWINLEAF_TOWN_PLAYER_HOUSE_2F,
    MAP_HEADER_TWINLEAF_TOWN_NORTHEAST_HOUSE,
    MAP_HEADER_TWINLEAF_TOWN_SOUTHWEST_HOUSE,
    MAP_HEADER_SANDGEM_TOWN,
    MAP_HEADER_SANDGEM_TOWN_MART,
    MAP_HEADER_SANDGEM_TOWN_POKECENTER_1F,
    MAP_HEADER_SANDGEM_TOWN_POKECENTER_2F,
    MAP_HEADER_SANDGEM_TOWN_POKEMON_RESEARCH_LAB,
    MAP_HEADER_SANDGEM_TOWN_COUNTERPART_HOUSE_1F,
    MAP_HEADER_SANDGEM_TOWN_COUNTERPART_HOUSE_2F,
    MAP_HEADER_SANDGEM_TOWN_HOUSE,
    MAP_HEADER_FLOAROMA_TOWN,
    MAP_HEADER_FLOAROMA_TOWN_MART,
    MAP_HEADER_FLOAROMA_TOWN_POKECENTER_1F,
    MAP_HEADER_FLOAROMA_TOWN_POKECENTER_2F,
    MAP_HEADER_FLOWER_SHOP,
    MAP_HEADER_FLOAROMA_TOWN_SOUTHEAST_HOUSE,
    MAP_HEADER_FLOAROMA_TOWN_MIDDLE_HOUSE,
    MAP_HEADER_SOLACEON_TOWN,
    MAP_HEADER_SOLACEON_TOWN_MART,
    MAP_HEADER_SOLACEON_TOWN_POKECENTER_1F,
    MAP_HEADER_SOLACEON_TOWN_POKECENTER_2F,
    MAP_HEADER_POKEMON_DAY_CARE,
    MAP_HEADER_SOLACEON_TOWN_NORTHEAST_HOUSE,
    MAP_HEADER_SOLACEON_TOWN_POKEMON_NEWS_PRESS,
    MAP_HEADER_SOLACEON_TOWN_NORTH_HOUSE,
    MAP_HEADER_SOLACEON_TOWN_EAST_HOUSE,
    MAP_HEADER_CELESTIC_TOWN,
    MAP_HEADER_CELESTIC_TOWN_POKECENTER_1F,
    MAP_HEADER_CELESTIC_TOWN_POKECENTER_2F,
    MAP_HEADER_CELESTIC_TOWN_NORTH_HOUSE,
    MAP_HEADER_CELESTIC_TOWN_NORTHWEST_HOUSE,
    MAP_HEADER_CELESTIC_TOWN_NORTHEAST_HOUSE,
    MAP_HEADER_CELESTIC_TOWN_SOUTHWEST_HOUSE,
    MAP_HEADER_CELESTIC_TOWN_CAVE,
    MAP_HEADER_SURVIVAL_AREA,
    MAP_HEADER_SURVIVAL_AREA_MART,
    MAP_HEADER_SURVIVAL_AREA_POKECENTER_1F,
    MAP_HEADER_SURVIVAL_AREA_POKECENTER_2F,
    MAP_HEADER_BATTLEGROUND,
    MAP_HEADER_SURVIVAL_AREA_SOUTH_HOUSE,
    MAP_HEADER_SURVIVAL_AREA_NORTH_HOUSE,
    MAP_HEADER_RESORT_AREA,
    MAP_HEADER_UNUSED_RESORT_AREA_MART,
    MAP_HEADER_RESORT_AREA_POKECENTER_1F,
    MAP_HEADER_RESORT_AREA_POKECENTER_2F,
    MAP_HEADER_RESORT_AREA_RIBBON_SYNDICATE_1F,
    MAP_HEADER_RESORT_AREA_RIBBON_SYNDICATE_2F,
    MAP_HEADER_RESORT_AREA_RIBBON_SYNDICATE_ELEVATOR,
    MAP_HEADER_VILLA,
    MAP_HEADER_RESORT_AREA_HOUSE,
    MAP_HEADER_UNION_ROOM,
    MAP_HEADER_ROUTE_220,
    MAP_HEADER_ROUTE_223,
    MAP_HEADER_ROUTE_226,
    MAP_HEADER_UNKNOWN_470,
    MAP_HEADER_ROUTE_230,
    MAP_HEADER_SEABREAK_PATH,
    MAP_HEADER_UNKNOWN_473,
    MAP_HEADER_JUBILIFE_CITY_POKECENTER_B1F,
    MAP_HEADER_CANALAVE_CITY_POKECENTER_B1F,
    MAP_HEADER_OREBURGH_CITY_POKECENTER_B1F,
    MAP_HEADER_ETERNA_CITY_POKECENTER_B1F,
    MAP_HEADER_HEARTHOME_CITY_POKECENTER_B1F,
    MAP_HEADER_PASTORIA_CITY_POKECENTER_B1F,
    MAP_HEADER_VEILSTONE_CITY_POKECENTER_B1F,
    MAP_HEADER_SUNYSHORE_CITY_POKECENTER_B1F,
    MAP_HEADER_SNOWPOINT_CITY_POKECENTER_B1F,
    MAP_HEADER_POKEMON_LEAGUE_SOUTH_POKECENTER_B1F,
    MAP_HEADER_FIGHT_AREA_POKECENTER_B1F,
    MAP_HEADER_SANDGEM_TOWN_POKECENTER_B1F,
    MAP_HEADER_FLOAROMA_TOWN_POKECENTER_B1F,
    MAP_HEADER_SOLACEON_TOWN_POKECENTER_B1F,
    MAP_HEADER_CELESTIC_TOWN_POKECENTER_B1F,
    MAP_HEADER_SURVIVAL_AREA_POKECENTER_B1F,
    MAP_HEADER_RESORT_AREA_POKECENTER_B1F,
    MAP_HEADER_CANALAVE_CITY_WEST_HOUSE,
    MAP_HEADER_CAFE,
    MAP_HEADER_BATTLE_TOWER_BATTLE_SALON,
    MAP_HEADER_GALACTIC_HQ_CONTROL_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_NORTH_POKECENTER_2F,
    MAP_HEADER_POKEMON_LEAGUE_NORTH_POKECENTER_B1F,
    MAP_HEADER_GALACTIC_HQ_LABORATORY,
    MAP_HEADER_ROUTE_225_HOUSE,
    MAP_HEADER_ROUTE_226_HOUSE,
    MAP_HEADER_ROUTE_227_HOUSE,
    MAP_HEADER_ROUTE_228_GATE_TO_ROUTE_226,
    MAP_HEADER_ROUTE_228_NORTH_HOUSE,
    MAP_HEADER_ROUTE_228_SOUTH_HOUSE,
    MAP_HEADER_GREAT_MARSH_1,
    MAP_HEADER_GREAT_MARSH_2,
    MAP_HEADER_GREAT_MARSH_3,
    MAP_HEADER_GREAT_MARSH_4,
    MAP_HEADER_GREAT_MARSH_5,
    MAP_HEADER_GREAT_MARSH_6,
    MAP_HEADER_HALL_OF_ORIGIN,
    MAP_HEADER_UNKNOWN_511,
    MAP_HEADER_RUIN_MANIAC_CAVE_LONG,
    MAP_HEADER_MANIAC_TUNNEL,
    MAP_HEADER_IRON_ISLAND_HOUSE,
    MAP_HEADER_SOLACEON_RUINS_ROOM_5_SOUTHEAST_DEADEND,
    MAP_HEADER_VISTA_LIGHTHOUSE_ELEVATOR,
    MAP_HEADER_JUBILIFE_CITY_SOUTHWEST_HOUSE_2F,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_4,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_5,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_6,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_1,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_2,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_3,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_4,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_5,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_6,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_1,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_2,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_3,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_4,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_5,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_6,
    MAP_HEADER_UNKNOWN_533,
    MAP_HEADER_UNKNOWN_534,
    MAP_HEADER_UNKNOWN_535,
    MAP_HEADER_UNKNOWN_536,
    MAP_HEADER_UNKNOWN_537,
    MAP_HEADER_UNKNOWN_538,
    MAP_HEADER_UNKNOWN_539,
    MAP_HEADER_UNKNOWN_540,
    MAP_HEADER_UNKNOWN_541,
    MAP_HEADER_UNKNOWN_542,
    MAP_HEADER_UNKNOWN_543,
    MAP_HEADER_UNKNOWN_544,
    MAP_HEADER_UNKNOWN_545,
    MAP_HEADER_UNKNOWN_546,
    MAP_HEADER_UNKNOWN_547,
    MAP_HEADER_UNKNOWN_548,
    MAP_HEADER_UNKNOWN_549,
    MAP_HEADER_UNKNOWN_550,
    MAP_HEADER_UNKNOWN_551,
    MAP_HEADER_UNKNOWN_552,
    MAP_HEADER_UNKNOWN_553,
    MAP_HEADER_UNKNOWN_554,
    MAP_HEADER_UNKNOWN_555,
    MAP_HEADER_UNKNOWN_556,
    MAP_HEADER_UNKNOWN_557,
    MAP_HEADER_CONTEST_HALL_STAGE_NO_CONTEST,
    MAP_HEADER_BATTLE_FRONTIER,
    MAP_HEADER_BATTLE_FRONTIER_GATE_TO_FIGHT_AREA,
    MAP_HEADER_UNKNOWN_561,
    MAP_HEADER_BATTLE_FACTORY,
    MAP_HEADER_BATTLE_HALL,
    MAP_HEADER_BATTLE_CASTLE,
    MAP_HEADER_BATTLE_ARCADE,
    MAP_HEADER_VEILSTONE_STORE_B1F,
    MAP_HEADER_GLOBAL_TERMINAL_2F,
    MAP_HEADER_GLOBAL_TERMINAL_3F,
    MAP_HEADER_GALACTIC_HQ_HALL,
    MAP_HEADER_UNKNOWN_570,
    MAP_HEADER_ROTOMS_ROOM,
    MAP_HEADER_UNKNOWN_572,
    MAP_HEADER_DISTORTION_WORLD_1F,
    MAP_HEADER_DISTORTION_WORLD_B1F,
    MAP_HEADER_DISTORTION_WORLD_B2F,
    MAP_HEADER_DISTORTION_WORLD_B3F,
    MAP_HEADER_DISTORTION_WORLD_B4F,
    MAP_HEADER_UNKNOWN_578,
    MAP_HEADER_DISTORTION_WORLD_B5F,
    MAP_HEADER_DISTORTION_WORLD_B6F,
    MAP_HEADER_DISTORTION_WORLD_B7F,
    MAP_HEADER_DISTORTION_WORLD_GIRATINA_ROOM,
    MAP_HEADER_DISTORTION_WORLD_TURNBACK_CAVE_ROOM,
    MAP_HEADER_SPEAR_PILLAR_DIALGA,
    MAP_HEADER_SPEAR_PILLAR_PALKIA,
    MAP_HEADER_WIFI_PLAZA_ENTRANCE,
    MAP_HEADER_IRON_ISLAND_IRON_RUINS,
    MAP_HEADER_IRON_RUINS,
    MAP_HEADER_MT_CORONET_ICEBERG_RUINS,
    MAP_HEADER_ICEBERG_RUINS,
    MAP_HEADER_ROUTE_228_ROCK_PEAK_RUINS,
    MAP_HEADER_ROCK_PEAK_RUINS,

    MAP_HEADER_INVALID,
    MAP_HEADER_DYNAMIC = 4095,
};


void Heap_Free(void *ptr);

typedef struct JournalEntry JournalEntry;

static inline void Location_Set(Location *location, enum MapHeaderID mapHeaderID, int warpId, int x, int z, int faceDirection) {
    location->mapId = mapHeaderID;
    location->warpId = warpId;
    location->x = x;
    location->y = z;
    location->direction = faceDirection;
}

typedef struct BattleTower BattleTower;

typedef struct MapLoadMode MapLoadMode;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_02095E80_sub1;

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef struct ColoredArrow ColoredArrow;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct Menu Menu;

typedef struct TradeRoomMonDisplayData {
    u16 species;
    u16 heldItemType;
    u8 pokeBall;
    u8 isEgg;
    u8 form;
    u16 gender;
    u16 iconShouldFlip;
    int ballCapsuleId;
} TradeRoomMonDisplayData;

typedef struct TradeRoomArgs TradeRoomArgs;

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

typedef struct FogManager FogManager;

typedef struct UnkStruct_ov5_021EB0E0 UnkStruct_ov5_021EB0E0;

typedef struct UnkStruct_ov56_02256468_t UnkStruct_ov56_02256468;

typedef struct {
    s64 unk_00;
} UnkStruct_ov66_0222DCE0_sub1;

typedef struct UndergroundTopScreenContext_t UndergroundTopScreenContext;

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

typedef struct FieldTask FieldTask;

typedef BOOL (*FieldTaskFunc)(FieldTask *);

struct FieldTask {
    FieldTask *prev;
    FieldTaskFunc func;
    int state;
    void *env;
    int dummy10;
    void *dummy14;
    FieldSystem *fieldSys;
    int *dummy1C;
};

typedef struct MapHeaderData MapHeaderData;

typedef struct PokedexMemory PokedexMemory;

typedef struct RadarChain RadarChain;

enum UnionRoomConnectState {
    UR_CONNECT_STATE_NONE = 0,
    UR_CONNECT_STATE_CONNECTED,
    UR_CONNECT_STATE_FAILED,
    UR_CONNECT_STATE_PEER_CONTACTED,
};

enum UnionRoomPendingConnection {
    UR_PENDING_CONNECTION_NONE = 0,
    UR_PENDING_CONNECTION_UNION,
    UR_PENDING_CONNECTION_DRAW,
};

typedef void (*UnionRoomTask)(UnionRoomManager *);

typedef struct HoneyTreeShakeList HoneyTreeShakeList;

typedef struct FieldWildBattleMetadata {
    u16 encounterAttempts;
    u16 wildMonDefeated;
} FieldWildBattleMetadata;




typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_0202F298_sub1;

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

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

BOOL sub_02087BE8(UnkStruct_0208BC3C *param0, enum HeapID heapID);

BOOL sub_02087E34(int param0);

BOOL sub_02087E34(int param0) {
    int v0;
    const int v1[] = {
        97,
        99,
        100,
        101,
        102,
    };

    for (v0 = 0; v0 < NELEMS(v1); v0++) {
        if (param0 == v1[v0]) {
            return 1;
        }
    }

    return 0;
}
