#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200B150.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"

struct G3DPipelineBuffers;
struct SpriteList;
struct SpriteResourceCollection;
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
BOOL SpriteList_Delete(SpriteList *collection);
typedef struct SpriteResourceCollection SpriteResourceCollection;
void Destroy2DGfxResObjMan(SpriteResourceCollection *spriteResources);
void ov48_02259724(UnkStruct_ov69_0225D35C *param0);

void ov48_02259724(UnkStruct_ov69_0225D35C *param0) {
    int v0;

    SpriteList_Delete(param0->unk_04);

    for (v0 = 0; v0 < 4; v0++) {
        Destroy2DGfxResObjMan(param0->unk_194[v0]);
    }

    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
}
