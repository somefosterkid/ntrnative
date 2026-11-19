#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
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

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

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

void Sprite_SetAnimActiveFlag(Sprite *sprite, BOOL animate);

void Sprite_SetAnimCtrlSeq(Sprite *sprite, u32 animID);

typedef struct SpriteResource {
    GF_2DGfxRawResObj *rawResource;
    GfGfxResType type;
    void *data;
} SpriteResource;

SpriteResource *AddCharResObjFromOpenNarc(SpriteResourceCollection *spriteResources, NARC *narc, int memberIdx, BOOL compressed, int id, NNS_G2D_VRAM_TYPE vramType, enum HeapID heapID);

SpriteResource *AddPlttResObjFromOpenNarc(SpriteResourceCollection *spriteResources, NARC *narc, int memberIdx, BOOL compressed, int id, NNS_G2D_VRAM_TYPE vramType, int paletteIdx, enum HeapID heapID);

SpriteResource *AddCellOrAnimResObjFromOpenNarc(SpriteResourceCollection *spriteResources, NARC *narc, int memberIdx, BOOL compressed, int id, GfGfxResType type, enum HeapID heapID);

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

extern const u8 ov49_022696E8[2];

void ov70_0225E27C(UnkStruct_ov70_0225DEE8 *param0, enum PlazaMinigame param1, u32 param2);

void ov70_0225F2D8(UnkStruct_ov70_0225F208 *param0, enum PlazaMinigame param1, u32 param2);

void ov49_0225CB50(u32 param0, enum TouchScreenButtonState param1, void *param2);

void ov49_0225AD20(UnkStruct_ov70_0225EC20 *param0, UnkStruct_ov70_0225E4EC *param1, u32 heapID);

void ov49_0225AD20(UnkStruct_ov70_0225EC20 *param0, UnkStruct_ov70_0225E4EC *param1, u32 heapID) {
    SpriteResourcesHeader v0;
    SpriteListTemplate v1;
    int v2;
    NARC *v3 = NARC_New((enum NarcId)60, (enum HeapID)(heapID));

    param0->unk_40[0] = AddCharResObjFromOpenNarc(param1->unk_194[0], (struct NARC *)(v3), 4, 0, 5000, NNS_G2D_VRAM_TYPE_2DMAIN, (enum HeapID)(heapID));
    param0->unk_40[1] = AddPlttResObjFromOpenNarc(param1->unk_194[1], (struct NARC *)(v3), 10, 0, 5000, NNS_G2D_VRAM_TYPE_2DMAIN, 1, (enum HeapID)(heapID));
    param0->unk_40[2] = AddCellOrAnimResObjFromOpenNarc(param1->unk_194[2], (struct NARC *)(v3), 5, 0, 5000, (GfGfxResType)(2), (enum HeapID)(heapID));
    param0->unk_40[3] = AddCellOrAnimResObjFromOpenNarc(param1->unk_194[3], (struct NARC *)(v3), 6, 0, 5000, (GfGfxResType)(3), (enum HeapID)(heapID));

    NARC_Delete(v3);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(param0->unk_40[0]);
    SpriteTransfer_CreatePlttTransferTask(param0->unk_40[1]);
    CreateSpriteResourcesHeader(&v0, 5000, 5000, 5000, 5000, 0xffffffff, 0xffffffff, 0, 0, param1->unk_194[0], param1->unk_194[1], param1->unk_194[2], param1->unk_194[3], 0L, 0L);

    v1.list = param1->unk_04;
    v1.resourceData = &v0;
    v1.priority = 0;
    v1.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
    v1.heapID = (enum HeapID)(heapID);
    v1.position.x = 192 * ((fx32)0x0000000000001000L);

    for (v2 = 0; v2 < 2; v2++) {
        v1.position.y = ov49_022696E8[v2] * ((fx32)0x0000000000001000L);
        param0->unk_50[v2] = Sprite_Create(&v1);

        Sprite_SetAnimCtrlSeq(param0->unk_50[v2], v2);
        Sprite_SetAnimActiveFlag(param0->unk_50[v2], 1);
        Sprite_SetDrawFlag(param0->unk_50[v2], 0);
    }

    param0->unk_3C = 0;
}
