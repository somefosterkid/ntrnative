#include "global.h"
#include "overlay_01_021FB878.h"
#include "constants/heap.h"
#include "field/overlay_01_021FB878.h"
#include "filesystem.h"
#include "filesystem_files_def.h"

struct ArcTool;

struct CityState;

struct FieldChunk;

struct FieldChunkContext;

struct FieldChunkHandle;

struct FieldChunkLocator;

struct FieldChunkPropHolder;

struct FieldChunkVTable;

struct FieldG3DMapper;

struct FieldPropHandle;

struct FieldPropInstance;

struct FieldPropPosition;

struct FieldPropRTCState;

struct FieldPropResAnmHeader;

struct FieldPropResBank;

struct FieldPropResBundle;

struct FieldPropResInfo;

struct FieldPropResInstance;

struct FieldPropResInstanceHandle;

struct FieldPropResource;

struct FieldPropSystem;

struct FieldTerrainAnimator;

struct FieldWFBC;

struct G3DActor;

struct HeightExList;

struct MMSys;

struct MapTerrainBuf;

struct MapTerrainSamplerOutput;

typedef struct ArcTool ArcTool;

typedef struct CityState CityState;

typedef struct FieldChunk FieldChunk;

typedef struct FieldChunkContext FieldChunkContext;

typedef struct FieldChunkPropHolder FieldChunkPropHolder;

typedef struct FieldG3DMapper FieldG3DMapper;

typedef struct FieldPropHandle FieldPropHandle;

typedef struct FieldPropSystem FieldPropSystem;

typedef struct FieldWFBC FieldWFBC;

typedef struct MapTerrainBuf MapTerrainBuf;

typedef struct MapTerrainSamplerOutput MapTerrainSamplerOutput;

typedef struct MMSys MMSys;

typedef struct FieldPropInstance FieldPropInstance;

typedef struct FieldPropResAnmHeader FieldPropResAnmHeader;

typedef struct FieldPropResBundle FieldPropResBundle;

typedef struct FieldPropResInfo FieldPropResInfo;

typedef struct FieldPropResInstance FieldPropResInstance;

typedef struct FieldPropRTCState FieldPropRTCState;

typedef struct FieldPropPosition FieldPropPosition;

typedef struct FieldPropResBank FieldPropResBank;

typedef struct FieldPropResInstanceHandle FieldPropResInstanceHandle;

typedef struct FieldPropResource FieldPropResource;

typedef struct FieldChunkLocator FieldChunkLocator;

typedef struct FieldChunkVTable FieldChunkVTable;

typedef struct FieldChunkHandle FieldChunkHandle;

typedef struct HeightExList HeightExList;

typedef struct HeightExList ResortMapCreateWork;

typedef struct G3DActor G3DActor;

typedef struct {
    VecFx32 translation;
    VecFx32 scale;
    MtxFx33 rotation;
} SRTMatrix;

typedef struct FieldTerrainAnimator FieldTerrainAnimator;

struct MapTerrainBuf {
    VecFx16 normal;
    u16 pad06;
    u32 tileType;
    fx32 height;
};

struct MapTerrainSamplerOutput {
    MapTerrainBuf layers[16];
    u32 layerCount;
};

typedef struct {
    u16 state;
    u16 pad02;
    u32 nowLoadedLength;
    u32 bufferLoadedLength;
    u32 totalRawLength;
    BOOL unk10;
    BOOL unk14;
    BOOL unk18;
    BOOL unk1c;
    BOOL terrainLoadDone;
} FieldChunkLoader;

struct FieldChunkVTable {
    u32 magic;

    BOOL(*loaderUpdate)
    (FieldChunk *chunk, FieldChunkContext *context);

    void (*getTerrain)(MapTerrainSamplerOutput *out, const void *container, const VecFx32 *pos, fx32 size, fx32 baseY);
    void (*getTerrainBaseLayer)(MapTerrainSamplerOutput *out, const void *container, const VecFx32 *pos, fx32 size, fx32 baseY);
};

struct FieldChunkContext {
    void *srtAnimatorState;
    FieldPropSystem *propSystem;
    FieldWFBC *wfbc;
    u32 chunkIndex;
    u16 heapId;
};

struct FieldChunk {
    u16 active;
    u16 disableStreamer;
    VecFx32 worldPos;
    ArcTool *arc;
    u32 reqLoadDatID;
    u32 vtableIndex;
    u32 bufferSize;
    u32 textureVRAMSize;
    u32 streamerBufSize;
    const FieldChunkVTable *vtable;
    NNSG3dRenderObj *model;
    NNSG3dRenderObj modelObj;
    void *modelRsc;
    void *texRsc;
    u32 textureVRAM;
    u32 paletteVRAM;
    void *mapTextures;
    FieldPropResBank *propResBank;
    FieldPropInstance *propInstances;
    FieldPropInstance *propInstances2;
    u16 propCapacity1;
    u16 propCapacity2;
    u32 unkA8;
    u32 unkAC;
    FieldChunkLoader loader;
    void *container;
    void *context;
};

struct FieldChunkLocator {
    u32 chunkId;
    VecFx32 position;
};

struct FieldChunkHandle {
    FieldChunk *chunk;
    FieldChunkContext context;
    FieldChunkLocator locator;
};

struct FieldG3DMapper {
    u16 heapId;
    fx32 chunkSpan;
    u32 unk08;
    u32 locatorGenType;
    u32 chunkArcId;
    u16 matrixWidth;
    u16 matrixHeight;
    u32 chunkIdCount;
    u32 *chunkDatIds;
    BOOL isSharedTexResource;
    FieldChunkHandle *chunkHandles;
    FieldChunkLocator *chunkLocators;
    u8 chunkLoadDiameterX;
    u8 chunkLoadDiameterZ;
    u16 chunkCapacity;
    u32 chunkIdOfPlayer;
    VecFx32 playerPos;
    u8 phase1MaxChunkCount;
    u8 drawnChunkCount;
    u8 lastDrawnCount;
    u8 nowDrawnCount;

    u8 *drawnChunkIndices;
    s32 maxConcurrentDrawChunkByteSize;
    u8 chunkSizeCapPassingChunkEndIdx;
    u8 nowDrawScanline;
    u16 unk4E;
    u32 unk50;
    VecFx32 chunkBasePos;
    void *mapTextures;
    FieldPropSystem *propSystem;
    FieldTerrainAnimator *terrainAnimator;
    FieldWFBC *wfbc;
    ResortMapCreateWork *resortMap;
};

struct FieldPropResAnmHeader {
    u8 controllerType;
    u8 unk1;
    u8 ambientAnimationCount;
    u8 unk3;
    u32 animationIds[4];
};

struct FieldPropResBundle {
    u8 unk0[2];
    u8 countFlags;
    u8 unk3;
    u32 offsets[1];
};

struct FieldPropResInfo {
    u16 resId;
    u16 type;
    u16 doorResId;
    s16 doorX;
    s16 doorY;
    s16 doorZ;
    u16 unk0c;
    u16 unk0e;
    FieldPropResAnmHeader animationHeader;
};

struct FieldPropResource {
    FieldPropResInfo *info;
    void *model;
    void *animations[4];
};

struct FieldPropPosition {
    VecFx32 position;
    u16 rotationY;
    u16 pad0e;
};

struct FieldPropInstance {
    u32 resIndex;
    FieldPropPosition pos;
};

struct FieldChunkPropHolder {
    FieldChunk *chunk;
    u32 savedResIndex;
    u16 propIndex;
    u16 visible;
    FieldPropInstance *instance;
};

struct FieldPropRTCState {
    u32 dayPeriod;
    u32 previousDayPeriod;
    BOOL dayPartChanged;
    u8 playAnmIndex;
    u8 season;
    u8 padding[2];
};

struct FieldPropResInstanceHandle {
    G3DActor *actor;
    u32 unk4;
};

struct FieldPropResBank {
    FieldPropResInstanceHandle *handles;
    u32 count;
    void *unk08;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
};

struct FieldPropSystem {
    u16 heapId;
    FieldG3DMapper *mapper;
    FieldPropRTCState rtcState;
    FieldPropResBundle *resBundle;
    u8 resIdToIndex[0x190];
    u16 resInfoCount;
    FieldPropResBank resBank;
    u32 resourceCount;
    FieldPropResource *resources;
    void *textureResource;
    u32 resInstanceCount;
    FieldPropResInstance *resInstances;
    FieldPropHandle *handles[7];
    FieldChunkPropHolder chunkPropHolders[0x120];
};

struct FieldPropResInstance {
    G3DActor *actor;
    FieldPropResource *resource;
    u32 animationState[4];
};

struct FieldPropHandle {
    FieldPropSystem *system;
    u32 animation;
    FieldChunkPropHolder *holder;
    FieldPropResInstance instance;
    SRTMatrix transform;
};

typedef struct {
    u8 exists : 4;

    u8 hasName : 4;

    u8 id;

    u8 unk2;
    u8 unk3;
    u16 name[8];
    u32 unk14;
} CityResident;

struct CityState {
    u16 initialized;

    u16 city;
    u32 unk4;

    CityResident residents[10];
    CityResident visitors[10];
};

typedef struct {
    u16 unk0[2];
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u8 unkA[0x12];
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    u16 unk22;
    u16 unk24;
    u16 unk26;
    u16 unk28;
    u16 unk2A;
    u16 unk2C;
    u16 unk2E;

} CityResidentData;

typedef struct {
    ArcTool *arc;
    u32 id;
    CityResidentData data;
} CityResidentDataHandle;

typedef struct {
    u8 unk0_0 : 4;
    u8 unk0_4 : 4;
    u8 unk1;
} Ov021Entry;

typedef struct {
    Ov021Entry entry;
    u16 unk2;
} Ov021Cell;

typedef struct {
    MATHRandContext32 rand;
    u16 unk18;
    u16 unk1A;
    s32 unk1C;
    u32 unk20;
} Ov021Random;

typedef struct {
    Ov021Entry unk0[8][8];
    u32 unk80[][16];
} Ov021Layout;

typedef struct {

    u16 unk_00;
    u8 unk_02[2];
    CityResidentData data;
    CityResident resident;
    u16 unk_4c;
    u16 unk_4e;
} FieldWFBCPerson;

struct FieldWFBC {
    u8 city;
    u8 isOther;
    u8 unk_02[2];

    FieldWFBCPerson *people;
    CityState *state;

    CityResidentDataHandle *dataHandle;

    ArcTool *arc;
    Ov021Layout *layout;
    Ov021Cell unk_18[8][8];

    u8 *unk_118;
    Ov021Random unk_11c;
    MMSys *unk_140;
};

void ov01_021FC030(Field3DModelAnimation *anim, const Field3dModel *model, void *data, NNSFndAllocator *allocator);

void Field3dObject_SetPos(Field3dObject *object, const VecFx32 *position) {
    object->matrix = *position;
}

void Field3dObject_SetXRotation(Field3dObject *arg0, s16 arg1, s32 arg2) {
    arg0->rotation[arg2] = arg1;
}

void ov01_021FC030(Field3DModelAnimation *anim, const Field3dModel *model, void *data, NNSFndAllocator *allocator) {
    anim->fileDataRaw = data;
    anim->animRaw = NNS_G3dGetAnmByIdx(anim->fileDataRaw, 0);
    anim->animObj = NNS_G3dAllocAnmObj(allocator, anim->animRaw, model->model);

    NNS_G3dAnmObjInit(anim->animObj, anim->animRaw, model->model, model->texture);
}
