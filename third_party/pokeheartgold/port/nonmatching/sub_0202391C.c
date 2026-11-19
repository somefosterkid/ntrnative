#include "global.h"
struct Billboard;
struct BillboardAnim;
struct BillboardGfxSequence;
struct BillboardList;
struct BillboardListVRAMTransfer;
struct BillboardVRAMTransfer;
typedef struct BillboardGfxSequence {
    const u16 *startFrame;
    const u8 *textureIdx;
    const u8 *plttIdx;
    u32 seqCount;
} BillboardGfxSequence;
typedef struct BillboardVRAMTransfer BillboardVRAMTransfer;
typedef struct BillboardListVRAMTransfer BillboardListVRAMTransfer;
typedef struct Billboard Billboard;
typedef struct BillboardList BillboardList;
typedef void (*BillboardCallback)(Billboard *, void *);
typedef struct BillboardAnim BillboardAnim;
struct Billboard {
    VecFx32 pos;
    VecFx32 scale;
    const MtxFx33 *rotMatrix;
    void *callbackParam;
    BillboardCallback callback;
    u8 draw;
    BillboardList *list;
    const BillboardAnim *anims;
    NNSG3dRenderObj renderObj;
    NNSG3dResMdlSet *modelSet;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
    const NNSG3dResTex *animTexture;
    NNSGfdTexKey texKey;
    NNSGfdTexKey tex4x4Key;
    NNSGfdPlttKey plttKey;
    BillboardGfxSequence gfxSequence;
    BillboardVRAMTransfer *vramTransfer;
    u8 state;
    u16 animNum;
    fx32 frameNum;
    Billboard *next;
    Billboard *prev;
};
struct BillboardList {
    u8 active;
    u8 draw;
    u8 unused;
    u8 redraw;
    Billboard *billboards;
    int capacity;
    Billboard sentinelData;
    Billboard **freeBillboards;
    int freeBillboardHead;
    NNSFndAllocator *allocator;
    BillboardListVRAMTransfer *vramTransfer;
};

/* invented */ typedef struct { BillboardList *lists; int count; } BillboardState;
extern BillboardState _021D2208;

BillboardList *sub_0202391C(void);
BillboardList *sub_0202391C(void)
{
    int i;
    for (i = 0; i < _021D2208.count; i++) {
        if (_021D2208.lists[i].active == FALSE) {
            return &_021D2208.lists[i];
        }
    }
    return NULL;
}
