#include "global.h"

#include "constants/heap.h"
#include "sprite.h"
#include "bg_window.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"

typedef enum HeapID HeapID;

struct Background;

struct BgConfig;

struct BgTemplate;

struct CharTransferTemplate;

struct G2dRenderer;

struct GraphicsModes;

struct Sprite;

struct SpriteList;

typedef struct CharTransferTemplate {
    int maxTasks;
    int sizeMain;
    int sizeSub;
    enum HeapID heapID;
} CharTransferTemplate;

void ObjCharTransfer_InitEx(const CharTransferTemplate *template, GXOBJVRamModeChar modeMain, GXOBJVRamModeChar modeSub);

void ObjCharTransfer_ClearBuffers(void);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void GfGfx_EngineBTogglePlanes(u8 layerMask, u8 enable);

void sub_0203A880(void);

void ObjPlttTransfer_Init(int capacity, enum HeapID heapID);

void ObjPlttTransfer_Reset(void);

void OamManager_Create(int mainOamStart,
    int mainOamCount,
    int mainAffineOamStart,
    int mainAffineOamCount,
    int subOamStart,
    int subOamCount,
    int subAffineOamStart,
    int subAffineOamCount,
    enum HeapID heapID);

SpriteList *G2dRenderer_Init(int maxElements, G2dRenderer *g2dRenderer, enum HeapID heapID);

void G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE vramType, GXOBJVRamModeChar vramMode);

void G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE vramType);

extern const CharTransferTemplate ov90_0225C294;

void ov114_0225CEF0(UnkStruct_ov114_0225CEF0 *param0, const GraphicsModes *param1, const UnkStruct_ov114_02260284 *param2, u32 param3, enum HeapID heapID);

void ov90_022590CC(UnkStruct_ov114_0225CFCC *param0, u32 param1, u32 param2, u32 param3, u32 param4);

void ov90_022590CC(UnkStruct_ov114_0225CFCC *param0, u32 param1, u32 param2, u32 param3, u32 param4) {

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 31, 0, 126, (HeapID)(0), 31, (HeapID)(param4));

    {
        CharTransferTemplate v1 = ov90_0225C294;

        v1.maxTasks = param2;
        v1.heapID = (HeapID)(param4);

        ObjCharTransfer_InitEx(&v1, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    }

    ObjPlttTransfer_Init(param3, (HeapID)(param4));
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);

    param0->unk_00 = (struct SpriteList *)((G2dRenderer_Init(param1, &param0->unk_04, (HeapID)(param4))));

    sub_0203A880();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 1);
}
