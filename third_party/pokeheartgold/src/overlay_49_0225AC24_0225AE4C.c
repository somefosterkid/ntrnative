#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu.h"
#include "pm_string.h"
#include "touch_hitbox_controller.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"

struct G3DPipelineBuffers;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct StringList;

typedef struct StringList StringList;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

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

void Sprite_Delete(Sprite *gfxElem);

typedef struct SpriteResource SpriteResource;

typedef struct SpriteResourceCollection SpriteResourceCollection;

void DestroySingle2DGfxResObj(SpriteResourceCollection *spriteResources, SpriteResource *resource);

void SpriteTransfer_DeleteCharTransferTask(SpriteResource *resource);

void SpriteTransfer_DeletePlttTransferTask(SpriteResource *resource);

void ov49_0225AEE0(UnkStruct_ov70_0225EC20 *param0);

void ov49_0225B014(UnkStruct_ov70_0225EC20 *param0, u16 *param1, u16 *param2);

void ov49_0225AE4C(UnkStruct_ov70_0225EC20 *param0, UnkStruct_ov70_0225E4EC *param1);

void ov49_0225AE4C(UnkStruct_ov70_0225EC20 *param0, UnkStruct_ov70_0225E4EC *param1) {
    int v0;

    if (param0->unk_34 != NULL) {
        ov49_0225AEE0(param0);
    }

    if (param0->unk_30 != NULL) {
        ov49_0225B014(param0, NULL, NULL);
    }

    for (v0 = 0; v0 < 2; v0++) {
        Sprite_Delete(param0->unk_50[v0]);
        param0->unk_50[v0] = NULL;
    }

    SpriteTransfer_DeleteCharTransferTask(param0->unk_40[0]);
    SpriteTransfer_DeletePlttTransferTask(param0->unk_40[1]);

    for (v0 = 0; v0 < 4; v0++) {
        DestroySingle2DGfxResObj(param1->unk_194[v0], param0->unk_40[v0]);
    }
}
