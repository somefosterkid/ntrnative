#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
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

void Sprite_SetAnimCtrlSeq(Sprite *sprite, u32 animID);

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

void ov70_0225E27C(UnkStruct_ov70_0225DEE8 *param0, enum PlazaMinigame param1, u32 param2);

void ov70_0225F2D8(UnkStruct_ov70_0225F208 *param0, enum PlazaMinigame param1, u32 param2);

void ov49_0225CB50(u32 param0, enum TouchScreenButtonState param1, void *param2);

void ov49_0225BFF0(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E4EC *param1, NARC *param2, u32 heapID, u32 param4, BOOL param5);

void ov49_0225BFF0(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E4EC *param1, NARC *param2, u32 heapID, u32 param4, BOOL param5) {
    u32 v0;
    u32 v1;

    ((param4 < 27) ? (void)0 : GF_AssertFail());
    ((param0->unk_54 == 0L) ? (void)0 : GF_AssertFail());

    v0 = param4 / 3;
    v1 = param4 % 3;

    {
        BOOL v2;

        param0->unk_58[0] = AddCharResObjFromOpenNarc((struct SpriteResourceCollection *)((struct NARC *)(param1->unk_194[0])), (struct NARC *)(param2), 61 + (v0 * 3), 0, 100, NNS_G2D_VRAM_TYPE_2DSUB, (enum HeapID)(heapID));

        v2 = SpriteTransfer_CreateCharTransferTask_AllocAtEnd(param0->unk_58[0]);
        ((v2) ? (void)0 : GF_AssertFail());
        sub_0200A740(param0->unk_58[0]);

        param0->unk_58[1] = AddPlttResObjFromOpenNarc((struct SpriteResourceCollection *)((struct NARC *)(param1->unk_194[1])), (struct NARC *)(param2), 89, 0, 100, NNS_G2D_VRAM_TYPE_2DSUB, 3, (enum HeapID)(heapID));

        v2 = SpriteTransfer_CreatePlttTransferTask(param0->unk_58[1]);
        ((v2) ? (void)0 : GF_AssertFail());
        sub_0200A740(param0->unk_58[1]);

        param0->unk_58[2] = AddCellOrAnimResObjFromOpenNarc((struct SpriteResourceCollection *)((struct NARC *)(param1->unk_194[2])), (struct NARC *)(param2), 60 + (v0 * 3), 0, 100, (GfGfxResType)(2), (enum HeapID)(heapID));
        param0->unk_58[3] = AddCellOrAnimResObjFromOpenNarc((struct SpriteResourceCollection *)((struct NARC *)(param1->unk_194[3])), (struct NARC *)(param2), 59 + (v0 * 3), 0, 100, (GfGfxResType)(3), (enum HeapID)(heapID));
    }

    {
        SpriteResourcesHeader v3;
        SpriteListTemplate v4 = { 0L };

        CreateSpriteResourcesHeader(&v3, 100, 100, 100, 100, 0xffffffff, 0xffffffff, 0, 0, param1->unk_194[0], param1->unk_194[1], param1->unk_194[2], param1->unk_194[3], 0L, 0L);

        v4.list = param1->unk_04;
        v4.resourceData = &v3;
        v4.priority = 16;
        v4.vramType = NNS_G2D_VRAM_TYPE_2DSUB;
        v4.heapID = (enum HeapID)(heapID);
        v4.position.x = ((fx32)(((208) > 0) ? ((208) * (1 << 12) + 0.5f) : ((208) * (1 << 12) - 0.5f)));
        v4.position.y = (((fx32)(((152) > 0) ? ((152) * (1 << 12) + 0.5f) : ((152) * (1 << 12) - 0.5f))) + (256 * ((fx32)0x0000000000001000L)));
        param0->unk_54 = Sprite_Create(&v4);

        Sprite_SetAnimCtrlSeq(param0->unk_54, v1);
        Sprite_SetDrawFlag(param0->unk_54, param5);
    }
}
