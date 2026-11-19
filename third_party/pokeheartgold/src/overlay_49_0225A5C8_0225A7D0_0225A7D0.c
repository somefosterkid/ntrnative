#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"

struct G3DPipelineBuffers;

struct SpriteList;

struct SpriteResourceCollection;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void GF_3DVramMan_Delete(G3DPipelineBuffers *state);

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

BOOL SpriteList_Delete(SpriteList *collection);

typedef struct SpriteResourceCollection SpriteResourceCollection;

void Destroy2DGfxResObjMan(SpriteResourceCollection *spriteResources);

extern const u32 ov49_02269734[4];

void ov49_0225A7D0(UnkStruct_ov70_0225E4EC *param0);

void ov49_0225A7D0(UnkStruct_ov70_0225E4EC *param0) {
    GF_DestroyVramTransferManager();

    {
        int v0;

        for (v0 = 0; v0 < 4; v0++) {
            FreeBgTilemapBuffer(param0->unk_00, ov49_02269734[v0]);
        }

        Heap_Free(param0->unk_00);
    }

    {
        int v1;

        sub_0202067C(param0->unk_1A4);
        param0->unk_1A4 = NULL;

        for (v1 = 0; v1 < 4; v1++) {
            Destroy2DGfxResObjMan(param0->unk_194[v1]);
        }

        SpriteList_Delete(param0->unk_04);
        ObjCharTransfer_Destroy();
        ObjPlttTransfer_Destroy();
        OamManager_Free();
    }

    {
        GF_3DVramMan_Delete(param0->unk_1A8);
    }
}
