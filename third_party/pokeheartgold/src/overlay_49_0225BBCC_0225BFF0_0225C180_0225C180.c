#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "pm_string.h"
#include "filesystem.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "list_menu.h"
#include "gf_3d_loader.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_49_unk_struct_ov70_0225f208.h"
#include "overlay_49_unk_struct_ov70_0225f350.h"

struct ListMenuCursor;

struct G3DPipelineBuffers;

struct MessageBank;

struct MessageBankEntry;

struct NARC;

struct GF_2DGfxRawResObj;

struct Sprite;

struct SpriteList;

struct SpriteListTemplate;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct TouchHitboxController;

struct TouchScreenHitTableActionStateInternal;

struct TrainerClassGraphicIndex;

struct UnkStruct_ov70_0225DEE8_t;

struct Window;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};

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

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

enum TrainerClass {
    TRAINER_CLASS_PLAYER_MALE = 0,
    TRAINER_CLASS_PLAYER_FEMALE = 1,
    TRAINER_CLASS_YOUNGSTER = 2,
    TRAINER_CLASS_LASS = 3,
    TRAINER_CLASS_CAMPER = 4,
    TRAINER_CLASS_PICNICKER = 5,
    TRAINER_CLASS_BUG_CATCHER = 6,
    TRAINER_CLASS_AROMA_LADY = 7,
    TRAINER_CLASS_TWINS = 8,
    TRAINER_CLASS_HIKER = 9,
    TRAINER_CLASS_BATTLE_GIRL = 10,
    TRAINER_CLASS_FISHERMAN = 11,
    TRAINER_CLASS_CYCLIST_MALE = 12,
    TRAINER_CLASS_CYCLIST_FEMALE = 13,
    TRAINER_CLASS_BLACK_BELT = 14,
    TRAINER_CLASS_ARTIST = 15,
    TRAINER_CLASS_BREEDER_MALE = 16,
    TRAINER_CLASS_BREEDER_FEMALE = 17,
    TRAINER_CLASS_COWGIRL = 18,
    TRAINER_CLASS_JOGGER = 19,
    TRAINER_CLASS_POKEFAN_MALE = 20,
    TRAINER_CLASS_POKEFAN_FEMALE = 21,
    TRAINER_CLASS_POKE_KID = 22,
    TRAINER_CLASS_YOUNG_COUPLE = 23,
    TRAINER_CLASS_ACE_TRAINER_MALE = 24,
    TRAINER_CLASS_ACE_TRAINER_FEMALE = 25,
    TRAINER_CLASS_WAITRESS = 26,
    TRAINER_CLASS_VETERAN = 27,
    TRAINER_CLASS_NINJA_BOY = 28,
    TRAINER_CLASS_DRAGON_TAMER = 29,
    TRAINER_CLASS_BIRD_KEEPER = 30,
    TRAINER_CLASS_DOUBLE_TEAM = 31,
    TRAINER_CLASS_RICH_BOY = 32,
    TRAINER_CLASS_LADY = 33,
    TRAINER_CLASS_GENTLEMAN = 34,
    TRAINER_CLASS_SOCIALITE = 35,
    TRAINER_CLASS_BEAUTY = 36,
    TRAINER_CLASS_COLLECTOR = 37,
    TRAINER_CLASS_POLICEMAN = 38,
    TRAINER_CLASS_RANGER_MALE = 39,
    TRAINER_CLASS_RANGER_FEMALE = 40,
    TRAINER_CLASS_SCIENTIST = 41,
    TRAINER_CLASS_SWIMMER_MALE = 42,
    TRAINER_CLASS_SWIMMER_FEMALE = 43,
    TRAINER_CLASS_TUBER_MALE = 44,
    TRAINER_CLASS_TUBER_FEMALE = 45,
    TRAINER_CLASS_SAILOR = 46,
    TRAINER_CLASS_SIS_AND_BRO = 47,
    TRAINER_CLASS_RUIN_MANIAC = 48,
    TRAINER_CLASS_PSYCHIC_MALE = 49,
    TRAINER_CLASS_PSYCHIC_FEMALE = 50,
    TRAINER_CLASS_PI = 51,
    TRAINER_CLASS_GUITARIST = 52,
    TRAINER_CLASS_ACE_TRAINER_SNOW_MALE = 53,
    TRAINER_CLASS_ACE_TRAINER_SNOW_FEMALE = 54,
    TRAINER_CLASS_SKIER_MALE = 55,
    TRAINER_CLASS_SKIER_FEMALE = 56,
    TRAINER_CLASS_ROUGHNECK = 57,
    TRAINER_CLASS_CLOWN = 58,
    TRAINER_CLASS_WORKER = 59,
    TRAINER_CLASS_SCHOOL_KID_MALE = 60,
    TRAINER_CLASS_SCHOOL_KID_FEMALE = 61,
    TRAINER_CLASS_LEADER_ROARK = 62,
    TRAINER_CLASS_RIVAL = 63,
    TRAINER_CLASS_LEADER_BYRON = 64,
    TRAINER_CLASS_ELITE_FOUR_AARON = 65,
    TRAINER_CLASS_ELITE_FOUR_BERTHA = 66,
    TRAINER_CLASS_ELITE_FOUR_FLINT = 67,
    TRAINER_CLASS_ELITE_FOUR_LUCIAN = 68,
    TRAINER_CLASS_CHAMPION_CYNTHIA = 69,
    TRAINER_CLASS_BELLE_AND_PA = 70,
    TRAINER_CLASS_RANCHER = 71,
    TRAINER_CLASS_COMMANDER_MARS = 72,
    TRAINER_CLASS_GALACTIC_GRUNT_MALE = 73,
    TRAINER_CLASS_LEADER_GARDENIA = 74,
    TRAINER_CLASS_LEADER_WAKE = 75,
    TRAINER_CLASS_LEADER_MAYLENE = 76,
    TRAINER_CLASS_LEADER_FANTINA = 77,
    TRAINER_CLASS_LEADER_CANDICE = 78,
    TRAINER_CLASS_LEADER_VOLKNER = 79,
    TRAINER_CLASS_PARASOL_LADY = 80,
    TRAINER_CLASS_WAITER = 81,
    TRAINER_CLASS_INTERVIEWERS = 82,
    TRAINER_CLASS_CAMERAMAN = 83,
    TRAINER_CLASS_REPORTER = 84,
    TRAINER_CLASS_IDOL = 85,
    TRAINER_CLASS_GALACTIC_BOSS = 86,
    TRAINER_CLASS_COMMANDER_JUPITER = 87,
    TRAINER_CLASS_COMMANDER_SATURN = 88,
    TRAINER_CLASS_GALACTIC_GRUNT_FEMALE = 89,
    TRAINER_CLASS_TRAINER_CHERYL = 90,
    TRAINER_CLASS_TRAINER_RILEY = 91,
    TRAINER_CLASS_TRAINER_MARLEY = 92,
    TRAINER_CLASS_TRAINER_BUCK = 93,
    TRAINER_CLASS_TRAINER_MIRA = 94,
    TRAINER_CLASS_DP_PLAYER_MALE = 95,
    TRAINER_CLASS_DP_PLAYER_FEMALE = 96,
    TRAINER_CLASS_TOWER_TYCOON = 97,
    TRAINER_CLASS_MAID = 98,
    TRAINER_CLASS_HALL_MATRON = 99,
    TRAINER_CLASS_FACTORY_HEAD = 100,
    TRAINER_CLASS_ARCADE_STAR = 101,
    TRAINER_CLASS_CASTLE_VALET = 102,
    TRAINER_CLASS_DP_PLAYER_MALE_2 = 103,
    TRAINER_CLASS_DP_PLAYER_FEMALE_2 = 104,
};

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};

typedef struct SpriteListTemplate {
    SpriteList *list;
    const SpriteResourcesHeader *resourceData;
    VecFx32 position;
    u32 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    enum HeapID heapID;
} SpriteListTemplate;

Sprite *Sprite_Create(const SpriteListTemplate *template);

typedef struct SpriteResource {
    GF_2DGfxRawResObj *rawResource;
    GfGfxResType type;
    void *data;
} SpriteResource;

SpriteResource *AddCharResObjFromNarc(SpriteResourceCollection *spriteResources, enum NarcId narcID, int memberIdx, BOOL compressed, int id, NNS_G2D_VRAM_TYPE vramType, enum HeapID heapID);

SpriteResource *AddPlttResObjFromNarc(SpriteResourceCollection *spriteResources, enum NarcId narcID, int memberIdx, BOOL compressed, int id, NNS_G2D_VRAM_TYPE vramType, int paletteIdx, enum HeapID heapID);

SpriteResource *AddCellOrAnimResObjFromNarc(SpriteResourceCollection *spriteResources, enum NarcId narcID, int memberIdx, BOOL compressed, int id, GfGfxResType type, enum HeapID heapID);

void sub_0200A740(SpriteResource *spriteRes);

NNSG2dPaletteData *GF2DGfxResObj_GetPlttDataPtr(const SpriteResource *spriteRes);

void CreateSpriteResourcesHeader(SpriteResourcesHeader *resourceHeader,
    int charResourceID,
    int plttResourceID,
    int cellResourceID,
    int animResourceID,
    int mcellResourceID,
    int manimResourceID,
    BOOL vramTransfer,
    int priority,
    SpriteResourceCollection *charResources,
    SpriteResourceCollection *plttResources,
    SpriteResourceCollection *cellResources,
    SpriteResourceCollection *animResources,
    SpriteResourceCollection *mcellResources,
    SpriteResourceCollection *manimResources);

typedef struct TrainerClassGraphicIndex {
    enum NarcId narcID;
    int tiles;
    int palette;
    int cells;
    int anims;
    int scan;
} TrainerClassGraphicIndex;

void sub_02070D84(enum TrainerClass trainerClass, int face, TrainerClassGraphicIndex *trainerClassGraphicIndex);

BOOL SpriteTransfer_CreateCharTransferTask_AllocAtEnd(const SpriteResource *resource);

BOOL SpriteTransfer_CreatePlttTransferTask(const SpriteResource *resource);

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
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
} UnkStruct_ov70_0226D724;

void ov49_0225C368(NNSG2dPaletteData *param0);

extern const UnkStruct_ov70_0226D724 ov49_0226988C[18];

void ov70_0225E27C(UnkStruct_ov70_0225DEE8 *param0, enum PlazaMinigame param1, u32 param2);

void ov70_0225F2D8(UnkStruct_ov70_0225F208 *param0, enum PlazaMinigame param1, u32 param2);

void ov49_0225CB50(u32 param0, enum TouchScreenButtonState param1, void *param2);

void ov49_0225C180(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E4EC *param1, NARC *param2, u32 heapID, u32 param4);

void ov49_0225C180(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E4EC *param1, NARC *param2, u32 heapID, u32 param4) {
    enum TrainerClass trainerClass;
    u16 v1, v2;
    u32 v3;
    TrainerClassGraphicIndex v4;

    ((param0->unk_68 == 0L) ? (void)0 : GF_AssertFail());

    for (v3 = 0; v3 < 18; v3++) {
        if (ov49_0226988C[v3].unk_00 == param4) {
            trainerClass = (enum TrainerClass)(ov49_0226988C[v3].unk_02);
            v1 = ov49_0226988C[v3].unk_04;
            v2 = ov49_0226988C[v3].unk_06;
        }
    }

    sub_02070D84(trainerClass, 2, &v4);

    {
        BOOL v5;

        param0->unk_6C[0] = AddCharResObjFromNarc(param1->unk_194[0], v4.narcID, v4.tiles, 0, 101, NNS_G2D_VRAM_TYPE_2DSUB, (enum HeapID)(heapID));
        v5 = SpriteTransfer_CreateCharTransferTask_AllocAtEnd(param0->unk_6C[0]);
        ((v5) ? (void)0 : GF_AssertFail());

        param0->unk_6C[1] = AddPlttResObjFromNarc(param1->unk_194[1], v4.narcID, v4.palette, 0, 101, NNS_G2D_VRAM_TYPE_2DSUB, 1, (enum HeapID)(heapID));
        ov49_0225C368(GF2DGfxResObj_GetPlttDataPtr(param0->unk_6C[1]));
        v5 = SpriteTransfer_CreatePlttTransferTask(param0->unk_6C[1]);
        ((v5) ? (void)0 : GF_AssertFail());

        sub_0200A740(param0->unk_6C[1]);

        param0->unk_6C[2] = AddCellOrAnimResObjFromNarc(param1->unk_194[2], v4.narcID, v4.cells, 0, 101, (GfGfxResType)(2), (enum HeapID)(heapID));
        param0->unk_6C[3] = AddCellOrAnimResObjFromNarc(param1->unk_194[3], v4.narcID, v4.anims, 0, 101, (GfGfxResType)(3), (enum HeapID)(heapID));
    }

    {
        SpriteResourcesHeader v6;
        SpriteListTemplate v7 = { 0L };

        CreateSpriteResourcesHeader(&v6, 101, 101, 101, 101, 0xffffffff, 0xffffffff, 1, 3, param1->unk_194[0], param1->unk_194[1], param1->unk_194[2], param1->unk_194[3], 0L, 0L);

        v7.list = param1->unk_04;
        v7.resourceData = &v6;
        v7.priority = 32;
        v7.vramType = NNS_G2D_VRAM_TYPE_2DSUB;
        v7.heapID = (enum HeapID)(heapID);
        v7.position.x = ((fx32)(((v1) > 0) ? ((v1) * (1 << 12) + 0.5f) : ((v1) * (1 << 12) - 0.5f)));
        v7.position.y = ((fx32)(((v2) > 0) ? ((v2) * (1 << 12) + 0.5f) : ((v2) * (1 << 12) - 0.5f))) + (256 * ((fx32)0x0000000000001000L));

        param0->unk_68 = Sprite_Create(&v7);
    }
}
