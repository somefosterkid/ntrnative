#include "global.h"

#include "constants/heap.h"
#include "sprite_resources_header.h"
#include "overlay_47_unk_struct_ov68_0225_d128.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "struct_defs/resdat_narc_structs.h"
#include "save.h"
#include "pm_string.h"
#include "filesystem.h"
#include "options.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "gf_3d_loader.h"
#include "overlay_41_sprite_resource_collection.h"
#include "g2d_renderer.h"

struct BgConfig;

struct ListMenuCursor;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct Options;

struct GF_2DGfxRawResObj;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct Sprite;

struct SpriteList;

struct SpriteListTemplate;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct UnkStruct_ov66_0222DFF8_t;

struct Window;

struct WindowTemplate;

typedef struct UnkStruct_ov66_0222DFF8_t WiFiPlazaSystem;

typedef struct {
    u32 questionNumber;
    u32 selectedAnswer;
} UnkStruct_ov66_0222E908;

typedef struct {
    u32 unk_00[3];
    u64 unk_0C;
} UnkStruct_ov68_0225DC74;

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
    SaveData *saveData;
    WiFiPlazaSystem *unk_04;
    int unk_08;
} UnkStruct_ov66_02231374;

void AddWindow(BgConfig *bgConfig, Window *window, const WindowTemplate *template);

u32 GfGfxLoader_LoadCharDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);

void GfGfxLoader_LoadScrnDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, enum HeapID heapID);

void GfGfxLoader_GXLoadPalFromOpenNarc(NARC *narc, u32 narcMemberIdx, enum GFPalLoadLocation loadLocation, u32 offset, u32 size, u32 heapID);

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

void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

typedef struct SpriteResource {
    GF_2DGfxRawResObj *rawResource;
    GfGfxResType type;
    void *data;
} SpriteResource;

SpriteResource *AddCharResObjFromOpenNarc(SpriteResourceCollection *spriteResources, NARC *narc, int memberIdx, BOOL compressed, int id, NNS_G2D_VRAM_TYPE vramType, enum HeapID heapID);

SpriteResource *AddPlttResObjFromOpenNarc(SpriteResourceCollection *spriteResources, NARC *narc, int memberIdx, BOOL compressed, int id, NNS_G2D_VRAM_TYPE vramType, int paletteIdx, enum HeapID heapID);

SpriteResource *AddCellOrAnimResObjFromOpenNarc(SpriteResourceCollection *spriteResources, NARC *narc, int memberIdx, BOOL compressed, int id, GfGfxResType type, enum HeapID heapID);

void sub_0200A740(SpriteResource *spriteRes);

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

typedef struct {
    BgConfig *unk_00;
    SpriteList *unk_04;
    G2dRenderer unk_08;
    SpriteResourceCollection *unk_194[4];
    NARC *unk_1A4;
} UnkStruct_ov68_0225C91C;

void ov47_02259C8C(UnkStruct_ov68_0225DB8C *param0, BOOL param1, enum HeapID heapID);

extern const WindowTemplate ov47_02259E40[1];

void ov68_0225C91C(UnkStruct_ov68_0225C91C *param0, SaveData *saveData, enum HeapID heapID);

void ov68_0225C9A0(UnkStruct_ov68_0225C91C *param0, Options *options, enum HeapID heapID);

void ov47_02258BB4(UnkStruct_ov68_0225C91C *param0, enum HeapID heapID);

void ov47_02258D78(UnkStruct_ov68_0225CB70 *param0, u32 param1, enum HeapID heapID);

BOOL ov68_0225CCB4(UnkUnion_ov68_0225CCA8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, enum HeapID heapID);

void ov47_02259228(UnkStruct_ov68_0225D128 *param0, UnkStruct_ov68_0225C91C *param1, SaveData *saveData, enum HeapID heapID);


void ov68_0225D388(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov66_02231374 *param3, enum HeapID heapID);


void ov68_0225D8F0(UnkStruct_ov68_0225D388 *param0, const UnkStruct_ov68_0225DB8C *param1, UnkStruct_ov68_0225CB70 *param2, UnkStruct_ov68_0225C91C *param3, const UnkStruct_ov66_0222E908 *param4, enum HeapID heapID, u32 param6, u32 param7, BOOL param8);

void ov47_02258DD0(UnkStruct_ov68_0225D0F8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, u32 param3);

void ov47_02258DD0(UnkStruct_ov68_0225D0F8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, u32 param3) {
    {
        GfGfxLoader_GXLoadPalFromOpenNarc(param2->unk_1A4, 199, (enum GFPalLoadLocation)(0), 0, 4 * 32, param3);
        GfGfxLoader_LoadCharDataFromOpenNarc(param2->unk_1A4, 200, param2->unk_00, 0, 0, 0, 0, param3);
        GfGfxLoader_LoadScrnDataFromOpenNarc(param2->unk_1A4, 201, param2->unk_00, 0, 0, 0, 0, (enum HeapID)(param3));
    }

    {
        BOOL v0;

        param0->unk_18[0] = AddCharResObjFromOpenNarc(param2->unk_194[0], param2->unk_1A4, 205, 0, 20, NNS_G2D_VRAM_TYPE_2DMAIN, (enum HeapID)(param3));
        param0->unk_18[1] = AddPlttResObjFromOpenNarc(param2->unk_194[1], param2->unk_1A4, 204, 0, 20, NNS_G2D_VRAM_TYPE_2DMAIN, 3, (enum HeapID)(param3));

        v0 = SpriteTransfer_CreateCharTransferTask_AllocAtEnd(param0->unk_18[0]);
        ((v0) ? (void)0 : GF_AssertFail());

        v0 = SpriteTransfer_CreatePlttTransferTask(param0->unk_18[1]);
        ((v0) ? (void)0 : GF_AssertFail());

        sub_0200A740(param0->unk_18[0]);
        sub_0200A740(param0->unk_18[1]);

        param0->unk_18[2] = AddCellOrAnimResObjFromOpenNarc(param2->unk_194[2], param2->unk_1A4, 206, 0, 20, (GfGfxResType)(2), (enum HeapID)(param3));
        param0->unk_18[3] = AddCellOrAnimResObjFromOpenNarc(param2->unk_194[3], param2->unk_1A4, 207, 0, 20, (GfGfxResType)(3), (enum HeapID)(param3));
    }

    {
        SpriteResourcesHeader v1;
        SpriteListTemplate v2 = { 0 };

        CreateSpriteResourcesHeader(&v1, 20, 20, 20, 20, 0xffffffff, 0xffffffff, 0, 0, param2->unk_194[0], param2->unk_194[1], param2->unk_194[2], param2->unk_194[3], 0L, 0L);

        v2.list = param2->unk_04;
        v2.resourceData = &v1;
        v2.priority = 0;
        v2.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
        v2.heapID = (enum HeapID)(param3);

        param0->unk_14 = Sprite_Create(&v2);
        Sprite_SetDrawFlag(param0->unk_14, 0);
    }

    {
        int v3;

        for (v3 = 0; v3 < 1; v3++) {
            AddWindow(param2->unk_00, &param0->unk_00[v3], &ov47_02259E40[v3]);
        }
    }

    ov47_02259C8C(&param0->unk_2C, 1, (enum HeapID)(param3));
}
