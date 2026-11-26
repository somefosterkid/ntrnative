#include "global.h"
#include "sprite_resources_header.h"

#include "error_handling.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "obj_char_transfer.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"

void ov41_0224A1DC(void *, u32);

void ov41_0224A1EC(u8 *, s32, u32);

void ov41_0224A8B0(u8 *param_1, s32 param_2);

void ov41_0224A8D4(u8 *, u32);

void ov41_0224A918(u8 *, u32, u32, u32);

void ov41_0224A60C(s32 param_1, u32 param_2, u8 *param_3);

struct FontOAM;
struct PokemonSpriteManager;
struct ResourceCollection;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct UnkStruct_02012744_t;
struct UnkStruct_ov22_02259C58_tag;
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
typedef struct AffineSpriteListTemplate {
    SpriteList *list;
    const SpriteResourcesHeader *resourceData;
    VecFx32 position;
    VecFx32 affineScale;
    u16 affineZRotation;
    u32 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    enum HeapID heapID;
} AffineSpriteListTemplate;
typedef struct UnkStruct_ov22_02259C58_tag UnkStruct_ov22_02259C58;
typedef void (*UnkFuncPtr_ov22_02259D78)(UnkStruct_ov22_02259C58 *param0, void *param1);
typedef struct {
    AffineSpriteListTemplate *unk_00;
    UnkFuncPtr_ov22_02259D78 unk_04;
    void *unk_08;
    int unk_0C;
} UnkStruct_ov22_02259C58_1;
typedef struct PokemonSpriteManager PokemonSpriteManager;
typedef struct ResourceCollection ResourceCollection;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef struct SpriteResourceCollection SpriteResourceCollection;
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
void ov41_0224A118(UnkStruct_ov22_02259C58 *param0, UnkStruct_ov22_02259C58_1 *param1);
struct UnkStruct_ov22_02259C58_tag {
    Sprite *unk_00;
    int unk_04;
    void *unk_08;
    void (*unk_0C)(struct UnkStruct_ov22_02259C58_tag *param0, void *param1);
};
typedef struct UnkStruct_02012744_t UnkStruct_02012744;
typedef struct {
    UnkStruct_ov22_02259C58_1 unk_00;
    const Window *unk_10;
    const UnkStruct_02012744 *unk_14;
    const NNSG2dImagePaletteProxy *unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
} UnkStruct_ov22_02259C9C;
typedef struct FontOAM FontOAM;
typedef struct {
    UnkStruct_ov22_02259C58 unk_00;
    FontOAM *unk_10;
    UnkStruct_02021AC8 unk_14;
} UnkStruct_ov22_0225A154;
typedef struct SpriteResource SpriteResource;
SpriteResource *SpriteResourceCollection_Find(const SpriteResourceCollection *spriteResources, int id);
void ov41_0224A15C(UnkStruct_ov22_0225A154 *param0, UnkStruct_ov22_02259C9C *param1);
const NNSG2dImagePaletteProxy *SpriteTransfer_GetPaletteProxy(const SpriteResource *resource, NNSG2dImageProxy *imageProxy);
int sub_02013688(const Window *param0, int param1, enum HeapID heapID);
void ov41_022462E4(UnkStruct_ov22_0225A0E4 *param0, NARC *param1, int param2, BOOL param3, int param4, int param5);
void ov41_02246304(UnkStruct_ov22_0225A0E4 *param0, NARC *param1, int param2, BOOL param3, int param4, int param5, int param6);
void ov41_02246328(UnkStruct_ov22_0225A0E4 *param0, NARC *param1, int param2, BOOL param3, int param4);
void ov41_02246344(UnkStruct_ov22_0225A0E4 *param0, NARC *param1, int param2, BOOL param3, int param4);
void ov41_0224A6C4(UnkStruct_ov22_02259C58 *param0, int param1, UnkStruct_ov22_0225A0E4 *param2, int param3, int param4, int param5, int param6);
void ov41_0224A734(UnkStruct_ov22_0225A154 *param0, int param1, UnkStruct_ov22_0225A0E4 *param2, const UnkStruct_02012744 *param3, const Window *param4, int param5, int param6, int param7, int param8);
void ov41_0224A7E0(TouchscreenHitbox *hitTable, int param1, int param2, int param3, int param4, int param5);
void ov41_0224A7F8(UnkStruct_ov22_0225A0E4 *param0);

s32 ov41_02246360(void *, s32);
s32 ov41_02246374(void *, s32);
void ov41_0224A888(void *arg0);

void ov41_0224A60C(s32 param_1, u32 param_2, u8 *param_3) {
    switch (param_1) {
    case 0:
        ov41_0224A8B0(param_3, param_2);
        ov41_0224A918(param_3, 0x680, 0, param_2);
        ov41_0224A1DC(param_3, param_2);
        return;
    case 1:
        ov41_0224A8B0(param_3 + 0x10, param_2);
        ov41_0224A918(param_3 + 0x10, 0x680, 0, param_2);
        ov41_0224A1DC(param_3 + 0x10, param_2);
        return;
    case 2:
    case 3:
        ov41_0224A1EC(param_3, param_1, param_2);
        ov41_0224A1DC(param_3 + param_1 * 0x10, param_2);
        return;
    case 4:
        if (*(s32 *)(param_3 + 0x60) == 1) {
            ov41_0224A8B0(param_3 + 0x40, param_2);
            ov41_0224A8D4(param_3 + 0x40, param_2);
            ov41_0224A918(param_3 + 0x40, 0x5e2, 0, param_2);
            ov41_0224A1DC(param_3 + 0x40, param_2);
            return;
        }
        break;
    default:
        GF_AssertFail();
    }
    return;
}

void ov41_0224A6C4(UnkStruct_ov22_02259C58 *param0, int param1, UnkStruct_ov22_0225A0E4 *param2, int param3, int param4, int param5, int param6) {
    UnkStruct_ov22_02259C58_1 v0;
    SpriteResourcesHeader v1;
    AffineSpriteListTemplate v2;

    CreateSpriteResourcesHeader(&v1, param1, 0, param1, param1, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0, param2->unk_48[0], param2->unk_48[1], param2->unk_48[2], param2->unk_48[3], 0L, 0L);

    v2.list = param2->unk_44;
    v2.resourceData = &v1;
    v2.position.x = param3 << 12;
    v2.position.y = param4 << 12;
    v2.position.z = 0;
    v2.priority = 2;
    v2.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
    v2.heapID = HEAP_ID_14;

    v0.unk_00 = &v2;
    v0.unk_04 = 0L;
    v0.unk_08 = 0L;
    v0.unk_0C = 1;

    ov41_0224A118(param0, &v0);
}

void ov41_0224A734(UnkStruct_ov22_0225A154 *param0, int param1, UnkStruct_ov22_0225A0E4 *param2, const UnkStruct_02012744 *param3, const Window *param4, int param5, int param6, int param7, int param8) {
    UnkStruct_ov22_02259C9C v0;
    SpriteResourcesHeader v1;
    AffineSpriteListTemplate v2;
    SpriteResource *v3;

    CreateSpriteResourcesHeader(&v1, param1, 0, param1, param1, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0, param2->unk_48[0], param2->unk_48[1], param2->unk_48[2], param2->unk_48[3], 0L, 0L);

    v2.list = param2->unk_44;
    v2.resourceData = &v1;
    v2.position.x = param5 << 12;
    v2.position.y = param6 << 12;
    v2.position.z = 0;
    v2.priority = 2;
    v2.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
    v2.heapID = HEAP_ID_14;

    v0.unk_00.unk_00 = &v2;
    v0.unk_00.unk_04 = 0L;
    v0.unk_00.unk_08 = 0L;
    v0.unk_00.unk_0C = 1;
    v0.unk_14 = param3;
    v0.unk_10 = param4;
    v0.unk_1C = 0;
    v0.unk_20 = 19;

    v3 = SpriteResourceCollection_Find(param2->unk_48[1], 1);
    v0.unk_18 = SpriteTransfer_GetPaletteProxy(v3, 0L);

    ((sub_02021AC8(sub_02013688(param4, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_13), 1, NNS_G2D_VRAM_TYPE_2DMAIN, &param0->unk_14)) ? (void)0 : GF_AssertFail());

    v0.unk_24 = param0->unk_14.offset;

    ov41_0224A15C(param0, &v0);
}

void ov41_0224A7E0(TouchscreenHitbox *hitTable, int param1, int param2, int param3, int param4, int param5) {
    hitTable[param1].rect.top = param3;
    hitTable[param1].rect.left = param2;
    hitTable[param1].rect.bottom = param3 + param5;
    hitTable[param1].rect.right = param2 + param4;
}

void ov41_0224A7F8(UnkStruct_ov22_0225A0E4 *param0) {
    int v0;

    for (v0 = 0; v0 < 5; v0++) {
        ov41_022462E4(param0, param0->unk_5C, (v0) * 3 + 105 + 2, 0, NNS_G2D_VRAM_TYPE_2DMAIN, v0);
        ov41_02246328(param0, param0->unk_5C, (v0) * 3 + 105 + 1, 0, v0);
        ov41_02246344(param0, param0->unk_5C, (v0) * 3 + 105 + 0, 0, v0);
    }

    ov41_02246304(param0, param0->unk_5C, 104, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 3, 0);
    ov41_02246304(param0, param0->unk_5C, 120, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 2, 1);
}

void ov41_0224A888(void *arg0) {
    s32 v0;

    v0 = 0;
    do {
        ov41_02246360(arg0, v0);
        v0 += 1;
    } while (v0 < 5);
    ov41_02246374(arg0, 0);
    ov41_02246374(arg0, 1);
}
