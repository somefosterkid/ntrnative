#ifndef POKEHEARTGOLD_BILLBOARD_H
#define POKEHEARTGOLD_BILLBOARD_H

#include "global.h"

#include "constants/heap.h"

#include "billboard_gfx_sequence.h"

typedef struct Billboard Billboard;
typedef struct BillboardAnim BillboardAnim;
typedef struct BillboardList BillboardList;
typedef struct BillboardListVRAMTransfer BillboardListVRAMTransfer;
typedef struct BillboardVRAMTransfer BillboardVRAMTransfer;

typedef void (*BillboardCallback)(Billboard *, void *);

enum BillboardAnimType {
    BILLBOARD_ANIM_TYPE_LOOP = 0,
    BILLBOARD_ANIM_TYPE_ONESHOT,
    BILLBOARD_ANIM_TYPE_TABLE_END,
};

enum BillboardAnimStatus {
    BILLBOARD_ANIM_STATUS_RUNNING = 0,
    BILLBOARD_ANIM_STATUS_FINISHED,
};

enum BillboardRedraw {
    BILLBOARD_REDRAW_NONE = 0,
    BILLBOARD_REDRAW_NEEDED,
    BILLBOARD_REDRAW_FINISHED,
};

enum BillboardState {
    BILLBOARD_STATE_INACTIVE = 0,
    BILLBOARD_STATE_INITIALIZED,
    BILLBOARD_STATE_VRAM_TRANSFER,
    BILLBOARD_STATE_ACTIVE,
};

enum BillboardResourceValue {
    BILLBOARD_RESOURCE_MODEL_RES = 0,
    BILLBOARD_RESOURCE_TEXTURE,
};

struct BillboardAnim {
    int startFrame;
    int endFrame;
    int animType;
};

typedef struct BillboardResources {
    void *modelRes;
    const NNSG3dResTex *texture;
    const BillboardAnim *anims;
    BillboardGfxSequence gfxSequence;
    NNSGfdTexKey texKey;
    NNSGfdTexKey tex4x4Key;
    NNSGfdPlttKey plttKey;
} BillboardResources;

struct BillboardVRAMTransfer {
    const BillboardGfxSequence *gfxSequence;
    const NNSG3dResTex *texture;
    NNSGfdTexKey texKey;
    NNSGfdPlttKey plttKey;
    u8 textureIdx;
    u8 plttIdx;
};

struct BillboardListVRAMTransfer {
    BillboardVRAMTransfer *billboardTransfers;
    int maxElements;
};

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

typedef struct BillboardTemplate {
    BillboardList *list;
    const BillboardResources *resources;
    VecFx32 pos;
    VecFx32 scale;
} BillboardTemplate;

typedef struct BillboardListParams {
    int maxElements;
    enum HeapID heapID;
} BillboardListParams;

// The global billboard list pool (_021D2208): `count` BillboardLists at `lists`, allocated by BillboardLists_Create.
typedef struct BillboardLists {
    BillboardList *lists;
    int count;
} BillboardLists;

extern BillboardLists _021D2208;

#endif // POKEHEARTGOLD_BILLBOARD_H
