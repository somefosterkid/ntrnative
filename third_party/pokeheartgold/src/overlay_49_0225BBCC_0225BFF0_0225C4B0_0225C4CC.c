#include "global.h"

#include "constants/heap.h"
#include "sprite_resources_header.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"

struct Background;

struct BgConfig;

struct ColoredArrow;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct ListMenu;

struct ListMenuTemplate;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct Resource;

struct ResourceCollection;

struct Sprite;

struct SpriteList;

struct SpriteListTemplate;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct StringList;

struct StringTemplate;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct TouchScreenActions;

struct TouchScreenHitTableActionStateInternal;

struct PlayerProfile;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_02260AD4_t;

struct UnkStruct_ov70_02261E10_t;

struct UnkStruct_ov70_022630A4_t;

struct UnkStruct_ov70_02269204_t;

struct UnkStruct_ov70_0226C6F8_t;

struct WiFiPlazaProfile;

struct Window;

typedef struct UnkStruct_ov66_0222DFF8_t WiFiPlazaSystem;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;

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

typedef struct ColoredArrow {
    TextColor color;
    String *string;
} ColoredArrow;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;


typedef struct StringList {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
} StringList;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct {
    u32 questionNumber;
    u32 selectedAnswer;
} UnkStruct_ov66_0222E908;

typedef struct {
    u8 pad_0[4];
    u8 unk_4;
    u8 unk_5;
    u16 unk_6;
    u32 unk_8;
} UnkStruct_ov66_02231428;

typedef struct WiFiPlazaProfile {
    s32 unk_00;
    u32 trainerID;
    u16 trainerName[7 + 1];
    UnkStruct_ov66_02231428 unk_18;
    UnkStruct_ov66_02231428 unk_1C;
    u16 partySpecies[6];
    u8 partyForms[6];
    u8 partyIsEgg[6];
    u8 trainerGender;
    u8 language;
    u16 trainerAppearanceIdx;
    u16 country;
    u8 region;
    u8 isNationalDexObtained;
    u8 isMainStoryCleared;
    u8 unk_41;
    u8 gameVersion;
    u8 unk_43;
    s64 startTimestamp;
    u8 unk_4C[12];
    s32 unk_58[12];
    u16 unk_88[2];
    UnkStruct_ov66_0222E908 unk_8C;
} WiFiPlazaProfile;

int ov66_0222E728(const WiFiPlazaProfile *profile, u32 param1);

s32 ov45_0222A964(const WiFiPlazaProfile *profile, u32 param1);

s32 ov45_0222A99C(const WiFiPlazaProfile *profile);

u32 WiFiPlazaProfile_GetGender(const WiFiPlazaProfile *profile);

typedef struct UnkStruct_ov70_02269204_t UnkStruct_ov70_02269204;

typedef struct UnkStruct_ov70_0226C6F8_t UnkStruct_ov70_0226C6F8;

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

void Sprite_SetPalOffsetRespectVramOffset(Sprite *sprite, u8 paletteOffset);

typedef struct Resource {
    int id;
    void *data;
} Resource;

typedef struct ResourceCollection {
    Resource *resources;
    int capacity;
    int count;
} ResourceCollection;

enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};

typedef struct SpriteResource {
    Resource *rawResource;
    enum SpriteResourceType type;
    void *data;
} SpriteResource;

typedef struct SpriteResourceCollection {
    ResourceCollection *collection;
    SpriteResource *resources;
    void *capacity;
    void *count;
    enum SpriteResourceType type;
} SpriteResourceCollection;

SpriteResource *AddCharResObjFromOpenNarc(SpriteResourceCollection *spriteResources, NARC *narc, int memberIdx, BOOL compressed, int id, NNS_G2D_VRAM_TYPE vramType, enum HeapID heapID);

SpriteResource *AddPlttResObjFromOpenNarc(SpriteResourceCollection *spriteResources, NARC *narc, int memberIdx, BOOL compressed, int id, NNS_G2D_VRAM_TYPE vramType, int paletteIdx, enum HeapID heapID);

SpriteResource *AddCellOrAnimResObjFromOpenNarc(SpriteResourceCollection *spriteResources, NARC *narc, int memberIdx, BOOL compressed, int id, enum SpriteResourceType type, enum HeapID heapID);

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

typedef struct StringTemplate {
    u32 maxArgs;
    enum HeapID heapID;
    StringTemplateArg *args;
    String *templateBuf;
} StringTemplate;

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
} TouchScreenRect, TouchScreenHitTable;

typedef void (*TouchScreenHitTableAction)(u32, enum TouchScreenButtonState, void *);

typedef struct TouchScreenHitTableActionStateInternal TouchScreenHitTableActionStateInternal;

typedef struct TouchScreenActions {
    const TouchScreenHitTable *hitTables;
    u32 hitTablesLength;
    TouchScreenHitTableAction action;
    void *hitTableContext;
    TouchScreenHitTableActionStateInternal *actionStates;
} TouchScreenActions;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
    s16 unk_04;
    s16 unk_06;
} UnkStruct_ov70_0226D844;

const UnkStruct_ov70_0226D844 *ov49_0225C828(int param0, s32 param1, s32 param2, u32 param3);

extern const VecFx32 ov49_0226991C[12];

void ov70_0225E27C(UnkStruct_ov70_0225DEE8 *param0, enum PlazaMinigame param1, u32 param2);

void ov70_0225F2D8(UnkStruct_ov70_0225F208 *param0, enum PlazaMinigame param1, u32 param2);

void ov49_0225CB50(u32 param0, enum TouchScreenButtonState param1, void *param2);

void ov49_0225C4CC(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E4EC *param1, NARC *param2, u32 heapID, const WiFiPlazaProfile *param4, const WiFiPlazaProfile *param5);

u32 ov45_0222A9CC(const WiFiPlazaProfile *profile);

int ov45_0222A92C(const WiFiPlazaProfile *profile, u32 param1);

void ov49_0225C4CC(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E4EC *param1, NARC *param2, u32 heapID, const WiFiPlazaProfile *param4, const WiFiPlazaProfile *param5) {
    int v0;
    NARC *v1;
    NARC *v2;
    BOOL v3;
    u32 v4;
    u32 v5;
    u32 v6;
    u32 v7;
    SpriteListTemplate v8;
    SpriteResourcesHeader v9;
    const UnkStruct_ov70_0226D844 *v10;
    extern const u16 ov49_02269774[4][3];
    extern const u16 ov49_02269704[4];
    extern const u16 ov49_022696FC[4];

    v1 = NARC_New(NARC_a_2_1_3, (enum HeapID)(heapID));
    v6 = ov45_0222A99C(param5);
    v7 = ov45_0222A9CC(param5);

    for (v0 = 0; v0 < 4; v0++) {
        if (v0 == 2) {
            if (v7 == 1) {
                param0->unk_AC[v0] = 0L;
                param0->unk_EC[v0] = 0L;
                param0->unk_FC[v0] = 0L;
                continue;
            }
        } else if (v0 == 3) {
            if (v7 == 0) {
                param0->unk_AC[v0] = 0L;
                param0->unk_EC[v0] = 0L;
                param0->unk_FC[v0] = 0L;
                continue;
            }
        }

        if (ov49_02269774[v0][0] == 215) {
            v2 = v1;
        } else {
            v2 = param2;
        }

        param0->unk_AC[v0] = AddPlttResObjFromOpenNarc(param1->unk_194[1], (struct NARC *)(v2), ov49_02269774[v0][1], 0, 150 + v0, NNS_G2D_VRAM_TYPE_2DSUB, ov49_02269774[v0][2], (enum HeapID)(heapID));
        v3 = SpriteTransfer_CreatePlttTransferTask(param0->unk_AC[v0]);
        ((v3) ? (void)0 : GF_AssertFail());

        sub_0200A740(param0->unk_AC[v0]);

        param0->unk_EC[v0] = AddCellOrAnimResObjFromOpenNarc(param1->unk_194[2], (struct NARC *)(v2), ov49_02269704[v0], 0, 150 + v0, (enum SpriteResourceType)(2), (enum HeapID)(heapID));
        param0->unk_FC[v0] = AddCellOrAnimResObjFromOpenNarc(param1->unk_194[3], (struct NARC *)(v2), ov49_022696FC[v0], 0, 150 + v0, (enum SpriteResourceType)(3), (enum HeapID)(heapID));
    }

    for (v0 = 0; v0 < 12; v0++) {
        v4 = ov45_0222A92C(param4, v0);
        v5 = ov45_0222A964(param4, v0);

        if (v4 == 24) {
            param0->unk_BC[v0] = 0L;
            param0->unk_7C[v0] = 0L;
        } else {
            v10 = ov49_0225C828(v4, v5, v6, v7);

            if (v10->unk_00 == 1) {
                v2 = param2;
            } else {
                v2 = v1;
            }

            param0->unk_BC[v0] = AddCharResObjFromOpenNarc(param1->unk_194[0], (struct NARC *)(v2), v10->unk_02, 0, 150 + v0, NNS_G2D_VRAM_TYPE_2DSUB, (enum HeapID)(heapID));

            v3 = SpriteTransfer_CreateCharTransferTask_AllocAtEnd(param0->unk_BC[v0]);
            ((v3) ? (void)0 : GF_AssertFail());

            sub_0200A740(param0->unk_BC[v0]);
            CreateSpriteResourcesHeader(&v9, 150 + v0, 150 + v10->unk_00, 150 + v10->unk_00, 150 + v10->unk_00, 0xffffffff, 0xffffffff, 0, 0, param1->unk_194[0], param1->unk_194[1], param1->unk_194[2], param1->unk_194[3], 0L, 0L);

            v8.list = param1->unk_04;
            v8.resourceData = &v9;
            v8.priority = 0;
            v8.vramType = NNS_G2D_VRAM_TYPE_2DSUB;
            v8.heapID = (enum HeapID)(heapID);
            v8.position = ov49_0226991C[v0];
            v8.position.x += ((fx32)(((v10->unk_04) > 0) ? ((v10->unk_04) * (1 << 12) + 0.5f) : ((v10->unk_04) * (1 << 12) - 0.5f)));
            v8.position.y += ((fx32)(((v10->unk_06) > 0) ? ((v10->unk_06) * (1 << 12) + 0.5f) : ((v10->unk_06) * (1 << 12) - 0.5f)));

            param0->unk_7C[v0] = Sprite_Create(&v8);

            Sprite_SetPalOffsetRespectVramOffset(param0->unk_7C[v0], v10->unk_01);
        }
    }

    NARC_Delete(v1);
}
